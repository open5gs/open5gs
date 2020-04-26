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

#include "sgw-event.h"
#include "sgw-context.h"
#include "sgw-gtp-path.h"
#include "sgw-s11-handler.h"

static void timeout(ogs_gtp_xact_t *xact, void *data)
{
    sgw_sess_t *sess = data;
    sgw_ue_t *sgw_ue = NULL;
    uint8_t type = 0;

    ogs_assert(xact);
    ogs_assert(sess);
    sgw_ue = sess->sgw_ue;
    ogs_assert(sgw_ue);

    type = xact->seq[0].type;

    ogs_error("GTP Timeout : IMSI[%s] Message-Type[%d]",
            sgw_ue->imsi_bcd, type);
}

void sgw_s11_handle_create_session_request(ogs_gtp_xact_t *s11_xact,
        sgw_ue_t *sgw_ue, ogs_gtp_message_t *message)
{
    int rv;
    uint8_t cause_value = 0;
    uint16_t decoded;
    ogs_gtp_create_session_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_f_teid_t *mme_s11_teid = NULL;
    ogs_gtp_f_teid_t *pgw_s5c_teid = NULL;
    int len = 0;
    ogs_gtp_node_t *pgw = NULL;
    ogs_gtp_f_teid_t sgw_s5c_teid, sgw_s5u_teid;
    ogs_gtp_uli_t uli;

    ogs_gtp_xact_t *s5c_xact = NULL;
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *s5u_tunnel = NULL;

    char apn[OGS_MAX_APN_LEN];

    ogs_assert(s11_xact);
    ogs_assert(message);

    ogs_debug("[SGW] Create Session Request");

    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;
    req = &message->create_session_request;

    if (!sgw_ue) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (req->imsi.presence == 0) {
        ogs_error("No IMSI");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts_to_be_created.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts_to_be_created.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->access_point_name.presence == 0) {
        ogs_error("No APN");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->sender_f_teid_for_control_plane.presence == 0) {
        ogs_error("No Sender F-TEID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->pgw_s5_s8_address_for_control_plane_or_pmip.presence == 0) {
        ogs_error("No PGW IP");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->user_location_information.presence == 0) {
        ogs_error("No User Location Inforamtion");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s11_xact, sgw_ue ? sgw_ue->mme_s11_teid : 0,
                OGS_GTP_CREATE_SESSION_RESPONSE_TYPE, cause_value);
        return;
    }

    ogs_fqdn_parse(apn,
            req->access_point_name.data, req->access_point_name.len);
    sess = sgw_sess_find_by_ebi(sgw_ue,
            req->bearer_contexts_to_be_created.eps_bearer_id.u8);
    if (sess) {
        ogs_warn("OLD Session Release [IMSI:%s,APN:%s]",
                sgw_ue->imsi_bcd, sess->pdn.apn);
        sgw_sess_remove(sess);
    }
    sess = sgw_sess_add(sgw_ue, apn,
            req->bearer_contexts_to_be_created.eps_bearer_id.u8);
    ogs_assert(sess);

    bearer = sgw_default_bearer_in_sess(sess);
    ogs_assert(bearer);
    s5u_tunnel = sgw_s5u_tunnel_in_bearer(bearer);
    ogs_assert(s5u_tunnel);

    /* Receive Control Plane(DL) : MME-S11 */
    mme_s11_teid = req->sender_f_teid_for_control_plane.data;
    ogs_assert(mme_s11_teid);
    sgw_ue->mme_s11_teid = ntohl(mme_s11_teid->teid);

    /* Send Control Plane(DL) : SGW-S5C */
    memset(&sgw_s5c_teid, 0, sizeof(ogs_gtp_f_teid_t));
    sgw_s5c_teid.interface_type = OGS_GTP_F_TEID_S5_S8_SGW_GTP_C;
    sgw_s5c_teid.teid = htonl(sess->sgw_s5c_teid);
    rv = ogs_gtp_sockaddr_to_f_teid(
        sgw_self()->gtpc_addr, sgw_self()->gtpc_addr6, &sgw_s5c_teid, &len);
    ogs_assert(rv == OGS_OK);
    req->sender_f_teid_for_control_plane.presence = 1;
    req->sender_f_teid_for_control_plane.data = &sgw_s5c_teid;
    req->sender_f_teid_for_control_plane.len = len;

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    ogs_debug("    SGW_S5U_TEID[%d] PGW_S5U_TEID[%d]",
        s5u_tunnel->local_teid, s5u_tunnel->remote_teid);

    pgw_s5c_teid = req->pgw_s5_s8_address_for_control_plane_or_pmip.data;
    ogs_assert(pgw_s5c_teid);

    pgw = ogs_gtp_node_find_by_f_teid(&sgw_self()->pgw_s5c_list, pgw_s5c_teid);
    if (!pgw) {
        pgw = ogs_gtp_node_add_by_f_teid(
            &sgw_self()->pgw_s5c_list, pgw_s5c_teid, sgw_self()->gtpc_port,
            ogs_config()->parameter.no_ipv4,
            ogs_config()->parameter.no_ipv6,
            ogs_config()->parameter.prefer_ipv4);
        ogs_assert(pgw);

        rv = ogs_gtp_connect(sgw_self()->gtpc_sock, sgw_self()->gtpc_sock6, pgw);
        ogs_assert(rv == OGS_OK);
    }
    /* Setup GTP Node */
    OGS_SETUP_GTP_NODE(sess, pgw);

    /* Remove PGW-S5C */
    req->pgw_s5_s8_address_for_control_plane_or_pmip.presence = 0;

    /* Data Plane(DL) : SGW-S5U */
    memset(&sgw_s5u_teid, 0, sizeof(ogs_gtp_f_teid_t));
    sgw_s5u_teid.teid = htonl(s5u_tunnel->local_teid);
    sgw_s5u_teid.interface_type = OGS_GTP_F_TEID_S5_S8_SGW_GTP_U;
    rv = ogs_gtp_sockaddr_to_f_teid(
        sgw_self()->gtpu_addr,  sgw_self()->gtpu_addr6, &sgw_s5u_teid, &len);
    ogs_assert(rv == OGS_OK);
    req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.presence = 1;
    req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.data = &sgw_s5u_teid;
    req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.len = len;

    /* Set User Location Information */
    decoded = ogs_gtp_parse_uli(&uli, &req->user_location_information);
    ogs_assert(req->user_location_information.len == decoded);
    memcpy(&bearer->tai.plmn_id, &uli.tai.plmn_id, sizeof(uli.tai.plmn_id));
    bearer->tai.tac = uli.tai.tac;
    memcpy(&bearer->e_cgi.plmn_id, &uli.e_cgi.plmn_id, sizeof(uli.e_cgi.plmn_id));
    bearer->e_cgi.cell_id = uli.e_cgi.cell_id;

    message->h.type = OGS_GTP_CREATE_SESSION_REQUEST_TYPE;
    message->h.teid = sess->pgw_s5c_teid;

    pkbuf = ogs_gtp_build_msg(message);
    ogs_expect_or_return(pkbuf);

    s5c_xact = ogs_gtp_xact_local_create(
            sess->gnode, &message->h, pkbuf, timeout, sess);
    ogs_expect_or_return(s5c_xact);

    ogs_gtp_xact_associate(s11_xact, s5c_xact);

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);
}

