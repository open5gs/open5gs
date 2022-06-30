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

#include "gtp-path.h"
#include "pfcp-path.h"

#include "s11-handler.h"

static void gtp_sess_timeout(ogs_gtp_xact_t *xact, void *data)
{
    sgwc_sess_t *sess = data;
    sgwc_ue_t *sgwc_ue = NULL;
    uint8_t type = 0;

    ogs_assert(xact);
    ogs_assert(sess);
    sgwc_ue = sess->sgwc_ue;
    ogs_assert(sgwc_ue);

    type = xact->seq[0].type;

    switch (type) {
    case OGS_GTP2_DELETE_SESSION_REQUEST_TYPE:
        ogs_error("[%s] No Delete Session Response", sgwc_ue->imsi_bcd);
        if (!sgwc_sess_cycle(sess)) {
            ogs_error("[%s] Session has already been removed",
                    sgwc_ue->imsi_bcd);
            break;
        }
        ogs_assert(OGS_OK ==
            sgwc_pfcp_send_session_deletion_request(sess, NULL, NULL));
        break;
    default:
        ogs_error("GTP Timeout : IMSI[%s] Message-Type[%d]",
                sgwc_ue->imsi_bcd, type);
    }
}

static void gtp_bearer_timeout(ogs_gtp_xact_t *xact, void *data)
{
    sgwc_bearer_t *bearer = data;
    sgwc_sess_t *sess = NULL;
    sgwc_ue_t *sgwc_ue = NULL;
    uint8_t type = 0;

    ogs_assert(xact);
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    sgwc_ue = sess->sgwc_ue;
    ogs_assert(sgwc_ue);

    type = xact->seq[0].type;

    ogs_error("GTP Timeout : IMSI[%s] Message-Type[%d]",
            sgwc_ue->imsi_bcd, type);
}

static void pfcp_sess_timeout(ogs_pfcp_xact_t *xact, void *data)
{
    uint8_t type;

    ogs_assert(xact);
    type = xact->seq[0].type;

    switch (type) {
    case OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
        ogs_error("No PFCP session establishment response");
        break;
    case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
        ogs_error("No PFCP session modification response");
        break;
    case OGS_PFCP_SESSION_DELETION_REQUEST_TYPE:
        ogs_error("No PFCP session deletion response");
        break;
    default:
        ogs_error("Not implemented [type:%d]", type);
        break;
    }
}

/* This code was created in case it will be used later,
 * and is currently not being used.  */
static uint8_t pfcp_cause_from_gtp(uint8_t gtp_cause)
{
    switch (gtp_cause) {
    case OGS_GTP2_CAUSE_REQUEST_ACCEPTED:
        return OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
    case OGS_GTP2_CAUSE_REQUEST_REJECTED_REASON_NOT_SPECIFIED:
        return OGS_PFCP_CAUSE_REQUEST_REJECTED;
    case OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND:
        return OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND;
    case OGS_GTP2_CAUSE_MANDATORY_IE_MISSING:
        return OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
    case OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING:
        return OGS_PFCP_CAUSE_CONDITIONAL_IE_MISSING;
    case OGS_GTP2_CAUSE_INVALID_LENGTH:
        return OGS_PFCP_CAUSE_INVALID_LENGTH;
    case OGS_GTP2_CAUSE_MANDATORY_IE_INCORRECT:
        return OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
    case OGS_GTP2_CAUSE_INVALID_MESSAGE_FORMAT:
        return OGS_PFCP_CAUSE_INVALID_FORWARDING_POLICY;
    case OGS_GTP2_CAUSE_REMOTE_PEER_NOT_RESPONDING:
        return OGS_PFCP_CAUSE_NO_ESTABLISHED_PFCP_ASSOCIATION;
    case OGS_GTP2_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION:
        return OGS_PFCP_CAUSE_RULE_CREATION_MODIFICATION_FAILURE;
    case OGS_GTP2_CAUSE_GTP_C_ENTITY_CONGESTION:
        return OGS_PFCP_CAUSE_PFCP_ENTITY_IN_CONGESTION;
    case OGS_GTP2_CAUSE_NO_RESOURCES_AVAILABLE:
        return OGS_PFCP_CAUSE_NO_RESOURCES_AVAILABLE;
    case OGS_GTP2_CAUSE_SERVICE_NOT_SUPPORTED:
        return OGS_PFCP_CAUSE_SERVICE_NOT_SUPPORTED;
    case OGS_GTP2_CAUSE_SYSTEM_FAILURE:
        return OGS_PFCP_CAUSE_SYSTEM_FAILURE;
    default:
        return OGS_PFCP_CAUSE_SYSTEM_FAILURE;
    }

    return OGS_PFCP_CAUSE_SYSTEM_FAILURE;
}

