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

#include "s5c-handler.h"

static void bearer_timeout(ogs_gtp_xact_t *xact, void *data)
{
    sgwc_bearer_t *bearer = NULL;
    ogs_pool_id_t bearer_id = OGS_INVALID_POOL_ID;
    sgwc_sess_t *sess = NULL;
    sgwc_ue_t *sgwc_ue = NULL;
    uint8_t type = 0;

    ogs_assert(xact);
    type = xact->seq[0].type;

    ogs_assert(data);
    bearer_id = OGS_POINTER_TO_UINT(data);
    ogs_assert(bearer_id >= OGS_MIN_POOL_ID && bearer_id <= OGS_MAX_POOL_ID);

    bearer = sgwc_bearer_find_by_id(bearer_id);
    if (!bearer) {
        ogs_error("Bearer has already been removed [%d]", type);
        return;
    }

    sess = sgwc_sess_find_by_id(bearer->sess_id);
    ogs_assert(sess);
    sgwc_ue = sgwc_ue_find_by_id(sess->sgwc_ue_id);
    ogs_assert(sgwc_ue);

    switch (type) {
    case OGS_GTP2_UPDATE_BEARER_REQUEST_TYPE:
        ogs_error("[%s] No Update Bearer Response", sgwc_ue->imsi_bcd);
        break;
    case OGS_GTP2_DELETE_BEARER_REQUEST_TYPE:
        ogs_error("[%s] No Delete Bearer Response", sgwc_ue->imsi_bcd);
        ogs_assert(OGS_OK ==
            sgwc_pfcp_send_bearer_modification_request(
                bearer, OGS_INVALID_POOL_ID, NULL, OGS_PFCP_MODIFY_REMOVE));
        break;
    default:
        ogs_error("GTP Timeout : IMSI[%s] Message-Type[%d]",
                sgwc_ue->imsi_bcd, type);
    }
}

