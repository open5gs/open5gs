/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#include "nas-security.h"
#include "gmm-build.h"
#include "amf-sm.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __gmm_log_domain

static uint16_t get_pdu_session_status(amf_ue_t *amf_ue);
static uint16_t get_pdu_session_reactivation_result(amf_ue_t *amf_ue);

ogs_pkbuf_t *gmm_build_registration_accept(amf_ue_t *amf_ue)
{
    int rv, served_tai_index = 0;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_registration_accept_t *registration_accept =
        &message.gmm.registration_accept;
    ogs_nas_5gs_registration_result_t *registration_result =
        &registration_accept->registration_result;
    ogs_nas_5gs_mobile_identity_t *mobile_identity =
        &registration_accept->guti;
    ogs_nas_5gs_mobile_identity_guti_t mobile_identity_guti;
    ogs_nas_nssai_t *allowed_nssai = &registration_accept->allowed_nssai;
    ogs_nas_rejected_nssai_t *rejected_nssai =
        &registration_accept->rejected_nssai;
    ogs_nas_5gs_network_feature_support_t *network_feature_support =
        &registration_accept->network_feature_support;
    ogs_nas_pdu_session_status_t *pdu_session_status =
        &registration_accept->pdu_session_status;
    ogs_nas_pdu_session_reactivation_result_t *pdu_session_reactivation_result =
        &registration_accept->pdu_session_reactivation_result;
    ogs_nas_gprs_timer_3_t *t3512_value = &registration_accept->t3512_value;
    ogs_nas_gprs_timer_2_t *t3502_value = &registration_accept->t3502_value;

    ogs_assert(amf_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;

    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_REGISTRATION_ACCEPT;

    /* Registration Result */
    registration_result->length = 1;
    registration_result->value = amf_ue->nas.access_type;

    /* Set GUTI */
    if (amf_ue->next.m_tmsi) {
        registration_accept->presencemask |=
            OGS_NAS_5GS_REGISTRATION_ACCEPT_5G_GUTI_PRESENT;

        ogs_debug("[%s]    5G-S_GUTI[AMF_ID:0x%x,M_TMSI:0x%x]", amf_ue->supi,
                ogs_amf_id_hexdump(&amf_ue->next.guti.amf_id),
                amf_ue->next.guti.m_tmsi);

        ogs_nas_5gs_nas_guti_to_mobility_identity_guti(
                &amf_ue->next.guti, &mobile_identity_guti);

        mobile_identity->length = sizeof(mobile_identity_guti);
        mobile_identity->buffer = &mobile_identity_guti;
    }

    /* Set TAI List */
    registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_TAI_LIST_PRESENT;

    ogs_debug("[%s]    TAI[PLMN_ID:%06x,TAC:%d]", amf_ue->supi,
            ogs_plmn_id_hexdump(&amf_ue->nr_tai.plmn_id), amf_ue->nr_tai.tac.v);
    ogs_debug("[%s]    NR_CGI[PLMN_ID:%06x,CELL_ID:0x%llx]", amf_ue->supi,
            ogs_plmn_id_hexdump(&amf_ue->nr_cgi.plmn_id),
            (long long)amf_ue->nr_cgi.cell_id);

    served_tai_index = amf_find_served_tai(&amf_ue->nr_tai);
    ogs_debug("[%s]    SERVED_TAI_INDEX[%d]", amf_ue->supi, served_tai_index);
    ogs_assert(served_tai_index >= 0 &&
            served_tai_index < OGS_MAX_NUM_OF_SUPPORTED_TA);

    ogs_assert(OGS_OK ==
        ogs_nas_5gs_tai_list_build(&registration_accept->tai_list,
            &amf_self()->served_tai[served_tai_index].list0,
            &amf_self()->served_tai[served_tai_index].list1,
            &amf_self()->served_tai[served_tai_index].list2));

    /* Set Allowed NSSAI */
    ogs_assert(amf_ue->allowed_nssai.num_of_s_nssai);

    ogs_nas_build_nssai(allowed_nssai,
            amf_ue->allowed_nssai.s_nssai,
            amf_ue->allowed_nssai.num_of_s_nssai);

    registration_accept->presencemask |=
        OGS_NAS_5GS_REGISTRATION_ACCEPT_ALLOWED_NSSAI_PRESENT;

    if (amf_ue->rejected_nssai.num_of_s_nssai) {
        ogs_nas_build_rejected_nssai(rejected_nssai,
                amf_ue->rejected_nssai.s_nssai,
                amf_ue->rejected_nssai.num_of_s_nssai);
        registration_accept->presencemask |=
            OGS_NAS_5GS_REGISTRATION_ACCEPT_REJECTED_NSSAI_PRESENT;
    }

    /* 5GS network feature support */
    registration_accept->presencemask |=
        OGS_NAS_5GS_REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_PRESENT;
    network_feature_support->length = 2;
    network_feature_support->
        ims_voice_over_ps_session_over_3gpp_access_indicator = 1;

    /* Set T3512 : Mandatory in Open5GS */
    ogs_assert(amf_self()->time.t3512.value);
    rv = ogs_nas_gprs_timer_3_from_sec(
            &t3512_value->t, amf_self()->time.t3512.value);
    ogs_assert(rv == OGS_OK);
    registration_accept->presencemask |=
        OGS_NAS_5GS_REGISTRATION_ACCEPT_T3512_VALUE_PRESENT;
    t3512_value->length = 1;

    /* Set T3502 */
    if (amf_self()->time.t3502.value) {
        rv = ogs_nas_gprs_timer_from_sec(
                &t3502_value->t, amf_self()->time.t3502.value);
        ogs_assert(rv == OGS_OK);
        registration_accept->presencemask |=
            OGS_NAS_5GS_REGISTRATION_ACCEPT_T3502_VALUE_PRESENT;
        t3502_value->length = 1;
    }

    if (amf_ue->nas.present.pdu_session_status) {
        registration_accept->presencemask |=
            OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_STATUS_PRESENT;
        pdu_session_status->length = 2;
        pdu_session_status->psi = get_pdu_session_status(amf_ue);
        ogs_debug("[%s]    PDU Session Status : %04x",
                amf_ue->supi, pdu_session_status->psi);
    }

    if (amf_ue->nas.present.uplink_data_status) {
        registration_accept->presencemask |=
            OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT;
        pdu_session_reactivation_result->length = 2;
        pdu_session_reactivation_result->psi =
            get_pdu_session_reactivation_result(amf_ue);
        ogs_debug("[%s]    PDU Session Reactivation Result : %04x",
                amf_ue->supi, pdu_session_reactivation_result->psi);
    }

    pkbuf = nas_5gs_security_encode(amf_ue, &message);

    return pkbuf;
}

ogs_pkbuf_t *gmm_build_registration_reject(
        amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t gmm_cause)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_registration_reject_t *registration_reject =
        &message.gmm.registration_reject;
    ogs_nas_rejected_nssai_t *rejected_nssai =
        &registration_reject->rejected_nssai;

    ogs_assert(amf_ue);

    memset(&message, 0, sizeof(message));
    message.gmm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_REGISTRATION_REJECT;

    registration_reject->gmm_cause = gmm_cause;

    if (amf_ue->rejected_nssai.num_of_s_nssai) {
        ogs_nas_build_rejected_nssai(rejected_nssai,
                amf_ue->rejected_nssai.s_nssai,
                amf_ue->rejected_nssai.num_of_s_nssai);
        registration_reject->presencemask |=
            OGS_NAS_5GS_REGISTRATION_REJECT_REJECTED_NSSAI_PRESENT;
    }

    return ogs_nas_5gs_plain_encode(&message);
}