void sgwc_s11_handle_create_session_request(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int i;
    uint8_t cause_value = 0;

    sgwc_sess_t *sess = NULL;
    sgwc_bearer_t *bearer = NULL;

    ogs_gtp2_create_session_request_t *req = NULL;

    uint16_t decoded;
    ogs_gtp2_f_teid_t *mme_s11_teid = NULL;
    ogs_gtp2_f_teid_t *pgw_s5c_teid = NULL;
    ogs_gtp2_uli_t uli;
    ogs_gtp2_bearer_qos_t bearer_qos;
    char apn[OGS_MAX_APN_LEN+1];

    ogs_assert(s11_xact);
    ogs_assert(gtpbuf);
    ogs_assert(message);
    req = &message->create_session_request;
    ogs_assert(req);

    ogs_debug("Create Session Request");

    /************************
     * Check SGWC-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sgwc_ue) {
        ogs_error("No Context");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE, cause_value);
        return;
    }

    /*****************************************
     * Check Mandatory/Conditional IE Missing
     *****************************************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (req->imsi.presence == 0) {
        ogs_error("No IMSI");
        cause_value = OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
    }
    if (req->bearer_contexts_to_be_created[0].presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts_to_be_created[0].eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts_to_be_created[0].bearer_level_qos.presence == 0) {
        ogs_error("No Bearer QoS");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->access_point_name.presence == 0) {
        ogs_error("No APN");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->sender_f_teid_for_control_plane.presence == 0) {
        ogs_error("No Sender F-TEID");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->pgw_s5_s8_address_for_control_plane_or_pmip.presence == 0) {
        ogs_error("No PGW IP");
        cause_value = OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE, cause_value);
        return;
    }

    /* Add Session */
    ogs_assert(0 < ogs_fqdn_parse(apn,
            req->access_point_name.data,
            ogs_min(req->access_point_name.len, OGS_MAX_APN_LEN)));
    sess = sgwc_sess_find_by_ebi(sgwc_ue,
            req->bearer_contexts_to_be_created[0].eps_bearer_id.u8);
    if (sess) {
        ogs_info("OLD Session Release [IMSI:%s,APN:%s]",
                sgwc_ue->imsi_bcd, sess->session.name);
        sgwc_sess_remove(sess);
    }
    sess = sgwc_sess_add(sgwc_ue, apn);
    ogs_assert(sess);

    /* Set User Location Information */
    if (req->user_location_information.presence == 1) {
        decoded = ogs_gtp2_parse_uli(&uli, &req->user_location_information);
        ogs_assert(req->user_location_information.len == decoded);

        sgwc_ue->uli_presence = true;

        ogs_nas_to_plmn_id(&sgwc_ue->e_tai.plmn_id, &uli.tai.nas_plmn_id);
        sgwc_ue->e_tai.tac = uli.tai.tac;
        ogs_nas_to_plmn_id(&sgwc_ue->e_cgi.plmn_id, &uli.e_cgi.nas_plmn_id);
        sgwc_ue->e_cgi.cell_id = uli.e_cgi.cell_id;

        ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
                ogs_plmn_id_hexdump(&sgwc_ue->e_tai.plmn_id),
                sgwc_ue->e_tai.tac);
        ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:0x%x]",
                ogs_plmn_id_hexdump(&sgwc_ue->e_cgi.plmn_id),
                sgwc_ue->e_cgi.cell_id);
    }

    /* Select SGW-U based on UE Location Information */
    sgwc_sess_select_sgwu(sess);

    /* Check if selected SGW-U is associated with SGW-C */
    ogs_assert(sess->pfcp_node);
    if (!OGS_FSM_CHECK(&sess->pfcp_node->sm, sgwc_pfcp_state_associated)) {
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE,
                OGS_GTP2_CAUSE_REMOTE_PEER_NOT_RESPONDING);
        return;
    }

    /* Remove all previous bearer */
    sgwc_bearer_remove_all(sess);

    /* Setup Bearer */
    for (i = 0; i < OGS_BEARER_PER_UE; i++) {
        if (req->bearer_contexts_to_be_created[i].presence == 0)
            break;
        if (req->bearer_contexts_to_be_created[i].eps_bearer_id.presence == 0) {
            ogs_error("No EPS Bearer ID");
            break;
        }
        if (req->bearer_contexts_to_be_created[i].
                bearer_level_qos.presence == 0) {
            ogs_error("No Bearer QoS");
            break;
        }

        decoded = ogs_gtp2_parse_bearer_qos(&bearer_qos,
                &req->bearer_contexts_to_be_created[i].bearer_level_qos);
        ogs_assert(decoded ==
                req->bearer_contexts_to_be_created[i].bearer_level_qos.len);

        bearer = sgwc_bearer_add(sess);
        ogs_assert(bearer);

        /* Set Bearer EBI */
        bearer->ebi = req->bearer_contexts_to_be_created[i].eps_bearer_id.u8;

        if (req->bearer_contexts_to_be_created[i].s1_u_enodeb_f_teid.presence) {

            sgwc_tunnel_t *dl_tunnel = NULL;
            ogs_pfcp_far_t *far = NULL;
            ogs_gtp2_f_teid_t *enb_s1u_teid = NULL;

            dl_tunnel = sgwc_dl_tunnel_in_bearer(bearer);
            ogs_assert(dl_tunnel);

            /* Data Plane(DL) : eNB-S1U */
            enb_s1u_teid = req->bearer_contexts_to_be_created[i].
                            s1_u_enodeb_f_teid.data;
            dl_tunnel->remote_teid = be32toh(enb_s1u_teid->teid);

            ogs_assert(OGS_OK ==
                    ogs_gtp2_f_teid_to_ip(enb_s1u_teid, &dl_tunnel->remote_ip));

            far = dl_tunnel->far;
            ogs_assert(far);

            far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;

            ogs_assert(OGS_OK == ogs_pfcp_ip_to_outer_header_creation(
                    &dl_tunnel->remote_ip, &far->outer_header_creation,
                    &far->outer_header_creation_len));
            far->outer_header_creation.teid = dl_tunnel->remote_teid;
        }

        if (req->bearer_contexts_to_be_created[i].s5_s8_u_sgw_f_teid.presence) {

            sgwc_tunnel_t *ul_tunnel = NULL;
            ogs_pfcp_far_t *far = NULL;
            ogs_gtp2_f_teid_t *pgw_s5u_teid = NULL;

            ul_tunnel = sgwc_ul_tunnel_in_bearer(bearer);
            ogs_assert(ul_tunnel);

            /* Data Plane(UL) : PGW-S5U */
            pgw_s5u_teid = req->bearer_contexts_to_be_created[i].
                            s5_s8_u_sgw_f_teid.data;
            ul_tunnel->remote_teid = be32toh(pgw_s5u_teid->teid);

            ogs_assert(OGS_OK ==
                    ogs_gtp2_f_teid_to_ip(pgw_s5u_teid, &ul_tunnel->remote_ip));

            far = ul_tunnel->far;
            ogs_assert(far);

            far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;

            ogs_assert(OGS_OK == ogs_pfcp_ip_to_outer_header_creation(
                    &ul_tunnel->remote_ip, &far->outer_header_creation,
                    &far->outer_header_creation_len));
            far->outer_header_creation.teid = ul_tunnel->remote_teid;
        }

        /* Set Session QoS from Default Bearer Level QoS */
        if (i == 0) {
            sess->session.qos.index = bearer_qos.qci;
            sess->session.qos.arp.priority_level = bearer_qos.priority_level;
            sess->session.qos.arp.pre_emption_capability =
                            bearer_qos.pre_emption_capability;
            sess->session.qos.arp.pre_emption_vulnerability =
                            bearer_qos.pre_emption_vulnerability;
        }
    }

    /* Receive Control Plane(DL) : MME-S11 */
    mme_s11_teid = req->sender_f_teid_for_control_plane.data;
    ogs_assert(mme_s11_teid);
    sgwc_ue->mme_s11_teid = be32toh(mme_s11_teid->teid);

    /* Receive Control Plane(UL) : PGW-S5C */
    pgw_s5c_teid = req->pgw_s5_s8_address_for_control_plane_or_pmip.data;
    ogs_assert(pgw_s5c_teid);
    sess->pgw_s5c_teid = be32toh(pgw_s5c_teid->teid);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);

    ogs_assert(OGS_OK ==
        sgwc_pfcp_send_session_establishment_request(sess, s11_xact, gtpbuf));
}

