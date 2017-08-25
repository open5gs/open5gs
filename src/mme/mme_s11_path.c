#define TRACE_MODULE _mme_s11_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "mme_event.h"
#include "mme_context.h"
#include "mme_s11_path.h"

static int _gtpv2_c_recv_cb(net_sock_t *sock, void *data)
{
    char buf[INET_ADDRSTRLEN];
    status_t rv;
    event_t e;
    pkbuf_t *pkbuf = NULL;
    gtp_node_t gnode;
    mme_sgw_t *sgw = NULL;

    d_assert(sock, return -1, "Null param");

    pkbuf = gtp_read(sock);
    if (pkbuf == NULL)
    {
        if (sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    gnode.addr = sock->remote.sin_addr.s_addr;
    gnode.port = ntohs(sock->remote.sin_port);

    sgw = mme_sgw_find_by_node(&gnode);
    d_assert(sgw, return -1, "Can't find SGW from [%s:%d]",
            INET_NTOP(&gnode.addr, buf), gnode.port);

    d_trace(10, "S11_PDU is received from SGW[%s:%d]\n",
            INET_NTOP(&gnode.addr, buf), gnode.port);
    d_trace_hex(10, pkbuf->payload, pkbuf->len);

    event_set(&e, MME_EVT_S11_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)sock);
    event_set_param2(&e, (c_uintptr_t)sgw);
    event_set_param3(&e, (c_uintptr_t)pkbuf);
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

status_t mme_s11_send_to_sgw(mme_sess_t *sess, c_uint8_t type, pkbuf_t *pkbuf)
{
    gtp_xact_t *xact = NULL;
    void *sgw = NULL;
    c_uint32_t teid;
    d_assert(sess, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    sgw = sess->sgw;
    d_assert(sgw, return CORE_ERROR, "Null param");
    teid = sess->sgw_s11_teid;

    xact = gtp_xact_local_create(&mme_self()->gtp_xact_ctx, 
            mme_self()->s11_sock, sgw);
    d_assert(xact, return CORE_ERROR, "Null param");

    d_assert(gtp_xact_commit(xact, type, teid, pkbuf) == CORE_OK,
            return CORE_ERROR, "xact commit error");

    return CORE_OK;
}