void sgw_s11_handle_modify_bearer_request(ogs_gtp_xact_t *s11_xact,
    sgw_ue_t *sgw_ue, ogs_gtp_modify_bearer_request_t *req)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];

    uint16_t decoded;
    ogs_gtp_node_t *enb = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *s1u_tunnel = NULL;
    ogs_gtp_modify_bearer_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_message_t message;
    
    ogs_gtp_cause_t cause;
    ogs_gtp_f_teid_t *enb_s1u_teid = NULL;
    ogs_gtp_uli_t uli;

    ogs_assert(s11_xact);
    ogs_assert(req);

    ogs_debug("[SGW] Modify Bearer Request");

    memset(&cause, 0, sizeof(cause));
    cause.value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    if (req->bearer_contexts_to_be_modified.presence == 0) {
        ogs_error("No Bearer");
        cause.value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts_to_be_modified.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause.value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (sgw_ue && cause.value == OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        bearer = sgw_bearer_find_by_ue_ebi(sgw_ue,
                    req->bearer_contexts_to_be_modified.eps_bearer_id.u8);
        ogs_assert(bearer);
    } 

    if (!bearer) {
        ogs_warn("No Context");
        cause.value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.presence == 0) {
        ogs_error("No eNB TEID");
        cause.value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause.value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s11_xact, sgw_ue ? sgw_ue->mme_s11_teid : 0,
                OGS_GTP_MODIFY_BEARER_RESPONSE_TYPE, cause.value);
        return;
    }

    rsp = &message.modify_bearer_response;
    memset(&message, 0, sizeof(ogs_gtp_message_t));

    rsp->cause.presence = 1;
    rsp->cause.data = &cause;
    rsp->cause.len = sizeof(cause);

    s1u_tunnel = sgw_s1u_tunnel_in_bearer(bearer);
    ogs_assert(s1u_tunnel);

    /* Data Plane(DL) : eNB-S1U */
    enb_s1u_teid =
        req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.data;
    s1u_tunnel->remote_teid = ntohl(enb_s1u_teid->teid);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    ENB_S1U_TEID[%d] SGW_S1U_TEID[%d]",
        s1u_tunnel->remote_teid, s1u_tunnel->local_teid);

    enb = ogs_gtp_node_find_by_f_teid(
            &sgw_self()->enb_s1u_list, enb_s1u_teid);
    if (!enb) {
        enb = ogs_gtp_node_add_by_f_teid(
            &sgw_self()->enb_s1u_list, enb_s1u_teid, sgw_self()->gtpu_port,
            ogs_config()->parameter.no_ipv4,
            ogs_config()->parameter.no_ipv6,
            ogs_config()->parameter.prefer_ipv4);
        ogs_assert(enb);

        rv = ogs_gtp_connect(sgw_self()->gtpu_sock, sgw_self()->gtpu_sock6, enb);
        ogs_assert(rv == OGS_OK);
    }

    /* Copy Bearer-Contexts-Modified from Modify-Bearer-Request
     *
     * TS 29.274 Table 7.2.7-2
     * NOTE 1: The SGW shall not change its F-TEID for a given interface
     * during the Handover, Service Request, E-UTRAN Initial Attach,
     * UE Requested PDN connectivity and PDP Context Activation procedures.
     * The SGW F-TEID shall be same for S1-U, S4-U and S12. During Handover
     * and Service Request the target eNodeB/RNC/SGSN may use a different
     * IP type than the one used by the source eNodeB/RNC/SGSN.
     * In order to support such a scenario, the SGW F-TEID should contain
     * both an IPv4 address and an IPv6 address
     * (see also subclause 8.22 "F-TEID").
     */
    rsp->bearer_contexts_modified.presence = 1;
    rsp->bearer_contexts_modified.eps_bearer_id.presence = 1;
    rsp->bearer_contexts_modified.eps_bearer_id.u8 =
        req->bearer_contexts_to_be_modified.eps_bearer_id.u8;
    rsp->bearer_contexts_modified.s1_u_enodeb_f_teid.presence = 1;
    rsp->bearer_contexts_modified.s1_u_enodeb_f_teid.data =
        req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.data;
    rsp->bearer_contexts_modified.s1_u_enodeb_f_teid.len =
        req->bearer_contexts_to_be_modified.s1_u_enodeb_f_teid.len;

    rsp->bearer_contexts_modified.cause.presence = 1;
    rsp->bearer_contexts_modified.cause.len = sizeof(cause);
    rsp->bearer_contexts_modified.cause.data = &cause;

    /* if GTP Node changes, End Marker is sent out or not */
    if (req->user_location_information.presence == 1) {
        /* Set User Location Information */
        decoded = ogs_gtp_parse_uli(&uli, &req->user_location_information);
        ogs_assert(req->user_location_information.len == decoded);
        memcpy(&bearer->tai.plmn_id, &uli.tai.plmn_id,
                sizeof(uli.tai.plmn_id));
        bearer->tai.tac = uli.tai.tac;
        memcpy(&bearer->e_cgi.plmn_id, &uli.e_cgi.plmn_id,
                sizeof(uli.e_cgi.plmn_id));
        bearer->e_cgi.cell_id = uli.e_cgi.cell_id;
        ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
                ogs_plmn_id_hexdump(&bearer->tai.plmn_id),
                bearer->tai.tac);
        ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
                ogs_plmn_id_hexdump(&bearer->e_cgi.plmn_id),
                bearer->e_cgi.cell_id);
    }

    if (s1u_tunnel->gnode && s1u_tunnel->gnode != enb) {
        ogs_assert(s1u_tunnel->gnode->sock);

        ogs_debug("[SGW] SEND End Marker to ENB[%s]: TEID[0x%x]",
            OGS_ADDR(&s1u_tunnel->gnode->addr, buf),
            s1u_tunnel->remote_teid);
        sgw_gtp_send_end_marker(s1u_tunnel);
    }

    /* Setup GTP Node */
    OGS_SETUP_GTP_NODE(s1u_tunnel, enb);

    /* Reset UE state */
    SGW_RESET_UE_STATE(sgw_ue, SGW_S1U_INACTIVE);

    message.h.type = OGS_GTP_MODIFY_BEARER_RESPONSE_TYPE;
    message.h.teid = sgw_ue->mme_s11_teid;

    pkbuf = ogs_gtp_build_msg(&message);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(s11_xact, &message.h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);
}

