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
 * This file had been created by nas-message.py script v0.1.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2022-01-22 09:24:28.380730 by acetcom
 * from 24301-g40.docx
 ******************************************************************************/

#include "ogs-nas-eps.h"

int ogs_nas_eps_decode_attach_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_attach_accept(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_attach_complete(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_attach_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_detach_request_from_ue(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_detach_request_to_ue(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_tracking_area_update_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_tracking_area_update_accept(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_tracking_area_update_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_extended_service_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_service_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_service_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_guti_reallocation_command(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_authentication_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_authentication_response(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_identity_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_identity_response(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_authentication_failure(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_security_mode_command(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_security_mode_complete(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_security_mode_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_emm_status(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_emm_information(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_downlink_nas_transport(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_uplink_nas_transport(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_cs_service_notification(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_uplink_generic_nas_transport(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_downlink_generic_nas_transport(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_activate_default_eps_bearer_context_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_activate_default_eps_bearer_context_accept(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_activate_default_eps_bearer_context_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_activate_dedicated_eps_bearer_context_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_activate_dedicated_eps_bearer_context_accept(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_activate_dedicated_eps_bearer_context_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_modify_eps_bearer_context_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_modify_eps_bearer_context_accept(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_modify_eps_bearer_context_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_deactivate_eps_bearer_context_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_deactivate_eps_bearer_context_accept(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_pdn_connectivity_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_pdn_connectivity_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_pdn_disconnect_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_pdn_disconnect_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_bearer_resource_allocation_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_bearer_resource_allocation_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_bearer_resource_modification_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_bearer_resource_modification_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_esm_information_response(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_esm_status(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_eps_decode_attach_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_attach_request_t *attach_request = &message->emm.attach_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode ATTACH_REQUEST\n");

    size = ogs_nas_eps_decode_eps_attach_type(&attach_request->eps_attach_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_eps_attach_type() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_eps_mobile_identity(&attach_request->eps_mobile_identity, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_eps_mobile_identity() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_ue_network_capability(&attach_request->ue_network_capability, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_ue_network_capability() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_esm_message_container(&attach_request->esm_message_container, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_esm_message_container() failed");
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

        switch (type) {
        case OGS_NAS_EPS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE:
            size = ogs_nas_eps_decode_p_tmsi_signature(&attach_request->old_p_tmsi_signature, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_p_tmsi_signature() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_GUTI_TYPE:
            size = ogs_nas_eps_decode_eps_mobile_identity(&attach_request->additional_guti, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_eps_mobile_identity() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_GUTI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE:
            size = ogs_nas_eps_decode_tracking_area_identity(&attach_request->last_visited_registered_tai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_tracking_area_identity() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_DRX_PARAMETER_TYPE:
            size = ogs_nas_eps_decode_drx_parameter(&attach_request->drx_parameter, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_drx_parameter() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_DRX_PARAMETER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_TYPE:
            size = ogs_nas_eps_decode_ms_network_capability(&attach_request->ms_network_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ms_network_capability() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_TYPE:
            size = ogs_nas_eps_decode_location_area_identification(&attach_request->old_location_area_identification, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_location_area_identification() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_TMSI_STATUS_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_tmsi_status(&attach_request->tmsi_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_tmsi_status() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_TMSI_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE:
            size = ogs_nas_eps_decode_mobile_station_classmark_2(&attach_request->mobile_station_classmark_2, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_mobile_station_classmark_2() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_TYPE:
            size = ogs_nas_eps_decode_mobile_station_classmark_3(&attach_request->mobile_station_classmark_3, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_mobile_station_classmark_3() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_SUPPORTED_CODECS_TYPE:
            size = ogs_nas_eps_decode_supported_codec_list(&attach_request->supported_codecs, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_supported_codec_list() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_SUPPORTED_CODECS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_additional_update_type(&attach_request->additional_update_type, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_additional_update_type() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_TYPE:
            size = ogs_nas_eps_decode_voice_domain_preference_and_ue_usage_setting(&attach_request->voice_domain_preference_and_ue_usage_setting, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_voice_domain_preference_and_ue_usage_setting() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_DEVICE_PROPERTIES_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_device_properties(&attach_request->device_properties, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_device_properties() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_DEVICE_PROPERTIES_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_OLD_GUTI_TYPE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_guti_type(&attach_request->old_guti_type, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_guti_type() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_OLD_GUTI_TYPE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_ms_network_feature_support(&attach_request->ms_network_feature_support, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ms_network_feature_support() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_network_resource_identifier_container(&attach_request->tmsi_based_nri_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_network_resource_identifier_container() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_T3324_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_2(&attach_request->t3324_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_2() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_T3324_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_3(&attach_request->t3412_extended_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_3() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_TYPE:
            size = ogs_nas_eps_decode_extended_drx_parameters(&attach_request->extended_drx_parameters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_drx_parameters() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_TYPE:
            size = ogs_nas_eps_decode_ue_additional_security_capability(&attach_request->ue_additional_security_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_additional_security_capability() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_UE_STATUS_TYPE:
            size = ogs_nas_eps_decode_ue_status(&attach_request->ue_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_status() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_UE_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_TYPE:
            size = ogs_nas_eps_decode_additional_information_requested(&attach_request->additional_information_requested, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_additional_information_requested() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REQUEST_N1_UE_NETWORK_CAPABILITY_TYPE:
            size = ogs_nas_eps_decode_n1_ue_network_capability(&attach_request->n1_ue_network_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_n1_ue_network_capability() failed");
               return size;
            }

            attach_request->presencemask |= OGS_NAS_EPS_ATTACH_REQUEST_N1_UE_NETWORK_CAPABILITY_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_attach_accept(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_attach_accept_t *attach_accept = &message->emm.attach_accept;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode ATTACH_ACCEPT\n");

    size = ogs_nas_eps_decode_eps_attach_result(&attach_accept->eps_attach_result, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_eps_attach_result() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_gprs_timer(&attach_accept->t3412_value, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_gprs_timer() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_tracking_area_identity_list(&attach_accept->tai_list, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_tracking_area_identity_list() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_esm_message_container(&attach_accept->esm_message_container, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_esm_message_container() failed");
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

        switch (type) {
        case OGS_NAS_EPS_ATTACH_ACCEPT_GUTI_TYPE:
            size = ogs_nas_eps_decode_eps_mobile_identity(&attach_accept->guti, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_eps_mobile_identity() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_GUTI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE:
            size = ogs_nas_eps_decode_location_area_identification(&attach_accept->location_area_identification, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_location_area_identification() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_MS_IDENTITY_TYPE:
            size = ogs_nas_eps_decode_mobile_identity(&attach_accept->ms_identity, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_mobile_identity() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_MS_IDENTITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_EMM_CAUSE_TYPE:
            size = ogs_nas_eps_decode_emm_cause(&attach_accept->emm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_emm_cause() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_EMM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_T3402_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer(&attach_accept->t3402_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_T3402_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_T3423_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer(&attach_accept->t3423_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_T3423_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_TYPE:
            size = ogs_nas_eps_decode_plmn_list(&attach_accept->equivalent_plmns, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_plmn_list() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE:
            size = ogs_nas_eps_decode_emergency_number_list(&attach_accept->emergency_number_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_emergency_number_list() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE:
            size = ogs_nas_eps_decode_eps_network_feature_support(&attach_accept->eps_network_feature_support, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_eps_network_feature_support() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_additional_update_result(&attach_accept->additional_update_result, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_additional_update_result() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_3(&attach_accept->t3412_extended_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_3() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_T3324_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_2(&attach_accept->t3324_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_2() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_T3324_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE:
            size = ogs_nas_eps_decode_extended_drx_parameters(&attach_accept->extended_drx_parameters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_drx_parameters() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_DCN_ID_TYPE:
            size = ogs_nas_eps_decode_dcn_id(&attach_accept->dcn_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_dcn_id() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_DCN_ID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_SMS_SERVICES_STATUS_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_sms_services_status(&attach_accept->sms_services_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_sms_services_status() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_SMS_SERVICES_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_NON__NW_PROVIDED_POLICIES_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_non__nw_provided_policies(&attach_accept->non__nw_provided_policies, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_non__nw_provided_policies() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_NON__NW_PROVIDED_POLICIES_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_T3448_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_2(&attach_accept->t3448_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_2() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_T3448_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_NETWORK_POLICY_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_network_policy(&attach_accept->network_policy, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_network_policy() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_NETWORK_POLICY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_T3447_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_3(&attach_accept->t3447_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_3() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_T3447_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_TYPE:
            size = ogs_nas_eps_decode_extended_emergency_number_list(&attach_accept->extended_emergency_number_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_emergency_number_list() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_CIPHERING_KEY_DATA_TYPE:
            size = ogs_nas_eps_decode_ciphering_key_data(&attach_accept->ciphering_key_data, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ciphering_key_data() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_CIPHERING_KEY_DATA_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_UE_RADIO_CAPABILITY_ID_TYPE:
            size = ogs_nas_eps_decode_ue_radio_capability_id(&attach_accept->ue_radio_capability_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_radio_capability_id() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_UE_RADIO_CAPABILITY_ID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_ue_radio_capability_id_deletion_indication(&attach_accept->ue_radio_capability_id_deletion_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_radio_capability_id_deletion_indication() failed");
               return size;
            }

            attach_accept->presencemask |= OGS_NAS_EPS_ATTACH_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_attach_complete(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_attach_complete_t *attach_complete = &message->emm.attach_complete;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode ATTACH_COMPLETE\n");

    size = ogs_nas_eps_decode_esm_message_container(&attach_complete->esm_message_container, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_esm_message_container() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_eps_decode_attach_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_attach_reject_t *attach_reject = &message->emm.attach_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode ATTACH_REJECT\n");

    size = ogs_nas_eps_decode_emm_cause(&attach_reject->emm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_emm_cause() failed");
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

        switch (type) {
        case OGS_NAS_EPS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_esm_message_container(&attach_reject->esm_message_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_esm_message_container() failed");
               return size;
            }

            attach_reject->presencemask |= OGS_NAS_EPS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REJECT_T3346_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_2(&attach_reject->t3346_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_2() failed");
               return size;
            }

            attach_reject->presencemask |= OGS_NAS_EPS_ATTACH_REJECT_T3346_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REJECT_T3402_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_2(&attach_reject->t3402_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_2() failed");
               return size;
            }

            attach_reject->presencemask |= OGS_NAS_EPS_ATTACH_REJECT_T3402_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_extended_emm_cause(&attach_reject->extended_emm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_emm_cause() failed");
               return size;
            }

            attach_reject->presencemask |= OGS_NAS_EPS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_detach_request_from_ue(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_detach_request_from_ue_t *detach_request_from_ue = &message->emm.detach_request_from_ue;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode DETACH_REQUEST\n");

    size = ogs_nas_eps_decode_detach_type(&detach_request_from_ue->detach_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_detach_type() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_eps_mobile_identity(&detach_request_from_ue->eps_mobile_identity, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_eps_mobile_identity() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_eps_decode_detach_request_to_ue(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_detach_request_to_ue_t *detach_request_to_ue = &message->emm.detach_request_to_ue;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode DETACH_REQUEST\n");

    size = ogs_nas_eps_decode_detach_type(&detach_request_to_ue->detach_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_detach_type() failed");
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

        switch (type) {
        case OGS_NAS_EPS_DETACH_REQUEST_EMM_CAUSE_TYPE:
            size = ogs_nas_eps_decode_emm_cause(&detach_request_to_ue->emm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_emm_cause() failed");
               return size;
            }

            detach_request_to_ue->presencemask |= OGS_NAS_EPS_DETACH_REQUEST_EMM_CAUSE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_tracking_area_update_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_tracking_area_update_request_t *tracking_area_update_request = &message->emm.tracking_area_update_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode TRACKING_AREA_UPDATE_REQUEST\n");

    size = ogs_nas_eps_decode_eps_update_type(&tracking_area_update_request->eps_update_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_eps_update_type() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_eps_mobile_identity(&tracking_area_update_request->old_guti, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_eps_mobile_identity() failed");
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

        switch (type) {
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_key_set_identifier(&tracking_area_update_request->non_current_native_nas_key_set_identifier, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_key_set_identifier() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_GPRS_CIPHERING_KEY_SEQUENCE_NUMBER_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_ciphering_key_sequence_number(&tracking_area_update_request->gprs_ciphering_key_sequence_number, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ciphering_key_sequence_number() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_GPRS_CIPHERING_KEY_SEQUENCE_NUMBER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE:
            size = ogs_nas_eps_decode_p_tmsi_signature(&tracking_area_update_request->old_p_tmsi_signature, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_p_tmsi_signature() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_P_TMSI_SIGNATURE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_GUTI_TYPE:
            size = ogs_nas_eps_decode_eps_mobile_identity(&tracking_area_update_request->additional_guti, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_eps_mobile_identity() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_GUTI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_TYPE:
            size = ogs_nas_eps_decode_nonce(&tracking_area_update_request->nonceue, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nonce() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_TYPE:
            size = ogs_nas_eps_decode_ue_network_capability(&tracking_area_update_request->ue_network_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_network_capability() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE:
            size = ogs_nas_eps_decode_tracking_area_identity(&tracking_area_update_request->last_visited_registered_tai, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_tracking_area_identity() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_TYPE:
            size = ogs_nas_eps_decode_drx_parameter(&tracking_area_update_request->drx_parameter, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_drx_parameter() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_ue_radio_capability_information_update_needed(&tracking_area_update_request->ue_radio_capability_information_update_needed, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_radio_capability_information_update_needed() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE:
            size = ogs_nas_eps_decode_eps_bearer_context_status(&tracking_area_update_request->eps_bearer_context_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_eps_bearer_context_status() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_TYPE:
            size = ogs_nas_eps_decode_ms_network_capability(&tracking_area_update_request->ms_network_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ms_network_capability() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_TYPE:
            size = ogs_nas_eps_decode_location_area_identification(&tracking_area_update_request->old_location_area_identification, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_location_area_identification() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_STATUS_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_tmsi_status(&tracking_area_update_request->tmsi_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_tmsi_status() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE:
            size = ogs_nas_eps_decode_mobile_station_classmark_2(&tracking_area_update_request->mobile_station_classmark_2, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_mobile_station_classmark_2() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_3_TYPE:
            size = ogs_nas_eps_decode_mobile_station_classmark_3(&tracking_area_update_request->mobile_station_classmark_3, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_mobile_station_classmark_3() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_SUPPORTED_CODECS_TYPE:
            size = ogs_nas_eps_decode_supported_codec_list(&tracking_area_update_request->supported_codecs, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_supported_codec_list() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_SUPPORTED_CODECS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_UPDATE_TYPE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_additional_update_type(&tracking_area_update_request->additional_update_type, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_additional_update_type() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_TYPE:
            size = ogs_nas_eps_decode_voice_domain_preference_and_ue_usage_setting(&tracking_area_update_request->voice_domain_preference_and_ue_usage_setting, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_voice_domain_preference_and_ue_usage_setting() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_guti_type(&tracking_area_update_request->old_guti_type, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_guti_type() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DEVICE_PROPERTIES_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_device_properties(&tracking_area_update_request->device_properties, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_device_properties() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DEVICE_PROPERTIES_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_FEATURE_SUPPORT_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_ms_network_feature_support(&tracking_area_update_request->ms_network_feature_support, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ms_network_feature_support() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_BASED_NRI_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_network_resource_identifier_container(&tracking_area_update_request->tmsi_based_nri_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_network_resource_identifier_container() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_T3324_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_2(&tracking_area_update_request->t3324_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_2() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_T3324_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_T3412_EXTENDED_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_3(&tracking_area_update_request->t3412_extended_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_3() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_T3412_EXTENDED_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_EXTENDED_DRX_PARAMETERS_TYPE:
            size = ogs_nas_eps_decode_extended_drx_parameters(&tracking_area_update_request->extended_drx_parameters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_drx_parameters() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_EXTENDED_DRX_PARAMETERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_TYPE:
            size = ogs_nas_eps_decode_ue_additional_security_capability(&tracking_area_update_request->ue_additional_security_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_additional_security_capability() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_STATUS_TYPE:
            size = ogs_nas_eps_decode_ue_status(&tracking_area_update_request->ue_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_status() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_TYPE:
            size = ogs_nas_eps_decode_additional_information_requested(&tracking_area_update_request->additional_information_requested, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_additional_information_requested() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_N1_UE_NETWORK_CAPABILITY_TYPE:
            size = ogs_nas_eps_decode_n1_ue_network_capability(&tracking_area_update_request->n1_ue_network_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_n1_ue_network_capability() failed");
               return size;
            }

            tracking_area_update_request->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_N1_UE_NETWORK_CAPABILITY_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_tracking_area_update_accept(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_tracking_area_update_accept_t *tracking_area_update_accept = &message->emm.tracking_area_update_accept;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode TRACKING_AREA_UPDATE_ACCEPT\n");

    size = ogs_nas_eps_decode_eps_update_result(&tracking_area_update_accept->eps_update_result, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_eps_update_result() failed");
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

        switch (type) {
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer(&tracking_area_update_accept->t3412_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_GUTI_TYPE:
            size = ogs_nas_eps_decode_eps_mobile_identity(&tracking_area_update_accept->guti, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_eps_mobile_identity() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_GUTI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_TYPE:
            size = ogs_nas_eps_decode_tracking_area_identity_list(&tracking_area_update_accept->tai_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_tracking_area_identity_list() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_TYPE:
            size = ogs_nas_eps_decode_eps_bearer_context_status(&tracking_area_update_accept->eps_bearer_context_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_eps_bearer_context_status() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE:
            size = ogs_nas_eps_decode_location_area_identification(&tracking_area_update_accept->location_area_identification, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_location_area_identification() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_MS_IDENTITY_TYPE:
            size = ogs_nas_eps_decode_mobile_identity(&tracking_area_update_accept->ms_identity, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_mobile_identity() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_MS_IDENTITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EMM_CAUSE_TYPE:
            size = ogs_nas_eps_decode_emm_cause(&tracking_area_update_accept->emm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_emm_cause() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EMM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer(&tracking_area_update_accept->t3402_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer(&tracking_area_update_accept->t3423_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EQUIVALENT_PLMNS_TYPE:
            size = ogs_nas_eps_decode_plmn_list(&tracking_area_update_accept->equivalent_plmns, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_plmn_list() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EQUIVALENT_PLMNS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE:
            size = ogs_nas_eps_decode_emergency_number_list(&tracking_area_update_accept->emergency_number_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_emergency_number_list() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE:
            size = ogs_nas_eps_decode_eps_network_feature_support(&tracking_area_update_accept->eps_network_feature_support, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_eps_network_feature_support() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_additional_update_result(&tracking_area_update_accept->additional_update_result, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_additional_update_result() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3412_EXTENDED_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_3(&tracking_area_update_accept->t3412_extended_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_3() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3412_EXTENDED_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3324_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_2(&tracking_area_update_accept->t3324_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_2() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3324_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE:
            size = ogs_nas_eps_decode_extended_drx_parameters(&tracking_area_update_accept->extended_drx_parameters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_drx_parameters() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_STATUS_TYPE:
            size = ogs_nas_eps_decode_header_compression_configuration_status(&tracking_area_update_accept->header_compression_configuration_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_header_compression_configuration_status() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_DCN_ID_TYPE:
            size = ogs_nas_eps_decode_dcn_id(&tracking_area_update_accept->dcn_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_dcn_id() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_DCN_ID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_SMS_SERVICES_STATUS_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_sms_services_status(&tracking_area_update_accept->sms_services_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_sms_services_status() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_SMS_SERVICES_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NON__NW_POLICIES_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_non__nw_provided_policies(&tracking_area_update_accept->non__nw_policies, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_non__nw_provided_policies() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NON__NW_POLICIES_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3448_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_2(&tracking_area_update_accept->t3448_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_2() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3448_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NETWORK_POLICY_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_network_policy(&tracking_area_update_accept->network_policy, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_network_policy() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NETWORK_POLICY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3447_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_3(&tracking_area_update_accept->t3447_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_3() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3447_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_TYPE:
            size = ogs_nas_eps_decode_extended_emergency_number_list(&tracking_area_update_accept->extended_emergency_number_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_emergency_number_list() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_CIPHERING_KEY_DATA_TYPE:
            size = ogs_nas_eps_decode_ciphering_key_data(&tracking_area_update_accept->ciphering_key_data, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ciphering_key_data() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_CIPHERING_KEY_DATA_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_UE_RADIO_CAPABILITY_ID_TYPE:
            size = ogs_nas_eps_decode_ue_radio_capability_id(&tracking_area_update_accept->ue_radio_capability_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_radio_capability_id() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_UE_RADIO_CAPABILITY_ID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_ue_radio_capability_id_deletion_indication(&tracking_area_update_accept->ue_radio_capability_id_deletion_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_radio_capability_id_deletion_indication() failed");
               return size;
            }

            tracking_area_update_accept->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_tracking_area_update_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_tracking_area_update_reject_t *tracking_area_update_reject = &message->emm.tracking_area_update_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode TRACKING_AREA_UPDATE_REJECT\n");

    size = ogs_nas_eps_decode_emm_cause(&tracking_area_update_reject->emm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_emm_cause() failed");
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

        switch (type) {
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_T3346_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_2(&tracking_area_update_reject->t3346_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_2() failed");
               return size;
            }

            tracking_area_update_reject->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_T3346_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_EXTENDED_EMM_CAUSE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_extended_emm_cause(&tracking_area_update_reject->extended_emm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_emm_cause() failed");
               return size;
            }

            tracking_area_update_reject->presencemask |= OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_EXTENDED_EMM_CAUSE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_extended_service_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_extended_service_request_t *extended_service_request = &message->emm.extended_service_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode EXTENDED_SERVICE_REQUEST\n");

    size = ogs_nas_eps_decode_service_type(&extended_service_request->service_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_service_type() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_mobile_identity(&extended_service_request->m_tmsi, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_mobile_identity() failed");
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

        switch (type) {
        case OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_CSFB_RESPONSE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_csfb_response(&extended_service_request->csfb_response, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_csfb_response() failed");
               return size;
            }

            extended_service_request->presencemask |= OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_CSFB_RESPONSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE:
            size = ogs_nas_eps_decode_eps_bearer_context_status(&extended_service_request->eps_bearer_context_status, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_eps_bearer_context_status() failed");
               return size;
            }

            extended_service_request->presencemask |= OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_DEVICE_PROPERTIES_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_device_properties(&extended_service_request->device_properties, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_device_properties() failed");
               return size;
            }

            extended_service_request->presencemask |= OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_DEVICE_PROPERTIES_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_service_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_service_request_t *service_request = &message->emm.service_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode SERVICE_REQUEST\n");

    size = ogs_nas_eps_decode_ksi_and_sequence_number(&service_request->ksi_and_sequence_number, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_ksi_and_sequence_number() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_short_mac(&service_request->message_authentication_code, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_short_mac() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_eps_decode_service_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_service_reject_t *service_reject = &message->emm.service_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode SERVICE_REJECT\n");

    size = ogs_nas_eps_decode_emm_cause(&service_reject->emm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_emm_cause() failed");
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

        switch (type) {
        case OGS_NAS_EPS_SERVICE_REJECT_T3346_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_2(&service_reject->t3346_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_2() failed");
               return size;
            }

            service_reject->presencemask |= OGS_NAS_EPS_SERVICE_REJECT_T3346_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_SERVICE_REJECT_T3448_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_2(&service_reject->t3448_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_2() failed");
               return size;
            }

            service_reject->presencemask |= OGS_NAS_EPS_SERVICE_REJECT_T3448_VALUE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_guti_reallocation_command(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_guti_reallocation_command_t *guti_reallocation_command = &message->emm.guti_reallocation_command;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode GUTI_REALLOCATION_COMMAND\n");

    size = ogs_nas_eps_decode_eps_mobile_identity(&guti_reallocation_command->guti, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_eps_mobile_identity() failed");
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

        switch (type) {
        case OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_TAI_LIST_TYPE:
            size = ogs_nas_eps_decode_tracking_area_identity_list(&guti_reallocation_command->tai_list, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_tracking_area_identity_list() failed");
               return size;
            }

            guti_reallocation_command->presencemask |= OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_TAI_LIST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_DCN_ID_TYPE:
            size = ogs_nas_eps_decode_dcn_id(&guti_reallocation_command->dcn_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_dcn_id() failed");
               return size;
            }

            guti_reallocation_command->presencemask |= OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_DCN_ID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_UE_RADIO_CAPABILITY_ID_TYPE:
            size = ogs_nas_eps_decode_ue_radio_capability_id(&guti_reallocation_command->ue_radio_capability_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_radio_capability_id() failed");
               return size;
            }

            guti_reallocation_command->presencemask |= OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_UE_RADIO_CAPABILITY_ID_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_ue_radio_capability_id_deletion_indication(&guti_reallocation_command->ue_radio_capability_id_deletion_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_radio_capability_id_deletion_indication() failed");
               return size;
            }

            guti_reallocation_command->presencemask |= OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_authentication_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_authentication_request_t *authentication_request = &message->emm.authentication_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode AUTHENTICATION_REQUEST\n");

    size = ogs_nas_eps_decode_key_set_identifier(&authentication_request->nas_key_set_identifierasme, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_key_set_identifier() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_authentication_parameter_rand(&authentication_request->authentication_parameter_rand, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_authentication_parameter_rand() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_authentication_parameter_autn(&authentication_request->authentication_parameter_autn, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_authentication_parameter_autn() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_eps_decode_authentication_response(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_authentication_response_t *authentication_response = &message->emm.authentication_response;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode AUTHENTICATION_RESPONSE\n");

    size = ogs_nas_eps_decode_authentication_response_parameter(&authentication_response->authentication_response_parameter, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_authentication_response_parameter() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_eps_decode_identity_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_identity_request_t *identity_request = &message->emm.identity_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode IDENTITY_REQUEST\n");

    size = ogs_nas_eps_decode_identity_type_2(&identity_request->identity_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_identity_type_2() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_eps_decode_identity_response(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_identity_response_t *identity_response = &message->emm.identity_response;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode IDENTITY_RESPONSE\n");

    size = ogs_nas_eps_decode_mobile_identity(&identity_response->mobile_identity, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_mobile_identity() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_eps_decode_authentication_failure(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_authentication_failure_t *authentication_failure = &message->emm.authentication_failure;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode AUTHENTICATION_FAILURE\n");

    size = ogs_nas_eps_decode_emm_cause(&authentication_failure->emm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_emm_cause() failed");
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

        switch (type) {
        case OGS_NAS_EPS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_TYPE:
            size = ogs_nas_eps_decode_authentication_failure_parameter(&authentication_failure->authentication_failure_parameter, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_authentication_failure_parameter() failed");
               return size;
            }

            authentication_failure->presencemask |= OGS_NAS_EPS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_security_mode_command(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_security_mode_command_t *security_mode_command = &message->emm.security_mode_command;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode SECURITY_MODE_COMMAND\n");

    size = ogs_nas_eps_decode_security_algorithms(&security_mode_command->selected_nas_security_algorithms, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_security_algorithms() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_key_set_identifier(&security_mode_command->nas_key_set_identifier, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_key_set_identifier() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_ue_security_capability(&security_mode_command->replayed_ue_security_capabilities, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_ue_security_capability() failed");
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

        switch (type) {
        case OGS_NAS_EPS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_imeisv_request(&security_mode_command->imeisv_request, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_imeisv_request() failed");
               return size;
            }

            security_mode_command->presencemask |= OGS_NAS_EPS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_SECURITY_MODE_COMMAND_REPLAYED_NONCEUE_TYPE:
            size = ogs_nas_eps_decode_nonce(&security_mode_command->replayed_nonceue, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nonce() failed");
               return size;
            }

            security_mode_command->presencemask |= OGS_NAS_EPS_SECURITY_MODE_COMMAND_REPLAYED_NONCEUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_SECURITY_MODE_COMMAND_NONCEMME_TYPE:
            size = ogs_nas_eps_decode_nonce(&security_mode_command->noncemme, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nonce() failed");
               return size;
            }

            security_mode_command->presencemask |= OGS_NAS_EPS_SECURITY_MODE_COMMAND_NONCEMME_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_SECURITY_MODE_COMMAND_HASHMME_TYPE:
            size = ogs_nas_eps_decode_hashmme(&security_mode_command->hashmme, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_hashmme() failed");
               return size;
            }

            security_mode_command->presencemask |= OGS_NAS_EPS_SECURITY_MODE_COMMAND_HASHMME_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_SECURITY_MODE_COMMAND_REPLAYED_UE_ADDITIONAL_SECURITY_CAPABILITY_TYPE:
            size = ogs_nas_eps_decode_ue_additional_security_capability(&security_mode_command->replayed_ue_additional_security_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_additional_security_capability() failed");
               return size;
            }

            security_mode_command->presencemask |= OGS_NAS_EPS_SECURITY_MODE_COMMAND_REPLAYED_UE_ADDITIONAL_SECURITY_CAPABILITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_SECURITY_MODE_COMMAND_UE_RADIO_CAPABILITY_ID_REQUEST_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_ue_radio_capability_id_request(&security_mode_command->ue_radio_capability_id_request, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_radio_capability_id_request() failed");
               return size;
            }

            security_mode_command->presencemask |= OGS_NAS_EPS_SECURITY_MODE_COMMAND_UE_RADIO_CAPABILITY_ID_REQUEST_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_security_mode_complete(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_security_mode_complete_t *security_mode_complete = &message->emm.security_mode_complete;
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

        switch (type) {
        case OGS_NAS_EPS_SECURITY_MODE_COMPLETE_IMEISV_TYPE:
            size = ogs_nas_eps_decode_mobile_identity(&security_mode_complete->imeisv, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_mobile_identity() failed");
               return size;
            }

            security_mode_complete->presencemask |= OGS_NAS_EPS_SECURITY_MODE_COMPLETE_IMEISV_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_SECURITY_MODE_COMPLETE_REPLAYED_NAS_MESSAGE_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_replayed_nas_message_container(&security_mode_complete->replayed_nas_message_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_replayed_nas_message_container() failed");
               return size;
            }

            security_mode_complete->presencemask |= OGS_NAS_EPS_SECURITY_MODE_COMPLETE_REPLAYED_NAS_MESSAGE_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_SECURITY_MODE_COMPLETE_UE_RADIO_CAPABILITY_ID_TYPE:
            size = ogs_nas_eps_decode_ue_radio_capability_id(&security_mode_complete->ue_radio_capability_id, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ue_radio_capability_id() failed");
               return size;
            }

            security_mode_complete->presencemask |= OGS_NAS_EPS_SECURITY_MODE_COMPLETE_UE_RADIO_CAPABILITY_ID_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_security_mode_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_security_mode_reject_t *security_mode_reject = &message->emm.security_mode_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode SECURITY_MODE_REJECT\n");

    size = ogs_nas_eps_decode_emm_cause(&security_mode_reject->emm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_emm_cause() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_eps_decode_emm_status(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_emm_status_t *emm_status = &message->emm.emm_status;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode EMM_STATUS\n");

    size = ogs_nas_eps_decode_emm_cause(&emm_status->emm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_emm_cause() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_eps_decode_emm_information(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_emm_information_t *emm_information = &message->emm.emm_information;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode EMM_INFORMATION\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch (type) {
        case OGS_NAS_EPS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_TYPE:
            size = ogs_nas_eps_decode_network_name(&emm_information->full_name_for_network, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_network_name() failed");
               return size;
            }

            emm_information->presencemask |= OGS_NAS_EPS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_TYPE:
            size = ogs_nas_eps_decode_network_name(&emm_information->short_name_for_network, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_network_name() failed");
               return size;
            }

            emm_information->presencemask |= OGS_NAS_EPS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_EMM_INFORMATION_LOCAL_TIME_ZONE_TYPE:
            size = ogs_nas_eps_decode_time_zone(&emm_information->local_time_zone, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_time_zone() failed");
               return size;
            }

            emm_information->presencemask |= OGS_NAS_EPS_EMM_INFORMATION_LOCAL_TIME_ZONE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_TYPE:
            size = ogs_nas_eps_decode_time_zone_and_time(&emm_information->universal_time_and_local_time_zone, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_time_zone_and_time() failed");
               return size;
            }

            emm_information->presencemask |= OGS_NAS_EPS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_TYPE:
            size = ogs_nas_eps_decode_daylight_saving_time(&emm_information->network_daylight_saving_time, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_daylight_saving_time() failed");
               return size;
            }

            emm_information->presencemask |= OGS_NAS_EPS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_downlink_nas_transport(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_downlink_nas_transport_t *downlink_nas_transport = &message->emm.downlink_nas_transport;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode DOWNLINK_NAS_TRANSPORT\n");

    size = ogs_nas_eps_decode_eps_message_container(&downlink_nas_transport->nas_message_container, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_eps_message_container() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_eps_decode_uplink_nas_transport(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_uplink_nas_transport_t *uplink_nas_transport = &message->emm.uplink_nas_transport;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode UPLINK_NAS_TRANSPORT\n");

    size = ogs_nas_eps_decode_eps_message_container(&uplink_nas_transport->nas_message_container, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_eps_message_container() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_eps_decode_cs_service_notification(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_cs_service_notification_t *cs_service_notification = &message->emm.cs_service_notification;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode CS_SERVICE_NOTIFICATION\n");

    size = ogs_nas_eps_decode_paging_identity(&cs_service_notification->paging_identity, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_paging_identity() failed");
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

        switch (type) {
        case OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_CLI_TYPE:
            size = ogs_nas_eps_decode_cli(&cs_service_notification->cli, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_cli() failed");
               return size;
            }

            cs_service_notification->presencemask |= OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_CLI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_SS_CODE_TYPE:
            size = ogs_nas_eps_decode_ss_code(&cs_service_notification->ss_code, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_ss_code() failed");
               return size;
            }

            cs_service_notification->presencemask |= OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_SS_CODE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_LCS_INDICATOR_TYPE:
            size = ogs_nas_eps_decode_lcs_indicator(&cs_service_notification->lcs_indicator, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_lcs_indicator() failed");
               return size;
            }

            cs_service_notification->presencemask |= OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_LCS_INDICATOR_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_TYPE:
            size = ogs_nas_eps_decode_lcs_client_identity(&cs_service_notification->lcs_client_identity, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_lcs_client_identity() failed");
               return size;
            }

            cs_service_notification->presencemask |= OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_uplink_generic_nas_transport(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_uplink_generic_nas_transport_t *uplink_generic_nas_transport = &message->emm.uplink_generic_nas_transport;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode UPLINK_GENERIC_NAS_TRANSPORT\n");

    size = ogs_nas_eps_decode_generic_message_container_type(&uplink_generic_nas_transport->generic_message_container_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_generic_message_container_type() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_generic_message_container(&uplink_generic_nas_transport->generic_message_container, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_generic_message_container() failed");
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

        switch (type) {
        case OGS_NAS_EPS_UPLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE:
            size = ogs_nas_eps_decode_additional_information(&uplink_generic_nas_transport->additional_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_additional_information() failed");
               return size;
            }

            uplink_generic_nas_transport->presencemask |= OGS_NAS_EPS_UPLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_downlink_generic_nas_transport(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_downlink_generic_nas_transport_t *downlink_generic_nas_transport = &message->emm.downlink_generic_nas_transport;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode DOWNLINK_GENERIC_NAS_TRANSPORT\n");

    size = ogs_nas_eps_decode_generic_message_container_type(&downlink_generic_nas_transport->generic_message_container_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_generic_message_container_type() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_generic_message_container(&downlink_generic_nas_transport->generic_message_container, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_generic_message_container() failed");
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

        switch (type) {
        case OGS_NAS_EPS_DOWNLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE:
            size = ogs_nas_eps_decode_additional_information(&downlink_generic_nas_transport->additional_information, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_additional_information() failed");
               return size;
            }

            downlink_generic_nas_transport->presencemask |= OGS_NAS_EPS_DOWNLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_activate_default_eps_bearer_context_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_activate_default_eps_bearer_context_request_t *activate_default_eps_bearer_context_request = &message->esm.activate_default_eps_bearer_context_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST\n");

    size = ogs_nas_eps_decode_eps_quality_of_service(&activate_default_eps_bearer_context_request->eps_qos, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_eps_quality_of_service() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_access_point_name(&activate_default_eps_bearer_context_request->access_point_name, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_access_point_name() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_pdn_address(&activate_default_eps_bearer_context_request->pdn_address, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_pdn_address() failed");
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

        switch (type) {
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_TYPE:
            size = ogs_nas_eps_decode_transaction_identifier(&activate_default_eps_bearer_context_request->transaction_identifier, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_transaction_identifier() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_TYPE:
            size = ogs_nas_eps_decode_quality_of_service(&activate_default_eps_bearer_context_request->negotiated_qos, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_quality_of_service() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE:
            size = ogs_nas_eps_decode_llc_service_access_point_identifier(&activate_default_eps_bearer_context_request->negotiated_llc_sapi, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_llc_service_access_point_identifier() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_radio_priority(&activate_default_eps_bearer_context_request->radio_priority, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_radio_priority() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE:
            size = ogs_nas_eps_decode_packet_flow_identifier(&activate_default_eps_bearer_context_request->packet_flow_identifier, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_packet_flow_identifier() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_TYPE:
            size = ogs_nas_eps_decode_apn_aggregate_maximum_bit_rate(&activate_default_eps_bearer_context_request->apn_ambr, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_apn_aggregate_maximum_bit_rate() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_TYPE:
            size = ogs_nas_eps_decode_esm_cause(&activate_default_eps_bearer_context_request->esm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_esm_cause() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&activate_default_eps_bearer_context_request->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_connectivity_type(&activate_default_eps_bearer_context_request->connectivity_type, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_connectivity_type() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_wlan_offload_acceptability(&activate_default_eps_bearer_context_request->wlan_offload_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_wlan_offload_acceptability() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&activate_default_eps_bearer_context_request->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE:
            size = ogs_nas_eps_decode_header_compression_configuration(&activate_default_eps_bearer_context_request->header_compression_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_header_compression_configuration() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_control_plane_only_indication(&activate_default_eps_bearer_context_request->control_plane_only_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_control_plane_only_indication() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&activate_default_eps_bearer_context_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_TYPE:
            size = ogs_nas_eps_decode_serving_plmn_rate_control(&activate_default_eps_bearer_context_request->serving_plmn_rate_control, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_serving_plmn_rate_control() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_APN_AMBR_TYPE:
            size = ogs_nas_eps_decode_extended_apn_aggregate_maximum_bit_rate(&activate_default_eps_bearer_context_request->extended_apn_ambr, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_apn_aggregate_maximum_bit_rate() failed");
               return size;
            }

            activate_default_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_APN_AMBR_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_activate_default_eps_bearer_context_accept(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_activate_default_eps_bearer_context_accept_t *activate_default_eps_bearer_context_accept = &message->esm.activate_default_eps_bearer_context_accept;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch (type) {
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&activate_default_eps_bearer_context_accept->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            activate_default_eps_bearer_context_accept->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&activate_default_eps_bearer_context_accept->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            activate_default_eps_bearer_context_accept->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_activate_default_eps_bearer_context_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_activate_default_eps_bearer_context_reject_t *activate_default_eps_bearer_context_reject = &message->esm.activate_default_eps_bearer_context_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT\n");

    size = ogs_nas_eps_decode_esm_cause(&activate_default_eps_bearer_context_reject->esm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_esm_cause() failed");
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

        switch (type) {
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&activate_default_eps_bearer_context_reject->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            activate_default_eps_bearer_context_reject->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&activate_default_eps_bearer_context_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            activate_default_eps_bearer_context_reject->presencemask |= OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_activate_dedicated_eps_bearer_context_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_activate_dedicated_eps_bearer_context_request_t *activate_dedicated_eps_bearer_context_request = &message->esm.activate_dedicated_eps_bearer_context_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST\n");

    size = ogs_nas_eps_decode_linked_eps_bearer_identity(&activate_dedicated_eps_bearer_context_request->linked_eps_bearer_identity, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_linked_eps_bearer_identity() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_eps_quality_of_service(&activate_dedicated_eps_bearer_context_request->eps_qos, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_eps_quality_of_service() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_traffic_flow_template(&activate_dedicated_eps_bearer_context_request->tft, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_traffic_flow_template() failed");
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

        switch (type) {
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_TYPE:
            size = ogs_nas_eps_decode_transaction_identifier(&activate_dedicated_eps_bearer_context_request->transaction_identifier, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_transaction_identifier() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_TYPE:
            size = ogs_nas_eps_decode_quality_of_service(&activate_dedicated_eps_bearer_context_request->negotiated_qos, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_quality_of_service() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE:
            size = ogs_nas_eps_decode_llc_service_access_point_identifier(&activate_dedicated_eps_bearer_context_request->negotiated_llc_sapi, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_llc_service_access_point_identifier() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_radio_priority(&activate_dedicated_eps_bearer_context_request->radio_priority, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_radio_priority() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE:
            size = ogs_nas_eps_decode_packet_flow_identifier(&activate_dedicated_eps_bearer_context_request->packet_flow_identifier, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_packet_flow_identifier() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&activate_dedicated_eps_bearer_context_request->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_wlan_offload_acceptability(&activate_dedicated_eps_bearer_context_request->wlan_offload_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_wlan_offload_acceptability() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&activate_dedicated_eps_bearer_context_request->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&activate_dedicated_eps_bearer_context_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_EPS_QOS_TYPE:
            size = ogs_nas_eps_decode_extended_quality_of_service(&activate_dedicated_eps_bearer_context_request->extended_eps_qos, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_quality_of_service() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_EPS_QOS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_activate_dedicated_eps_bearer_context_accept(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_activate_dedicated_eps_bearer_context_accept_t *activate_dedicated_eps_bearer_context_accept = &message->esm.activate_dedicated_eps_bearer_context_accept;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch (type) {
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&activate_dedicated_eps_bearer_context_accept->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_accept->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&activate_dedicated_eps_bearer_context_accept->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_accept->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&activate_dedicated_eps_bearer_context_accept->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_accept->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_activate_dedicated_eps_bearer_context_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_activate_dedicated_eps_bearer_context_reject_t *activate_dedicated_eps_bearer_context_reject = &message->esm.activate_dedicated_eps_bearer_context_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT\n");

    size = ogs_nas_eps_decode_esm_cause(&activate_dedicated_eps_bearer_context_reject->esm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_esm_cause() failed");
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

        switch (type) {
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&activate_dedicated_eps_bearer_context_reject->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_reject->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&activate_dedicated_eps_bearer_context_reject->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_reject->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&activate_dedicated_eps_bearer_context_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            activate_dedicated_eps_bearer_context_reject->presencemask |= OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_modify_eps_bearer_context_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_modify_eps_bearer_context_request_t *modify_eps_bearer_context_request = &message->esm.modify_eps_bearer_context_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode MODIFY_EPS_BEARER_CONTEXT_REQUEST\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch (type) {
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_EPS_QOS_TYPE:
            size = ogs_nas_eps_decode_eps_quality_of_service(&modify_eps_bearer_context_request->new_eps_qos, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_eps_quality_of_service() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_EPS_QOS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_TFT_TYPE:
            size = ogs_nas_eps_decode_traffic_flow_template(&modify_eps_bearer_context_request->tft, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_traffic_flow_template() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_TFT_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_QOS_TYPE:
            size = ogs_nas_eps_decode_quality_of_service(&modify_eps_bearer_context_request->new_qos, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_quality_of_service() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_QOS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE:
            size = ogs_nas_eps_decode_llc_service_access_point_identifier(&modify_eps_bearer_context_request->negotiated_llc_sapi, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_llc_service_access_point_identifier() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_radio_priority(&modify_eps_bearer_context_request->radio_priority, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_radio_priority() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE:
            size = ogs_nas_eps_decode_packet_flow_identifier(&modify_eps_bearer_context_request->packet_flow_identifier, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_packet_flow_identifier() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_TYPE:
            size = ogs_nas_eps_decode_apn_aggregate_maximum_bit_rate(&modify_eps_bearer_context_request->apn_ambr, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_apn_aggregate_maximum_bit_rate() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&modify_eps_bearer_context_request->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_wlan_offload_acceptability(&modify_eps_bearer_context_request->wlan_offload_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_wlan_offload_acceptability() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&modify_eps_bearer_context_request->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE:
            size = ogs_nas_eps_decode_header_compression_configuration(&modify_eps_bearer_context_request->header_compression_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_header_compression_configuration() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&modify_eps_bearer_context_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_APN_AMBR_TYPE:
            size = ogs_nas_eps_decode_extended_apn_aggregate_maximum_bit_rate(&modify_eps_bearer_context_request->extended_apn_ambr, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_apn_aggregate_maximum_bit_rate() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_APN_AMBR_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_EPS_QOS_TYPE:
            size = ogs_nas_eps_decode_extended_quality_of_service(&modify_eps_bearer_context_request->extended_eps_qos, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_quality_of_service() failed");
               return size;
            }

            modify_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_EPS_QOS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_modify_eps_bearer_context_accept(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_modify_eps_bearer_context_accept_t *modify_eps_bearer_context_accept = &message->esm.modify_eps_bearer_context_accept;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode MODIFY_EPS_BEARER_CONTEXT_ACCEPT\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch (type) {
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&modify_eps_bearer_context_accept->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            modify_eps_bearer_context_accept->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&modify_eps_bearer_context_accept->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            modify_eps_bearer_context_accept->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&modify_eps_bearer_context_accept->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            modify_eps_bearer_context_accept->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_modify_eps_bearer_context_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_modify_eps_bearer_context_reject_t *modify_eps_bearer_context_reject = &message->esm.modify_eps_bearer_context_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode MODIFY_EPS_BEARER_CONTEXT_REJECT\n");

    size = ogs_nas_eps_decode_esm_cause(&modify_eps_bearer_context_reject->esm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_esm_cause() failed");
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

        switch (type) {
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&modify_eps_bearer_context_reject->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            modify_eps_bearer_context_reject->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&modify_eps_bearer_context_reject->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            modify_eps_bearer_context_reject->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&modify_eps_bearer_context_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            modify_eps_bearer_context_reject->presencemask |= OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_deactivate_eps_bearer_context_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_deactivate_eps_bearer_context_request_t *deactivate_eps_bearer_context_request = &message->esm.deactivate_eps_bearer_context_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST\n");

    size = ogs_nas_eps_decode_esm_cause(&deactivate_eps_bearer_context_request->esm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_esm_cause() failed");
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

        switch (type) {
        case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&deactivate_eps_bearer_context_request->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            deactivate_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_T3396_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_3(&deactivate_eps_bearer_context_request->t3396_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_3() failed");
               return size;
            }

            deactivate_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_T3396_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_wlan_offload_acceptability(&deactivate_eps_bearer_context_request->wlan_offload_indication, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_wlan_offload_acceptability() failed");
               return size;
            }

            deactivate_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&deactivate_eps_bearer_context_request->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            deactivate_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&deactivate_eps_bearer_context_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            deactivate_eps_bearer_context_request->presencemask |= OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_deactivate_eps_bearer_context_accept(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_deactivate_eps_bearer_context_accept_t *deactivate_eps_bearer_context_accept = &message->esm.deactivate_eps_bearer_context_accept;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch (type) {
        case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&deactivate_eps_bearer_context_accept->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            deactivate_eps_bearer_context_accept->presencemask |= OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&deactivate_eps_bearer_context_accept->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            deactivate_eps_bearer_context_accept->presencemask |= OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_pdn_connectivity_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_pdn_connectivity_request_t *pdn_connectivity_request = &message->esm.pdn_connectivity_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDN_CONNECTIVITY_REQUEST\n");

    size = ogs_nas_eps_decode_request_type(&pdn_connectivity_request->request_type, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_request_type() failed");
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

        switch (type) {
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_esm_information_transfer_flag(&pdn_connectivity_request->esm_information_transfer_flag, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_esm_information_transfer_flag() failed");
               return size;
            }

            pdn_connectivity_request->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_TYPE:
            size = ogs_nas_eps_decode_access_point_name(&pdn_connectivity_request->access_point_name, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_access_point_name() failed");
               return size;
            }

            pdn_connectivity_request->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&pdn_connectivity_request->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            pdn_connectivity_request->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_DEVICE_PROPERTIES_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_device_properties(&pdn_connectivity_request->device_properties, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_device_properties() failed");
               return size;
            }

            pdn_connectivity_request->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_DEVICE_PROPERTIES_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&pdn_connectivity_request->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            pdn_connectivity_request->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE:
            size = ogs_nas_eps_decode_header_compression_configuration(&pdn_connectivity_request->header_compression_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_header_compression_configuration() failed");
               return size;
            }

            pdn_connectivity_request->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&pdn_connectivity_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdn_connectivity_request->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_pdn_connectivity_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_pdn_connectivity_reject_t *pdn_connectivity_reject = &message->esm.pdn_connectivity_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDN_CONNECTIVITY_REJECT\n");

    size = ogs_nas_eps_decode_esm_cause(&pdn_connectivity_reject->esm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_esm_cause() failed");
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

        switch (type) {
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&pdn_connectivity_reject->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            pdn_connectivity_reject->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_BACK_OFF_TIMER_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_3(&pdn_connectivity_reject->back_off_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_3() failed");
               return size;
            }

            pdn_connectivity_reject->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_BACK_OFF_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_RE_ATTEMPT_INDICATOR_TYPE:
            size = ogs_nas_eps_decode_re_attempt_indicator(&pdn_connectivity_reject->re_attempt_indicator, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_re_attempt_indicator() failed");
               return size;
            }

            pdn_connectivity_reject->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_RE_ATTEMPT_INDICATOR_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&pdn_connectivity_reject->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            pdn_connectivity_reject->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&pdn_connectivity_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdn_connectivity_reject->presencemask |= OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_pdn_disconnect_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_pdn_disconnect_request_t *pdn_disconnect_request = &message->esm.pdn_disconnect_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDN_DISCONNECT_REQUEST\n");

    size = ogs_nas_eps_decode_linked_eps_bearer_identity(&pdn_disconnect_request->linked_eps_bearer_identity, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_linked_eps_bearer_identity() failed");
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

        switch (type) {
        case OGS_NAS_EPS_PDN_DISCONNECT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&pdn_disconnect_request->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            pdn_disconnect_request->presencemask |= OGS_NAS_EPS_PDN_DISCONNECT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_PDN_DISCONNECT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&pdn_disconnect_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdn_disconnect_request->presencemask |= OGS_NAS_EPS_PDN_DISCONNECT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_pdn_disconnect_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_pdn_disconnect_reject_t *pdn_disconnect_reject = &message->esm.pdn_disconnect_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDN_DISCONNECT_REJECT\n");

    size = ogs_nas_eps_decode_esm_cause(&pdn_disconnect_reject->esm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_esm_cause() failed");
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

        switch (type) {
        case OGS_NAS_EPS_PDN_DISCONNECT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&pdn_disconnect_reject->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            pdn_disconnect_reject->presencemask |= OGS_NAS_EPS_PDN_DISCONNECT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_PDN_DISCONNECT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&pdn_disconnect_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdn_disconnect_reject->presencemask |= OGS_NAS_EPS_PDN_DISCONNECT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_bearer_resource_allocation_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_bearer_resource_allocation_request_t *bearer_resource_allocation_request = &message->esm.bearer_resource_allocation_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode BEARER_RESOURCE_ALLOCATION_REQUEST\n");

    size = ogs_nas_eps_decode_linked_eps_bearer_identity(&bearer_resource_allocation_request->linked_eps_bearer_identity, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_linked_eps_bearer_identity() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_traffic_flow_aggregate_description(&bearer_resource_allocation_request->traffic_flow_aggregate, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_traffic_flow_aggregate_description() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_eps_quality_of_service(&bearer_resource_allocation_request->required_traffic_flow_qos, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_eps_quality_of_service() failed");
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

        switch (type) {
        case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&bearer_resource_allocation_request->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            bearer_resource_allocation_request->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_DEVICE_PROPERTIES_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_device_properties(&bearer_resource_allocation_request->device_properties, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_device_properties() failed");
               return size;
            }

            bearer_resource_allocation_request->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_DEVICE_PROPERTIES_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&bearer_resource_allocation_request->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            bearer_resource_allocation_request->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&bearer_resource_allocation_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            bearer_resource_allocation_request->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_EPS_QOS_TYPE:
            size = ogs_nas_eps_decode_extended_quality_of_service(&bearer_resource_allocation_request->extended_eps_qos, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_quality_of_service() failed");
               return size;
            }

            bearer_resource_allocation_request->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_EPS_QOS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_bearer_resource_allocation_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_bearer_resource_allocation_reject_t *bearer_resource_allocation_reject = &message->esm.bearer_resource_allocation_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode BEARER_RESOURCE_ALLOCATION_REJECT\n");

    size = ogs_nas_eps_decode_esm_cause(&bearer_resource_allocation_reject->esm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_esm_cause() failed");
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

        switch (type) {
        case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&bearer_resource_allocation_reject->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            bearer_resource_allocation_reject->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_3(&bearer_resource_allocation_reject->back_off_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_3() failed");
               return size;
            }

            bearer_resource_allocation_reject->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE:
            size = ogs_nas_eps_decode_re_attempt_indicator(&bearer_resource_allocation_reject->re_attempt_indicator, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_re_attempt_indicator() failed");
               return size;
            }

            bearer_resource_allocation_reject->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&bearer_resource_allocation_reject->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            bearer_resource_allocation_reject->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&bearer_resource_allocation_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            bearer_resource_allocation_reject->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_bearer_resource_modification_request(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_bearer_resource_modification_request_t *bearer_resource_modification_request = &message->esm.bearer_resource_modification_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode BEARER_RESOURCE_MODIFICATION_REQUEST\n");

    size = ogs_nas_eps_decode_linked_eps_bearer_identity(&bearer_resource_modification_request->eps_bearer_identity_for_packet_filter, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_linked_eps_bearer_identity() failed");
        return size;
    }

    decoded += size;

    size = ogs_nas_eps_decode_traffic_flow_aggregate_description(&bearer_resource_modification_request->traffic_flow_aggregate, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_traffic_flow_aggregate_description() failed");
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

        switch (type) {
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_TYPE:
            size = ogs_nas_eps_decode_eps_quality_of_service(&bearer_resource_modification_request->required_traffic_flow_qos, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_eps_quality_of_service() failed");
               return size;
            }

            bearer_resource_modification_request->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_ESM_CAUSE_TYPE:
            size = ogs_nas_eps_decode_esm_cause(&bearer_resource_modification_request->esm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_esm_cause() failed");
               return size;
            }

            bearer_resource_modification_request->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_ESM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&bearer_resource_modification_request->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            bearer_resource_modification_request->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_DEVICE_PROPERTIES_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_eps_decode_device_properties(&bearer_resource_modification_request->device_properties, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_device_properties() failed");
               return size;
            }

            bearer_resource_modification_request->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_DEVICE_PROPERTIES_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&bearer_resource_modification_request->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            bearer_resource_modification_request->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE:
            size = ogs_nas_eps_decode_header_compression_configuration(&bearer_resource_modification_request->header_compression_configuration, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_header_compression_configuration() failed");
               return size;
            }

            bearer_resource_modification_request->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&bearer_resource_modification_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            bearer_resource_modification_request->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_EPS_QOS_TYPE:
            size = ogs_nas_eps_decode_extended_quality_of_service(&bearer_resource_modification_request->extended_eps_qos, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_quality_of_service() failed");
               return size;
            }

            bearer_resource_modification_request->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_EPS_QOS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_bearer_resource_modification_reject(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_bearer_resource_modification_reject_t *bearer_resource_modification_reject = &message->esm.bearer_resource_modification_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode BEARER_RESOURCE_MODIFICATION_REJECT\n");

    size = ogs_nas_eps_decode_esm_cause(&bearer_resource_modification_reject->esm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_esm_cause() failed");
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

        switch (type) {
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&bearer_resource_modification_reject->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            bearer_resource_modification_reject->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE:
            size = ogs_nas_eps_decode_gprs_timer_3(&bearer_resource_modification_reject->back_off_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_gprs_timer_3() failed");
               return size;
            }

            bearer_resource_modification_reject->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE:
            size = ogs_nas_eps_decode_re_attempt_indicator(&bearer_resource_modification_reject->re_attempt_indicator, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_re_attempt_indicator() failed");
               return size;
            }

            bearer_resource_modification_reject->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_NBIFOM_CONTAINER_TYPE:
            size = ogs_nas_eps_decode_nbifom_container(&bearer_resource_modification_reject->nbifom_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_nbifom_container() failed");
               return size;
            }

            bearer_resource_modification_reject->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_NBIFOM_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&bearer_resource_modification_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            bearer_resource_modification_reject->presencemask |= OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_esm_information_response(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_esm_information_response_t *esm_information_response = &message->esm.esm_information_response;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode ESM_INFORMATION_RESPONSE\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch (type) {
        case OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_TYPE:
            size = ogs_nas_eps_decode_access_point_name(&esm_information_response->access_point_name, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_access_point_name() failed");
               return size;
            }

            esm_information_response->presencemask |= OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_protocol_configuration_options(&esm_information_response->protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_protocol_configuration_options() failed");
               return size;
            }

            esm_information_response->presencemask |= OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_eps_decode_extended_protocol_configuration_options(&esm_information_response->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_eps_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            esm_information_response->presencemask |= OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_warn("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

int ogs_nas_eps_decode_esm_status(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_eps_esm_status_t *esm_status = &message->esm.esm_status;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode ESM_STATUS\n");

    size = ogs_nas_eps_decode_esm_cause(&esm_status->esm_cause, pkbuf);
    if (size < 0) {
        ogs_error("ogs_nas_eps_decode_esm_cause() failed");
        return size;
    }

    decoded += size;

    return decoded;
}

int ogs_nas_emm_decode(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    int decoded = 0;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);
    ogs_assert(pkbuf->len);

    memset(message, 0, sizeof(ogs_nas_eps_message_t));

    size = sizeof(ogs_nas_emm_header_t);
    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return OGS_ERROR;
    }
    memcpy(&message->emm.h, pkbuf->data - size, size);
    decoded += size;

    if (message->emm.h.security_header_type >=
            OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
        ogs_assert(ogs_pkbuf_push(pkbuf, 1));
        decoded -= 1;
        size = ogs_nas_eps_decode_service_request(message, pkbuf);
        ogs_assert(size >= 0);
        decoded += size;

        goto out;
    }

    switch (message->emm.h.message_type) {
    case OGS_NAS_EPS_ATTACH_REQUEST:
        size = ogs_nas_eps_decode_attach_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_attach_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_ATTACH_ACCEPT:
        size = ogs_nas_eps_decode_attach_accept(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_attach_accept() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_ATTACH_COMPLETE:
        size = ogs_nas_eps_decode_attach_complete(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_attach_complete() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_ATTACH_REJECT:
        size = ogs_nas_eps_decode_attach_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_attach_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_DETACH_REQUEST:
        size = ogs_nas_eps_decode_detach_request_from_ue(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_detach_request_from_ue() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_DETACH_ACCEPT:
        break;
    case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST:
        size = ogs_nas_eps_decode_tracking_area_update_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_tracking_area_update_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT:
        size = ogs_nas_eps_decode_tracking_area_update_accept(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_tracking_area_update_accept() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_TRACKING_AREA_UPDATE_COMPLETE:
        break;
    case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT:
        size = ogs_nas_eps_decode_tracking_area_update_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_tracking_area_update_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST:
        size = ogs_nas_eps_decode_extended_service_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_extended_service_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_SERVICE_REJECT:
        size = ogs_nas_eps_decode_service_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_service_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND:
        size = ogs_nas_eps_decode_guti_reallocation_command(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_guti_reallocation_command() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_GUTI_REALLOCATION_COMPLETE:
        break;
    case OGS_NAS_EPS_AUTHENTICATION_REQUEST:
        size = ogs_nas_eps_decode_authentication_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_authentication_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_AUTHENTICATION_RESPONSE:
        size = ogs_nas_eps_decode_authentication_response(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_authentication_response() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_AUTHENTICATION_REJECT:
        break;
    case OGS_NAS_EPS_IDENTITY_REQUEST:
        size = ogs_nas_eps_decode_identity_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_identity_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_IDENTITY_RESPONSE:
        size = ogs_nas_eps_decode_identity_response(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_identity_response() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_AUTHENTICATION_FAILURE:
        size = ogs_nas_eps_decode_authentication_failure(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_authentication_failure() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_SECURITY_MODE_COMMAND:
        size = ogs_nas_eps_decode_security_mode_command(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_security_mode_command() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_SECURITY_MODE_COMPLETE:
        size = ogs_nas_eps_decode_security_mode_complete(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_security_mode_complete() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_SECURITY_MODE_REJECT:
        size = ogs_nas_eps_decode_security_mode_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_security_mode_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_EMM_STATUS:
        size = ogs_nas_eps_decode_emm_status(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_emm_status() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_EMM_INFORMATION:
        size = ogs_nas_eps_decode_emm_information(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_emm_information() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_DOWNLINK_NAS_TRANSPORT:
        size = ogs_nas_eps_decode_downlink_nas_transport(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_downlink_nas_transport() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_UPLINK_NAS_TRANSPORT:
        size = ogs_nas_eps_decode_uplink_nas_transport(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_uplink_nas_transport() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_CS_SERVICE_NOTIFICATION:
        size = ogs_nas_eps_decode_cs_service_notification(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_cs_service_notification() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_UPLINK_GENERIC_NAS_TRANSPORT:
        size = ogs_nas_eps_decode_uplink_generic_nas_transport(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_uplink_generic_nas_transport() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_DOWNLINK_GENERIC_NAS_TRANSPORT:
        size = ogs_nas_eps_decode_downlink_generic_nas_transport(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_downlink_generic_nas_transport() failed");
           return size;
        }

        decoded += size;
        break;
    default:
        ogs_error("Unknown message type (0x%x) or not implemented", 
                message->emm.h.message_type);
        break;
    }

out:
    ogs_assert(ogs_pkbuf_push(pkbuf, decoded));

    return OGS_OK;
}
int ogs_nas_esm_decode(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    int decoded = 0;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);
    ogs_assert(pkbuf->len);

    memset(message, 0, sizeof(ogs_nas_eps_message_t));

    size = sizeof(ogs_nas_esm_header_t);
    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return OGS_ERROR;
    }
    memcpy(&message->esm.h, pkbuf->data - size, size);
    decoded += size;

    switch (message->esm.h.message_type) {
    case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST:
        size = ogs_nas_eps_decode_activate_default_eps_bearer_context_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_activate_default_eps_bearer_context_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT:
        size = ogs_nas_eps_decode_activate_default_eps_bearer_context_accept(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_activate_default_eps_bearer_context_accept() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT:
        size = ogs_nas_eps_decode_activate_default_eps_bearer_context_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_activate_default_eps_bearer_context_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST:
        size = ogs_nas_eps_decode_activate_dedicated_eps_bearer_context_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_activate_dedicated_eps_bearer_context_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT:
        size = ogs_nas_eps_decode_activate_dedicated_eps_bearer_context_accept(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_activate_dedicated_eps_bearer_context_accept() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT:
        size = ogs_nas_eps_decode_activate_dedicated_eps_bearer_context_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_activate_dedicated_eps_bearer_context_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST:
        size = ogs_nas_eps_decode_modify_eps_bearer_context_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_modify_eps_bearer_context_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT:
        size = ogs_nas_eps_decode_modify_eps_bearer_context_accept(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_modify_eps_bearer_context_accept() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT:
        size = ogs_nas_eps_decode_modify_eps_bearer_context_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_modify_eps_bearer_context_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST:
        size = ogs_nas_eps_decode_deactivate_eps_bearer_context_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_deactivate_eps_bearer_context_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT:
        size = ogs_nas_eps_decode_deactivate_eps_bearer_context_accept(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_deactivate_eps_bearer_context_accept() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST:
        size = ogs_nas_eps_decode_pdn_connectivity_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdn_connectivity_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT:
        size = ogs_nas_eps_decode_pdn_connectivity_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdn_connectivity_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_PDN_DISCONNECT_REQUEST:
        size = ogs_nas_eps_decode_pdn_disconnect_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdn_disconnect_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_PDN_DISCONNECT_REJECT:
        size = ogs_nas_eps_decode_pdn_disconnect_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_pdn_disconnect_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST:
        size = ogs_nas_eps_decode_bearer_resource_allocation_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_bearer_resource_allocation_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT:
        size = ogs_nas_eps_decode_bearer_resource_allocation_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_bearer_resource_allocation_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST:
        size = ogs_nas_eps_decode_bearer_resource_modification_request(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_bearer_resource_modification_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT:
        size = ogs_nas_eps_decode_bearer_resource_modification_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_bearer_resource_modification_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_ESM_INFORMATION_REQUEST:
        break;
    case OGS_NAS_EPS_ESM_INFORMATION_RESPONSE:
        size = ogs_nas_eps_decode_esm_information_response(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_esm_information_response() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_EPS_ESM_STATUS:
        size = ogs_nas_eps_decode_esm_status(message, pkbuf);
        if (size < 0) {
           ogs_error("ogs_nas_5gs_decode_esm_status() failed");
           return size;
        }

        decoded += size;
        break;
    default:
        ogs_error("Unknown message type (0x%x) or not implemented", 
               message->esm.h.message_type);
        break;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, decoded));

    return OGS_OK;
}