void sgwc_s11_handle_modify_bearer_request(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int i = 0;
    uint16_t decoded;
    uint8_t cause_value = 0;

    OGS_LIST(pfcp_xact_list);
    ogs_pfcp_xact_t *pfcp_xact = NULL;

    sgwc_sess_t *sess = NULL;
    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *dl_tunnel = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_ip_t remote_ip;
    ogs_ip_t zero_ip;

    ogs_gtp2_modify_bearer_request_t *req = NULL;

    ogs_gtp2_uli_t uli;
    ogs_gtp2_f_teid_t *enb_s1u_teid = NULL;

    ogs_assert(s11_xact);
    ogs_assert(message);
    req = &message->modify_bearer_request;
    ogs_assert(req);

    ogs_debug("Modify Bearer Request");

    /************************
     * Check SGWC-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sgwc_ue) {
        ogs_error("No Context");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /*****************************************
     * Check Mandatory/Conditional IE Missing
     *****************************************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    for (i = 0; i < OGS_BEARER_PER_UE; i++) {
        ogs_pfcp_xact_t *current_xact = NULL;

        if (req->bearer_contexts_to_be_modified[i].presence == 0) {
            cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
            break;
        }
        if (req->bearer_contexts_to_be_modified[i].eps_bearer_id.
            presence == 0) {
            cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
            break;
        }
        if (req->bearer_contexts_to_be_modified[i].s1_u_enodeb_f_teid.
            presence == 0) {
            cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
            break;
        }

        bearer = sgwc_bearer_find_by_ue_ebi(sgwc_ue,
                    req->bearer_contexts_to_be_modified[i].eps_bearer_id.u8);
        if (!bearer) {
            ogs_error("Unknown EPS Bearer ID[%d]",
                    req->bearer_contexts_to_be_modified[i].eps_bearer_id.u8);
            cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
            break;
        }

        sess = bearer->sess;
        ogs_assert(sess);

        ogs_list_for_each_entry(&pfcp_xact_list, pfcp_xact, tmpnode) {
            if (sess == pfcp_xact->data) {
                current_xact = pfcp_xact;
                break;
            }
        }

        if (!current_xact) {
            current_xact = ogs_pfcp_xact_local_create(
                    sess->pfcp_node, pfcp_sess_timeout, sess);
            ogs_assert(current_xact);

            current_xact->assoc_xact = s11_xact;
            current_xact->modify_flags = OGS_PFCP_MODIFY_SESSION|
                OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_ACTIVATE;
            if (gtpbuf) {
                current_xact->gtpbuf = ogs_pkbuf_copy(gtpbuf);
                ogs_assert(current_xact->gtpbuf);
            }
            current_xact->local_seid = sess->sgwc_sxa_seid;

            ogs_list_add(&pfcp_xact_list, &current_xact->tmpnode);
        }

        dl_tunnel = sgwc_dl_tunnel_in_bearer(bearer);
        ogs_assert(dl_tunnel);

        /* Data Plane(DL) : eNB-S1U */
        enb_s1u_teid =
            req->bearer_contexts_to_be_modified[i].s1_u_enodeb_f_teid.data;
        dl_tunnel->remote_teid = be32toh(enb_s1u_teid->teid);

        ogs_assert(OGS_OK == ogs_gtp2_f_teid_to_ip(enb_s1u_teid, &remote_ip));

        memset(&zero_ip, 0, sizeof(ogs_ip_t));

        if (memcmp(&dl_tunnel->remote_ip, &zero_ip, sizeof(ogs_ip_t)) != 0 &&
            memcmp(&dl_tunnel->remote_ip, &remote_ip, sizeof(ogs_ip_t)) != 0) {

            ogs_assert(sess->pfcp_node);

            /* eNB IP is changed during handover */
            if (sess->pfcp_node->up_function_features.empu) {
                current_xact->modify_flags |= OGS_PFCP_MODIFY_END_MARKER;
            } else {
                ogs_error("SGW-U does not support End Marker");
            }
        }

        memcpy(&dl_tunnel->remote_ip, &remote_ip, sizeof(ogs_ip_t));

        far = dl_tunnel->far;
        ogs_assert(far);

        far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;

        ogs_assert(OGS_OK ==
            ogs_pfcp_ip_to_outer_header_creation(&dl_tunnel->remote_ip,
                &far->outer_header_creation, &far->outer_header_creation_len));
        far->outer_header_creation.teid = dl_tunnel->remote_teid;

        ogs_list_add(&current_xact->bearer_to_modify_list,
                        &bearer->to_modify_node);
    }

    if (i == 0) {
        ogs_error("No Bearer");
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    if (req->user_location_information.presence == 1) {
        decoded = ogs_gtp2_parse_uli(&uli, &req->user_location_information);
        ogs_assert(req->user_location_information.len == decoded);

        sgwc_ue->uli_presence = true;

        ogs_nas_to_plmn_id(&sgwc_ue->e_tai.plmn_id, &uli.tai.nas_plmn_id);
        sgwc_ue->e_tai.tac = uli.tai.tac;
        ogs_nas_to_plmn_id(&sgwc_ue->e_cgi.plmn_id, &uli.e_cgi.nas_plmn_id);
        sgwc_ue->e_cgi.cell_id = uli.e_cgi.cell_id;

        ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
                ogs_plmn_id_hexdump(&sgwc_ue->e_tai.plmn_id),
                sgwc_ue->e_tai.tac);
        ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:0x%x]",
                ogs_plmn_id_hexdump(&sgwc_ue->e_cgi.plmn_id),
                sgwc_ue->e_cgi.cell_id);
    }

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);
    ogs_debug("    ENB_S1U_TEID[%d] SGW_S1U_TEID[%d]",
        dl_tunnel->remote_teid, dl_tunnel->local_teid);

    ogs_list_for_each_entry(&pfcp_xact_list, pfcp_xact, tmpnode)
        sgwc_pfcp_send_bearer_to_modify_list(pfcp_xact->data, pfcp_xact);
}