void sgw_s11_handle_delete_session_request(ogs_gtp_xact_t *s11_xact,
    sgw_ue_t *sgw_ue, ogs_gtp_message_t *message)
{
    int rv;
    uint8_t cause_value = 0;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_xact_t *s5c_xact = NULL;
    sgw_sess_t *sess = NULL;
    ogs_gtp_delete_session_request_t *req = NULL;

    ogs_assert(s11_xact);
    ogs_assert(message);

    ogs_debug("[SGW] Delete Session Request");

    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;
    req = &message->delete_session_request;

    if (!sgw_ue) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (req->linked_eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s11_xact, sgw_ue ? sgw_ue->mme_s11_teid : 0,
                OGS_GTP_DELETE_SESSION_RESPONSE_TYPE, cause_value);
        return;
    }

    sess = sgw_sess_find_by_ebi(sgw_ue, req->linked_eps_bearer_id.u8);
    ogs_assert(sess);
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    message->h.type = OGS_GTP_DELETE_SESSION_REQUEST_TYPE;
    message->h.teid = sess->pgw_s5c_teid;

    pkbuf = ogs_gtp_build_msg(message);
    ogs_expect_or_return(pkbuf);

    s5c_xact = ogs_gtp_xact_local_create(
            sess->gnode, &message->h, pkbuf, timeout, sess);
    ogs_expect_or_return(s5c_xact);

    ogs_gtp_xact_associate(s11_xact, s5c_xact);

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);
}

