#define TRACE_MODULE _mme_gtp_path
#include "core_debug.h"
#include "core_pkbuf.h"

#include "gtp/gtp_node.h"
#include "gtp/gtp_path.h"
#include "gtp/gtp_xact.h"

#include "mme_event.h"
#include "mme_gtp_path.h"
#include "mme_s11_build.h"
#include "mme_sm.h"

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

    d_trace(50, "[GTPv2] RECV : ");
    d_trace_hex(50, pkbuf->payload, pkbuf->len);

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

static c_sockaddr_t *gtp_addr_find_by_family(list_t *list, int family)
{
    gtp_node_t *gnode = NULL;
    d_assert(list, return NULL,);

    for (gnode = list_first(list); gnode; gnode = list_next(gnode))
    {
        c_sockaddr_t *addr = gnode->sa_list;
        while(addr)
        {
            if (addr->c_sa_family == family)
            {
                return addr;
            }
            addr = addr->next;
        }
    }

    return NULL;
}

status_t mme_gtp_open()
{
    status_t rv;
    gtp_node_t *gnode = NULL;

    rv = gtp_server_list(&mme_self()->gtpc_list, _gtpv2_c_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = gtp_server_list(&mme_self()->gtpc_list6, _gtpv2_c_recv_cb);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    mme_self()->gtpc_sock = gtp_local_sock_first(&mme_self()->gtpc_list);
    mme_self()->gtpc_sock6 = gtp_local_sock_first(&mme_self()->gtpc_list6);
    mme_self()->gtpc_addr = gtp_local_addr_first(&mme_self()->gtpc_list);
    mme_self()->gtpc_addr6 = gtp_local_addr_first(&mme_self()->gtpc_list6);

    d_assert(mme_self()->gtpc_addr || mme_self()->gtpc_addr6,
            return CORE_ERROR, "No GTP Server");

    mme_self()->pgw_addr = gtp_addr_find_by_family(
            &mme_self()->pgw_list, AF_INET);
    mme_self()->pgw_addr6 = gtp_addr_find_by_family(
            &mme_self()->pgw_list, AF_INET6);
    d_assert(mme_self()->pgw_addr || mme_self()->pgw_addr6,
            return CORE_ERROR,);

    for (gnode = list_first(&mme_self()->sgw_list);
            gnode; gnode = list_next(gnode))
    {
        rv = gtp_client(gnode);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
    }

    return CORE_OK;
}

status_t mme_gtp_close()
{
    sock_delete_list(&mme_self()->gtpc_list);
    sock_delete_list(&mme_self()->gtpc_list6);

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

    xact = gtp_xact_local_create(mme_ue->gnode, &h, pkbuf);
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

    xact = gtp_xact_local_create(mme_ue->gnode, &h, pkbuf);
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

    xact = gtp_xact_local_create(mme_ue->gnode, &h, s11buf);
    d_assert(xact, return CORE_ERROR, "Null param");

    GTP_XACT_STORE_SESSION(xact, sess);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return CORE_ERROR, "xact_commit error");

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
            mme_bearer_t *bearer = mme_default_bearer_in_sess(sess);
            d_assert(bearer,, "Null param");

            if (bearer && FSM_CHECK(&bearer->sm, esm_state_pdn_will_disconnect))
            {
                d_warn("PDN will disconnect[EBI:%d]", bearer->ebi);
            }
            else
            {
                rv = mme_gtp_send_delete_session_request(sess);
                d_assert(rv == CORE_OK, return CORE_ERROR,
                        "mme_gtp_send_delete_session_request error");
            }
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

status_t mme_gtp_send_update_bearer_response(mme_bearer_t *bearer)
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
    h.type = GTP_UPDATE_BEARER_RESPONSE_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_update_bearer_response(&pkbuf, h.type, bearer);
    d_assert(rv == CORE_OK, return CORE_ERROR, "S11 build error");

    rv = gtp_xact_update_tx(xact, &h, pkbuf);
    d_assert(xact, return CORE_ERROR, "Null param");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return CORE_ERROR, "xact_commit error");

    return CORE_OK;
}

status_t mme_gtp_send_delete_bearer_response(mme_bearer_t *bearer)
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
    h.type = GTP_DELETE_BEARER_RESPONSE_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_delete_bearer_response(&pkbuf, h.type, bearer);
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

    xact = gtp_xact_local_create(mme_ue->gnode, &h, pkbuf);
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

    xact = gtp_xact_local_create(mme_ue->gnode, &h, pkbuf);
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

    xact = gtp_xact_local_create(mme_ue->gnode, &h, pkbuf);
    d_assert(xact, return CORE_ERROR, "Null param");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return CORE_ERROR, "xact_commit error");

    return CORE_OK;
}
