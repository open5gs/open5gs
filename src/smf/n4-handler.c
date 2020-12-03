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

#include "context.h"
#include "timer.h"
#include "pfcp-path.h"
#include "gtp-path.h"
#include "n4-handler.h"
#include "bearer-binding.h"
#include "sbi-path.h"

static uint8_t gtp_cause_from_pfcp(uint8_t pfcp_cause)
{
    switch (pfcp_cause) {
    case OGS_PFCP_CAUSE_REQUEST_ACCEPTED:
        return OGS_GTP_CAUSE_REQUEST_ACCEPTED;
    case OGS_PFCP_CAUSE_REQUEST_REJECTED:
        return OGS_GTP_CAUSE_REQUEST_REJECTED_REASON_NOT_SPECIFIED;
    case OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND:
        return OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    case OGS_PFCP_CAUSE_MANDATORY_IE_MISSING:
        return OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    case OGS_PFCP_CAUSE_CONDITIONAL_IE_MISSING:
        return OGS_GTP_CAUSE_CONDITIONAL_IE_MISSING;
    case OGS_PFCP_CAUSE_INVALID_LENGTH:
        return OGS_GTP_CAUSE_INVALID_LENGTH;
    case OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT:
        return OGS_GTP_CAUSE_MANDATORY_IE_INCORRECT;
    case OGS_PFCP_CAUSE_INVALID_FORWARDING_POLICY:
    case OGS_PFCP_CAUSE_INVALID_F_TEID_ALLOCATION_OPTION:
        return OGS_GTP_CAUSE_INVALID_MESSAGE_FORMAT;
    case OGS_PFCP_CAUSE_NO_ESTABLISHED_PFCP_ASSOCIATION:
        return OGS_GTP_CAUSE_REMOTE_PEER_NOT_RESPONDING;
    case OGS_PFCP_CAUSE_RULE_CREATION_MODIFICATION_FAILURE:
        return OGS_GTP_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION;
    case OGS_PFCP_CAUSE_PFCP_ENTITY_IN_CONGESTION:
        return OGS_GTP_CAUSE_GTP_C_ENTITY_CONGESTION;
    case OGS_PFCP_CAUSE_NO_RESOURCES_AVAILABLE:
        return OGS_GTP_CAUSE_NO_RESOURCES_AVAILABLE;
    case OGS_PFCP_CAUSE_SERVICE_NOT_SUPPORTED:
        return OGS_GTP_CAUSE_SERVICE_NOT_SUPPORTED;
    case OGS_PFCP_CAUSE_SYSTEM_FAILURE:
        return OGS_GTP_CAUSE_SYSTEM_FAILURE;
    default:
        return OGS_GTP_CAUSE_SYSTEM_FAILURE;
    }

    return OGS_GTP_CAUSE_SYSTEM_FAILURE;
}
static int sbi_status_from_pfcp(uint8_t pfcp_cause)
{
    switch (pfcp_cause) {
    case OGS_PFCP_CAUSE_REQUEST_ACCEPTED:
        return OGS_SBI_HTTP_STATUS_OK;
    case OGS_PFCP_CAUSE_REQUEST_REJECTED:
        return OGS_SBI_HTTP_STATUS_FORBIDDEN;
    case OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND:
        return OGS_SBI_HTTP_STATUS_NOT_FOUND;
    case OGS_PFCP_CAUSE_MANDATORY_IE_MISSING:
    case OGS_PFCP_CAUSE_CONDITIONAL_IE_MISSING:
    case OGS_PFCP_CAUSE_INVALID_LENGTH:
    case OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT:
    case OGS_PFCP_CAUSE_INVALID_FORWARDING_POLICY:
    case OGS_PFCP_CAUSE_INVALID_F_TEID_ALLOCATION_OPTION:
    case OGS_PFCP_CAUSE_RULE_CREATION_MODIFICATION_FAILURE:
    case OGS_PFCP_CAUSE_PFCP_ENTITY_IN_CONGESTION:
    case OGS_PFCP_CAUSE_NO_RESOURCES_AVAILABLE:
        return OGS_SBI_HTTP_STATUS_BAD_REQUEST;
    case OGS_PFCP_CAUSE_NO_ESTABLISHED_PFCP_ASSOCIATION:
        return OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT;
    case OGS_PFCP_CAUSE_SERVICE_NOT_SUPPORTED:
        return OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE;
    case OGS_PFCP_CAUSE_SYSTEM_FAILURE:
        return OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR;
    default:
        return OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR;
    }

    return OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR;
}

