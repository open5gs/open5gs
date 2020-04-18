/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "mme-event.h"
#include "mme-sm.h"
#include "mme-context.h"

#include "s1ap-path.h"
#include "mme-gtp-path.h"
#include "nas-path.h"
#include "mme-fd-path.h"
#include "sgsap-path.h"
#include "mme-path.h"

#include "mme-s11-build.h"
#include "mme-s11-handler.h"

void mme_s11_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp_echo_request_t *req)
{
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[MME] Receiving Echo Request");
    /* FIXME : Before implementing recovery counter correctly,
     *         I'll re-use the recovery value in request message */
    ogs_gtp_send_echo_response(xact, req->recovery.u8, 0);
}

void mme_s11_handle_echo_response(
        ogs_gtp_xact_t *xact, ogs_gtp_echo_response_t *rsp)
{
    /* Not Implemented */
}

void mme_s11_handle_create_session_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp_create_session_response_t *rsp)
{
    int rv;
    uint8_t cause_value = 0;
    ogs_gtp_f_teid_t *sgw_s11_teid = NULL;
    ogs_gtp_f_teid_t *sgw_s1u_teid = NULL;

    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;
    ogs_pdn_t *pdn = NULL;
    
    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_debug("[MME] Create Session Response");

    if (!mme_ue) {
        ogs_warn("No Context in TEID");
        mme_ue = xact->data;
        ogs_assert(mme_ue);
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect_or_return(rv == OGS_OK);

    if (rsp->cause.presence) {
        ogs_gtp_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value == OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
            if (rsp->bearer_contexts_created.cause.presence) {
                cause = rsp->bearer_contexts_created.cause.data;
                ogs_assert(cause);

                cause_value = cause->value;
            } else {
                ogs_error("No Cause");
                cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
            }
        }
    } else {
        ogs_error("No Cause");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED)
        ogs_warn("Cause[%d] : No Accepted", cause_value);

    if (rsp->sender_f_teid_for_control_plane.presence == 0) {
        ogs_error("No S11 TEID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (rsp->pdn_address_allocation.presence == 0) {
        ogs_error("No PDN Address Allocation");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (rsp->bearer_contexts_created.s1_u_enodeb_f_teid.presence == 0) {
        ogs_error("No S1U TEID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (rsp->bearer_contexts_created.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (rsp->bearer_contexts_created.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_initial_context_setup)) {
            nas_send_attach_reject(mme_ue,
                EMM_CAUSE_NETWORK_FAILURE, ESM_CAUSE_NETWORK_FAILURE);
        }
        mme_send_delete_session_or_mme_ue_context_release(mme_ue);
        return;
    }

    bearer = mme_bearer_find_by_ue_ebi(mme_ue, 
            rsp->bearer_contexts_created.eps_bearer_id.u8);
    ogs_expect_or_return(bearer);
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
    if (rsp->protocol_configuration_options.presence) {
        OGS_TLV_STORE_DATA(&sess->pgw_pco,
                &rsp->protocol_configuration_options);
    }

    /* Data Plane(UL) : SGW-S1U */
    sgw_s1u_teid = rsp->bearer_contexts_created.s1_u_enodeb_f_teid.data;
    bearer->sgw_s1u_teid = ntohl(sgw_s1u_teid->teid);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);
    ogs_debug("    ENB_S1U_TEID[%d] SGW_S1U_TEID[%d]",
        bearer->enb_s1u_teid, bearer->sgw_s1u_teid);

    rv = ogs_gtp_f_teid_to_ip(sgw_s1u_teid, &bearer->sgw_s1u_ip);
    ogs_assert(rv == OGS_OK);

    if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_initial_context_setup)) {
        mme_csmap_t *csmap = mme_csmap_find_by_tai(&mme_ue->tai);
        mme_ue->csmap = csmap;

        if (csmap) {
            sgsap_send_location_update_request(mme_ue);
        } else {
            nas_send_attach_accept(mme_ue);
        }

    } else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_registered)) {
        nas_send_activate_default_bearer_context_request(bearer);
    } else
        ogs_assert_if_reached();
}

