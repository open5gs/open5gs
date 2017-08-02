#define TRACE_MODULE _mme_s11_handler

#include "core_debug.h"

#include "gtp_types.h"

#include "mme_event.h"
#include "mme_context.h"
#include "mme_s11_handler.h"

void mme_s11_handle_create_session_response(
        mme_sess_t *sess, gtp_create_session_response_t *rsp)
{
    event_t e;

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
    pdn = bearer->pdn;
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

    event_set(&e, MME_EVT_EMM_BEARER_FROM_S11);
    event_set_param1(&e, (c_uintptr_t)bearer->index);
    event_set_param2(&e, (c_uintptr_t)GTP_CREATE_SESSION_RESPONSE_TYPE);
    mme_event_send(&e);
}

void mme_s11_handle_modify_bearer_response(
        mme_sess_t *sess, gtp_modify_bearer_response_t *rsp)
{
    mme_ue_t *mme_ue = NULL;
    event_t e;

    d_assert(sess, return, "Null param");
    mme_ue = sess->mme_ue;

    event_set(&e, MME_EVT_EMM_UE_FROM_S11);
    event_set_param1(&e, (c_uintptr_t)mme_ue->index);
    event_set_param2(&e, (c_uintptr_t)GTP_MODIFY_BEARER_RESPONSE_TYPE);
    mme_event_send(&e);
}

void mme_s11_handle_delete_session_response(
        mme_sess_t *sess, gtp_delete_session_response_t *rsp)
{
    event_t e;

    mme_bearer_t *bearer;
    
    d_assert(sess, return, "Null param");
    d_assert(rsp, return, "Null param");
    bearer = mme_default_bearer_in_sess(sess);
    d_assert(bearer, return, "Null param");

    if (rsp->cause.presence == 0)
    {
        d_error("No Cause");
        return;
    }

    d_trace(3, "[GTP] Delete Session Response : "
            "MME[%d] <-- SGW[%d]\n", sess->mme_s11_teid, sess->sgw_s11_teid);

    event_set(&e, MME_EVT_EMM_BEARER_FROM_S11);
    event_set_param1(&e, (c_uintptr_t)bearer->index);
    event_set_param2(&e, (c_uintptr_t)GTP_DELETE_SESSION_RESPONSE_TYPE);
    mme_event_send(&e);
}

void mme_s11_handle_release_access_bearers_response(
        mme_sess_t *sess, gtp_release_access_bearers_response_t *rsp)
{
    event_t e;

    mme_ue_t *mme_ue;
    enb_ue_t *enb_ue;
    mme_enb_t *enb;
    
    d_assert(rsp, return, "Null param");
    d_assert(sess, return, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return, "Null param");

    if (rsp->cause.presence == 0)
    {
        d_error("No Cause");
        return;
    }

    d_trace(3, "[GTP] Release Access Bearers Response : "
            "MME[%d] <-- SGW[%d]\n", sess->mme_s11_teid, sess->sgw_s11_teid);

    event_set(&e, MME_EVT_S1AP_UE_FROM_S11);
    event_set_param1(&e, (c_uintptr_t)enb->index);
    event_set_param2(&e, (c_uintptr_t)enb_ue->index);
    event_set_param3(&e, (c_uintptr_t)GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE);
    mme_event_send(&e);
}

void mme_s11_handle_downlink_data_notification(
        mme_sess_t *sess, gtp_downlink_data_notification_t *noti)
{
    event_t e;
    mme_bearer_t *bearer = NULL;

    d_assert(sess, return, "Null param");
    d_assert(noti, return, "Null param");

    d_trace(3, "[GTP] Downlink Data Notification : "
            "MME[%d] <-- SGW[%d]\n", sess->mme_s11_teid, sess->sgw_s11_teid);

    bearer = mme_bearer_find_by_sess_ebi(sess, noti->eps_bearer_id.u8);
    d_assert(bearer, return, "No ESM Context");

    event_set(&e, MME_EVT_EMM_BEARER_FROM_S11);
    event_set_param1(&e, (c_uintptr_t)bearer->index);
    event_set_param2(&e, (c_uintptr_t)GTP_DOWNLINK_DATA_NOTIFICATION_TYPE);
    mme_event_send(&e);
}
