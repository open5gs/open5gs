#define TRACE_MODULE _gtp_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "3gpp_defs.h"
#include "gtp_path.h"

status_t gtp_open(gtp_node_t *gnode, net_sock_handler handler)
{
    char buf[INET_ADDRSTRLEN];
    int rc;
    
    d_assert(gnode, return CORE_ERROR, "Null param");

    rc = net_listen_with_addr(&gnode->s, SOCK_DGRAM, IPPROTO_UDP, 
            gnode->local_port, gnode->local_addr);
    if (rc != 0)
    {
        d_error("Can't establish GTP[%s:%d] path(%d:%s)",
            INET_NTOP(&gnode->local_addr, buf), gnode->local_port, 
            errno, strerror(errno));
        return CORE_ERROR;
    }

    rc = net_register_sock(gnode->s, handler, gnode);
    if (rc != 0)
    {
        d_error("Can't establish GTP path(%d:%s)",
            errno, strerror(errno));
        net_close(gnode->s);
        return CORE_ERROR;
    }

    d_trace(1, "gtp_listen() %s:%d\n", 
            INET_NTOP(&gnode->local_addr, buf), gnode->local_port);

    return CORE_OK;
}

status_t gtp_close(gtp_node_t *gnode)
{
    d_assert(gnode, return CORE_ERROR, "Null param");
    d_assert(gnode->s, return CORE_ERROR, "Null param");

    net_unregister_sock(gnode->s);
    net_close(gnode->s);

    return CORE_OK;
}

pkbuf_t *gtp_read(gtp_node_t *gnode)
{
    pkbuf_t *pkb;
    int r;

    d_assert(gnode, return NULL, "Null param");
    d_assert(gnode->s, return NULL, "Null param");

    pkb = pkbuf_alloc(0, MAX_SDU_LEN);
    d_assert(pkb, return NULL, "Can't allocate pkbuf");

    r = net_read(gnode->s, pkb->payload, pkb->len, 0);
    if (r <= 0)
    {
        pkbuf_free(pkb);

        if (gnode->s->sndrcv_errno != EAGAIN)
        {
            d_warn("net_read failed(%d:%s)", 
                    gnode->s->sndrcv_errno, strerror(gnode->s->sndrcv_errno));
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

    d_assert(gnode, return CORE_ERROR, "Null param");
    d_assert(gnode->s, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    sent = net_sendto(gnode->s, pkbuf->payload, pkbuf->len, 
            gnode->remote_addr, gnode->remote_port);
    d_trace(1,"Sent %d->%d bytes to [%s:%d]\n", pkbuf->len, sent, 
            INET_NTOP(&gnode->remote_addr, buf), gnode->remote_port);
    d_trace_hex(1, pkbuf->payload, pkbuf->len);
    if (sent < 0 || sent != pkbuf->len)
    {
        d_error("net_send error (%d:%s)", 
                gnode->s->sndrcv_errno, strerror(gnode->s->sndrcv_errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}
