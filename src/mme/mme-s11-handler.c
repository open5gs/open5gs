#include "gtp/gtp-types.h"
#include "gtp/gtp-conv.h"
#include "gtp/gtp-xact.h"

#include "mme-event.h"
#include "mme-sm.h"
#include "mme-context.h"

#include "s1ap-path.h"
#include "mme-gtp-path.h"
#include "nas-path.h"
#include "mme-fd-path.h"

#include "mme-s11-build.h"
#include "mme-s11-handler.h"

void mme_s11_handle_create_session_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue, gtp_create_session_response_t *rsp)
{
    int rv;
    gtp_f_teid_t *sgw_s11_teid = NULL;
    gtp_f_teid_t *sgw_s1u_teid = NULL;

    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;
    pdn_t *pdn = NULL;
    
    ogs_assert(xact);
    ogs_assert(mme_ue);
    ogs_assert(rsp);

    ogs_debug("[MME] Create Session Response");

    if (rsp->sender_f_teid_for_control_plane.presence == 0)
    {
        ogs_error("No S11 TEID");
        return;
    }
    if (rsp->pdn_address_allocation.presence == 0)
    {
        ogs_error("No PDN Address Allocation");
        return;
    }
    if (rsp->bearer_contexts_created.s1_u_enodeb_f_teid.presence == 0)
    {
        ogs_error("No S1U TEID");
        return;
    }
    if (rsp->bearer_contexts_created.presence == 0)
    {
        ogs_error("No Bearer");
        return;
    }
    if (rsp->bearer_contexts_created.  eps_bearer_id.presence == 0)
    {
        ogs_error("No EPS Bearer ID");
        return;
    }

    ogs_assert(mme_ue);

    bearer = mme_bearer_find_by_ue_ebi(mme_ue, 
            rsp->bearer_contexts_created.eps_bearer_id.u8);
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    pdn = sess->pdn;
    ogs_assert(pdn);

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

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);
    ogs_debug("    ENB_S1U_TEID[%d] SGW_S1U_TEID[%d]",
        bearer->enb_s1u_teid, bearer->sgw_s1u_teid);

    rv = gtp_f_teid_to_ip(sgw_s1u_teid, &bearer->sgw_s1u_ip);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_initial_context_setup))
    {
        rv = nas_send_attach_accept(mme_ue);
        ogs_assert(rv == OGS_OK);
    }
    else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_registered))
    {
        rv = nas_send_activate_default_bearer_context_request(bearer);
        ogs_assert(rv == OGS_OK);
    }
    else
        ogs_assert_if_reached();
}

void mme_s11_handle_modify_bearer_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue, gtp_modify_bearer_response_t *rsp)
{
    int rv;
    enb_ue_t *source_ue = NULL, *target_ue = NULL;

    ogs_assert(mme_ue);
    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_debug("[MME] Modify Bearer Response");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    GTP_COUNTER_CHECK(mme_ue, GTP_COUNTER_MODIFY_BEARER_BY_PATH_SWITCH,
        rv = s1ap_send_path_switch_ack(mme_ue);
        ogs_assert(rv == OGS_OK);
    );

    GTP_COUNTER_CHECK(mme_ue, GTP_COUNTER_MODIFY_BEARER_BY_HANDOVER_NOTIFY,
        target_ue = mme_ue->enb_ue;
        ogs_assert(target_ue);
        source_ue = target_ue->source_ue;
        ogs_assert(source_ue);

        rv = s1ap_send_ue_context_release_command(source_ue,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_successful_handover,
                S1AP_UE_CTX_REL_DELETE_INDIRECT_TUNNEL,
                ogs_time_from_msec(300));
        ogs_assert(rv == OGS_OK);
    );
}