void smf_5gc_n4_handle_session_establishment_response(
        smf_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_establishment_response_t *rsp)
{
    int i;

    ogs_sbi_stream_t *stream = NULL;

    uint8_t pfcp_cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
    uint8_t offending_ie_value = 0;

    ogs_pfcp_f_seid_t *up_f_seid = NULL;

    ogs_assert(xact);
    ogs_assert(rsp);

    stream = xact->assoc_stream;
    ogs_assert(stream);

    ogs_pfcp_xact_commit(xact);

    if (!sess) {
        ogs_warn("No Context");
        return;
    }

    if (rsp->up_f_seid.presence == 0) {
        ogs_error("No UP F-SEID");
        return;
    }

    if (rsp->created_pdr[0].presence == 0) {
        ogs_error("No Created PDR");
        return;
    }

    if (rsp->cause.presence) {
        if (rsp->cause.u8 != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
            ogs_error("PFCP Cause [%d] : Not Accepted", rsp->cause.u8);
            return;
        }
    } else {
        ogs_error("No Cause");
        return;
    }

    ogs_assert(sess);

    pfcp_cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
    for (i = 0; i < OGS_MAX_NUM_OF_PDR; i++) {
        ogs_pfcp_pdr_t *pdr = NULL;

        pdr = ogs_pfcp_handle_created_pdr(
                &sess->pfcp, &rsp->created_pdr[i],
                &pfcp_cause_value, &offending_ie_value);

        if (!pdr)
            break;

        if (pdr->src_if != OGS_PFCP_INTERFACE_ACCESS)
            continue;

        ogs_assert(sess->pfcp_node);
        if (sess->pfcp_node->up_function_features.ftup &&
            pdr->f_teid_len) {
            if (sess->upf_n3_addr)
                ogs_freeaddrinfo(sess->upf_n3_addr);
            if (sess->upf_n3_addr6)
                ogs_freeaddrinfo(sess->upf_n3_addr6);

            ogs_pfcp_f_teid_to_sockaddr(
                    &pdr->f_teid, pdr->f_teid_len,
                    &sess->upf_n3_addr, &sess->upf_n3_addr6);
            sess->upf_n3_teid = pdr->f_teid.teid;
        }
    }

    if (pfcp_cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
        ogs_error("PFCP Cause [%d] : Not Accepted", pfcp_cause_value);
        return;
    }

    if (sess->upf_n3_addr == NULL && sess->upf_n3_addr6 == NULL) {
        ogs_error("No UP F-TEID");
        return;
    }

    /* UP F-SEID */
    up_f_seid = rsp->up_f_seid.data;
    ogs_assert(up_f_seid);
    sess->upf_n4_seid = be64toh(up_f_seid->seid);

    smf_sbi_discover_and_send(OpenAPI_nf_type_AMF, sess, stream, NULL,
            smf_namf_comm_build_n1_n2_message_transfer);

#if 0
    smf_bearer_binding(sess);
#endif
}

