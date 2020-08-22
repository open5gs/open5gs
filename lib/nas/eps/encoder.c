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
 * Created on: 2020-08-16 17:47:29.440664 by acetcom
 * from 24301-g40.docx
 ******************************************************************************/

#include "ogs-nas-eps.h"

int ogs_nas_eps_encode_attach_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_attach_accept(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_attach_complete(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_attach_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_detach_request_from_ue(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_detach_request_to_ue(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_tracking_area_update_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_tracking_area_update_accept(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_tracking_area_update_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_extended_service_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_service_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_service_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_guti_reallocation_command(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_authentication_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_authentication_response(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_identity_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_identity_response(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_authentication_failure(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_security_mode_command(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_security_mode_complete(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_security_mode_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_emm_status(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_emm_information(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_downlink_nas_transport(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_uplink_nas_transport(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_cs_service_notification(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_uplink_generic_nas_transport(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_downlink_generic_nas_transport(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_activate_default_eps_bearer_context_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_activate_default_eps_bearer_context_accept(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_activate_default_eps_bearer_context_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_activate_dedicated_eps_bearer_context_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_activate_dedicated_eps_bearer_context_accept(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_activate_dedicated_eps_bearer_context_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_modify_eps_bearer_context_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_modify_eps_bearer_context_accept(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_modify_eps_bearer_context_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_deactivate_eps_bearer_context_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_deactivate_eps_bearer_context_accept(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_pdn_connectivity_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_pdn_connectivity_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_pdn_disconnect_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_pdn_disconnect_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_bearer_resource_allocation_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_bearer_resource_allocation_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_bearer_resource_modification_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_bearer_resource_modification_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_esm_information_response(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_esm_status(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message);
int ogs_nas_eps_encode_attach_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_attach_request_t *attach_request = &message->emm.attach_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode ATTACH_REQUEST");

    size = ogs_nas_eps_encode_eps_attach_type(pkbuf, &attach_request->eps_attach_type);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_eps_mobile_identity(pkbuf, &attach_request->eps_mobile_identity);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_ue_network_capability(pkbuf, &attach_request->ue_network_capability);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_esm_message_container(pkbuf, &attach_request->esm_message_container);
    ogs_assert(size >= 0);
    encoded += size;

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_p_tmsi_signature(pkbuf, &attach_request->old_p_tmsi_signature);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_GUTI_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_GUTI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_eps_mobile_identity(pkbuf, &attach_request->additional_guti);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_tracking_area_identity(pkbuf, &attach_request->last_visited_registered_tai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_DRX_PARAMETER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_DRX_PARAMETER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_drx_parameter(pkbuf, &attach_request->drx_parameter);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ms_network_capability(pkbuf, &attach_request->ms_network_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_location_area_identification(pkbuf, &attach_request->old_location_area_identification);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_TMSI_STATUS_PRESENT) {
        attach_request->tmsi_status.type = (OGS_NAS_EPS_ATTACH_REQUEST_TMSI_STATUS_TYPE >> 4);

        size = ogs_nas_eps_encode_tmsi_status(pkbuf, &attach_request->tmsi_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_mobile_station_classmark_2(pkbuf, &attach_request->mobile_station_classmark_2);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_mobile_station_classmark_3(pkbuf, &attach_request->mobile_station_classmark_3);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_SUPPORTED_CODECS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_SUPPORTED_CODECS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_supported_codec_list(pkbuf, &attach_request->supported_codecs);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT) {
        attach_request->additional_update_type.type = (OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_TYPE >> 4);

        size = ogs_nas_eps_encode_additional_update_type(pkbuf, &attach_request->additional_update_type);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_voice_domain_preference_and_ue_usage_setting(pkbuf, &attach_request->voice_domain_preference_and_ue_usage_setting);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_DEVICE_PROPERTIES_PRESENT) {
        attach_request->device_properties.type = (OGS_NAS_EPS_ATTACH_REQUEST_DEVICE_PROPERTIES_TYPE >> 4);

        size = ogs_nas_eps_encode_device_properties(pkbuf, &attach_request->device_properties);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_OLD_GUTI_TYPE_PRESENT) {
        attach_request->old_guti_type.type = (OGS_NAS_EPS_ATTACH_REQUEST_OLD_GUTI_TYPE_TYPE >> 4);

        size = ogs_nas_eps_encode_guti_type(pkbuf, &attach_request->old_guti_type);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT) {
        attach_request->ms_network_feature_support.type = (OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_TYPE >> 4);

        size = ogs_nas_eps_encode_ms_network_feature_support(pkbuf, &attach_request->ms_network_feature_support);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_network_resource_identifier_container(pkbuf, &attach_request->tmsi_based_nri_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_T3324_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_T3324_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_2(pkbuf, &attach_request->t3324_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_3(pkbuf, &attach_request->t3412_extended_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_drx_parameters(pkbuf, &attach_request->extended_drx_parameters);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ue_additional_security_capability(pkbuf, &attach_request->ue_additional_security_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_UE_STATUS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_UE_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ue_status(pkbuf, &attach_request->ue_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_additional_information_requested(pkbuf, &attach_request->additional_information_requested);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_request->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_N1_UE_NETWORK_CAPABILITY_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REQUEST_N1_UE_NETWORK_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_n1_ue_network_capability(pkbuf, &attach_request->n1_ue_network_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_attach_accept(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_attach_accept_t *attach_accept = &message->emm.attach_accept;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode ATTACH_ACCEPT");

    size = ogs_nas_eps_encode_eps_attach_result(pkbuf, &attach_accept->eps_attach_result);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_gprs_timer(pkbuf, &attach_accept->t3412_value);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_tracking_area_identity_list(pkbuf, &attach_accept->tai_list);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_esm_message_container(pkbuf, &attach_accept->esm_message_container);
    ogs_assert(size >= 0);
    encoded += size;

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_GUTI_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_GUTI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_eps_mobile_identity(pkbuf, &attach_accept->guti);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_location_area_identification(pkbuf, &attach_accept->location_area_identification);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_MS_IDENTITY_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_MS_IDENTITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_mobile_identity(pkbuf, &attach_accept->ms_identity);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_EMM_CAUSE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_EMM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_emm_cause(pkbuf, &attach_accept->emm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_T3402_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_T3402_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer(pkbuf, &attach_accept->t3402_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_T3423_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_T3423_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer(pkbuf, &attach_accept->t3423_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_plmn_list(pkbuf, &attach_accept->equivalent_plmns);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_emergency_number_list(pkbuf, &attach_accept->emergency_number_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_eps_network_feature_support(pkbuf, &attach_accept->eps_network_feature_support);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT) {
        attach_accept->additional_update_result.type = (OGS_NAS_EPS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE >> 4);

        size = ogs_nas_eps_encode_additional_update_result(pkbuf, &attach_accept->additional_update_result);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_3(pkbuf, &attach_accept->t3412_extended_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_T3324_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_T3324_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_2(pkbuf, &attach_accept->t3324_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_drx_parameters(pkbuf, &attach_accept->extended_drx_parameters);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_DCN_ID_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_DCN_ID_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_dcn_id(pkbuf, &attach_accept->dcn_id);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_SMS_SERVICES_STATUS_PRESENT) {
        attach_accept->sms_services_status.type = (OGS_NAS_EPS_ATTACH_ACCEPT_SMS_SERVICES_STATUS_TYPE >> 4);

        size = ogs_nas_eps_encode_sms_services_status(pkbuf, &attach_accept->sms_services_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_NON__NW_PROVIDED_POLICIES_PRESENT) {
        attach_accept->non__nw_provided_policies.type = (OGS_NAS_EPS_ATTACH_ACCEPT_NON__NW_PROVIDED_POLICIES_TYPE >> 4);

        size = ogs_nas_eps_encode_non__nw_provided_policies(pkbuf, &attach_accept->non__nw_provided_policies);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_T3448_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_T3448_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_2(pkbuf, &attach_accept->t3448_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_NETWORK_POLICY_PRESENT) {
        attach_accept->network_policy.type = (OGS_NAS_EPS_ATTACH_ACCEPT_NETWORK_POLICY_TYPE >> 4);

        size = ogs_nas_eps_encode_network_policy(pkbuf, &attach_accept->network_policy);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_T3447_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_T3447_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_3(pkbuf, &attach_accept->t3447_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_emergency_number_list(pkbuf, &attach_accept->extended_emergency_number_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_CIPHERING_KEY_DATA_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_CIPHERING_KEY_DATA_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ciphering_key_data(pkbuf, &attach_accept->ciphering_key_data);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_UE_RADIO_CAPABILITY_ID_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_ACCEPT_UE_RADIO_CAPABILITY_ID_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ue_radio_capability_id(pkbuf, &attach_accept->ue_radio_capability_id);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT) {
        attach_accept->ue_radio_capability_id_deletion_indication.type = (OGS_NAS_EPS_ATTACH_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE >> 4);

        size = ogs_nas_eps_encode_ue_radio_capability_id_deletion_indication(pkbuf, &attach_accept->ue_radio_capability_id_deletion_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_attach_complete(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_attach_complete_t *attach_complete = &message->emm.attach_complete;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode ATTACH_COMPLETE");

    size = ogs_nas_eps_encode_esm_message_container(pkbuf, &attach_complete->esm_message_container);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_eps_encode_attach_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_attach_reject_t *attach_reject = &message->emm.attach_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode ATTACH_REJECT");

    size = ogs_nas_eps_encode_emm_cause(pkbuf, &attach_reject->emm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (attach_reject->presencemask & OGS_NAS_EPS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_esm_message_container(pkbuf, &attach_reject->esm_message_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_reject->presencemask & OGS_NAS_EPS_ATTACH_REJECT_T3346_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REJECT_T3346_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_2(pkbuf, &attach_reject->t3346_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_reject->presencemask & OGS_NAS_EPS_ATTACH_REJECT_T3402_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ATTACH_REJECT_T3402_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_2(pkbuf, &attach_reject->t3402_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (attach_reject->presencemask & OGS_NAS_EPS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_PRESENT) {
        attach_reject->extended_emm_cause.type = (OGS_NAS_EPS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_TYPE >> 4);

        size = ogs_nas_eps_encode_extended_emm_cause(pkbuf, &attach_reject->extended_emm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_detach_request_from_ue(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_detach_request_from_ue_t *detach_request_from_ue = &message->emm.detach_request_from_ue;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode DETACH_REQUEST");

    size = ogs_nas_eps_encode_detach_type(pkbuf, &detach_request_from_ue->detach_type);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_eps_mobile_identity(pkbuf, &detach_request_from_ue->eps_mobile_identity);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_eps_encode_detach_request_to_ue(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_detach_request_to_ue_t *detach_request_to_ue = &message->emm.detach_request_to_ue;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode DETACH_REQUEST");

    size = ogs_nas_eps_encode_detach_type(pkbuf, &detach_request_to_ue->detach_type);
    ogs_assert(size >= 0);
    encoded += size;

    if (detach_request_to_ue->presencemask & OGS_NAS_EPS_DETACH_REQUEST_EMM_CAUSE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_DETACH_REQUEST_EMM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_emm_cause(pkbuf, &detach_request_to_ue->emm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_tracking_area_update_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_tracking_area_update_request_t *tracking_area_update_request = &message->emm.tracking_area_update_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode TRACKING_AREA_UPDATE_REQUEST");

    size = ogs_nas_eps_encode_eps_update_type(pkbuf, &tracking_area_update_request->eps_update_type);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_eps_mobile_identity(pkbuf, &tracking_area_update_request->old_guti);
    ogs_assert(size >= 0);
    encoded += size;

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_PRESENT) {
        tracking_area_update_request->non_current_native_nas_key_set_identifier.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_TYPE >> 4);

        size = ogs_nas_eps_encode_key_set_identifier(pkbuf, &tracking_area_update_request->non_current_native_nas_key_set_identifier);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_GPRS_CIPHERING_KEY_SEQUENCE_NUMBER_PRESENT) {
        tracking_area_update_request->gprs_ciphering_key_sequence_number.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_GPRS_CIPHERING_KEY_SEQUENCE_NUMBER_TYPE >> 4);

        size = ogs_nas_eps_encode_ciphering_key_sequence_number(pkbuf, &tracking_area_update_request->gprs_ciphering_key_sequence_number);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_P_TMSI_SIGNATURE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_p_tmsi_signature(pkbuf, &tracking_area_update_request->old_p_tmsi_signature);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_GUTI_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_GUTI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_eps_mobile_identity(pkbuf, &tracking_area_update_request->additional_guti);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nonce(pkbuf, &tracking_area_update_request->nonceue);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ue_network_capability(pkbuf, &tracking_area_update_request->ue_network_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_tracking_area_identity(pkbuf, &tracking_area_update_request->last_visited_registered_tai);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_drx_parameter(pkbuf, &tracking_area_update_request->drx_parameter);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED_PRESENT) {
        tracking_area_update_request->ue_radio_capability_information_update_needed.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED_TYPE >> 4);

        size = ogs_nas_eps_encode_ue_radio_capability_information_update_needed(pkbuf, &tracking_area_update_request->ue_radio_capability_information_update_needed);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_eps_bearer_context_status(pkbuf, &tracking_area_update_request->eps_bearer_context_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ms_network_capability(pkbuf, &tracking_area_update_request->ms_network_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_location_area_identification(pkbuf, &tracking_area_update_request->old_location_area_identification);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_STATUS_PRESENT) {
        tracking_area_update_request->tmsi_status.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_STATUS_TYPE >> 4);

        size = ogs_nas_eps_encode_tmsi_status(pkbuf, &tracking_area_update_request->tmsi_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_mobile_station_classmark_2(pkbuf, &tracking_area_update_request->mobile_station_classmark_2);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_3_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_mobile_station_classmark_3(pkbuf, &tracking_area_update_request->mobile_station_classmark_3);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_SUPPORTED_CODECS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_SUPPORTED_CODECS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_supported_codec_list(pkbuf, &tracking_area_update_request->supported_codecs);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT) {
        tracking_area_update_request->additional_update_type.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_UPDATE_TYPE_TYPE >> 4);

        size = ogs_nas_eps_encode_additional_update_type(pkbuf, &tracking_area_update_request->additional_update_type);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_voice_domain_preference_and_ue_usage_setting(pkbuf, &tracking_area_update_request->voice_domain_preference_and_ue_usage_setting);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_PRESENT) {
        tracking_area_update_request->old_guti_type.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_TYPE >> 4);

        size = ogs_nas_eps_encode_guti_type(pkbuf, &tracking_area_update_request->old_guti_type);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DEVICE_PROPERTIES_PRESENT) {
        tracking_area_update_request->device_properties.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DEVICE_PROPERTIES_TYPE >> 4);

        size = ogs_nas_eps_encode_device_properties(pkbuf, &tracking_area_update_request->device_properties);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT) {
        tracking_area_update_request->ms_network_feature_support.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_FEATURE_SUPPORT_TYPE >> 4);

        size = ogs_nas_eps_encode_ms_network_feature_support(pkbuf, &tracking_area_update_request->ms_network_feature_support);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_BASED_NRI_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_network_resource_identifier_container(pkbuf, &tracking_area_update_request->tmsi_based_nri_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_T3324_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_T3324_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_2(pkbuf, &tracking_area_update_request->t3324_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_T3412_EXTENDED_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_T3412_EXTENDED_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_3(pkbuf, &tracking_area_update_request->t3412_extended_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_EXTENDED_DRX_PARAMETERS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_EXTENDED_DRX_PARAMETERS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_drx_parameters(pkbuf, &tracking_area_update_request->extended_drx_parameters);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ue_additional_security_capability(pkbuf, &tracking_area_update_request->ue_additional_security_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_STATUS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ue_status(pkbuf, &tracking_area_update_request->ue_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_additional_information_requested(pkbuf, &tracking_area_update_request->additional_information_requested);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_N1_UE_NETWORK_CAPABILITY_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_N1_UE_NETWORK_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_n1_ue_network_capability(pkbuf, &tracking_area_update_request->n1_ue_network_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_tracking_area_update_accept(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_tracking_area_update_accept_t *tracking_area_update_accept = &message->emm.tracking_area_update_accept;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode TRACKING_AREA_UPDATE_ACCEPT");

    size = ogs_nas_eps_encode_eps_update_result(pkbuf, &tracking_area_update_accept->eps_update_result);
    ogs_assert(size >= 0);
    encoded += size;

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer(pkbuf, &tracking_area_update_accept->t3412_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_GUTI_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_GUTI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_eps_mobile_identity(pkbuf, &tracking_area_update_accept->guti);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_tracking_area_identity_list(pkbuf, &tracking_area_update_accept->tai_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_eps_bearer_context_status(pkbuf, &tracking_area_update_accept->eps_bearer_context_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_location_area_identification(pkbuf, &tracking_area_update_accept->location_area_identification);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_MS_IDENTITY_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_MS_IDENTITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_mobile_identity(pkbuf, &tracking_area_update_accept->ms_identity);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EMM_CAUSE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EMM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_emm_cause(pkbuf, &tracking_area_update_accept->emm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer(pkbuf, &tracking_area_update_accept->t3402_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer(pkbuf, &tracking_area_update_accept->t3423_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EQUIVALENT_PLMNS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EQUIVALENT_PLMNS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_plmn_list(pkbuf, &tracking_area_update_accept->equivalent_plmns);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_emergency_number_list(pkbuf, &tracking_area_update_accept->emergency_number_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_eps_network_feature_support(pkbuf, &tracking_area_update_accept->eps_network_feature_support);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT) {
        tracking_area_update_accept->additional_update_result.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE >> 4);

        size = ogs_nas_eps_encode_additional_update_result(pkbuf, &tracking_area_update_accept->additional_update_result);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3412_EXTENDED_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3412_EXTENDED_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_3(pkbuf, &tracking_area_update_accept->t3412_extended_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3324_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3324_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_2(pkbuf, &tracking_area_update_accept->t3324_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_drx_parameters(pkbuf, &tracking_area_update_accept->extended_drx_parameters);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_STATUS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_header_compression_configuration_status(pkbuf, &tracking_area_update_accept->header_compression_configuration_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_DCN_ID_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_DCN_ID_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_dcn_id(pkbuf, &tracking_area_update_accept->dcn_id);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_SMS_SERVICES_STATUS_PRESENT) {
        tracking_area_update_accept->sms_services_status.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_SMS_SERVICES_STATUS_TYPE >> 4);

        size = ogs_nas_eps_encode_sms_services_status(pkbuf, &tracking_area_update_accept->sms_services_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NON__NW_POLICIES_PRESENT) {
        tracking_area_update_accept->non__nw_policies.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NON__NW_POLICIES_TYPE >> 4);

        size = ogs_nas_eps_encode_non__nw_provided_policies(pkbuf, &tracking_area_update_accept->non__nw_policies);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3448_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3448_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_2(pkbuf, &tracking_area_update_accept->t3448_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NETWORK_POLICY_PRESENT) {
        tracking_area_update_accept->network_policy.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NETWORK_POLICY_TYPE >> 4);

        size = ogs_nas_eps_encode_network_policy(pkbuf, &tracking_area_update_accept->network_policy);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3447_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3447_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_3(pkbuf, &tracking_area_update_accept->t3447_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_emergency_number_list(pkbuf, &tracking_area_update_accept->extended_emergency_number_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_CIPHERING_KEY_DATA_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_CIPHERING_KEY_DATA_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ciphering_key_data(pkbuf, &tracking_area_update_accept->ciphering_key_data);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_UE_RADIO_CAPABILITY_ID_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_UE_RADIO_CAPABILITY_ID_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ue_radio_capability_id(pkbuf, &tracking_area_update_accept->ue_radio_capability_id);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT) {
        tracking_area_update_accept->ue_radio_capability_id_deletion_indication.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE >> 4);

        size = ogs_nas_eps_encode_ue_radio_capability_id_deletion_indication(pkbuf, &tracking_area_update_accept->ue_radio_capability_id_deletion_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_tracking_area_update_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_tracking_area_update_reject_t *tracking_area_update_reject = &message->emm.tracking_area_update_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode TRACKING_AREA_UPDATE_REJECT");

    size = ogs_nas_eps_encode_emm_cause(pkbuf, &tracking_area_update_reject->emm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (tracking_area_update_reject->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_T3346_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_T3346_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_2(pkbuf, &tracking_area_update_reject->t3346_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (tracking_area_update_reject->presencemask & OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_EXTENDED_EMM_CAUSE_PRESENT) {
        tracking_area_update_reject->extended_emm_cause.type = (OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_EXTENDED_EMM_CAUSE_TYPE >> 4);

        size = ogs_nas_eps_encode_extended_emm_cause(pkbuf, &tracking_area_update_reject->extended_emm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_extended_service_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_extended_service_request_t *extended_service_request = &message->emm.extended_service_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode EXTENDED_SERVICE_REQUEST");

    size = ogs_nas_eps_encode_service_type(pkbuf, &extended_service_request->service_type);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_mobile_identity(pkbuf, &extended_service_request->m_tmsi);
    ogs_assert(size >= 0);
    encoded += size;

    if (extended_service_request->presencemask & OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_CSFB_RESPONSE_PRESENT) {
        extended_service_request->csfb_response.type = (OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_CSFB_RESPONSE_TYPE >> 4);

        size = ogs_nas_eps_encode_csfb_response(pkbuf, &extended_service_request->csfb_response);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (extended_service_request->presencemask & OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_eps_bearer_context_status(pkbuf, &extended_service_request->eps_bearer_context_status);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (extended_service_request->presencemask & OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_DEVICE_PROPERTIES_PRESENT) {
        extended_service_request->device_properties.type = (OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_DEVICE_PROPERTIES_TYPE >> 4);

        size = ogs_nas_eps_encode_device_properties(pkbuf, &extended_service_request->device_properties);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_service_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_service_request_t *service_request = &message->emm.service_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode SERVICE_REQUEST");

    size = ogs_nas_eps_encode_ksi_and_sequence_number(pkbuf, &service_request->ksi_and_sequence_number);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_short_mac(pkbuf, &service_request->message_authentication_code);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_eps_encode_service_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_service_reject_t *service_reject = &message->emm.service_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode SERVICE_REJECT");

    size = ogs_nas_eps_encode_emm_cause(pkbuf, &service_reject->emm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (service_reject->presencemask & OGS_NAS_EPS_SERVICE_REJECT_T3346_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_SERVICE_REJECT_T3346_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_2(pkbuf, &service_reject->t3346_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (service_reject->presencemask & OGS_NAS_EPS_SERVICE_REJECT_T3448_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_SERVICE_REJECT_T3448_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_2(pkbuf, &service_reject->t3448_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_guti_reallocation_command(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_guti_reallocation_command_t *guti_reallocation_command = &message->emm.guti_reallocation_command;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode GUTI_REALLOCATION_COMMAND");

    size = ogs_nas_eps_encode_eps_mobile_identity(pkbuf, &guti_reallocation_command->guti);
    ogs_assert(size >= 0);
    encoded += size;

    if (guti_reallocation_command->presencemask & OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_TAI_LIST_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_TAI_LIST_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_tracking_area_identity_list(pkbuf, &guti_reallocation_command->tai_list);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (guti_reallocation_command->presencemask & OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_DCN_ID_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_DCN_ID_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_dcn_id(pkbuf, &guti_reallocation_command->dcn_id);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (guti_reallocation_command->presencemask & OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_UE_RADIO_CAPABILITY_ID_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_UE_RADIO_CAPABILITY_ID_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ue_radio_capability_id(pkbuf, &guti_reallocation_command->ue_radio_capability_id);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (guti_reallocation_command->presencemask & OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT) {
        guti_reallocation_command->ue_radio_capability_id_deletion_indication.type = (OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE >> 4);

        size = ogs_nas_eps_encode_ue_radio_capability_id_deletion_indication(pkbuf, &guti_reallocation_command->ue_radio_capability_id_deletion_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_authentication_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_authentication_request_t *authentication_request = &message->emm.authentication_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode AUTHENTICATION_REQUEST");

    size = ogs_nas_eps_encode_key_set_identifier(pkbuf, &authentication_request->nas_key_set_identifierasme);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_authentication_parameter_rand(pkbuf, &authentication_request->authentication_parameter_rand);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_authentication_parameter_autn(pkbuf, &authentication_request->authentication_parameter_autn);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_eps_encode_authentication_response(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_authentication_response_t *authentication_response = &message->emm.authentication_response;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode AUTHENTICATION_RESPONSE");

    size = ogs_nas_eps_encode_authentication_response_parameter(pkbuf, &authentication_response->authentication_response_parameter);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_eps_encode_identity_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_identity_request_t *identity_request = &message->emm.identity_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode IDENTITY_REQUEST");

    size = ogs_nas_eps_encode_identity_type_2(pkbuf, &identity_request->identity_type);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_eps_encode_identity_response(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_identity_response_t *identity_response = &message->emm.identity_response;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode IDENTITY_RESPONSE");

    size = ogs_nas_eps_encode_mobile_identity(pkbuf, &identity_response->mobile_identity);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_eps_encode_authentication_failure(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_authentication_failure_t *authentication_failure = &message->emm.authentication_failure;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode AUTHENTICATION_FAILURE");

    size = ogs_nas_eps_encode_emm_cause(pkbuf, &authentication_failure->emm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (authentication_failure->presencemask & OGS_NAS_EPS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_authentication_failure_parameter(pkbuf, &authentication_failure->authentication_failure_parameter);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_security_mode_command(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_security_mode_command_t *security_mode_command = &message->emm.security_mode_command;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode SECURITY_MODE_COMMAND");

    size = ogs_nas_eps_encode_security_algorithms(pkbuf, &security_mode_command->selected_nas_security_algorithms);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_key_set_identifier(pkbuf, &security_mode_command->nas_key_set_identifier);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_ue_security_capability(pkbuf, &security_mode_command->replayed_ue_security_capabilities);
    ogs_assert(size >= 0);
    encoded += size;

    if (security_mode_command->presencemask & OGS_NAS_EPS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT) {
        security_mode_command->imeisv_request.type = (OGS_NAS_EPS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_TYPE >> 4);

        size = ogs_nas_eps_encode_imeisv_request(pkbuf, &security_mode_command->imeisv_request);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_command->presencemask & OGS_NAS_EPS_SECURITY_MODE_COMMAND_REPLAYED_NONCEUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_SECURITY_MODE_COMMAND_REPLAYED_NONCEUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nonce(pkbuf, &security_mode_command->replayed_nonceue);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_command->presencemask & OGS_NAS_EPS_SECURITY_MODE_COMMAND_NONCEMME_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_SECURITY_MODE_COMMAND_NONCEMME_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nonce(pkbuf, &security_mode_command->noncemme);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_command->presencemask & OGS_NAS_EPS_SECURITY_MODE_COMMAND_HASHMME_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_SECURITY_MODE_COMMAND_HASHMME_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_hashmme(pkbuf, &security_mode_command->hashmme);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_command->presencemask & OGS_NAS_EPS_SECURITY_MODE_COMMAND_REPLAYED_UE_ADDITIONAL_SECURITY_CAPABILITY_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_SECURITY_MODE_COMMAND_REPLAYED_UE_ADDITIONAL_SECURITY_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ue_additional_security_capability(pkbuf, &security_mode_command->replayed_ue_additional_security_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_command->presencemask & OGS_NAS_EPS_SECURITY_MODE_COMMAND_UE_RADIO_CAPABILITY_ID_REQUEST_PRESENT) {
        security_mode_command->ue_radio_capability_id_request.type = (OGS_NAS_EPS_SECURITY_MODE_COMMAND_UE_RADIO_CAPABILITY_ID_REQUEST_TYPE >> 4);

        size = ogs_nas_eps_encode_ue_radio_capability_id_request(pkbuf, &security_mode_command->ue_radio_capability_id_request);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_security_mode_complete(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_security_mode_complete_t *security_mode_complete = &message->emm.security_mode_complete;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode SECURITY_MODE_COMPLETE");

    if (security_mode_complete->presencemask & OGS_NAS_EPS_SECURITY_MODE_COMPLETE_IMEISV_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_SECURITY_MODE_COMPLETE_IMEISV_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_mobile_identity(pkbuf, &security_mode_complete->imeisv);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_complete->presencemask & OGS_NAS_EPS_SECURITY_MODE_COMPLETE_REPLAYED_NAS_MESSAGE_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_SECURITY_MODE_COMPLETE_REPLAYED_NAS_MESSAGE_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_replayed_nas_message_container(pkbuf, &security_mode_complete->replayed_nas_message_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (security_mode_complete->presencemask & OGS_NAS_EPS_SECURITY_MODE_COMPLETE_UE_RADIO_CAPABILITY_ID_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_SECURITY_MODE_COMPLETE_UE_RADIO_CAPABILITY_ID_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ue_radio_capability_id(pkbuf, &security_mode_complete->ue_radio_capability_id);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_security_mode_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_security_mode_reject_t *security_mode_reject = &message->emm.security_mode_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode SECURITY_MODE_REJECT");

    size = ogs_nas_eps_encode_emm_cause(pkbuf, &security_mode_reject->emm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_eps_encode_emm_status(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_emm_status_t *emm_status = &message->emm.emm_status;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode EMM_STATUS");

    size = ogs_nas_eps_encode_emm_cause(pkbuf, &emm_status->emm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_eps_encode_emm_information(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_emm_information_t *emm_information = &message->emm.emm_information;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode EMM_INFORMATION");

    if (emm_information->presencemask & OGS_NAS_EPS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_network_name(pkbuf, &emm_information->full_name_for_network);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (emm_information->presencemask & OGS_NAS_EPS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_network_name(pkbuf, &emm_information->short_name_for_network);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (emm_information->presencemask & OGS_NAS_EPS_EMM_INFORMATION_LOCAL_TIME_ZONE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_EMM_INFORMATION_LOCAL_TIME_ZONE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_time_zone(pkbuf, &emm_information->local_time_zone);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (emm_information->presencemask & OGS_NAS_EPS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_time_zone_and_time(pkbuf, &emm_information->universal_time_and_local_time_zone);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (emm_information->presencemask & OGS_NAS_EPS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_daylight_saving_time(pkbuf, &emm_information->network_daylight_saving_time);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_downlink_nas_transport(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_downlink_nas_transport_t *downlink_nas_transport = &message->emm.downlink_nas_transport;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode DOWNLINK_NAS_TRANSPORT");

    size = ogs_nas_eps_encode_eps_message_container(pkbuf, &downlink_nas_transport->nas_message_container);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_eps_encode_uplink_nas_transport(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_uplink_nas_transport_t *uplink_nas_transport = &message->emm.uplink_nas_transport;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode UPLINK_NAS_TRANSPORT");

    size = ogs_nas_eps_encode_eps_message_container(pkbuf, &uplink_nas_transport->nas_message_container);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

int ogs_nas_eps_encode_cs_service_notification(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_cs_service_notification_t *cs_service_notification = &message->emm.cs_service_notification;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode CS_SERVICE_NOTIFICATION");

    size = ogs_nas_eps_encode_paging_identity(pkbuf, &cs_service_notification->paging_identity);
    ogs_assert(size >= 0);
    encoded += size;

    if (cs_service_notification->presencemask & OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_CLI_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_CLI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_cli(pkbuf, &cs_service_notification->cli);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (cs_service_notification->presencemask & OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_SS_CODE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_SS_CODE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_ss_code(pkbuf, &cs_service_notification->ss_code);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (cs_service_notification->presencemask & OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_LCS_INDICATOR_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_LCS_INDICATOR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_lcs_indicator(pkbuf, &cs_service_notification->lcs_indicator);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (cs_service_notification->presencemask & OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_lcs_client_identity(pkbuf, &cs_service_notification->lcs_client_identity);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_uplink_generic_nas_transport(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_uplink_generic_nas_transport_t *uplink_generic_nas_transport = &message->emm.uplink_generic_nas_transport;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode UPLINK_GENERIC_NAS_TRANSPORT");

    size = ogs_nas_eps_encode_generic_message_container_type(pkbuf, &uplink_generic_nas_transport->generic_message_container_type);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_generic_message_container(pkbuf, &uplink_generic_nas_transport->generic_message_container);
    ogs_assert(size >= 0);
    encoded += size;

    if (uplink_generic_nas_transport->presencemask & OGS_NAS_EPS_UPLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_UPLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_additional_information(pkbuf, &uplink_generic_nas_transport->additional_information);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_downlink_generic_nas_transport(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_downlink_generic_nas_transport_t *downlink_generic_nas_transport = &message->emm.downlink_generic_nas_transport;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode DOWNLINK_GENERIC_NAS_TRANSPORT");

    size = ogs_nas_eps_encode_generic_message_container_type(pkbuf, &downlink_generic_nas_transport->generic_message_container_type);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_generic_message_container(pkbuf, &downlink_generic_nas_transport->generic_message_container);
    ogs_assert(size >= 0);
    encoded += size;

    if (downlink_generic_nas_transport->presencemask & OGS_NAS_EPS_DOWNLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_DOWNLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_additional_information(pkbuf, &downlink_generic_nas_transport->additional_information);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_activate_default_eps_bearer_context_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_activate_default_eps_bearer_context_request_t *activate_default_eps_bearer_context_request = &message->esm.activate_default_eps_bearer_context_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST");

    size = ogs_nas_eps_encode_eps_quality_of_service(pkbuf, &activate_default_eps_bearer_context_request->eps_qos);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_access_point_name(pkbuf, &activate_default_eps_bearer_context_request->access_point_name);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_pdn_address(pkbuf, &activate_default_eps_bearer_context_request->pdn_address);
    ogs_assert(size >= 0);
    encoded += size;

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_transaction_identifier(pkbuf, &activate_default_eps_bearer_context_request->transaction_identifier);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_quality_of_service(pkbuf, &activate_default_eps_bearer_context_request->negotiated_qos);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_llc_service_access_point_identifier(pkbuf, &activate_default_eps_bearer_context_request->negotiated_llc_sapi);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT) {
        activate_default_eps_bearer_context_request->radio_priority.type = (OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE >> 4);

        size = ogs_nas_eps_encode_radio_priority(pkbuf, &activate_default_eps_bearer_context_request->radio_priority);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_packet_flow_identifier(pkbuf, &activate_default_eps_bearer_context_request->packet_flow_identifier);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_apn_aggregate_maximum_bit_rate(pkbuf, &activate_default_eps_bearer_context_request->apn_ambr);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_esm_cause(pkbuf, &activate_default_eps_bearer_context_request->esm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &activate_default_eps_bearer_context_request->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_PRESENT) {
        activate_default_eps_bearer_context_request->connectivity_type.type = (OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_TYPE >> 4);

        size = ogs_nas_eps_encode_connectivity_type(pkbuf, &activate_default_eps_bearer_context_request->connectivity_type);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT) {
        activate_default_eps_bearer_context_request->wlan_offload_indication.type = (OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE >> 4);

        size = ogs_nas_eps_encode_wlan_offload_acceptability(pkbuf, &activate_default_eps_bearer_context_request->wlan_offload_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &activate_default_eps_bearer_context_request->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_header_compression_configuration(pkbuf, &activate_default_eps_bearer_context_request->header_compression_configuration);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_PRESENT) {
        activate_default_eps_bearer_context_request->control_plane_only_indication.type = (OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_TYPE >> 4);

        size = ogs_nas_eps_encode_control_plane_only_indication(pkbuf, &activate_default_eps_bearer_context_request->control_plane_only_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &activate_default_eps_bearer_context_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_serving_plmn_rate_control(pkbuf, &activate_default_eps_bearer_context_request->serving_plmn_rate_control);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_APN_AMBR_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_APN_AMBR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_apn_aggregate_maximum_bit_rate(pkbuf, &activate_default_eps_bearer_context_request->extended_apn_ambr);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_activate_default_eps_bearer_context_accept(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_activate_default_eps_bearer_context_accept_t *activate_default_eps_bearer_context_accept = &message->esm.activate_default_eps_bearer_context_accept;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT");

    if (activate_default_eps_bearer_context_accept->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &activate_default_eps_bearer_context_accept->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_accept->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &activate_default_eps_bearer_context_accept->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_activate_default_eps_bearer_context_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_activate_default_eps_bearer_context_reject_t *activate_default_eps_bearer_context_reject = &message->esm.activate_default_eps_bearer_context_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT");

    size = ogs_nas_eps_encode_esm_cause(pkbuf, &activate_default_eps_bearer_context_reject->esm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (activate_default_eps_bearer_context_reject->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &activate_default_eps_bearer_context_reject->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_default_eps_bearer_context_reject->presencemask & OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &activate_default_eps_bearer_context_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_activate_dedicated_eps_bearer_context_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_activate_dedicated_eps_bearer_context_request_t *activate_dedicated_eps_bearer_context_request = &message->esm.activate_dedicated_eps_bearer_context_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST");

    size = ogs_nas_eps_encode_linked_eps_bearer_identity(pkbuf, &activate_dedicated_eps_bearer_context_request->linked_eps_bearer_identity);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_eps_quality_of_service(pkbuf, &activate_dedicated_eps_bearer_context_request->eps_qos);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_traffic_flow_template(pkbuf, &activate_dedicated_eps_bearer_context_request->tft);
    ogs_assert(size >= 0);
    encoded += size;

    if (activate_dedicated_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_transaction_identifier(pkbuf, &activate_dedicated_eps_bearer_context_request->transaction_identifier);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_quality_of_service(pkbuf, &activate_dedicated_eps_bearer_context_request->negotiated_qos);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_llc_service_access_point_identifier(pkbuf, &activate_dedicated_eps_bearer_context_request->negotiated_llc_sapi);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT) {
        activate_dedicated_eps_bearer_context_request->radio_priority.type = (OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE >> 4);

        size = ogs_nas_eps_encode_radio_priority(pkbuf, &activate_dedicated_eps_bearer_context_request->radio_priority);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_packet_flow_identifier(pkbuf, &activate_dedicated_eps_bearer_context_request->packet_flow_identifier);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &activate_dedicated_eps_bearer_context_request->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT) {
        activate_dedicated_eps_bearer_context_request->wlan_offload_indication.type = (OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE >> 4);

        size = ogs_nas_eps_encode_wlan_offload_acceptability(pkbuf, &activate_dedicated_eps_bearer_context_request->wlan_offload_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &activate_dedicated_eps_bearer_context_request->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &activate_dedicated_eps_bearer_context_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_EPS_QOS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_EPS_QOS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_quality_of_service(pkbuf, &activate_dedicated_eps_bearer_context_request->extended_eps_qos);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_activate_dedicated_eps_bearer_context_accept(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_activate_dedicated_eps_bearer_context_accept_t *activate_dedicated_eps_bearer_context_accept = &message->esm.activate_dedicated_eps_bearer_context_accept;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT");

    if (activate_dedicated_eps_bearer_context_accept->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &activate_dedicated_eps_bearer_context_accept->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_accept->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &activate_dedicated_eps_bearer_context_accept->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_accept->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &activate_dedicated_eps_bearer_context_accept->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_activate_dedicated_eps_bearer_context_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_activate_dedicated_eps_bearer_context_reject_t *activate_dedicated_eps_bearer_context_reject = &message->esm.activate_dedicated_eps_bearer_context_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT");

    size = ogs_nas_eps_encode_esm_cause(pkbuf, &activate_dedicated_eps_bearer_context_reject->esm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (activate_dedicated_eps_bearer_context_reject->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &activate_dedicated_eps_bearer_context_reject->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_reject->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &activate_dedicated_eps_bearer_context_reject->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_reject->presencemask & OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &activate_dedicated_eps_bearer_context_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_modify_eps_bearer_context_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_modify_eps_bearer_context_request_t *modify_eps_bearer_context_request = &message->esm.modify_eps_bearer_context_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode MODIFY_EPS_BEARER_CONTEXT_REQUEST");

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_EPS_QOS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_EPS_QOS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_eps_quality_of_service(pkbuf, &modify_eps_bearer_context_request->new_eps_qos);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_TFT_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_TFT_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_traffic_flow_template(pkbuf, &modify_eps_bearer_context_request->tft);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_QOS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_QOS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_quality_of_service(pkbuf, &modify_eps_bearer_context_request->new_qos);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_llc_service_access_point_identifier(pkbuf, &modify_eps_bearer_context_request->negotiated_llc_sapi);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT) {
        modify_eps_bearer_context_request->radio_priority.type = (OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE >> 4);

        size = ogs_nas_eps_encode_radio_priority(pkbuf, &modify_eps_bearer_context_request->radio_priority);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_packet_flow_identifier(pkbuf, &modify_eps_bearer_context_request->packet_flow_identifier);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_apn_aggregate_maximum_bit_rate(pkbuf, &modify_eps_bearer_context_request->apn_ambr);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &modify_eps_bearer_context_request->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT) {
        modify_eps_bearer_context_request->wlan_offload_indication.type = (OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE >> 4);

        size = ogs_nas_eps_encode_wlan_offload_acceptability(pkbuf, &modify_eps_bearer_context_request->wlan_offload_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &modify_eps_bearer_context_request->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_header_compression_configuration(pkbuf, &modify_eps_bearer_context_request->header_compression_configuration);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &modify_eps_bearer_context_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_APN_AMBR_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_APN_AMBR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_apn_aggregate_maximum_bit_rate(pkbuf, &modify_eps_bearer_context_request->extended_apn_ambr);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_EPS_QOS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_EPS_QOS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_quality_of_service(pkbuf, &modify_eps_bearer_context_request->extended_eps_qos);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_modify_eps_bearer_context_accept(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_modify_eps_bearer_context_accept_t *modify_eps_bearer_context_accept = &message->esm.modify_eps_bearer_context_accept;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode MODIFY_EPS_BEARER_CONTEXT_ACCEPT");

    if (modify_eps_bearer_context_accept->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &modify_eps_bearer_context_accept->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_accept->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &modify_eps_bearer_context_accept->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_accept->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &modify_eps_bearer_context_accept->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_modify_eps_bearer_context_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_modify_eps_bearer_context_reject_t *modify_eps_bearer_context_reject = &message->esm.modify_eps_bearer_context_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode MODIFY_EPS_BEARER_CONTEXT_REJECT");

    size = ogs_nas_eps_encode_esm_cause(pkbuf, &modify_eps_bearer_context_reject->esm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (modify_eps_bearer_context_reject->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &modify_eps_bearer_context_reject->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_reject->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &modify_eps_bearer_context_reject->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (modify_eps_bearer_context_reject->presencemask & OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &modify_eps_bearer_context_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_deactivate_eps_bearer_context_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_deactivate_eps_bearer_context_request_t *deactivate_eps_bearer_context_request = &message->esm.deactivate_eps_bearer_context_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST");

    size = ogs_nas_eps_encode_esm_cause(pkbuf, &deactivate_eps_bearer_context_request->esm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (deactivate_eps_bearer_context_request->presencemask & OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &deactivate_eps_bearer_context_request->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (deactivate_eps_bearer_context_request->presencemask & OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_T3396_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_T3396_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_3(pkbuf, &deactivate_eps_bearer_context_request->t3396_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (deactivate_eps_bearer_context_request->presencemask & OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT) {
        deactivate_eps_bearer_context_request->wlan_offload_indication.type = (OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE >> 4);

        size = ogs_nas_eps_encode_wlan_offload_acceptability(pkbuf, &deactivate_eps_bearer_context_request->wlan_offload_indication);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (deactivate_eps_bearer_context_request->presencemask & OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &deactivate_eps_bearer_context_request->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (deactivate_eps_bearer_context_request->presencemask & OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &deactivate_eps_bearer_context_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_deactivate_eps_bearer_context_accept(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_deactivate_eps_bearer_context_accept_t *deactivate_eps_bearer_context_accept = &message->esm.deactivate_eps_bearer_context_accept;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT");

    if (deactivate_eps_bearer_context_accept->presencemask & OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &deactivate_eps_bearer_context_accept->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (deactivate_eps_bearer_context_accept->presencemask & OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &deactivate_eps_bearer_context_accept->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_pdn_connectivity_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_pdn_connectivity_request_t *pdn_connectivity_request = &message->esm.pdn_connectivity_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDN_CONNECTIVITY_REQUEST");

    size = ogs_nas_eps_encode_request_type(pkbuf, &pdn_connectivity_request->request_type);
    ogs_assert(size >= 0);
    encoded += size;

    if (pdn_connectivity_request->presencemask & OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_PRESENT) {
        pdn_connectivity_request->esm_information_transfer_flag.type = (OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_TYPE >> 4);

        size = ogs_nas_eps_encode_esm_information_transfer_flag(pkbuf, &pdn_connectivity_request->esm_information_transfer_flag);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdn_connectivity_request->presencemask & OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_access_point_name(pkbuf, &pdn_connectivity_request->access_point_name);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdn_connectivity_request->presencemask & OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &pdn_connectivity_request->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdn_connectivity_request->presencemask & OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_DEVICE_PROPERTIES_PRESENT) {
        pdn_connectivity_request->device_properties.type = (OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_DEVICE_PROPERTIES_TYPE >> 4);

        size = ogs_nas_eps_encode_device_properties(pkbuf, &pdn_connectivity_request->device_properties);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdn_connectivity_request->presencemask & OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &pdn_connectivity_request->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdn_connectivity_request->presencemask & OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_header_compression_configuration(pkbuf, &pdn_connectivity_request->header_compression_configuration);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdn_connectivity_request->presencemask & OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &pdn_connectivity_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_pdn_connectivity_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_pdn_connectivity_reject_t *pdn_connectivity_reject = &message->esm.pdn_connectivity_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDN_CONNECTIVITY_REJECT");

    size = ogs_nas_eps_encode_esm_cause(pkbuf, &pdn_connectivity_reject->esm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (pdn_connectivity_reject->presencemask & OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &pdn_connectivity_reject->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdn_connectivity_reject->presencemask & OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_BACK_OFF_TIMER_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_BACK_OFF_TIMER_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_3(pkbuf, &pdn_connectivity_reject->back_off_timer_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdn_connectivity_reject->presencemask & OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_RE_ATTEMPT_INDICATOR_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_RE_ATTEMPT_INDICATOR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_re_attempt_indicator(pkbuf, &pdn_connectivity_reject->re_attempt_indicator);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdn_connectivity_reject->presencemask & OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &pdn_connectivity_reject->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdn_connectivity_reject->presencemask & OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &pdn_connectivity_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_pdn_disconnect_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_pdn_disconnect_request_t *pdn_disconnect_request = &message->esm.pdn_disconnect_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDN_DISCONNECT_REQUEST");

    size = ogs_nas_eps_encode_linked_eps_bearer_identity(pkbuf, &pdn_disconnect_request->linked_eps_bearer_identity);
    ogs_assert(size >= 0);
    encoded += size;

    if (pdn_disconnect_request->presencemask & OGS_NAS_EPS_PDN_DISCONNECT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_DISCONNECT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &pdn_disconnect_request->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdn_disconnect_request->presencemask & OGS_NAS_EPS_PDN_DISCONNECT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_DISCONNECT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &pdn_disconnect_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_pdn_disconnect_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_pdn_disconnect_reject_t *pdn_disconnect_reject = &message->esm.pdn_disconnect_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode PDN_DISCONNECT_REJECT");

    size = ogs_nas_eps_encode_esm_cause(pkbuf, &pdn_disconnect_reject->esm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (pdn_disconnect_reject->presencemask & OGS_NAS_EPS_PDN_DISCONNECT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_DISCONNECT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &pdn_disconnect_reject->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdn_disconnect_reject->presencemask & OGS_NAS_EPS_PDN_DISCONNECT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_PDN_DISCONNECT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &pdn_disconnect_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_bearer_resource_allocation_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_bearer_resource_allocation_request_t *bearer_resource_allocation_request = &message->esm.bearer_resource_allocation_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode BEARER_RESOURCE_ALLOCATION_REQUEST");

    size = ogs_nas_eps_encode_linked_eps_bearer_identity(pkbuf, &bearer_resource_allocation_request->linked_eps_bearer_identity);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_traffic_flow_aggregate_description(pkbuf, &bearer_resource_allocation_request->traffic_flow_aggregate);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_eps_quality_of_service(pkbuf, &bearer_resource_allocation_request->required_traffic_flow_qos);
    ogs_assert(size >= 0);
    encoded += size;

    if (bearer_resource_allocation_request->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &bearer_resource_allocation_request->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_allocation_request->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_DEVICE_PROPERTIES_PRESENT) {
        bearer_resource_allocation_request->device_properties.type = (OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_DEVICE_PROPERTIES_TYPE >> 4);

        size = ogs_nas_eps_encode_device_properties(pkbuf, &bearer_resource_allocation_request->device_properties);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_allocation_request->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &bearer_resource_allocation_request->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_allocation_request->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &bearer_resource_allocation_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_allocation_request->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_EPS_QOS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_EPS_QOS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_quality_of_service(pkbuf, &bearer_resource_allocation_request->extended_eps_qos);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_bearer_resource_allocation_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_bearer_resource_allocation_reject_t *bearer_resource_allocation_reject = &message->esm.bearer_resource_allocation_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode BEARER_RESOURCE_ALLOCATION_REJECT");

    size = ogs_nas_eps_encode_esm_cause(pkbuf, &bearer_resource_allocation_reject->esm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (bearer_resource_allocation_reject->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &bearer_resource_allocation_reject->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_allocation_reject->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_3(pkbuf, &bearer_resource_allocation_reject->back_off_timer_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_allocation_reject->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_re_attempt_indicator(pkbuf, &bearer_resource_allocation_reject->re_attempt_indicator);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_allocation_reject->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &bearer_resource_allocation_reject->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_allocation_reject->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &bearer_resource_allocation_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_bearer_resource_modification_request(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_bearer_resource_modification_request_t *bearer_resource_modification_request = &message->esm.bearer_resource_modification_request;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode BEARER_RESOURCE_MODIFICATION_REQUEST");

    size = ogs_nas_eps_encode_linked_eps_bearer_identity(pkbuf, &bearer_resource_modification_request->eps_bearer_identity_for_packet_filter);
    ogs_assert(size >= 0);
    encoded += size;

    size = ogs_nas_eps_encode_traffic_flow_aggregate_description(pkbuf, &bearer_resource_modification_request->traffic_flow_aggregate);
    ogs_assert(size >= 0);
    encoded += size;

    if (bearer_resource_modification_request->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_eps_quality_of_service(pkbuf, &bearer_resource_modification_request->required_traffic_flow_qos);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_modification_request->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_ESM_CAUSE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_ESM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_esm_cause(pkbuf, &bearer_resource_modification_request->esm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_modification_request->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &bearer_resource_modification_request->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_modification_request->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_DEVICE_PROPERTIES_PRESENT) {
        bearer_resource_modification_request->device_properties.type = (OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_DEVICE_PROPERTIES_TYPE >> 4);

        size = ogs_nas_eps_encode_device_properties(pkbuf, &bearer_resource_modification_request->device_properties);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_modification_request->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &bearer_resource_modification_request->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_modification_request->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_header_compression_configuration(pkbuf, &bearer_resource_modification_request->header_compression_configuration);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_modification_request->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &bearer_resource_modification_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_modification_request->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_EPS_QOS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_EPS_QOS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_quality_of_service(pkbuf, &bearer_resource_modification_request->extended_eps_qos);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_bearer_resource_modification_reject(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_bearer_resource_modification_reject_t *bearer_resource_modification_reject = &message->esm.bearer_resource_modification_reject;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode BEARER_RESOURCE_MODIFICATION_REJECT");

    size = ogs_nas_eps_encode_esm_cause(pkbuf, &bearer_resource_modification_reject->esm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    if (bearer_resource_modification_reject->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &bearer_resource_modification_reject->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_modification_reject->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_gprs_timer_3(pkbuf, &bearer_resource_modification_reject->back_off_timer_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_modification_reject->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_re_attempt_indicator(pkbuf, &bearer_resource_modification_reject->re_attempt_indicator);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_modification_reject->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_NBIFOM_CONTAINER_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_NBIFOM_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_nbifom_container(pkbuf, &bearer_resource_modification_reject->nbifom_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (bearer_resource_modification_reject->presencemask & OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &bearer_resource_modification_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_esm_information_response(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_esm_information_response_t *esm_information_response = &message->esm.esm_information_response;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode ESM_INFORMATION_RESPONSE");

    if (esm_information_response->presencemask & OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_access_point_name(pkbuf, &esm_information_response->access_point_name);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (esm_information_response->presencemask & OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_protocol_configuration_options(pkbuf, &esm_information_response->protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (esm_information_response->presencemask & OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_eps_encode_optional_type(pkbuf, OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_eps_encode_extended_protocol_configuration_options(pkbuf, &esm_information_response->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

int ogs_nas_eps_encode_esm_status(ogs_pkbuf_t *pkbuf, ogs_nas_eps_message_t *message)
{
    ogs_nas_eps_esm_status_t *esm_status = &message->esm.esm_status;
    int encoded = 0;
    int size = 0;

    ogs_trace("[NAS] Encode ESM_STATUS");

    size = ogs_nas_eps_encode_esm_cause(pkbuf, &esm_status->esm_cause);
    ogs_assert(size >= 0);
    encoded += size;

    return encoded;
}

ogs_pkbuf_t *ogs_nas_emm_encode(ogs_nas_eps_message_t *message)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int size = 0;
    int encoded = 0;

    ogs_assert(message);

    /* The Packet Buffer(ogs_pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-OGS_NAS_HEADROOM);

    size = sizeof(ogs_nas_emm_header_t);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));

    memcpy(pkbuf->data - size, &message->emm.h, size);
    encoded += size;

    if (message->emm.h.security_header_type >=
            OGS_NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE) {
        ogs_assert(ogs_pkbuf_push(pkbuf, 1));
        encoded -= 1;
        size = ogs_nas_eps_encode_service_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;

        goto out;
    }

    switch (message->emm.h.message_type) {
    case OGS_NAS_EPS_ATTACH_REQUEST:
        size = ogs_nas_eps_encode_attach_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_ATTACH_ACCEPT:
        size = ogs_nas_eps_encode_attach_accept(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_ATTACH_COMPLETE:
        size = ogs_nas_eps_encode_attach_complete(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_ATTACH_REJECT:
        size = ogs_nas_eps_encode_attach_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_DETACH_REQUEST:
        size = ogs_nas_eps_encode_detach_request_to_ue(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_DETACH_ACCEPT:
        break;
    case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST:
        size = ogs_nas_eps_encode_tracking_area_update_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT:
        size = ogs_nas_eps_encode_tracking_area_update_accept(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_TRACKING_AREA_UPDATE_COMPLETE:
        break;
    case OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT:
        size = ogs_nas_eps_encode_tracking_area_update_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST:
        size = ogs_nas_eps_encode_extended_service_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_SERVICE_REJECT:
        size = ogs_nas_eps_encode_service_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND:
        size = ogs_nas_eps_encode_guti_reallocation_command(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_GUTI_REALLOCATION_COMPLETE:
        break;
    case OGS_NAS_EPS_AUTHENTICATION_REQUEST:
        size = ogs_nas_eps_encode_authentication_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_AUTHENTICATION_RESPONSE:
        size = ogs_nas_eps_encode_authentication_response(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_AUTHENTICATION_REJECT:
        break;
    case OGS_NAS_EPS_IDENTITY_REQUEST:
        size = ogs_nas_eps_encode_identity_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_IDENTITY_RESPONSE:
        size = ogs_nas_eps_encode_identity_response(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_AUTHENTICATION_FAILURE:
        size = ogs_nas_eps_encode_authentication_failure(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_SECURITY_MODE_COMMAND:
        size = ogs_nas_eps_encode_security_mode_command(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_SECURITY_MODE_COMPLETE:
        size = ogs_nas_eps_encode_security_mode_complete(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_SECURITY_MODE_REJECT:
        size = ogs_nas_eps_encode_security_mode_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_EMM_STATUS:
        size = ogs_nas_eps_encode_emm_status(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_EMM_INFORMATION:
        size = ogs_nas_eps_encode_emm_information(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_DOWNLINK_NAS_TRANSPORT:
        size = ogs_nas_eps_encode_downlink_nas_transport(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_UPLINK_NAS_TRANSPORT:
        size = ogs_nas_eps_encode_uplink_nas_transport(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_CS_SERVICE_NOTIFICATION:
        size = ogs_nas_eps_encode_cs_service_notification(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_UPLINK_GENERIC_NAS_TRANSPORT:
        size = ogs_nas_eps_encode_uplink_generic_nas_transport(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_DOWNLINK_GENERIC_NAS_TRANSPORT:
        size = ogs_nas_eps_encode_downlink_generic_nas_transport(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    default:
        ogs_error("Unknown message type (0x%x) or not implemented", 
               message->emm.h.message_type);
        ogs_pkbuf_free(pkbuf);
        return NULL;
    }

out:
    ogs_assert(ogs_pkbuf_push(pkbuf, encoded));

    pkbuf->len = encoded;

    return pkbuf;
}

ogs_pkbuf_t *ogs_nas_esm_encode(ogs_nas_eps_message_t *message)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int size = 0;
    int encoded = 0;

    ogs_assert(message);

    /* The Packet Buffer(ogs_pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-OGS_NAS_HEADROOM);

    size = sizeof(ogs_nas_esm_header_t);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &message->esm.h, size);
    encoded += size;

    switch (message->esm.h.message_type) {
    case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST:
        size = ogs_nas_eps_encode_activate_default_eps_bearer_context_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT:
        size = ogs_nas_eps_encode_activate_default_eps_bearer_context_accept(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT:
        size = ogs_nas_eps_encode_activate_default_eps_bearer_context_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST:
        size = ogs_nas_eps_encode_activate_dedicated_eps_bearer_context_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT:
        size = ogs_nas_eps_encode_activate_dedicated_eps_bearer_context_accept(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT:
        size = ogs_nas_eps_encode_activate_dedicated_eps_bearer_context_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST:
        size = ogs_nas_eps_encode_modify_eps_bearer_context_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT:
        size = ogs_nas_eps_encode_modify_eps_bearer_context_accept(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT:
        size = ogs_nas_eps_encode_modify_eps_bearer_context_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST:
        size = ogs_nas_eps_encode_deactivate_eps_bearer_context_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT:
        size = ogs_nas_eps_encode_deactivate_eps_bearer_context_accept(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST:
        size = ogs_nas_eps_encode_pdn_connectivity_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT:
        size = ogs_nas_eps_encode_pdn_connectivity_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_PDN_DISCONNECT_REQUEST:
        size = ogs_nas_eps_encode_pdn_disconnect_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_PDN_DISCONNECT_REJECT:
        size = ogs_nas_eps_encode_pdn_disconnect_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST:
        size = ogs_nas_eps_encode_bearer_resource_allocation_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT:
        size = ogs_nas_eps_encode_bearer_resource_allocation_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST:
        size = ogs_nas_eps_encode_bearer_resource_modification_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT:
        size = ogs_nas_eps_encode_bearer_resource_modification_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_ESM_INFORMATION_REQUEST:
        break;
    case OGS_NAS_EPS_ESM_INFORMATION_RESPONSE:
        size = ogs_nas_eps_encode_esm_information_response(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_EPS_ESM_STATUS:
        size = ogs_nas_eps_encode_esm_status(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
   default:
        ogs_error("Unknown message type (0x%x) or not implemented", 
                message->esm.h.message_type);
        ogs_pkbuf_free(pkbuf);
        return NULL;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, encoded));
    pkbuf->len = encoded;

    return pkbuf;
}

ogs_pkbuf_t *ogs_nas_eps_plain_encode(ogs_nas_eps_message_t *message)
{
    ogs_assert(message);

    ogs_assert(message->emm.h.protocol_discriminator ==
            message->esm.h.protocol_discriminator);

    if (message->emm.h.protocol_discriminator == 
            OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM)
        return ogs_nas_emm_encode(message);
    else if (message->emm.h.protocol_discriminator == 
            OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM)
        return ogs_nas_esm_encode(message);

    return NULL;
}