void sgwc_s11_handle_delete_session_request(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int rv;
    uint8_t cause_value = 0;
    sgwc_sess_t *sess = NULL;
    ogs_gtp_xact_t *s5c_xact = NULL;
    ogs_gtp2_delete_session_request_t *req = NULL;
    ogs_gtp2_indication_t *indication = NULL;

    ogs_assert(s11_xact);
    ogs_assert(gtpbuf);
    ogs_assert(message);
    req = &message->delete_session_request;
    ogs_assert(req);

    ogs_debug("Delete Session Request");

    /************************
     * Check SGWC-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sgwc_ue) {
        ogs_error("No Context");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    } else {
        if (req->linked_eps_bearer_id.presence == 0) {
            ogs_error("No EPS Bearer ID");
            cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        }

        if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
            sess = sgwc_sess_find_by_ebi(sgwc_ue, req->linked_eps_bearer_id.u8);
            if (!sess) {
                ogs_error("Unknown EPS Bearer [IMSI:%s, EBI:%d]",
                        sgwc_ue->imsi_bcd, req->linked_eps_bearer_id.u8);
                cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
            }
        }
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE, cause_value);
        return;
    }

    /*****************************************
     * Check Mandatory/Conditional IE Missing
     *****************************************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (req->indication_flags.presence &&
        req->indication_flags.data && req->indication_flags.len) {
        indication = req->indication_flags.data;
    }

    if (indication &&
        indication->operation_indication == 1 &&
        indication->scope_indication == 1) {
        ogs_error("Invalid Indication");
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE,
                OGS_GTP2_CAUSE_INVALID_MESSAGE_FORMAT);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(sgwc_ue);
    ogs_assert(sess);
    ogs_assert(sess->gnode);
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    if (indication &&
        indication->operation_indication == 0 &&
        indication->scope_indication == 1) {

        ogs_assert(OGS_OK ==
            sgwc_pfcp_send_session_deletion_request(sess, s11_xact, gtpbuf));

    } else {
        message->h.type = OGS_GTP2_DELETE_SESSION_REQUEST_TYPE;
        message->h.teid = sess->pgw_s5c_teid;

        gtpbuf = ogs_gtp2_build_msg(message);
        ogs_expect_or_return(gtpbuf);

        s5c_xact = ogs_gtp_xact_local_create(
                sess->gnode, &message->h, gtpbuf, gtp_sess_timeout, sess);
        ogs_expect_or_return(s5c_xact);

        ogs_gtp_xact_associate(s11_xact, s5c_xact);

        rv = ogs_gtp_xact_commit(s5c_xact);
        ogs_expect(rv == OGS_OK);
    }
}

void sgwc_s11_handle_create_bearer_response(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int rv;
    ogs_gtp2_cause_t *cause = NULL;
    uint8_t cause_value;
    uint16_t decoded;

    sgwc_sess_t *sess = NULL;
    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *dl_tunnel = NULL, *ul_tunnel = NULL;
    ogs_pfcp_far_t *far = NULL;

    ogs_gtp_xact_t *s5c_xact = NULL;

    ogs_gtp2_create_bearer_response_t *rsp = NULL;
    ogs_gtp2_f_teid_t *sgw_s1u_teid = NULL, *enb_s1u_teid = NULL;
    ogs_gtp2_uli_t uli;

    ogs_assert(message);
    rsp = &message->create_bearer_response;
    ogs_assert(rsp);

    ogs_debug("Create Bearer Response");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(s11_xact);
    s5c_xact = s11_xact->assoc_xact;
    ogs_assert(s5c_xact);

    if (s11_xact->xid & OGS_GTP_CMD_XACT_ID)
        /* MME received Bearer Resource Modification Request */
        bearer = s5c_xact->data;
    else
        bearer = s11_xact->data;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);

    /************************
     * Check SGWC-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sgwc_ue) {
        ogs_error("No Context in TEID");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_assert(OGS_OK ==
            sgwc_pfcp_send_bearer_modification_request(
                bearer, NULL, NULL,
                OGS_PFCP_MODIFY_UL_ONLY|OGS_PFCP_MODIFY_REMOVE));
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP2_CREATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /*****************************************
     * Check Mandatory/Conditional IE Missing
     *****************************************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (rsp->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (rsp->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (rsp->bearer_contexts.s1_u_enodeb_f_teid.presence == 0) {
        ogs_error("No eNB TEID");
        cause_value = OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
    }
    if (rsp->bearer_contexts.s4_u_sgsn_f_teid.presence == 0) {
        ogs_error("No SGW TEID");
        cause_value = OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
    }

    if (rsp->cause.presence == 0) {
        ogs_error("No Cause");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (rsp->bearer_contexts.cause.presence == 0) {
        ogs_error("No Bearer Cause");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_assert(OGS_OK ==
            sgwc_pfcp_send_bearer_modification_request(
                bearer, NULL, NULL,
                OGS_PFCP_MODIFY_UL_ONLY|OGS_PFCP_MODIFY_REMOVE));
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP2_CREATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /********************
     * Check Cause Value
     ********************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    cause = rsp->bearer_contexts.cause.data;
    ogs_assert(cause);
    cause_value = cause->value;
    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_error("GTP Failed [CAUSE:%d]", cause_value);
        ogs_assert(OGS_OK ==
            sgwc_pfcp_send_bearer_modification_request(
                bearer, NULL, NULL,
                OGS_PFCP_MODIFY_UL_ONLY|OGS_PFCP_MODIFY_REMOVE));
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP2_CREATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(sgwc_ue);
    ogs_assert(sess);
    ogs_assert(bearer);

    /* Correlate with SGW-S1U-TEID */
    sgw_s1u_teid = rsp->bearer_contexts.s4_u_sgsn_f_teid.data;
    ogs_assert(sgw_s1u_teid);

    /* Find the Tunnel by SGW-S1U-TEID */
    ul_tunnel = sgwc_tunnel_find_by_teid(sgwc_ue, be32toh(sgw_s1u_teid->teid));
    ogs_assert(ul_tunnel);
    dl_tunnel = sgwc_dl_tunnel_in_bearer(bearer);
    ogs_assert(dl_tunnel);

    /* Set EBI */
    bearer->ebi = rsp->bearer_contexts.eps_bearer_id.u8;

    /* Data Plane(DL) : eNB-S1U */
    enb_s1u_teid = rsp->bearer_contexts.s1_u_enodeb_f_teid.data;
    dl_tunnel->remote_teid = be32toh(enb_s1u_teid->teid);

    ogs_debug("    ENB_S1U_TEID[%d] SGW_S1U_TEID[%d]",
        dl_tunnel->remote_teid, dl_tunnel->local_teid);

    ogs_assert(OGS_OK ==
            ogs_gtp2_f_teid_to_ip(enb_s1u_teid, &dl_tunnel->remote_ip));

    far = dl_tunnel->far;
    ogs_assert(far);

    far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;

    ogs_assert(OGS_OK ==
        ogs_pfcp_ip_to_outer_header_creation(&dl_tunnel->remote_ip,
            &far->outer_header_creation, &far->outer_header_creation_len));
    far->outer_header_creation.teid = dl_tunnel->remote_teid;

    if (rsp->user_location_information.presence == 1) {
        decoded = ogs_gtp2_parse_uli(&uli, &rsp->user_location_information);
        ogs_assert(rsp->user_location_information.len == decoded);

        sgwc_ue->uli_presence = true;

        ogs_nas_to_plmn_id(&sgwc_ue->e_tai.plmn_id, &uli.tai.nas_plmn_id);
        sgwc_ue->e_tai.tac = uli.tai.tac;
        ogs_nas_to_plmn_id(&sgwc_ue->e_cgi.plmn_id, &uli.e_cgi.nas_plmn_id);
        sgwc_ue->e_cgi.cell_id = uli.e_cgi.cell_id;

        ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
                ogs_plmn_id_hexdump(&sgwc_ue->e_tai.plmn_id),
                sgwc_ue->e_tai.tac);
        ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:0x%x]",
                ogs_plmn_id_hexdump(&sgwc_ue->e_cgi.plmn_id),
                sgwc_ue->e_cgi.cell_id);
    }

    ogs_assert(OGS_OK ==
        sgwc_pfcp_send_bearer_modification_request(
            bearer, s5c_xact, gtpbuf,
            OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_CREATE));
}

