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

#include "pfcp-path.h"
#include "gtp-path.h"
#include "sxa-handler.h"

static uint8_t gtp_cause_from_pfcp(uint8_t pfcp_cause)
{
    switch (pfcp_cause) {
    case OGS_PFCP_CAUSE_REQUEST_ACCEPTED:
        return OGS_GTP2_CAUSE_REQUEST_ACCEPTED;
    case OGS_PFCP_CAUSE_REQUEST_REJECTED:
        return OGS_GTP2_CAUSE_REQUEST_REJECTED_REASON_NOT_SPECIFIED;
    case OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND:
        return OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    case OGS_PFCP_CAUSE_MANDATORY_IE_MISSING:
        return OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    case OGS_PFCP_CAUSE_CONDITIONAL_IE_MISSING:
        return OGS_GTP2_CAUSE_CONDITIONAL_IE_MISSING;
    case OGS_PFCP_CAUSE_INVALID_LENGTH:
        return OGS_GTP2_CAUSE_INVALID_LENGTH;
    case OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT:
        return OGS_GTP2_CAUSE_MANDATORY_IE_INCORRECT;
    case OGS_PFCP_CAUSE_INVALID_FORWARDING_POLICY:
    case OGS_PFCP_CAUSE_INVALID_F_TEID_ALLOCATION_OPTION:
        return OGS_GTP2_CAUSE_INVALID_MESSAGE_FORMAT;
    case OGS_PFCP_CAUSE_NO_ESTABLISHED_PFCP_ASSOCIATION:
        return OGS_GTP2_CAUSE_REMOTE_PEER_NOT_RESPONDING;
    case OGS_PFCP_CAUSE_RULE_CREATION_MODIFICATION_FAILURE:
        return OGS_GTP2_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION;
    case OGS_PFCP_CAUSE_PFCP_ENTITY_IN_CONGESTION:
        return OGS_GTP2_CAUSE_GTP_C_ENTITY_CONGESTION;
    case OGS_PFCP_CAUSE_NO_RESOURCES_AVAILABLE:
        return OGS_GTP2_CAUSE_NO_RESOURCES_AVAILABLE;
    case OGS_PFCP_CAUSE_SERVICE_NOT_SUPPORTED:
        return OGS_GTP2_CAUSE_SERVICE_NOT_SUPPORTED;
    case OGS_PFCP_CAUSE_SYSTEM_FAILURE:
        return OGS_GTP2_CAUSE_SYSTEM_FAILURE;
    default:
        return OGS_GTP2_CAUSE_SYSTEM_FAILURE;
    }

    return OGS_GTP2_CAUSE_SYSTEM_FAILURE;
}