void smf_5gc_n4_handle_session_modification_response(
        smf_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_modification_response_t *rsp)
{
    int status = 0;
    uint64_t flags = 0;
    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(xact);
    ogs_assert(rsp);

    stream = xact->assoc_stream;
    ogs_assert(stream);
    flags = xact->modify_flags;
    ogs_assert(flags);

    ogs_pfcp_xact_commit(xact);

    status = OGS_SBI_HTTP_STATUS_OK;

    if (!sess) {
        ogs_warn("No Context");
        status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
    }

    if (rsp->cause.presence) {
        if (rsp->cause.u8 != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
            ogs_warn("PFCP Cause [%d] : Not Accepted", rsp->cause.u8);
            status = sbi_status_from_pfcp(rsp->cause.u8);
        }
    } else {
        ogs_error("No Cause");
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
    }

    if (status == OGS_SBI_HTTP_STATUS_OK) {
        int i;

        uint8_t pfcp_cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
        uint8_t offending_ie_value = 0;

        ogs_assert(sess);
        for (i = 0; i < OGS_MAX_NUM_OF_PDR; i++) {
            ogs_pfcp_pdr_t *pdr = NULL;

            pdr = ogs_pfcp_handle_created_pdr(
                    &sess->pfcp, &rsp->created_pdr[i],
                    &pfcp_cause_value, &offending_ie_value);

            if (!pdr)
                break;

            if (pdr->src_if != OGS_PFCP_INTERFACE_ACCESS)
                continue;

            ogs_assert(sess->pfcp_node);
            if (sess->pfcp_node->up_function_features.ftup &&
                pdr->f_teid_len) {
                if (sess->upf_n3_addr)
                    ogs_freeaddrinfo(sess->upf_n3_addr);
                if (sess->upf_n3_addr6)
                    ogs_freeaddrinfo(sess->upf_n3_addr6);

                ogs_pfcp_f_teid_to_sockaddr(
                        &pdr->f_teid, pdr->f_teid_len,
                        &sess->upf_n3_addr, &sess->upf_n3_addr6);
                sess->upf_n3_teid = pdr->f_teid.teid;
            }
        }

        status = sbi_status_from_pfcp(pfcp_cause_value);
    }

    if (status != OGS_SBI_HTTP_STATUS_OK) {
        char *strerror = ogs_msprintf(
                "PFCP Cause [%d] : Not Accepted", rsp->cause.u8);
        smf_sbi_send_sm_context_update_error(stream, status, strerror,
                NULL, NULL, NULL);
        ogs_free(strerror);
        return;
    }

    ogs_assert(sess);

    if (sess->upf_n3_addr == NULL && sess->upf_n3_addr6 == NULL) {
        smf_sbi_send_sm_context_update_error(stream, status, "No UP F_TEID",
                NULL, NULL, NULL);
        return;
    }

    if (flags & OGS_PFCP_MODIFY_ACTIVATE) {
        /* ACTIVATED Is NOT Inlcuded in RESPONSE */
        smf_sbi_send_sm_context_updated_data(sess, stream, 0);

    } else if (flags & OGS_PFCP_MODIFY_DEACTIVATE) {
        /* Only ACTIVING & DEACTIVATED is Included */
        smf_sbi_send_sm_context_updated_data(
                sess, stream, OpenAPI_up_cnx_state_DEACTIVATED);
    }
}