void mme_s11_handle_modify_bearer_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp_modify_bearer_response_t *rsp)
{
    int rv;
    uint8_t cause_value = 0;
    enb_ue_t *source_ue = NULL, *target_ue = NULL;

    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_debug("[MME] Modify Bearer Response");

    if (!mme_ue) {
        ogs_warn("No Context in TEID");
        mme_ue = xact->data;
        ogs_assert(mme_ue);
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect_or_return(rv == OGS_OK);

    if (rsp->cause.presence) {
        ogs_gtp_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED)
            ogs_warn("GTP Failed [CAUSE:%d]", cause_value);
    }

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        mme_send_delete_session_or_mme_ue_context_release(mme_ue);
        return;
    }

    GTP_COUNTER_CHECK(mme_ue, GTP_COUNTER_MODIFY_BEARER_BY_PATH_SWITCH,
        s1ap_send_path_switch_ack(mme_ue);
    );

    GTP_COUNTER_CHECK(mme_ue, GTP_COUNTER_MODIFY_BEARER_BY_HANDOVER_NOTIFY,
        target_ue = mme_ue->enb_ue;
        ogs_assert(target_ue);
        source_ue = target_ue->source_ue;
        ogs_assert(source_ue);

        s1ap_send_ue_context_release_command(source_ue,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_successful_handover,
                S1AP_UE_CTX_REL_DELETE_INDIRECT_TUNNEL,
                ogs_time_from_msec(300));
    );
}

