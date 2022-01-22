/*
 * The MIT License
 *
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*******************************************************************************
 * This file had been created by nas-message.py script v0.2.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2022-01-22 09:24:09.053958 by acetcom
 * from 24501-g41.docx
 ******************************************************************************/

#include "ogs-nas-5gs.h"

int ogs_nas_5gs_encode_registration_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_registration_accept(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_registration_complete(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_registration_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_deregistration_request_from_ue(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_deregistration_request_to_ue(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_service_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_service_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_service_accept(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_configuration_update_command(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_authentication_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_authentication_response(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_authentication_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_authentication_failure(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_authentication_result(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_identity_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_identity_response(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_security_mode_command(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_security_mode_complete(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_security_mode_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_5gmm_status(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_notification(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_notification_response(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_ul_nas_transport(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_dl_nas_transport(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_establishment_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_establishment_accept(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_establishment_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_authentication_command(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_authentication_complete(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_authentication_result(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_modification_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_modification_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_modification_command(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_modification_complete(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_modification_command_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_release_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_release_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_release_command(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_pdu_session_release_complete(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_5gsm_status(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message);
int ogs_nas_5gs_encode_registration_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_registration_request_t *registration_request = &message->gmm.registration_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode REGISTRATION_REQUEST");

    size = ogs_nas_5gs_encode_5gs_registration_type(pkbuf, &registration_request->registration_type);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_5gs_encode_5gs_mobile_identity(pkbuf, &registration_request->mobile_identity);
    ogs_assert(size >= 0);
    encoded += size;

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_PRESENT) {
        registration_request->non_current_native_nas_key_set_identifier.type = (OGS_NAS_5GS_REGISTRATION_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_TYPE >> 4);

        size = ogs_nas_5gs_encode_key_set_identifier(pkbuf, &registration_request->non_current_native_nas_key_set_identifier);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_5GMM_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gmm_capability(pkbuf, &registration_request->gmm_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_ue_security_capability(pkbuf, &registration_request->ue_security_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_NSSAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_NSSAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_nssai(pkbuf, &registration_request->requested_nssai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gs_tracking_area_identity(pkbuf, &registration_request->last_visited_registered_tai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_s1_ue_network_capability(pkbuf, &registration_request->s1_ue_network_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_UPLINK_DATA_STATUS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_UPLINK_DATA_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_uplink_data_status(pkbuf, &registration_request->uplink_data_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_PDU_SESSION_STATUS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_PDU_SESSION_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_session_status(pkbuf, &registration_request->pdu_session_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_MICO_INDICATION_PRESENT) {
        registration_request->mico_indication.type = (OGS_NAS_5GS_REGISTRATION_REQUEST_MICO_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_mico_indication(pkbuf, &registration_request->mico_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_UE_STATUS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_UE_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_ue_status(pkbuf, &registration_request->ue_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_ADDITIONAL_GUTI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_ADDITIONAL_GUTI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gs_mobile_identity(pkbuf, &registration_request->additional_guti);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_allowed_pdu_session_status(pkbuf, &registration_request->allowed_pdu_session_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_UE_USAGE_SETTING_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_UE_USAGE_SETTING_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_ue_usage_setting(pkbuf, &registration_request->ue_usage_setting);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_DRX_PARAMETERS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_DRX_PARAMETERS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gs_drx_parameters(pkbuf, &registration_request->requested_drx_parameters);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eps_nas_message_container(pkbuf, &registration_request->eps_nas_message_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_LADN_INDICATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_LADN_INDICATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_ladn_indication(pkbuf, &registration_request->ladn_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_PRESENT) {
        registration_request->payload_container_type.type = (OGS_NAS_5GS_REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_TYPE >> 4);

        size = ogs_nas_5gs_encode_payload_container_type(pkbuf, &registration_request->payload_container_type);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_PAYLOAD_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_payload_container(pkbuf, &registration_request->payload_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_PRESENT) {
        registration_request->network_slicing_indication.type = (OGS_NAS_5GS_REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_network_slicing_indication(pkbuf, &registration_request->network_slicing_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_5GS_UPDATE_TYPE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_5GS_UPDATE_TYPE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gs_update_type(pkbuf, &registration_request->update_type);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_mobile_station_classmark_2(pkbuf, &registration_request->mobile_station_classmark_2);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_SUPPORTED_CODECS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_SUPPORTED_CODECS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_supported_codec_list(pkbuf, &registration_request->supported_codecs);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_message_container(pkbuf, &registration_request->nas_message_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eps_bearer_context_status(pkbuf, &registration_request->eps_bearer_context_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_EXTENDED_DRX_PARAMETERS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_EXTENDED_DRX_PARAMETERS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_drx_parameters(pkbuf, &registration_request->requested_extended_drx_parameters);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_T3324_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_T3324_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_3(pkbuf, &registration_request->t3324_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_UE_RADIO_CAPABILITY_ID_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_UE_RADIO_CAPABILITY_ID_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_ue_radio_capability_id(pkbuf, &registration_request->ue_radio_capability_id);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_MAPPED_NSSAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_MAPPED_NSSAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_mapped_nssai(pkbuf, &registration_request->requested_mapped_nssai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_additional_information_requested(pkbuf, &registration_request->additional_information_requested);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_WUS_ASSISTANCE_INFORMATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_WUS_ASSISTANCE_INFORMATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_wus_assistance_information(pkbuf, &registration_request->requested_wus_assistance_information);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_request->presencemask & OGS_NAS_5GS_REGISTRATION_REQUEST_N5GC_INDICATION_PRESENT) {
        registration_request->n5gc_indication.type = OGS_NAS_5GS_REGISTRATION_REQUEST_N5GC_INDICATION_TYPE;

        size = ogs_nas_5gs_encode_n5gc_indication(pkbuf, &registration_request->n5gc_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_registration_accept(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_registration_accept_t *registration_accept = &message->gmm.registration_accept;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode REGISTRATION_ACCEPT");

    size = ogs_nas_5gs_encode_5gs_registration_result(pkbuf, &registration_accept->registration_result);
    ogs_assert(size >= 0);
    encoded += size;

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_5G_GUTI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_5G_GUTI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gs_mobile_identity(pkbuf, &registration_accept->guti);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_EQUIVALENT_PLMNS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_EQUIVALENT_PLMNS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_plmn_list(pkbuf, &registration_accept->equivalent_plmns);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_TAI_LIST_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_TAI_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gs_tracking_area_identity_list(pkbuf, &registration_accept->tai_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_ALLOWED_NSSAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_ALLOWED_NSSAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_nssai(pkbuf, &registration_accept->allowed_nssai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_REJECTED_NSSAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_REJECTED_NSSAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_rejected_nssai(pkbuf, &registration_accept->rejected_nssai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_CONFIGURED_NSSAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_CONFIGURED_NSSAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_nssai(pkbuf, &registration_accept->configured_nssai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gs_network_feature_support(pkbuf, &registration_accept->network_feature_support);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_STATUS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_session_status(pkbuf, &registration_accept->pdu_session_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_session_reactivation_result(pkbuf, &registration_accept->pdu_session_reactivation_result);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_session_reactivation_result_error_cause(pkbuf, &registration_accept->pdu_session_reactivation_result_error_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_LADN_INFORMATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_LADN_INFORMATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_ladn_information(pkbuf, &registration_accept->ladn_information);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_MICO_INDICATION_PRESENT) {
        registration_accept->mico_indication.type = (OGS_NAS_5GS_REGISTRATION_ACCEPT_MICO_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_mico_indication(pkbuf, &registration_accept->mico_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_PRESENT) {
        registration_accept->network_slicing_indication.type = (OGS_NAS_5GS_REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_network_slicing_indication(pkbuf, &registration_accept->network_slicing_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_SERVICE_AREA_LIST_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_SERVICE_AREA_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_service_area_list(pkbuf, &registration_accept->service_area_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_T3512_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_T3512_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_3(pkbuf, &registration_accept->t3512_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_NON_3GPP_DE_REGISTRATION_TIMER_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_NON_3GPP_DE_REGISTRATION_TIMER_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_2(pkbuf, &registration_accept->non_3gpp_de_registration_timer_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_T3502_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_T3502_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_2(pkbuf, &registration_accept->t3502_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_emergency_number_list(pkbuf, &registration_accept->emergency_number_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_emergency_number_list(pkbuf, &registration_accept->extended_emergency_number_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_SOR_TRANSPARENT_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_SOR_TRANSPARENT_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_sor_transparent_container(pkbuf, &registration_accept->sor_transparent_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &registration_accept->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_PRESENT) {
        registration_accept->nssai_inclusion_mode.type = (OGS_NAS_5GS_REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_TYPE >> 4);

        size = ogs_nas_5gs_encode_nssai_inclusion_mode(pkbuf, &registration_accept->nssai_inclusion_mode);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_operator_defined_access_category_definitions(pkbuf, &registration_accept->operator_defined_access_category_definitions);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_DRX_PARAMETERS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_DRX_PARAMETERS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gs_drx_parameters(pkbuf, &registration_accept->negotiated_drx_parameters);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_NON_3GPP_NW_POLICIES_PRESENT) {
        registration_accept->non_3gpp_nw_policies.type = (OGS_NAS_5GS_REGISTRATION_ACCEPT_NON_3GPP_NW_POLICIES_TYPE >> 4);

        size = ogs_nas_5gs_encode_non_3gpp_nw_provided_policies(pkbuf, &registration_accept->non_3gpp_nw_policies);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_EPS_BEARER_CONTEXT_STATUS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_EPS_BEARER_CONTEXT_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eps_bearer_context_status(pkbuf, &registration_accept->eps_bearer_context_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_EXTENDED_DRX_PARAMETERS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_EXTENDED_DRX_PARAMETERS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_drx_parameters(pkbuf, &registration_accept->negotiated_extended_drx_parameters);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_T3447_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_T3447_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_3(pkbuf, &registration_accept->t3447_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_T3448_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_T3448_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_3(pkbuf, &registration_accept->t3448_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_T3324_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_T3324_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_3(pkbuf, &registration_accept->t3324_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_UE_RADIO_CAPABILITY_ID_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_UE_RADIO_CAPABILITY_ID_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_ue_radio_capability_id(pkbuf, &registration_accept->ue_radio_capability_id);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT) {
        registration_accept->ue_radio_capability_id_deletion_indication.type = (OGS_NAS_5GS_REGISTRATION_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_ue_radio_capability_id_deletion_indication(pkbuf, &registration_accept->ue_radio_capability_id_deletion_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_PENDING_NSSAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_PENDING_NSSAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_nssai(pkbuf, &registration_accept->pending_nssai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_CIPHERING_KEY_DATA_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_CIPHERING_KEY_DATA_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_ciphering_key_data(pkbuf, &registration_accept->ciphering_key_data);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_CAG_INFORMATION_LIST_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_CAG_INFORMATION_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_cag_information_list(pkbuf, &registration_accept->cag_information_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_TRUNCATED_5G_S_TMSI_CONFIGURATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_TRUNCATED_5G_S_TMSI_CONFIGURATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_truncated_5g_s_tmsi_configuration(pkbuf, &registration_accept->truncated_s_tmsi_configuration);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_accept->presencemask & OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_WUS_ASSISTANCE_INFORMATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_WUS_ASSISTANCE_INFORMATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_wus_assistance_information(pkbuf, &registration_accept->negotiated_wus_assistance_information);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_registration_complete(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_registration_complete_t *registration_complete = &message->gmm.registration_complete;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode REGISTRATION_COMPLETE");

    if (registration_complete->presencemask & OGS_NAS_5GS_REGISTRATION_COMPLETE_SOR_TRANSPARENT_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_COMPLETE_SOR_TRANSPARENT_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_sor_transparent_container(pkbuf, &registration_complete->sor_transparent_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_registration_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_registration_reject_t *registration_reject = &message->gmm.registration_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode REGISTRATION_REJECT");

    size = ogs_nas_5gs_encode_5gmm_cause(pkbuf, &registration_reject->gmm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (registration_reject->presencemask & OGS_NAS_5GS_REGISTRATION_REJECT_T3346_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REJECT_T3346_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_2(pkbuf, &registration_reject->t3346_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_reject->presencemask & OGS_NAS_5GS_REGISTRATION_REJECT_T3502_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REJECT_T3502_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_2(pkbuf, &registration_reject->t3502_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_reject->presencemask & OGS_NAS_5GS_REGISTRATION_REJECT_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REJECT_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &registration_reject->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (registration_reject->presencemask & OGS_NAS_5GS_REGISTRATION_REJECT_REJECTED_NSSAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_REGISTRATION_REJECT_REJECTED_NSSAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_rejected_nssai(pkbuf, &registration_reject->rejected_nssai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_deregistration_request_from_ue(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_deregistration_request_from_ue_t *deregistration_request_from_ue = &message->gmm.deregistration_request_from_ue;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode DEREGISTRATION_REQUEST");

    size = ogs_nas_5gs_encode_de_registration_type(pkbuf, &deregistration_request_from_ue->de_registration_type);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_5gs_encode_5gs_mobile_identity(pkbuf, &deregistration_request_from_ue->mobile_identity);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_5gs_encode_deregistration_request_to_ue(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_deregistration_request_to_ue_t *deregistration_request_to_ue = &message->gmm.deregistration_request_to_ue;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode DEREGISTRATION_REQUEST");

    size = ogs_nas_5gs_encode_de_registration_type(pkbuf, &deregistration_request_to_ue->de_registration_type);
    ogs_assert(size >= 0);
    encoded += size;

    if (deregistration_request_to_ue->presencemask & OGS_NAS_5GS_DEREGISTRATION_REQUEST_5GMM_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_DEREGISTRATION_REQUEST_5GMM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gmm_cause(pkbuf, &deregistration_request_to_ue->gmm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (deregistration_request_to_ue->presencemask & OGS_NAS_5GS_DEREGISTRATION_REQUEST_T3346_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_DEREGISTRATION_REQUEST_T3346_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_2(pkbuf, &deregistration_request_to_ue->t3346_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (deregistration_request_to_ue->presencemask & OGS_NAS_5GS_DEREGISTRATION_REQUEST_REJECTED_NSSAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_DEREGISTRATION_REQUEST_REJECTED_NSSAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_rejected_nssai(pkbuf, &deregistration_request_to_ue->rejected_nssai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_service_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_service_request_t *service_request = &message->gmm.service_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode SERVICE_REQUEST");

    size = ogs_nas_5gs_encode_key_set_identifier(pkbuf, &service_request->ngksi);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_5gs_encode_5gs_mobile_identity(pkbuf, &service_request->s_tmsi);
    ogs_assert(size >= 0);
    encoded += size;

    if (service_request->presencemask & OGS_NAS_5GS_SERVICE_REQUEST_UPLINK_DATA_STATUS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SERVICE_REQUEST_UPLINK_DATA_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_uplink_data_status(pkbuf, &service_request->uplink_data_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (service_request->presencemask & OGS_NAS_5GS_SERVICE_REQUEST_PDU_SESSION_STATUS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SERVICE_REQUEST_PDU_SESSION_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_session_status(pkbuf, &service_request->pdu_session_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (service_request->presencemask & OGS_NAS_5GS_SERVICE_REQUEST_ALLOWED_PDU_SESSION_STATUS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SERVICE_REQUEST_ALLOWED_PDU_SESSION_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_allowed_pdu_session_status(pkbuf, &service_request->allowed_pdu_session_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (service_request->presencemask & OGS_NAS_5GS_SERVICE_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SERVICE_REQUEST_NAS_MESSAGE_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_message_container(pkbuf, &service_request->nas_message_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_service_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_service_reject_t *service_reject = &message->gmm.service_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode SERVICE_REJECT");

    size = ogs_nas_5gs_encode_5gmm_cause(pkbuf, &service_reject->gmm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (service_reject->presencemask & OGS_NAS_5GS_SERVICE_REJECT_PDU_SESSION_STATUS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SERVICE_REJECT_PDU_SESSION_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_session_status(pkbuf, &service_reject->pdu_session_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (service_reject->presencemask & OGS_NAS_5GS_SERVICE_REJECT_T3346_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SERVICE_REJECT_T3346_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_2(pkbuf, &service_reject->t3346_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (service_reject->presencemask & OGS_NAS_5GS_SERVICE_REJECT_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SERVICE_REJECT_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &service_reject->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (service_reject->presencemask & OGS_NAS_5GS_SERVICE_REJECT_T3448_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SERVICE_REJECT_T3448_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_3(pkbuf, &service_reject->t3448_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_service_accept(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_service_accept_t *service_accept = &message->gmm.service_accept;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode SERVICE_ACCEPT");

    if (service_accept->presencemask & OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_STATUS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_session_status(pkbuf, &service_accept->pdu_session_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (service_accept->presencemask & OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_session_reactivation_result(pkbuf, &service_accept->pdu_session_reactivation_result);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (service_accept->presencemask & OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_session_reactivation_result_error_cause(pkbuf, &service_accept->pdu_session_reactivation_result_error_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (service_accept->presencemask & OGS_NAS_5GS_SERVICE_ACCEPT_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SERVICE_ACCEPT_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &service_accept->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (service_accept->presencemask & OGS_NAS_5GS_SERVICE_ACCEPT_T3448_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SERVICE_ACCEPT_T3448_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_3(pkbuf, &service_accept->t3448_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_configuration_update_command(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_configuration_update_command_t *configuration_update_command = &message->gmm.configuration_update_command;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode CONFIGURATION_UPDATE_COMMAND");

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CONFIGURATION_UPDATE_INDICATION_PRESENT) {
        configuration_update_command->configuration_update_indication.type = (OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CONFIGURATION_UPDATE_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_configuration_update_indication(pkbuf, &configuration_update_command->configuration_update_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_5G_GUTI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_5G_GUTI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gs_mobile_identity(pkbuf, &configuration_update_command->guti);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_TAI_LIST_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_TAI_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gs_tracking_area_identity_list(pkbuf, &configuration_update_command->tai_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_ALLOWED_NSSAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_ALLOWED_NSSAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_nssai(pkbuf, &configuration_update_command->allowed_nssai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SERVICE_AREA_LIST_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SERVICE_AREA_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_service_area_list(pkbuf, &configuration_update_command->service_area_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_FULL_NAME_FOR_NETWORK_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_FULL_NAME_FOR_NETWORK_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_network_name(pkbuf, &configuration_update_command->full_name_for_network);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SHORT_NAME_FOR_NETWORK_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SHORT_NAME_FOR_NETWORK_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_network_name(pkbuf, &configuration_update_command->short_name_for_network);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LOCAL_TIME_ZONE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LOCAL_TIME_ZONE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_time_zone(pkbuf, &configuration_update_command->local_time_zone);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_time_zone_and_time(pkbuf, &configuration_update_command->universal_time_and_local_time_zone);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NETWORK_DAYLIGHT_SAVING_TIME_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_daylight_saving_time(pkbuf, &configuration_update_command->network_daylight_saving_time);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LADN_INFORMATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LADN_INFORMATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_ladn_information(pkbuf, &configuration_update_command->ladn_information);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_MICO_INDICATION_PRESENT) {
        configuration_update_command->mico_indication.type = (OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_MICO_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_mico_indication(pkbuf, &configuration_update_command->mico_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NETWORK_SLICING_INDICATION_PRESENT) {
        configuration_update_command->network_slicing_indication.type = (OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NETWORK_SLICING_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_network_slicing_indication(pkbuf, &configuration_update_command->network_slicing_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CONFIGURED_NSSAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CONFIGURED_NSSAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_nssai(pkbuf, &configuration_update_command->configured_nssai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_REJECTED_NSSAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_REJECTED_NSSAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_rejected_nssai(pkbuf, &configuration_update_command->rejected_nssai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_operator_defined_access_category_definitions(pkbuf, &configuration_update_command->operator_defined_access_category_definitions);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SMS_INDICATION_PRESENT) {
        configuration_update_command->sms_indication.type = (OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SMS_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_sms_indication(pkbuf, &configuration_update_command->sms_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_T3447_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_T3447_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_3(pkbuf, &configuration_update_command->t3447_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CAG_INFORMATION_LIST_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CAG_INFORMATION_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_cag_information_list(pkbuf, &configuration_update_command->cag_information_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UE_RADIO_CAPABILITY_ID_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UE_RADIO_CAPABILITY_ID_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_ue_radio_capability_id(pkbuf, &configuration_update_command->ue_radio_capability_id);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT) {
        configuration_update_command->ue_radio_capability_id_deletion_indication.type = (OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_ue_radio_capability_id_deletion_indication(pkbuf, &configuration_update_command->ue_radio_capability_id_deletion_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_5GS_REGISTRATION_RESULT_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_5GS_REGISTRATION_RESULT_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gs_registration_result(pkbuf, &configuration_update_command->registration_result);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (configuration_update_command->presencemask & OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_TRUNCATED_5G_S_TMSI_CONFIGURATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_TRUNCATED_5G_S_TMSI_CONFIGURATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_truncated_5g_s_tmsi_configuration(pkbuf, &configuration_update_command->truncated_s_tmsi_configuration);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_authentication_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_authentication_request_t *authentication_request = &message->gmm.authentication_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode AUTHENTICATION_REQUEST");

    size = ogs_nas_5gs_encode_key_set_identifier(pkbuf, &authentication_request->ngksi);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_5gs_encode_abba(pkbuf, &authentication_request->abba);
    ogs_assert(size >= 0);
    encoded += size;

    if (authentication_request->presencemask & OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_RAND_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_RAND_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_authentication_parameter_rand(pkbuf, &authentication_request->authentication_parameter_rand);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (authentication_request->presencemask & OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_AUTN_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_AUTN_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_authentication_parameter_autn(pkbuf, &authentication_request->authentication_parameter_autn);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (authentication_request->presencemask & OGS_NAS_5GS_AUTHENTICATION_REQUEST_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_AUTHENTICATION_REQUEST_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &authentication_request->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_authentication_response(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_authentication_response_t *authentication_response = &message->gmm.authentication_response;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode AUTHENTICATION_RESPONSE");

    if (authentication_response->presencemask & OGS_NAS_5GS_AUTHENTICATION_RESPONSE_AUTHENTICATION_RESPONSE_PARAMETER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_AUTHENTICATION_RESPONSE_AUTHENTICATION_RESPONSE_PARAMETER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_authentication_response_parameter(pkbuf, &authentication_response->authentication_response_parameter);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (authentication_response->presencemask & OGS_NAS_5GS_AUTHENTICATION_RESPONSE_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_AUTHENTICATION_RESPONSE_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &authentication_response->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_authentication_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_authentication_reject_t *authentication_reject = &message->gmm.authentication_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode AUTHENTICATION_REJECT");

    if (authentication_reject->presencemask & OGS_NAS_5GS_AUTHENTICATION_REJECT_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_AUTHENTICATION_REJECT_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &authentication_reject->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_authentication_failure(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_authentication_failure_t *authentication_failure = &message->gmm.authentication_failure;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode AUTHENTICATION_FAILURE");

    size = ogs_nas_5gs_encode_5gmm_cause(pkbuf, &authentication_failure->gmm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (authentication_failure->presencemask & OGS_NAS_5GS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_authentication_failure_parameter(pkbuf, &authentication_failure->authentication_failure_parameter);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_authentication_result(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_authentication_result_t *authentication_result = &message->gmm.authentication_result;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode AUTHENTICATION_RESULT");

    size = ogs_nas_5gs_encode_key_set_identifier(pkbuf, &authentication_result->ngksi);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_5gs_encode_eap_message(pkbuf, &authentication_result->eap_message);
    ogs_assert(size >= 0);
    encoded += size;

    if (authentication_result->presencemask & OGS_NAS_5GS_AUTHENTICATION_RESULT_ABBA_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_AUTHENTICATION_RESULT_ABBA_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_abba(pkbuf, &authentication_result->abba);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_identity_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_identity_request_t *identity_request = &message->gmm.identity_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode IDENTITY_REQUEST");

    size = ogs_nas_5gs_encode_5gs_identity_type(pkbuf, &identity_request->identity_type);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_5gs_encode_identity_response(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_identity_response_t *identity_response = &message->gmm.identity_response;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode IDENTITY_RESPONSE");

    size = ogs_nas_5gs_encode_5gs_mobile_identity(pkbuf, &identity_response->mobile_identity);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_5gs_encode_security_mode_command(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_security_mode_command_t *security_mode_command = &message->gmm.security_mode_command;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode SECURITY_MODE_COMMAND");

    size = ogs_nas_5gs_encode_security_algorithms(pkbuf, &security_mode_command->selected_nas_security_algorithms);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_5gs_encode_key_set_identifier(pkbuf, &security_mode_command->ngksi);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_5gs_encode_ue_security_capability(pkbuf, &security_mode_command->replayed_ue_security_capabilities);
    ogs_assert(size >= 0);
    encoded += size;

    if (security_mode_command->presencemask & OGS_NAS_5GS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT) {
        security_mode_command->imeisv_request.type = (OGS_NAS_5GS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_TYPE >> 4);

        size = ogs_nas_5gs_encode_imeisv_request(pkbuf, &security_mode_command->imeisv_request);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_command->presencemask & OGS_NAS_5GS_SECURITY_MODE_COMMAND_SELECTED_EPS_NAS_SECURITY_ALGORITHMS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SECURITY_MODE_COMMAND_SELECTED_EPS_NAS_SECURITY_ALGORITHMS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eps_nas_security_algorithms(pkbuf, &security_mode_command->selected_eps_nas_security_algorithms);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_command->presencemask & OGS_NAS_5GS_SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_additional_5g_security_information(pkbuf, &security_mode_command->additional_security_information);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_command->presencemask & OGS_NAS_5GS_SECURITY_MODE_COMMAND_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SECURITY_MODE_COMMAND_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &security_mode_command->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_command->presencemask & OGS_NAS_5GS_SECURITY_MODE_COMMAND_ABBA_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SECURITY_MODE_COMMAND_ABBA_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_abba(pkbuf, &security_mode_command->abba);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_command->presencemask & OGS_NAS_5GS_SECURITY_MODE_COMMAND_REPLAYED_S1_UE_SECURITY_CAPABILITIES_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SECURITY_MODE_COMMAND_REPLAYED_S1_UE_SECURITY_CAPABILITIES_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_s1_ue_security_capability(pkbuf, &security_mode_command->replayed_s1_ue_security_capabilities);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_security_mode_complete(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_security_mode_complete_t *security_mode_complete = &message->gmm.security_mode_complete;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode SECURITY_MODE_COMPLETE");

    if (security_mode_complete->presencemask & OGS_NAS_5GS_SECURITY_MODE_COMPLETE_IMEISV_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SECURITY_MODE_COMPLETE_IMEISV_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gs_mobile_identity(pkbuf, &security_mode_complete->imeisv);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_complete->presencemask & OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NAS_MESSAGE_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NAS_MESSAGE_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_message_container(pkbuf, &security_mode_complete->nas_message_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_complete->presencemask & OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NON_IMEISV_PEI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NON_IMEISV_PEI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gs_mobile_identity(pkbuf, &security_mode_complete->non_imeisv_pei);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_security_mode_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_security_mode_reject_t *security_mode_reject = &message->gmm.security_mode_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode SECURITY_MODE_REJECT");

    size = ogs_nas_5gs_encode_5gmm_cause(pkbuf, &security_mode_reject->gmm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_5gs_encode_5gmm_status(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_5gmm_status_t *gmm_status = &message->gmm.gmm_status;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode 5GMM_STATUS");

    size = ogs_nas_5gs_encode_5gmm_cause(pkbuf, &gmm_status->gmm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_5gs_encode_notification(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_notification_t *notification = &message->gmm.notification;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode NOTIFICATION");

    size = ogs_nas_5gs_encode_access_type(pkbuf, &notification->access_type);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_5gs_encode_notification_response(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_notification_response_t *notification_response = &message->gmm.notification_response;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode NOTIFICATION_RESPONSE");

    if (notification_response->presencemask & OGS_NAS_5GS_NOTIFICATION_RESPONSE_PDU_SESSION_STATUS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_NOTIFICATION_RESPONSE_PDU_SESSION_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_session_status(pkbuf, &notification_response->pdu_session_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_ul_nas_transport(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_ul_nas_transport_t *ul_nas_transport = &message->gmm.ul_nas_transport;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode UL_NAS_TRANSPORT");

    size = ogs_nas_5gs_encode_payload_container_type(pkbuf, &ul_nas_transport->payload_container_type);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_5gs_encode_payload_container(pkbuf, &ul_nas_transport->payload_container);
    ogs_assert(size >= 0);
    encoded += size;

    if (ul_nas_transport->presencemask & OGS_NAS_5GS_UL_NAS_TRANSPORT_PDU_SESSION_ID_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_UL_NAS_TRANSPORT_PDU_SESSION_ID_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_session_identity_2(pkbuf, &ul_nas_transport->pdu_session_id);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (ul_nas_transport->presencemask & OGS_NAS_5GS_UL_NAS_TRANSPORT_OLD_PDU_SESSION_ID_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_UL_NAS_TRANSPORT_OLD_PDU_SESSION_ID_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_session_identity_2(pkbuf, &ul_nas_transport->old_pdu_session_id);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (ul_nas_transport->presencemask & OGS_NAS_5GS_UL_NAS_TRANSPORT_REQUEST_TYPE_PRESENT) {
        ul_nas_transport->request_type.type = (OGS_NAS_5GS_UL_NAS_TRANSPORT_REQUEST_TYPE_TYPE >> 4);

        size = ogs_nas_5gs_encode_request_type(pkbuf, &ul_nas_transport->request_type);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (ul_nas_transport->presencemask & OGS_NAS_5GS_UL_NAS_TRANSPORT_S_NSSAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_UL_NAS_TRANSPORT_S_NSSAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_s_nssai(pkbuf, &ul_nas_transport->s_nssai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (ul_nas_transport->presencemask & OGS_NAS_5GS_UL_NAS_TRANSPORT_DNN_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_UL_NAS_TRANSPORT_DNN_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_dnn(pkbuf, &ul_nas_transport->dnn);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (ul_nas_transport->presencemask & OGS_NAS_5GS_UL_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_UL_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_additional_information(pkbuf, &ul_nas_transport->additional_information);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (ul_nas_transport->presencemask & OGS_NAS_5GS_UL_NAS_TRANSPORT_MA_PDU_SESSION_INFORMATION_PRESENT) {
        ul_nas_transport->ma_pdu_session_information.type = (OGS_NAS_5GS_UL_NAS_TRANSPORT_MA_PDU_SESSION_INFORMATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_ma_pdu_session_information(pkbuf, &ul_nas_transport->ma_pdu_session_information);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (ul_nas_transport->presencemask & OGS_NAS_5GS_UL_NAS_TRANSPORT_RELEASE_ASSISTANCE_INDICATION_PRESENT) {
        ul_nas_transport->release_assistance_indication.type = (OGS_NAS_5GS_UL_NAS_TRANSPORT_RELEASE_ASSISTANCE_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_release_assistance_indication(pkbuf, &ul_nas_transport->release_assistance_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_dl_nas_transport(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_dl_nas_transport_t *dl_nas_transport = &message->gmm.dl_nas_transport;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode DL_NAS_TRANSPORT");

    size = ogs_nas_5gs_encode_payload_container_type(pkbuf, &dl_nas_transport->payload_container_type);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_5gs_encode_payload_container(pkbuf, &dl_nas_transport->payload_container);
    ogs_assert(size >= 0);
    encoded += size;

    if (dl_nas_transport->presencemask & OGS_NAS_5GS_DL_NAS_TRANSPORT_PDU_SESSION_ID_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_DL_NAS_TRANSPORT_PDU_SESSION_ID_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_session_identity_2(pkbuf, &dl_nas_transport->pdu_session_id);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (dl_nas_transport->presencemask & OGS_NAS_5GS_DL_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_DL_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_additional_information(pkbuf, &dl_nas_transport->additional_information);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (dl_nas_transport->presencemask & OGS_NAS_5GS_DL_NAS_TRANSPORT_5GMM_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_DL_NAS_TRANSPORT_5GMM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gmm_cause(pkbuf, &dl_nas_transport->gmm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (dl_nas_transport->presencemask & OGS_NAS_5GS_DL_NAS_TRANSPORT_BACK_OFF_TIMER_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_DL_NAS_TRANSPORT_BACK_OFF_TIMER_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_3(pkbuf, &dl_nas_transport->back_off_timer_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_establishment_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_establishment_request_t *pdu_session_establishment_request = &message->gsm.pdu_session_establishment_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_ESTABLISHMENT_REQUEST");

    size = ogs_nas_5gs_encode_integrity_protection_maximum_data_rate(pkbuf, &pdu_session_establishment_request->integrity_protection_maximum_data_rate);
    ogs_assert(size >= 0);
    encoded += size;

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_TYPE_PRESENT) {
        pdu_session_establishment_request->pdu_session_type.type = (OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_TYPE_TYPE >> 4);

        size = ogs_nas_5gs_encode_pdu_session_type(pkbuf, &pdu_session_establishment_request->pdu_session_type);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SSC_MODE_PRESENT) {
        pdu_session_establishment_request->ssc_mode.type = (OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SSC_MODE_TYPE >> 4);

        size = ogs_nas_5gs_encode_ssc_mode(pkbuf, &pdu_session_establishment_request->ssc_mode);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_5GSM_CAPABILITY_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_5GSM_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_capability(pkbuf, &pdu_session_establishment_request->gsm_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_maximum_number_of_supported_packet_filters(pkbuf, &pdu_session_establishment_request->maximum_number_of_supported_packet_filters);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_PRESENT) {
        pdu_session_establishment_request->always_on_pdu_session_requested.type = (OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_TYPE >> 4);

        size = ogs_nas_5gs_encode_always_on_pdu_session_requested(pkbuf, &pdu_session_establishment_request->always_on_pdu_session_requested);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SM_PDU_DN_REQUEST_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SM_PDU_DN_REQUEST_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_sm_pdu_dn_request_container(pkbuf, &pdu_session_establishment_request->sm_pdu_dn_request_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_establishment_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_header_compression_configuration(pkbuf, &pdu_session_establishment_request->header_compression_configuration);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_DS_TT_ETHERNET_PORT_MAC_ADDRESS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_DS_TT_ETHERNET_PORT_MAC_ADDRESS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_ds_tt_ethernet_port_mac_address(pkbuf, &pdu_session_establishment_request->ds_tt_ethernet_port_mac_address);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_UE_DS_TT_RESIDENCE_TIME_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_UE_DS_TT_RESIDENCE_TIME_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_ue_ds_tt_residence_time(pkbuf, &pdu_session_establishment_request->ue_ds_tt_residence_time);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PORT_MANAGEMENT_INFORMATION_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_port_management_information_container(pkbuf, &pdu_session_establishment_request->port_management_information_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_establishment_accept(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_establishment_accept_t *pdu_session_establishment_accept = &message->gsm.pdu_session_establishment_accept;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_ESTABLISHMENT_ACCEPT");

    size = ogs_nas_5gs_encode_pdu_session_type(pkbuf, &pdu_session_establishment_accept->selected_pdu_session_type);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_5gs_encode_qos_rules(pkbuf, &pdu_session_establishment_accept->authorized_qos_rules);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_5gs_encode_session_ambr(pkbuf, &pdu_session_establishment_accept->session_ambr);
    ogs_assert(size >= 0);
    encoded += size;

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_establishment_accept->gsm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_PDU_ADDRESS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_PDU_ADDRESS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_pdu_address(pkbuf, &pdu_session_establishment_accept->pdu_address);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RQ_TIMER_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RQ_TIMER_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer(pkbuf, &pdu_session_establishment_accept->rq_timer_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_S_NSSAI_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_S_NSSAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_s_nssai(pkbuf, &pdu_session_establishment_accept->s_nssai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ALWAYS_ON_PDU_SESSION_INDICATION_PRESENT) {
        pdu_session_establishment_accept->always_on_pdu_session_indication.type = (OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ALWAYS_ON_PDU_SESSION_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_always_on_pdu_session_indication(pkbuf, &pdu_session_establishment_accept->always_on_pdu_session_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_MAPPED_EPS_BEARER_CONTEXTS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_MAPPED_EPS_BEARER_CONTEXTS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_mapped_eps_bearer_contexts(pkbuf, &pdu_session_establishment_accept->mapped_eps_bearer_contexts);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &pdu_session_establishment_accept->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_qos_flow_descriptions(pkbuf, &pdu_session_establishment_accept->authorized_qos_flow_descriptions);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_establishment_accept->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_DNN_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_DNN_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_dnn(pkbuf, &pdu_session_establishment_accept->dnn);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_NETWORK_FEATURE_SUPPORT_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_NETWORK_FEATURE_SUPPORT_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_network_feature_support(pkbuf, &pdu_session_establishment_accept->gsm_network_feature_support);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_SERVING_PLMN_RATE_CONTROL_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_SERVING_PLMN_RATE_CONTROL_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_serving_plmn_rate_control(pkbuf, &pdu_session_establishment_accept->serving_plmn_rate_control);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ATSSS_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ATSSS_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_atsss_container(pkbuf, &pdu_session_establishment_accept->atsss_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_CONTROL_PLANE_ONLY_INDICATION_PRESENT) {
        pdu_session_establishment_accept->control_plane_only_indication.type = (OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_CONTROL_PLANE_ONLY_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_control_plane_only_indication(pkbuf, &pdu_session_establishment_accept->control_plane_only_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_header_compression_configuration(pkbuf, &pdu_session_establishment_accept->header_compression_configuration);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_establishment_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_establishment_reject_t *pdu_session_establishment_reject = &message->gsm.pdu_session_establishment_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_ESTABLISHMENT_REJECT");

    size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_establishment_reject->gsm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (pdu_session_establishment_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_BACK_OFF_TIMER_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_BACK_OFF_TIMER_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_3(pkbuf, &pdu_session_establishment_reject->back_off_timer_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_PRESENT) {
        pdu_session_establishment_reject->allowed_ssc_mode.type = (OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_TYPE >> 4);

        size = ogs_nas_5gs_encode_allowed_ssc_mode(pkbuf, &pdu_session_establishment_reject->allowed_ssc_mode);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &pdu_session_establishment_reject->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_establishment_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_RE_ATTEMPT_INDICATOR_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_RE_ATTEMPT_INDICATOR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_re_attempt_indicator(pkbuf, &pdu_session_establishment_reject->re_attempt_indicator);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_congestion_re_attempt_indicator(pkbuf, &pdu_session_establishment_reject->gsm_congestion_re_attempt_indicator);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_authentication_command(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_authentication_command_t *pdu_session_authentication_command = &message->gsm.pdu_session_authentication_command;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_AUTHENTICATION_COMMAND");

    size = ogs_nas_5gs_encode_eap_message(pkbuf, &pdu_session_authentication_command->eap_message);
    ogs_assert(size >= 0);
    encoded += size;

    if (pdu_session_authentication_command->presencemask & OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_authentication_command->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_authentication_complete(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_authentication_complete_t *pdu_session_authentication_complete = &message->gsm.pdu_session_authentication_complete;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_AUTHENTICATION_COMPLETE");

    size = ogs_nas_5gs_encode_eap_message(pkbuf, &pdu_session_authentication_complete->eap_message);
    ogs_assert(size >= 0);
    encoded += size;

    if (pdu_session_authentication_complete->presencemask & OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_authentication_complete->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_authentication_result(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_authentication_result_t *pdu_session_authentication_result = &message->gsm.pdu_session_authentication_result;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_AUTHENTICATION_RESULT");

    if (pdu_session_authentication_result->presencemask & OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &pdu_session_authentication_result->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_authentication_result->presencemask & OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_authentication_result->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_modification_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_modification_request_t *pdu_session_modification_request = &message->gsm.pdu_session_modification_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_MODIFICATION_REQUEST");

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAPABILITY_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_capability(pkbuf, &pdu_session_modification_request->gsm_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_modification_request->gsm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_maximum_number_of_supported_packet_filters(pkbuf, &pdu_session_modification_request->maximum_number_of_supported_packet_filters);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_PRESENT) {
        pdu_session_modification_request->always_on_pdu_session_requested.type = (OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_TYPE >> 4);

        size = ogs_nas_5gs_encode_always_on_pdu_session_requested(pkbuf, &pdu_session_modification_request->always_on_pdu_session_requested);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_integrity_protection_maximum_data_rate(pkbuf, &pdu_session_modification_request->integrity_protection_maximum_data_rate);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_qos_rules(pkbuf, &pdu_session_modification_request->requested_qos_rules);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_qos_flow_descriptions(pkbuf, &pdu_session_modification_request->requested_qos_flow_descriptions);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAPPED_EPS_BEARER_CONTEXTS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAPPED_EPS_BEARER_CONTEXTS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_mapped_eps_bearer_contexts(pkbuf, &pdu_session_modification_request->mapped_eps_bearer_contexts);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_modification_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_PORT_MANAGEMENT_INFORMATION_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_port_management_information_container(pkbuf, &pdu_session_modification_request->port_management_information_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_header_compression_configuration(pkbuf, &pdu_session_modification_request->header_compression_configuration);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_modification_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_modification_reject_t *pdu_session_modification_reject = &message->gsm.pdu_session_modification_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_MODIFICATION_REJECT");

    size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_modification_reject->gsm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (pdu_session_modification_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_3(pkbuf, &pdu_session_modification_reject->back_off_timer_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_modification_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_re_attempt_indicator(pkbuf, &pdu_session_modification_reject->re_attempt_indicator);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_congestion_re_attempt_indicator(pkbuf, &pdu_session_modification_reject->gsm_congestion_re_attempt_indicator);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_modification_command(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_modification_command_t *pdu_session_modification_command = &message->gsm.pdu_session_modification_command;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_MODIFICATION_COMMAND");

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_5GSM_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_5GSM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_modification_command->gsm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_session_ambr(pkbuf, &pdu_session_modification_command->session_ambr);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RQ_TIMER_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RQ_TIMER_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer(pkbuf, &pdu_session_modification_command->rq_timer_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ALWAYS_ON_PDU_SESSION_INDICATION_PRESENT) {
        pdu_session_modification_command->always_on_pdu_session_indication.type = (OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ALWAYS_ON_PDU_SESSION_INDICATION_TYPE >> 4);

        size = ogs_nas_5gs_encode_always_on_pdu_session_indication(pkbuf, &pdu_session_modification_command->always_on_pdu_session_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_RULES_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_RULES_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_qos_rules(pkbuf, &pdu_session_modification_command->authorized_qos_rules);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_mapped_eps_bearer_contexts(pkbuf, &pdu_session_modification_command->mapped_eps_bearer_contexts);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_qos_flow_descriptions(pkbuf, &pdu_session_modification_command->authorized_qos_flow_descriptions);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_modification_command->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ATSSS_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ATSSS_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_atsss_container(pkbuf, &pdu_session_modification_command->atsss_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_HEADER_COMPRESSION_CONFIGURATION_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_HEADER_COMPRESSION_CONFIGURATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_header_compression_configuration(pkbuf, &pdu_session_modification_command->header_compression_configuration);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_PORT_MANAGEMENT_INFORMATION_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_port_management_information_container(pkbuf, &pdu_session_modification_command->port_management_information_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SERVING_PLMN_RATE_CONTROL_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SERVING_PLMN_RATE_CONTROL_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_serving_plmn_rate_control(pkbuf, &pdu_session_modification_command->serving_plmn_rate_control);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_modification_complete(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_modification_complete_t *pdu_session_modification_complete = &message->gsm.pdu_session_modification_complete;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_MODIFICATION_COMPLETE");

    if (pdu_session_modification_complete->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_modification_complete->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_complete->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_PORT_MANAGEMENT_INFORMATION_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_port_management_information_container(pkbuf, &pdu_session_modification_complete->port_management_information_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_modification_command_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_modification_command_reject_t *pdu_session_modification_command_reject = &message->gsm.pdu_session_modification_command_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_MODIFICATION_COMMAND_REJECT");

    size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_modification_command_reject->gsm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (pdu_session_modification_command_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_modification_command_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_release_request(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_release_request_t *pdu_session_release_request = &message->gsm.pdu_session_release_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_RELEASE_REQUEST");

    if (pdu_session_release_request->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_5GSM_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_5GSM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_release_request->gsm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_release_request->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_release_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_release_reject(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_release_reject_t *pdu_session_release_reject = &message->gsm.pdu_session_release_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_RELEASE_REJECT");

    size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_release_reject->gsm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (pdu_session_release_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_release_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_release_command(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_release_command_t *pdu_session_release_command = &message->gsm.pdu_session_release_command;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_RELEASE_COMMAND");

    size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_release_command->gsm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (pdu_session_release_command->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_BACK_OFF_TIMER_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_BACK_OFF_TIMER_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer_3(pkbuf, &pdu_session_release_command->back_off_timer_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_release_command->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &pdu_session_release_command->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_release_command->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_congestion_re_attempt_indicator(pkbuf, &pdu_session_release_command->gsm_congestion_re_attempt_indicator);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_release_command->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_release_command->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_release_command->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_ACCESS_TYPE_PRESENT) {
        pdu_session_release_command->access_type.type = (OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_ACCESS_TYPE_TYPE >> 4);

        size = ogs_nas_5gs_encode_access_type(pkbuf, &pdu_session_release_command->access_type);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_pdu_session_release_complete(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_release_complete_t *pdu_session_release_complete = &message->gsm.pdu_session_release_complete;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDU_SESSION_RELEASE_COMPLETE");

    if (pdu_session_release_complete->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_5GSM_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_5GSM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_release_complete->gsm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_release_complete->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_release_complete->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_5gs_encode_5gsm_status(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_5gsm_status_t *gsm_status = &message->gsm.gsm_status;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode 5GSM_STATUS");

    size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &gsm_status->gsm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

ogs_pkbuf_t *ogs_nas_5gmm_encode(ogs_nas_5gs_message_t *message)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int size = 0;
    int encoded = 0;

    ogs_assert(message);

    /* The Packet Buffer(ogs_pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_expect_or_return_val(pkbuf, NULL);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-OGS_NAS_HEADROOM);

    size = sizeof(ogs_nas_5gmm_header_t);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));

    memcpy(pkbuf->data - size, &message->gmm.h, size);
    encoded += size;

    switch(message->gmm.h.message_type) {
    case OGS_NAS_5GS_REGISTRATION_REQUEST:
        size = ogs_nas_5gs_encode_registration_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_REGISTRATION_ACCEPT:
        size = ogs_nas_5gs_encode_registration_accept(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_REGISTRATION_COMPLETE:
        size = ogs_nas_5gs_encode_registration_complete(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_REGISTRATION_REJECT:
        size = ogs_nas_5gs_encode_registration_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_DEREGISTRATION_REQUEST:
        size = ogs_nas_5gs_encode_deregistration_request_to_ue(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_DEREGISTRATION_ACCEPT:
        break;
    case OGS_NAS_5GS_SERVICE_REQUEST:
        size = ogs_nas_5gs_encode_service_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_SERVICE_REJECT:
        size = ogs_nas_5gs_encode_service_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_SERVICE_ACCEPT:
        size = ogs_nas_5gs_encode_service_accept(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND:
        size = ogs_nas_5gs_encode_configuration_update_command(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMPLETE:
        break;
    case OGS_NAS_5GS_AUTHENTICATION_REQUEST:
        size = ogs_nas_5gs_encode_authentication_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_AUTHENTICATION_RESPONSE:
        size = ogs_nas_5gs_encode_authentication_response(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_AUTHENTICATION_REJECT:
        size = ogs_nas_5gs_encode_authentication_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_AUTHENTICATION_FAILURE:
        size = ogs_nas_5gs_encode_authentication_failure(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_AUTHENTICATION_RESULT:
        size = ogs_nas_5gs_encode_authentication_result(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_IDENTITY_REQUEST:
        size = ogs_nas_5gs_encode_identity_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_IDENTITY_RESPONSE:
        size = ogs_nas_5gs_encode_identity_response(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_SECURITY_MODE_COMMAND:
        size = ogs_nas_5gs_encode_security_mode_command(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_SECURITY_MODE_COMPLETE:
        size = ogs_nas_5gs_encode_security_mode_complete(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_SECURITY_MODE_REJECT:
        size = ogs_nas_5gs_encode_security_mode_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_5GMM_STATUS:
        size = ogs_nas_5gs_encode_5gmm_status(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_NOTIFICATION:
        size = ogs_nas_5gs_encode_notification(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_NOTIFICATION_RESPONSE:
        size = ogs_nas_5gs_encode_notification_response(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_UL_NAS_TRANSPORT:
        size = ogs_nas_5gs_encode_ul_nas_transport(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_DL_NAS_TRANSPORT:
        size = ogs_nas_5gs_encode_dl_nas_transport(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    default:
        ogs_error("Unknown message type (0x%x) or not implemented", 
                message->gmm.h.message_type);
        ogs_pkbuf_free(pkbuf);
        return NULL;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, encoded));

    pkbuf->len = encoded;

    return pkbuf;
}

ogs_pkbuf_t *ogs_nas_5gsm_encode(ogs_nas_5gs_message_t *message)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int size = 0;
    int encoded = 0;

    ogs_assert(message);

    /* The Packet Buffer(ogs_pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_expect_or_return_val(pkbuf, NULL);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-OGS_NAS_HEADROOM);

    size = sizeof(ogs_nas_5gsm_header_t);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &message->gsm.h, size);
    encoded += size;

    switch(message->gsm.h.message_type) {
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST:
        size = ogs_nas_5gs_encode_pdu_session_establishment_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT:
        size = ogs_nas_5gs_encode_pdu_session_establishment_accept(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT:
        size = ogs_nas_5gs_encode_pdu_session_establishment_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMMAND:
        size = ogs_nas_5gs_encode_pdu_session_authentication_command(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMPLETE:
        size = ogs_nas_5gs_encode_pdu_session_authentication_complete(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT:
        size = ogs_nas_5gs_encode_pdu_session_authentication_result(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST:
        size = ogs_nas_5gs_encode_pdu_session_modification_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT:
        size = ogs_nas_5gs_encode_pdu_session_modification_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND:
        size = ogs_nas_5gs_encode_pdu_session_modification_command(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE:
        size = ogs_nas_5gs_encode_pdu_session_modification_complete(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT:
        size = ogs_nas_5gs_encode_pdu_session_modification_command_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST:
        size = ogs_nas_5gs_encode_pdu_session_release_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT:
        size = ogs_nas_5gs_encode_pdu_session_release_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND:
        size = ogs_nas_5gs_encode_pdu_session_release_command(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
        size = ogs_nas_5gs_encode_pdu_session_release_complete(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_5GSM_STATUS:
        size = ogs_nas_5gs_encode_5gsm_status(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    default:
        ogs_error("Unknown message type (0x%x) or not implemented", 
                message->gsm.h.message_type);
        ogs_pkbuf_free(pkbuf);
        return NULL;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, encoded));
    pkbuf->len = encoded;

    return pkbuf;
}

ogs_pkbuf_t *ogs_nas_5gs_plain_encode(ogs_nas_5gs_message_t *message)
{
    ogs_assert(message);

    ogs_assert(message->gmm.h.extended_protocol_discriminator ==
            message->gsm.h.extended_protocol_discriminator);

    if (message->gmm.h.extended_protocol_discriminator == 
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM)
        return ogs_nas_5gmm_encode(message);
    else if (message->gmm.h.extended_protocol_discriminator == 
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM)
        return ogs_nas_5gsm_encode(message);

    return NULL;
}