void mme_s11_handle_delete_session_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue, gtp_delete_session_response_t *rsp)
{
    int rv;
    mme_sess_t *sess = NULL;

    ogs_assert(mme_ue);
    ogs_assert(xact);
    sess = GTP_XACT_RETRIEVE_SESSION(xact);
    ogs_assert(sess);
    ogs_assert(rsp);

    ogs_debug("[MME] Delete Session Response");
    if (rsp->cause.presence == 0)
    {
        ogs_error("No Cause");
        goto cleanup;
    }
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_authentication))
    {
        if (mme_sess_count(mme_ue) == 1) /* Last Session */
        {
            mme_s6a_send_air(mme_ue, NULL);
        }
    }
    else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_de_registered))
    {
        if (mme_sess_count(mme_ue) == 1) /* Last Session */
        {
            rv = nas_send_detach_accept(mme_ue);
            ogs_assert(rv == OGS_OK);
        }
    }
    else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_registered))
    {
        mme_bearer_t *bearer = mme_default_bearer_in_sess(sess);
        ogs_assert(bearer);

        if (OGS_FSM_CHECK(&bearer->sm, esm_state_pdn_will_disconnect))
        {
            rv = nas_send_deactivate_bearer_context_request(bearer);
            ogs_assert(rv == OGS_OK);
            
            /*
             * mme_sess_remove() should not be called here.
             *
             * Session will be removed if Deactivate bearer context 
             * accept is received */
            CLEAR_SGW_S1U_PATH(sess);
            return;
        }
        else if (OGS_FSM_CHECK(&bearer->sm, esm_state_active))
        {
            if (mme_sess_count(mme_ue) == 1) /* Last Session */
            {
                enb_ue_t *enb_ue = NULL;

                enb_ue = mme_ue->enb_ue;
                ogs_assert(enb_ue);

                rv = s1ap_send_ue_context_release_command(enb_ue,
                    S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                    S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
                ogs_assert(rv == OGS_OK);
            }
        }
        else
            ogs_assert_if_reached();
    }
    else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_initial_context_setup) ||
             OGS_FSM_CHECK(&mme_ue->sm, emm_state_exception))
    {
        if (mme_sess_count(mme_ue) == 1) /* Last Session */
        {
            enb_ue_t *enb_ue = NULL;

            enb_ue = mme_ue->enb_ue;
            ogs_assert(enb_ue);

            rv = s1ap_send_ue_context_release_command(enb_ue,
                S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
            ogs_assert(rv == OGS_OK);
        }
    }
    else
        ogs_assert_if_reached();

cleanup:
    if (mme_sess_count(mme_ue) == 1) /* Last Session */
        CLEAR_SESSION_CONTEXT(mme_ue);

    mme_sess_remove(sess);
}

void mme_s11_handle_create_bearer_request(
        gtp_xact_t *xact, mme_ue_t *mme_ue, gtp_create_bearer_request_t *req)
{
    int rv;
    mme_bearer_t *bearer = NULL, *default_bearer = NULL;
    mme_sess_t *sess = NULL;

    gtp_f_teid_t *sgw_s1u_teid = NULL;
    gtp_bearer_qos_t bearer_qos;

    ogs_assert(xact);
    ogs_assert(mme_ue);
    ogs_assert(req);

    ogs_debug("[MME] Create Bearer Response");

    if (req->linked_eps_bearer_id.presence == 0)
    {
        ogs_error("No Linked EBI");
        return;
    }
    if (req->bearer_contexts.presence == 0)
    {
        ogs_error("No Bearer");
        return;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0)
    {
        ogs_error("No EPS Bearer ID");
        return;
    }
    if (req->bearer_contexts.s1_u_enodeb_f_teid.presence == 0)
    {
        ogs_error("No GTP TEID");
        return;
    }
    if (req->bearer_contexts.bearer_level_qos.presence == 0)
    {
        ogs_error("No QoS");
        return;
    }
    if (req->bearer_contexts.tft.presence == 0)
    {
        ogs_error("No TFT");
        return;
    }

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    sess = mme_sess_find_by_ebi(mme_ue, req->linked_eps_bearer_id.u8);
    ogs_assert(sess);

    bearer = mme_bearer_add(sess);
    ogs_assert(bearer);

    /* Data Plane(UL) : SGW-S1U */
    sgw_s1u_teid = req->bearer_contexts.s1_u_enodeb_f_teid.data;
    bearer->sgw_s1u_teid = ntohl(sgw_s1u_teid->teid);
    rv = gtp_f_teid_to_ip(sgw_s1u_teid, &bearer->sgw_s1u_ip);
    ogs_assert(rv == OGS_OK);

    /* Bearer QoS */
    ogs_assert(gtp_parse_bearer_qos(&bearer_qos,
        &req->bearer_contexts.bearer_level_qos) ==
        req->bearer_contexts.bearer_level_qos.len);
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

    /* Before Activate DEDICATED bearer, we'll check DEFAULT bearer status */
    default_bearer = mme_default_bearer_in_sess(sess);
    ogs_assert(default_bearer);

    if (/* Check if Activate Default Bearer Accept is received */
        OGS_FSM_CHECK(&default_bearer->sm, esm_state_active) &&
        /* Check if Initial Context Setup Response or 
         *          E-RAB Setup Response is received */
        MME_HAVE_ENB_S1U_PATH(default_bearer))
    {
        rv = nas_send_activate_dedicated_bearer_context_request(bearer);
        ogs_assert(rv == OGS_OK);
    }
}

