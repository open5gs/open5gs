#define TRACE_MODULE _mme_s11_path
#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_net.h"

#include "mme_event.h"
#include "mme_gtp_path.h"
#include "mme_s11_build.h"

static int _gtpv2_c_recv_cb(net_sock_t *sock, void *data)
{
    char buf[INET_ADDRSTRLEN];
    status_t rv;
    event_t e;
    pkbuf_t *pkbuf = NULL;
    c_uint32_t addr;
    c_uint16_t port;
    mme_sgw_t *sgw = NULL;

    d_assert(sock, return -1, "Null param");

    pkbuf = gtp_read(sock);
    if (pkbuf == NULL)
    {
        if (sock->sndrcv_errno == EAGAIN)
            return 0;

        return -1;
    }

    addr = sock->remote.sin_addr.s_addr;
    port = ntohs(sock->remote.sin_port);

    sgw = mme_sgw_find(addr, port);
    d_assert(sgw, return -1, "Can't find SGW from [%s:%d]",
            INET_NTOP(&addr, buf), port);

    sgw->sock = sock; /* Is it needed? */

    d_trace(10, "S11_PDU is received from SGW[%s:%d]\n",
            INET_NTOP(&addr, buf), port);
    d_trace_hex(10, pkbuf->payload, pkbuf->len);

    event_set(&e, MME_EVT_S11_MESSAGE);
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

status_t mme_gtp_open()
{
    status_t rv;
    mme_sgw_t *sgw = mme_sgw_first();

    rv = gtp_listen(&mme_self()->s11_sock, _gtpv2_c_recv_cb, 
            mme_self()->s11_addr, mme_self()->s11_port, NULL);
    if (rv != CORE_OK)
    {
        d_error("Can't establish S11 Path for SGW");
        return rv;
    }

    /* socket descriptor needs in gnode when packet is sending initilly */
    while(sgw)
    {
        sgw->sock = mme_self()->s11_sock;
        sgw = mme_sgw_next(sgw);
    }

    return CORE_OK;
}

status_t mme_gtp_close()
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

status_t mme_gtp_send_modify_bearer_request(mme_bearer_t *bearer)
{
    status_t rv;

    gtp_xact_t *xact = NULL;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;

    gtp_header_t h;
    pkbuf_t *pkbuf = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    sess = bearer->sess;
    d_assert(sess, return CORE_ERROR, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_MODIFY_BEARER_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_modify_bearer_request(&pkbuf, h.type, bearer);
    d_assert(rv == CORE_OK, return CORE_ERROR, "S11 build error");

    xact = gtp_xact_local_create(sess->sgw, &h, pkbuf);
    d_assert(xact, return CORE_ERROR, "Null param");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return CORE_ERROR, "xact_commit error");

    return CORE_OK;
}

status_t mme_gtp_send_create_bearer_response(mme_bearer_t *bearer)
{
    status_t rv;

    gtp_xact_t *xact = NULL;
    mme_ue_t *mme_ue = NULL;

    gtp_header_t h;
    pkbuf_t *pkbuf = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    xact = bearer->xact;
    d_assert(xact, return CORE_ERROR, "Null param");

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_CREATE_BEARER_RESPONSE_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_create_bearer_response(&pkbuf, h.type, bearer);
    d_assert(rv == CORE_OK, return CORE_ERROR, "S11 build error");

    rv = gtp_xact_update_tx(xact, &h, pkbuf);
    d_assert(xact, return CORE_ERROR, "Null param");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return CORE_ERROR, "xact_commit error");

    return CORE_OK;
}