ogs_pkbuf_t *gmm_build_service_accept(amf_ue_t *amf_ue)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_service_accept_t *service_accept = &message.gmm.service_accept;
    ogs_nas_pdu_session_status_t *pdu_session_status = NULL;
    ogs_nas_pdu_session_reactivation_result_t *pdu_session_reactivation_result;

    ogs_assert(amf_ue);

    pdu_session_status = &service_accept->pdu_session_status;
    ogs_assert(pdu_session_status);
    pdu_session_reactivation_result = &service_accept->
        pdu_session_reactivation_result;
    ogs_assert(pdu_session_reactivation_result);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;

    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_SERVICE_ACCEPT;

    if (amf_ue->nas.present.pdu_session_status) {
        service_accept->presencemask |=
            OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_STATUS_PRESENT;
        pdu_session_status->length = 2;
        pdu_session_status->psi = get_pdu_session_status(amf_ue);
        ogs_debug("[%s]    PDU Session Status : %04x",
                amf_ue->supi, pdu_session_status->psi);
    }

    if (amf_ue->nas.present.uplink_data_status) {
        service_accept->presencemask |=
            OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT;
        pdu_session_reactivation_result->length = 2;
        pdu_session_reactivation_result->psi =
            get_pdu_session_reactivation_result(amf_ue);
        ogs_debug("[%s]    PDU Session Reactivation Result : %04x",
                amf_ue->supi, pdu_session_reactivation_result->psi);
    }

    return nas_5gs_security_encode(amf_ue, &message);
}