void sgwc_s5c_handle_create_session_response(
        sgwc_sess_t *sess, ogs_gtp_xact_t *s5c_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int rv, i;
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    uint8_t session_cause = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    uint8_t bearer_cause = OGS_GTP2_CAUSE_UNDEFINED_VALUE;

    sgwc_ue_t *sgwc_ue = NULL;
    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *ul_tunnel = NULL;
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;

    ogs_gtp2_f_teid_t *pgw_s5c_teid = NULL;
    ogs_gtp2_f_teid_t *pgw_s5u_teid = NULL;

    ogs_gtp2_create_session_response_t *rsp = NULL;
    ogs_gtp_xact_t *s11_xact = NULL;
    ogs_gtp_node_t *pgw = NULL;

    ogs_assert(gtpbuf);
    ogs_assert(message);
    rsp = &message->create_session_response;
    ogs_assert(rsp);

    ogs_debug("Create Session Response");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(s5c_xact);
    s11_xact = ogs_gtp_xact_find_by_id(s5c_xact->assoc_xact_id);
    ogs_assert(s11_xact);

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);

    /************************
     * Getting Cause Value
     ************************/
    if (rsp->cause.presence && rsp->cause.data) {
        ogs_gtp2_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);
        session_cause = cause->value;
    }

    /************************
     * Check Session Context
     *
     * - Session could be deleted before a message is received from SMF.
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_error("No Context in TEID [Cause:%d]", session_cause);
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    } else {
        sgwc_ue = sgwc_ue_find_by_id(sess->sgwc_ue_id);
        ogs_assert(sgwc_ue);
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

    if (rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.presence == 0) {
        ogs_error("No GTP TEID [Cause:%d]", session_cause);
        cause_value = OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
    }
    if (rsp->pdn_address_allocation.presence == 0) {
        ogs_error("No PDN Address Allocation [Cause:%d]", session_cause);
        cause_value = OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
    } else {
        memcpy(&sess->paa, rsp->pdn_address_allocation.data,
                rsp->pdn_address_allocation.len);
        sess->session.session_type = sess->paa.session_type;
        if (sess->session.session_type == OGS_PDU_SESSION_TYPE_IPV4) {
        } else if (sess->session.session_type == OGS_PDU_SESSION_TYPE_IPV6) {
        } else if (sess->session.session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
        } else {
            ogs_error("Unknown session-type [%d]", sess->session.session_type);
            cause_value = OGS_GTP2_CAUSE_PREFERRED_PDN_TYPE_NOT_SUPPORTED;
        }
    }

    if (rsp->cause.presence == 0) {
        ogs_error("No Cause [VALUE:%d]", session_cause);
        cause_value = OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE, cause_value);
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
            ogs_error("GTP Bearer Cause [VALUE:%d]", bearer_cause);
            ogs_gtp_send_error_message(
                    s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                    OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE, bearer_cause);
            return;
        }
    }

    if (session_cause != OGS_GTP2_CAUSE_REQUEST_ACCEPTED &&
        session_cause != OGS_GTP2_CAUSE_REQUEST_ACCEPTED_PARTIALLY &&
        session_cause !=
            OGS_GTP2_CAUSE_NEW_PDN_TYPE_DUE_TO_NETWORK_PREFERENCE &&
        session_cause !=
            OGS_GTP2_CAUSE_NEW_PDN_TYPE_DUE_TO_SINGLE_ADDRESS_BEARER_ONLY) {
        ogs_error("GTP Cause [VALUE:%d]", session_cause);
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE, session_cause);
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

    for (i = 0; i < OGS_BEARER_PER_UE; i++) {
        if (rsp->bearer_contexts_created[i].presence == 0) {
            break;
        }
        if (rsp->bearer_contexts_created[i].eps_bearer_id.presence == 0) {
            ogs_error("No EPS Bearer ID");
            break;
        }
        if (rsp->bearer_contexts_created[i].s5_s8_u_sgw_f_teid.presence == 0) {
            ogs_error("No GTP TEID");
            break;
        }

        /* EPS Bearer ID */
        bearer = sgwc_bearer_find_by_sess_ebi(sess,
                    rsp->bearer_contexts_created[i].eps_bearer_id.u8);
        if (!bearer) {
            ogs_gtp_send_error_message(
                    s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                    OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE,
                    OGS_GTP2_CAUSE_MANDATORY_IE_MISSING);
            return;
        }

        ul_tunnel = sgwc_ul_tunnel_in_bearer(bearer);
        ogs_assert(ul_tunnel);

        ogs_debug("    SGW_S5U_TEID[%d] PGW_S5U_TEID[%d]",
            ul_tunnel->local_teid, ul_tunnel->remote_teid);

        /* Receive Data Plane(UL) : PGW-S5U */
        pgw_s5u_teid = rsp->bearer_contexts_created[i].s5_s8_u_sgw_f_teid.data;
        ogs_assert(pgw_s5u_teid);
        ul_tunnel->remote_teid = be32toh(pgw_s5u_teid->teid);

        rv = ogs_gtp2_f_teid_to_ip(pgw_s5u_teid, &ul_tunnel->remote_ip);
        if (rv != OGS_OK) {
            ogs_gtp_send_error_message(
                    s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                    OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE,
                    OGS_GTP2_CAUSE_MANDATORY_IE_MISSING);
            return;
        }

        pdr = ul_tunnel->pdr;
        ogs_assert(pdr);

        pdr->outer_header_removal_len = 1;
        pdr->outer_header_removal.description =
            OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IP;

        far = ul_tunnel->far;
        ogs_assert(far);

        far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;

        ogs_assert(OGS_OK ==
            ogs_pfcp_ip_to_outer_header_creation(&ul_tunnel->remote_ip,
                &far->outer_header_creation, &far->outer_header_creation_len));
        far->outer_header_creation.teid = ul_tunnel->remote_teid;
    }

    /* Receive Control Plane(UL) : PGW-S5C */
    pgw_s5c_teid = rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.data;
    ogs_assert(pgw_s5c_teid);
    sess->pgw_s5c_teid = be32toh(pgw_s5c_teid->teid);

    pgw = ogs_gtp_node_find_by_f_teid(&sgwc_self()->pgw_s5c_list, pgw_s5c_teid);
    if (!pgw) {
        pgw = ogs_gtp_node_add_by_f_teid(
                &sgwc_self()->pgw_s5c_list,
                pgw_s5c_teid, ogs_gtp_self()->gtpc_port);
        ogs_assert(pgw);

        rv = ogs_gtp_connect(
                ogs_gtp_self()->gtpc_sock, ogs_gtp_self()->gtpc_sock6, pgw);
        ogs_assert(rv == OGS_OK);
    }
    /* Setup GTP Node */
    OGS_SETUP_GTP_NODE(sess, pgw);

    ogs_assert(ogs_list_count(&sess->bearer_list));
    ogs_assert(OGS_OK ==
        sgwc_pfcp_send_session_modification_request(
            sess, s11_xact->id, gtpbuf,
            OGS_PFCP_MODIFY_UL_ONLY|
            OGS_PFCP_MODIFY_OUTER_HEADER_REMOVAL|
            OGS_PFCP_MODIFY_ACTIVATE));
}