void mme_s11_handle_update_bearer_request(
        gtp_xact_t *xact, mme_ue_t *mme_ue, gtp_update_bearer_request_t *req)
{
    int rv;
    mme_bearer_t *bearer = NULL;
    gtp_bearer_qos_t bearer_qos;

    ogs_assert(xact);
    ogs_assert(mme_ue);
    ogs_assert(req);

    ogs_debug("[MME] Update Bearer Request");
    if (req->bearer_contexts.presence == 0)
    {
        ogs_error("No Bearer");
        return;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0)
    {
        ogs_error("No EPS Bearer ID");
        return;
    }
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    bearer = mme_bearer_find_by_ue_ebi(mme_ue,
            req->bearer_contexts.eps_bearer_id.u8);
    ogs_assert(bearer);

    /* Save Transaction. will be handled after EMM-attached */
    bearer->xact = xact;

    if (/* Check if Activate Default/Dedicated Bearer Accept is received */
        OGS_FSM_CHECK(&bearer->sm, esm_state_active) &&
        /* Check if Initial Context Setup Response or 
         *          E-RAB Setup Response is received */
        MME_HAVE_ENB_S1U_PATH(bearer))
    {
        if (req->bearer_contexts.bearer_level_qos.presence == 1)
        {
            /* Bearer QoS */
            ogs_assert(gtp_parse_bearer_qos(&bearer_qos,
                &req->bearer_contexts.bearer_level_qos) ==
                req->bearer_contexts.bearer_level_qos.len);
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
        }

        if (req->bearer_contexts.tft.presence == 1)
        {
            /* Save Bearer TFT */
            TLV_STORE_DATA(&bearer->tft, &req->bearer_contexts.tft);
        }

        if (req->bearer_contexts.bearer_level_qos.presence == 1 ||
            req->bearer_contexts.tft.presence == 1)
        {
            rv = nas_send_modify_bearer_context_request(
                    bearer, 
                    req->bearer_contexts.bearer_level_qos.presence,
                    req->bearer_contexts.tft.presence);
            ogs_assert(rv == OGS_OK);
        }
        else
        {
            ogs_warn("[IGNORE] Update Bearer Request : "
                    "Both QoS and TFT is NULL");
            rv = mme_gtp_send_update_bearer_response(bearer);
            ogs_assert(rv == OGS_OK);
        }
    }
    else
    {
        if (!OGS_FSM_CHECK(&bearer->sm, esm_state_active))
        {
            ogs_assert_if_reached();
        }
        else if (!MME_HAVE_ENB_S1U_PATH(bearer))
        {
            ogs_assert_if_reached();
        }
        else
            ogs_assert_if_reached();
    }

}

void mme_s11_handle_delete_bearer_request(
        gtp_xact_t *xact, mme_ue_t *mme_ue, gtp_delete_bearer_request_t *req)
{
    int rv;
    mme_bearer_t *bearer = NULL;

    ogs_assert(xact);
    ogs_assert(mme_ue);
    ogs_assert(req);

    ogs_debug("[MME] Delete Bearer Request");
    if (req->linked_eps_bearer_id.presence == 1)
    {
        bearer = mme_bearer_find_by_ue_ebi(mme_ue, req->linked_eps_bearer_id.u8);
        ogs_assert(bearer);
    }
    else if (req->eps_bearer_ids.presence == 1)
    {
        bearer = mme_bearer_find_by_ue_ebi(
                mme_ue, req->eps_bearer_ids.u8);
        ogs_assert(bearer);
    }
    else
    {
        ogs_error("No Linked EBI or EPS Bearer ID");
        return;
    }
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    /* Save Transaction. will be handled after EMM-attached */
    bearer->xact = xact;

    if (/* Check if Activate Default/Dedicated Bearer Accept is received */
        OGS_FSM_CHECK(&bearer->sm, esm_state_active) &&
        /* Check if Initial Context Setup Response or 
         *          E-RAB Setup Response is received */
        MME_HAVE_ENB_S1U_PATH(bearer))
    {
        rv = nas_send_deactivate_bearer_context_request(bearer);
        ogs_assert(rv == OGS_OK);
    }
    else
    {
        if (!OGS_FSM_CHECK(&bearer->sm, esm_state_active))
        {
            ogs_assert_if_reached();
        }
        else if (!MME_HAVE_ENB_S1U_PATH(bearer))
        {
            ogs_assert_if_reached();
        }
        else
            ogs_assert_if_reached();
    }

}

