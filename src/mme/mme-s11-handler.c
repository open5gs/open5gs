/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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
#include "mme-timer.h"

#include "s1ap-path.h"
#include "mme-gtp-path.h"
#include "nas-path.h"
#include "mme-fd-path.h"
#include "sgsap-path.h"
#include "mme-path.h"

#include "mme-s11-build.h"
#include "mme-s11-handler.h"

static uint8_t esm_cause_from_gtp(uint8_t gtp_cause)
{
    switch (gtp_cause) {
    case OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND:
        return OGS_NAS_ESM_CAUSE_INVALID_EPS_BEARER_IDENTITY;
    case OGS_GTP2_CAUSE_SERVICE_NOT_SUPPORTED:
        return OGS_NAS_ESM_CAUSE_SERVICE_OPTION_NOT_SUPPORTED;
    case OGS_GTP2_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION:
        return OGS_NAS_ESM_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION;
    case OGS_GTP2_CAUSE_SYNTACTIC_ERROR_IN_THE_TFT_OPERATION:
        return OGS_NAS_ESM_CAUSE_SYNTACTICAL_ERROR_IN_THE_TFT_OPERATION;
    case OGS_GTP2_CAUSE_SYNTACTIC_ERRORS_IN_PACKET_FILTER:
        return OGS_NAS_ESM_CAUSE_SYNTACTICAL_ERROR_IN_PACKET_FILTERS;
    case OGS_GTP2_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTER:
        return OGS_NAS_ESM_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTERS;
    default:
        break;
    }

    /*
     * OGS_GTP2_CAUSE_SYSTEM_FAILURE
     * OGS_GTP2_CAUSE_MANDATORY_IE_MISSING
     * ...
     */
    return OGS_NAS_ESM_CAUSE_NETWORK_FAILURE;
}

static void gtp_remote_peer_timeout(ogs_gtp_xact_t *xact, void *data)
{
    char buf[OGS_ADDRSTRLEN];
    mme_bearer_t *bearer = NULL;
    uint8_t type;

    ogs_assert(xact);
    type = xact->seq[xact->step-1].type;

    ogs_error("[%d] %s Peer Timeout for step %d type %d peer [%s]:%d",
            xact->xid,
            xact->org == OGS_GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            xact->step, type,
            OGS_ADDR(&xact->gnode->addr, buf),
            OGS_PORT(&xact->gnode->addr));

    ogs_assert(data);
    bearer = mme_bearer_find_by_id(OGS_POINTER_TO_UINT(data));
    if (!bearer) {
        ogs_error("Bearer has already been removed [%d]", type);
        return;
    }

    /*
     * Issues #3240
     *
     * SMF->SGW-C->MME: First Update Bearer Request
     * MME->UE:         First Modify EPS bearer context request
     * SMF->SGW-C->MME: Second Update Bearer Request
     * MME->UE:         Second Modify EPS bearer context request
     * UE->MME:         First Modify EPS bearer context accept
     * MME->SGW-C->SMF: First Update Bearer Response
     * UE->MME:         Second Modify EPS bearer context accept
     * MME->SGW-C->SMF: Second Update Bearer Response
     */
    switch (type) {
    case OGS_GTP2_UPDATE_BEARER_REQUEST_TYPE:
        /*
         * In this case, a timeout occurs while waiting
         * for Modify EPS bearer context accept from UE.
         *
         * If the UE does not send a Modify EPS bearer context accept,
         * the MME fails to send an Update Bearer Response.
         *
         * Therefore, we need to delete the Transaction Node
         * that was managed by the Bearer Context from the List.
         */
        if (ogs_list_exists(
                    &bearer->update.xact_list,
                    &xact->to_update_node) == true) {
            ogs_error("Bearer-ID [%d] removed from the list", bearer->id);
            ogs_list_remove(&bearer->update.xact_list, &xact->to_update_node);
        } else {
            ogs_error("[%d] %s HAVE ALREADY BEEN REMOVED "
                    "for step %d type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == OGS_GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, type,
                    OGS_ADDR(&xact->gnode->addr, buf),
                    OGS_PORT(&xact->gnode->addr));
        }
        break;
#if 0
    case OGS_GTP2_UPDATE_BEARER_RESPONSE_TYPE:
        /*
         * The following is the case where the UE sends
         * Modify EPS bearer context accept to the MME.
         *
         * In this case, the MME sends Update Bearer Response
         * to SGW-C and deletes the Transaction Node.
         *
         * Therefore, there is no need to delete the Transaction Node
         * from the list managed by the Bearer Context here.
         */
        if (ogs_list_exists(
                    &bearer->update.xact_list,
                    &xact->to_update_node) == true) {
            ogs_error("[%d] %s SHOULD HAVE REMOVED "
                    "for step %d type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == OGS_GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, type,
                    OGS_ADDR(&xact->gnode->addr, buf),
                    OGS_PORT(&xact->gnode->addr));
        } else {
            ogs_error("Not existed");
        }
        break;
#endif
    default:
        ogs_fatal("Unknown type[%d]", type);
        ogs_assert_if_reached();
        break;
    }
}

void mme_s11_handle_echo_request(
        ogs_gtp_xact_t *xact, ogs_gtp2_echo_request_t *req)
{
    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("Receiving Echo Request");
    /* FIXME : Before implementing recovery counter correctly,
     *         I'll re-use the recovery value in request message */
    ogs_gtp2_send_echo_response(xact, req->recovery.u8, 0);
}

void mme_s11_handle_echo_response(
        ogs_gtp_xact_t *xact, ogs_gtp2_echo_response_t *rsp)
{
    /* Not Implemented */
}

