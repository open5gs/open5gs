#define TRACE_MODULE _gtp_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "types.h"
#include "gtp_message.h"
#include "gtp_path.h"

status_t gtp_listen(net_sock_t **sock, 
    net_sock_handler handler, c_uint32_t addr, c_uint16_t port, void *data)
{
    char buf[INET_ADDRSTRLEN];
    int rc;
    
    rc = net_listen_ext(sock, SOCK_DGRAM, IPPROTO_UDP, 0, addr, port);
    if (rc != 0)
    {
        d_error("Can't establish GTP[%s:%d] path(%d:%s)",
            INET_NTOP(&addr, buf), port, errno, strerror(errno));
        return CORE_ERROR;
    }

    rc = net_register_sock(*sock, handler, data);
    if (rc != 0)
    {
        d_error("Can't establish GTP path(%d:%s)",
            errno, strerror(errno));
        net_close(*sock);
        return CORE_ERROR;
    }

    d_trace(1, "gtp_listen() %s:%d\n", INET_NTOP(&addr, buf), port);

    return CORE_OK;
}

status_t gtp_close(net_sock_t *sock)
{
    d_assert(sock, return CORE_ERROR, "Null param");

    net_unregister_sock(sock);
    net_close(sock);

    return CORE_OK;
}

pkbuf_t *gtp_read(net_sock_t *sock)
{
    pkbuf_t *pkb;
    int r;

    d_assert(sock, return NULL, "Null param");

    pkb = pkbuf_alloc(0, MAX_SDU_LEN);
    if (pkb == NULL)
    {
        char tmp_buf[MAX_SDU_LEN];

        d_fatal("Can't allocate pkbuf");

        /* Read data from socket to exit from select */
        net_read(sock, tmp_buf, MAX_SDU_LEN, 0);

        return NULL;
    }

    r = net_read(sock, pkb->payload, pkb->len, 0);
    if (r <= 0)
    {
        pkbuf_free(pkb);

        if (sock->sndrcv_errno != EAGAIN)
        {
            d_warn("net_read failed(%d:%s)", 
                    sock->sndrcv_errno, strerror(sock->sndrcv_errno));
        }

        return NULL;
    }
    else
    {
        pkb->len = r;

        return pkb;
    }
}

status_t gtp_send(gtp_node_t *gnode, pkbuf_t *pkbuf)
{
    char buf[INET_ADDRSTRLEN];
    ssize_t sent;
    net_sock_t *sock = NULL;

    d_assert(gnode, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");
    sock = gnode->sock;
    d_assert(sock, return CORE_ERROR, "Null param");

    sent = net_sendto(sock, pkbuf->payload, pkbuf->len, 
            gnode->addr, gnode->port);
    d_trace(50, "Sent %d->%d bytes to [%s:%d]\n", pkbuf->len, sent, 
            INET_NTOP(&gnode->addr, buf), gnode->port);
    d_trace_hex(50, pkbuf->payload, pkbuf->len);
    if (sent < 0 || sent != pkbuf->len)
    {
        d_error("net_send error (%d:%s)", 
                sock->sndrcv_errno, strerror(sock->sndrcv_errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}

pkbuf_t *gtp_handle_echo_req(pkbuf_t *pkb)
{
    gtp_header_t *gtph = NULL;
    pkbuf_t *pkb_resp;NULL;
    gtp_header_t *gtph_resp;NULL;
    c_uint16_t length;
    int idx;

    d_assert(pkb, return NULL, "pkt is NULL");

    gtph = (gtp_header_t *)pkb->payload;
    /* Check GTP version. Now only support GTPv1(version = 1) */
    if ((gtph->flags >> 5) != 1)
    {
        return NULL;
    }

    if (gtph->type != GTPU_MSGTYPE_ECHO_REQ)
    {
        return NULL;
    }


    d_trace(3, "gtp_handle_without_teid(ECHO_REQ)\n");

    pkb_resp = pkbuf_alloc(0, 100 /* enough for ECHO_RSP; use smaller buffer */);
    d_assert(pkb_resp, return NULL, "Can't allocate pkbuf");
    gtph_resp = (gtp_header_t *)pkb_resp->payload;

    /* reply back immediately */
    gtph_resp->flags = (1 << 5); /* set version */
    gtph_resp->flags |= (1 << 4); /* set PT */
    gtph_resp->type = GTPU_MSGTYPE_ECHO_RSP;
    length = 0;     /* length of Recovery IE */
    gtph_resp->length = htons(length); /* to be overwriten */
    gtph_resp->teid = 0;
    idx = 8;

    if (gtph->flags & (GTPU_FLAGS_PN | GTPU_FLAGS_S))
    {
        length += 4;
        if (gtph->flags & GTPU_FLAGS_S)
        {
            /* sequence exists */
            gtph_resp->flags |= GTPU_FLAGS_S;
            *((c_uint8_t *)pkb_resp->payload + idx) = *((c_uint8_t *)pkb->payload + idx);
            *((c_uint8_t *)pkb_resp->payload + idx + 1) = *((c_uint8_t *)pkb->payload + idx + 1);
        }
        else
        {
            *((c_uint8_t *)pkb_resp->payload + idx) = 0;
            *((c_uint8_t *)pkb_resp->payload + idx + 1) = 0;
        }
        idx += 2;
        if (gtph->flags & GTPU_FLAGS_PN)
        {
            /* sequence exists */
            gtph_resp->flags |= GTPU_FLAGS_PN;
            *((c_uint8_t *)pkb_resp->payload + idx) = *((c_uint8_t *)pkb->payload + idx);
        }
        else
        {
            *((c_uint8_t *)pkb_resp->payload + idx) = 0;
        }
        idx++;
        *((c_uint8_t *)pkb_resp->payload + idx) = 0; /* next-extension header */
        idx++;
    }
    
    /* fill Recovery IE */
    length += 2;
    *((c_uint8_t *)pkb_resp->payload + idx) = 14; idx++; /* type */
    *((c_uint8_t *)pkb_resp->payload + idx) = 0; idx++; /* restart counter */

    gtph_resp->length = htons(length);
    pkb_resp->len = idx;                /* buffer length */

    return pkb_resp;
}