void sgwc_s5c_handle_modify_bearer_response(
        sgwc_sess_t *sess, ogs_gtp_xact_t *s5c_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int rv;
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    uint8_t session_cause = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    int modify_action;

    sgwc_ue_t *sgwc_ue = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_gtp_xact_t *s11_xact = NULL;
    ogs_gtp2_modify_bearer_response_t *rsp = NULL;

    ogs_assert(message);
    rsp = &message->modify_bearer_response;
    ogs_assert(rsp);

    ogs_debug("Modify Bearer Response");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(s5c_xact);
    s11_xact = ogs_gtp_xact_find_by_id(s5c_xact->assoc_xact_id);
    ogs_assert(s11_xact);
    modify_action = s5c_xact->modify_action;

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);

    /************************
     * Getting Cause Value
     ************************/
    if (rsp->cause.presence && rsp->cause.data) {
        ogs_gtp2_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);
        session_cause = cause->value;
    }

    /************************
     * Check Session Context
     *
     * - Session could be deleted before a message is received from SMF.
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_error("No Context in TEID [Cause:%d]", session_cause);
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    } else {
        sgwc_ue = sgwc_ue_find_by_id(sess->sgwc_ue_id);
        ogs_assert(sgwc_ue);
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        if (modify_action == OGS_GTP_MODIFY_IN_PATH_SWITCH_REQUEST)
            ogs_gtp_send_error_message(
                    s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                    OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE, cause_value);
        else
            ogs_gtp_send_error_message(
                    s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                    OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /*****************************************
     * Check Mandatory/Conditional IE Missing
     *****************************************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (rsp->cause.presence == 0) {
        ogs_error("No Cause [VALUE:%d]", session_cause);
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        if (modify_action == OGS_GTP_MODIFY_IN_PATH_SWITCH_REQUEST)
            ogs_gtp_send_error_message(
                    s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                    OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE, cause_value);
        else
            ogs_gtp_send_error_message(
                    s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                    OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /********************
     * Check Cause Value
     ********************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (session_cause != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_error("GTP Cause [VALUE:%d]", session_cause);
        if (modify_action == OGS_GTP_MODIFY_IN_PATH_SWITCH_REQUEST)
            ogs_gtp_send_error_message(
                    s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                    OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE, session_cause);
        else
            ogs_gtp_send_error_message(
                    s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                    OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE, session_cause);
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

    if (modify_action == OGS_GTP_MODIFY_IN_PATH_SWITCH_REQUEST) {
        ogs_assert(OGS_OK ==
            sgwc_gtp_send_create_session_response(sess, s11_xact));
    } else {
        message->h.type = OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE;
        message->h.teid = sgwc_ue->mme_s11_teid;

        pkbuf = ogs_gtp2_build_msg(message);
        if (!pkbuf) {
            ogs_error("ogs_gtp2_build_msg() failed");
            return;
        }

        rv = ogs_gtp_xact_update_tx(s11_xact, &message->h, pkbuf);
        if (rv != OGS_OK) {
            ogs_error("ogs_gtp_xact_update_tx() failed");
            return;
        }

        rv = ogs_gtp_xact_commit(s11_xact);
        ogs_expect(rv == OGS_OK);
    }
}

void sgwc_s5c_handle_delete_session_response(
        sgwc_sess_t *sess, ogs_gtp_xact_t *s5c_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int rv;
    uint8_t cause_value = OGS_GTP2_CAUSE_UNDEFINED_VALUE;
    uint8_t session_cause = OGS_GTP2_CAUSE_UNDEFINED_VALUE;

    sgwc_ue_t *sgwc_ue = NULL;

    ogs_gtp_xact_t *s11_xact = NULL;
    ogs_gtp2_delete_session_response_t *rsp = NULL;

    ogs_assert(message);
    rsp = &message->delete_session_response;
    ogs_assert(rsp);

    ogs_debug("Delete Session Response");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(s5c_xact);
    s11_xact = ogs_gtp_xact_find_by_id(s5c_xact->assoc_xact_id);
    ogs_assert(s11_xact);

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);

    /************************
     * Getting Cause Value
     ************************/
    if (rsp->cause.presence && rsp->cause.data) {
        ogs_gtp2_cause_t *cause = rsp->cause.data;
        ogs_assert(cause);
        session_cause = cause->value;
    }

    /************************
     * Check Session Context
     *
     * - Session could be deleted before a message is received from SMF.
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_error("No Context in TEID [Cause:%d]", session_cause);
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    } else {
        sgwc_ue = sgwc_ue_find_by_id(sess->sgwc_ue_id);
        ogs_assert(sgwc_ue);
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE, cause_value);
        return;
    }

    /********************
     * Check Cause Value
     ********************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (rsp->cause.presence == 0) {
        ogs_error("No Cause [VALUE:%d]", session_cause);
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (session_cause != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_error("GTP Cause [VALUE:%d] - Ignored", session_cause);
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(sess);
    ogs_assert(sgwc_ue);

    /* Remove a pgw session */
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    /*
     * 1. MME sends Delete Session Request to SGW/SMF.
     * 2. SMF sends Delete Session Response to SGW/MME.
     */
    ogs_assert(OGS_OK ==
        sgwc_pfcp_send_session_deletion_request(sess, s11_xact->id, gtpbuf));
}

