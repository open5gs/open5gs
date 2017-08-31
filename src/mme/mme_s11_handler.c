#define TRACE_MODULE _mme_s11_handler

#include "core_debug.h"

#include "gtp_types.h"

#include "mme_event.h"
#include "mme_context.h"

#include "s1ap_build.h"
#include "s1ap_path.h"
#include "mme_s11_build.h"
#include "mme_s11_handler.h"
#include "mme_gtp_path.h"

void mme_s11_handle_create_session_request(mme_sess_t *sess)
{
    status_t rv;
    pkbuf_t *pkbuf = NULL;

    /* Use round-robin for selecting SGW */
    d_assert(sess, return, "Null param");
    d_assert(sess->sgw, return, "Null param");
    sess->sgw = mme_sgw_next(sess->sgw);
    if (!sess->sgw) sess->sgw = mme_sgw_first();

    rv = mme_s11_build_create_session_request(&pkbuf, sess);
    d_assert(rv == CORE_OK, return,
            "S11 build error");

    rv = mme_s11_send_to_sgw(sess, GTP_CREATE_SESSION_REQUEST_TYPE, pkbuf);
    d_assert(rv == CORE_OK, return, "S11 send error");
}

void mme_s11_handle_create_session_response(
        gtp_xact_t *xact, mme_sess_t *sess, 
        gtp_create_session_response_t *rsp)
{
    status_t rv;
    gtp_f_teid_t *sgw_s11_teid = NULL;
    gtp_f_teid_t *sgw_s1u_teid = NULL;

    mme_bearer_t *bearer = NULL;
    pdn_t *pdn = NULL;
    
    d_assert(rsp, return, "Null param");
    d_assert(sess, return, "Null param");
    bearer = mme_default_bearer_in_sess(sess);
    d_assert(bearer, return, "Null param");
    pdn = sess->pdn;
    d_assert(pdn, return, "Null param");

    if (rsp->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No GTP TEID");
        return;
    }
    if (rsp->pdn_address_allocation.presence == 0)
    {
        d_error("No PDN Address Allocation");
        return;
    }
    if (rsp->bearer_contexts_created.presence == 0)
    {
        d_error("No Bearer");
        return;
    }
    if (rsp->bearer_contexts_created.eps_bearer_id.presence == 0)
    {
        d_error("No EPS Bearer ID");
        return;
    }
    if (rsp->bearer_contexts_created.s1_u_enodeb_f_teid.presence == 0)
    {
        d_error("No GTP TEID");
        return;
    }

    /* Receive Control Plane(UL) : SGW-S11 */
    sgw_s11_teid = rsp->sender_f_teid_for_control_plane.data;
    sess->sgw_s11_teid = ntohl(sgw_s11_teid->teid);
    sess->sgw_s11_addr = sgw_s11_teid->ipv4_addr;

    memcpy(&pdn->paa, rsp->pdn_address_allocation.data,
            rsp->pdn_address_allocation.len);

    if (rsp->protocol_configuration_options.presence)
    {
        sess->pgw_pco_len = rsp->protocol_configuration_options.len;
        memcpy(sess->pgw_pco, rsp->protocol_configuration_options.data,
                sess->pgw_pco_len);
    }

    /* Receive Data Plane(UL) : SGW-S1U */
    sgw_s1u_teid = rsp->bearer_contexts_created.s1_u_enodeb_f_teid.data;
    bearer->sgw_s1u_teid = ntohl(sgw_s1u_teid->teid);
    bearer->sgw_s1u_addr = sgw_s1u_teid->ipv4_addr;

    d_trace(3, "[GTP] Create Session Response : "
            "MME[%d] <-- SGW[%d]\n", sess->mme_s11_teid, sess->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void mme_s11_handle_delete_all_sessions_request_in_ue(mme_ue_t *mme_ue)
{
    status_t rv;
    pkbuf_t *s11buf = NULL;
    mme_sess_t *sess = NULL;

    d_assert(mme_ue, return, "Null param");
    sess = mme_sess_first(mme_ue);
    while (sess != NULL)
    {
        rv = mme_s11_build_delete_session_request(&s11buf, sess);
        d_assert(rv == CORE_OK, return, "S11 build error");

        rv = mme_s11_send_to_sgw(sess, GTP_DELETE_SESSION_REQUEST_TYPE, s11buf);
        d_assert(rv == CORE_OK, return, "S11 send error");

        sess = mme_sess_next(sess);
    }
}

void mme_s11_handle_delete_session_response(
        gtp_xact_t *xact, mme_sess_t *sess, 
        gtp_delete_session_response_t *rsp)
{
    status_t rv;

    if (rsp->cause.presence == 0)
    {
        d_error("No Cause");
        return;
    }

    d_trace(3, "[GTP] Delete Session Response : "
            "MME[%d] <-- SGW[%d]\n", sess->mme_s11_teid, sess->sgw_s11_teid);

    mme_sess_remove(sess);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void mme_s11_handle_modify_bearer_response(
        gtp_xact_t *xact, mme_sess_t *sess, 
        gtp_modify_bearer_response_t *rsp)
{
    status_t rv;

    d_trace(3, "[GTP] Modify Bearer Response : "
            "MME[%d] <-- SGW[%d]\n", sess->mme_s11_teid, sess->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void mme_s11_handle_release_access_bearers_response(
        gtp_xact_t *xact, mme_sess_t *sess, 
        gtp_release_access_bearers_response_t *rsp)
{
    status_t rv;
    d_assert(rsp, return, "Null param");
    d_assert(sess, return, "Null param");

    if (rsp->cause.presence == 0)
    {
        d_error("No Cause");
        return;
    }

    d_trace(3, "[GTP] Release Access Bearers Response : "
            "MME[%d] <-- SGW[%d]\n", sess->mme_s11_teid, sess->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void mme_s11_handle_downlink_data_notification(
        gtp_xact_t *xact, mme_sess_t *sess, 
        gtp_downlink_data_notification_t *noti)
{
    status_t rv;
    pkbuf_t *s11buf = NULL;

    d_assert(xact, return, "Null param");
    d_assert(sess, return, "Null param");
    d_assert(noti, return, "Null param");

    d_trace(3, "[GTP] Downlink Data Notification : "
            "MME[%d] <-- SGW[%d]\n", sess->mme_s11_teid, sess->sgw_s11_teid);

    /* Build Downlink data notification ack */
    rv = mme_s11_build_downlink_data_notification_ack(&s11buf, sess);
    d_assert(rv == CORE_OK, return, "S11 build error");

    rv = gtp_xact_update_tx(xact, 
            GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE, 
            sess->sgw_s11_teid, s11buf);
    d_assert(rv == CORE_OK, return, "xact_update_tx error");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}
