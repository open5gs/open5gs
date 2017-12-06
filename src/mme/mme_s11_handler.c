#define TRACE_MODULE _mme_s11_handler

#include "core_debug.h"

#include "gtp_types.h"
#include "gtp_conv.h"
#include "gtp_xact.h"

#include "mme_event.h"
#include "mme_sm.h"
#include "mme_context.h"

#include "s1ap_path.h"
#include "mme_gtp_path.h"
#include "nas_path.h"
#include "mme_fd_path.h"

#include "mme_s11_build.h"
#include "mme_s11_handler.h"

void mme_s11_handle_create_session_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue, gtp_create_session_response_t *rsp)
{
    status_t rv;
    gtp_f_teid_t *sgw_s11_teid = NULL;
    gtp_f_teid_t *sgw_s1u_teid = NULL;

    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;
    pdn_t *pdn = NULL;
    
    d_assert(xact, return, "Null param");
    d_assert(mme_ue, return, "Null param");
    d_assert(rsp, return, "Null param");

    if (rsp->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No S11 TEID");
        return;
    }
    if (rsp->pdn_address_allocation.presence == 0)
    {
        d_error("No PDN Address Allocation");
        return;
    }
    if (rsp->bearer_contexts_created.s1_u_enodeb_f_teid.presence == 0)
    {
        d_error("No S1U TEID");
        return;
    }
    if (rsp->bearer_contexts_created.presence == 0)
    {
        d_error("No Bearer");
        return;
    }
    if (rsp->bearer_contexts_created.  eps_bearer_id.presence == 0)
    {
        d_error("No EPS Bearer ID");
        return;
    }

    d_assert(mme_ue, return, "Null param");

    bearer = mme_bearer_find_by_ue_ebi(mme_ue, 
            rsp->bearer_contexts_created.eps_bearer_id.u8);
    d_assert(bearer, return, "Null param");
    sess = bearer->sess;
    d_assert(sess, return, "Null param");
    pdn = sess->pdn;
    d_assert(pdn, return, "Null param");

    /* Control Plane(UL) : SGW-S11 */
    sgw_s11_teid = rsp->sender_f_teid_for_control_plane.data;
    mme_ue->sgw_s11_teid = ntohl(sgw_s11_teid->teid);

    memcpy(&pdn->paa, rsp->pdn_address_allocation.data,
            rsp->pdn_address_allocation.len);

    /* PCO */
    if (rsp->protocol_configuration_options.presence)
    {
        TLV_STORE_DATA(&sess->pgw_pco, &rsp->protocol_configuration_options);
    }

    /* Data Plane(UL) : SGW-S1U */
    sgw_s1u_teid = rsp->bearer_contexts_created.s1_u_enodeb_f_teid.data;
    bearer->sgw_s1u_teid = ntohl(sgw_s1u_teid->teid);
    bearer->sgw_s1u_addr = sgw_s1u_teid->ip.addr;

    d_trace(3, "[GTP] Create Session Response : "
            "MME[%d] <-- SGW[%d]\n", mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    if (FSM_CHECK(&mme_ue->sm, emm_state_default_esm))
    {
        rv = nas_send_attach_accept(mme_ue);
        d_assert(rv == CORE_OK, return, "nas_send_attach_accept failed");
    }
    else if (FSM_CHECK(&mme_ue->sm, emm_state_attached))
    {
        rv = nas_send_activate_default_bearer_context_request(bearer);
        d_assert(rv == CORE_OK, return, "nas send failed");
    }
    else
        d_assert(0,, "Invalid EMM state");
}

void mme_s11_handle_modify_bearer_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue, gtp_modify_bearer_response_t *rsp)
{
    status_t rv;
    S1ap_Cause_t cause;
    enb_ue_t *source_ue = NULL, *target_ue = NULL;

    d_assert(xact, return, "Null param");
    d_assert(mme_ue, return, "Null param");
    d_assert(rsp, return, "Null param");

    d_trace(3, "[GTP] Modify Bearer Response : "
            "MME[%d] <-- SGW[%d]\n", mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    GTP_COUNTER_CHECK(mme_ue, GTP_COUNTER_MODIFY_BEARER_BY_PATH_SWITCH,
        rv = s1ap_send_path_switch_ack(mme_ue);
        d_assert(rv == CORE_OK, return, "s1ap send error");
    );

    GTP_COUNTER_CHECK(mme_ue, GTP_COUNTER_MODIFY_BEARER_BY_HANDOVER_NOTIFY,
        target_ue = mme_ue->enb_ue;
        d_assert(target_ue, return, "Null param");
        source_ue = target_ue->source_ue;
        d_assert(source_ue, return, "Null param");

        cause.present = S1ap_Cause_PR_nas;
        cause.choice.nas = S1ap_CauseNas_normal_release;

        rv = s1ap_send_ue_context_release_commmand(source_ue, &cause, 300);
        d_assert(rv == CORE_OK, return, "s1ap send error");
    );
}

void mme_s11_handle_delete_session_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue, gtp_delete_session_response_t *rsp)
{
    status_t rv;
    mme_sess_t *sess = NULL;

    d_assert(rsp, return, "Null param");
    sess = GTP_XACT_RETRIEVE_SESSION(xact);
    d_assert(sess, return, "Null param");

    if (rsp->cause.presence == 0)
    {
        d_error("No Cause");
        return;
    }

    d_trace(3, "[GTP] Delete Session Response : MME[%d] <-- SGW[%d]\n",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    if (FSM_CHECK(&mme_ue->sm, emm_state_authentication))
    {
        GTP_COUNTER_CHECK(mme_ue, GTP_COUNTER_DELETE_SESSION,
            CLEAR_SGW_S11_PATH(mme_ue);
            mme_s6a_send_air(mme_ue);
        );

        mme_sess_remove(sess);
    }
    else if (FSM_CHECK(&mme_ue->sm, emm_state_detached))
    {
        GTP_COUNTER_CHECK(mme_ue, GTP_COUNTER_DELETE_SESSION,
            CLEAR_SGW_S11_PATH(mme_ue);
            rv = nas_send_detach_accept(mme_ue);
            d_assert(rv == CORE_OK, return, "nas_send_detach_accept failed");
        );

        mme_sess_remove(sess);
    }
    else if (FSM_CHECK(&mme_ue->sm, emm_state_attached))
    {
        mme_bearer_t *bearer = mme_default_bearer_in_sess(sess);
        d_assert(bearer, return, "Null param");

        GTP_COUNTER_CHECK(mme_ue, GTP_COUNTER_DELETE_SESSION,); 

        if (FSM_CHECK(&bearer->sm, esm_state_disconnect))
        {
            rv = nas_send_deactivate_bearer_context_request(bearer);
            d_assert(rv == CORE_OK, return,
                "nas_send_deactivate_bearer_context_request failed");
        }
        else
        {
            d_assert(0,, "Invalid ESM state");
        }
    }
    else
        d_assert(0,, "Invalid EMM state");

}
void mme_s11_handle_create_bearer_request(
        gtp_xact_t *xact, mme_ue_t *mme_ue, gtp_create_bearer_request_t *req)
{
    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;

    gtp_f_teid_t *sgw_s1u_teid = NULL;
    gtp_bearer_qos_t bearer_qos;

    d_assert(xact, return, "Null param");
    d_assert(mme_ue, return, "Null param");
    d_assert(req, return, "Null param");

    d_trace(3, "[GTP] Create Bearer Request : "
            "MME[%d] <-- SGW[%d]\n", mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    if (req->linked_eps_bearer_id.presence == 0)
    {
        d_error("No Linked EBI");
        return;
    }
    if (req->bearer_contexts.presence == 0)
    {
        d_error("No Bearer");
        return;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0)
    {
        d_error("No EPS Bearer ID");
        return;
    }
    if (req->bearer_contexts.s1_u_enodeb_f_teid.presence == 0)
    {
        d_error("No GTP TEID");
        return;
    }
    if (req->bearer_contexts.bearer_level_qos.presence == 0)
    {
        d_error("No QoS");
        return;
    }
    if (req->bearer_contexts.tft.presence == 0)
    {
        d_error("No TFT");
        return;
    }

    sess = mme_sess_find_by_ebi(mme_ue, req->linked_eps_bearer_id.u8);
    d_assert(sess, return, 
            "No Session Context(EBI:%d)", req->linked_eps_bearer_id);

    bearer = mme_bearer_add(sess);
    d_assert(bearer, return, "No Bearer Context");

    /* Data Plane(UL) : SGW-S1U */
    sgw_s1u_teid = req->bearer_contexts.s1_u_enodeb_f_teid.data;
    bearer->sgw_s1u_teid = ntohl(sgw_s1u_teid->teid);
    bearer->sgw_s1u_addr = sgw_s1u_teid->ip.addr;

    /* Bearer QoS */
    d_assert(gtp_parse_bearer_qos(&bearer_qos,
        &req->bearer_contexts.bearer_level_qos) ==
        req->bearer_contexts.bearer_level_qos.len, return,);
    bearer->qos.qci = bearer_qos.qci;
    bearer->qos.arp.priority_level = bearer_qos.priority_level;
    bearer->qos.arp.pre_emption_capability =
                    bearer_qos.pre_emption_capability;
    bearer->qos.arp.pre_emption_vulnerability =
                    bearer_qos.pre_emption_vulnerability;
    bearer->qos.mbr.downlink = bearer_qos.dl_mbr;
    bearer->qos.mbr.uplink = bearer_qos.ul_mbr;
    bearer->qos.gbr.downlink = bearer_qos.dl_gbr;
    bearer->qos.gbr.uplink = bearer_qos.ul_gbr;

    /* Save Bearer TFT */
    TLV_STORE_DATA(&bearer->tft, &req->bearer_contexts.tft);

    /* Save Transaction. will be handled after EMM-attached */
    bearer->xact = xact;
}

void mme_s11_handle_release_access_bearers_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue,
        gtp_release_access_bearers_response_t *rsp)
{
    status_t rv;
    enb_ue_t *enb_ue = NULL;
    S1ap_Cause_t cause;

    d_assert(xact, return, "Null param");
    d_assert(mme_ue, return, "Null param");
    d_assert(rsp, return, "Null param");

    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");

    if (rsp->cause.presence == 0)
    {
        d_error("No Cause");
        return;
    }

    d_trace(3, "[GTP] Release Access Bearers Response : "
            "MME[%d] <-- SGW[%d]\n", mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    cause.present = S1ap_Cause_PR_nas;
    cause.choice.nas = S1ap_CauseNas_normal_release;
    rv = s1ap_send_ue_context_release_commmand(enb_ue, &cause, 0);
    d_assert(rv == CORE_OK,, "s1ap send error");
}

void mme_s11_handle_downlink_data_notification(
        gtp_xact_t *xact, mme_ue_t *mme_ue,
        gtp_downlink_data_notification_t *noti)
{
    status_t rv;
    gtp_header_t h;
    pkbuf_t *s11buf = NULL;

    d_assert(xact, return, "Null param");
    d_assert(mme_ue, return, "Null param");
    d_assert(noti, return, "Null param");

    d_trace(3, "[GTP] Downlink Data Notification : "
            "MME[%d] <-- SGW[%d]\n", mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    /* Build Downlink data notification ack */
    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_downlink_data_notification_ack(&s11buf, h.type);
    d_assert(rv == CORE_OK, return, "S11 build error");

    rv = gtp_xact_update_tx(xact, &h, s11buf);
    d_assert(rv == CORE_OK, return, "xact_update_tx error");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");
}

void mme_s11_handle_create_indirect_data_forwarding_tunnel_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue,
        gtp_create_indirect_data_forwarding_tunnel_response_t *rsp)
{
    status_t rv;
    mme_bearer_t *bearer = NULL;
    enb_ue_t *source_ue = NULL;
    int i;

    tlv_bearer_context_t *bearers[GTP_MAX_NUM_OF_INDIRECT_TUNNEL];
    gtp_f_teid_t *teid = NULL;

    d_assert(xact, return, "Null param");
    d_assert(mme_ue, return, "Null param");
    d_assert(rsp, return, "Null param");

    source_ue = mme_ue->enb_ue;
    d_assert(source_ue, return, "Null param");

    if (rsp->cause.presence == 0)
    {
        d_error("No Cause");
        return;
    }

    d_trace(3, "[GTP] Create Indirect Data Forwarding Tunnel Response : "
            "MME[%d] <-- SGW[%d]\n", mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    gtp_bearers_in_create_indirect_tunnel_response(&bearers, rsp);

    for (i = 0; bearers[i]->presence; i++)
    {
        if (bearers[i]->eps_bearer_id.presence == 0)
        {
            d_error("No EBI");
            return;
        }

        bearer = mme_bearer_find_by_ue_ebi(mme_ue, 
                    bearers[i]->eps_bearer_id.u8);
        d_assert(bearer, return, "No Bearer Context");

        if (bearers[i]->s4_u_sgsn_f_teid.presence)
        {
            teid = bearers[i]->s4_u_sgsn_f_teid.data;
            d_assert(teid, return,);

            bearer->sgw_dl_teid = ntohl(teid->teid);
            bearer->sgw_dl_addr = teid->ip.addr;
        }
        if (bearers[i]->s2b_u_epdg_f_teid_5.presence)
        {
            teid = bearers[i]->s2b_u_epdg_f_teid_5.data;
            d_assert(teid, return,);

            bearer->sgw_ul_teid = ntohl(teid->teid);
            bearer->sgw_ul_addr = teid->ip.addr;
        }
    }

    rv = s1ap_send_handover_command(source_ue);
    d_assert(rv == CORE_OK,, "s1ap send error");
}

void mme_s11_handle_delete_indirect_data_forwarding_tunnel_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue,
        gtp_delete_indirect_data_forwarding_tunnel_response_t *rsp)
{
    status_t rv;

    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    d_assert(xact, return, "Null param");
    d_assert(mme_ue, return, "Null param");
    d_assert(rsp, return, "Null param");

    if (rsp->cause.presence == 0)
    {
        d_error("No Cause");
        return;
    }

    d_trace(3, "[GTP] Delete Indirect Data Forwarding Tunnel Response : "
            "MME[%d] <-- SGW[%d]\n", mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return, "xact_commit error");

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        bearer = mme_bearer_first(sess);
        while(bearer)
        {
            CLEAR_INDIRECT_TUNNEL(bearer);

            bearer = mme_bearer_next(bearer);
        }
        sess = mme_sess_next(sess);
    }
}
