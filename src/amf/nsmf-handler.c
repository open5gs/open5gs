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

#include "nsmf-handler.h"
#include "nas-path.h"
#include "ngap-path.h"

#include "gmm-build.h"

int amf_nsmf_pdu_session_handle_create_sm_context(
        amf_sess_t *sess, ogs_sbi_message_t *recvmsg)
{
    ogs_assert(sess);
    ogs_assert(recvmsg);

    if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_CREATED) {
        int rv;
        ogs_sbi_message_t message;
        ogs_sbi_header_t header;

        if (!recvmsg->http.location) {
            ogs_error("[%d:%d] No http.location", sess->psi, sess->pti);
            nas_5gs_send_back_5gsm_message_from_sbi(sess,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
            return OGS_ERROR;
        }

        memset(&header, 0, sizeof(header));
        header.uri = recvmsg->http.location;

        rv = ogs_sbi_parse_header(&message, &header);
        if (rv != OGS_OK) {
            ogs_error("[%d:%d] Cannot parse http.location [%s]",
                    sess->psi, sess->pti, recvmsg->http.location);
            nas_5gs_send_back_5gsm_message_from_sbi(sess,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
            return OGS_ERROR;
        }

        if (!message.h.resource.component[1]) {
            ogs_error("[%d:%d] No SmContextRef [%s]",
                    sess->psi, sess->pti, recvmsg->http.location);

            ogs_sbi_header_free(&header);
            nas_5gs_send_back_5gsm_message_from_sbi(sess,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
            return OGS_ERROR;
        }

        if (sess->sm_context_ref)
            ogs_free(sess->sm_context_ref);
        sess->sm_context_ref = ogs_strdup(message.h.resource.component[1]);

        ogs_sbi_header_free(&header);

    } else {
        OpenAPI_sm_context_create_error_t *SmContextCreateError = NULL;
        OpenAPI_ref_to_binary_data_t *n1SmMsg = NULL;
        ogs_pkbuf_t *n1smbuf = NULL;

        SmContextCreateError = recvmsg->SmContextCreateError;
        if (!SmContextCreateError) {
            ogs_error("[%d:%d] No SmContextCreateError", sess->psi, sess->pti);
            nas_5gs_send_back_5gsm_message_from_sbi(sess, recvmsg->res_status);
            return OGS_ERROR;
        }
        if (!SmContextCreateError->error) {
            ogs_error("[%d:%d] No Error", sess->psi, sess->pti);
            nas_5gs_send_back_5gsm_message_from_sbi(sess, recvmsg->res_status);
            return OGS_ERROR;
        }
        n1SmMsg = SmContextCreateError->n1_sm_msg;
        if (!n1SmMsg || !n1SmMsg->content_id) {
            ogs_error("[%d:%d] No N1 SM Message", sess->psi, sess->pti);
            nas_5gs_send_back_5gsm_message_from_sbi(sess, recvmsg->res_status);
            return OGS_ERROR;
        }

        n1smbuf = ogs_sbi_find_part_by_content_id(recvmsg, n1SmMsg->content_id);
        if (!n1smbuf) {
            ogs_error("[%d:%d] No N1 SM Content [%s]",
                    sess->psi, sess->pti, n1SmMsg->content_id);
            nas_5gs_send_back_5gsm_message_from_sbi(sess, recvmsg->res_status);
            return OGS_ERROR;
        }

        n1smbuf = ogs_pkbuf_copy(n1smbuf);
        ogs_assert(n1smbuf);
        nas_5gs_send_gsm_reject_from_sbi(sess,
            OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION,
            n1smbuf, recvmsg->res_status);
    }

    return OGS_OK;
}

int amf_nsmf_pdu_session_handle_update_sm_context(
        amf_sess_t *sess, ogs_sbi_message_t *recvmsg)
{
    ogs_assert(sess);
    ogs_assert(recvmsg);

    if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_NO_CONTENT) {
        /* Nothing */

    } else if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_OK) {
        ogs_fatal("TODO");

    } else {
        amf_ue_t *amf_ue = NULL;

        OpenAPI_sm_context_update_error_t *SmContextUpdateError = NULL;
#if 0 /* Is it needed? */
        OpenAPI_ref_to_binary_data_t *n2SmInfo = NULL;
        ogs_pkbuf_t *n2smbuf = NULL;
#endif

        amf_ue = sess->amf_ue;
        ogs_assert(amf_ue);

        SmContextUpdateError = recvmsg->SmContextUpdateError;
        if (!SmContextUpdateError) {
            ogs_error("[%d:%d] No SmContextUpdateError", sess->psi, sess->pti);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return OGS_ERROR;
        }
        if (!SmContextUpdateError->error) {
            ogs_error("[%d:%d] No Error", sess->psi, sess->pti);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return OGS_ERROR;
        }
#if 0 /* Is it needed? */
        n2SmInfo = SmContextUpdateError->n2_sm_info;
        if (!n2SmInfo || !n2SmInfo->content_id) {
            ogs_error("[%d:%d] No N2 SM Message", sess->psi, sess->pti);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return OGS_ERROR;
        }

        n2smbuf = ogs_sbi_find_part_by_content_id(
                recvmsg, n2SmInfo->content_id);
        if (!n2smbuf) {
            ogs_error("[%d:%d] No N2 SM Content [%s]",
                    sess->psi, sess->pti, n2SmInfo->content_id);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return OGS_ERROR;
        }
#endif

        ngap_send_error_indication2(amf_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
    }

    return OGS_OK;
}
