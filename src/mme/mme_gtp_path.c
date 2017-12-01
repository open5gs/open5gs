#define TRACE_MODULE _mme_s11_path
#include "core_debug.h"
#include "core_pkbuf.h"

#include "gtp_node.h"
#include "gtp_path.h"
#include "gtp_xact.h"

#include "mme_event.h"
#include "mme_gtp_path.h"
#include "mme_s11_build.h"

static int _gtpv2_c_recv_cb(sock_id sock, void *data)
{
    status_t rv;
    event_t e;
    pkbuf_t *pkbuf = NULL;

    d_assert(sock, return -1, "Null param");

    rv = gtp_recv(sock, &pkbuf);
    if (rv != CORE_OK)
    {
        if (errno == EAGAIN)
            return 0;

        return -1;
    }

    d_trace(10, "S11_PDU is received from SGW\n");
    d_trace_hex(10, pkbuf->payload, pkbuf->len);

    event_set(&e, MME_EVT_S11_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)pkbuf);
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
    c_socknode_t *node;
    sock_id temp;  /* FIXME ADDR */

    for (node = list_first(&mme_self()->gtpc4_list);
            node; node = list_next(node))
    {
        rv = gtp_server(&node->sock, node->sa_list, _gtpv2_c_recv_cb);
        if (rv != CORE_OK)
        {
            d_error("Can't establish GTP-C Path for SGW");
            return rv;
        }
        temp = node->sock; /* FIXME ADDR : Shoud be removed */
    }

#if 0
    for (node = list_first(&mme_self()->gtpc6_list);
            node; node = list_next(node))
    {
        rv = gtp_server(&node->sock, node->sa_list, _gtpv2_c_recv_cb);
        if (rv != CORE_OK)
        {
            d_error("Can't establish GTP-C Path for SGW");
            return rv;
        }
    }
#endif

    /* FIXME : socket descriptor needs in gnode when packet is sending initilly */
    while(sgw)
    {
        sgw->sock = temp;
        sgw = mme_sgw_next(sgw);
    }

    return CORE_OK;
}

status_t mme_gtp_close()
{
    c_socknode_t *node;

    for (node = list_first(&mme_self()->gtpc4_list);
            node; node = list_next(node))
    {
        sock_delete(node->sock);
    }

#if 0
    for (node = list_first(&mme_self()->gtpc6_list);
            node; node = list_next(node))
    {
        sock_delete(node->sock);
    }
#endif

    return CORE_OK;
}

status_t mme_gtp_send_create_session_request(mme_sess_t *sess)
{
    status_t rv;
    gtp_header_t h;
    pkbuf_t *pkbuf = NULL;
    gtp_xact_t *xact = NULL;
    mme_ue_t *mme_ue = NULL;

    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_CREATE_SESSION_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_create_session_request(&pkbuf, h.type, sess);
    d_assert(rv == CORE_OK, return CORE_ERROR,
            "S11 build error");

    xact = gtp_xact_local_create(mme_ue->sgw, &h, pkbuf);
    d_assert(xact, return CORE_ERROR, "Null param");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return CORE_ERROR, "xact_commit error");

    return CORE_OK;
}


status_t mme_gtp_send_modify_bearer_request(
        mme_bearer_t *bearer, int uli_presence)
{
    status_t rv;

    gtp_xact_t *xact = NULL;
    mme_ue_t *mme_ue = NULL;

    gtp_header_t h;
    pkbuf_t *pkbuf = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_MODIFY_BEARER_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_modify_bearer_request(
            &pkbuf, h.type, bearer, uli_presence);
    d_assert(rv == CORE_OK, return CORE_ERROR, "S11 build error");

    xact = gtp_xact_local_create(mme_ue->sgw, &h, pkbuf);
    d_assert(xact, return CORE_ERROR, "Null param");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return CORE_ERROR, "xact_commit error");

    return CORE_OK;
}

status_t mme_gtp_send_delete_session_request(mme_sess_t *sess)
{
    status_t rv;
    pkbuf_t *s11buf = NULL;
    gtp_header_t h;
    gtp_xact_t *xact = NULL;
    mme_ue_t *mme_ue = NULL;

    d_assert(sess, return CORE_ERROR, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_DELETE_SESSION_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_delete_session_request(&s11buf, h.type, sess);
    d_assert(rv == CORE_OK, return CORE_ERROR, "S11 build error");

    xact = gtp_xact_local_create(mme_ue->sgw, &h, s11buf);
    d_assert(xact, return CORE_ERROR, "Null param");

    GTP_XACT_STORE_SESSION(xact, sess);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return CORE_ERROR, "xact_commit error");

    GTP_COUNTER_INCREMENT(mme_ue, GTP_COUNTER_DELETE_SESSION);

    return CORE_OK;
}

status_t mme_gtp_send_delete_all_sessions(mme_ue_t *mme_ue)
{
    status_t rv;
    mme_sess_t *sess = NULL, *next_sess = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    sess = mme_sess_first(mme_ue);
    while (sess != NULL)
    {
        next_sess = mme_sess_next(sess);

        if (MME_HAVE_SGW_S1U_PATH(sess))
        {
            rv = mme_gtp_send_delete_session_request(sess);
            d_assert(rv == CORE_OK, return CORE_ERROR,
                    "mme_gtp_send_delete_session_request error");
        }
        else
        {
            mme_sess_remove(sess);
        }

        sess = next_sess;
    }

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

status_t mme_gtp_send_release_access_bearers_request(mme_ue_t *mme_ue)
{
    status_t rv;
    gtp_header_t h;
    pkbuf_t *pkbuf = NULL;
    gtp_xact_t *xact = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_RELEASE_ACCESS_BEARERS_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_release_access_bearers_request(&pkbuf, h.type);
    d_assert(rv == CORE_OK, return CORE_ERROR, "S11 build error");

    xact = gtp_xact_local_create(mme_ue->sgw, &h, pkbuf);
    d_assert(xact, return CORE_ERROR, "Null param");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return CORE_ERROR, "xact_commit error");

    return CORE_OK;
}

status_t mme_gtp_send_create_indirect_data_forwarding_tunnel_request(
        mme_ue_t *mme_ue)
{
    status_t rv;
    gtp_header_t h;
    pkbuf_t *pkbuf = NULL;
    gtp_xact_t *xact = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_create_indirect_data_forwarding_tunnel_request(
            &pkbuf, h.type, mme_ue);
    d_assert(rv == CORE_OK, return CORE_ERROR, "S11 build error");

    xact = gtp_xact_local_create(mme_ue->sgw, &h, pkbuf);
    d_assert(xact, return CORE_ERROR, "Null param");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return CORE_ERROR, "xact_commit error");

    return CORE_OK;
}

status_t mme_gtp_send_delete_indirect_data_forwarding_tunnel_request(
        mme_ue_t *mme_ue)
{
    status_t rv;
    gtp_header_t h;
    pkbuf_t *pkbuf = NULL;
    gtp_xact_t *xact = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    pkbuf = pkbuf_alloc(TLV_MAX_HEADROOM, 0);
    d_assert(pkbuf, return CORE_ERROR, "S11 build error");

    xact = gtp_xact_local_create(mme_ue->sgw, &h, pkbuf);
    d_assert(xact, return CORE_ERROR, "Null param");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return CORE_ERROR, "xact_commit error");

    return CORE_OK;
}