void mme_s11_handle_delete_session_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp_delete_session_response_t *rsp)
{
    int rv;
    uint8_t cause_value = 0;
    mme_sess_t *sess = NULL;

    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_debug("[MME] Delete Session Response");

    sess = xact->data;
    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect_or_return(rv == OGS_OK);

    if (rsp->cause.presence) {
        ogs_gtp_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED)
            ogs_warn("GTP Failed [CAUSE:%d] - Ignored", cause_value);
    }

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_authentication)) {
        if (mme_sess_count(mme_ue) == 1) /* Last Session */ {
            mme_s6a_send_air(mme_ue, NULL);
        }
    } else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_de_registered)) {
        if (mme_sess_count(mme_ue) == 1) /* Last Session */ {
            nas_send_detach_accept(mme_ue);
        }
    } else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_registered)) {
        mme_bearer_t *bearer = mme_default_bearer_in_sess(sess);
        ogs_expect_or_return(bearer);

        if (OGS_FSM_CHECK(&bearer->sm, esm_state_pdn_will_disconnect)) {
            nas_send_deactivate_bearer_context_request(bearer);
            
            /*
             * mme_sess_remove() should not be called here.
             *
             * Session will be removed if Deactivate bearer context 
             * accept is received */
            CLEAR_SGW_S1U_PATH(sess);
            return;
        } else if (OGS_FSM_CHECK(&bearer->sm, esm_state_active) ||
            /*
             * MME sends InitialContextSetupRequest to eNB.
             * eNB sends InitialContextSetupFailure to MME.
             *
             * In this case, ESM state is INACTIVE.
             *
             * So, if Delete-Session-Response is received,
             * MME needs to send UEContextReleaseCommand to eNB. 
             */
                OGS_FSM_CHECK(&bearer->sm, esm_state_inactive)) {

            if (mme_sess_count(mme_ue) == 1) /* Last Session */ {
                enb_ue_t *enb_ue = NULL;

                enb_ue = mme_ue->enb_ue;
                if (enb_ue) {
                    s1ap_send_ue_context_release_command(enb_ue,
                        S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                        S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
                } else
                    ogs_warn("ENB-S1 Context has already been removed");
            }
        } else
            ogs_assert_if_reached();

    } else if (OGS_FSM_CHECK(&mme_ue->sm, emm_state_initial_context_setup) ||
             OGS_FSM_CHECK(&mme_ue->sm, emm_state_exception)) {
        if (mme_sess_count(mme_ue) == 1) /* Last Session */ {
            enb_ue_t *enb_ue = NULL;

            enb_ue = mme_ue->enb_ue;
            if (enb_ue) {
                s1ap_send_ue_context_release_command(enb_ue,
                    S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                    S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
            } else
                ogs_warn("ENB-S1 Context has already been removed");
        }
    } else
        ogs_assert_if_reached();

    if (mme_sess_count(mme_ue) == 1) /* Last Session */
        CLEAR_SESSION_CONTEXT(mme_ue);

    mme_sess_remove(sess);
}

void mme_s11_handle_create_bearer_request(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp_create_bearer_request_t *req)
{
    int rv;
    uint8_t cause_value = 0;
    mme_bearer_t *bearer = NULL, *default_bearer = NULL;
    mme_sess_t *sess = NULL;

    ogs_gtp_f_teid_t *sgw_s1u_teid = NULL;
    ogs_gtp_bearer_qos_t bearer_qos;

    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[MME] Create Bearer Response");

    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    if (req->linked_eps_bearer_id.presence == 0) {
        ogs_error("No Linked EBI");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (mme_ue && cause_value == OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        sess = mme_sess_find_by_ebi(mme_ue, req->linked_eps_bearer_id.u8);
        ogs_assert(sess);

        bearer = mme_bearer_add(sess);
        ogs_assert(bearer);
    }

    if (!bearer) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (req->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.s1_u_enodeb_f_teid.presence == 0) {
        ogs_error("No GTP TEID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.bearer_level_qos.presence == 0) {
        ogs_error("No QoS");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.tft.presence == 0) {
        ogs_error("No TFT");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(xact, mme_ue ? mme_ue->sgw_s11_teid : 0,
                OGS_GTP_CREATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    /* Set PTI */
    sess = bearer->sess;
    ogs_assert(sess);
    sess->pti = OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED;
    if (req->procedure_transaction_id.presence) {
        sess->pti = req->procedure_transaction_id.u8;
        ogs_debug("    PTI[%d]", sess->pti);
    }

    /* Data Plane(UL) : SGW-S1U */
    sgw_s1u_teid = req->bearer_contexts.s1_u_enodeb_f_teid.data;
    bearer->sgw_s1u_teid = ntohl(sgw_s1u_teid->teid);
    rv = ogs_gtp_f_teid_to_ip(sgw_s1u_teid, &bearer->sgw_s1u_ip);
    ogs_assert(rv == OGS_OK);

    /* Bearer QoS */
    ogs_expect_or_return(ogs_gtp_parse_bearer_qos(&bearer_qos,
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
    OGS_TLV_STORE_DATA(&bearer->tft, &req->bearer_contexts.tft);

    /* Save Transaction. will be handled after EMM-attached */
    bearer->xact = xact;

    /* Before Activate DEDICATED bearer, we'll check DEFAULT bearer status */
    default_bearer = mme_default_bearer_in_sess(sess);
    ogs_expect_or_return(default_bearer);

    if (/* Check if Activate Default Bearer Accept is received */
        OGS_FSM_CHECK(&default_bearer->sm, esm_state_active) &&
        /* Check if Initial Context Setup Response or 
         *          E-RAB Setup Response is received */
        MME_HAVE_ENB_S1U_PATH(default_bearer)) {
        nas_send_activate_dedicated_bearer_context_request(bearer);
    }
}

void mme_s11_handle_update_bearer_request(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp_update_bearer_request_t *req)
{
    uint8_t cause_value = 0;
    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;
    ogs_gtp_bearer_qos_t bearer_qos;

    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[MME] Update Bearer Request");

    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    if (req->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (mme_ue && cause_value == OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        bearer = mme_bearer_find_by_ue_ebi(mme_ue,
                req->bearer_contexts.eps_bearer_id.u8);
        ogs_expect_or_return(bearer);
    }

    if (!bearer) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(xact, mme_ue ? mme_ue->sgw_s11_teid : 0,
                OGS_GTP_UPDATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    /* Set PTI */
    sess = bearer->sess;
    ogs_assert(sess);
    sess->pti = OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED;
    if (req->procedure_transaction_id.presence) {
        sess->pti = req->procedure_transaction_id.u8;
        ogs_debug("    PTI[%d]", sess->pti);
    }

    /* Save Transaction. will be handled after EMM-attached */
    bearer->xact = xact;

    if (/* Check if Activate Default/Dedicated Bearer Accept is received */
        OGS_FSM_CHECK(&bearer->sm, esm_state_active) &&
        /* Check if Initial Context Setup Response or 
         *          E-RAB Setup Response is received */
        MME_HAVE_ENB_S1U_PATH(bearer)) {
        if (req->bearer_contexts.bearer_level_qos.presence == 1) {
            /* Bearer QoS */
            ogs_expect_or_return(ogs_gtp_parse_bearer_qos(&bearer_qos,
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

        if (req->bearer_contexts.tft.presence == 1) {
            /* Save Bearer TFT */
            OGS_TLV_STORE_DATA(&bearer->tft, &req->bearer_contexts.tft);
        }

        if (req->bearer_contexts.bearer_level_qos.presence == 1 ||
            req->bearer_contexts.tft.presence == 1) {
            nas_send_modify_bearer_context_request(bearer, 
                    req->bearer_contexts.bearer_level_qos.presence,
                    req->bearer_contexts.tft.presence);
        } else {
            ogs_warn("[IGNORE] Update Bearer Request : "
                    "Both QoS and TFT is NULL");

            if (xact->xid & OGS_GTP_CMD_XACT_ID) {
                /* MME recieved Bearer resource modification request */
                nas_send_bearer_resource_modification_reject(
                        bearer, ESM_CAUSE_SERVICE_OPTION_NOT_SUPPORTED);
            }

            mme_gtp_send_update_bearer_response(bearer);
        }
    } else {
        if (!OGS_FSM_CHECK(&bearer->sm, esm_state_active)) {
            ogs_assert_if_reached();
        } else if (!MME_HAVE_ENB_S1U_PATH(bearer)) {
            ogs_assert_if_reached();
        } else
            ogs_assert_if_reached();
    }
}

void mme_s11_handle_delete_bearer_request(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp_delete_bearer_request_t *req)
{
    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;

    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("[MME] Delete Bearer Request");

    if (mme_ue && req->linked_eps_bearer_id.presence == 1) {
        bearer = mme_bearer_find_by_ue_ebi(
                mme_ue, req->linked_eps_bearer_id.u8);
        ogs_expect_or_return(bearer);
    } else if (mme_ue && req->eps_bearer_ids.presence == 1) {
        bearer = mme_bearer_find_by_ue_ebi(
                mme_ue, req->eps_bearer_ids.u8);
        ogs_expect_or_return(bearer);
    } else {
        ogs_error("No Linked EBI or EPS Bearer ID");
        ogs_gtp_send_error_message(xact, mme_ue ? mme_ue->sgw_s11_teid : 0,
                OGS_GTP_DELETE_BEARER_RESPONSE_TYPE,
                OGS_GTP_CAUSE_CONTEXT_NOT_FOUND);
        return;
    }

    ogs_assert(mme_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    /* Set PTI */
    sess = bearer->sess;
    ogs_assert(sess);
    sess->pti = OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED;
    if (req->procedure_transaction_id.presence) {
        sess->pti = req->procedure_transaction_id.u8;
        ogs_debug("    PTI[%d]", sess->pti);
    }

    /* Save Transaction. will be handled after EMM-attached */
    ogs_assert(bearer);
    bearer->xact = xact;

    if (/* Check if Activate Default/Dedicated Bearer Accept is received */
        OGS_FSM_CHECK(&bearer->sm, esm_state_active) &&
        /* Check if Initial Context Setup Response or 
         *          E-RAB Setup Response is received */
        MME_HAVE_ENB_S1U_PATH(bearer)) {
        nas_send_deactivate_bearer_context_request(bearer);
    } else {
        if (!OGS_FSM_CHECK(&bearer->sm, esm_state_active)) {
            ogs_assert_if_reached();
        } else if (!MME_HAVE_ENB_S1U_PATH(bearer)) {
            ogs_assert_if_reached();
        } else
            ogs_assert_if_reached();
    }

}

void mme_s11_handle_release_access_bearers_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp_release_access_bearers_response_t *rsp)
{
    int rv;
    uint8_t cause_value = 0;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_debug("[MME] Release Access Bearers Response");

    if (!mme_ue) {
        ogs_warn("No Context in TEID");
        mme_ue = xact->data;
        ogs_assert(mme_ue);
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect_or_return(rv == OGS_OK);

    if (rsp->cause.presence) {
        ogs_gtp_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED)
            ogs_warn("GTP Failed [CAUSE:%d]", cause_value);
    }

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        mme_send_delete_session_or_mme_ue_context_release(mme_ue);
        return;
    }

    rv = CLEAR_BEARER_CONTEXT(mme_ue);
    ogs_expect(rv == OGS_OK);

    enb_ue = mme_ue->enb_ue;
    if (enb_ue) {
        s1ap_send_ue_context_release_command(enb_ue,
                S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                S1AP_UE_CTX_REL_S1_REMOVE_AND_UNLINK, 0);
    } else {
        ogs_warn("ENB-S1 Context has already been removed");
    }
}

void mme_s11_handle_downlink_data_notification(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp_downlink_data_notification_t *noti)
{
    int rv;
    ogs_gtp_cause_t cause;
    ogs_gtp_header_t h;
    ogs_pkbuf_t *s11buf = NULL;

    ogs_assert(xact);
    ogs_assert(noti);

    ogs_debug("[MME] Downlink Data Notification");

    memset(&cause, 0, sizeof(cause));
    cause.value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    if (!mme_ue) {
        ogs_warn("No Context");
        cause.value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause.value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(xact, mme_ue ? mme_ue->sgw_s11_teid : 0,
                OGS_GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE,
                cause.value);
        return;
    }

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    /* Build Downlink data notification ack */
    memset(&h, 0, sizeof(ogs_gtp_header_t));
    h.type = OGS_GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE;
    h.teid = mme_ue->sgw_s11_teid;

    s11buf = mme_s11_build_downlink_data_notification_ack(h.type);
    ogs_expect_or_return(s11buf);

    rv = ogs_gtp_xact_update_tx(xact, &h, s11buf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void mme_s11_handle_create_indirect_data_forwarding_tunnel_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp_create_indirect_data_forwarding_tunnel_response_t *rsp)
{
    int rv;
    uint8_t cause_value = 0;
    mme_bearer_t *bearer = NULL;
    enb_ue_t *source_ue = NULL;
    int i;

    ogs_gtp_tlv_bearer_context_t *bearers[OGS_GTP_MAX_INDIRECT_TUNNEL];
    ogs_gtp_f_teid_t *teid = NULL;

    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_debug("[MME] Create Indirect Data Forwarding Tunnel Response");

    if (!mme_ue) {
        ogs_warn("No Context in TEID");
        mme_ue = xact->data;
        ogs_assert(mme_ue);
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect_or_return(rv == OGS_OK);

    if (rsp->cause.presence) {
        ogs_gtp_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED)
            ogs_warn("GTP Failed [CAUSE:%d]", cause_value);
    }

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        mme_send_delete_session_or_mme_ue_context_release(mme_ue);
        return;
    }

    ogs_gtp_bearers_in_create_indirect_tunnel_response(&bearers, rsp);

    for (i = 0; bearers[i]->presence; i++) {
        if (bearers[i]->eps_bearer_id.presence == 0) {
            ogs_error("No EBI");
            return;
        }

        bearer = mme_bearer_find_by_ue_ebi(mme_ue, 
                    bearers[i]->eps_bearer_id.u8);
        ogs_expect_or_return(bearer);

        if (bearers[i]->s4_u_sgsn_f_teid.presence) {
            teid = bearers[i]->s4_u_sgsn_f_teid.data;
            ogs_assert(teid);

            bearer->sgw_dl_teid = ntohl(teid->teid);
            rv = ogs_gtp_f_teid_to_ip(teid, &bearer->sgw_dl_ip);
            ogs_expect_or_return(rv == OGS_OK);
        }
        if (bearers[i]->s2b_u_epdg_f_teid_5.presence) {
            teid = bearers[i]->s2b_u_epdg_f_teid_5.data;
            ogs_assert(teid);

            bearer->sgw_ul_teid = ntohl(teid->teid);
            rv = ogs_gtp_f_teid_to_ip(teid, &bearer->sgw_ul_ip);
            ogs_expect_or_return(rv == OGS_OK);
        }
    }

    source_ue = mme_ue->enb_ue;
    ogs_assert(source_ue);

    s1ap_send_handover_command(source_ue);
}

void mme_s11_handle_delete_indirect_data_forwarding_tunnel_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp_delete_indirect_data_forwarding_tunnel_response_t *rsp)
{
    int rv;
    uint8_t cause_value = 0;

    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_debug("[MME] Delete Indirect Data Forwarding Tunnel Response");

    if (!mme_ue) {
        ogs_warn("No Context in TEID");
        mme_ue = xact->data;
        ogs_assert(mme_ue);
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect_or_return(rv == OGS_OK);

    if (rsp->cause.presence) {
        ogs_gtp_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED)
            ogs_warn("GTP Failed [CAUSE:%d]", cause_value);
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        mme_send_delete_session_or_mme_ue_context_release(mme_ue);
        return;
    }

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    rv = mme_ue_clear_indirect_tunnel(mme_ue);
    ogs_expect(rv == OGS_OK);
}

void mme_s11_handle_bearer_resource_failure_indication(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp_bearer_resource_failure_indication_t *ind)
{
    int rv;
    uint8_t cause_value = 0;

    mme_bearer_t *bearer = NULL;

    ogs_assert(xact);
    bearer = xact->data;
    ogs_assert(ind);

    ogs_debug("[MME] Bearer Resource Failure Indication");

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect_or_return(rv == OGS_OK);

    if (ind->cause.presence) {
        ogs_gtp_cause_t *cause = ind->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED)
            ogs_warn("GTP Failed [CAUSE:%d] - Ignored", cause_value);
    } else {
        ogs_error("No Cause");
    }

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, mme_ue->sgw_s11_teid);

    nas_send_bearer_resource_modification_reject(
            bearer, ESM_CAUSE_SERVICE_OPTION_NOT_SUPPORTED);
}