void mme_s11_handle_create_session_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue_from_teid,
        ogs_gtp2_create_session_response_t *rsp)
{
    int i, r, rv;
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    uint8_t session_cause = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    uint8_t bearer_cause = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    ogs_gtp2_f_teid_t *sgw_s11_teid = NULL;
    ogs_gtp2_f_teid_t *pgw_s5c_teid = NULL;
    ogs_gtp2_f_teid_t *sgw_s1u_teid = NULL;
    ogs_gtp2_f_teid_t *pgw_s5u_teid = NULL;

    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;
    mme_ue_t *mme_ue = NULL;
    sgw_ue_t *source_ue = NULL, *target_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    ogs_session_t *session = NULL;
    ogs_gtp2_bearer_qos_t bearer_qos;
    ogs_gtp2_ambr_t *ambr = NULL;
    uint16_t decoded = 0;
    int create_action = 0;

    ogs_assert(rsp);

    ogs_debug("Create Session Response");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(xact);
    create_action = xact->create_action;
    sess = mme_sess_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    if (sess)
        mme_ue = mme_ue_find_by_id(sess->mme_ue_id);
    enb_ue = enb_ue_find_by_id(xact->enb_ue_id);

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_commit() failed");
        return;
    }

    if (!sess) {
        ogs_error("Session Context has already been removed");
        return;
    }

    if (!enb_ue) {
        ogs_error("ENB-S1 Context has already been removed");
        return;
    }

    if (!mme_ue) {
        ogs_error("MME-UE Context has already been removed");
        return;
    }
    source_ue = sgw_ue_find_by_id(mme_ue->sgw_ue_id);
    ogs_assert(source_ue);

    if (create_action == OGS_GTP_CREATE_IN_PATH_SWITCH_REQUEST) {
        target_ue = sgw_ue_find_by_id(source_ue->target_ue_id);
        ogs_assert(target_ue);
    } else {
        target_ue = source_ue;
        ogs_assert(target_ue);
    }

    /************************
     * Getting Cause Value
     ************************/
    if (rsp->cause.presence && rsp->cause.data) {
        ogs_gtp2_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);
        session_cause = cause->value;
    }

    /************************
     * Check MME-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!mme_ue_from_teid) {
        ogs_error("[%s] No Context in TEID [Cause:%d]",
                mme_ue->imsi_bcd, session_cause);
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        if (create_action == OGS_GTP_CREATE_IN_ATTACH_REQUEST) {
            ogs_error("[%s] Attach reject [Cause:%d]",
                    mme_ue->imsi_bcd, session_cause);
            r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_NETWORK_FAILURE,
                    OGS_NAS_ESM_CAUSE_NETWORK_FAILURE);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
        mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
        return;
    }

    /*****************************************
     * Check Mandatory/Conditional IE Missing
     *****************************************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (rsp->sender_f_teid_for_control_plane.presence == 0) {
        ogs_error("[%s] No S11 TEID [Cause:%d]",
                mme_ue->imsi_bcd, session_cause);
        cause_value = OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
    }

    switch (create_action) {
    case OGS_GTP_CREATE_IN_PATH_SWITCH_REQUEST:
        /* No need for PAA or S5C TEID in PathSwitchRequest */
        break;
    case OGS_GTP_CREATE_IN_TRACKING_AREA_UPDATE:
        /* No need for PAA or S5C TEID in 2G->4G mobility, it was already provided by SGSN peer */
        break;
    default:
        if (rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.presence == 0) {
            ogs_error("[%s] No S5C TEID [Cause:%d]",
                    mme_ue->imsi_bcd, session_cause);
            cause_value = OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
        }

        if (rsp->pdn_address_allocation.presence == 0) {
            ogs_error("[%s] No PDN Address Allocation [Cause:%d]",
                    mme_ue->imsi_bcd, session_cause);
            cause_value = OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
        }
    }

    if (rsp->cause.presence == 0) {
        ogs_error("[%s] No Cause [VALUE:%d]", mme_ue->imsi_bcd, session_cause);
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        if (create_action == OGS_GTP_CREATE_IN_ATTACH_REQUEST) {
            ogs_error("[%s] Attach reject [Cause:%d]",
                    mme_ue->imsi_bcd, session_cause);
            r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_NETWORK_FAILURE,
                    OGS_NAS_ESM_CAUSE_NETWORK_FAILURE);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        } else if (create_action == OGS_GTP_CREATE_IN_TRACKING_AREA_UPDATE) {
            ogs_error("[%s] TAU reject [Cause:%d]",
                    mme_ue->imsi_bcd, session_cause);
            r = nas_eps_send_tau_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_NETWORK_FAILURE);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
        mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
        return;
    }

    /********************
     * Check Cause Value
     ********************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    for (i = 0; i < OGS_BEARER_PER_UE; i++) {
        ogs_gtp2_cause_t *cause = NULL;

        if (rsp->bearer_contexts_created[i].cause.presence == 0) {
            break;
        }
        cause = rsp->bearer_contexts_created[i].cause.data;
        if (cause == NULL) {
            ogs_error("No Cause Data");
            continue;
        }
        bearer_cause = cause->value;
        if (bearer_cause != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
            ogs_error("[%s] GTP Bearer Cause [VALUE:%d]",
                    mme_ue->imsi_bcd, bearer_cause);
            if (create_action == OGS_GTP_CREATE_IN_ATTACH_REQUEST) {
                ogs_error("[%s] Attach reject", mme_ue->imsi_bcd);
                r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                        OGS_NAS_EMM_CAUSE_NETWORK_FAILURE,
                        OGS_NAS_ESM_CAUSE_NETWORK_FAILURE);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
            mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
            return;
        }
    }

    if (session_cause != OGS_GTP2_CAUSE_REQUEST_ACCEPTED &&
        session_cause != OGS_GTP2_CAUSE_REQUEST_ACCEPTED_PARTIALLY &&
        session_cause !=
            OGS_GTP2_CAUSE_NEW_PDN_TYPE_DUE_TO_NETWORK_PREFERENCE &&
        session_cause !=
            OGS_GTP2_CAUSE_NEW_PDN_TYPE_DUE_TO_SINGLE_ADDRESS_BEARER_ONLY) {
        ogs_error("[%s] GTP Cause [VALUE:%d]", mme_ue->imsi_bcd, session_cause);
        if (create_action == OGS_GTP_CREATE_IN_ATTACH_REQUEST) {
            ogs_error("[%s] Attach reject", mme_ue->imsi_bcd);
            r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                    OGS_NAS_EMM_CAUSE_NETWORK_FAILURE,
                    OGS_NAS_ESM_CAUSE_NETWORK_FAILURE);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
        mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(sess);
    session = sess->session;
    ogs_assert(session);

    ogs_assert(mme_ue);
    ogs_assert(source_ue);
    ogs_assert(target_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, target_ue->sgw_s11_teid);

    for (i = 0; i < OGS_BEARER_PER_UE; i++) {
        if (rsp->bearer_contexts_created[i].presence == 0) {
            break;
        }
        if (rsp->bearer_contexts_created[i].eps_bearer_id.presence == 0) {
            ogs_error("No EPS Bearer ID");
            break;
        }
        if (rsp->bearer_contexts_created[i].s1_u_enodeb_f_teid.presence == 0) {
            ogs_error("No SGW-S1U TEID");
            break;
        }
        if (rsp->bearer_contexts_created[i].s5_s8_u_sgw_f_teid.presence == 0) {
            ogs_error("No PGW-S5U TEID");
            break;
        }

        /* EPS Bearer ID */
        bearer = mme_bearer_find_by_ue_ebi(mme_ue,
                rsp->bearer_contexts_created[i].eps_bearer_id.u8);
        if (!bearer) {
            mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
            return;
        }

        /* Data Plane(UL) : SGW-S1U */
        sgw_s1u_teid = rsp->bearer_contexts_created[i].s1_u_enodeb_f_teid.data;
        bearer->sgw_s1u_teid = be32toh(sgw_s1u_teid->teid);
        ogs_assert(OGS_OK ==
                ogs_gtp2_f_teid_to_ip(sgw_s1u_teid, &bearer->sgw_s1u_ip));

        /* Data Plane(UL) : PGW-S5U */
        pgw_s5u_teid = rsp->bearer_contexts_created[i].s5_s8_u_sgw_f_teid.data;
        bearer->pgw_s5u_teid = be32toh(pgw_s5u_teid->teid);
        ogs_assert(OGS_OK ==
                ogs_gtp2_f_teid_to_ip(pgw_s5u_teid, &bearer->pgw_s5u_ip));

        ogs_debug("    ENB_S1U_TEID[%d] SGW_S1U_TEID[%d] PGW_S5U_TEID[%d]",
            bearer->enb_s1u_teid, bearer->sgw_s1u_teid, bearer->pgw_s5u_teid);

        if (create_action == OGS_GTP_CREATE_IN_TRACKING_AREA_UPDATE &&
            !OGS_FSM_CHECK(&bearer->sm, esm_state_active))
            OGS_FSM_TRAN(&bearer->sm, esm_state_active);
    }

    /* Bearer Level QoS */
    if (rsp->bearer_contexts_created[0].bearer_level_qos.presence) {
        decoded = ogs_gtp2_parse_bearer_qos(&bearer_qos,
            &rsp->bearer_contexts_created[0].bearer_level_qos);
        ogs_assert(decoded ==
                rsp->bearer_contexts_created[0].bearer_level_qos.len);
        session->qos.index = bearer_qos.qci;
        session->qos.arp.priority_level = bearer_qos.priority_level;
        session->qos.arp.pre_emption_capability =
                        bearer_qos.pre_emption_capability;
        session->qos.arp.pre_emption_vulnerability =
                        bearer_qos.pre_emption_vulnerability;
    }

    /* Control Plane(UL) : SGW-S11 */
    sgw_s11_teid = rsp->sender_f_teid_for_control_plane.data;
    target_ue->sgw_s11_teid = be32toh(sgw_s11_teid->teid);

    /* Control Plane(UL) : PGW-S5C */
    if (rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.presence) {
        pgw_s5c_teid = rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.data;
        sess->pgw_s5c_teid = be32toh(pgw_s5c_teid->teid);
        ogs_assert(OGS_OK ==
                ogs_gtp2_f_teid_to_ip(pgw_s5c_teid, &sess->pgw_s5c_ip));
    }

    /* PDN Addresss Allocation */
    if (rsp->pdn_address_allocation.presence) {
        memcpy(&sess->paa, rsp->pdn_address_allocation.data,
                rsp->pdn_address_allocation.len);
        /*
         * Issue #3209
         *
         * The Session-Type in the Subscriber DB should not be changed
         * in case the UE can change the PDN-Type for the APN.
         * (e.g IPv4v6 -> IPv4 -> IPv4v6)
         *
         * For resolving this problem,
         * session->session_type and session->ue_ip should not be modified.
         *
         * Therefore, the code below will be deleted.
         */
#if 0 /* WILL BE DELETED */
        session->session_type = sess->paa.session_type;
        ogs_assert(OGS_OK == ogs_paa_to_ip(&sess->paa, &session->ue_ip));
#endif
    }

    /* ePCO */
    if (rsp->extended_protocol_configuration_options.presence) {
        OGS_TLV_STORE_DATA(&sess->pgw_epco,
                &rsp->extended_protocol_configuration_options);

    /* PCO */
    } else if (rsp->protocol_configuration_options.presence) {
        OGS_TLV_STORE_DATA(&sess->pgw_pco,
                &rsp->protocol_configuration_options);
    }

    /* Bearer QoS */
    if (rsp->bearer_contexts_created[0].bearer_level_qos.presence) {
        decoded = ogs_gtp2_parse_bearer_qos(&bearer_qos,
            &rsp->bearer_contexts_created[0].bearer_level_qos);
        ogs_assert(rsp->bearer_contexts_created[0].bearer_level_qos.len ==
                decoded);
        session->qos.index = bearer_qos.qci;
        session->qos.arp.priority_level = bearer_qos.priority_level;
        session->qos.arp.pre_emption_capability =
                        bearer_qos.pre_emption_capability;
        session->qos.arp.pre_emption_vulnerability =
                        bearer_qos.pre_emption_vulnerability;
    }

    /* AMBR */
    if (rsp->aggregate_maximum_bit_rate.presence) {
        ambr = rsp->aggregate_maximum_bit_rate.data;
        session->ambr.downlink = be32toh(ambr->downlink) * 1000;
        session->ambr.uplink = be32toh(ambr->uplink) * 1000;
    }

    if (create_action == OGS_GTP_CREATE_IN_ATTACH_REQUEST) {
        mme_csmap_t *csmap = mme_csmap_find_by_tai(&mme_ue->tai);
        mme_ue->csmap = csmap;

        if (!csmap ||
            mme_ue->network_access_mode ==
                OGS_NETWORK_ACCESS_MODE_ONLY_PACKET ||
            mme_ue->nas_eps.attach.value ==
                OGS_NAS_ATTACH_TYPE_EPS_ATTACH) {
            r = nas_eps_send_attach_accept(mme_ue);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        } else {
            ogs_assert(OGS_OK == sgsap_send_location_update_request(mme_ue));
        }

    } else if (create_action == OGS_GTP_CREATE_IN_TRACKING_AREA_UPDATE) {
        /* 3GPP TS 23.401 D.3.6 step 13, 14: */
        mme_s6a_send_ulr(enb_ue, mme_ue);
    } else if (create_action == OGS_GTP_CREATE_IN_UPLINK_NAS_TRANSPORT) {
        r = nas_eps_send_activate_default_bearer_context_request(
                bearer, create_action);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    } else if (create_action == OGS_GTP_CREATE_IN_PATH_SWITCH_REQUEST) {

        GTP_COUNTER_CHECK(mme_ue, GTP_COUNTER_CREATE_SESSION_BY_PATH_SWITCH,
            ogs_timer_start(source_ue->t_s11_holding,
                    mme_timer_cfg(MME_TIMER_S11_HOLDING)->duration);

            sgw_ue_associate_mme_ue(target_ue, mme_ue);
            r = s1ap_send_path_switch_ack(mme_ue, true);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        );

    } else {
        ogs_fatal("Invalid Create Session Action[%d]", create_action);
        ogs_assert_if_reached();
    }
}

