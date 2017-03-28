#define TRACE_MODULE _mme_s11_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "event.h"
#include "context.h"
#include "s11_path.h"

static int _gtpv2_c_recv_cb(net_sock_t *net_sock, void *data)
{
    char buf[INET_ADDRSTRLEN];
    status_t rv;
    event_t e;
    pkbuf_t *pkbuf = NULL;
    gtp_node_t gnode;
    sgw_ctx_t *sgw = NULL;

    d_assert(net_sock, return -1, "Null param");

    gnode.addr = net_sock->remote.sin_addr.s_addr;
    gnode.port = net_sock->remote.sin_port;

    sgw = mme_ctx_sgw_find_by_node(&gnode);
    d_assert(sgw, return -1, "Can't find SGW from [%s:%d]",
            INET_NTOP(&gnode.addr, buf), gnode.port);

    pkbuf = gtp_read(net_sock);
    if (pkbuf == NULL)
    {
        if (net_sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(1, "S11_PDU is received from SGW[%s:%d]\n",
            INET_NTOP(&gnode.addr, buf), gnode.port);
    d_trace_hex(1, pkbuf->payload, pkbuf->len);

    event_set(&e, EVT_MSG_MME_S11);
    event_set_param1(&e, (c_uintptr_t)sgw);
    event_set_param2(&e, (c_uintptr_t)pkbuf);
    rv = mme_event_send(&e);
    if (rv != CORE_OK)
    {
        d_error("mme_event_send error");
        pkbuf_free(pkbuf);
        return -1;
    }
    return 0;
}

status_t mme_s11_listen()
{
    status_t rv;

    rv = gtp_listen(&mme_self()->s11_sock, _gtpv2_c_recv_cb, 
            mme_self()->s11_addr, mme_self()->s11_port, NULL);
    if (rv != CORE_OK)
    {
        d_error("Can't establish S11 Path for SGW");
        return rv;
    }

    return CORE_OK;
}

status_t mme_s11_close()
{
    status_t rv;

    rv = gtp_close(mme_self()->s11_sock);
    if (rv != CORE_OK)
    {
        d_error("Can't close S11 Path for SGW");
        return rv;
    }

    return CORE_OK;
}

status_t mme_s11_send_to_sgw(void *sgw, pkbuf_t *pkbuf)
{
    d_assert(sgw, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    return gtp_send(mme_self()->s11_sock, sgw, pkbuf);
}
