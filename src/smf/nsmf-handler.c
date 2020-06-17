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

#include "sbi-path.h"
#include "nas-path.h"
#include "ngap-path.h"
#include "nsmf-handler.h"

bool smf_nsmf_handle_create_sm_context(
        smf_sess_t *sess, ogs_sbi_message_t *message)
{
    ogs_sbi_session_t *session = NULL;

    OpenAPI_sm_context_create_data_t *SmContextCreateData = NULL;
    OpenAPI_snssai_t *sNssai = NULL;
    OpenAPI_plmn_id_nid_t *servingNetwork = NULL;
    OpenAPI_ref_to_binary_data_t *n1SmMsg = NULL;

    ogs_pkbuf_t *n1smbuf = NULL;

    ogs_assert(sess);
    session = sess->sbi.session;
    ogs_assert(session);

    ogs_assert(message);

    SmContextCreateData = message->SmContextCreateData;
    if (!SmContextCreateData) {
        ogs_error("[%s:%d] No SmContextCreateData", sess->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No SmContextCreateData", sess->supi_psi_keybuf, n1smbuf);
        return false;
    }

    sNssai = SmContextCreateData->s_nssai;
    if (!sNssai) {
        ogs_error("[%s:%d] No sNssai", sess->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No sNssai", sess->supi_psi_keybuf, n1smbuf);
        return false;
    }

    servingNetwork = SmContextCreateData->serving_network;
    if (!servingNetwork || !servingNetwork->mnc || !servingNetwork->mcc) {
        ogs_error("[%s:%d] No servingNetwork", sess->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No servingNetwork", sess->supi_psi_keybuf, n1smbuf);
        return false;
    }

    n1SmMsg = SmContextCreateData->n1_sm_msg;
    if (!n1SmMsg || !n1SmMsg->content_id) {
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N1 SM Message", sess->supi_psi_keybuf, n1smbuf);
        return false;
    }

    n1smbuf = ogs_sbi_find_part_by_content_id(message, n1SmMsg->content_id);
    if (!n1smbuf) {
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_create_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N1 SM Content", sess->supi_psi_keybuf, n1smbuf);
        return false;
    }

    ogs_plmn_id_build(&sess->plmn_id,
        atoi(servingNetwork->mcc), atoi(servingNetwork->mnc),
        strlen(servingNetwork->mnc));
    sess->nid = servingNetwork->nid;

    sess->s_nssai.sst = sNssai->sst;
    sess->s_nssai.sd = ogs_s_nssai_sd_from_string(sNssai->sd);

    if (SmContextCreateData->dnn) {
        if (sess->dnn) ogs_free(sess->dnn);
        sess->dnn = ogs_strdup(SmContextCreateData->dnn);
    }

    nas_5gs_send_to_gsm(sess, n1smbuf);

    return true;
}

bool smf_nsmf_handle_update_sm_context(
        smf_sess_t *sess, ogs_sbi_message_t *message)
{
    ogs_sbi_session_t *session = NULL;

    OpenAPI_sm_context_update_data_t *SmContextUpdateData = NULL;
    OpenAPI_ref_to_binary_data_t *n2SmMsg = NULL;

    ogs_pkbuf_t *n2smbuf = NULL;

    ogs_assert(sess);
    session = sess->sbi.session;
    ogs_assert(session);

    ogs_assert(message);

    SmContextUpdateData = message->SmContextUpdateData;
    if (!SmContextUpdateData) {
        ogs_error("[%s:%d] No SmContextUpdateData", sess->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No SmContextUpdateData", sess->supi_psi_keybuf, NULL);
        return false;
    }
    
    if (!SmContextUpdateData->n2_sm_info_type) {
        ogs_error("[%s:%d] No n2SmInfoType", sess->supi, sess->psi);
        smf_sbi_send_sm_context_update_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N2 SM Info Type", sess->supi_psi_keybuf, NULL);
        return false;
    }

    n2SmMsg = SmContextUpdateData->n2_sm_info;
    if (!n2SmMsg || !n2SmMsg->content_id) {
        smf_sbi_send_sm_context_update_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No n2SmInfo", sess->supi_psi_keybuf, NULL);
        return false;
    }

    n2smbuf = ogs_sbi_find_part_by_content_id(message, n2SmMsg->content_id);
    if (!n2smbuf) {
        smf_sbi_send_sm_context_update_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N2 SM Content", sess->supi_psi_keybuf, NULL);
        return false;
    }

    ngap_send_to_n2sm(sess, SmContextUpdateData->n2_sm_info_type, n2smbuf);

    return true;
}

bool smf_nsmf_handle_release_sm_context(
        smf_sess_t *sess, ogs_sbi_message_t *message)
{
    ogs_fatal("TODO");
#if 0
    ogs_sbi_session_t *session = NULL;

    OpenAPI_sm_context_release_data_t *SmContextReleaseData = NULL;
    OpenAPI_snssai_t *sNssai = NULL;
    OpenAPI_plmn_id_nid_t *servingNetwork = NULL;
    OpenAPI_ref_to_binary_data_t *n1SmMsg = NULL;

    ogs_pkbuf_t *n1smbuf = NULL;

    ogs_assert(sess);
    session = sess->sbi.session;
    ogs_assert(session);

    ogs_assert(message);

    SmContextReleaseData = message->SmContextReleaseData;
    if (!SmContextReleaseData) {
        ogs_error("[%s:%d] No SmContextReleaseData", sess->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_release_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No SmContextReleaseData", sess->supi_psi_keybuf, n1smbuf);
        return false;
    }

    sNssai = SmContextReleaseData->s_nssai;
    if (!sNssai) {
        ogs_error("[%s:%d] No sNssai", sess->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_release_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No sNssai", sess->supi_psi_keybuf, n1smbuf);
        return false;
    }

    servingNetwork = SmContextReleaseData->serving_network;
    if (!servingNetwork || !servingNetwork->mnc || !servingNetwork->mcc) {
        ogs_error("[%s:%d] No servingNetwork", sess->supi, sess->psi);
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_release_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No servingNetwork", sess->supi_psi_keybuf, n1smbuf);
        return false;
    }

    n1SmMsg = SmContextReleaseData->n1_sm_msg;
    if (!n1SmMsg || !n1SmMsg->content_id) {
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_release_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N1 SM Message", sess->supi_psi_keybuf, n1smbuf);
        return false;
    }

    n1smbuf = ogs_sbi_find_part_by_content_id(message, n1SmMsg->content_id);
    if (!n1smbuf) {
        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION);
        smf_sbi_send_sm_context_release_error(session,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                "No N1 SM Content", sess->supi_psi_keybuf, n1smbuf);
        return false;
    }

    ogs_plmn_id_build(&sess->plmn_id,
        atoi(servingNetwork->mcc), atoi(servingNetwork->mnc),
        strlen(servingNetwork->mnc));
    sess->nid = servingNetwork->nid;

    sess->s_nssai.sst = sNssai->sst;
    sess->s_nssai.sd = ogs_s_nssai_sd_from_string(sNssai->sd);

    if (SmContextReleaseData->dnn) {
        if (sess->dnn) ogs_free(sess->dnn);
        sess->dnn = ogs_strdup(SmContextReleaseData->dnn);
    }

    nas_5gs_send_to_gsm(sess, n1smbuf);
#endif

    return true;
}