static void sess_timeout(ogs_gtp_xact_t *xact, void *data)
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
    case OGS_GTP2_CREATE_SESSION_REQUEST_TYPE:
        ogs_error("[%s] No Create Session Response", sgwc_ue->imsi_bcd);
        if (!sgwc_sess_cycle(sess)) {
            ogs_warn("[%s] Session has already been removed",
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

static void bearer_timeout(ogs_gtp_xact_t *xact, void *data)
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

    switch (type) {
    case OGS_GTP2_CREATE_BEARER_REQUEST_TYPE:
        ogs_error("[%s] No Create Bearer Response", sgwc_ue->imsi_bcd);
        if (!sgwc_bearer_cycle(bearer)) {
            ogs_warn("[%s] Bearer has already been removed", sgwc_ue->imsi_bcd);
            break;
        }
        ogs_assert(OGS_OK ==
            sgwc_pfcp_send_bearer_modification_request(
                bearer, NULL, NULL,
                OGS_PFCP_MODIFY_UL_ONLY|OGS_PFCP_MODIFY_REMOVE));
        break;
    default:
        ogs_error("GTP Timeout : IMSI[%s] Message-Type[%d]",
                sgwc_ue->imsi_bcd, type);
    }
}

void sgwc_sxa_handle_session_establishment_response(
        sgwc_sess_t *sess, ogs_pfcp_xact_t *pfcp_xact,
        ogs_gtp2_message_t *recv_message,
        ogs_pfcp_session_establishment_response_t *pfcp_rsp)
{
    int rv;
    uint8_t cause_value = 0;

    ogs_pfcp_f_seid_t *up_f_seid = NULL;

    int sgw_s5c_len;
    ogs_gtp2_f_teid_t sgw_s5c_teid;
    ogs_gtp2_f_teid_t *pgw_s5c_teid = NULL;

    int i, num_of_sgw_s5u;
    uint8_t ebi[OGS_BEARER_PER_UE];
    int sgw_s5u_len[OGS_BEARER_PER_UE];
    ogs_gtp2_f_teid_t sgw_s5u_teid[OGS_BEARER_PER_UE];

    ogs_gtp_xact_t *s11_xact = NULL, *s5c_xact = NULL;
    ogs_gtp_node_t *pgw = NULL;

    sgwc_ue_t *sgwc_ue = NULL;
    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *dl_tunnel = NULL;

    ogs_gtp2_create_session_request_t *create_session_request = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_gtp2_indication_t *indication = NULL;

    ogs_debug("Session Establishment Response");

    ogs_assert(pfcp_xact);
    ogs_assert(pfcp_rsp);
    ogs_assert(recv_message);

    create_session_request = &recv_message->create_session_request;
    ogs_assert(create_session_request);

    s11_xact = pfcp_xact->assoc_xact;
    ogs_assert(s11_xact);

    ogs_pfcp_xact_commit(pfcp_xact);

    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_warn("No Context");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (pfcp_rsp->up_f_seid.presence == 0) {
        ogs_error("No UP F-SEID");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (pfcp_rsp->cause.presence) {
        if (pfcp_rsp->cause.u8 != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
            ogs_warn("PFCP Cause [%d] : Not Accepted", pfcp_rsp->cause.u8);
            cause_value = gtp_cause_from_pfcp(pfcp_rsp->cause.u8);
        }
    } else {
        ogs_error("No Cause");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        int i;

        uint8_t pfcp_cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
        uint8_t offending_ie_value = 0;

        sgwc_tunnel_t *tunnel = NULL;
        ogs_pfcp_pdr_t *pdr = NULL;
        ogs_pfcp_far_t *far = NULL;

        ogs_assert(sess);
        for (i = 0; i < OGS_MAX_NUM_OF_PDR; i++) {
            pdr = ogs_pfcp_handle_created_pdr(
                    &sess->pfcp, &pfcp_rsp->created_pdr[i],
                    &pfcp_cause_value, &offending_ie_value);

            if (!pdr)
                break;
        }

        ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
            far = pdr->far;
            ogs_assert(far);

            if (pdr->src_if == OGS_PFCP_INTERFACE_CP_FUNCTION)
                ogs_assert(OGS_ERROR != ogs_pfcp_setup_pdr_gtpu_node(pdr));

            if (far->dst_if == OGS_PFCP_INTERFACE_CP_FUNCTION)
                ogs_pfcp_far_teid_hash_set(far);

            tunnel = sgwc_tunnel_find_by_pdr_id(sess, pdr->id);
            if (tunnel) {
                ogs_assert(sess->pfcp_node);
                if (sess->pfcp_node->up_function_features.ftup &&
                    pdr->f_teid_len) {
                    if (tunnel->local_addr)
                        ogs_freeaddrinfo(tunnel->local_addr);
                    if (tunnel->local_addr6)
                        ogs_freeaddrinfo(tunnel->local_addr6);

                    ogs_assert(OGS_OK ==
                        ogs_pfcp_f_teid_to_sockaddr(
                            &pdr->f_teid, pdr->f_teid_len,
                            &tunnel->local_addr, &tunnel->local_addr6));
                    tunnel->local_teid = pdr->f_teid.teid;
                }
            }
        }

        cause_value = gtp_cause_from_pfcp(pfcp_cause_value);
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        if (sess) sgwc_ue = sess->sgwc_ue;
        ogs_gtp_send_error_message(
                s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE, cause_value);
        return;
    }

    ogs_assert(sess);

    ogs_debug("    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]",
        sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    /* Data Plane(DL) : SGW-S5U */
    i = 0;
    ogs_list_for_each(&sess->bearer_list, bearer) {
        ogs_assert(i < OGS_BEARER_PER_UE);

        dl_tunnel = sgwc_dl_tunnel_in_bearer(bearer);
        ogs_assert(dl_tunnel);

        ogs_debug("    SGW_S5U_TEID[%d] PGW_S5U_TEID[%d]",
            dl_tunnel->local_teid, dl_tunnel->remote_teid);

        if (dl_tunnel->local_addr == NULL && dl_tunnel->local_addr6 == NULL) {
            ogs_error("No UP F-TEID");
            ogs_gtp_send_error_message(
                    s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                    OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE,
                    OGS_GTP2_CAUSE_GRE_KEY_NOT_FOUND);
            return;
        }

        ebi[i] = bearer->ebi;

        memset(&sgw_s5u_teid[i], 0, sizeof(ogs_gtp2_f_teid_t));
        sgw_s5u_teid[i].teid = htobe32(dl_tunnel->local_teid);
        sgw_s5u_teid[i].interface_type = dl_tunnel->interface_type;
        ogs_assert(dl_tunnel->local_addr || dl_tunnel->local_addr6);
        rv = ogs_gtp2_sockaddr_to_f_teid(
                dl_tunnel->local_addr, dl_tunnel->local_addr6,
                &sgw_s5u_teid[i], &sgw_s5u_len[i]);
        ogs_assert(rv == OGS_OK);

        i++;
    }

    num_of_sgw_s5u = i;

    /* Send Control Plane(DL) : SGW-S5C */
    memset(&sgw_s5c_teid, 0, sizeof(ogs_gtp2_f_teid_t));
    sgw_s5c_teid.interface_type = OGS_GTP2_F_TEID_S5_S8_SGW_GTP_C;
    sgw_s5c_teid.teid = htobe32(sess->sgw_s5c_teid);
    rv = ogs_gtp2_sockaddr_to_f_teid(
            ogs_gtp_self()->gtpc_addr, ogs_gtp_self()->gtpc_addr6,
            &sgw_s5c_teid, &sgw_s5c_len);
    ogs_assert(rv == OGS_OK);

    /* UP F-SEID */
    up_f_seid = pfcp_rsp->up_f_seid.data;
    ogs_assert(up_f_seid);
    sess->sgwu_sxa_seid = be64toh(up_f_seid->seid);

    /* Receive Control Plane(UL) : PGW-S5C */
    pgw_s5c_teid = create_session_request->
        pgw_s5_s8_address_for_control_plane_or_pmip.data;
    ogs_assert(pgw_s5c_teid);

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

    /* Check Indication */
    if (create_session_request->indication_flags.presence &&
        create_session_request->indication_flags.data &&
        create_session_request->indication_flags.len) {
        indication = create_session_request->indication_flags.data;
    }

    if (indication && indication->operation_indication) {
        ogs_gtp2_message_t send_message;
        ogs_gtp2_modify_bearer_request_t *modify_bearer_request =
            &send_message.modify_bearer_request;

        /*
         * Operation Indication:
         * This flag shall be set to 1 on the S4/S11 interface
         * for a TAU/RAU procedure with SGW relocation, Enhanced
         * SRNS Relocation with SGW relocation, X2-based handovers
         * with SGW relocation and MME triggered Serving GW relocation
         */
        memset(&send_message, 0, sizeof(ogs_gtp2_message_t));

        send_message.h.type = OGS_GTP2_MODIFY_BEARER_REQUEST_TYPE;
        send_message.h.teid = sess->pgw_s5c_teid;

        /* Send Control Plane(DL) : SGW-S5C */
        modify_bearer_request->sender_f_teid_for_control_plane.presence = 1;
        modify_bearer_request->sender_f_teid_for_control_plane.
            data = &sgw_s5c_teid;
        modify_bearer_request->sender_f_teid_for_control_plane.
            len = sgw_s5c_len;

        for (i = 0; i < num_of_sgw_s5u; i++) {
            modify_bearer_request->bearer_contexts_to_be_modified[i].
                presence = 1;
            modify_bearer_request->bearer_contexts_to_be_modified[i].
                eps_bearer_id.presence = 1;

            /* Bearer Context : EBI */
            modify_bearer_request->bearer_contexts_to_be_modified[i].
                eps_bearer_id.u8 = ebi[i];

            /* Data Plane(DL) : SGW-S5U */
            modify_bearer_request->bearer_contexts_to_be_modified[i].
                s4_u_sgsn_f_teid.presence = 1;
            modify_bearer_request->bearer_contexts_to_be_modified[i].
                s4_u_sgsn_f_teid.data = &sgw_s5u_teid[i];
            modify_bearer_request->bearer_contexts_to_be_modified[i].
                s4_u_sgsn_f_teid.len = sgw_s5u_len[i];
        }

        pkbuf = ogs_gtp2_build_msg(&send_message);
        ogs_expect_or_return(pkbuf);

        ogs_assert(sess->gnode);
        s5c_xact = ogs_gtp_xact_local_create(
                sess->gnode, &send_message.h, pkbuf, sess_timeout, sess);
        ogs_expect_or_return(s5c_xact);
        s5c_xact->local_teid = sess->sgw_s5c_teid;

        s5c_xact->modify_action = OGS_GTP_MODIFY_IN_PATH_SWITCH_REQUEST;

    } else {

        /* Create Session Request */
        recv_message->h.type = OGS_GTP2_CREATE_SESSION_REQUEST_TYPE;
        recv_message->h.teid = sess->pgw_s5c_teid;

        /* Send Control Plane(DL) : SGW-S5C */
        create_session_request->sender_f_teid_for_control_plane.presence = 1;
        create_session_request->sender_f_teid_for_control_plane.
            data = &sgw_s5c_teid;
        create_session_request->sender_f_teid_for_control_plane.
            len = sgw_s5c_len;

        /* Remove PGW-S5C */
        create_session_request->pgw_s5_s8_address_for_control_plane_or_pmip.
            presence = 0;

        /* Bearer Contexts */
        for (i = 0; i < num_of_sgw_s5u; i++) {
            create_session_request->bearer_contexts_to_be_created[i].
                s5_s8_u_sgw_f_teid.presence = 1;
            create_session_request->bearer_contexts_to_be_created[i].
                s5_s8_u_sgw_f_teid.data = &sgw_s5u_teid[i];
            create_session_request->bearer_contexts_to_be_created[i].
                s5_s8_u_sgw_f_teid.len = sgw_s5u_len[i];
        }

        pkbuf = ogs_gtp2_build_msg(recv_message);
        ogs_expect_or_return(pkbuf);

        ogs_assert(sess->gnode);
        s5c_xact = ogs_gtp_xact_local_create(
                sess->gnode, &recv_message->h, pkbuf, sess_timeout, sess);
        ogs_expect_or_return(s5c_xact);
        s5c_xact->local_teid = sess->sgw_s5c_teid;
    }

    ogs_gtp_xact_associate(s11_xact, s5c_xact);

    rv = ogs_gtp_xact_commit(s5c_xact);
    ogs_expect(rv == OGS_OK);
}

void sgwc_sxa_handle_session_modification_response(
        sgwc_sess_t *sess, ogs_pfcp_xact_t *pfcp_xact,
        ogs_gtp2_message_t *recv_message,
        ogs_pfcp_session_modification_response_t *pfcp_rsp)
{
    int i, rv, len = 0;
    uint8_t cause_value = 0;
    uint64_t flags;

    ogs_gtp_xact_t *s11_xact = NULL;
    ogs_gtp_xact_t *s5c_xact = NULL;

    ogs_gtp2_message_t send_message;

    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *dl_tunnel = NULL, *ul_tunnel = NULL;
    sgwc_ue_t *sgwc_ue = NULL;

    ogs_pkbuf_t *pkbuf = NULL;

    ogs_gtp2_cause_t cause;

    ogs_debug("Session Modification Response");

    ogs_assert(pfcp_xact);
    ogs_assert(pfcp_rsp);

    flags = pfcp_xact->modify_flags;
    ogs_assert(flags);

    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (flags & OGS_PFCP_MODIFY_SESSION) {
        if (!sess) {
            ogs_warn("No Context");

            sess = pfcp_xact->data;
            ogs_assert(sess);

            cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
        }
        sgwc_ue = sess->sgwc_ue;
        ogs_assert(sgwc_ue);

    } else {
        bearer = pfcp_xact->data;
        ogs_assert(bearer);

        if (!sess) {
            ogs_warn("No Context");

            sess = bearer->sess;
            ogs_assert(sess);

            cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
        }

        sgwc_ue = bearer->sgwc_ue;
        ogs_assert(sgwc_ue);
    }

    if (pfcp_rsp->cause.presence) {
        if (pfcp_rsp->cause.u8 != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
            ogs_warn("PFCP Cause [%d] : Not Accepted", pfcp_rsp->cause.u8);
            cause_value = gtp_cause_from_pfcp(pfcp_rsp->cause.u8);
        }
    } else {
        ogs_error("No Cause");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        uint8_t pfcp_cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
        uint8_t offending_ie_value = 0;

        sgwc_tunnel_t *tunnel = NULL;
        ogs_pfcp_pdr_t *pdr = NULL;
        ogs_pfcp_far_t *far = NULL;

        OGS_LIST(pdr_to_create_list);

        ogs_assert(sess);

        ogs_list_copy(&pdr_to_create_list, &pfcp_xact->pdr_to_create_list);

        for (i = 0; i < OGS_MAX_NUM_OF_PDR; i++) {
            pdr = ogs_pfcp_handle_created_pdr(
                    &sess->pfcp, &pfcp_rsp->created_pdr[i],
                    &pfcp_cause_value, &offending_ie_value);

            if (!pdr)
                break;
        }

        ogs_list_for_each_entry(&pdr_to_create_list, pdr, to_create_node) {
            far = pdr->far;
            ogs_assert(far);

            if (pdr->src_if == OGS_PFCP_INTERFACE_CP_FUNCTION)
                ogs_assert(OGS_ERROR != ogs_pfcp_setup_pdr_gtpu_node(pdr));

            if (far->dst_if == OGS_PFCP_INTERFACE_CP_FUNCTION)
                ogs_pfcp_far_teid_hash_set(far);

            tunnel = sgwc_tunnel_find_by_pdr_id(sess, pdr->id);
            if (tunnel) {
                ogs_assert(sess->pfcp_node);
                if (sess->pfcp_node->up_function_features.ftup &&
                    pdr->f_teid_len) {
                    if (tunnel->local_addr)
                        ogs_freeaddrinfo(tunnel->local_addr);
                    if (tunnel->local_addr6)
                        ogs_freeaddrinfo(tunnel->local_addr6);

                    ogs_assert(OGS_OK ==
                        ogs_pfcp_f_teid_to_sockaddr(
                            &pdr->f_teid, pdr->f_teid_len,
                            &tunnel->local_addr, &tunnel->local_addr6));
                    tunnel->local_teid = pdr->f_teid.teid;
                }
            }
        }

        cause_value = gtp_cause_from_pfcp(pfcp_cause_value);
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        /*
         * You should not change the following order to support
         * OGS_PFCP_MODIFY_REMOVE|OGS_PFCP_MODIFY_CREATE.
         *
         * 1. if (flags & OGS_PFCP_MODIFY_REMOVE) {
         * 2. } else if (flags & OGS_PFCP_MODIFY_CREATE) {
         *    }
         */
        if (flags & OGS_PFCP_MODIFY_REMOVE) {
            s5c_xact = pfcp_xact->assoc_xact;

            if (s5c_xact) {
                ogs_gtp_send_error_message(
                        s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                        OGS_GTP2_DELETE_BEARER_RESPONSE_TYPE, cause_value);
            }

            sgwc_bearer_remove(bearer);
        } else if (flags & OGS_PFCP_MODIFY_CREATE) {
            s5c_xact = pfcp_xact->assoc_xact;
            ogs_assert(s5c_xact);

            ogs_gtp_send_error_message(
                    s5c_xact, sess ? sess->pgw_s5c_teid : 0,
                    OGS_GTP2_CREATE_BEARER_RESPONSE_TYPE, cause_value);


        } else if (flags & OGS_PFCP_MODIFY_ACTIVATE) {
            if (flags & OGS_PFCP_MODIFY_UL_ONLY) {
                s11_xact = pfcp_xact->assoc_xact;
                ogs_assert(s11_xact);

                ogs_gtp_send_error_message(
                        s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                        OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE, cause_value);

            } else if (flags & OGS_PFCP_MODIFY_DL_ONLY) {
                s11_xact = pfcp_xact->assoc_xact;
                ogs_assert(s11_xact);

                ogs_gtp_send_error_message(
                        s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                        OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE, cause_value);
            } else {
                ogs_fatal("Invalid modify_flags[0x%llx]", (long long)flags);
                ogs_assert_if_reached();
            }
        } else if (flags & OGS_PFCP_MODIFY_DEACTIVATE) {
            s11_xact = pfcp_xact->assoc_xact;
            ogs_assert(s11_xact);

            ogs_gtp_send_error_message(
                    s11_xact, sgwc_ue ? sgwc_ue->mme_s11_teid : 0,
                    OGS_GTP2_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE, cause_value);
        }

        ogs_pfcp_xact_commit(pfcp_xact);
        return;
    }

    if (flags & OGS_PFCP_MODIFY_SESSION) {

        /* Nothing */

    } else {
        ogs_assert(bearer);

        dl_tunnel = sgwc_dl_tunnel_in_bearer(bearer);
        ogs_assert(dl_tunnel);
        ul_tunnel = sgwc_ul_tunnel_in_bearer(bearer);
        ogs_assert(ul_tunnel);
    }

    /*
     * You should not change the following order to support
     * OGS_PFCP_MODIFY_REMOVE|OGS_PFCP_MODIFY_CREATE.
     *
     * 1. if (flags & OGS_PFCP_MODIFY_REMOVE) {
     * 2. } else if (flags & OGS_PFCP_MODIFY_CREATE) {
     *    }
     */
    if (flags & OGS_PFCP_MODIFY_REMOVE) {
        if (flags & OGS_PFCP_MODIFY_INDIRECT) {
            s11_xact = pfcp_xact->assoc_xact;
            ogs_assert(s11_xact);

            ogs_pfcp_xact_commit(pfcp_xact);

            ogs_assert(flags & OGS_PFCP_MODIFY_SESSION);
            if (SGWC_SESSION_SYNC_DONE(sgwc_ue,
                OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE, flags)) {

                sgwc_tunnel_t *tunnel = NULL, *next_tunnel = NULL;
                ogs_gtp2_delete_indirect_data_forwarding_tunnel_response_t
                    *gtp_rsp = NULL;

                ogs_list_for_each(&sgwc_ue->sess_list, sess) {
                    ogs_list_for_each(&sess->bearer_list, bearer) {
                        ogs_list_for_each_safe(&bearer->tunnel_list,
                                next_tunnel, tunnel) {
                            if (tunnel->interface_type ==
                            OGS_GTP2_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING ||
                                tunnel->interface_type ==
                            OGS_GTP2_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING) {
                                sgwc_tunnel_remove(tunnel);
                            }
                        }
                    }
                }

                gtp_rsp = &send_message.
                    delete_indirect_data_forwarding_tunnel_response;
                ogs_assert(gtp_rsp);

                memset(&send_message, 0, sizeof(ogs_gtp2_message_t));

                memset(&cause, 0, sizeof(cause));
                cause.value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

                gtp_rsp->cause.presence = 1;
                gtp_rsp->cause.data = &cause;
                gtp_rsp->cause.len = sizeof(cause);

                send_message.h.type =
                OGS_GTP2_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE;
                send_message.h.teid = sgwc_ue->mme_s11_teid;

                pkbuf = ogs_gtp2_build_msg(&send_message);
                ogs_expect_or_return(pkbuf);

                rv = ogs_gtp_xact_update_tx(s11_xact, &send_message.h, pkbuf);
                ogs_expect_or_return(rv == OGS_OK);

                rv = ogs_gtp_xact_commit(s11_xact);
                ogs_expect(rv == OGS_OK);
            }

        } else {
            s5c_xact = pfcp_xact->assoc_xact;

            ogs_pfcp_xact_commit(pfcp_xact);

            if (s5c_xact) {
                ogs_assert(recv_message);
                recv_message->h.type = OGS_GTP2_DELETE_BEARER_RESPONSE_TYPE;
                recv_message->h.teid = sess->pgw_s5c_teid;

                pkbuf = ogs_gtp2_build_msg(recv_message);
                ogs_expect_or_return(pkbuf);

                rv = ogs_gtp_xact_update_tx(s5c_xact, &recv_message->h, pkbuf);
                ogs_expect_or_return(rv == OGS_OK);

                rv = ogs_gtp_xact_commit(s5c_xact);
                ogs_expect(rv == OGS_OK);
            }

            sgwc_bearer_remove(bearer);
        }

    } else if (flags & OGS_PFCP_MODIFY_CREATE) {
        if (flags & OGS_PFCP_MODIFY_UL_ONLY) {
            ogs_gtp2_create_bearer_request_t *gtp_req = NULL;
            ogs_gtp2_f_teid_t sgw_s1u_teid;

            s5c_xact = pfcp_xact->assoc_xact;
            ogs_assert(s5c_xact);

            ogs_pfcp_xact_commit(pfcp_xact);

            ogs_assert(recv_message);
            gtp_req = &recv_message->create_bearer_request;
            ogs_assert(gtp_req);

            /* Send Data Plane(UL) : SGW-S1U */
            memset(&sgw_s1u_teid, 0, sizeof(ogs_gtp2_f_teid_t));
            sgw_s1u_teid.interface_type = ul_tunnel->interface_type;
            sgw_s1u_teid.teid = htobe32(ul_tunnel->local_teid);
            ogs_assert(ul_tunnel->local_addr || ul_tunnel->local_addr6);
            rv = ogs_gtp2_sockaddr_to_f_teid(
                    ul_tunnel->local_addr, ul_tunnel->local_addr6,
                    &sgw_s1u_teid, &len);
            ogs_assert(rv == OGS_OK);
            gtp_req->bearer_contexts.s1_u_enodeb_f_teid.presence = 1;
            gtp_req->bearer_contexts.s1_u_enodeb_f_teid.data = &sgw_s1u_teid;
            gtp_req->bearer_contexts.s1_u_enodeb_f_teid.len = len;

            recv_message->h.type = OGS_GTP2_CREATE_BEARER_REQUEST_TYPE;
            recv_message->h.teid = sgwc_ue->mme_s11_teid;

            pkbuf = ogs_gtp2_build_msg(recv_message);
            ogs_expect_or_return(pkbuf);

            ogs_assert(sgwc_ue->gnode);
            ogs_assert(bearer);
            s11_xact = ogs_gtp_xact_local_create(sgwc_ue->gnode,
                    &recv_message->h, pkbuf, bearer_timeout, bearer);
            ogs_expect_or_return(s11_xact);
            s11_xact->local_teid = sgwc_ue->sgw_s11_teid;

            ogs_gtp_xact_associate(s5c_xact, s11_xact);

            rv = ogs_gtp_xact_commit(s11_xact);
            ogs_expect(rv == OGS_OK);

        } else if (flags & OGS_PFCP_MODIFY_DL_ONLY) {
            ogs_gtp2_create_bearer_response_t *gtp_rsp = NULL;
            ogs_gtp2_f_teid_t sgw_s5u_teid, pgw_s5u_teid;

            s5c_xact = pfcp_xact->assoc_xact;
            ogs_assert(s5c_xact);

            ogs_pfcp_xact_commit(pfcp_xact);

            ogs_assert(recv_message);
            gtp_rsp = &recv_message->create_bearer_response;
            ogs_assert(gtp_rsp);

            /* Remove SGW-S1U-TEID */
            gtp_rsp->bearer_contexts.s4_u_sgsn_f_teid.presence = 0;

            /* Remove S1U-F-TEID */
            gtp_rsp->bearer_contexts.s1_u_enodeb_f_teid.presence = 0;

            /* Data Plane(DL) : SGW-S5U */
            ogs_assert(dl_tunnel);
            memset(&sgw_s5u_teid, 0, sizeof(ogs_gtp2_f_teid_t));
            sgw_s5u_teid.interface_type = OGS_GTP2_F_TEID_S5_S8_SGW_GTP_U;
            sgw_s5u_teid.teid = htobe32(dl_tunnel->local_teid);
            ogs_assert(dl_tunnel->local_addr || dl_tunnel->local_addr6);
            rv = ogs_gtp2_sockaddr_to_f_teid(
                    dl_tunnel->local_addr, dl_tunnel->local_addr6,
                    &sgw_s5u_teid, &len);
            ogs_assert(rv == OGS_OK);
            gtp_rsp->bearer_contexts.s5_s8_u_sgw_f_teid.presence = 1;
            gtp_rsp->bearer_contexts.s5_s8_u_sgw_f_teid.data = &sgw_s5u_teid;
            gtp_rsp->bearer_contexts.s5_s8_u_sgw_f_teid.len = len;

            /* Data Plane(UL) : PGW-S5U */
            ogs_assert(ul_tunnel);
            pgw_s5u_teid.interface_type = OGS_GTP2_F_TEID_S5_S8_PGW_GTP_U;
            pgw_s5u_teid.teid = htobe32(ul_tunnel->remote_teid);
            rv = ogs_gtp2_ip_to_f_teid(
                    &ul_tunnel->remote_ip, &pgw_s5u_teid, &len);
            gtp_rsp->bearer_contexts.s5_s8_u_pgw_f_teid.presence = 1;
            gtp_rsp->bearer_contexts.s5_s8_u_pgw_f_teid.data = &pgw_s5u_teid;
            gtp_rsp->bearer_contexts.s5_s8_u_pgw_f_teid.len = len;

            recv_message->h.type = OGS_GTP2_CREATE_BEARER_RESPONSE_TYPE;
            recv_message->h.teid = sess->pgw_s5c_teid;

            pkbuf = ogs_gtp2_build_msg(recv_message);
            ogs_expect_or_return(pkbuf);

            rv = ogs_gtp_xact_update_tx(s5c_xact, &recv_message->h, pkbuf);
            ogs_expect_or_return(rv == OGS_OK);

            rv = ogs_gtp_xact_commit(s5c_xact);
            ogs_expect(rv == OGS_OK);

        } else if (flags & OGS_PFCP_MODIFY_INDIRECT) {
            s11_xact = pfcp_xact->assoc_xact;
            ogs_assert(s11_xact);

            ogs_pfcp_xact_commit(pfcp_xact);

            ogs_assert(flags & OGS_PFCP_MODIFY_SESSION);
            if (SGWC_SESSION_SYNC_DONE(sgwc_ue,
                OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE, flags)) {

                sgwc_tunnel_t *tunnel = NULL;

                ogs_gtp2_create_indirect_data_forwarding_tunnel_request_t
                    *gtp_req = NULL;
                ogs_gtp2_create_indirect_data_forwarding_tunnel_response_t
                    *gtp_rsp = NULL;

                ogs_gtp2_f_teid_t rsp_dl_teid[OGS_GTP2_MAX_INDIRECT_TUNNEL];
                ogs_gtp2_f_teid_t rsp_ul_teid[OGS_GTP2_MAX_INDIRECT_TUNNEL];

                ogs_assert(recv_message);
                gtp_req = &recv_message->
                    create_indirect_data_forwarding_tunnel_request;
                ogs_assert(gtp_req);
                gtp_rsp = &send_message.
                    create_indirect_data_forwarding_tunnel_response;
                ogs_assert(gtp_rsp);

                memset(&send_message, 0, sizeof(ogs_gtp2_message_t));

                memset(&cause, 0, sizeof(cause));
                cause.value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

                gtp_rsp->cause.presence = 1;
                gtp_rsp->cause.data = &cause;
                gtp_rsp->cause.len = sizeof(cause);

                for (i = 0; gtp_req->bearer_contexts[i].presence; i++) {
                    ogs_assert(gtp_req->
                            bearer_contexts[i].eps_bearer_id.presence);
                    bearer = sgwc_bearer_find_by_ue_ebi(sgwc_ue,
                                gtp_req->bearer_contexts[i].eps_bearer_id.u8);
                    ogs_assert(bearer);

                    ogs_list_for_each(&bearer->tunnel_list, tunnel) {
                        if (tunnel->interface_type ==
                            OGS_GTP2_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING) {

                            memset(&rsp_dl_teid[i],
                                    0, sizeof(ogs_gtp2_f_teid_t));
                            rsp_dl_teid[i].interface_type =
                                tunnel->interface_type;
                            rsp_dl_teid[i].teid = htobe32(tunnel->local_teid);
                            ogs_assert(
                                    tunnel->local_addr || tunnel->local_addr6);
                            rv = ogs_gtp2_sockaddr_to_f_teid(
                                tunnel->local_addr, tunnel->local_addr6,
                                &rsp_dl_teid[i], &len);
                            ogs_assert(rv == OGS_OK);
                            gtp_rsp->bearer_contexts[i].
                                s4_u_sgsn_f_teid.presence = 1;
                            gtp_rsp->bearer_contexts[i].
                                s4_u_sgsn_f_teid.data = &rsp_dl_teid[i];
                            gtp_rsp->bearer_contexts[i].
                                s4_u_sgsn_f_teid.len = len;

                        } else if (tunnel->interface_type ==
                            OGS_GTP2_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING) {

                            memset(&rsp_ul_teid[i],
                                0, sizeof(ogs_gtp2_f_teid_t));
                            rsp_ul_teid[i].teid = htobe32(tunnel->local_teid);
                            rsp_ul_teid[i].interface_type =
                                tunnel->interface_type;
                            ogs_assert(
                                    tunnel->local_addr || tunnel->local_addr6);
                            rv = ogs_gtp2_sockaddr_to_f_teid(
                                tunnel->local_addr, tunnel->local_addr6,
                                &rsp_ul_teid[i], &len);
                            ogs_assert(rv == OGS_OK);
                            gtp_rsp->bearer_contexts[i].
                                s2b_u_epdg_f_teid_5.presence = 1;
                            gtp_rsp->bearer_contexts[i].
                                s2b_u_epdg_f_teid_5.data = &rsp_ul_teid[i];
                            gtp_rsp->bearer_contexts[i].
                                s2b_u_epdg_f_teid_5.len = len;

                        }

                    }

                    if (gtp_rsp->bearer_contexts[i].
                            s4_u_sgsn_f_teid.presence ||
                        gtp_rsp->bearer_contexts[i].
                            s2b_u_epdg_f_teid_5.presence) {

                        gtp_rsp->bearer_contexts[i].presence = 1;
                        gtp_rsp->bearer_contexts[i].eps_bearer_id.presence = 1;
                        gtp_rsp->bearer_contexts[i].eps_bearer_id.u8 =
                            bearer->ebi;

                        gtp_rsp->bearer_contexts[i].cause.presence = 1;
                        gtp_rsp->bearer_contexts[i].cause.data = &cause;
                        gtp_rsp->bearer_contexts[i].cause.len = sizeof(cause);
                    }
                }

                send_message.h.type =
                OGS_GTP2_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE;
                send_message.h.teid = sgwc_ue->mme_s11_teid;

                pkbuf = ogs_gtp2_build_msg(&send_message);
                ogs_expect_or_return(pkbuf);

                rv = ogs_gtp_xact_update_tx(s11_xact, &send_message.h, pkbuf);
                ogs_expect_or_return(rv == OGS_OK);

                rv = ogs_gtp_xact_commit(s11_xact);
                ogs_expect(rv == OGS_OK);
            }
        } else {
            ogs_fatal("Invalid modify_flags[0x%llx]", (long long)flags);
            ogs_assert_if_reached();
        }

    } else if (flags & OGS_PFCP_MODIFY_ACTIVATE) {
        OGS_LIST(bearer_to_modify_list);

        s11_xact = pfcp_xact->assoc_xact;
        ogs_assert(s11_xact);

        ogs_list_copy(&bearer_to_modify_list,
                &pfcp_xact->bearer_to_modify_list);

        ogs_pfcp_xact_commit(pfcp_xact);

        ogs_assert(flags & OGS_PFCP_MODIFY_SESSION);
        if (flags & OGS_PFCP_MODIFY_UL_ONLY) {
            ogs_gtp2_create_session_response_t *gtp_rsp = NULL;
            ogs_gtp2_f_teid_t sgw_s11_teid;
            ogs_gtp2_f_teid_t sgw_s1u_teid[OGS_BEARER_PER_UE];
            int sgw_s1u_len[OGS_BEARER_PER_UE];

            ogs_assert(recv_message);
            gtp_rsp = &recv_message->create_session_response;
            ogs_assert(gtp_rsp);

            /* Send Control Plane(UL) : SGW-S11 */
            memset(&sgw_s11_teid, 0, sizeof(ogs_gtp2_f_teid_t));
            sgw_s11_teid.interface_type = OGS_GTP2_F_TEID_S11_S4_SGW_GTP_C;
            sgw_s11_teid.teid = htobe32(sgwc_ue->sgw_s11_teid);
            rv = ogs_gtp2_sockaddr_to_f_teid(
                    ogs_gtp_self()->gtpc_addr, ogs_gtp_self()->gtpc_addr6,
                    &sgw_s11_teid, &len);
            ogs_assert(rv == OGS_OK);
            gtp_rsp->sender_f_teid_for_control_plane.presence = 1;
            gtp_rsp->sender_f_teid_for_control_plane.data = &sgw_s11_teid;
            gtp_rsp->sender_f_teid_for_control_plane.len = len;

            /* Send Data Plane(UL) : SGW-S1U */
            i = 0;
            ogs_list_for_each_entry(
                    &bearer_to_modify_list, bearer, to_modify_node) {
                ogs_assert(i < OGS_BEARER_PER_UE);

                ul_tunnel = sgwc_ul_tunnel_in_bearer(bearer);
                ogs_assert(ul_tunnel);

                memset(&sgw_s1u_teid[i], 0, sizeof(ogs_gtp2_f_teid_t));
                sgw_s1u_teid[i].interface_type = ul_tunnel->interface_type;
                sgw_s1u_teid[i].teid = htobe32(ul_tunnel->local_teid);
                ogs_assert(ul_tunnel->local_addr || ul_tunnel->local_addr6);
                rv = ogs_gtp2_sockaddr_to_f_teid(
                    ul_tunnel->local_addr, ul_tunnel->local_addr6,
                    &sgw_s1u_teid[i], &sgw_s1u_len[i]);
                ogs_assert(rv == OGS_OK);
                gtp_rsp->bearer_contexts_created[i].s1_u_enodeb_f_teid.
                    presence = 1;
                gtp_rsp->bearer_contexts_created[i].s1_u_enodeb_f_teid.
                    data = &sgw_s1u_teid[i];
                gtp_rsp->bearer_contexts_created[i].s1_u_enodeb_f_teid.
                    len = sgw_s1u_len[i];

                i++;
            }

            recv_message->h.type = OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE;
            recv_message->h.teid = sgwc_ue->mme_s11_teid;

            pkbuf = ogs_gtp2_build_msg(recv_message);
            ogs_expect_or_return(pkbuf);

            rv = ogs_gtp_xact_update_tx(s11_xact, &recv_message->h, pkbuf);
            ogs_expect_or_return(rv == OGS_OK);

            rv = ogs_gtp_xact_commit(s11_xact);
            ogs_expect(rv == OGS_OK);

        } else if (flags & OGS_PFCP_MODIFY_DL_ONLY) {
            if (SGWC_SESSION_SYNC_DONE(sgwc_ue,
                    OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE, flags)) {
                ogs_gtp2_modify_bearer_request_t *gtp_req = NULL;
                ogs_gtp2_modify_bearer_response_t *gtp_rsp = NULL;

                ogs_gtp2_indication_t *indication = NULL;

                ogs_assert(recv_message);
                gtp_req = &recv_message->modify_bearer_request;
                ogs_assert(gtp_req);

                if (gtp_req->indication_flags.presence &&
                    gtp_req->indication_flags.data &&
                    gtp_req->indication_flags.len) {
                    indication = gtp_req->indication_flags.data;
                }

                if (indication && indication->handover_indication) {
                    recv_message->h.type = OGS_GTP2_MODIFY_BEARER_REQUEST_TYPE;
                    recv_message->h.teid = sess->pgw_s5c_teid;

                    pkbuf = ogs_gtp2_build_msg(recv_message);
                    ogs_expect_or_return(pkbuf);

                    ogs_assert(sess->gnode);
                    s5c_xact = ogs_gtp_xact_local_create(
                            sess->gnode, &recv_message->h, pkbuf,
                            sess_timeout, sess);
                    ogs_expect_or_return(s5c_xact);
                    s5c_xact->local_teid = sess->sgw_s5c_teid;

                    ogs_gtp_xact_associate(s11_xact, s5c_xact);

                    rv = ogs_gtp_xact_commit(s5c_xact);
                    ogs_expect(rv == OGS_OK);

                } else {
                    gtp_rsp = &send_message.modify_bearer_response;
                    ogs_assert(gtp_rsp);

                    memset(&send_message, 0, sizeof(ogs_gtp2_message_t));

                    memset(&cause, 0, sizeof(cause));
                    cause.value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

                    gtp_rsp->cause.presence = 1;
                    gtp_rsp->cause.data = &cause;
                    gtp_rsp->cause.len = sizeof(cause);

        /* Copy Bearer-Contexts-Modified from Modify-Bearer-Request
         *
         * TS 29.274 Table 7.2.8-2
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
                    for (i = 0; i < OGS_BEARER_PER_UE; i++) {
                        if (gtp_req->bearer_contexts_to_be_modified[i].
                            presence == 0)
                            break;
                        if (gtp_req->bearer_contexts_to_be_modified[i].
                            eps_bearer_id.presence == 0)
                            break;
                        if (gtp_req->bearer_contexts_to_be_modified[i].
                            s1_u_enodeb_f_teid.presence == 0)
                            break;

                        gtp_rsp->bearer_contexts_modified[i].presence = 1;
                        gtp_rsp->bearer_contexts_modified[i].eps_bearer_id.
                            presence = 1;
                        gtp_rsp->bearer_contexts_modified[i].eps_bearer_id.u8 =
                            gtp_req->bearer_contexts_to_be_modified[i].
                                eps_bearer_id.u8;
                        gtp_rsp->bearer_contexts_modified[i].
                                s1_u_enodeb_f_teid.presence = 1;
                        gtp_rsp->bearer_contexts_modified[i].
                            s1_u_enodeb_f_teid.data =
                                gtp_req->bearer_contexts_to_be_modified[i].
                                    s1_u_enodeb_f_teid.data;
                        gtp_rsp->bearer_contexts_modified[i].
                            s1_u_enodeb_f_teid.len =
                                gtp_req->bearer_contexts_to_be_modified[i].
                                    s1_u_enodeb_f_teid.len;

                        gtp_rsp->bearer_contexts_modified[i].cause.presence = 1;
                        gtp_rsp->bearer_contexts_modified[i].cause.len =
                            sizeof(cause);
                        gtp_rsp->bearer_contexts_modified[i].cause.data =
                            &cause;
                    }

                    send_message.h.type = OGS_GTP2_MODIFY_BEARER_RESPONSE_TYPE;
                    send_message.h.teid = sgwc_ue->mme_s11_teid;

                    pkbuf = ogs_gtp2_build_msg(&send_message);
                    ogs_expect_or_return(pkbuf);

                    rv = ogs_gtp_xact_update_tx(
                            s11_xact, &send_message.h, pkbuf);
                    ogs_expect_or_return(rv == OGS_OK);

                    rv = ogs_gtp_xact_commit(s11_xact);
                    ogs_expect(rv == OGS_OK);
                }
            }
        } else {
            ogs_fatal("Invalid modify_flags[0x%llx]", (long long)flags);
            ogs_assert_if_reached();
        }
    } else if (flags & OGS_PFCP_MODIFY_DEACTIVATE) {
        if (flags & OGS_PFCP_MODIFY_ERROR_INDICATION) {
            /* It's faked method for receiving `bearer` context */
            bearer = pfcp_xact->assoc_xact;
            ogs_assert(bearer);

            ogs_pfcp_xact_commit(pfcp_xact);

            ogs_assert(flags & OGS_PFCP_MODIFY_SESSION);
            if (SGWC_SESSION_SYNC_DONE(sgwc_ue,
                    OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE, flags)) {
                ogs_assert(OGS_OK ==
                    sgwc_gtp_send_downlink_data_notification(
                        OGS_GTP2_CAUSE_ERROR_INDICATION_RECEIVED, bearer));
            }

        } else {
            s11_xact = pfcp_xact->assoc_xact;
            ogs_assert(s11_xact);

            ogs_pfcp_xact_commit(pfcp_xact);

            ogs_assert(flags & OGS_PFCP_MODIFY_SESSION);
            if (SGWC_SESSION_SYNC_DONE(sgwc_ue,
                    OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE, flags)) {

                ogs_gtp2_release_access_bearers_response_t *gtp_rsp = NULL;

                gtp_rsp = &send_message.release_access_bearers_response;
                ogs_assert(gtp_rsp);

                memset(&send_message, 0, sizeof(ogs_gtp2_message_t));

                memset(&cause, 0, sizeof(cause));
                cause.value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

                gtp_rsp->cause.presence = 1;
                gtp_rsp->cause.data = &cause;
                gtp_rsp->cause.len = sizeof(cause);

                send_message.h.type =
                    OGS_GTP2_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE;
                send_message.h.teid = sgwc_ue->mme_s11_teid;

                pkbuf = ogs_gtp2_build_msg(&send_message);
                ogs_expect_or_return(pkbuf);

                rv = ogs_gtp_xact_update_tx(s11_xact, &send_message.h, pkbuf);
                ogs_expect_or_return(rv == OGS_OK);

                rv = ogs_gtp_xact_commit(s11_xact);
                ogs_expect(rv == OGS_OK);
            }
        }
    } else {
        ogs_fatal("Invalid modify_flags[0x%llx]", (long long)flags);
        ogs_assert_if_reached();
    }
}

void sgwc_sxa_handle_session_deletion_response(
        sgwc_sess_t *sess, ogs_pfcp_xact_t *pfcp_xact,
        ogs_gtp2_message_t *gtp_message,
        ogs_pfcp_session_deletion_response_t *pfcp_rsp)
{
    int rv;
    uint8_t cause_value = 0;
    uint32_t teid = 0;

    sgwc_ue_t *sgwc_ue = NULL;

    ogs_gtp_xact_t *gtp_xact = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_debug("Session Deletion Response");

    ogs_assert(pfcp_xact);
    ogs_assert(pfcp_rsp);

    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_warn("No Context");
        cause_value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (pfcp_rsp->cause.presence) {
        if (pfcp_rsp->cause.u8 != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
            ogs_warn("PFCP Cause[%d] : Not Accepted", pfcp_rsp->cause.u8);
            cause_value = gtp_cause_from_pfcp(pfcp_rsp->cause.u8);
        }
    } else {
        ogs_error("No Cause");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    gtp_xact = pfcp_xact->assoc_xact;

    ogs_pfcp_xact_commit(pfcp_xact);

    if (!gtp_message) {
        ogs_error("No GTP Message");
        goto cleanup;
    }

    if (gtp_message->h.type == OGS_GTP2_DELETE_SESSION_REQUEST_TYPE) {
        /*
         * X2-based Handover with SGW change
         * 1. MME sends Delete Session Request to SGW-C
         * 2. SGW-C sends Delete Session Response to MME.
         */
        gtp_message->h.type = OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE;
    }

    switch (gtp_message->h.type) {
    case OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE:
        /*
         * 1. MME sends Delete Session Request to SGW/SMF.
         * 2. SMF sends Delete Session Response to SGW/MME.
         */
        if (sess) sgwc_ue = sess->sgwc_ue;
        teid = sgwc_ue ? sgwc_ue->mme_s11_teid : 0;
        break;
    case OGS_GTP2_DELETE_BEARER_RESPONSE_TYPE:
        /*
         * 1. SMF sends Delete Bearer Request(DEFAULT BEARER) to SGW/MME.
         * 2. MME sends Delete Bearer Response to SGW/SMF.
         *
         * OR
         *
         * 1. SMF sends Delete Bearer Request(DEFAULT BEARER) to ePDG.
         * 2. ePDG sends Delete Bearer Response(DEFAULT BEARER) to SMF.
         *
         * Note that the following messages are not processed here.
         * - Bearer Resource Command
         * - Delete Bearer Request/Response with DEDICATED BEARER.
         */
        teid = sess ? sess->pgw_s5c_teid : 0;
        break;
    default:
        ogs_fatal("Unknown GTP message type [%d]", gtp_message->h.type);
        ogs_assert_if_reached();
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        if (gtp_xact) {
            ogs_gtp_send_error_message(
                    gtp_xact, teid, gtp_message->h.type, cause_value);
        }
        return;
    }

    ogs_assert(sess);
    sgwc_ue = sess->sgwc_ue;
    ogs_assert(sgwc_ue);

    if (gtp_xact) {
        /*
         * If gtp_message->h.type == OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE
         * Then gtp_xact is S11-XACT
         *
         * If gtp_message->h.type == OGS_GTP2_DELETE_BEARER_RESPONSE_TYPE
         * Then gtp_xact is S5C-XACT
         */
        gtp_message->h.teid = teid;

        pkbuf = ogs_gtp2_build_msg(gtp_message);
        ogs_expect_or_return(pkbuf);

        rv = ogs_gtp_xact_update_tx(gtp_xact, &gtp_message->h, pkbuf);
        ogs_expect_or_return(rv == OGS_OK);

        rv = ogs_gtp_xact_commit(gtp_xact);
        ogs_expect(rv == OGS_OK);
    }

cleanup:
    sgwc_sess_remove(sess);
}

void sgwc_sxa_handle_session_report_request(
        sgwc_sess_t *sess, ogs_pfcp_xact_t *pfcp_xact,
        ogs_pfcp_session_report_request_t *pfcp_req)
{
    sgwc_ue_t *sgwc_ue = NULL;
    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *tunnel = NULL;

    ogs_pfcp_report_type_t report_type;
    uint8_t cause_value = 0;
    uint16_t pdr_id = 0;

    ogs_debug("Session Report Request");

    ogs_assert(pfcp_xact);
    ogs_assert(pfcp_req);

    cause_value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_warn("No Context");
        cause_value = OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND;
    }

    if (pfcp_req->report_type.presence == 0) {
        ogs_error("No Report Type");
        cause_value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ogs_pfcp_send_error_message(pfcp_xact, 0,
                OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE,
                cause_value, 0);
        return;
    }

    ogs_assert(sess);
    sgwc_ue = sess->sgwc_ue;
    ogs_assert(sgwc_ue);

    if (!sgwc_ue->gnode) {
        ogs_error("No SGWC-UE GTP Node");
        ogs_pfcp_send_error_message(pfcp_xact, sess ? sess->sgwu_sxa_seid : 0,
                OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE,
                OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND, 0);
        return;
    }

    ogs_assert(OGS_OK ==
        sgwc_pfcp_send_session_report_response(
            pfcp_xact, sess, OGS_PFCP_CAUSE_REQUEST_ACCEPTED));

    report_type.value = pfcp_req->report_type.u8;

    if (report_type.downlink_data_report) {
        if (pfcp_req->downlink_data_report.presence == 0) {
            ogs_error("No Downlink Data Report");
            return;
        }

        if (pfcp_req->downlink_data_report.pdr_id.presence == 0) {
            ogs_error("No PDR-ID");
            return;
        }

        pdr_id = pfcp_req->downlink_data_report.pdr_id.u16;

        ogs_list_for_each(&sess->bearer_list, bearer) {
            ogs_list_for_each(&bearer->tunnel_list, tunnel) {
                ogs_assert(tunnel->pdr);
                if (tunnel->pdr->id == pdr_id) {
                    ogs_assert(OGS_OK ==
                        sgwc_gtp_send_downlink_data_notification(
                            OGS_GTP2_CAUSE_UNDEFINED_VALUE, bearer));
                    return;
                }
            }
        }

        ogs_error("Cannot find the PDR-ID[%d]", pdr_id);

    } else if (report_type.error_indication_report) {
        bearer = sgwc_bearer_find_by_error_indication_report(
                sess, &pfcp_req->error_indication_report);

        if (!bearer) return;

        ogs_list_for_each(&sgwc_ue->sess_list, sess) {

            ogs_assert(OGS_OK ==
                sgwc_pfcp_send_session_modification_request(sess,
                /* We only use the `assoc_xact` parameter temporarily here
                 * to pass the `bearer` context. */
                    (ogs_gtp_xact_t *)bearer,
                    NULL,
                    OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_DEACTIVATE|
                    OGS_PFCP_MODIFY_ERROR_INDICATION));
        }

    } else {
        ogs_error("Not supported Report Type[%d]", report_type.value);
    }
}