ogs_pkbuf_t *gmm_build_service_reject(
        amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t gmm_cause)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_service_reject_t *service_reject = &message.gmm.service_reject;
    ogs_nas_pdu_session_status_t *pdu_session_status = NULL;

    ogs_assert(amf_ue);

    pdu_session_status = &service_reject->pdu_session_status;

    memset(&message, 0, sizeof(message));
    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_SERVICE_REJECT;

    service_reject->gmm_cause = gmm_cause;

    if (amf_ue->nas.present.pdu_session_status) {
        service_reject->presencemask |=
            OGS_NAS_5GS_SERVICE_REJECT_PDU_SESSION_STATUS_PRESENT;
        pdu_session_status->length = 2;
        pdu_session_status->psi = get_pdu_session_status(amf_ue);
        ogs_debug("[%s]    PDU Session Status : %04x",
                amf_ue->supi, pdu_session_status->psi);
    }

    return ogs_nas_5gs_plain_encode(&message);
}

ogs_pkbuf_t *gmm_build_de_registration_accept(amf_ue_t *amf_ue)
{
    ogs_nas_5gs_message_t message;

    ogs_assert(amf_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;

    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_DEREGISTRATION_ACCEPT_FROM_UE;

    return nas_5gs_security_encode(amf_ue, &message);
}

ogs_pkbuf_t *gmm_build_de_registration_request(
        amf_ue_t *amf_ue,
        OpenAPI_deregistration_reason_e dereg_reason,
        ogs_nas_5gmm_cause_t gmm_cause)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_deregistration_request_to_ue_t *dereg_req =
        &message.gmm.deregistration_request_to_ue;

    ogs_assert(amf_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;

    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE;

    dereg_req->de_registration_type.re_registration_required =
        dereg_reason == OpenAPI_deregistration_reason_REREGISTRATION_REQUIRED;
    dereg_req->de_registration_type.access_type = OGS_ACCESS_TYPE_3GPP;

    if (gmm_cause) {
        dereg_req->presencemask |=
            OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_5GMM_CAUSE_PRESENT;
        dereg_req->gmm_cause = gmm_cause;
    }

    return nas_5gs_security_encode(amf_ue, &message);
}

ogs_pkbuf_t *gmm_build_identity_request(amf_ue_t *amf_ue)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_identity_request_t *identity_request =
        &message.gmm.identity_request;

    ogs_assert(amf_ue);

    memset(&message, 0, sizeof(message));
    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_IDENTITY_REQUEST;

    /* Request IMSI */
    ogs_debug("    Identity Type 2 : SUCI");
    identity_request->identity_type.value = OGS_NAS_5GS_MOBILE_IDENTITY_SUCI;

    return ogs_nas_5gs_plain_encode(&message);
}

ogs_pkbuf_t *gmm_build_authentication_request(amf_ue_t *amf_ue)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_authentication_request_t *authentication_request =
        &message.gmm.authentication_request;

    ogs_assert(amf_ue);

    memset(&message, 0, sizeof(message));
    message.gmm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_AUTHENTICATION_REQUEST;

    authentication_request->ngksi.tsc = amf_ue->nas.amf.tsc;
    authentication_request->ngksi.value = amf_ue->nas.amf.ksi;
    authentication_request->abba.length = amf_ue->abba_len;
    memcpy(authentication_request->abba.value, amf_ue->abba, amf_ue->abba_len);

    authentication_request->presencemask |=
    OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_RAND_PRESENT;
    authentication_request->presencemask |=
    OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_AUTN_PRESENT;

    memcpy(authentication_request->authentication_parameter_rand.rand,
            amf_ue->rand, OGS_RAND_LEN);
    memcpy(authentication_request->authentication_parameter_autn.autn,
            amf_ue->autn, OGS_AUTN_LEN);
    authentication_request->authentication_parameter_autn.length =
            OGS_AUTN_LEN;

    return ogs_nas_5gs_plain_encode(&message);
}

ogs_pkbuf_t *gmm_build_authentication_reject(void)
{
    ogs_nas_5gs_message_t message;

    memset(&message, 0, sizeof(message));

    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_AUTHENTICATION_REJECT;

    return ogs_nas_5gs_plain_encode(&message);
}

