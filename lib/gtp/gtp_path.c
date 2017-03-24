#define TRACE_MODULE _gtp_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "3gpp_defs.h"
#include "gtp_path.h"

status_t gtp_open(net_sock_t **sock, net_sock_handler handler, void *data,
        const c_uint32_t addr, const c_uint16_t port)
{
    char buf[INET_ADDRSTRLEN];
    int rc;
    
    rc = net_listen_with_addr(sock, SOCK_DGRAM, IPPROTO_UDP, port, addr);
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

pkbuf_t *gtp_read(net_sock_t *net_sock)
{
    pkbuf_t *pkb;
    int r;

    d_assert(net_sock, return NULL, "Null param");

    pkb = pkbuf_alloc(0, MESSAGE_SDU_SIZE);
    d_assert(pkb, return NULL, "Can't allocate pkbuf");

    r = net_read(net_sock, pkb->payload, pkb->len, 0);
    if (r <= 0)
    {
        pkbuf_free(pkb);

        if (net_sock->sndrcv_errno != EAGAIN)
        {
            d_warn("net_read failed(%d:%s)", 
                    net_sock->sndrcv_errno, strerror(net_sock->sndrcv_errno));
        }

        return NULL;
    }
    else
    {
        pkb->len = r;

        return pkb;
    }
}

status_t gtp_send(net_sock_t *s, 
        pkbuf_t *pkbuf, c_uint32_t ip_addr, c_uint16_t port)
{
    char buf[INET_ADDRSTRLEN];
    ssize_t sent;

    d_assert(s, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    sent = net_sendto(s, pkbuf->payload, pkbuf->len, ip_addr, port);
    d_trace(1,"Sent %d->%d bytes to [%s:%d]\n", 
            pkbuf->len, sent, INET_NTOP(&ip_addr, buf), port);
    d_trace_hex(1, pkbuf->payload, pkbuf->len);
    if (sent < 0 || sent != pkbuf->len)
    {
        d_error("net_send error (%d:%s)", 
                s->sndrcv_errno, strerror(s->sndrcv_errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}
