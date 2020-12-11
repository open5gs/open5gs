/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "sbi-path.h"

#include "npcf-handler.h"

bool pcf_npcf_am_policy_contrtol_handle_create(pcf_ue_t *pcf_ue,
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    OpenAPI_policy_association_request_t *PolicyAssociationRequest = NULL;
    OpenAPI_guami_t *Guami = NULL;

    ogs_assert(pcf_ue);
    ogs_assert(stream);
    ogs_assert(message);

    PolicyAssociationRequest = message->PolicyAssociationRequest;
    if (!PolicyAssociationRequest) {
        ogs_error("[%s] No PolicyAssociationRequest", pcf_ue->supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "[%s] No PolicyAssociationRequest", pcf_ue->supi);
        return false;
    }

    if (!PolicyAssociationRequest->notification_uri) {
        ogs_error("[%s] No notificationUri", pcf_ue->supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No notificationUri", pcf_ue->supi);
        return false;
    }

    if (!PolicyAssociationRequest->supi) {
        ogs_error("[%s] No supi", pcf_ue->supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No supi", pcf_ue->supi);
        return false;
    }

    if (!PolicyAssociationRequest->supp_feat) {
        ogs_error("[%s] No suppFeat", pcf_ue->supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No suppFeat", pcf_ue->supi);
        return false;
    }

    if (pcf_ue->notification_uri)
        ogs_free(pcf_ue->notification_uri);
    pcf_ue->notification_uri = ogs_strdup(
            PolicyAssociationRequest->notification_uri);

    Guami = PolicyAssociationRequest->guami;
    if (Guami && Guami->amf_id &&
        Guami->plmn_id && Guami->plmn_id->mnc && Guami->plmn_id->mcc) {
        ogs_sbi_parse_guami(&pcf_ue->guami, PolicyAssociationRequest->guami);
    }

    if (PolicyAssociationRequest->rat_type)
        pcf_ue->rat_type = PolicyAssociationRequest->rat_type;

    pcf_ue->policy_association_request =
        OpenAPI_policy_association_request_copy(
                pcf_ue->policy_association_request,
                message->PolicyAssociationRequest);

    pcf_ue_sbi_discover_and_send(OpenAPI_nf_type_UDR, pcf_ue, stream, NULL,
            pcf_nudr_dr_build_query_am_data);

    return true;
}

bool pcf_npcf_smpolicycontrtol_handle_create(pcf_sess_t *sess,
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    char *strerror = NULL;
    pcf_ue_t *pcf_ue = NULL;

    OpenAPI_sm_policy_context_data_t *SmPolicyContextData = NULL;
    OpenAPI_snssai_t *sliceInfo = NULL;

    ogs_assert(sess);
    pcf_ue = sess->pcf_ue;
    ogs_assert(stream);
    ogs_assert(message);

    SmPolicyContextData = message->SmPolicyContextData;
    if (!SmPolicyContextData) {
        strerror = ogs_msprintf("[%s:%d] No SmPolicyContextData",
                pcf_ue->supi, sess->psi);
        goto cleanup;
    }

    if (!SmPolicyContextData->supi) {
        strerror = ogs_msprintf("[%s:%d] No supi", pcf_ue->supi, sess->psi);
        goto cleanup;
    }

    if (!SmPolicyContextData->pdu_session_id) {
        strerror = ogs_msprintf("[%s:%d] No pduSessionId",
                pcf_ue->supi, sess->psi);
        goto cleanup;
    }

    if (!SmPolicyContextData->pdu_session_type) {
        strerror = ogs_msprintf("[%s:%d] No pduSessionType",
                pcf_ue->supi, sess->psi);
        goto cleanup;
    }

    if (!SmPolicyContextData->dnn) {
        strerror = ogs_msprintf("[%s:%d] No dnn", pcf_ue->supi, sess->psi);
        goto cleanup;
    }

    if (!SmPolicyContextData->notification_uri) {
        strerror = ogs_msprintf("[%s:%d] No notificationUri",
                pcf_ue->supi, sess->psi);
        goto cleanup;
    }

    sliceInfo = SmPolicyContextData->slice_info;
    if (!sliceInfo) {
        strerror = ogs_msprintf("[%s:%d] No sliceInfo",
                pcf_ue->supi, sess->psi);
        goto cleanup;
    }

    sess->pdu_session_type = SmPolicyContextData->pdu_session_type;

    if (sess->dnn)
        ogs_free(sess->dnn);
    sess->dnn = ogs_strdup(SmPolicyContextData->dnn);

    if (sess->notification_uri)
        ogs_free(sess->notification_uri);
    sess->notification_uri = ogs_strdup(SmPolicyContextData->notification_uri);

    sess->s_nssai.sst = sliceInfo->sst;
    sess->s_nssai.sd = ogs_s_nssai_sd_from_string(sliceInfo->sd);

    pcf_sess_sbi_discover_and_send(OpenAPI_nf_type_UDR, sess, stream, NULL,
            pcf_nudr_dr_build_query_sm_data);

    return true;

cleanup:
    ogs_assert(strerror);
    ogs_error("%s", strerror);
    ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
            message, strerror, NULL);
    ogs_free(strerror);

    return false;
}