void mme_s11_handle_release_access_bearers_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue,
        gtp_release_access_bearers_response_t *rsp)
{
    int rv;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(xact);
    ogs_assert(mme_ue);
    ogs_assert(rsp);

    ogs_debug("[MME] Release Access Bearers Response");
    enb_ue = mme_ue->enb_ue;
    ogs_assert(enb_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    if (rsp->cause.presence == 0)
    {
        ogs_error("No Cause");
        return;
    }

    rv = CLEAR_BEARER_CONTEXT(mme_ue);
    ogs_assert(rv == OGS_OK);

    rv = s1ap_send_ue_context_release_command(enb_ue,
            S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
            S1AP_UE_CTX_REL_S1_NORMAL_RELEASE, 0);
    ogs_assert(rv == OGS_OK);
}

void mme_s11_handle_downlink_data_notification(
        gtp_xact_t *xact, mme_ue_t *mme_ue,
        gtp_downlink_data_notification_t *noti)
{
    int rv;
    gtp_header_t h;
    ogs_pkbuf_t *s11buf = NULL;

    ogs_assert(xact);
    ogs_assert(mme_ue);
    ogs_assert(noti);

    ogs_debug("[MME] Downlink Data Notification");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    /* Build Downlink data notification ack */
    memset(&h, 0, sizeof(gtp_header_t));
    h.type = GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    rv = mme_s11_build_downlink_data_notification_ack(&s11buf, h.type);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_update_tx(xact, &h, s11buf);
    ogs_assert(rv == OGS_OK);

    rv = gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);
}

void mme_s11_handle_create_indirect_data_forwarding_tunnel_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue,
        gtp_create_indirect_data_forwarding_tunnel_response_t *rsp)
{
    int rv;
    mme_bearer_t *bearer = NULL;
    enb_ue_t *source_ue = NULL;
    int i;

    tlv_bearer_context_t *bearers[GTP_MAX_NUM_OF_INDIRECT_TUNNEL];
    gtp_f_teid_t *teid = NULL;

    ogs_assert(xact);
    ogs_assert(mme_ue);
    ogs_assert(rsp);

    source_ue = mme_ue->enb_ue;
    ogs_assert(source_ue);

    ogs_debug("[MME] Create Indirect Data Forwarding Tunnel Response");
    if (rsp->cause.presence == 0)
    {
        ogs_error("No Cause");
        return;
    }
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    gtp_bearers_in_create_indirect_tunnel_response(&bearers, rsp);

    for (i = 0; bearers[i]->presence; i++)
    {
        if (bearers[i]->eps_bearer_id.presence == 0)
        {
            ogs_error("No EBI");
            return;
        }

        bearer = mme_bearer_find_by_ue_ebi(mme_ue, 
                    bearers[i]->eps_bearer_id.u8);
        ogs_assert(bearer);

        if (bearers[i]->s4_u_sgsn_f_teid.presence)
        {
            teid = bearers[i]->s4_u_sgsn_f_teid.data;
            ogs_assert(teid);

            bearer->sgw_dl_teid = ntohl(teid->teid);
            rv = gtp_f_teid_to_ip(teid, &bearer->sgw_dl_ip);
            ogs_assert(rv == OGS_OK);
        }
        if (bearers[i]->s2b_u_epdg_f_teid_5.presence)
        {
            teid = bearers[i]->s2b_u_epdg_f_teid_5.data;
            ogs_assert(teid);

            bearer->sgw_ul_teid = ntohl(teid->teid);
            rv = gtp_f_teid_to_ip(teid, &bearer->sgw_ul_ip);
            ogs_assert(rv == OGS_OK);
        }
    }

    rv = s1ap_send_handover_command(source_ue);
    ogs_assert(rv == OGS_OK);
}

void mme_s11_handle_delete_indirect_data_forwarding_tunnel_response(
        gtp_xact_t *xact, mme_ue_t *mme_ue,
        gtp_delete_indirect_data_forwarding_tunnel_response_t *rsp)
{
    int rv;

    ogs_assert(xact);
    ogs_assert(mme_ue);
    ogs_assert(rsp);

    ogs_debug("[MME] Delete Indirect Data Forwarding Tunnel Response");
    if (rsp->cause.presence == 0)
    {
        ogs_error("No Cause");
        return;
    }
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = gtp_xact_commit(xact);
    ogs_assert(rv == OGS_OK);

    rv = mme_ue_clear_indirect_tunnel(mme_ue);
    ogs_assert(rv == OGS_OK);
}
