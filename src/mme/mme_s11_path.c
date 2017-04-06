#define TRACE_MODULE _mme_s11_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "mme_event.h"
#include "mme_ctx.h"
#include "mme_s11_path.h"

static int _gtpv2_c_recv_cb(net_sock_t *sock, void *data)
{
    char buf[INET_ADDRSTRLEN];
    status_t rv;
    event_t e;
    pkbuf_t *pkbuf = NULL;
    gtp_node_t gnode;
    sgw_ctx_t *sgw = NULL;

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

    sgw = mme_ctx_sgw_find_by_node(&gnode);
    d_assert(sgw, return -1, "Can't find SGW from [%s:%d]",
            INET_NTOP(&gnode.addr, buf), gnode.port);

    d_trace(1, "S11_PDU is received from SGW[%s:%d]\n",
            INET_NTOP(&gnode.addr, buf), gnode.port);
    d_trace_hex(1, pkbuf->payload, pkbuf->len);

    event_set(&e, EVT_MSG_MME_S11);
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

status_t mme_s11_send_to_sgw(void *sgw, 
        c_uint8_t type, c_uint32_t teid, pkbuf_t *pkbuf)
{
    gtp_xact_t *xact = NULL;
    d_assert(sgw, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    xact = gtp_xact_local_create(&mme_self()->gtp_xact_ctx, 
            mme_self()->s11_sock, sgw);
    d_assert(xact, return CORE_ERROR, "Null param");

    d_assert(gtp_xact_commit(xact, type, teid, pkbuf) == CORE_OK,
            return CORE_ERROR, "xact commit error");

    return CORE_OK;
}

#include "mme_s11_build.h"
#include "3gpp_types.h"
#include "3gpp_conv.h"
#include "gtp_types.h"
#include "gtp_tlv.h"
#include "core_lib.h"
void test_send()
{
    pkbuf_t *pkbuf;
    c_uint8_t type;
    c_uint32_t teid = 0;

    mme_s11_build_create_session_req(&type, &pkbuf, NULL);

    mme_s11_send_to_sgw(mme_ctx_sgw_first(), type, teid, pkbuf);
}
