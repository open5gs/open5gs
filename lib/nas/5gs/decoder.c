/*
 * The MIT License
 *
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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
 * Created on: 2024-01-21 18:47:47.827076 by acetcom
 * from 24501-h90.docx
 ******************************************************************************/

#include "ogs-nas-5gs.h"

int ogs_nas_5gs_decode_registration_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_registration_accept(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_registration_complete(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_registration_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_deregistration_request_from_ue(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_deregistration_request_to_ue(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_service_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_service_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_service_accept(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_configuration_update_command(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_authentication_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_authentication_response(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_authentication_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_authentication_failure(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_authentication_result(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_identity_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_identity_response(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_security_mode_command(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_security_mode_complete(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_security_mode_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gmm_status(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_notification(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_notification_response(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_ul_nas_transport(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_dl_nas_transport(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_establishment_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_establishment_accept(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_establishment_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_authentication_command(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_authentication_complete(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_authentication_result(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_modification_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_modification_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_modification_command(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_modification_complete(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_modification_command_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_release_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_release_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_release_command(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_release_complete(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gsm_status(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_registration_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_registration_request_t *registration_request = &message->gmm.registration_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode REGISTRATION_REQUEST\n");

    size = ogs_nas_5gs_decode_5gs_registration_type(&registration_request->registration_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gs_registration_type() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_5gs_decode_5gs_mobile_identity(&registration_request->mobile_identity, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gs_mobile_identity() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_REGISTRATION_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_key_set_identifier(&registration_request->non_current_native_nas_key_set_identifier, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_key_set_identifier() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_5GMM_CAPABILITY_TYPE:
            size = ogs_nas_5gs_decode_5gmm_capability(&registration_request->gmm_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gmm_capability() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_TYPE:
            size = ogs_nas_5gs_decode_ue_security_capability(&registration_request->ue_security_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ue_security_capability() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_nssai(&registration_request->requested_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nssai() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE:
            size = ogs_nas_5gs_decode_5gs_tracking_area_identity(&registration_request->last_visited_registered_tai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_tracking_area_identity() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_TYPE:
            size = ogs_nas_5gs_decode_s1_ue_network_capability(&registration_request->s1_ue_network_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_s1_ue_network_capability() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_UPLINK_DATA_STATUS_TYPE:
            size = ogs_nas_5gs_decode_uplink_data_status(&registration_request->uplink_data_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_uplink_data_status() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_UPLINK_DATA_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_PDU_SESSION_STATUS_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_status(&registration_request->pdu_session_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_status() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_PDU_SESSION_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_MICO_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_mico_indication(&registration_request->mico_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_mico_indication() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_MICO_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_UE_STATUS_TYPE:
            size = ogs_nas_5gs_decode_ue_status(&registration_request->ue_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ue_status() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_UE_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_ADDITIONAL_GUTI_TYPE:
            size = ogs_nas_5gs_decode_5gs_mobile_identity(&registration_request->additional_guti, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_mobile_identity() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_ADDITIONAL_GUTI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_TYPE:
            size = ogs_nas_5gs_decode_allowed_pdu_session_status(&registration_request->allowed_pdu_session_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_allowed_pdu_session_status() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_UE_USAGE_SETTING_TYPE:
            size = ogs_nas_5gs_decode_ue_usage_setting(&registration_request->ue_usage_setting, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ue_usage_setting() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_UE_USAGE_SETTING_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_DRX_PARAMETERS_TYPE:
            size = ogs_nas_5gs_decode_5gs_drx_parameters(&registration_request->requested_drx_parameters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_drx_parameters() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_DRX_PARAMETERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_eps_nas_message_container(&registration_request->eps_nas_message_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eps_nas_message_container() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_LADN_INDICATION_TYPE:
            size = ogs_nas_5gs_decode_ladn_indication(&registration_request->ladn_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ladn_indication() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_LADN_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_payload_container_type(&registration_request->payload_container_type, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_payload_container_type() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_payload_container(&registration_request->payload_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_payload_container() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_PAYLOAD_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_network_slicing_indication(&registration_request->network_slicing_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_network_slicing_indication() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_5GS_UPDATE_TYPE_TYPE:
            size = ogs_nas_5gs_decode_5gs_update_type(&registration_request->update_type, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_update_type() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_5GS_UPDATE_TYPE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE:
            size = ogs_nas_5gs_decode_mobile_station_classmark_2(&registration_request->mobile_station_classmark_2, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_mobile_station_classmark_2() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_SUPPORTED_CODECS_TYPE:
            size = ogs_nas_5gs_decode_supported_codec_list(&registration_request->supported_codecs, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_supported_codec_list() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_SUPPORTED_CODECS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_message_container(&registration_request->nas_message_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_message_container() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE:
            size = ogs_nas_5gs_decode_eps_bearer_context_status(&registration_request->eps_bearer_context_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eps_bearer_context_status() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_EXTENDED_DRX_PARAMETERS_TYPE:
            size = ogs_nas_5gs_decode_extended_drx_parameters(&registration_request->requested_extended_drx_parameters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_drx_parameters() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_EXTENDED_DRX_PARAMETERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_T3324_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&registration_request->t3324_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_T3324_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_UE_RADIO_CAPABILITY_ID_TYPE:
            size = ogs_nas_5gs_decode_ue_radio_capability_id(&registration_request->ue_radio_capability_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ue_radio_capability_id() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_UE_RADIO_CAPABILITY_ID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_MAPPED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_mapped_nssai(&registration_request->requested_mapped_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_mapped_nssai() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_MAPPED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_TYPE:
            size = ogs_nas_5gs_decode_additional_information_requested(&registration_request->additional_information_requested, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_additional_information_requested() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_WUS_ASSISTANCE_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_wus_assistance_information(&registration_request->requested_wus_assistance_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_wus_assistance_information() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_WUS_ASSISTANCE_INFORMATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_N5GC_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_n5gc_indication(&registration_request->n5gc_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_n5gc_indication() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_N5GC_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_NB_N1_MODE_DRX_PARAMETERS_TYPE:
            size = ogs_nas_5gs_decode_nb_n1_mode_drx_parameters(&registration_request->requested_nb_n1_mode_drx_parameters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nb_n1_mode_drx_parameters() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_NB_N1_MODE_DRX_PARAMETERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_UE_REQUEST_TYPE_TYPE:
            size = ogs_nas_5gs_decode_ue_request_type(&registration_request->ue_request_type, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ue_request_type() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_UE_REQUEST_TYPE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_PAGING_RESTRICTION_TYPE:
            size = ogs_nas_5gs_decode_paging_restriction(&registration_request->paging_restriction, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_paging_restriction() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_PAGING_RESTRICTION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_SERVICE_LEVEL_AA_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_service_level_aa_container(&registration_request->service_level_aa_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_service_level_aa_container() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_SERVICE_LEVEL_AA_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_NID_TYPE:
            size = ogs_nas_5gs_decode_nid(&registration_request->nid, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nid() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_NID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_MS_DETERMINED_PLMN_WITH_DISASTER_CONDITION_TYPE:
            size = ogs_nas_5gs_decode_plmn_identity(&registration_request->ms_determined_plmn_with_disaster_condition, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_plmn_identity() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_MS_DETERMINED_PLMN_WITH_DISASTER_CONDITION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_PEIPS_ASSISTANCE_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_peips_assistance_information(&registration_request->requested_peips_assistance_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_peips_assistance_information() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_PEIPS_ASSISTANCE_INFORMATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_T3512_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&registration_request->requested_t3512_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            registration_request->presencemask |= OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_T3512_VALUE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_registration_accept(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_registration_accept_t *registration_accept = &message->gmm.registration_accept;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode REGISTRATION_ACCEPT\n");

    size = ogs_nas_5gs_decode_5gs_registration_result(&registration_accept->registration_result, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gs_registration_result() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_5G_GUTI_TYPE:
            size = ogs_nas_5gs_decode_5gs_mobile_identity(&registration_accept->guti, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_mobile_identity() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_5G_GUTI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_EQUIVALENT_PLMNS_TYPE:
            size = ogs_nas_5gs_decode_plmn_list(&registration_accept->equivalent_plmns, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_plmn_list() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_EQUIVALENT_PLMNS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_TAI_LIST_TYPE:
            size = ogs_nas_5gs_decode_5gs_tracking_area_identity_list(&registration_accept->tai_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_tracking_area_identity_list() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_TAI_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_ALLOWED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_nssai(&registration_accept->allowed_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nssai() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_ALLOWED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_REJECTED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_rejected_nssai(&registration_accept->rejected_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_rejected_nssai() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_REJECTED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_CONFIGURED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_nssai(&registration_accept->configured_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nssai() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_CONFIGURED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_TYPE:
            size = ogs_nas_5gs_decode_5gs_network_feature_support(&registration_accept->network_feature_support, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_network_feature_support() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_STATUS_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_status(&registration_accept->pdu_session_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_status() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_reactivation_result(&registration_accept->pdu_session_reactivation_result, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_reactivation_result() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_reactivation_result_error_cause(&registration_accept->pdu_session_reactivation_result_error_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_reactivation_result_error_cause() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_LADN_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_ladn_information(&registration_accept->ladn_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ladn_information() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_LADN_INFORMATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_MICO_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_mico_indication(&registration_accept->mico_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_mico_indication() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_MICO_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_network_slicing_indication(&registration_accept->network_slicing_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_network_slicing_indication() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_SERVICE_AREA_LIST_TYPE:
            size = ogs_nas_5gs_decode_service_area_list(&registration_accept->service_area_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_service_area_list() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_SERVICE_AREA_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_T3512_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&registration_accept->t3512_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_T3512_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_NON_3GPP_DE_REGISTRATION_TIMER_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_2(&registration_accept->non_3gpp_de_registration_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_2() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_NON_3GPP_DE_REGISTRATION_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_T3502_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_2(&registration_accept->t3502_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_2() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_T3502_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE:
            size = ogs_nas_5gs_decode_emergency_number_list(&registration_accept->emergency_number_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_emergency_number_list() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_TYPE:
            size = ogs_nas_5gs_decode_extended_emergency_number_list(&registration_accept->extended_emergency_number_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_emergency_number_list() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_SOR_TRANSPARENT_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_sor_transparent_container(&registration_accept->sor_transparent_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_sor_transparent_container() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_SOR_TRANSPARENT_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&registration_accept->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_nssai_inclusion_mode(&registration_accept->nssai_inclusion_mode, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nssai_inclusion_mode() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_TYPE:
            size = ogs_nas_5gs_decode_operator_defined_access_category_definitions(&registration_accept->operator_defined_access_category_definitions, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_operator_defined_access_category_definitions() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_DRX_PARAMETERS_TYPE:
            size = ogs_nas_5gs_decode_5gs_drx_parameters(&registration_accept->negotiated_drx_parameters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_drx_parameters() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_DRX_PARAMETERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_NON_3GPP_NW_POLICIES_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_non_3gpp_nw_provided_policies(&registration_accept->non_3gpp_nw_policies, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_non_3gpp_nw_provided_policies() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_NON_3GPP_NW_POLICIES_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_EPS_BEARER_CONTEXT_STATUS_TYPE:
            size = ogs_nas_5gs_decode_eps_bearer_context_status(&registration_accept->eps_bearer_context_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eps_bearer_context_status() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_EPS_BEARER_CONTEXT_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_EXTENDED_DRX_PARAMETERS_TYPE:
            size = ogs_nas_5gs_decode_extended_drx_parameters(&registration_accept->negotiated_extended_drx_parameters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_drx_parameters() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_EXTENDED_DRX_PARAMETERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_T3447_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&registration_accept->t3447_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_T3447_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_T3448_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_2(&registration_accept->t3448_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_2() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_T3448_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_T3324_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&registration_accept->t3324_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_T3324_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_UE_RADIO_CAPABILITY_ID_TYPE:
            size = ogs_nas_5gs_decode_ue_radio_capability_id(&registration_accept->ue_radio_capability_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ue_radio_capability_id() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_UE_RADIO_CAPABILITY_ID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_ue_radio_capability_id_deletion_indication(&registration_accept->ue_radio_capability_id_deletion_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ue_radio_capability_id_deletion_indication() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_PENDING_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_nssai(&registration_accept->pending_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nssai() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_PENDING_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_CIPHERING_KEY_DATA_TYPE:
            size = ogs_nas_5gs_decode_ciphering_key_data(&registration_accept->ciphering_key_data, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ciphering_key_data() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_CIPHERING_KEY_DATA_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_CAG_INFORMATION_LIST_TYPE:
            size = ogs_nas_5gs_decode_cag_information_list(&registration_accept->cag_information_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_cag_information_list() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_CAG_INFORMATION_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_TRUNCATED_5G_S_TMSI_CONFIGURATION_TYPE:
            size = ogs_nas_5gs_decode_truncated_5g_s_tmsi_configuration(&registration_accept->truncated_s_tmsi_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_truncated_5g_s_tmsi_configuration() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_TRUNCATED_5G_S_TMSI_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_WUS_ASSISTANCE_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_wus_assistance_information(&registration_accept->negotiated_wus_assistance_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_wus_assistance_information() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_WUS_ASSISTANCE_INFORMATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_NB_N1_MODE_DRX_PARAMETERS_TYPE:
            size = ogs_nas_5gs_decode_nb_n1_mode_drx_parameters(&registration_accept->negotiated_nb_n1_mode_drx_parameters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nb_n1_mode_drx_parameters() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_NB_N1_MODE_DRX_PARAMETERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_REJECTED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_extended_rejected_nssai(&registration_accept->extended_rejected_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_rejected_nssai() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_REJECTED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_SERVICE_LEVEL_AA_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_service_level_aa_container(&registration_accept->service_level_aa_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_service_level_aa_container() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_SERVICE_LEVEL_AA_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_PEIPS_ASSISTANCE_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_peips_assistance_information(&registration_accept->negotiated_peips_assistance_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_peips_assistance_information() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_PEIPS_ASSISTANCE_INFORMATION_PRESENT;
            decoded += size;
            break;
#if 0 /* Modified by acetcom */
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_5GS_ADDITIONAL_REQUEST_RESULT_TYPE:
            size = ogs_nas_5gs_decode_5gs_additional_request_result(&registration_accept->additional_request_result, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_additional_request_result() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_5GS_ADDITIONAL_REQUEST_RESULT_PRESENT;
            decoded += size;
            break;
#endif
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_NSSRG_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_nssrg_information(&registration_accept->nssrg_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nssrg_information() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_NSSRG_INFORMATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_DISASTER_ROAMING_WAIT_RANGE_TYPE:
            size = ogs_nas_5gs_decode_registration_wait_range(&registration_accept->disaster_roaming_wait_range, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_registration_wait_range() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_DISASTER_ROAMING_WAIT_RANGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_DISASTER_RETURN_WAIT_RANGE_TYPE:
            size = ogs_nas_5gs_decode_registration_wait_range(&registration_accept->disaster_return_wait_range, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_registration_wait_range() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_DISASTER_RETURN_WAIT_RANGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_LIST_OF_PLMNS_TO_BE_USED_IN_DISASTER_CONDITION_TYPE:
            size = ogs_nas_5gs_decode_list_of_plmns_to_be_used_in_disaster_condition(&registration_accept->list_of_plmns_to_be_used_in_disaster_condition, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_list_of_plmns_to_be_used_in_disaster_condition() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_LIST_OF_PLMNS_TO_BE_USED_IN_DISASTER_CONDITION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE:
            size = ogs_nas_5gs_decode_5gs_tracking_area_identity_list(&registration_accept->forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_tracking_area_identity_list() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE:
            size = ogs_nas_5gs_decode_5gs_tracking_area_identity_list(&registration_accept->forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_tracking_area_identity_list() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_CAG_INFORMATION_LIST_TYPE:
            size = ogs_nas_5gs_decode_extended_cag_information_list(&registration_accept->extended_cag_information_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_cag_information_list() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_CAG_INFORMATION_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_ACCEPT_NSAG_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_nsag_information(&registration_accept->nsag_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nsag_information() failed");
               return size;
            }

            registration_accept->presencemask |= OGS_NAS_5GS_REGISTRATION_ACCEPT_NSAG_INFORMATION_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_registration_complete(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_registration_complete_t *registration_complete = &message->gmm.registration_complete;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode REGISTRATION_COMPLETE\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_REGISTRATION_COMPLETE_SOR_TRANSPARENT_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_sor_transparent_container(&registration_complete->sor_transparent_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_sor_transparent_container() failed");
               return size;
            }

            registration_complete->presencemask |= OGS_NAS_5GS_REGISTRATION_COMPLETE_SOR_TRANSPARENT_CONTAINER_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_registration_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_registration_reject_t *registration_reject = &message->gmm.registration_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode REGISTRATION_REJECT\n");

    size = ogs_nas_5gs_decode_5gmm_cause(&registration_reject->gmm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gmm_cause() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_REGISTRATION_REJECT_T3346_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_2(&registration_reject->t3346_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_2() failed");
               return size;
            }

            registration_reject->presencemask |= OGS_NAS_5GS_REGISTRATION_REJECT_T3346_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REJECT_T3502_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_2(&registration_reject->t3502_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_2() failed");
               return size;
            }

            registration_reject->presencemask |= OGS_NAS_5GS_REGISTRATION_REJECT_T3502_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REJECT_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&registration_reject->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            registration_reject->presencemask |= OGS_NAS_5GS_REGISTRATION_REJECT_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REJECT_REJECTED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_rejected_nssai(&registration_reject->rejected_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_rejected_nssai() failed");
               return size;
            }

            registration_reject->presencemask |= OGS_NAS_5GS_REGISTRATION_REJECT_REJECTED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REJECT_CAG_INFORMATION_LIST_TYPE:
            size = ogs_nas_5gs_decode_cag_information_list(&registration_reject->cag_information_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_cag_information_list() failed");
               return size;
            }

            registration_reject->presencemask |= OGS_NAS_5GS_REGISTRATION_REJECT_CAG_INFORMATION_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REJECT_EXTENDED_REJECTED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_extended_rejected_nssai(&registration_reject->extended_rejected_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_rejected_nssai() failed");
               return size;
            }

            registration_reject->presencemask |= OGS_NAS_5GS_REGISTRATION_REJECT_EXTENDED_REJECTED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REJECT_DISASTER_RETURN_WAIT_RANGE_TYPE:
            size = ogs_nas_5gs_decode_registration_wait_range(&registration_reject->disaster_return_wait_range, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_registration_wait_range() failed");
               return size;
            }

            registration_reject->presencemask |= OGS_NAS_5GS_REGISTRATION_REJECT_DISASTER_RETURN_WAIT_RANGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REJECT_EXTENDED_CAG_INFORMATION_LIST_TYPE:
            size = ogs_nas_5gs_decode_extended_cag_information_list(&registration_reject->extended_cag_information_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_cag_information_list() failed");
               return size;
            }

            registration_reject->presencemask |= OGS_NAS_5GS_REGISTRATION_REJECT_EXTENDED_CAG_INFORMATION_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REJECT_LOWER_BOUND_TIMER_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&registration_reject->lower_bound_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            registration_reject->presencemask |= OGS_NAS_5GS_REGISTRATION_REJECT_LOWER_BOUND_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE:
            size = ogs_nas_5gs_decode_5gs_tracking_area_identity_list(&registration_reject->forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_tracking_area_identity_list() failed");
               return size;
            }

            registration_reject->presencemask |= OGS_NAS_5GS_REGISTRATION_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_REGISTRATION_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE:
            size = ogs_nas_5gs_decode_5gs_tracking_area_identity_list(&registration_reject->forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_tracking_area_identity_list() failed");
               return size;
            }

            registration_reject->presencemask |= OGS_NAS_5GS_REGISTRATION_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_deregistration_request_from_ue(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_deregistration_request_from_ue_t *deregistration_request_from_ue = &message->gmm.deregistration_request_from_ue;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode DEREGISTRATION_REQUEST_FROM_UE\n");

    size = ogs_nas_5gs_decode_de_registration_type(&deregistration_request_from_ue->de_registration_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_de_registration_type() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_5gs_decode_5gs_mobile_identity(&deregistration_request_from_ue->mobile_identity, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gs_mobile_identity() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_5gs_decode_deregistration_request_to_ue(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_deregistration_request_to_ue_t *deregistration_request_to_ue = &message->gmm.deregistration_request_to_ue;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode DEREGISTRATION_REQUEST_TO_UE\n");

    size = ogs_nas_5gs_decode_de_registration_type(&deregistration_request_to_ue->de_registration_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_de_registration_type() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_5GMM_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_5gmm_cause(&deregistration_request_to_ue->gmm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gmm_cause() failed");
               return size;
            }

            deregistration_request_to_ue->presencemask |= OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_5GMM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_T3346_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_2(&deregistration_request_to_ue->t3346_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_2() failed");
               return size;
            }

            deregistration_request_to_ue->presencemask |= OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_T3346_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_REJECTED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_rejected_nssai(&deregistration_request_to_ue->rejected_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_rejected_nssai() failed");
               return size;
            }

            deregistration_request_to_ue->presencemask |= OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_REJECTED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_CAG_INFORMATION_LIST_TYPE:
            size = ogs_nas_5gs_decode_cag_information_list(&deregistration_request_to_ue->cag_information_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_cag_information_list() failed");
               return size;
            }

            deregistration_request_to_ue->presencemask |= OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_CAG_INFORMATION_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_EXTENDED_REJECTED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_extended_rejected_nssai(&deregistration_request_to_ue->extended_rejected_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_rejected_nssai() failed");
               return size;
            }

            deregistration_request_to_ue->presencemask |= OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_EXTENDED_REJECTED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_DISASTER_RETURN_WAIT_RANGE_TYPE:
            size = ogs_nas_5gs_decode_registration_wait_range(&deregistration_request_to_ue->disaster_return_wait_range, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_registration_wait_range() failed");
               return size;
            }

            deregistration_request_to_ue->presencemask |= OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_DISASTER_RETURN_WAIT_RANGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_EXTENDED_CAG_INFORMATION_LIST_TYPE:
            size = ogs_nas_5gs_decode_extended_cag_information_list(&deregistration_request_to_ue->extended_cag_information_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_cag_information_list() failed");
               return size;
            }

            deregistration_request_to_ue->presencemask |= OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_EXTENDED_CAG_INFORMATION_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_LOWER_BOUND_TIMER_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&deregistration_request_to_ue->lower_bound_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            deregistration_request_to_ue->presencemask |= OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_LOWER_BOUND_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE:
            size = ogs_nas_5gs_decode_5gs_tracking_area_identity_list(&deregistration_request_to_ue->forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_tracking_area_identity_list() failed");
               return size;
            }

            deregistration_request_to_ue->presencemask |= OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE:
            size = ogs_nas_5gs_decode_5gs_tracking_area_identity_list(&deregistration_request_to_ue->forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_tracking_area_identity_list() failed");
               return size;
            }

            deregistration_request_to_ue->presencemask |= OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_service_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_service_request_t *service_request = &message->gmm.service_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode SERVICE_REQUEST\n");

    size = ogs_nas_5gs_decode_key_set_identifier(&service_request->ngksi, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_key_set_identifier() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_5gs_decode_5gs_mobile_identity(&service_request->s_tmsi, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gs_mobile_identity() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_SERVICE_REQUEST_UPLINK_DATA_STATUS_TYPE:
            size = ogs_nas_5gs_decode_uplink_data_status(&service_request->uplink_data_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_uplink_data_status() failed");
               return size;
            }

            service_request->presencemask |= OGS_NAS_5GS_SERVICE_REQUEST_UPLINK_DATA_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REQUEST_PDU_SESSION_STATUS_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_status(&service_request->pdu_session_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_status() failed");
               return size;
            }

            service_request->presencemask |= OGS_NAS_5GS_SERVICE_REQUEST_PDU_SESSION_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REQUEST_ALLOWED_PDU_SESSION_STATUS_TYPE:
            size = ogs_nas_5gs_decode_allowed_pdu_session_status(&service_request->allowed_pdu_session_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_allowed_pdu_session_status() failed");
               return size;
            }

            service_request->presencemask |= OGS_NAS_5GS_SERVICE_REQUEST_ALLOWED_PDU_SESSION_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REQUEST_NAS_MESSAGE_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_message_container(&service_request->nas_message_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_message_container() failed");
               return size;
            }

            service_request->presencemask |= OGS_NAS_5GS_SERVICE_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REQUEST_UE_REQUEST_TYPE_TYPE:
            size = ogs_nas_5gs_decode_ue_request_type(&service_request->ue_request_type, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ue_request_type() failed");
               return size;
            }

            service_request->presencemask |= OGS_NAS_5GS_SERVICE_REQUEST_UE_REQUEST_TYPE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REQUEST_PAGING_RESTRICTION_TYPE:
            size = ogs_nas_5gs_decode_paging_restriction(&service_request->paging_restriction, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_paging_restriction() failed");
               return size;
            }

            service_request->presencemask |= OGS_NAS_5GS_SERVICE_REQUEST_PAGING_RESTRICTION_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_service_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_service_reject_t *service_reject = &message->gmm.service_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode SERVICE_REJECT\n");

    size = ogs_nas_5gs_decode_5gmm_cause(&service_reject->gmm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gmm_cause() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_SERVICE_REJECT_PDU_SESSION_STATUS_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_status(&service_reject->pdu_session_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_status() failed");
               return size;
            }

            service_reject->presencemask |= OGS_NAS_5GS_SERVICE_REJECT_PDU_SESSION_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REJECT_T3346_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_2(&service_reject->t3346_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_2() failed");
               return size;
            }

            service_reject->presencemask |= OGS_NAS_5GS_SERVICE_REJECT_T3346_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REJECT_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&service_reject->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            service_reject->presencemask |= OGS_NAS_5GS_SERVICE_REJECT_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REJECT_T3448_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_2(&service_reject->t3448_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_2() failed");
               return size;
            }

            service_reject->presencemask |= OGS_NAS_5GS_SERVICE_REJECT_T3448_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REJECT_CAG_INFORMATION_LIST_TYPE:
            size = ogs_nas_5gs_decode_cag_information_list(&service_reject->cag_information_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_cag_information_list() failed");
               return size;
            }

            service_reject->presencemask |= OGS_NAS_5GS_SERVICE_REJECT_CAG_INFORMATION_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REJECT_DISASTER_RETURN_WAIT_RANGE_TYPE:
            size = ogs_nas_5gs_decode_registration_wait_range(&service_reject->disaster_return_wait_range, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_registration_wait_range() failed");
               return size;
            }

            service_reject->presencemask |= OGS_NAS_5GS_SERVICE_REJECT_DISASTER_RETURN_WAIT_RANGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REJECT_EXTENDED_CAG_INFORMATION_LIST_TYPE:
            size = ogs_nas_5gs_decode_extended_cag_information_list(&service_reject->extended_cag_information_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_cag_information_list() failed");
               return size;
            }

            service_reject->presencemask |= OGS_NAS_5GS_SERVICE_REJECT_EXTENDED_CAG_INFORMATION_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REJECT_LOWER_BOUND_TIMER_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&service_reject->lower_bound_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            service_reject->presencemask |= OGS_NAS_5GS_SERVICE_REJECT_LOWER_BOUND_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE:
            size = ogs_nas_5gs_decode_5gs_tracking_area_identity_list(&service_reject->forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_tracking_area_identity_list() failed");
               return size;
            }

            service_reject->presencemask |= OGS_NAS_5GS_SERVICE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE:
            size = ogs_nas_5gs_decode_5gs_tracking_area_identity_list(&service_reject->forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_tracking_area_identity_list() failed");
               return size;
            }

            service_reject->presencemask |= OGS_NAS_5GS_SERVICE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_service_accept(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_service_accept_t *service_accept = &message->gmm.service_accept;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode SERVICE_ACCEPT\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_STATUS_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_status(&service_accept->pdu_session_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_status() failed");
               return size;
            }

            service_accept->presencemask |= OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_reactivation_result(&service_accept->pdu_session_reactivation_result, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_reactivation_result() failed");
               return size;
            }

            service_accept->presencemask |= OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_reactivation_result_error_cause(&service_accept->pdu_session_reactivation_result_error_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_reactivation_result_error_cause() failed");
               return size;
            }

            service_accept->presencemask |= OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_ACCEPT_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&service_accept->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            service_accept->presencemask |= OGS_NAS_5GS_SERVICE_ACCEPT_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_ACCEPT_T3448_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_2(&service_accept->t3448_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_2() failed");
               return size;
            }

            service_accept->presencemask |= OGS_NAS_5GS_SERVICE_ACCEPT_T3448_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_ACCEPT_5GS_ADDITIONAL_REQUEST_RESULT_TYPE:
            size = ogs_nas_5gs_decode_5gs_additional_request_result(&service_accept->additional_request_result, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_additional_request_result() failed");
               return size;
            }

            service_accept->presencemask |= OGS_NAS_5GS_SERVICE_ACCEPT_5GS_ADDITIONAL_REQUEST_RESULT_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE:
            size = ogs_nas_5gs_decode_5gs_tracking_area_identity_list(&service_accept->forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_tracking_area_identity_list() failed");
               return size;
            }

            service_accept->presencemask |= OGS_NAS_5GS_SERVICE_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SERVICE_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE:
            size = ogs_nas_5gs_decode_5gs_tracking_area_identity_list(&service_accept->forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_tracking_area_identity_list() failed");
               return size;
            }

            service_accept->presencemask |= OGS_NAS_5GS_SERVICE_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_configuration_update_command(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_configuration_update_command_t *configuration_update_command = &message->gmm.configuration_update_command;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode CONFIGURATION_UPDATE_COMMAND\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CONFIGURATION_UPDATE_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_configuration_update_indication(&configuration_update_command->configuration_update_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_configuration_update_indication() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CONFIGURATION_UPDATE_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_5G_GUTI_TYPE:
            size = ogs_nas_5gs_decode_5gs_mobile_identity(&configuration_update_command->guti, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_mobile_identity() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_5G_GUTI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_TAI_LIST_TYPE:
            size = ogs_nas_5gs_decode_5gs_tracking_area_identity_list(&configuration_update_command->tai_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_tracking_area_identity_list() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_TAI_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_ALLOWED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_nssai(&configuration_update_command->allowed_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nssai() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_ALLOWED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SERVICE_AREA_LIST_TYPE:
            size = ogs_nas_5gs_decode_service_area_list(&configuration_update_command->service_area_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_service_area_list() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SERVICE_AREA_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_FULL_NAME_FOR_NETWORK_TYPE:
            size = ogs_nas_5gs_decode_network_name(&configuration_update_command->full_name_for_network, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_network_name() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_FULL_NAME_FOR_NETWORK_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SHORT_NAME_FOR_NETWORK_TYPE:
            size = ogs_nas_5gs_decode_network_name(&configuration_update_command->short_name_for_network, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_network_name() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SHORT_NAME_FOR_NETWORK_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LOCAL_TIME_ZONE_TYPE:
            size = ogs_nas_5gs_decode_time_zone(&configuration_update_command->local_time_zone, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_time_zone() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LOCAL_TIME_ZONE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_TYPE:
            size = ogs_nas_5gs_decode_time_zone_and_time(&configuration_update_command->universal_time_and_local_time_zone, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_time_zone_and_time() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NETWORK_DAYLIGHT_SAVING_TIME_TYPE:
            size = ogs_nas_5gs_decode_daylight_saving_time(&configuration_update_command->network_daylight_saving_time, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_daylight_saving_time() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LADN_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_ladn_information(&configuration_update_command->ladn_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ladn_information() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LADN_INFORMATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_MICO_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_mico_indication(&configuration_update_command->mico_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_mico_indication() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_MICO_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NETWORK_SLICING_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_network_slicing_indication(&configuration_update_command->network_slicing_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_network_slicing_indication() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NETWORK_SLICING_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CONFIGURED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_nssai(&configuration_update_command->configured_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nssai() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CONFIGURED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_REJECTED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_rejected_nssai(&configuration_update_command->rejected_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_rejected_nssai() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_REJECTED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_TYPE:
            size = ogs_nas_5gs_decode_operator_defined_access_category_definitions(&configuration_update_command->operator_defined_access_category_definitions, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_operator_defined_access_category_definitions() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SMS_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_sms_indication(&configuration_update_command->sms_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_sms_indication() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SMS_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_T3447_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&configuration_update_command->t3447_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_T3447_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CAG_INFORMATION_LIST_TYPE:
            size = ogs_nas_5gs_decode_cag_information_list(&configuration_update_command->cag_information_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_cag_information_list() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CAG_INFORMATION_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UE_RADIO_CAPABILITY_ID_TYPE:
            size = ogs_nas_5gs_decode_ue_radio_capability_id(&configuration_update_command->ue_radio_capability_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ue_radio_capability_id() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UE_RADIO_CAPABILITY_ID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_ue_radio_capability_id_deletion_indication(&configuration_update_command->ue_radio_capability_id_deletion_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ue_radio_capability_id_deletion_indication() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_5GS_REGISTRATION_RESULT_TYPE:
            size = ogs_nas_5gs_decode_5gs_registration_result(&configuration_update_command->registration_result, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_registration_result() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_5GS_REGISTRATION_RESULT_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_TRUNCATED_5G_S_TMSI_CONFIGURATION_TYPE:
            size = ogs_nas_5gs_decode_truncated_5g_s_tmsi_configuration(&configuration_update_command->truncated_s_tmsi_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_truncated_5g_s_tmsi_configuration() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_TRUNCATED_5G_S_TMSI_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_ADDITIONAL_CONFIGURATION_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_additional_configuration_indication(&configuration_update_command->additional_configuration_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_additional_configuration_indication() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_ADDITIONAL_CONFIGURATION_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_EXTENDED_REJECTED_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_extended_rejected_nssai(&configuration_update_command->extended_rejected_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_rejected_nssai() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_EXTENDED_REJECTED_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SERVICE_LEVEL_AA_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_service_level_aa_container(&configuration_update_command->service_level_aa_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_service_level_aa_container() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SERVICE_LEVEL_AA_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NSSRG_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_nssrg_information(&configuration_update_command->nssrg_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nssrg_information() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NSSRG_INFORMATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_DISASTER_ROAMING_WAIT_RANGE_TYPE:
            size = ogs_nas_5gs_decode_registration_wait_range(&configuration_update_command->disaster_roaming_wait_range, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_registration_wait_range() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_DISASTER_ROAMING_WAIT_RANGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_DISASTER_RETURN_WAIT_RANGE_TYPE:
            size = ogs_nas_5gs_decode_registration_wait_range(&configuration_update_command->disaster_return_wait_range, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_registration_wait_range() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_DISASTER_RETURN_WAIT_RANGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LIST_OF_PLMNS_TO_BE_USED_IN_DISASTER_CONDITION_TYPE:
            size = ogs_nas_5gs_decode_list_of_plmns_to_be_used_in_disaster_condition(&configuration_update_command->list_of_plmns_to_be_used_in_disaster_condition, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_list_of_plmns_to_be_used_in_disaster_condition() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LIST_OF_PLMNS_TO_BE_USED_IN_DISASTER_CONDITION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_EXTENDED_CAG_INFORMATION_LIST_TYPE:
            size = ogs_nas_5gs_decode_extended_cag_information_list(&configuration_update_command->extended_cag_information_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_cag_information_list() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_EXTENDED_CAG_INFORMATION_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UPDATED_PEIPS_ASSISTANCE_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_peips_assistance_information(&configuration_update_command->updated_peips_assistance_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_peips_assistance_information() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UPDATED_PEIPS_ASSISTANCE_INFORMATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NSAG_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_nsag_information(&configuration_update_command->nsag_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_nsag_information() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NSAG_INFORMATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_PRIORITY_INDICATOR_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_priority_indicator(&configuration_update_command->priority_indicator, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_priority_indicator() failed");
               return size;
            }

            configuration_update_command->presencemask |= OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_PRIORITY_INDICATOR_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_authentication_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_authentication_request_t *authentication_request = &message->gmm.authentication_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode AUTHENTICATION_REQUEST\n");

    size = ogs_nas_5gs_decode_key_set_identifier(&authentication_request->ngksi, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_key_set_identifier() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_5gs_decode_abba(&authentication_request->abba, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_abba() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_RAND_TYPE:
            size = ogs_nas_5gs_decode_authentication_parameter_rand(&authentication_request->authentication_parameter_rand, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_authentication_parameter_rand() failed");
               return size;
            }

            authentication_request->presencemask |= OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_RAND_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_AUTN_TYPE:
            size = ogs_nas_5gs_decode_authentication_parameter_autn(&authentication_request->authentication_parameter_autn, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_authentication_parameter_autn() failed");
               return size;
            }

            authentication_request->presencemask |= OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_AUTN_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_AUTHENTICATION_REQUEST_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&authentication_request->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            authentication_request->presencemask |= OGS_NAS_5GS_AUTHENTICATION_REQUEST_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_authentication_response(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_authentication_response_t *authentication_response = &message->gmm.authentication_response;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode AUTHENTICATION_RESPONSE\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_AUTHENTICATION_RESPONSE_AUTHENTICATION_RESPONSE_PARAMETER_TYPE:
            size = ogs_nas_5gs_decode_authentication_response_parameter(&authentication_response->authentication_response_parameter, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_authentication_response_parameter() failed");
               return size;
            }

            authentication_response->presencemask |= OGS_NAS_5GS_AUTHENTICATION_RESPONSE_AUTHENTICATION_RESPONSE_PARAMETER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_AUTHENTICATION_RESPONSE_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&authentication_response->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            authentication_response->presencemask |= OGS_NAS_5GS_AUTHENTICATION_RESPONSE_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_authentication_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_authentication_reject_t *authentication_reject = &message->gmm.authentication_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode AUTHENTICATION_REJECT\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_AUTHENTICATION_REJECT_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&authentication_reject->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            authentication_reject->presencemask |= OGS_NAS_5GS_AUTHENTICATION_REJECT_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_authentication_failure(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_authentication_failure_t *authentication_failure = &message->gmm.authentication_failure;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode AUTHENTICATION_FAILURE\n");

    size = ogs_nas_5gs_decode_5gmm_cause(&authentication_failure->gmm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gmm_cause() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_TYPE:
            size = ogs_nas_5gs_decode_authentication_failure_parameter(&authentication_failure->authentication_failure_parameter, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_authentication_failure_parameter() failed");
               return size;
            }

            authentication_failure->presencemask |= OGS_NAS_5GS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_authentication_result(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_authentication_result_t *authentication_result = &message->gmm.authentication_result;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode AUTHENTICATION_RESULT\n");

    size = ogs_nas_5gs_decode_key_set_identifier(&authentication_result->ngksi, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_key_set_identifier() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_5gs_decode_eap_message(&authentication_result->eap_message, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_eap_message() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_AUTHENTICATION_RESULT_ABBA_TYPE:
            size = ogs_nas_5gs_decode_abba(&authentication_result->abba, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_abba() failed");
               return size;
            }

            authentication_result->presencemask |= OGS_NAS_5GS_AUTHENTICATION_RESULT_ABBA_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_identity_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_identity_request_t *identity_request = &message->gmm.identity_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode IDENTITY_REQUEST\n");

    size = ogs_nas_5gs_decode_5gs_identity_type(&identity_request->identity_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gs_identity_type() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_5gs_decode_identity_response(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_identity_response_t *identity_response = &message->gmm.identity_response;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode IDENTITY_RESPONSE\n");

    size = ogs_nas_5gs_decode_5gs_mobile_identity(&identity_response->mobile_identity, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gs_mobile_identity() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_5gs_decode_security_mode_command(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_security_mode_command_t *security_mode_command = &message->gmm.security_mode_command;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode SECURITY_MODE_COMMAND\n");

    size = ogs_nas_5gs_decode_security_algorithms(&security_mode_command->selected_nas_security_algorithms, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_security_algorithms() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_5gs_decode_key_set_identifier(&security_mode_command->ngksi, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_key_set_identifier() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_5gs_decode_ue_security_capability(&security_mode_command->replayed_ue_security_capabilities, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_ue_security_capability() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_imeisv_request(&security_mode_command->imeisv_request, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_imeisv_request() failed");
               return size;
            }

            security_mode_command->presencemask |= OGS_NAS_5GS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SECURITY_MODE_COMMAND_SELECTED_EPS_NAS_SECURITY_ALGORITHMS_TYPE:
            size = ogs_nas_5gs_decode_eps_nas_security_algorithms(&security_mode_command->selected_eps_nas_security_algorithms, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eps_nas_security_algorithms() failed");
               return size;
            }

            security_mode_command->presencemask |= OGS_NAS_5GS_SECURITY_MODE_COMMAND_SELECTED_EPS_NAS_SECURITY_ALGORITHMS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_additional_5g_security_information(&security_mode_command->additional_security_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_additional_5g_security_information() failed");
               return size;
            }

            security_mode_command->presencemask |= OGS_NAS_5GS_SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SECURITY_MODE_COMMAND_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&security_mode_command->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            security_mode_command->presencemask |= OGS_NAS_5GS_SECURITY_MODE_COMMAND_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SECURITY_MODE_COMMAND_ABBA_TYPE:
            size = ogs_nas_5gs_decode_abba(&security_mode_command->abba, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_abba() failed");
               return size;
            }

            security_mode_command->presencemask |= OGS_NAS_5GS_SECURITY_MODE_COMMAND_ABBA_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SECURITY_MODE_COMMAND_REPLAYED_S1_UE_SECURITY_CAPABILITIES_TYPE:
            size = ogs_nas_5gs_decode_s1_ue_security_capability(&security_mode_command->replayed_s1_ue_security_capabilities, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_s1_ue_security_capability() failed");
               return size;
            }

            security_mode_command->presencemask |= OGS_NAS_5GS_SECURITY_MODE_COMMAND_REPLAYED_S1_UE_SECURITY_CAPABILITIES_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_security_mode_complete(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_security_mode_complete_t *security_mode_complete = &message->gmm.security_mode_complete;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode SECURITY_MODE_COMPLETE\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_SECURITY_MODE_COMPLETE_IMEISV_TYPE:
            size = ogs_nas_5gs_decode_5gs_mobile_identity(&security_mode_complete->imeisv, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_mobile_identity() failed");
               return size;
            }

            security_mode_complete->presencemask |= OGS_NAS_5GS_SECURITY_MODE_COMPLETE_IMEISV_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NAS_MESSAGE_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_message_container(&security_mode_complete->nas_message_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_message_container() failed");
               return size;
            }

            security_mode_complete->presencemask |= OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NAS_MESSAGE_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NON_IMEISV_PEI_TYPE:
            size = ogs_nas_5gs_decode_5gs_mobile_identity(&security_mode_complete->non_imeisv_pei, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gs_mobile_identity() failed");
               return size;
            }

            security_mode_complete->presencemask |= OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NON_IMEISV_PEI_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_security_mode_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_security_mode_reject_t *security_mode_reject = &message->gmm.security_mode_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode SECURITY_MODE_REJECT\n");

    size = ogs_nas_5gs_decode_5gmm_cause(&security_mode_reject->gmm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gmm_cause() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_5gs_decode_5gmm_status(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_5gmm_status_t *gmm_status = &message->gmm.gmm_status;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode 5GMM_STATUS\n");

    size = ogs_nas_5gs_decode_5gmm_cause(&gmm_status->gmm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gmm_cause() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_5gs_decode_notification(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_notification_t *notification = &message->gmm.notification;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode NOTIFICATION\n");

    size = ogs_nas_5gs_decode_access_type(&notification->access_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_access_type() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_5gs_decode_notification_response(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_notification_response_t *notification_response = &message->gmm.notification_response;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode NOTIFICATION_RESPONSE\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_NOTIFICATION_RESPONSE_PDU_SESSION_STATUS_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_status(&notification_response->pdu_session_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_status() failed");
               return size;
            }

            notification_response->presencemask |= OGS_NAS_5GS_NOTIFICATION_RESPONSE_PDU_SESSION_STATUS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_ul_nas_transport(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_ul_nas_transport_t *ul_nas_transport = &message->gmm.ul_nas_transport;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode UL_NAS_TRANSPORT\n");

    size = ogs_nas_5gs_decode_payload_container_type(&ul_nas_transport->payload_container_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_payload_container_type() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_5gs_decode_payload_container(&ul_nas_transport->payload_container, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_payload_container() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_UL_NAS_TRANSPORT_PDU_SESSION_ID_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_identity_2(&ul_nas_transport->pdu_session_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_identity_2() failed");
               return size;
            }

            ul_nas_transport->presencemask |= OGS_NAS_5GS_UL_NAS_TRANSPORT_PDU_SESSION_ID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_UL_NAS_TRANSPORT_OLD_PDU_SESSION_ID_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_identity_2(&ul_nas_transport->old_pdu_session_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_identity_2() failed");
               return size;
            }

            ul_nas_transport->presencemask |= OGS_NAS_5GS_UL_NAS_TRANSPORT_OLD_PDU_SESSION_ID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_UL_NAS_TRANSPORT_REQUEST_TYPE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_request_type(&ul_nas_transport->request_type, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_request_type() failed");
               return size;
            }

            ul_nas_transport->presencemask |= OGS_NAS_5GS_UL_NAS_TRANSPORT_REQUEST_TYPE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_UL_NAS_TRANSPORT_S_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_s_nssai(&ul_nas_transport->s_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_s_nssai() failed");
               return size;
            }

            ul_nas_transport->presencemask |= OGS_NAS_5GS_UL_NAS_TRANSPORT_S_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_UL_NAS_TRANSPORT_DNN_TYPE:
            size = ogs_nas_5gs_decode_dnn(&ul_nas_transport->dnn, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_dnn() failed");
               return size;
            }

            ul_nas_transport->presencemask |= OGS_NAS_5GS_UL_NAS_TRANSPORT_DNN_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_UL_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_additional_information(&ul_nas_transport->additional_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_additional_information() failed");
               return size;
            }

            ul_nas_transport->presencemask |= OGS_NAS_5GS_UL_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_UL_NAS_TRANSPORT_MA_PDU_SESSION_INFORMATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_ma_pdu_session_information(&ul_nas_transport->ma_pdu_session_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ma_pdu_session_information() failed");
               return size;
            }

            ul_nas_transport->presencemask |= OGS_NAS_5GS_UL_NAS_TRANSPORT_MA_PDU_SESSION_INFORMATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_UL_NAS_TRANSPORT_RELEASE_ASSISTANCE_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_release_assistance_indication(&ul_nas_transport->release_assistance_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_release_assistance_indication() failed");
               return size;
            }

            ul_nas_transport->presencemask |= OGS_NAS_5GS_UL_NAS_TRANSPORT_RELEASE_ASSISTANCE_INDICATION_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_dl_nas_transport(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_dl_nas_transport_t *dl_nas_transport = &message->gmm.dl_nas_transport;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode DL_NAS_TRANSPORT\n");

    size = ogs_nas_5gs_decode_payload_container_type(&dl_nas_transport->payload_container_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_payload_container_type() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_5gs_decode_payload_container(&dl_nas_transport->payload_container, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_payload_container() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_DL_NAS_TRANSPORT_PDU_SESSION_ID_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_identity_2(&dl_nas_transport->pdu_session_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_identity_2() failed");
               return size;
            }

            dl_nas_transport->presencemask |= OGS_NAS_5GS_DL_NAS_TRANSPORT_PDU_SESSION_ID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_DL_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE:
            size = ogs_nas_5gs_decode_additional_information(&dl_nas_transport->additional_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_additional_information() failed");
               return size;
            }

            dl_nas_transport->presencemask |= OGS_NAS_5GS_DL_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_DL_NAS_TRANSPORT_5GMM_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_5gmm_cause(&dl_nas_transport->gmm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gmm_cause() failed");
               return size;
            }

            dl_nas_transport->presencemask |= OGS_NAS_5GS_DL_NAS_TRANSPORT_5GMM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_DL_NAS_TRANSPORT_BACK_OFF_TIMER_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&dl_nas_transport->back_off_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            dl_nas_transport->presencemask |= OGS_NAS_5GS_DL_NAS_TRANSPORT_BACK_OFF_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_DL_NAS_TRANSPORT_LOWER_BOUND_TIMER_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&dl_nas_transport->lower_bound_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            dl_nas_transport->presencemask |= OGS_NAS_5GS_DL_NAS_TRANSPORT_LOWER_BOUND_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_establishment_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_establishment_request_t *pdu_session_establishment_request = &message->gsm.pdu_session_establishment_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_ESTABLISHMENT_REQUEST\n");

    size = ogs_nas_5gs_decode_integrity_protection_maximum_data_rate(&pdu_session_establishment_request->integrity_protection_maximum_data_rate, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_integrity_protection_maximum_data_rate() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_TYPE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_pdu_session_type(&pdu_session_establishment_request->pdu_session_type, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_type() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_TYPE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SSC_MODE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_ssc_mode(&pdu_session_establishment_request->ssc_mode, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ssc_mode() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SSC_MODE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_5GSM_CAPABILITY_TYPE:
            size = ogs_nas_5gs_decode_5gsm_capability(&pdu_session_establishment_request->gsm_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_capability() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_5GSM_CAPABILITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_TYPE:
            size = ogs_nas_5gs_decode_maximum_number_of_supported_packet_filters(&pdu_session_establishment_request->maximum_number_of_supported_packet_filters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_maximum_number_of_supported_packet_filters() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_always_on_pdu_session_requested(&pdu_session_establishment_request->always_on_pdu_session_requested, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_always_on_pdu_session_requested() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SM_PDU_DN_REQUEST_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_sm_pdu_dn_request_container(&pdu_session_establishment_request->sm_pdu_dn_request_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_sm_pdu_dn_request_container() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SM_PDU_DN_REQUEST_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_establishment_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_IP_HEADER_COMPRESSION_CONFIGURATION_TYPE:
            size = ogs_nas_5gs_decode_ip_header_compression_configuration(&pdu_session_establishment_request->ip_header_compression_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ip_header_compression_configuration() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_IP_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_DS_TT_ETHERNET_PORT_MAC_ADDRESS_TYPE:
            size = ogs_nas_5gs_decode_ds_tt_ethernet_port_mac_address(&pdu_session_establishment_request->ds_tt_ethernet_port_mac_address, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ds_tt_ethernet_port_mac_address() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_DS_TT_ETHERNET_PORT_MAC_ADDRESS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_UE_DS_TT_RESIDENCE_TIME_TYPE:
            size = ogs_nas_5gs_decode_ue_ds_tt_residence_time(&pdu_session_establishment_request->ue_ds_tt_residence_time, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ue_ds_tt_residence_time() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_UE_DS_TT_RESIDENCE_TIME_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_port_management_information_container(&pdu_session_establishment_request->port_management_information_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_port_management_information_container() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PORT_MANAGEMENT_INFORMATION_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_TYPE:
            size = ogs_nas_5gs_decode_ethernet_header_compression_configuration(&pdu_session_establishment_request->ethernet_header_compression_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ethernet_header_compression_configuration() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SUGGESTED_INTERFACE_IDENTIFIER_TYPE:
            size = ogs_nas_5gs_decode_pdu_address(&pdu_session_establishment_request->suggested_interface_identifier, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_address() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SUGGESTED_INTERFACE_IDENTIFIER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SERVICE_LEVEL_AA_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_service_level_aa_container(&pdu_session_establishment_request->service_level_aa_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_service_level_aa_container() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SERVICE_LEVEL_AA_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_REQUESTED_MBS_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_requested_mbs_container(&pdu_session_establishment_request->requested_mbs_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_requested_mbs_container() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_REQUESTED_MBS_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_PAIR_ID_TYPE:
            size = ogs_nas_5gs_decode_pdu_session_pair_id(&pdu_session_establishment_request->pdu_session_pair_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_pair_id() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_PAIR_ID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_RSN_TYPE:
            size = ogs_nas_5gs_decode_rsn(&pdu_session_establishment_request->rsn, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_rsn() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_RSN_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_establishment_accept(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_establishment_accept_t *pdu_session_establishment_accept = &message->gsm.pdu_session_establishment_accept;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_ESTABLISHMENT_ACCEPT\n");

    size = ogs_nas_5gs_decode_pdu_session_type(&pdu_session_establishment_accept->selected_pdu_session_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_pdu_session_type() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_5gs_decode_qos_rules(&pdu_session_establishment_accept->authorized_qos_rules, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_qos_rules() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_5gs_decode_session_ambr(&pdu_session_establishment_accept->session_ambr, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_session_ambr() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_establishment_accept->gsm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_PDU_ADDRESS_TYPE:
            size = ogs_nas_5gs_decode_pdu_address(&pdu_session_establishment_accept->pdu_address, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_address() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_PDU_ADDRESS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RQ_TIMER_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer(&pdu_session_establishment_accept->rq_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RQ_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_S_NSSAI_TYPE:
            size = ogs_nas_5gs_decode_s_nssai(&pdu_session_establishment_accept->s_nssai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_s_nssai() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_S_NSSAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ALWAYS_ON_PDU_SESSION_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_always_on_pdu_session_indication(&pdu_session_establishment_accept->always_on_pdu_session_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_always_on_pdu_session_indication() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ALWAYS_ON_PDU_SESSION_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_MAPPED_EPS_BEARER_CONTEXTS_TYPE:
            size = ogs_nas_5gs_decode_mapped_eps_bearer_contexts(&pdu_session_establishment_accept->mapped_eps_bearer_contexts, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_mapped_eps_bearer_contexts() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_MAPPED_EPS_BEARER_CONTEXTS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&pdu_session_establishment_accept->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_TYPE:
            size = ogs_nas_5gs_decode_qos_flow_descriptions(&pdu_session_establishment_accept->authorized_qos_flow_descriptions, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_qos_flow_descriptions() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_establishment_accept->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_DNN_TYPE:
            size = ogs_nas_5gs_decode_dnn(&pdu_session_establishment_accept->dnn, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_dnn() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_DNN_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_NETWORK_FEATURE_SUPPORT_TYPE:
            size = ogs_nas_5gs_decode_5gsm_network_feature_support(&pdu_session_establishment_accept->gsm_network_feature_support, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_network_feature_support() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_NETWORK_FEATURE_SUPPORT_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_SERVING_PLMN_RATE_CONTROL_TYPE:
            size = ogs_nas_5gs_decode_serving_plmn_rate_control(&pdu_session_establishment_accept->serving_plmn_rate_control, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_serving_plmn_rate_control() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_SERVING_PLMN_RATE_CONTROL_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ATSSS_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_atsss_container(&pdu_session_establishment_accept->atsss_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_atsss_container() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ATSSS_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_CONTROL_PLANE_ONLY_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_control_plane_only_indication(&pdu_session_establishment_accept->control_plane_only_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_control_plane_only_indication() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_CONTROL_PLANE_ONLY_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_IP_HEADER_COMPRESSION_CONFIGURATION_TYPE:
            size = ogs_nas_5gs_decode_ip_header_compression_configuration(&pdu_session_establishment_accept->ip_header_compression_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ip_header_compression_configuration() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_IP_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_TYPE:
            size = ogs_nas_5gs_decode_ethernet_header_compression_configuration(&pdu_session_establishment_accept->ethernet_header_compression_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ethernet_header_compression_configuration() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_SERVICE_LEVEL_AA_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_service_level_aa_container(&pdu_session_establishment_accept->service_level_aa_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_service_level_aa_container() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_SERVICE_LEVEL_AA_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RECEIVED_MBS_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_received_mbs_container(&pdu_session_establishment_accept->received_mbs_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_received_mbs_container() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RECEIVED_MBS_CONTAINER_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_establishment_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_establishment_reject_t *pdu_session_establishment_reject = &message->gsm.pdu_session_establishment_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_ESTABLISHMENT_REJECT\n");

    size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_establishment_reject->gsm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_BACK_OFF_TIMER_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&pdu_session_establishment_reject->back_off_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            pdu_session_establishment_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_BACK_OFF_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_allowed_ssc_mode(&pdu_session_establishment_reject->allowed_ssc_mode, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_allowed_ssc_mode() failed");
               return size;
            }

            pdu_session_establishment_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&pdu_session_establishment_reject->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            pdu_session_establishment_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_TYPE:
            size = ogs_nas_5gs_decode_5gsm_congestion_re_attempt_indicator(&pdu_session_establishment_reject->gsm_congestion_re_attempt_indicator, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_congestion_re_attempt_indicator() failed");
               return size;
            }

            pdu_session_establishment_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_establishment_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_establishment_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_RE_ATTEMPT_INDICATOR_TYPE:
            size = ogs_nas_5gs_decode_re_attempt_indicator(&pdu_session_establishment_reject->re_attempt_indicator, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_re_attempt_indicator() failed");
               return size;
            }

            pdu_session_establishment_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_RE_ATTEMPT_INDICATOR_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_SERVICE_LEVEL_AA_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_service_level_aa_container(&pdu_session_establishment_reject->service_level_aa_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_service_level_aa_container() failed");
               return size;
            }

            pdu_session_establishment_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_SERVICE_LEVEL_AA_CONTAINER_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_authentication_command(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_authentication_command_t *pdu_session_authentication_command = &message->gsm.pdu_session_authentication_command;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_AUTHENTICATION_COMMAND\n");

    size = ogs_nas_5gs_decode_eap_message(&pdu_session_authentication_command->eap_message, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_eap_message() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_authentication_command->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_authentication_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_authentication_complete(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_authentication_complete_t *pdu_session_authentication_complete = &message->gsm.pdu_session_authentication_complete;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_AUTHENTICATION_COMPLETE\n");

    size = ogs_nas_5gs_decode_eap_message(&pdu_session_authentication_complete->eap_message, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_eap_message() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_authentication_complete->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_authentication_complete->presencemask |= OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_authentication_result(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_authentication_result_t *pdu_session_authentication_result = &message->gsm.pdu_session_authentication_result;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_AUTHENTICATION_RESULT\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&pdu_session_authentication_result->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            pdu_session_authentication_result->presencemask |= OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_authentication_result->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_authentication_result->presencemask |= OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_modification_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_modification_request_t *pdu_session_modification_request = &message->gsm.pdu_session_modification_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_MODIFICATION_REQUEST\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAPABILITY_TYPE:
            size = ogs_nas_5gs_decode_5gsm_capability(&pdu_session_modification_request->gsm_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_capability() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAPABILITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_modification_request->gsm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_TYPE:
            size = ogs_nas_5gs_decode_maximum_number_of_supported_packet_filters(&pdu_session_modification_request->maximum_number_of_supported_packet_filters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_maximum_number_of_supported_packet_filters() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_always_on_pdu_session_requested(&pdu_session_modification_request->always_on_pdu_session_requested, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_always_on_pdu_session_requested() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_TYPE:
            size = ogs_nas_5gs_decode_integrity_protection_maximum_data_rate(&pdu_session_modification_request->integrity_protection_maximum_data_rate, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_integrity_protection_maximum_data_rate() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_TYPE:
            size = ogs_nas_5gs_decode_qos_rules(&pdu_session_modification_request->requested_qos_rules, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_qos_rules() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_TYPE:
            size = ogs_nas_5gs_decode_qos_flow_descriptions(&pdu_session_modification_request->requested_qos_flow_descriptions, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_qos_flow_descriptions() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAPPED_EPS_BEARER_CONTEXTS_TYPE:
            size = ogs_nas_5gs_decode_mapped_eps_bearer_contexts(&pdu_session_modification_request->mapped_eps_bearer_contexts, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_mapped_eps_bearer_contexts() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAPPED_EPS_BEARER_CONTEXTS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_modification_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_port_management_information_container(&pdu_session_modification_request->port_management_information_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_port_management_information_container() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_PORT_MANAGEMENT_INFORMATION_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_IP_HEADER_COMPRESSION_CONFIGURATION_TYPE:
            size = ogs_nas_5gs_decode_header_compression_configuration(&pdu_session_modification_request->ip_header_compression_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_header_compression_configuration() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_IP_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_TYPE:
            size = ogs_nas_5gs_decode_ethernet_header_compression_configuration(&pdu_session_modification_request->ethernet_header_compression_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ethernet_header_compression_configuration() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_MBS_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_requested_mbs_container(&pdu_session_modification_request->requested_mbs_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_requested_mbs_container() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_MBS_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_SERVICE_LEVEL_AA_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_service_level_aa_container(&pdu_session_modification_request->service_level_aa_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_service_level_aa_container() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_SERVICE_LEVEL_AA_CONTAINER_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_modification_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_modification_reject_t *pdu_session_modification_reject = &message->gsm.pdu_session_modification_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_MODIFICATION_REJECT\n");

    size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_modification_reject->gsm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&pdu_session_modification_reject->back_off_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            pdu_session_modification_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_TYPE:
            size = ogs_nas_5gs_decode_5gsm_congestion_re_attempt_indicator(&pdu_session_modification_reject->gsm_congestion_re_attempt_indicator, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_congestion_re_attempt_indicator() failed");
               return size;
            }

            pdu_session_modification_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_modification_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_modification_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE:
            size = ogs_nas_5gs_decode_re_attempt_indicator(&pdu_session_modification_reject->re_attempt_indicator, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_re_attempt_indicator() failed");
               return size;
            }

            pdu_session_modification_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_modification_command(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_modification_command_t *pdu_session_modification_command = &message->gsm.pdu_session_modification_command;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_MODIFICATION_COMMAND\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_5GSM_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_modification_command->gsm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_5GSM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_TYPE:
            size = ogs_nas_5gs_decode_session_ambr(&pdu_session_modification_command->session_ambr, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_session_ambr() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RQ_TIMER_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer(&pdu_session_modification_command->rq_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RQ_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ALWAYS_ON_PDU_SESSION_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_always_on_pdu_session_indication(&pdu_session_modification_command->always_on_pdu_session_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_always_on_pdu_session_indication() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ALWAYS_ON_PDU_SESSION_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_RULES_TYPE:
            size = ogs_nas_5gs_decode_qos_rules(&pdu_session_modification_command->authorized_qos_rules, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_qos_rules() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_RULES_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_TYPE:
            size = ogs_nas_5gs_decode_mapped_eps_bearer_contexts(&pdu_session_modification_command->mapped_eps_bearer_contexts, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_mapped_eps_bearer_contexts() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_TYPE:
            size = ogs_nas_5gs_decode_qos_flow_descriptions(&pdu_session_modification_command->authorized_qos_flow_descriptions, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_qos_flow_descriptions() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_modification_command->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ATSSS_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_atsss_container(&pdu_session_modification_command->atsss_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_atsss_container() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ATSSS_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_IP_HEADER_COMPRESSION_CONFIGURATION_TYPE:
            size = ogs_nas_5gs_decode_ip_header_compression_configuration(&pdu_session_modification_command->ip_header_compression_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ip_header_compression_configuration() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_IP_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_port_management_information_container(&pdu_session_modification_command->port_management_information_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_port_management_information_container() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_PORT_MANAGEMENT_INFORMATION_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SERVING_PLMN_RATE_CONTROL_TYPE:
            size = ogs_nas_5gs_decode_serving_plmn_rate_control(&pdu_session_modification_command->serving_plmn_rate_control, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_serving_plmn_rate_control() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SERVING_PLMN_RATE_CONTROL_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_TYPE:
            size = ogs_nas_5gs_decode_ethernet_header_compression_configuration(&pdu_session_modification_command->ethernet_header_compression_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ethernet_header_compression_configuration() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RECEIVED_MBS_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_received_mbs_container(&pdu_session_modification_command->received_mbs_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_received_mbs_container() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RECEIVED_MBS_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SERVICE_LEVEL_AA_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_service_level_aa_container(&pdu_session_modification_command->service_level_aa_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_service_level_aa_container() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SERVICE_LEVEL_AA_CONTAINER_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_modification_complete(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_modification_complete_t *pdu_session_modification_complete = &message->gsm.pdu_session_modification_complete;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_MODIFICATION_COMPLETE\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_modification_complete->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_modification_complete->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_port_management_information_container(&pdu_session_modification_complete->port_management_information_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_port_management_information_container() failed");
               return size;
            }

            pdu_session_modification_complete->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_PORT_MANAGEMENT_INFORMATION_CONTAINER_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_modification_command_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_modification_command_reject_t *pdu_session_modification_command_reject = &message->gsm.pdu_session_modification_command_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_MODIFICATION_COMMAND_REJECT\n");

    size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_modification_command_reject->gsm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_modification_command_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_modification_command_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_release_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_release_request_t *pdu_session_release_request = &message->gsm.pdu_session_release_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_RELEASE_REQUEST\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_5GSM_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_release_request->gsm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
               return size;
            }

            pdu_session_release_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_5GSM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_release_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_release_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_release_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_release_reject_t *pdu_session_release_reject = &message->gsm.pdu_session_release_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_RELEASE_REJECT\n");

    size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_release_reject->gsm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_release_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_release_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_release_command(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_release_command_t *pdu_session_release_command = &message->gsm.pdu_session_release_command;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_RELEASE_COMMAND\n");

    size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_release_command->gsm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
        return size;
    }

    decoded += size;

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_BACK_OFF_TIMER_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer_3(&pdu_session_release_command->back_off_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer_3() failed");
               return size;
            }

            pdu_session_release_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_BACK_OFF_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&pdu_session_release_command->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            pdu_session_release_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_TYPE:
            size = ogs_nas_5gs_decode_5gsm_congestion_re_attempt_indicator(&pdu_session_release_command->gsm_congestion_re_attempt_indicator, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_congestion_re_attempt_indicator() failed");
               return size;
            }

            pdu_session_release_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_release_command->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_release_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_ACCESS_TYPE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_access_type(&pdu_session_release_command->access_type, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_access_type() failed");
               return size;
            }

            pdu_session_release_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_ACCESS_TYPE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_SERVICE_LEVEL_AA_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_service_level_aa_container(&pdu_session_release_command->service_level_aa_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_service_level_aa_container() failed");
               return size;
            }

            pdu_session_release_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_SERVICE_LEVEL_AA_CONTAINER_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_pdu_session_release_complete(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_release_complete_t *pdu_session_release_complete = &message->gsm.pdu_session_release_complete;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_RELEASE_COMPLETE\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_5GSM_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_release_complete->gsm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
               return size;
            }

            pdu_session_release_complete->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_5GSM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_release_complete->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_release_complete->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_5gs_decode_5gsm_status(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_5gsm_status_t *gsm_status = &message->gsm.gsm_status;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode 5GSM_STATUS\n");

    size = ogs_nas_5gs_decode_5gsm_cause(&gsm_status->gsm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_5gmm_decode(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    int decoded = 0;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);

    size = sizeof(ogs_nas_5gmm_header_t);
    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return OGS_ERROR;
    }

    memset(message, 0, sizeof(ogs_nas_5gs_message_t));
    memcpy(&message->gmm.h, pkbuf->data - size, size);
    decoded += size;

    switch(message->gmm.h.message_type) {
    case OGS_NAS_5GS_REGISTRATION_REQUEST:
        size = ogs_nas_5gs_decode_registration_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_registration_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_REGISTRATION_ACCEPT:
        size = ogs_nas_5gs_decode_registration_accept(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_registration_accept() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_REGISTRATION_COMPLETE:
        size = ogs_nas_5gs_decode_registration_complete(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_registration_complete() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_REGISTRATION_REJECT:
        size = ogs_nas_5gs_decode_registration_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_registration_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_DEREGISTRATION_REQUEST_FROM_UE:
        size = ogs_nas_5gs_decode_deregistration_request_from_ue(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_deregistration_request_from_ue() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_DEREGISTRATION_ACCEPT_FROM_UE:
        break;
    case OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE:
        size = ogs_nas_5gs_decode_deregistration_request_to_ue(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_deregistration_request_to_ue() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_DEREGISTRATION_ACCEPT_TO_UE:
        break;
    case OGS_NAS_5GS_SERVICE_REQUEST:
        size = ogs_nas_5gs_decode_service_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_service_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_SERVICE_REJECT:
        size = ogs_nas_5gs_decode_service_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_service_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_SERVICE_ACCEPT:
        size = ogs_nas_5gs_decode_service_accept(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_service_accept() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND:
        size = ogs_nas_5gs_decode_configuration_update_command(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_configuration_update_command() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMPLETE:
        break;
    case OGS_NAS_5GS_AUTHENTICATION_REQUEST:
        size = ogs_nas_5gs_decode_authentication_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_authentication_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_AUTHENTICATION_RESPONSE:
        size = ogs_nas_5gs_decode_authentication_response(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_authentication_response() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_AUTHENTICATION_REJECT:
        size = ogs_nas_5gs_decode_authentication_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_authentication_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_AUTHENTICATION_FAILURE:
        size = ogs_nas_5gs_decode_authentication_failure(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_authentication_failure() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_AUTHENTICATION_RESULT:
        size = ogs_nas_5gs_decode_authentication_result(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_authentication_result() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_IDENTITY_REQUEST:
        size = ogs_nas_5gs_decode_identity_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_identity_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_IDENTITY_RESPONSE:
        size = ogs_nas_5gs_decode_identity_response(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_identity_response() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_SECURITY_MODE_COMMAND:
        size = ogs_nas_5gs_decode_security_mode_command(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_security_mode_command() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_SECURITY_MODE_COMPLETE:
        size = ogs_nas_5gs_decode_security_mode_complete(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_security_mode_complete() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_SECURITY_MODE_REJECT:
        size = ogs_nas_5gs_decode_security_mode_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_security_mode_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_5GMM_STATUS:
        size = ogs_nas_5gs_decode_5gmm_status(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_5gmm_status() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_NOTIFICATION:
        size = ogs_nas_5gs_decode_notification(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_notification() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_NOTIFICATION_RESPONSE:
        size = ogs_nas_5gs_decode_notification_response(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_notification_response() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_UL_NAS_TRANSPORT:
        size = ogs_nas_5gs_decode_ul_nas_transport(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_ul_nas_transport() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_DL_NAS_TRANSPORT:
        size = ogs_nas_5gs_decode_dl_nas_transport(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_dl_nas_transport() failed");
           return size;
        }

        decoded += size;
        break;
    default:
        ogs_error("Unknown message type (0x%x) or not implemented", 
                message->gmm.h.message_type);
        break;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, decoded));

    return OGS_OK;
}
int ogs_nas_5gsm_decode(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    int decoded = 0;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);

    size = sizeof(ogs_nas_5gsm_header_t);
    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return OGS_ERROR;
    }

    memset(message, 0, sizeof(ogs_nas_5gs_message_t));
    memcpy(&message->gsm.h, pkbuf->data - size, size);
    decoded += size;

    switch(message->gsm.h.message_type) {
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST:
        size = ogs_nas_5gs_decode_pdu_session_establishment_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_establishment_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT:
        size = ogs_nas_5gs_decode_pdu_session_establishment_accept(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_establishment_accept() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT:
        size = ogs_nas_5gs_decode_pdu_session_establishment_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_establishment_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMMAND:
        size = ogs_nas_5gs_decode_pdu_session_authentication_command(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_authentication_command() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMPLETE:
        size = ogs_nas_5gs_decode_pdu_session_authentication_complete(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_authentication_complete() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT:
        size = ogs_nas_5gs_decode_pdu_session_authentication_result(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_authentication_result() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST:
        size = ogs_nas_5gs_decode_pdu_session_modification_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_modification_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT:
        size = ogs_nas_5gs_decode_pdu_session_modification_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_modification_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND:
        size = ogs_nas_5gs_decode_pdu_session_modification_command(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_modification_command() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE:
        size = ogs_nas_5gs_decode_pdu_session_modification_complete(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_modification_complete() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT:
        size = ogs_nas_5gs_decode_pdu_session_modification_command_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_modification_command_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST:
        size = ogs_nas_5gs_decode_pdu_session_release_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_release_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT:
        size = ogs_nas_5gs_decode_pdu_session_release_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_release_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND:
        size = ogs_nas_5gs_decode_pdu_session_release_command(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_release_command() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
        size = ogs_nas_5gs_decode_pdu_session_release_complete(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdu_session_release_complete() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_5GSM_STATUS:
        size = ogs_nas_5gs_decode_5gsm_status(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_5gsm_status() failed");
           return size;
        }

        decoded += size;
        break;
    default:
        ogs_error("Unknown message type (0x%x) or not implemented", 
                message->gsm.h.message_type);
        break;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, decoded));

    return OGS_OK;
}