void mme_s11_handle_modify_bearer_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue_from_teid,
        ogs_gtp2_modify_bearer_response_t *rsp)
{
    int r, rv;
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    uint8_t session_cause = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    int modify_action = 0;

    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(rsp);

    ogs_debug("Modify Bearer Response");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(xact);
    modify_action = xact->modify_action;

    mme_ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    enb_ue = enb_ue_find_by_id(xact->enb_ue_id);

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_commit() failed");
        return;
    }

    if (!mme_ue) {
        ogs_error("MME-UE Context has already been removed");
        return;
    }
    sgw_ue = sgw_ue_find_by_id(mme_ue->sgw_ue_id);
    ogs_assert(sgw_ue);

    /************************
     * Getting Cause Value
     ************************/
    if (rsp->cause.presence && rsp->cause.data) {
        ogs_gtp2_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);
        session_cause = cause->value;
    }

    /************************
     * Check MME-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!mme_ue_from_teid) {
        ogs_error("[%s] No Context in TEID [Cause:%d]",
                mme_ue->imsi_bcd, session_cause);
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        if (enb_ue)
            mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
        else
            ogs_error("ENB-S1 Context has already been removed");
        return;
    }

    /*****************************************
     * Check Mandatory/Conditional IE Missing
     *****************************************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (rsp->cause.presence == 0) {
        ogs_error("[%s] No Cause [%d]", mme_ue->imsi_bcd, session_cause);
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        if (enb_ue)
            mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
        else
            ogs_error("ENB-S1 Context has already been removed");
        return;
    }

    /********************
     * Check Cause Value
     ********************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (session_cause != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_error("[%s] GTP Cause [VALUE:%d]", mme_ue->imsi_bcd, session_cause);
        if (enb_ue)
            mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
        else
            ogs_error("ENB-S1 Context has already been removed");
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(mme_ue);
    ogs_assert(sgw_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    switch (modify_action) {
    case OGS_GTP_MODIFY_IN_PATH_SWITCH_REQUEST:
        r = s1ap_send_path_switch_ack(mme_ue, false);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        break;
    case OGS_GTP_MODIFY_IN_E_RAB_MODIFICATION:
        r = s1ap_send_e_rab_modification_confirm(mme_ue);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        break;
    default:
        break;
    }
}

void mme_s11_handle_delete_session_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue_from_teid,
        ogs_gtp2_delete_session_response_t *rsp)
{
    int r, rv;
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    int action = 0;
    sgw_ue_t *source_ue = NULL, *target_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(rsp);

    ogs_debug("Delete Session Response");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(xact);
    action = xact->delete_action;
    ogs_assert(action);
    sess = mme_sess_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    if (sess)
        mme_ue = mme_ue_find_by_id(sess->mme_ue_id);
    enb_ue = enb_ue_find_by_id(xact->enb_ue_id);

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_commit() failed");
        return;
    }

    if (!sess) {
        ogs_error("Session Context has already been removed");
        return;
    }

    if (!enb_ue) {
        ogs_error("ENB-S1 Context has already been removed");
        return;
    }

    if (!mme_ue) {
        ogs_error("MME-UE Context has already been removed");
        return;
    }
    target_ue = sgw_ue_find_by_id(mme_ue->sgw_ue_id);
    ogs_assert(target_ue);

    if (action == OGS_GTP_DELETE_IN_PATH_SWITCH_REQUEST) {
        source_ue = sgw_ue_find_by_id(target_ue->source_ue_id);
        if (!source_ue) /* InterRAT to 2G/3G (SGSN) case: */
             source_ue = target_ue;
        ogs_assert(source_ue);
    } else {
        source_ue = target_ue;
        ogs_assert(source_ue);
    }

    /************************
     * Check MME-UE Context
     ************************/
    if (!mme_ue_from_teid) {
        /*
         * In mme_ue_set_imsi(),
         * OLD MME-UE Context could be switched to NEW MME-UE Context
         *
         * OLD/NEW MME-UE Contexts have different MME-S11-TEID.
         * Since MME has NEW MME-S11-TEID and SGW-C has OLD MME-S11-TEID.
         *
         * At this time, if the MME receives OLD MME-S11-TEID from SGW-C
         * in the Delete Session Response, MME cannot find MME-UE Context.
         *
         * Since there is such a case,
         * Delete Session Response treats this as a WARNING.
         */
        ogs_warn("No Context in TEID");
    }

    /********************
     * Check Cause Value
     ********************/
    if (rsp->cause.presence) {
        ogs_gtp2_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED)
            ogs_error("GTP Cause [VALUE:%d] - Ignored", cause_value);
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(sess);
    ogs_assert(mme_ue);
    ogs_assert(target_ue);
    ogs_assert(source_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, source_ue->sgw_s11_teid);

    if (action == OGS_GTP_DELETE_NO_ACTION) {
        /* No Action to be taken after sessions are deleted during
         * MME Initiated detach. S1 will be cleared after receipt
         * of the detach accept from UE */
    } else if (action == OGS_GTP_DELETE_SEND_AUTHENTICATION_REQUEST) {
        if (mme_sess_count(mme_ue) == 1) /* Last Session */ {
            mme_s6a_send_air(enb_ue, mme_ue, NULL);
        }

    } else if (action == OGS_GTP_DELETE_SEND_DETACH_ACCEPT) {
        if (mme_sess_count(mme_ue) == 1) /* Last Session */ {
            r = nas_eps_send_detach_accept(mme_ue);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }

    } else if (action ==
            OGS_GTP_DELETE_SEND_DEACTIVATE_BEARER_CONTEXT_REQUEST) {
        mme_bearer_t *bearer = mme_default_bearer_in_sess(sess);
        if (!bearer) {
            ogs_error("No Bearer");
            return;
        }

        r = nas_eps_send_deactivate_bearer_context_request(bearer);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        /* mme_sess_remove() should not be called here. */
        return;

    } else if (action == OGS_GTP_DELETE_SEND_RELEASE_WITH_UE_CONTEXT_REMOVE) {
        if (mme_sess_count(mme_ue) == 1) /* Last Session */ {
            if (ECM_IDLE(mme_ue)) {
                mme_ue_remove(mme_ue);

                /* mme_sess_remove() should not be called here
                 * since mme_ue_remove() has already been executed. */
                return;
            } else {
                r = s1ap_send_ue_context_release_command(enb_ue,
                    S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                    S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
        }

    } else if (action ==
            OGS_GTP_DELETE_SEND_RELEASE_WITH_S1_REMOVE_AND_UNLINK) {
        if (mme_sess_count(mme_ue) == 1) /* Last Session */ {
            r = s1ap_send_ue_context_release_command(enb_ue,
                    S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                    S1AP_UE_CTX_REL_S1_REMOVE_AND_UNLINK, 0);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }

    } else if (action == OGS_GTP_DELETE_HANDLE_PDN_CONNECTIVITY_REQUEST) {
        if (mme_sess_count(mme_ue) == 1) /* Last Session */ {
            rv = nas_eps_send_emm_to_esm(mme_ue,
                    &mme_ue->pdn_connectivity_request);
            if (rv != OGS_OK) {
                ogs_error("nas_eps_send_emm_to_esm() failed");
                r = nas_eps_send_attach_reject(enb_ue, mme_ue,
                        OGS_NAS_EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED,
                        OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
        }

    } else if (action == OGS_GTP_DELETE_IN_PATH_SWITCH_REQUEST) {

        /* Don't have to remove Session in X2 Handover with SGW relocation */
        ogs_assert(source_ue);

        GTP_COUNTER_CHECK(mme_ue, GTP_COUNTER_DELETE_SESSION_BY_PATH_SWITCH,

            sgw_ue_source_deassociate_target(source_ue);
            sgw_ue_remove(source_ue);

        );

        return;

    } else {
        ogs_fatal("Invalid action = %d", action);
        ogs_assert_if_reached();
    }

    MME_SESS_CLEAR(sess);
}

void mme_s11_handle_create_bearer_request(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp2_create_bearer_request_t *req)
{
    int r;
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    mme_bearer_t *bearer = NULL, *default_bearer = NULL;
    mme_sess_t *sess = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_gtp2_f_teid_t *sgw_s1u_teid = NULL;
    ogs_gtp2_f_teid_t *pgw_s5u_teid = NULL;
    ogs_gtp2_bearer_qos_t bearer_qos;

    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("Create Bearer Request");

    /***********************
     * Check MME-UE Context
     ***********************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!mme_ue) {
        ogs_error("No Context in TEID");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    } else {
        sgw_ue = sgw_ue_find_by_id(mme_ue->sgw_ue_id);
        ogs_assert(sgw_ue);

        if (req->linked_eps_bearer_id.presence == 0) {
            ogs_error("No Linked EBI");
            cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        }

        if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
            sess = mme_sess_find_by_ebi(mme_ue, req->linked_eps_bearer_id.u8);
            if (!sess) {
                ogs_error("No Context for Linked EPS Bearer ID[%d]",
                        req->linked_eps_bearer_id.u8);
                cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
            }
        }
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp2_send_error_message(xact, sgw_ue ? sgw_ue->sgw_s11_teid : 0,
                OGS_GTP2_CREATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /*****************************************
     * Check Mandatory/Conditional IE Missing
     *****************************************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (req->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.s1_u_enodeb_f_teid.presence == 0) {
        ogs_error("No SGW-S1U TEID");
        cause_value = OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
    }
    if (req->bearer_contexts.s4_u_sgsn_f_teid.presence == 0) {
        ogs_error("No PGW-S5U TEID");
        cause_value = OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
    }
    if (req->bearer_contexts.bearer_level_qos.presence == 0) {
        ogs_error("No QoS");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.tft.presence == 0) {
        ogs_error("No TFT");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp2_send_error_message(xact, sgw_ue ? sgw_ue->sgw_s11_teid : 0,
                OGS_GTP2_CREATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(mme_ue);
    ogs_assert(sgw_ue);

    ogs_assert(sess);
    bearer = mme_bearer_add(sess);
    ogs_assert(bearer);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    /*
     * DEPRECATED : Issues #3072
     *
     * PTI 0 is set here to prevent a InitialContextSetupRequest message
     * with a PTI of 0 from being created when the Create Bearer Request occurs
     * and InitialContextSetupRequest occurs.
     *
     * If you implement the creation of a dedicated bearer
     * in the ESM procedure reqeusted by the UE,
     * you will need to refactor the part that sets the PTI.
     */
#if 0
    /* Set PTI */
    sess->pti = OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED;
    if (req->procedure_transaction_id.presence) {
        sess->pti = req->procedure_transaction_id.u8;
        ogs_debug("    PTI[%d]", sess->pti);
    }
#endif

    /* Data Plane(UL) : SGW-S1U */
    sgw_s1u_teid = req->bearer_contexts.s1_u_enodeb_f_teid.data;
    bearer->sgw_s1u_teid = be32toh(sgw_s1u_teid->teid);
    ogs_assert(OGS_OK ==
            ogs_gtp2_f_teid_to_ip(sgw_s1u_teid, &bearer->sgw_s1u_ip));

    /* Data Plane(UL) : PGW-S5U */
    pgw_s5u_teid = req->bearer_contexts.s4_u_sgsn_f_teid.data;
    bearer->pgw_s5u_teid = be32toh(pgw_s5u_teid->teid);
    ogs_assert(OGS_OK ==
            ogs_gtp2_f_teid_to_ip(pgw_s5u_teid, &bearer->pgw_s5u_ip));

    /* Bearer QoS */
    if (ogs_gtp2_parse_bearer_qos(
                &bearer_qos, &req->bearer_contexts.bearer_level_qos) !=
            req->bearer_contexts.bearer_level_qos.len) {
        ogs_error("ogs_gtp2_parse_bearer_qos() failed");
        return;
    }
    bearer->qos.index = bearer_qos.qci;
    bearer->qos.arp.priority_level = bearer_qos.priority_level;
    bearer->qos.arp.pre_emption_capability =
                    bearer_qos.pre_emption_capability;
    bearer->qos.arp.pre_emption_vulnerability =
                    bearer_qos.pre_emption_vulnerability;
    bearer->qos.mbr.downlink = bearer_qos.dl_mbr;
    bearer->qos.mbr.uplink = bearer_qos.ul_mbr;
    bearer->qos.gbr.downlink = bearer_qos.dl_gbr;
    bearer->qos.gbr.uplink = bearer_qos.ul_gbr;

    if (bearer->qos.mbr.downlink || bearer->qos.mbr.uplink ||
        bearer->qos.gbr.downlink || bearer->qos.gbr.uplink) {
        if (bearer->qos.mbr.downlink == 0)
            bearer->qos.mbr.downlink = MAX_BIT_RATE;
        if (bearer->qos.mbr.uplink == 0)
            bearer->qos.mbr.uplink = MAX_BIT_RATE;
        if (bearer->qos.gbr.downlink == 0)
            bearer->qos.gbr.downlink = MAX_BIT_RATE;
        if (bearer->qos.gbr.uplink == 0)
            bearer->qos.gbr.uplink = MAX_BIT_RATE;
    }

    /* Save Bearer TFT */
    OGS_TLV_STORE_DATA(&bearer->tft, &req->bearer_contexts.tft);

    /*
     * Save Transaction. It will be handled after EMM-attached
     *
     * You should not remove OLD bearer->xact.
     * If GTP-xact Holding timer is expired,
     * OLD bearer->xact memory will be automatically removed.
     */
    ogs_assert(xact->id >= OGS_MIN_POOL_ID && xact->id <= OGS_MAX_POOL_ID);
    bearer->create.xact_id = xact->id;

    /* Before Activate DEDICATED bearer, check DEFAULT bearer status */
    default_bearer = mme_default_bearer_in_sess(sess);
    if (!default_bearer) {
        ogs_error("No Default Bearer");
        return;
    }

    if (OGS_FSM_CHECK(&default_bearer->sm, esm_state_active)) {
        if (ECM_IDLE(mme_ue)) {
            MME_STORE_PAGING_INFO(mme_ue,
                MME_PAGING_TYPE_CREATE_BEARER, bearer->id);
            r = s1ap_send_paging(mme_ue, S1AP_CNDomain_ps);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        } else {
            MME_CLEAR_PAGING_INFO(mme_ue);
            r = nas_eps_send_activate_dedicated_bearer_context_request(bearer);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
    } else {
        MME_CLEAR_PAGING_INFO(mme_ue);
        /*
         * After received Activate EPS default bearer context accept
         * Invoke nas_eps_send_activate_all_dedicated_bearers()
         */
    }
}

void mme_s11_handle_update_bearer_request(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp2_update_bearer_request_t *req)
{
    int r;
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;
    sgw_ue_t *sgw_ue = NULL;
    ogs_gtp2_bearer_qos_t bearer_qos;

    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("Update Bearer Request");

    /***********************
     * Check MME-UE Context
     ***********************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!mme_ue) {
        ogs_error("No Context in TEID");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    } else {
        sgw_ue = sgw_ue_find_by_id(mme_ue->sgw_ue_id);
        ogs_assert(sgw_ue);

        if (req->bearer_contexts.presence == 0) {
            ogs_error("No Bearer");
            cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        }
        if (req->bearer_contexts.eps_bearer_id.presence == 0) {
            ogs_error("No EPS Bearer ID");
            cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        }

        if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
            bearer = mme_bearer_find_by_ue_ebi(mme_ue,
                    req->bearer_contexts.eps_bearer_id.u8);
            if (!bearer) {
                ogs_error("No Context");
                cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
            }
        }
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp2_send_error_message(xact, sgw_ue ? sgw_ue->sgw_s11_teid : 0,
                OGS_GTP2_UPDATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(mme_ue);
    ogs_assert(sgw_ue);

    ogs_assert(bearer);
    sess = mme_sess_find_by_id(bearer->sess_id);
    ogs_assert(sess);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    /* Set PTI */
    sess->pti = OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED;
    if (req->procedure_transaction_id.presence) {
        sess->pti = req->procedure_transaction_id.u8;
        ogs_debug("    PTI[%d]", sess->pti);
    }

    /*
     * Issues #3240
     *
     * SMF->SGW-C->MME: First Update Bearer Request
     * MME->UE:         First Modify EPS bearer context request
     * SMF->SGW-C->MME: Second Update Bearer Request
     * MME->UE:         Second Modify EPS bearer context request
     * UE->MME:         First Modify EPS bearer context accept
     * MME->SGW-C->SMF: First Update Bearer Response
     * UE->MME:         Second Modify EPS bearer context accept
     * MME->SGW-C->SMF: Second Update Bearer Response
     *
     * If the UE does not send a Modify EPS bearer context accept,
     * the MME cannot send an Update Bearer Response to the SGW-C.
     *
     * In this case, peer timeout occurs, and a callback function
     * is registered as follows to free memory.
     *
     * Also, as shown above, multiple Update Bearer Request/Response can occur,
     * so we manage the Transaction Node as a list within the Bearer Context.
     */

    xact->peer_cb = gtp_remote_peer_timeout;
    xact->peer_data = OGS_UINT_TO_POINTER(bearer->id);

    ogs_list_add(&bearer->update.xact_list, &xact->to_update_node);

    if (req->bearer_contexts.bearer_level_qos.presence == 1) {
        /* Bearer QoS */
        if (ogs_gtp2_parse_bearer_qos(
                    &bearer_qos, &req->bearer_contexts.bearer_level_qos) !=
                req->bearer_contexts.bearer_level_qos.len) {
            ogs_error("ogs_gtp2_parse_bearer_qos() failed");
            return;
        }
        bearer->qos.index = bearer_qos.qci;
        bearer->qos.arp.priority_level = bearer_qos.priority_level;
        bearer->qos.arp.pre_emption_capability =
                        bearer_qos.pre_emption_capability;
        bearer->qos.arp.pre_emption_vulnerability =
                        bearer_qos.pre_emption_vulnerability;
        bearer->qos.mbr.downlink = bearer_qos.dl_mbr;
        bearer->qos.mbr.uplink = bearer_qos.ul_mbr;
        bearer->qos.gbr.downlink = bearer_qos.dl_gbr;
        bearer->qos.gbr.uplink = bearer_qos.ul_gbr;

        xact->update_flags |=  OGS_GTP_MODIFY_QOS_UPDATE;
    }

    if (req->bearer_contexts.tft.presence == 1) {
        /* Save Bearer TFT */
        OGS_TLV_STORE_DATA(&bearer->tft, &req->bearer_contexts.tft);

        xact->update_flags |=  OGS_GTP_MODIFY_TFT_UPDATE;
    }

    if (req->bearer_contexts.bearer_level_qos.presence == 1 ||
        req->bearer_contexts.tft.presence == 1) {
        if (ECM_IDLE(mme_ue)) {
            MME_STORE_PAGING_INFO(mme_ue,
                MME_PAGING_TYPE_UPDATE_BEARER, bearer->id);
            r = s1ap_send_paging(mme_ue, S1AP_CNDomain_ps);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        } else {
            /*
             * MME must wait for Modify Bearer Context Accept
             * before sending Update Bearer Response,
             * To check this, start a peer timer to check it.
             */
            ogs_timer_start(xact->tm_peer,
                    ogs_local_conf()->time.message.gtp.t3_response_duration);

            MME_CLEAR_PAGING_INFO(mme_ue);
            r = nas_eps_send_modify_bearer_context_request(bearer,
                    req->bearer_contexts.bearer_level_qos.presence,
                    req->bearer_contexts.tft.presence);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
    } else {
        MME_CLEAR_PAGING_INFO(mme_ue);
        ogs_error("[IGNORE] Update Bearer Request : "
                "Both QoS and TFT is NULL");

        if (xact->xid & OGS_GTP_CMD_XACT_ID) {
            /* MME received Bearer Resource Modification Request */
            r = nas_eps_send_bearer_resource_modification_reject(
                    mme_ue, sess->pti,
                    OGS_NAS_ESM_CAUSE_SERVICE_OPTION_NOT_SUPPORTED);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }

        ogs_assert(OGS_OK ==
            mme_gtp_send_update_bearer_response(
                bearer, OGS_GTP2_CAUSE_REQUEST_ACCEPTED));
    }
}

void mme_s11_handle_delete_bearer_request(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp2_delete_bearer_request_t *req)
{
    int r;
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;

    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("Delete Bearer Request");

    /***********************
     * Check MME-UE Context
     ***********************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!mme_ue) {
        ogs_error("No Context in TEID");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    } else {
        sgw_ue = sgw_ue_find_by_id(mme_ue->sgw_ue_id);
        ogs_assert(sgw_ue);

        if (req->linked_eps_bearer_id.presence == 1) {
           /*
            * << Linked EPS Bearer ID >>
            *
            * 1. SMF sends Delete Bearer Request(DEFAULT BEARER) to SGW/MME.
            * 2. MME sends Delete Bearer Response to SGW/SMF.
            *
            * OR
            *
            * 1. SMF sends Delete Bearer Request(DEFAULT BEARER) to ePDG.
            * 2. ePDG sends Delete Bearer Response(DEFAULT BEARER) to SMF.
            */
            bearer = mme_bearer_find_by_ue_ebi(
                        mme_ue, req->linked_eps_bearer_id.u8);
            if (!bearer) {
                ogs_error("Cannot find Bearer [%d]",
                        req->linked_eps_bearer_id.u8);
                cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
            }
        } else if (req->eps_bearer_ids.presence == 1) {
           /*
            * << EPS Bearer IDs >>
            *
            * 1. MME sends Bearer Resource Command to SGW/SMF.
            * 2. SMF sends Delete Bearer Request(DEDICATED BEARER) to SGW/MME.
            * 3. MME sends Delete Bearer Response(DEDICATED BEARER) to SGW/SMF.
            *
            * OR
            *
            * 1. SMF sends Delete Bearer Request(DEDICATED BEARER) to SGW/MME.
            * 2. MME sends Delete Bearer Response(DEDICATED BEARER) to SGW/SMF.
            */
            bearer = mme_bearer_find_by_ue_ebi(mme_ue, req->eps_bearer_ids.u8);
            if (!bearer) {
                ogs_error("Cannot find Bearer [%d]", req->eps_bearer_ids.u8);
                cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
            }
        } else {
            ogs_error("No Linked EBI or EPS Bearer ID");
            cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        }
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp2_send_error_message(xact, sgw_ue ? sgw_ue->sgw_s11_teid : 0,
                OGS_GTP2_DELETE_BEARER_RESPONSE_TYPE,
                OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(mme_ue);
    ogs_assert(sgw_ue);

    ogs_assert(bearer);
    sess = mme_sess_find_by_id(bearer->sess_id);
    ogs_assert(sess);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    /* Set PTI */
    sess->pti = OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED;
    if (req->procedure_transaction_id.presence) {
        sess->pti = req->procedure_transaction_id.u8;
        ogs_debug("    PTI[%d]", sess->pti);
    }

    /*
     * Save Transaction. It will be handled after EMM-attached
     *
     * You should not remove OLD bearer->xact.
     * If GTP-xact Holding timer is expired,
     * OLD bearer->xact memory will be automatically removed.
     */
    ogs_assert(xact->id >= OGS_MIN_POOL_ID && xact->id <= OGS_MAX_POOL_ID);
    bearer->delete.xact_id = xact->id;

    if (ECM_IDLE(mme_ue)) {
        MME_STORE_PAGING_INFO(mme_ue,
            MME_PAGING_TYPE_DELETE_BEARER, bearer->id);
        r = s1ap_send_paging(mme_ue, S1AP_CNDomain_ps);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    } else {
        MME_CLEAR_PAGING_INFO(mme_ue);
        r = nas_eps_send_deactivate_bearer_context_request(bearer);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    }
}

void mme_s11_handle_release_access_bearers_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue_from_teid,
        ogs_gtp2_release_access_bearers_response_t *rsp)
{
    int r, rv;
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    int action = 0;
    enb_ue_t *enb_ue = NULL;

    sgw_ue_t *sgw_ue = NULL;;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    ogs_assert(rsp);

    ogs_debug("Release Access Bearers Response");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(xact);
    action = xact->release_action;
    ogs_assert(action);

    mme_ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    enb_ue = enb_ue_find_by_id(xact->enb_ue_id);

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_commit() failed");
        return;
    }

    if (!mme_ue) {
        ogs_error("MME-UE Context has already been removed");
        return;
    }
    sgw_ue = sgw_ue_find_by_id(mme_ue->sgw_ue_id);
    ogs_assert(sgw_ue);

    /***********************
     * Check MME-UE Context
     ***********************/
    if (!mme_ue_from_teid) {
        ogs_error("No Context in TEID [ACTION:%d]", action);
    }

    /********************
     * Check Cause Value
     ********************/
    if (rsp->cause.presence) {
        ogs_gtp2_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED)
            ogs_error("GTP Cause [VALUE:%d, ACTION:%d]", cause_value, action);
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(mme_ue);
    ogs_assert(sgw_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    ogs_list_for_each(&mme_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            CLEAR_ENB_S1U_PATH(bearer);
        }
    }

    if (action == OGS_GTP_RELEASE_SEND_UE_CONTEXT_RELEASE_COMMAND) {
        if (enb_ue) {
            ogs_assert(enb_ue->relcause.group);
            r = s1ap_send_ue_context_release_command(enb_ue,
                    enb_ue->relcause.group, enb_ue->relcause.cause,
                    S1AP_UE_CTX_REL_S1_REMOVE_AND_UNLINK, 0);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        } else {
            ogs_error("ENB-S1 Context has already been removed");
        }
    } else if (action == OGS_GTP_RELEASE_S1_CONTEXT_REMOVE_BY_LO_CONNREFUSED) {
    /* enb_ue_unlink() and enb_ue_remove() has already been executed.
     * So, there is no `enb_ue` context */

    } else if (action == OGS_GTP_RELEASE_S1_CONTEXT_REMOVE_BY_RESET_ALL) {
    /*
     * TS36.413
     * 8.7.1.2.1 Reset Procedure Initiated from the MME
     *
     * The eNB does not need to wait for the release of radio resources
     * to be completed before returning the RESET ACKNOWLEDGE message.
     *
     * 8.7.1.2.2 Reset Procedure Initiated from the E-UTRAN
     * After the MME has released all assigned S1 resources and
     * the UE S1AP IDs for all indicated UE associations which can be used
     * for new UE-associated logical S1-connections over the S1 interface,
     * the MME shall respond with the RESET ACKNOWLEDGE message.
     */
        enb_ue_unlink(mme_ue);

        if (enb_ue) {
            mme_enb_t *enb = NULL;

            enb = mme_enb_find_by_id(enb_ue->enb_id);

            enb_ue_remove(enb_ue);

            if (enb && ogs_list_count(&enb->enb_ue_list) == 0) {
                r = s1ap_send_s1_reset_ack(enb, NULL);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
        } else {
            ogs_error("ENB-S1 Context has already been removed");
        }

    } else if (action == OGS_GTP_RELEASE_S1_CONTEXT_REMOVE_BY_RESET_PARTIAL) {
        enb_ue_t *iter = NULL;

        enb_ue_unlink(mme_ue);

        if (enb_ue) {
            mme_enb_t *enb = NULL;

            enb = mme_enb_find_by_id(enb_ue->enb_id);

            enb_ue_remove(enb_ue);

            if (enb) {
                ogs_list_for_each(&enb->enb_ue_list, iter) {
                    if (iter->part_of_s1_reset_requested == true) {
                        /* The ENB_UE context
                         * where PartOfS1_interface was requested
                         * still remains */
                        return;
                    }
                }

                /* All ENB_UE context
                 * where PartOfS1_interface was requested
                 * REMOVED */
                ogs_assert(enb->s1_reset_ack);
                r = s1ap_send_to_enb(
                        enb, enb->s1_reset_ack, S1AP_NON_UE_SIGNALLING);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

                /* Clear S1-Reset Ack Buffer */
                enb->s1_reset_ack = NULL;
            }
        } else {
            ogs_error("ENB-S1 Context has already been removed");
        }

    } else {
        ogs_fatal("Invalid action = %d", action);
        ogs_assert_if_reached();
    }
}

void mme_s11_handle_downlink_data_notification(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue,
        ogs_gtp2_downlink_data_notification_t *noti)
{
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    int r;

    mme_bearer_t *bearer = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(xact);
    ogs_assert(noti);

    ogs_debug("Downlink Data Notification");

    /************************
     * Check MME-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!mme_ue) {
        ogs_error("No UE Context");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    } else {
        sgw_ue = sgw_ue_find_by_id(mme_ue->sgw_ue_id);
        ogs_assert(sgw_ue);

        if (noti->eps_bearer_id.presence == 0) {
            ogs_error("No Bearer ID");
            cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        }

        if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
            bearer = mme_bearer_find_by_ue_ebi(mme_ue, noti->eps_bearer_id.u8);
            if (!bearer) {
                ogs_error("No Context for EPS Bearer ID[%d]",
                        noti->eps_bearer_id.u8);
                cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
            }
        }
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp2_send_error_message(xact, sgw_ue ? sgw_ue->sgw_s11_teid : 0,
                OGS_GTP2_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE,
                OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(mme_ue);
    ogs_assert(sgw_ue);

    ogs_assert(bearer);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    /*
     * Save Transaction. It will be handled after ECM-Connected
     *
     * You should not remove OLD bearer->xact.
     * If GTP-xact Holding timer is expired,
     * OLD bearer->xact memory will be automatically removed.
     */
    ogs_assert(xact->id >= OGS_MIN_POOL_ID && xact->id <= OGS_MAX_POOL_ID);
    bearer->notify.xact_id = xact->id;

    if (noti->cause.presence) {
        ogs_gtp2_cause_t *cause = noti->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
    }
/*
 * 5.3.4.2 in Spec 23.401
 * Under certain conditions, the current UE triggered Service Request
 * procedure can cause unnecessary Downlink Packet Notification messages
 * which increase the load of the MME.
 *
 * This can occur when uplink data sent in step 6 causes a response
 * on the downlink which arrives at the Serving GW before the Modify Bearer
 * Request message, step 8. This data cannot be forwarded from the Serving GW
 * to the eNodeB and hence it triggers a Downlink Data Notification message.
 *
 * If the MME receives a Downlink Data Notification after step 2 and
 * before step 9, the MME shall not send S1 interface paging messages
 */
    if (ECM_IDLE(mme_ue)) {
        MME_STORE_PAGING_INFO(mme_ue,
            MME_PAGING_TYPE_DOWNLINK_DATA_NOTIFICATION, bearer->id);
        r = s1ap_send_paging(mme_ue, S1AP_CNDomain_ps);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    } else if (ECM_CONNECTED(mme_ue)) {
        MME_CLEAR_PAGING_INFO(mme_ue);
        ogs_assert(OGS_OK ==
            mme_gtp_send_downlink_data_notification_ack(
                bearer, OGS_GTP2_CAUSE_UE_ALREADY_RE_ATTACHED));

        if (cause_value == OGS_GTP2_CAUSE_ERROR_INDICATION_RECEIVED) {

/*
 * TS23.007 22. Downlink Data Notification Handling at MME/S4 SGSN
 *
 * If the MME/S4 SGSN receives a Downlink Data Notification message from
 * the SGW as a result of the SGW having received an Error Indication message
 * from the eNodeB/RNC or S4-SGSN over S4 User Plane, the MME/S4 SGSN should
 * perform the following:
 *
 * - If the UE is in IDLE state, upon receipt of the Downlink Data
 *   Notification message, the MME/S4 SGSN shall perform the Network
 *   Triggered Service Request procedure as specified in 3GPP TS 23.060[5]
 *   and 3GPP TS 23.401 [15].
 * - If the UE is in CONNECTED state, upon receipt of the Downlink Data
 *   Notification message, the MME shall perform S1 Release procedure and
 *   perform Network Triggered Service Request procedure as specified
 *   in 3GPP TS 23.401 [15].
 * - If the UE is in CONNECTED state, upon receipt of the Downlink Data
 *   Notification message and Direct Tunnel is used, the S4-SGSN shall
 *   perform Iu Release procedure and perform Network Triggered Service
 *   Request procedure as specified in 3GPP TS 23.060 [5]
 *   if the cause value included in Downlink Data Notification is
 *   "Error Indication received from RNC/eNodeB/S4-SGSN",
 * - If the UE is in CONNECTED state, upon receipt of the Downlink Data
 *   Notification message and Direct Tunnel is not used, the S4-SGSN should
 *   re-establish all of the S4-U bearers of this UE if the cause value
 *   included in Downlink Data Notification is "Error Indication received
 *   from RNC/eNodeB/S4-SGSN"
 */
            enb_ue_t *enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
            ogs_assert(enb_ue);

            r = s1ap_send_ue_context_release_command(enb_ue,
                    S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                    S1AP_UE_CTX_REL_S1_PAGING, 0);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
    }
}

void mme_s11_handle_create_indirect_data_forwarding_tunnel_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue_from_teid,
        ogs_gtp2_create_indirect_data_forwarding_tunnel_response_t *rsp)
{
    int i, r, rv;
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    uint8_t session_cause = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    sgw_ue_t *sgw_ue = NULL;
    mme_bearer_t *bearer = NULL;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    enb_ue_t *source_ue = NULL;

    ogs_gtp2_f_teid_t *teid = NULL;

    ogs_assert(rsp);

    ogs_debug("Create Indirect Data Forwarding Tunnel Response");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(xact);
    mme_ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    enb_ue = enb_ue_find_by_id(xact->enb_ue_id);

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_commit() failed");
        return;
    }

    if (!mme_ue) {
        ogs_error("MME-UE Context has already been removed");
        return;
    }
    sgw_ue = sgw_ue_find_by_id(mme_ue->sgw_ue_id);
    ogs_assert(sgw_ue);

    /************************
     * Getting Cause Value
     ************************/
    if (rsp->cause.presence && rsp->cause.data) {
        ogs_gtp2_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);
        session_cause = cause->value;
    }

    /************************
     * Check MME-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!mme_ue_from_teid) {
        ogs_error("[%s] No Context in TEID [Cause:%d]",
                mme_ue->imsi_bcd, session_cause);
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        if (enb_ue)
            mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
        else
            ogs_error("ENB-S1 Context has already been removed");
        return;
    }

    /*****************************************
     * Check Mandatory/Conditional IE Missing
     *****************************************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (rsp->cause.presence == 0) {
        ogs_error("[%s] No Cause [VALUE:%d]", mme_ue->imsi_bcd, session_cause);
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        if (enb_ue)
            mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
        else
            ogs_error("ENB-S1 Context has already been removed");
        return;
    }

    /********************
     * Check Cause Value
     ********************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (session_cause != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_error("[%s] GTP Cause [VALUE:%d]", mme_ue->imsi_bcd, session_cause);
        if (enb_ue)
            mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
        else
            ogs_error("ENB-S1 Context has already been removed");
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(mme_ue);
    ogs_assert(sgw_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    for (i = 0; rsp->bearer_contexts[i].presence; i++) {
        if (rsp->bearer_contexts[i].eps_bearer_id.presence == 0) {
            ogs_error("No EBI");
            return;
        }

        bearer = mme_bearer_find_by_ue_ebi(mme_ue,
                    rsp->bearer_contexts[i].eps_bearer_id.u8);
        if (!bearer) {
            ogs_error("No Bearer");
            return;
        }

        if (rsp->bearer_contexts[i].s4_u_sgsn_f_teid.presence) {
            teid = rsp->bearer_contexts[i].s4_u_sgsn_f_teid.data;
            ogs_assert(teid);

            bearer->sgw_dl_teid = be32toh(teid->teid);
            rv = ogs_gtp2_f_teid_to_ip(teid, &bearer->sgw_dl_ip);
            ogs_assert(rv == OGS_OK);
        }
        if (rsp->bearer_contexts[i].s2b_u_epdg_f_teid_5.presence) {
            teid = rsp->bearer_contexts[i].s2b_u_epdg_f_teid_5.data;
            ogs_assert(teid);

            bearer->sgw_ul_teid = be32toh(teid->teid);
            rv = ogs_gtp2_f_teid_to_ip(teid, &bearer->sgw_ul_ip);
            ogs_assert(rv == OGS_OK);
        }
    }

    source_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    ogs_assert(source_ue);

    r = s1ap_send_handover_command(source_ue);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);
}

void mme_s11_handle_delete_indirect_data_forwarding_tunnel_response(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue_from_teid,
        ogs_gtp2_delete_indirect_data_forwarding_tunnel_response_t *rsp)
{
    int r, rv;
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    uint8_t session_cause = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    int action = 0;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(rsp);

    ogs_debug("Delete Indirect Data Forwarding Tunnel Response");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(xact);
    action = xact->delete_indirect_action;
    ogs_assert(action);
    mme_ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    enb_ue = enb_ue_find_by_id(xact->enb_ue_id);

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_commit() failed");
        return;
    }

    if (!mme_ue) {
        ogs_error("MME-UE Context has already been removed");
        return;
    }
    sgw_ue = sgw_ue_find_by_id(mme_ue->sgw_ue_id);
    ogs_assert(sgw_ue);

    /************************
     * Getting Cause Value
     ************************/
    if (rsp->cause.presence && rsp->cause.data) {
        ogs_gtp2_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);
        session_cause = cause->value;
    }

    /************************
     * Check MME-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!mme_ue_from_teid) {
        ogs_error("[%s] No Context in TEID [Cause:%d]",
                mme_ue->imsi_bcd, session_cause);
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        if (enb_ue)
            mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
        else
            ogs_error("ENB-S1 Context has already been removed");
        return;
    }

    /*****************************************
     * Check Mandatory/Conditional IE Missing
     *****************************************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (rsp->cause.presence == 0) {
        ogs_error("[%s] No Cause [VALUE:%d]", mme_ue->imsi_bcd, session_cause);
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        if (enb_ue)
            mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
        else
            ogs_error("ENB-S1 Context has already been removed");
        return;
    }

    /********************
     * Check Cause Value
     ********************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (session_cause != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_error("GTP Cause [VALUE:%d]", session_cause);
        if (enb_ue)
            mme_send_delete_session_or_mme_ue_context_release(enb_ue, mme_ue);
        else
            ogs_error("ENB-S1 Context has already been removed");
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(mme_ue);
    ogs_assert(sgw_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    mme_ue_clear_indirect_tunnel(mme_ue);

    if (action == OGS_GTP_DELETE_INDIRECT_HANDOVER_COMPLETE) {
        /* Nothing to do */
    } else if (action == OGS_GTP_DELETE_INDIRECT_HANDOVER_CANCEL) {
        r = s1ap_send_handover_cancel_ack(enb_ue_find_by_id(mme_ue->enb_ue_id));
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    } else {
        ogs_fatal("Invalid action = %d", action);
        ogs_assert_if_reached();
    }
}