void sgwc_s11_handle_update_bearer_response(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int rv;
    ogs_gtp2_cause_t *cause = NULL;
    uint8_t cause_value;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_xact_t *s5c_xact = NULL;
    sgwc_sess_t *sess = NULL;
    sgwc_bearer_t *bearer = NULL;
    ogs_gtp2_update_bearer_response_t *rsp = NULL;

    ogs_assert(message);
    rsp = &message->update_bearer_response;
    ogs_assert(rsp);

    ogs_debug("Update Bearer Response");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(s11_xact);
    s5c_xact = s11_xact->assoc_xact;
    ogs_assert(s5c_xact);

    if (s11_xact->xid & OGS_GTP_CMD_XACT_ID)
        /* MME received Bearer Resource Modification Request */
        bearer = s5c_xact->data;
    else
        bearer = s11_xact->data;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);

    /************************
     * Check SGWC-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sgwc_ue) {
        ogs_error("No Context in TEID");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP2_UPDATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /*****************************************
     * Check Mandatory/Conditional IE Missing
     *****************************************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (rsp->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (rsp->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (rsp->cause.presence == 0) {
        ogs_error("No Cause");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (rsp->bearer_contexts.cause.presence == 0) {
        ogs_error("No Bearer Cause");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP2_UPDATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /********************
     * Check Cause Value
     ********************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    cause = rsp->bearer_contexts.cause.data;
    ogs_assert(cause);
    cause_value = cause->value;
    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_error("GTP Failed [Bearer-CAUSE:%d]", cause_value);
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP2_UPDATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    cause = rsp->cause.data;
    ogs_assert(cause);
    cause_value = cause->value;
    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_error("GTP Failed [CAUSE:%d]", cause_value);
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP2_UPDATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(sgwc_ue);
    ogs_assert(sess);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    message->h.type = OGS_GTP2_UPDATE_BEARER_RESPONSE_TYPE;
    message->h.teid = sess->pgw_s5c_teid;

    pkbuf = ogs_gtp2_build_msg(message);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(s5c_xact, &message->h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);
}

void sgwc_s11_handle_delete_bearer_response(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int rv;
    uint8_t cause_value;
    ogs_gtp_xact_t *s5c_xact = NULL;

    sgwc_sess_t *sess = NULL;
    sgwc_bearer_t *bearer = NULL;
    ogs_gtp2_delete_bearer_response_t *rsp = NULL;

    ogs_assert(message);
    rsp = &message->delete_bearer_response;
    ogs_assert(rsp);

    ogs_debug("Delete Bearer Response");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(s11_xact);
    s5c_xact = s11_xact->assoc_xact;
    ogs_assert(s5c_xact);

    if (s11_xact->xid & OGS_GTP_CMD_XACT_ID)
        /* MME received Bearer Resource Modification Request */
        bearer = s5c_xact->data;
    else
        bearer = s11_xact->data;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);

    /************************
     * Check SGWC-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sgwc_ue) {
        ogs_error("No Context in TEID");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (rsp->linked_eps_bearer_id.presence) {
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
        if (rsp->cause.presence) {
            ogs_gtp2_cause_t *cause = rsp->cause.data;
            ogs_assert(cause);

            cause_value = cause->value;
            if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
            } else {
                ogs_error("GTP Failed [CAUSE:%d]", cause_value);
            }
        } else {
            ogs_error("No Cause");
        }

        ogs_assert(OGS_OK ==
            sgwc_pfcp_send_session_deletion_request(sess, s5c_xact, gtpbuf));
    } else {
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
        if (rsp->bearer_contexts.presence == 0) {
            ogs_error("No Bearer");
        }
        if (rsp->bearer_contexts.eps_bearer_id.presence == 0) {
            ogs_error("No EPS Bearer ID");
        }

        if (rsp->cause.presence) {
            ogs_gtp2_cause_t *cause = rsp->cause.data;
            ogs_assert(cause);

            cause_value = cause->value;
            if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
                if (rsp->bearer_contexts.cause.presence) {
                    cause = rsp->bearer_contexts.cause.data;
                    ogs_assert(cause);

                    if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
                    } else {
                        ogs_error("GTP Failed [CAUSE:%d]", cause_value);
                    }
                } else {
                    ogs_error("No Cause");
                }
            } else {
                ogs_error("GTP Failed [CAUSE:%d]", cause_value);
            }
        } else {
            ogs_error("No Cause");
        }

        if (sgwc_ue) {
            ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
                sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);
        }
        ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
            sess->sgw_s5c_teid, sess->pgw_s5c_teid);

        ogs_assert(OGS_OK ==
            sgwc_pfcp_send_bearer_modification_request(
                bearer, s5c_xact, gtpbuf, OGS_PFCP_MODIFY_REMOVE));
    }
}

void sgwc_s11_handle_release_access_bearers_request(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    sgwc_sess_t *sess = NULL;
    uint8_t cause_value;

    ogs_gtp2_release_access_bearers_request_t *req = NULL;

    ogs_assert(s11_xact);
    ogs_assert(message);
    req = &message->release_access_bearers_request;
    ogs_assert(req);

    ogs_debug("Release Access Bearers Request");

    /************************
     * Check SGWC-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sgwc_ue) {
        ogs_error("No Context");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE, cause_value);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(sgwc_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);

    ogs_list_for_each(&sgwc_ue->sess_list, sess) {

        ogs_assert(OGS_OK ==
            sgwc_pfcp_send_session_modification_request(
                sess, s11_xact, gtpbuf,
                OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_DEACTIVATE));
    }
}

void sgwc_s11_handle_downlink_data_notification_ack(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int rv;
    uint8_t cause_value;

    sgwc_bearer_t *bearer = NULL;
    sgwc_sess_t *sess = NULL;

    ogs_gtp2_downlink_data_notification_acknowledge_t *ack = NULL;

    ogs_assert(message);
    ack = &message->downlink_data_notification_acknowledge;
    ogs_assert(ack);

    /********************
     * Check Transaction
     ********************/
    ogs_assert(s11_xact);
    bearer = s11_xact->data;
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);

    /************************
     * Check SGWC-UE Context
     ************************/
    if (ack->cause.presence) {
        ogs_gtp2_cause_t *cause = ack->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED)
            ogs_warn("GTP Failed [CAUSE:%d] - PFCP_CAUSE[%d]",
                    cause_value, pfcp_cause_from_gtp(cause_value));
    } else {
        ogs_error("No Cause");
    }

    ogs_debug("Downlink Data Notification Acknowledge");
    if (sgwc_ue) {
        ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
            sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);
    }
}

