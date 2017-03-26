#define TRACE_MODULE _mme_s11_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "event.h"
#include "s11_path.h"

static int _mme_s11_recv_cb(net_sock_t *net_sock, void *data)
{
    char buf[INET_ADDRSTRLEN];
    event_t e;
    int rc;
    pkbuf_t *pkbuf = NULL;
    gtp_node_t *gnode = data;

    d_assert(net_sock, return -1, "Null param");
    d_assert(gnode, return -1, "Null param");

    pkbuf = gtp_read(gnode);
    if (pkbuf == NULL)
    {
        if (net_sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(1, "S11_PDU is received from SGW[%s:%d]\n",
            INET_NTOP(&gnode->remote_addr, buf), gnode->remote_port);
    d_trace_hex(1, pkbuf->payload, pkbuf->len);

    event_set(&e, EVT_MSG_MME_S11);
    event_set_param1(&e, (c_uintptr_t)gnode);
    event_set_param2(&e, (c_uintptr_t)pkbuf);

    rc = event_send(mme_self()->queue_id, &e);
    if (rc <= 0)
    {
        pkbuf_free(pkbuf);
        return rc;
    }

    return 0;
}

status_t mme_s11_open(void *sgw)
{
    char buf[INET_ADDRSTRLEN];
    status_t rv;
    gtp_node_t *gnode = sgw;

    rv = gtp_open(gnode, _mme_s11_recv_cb);
    if (rv != CORE_OK)
    {
        d_error("Can't establish S11 Path with SGW[%s:%d]",
            INET_NTOP(&gnode->remote_addr, buf), gnode->remote_port);
        return rv;
    }

    return CORE_OK;
}

status_t mme_s11_close(void *sgw)
{
    char buf[INET_ADDRSTRLEN];
    status_t rv;
    gtp_node_t *gnode = sgw;

    rv = gtp_close(gnode);
    if (rv != CORE_OK)
    {
        d_error("Can't close S11 Path with SGW[%s:%d]",
            INET_NTOP(&gnode->remote_addr, buf), gnode->remote_port);
        return rv;
    }

    return CORE_OK;
}

status_t mme_s11_send_to_sgw(void *sgw, pkbuf_t *pkbuf)
{
    return  gtp_send(sgw, pkbuf);
}