ogs_pkbuf_t *gmm_build_security_mode_command(amf_ue_t *amf_ue)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_security_mode_command_t *security_mode_command =
        &message.gmm.security_mode_command;
    ogs_nas_security_algorithms_t *selected_nas_security_algorithms =
        &security_mode_command->selected_nas_security_algorithms;
    ogs_nas_key_set_identifier_t *ngksi = &security_mode_command->ngksi;
    ogs_nas_ue_security_capability_t *replayed_ue_security_capabilities =
        &security_mode_command->replayed_ue_security_capabilities;
    ogs_nas_imeisv_request_t *imeisv_request =
        &security_mode_command->imeisv_request;
    ogs_nas_additional_5g_security_information_t
        *additional_security_information =
            &security_mode_command->additional_security_information;

    ogs_assert(amf_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT;
    message.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;

    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_SECURITY_MODE_COMMAND;

    selected_nas_security_algorithms->type_of_integrity_protection_algorithm =
        amf_ue->selected_int_algorithm;
    selected_nas_security_algorithms->type_of_ciphering_algorithm =
        amf_ue->selected_enc_algorithm;

    ngksi->tsc = amf_ue->nas.amf.tsc;
    ngksi->value = amf_ue->nas.amf.ksi;

    replayed_ue_security_capabilities->nr_ea =
        amf_ue->ue_security_capability.nr_ea;
    replayed_ue_security_capabilities->nr_ia =
        amf_ue->ue_security_capability.nr_ia;
    replayed_ue_security_capabilities->eutra_ea =
        amf_ue->ue_security_capability.eutra_ea;
    replayed_ue_security_capabilities->eutra_ia =
        amf_ue->ue_security_capability.eutra_ia;

    replayed_ue_security_capabilities->length =
        sizeof(replayed_ue_security_capabilities->nr_ea) +
        sizeof(replayed_ue_security_capabilities->nr_ia);
    if (replayed_ue_security_capabilities->eutra_ea ||
        replayed_ue_security_capabilities->eutra_ia)
        replayed_ue_security_capabilities->length =
            sizeof(replayed_ue_security_capabilities->nr_ea) +
            sizeof(replayed_ue_security_capabilities->nr_ia) +
            sizeof(replayed_ue_security_capabilities->eutra_ea) +
            sizeof(replayed_ue_security_capabilities->eutra_ia);
    ogs_debug("    Replayed UE SEC[LEN:%d NEA:0x%x NIA:0x%x EEA:0x%x EIA:0x%x",
            replayed_ue_security_capabilities->length,
            replayed_ue_security_capabilities->nr_ea,
            replayed_ue_security_capabilities->nr_ia,
            replayed_ue_security_capabilities->eutra_ea,
            replayed_ue_security_capabilities->eutra_ia);
    ogs_debug("    Selected[Integrity:0x%x Encrypt:0x%x]",
            amf_ue->selected_int_algorithm, amf_ue->selected_enc_algorithm);

    security_mode_command->presencemask |=
        OGS_NAS_5GS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT;
    imeisv_request->type = OGS_NAS_IMEISV_TYPE;
    imeisv_request->value = OGS_NAS_IMEISV_REQUESTED;

    security_mode_command->presencemask |= OGS_NAS_5GS_SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_PRESENT;
    additional_security_information->length = 1;
    additional_security_information->
        retransmission_of_initial_nas_message_request = 1;

    ogs_assert(amf_ue->selected_int_algorithm !=
            OGS_NAS_SECURITY_ALGORITHMS_EIA0);

    ogs_kdf_nas_5gs(OGS_KDF_NAS_INT_ALG, amf_ue->selected_int_algorithm,
            amf_ue->kamf, amf_ue->knas_int);
    ogs_kdf_nas_5gs(OGS_KDF_NAS_ENC_ALG, amf_ue->selected_enc_algorithm,
            amf_ue->kamf, amf_ue->knas_enc);

    return nas_5gs_security_encode(amf_ue, &message);
}