void sgwc_s11_handle_create_indirect_data_forwarding_tunnel_request(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int rv, i;

    sgwc_sess_t *sess = NULL;
    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *tunnel = NULL;
    ogs_pfcp_far_t *far = NULL;

    ogs_gtp2_create_indirect_data_forwarding_tunnel_request_t *req = NULL;
    ogs_gtp2_f_teid_t *req_teid = NULL;
    uint8_t cause_value = 0;

    ogs_assert(s11_xact);
    ogs_assert(message);
    req = &message->create_indirect_data_forwarding_tunnel_request;
    ogs_assert(req);

    ogs_debug("Create Indirect Data Forwarding Tunnel Request");

    /************************
     * Check SGWC-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sgwc_ue) {
        ogs_error("No Context");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE,
                cause_value);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(sgwc_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);

    for (i = 0; req->bearer_contexts[i].presence; i++) {
        if (req->bearer_contexts[i].eps_bearer_id.presence == 0) {
            ogs_error("No EBI");
            ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE,
                OGS_GTP2_CAUSE_MANDATORY_IE_MISSING);
            return;
        }

        bearer = sgwc_bearer_find_by_ue_ebi(sgwc_ue,
                    req->bearer_contexts[i].eps_bearer_id.u8);
        ogs_assert(bearer);

        if (req->bearer_contexts[i].s1_u_enodeb_f_teid.presence) {
            req_teid = req->bearer_contexts[i].s1_u_enodeb_f_teid.data;
            ogs_assert(req_teid);

            tunnel = sgwc_tunnel_add(bearer,
                    OGS_GTP2_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING);
            ogs_assert(tunnel);

            tunnel->remote_teid = be32toh(req_teid->teid);

            rv = ogs_gtp2_f_teid_to_ip(req_teid, &tunnel->remote_ip);
            if (rv != OGS_OK) {
                ogs_gtp_send_error_message(s11_xact,
                        sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE,
                OGS_GTP2_CAUSE_MANDATORY_IE_MISSING);
                return;
            }

            far = tunnel->far;
            ogs_assert(far);

            far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;

            ogs_assert(OGS_OK ==
                ogs_pfcp_ip_to_outer_header_creation(
                    &tunnel->remote_ip,
                    &far->outer_header_creation,
                    &far->outer_header_creation_len));
            far->outer_header_creation.teid = tunnel->remote_teid;

            ogs_debug("    SGW_DL_TEID[%d] ENB_DL_TEID[%d]",
                    tunnel->local_teid, tunnel->remote_teid);
        }

        if (req->bearer_contexts[i].s12_rnc_f_teid.presence) {
            req_teid = req->bearer_contexts[i].s12_rnc_f_teid.data;
            ogs_assert(req_teid);

            tunnel = sgwc_tunnel_add(bearer,
                    OGS_GTP2_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING);
            ogs_assert(tunnel);

            tunnel->remote_teid = be32toh(req_teid->teid);

            rv = ogs_gtp2_f_teid_to_ip(req_teid, &tunnel->remote_ip);
            if (rv != OGS_OK) {
                ogs_gtp_send_error_message(s11_xact,
                        sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE,
                OGS_GTP2_CAUSE_MANDATORY_IE_MISSING);
                return;
            }

            far = tunnel->far;
            ogs_assert(far);

            far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;

            ogs_assert(OGS_OK ==
                ogs_pfcp_ip_to_outer_header_creation(
                    &tunnel->remote_ip,
                    &far->outer_header_creation,
                    &far->outer_header_creation_len));
            far->outer_header_creation.teid = tunnel->remote_teid;

            ogs_debug("    SGW_UL_TEID[%d] ENB_UL_TEID[%d]",
                    tunnel->local_teid, tunnel->remote_teid);
        }
    }

    ogs_list_for_each(&sgwc_ue->sess_list, sess) {

        ogs_assert(OGS_OK ==
            sgwc_pfcp_send_session_modification_request(
                sess, s11_xact, gtpbuf,
                OGS_PFCP_MODIFY_INDIRECT|OGS_PFCP_MODIFY_CREATE));
    }
}

void sgwc_s11_handle_delete_indirect_data_forwarding_tunnel_request(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *recv_message)
{
    sgwc_sess_t *sess = NULL;
    uint8_t cause_value = 0;

    ogs_assert(s11_xact);

    ogs_debug("Delete Indirect Data Forwarding Tunnel Request");

    /************************
     * Check SGWC-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sgwc_ue) {
        ogs_error("No Context");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE,
                cause_value);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(sgwc_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);

    ogs_list_for_each(&sgwc_ue->sess_list, sess) {

        ogs_assert(OGS_OK ==
            sgwc_pfcp_send_session_modification_request(
                sess, s11_xact, gtpbuf,
                OGS_PFCP_MODIFY_INDIRECT| OGS_PFCP_MODIFY_REMOVE));
    }
}

void sgwc_s11_handle_bearer_resource_command(
        sgwc_ue_t *sgwc_ue, ogs_gtp_xact_t *s11_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp2_bearer_resource_command_t *cmd = NULL;

    uint8_t cause_value = 0;
    ogs_gtp_xact_t *s5c_xact = NULL;

    sgwc_sess_t *sess = NULL;
    sgwc_bearer_t *bearer = NULL;

    ogs_assert(s11_xact);
    ogs_assert(message);
    cmd = &message->bearer_resource_command;
    ogs_assert(cmd);

    ogs_debug("Bearer Resource Command");

    /************************
     * Check SGWC-UE Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sgwc_ue) {
        ogs_error("No Context");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    } else {
        if (cmd->linked_eps_bearer_id.presence == 0) {
            ogs_error("No Linked EPS Bearer ID");
            cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        }

        if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
            uint8_t ebi = cmd->linked_eps_bearer_id.u8;

            if (cmd->eps_bearer_id.presence)
                ebi = cmd->eps_bearer_id.u8;

            bearer = sgwc_bearer_find_by_ue_ebi(sgwc_ue, ebi);
            if (!bearer) {
                ogs_error("No Context for Linked EPS Bearer ID[%d:%d]",
                        cmd->linked_eps_bearer_id.u8, ebi);
                cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
            }
        }
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_BEARER_RESOURCE_FAILURE_INDICATION_TYPE, cause_value);
        return;
    }

    /*****************************************
     * Check Mandatory/Conditional IE Missing
     *****************************************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (cmd->procedure_transaction_id.presence == 0) {
        ogs_error("No PTI");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (cmd->traffic_aggregate_description.presence == 0) {
        ogs_error("No Traffic aggregate description(TAD)");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_BEARER_RESOURCE_FAILURE_INDICATION_TYPE, cause_value);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    ogs_assert(sess->gnode);
    ogs_assert(sgwc_ue);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    message->h.type = OGS_GTP2_BEARER_RESOURCE_COMMAND_TYPE;
    message->h.teid = sess->pgw_s5c_teid;

    pkbuf = ogs_gtp2_build_msg(message);
    ogs_expect_or_return(pkbuf);

    s5c_xact = ogs_gtp_xact_local_create(
            sess->gnode, &message->h, pkbuf, gtp_bearer_timeout, bearer);
    ogs_expect_or_return(s5c_xact);

    ogs_gtp_xact_associate(s11_xact, s5c_xact);

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);
}