void smf_5gc_n4_handle_session_deletion_response(
        smf_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_deletion_response_t *rsp)
{
    int status = 0;
    int trigger;

    ogs_sbi_stream_t *stream = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(xact);
    ogs_assert(rsp);

    stream = xact->assoc_stream;
    ogs_assert(stream);
    trigger = xact->delete_trigger;
    ogs_assert(trigger);

    ogs_pfcp_xact_commit(xact);

    status = OGS_SBI_HTTP_STATUS_OK;

    if (!sess) {
        ogs_warn("No Context");
        status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
    }

    if (rsp->cause.presence) {
        if (rsp->cause.u8 != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
            ogs_warn("PFCP Cause [%d] : Not Accepted", rsp->cause.u8);
            status = sbi_status_from_pfcp(rsp->cause.u8);
        }
    } else {
        ogs_error("No Cause");
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
    }

    if (status != OGS_SBI_HTTP_STATUS_OK) {
        char *strerror = ogs_msprintf(
                "PFCP Cause [%d] : Not Accepted", rsp->cause.u8);
        ogs_sbi_server_send_error(stream, status, NULL, NULL, NULL);
        ogs_free(strerror);
        return;
    }

    ogs_assert(sess);

    if (trigger == OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED) {

        smf_sbi_send_sm_context_updated_data_in_session_deletion(sess, stream);

    } else {

        memset(&sendmsg, 0, sizeof(sendmsg));

        response = ogs_sbi_build_response(
                &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        SMF_SESS_CLEAR(sess);
    }
}

void smf_epc_n4_handle_session_establishment_response(
        smf_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_establishment_response_t *rsp)
{
    uint8_t cause_value = 0;

    smf_bearer_t *bearer = NULL;
    ogs_gtp_xact_t *gtp_xact = NULL;

    ogs_pfcp_f_seid_t *up_f_seid = NULL;

    ogs_assert(xact);
    ogs_assert(rsp);

    gtp_xact = xact->assoc_xact;
    ogs_assert(gtp_xact);

    ogs_pfcp_xact_commit(xact);

    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (rsp->up_f_seid.presence == 0) {
        ogs_error("No UP F-SEID");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (rsp->created_pdr[0].presence == 0) {
        ogs_error("No Created PDR");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (rsp->cause.presence) {
        if (rsp->cause.u8 != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
            ogs_warn("PFCP Cause [%d] : Not Accepted", rsp->cause.u8);
            cause_value = gtp_cause_from_pfcp(rsp->cause.u8);
        }
    } else {
        ogs_error("No Cause");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value == OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        int i;

        uint8_t pfcp_cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
        uint8_t offending_ie_value = 0;

        ogs_assert(sess);
        for (i = 0; i < OGS_MAX_NUM_OF_PDR; i++) {
            ogs_pfcp_pdr_t *pdr = NULL;

            pdr = ogs_pfcp_handle_created_pdr(
                    &sess->pfcp, &rsp->created_pdr[i],
                    &pfcp_cause_value, &offending_ie_value);

            if (!pdr)
                break;

            if (pdr->src_if != OGS_PFCP_INTERFACE_ACCESS)
                continue;

            bearer = smf_bearer_find_by_pdr_id(sess, pdr->id);
            if (!bearer) {
                pfcp_cause_value = OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND;
                break;
            }

            ogs_assert(sess->pfcp_node);
            if (sess->pfcp_node->up_function_features.ftup &&
                pdr->f_teid_len) {
                if (bearer->pgw_s5u_addr)
                    ogs_freeaddrinfo(bearer->pgw_s5u_addr);
                if (bearer->pgw_s5u_addr)
                    ogs_freeaddrinfo(bearer->pgw_s5u_addr6);

                ogs_pfcp_f_teid_to_sockaddr(
                        &pdr->f_teid, pdr->f_teid_len,
                        &bearer->pgw_s5u_addr, &bearer->pgw_s5u_addr6);
                bearer->pgw_s5u_teid = pdr->f_teid.teid;
            }
        }

        cause_value = gtp_cause_from_pfcp(pfcp_cause_value);
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(gtp_xact, sess ? sess->sgw_s5c_teid : 0,
                OGS_GTP_CREATE_SESSION_RESPONSE_TYPE, cause_value);
        return;
    }

    ogs_assert(sess);
    bearer = smf_default_bearer_in_sess(sess);
    ogs_assert(bearer);

    if (bearer->pgw_s5u_addr == NULL && bearer->pgw_s5u_addr6 == NULL) {
        ogs_error("No UP F-TEID");
        ogs_gtp_send_error_message(gtp_xact, sess ? sess->sgw_s5c_teid : 0,
                OGS_GTP_CREATE_SESSION_RESPONSE_TYPE,
                OGS_GTP_CAUSE_GRE_KEY_NOT_FOUND);
        return;
    }

    /* UP F-SEID */
    up_f_seid = rsp->up_f_seid.data;
    ogs_assert(up_f_seid);
    sess->upf_n4_seid = be64toh(up_f_seid->seid);

    smf_gtp_send_create_session_response(sess, gtp_xact);

    smf_bearer_binding(sess);
}

void smf_epc_n4_handle_session_modification_response(
        smf_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_modification_response_t *rsp)
{
    int i;

    smf_bearer_t *bearer = NULL;
    uint64_t flags = 0;

    uint8_t pfcp_cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
    uint8_t offending_ie_value = 0;

    ogs_assert(xact);
    ogs_assert(rsp);

    bearer = xact->data;
    ogs_assert(bearer);
    flags = xact->modify_flags;
    ogs_assert(flags);

    ogs_pfcp_xact_commit(xact);

    if (!sess) {
        ogs_error("No Context");
        return;
    }

    if (rsp->cause.presence) {
        if (rsp->cause.u8 != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
            ogs_error("PFCP Cause [%d] : Not Accepted", rsp->cause.u8);
            return;
        }
    } else {
        ogs_error("No Cause");
        return;
    }

    ogs_assert(sess);

    pfcp_cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;
    for (i = 0; i < OGS_MAX_NUM_OF_PDR; i++) {
        ogs_pfcp_pdr_t *pdr = NULL;

        pdr = ogs_pfcp_handle_created_pdr(
                &sess->pfcp, &rsp->created_pdr[i],
                &pfcp_cause_value, &offending_ie_value);

        if (!pdr)
            break;

        if (pdr->src_if != OGS_PFCP_INTERFACE_ACCESS)
            continue;

        ogs_assert(sess->pfcp_node);
        if (sess->pfcp_node->up_function_features.ftup &&
            pdr->f_teid_len) {
            if (bearer->pgw_s5u_addr)
                ogs_freeaddrinfo(bearer->pgw_s5u_addr);
            if (bearer->pgw_s5u_addr)
                ogs_freeaddrinfo(bearer->pgw_s5u_addr6);

            ogs_pfcp_f_teid_to_sockaddr(
                    &pdr->f_teid, pdr->f_teid_len,
                    &bearer->pgw_s5u_addr, &bearer->pgw_s5u_addr6);
            bearer->pgw_s5u_teid = pdr->f_teid.teid;
        }
    }

    if (pfcp_cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
        ogs_error("PFCP Cause [%d] : Not Accepted", pfcp_cause_value);
        return;
    }

    if (flags & OGS_PFCP_MODIFY_CREATE) {
        smf_gtp_send_create_bearer_request(bearer);

    } else if (flags & OGS_PFCP_MODIFY_REMOVE) {
        smf_bearer_remove(bearer);

    } else if (flags & OGS_PFCP_MODIFY_ACTIVATE) {
        /* Nothing */
    }
}

void smf_epc_n4_handle_session_deletion_response(
        smf_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_deletion_response_t *rsp)
{
    uint8_t cause_value = 0;
    ogs_gtp_xact_t *gtp_xact = NULL;

    ogs_assert(xact);
    ogs_assert(rsp);

    gtp_xact = xact->assoc_xact;
    ogs_assert(gtp_xact);

    ogs_pfcp_xact_commit(xact);

    cause_value = OGS_GTP_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_warn("No Context");
        cause_value = OGS_GTP_CAUSE_CONTEXT_NOT_FOUND;
    }

    if (rsp->cause.presence) {
        if (rsp->cause.u8 != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
            ogs_warn("PFCP Cause[%d] : Not Accepted", rsp->cause.u8);
            cause_value = gtp_cause_from_pfcp(rsp->cause.u8);
        }
    } else {
        ogs_error("No Cause");
        cause_value = OGS_GTP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_GTP_CAUSE_REQUEST_ACCEPTED) {
        ogs_gtp_send_error_message(gtp_xact, sess ? sess->sgw_s5c_teid : 0,
                OGS_GTP_DELETE_SESSION_RESPONSE_TYPE, cause_value);
        return;
    }

    ogs_assert(sess);

    smf_gtp_send_delete_session_response(sess, gtp_xact);

    SMF_SESS_CLEAR(sess);
}