void mme_s11_handle_bearer_resource_failure_indication(
        ogs_gtp_xact_t *xact, mme_ue_t *mme_ue_from_teid,
        ogs_gtp2_bearer_resource_failure_indication_t *ind)
{
    int r, rv;
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;

    mme_bearer_t *bearer = NULL;
    mme_sess_t *sess = NULL;
    mme_ue_t *mme_ue = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_debug("Bearer Resource Failure Indication");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(ind);
    ogs_assert(xact);
    bearer = mme_bearer_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    if (bearer) {
        sess = mme_sess_find_by_id(bearer->sess_id);
        if (sess)
            mme_ue = mme_ue_find_by_id(sess->mme_ue_id);
    }

    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_commit() failed");
        return;
    }

    if (!bearer) {
        ogs_error("Bearer Context has already been removed");
        return;
    }

    if (!sess) {
        ogs_error("Session Context has already been removed");
        return;
    }

    if (!mme_ue) {
        ogs_error("MME-UE Context has already been removed");
        return;
    }
    sgw_ue = sgw_ue_find_by_id(mme_ue->sgw_ue_id);
    ogs_assert(sgw_ue);

    if (!mme_ue_from_teid)
        ogs_error("No Context in TEID");

    /********************
     * Check Cause Value
     ********************/
    if (ind->cause.presence) {
        ogs_gtp2_cause_t *cause = ind->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        ogs_warn("GTP Cause [VALUE:%d] - Ignored", cause_value);
    } else {
        ogs_error("No Cause");
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(mme_ue);
    ogs_assert(sgw_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            mme_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    r = nas_eps_send_bearer_resource_modification_reject(
            mme_ue, sess->pti, esm_cause_from_gtp(cause_value));
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    if (!sgw_ue ||
        cause_value == OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND) {
        ogs_error("No Bearer");
        mme_bearer_remove(bearer);
    }
}