void sgwc_s5c_handle_create_bearer_request(
        sgwc_sess_t *sess, ogs_gtp_xact_t *s5c_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int rv;
    uint8_t cause_value = 0;

    sgwc_ue_t *sgwc_ue = NULL;
    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *ul_tunnel = NULL;
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;

    ogs_gtp2_create_bearer_request_t *req = NULL;
    ogs_gtp2_f_teid_t *pgw_s5u_teid = NULL;

    ogs_assert(message);
    req = &message->create_bearer_request;
    ogs_assert(req);

    ogs_debug("Create Bearer Request");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(s5c_xact);

    /************************
     * Check Session Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_error("No Context in TEID");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    } else {
        sgwc_ue = sgwc_ue_find_by_id(sess->sgwc_ue_id);
        ogs_assert(sgwc_ue);
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP2_CREATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /*****************************************
     * Check Mandatory/Conditional IE Missing
     *****************************************/
    ogs_assert(cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (req->linked_eps_bearer_id.presence == 0) {
        ogs_error("No Linked EBI");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.s4_u_sgsn_f_teid.presence == 0) {
        ogs_error("No GTP TEID");
        cause_value = OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP2_CREATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(sess);
    ogs_assert(sgwc_ue);

    bearer = sgwc_bearer_add(sess);
    ogs_assert(bearer);
    ul_tunnel = sgwc_ul_tunnel_in_bearer(bearer);
    ogs_assert(ul_tunnel);

    /* Receive Data Plane(UL) : PGW-S5U */
    pgw_s5u_teid = req->bearer_contexts.s4_u_sgsn_f_teid.data;
    ogs_assert(pgw_s5u_teid);
    ul_tunnel->remote_teid = be32toh(pgw_s5u_teid->teid);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    ogs_debug("    SGW_S5U_TEID[%d] PGW_S5U_TEID[%d]",
        ul_tunnel->local_teid, ul_tunnel->remote_teid);

    rv = ogs_gtp2_f_teid_to_ip(pgw_s5u_teid, &ul_tunnel->remote_ip);
    if (rv != OGS_OK) {
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP2_CREATE_BEARER_RESPONSE_TYPE,
                OGS_GTP2_CAUSE_MANDATORY_IE_MISSING);
        return;
    }

    pdr = ul_tunnel->pdr;
    ogs_assert(pdr);

    pdr->outer_header_removal_len = 1;
    pdr->outer_header_removal.description =
        OGS_PFCP_OUTER_HEADER_REMOVAL_GTPU_UDP_IP;

    far = ul_tunnel->far;
    ogs_assert(far);

    far->apply_action = OGS_PFCP_APPLY_ACTION_FORW;

    ogs_assert(OGS_OK ==
        ogs_pfcp_ip_to_outer_header_creation(&ul_tunnel->remote_ip,
            &far->outer_header_creation, &far->outer_header_creation_len));
    far->outer_header_creation.teid = ul_tunnel->remote_teid;

    ogs_assert(OGS_OK ==
        sgwc_pfcp_send_bearer_modification_request(
            bearer, s5c_xact->id, gtpbuf,
            OGS_PFCP_MODIFY_UL_ONLY|OGS_PFCP_MODIFY_CREATE));
}

void sgwc_s5c_handle_update_bearer_request(
        sgwc_sess_t *sess, ogs_gtp_xact_t *s5c_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int rv;
    uint8_t cause_value = 0;
    ogs_gtp_xact_t *s11_xact = NULL;
    ogs_gtp2_update_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    sgwc_ue_t *sgwc_ue = NULL;
    sgwc_bearer_t *bearer = NULL;

    ogs_assert(message);
    req = &message->update_bearer_request;
    ogs_assert(req);

    ogs_debug("Update Bearer Request");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(s5c_xact);

    /************************
     * Check Session Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_error("No Context in TEID");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    } else {
        sgwc_ue = sgwc_ue_find_by_id(sess->sgwc_ue_id);
        ogs_assert(sgwc_ue);

        if (req->bearer_contexts.presence == 0) {
            ogs_error("No Bearer");
            cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        }
        if (req->bearer_contexts.eps_bearer_id.presence == 0) {
            ogs_error("No EPS Bearer ID");
            cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
        }

        if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
            bearer = sgwc_bearer_find_by_sess_ebi(sess,
                        req->bearer_contexts.eps_bearer_id.u8);
            if (!bearer) {
                ogs_error("No Context for EPS Bearer ID[%d]",
                        req->bearer_contexts.eps_bearer_id.u8);
                cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
            }
        }
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP2_UPDATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(sess);
    ogs_assert(bearer);
    ogs_assert(sgwc_ue);
    ogs_assert(sgwc_ue->gnode);

    ogs_debug("    EBI[%d]", bearer->ebi);
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    message->h.type = OGS_GTP2_UPDATE_BEARER_REQUEST_TYPE;
    message->h.teid = sgwc_ue->mme_s11_teid;

    pkbuf = ogs_gtp2_build_msg(message);
    if (!pkbuf) {
        ogs_error("ogs_gtp2_build_msg() failed");
        return;
    }

    s11_xact = ogs_gtp_xact_find_by_id(s5c_xact->assoc_xact_id);
    if (!s11_xact) {
        s11_xact = ogs_gtp_xact_local_create(
                sgwc_ue->gnode, &message->h, pkbuf, bearer_timeout,
                OGS_UINT_TO_POINTER(bearer->id));
        if (!s11_xact) {
            ogs_error("ogs_gtp_xact_local_create() failed");
            return;
        }
        s11_xact->local_teid = sgwc_ue->sgw_s11_teid;

        ogs_gtp_xact_associate(s5c_xact, s11_xact);
    } else {
        rv = ogs_gtp_xact_update_tx(s11_xact, &message->h, pkbuf);
        if (rv != OGS_OK) {
            ogs_error("ogs_gtp_xact_update_tx() failed");
            return;
        }
    }
    s11_xact->local_teid = sgwc_ue->sgw_s11_teid;

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);

    ogs_debug("Update Bearer Request : SGW <-- PGW");
}

void sgwc_s5c_handle_delete_bearer_request(
        sgwc_sess_t *sess, ogs_gtp_xact_t *s5c_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    int rv;
    uint8_t cause_value = 0;
    ogs_gtp_xact_t *s11_xact = NULL;
    ogs_gtp2_delete_bearer_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    sgwc_ue_t *sgwc_ue = NULL;
    sgwc_bearer_t *bearer = NULL;

    ogs_assert(message);
    req = &message->delete_bearer_request;
    ogs_assert(req);

    ogs_debug("Delete Bearer Request");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(s5c_xact);

    /************************
     * Check Session Context
     ************************/
    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_error("No Context in TEID");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    } else {
        sgwc_ue = sgwc_ue_find_by_id(sess->sgwc_ue_id);
        ogs_assert(sgwc_ue);

        if (req->linked_eps_bearer_id.presence == 0 &&
            req->eps_bearer_ids.presence == 0) {
            ogs_error("No Linked EBI or EPS Bearer ID");
            cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
        }

        if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
            uint8_t ebi;

            if (req->linked_eps_bearer_id.presence) {
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
                ebi = req->linked_eps_bearer_id.u8;
            } else if (req->eps_bearer_ids.presence) {
               /*
                * << EPS Bearer IDs >>
                *
                * 1. MME sends Bearer Resource Command to SGW/SMF.
                * 2. SMF sends Delete Bearer Request(DEDICATED BEARER)
                *    to SGW/MME.
                * 3. MME sends Delete Bearer Response(DEDICATED BEARER)
                *    to SGW/SMF.
                *
                * OR
                *
                * 1. SMF sends Delete Bearer Request(DEDICATED BEARER)
                *    to SGW/MME.
                * 2. MME sends Delete Bearer Response(DEDICATED BEARER)
                *    to SGW/SMF.
                */
                ebi = req->eps_bearer_ids.u8;
            } else
                ogs_assert_if_reached();

            bearer = sgwc_bearer_find_by_sess_ebi(sess, ebi);
            if (!bearer) {
                ogs_error("No Context for EPS Bearer ID[%d]", ebi);
                cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
            }
        }
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP2_DELETE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /********************
     * Check ALL Context
     ********************/
    ogs_assert(sess);
    ogs_assert(bearer);
    ogs_assert(sgwc_ue);
    ogs_assert(sgwc_ue->gnode);

    ogs_debug("    EBI[%d]", bearer->ebi);
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgwc_ue->mme_s11_teid, sgwc_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    message->h.type = OGS_GTP2_DELETE_BEARER_REQUEST_TYPE;
    message->h.teid = sgwc_ue->mme_s11_teid;

    pkbuf = ogs_gtp2_build_msg(message);
    if (!pkbuf) {
        ogs_error("ogs_gtp2_build_msg() failed");
        return;
    }

    s11_xact = ogs_gtp_xact_find_by_id(s5c_xact->assoc_xact_id);
    if (!s11_xact) {
       /*
        * 1. SMF sends Delete Bearer Request(DEFAULT BEARER) to SGW/MME.
        * 2. MME sends Delete Bearer Response to SGW/SMF.
        *
        * OR
        *
        * 1. SMF sends Delete Bearer Request(DEFAULT BEARER) to ePDG.
        * 2. ePDG sends Delete Bearer Response(DEFAULT BEARER) to SMF.
        *
        * OR
        *
        * 1. SMF sends Delete Bearer Request(DEDICATED BEARER) to SGW/MME.
        * 2. MME sends Delete Bearer Response(DEDICATED BEARER) to SGW/SMF.
        */
        s11_xact = ogs_gtp_xact_local_create(
                sgwc_ue->gnode, &message->h, pkbuf, bearer_timeout,
                OGS_UINT_TO_POINTER(bearer->id));
        if (!s11_xact) {
            ogs_error("ogs_gtp_xact_local_create() failed");
            return;
        }
        s11_xact->local_teid = sgwc_ue->sgw_s11_teid;

        ogs_gtp_xact_associate(s5c_xact, s11_xact);
    } else {
       /*
        * 1. MME sends Bearer Resource Command to SGW/SMF.
        * 2. SMF sends Delete Bearer Request(DEDICATED BEARER) to SGW/MME.
        * 3. MME sends Delete Bearer Response(DEDICATED BEARER) to SGW/SMF.
        */
        rv = ogs_gtp_xact_update_tx(s11_xact, &message->h, pkbuf);
        if (rv != OGS_OK) {
            ogs_error("ogs_gtp_xact_update_tx() failed");
            return;
        }
    }
    s11_xact->local_teid = sgwc_ue->sgw_s11_teid;

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);
}

