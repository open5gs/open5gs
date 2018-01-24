#define TRACE_MODULE _gtp_path

#include "core_debug.h"
#include "core_pkbuf.h"

#include "3gpp_types.h"
#include "gtp_message.h"
#include "gtp_conv.h"
#include "gtp_node.h"

#include "gtp_path.h"

status_t gtp_server(sock_node_t *snode, sock_handler handler)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];

    d_assert(snode, return CORE_ERROR,);

    rv = udp_server(&snode->sock, snode->list);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    rv = sock_register(snode->sock, handler, NULL);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    d_trace(1, "gtp_server() [%s]:%d\n",
            CORE_ADDR(snode->list, buf), CORE_PORT(snode->list));

    return CORE_OK;
}

status_t gtp_client(gtp_node_t *gnode)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];

    d_assert(gnode, return CORE_ERROR,);

    rv = udp_client(&gnode->sock, gnode->sa_list);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    d_trace(1, "gtp_client() [%s]:%d\n",
            CORE_ADDR(gnode->sa_list, buf), CORE_PORT(gnode->sa_list));

    return CORE_OK;
}

status_t gtp_connect(sock_id ipv4, sock_id ipv6, gtp_node_t *gnode)
{
    c_sockaddr_t *addr;
    char buf[CORE_ADDRSTRLEN];

    d_assert(ipv4 || ipv6, return CORE_ERROR,);
    d_assert(gnode, return CORE_ERROR,);
    d_assert(gnode->sa_list, return CORE_ERROR,);

    addr = gnode->sa_list;
    while(addr)
    {
        sock_id id;

        if (addr->c_sa_family == AF_INET) id = ipv4;
        else if (addr->c_sa_family == AF_INET6) id = ipv6;
        else
            d_assert(0, return CORE_ERROR,);

        if (id)
        {
            if (sock_connect(id, addr) == CORE_OK)
            {
                d_trace(1, "gtp_connect() [%s]:%d\n",
                        CORE_ADDR(addr, buf), CORE_PORT(addr));

                gnode->sock = id;
                break;
            }
        }

        addr = addr->next;
    }

    if (addr == NULL)
    {
        d_warn("gtp_connect() [%s]:%d failed(%d:%s)",
                CORE_ADDR(gnode->sa_list, buf), CORE_PORT(gnode->sa_list),
                errno, strerror(errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t gtp_server_list(list_t *list, sock_handler handler)
{
    status_t rv;
    sock_node_t *snode = NULL;

    d_assert(list, return CORE_ERROR,);
    d_assert(handler, return CORE_ERROR,);

    for (snode = list_first(list); snode; snode = list_next(snode))
    {
        rv = gtp_server(snode, handler);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
    }

    return CORE_OK;
}

sock_id gtp_local_sock_first(list_t *list)
{
    sock_node_t *snode = NULL;
    sock_id sock = 0;

    d_assert(list, return 0,);

    for (snode = list_first(list); snode; snode = list_next(snode))
    {
        sock = snode->sock;
        if (sock) return sock;
    }

    return 0;
}

c_sockaddr_t *gtp_local_addr_first(list_t *list)
{
    sock_node_t *snode = NULL;
    c_sockaddr_t *addr = NULL;

    d_assert(list, return NULL,);

    for (snode = list_first(list); snode; snode = list_next(snode))
    {
        addr = sock_local_addr(snode->sock);
        if (addr) return addr;
    }

    return NULL;
}

status_t gtp_recv(sock_id sock, pkbuf_t **pkbuf)
{
    ssize_t size;

    d_assert(sock, return CORE_ERROR,);

    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if ((*pkbuf) == NULL)
    {
        char tmp_buf[MAX_SDU_LEN];

        d_fatal("Can't allocate pkbuf");

        /* Read data from socket to exit from select */
        core_recv(sock, tmp_buf, MAX_SDU_LEN, 0);

        return CORE_ERROR;
    }

    size = core_recv(sock, (*pkbuf)->payload, (*pkbuf)->len, 0);
    if (size <= 0)
    {
        pkbuf_free((*pkbuf));

        if (errno != EAGAIN)
        {
            d_warn("net_read failed(%d:%s)", errno, strerror(errno));
        }

        return CORE_ERROR;
    }
    else
    {
        (*pkbuf)->len = size;

        return CORE_OK;;
    }
}

status_t gtp_recvfrom(sock_id sock, pkbuf_t **pkbuf, c_sockaddr_t *from)
{
    ssize_t size;

    d_assert(sock, return CORE_ERROR,);
    d_assert(from, return CORE_ERROR,);

    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    if ((*pkbuf) == NULL)
    {
        char tmp_buf[MAX_SDU_LEN];

        d_fatal("Can't allocate pkbuf");

        /* Read data from socket to exit from select */
        core_recv(sock, tmp_buf, MAX_SDU_LEN, 0);

        return CORE_ERROR;
    }

    size = core_recvfrom(sock, (*pkbuf)->payload, (*pkbuf)->len, 0, from);
    if (size <= 0)
    {
        pkbuf_free((*pkbuf));

        if (errno != EAGAIN)
        {
            d_warn("core_recv failed(%d:%s)", errno, strerror(errno));
        }

        return CORE_ERROR;
    }
    else
    {
        (*pkbuf)->len = size;

        return CORE_OK;;
    }
}

status_t gtp_send(gtp_node_t *gnode, pkbuf_t *pkbuf)
{
    char buf[CORE_ADDRSTRLEN];
    ssize_t sent;
    sock_id sock = 0;
    c_sockaddr_t *addr = NULL;

    d_assert(gnode, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");
    sock = gnode->sock;
    d_assert(sock, return CORE_ERROR, "Null param");

    /* New interface */
    sock = gnode->sock;
    d_assert(sock, return CORE_ERROR,);
    addr = sock_remote_addr(sock);
    d_assert(addr, return CORE_ERROR,);

    sent = core_send(sock, pkbuf->payload, pkbuf->len, 0);
    if (sent < 0 || sent != pkbuf->len)
    {
        d_error("core_send [%s]:%d failed(%d:%s)",
            CORE_ADDR(addr, buf), CORE_PORT(addr), errno, strerror(errno));
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