ogs_pkbuf_t *gmm_build_configuration_update_command(
        amf_ue_t *amf_ue, gmm_configuration_update_command_param_t *param)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_configuration_update_command_t *configuration_update_command =
        &message.gmm.configuration_update_command;

    ogs_nas_time_zone_t *local_time_zone =
        &configuration_update_command->local_time_zone;
    ogs_nas_time_zone_and_time_t *universal_time_and_local_time_zone =
        &configuration_update_command->universal_time_and_local_time_zone;
    ogs_nas_daylight_saving_time_t *network_daylight_saving_time =
        &configuration_update_command->network_daylight_saving_time;
    ogs_nas_configuration_update_indication_t
        *configuration_update_indication =
            &configuration_update_command->configuration_update_indication;
    ogs_nas_5gs_mobile_identity_t *mobile_identity =
        &configuration_update_command->guti;
    ogs_nas_5gs_mobile_identity_guti_t mobile_identity_guti;

    struct timeval tv;
    struct tm gmt, local;

    ogs_assert(amf_ue);
    ogs_assert(param);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;

    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND;

    if (param->registration_requested || param->acknowledgement_requested) {
        configuration_update_command->presencemask |=
            OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CONFIGURATION_UPDATE_INDICATION_PRESENT;

        configuration_update_indication->acknowledgement_requested =
            param->acknowledgement_requested;
        configuration_update_indication->registration_requested =
            param->registration_requested;
    }

    if (param->nitz) {
        if (amf_self()->full_name.length) {
            configuration_update_command->presencemask |=
                OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_FULL_NAME_FOR_NETWORK_PRESENT;
            memcpy(&configuration_update_command->full_name_for_network,
                &amf_self()->full_name, sizeof(ogs_nas_network_name_t));
        }

        if (amf_self()->short_name.length) {
            configuration_update_command->presencemask |=
                OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SHORT_NAME_FOR_NETWORK_PRESENT;
            memcpy(&configuration_update_command->short_name_for_network,
                &amf_self()->short_name, sizeof(ogs_nas_network_name_t));
        }

        if (!ogs_global_conf()->parameter.no_time_zone_information) {
            ogs_gettimeofday(&tv);
            ogs_gmtime(tv.tv_sec, &gmt);
            ogs_localtime(tv.tv_sec, &local);

            ogs_info("    UTC [%04d-%02d-%02dT%02d:%02d:%02d] "
                    "Timezone[%d]/DST[%d]",
                gmt.tm_year+1900, gmt.tm_mon+1, gmt.tm_mday,
                gmt.tm_hour, gmt.tm_min, gmt.tm_sec,
                (int)gmt.tm_gmtoff, gmt.tm_isdst);
            ogs_info("    LOCAL [%04d-%02d-%02dT%02d:%02d:%02d] "
                    "Timezone[%d]/DST[%d]",
                local.tm_year+1900, local.tm_mon+1, local.tm_mday,
                local.tm_hour, local.tm_min, local.tm_sec,
                (int)local.tm_gmtoff, local.tm_isdst);

            configuration_update_command->presencemask |=
                OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LOCAL_TIME_ZONE_PRESENT;
            if (local.tm_gmtoff >= 0) {
                *local_time_zone = OGS_NAS_TIME_TO_BCD(local.tm_gmtoff / 900);
            } else {
                *local_time_zone = OGS_NAS_TIME_TO_BCD((-local.tm_gmtoff) / 900);
                *local_time_zone |= 0x08;
            }
            ogs_debug("    Timezone:0x%x", *local_time_zone);

            configuration_update_command->presencemask |=
                OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT;
            universal_time_and_local_time_zone->year =
                        OGS_NAS_TIME_TO_BCD(gmt.tm_year % 100);
            universal_time_and_local_time_zone->mon =
                        OGS_NAS_TIME_TO_BCD(gmt.tm_mon+1);
            universal_time_and_local_time_zone->mday =
                        OGS_NAS_TIME_TO_BCD(gmt.tm_mday);
            universal_time_and_local_time_zone->hour =
                        OGS_NAS_TIME_TO_BCD(gmt.tm_hour);
            universal_time_and_local_time_zone->min =
                        OGS_NAS_TIME_TO_BCD(gmt.tm_min);
            universal_time_and_local_time_zone->sec =
                        OGS_NAS_TIME_TO_BCD(gmt.tm_sec);
            universal_time_and_local_time_zone->timezone = *local_time_zone;

            configuration_update_command->presencemask |=
                OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT;
            network_daylight_saving_time->length = 1;
            if (local.tm_isdst > 0) {
                network_daylight_saving_time->value = 1;
            }
        }
    }

    if (param->guti) {
        configuration_update_command->presencemask |=
            OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_5G_GUTI_PRESENT;

        ogs_assert(amf_ue->next.m_tmsi);
        ogs_info("[%s]    5G-S_GUTI[AMF_ID:0x%x,M_TMSI:0x%x]", amf_ue->supi,
                ogs_amf_id_hexdump(&amf_ue->next.guti.amf_id),
                amf_ue->next.guti.m_tmsi);

        ogs_nas_5gs_nas_guti_to_mobility_identity_guti(
                &amf_ue->next.guti, &mobile_identity_guti);

        mobile_identity->length = sizeof(mobile_identity_guti);
        mobile_identity->buffer = &mobile_identity_guti;
    }

    return nas_5gs_security_encode(amf_ue, &message);
}