void sgwc_s5c_handle_bearer_resource_failure_indication(
        sgwc_sess_t *sess, ogs_gtp_xact_t *s5c_xact,
        ogs_pkbuf_t *gtpbuf, ogs_gtp2_message_t *message)
{
    uint8_t cause_value = 0;
    ogs_gtp_xact_t *s11_xact = NULL;
    ogs_gtp2_bearer_resource_failure_indication_t *ind = NULL;

    sgwc_ue_t *sgwc_ue = NULL;

    ogs_assert(message);
    ind = &message->bearer_resource_failure_indication;
    ogs_assert(ind);

    ogs_debug("Bearer Resource Failure Indication");

    /********************
     * Check Transaction
     ********************/
    ogs_assert(s5c_xact);
    s11_xact = ogs_gtp_xact_find_by_id(s5c_xact->assoc_xact_id);
    ogs_assert(s11_xact);

    /************************
     * Check Session Context
     *
     * - Session could be deleted before a message is received from SMF.
     ************************/
    if (!sess) {
        ogs_error("No Context in TEID");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    } else {
        sgwc_ue = sgwc_ue_find_by_id(sess->sgwc_ue_id);
        ogs_assert(sgwc_ue);
    }

    /********************
     * Check Cause Value
     ********************/
    if (ind->cause.presence) {
        ogs_gtp2_cause_t *cause = ind->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
    } else {
        ogs_error("No Cause");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    ogs_gtp_send_error_message(s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
            OGS_GTP2_BEARER_RESOURCE_FAILURE_INDICATION_TYPE, cause_value);
}