void sgw_s11_handle_create_bearer_response(ogs_gtp_xact_t *s11_xact,
    sgw_ue_t *sgw_ue, ogs_gtp_message_t *message)
{
    int rv;
    uint8_t cause_value;
    uint16_t decoded;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_node_t *enb = NULL;
    ogs_gtp_xact_t *s5c_xact = NULL;
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *s1u_tunnel = NULL, *s5u_tunnel = NULL;
    ogs_gtp_create_bearer_response_t *req = NULL;

    ogs_gtp_f_teid_t *sgw_s1u_teid = NULL, *enb_s1u_teid = NULL;
    ogs_gtp_f_teid_t sgw_s5u_teid, pgw_s5u_teid;
    int len;
    ogs_gtp_uli_t uli;

    ogs_assert(s11_xact);
    s5c_xact = s11_xact->assoc_xact;
    ogs_assert(s5c_xact);
    ogs_assert(message);

    ogs_debug("[SGW] Cerate Bearer Response");

    if (!sgw_ue) {
        sgw_sess_t *sess = NULL;

        ogs_warn("No Context in TEID");
        sess = s11_xact->data;
        ogs_assert(sess);
        sgw_ue = sess->sgw_ue;
        ogs_assert(sgw_ue);
    }

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);

    req = &message->create_bearer_response;

    if (req->cause.presence) {
        ogs_gtp_cause_t *cause = req->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value == OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
            if (req->bearer_contexts.cause.presence) {
                cause = req->bearer_contexts.cause.data;
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

    if (req->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.s1_u_enodeb_f_teid.presence == 0) {
        ogs_error("No eNB TEID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.s4_u_sgsn_f_teid.presence == 0) {
        ogs_error("No SGW TEID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->user_location_information.presence == 0) {
        ogs_error("No User Location Inforamtion");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP_CREATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    /* Correlate with SGW-S1U-TEID */
    sgw_s1u_teid = req->bearer_contexts.s4_u_sgsn_f_teid.data;
    ogs_assert(sgw_s1u_teid);
    req->bearer_contexts.s4_u_sgsn_f_teid.presence = 0;

    /* Find the Tunnel by SGW-S1U-TEID */
    s1u_tunnel = sgw_tunnel_find_by_teid(ntohl(sgw_s1u_teid->teid));
    ogs_assert(s1u_tunnel);
    bearer = s1u_tunnel->bearer;
    ogs_assert(bearer);
    s5u_tunnel = sgw_s5u_tunnel_in_bearer(bearer);
    ogs_assert(s5u_tunnel);
    sess = bearer->sess;
    ogs_assert(sess);

    /* Set EBI */
    bearer->ebi = req->bearer_contexts.eps_bearer_id.u8;

    /* Data Plane(DL) : eNB-S1U */
    enb_s1u_teid = req->bearer_contexts.s1_u_enodeb_f_teid.data;
    s1u_tunnel->remote_teid = ntohl(enb_s1u_teid->teid);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    ogs_debug("    ENB_S1U_TEID[%d] SGW_S1U_TEID[%d]",
        s1u_tunnel->remote_teid, s1u_tunnel->local_teid);
    ogs_debug("    SGW_S5U_TEID[%d] PGW_S5U_TEID[%d]",
        s5u_tunnel->local_teid, s5u_tunnel->remote_teid);

    enb = ogs_gtp_node_find_by_f_teid(&sgw_self()->enb_s1u_list, enb_s1u_teid);
    if (!enb) {
        enb = ogs_gtp_node_add_by_f_teid(
            &sgw_self()->enb_s1u_list, enb_s1u_teid, sgw_self()->gtpu_port,
            ogs_config()->parameter.no_ipv4,
            ogs_config()->parameter.no_ipv6,
            ogs_config()->parameter.prefer_ipv4);
        ogs_assert(enb);

        rv = ogs_gtp_connect(sgw_self()->gtpu_sock, sgw_self()->gtpu_sock6, enb);
        ogs_assert(rv == OGS_OK);
    }
    /* Setup GTP Node */
    OGS_SETUP_GTP_NODE(s1u_tunnel, enb);

    /* Remove S1U-F-TEID */
    req->bearer_contexts.s1_u_enodeb_f_teid.presence = 0;

    decoded = ogs_gtp_parse_uli(&uli, &req->user_location_information);
    ogs_assert(req->user_location_information.len == decoded);
    memcpy(&bearer->tai.plmn_id, &uli.tai.plmn_id, sizeof(uli.tai.plmn_id));
    bearer->tai.tac = uli.tai.tac;
    memcpy(&bearer->e_cgi.plmn_id, &uli.e_cgi.plmn_id, sizeof(uli.e_cgi.plmn_id));
    bearer->e_cgi.cell_id = uli.e_cgi.cell_id;

    /* Reset UE state */
    SGW_RESET_UE_STATE(sgw_ue, SGW_S1U_INACTIVE);

    /* Data Plane(DL) : SGW-S5U */
    memset(&sgw_s5u_teid, 0, sizeof(ogs_gtp_f_teid_t));
    sgw_s5u_teid.interface_type = OGS_GTP_F_TEID_S5_S8_SGW_GTP_U;
    sgw_s5u_teid.teid = htonl(s5u_tunnel->local_teid);
    rv = ogs_gtp_sockaddr_to_f_teid(
        sgw_self()->gtpu_addr,  sgw_self()->gtpu_addr6, &sgw_s5u_teid, &len);
    ogs_assert(rv == OGS_OK);
    req->bearer_contexts.s5_s8_u_sgw_f_teid.presence = 1;
    req->bearer_contexts.s5_s8_u_sgw_f_teid.data = &sgw_s5u_teid;
    req->bearer_contexts.s5_s8_u_sgw_f_teid.len = len;

    /* Data Plane(DL) : PGW-S5U */
    ogs_assert(s5u_tunnel->gnode);
    pgw_s5u_teid.interface_type = OGS_GTP_F_TEID_S5_S8_PGW_GTP_U;
    pgw_s5u_teid.teid = htonl(s5u_tunnel->remote_teid);
    rv = ogs_gtp_ip_to_f_teid(&s5u_tunnel->gnode->ip, &pgw_s5u_teid, &len);
    req->bearer_contexts.s5_s8_u_pgw_f_teid.presence = 1;
    req->bearer_contexts.s5_s8_u_pgw_f_teid.data = &pgw_s5u_teid;
    req->bearer_contexts.s5_s8_u_pgw_f_teid.len = len;

    message->h.type = OGS_GTP_CREATE_BEARER_RESPONSE_TYPE;
    message->h.teid = sess->pgw_s5c_teid;

    pkbuf = ogs_gtp_build_msg(message);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(s5c_xact, &message->h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);
}

void sgw_s11_handle_update_bearer_response(ogs_gtp_xact_t *s11_xact,
    sgw_ue_t *sgw_ue, ogs_gtp_message_t *message)
{
    int rv;
    uint8_t cause_value;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_xact_t *s5c_xact = NULL;
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    ogs_gtp_update_bearer_response_t *req = NULL;

    ogs_assert(s11_xact);
    s5c_xact = s11_xact->assoc_xact;
    ogs_assert(s5c_xact);
    ogs_assert(message);

    ogs_debug("[SGW] Update Bearer Response");
    if (!sgw_ue) {
        sgw_sess_t *sess = NULL;

        ogs_warn("No Context in TEID");
        sess = s11_xact->data;
        ogs_assert(sess);
        sgw_ue = sess->sgw_ue;
        ogs_assert(sgw_ue);
    }

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);

    req = &message->update_bearer_response;

    if (req->cause.presence) {
        ogs_gtp_cause_t *cause = req->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value == OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
            if (req->bearer_contexts.cause.presence) {
                cause = req->bearer_contexts.cause.data;
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

    if (req->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (sgw_ue && cause_value == OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        bearer = sgw_bearer_find_by_ue_ebi(
                sgw_ue, req->bearer_contexts.eps_bearer_id.u8);
    }

    if (!bearer) {
        ogs_error("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP_UPDATE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    message->h.type = OGS_GTP_UPDATE_BEARER_RESPONSE_TYPE;
    message->h.teid = sess->pgw_s5c_teid;

    pkbuf = ogs_gtp_build_msg(message);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(s5c_xact, &message->h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);
}

void sgw_s11_handle_delete_bearer_response(ogs_gtp_xact_t *s11_xact,
    sgw_ue_t *sgw_ue, ogs_gtp_message_t *message)
{
    int rv;
    uint8_t cause_value;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_xact_t *s5c_xact = NULL;
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    ogs_gtp_delete_bearer_response_t *req = NULL;

    ogs_assert(s11_xact);
    s5c_xact = s11_xact->assoc_xact;
    ogs_assert(s5c_xact);
    ogs_assert(message);

    ogs_debug("[SGW] Delete Bearer Response");

    if (!sgw_ue) {
        sgw_sess_t *sess = NULL;

        ogs_warn("No Context in TEID");
        sess = s11_xact->data;
        ogs_assert(sess);
        sgw_ue = sess->sgw_ue;
        ogs_assert(sgw_ue);
    }

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);

    req = &message->delete_bearer_response;

    if (req->cause.presence) {
        ogs_gtp_cause_t *cause = req->cause.data;
        ogs_assert(cause);

        cause_value = cause->value;
        if (cause_value == OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
            if (req->bearer_contexts.cause.presence) {
                cause = req->bearer_contexts.cause.data;
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

    if (req->bearer_contexts.presence == 0) {
        ogs_error("No Bearer");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (req->bearer_contexts.eps_bearer_id.presence == 0) {
        ogs_error("No EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (sgw_ue && cause_value == OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        bearer = sgw_bearer_find_by_ue_ebi(
                sgw_ue, req->bearer_contexts.eps_bearer_id.u8);
        ogs_assert(bearer);
    }

    if (!bearer) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                OGS_GTP_DELETE_BEARER_RESPONSE_TYPE, cause_value);
        return;
    }

    sess = bearer->sess;
    ogs_assert(sess);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    message->h.type = OGS_GTP_DELETE_BEARER_RESPONSE_TYPE;
    message->h.teid = sess->pgw_s5c_teid;

    pkbuf = ogs_gtp_build_msg(message);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(s5c_xact, &message->h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);

    sgw_bearer_remove(bearer);
}

void sgw_s11_handle_release_access_bearers_request(ogs_gtp_xact_t *s11_xact, 
        sgw_ue_t *sgw_ue, ogs_gtp_release_access_bearers_request_t *req)
{
    int rv;
    ogs_gtp_release_access_bearers_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_message_t message;
    sgw_bearer_t *bearer = NULL, *next_bearer = NULL;
    sgw_tunnel_t *s1u_tunnel = NULL;
    sgw_sess_t *sess = NULL;
    
    ogs_gtp_cause_t cause;

    ogs_assert(s11_xact);
    ogs_assert(req);

    ogs_debug("[SGW] Release Access Bearers Request");

    memset(&cause, 0, sizeof(cause));
    cause.value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    if (!sgw_ue) {
        ogs_warn("No Context");
        cause.value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause.value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s11_xact, sgw_ue ? sgw_ue->mme_s11_teid : 0,
                OGS_GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE, cause.value);
        return;
    }

    rsp = &message.release_access_bearers_response;
    memset(&message, 0, sizeof(ogs_gtp_message_t));

    rsp->cause.presence = 1;
    rsp->cause.data = &cause;
    rsp->cause.len = sizeof(cause);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    /* Set UE state to S1UE_INACTIVE */
    SGW_SET_UE_STATE(sgw_ue, SGW_S1U_INACTIVE);
    /* ReSet UE state to S1UE_INACTIVE */
    SGW_RESET_UE_STATE(sgw_ue, SGW_DL_NOTI_SENT);

    /* Release S1U(DL) path */
    sess = sgw_sess_first(sgw_ue);
    while (sess) {
        bearer = ogs_list_first(&sess->bearer_list);
        while (bearer) {
            next_bearer = ogs_list_next(bearer);

            s1u_tunnel = sgw_s1u_tunnel_in_bearer(bearer);
            ogs_assert(s1u_tunnel);

            s1u_tunnel->remote_teid = 0;

            bearer = next_bearer;
        }

        sess = sgw_sess_next(sess);
    }

    message.h.type = OGS_GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE;
    message.h.teid = sgw_ue->mme_s11_teid;

    pkbuf = ogs_gtp_build_msg(&message);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(s11_xact, &message.h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);
}

void sgw_s11_handle_lo_dldata_notification(sgw_bearer_t *bearer)
{
    int rv;
    ogs_gtp_downlink_data_notification_t *noti = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_message_t message;
    sgw_ue_t *sgw_ue = NULL;
    ogs_gtp_xact_t *xact = NULL;
    /* FIXME : ARP should be retrieved from ? */
    uint8_t arp = 0x61;

    ogs_assert(bearer);

    sgw_ue = bearer->sgw_ue;
    ogs_assert(sgw_ue);

    ogs_debug("[SGW] Downlink Data Notification");
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    /* Build downlink notification message */
    noti = &message.downlink_data_notification;
    memset(&message, 0, sizeof(ogs_gtp_message_t));

    noti->eps_bearer_id.presence = 1;
    noti->eps_bearer_id.u8 = bearer->ebi;

    /* FIXME : ARP should be retrieved from ? */
    noti->allocation_retention_priority.presence = 1;
    noti->allocation_retention_priority.data = &arp;
    noti->allocation_retention_priority.len = sizeof(arp);

    message.h.type = OGS_GTP_DOWNLINK_DATA_NOTIFICATION_TYPE;
    message.h.teid = sgw_ue->mme_s11_teid;

    pkbuf = ogs_gtp_build_msg(&message);
    ogs_expect_or_return(pkbuf);

    xact = ogs_gtp_xact_local_create(
            sgw_ue->gnode, &message.h, pkbuf, NULL, sgw_ue);
    ogs_expect_or_return(xact);

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);
}

void sgw_s11_handle_downlink_data_notification_ack(
        ogs_gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue,
        ogs_gtp_downlink_data_notification_acknowledge_t *ack)
{
    int rv;
    ogs_assert(s11_xact);

    ogs_debug("[SGW] Downlink Data Notification Acknowledge");

    if (!sgw_ue) {
        ogs_warn("No context");

        sgw_ue = s11_xact->data;
        ogs_assert(sgw_ue);
    }

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

}

void sgw_s11_handle_create_indirect_data_forwarding_tunnel_request(
        ogs_gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue,
        ogs_gtp_create_indirect_data_forwarding_tunnel_request_t *req)
{
    int rv;
    ogs_gtp_create_indirect_data_forwarding_tunnel_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_message_t message;
    int i;

    ogs_gtp_node_t *enb = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *tunnel = NULL;
    
    ogs_gtp_cause_t cause;
    ogs_gtp_tlv_bearer_context_t *req_bearers[OGS_GTP_MAX_INDIRECT_TUNNEL];
    ogs_gtp_tlv_bearer_context_t *rsp_bearers[OGS_GTP_MAX_INDIRECT_TUNNEL];
    ogs_gtp_f_teid_t *req_teid = NULL;
    ogs_gtp_f_teid_t rsp_dl_teid[OGS_GTP_MAX_INDIRECT_TUNNEL];
    ogs_gtp_f_teid_t rsp_ul_teid[OGS_GTP_MAX_INDIRECT_TUNNEL];
    int len;

    ogs_assert(s11_xact);
    ogs_assert(req);

    ogs_debug("[SGW] Create Indirect Data Forwarding Tunnel Request");

    memset(&cause, 0, sizeof(cause));
    cause.value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    if (!sgw_ue) {
        ogs_warn("No Context");
        cause.value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause.value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s11_xact, sgw_ue ? sgw_ue->mme_s11_teid : 0,
                OGS_GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE,
                cause.value);
        return;
    }

    rsp = &message.create_indirect_data_forwarding_tunnel_response;
    memset(&message, 0, sizeof(ogs_gtp_message_t));

    rsp->cause.presence = 1;
    rsp->cause.data = &cause;
    rsp->cause.len = sizeof(cause);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    ogs_gtp_bearers_in_create_indirect_tunnel_request(&req_bearers, req);
    ogs_gtp_bearers_in_create_indirect_tunnel_response(&rsp_bearers, rsp);

    for (i = 0; req_bearers[i]->presence; i++) {
        if (req_bearers[i]->eps_bearer_id.presence == 0) {
            ogs_error("No EBI");
            return;
        }

        bearer = sgw_bearer_find_by_ue_ebi(sgw_ue, 
                    req_bearers[i]->eps_bearer_id.u8);
        ogs_assert(bearer);

        if (req_bearers[i]->s1_u_enodeb_f_teid.presence) {
            req_teid = req_bearers[i]->s1_u_enodeb_f_teid.data;
            ogs_assert(req_teid);

            tunnel = sgw_tunnel_add(bearer,
                    OGS_GTP_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING);
            ogs_assert(tunnel);

            tunnel->remote_teid = ntohl(req_teid->teid);
            enb = ogs_gtp_node_find_by_f_teid(
                    &sgw_self()->enb_s1u_list, req_teid);
            if (!enb) {
                enb = ogs_gtp_node_add_by_f_teid(
                    &sgw_self()->enb_s1u_list, req_teid, sgw_self()->gtpu_port,
                    ogs_config()->parameter.no_ipv4,
                    ogs_config()->parameter.no_ipv6,
                    ogs_config()->parameter.prefer_ipv4);
                ogs_assert(enb);

                rv = ogs_gtp_connect(
                        sgw_self()->gtpu_sock, sgw_self()->gtpu_sock6, enb);
                ogs_assert(rv == OGS_OK);
            }
            /* Setup GTP Node */
            OGS_SETUP_GTP_NODE(tunnel, enb);

            memset(&rsp_dl_teid[i], 0, sizeof(ogs_gtp_f_teid_t));
            rsp_dl_teid[i].interface_type = tunnel->interface_type;
            rsp_dl_teid[i].teid = htonl(tunnel->local_teid);
            rv = ogs_gtp_sockaddr_to_f_teid(sgw_self()->gtpu_addr,
                    sgw_self()->gtpu_addr6, &rsp_dl_teid[i], &len);
            ogs_assert(len > 0);
            rsp_bearers[i]->s4_u_sgsn_f_teid.presence = 1;
            rsp_bearers[i]->s4_u_sgsn_f_teid.data = &rsp_dl_teid[i];
            rsp_bearers[i]->s4_u_sgsn_f_teid.len = len;

            ogs_debug("    SGW_DL_TEID[%d] ENB_DL_TEID[%d]",
                    tunnel->local_teid, tunnel->remote_teid);
        }

        if (req_bearers[i]->s12_rnc_f_teid.presence) {
            req_teid = req_bearers[i]->s12_rnc_f_teid.data;
            ogs_assert(req_teid);

            tunnel = sgw_tunnel_add(bearer,
                    OGS_GTP_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING);
            ogs_assert(tunnel);

            tunnel->remote_teid = ntohl(req_teid->teid);
            enb = ogs_gtp_node_find_by_f_teid(
                    &sgw_self()->enb_s1u_list, req_teid);
            if (!enb) {
                enb = ogs_gtp_node_add_by_f_teid(
                    &sgw_self()->enb_s1u_list, req_teid,
                    sgw_self()->gtpu_port,
                    ogs_config()->parameter.no_ipv4,
                    ogs_config()->parameter.no_ipv6,
                    ogs_config()->parameter.prefer_ipv4);
                ogs_assert(enb);

                rv = ogs_gtp_connect(
                        sgw_self()->gtpu_sock, sgw_self()->gtpu_sock6, enb);
                ogs_assert(rv == OGS_OK);
            }
            /* Setup GTP Node */
            OGS_SETUP_GTP_NODE(tunnel, enb);

            memset(&rsp_ul_teid[i], 0, sizeof(ogs_gtp_f_teid_t));
            rsp_ul_teid[i].teid = htonl(tunnel->local_teid);
            rsp_ul_teid[i].interface_type = tunnel->interface_type;
            rv = ogs_gtp_sockaddr_to_f_teid(sgw_self()->gtpu_addr,
                    sgw_self()->gtpu_addr6, &rsp_ul_teid[i], &len);
            ogs_assert(rv == OGS_OK);
            rsp_bearers[i]->s2b_u_epdg_f_teid_5.presence = 1;
            rsp_bearers[i]->s2b_u_epdg_f_teid_5.data = &rsp_ul_teid[i];
            rsp_bearers[i]->s2b_u_epdg_f_teid_5.len = len;
            ogs_debug("    SGW_UL_TEID[%d] ENB_UL_TEID[%d]",
                    tunnel->local_teid, tunnel->remote_teid);
        }

        if (req_bearers[i]->s1_u_enodeb_f_teid.presence ||
            req_bearers[i]->s12_rnc_f_teid.presence) {
            rsp_bearers[i]->presence = 1;
            rsp_bearers[i]->eps_bearer_id.presence = 1;
            rsp_bearers[i]->eps_bearer_id.u8 = bearer->ebi;

            rsp_bearers[i]->cause.presence = 1;
            rsp_bearers[i]->cause.data = &cause;
            rsp_bearers[i]->cause.len = sizeof(cause);
        }
    }

    message.h.type =
        OGS_GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE;
    message.h.teid = sgw_ue->mme_s11_teid;

    pkbuf = ogs_gtp_build_msg(&message);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(s11_xact, &message.h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);
}

void sgw_s11_handle_delete_indirect_data_forwarding_tunnel_request(
        ogs_gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue)
{
    int rv;
    ogs_gtp_delete_indirect_data_forwarding_tunnel_response_t *rsp = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_message_t message;

    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;
    sgw_tunnel_t *tunnel = NULL, *next_tunnel;
    
    ogs_gtp_cause_t cause;

    ogs_assert(s11_xact);

    ogs_debug("[SGW] Delete Indirect Data Forwarding Tunnel Request");

    memset(&cause, 0, sizeof(cause));
    cause.value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    if (!sgw_ue) {
        ogs_warn("No Context");
        cause.value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cause.value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s11_xact, sgw_ue ? sgw_ue->mme_s11_teid : 0,
                OGS_GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE,
                cause.value);
        return;
    }

    rsp = &message.delete_indirect_data_forwarding_tunnel_response;
    memset(&message, 0, sizeof(ogs_gtp_message_t));

    rsp->cause.presence = 1;
    rsp->cause.data = &cause;
    rsp->cause.len = sizeof(cause);

    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);

    /* Delete Indirect Tunnel */
    sess = sgw_sess_first(sgw_ue);
    while (sess) {
        bearer = sgw_bearer_first(sess);
        while (bearer) {
            tunnel = sgw_tunnel_first(bearer);
            while(tunnel) {
                next_tunnel = sgw_tunnel_next(tunnel);

                if (tunnel->interface_type ==
                    OGS_GTP_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING ||
                    tunnel->interface_type ==
                    OGS_GTP_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING)
                        sgw_tunnel_remove(tunnel);

                tunnel = next_tunnel;
            }
            

            bearer = sgw_bearer_next(bearer);
        }

        sess = sgw_sess_next(sess);
    }

    message.h.type =
        OGS_GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE;
    message.h.teid = sgw_ue->mme_s11_teid;

    pkbuf = ogs_gtp_build_msg(&message);
    ogs_expect_or_return(pkbuf);

    rv = ogs_gtp_xact_update_tx(s11_xact, &message.h, pkbuf);
    ogs_expect_or_return(rv == OGS_OK);

    rv = ogs_gtp_xact_commit(s11_xact);
    ogs_expect(rv == OGS_OK);
}

void sgw_s11_handle_bearer_resource_command(ogs_gtp_xact_t *s11_xact,
        sgw_ue_t *sgw_ue, ogs_gtp_message_t *message)
{
    int rv;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_bearer_resource_command_t *cmd = NULL;

    uint8_t cause_value = 0;
    ogs_gtp_xact_t *s5c_xact = NULL;

    sgw_sess_t *sess = NULL;

    ogs_assert(s11_xact);
    ogs_assert(message);

    ogs_debug("[SGW] Bearer Resource Command");

    cmd = &message->bearer_resource_command;
    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    if (!sgw_ue) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (cmd->linked_eps_bearer_id.presence == 0) {
        ogs_error("No Linked EPS Bearer ID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    } else {
        sess = sgw_sess_find_by_ebi(sgw_ue, cmd->linked_eps_bearer_id.u8);
        if (!sess) {
            ogs_error("No Context for Linked EPS Bearer ID[%d]",
                    cmd->linked_eps_bearer_id.u8);
            cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
        }
    }

    if (cmd->procedure_transaction_id.presence == 0) {
        ogs_error("No PTI");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }
    if (cmd->traffic_aggregate_description.presence == 0) {
        ogs_error("No Traffic aggregate description(TAD)");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(s11_xact, sgw_ue ? sgw_ue->mme_s11_teid : 0,
                OGS_GTP_BEARER_RESOURCE_FAILURE_INDICATION_TYPE, cause_value);
        return;
    }

    ogs_assert(sess);
    ogs_debug("    MME_S11_TEID[%d] SGW_S11_TEID[%d]",
        sgw_ue->mme_s11_teid, sgw_ue->sgw_s11_teid);
    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    message->h.type = OGS_GTP_BEARER_RESOURCE_COMMAND_TYPE;
    message->h.teid = sess->pgw_s5c_teid;

    pkbuf = ogs_gtp_build_msg(message);
    ogs_expect_or_return(pkbuf);

    s5c_xact = ogs_gtp_xact_local_create(
            sess->gnode, &message->h, pkbuf, timeout, sess);
    ogs_expect_or_return(s5c_xact);

    ogs_gtp_xact_associate(s11_xact, s5c_xact);

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);
}