ogs_pkbuf_t *gmm_build_dl_nas_transport(amf_sess_t *sess,
        uint8_t payload_container_type, ogs_pkbuf_t *payload_container,
        ogs_nas_5gmm_cause_t cause, uint8_t backoff_time)
{
    amf_ue_t *amf_ue = NULL;
    ogs_pkbuf_t *gmmbuf = NULL;

    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_dl_nas_transport_t *dl_nas_transport =
        &message.gmm.dl_nas_transport;

    ogs_nas_pdu_session_identity_2_t *pdu_session_id = NULL;
    ogs_nas_5gmm_cause_t *gmm_cause = NULL;
    ogs_nas_gprs_timer_3_t *back_off_timer_value = NULL;

    ogs_assert(sess);
    amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
    ogs_assert(amf_ue);
    ogs_assert(payload_container_type);
    ogs_assert(payload_container);

    pdu_session_id = &dl_nas_transport->pdu_session_id;
    ogs_assert(pdu_session_id);
    gmm_cause = &dl_nas_transport->gmm_cause;
    ogs_assert(gmm_cause);
    back_off_timer_value = &dl_nas_transport->back_off_timer_value;
    ogs_assert(back_off_timer_value);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;

    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_DL_NAS_TRANSPORT;

    dl_nas_transport->payload_container_type.value = payload_container_type;
    dl_nas_transport->payload_container.length = payload_container->len;
    dl_nas_transport->payload_container.buffer = payload_container->data;

    dl_nas_transport->presencemask |=
        OGS_NAS_5GS_DL_NAS_TRANSPORT_PDU_SESSION_ID_PRESENT;
    *pdu_session_id = sess->psi;

    if (cause) {
        dl_nas_transport->presencemask |=
            OGS_NAS_5GS_DL_NAS_TRANSPORT_5GMM_CAUSE_PRESENT;
        *gmm_cause = cause;
    }

    if (backoff_time >= 2) {
        dl_nas_transport->presencemask |=
            OGS_NAS_5GS_DL_NAS_TRANSPORT_BACK_OFF_TIMER_VALUE_PRESENT;
        back_off_timer_value->length = 1;
        back_off_timer_value->t.unit =
            OGS_NAS_GPRS_TIMER_3_UNIT_MULTIPLES_OF_2_SS;
        back_off_timer_value->t.value = backoff_time / 2;
    }

    gmmbuf = nas_5gs_security_encode(amf_ue, &message);
    ogs_pkbuf_free(payload_container);

    return gmmbuf;
}

ogs_pkbuf_t *gmm_build_status(amf_ue_t *amf_ue, ogs_nas_5gmm_cause_t cause)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_5gmm_status_t *gmm_status = &message.gmm.gmm_status;
    ogs_nas_5gmm_cause_t *gmm_cause = &gmm_status->gmm_cause;

    ogs_assert(amf_ue);
    ogs_assert(cause);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type =
        OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;

    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_5GMM_STATUS;

    *gmm_cause = cause;

    return nas_5gs_security_encode(amf_ue, &message);
}

static uint16_t get_pdu_session_status(amf_ue_t *amf_ue)
{
    amf_sess_t *sess = NULL;

    uint16_t psimask = 0;
    uint16_t status = 0;

    ogs_assert(amf_ue);

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        psimask |= (1 << sess->psi);
    }

    status |= (psimask << 8);
    status |= (psimask >> 8);

    return status;
}

static uint16_t get_pdu_session_reactivation_result(amf_ue_t *amf_ue)
{
    amf_sess_t *sess = NULL;

    uint16_t psimask = 0;
    uint16_t status = 0;

    ogs_assert(amf_ue);

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        if (!SESSION_CONTEXT_IN_SMF(sess))
            psimask |= (1 << sess->psi);
    }

    status |= (psimask << 8);
    status |= (psimask >> 8);

    return status;
}
