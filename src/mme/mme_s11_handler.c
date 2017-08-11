#define TRACE_MODULE _mme_s11_handler

#include "core_debug.h"

#include "gtp_types.h"

#include "mme_event.h"
#include "mme_context.h"

#include "s1ap_build.h"
#include "s1ap_path.h"
#include "mme_s11_build.h"
#include "mme_s11_handler.h"
#include "mme_s11_path.h"

void mme_s11_handle_create_session_request(mme_sess_t *sess)
{
    status_t rv;
    pkbuf_t *pkbuf = NULL;

    /* FIXME : SGW Selection */
    MME_SESSION_SELECT_SGW(sess);

    rv = mme_s11_build_create_session_request(&pkbuf, sess);
    d_assert(rv == CORE_OK, return,
            "S11 build error");

    rv = mme_s11_send_to_sgw(sess, GTP_CREATE_SESSION_REQUEST_TYPE, pkbuf);
    d_assert(rv == CORE_OK, return, "S11 send error");
}

void mme_s11_handle_create_session_response(
        mme_sess_t *sess, gtp_create_session_response_t *rsp)
{
    gtp_f_teid_t *sgw_s11_teid = NULL;
    gtp_f_teid_t *sgw_s1u_teid = NULL;

    mme_bearer_t *bearer = NULL;
    pdn_t *pdn = NULL;
    
    d_assert(sess, return, "Null param");
    d_assert(rsp, return, "Null param");

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

    bearer = mme_bearer_find_by_sess_ebi(
            sess, rsp->bearer_contexts_created.eps_bearer_id.u8);
    d_assert(bearer, return, "No ESM Context");
    pdn = sess->pdn;
    d_assert(pdn, return, "No PDN Context");

    /* Receive Control Plane(UL) : SGW-S11 */
    sgw_s11_teid = rsp->sender_f_teid_for_control_plane.data;
    sess->sgw_s11_teid = ntohl(sgw_s11_teid->teid);
    sess->sgw_s11_addr = sgw_s11_teid->ipv4_addr;

    memcpy(&pdn->paa, rsp->pdn_address_allocation.data,
            rsp->pdn_address_allocation.len);

    if (rsp->protocol_configuration_options.presence)
    {
        bearer->pgw_pco_len = rsp->protocol_configuration_options.len;
        memcpy(bearer->pgw_pco, rsp->protocol_configuration_options.data,
                bearer->pgw_pco_len);
    }

    /* Receive Data Plane(UL) : SGW-S1U */
    sgw_s1u_teid = rsp->bearer_contexts_created.s1_u_enodeb_f_teid.data;
    bearer->sgw_s1u_teid = ntohl(sgw_s1u_teid->teid);
    bearer->sgw_s1u_addr = sgw_s1u_teid->ipv4_addr;

    d_trace(3, "[GTP] Create Session Response : "
            "MME[%d] <-- SGW[%d]\n", sess->mme_s11_teid, sess->sgw_s11_teid);
}

void mme_s11_handle_modify_bearer_response(
        mme_sess_t *sess, gtp_modify_bearer_response_t *rsp)
{
    d_trace(3, "[GTP] Modify Bearer Response : "
            "MME[%d] <-- SGW[%d]\n", sess->mme_s11_teid, sess->sgw_s11_teid);
}

void mme_s11_handle_delete_session_response(
        mme_sess_t *sess, gtp_delete_session_response_t *rsp)
{
    if (rsp->cause.presence == 0)
    {
        d_error("No Cause");
        return;
    }

    mme_sess_remove(sess);

    d_trace(3, "[GTP] Delete Session Response : "
            "MME[%d] <-- SGW[%d]\n", sess->mme_s11_teid, sess->sgw_s11_teid);
}

void mme_s11_handle_release_access_bearers_response(
        mme_sess_t *sess, gtp_release_access_bearers_response_t *rsp)
{
    d_assert(rsp, return, "Null param");
    d_assert(sess, return, "Null param");

    if (rsp->cause.presence == 0)
    {
        d_error("No Cause");
        return;
    }

    d_trace(3, "[GTP] Release Access Bearers Response : "
            "MME[%d] <-- SGW[%d]\n", sess->mme_s11_teid, sess->sgw_s11_teid);
}

void mme_s11_handle_downlink_data_notification(
        gtp_xact_t *xact, mme_sess_t *sess, 
        gtp_downlink_data_notification_t *noti)
{
    status_t rv;
    mme_bearer_t *bearer = NULL;
    pkbuf_t *s11buf = NULL;

    d_assert(xact, return, "Null param");
    d_assert(sess, return, "Null param");
    d_assert(noti, return, "Null param");

    d_trace(3, "[GTP] Downlink Data Notification : "
            "MME[%d] <-- SGW[%d]\n", sess->mme_s11_teid, sess->sgw_s11_teid);

    bearer = mme_bearer_find_by_sess_ebi(sess, noti->eps_bearer_id.u8);
    d_assert(bearer, return, "No ESM Context");

    /* Build Downlink data notification ack */
    rv = mme_s11_build_downlink_data_notification_ack(&s11buf, sess);
    d_assert(rv == CORE_OK, return, "S11 build error");

    d_assert(gtp_xact_commit(xact, 
                GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE, 
                sess->sgw_s11_teid, s11buf) == CORE_OK,
            return , "xact commit error");
}
