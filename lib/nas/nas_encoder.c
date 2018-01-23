/*
 * Copyright (c) 2017, NextEPC Group
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*******************************************************************************
 * This file had been created by gtpv2c_tlv.py script v0.1.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2018-01-23 15:58:42.903985 by acetcom
 * from 24301-d80.docx
 ******************************************************************************/

#define TRACE_MODULE _nas_encoder

#include "core_debug.h"
#include "nas_message.h"

c_int32_t nas_encode_attach_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_attach_request_t *attach_request = &message->emm.attach_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode ATTACH_REQUEST\n");

    size = nas_encode_eps_attach_type(pkbuf, &attach_request->eps_attach_type);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_eps_mobile_identity(pkbuf, &attach_request->eps_mobile_identity);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_ue_network_capability(pkbuf, &attach_request->ue_network_capability);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_esm_message_container(pkbuf, &attach_request->esm_message_container);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_p_tmsi_signature(pkbuf, &attach_request->old_p_tmsi_signature);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_ADDITIONAL_GUTI_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_ADDITIONAL_GUTI_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_eps_mobile_identity(pkbuf, &attach_request->additional_guti);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_tracking_area_identity(pkbuf, &attach_request->last_visited_registered_tai);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_DRX_PARAMETER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_DRX_PARAMETER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_drx_parameter(pkbuf, &attach_request->drx_parameter);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_ms_network_capability(pkbuf, &attach_request->ms_network_capability);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_location_area_identification(pkbuf, &attach_request->old_location_area_identification);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_TMSI_STATUS_PRESENT)
    {
        attach_request->tmsi_status.type = (NAS_ATTACH_REQUEST_TMSI_STATUS_TYPE >> 4);

        size = nas_encode_tmsi_status(pkbuf, &attach_request->tmsi_status);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_mobile_station_classmark_2(pkbuf, &attach_request->mobile_station_classmark_2);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_mobile_station_classmark_3(pkbuf, &attach_request->mobile_station_classmark_3);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_SUPPORTED_CODECS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_SUPPORTED_CODECS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_supported_codec_list(pkbuf, &attach_request->supported_codecs);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT)
    {
        attach_request->additional_update_type.type = (NAS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_TYPE >> 4);

        size = nas_encode_additional_update_type(pkbuf, &attach_request->additional_update_type);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_voice_domain_preference_and_ue_usage_setting(pkbuf, &attach_request->voice_domain_preference_and_ue_usage_setting);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_DEVICE_PROPERTIES_PRESENT)
    {
        attach_request->device_properties.type = (NAS_ATTACH_REQUEST_DEVICE_PROPERTIES_TYPE >> 4);

        size = nas_encode_device_properties(pkbuf, &attach_request->device_properties);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_OLD_GUTI_TYPE_PRESENT)
    {
        attach_request->old_guti_type.type = (NAS_ATTACH_REQUEST_OLD_GUTI_TYPE_TYPE >> 4);

        size = nas_encode_guti_type(pkbuf, &attach_request->old_guti_type);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT)
    {
        attach_request->ms_network_feature_support.type = (NAS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_TYPE >> 4);

        size = nas_encode_ms_network_feature_support(pkbuf, &attach_request->ms_network_feature_support);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_network_resource_identifier_container(pkbuf, &attach_request->tmsi_based_nri_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_T3324_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_T3324_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_2(pkbuf, &attach_request->t3324_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_3(pkbuf, &attach_request->t3412_extended_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_request->presencemask & NAS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_drx_parameters(pkbuf, &attach_request->extended_drx_parameters);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_attach_accept(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_attach_accept_t *attach_accept = &message->emm.attach_accept;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode ATTACH_ACCEPT\n");

    size = nas_encode_eps_attach_result(pkbuf, &attach_accept->eps_attach_result);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_gprs_timer(pkbuf, &attach_accept->t3412_value);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_tracking_area_identity_list(pkbuf, &attach_accept->tai_list);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_esm_message_container(pkbuf, &attach_accept->esm_message_container);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_GUTI_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_ACCEPT_GUTI_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_eps_mobile_identity(pkbuf, &attach_accept->guti);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_location_area_identification(pkbuf, &attach_accept->location_area_identification);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_MS_IDENTITY_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_ACCEPT_MS_IDENTITY_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_mobile_identity(pkbuf, &attach_accept->ms_identity);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_EMM_CAUSE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_ACCEPT_EMM_CAUSE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_emm_cause(pkbuf, &attach_accept->emm_cause);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_T3402_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_ACCEPT_T3402_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer(pkbuf, &attach_accept->t3402_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_T3423_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_ACCEPT_T3423_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer(pkbuf, &attach_accept->t3423_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_plmn_list(pkbuf, &attach_accept->equivalent_plmns);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_emergency_number_list(pkbuf, &attach_accept->emergency_number_list);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_eps_network_feature_support(pkbuf, &attach_accept->eps_network_feature_support);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT)
    {
        attach_accept->additional_update_result.type = (NAS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE >> 4);

        size = nas_encode_additional_update_result(pkbuf, &attach_accept->additional_update_result);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_3(pkbuf, &attach_accept->t3412_extended_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_T3324_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_ACCEPT_T3324_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_2(pkbuf, &attach_accept->t3324_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_drx_parameters(pkbuf, &attach_accept->extended_drx_parameters);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_attach_complete(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_attach_complete_t *attach_complete = &message->emm.attach_complete;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode ATTACH_COMPLETE\n");

    size = nas_encode_esm_message_container(pkbuf, &attach_complete->esm_message_container);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    return encoded;
}

c_int32_t nas_encode_attach_reject(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_attach_reject_t *attach_reject = &message->emm.attach_reject;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode ATTACH_REJECT\n");

    size = nas_encode_emm_cause(pkbuf, &attach_reject->emm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (attach_reject->presencemask & NAS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_esm_message_container(pkbuf, &attach_reject->esm_message_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_reject->presencemask & NAS_ATTACH_REJECT_T3346_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REJECT_T3346_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_2(pkbuf, &attach_reject->t3346_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_reject->presencemask & NAS_ATTACH_REJECT_T3402_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_REJECT_T3402_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_2(pkbuf, &attach_reject->t3402_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_reject->presencemask & NAS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_PRESENT)
    {
        attach_reject->extended_emm_cause.type = (NAS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_TYPE >> 4);

        size = nas_encode_extended_emm_cause(pkbuf, &attach_reject->extended_emm_cause);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_detach_request_from_ue(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_detach_request_from_ue_t *detach_request_from_ue = &message->emm.detach_request_from_ue;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode DETACH_REQUEST\n");

    size = nas_encode_detach_type(pkbuf, &detach_request_from_ue->detach_type);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_eps_mobile_identity(pkbuf, &detach_request_from_ue->eps_mobile_identity);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    return encoded;
}

c_int32_t nas_encode_detach_request_to_ue(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_detach_request_to_ue_t *detach_request_to_ue = &message->emm.detach_request_to_ue;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode DETACH_REQUEST\n");

    size = nas_encode_detach_type(pkbuf, &detach_request_to_ue->detach_type);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (detach_request_to_ue->presencemask & NAS_DETACH_REQUEST_EMM_CAUSE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_DETACH_REQUEST_EMM_CAUSE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_emm_cause(pkbuf, &detach_request_to_ue->emm_cause);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_tracking_area_update_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_tracking_area_update_request_t *tracking_area_update_request = &message->emm.tracking_area_update_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode TRACKING_AREA_UPDATE_REQUEST\n");

    size = nas_encode_eps_update_type(pkbuf, &tracking_area_update_request->eps_update_type);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_eps_mobile_identity(pkbuf, &tracking_area_update_request->old_guti);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_PRESENT)
    {
        tracking_area_update_request->non_current_native_nas_key_set_identifier.type = (NAS_TRACKING_AREA_UPDATE_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_TYPE >> 4);

        size = nas_encode_key_set_identifier(pkbuf, &tracking_area_update_request->non_current_native_nas_key_set_identifier);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_GPRS_CIPHERING_KEY_SEQUENCE_NUMBER_PRESENT)
    {
        tracking_area_update_request->gprs_ciphering_key_sequence_number.type = (NAS_TRACKING_AREA_UPDATE_REQUEST_GPRS_CIPHERING_KEY_SEQUENCE_NUMBER_TYPE >> 4);

        size = nas_encode_ciphering_key_sequence_number(pkbuf, &tracking_area_update_request->gprs_ciphering_key_sequence_number);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_P_TMSI_SIGNATURE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_p_tmsi_signature(pkbuf, &tracking_area_update_request->old_p_tmsi_signature);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_GUTI_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_GUTI_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_eps_mobile_identity(pkbuf, &tracking_area_update_request->additional_guti);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nonce(pkbuf, &tracking_area_update_request->nonceue);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_ue_network_capability(pkbuf, &tracking_area_update_request->ue_network_capability);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_tracking_area_identity(pkbuf, &tracking_area_update_request->last_visited_registered_tai);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_drx_parameter(pkbuf, &tracking_area_update_request->drx_parameter);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED_PRESENT)
    {
        tracking_area_update_request->ue_radio_capability_information_update_needed.type = (NAS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED_TYPE >> 4);

        size = nas_encode_ue_radio_capability_information_update_needed(pkbuf, &tracking_area_update_request->ue_radio_capability_information_update_needed);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_eps_bearer_context_status(pkbuf, &tracking_area_update_request->eps_bearer_context_status);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_ms_network_capability(pkbuf, &tracking_area_update_request->ms_network_capability);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_location_area_identification(pkbuf, &tracking_area_update_request->old_location_area_identification);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_TMSI_STATUS_PRESENT)
    {
        tracking_area_update_request->tmsi_status.type = (NAS_TRACKING_AREA_UPDATE_REQUEST_TMSI_STATUS_TYPE >> 4);

        size = nas_encode_tmsi_status(pkbuf, &tracking_area_update_request->tmsi_status);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_mobile_station_classmark_2(pkbuf, &tracking_area_update_request->mobile_station_classmark_2);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_3_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_mobile_station_classmark_3(pkbuf, &tracking_area_update_request->mobile_station_classmark_3);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_SUPPORTED_CODECS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_SUPPORTED_CODECS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_supported_codec_list(pkbuf, &tracking_area_update_request->supported_codecs);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT)
    {
        tracking_area_update_request->additional_update_type.type = (NAS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_UPDATE_TYPE_TYPE >> 4);

        size = nas_encode_additional_update_type(pkbuf, &tracking_area_update_request->additional_update_type);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_voice_domain_preference_and_ue_usage_setting(pkbuf, &tracking_area_update_request->voice_domain_preference_and_ue_usage_setting);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_PRESENT)
    {
        tracking_area_update_request->old_guti_type.type = (NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_TYPE >> 4);

        size = nas_encode_guti_type(pkbuf, &tracking_area_update_request->old_guti_type);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_DEVICE_PROPERTIES_PRESENT)
    {
        tracking_area_update_request->device_properties.type = (NAS_TRACKING_AREA_UPDATE_REQUEST_DEVICE_PROPERTIES_TYPE >> 4);

        size = nas_encode_device_properties(pkbuf, &tracking_area_update_request->device_properties);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT)
    {
        tracking_area_update_request->ms_network_feature_support.type = (NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_FEATURE_SUPPORT_TYPE >> 4);

        size = nas_encode_ms_network_feature_support(pkbuf, &tracking_area_update_request->ms_network_feature_support);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_TMSI_BASED_NRI_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_network_resource_identifier_container(pkbuf, &tracking_area_update_request->tmsi_based_nri_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_T3324_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_T3324_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_2(pkbuf, &tracking_area_update_request->t3324_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_T3412_EXTENDED_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_T3412_EXTENDED_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_3(pkbuf, &tracking_area_update_request->t3412_extended_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_request->presencemask & NAS_TRACKING_AREA_UPDATE_REQUEST_EXTENDED_DRX_PARAMETERS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REQUEST_EXTENDED_DRX_PARAMETERS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_drx_parameters(pkbuf, &tracking_area_update_request->extended_drx_parameters);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_tracking_area_update_accept(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_tracking_area_update_accept_t *tracking_area_update_accept = &message->emm.tracking_area_update_accept;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode TRACKING_AREA_UPDATE_ACCEPT\n");

    size = nas_encode_eps_update_result(pkbuf, &tracking_area_update_accept->eps_update_result);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer(pkbuf, &tracking_area_update_accept->t3412_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_GUTI_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_GUTI_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_eps_mobile_identity(pkbuf, &tracking_area_update_accept->guti);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_tracking_area_identity_list(pkbuf, &tracking_area_update_accept->tai_list);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_eps_bearer_context_status(pkbuf, &tracking_area_update_accept->eps_bearer_context_status);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_location_area_identification(pkbuf, &tracking_area_update_accept->location_area_identification);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_MS_IDENTITY_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_MS_IDENTITY_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_mobile_identity(pkbuf, &tracking_area_update_accept->ms_identity);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_EMM_CAUSE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_EMM_CAUSE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_emm_cause(pkbuf, &tracking_area_update_accept->emm_cause);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer(pkbuf, &tracking_area_update_accept->t3402_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer(pkbuf, &tracking_area_update_accept->t3423_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_EQUIVALENT_PLMNS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_EQUIVALENT_PLMNS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_plmn_list(pkbuf, &tracking_area_update_accept->equivalent_plmns);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_emergency_number_list(pkbuf, &tracking_area_update_accept->emergency_number_list);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_eps_network_feature_support(pkbuf, &tracking_area_update_accept->eps_network_feature_support);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT)
    {
        tracking_area_update_accept->additional_update_result.type = (NAS_TRACKING_AREA_UPDATE_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE >> 4);

        size = nas_encode_additional_update_result(pkbuf, &tracking_area_update_accept->additional_update_result);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_EXTENDED_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_EXTENDED_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_3(pkbuf, &tracking_area_update_accept->t3412_extended_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_T3324_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_T3324_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_2(pkbuf, &tracking_area_update_accept->t3324_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_drx_parameters(pkbuf, &tracking_area_update_accept->extended_drx_parameters);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_accept->presencemask & NAS_TRACKING_AREA_UPDATE_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_STATUS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_STATUS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_header_compression_configuration_status(pkbuf, &tracking_area_update_accept->header_compression_configuration_status);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_tracking_area_update_reject(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_tracking_area_update_reject_t *tracking_area_update_reject = &message->emm.tracking_area_update_reject;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode TRACKING_AREA_UPDATE_REJECT\n");

    size = nas_encode_emm_cause(pkbuf, &tracking_area_update_reject->emm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (tracking_area_update_reject->presencemask & NAS_TRACKING_AREA_UPDATE_REJECT_T3346_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_TRACKING_AREA_UPDATE_REJECT_T3346_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_2(pkbuf, &tracking_area_update_reject->t3346_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (tracking_area_update_reject->presencemask & NAS_TRACKING_AREA_UPDATE_REJECT_EXTENDED_EMM_CAUSE_PRESENT)
    {
        tracking_area_update_reject->extended_emm_cause.type = (NAS_TRACKING_AREA_UPDATE_REJECT_EXTENDED_EMM_CAUSE_TYPE >> 4);

        size = nas_encode_extended_emm_cause(pkbuf, &tracking_area_update_reject->extended_emm_cause);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_extended_service_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_extended_service_request_t *extended_service_request = &message->emm.extended_service_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode EXTENDED_SERVICE_REQUEST\n");

    size = nas_encode_service_type(pkbuf, &extended_service_request->service_type);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_mobile_identity(pkbuf, &extended_service_request->m_tmsi);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (extended_service_request->presencemask & NAS_EXTENDED_SERVICE_REQUEST_CSFB_RESPONSE_PRESENT)
    {
        extended_service_request->csfb_response.type = (NAS_EXTENDED_SERVICE_REQUEST_CSFB_RESPONSE_TYPE >> 4);

        size = nas_encode_csfb_response(pkbuf, &extended_service_request->csfb_response);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (extended_service_request->presencemask & NAS_EXTENDED_SERVICE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_EXTENDED_SERVICE_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_eps_bearer_context_status(pkbuf, &extended_service_request->eps_bearer_context_status);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (extended_service_request->presencemask & NAS_EXTENDED_SERVICE_REQUEST_DEVICE_PROPERTIES_PRESENT)
    {
        extended_service_request->device_properties.type = (NAS_EXTENDED_SERVICE_REQUEST_DEVICE_PROPERTIES_TYPE >> 4);

        size = nas_encode_device_properties(pkbuf, &extended_service_request->device_properties);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_service_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_service_request_t *service_request = &message->emm.service_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode SERVICE_REQUEST\n");

    size = nas_encode_ksi_and_sequence_number(pkbuf, &service_request->ksi_and_sequence_number);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_short_mac(pkbuf, &service_request->message_authentication_code);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    return encoded;
}

c_int32_t nas_encode_service_reject(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_service_reject_t *service_reject = &message->emm.service_reject;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode SERVICE_REJECT\n");

    size = nas_encode_emm_cause(pkbuf, &service_reject->emm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (service_reject->presencemask & NAS_SERVICE_REJECT_T3346_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_SERVICE_REJECT_T3346_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_2(pkbuf, &service_reject->t3346_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_guti_reallocation_command(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_guti_reallocation_command_t *guti_reallocation_command = &message->emm.guti_reallocation_command;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode GUTI_REALLOCATION_COMMAND\n");

    size = nas_encode_eps_mobile_identity(pkbuf, &guti_reallocation_command->guti);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (guti_reallocation_command->presencemask & NAS_GUTI_REALLOCATION_COMMAND_TAI_LIST_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_GUTI_REALLOCATION_COMMAND_TAI_LIST_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_tracking_area_identity_list(pkbuf, &guti_reallocation_command->tai_list);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_authentication_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_authentication_request_t *authentication_request = &message->emm.authentication_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode AUTHENTICATION_REQUEST\n");

    size = nas_encode_key_set_identifier(pkbuf, &authentication_request->nas_key_set_identifierasme);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_authentication_parameter_rand(pkbuf, &authentication_request->authentication_parameter_rand);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_authentication_parameter_autn(pkbuf, &authentication_request->authentication_parameter_autn);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    return encoded;
}

c_int32_t nas_encode_authentication_response(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_authentication_response_t *authentication_response = &message->emm.authentication_response;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode AUTHENTICATION_RESPONSE\n");

    size = nas_encode_authentication_response_parameter(pkbuf, &authentication_response->authentication_response_parameter);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    return encoded;
}

c_int32_t nas_encode_identity_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_identity_request_t *identity_request = &message->emm.identity_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode IDENTITY_REQUEST\n");

    size = nas_encode_identity_type_2(pkbuf, &identity_request->identity_type);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    return encoded;
}

c_int32_t nas_encode_identity_response(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_identity_response_t *identity_response = &message->emm.identity_response;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode IDENTITY_RESPONSE\n");

    size = nas_encode_mobile_identity(pkbuf, &identity_response->mobile_identity);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    return encoded;
}

c_int32_t nas_encode_authentication_failure(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_authentication_failure_t *authentication_failure = &message->emm.authentication_failure;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode AUTHENTICATION_FAILURE\n");

    size = nas_encode_emm_cause(pkbuf, &authentication_failure->emm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (authentication_failure->presencemask & NAS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_authentication_failure_parameter(pkbuf, &authentication_failure->authentication_failure_parameter);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_security_mode_command(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_security_mode_command_t *security_mode_command = &message->emm.security_mode_command;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode SECURITY_MODE_COMMAND\n");

    size = nas_encode_security_algorithms(pkbuf, &security_mode_command->selected_nas_security_algorithms);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_key_set_identifier(pkbuf, &security_mode_command->nas_key_set_identifier);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_ue_security_capability(pkbuf, &security_mode_command->replayed_ue_security_capabilities);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (security_mode_command->presencemask & NAS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT)
    {
        security_mode_command->imeisv_request.type = (NAS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_TYPE >> 4);

        size = nas_encode_imeisv_request(pkbuf, &security_mode_command->imeisv_request);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (security_mode_command->presencemask & NAS_SECURITY_MODE_COMMAND_REPLAYED_NONCEUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_SECURITY_MODE_COMMAND_REPLAYED_NONCEUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nonce(pkbuf, &security_mode_command->replayed_nonceue);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (security_mode_command->presencemask & NAS_SECURITY_MODE_COMMAND_NONCEMME_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_SECURITY_MODE_COMMAND_NONCEMME_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nonce(pkbuf, &security_mode_command->noncemme);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_security_mode_complete(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_security_mode_complete_t *security_mode_complete = &message->emm.security_mode_complete;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode SECURITY_MODE_COMPLETE\n");

    if (security_mode_complete->presencemask & NAS_SECURITY_MODE_COMPLETE_IMEISV_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_SECURITY_MODE_COMPLETE_IMEISV_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_mobile_identity(pkbuf, &security_mode_complete->imeisv);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_security_mode_reject(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_security_mode_reject_t *security_mode_reject = &message->emm.security_mode_reject;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode SECURITY_MODE_REJECT\n");

    size = nas_encode_emm_cause(pkbuf, &security_mode_reject->emm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    return encoded;
}

c_int32_t nas_encode_emm_status(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_emm_status_t *emm_status = &message->emm.emm_status;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode EMM_STATUS\n");

    size = nas_encode_emm_cause(pkbuf, &emm_status->emm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    return encoded;
}

c_int32_t nas_encode_emm_information(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_emm_information_t *emm_information = &message->emm.emm_information;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode EMM_INFORMATION\n");

    if (emm_information->presencemask & NAS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_network_name(pkbuf, &emm_information->full_name_for_network);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (emm_information->presencemask & NAS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_network_name(pkbuf, &emm_information->short_name_for_network);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (emm_information->presencemask & NAS_EMM_INFORMATION_LOCAL_TIME_ZONE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_EMM_INFORMATION_LOCAL_TIME_ZONE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_time_zone(pkbuf, &emm_information->local_time_zone);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (emm_information->presencemask & NAS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_time_zone_and_time(pkbuf, &emm_information->universal_time_and_local_time_zone);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (emm_information->presencemask & NAS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_daylight_saving_time(pkbuf, &emm_information->network_daylight_saving_time);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_downlink_nas_transport(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_downlink_nas_transport_t *downlink_nas_transport = &message->emm.downlink_nas_transport;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode DOWNLINK_NAS_TRANSPORT\n");

    size = nas_encode_message_container(pkbuf, &downlink_nas_transport->nas_message_container);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    return encoded;
}

c_int32_t nas_encode_uplink_nas_transport(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_uplink_nas_transport_t *uplink_nas_transport = &message->emm.uplink_nas_transport;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode UPLINK_NAS_TRANSPORT\n");

    size = nas_encode_message_container(pkbuf, &uplink_nas_transport->nas_message_container);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    return encoded;
}

c_int32_t nas_encode_cs_service_notification(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_cs_service_notification_t *cs_service_notification = &message->emm.cs_service_notification;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode CS_SERVICE_NOTIFICATION\n");

    size = nas_encode_paging_identity(pkbuf, &cs_service_notification->paging_identity);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (cs_service_notification->presencemask & NAS_CS_SERVICE_NOTIFICATION_CLI_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_CS_SERVICE_NOTIFICATION_CLI_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_cli(pkbuf, &cs_service_notification->cli);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (cs_service_notification->presencemask & NAS_CS_SERVICE_NOTIFICATION_SS_CODE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_CS_SERVICE_NOTIFICATION_SS_CODE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_ss_code(pkbuf, &cs_service_notification->ss_code);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (cs_service_notification->presencemask & NAS_CS_SERVICE_NOTIFICATION_LCS_INDICATOR_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_CS_SERVICE_NOTIFICATION_LCS_INDICATOR_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_lcs_indicator(pkbuf, &cs_service_notification->lcs_indicator);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (cs_service_notification->presencemask & NAS_CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_lcs_client_identity(pkbuf, &cs_service_notification->lcs_client_identity);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_uplink_generic_nas_transport(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_uplink_generic_nas_transport_t *uplink_generic_nas_transport = &message->emm.uplink_generic_nas_transport;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode UPLINK_GENERIC_NAS_TRANSPORT\n");

    size = nas_encode_generic_message_container_type(pkbuf, &uplink_generic_nas_transport->generic_message_container_type);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_generic_message_container(pkbuf, &uplink_generic_nas_transport->generic_message_container);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (uplink_generic_nas_transport->presencemask & NAS_UPLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_UPLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_additional_information(pkbuf, &uplink_generic_nas_transport->additional_information);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_downlink_generic_nas_transport(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_downlink_generic_nas_transport_t *downlink_generic_nas_transport = &message->emm.downlink_generic_nas_transport;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode DOWNLINK_GENERIC_NAS_TRANSPORT\n");

    size = nas_encode_generic_message_container_type(pkbuf, &downlink_generic_nas_transport->generic_message_container_type);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_generic_message_container(pkbuf, &downlink_generic_nas_transport->generic_message_container);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (downlink_generic_nas_transport->presencemask & NAS_DOWNLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_DOWNLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_additional_information(pkbuf, &downlink_generic_nas_transport->additional_information);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_activate_default_eps_bearer_context_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_activate_default_eps_bearer_context_request_t *activate_default_eps_bearer_context_request = &message->esm.activate_default_eps_bearer_context_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST\n");

    size = nas_encode_eps_quality_of_service(pkbuf, &activate_default_eps_bearer_context_request->eps_qos);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_access_point_name(pkbuf, &activate_default_eps_bearer_context_request->access_point_name);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_pdn_address(pkbuf, &activate_default_eps_bearer_context_request->pdn_address);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_transaction_identifier(pkbuf, &activate_default_eps_bearer_context_request->transaction_identifier);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_quality_of_service(pkbuf, &activate_default_eps_bearer_context_request->negotiated_qos);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_llc_service_access_point_identifier(pkbuf, &activate_default_eps_bearer_context_request->negotiated_llc_sapi);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT)
    {
        activate_default_eps_bearer_context_request->radio_priority.type = (NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE >> 4);

        size = nas_encode_radio_priority(pkbuf, &activate_default_eps_bearer_context_request->radio_priority);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_packet_flow_identifier(pkbuf, &activate_default_eps_bearer_context_request->packet_flow_identifier);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_apn_aggregate_maximum_bit_rate(pkbuf, &activate_default_eps_bearer_context_request->apn_ambr);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_esm_cause(pkbuf, &activate_default_eps_bearer_context_request->esm_cause);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &activate_default_eps_bearer_context_request->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_PRESENT)
    {
        activate_default_eps_bearer_context_request->connectivity_type.type = (NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_TYPE >> 4);

        size = nas_encode_connectivity_type(pkbuf, &activate_default_eps_bearer_context_request->connectivity_type);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT)
    {
        activate_default_eps_bearer_context_request->wlan_offload_indication.type = (NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE >> 4);

        size = nas_encode_wlan_offload_acceptability(pkbuf, &activate_default_eps_bearer_context_request->wlan_offload_indication);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &activate_default_eps_bearer_context_request->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_header_compression_configuration(pkbuf, &activate_default_eps_bearer_context_request->header_compression_configuration);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_PRESENT)
    {
        activate_default_eps_bearer_context_request->control_plane_only_indication.type = (NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_TYPE >> 4);

        size = nas_encode_control_plane_only_indication(pkbuf, &activate_default_eps_bearer_context_request->control_plane_only_indication);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &activate_default_eps_bearer_context_request->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_serving_plmn_rate_control(pkbuf, &activate_default_eps_bearer_context_request->serving_plmn_rate_control);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_activate_default_eps_bearer_context_accept(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_activate_default_eps_bearer_context_accept_t *activate_default_eps_bearer_context_accept = &message->esm.activate_default_eps_bearer_context_accept;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT\n");

    if (activate_default_eps_bearer_context_accept->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &activate_default_eps_bearer_context_accept->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_accept->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &activate_default_eps_bearer_context_accept->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_activate_default_eps_bearer_context_reject(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_activate_default_eps_bearer_context_reject_t *activate_default_eps_bearer_context_reject = &message->esm.activate_default_eps_bearer_context_reject;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT\n");

    size = nas_encode_esm_cause(pkbuf, &activate_default_eps_bearer_context_reject->esm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (activate_default_eps_bearer_context_reject->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &activate_default_eps_bearer_context_reject->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_default_eps_bearer_context_reject->presencemask & NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &activate_default_eps_bearer_context_reject->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_activate_dedicated_eps_bearer_context_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_activate_dedicated_eps_bearer_context_request_t *activate_dedicated_eps_bearer_context_request = &message->esm.activate_dedicated_eps_bearer_context_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST\n");

    size = nas_encode_linked_eps_bearer_identity(pkbuf, &activate_dedicated_eps_bearer_context_request->linked_eps_bearer_identity);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_eps_quality_of_service(pkbuf, &activate_dedicated_eps_bearer_context_request->eps_qos);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_traffic_flow_template(pkbuf, &activate_dedicated_eps_bearer_context_request->tft);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (activate_dedicated_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_transaction_identifier(pkbuf, &activate_dedicated_eps_bearer_context_request->transaction_identifier);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_quality_of_service(pkbuf, &activate_dedicated_eps_bearer_context_request->negotiated_qos);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_llc_service_access_point_identifier(pkbuf, &activate_dedicated_eps_bearer_context_request->negotiated_llc_sapi);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT)
    {
        activate_dedicated_eps_bearer_context_request->radio_priority.type = (NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE >> 4);

        size = nas_encode_radio_priority(pkbuf, &activate_dedicated_eps_bearer_context_request->radio_priority);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_packet_flow_identifier(pkbuf, &activate_dedicated_eps_bearer_context_request->packet_flow_identifier);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &activate_dedicated_eps_bearer_context_request->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT)
    {
        activate_dedicated_eps_bearer_context_request->wlan_offload_indication.type = (NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE >> 4);

        size = nas_encode_wlan_offload_acceptability(pkbuf, &activate_dedicated_eps_bearer_context_request->wlan_offload_indication);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &activate_dedicated_eps_bearer_context_request->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_request->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &activate_dedicated_eps_bearer_context_request->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_activate_dedicated_eps_bearer_context_accept(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_activate_dedicated_eps_bearer_context_accept_t *activate_dedicated_eps_bearer_context_accept = &message->esm.activate_dedicated_eps_bearer_context_accept;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT\n");

    if (activate_dedicated_eps_bearer_context_accept->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &activate_dedicated_eps_bearer_context_accept->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_accept->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &activate_dedicated_eps_bearer_context_accept->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_accept->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &activate_dedicated_eps_bearer_context_accept->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_activate_dedicated_eps_bearer_context_reject(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_activate_dedicated_eps_bearer_context_reject_t *activate_dedicated_eps_bearer_context_reject = &message->esm.activate_dedicated_eps_bearer_context_reject;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT\n");

    size = nas_encode_esm_cause(pkbuf, &activate_dedicated_eps_bearer_context_reject->esm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (activate_dedicated_eps_bearer_context_reject->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &activate_dedicated_eps_bearer_context_reject->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_reject->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &activate_dedicated_eps_bearer_context_reject->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (activate_dedicated_eps_bearer_context_reject->presencemask & NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &activate_dedicated_eps_bearer_context_reject->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_modify_eps_bearer_context_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_modify_eps_bearer_context_request_t *modify_eps_bearer_context_request = &message->esm.modify_eps_bearer_context_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode MODIFY_EPS_BEARER_CONTEXT_REQUEST\n");

    if (modify_eps_bearer_context_request->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_EPS_QOS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_EPS_QOS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_eps_quality_of_service(pkbuf, &modify_eps_bearer_context_request->new_eps_qos);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_TFT_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_TFT_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_traffic_flow_template(pkbuf, &modify_eps_bearer_context_request->tft);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_QOS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_QOS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_quality_of_service(pkbuf, &modify_eps_bearer_context_request->new_qos);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_llc_service_access_point_identifier(pkbuf, &modify_eps_bearer_context_request->negotiated_llc_sapi);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT)
    {
        modify_eps_bearer_context_request->radio_priority.type = (NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE >> 4);

        size = nas_encode_radio_priority(pkbuf, &modify_eps_bearer_context_request->radio_priority);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_packet_flow_identifier(pkbuf, &modify_eps_bearer_context_request->packet_flow_identifier);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_apn_aggregate_maximum_bit_rate(pkbuf, &modify_eps_bearer_context_request->apn_ambr);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &modify_eps_bearer_context_request->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT)
    {
        modify_eps_bearer_context_request->wlan_offload_indication.type = (NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE >> 4);

        size = nas_encode_wlan_offload_acceptability(pkbuf, &modify_eps_bearer_context_request->wlan_offload_indication);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &modify_eps_bearer_context_request->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_header_compression_configuration(pkbuf, &modify_eps_bearer_context_request->header_compression_configuration);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_request->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &modify_eps_bearer_context_request->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_modify_eps_bearer_context_accept(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_modify_eps_bearer_context_accept_t *modify_eps_bearer_context_accept = &message->esm.modify_eps_bearer_context_accept;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode MODIFY_EPS_BEARER_CONTEXT_ACCEPT\n");

    if (modify_eps_bearer_context_accept->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &modify_eps_bearer_context_accept->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_accept->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &modify_eps_bearer_context_accept->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_accept->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &modify_eps_bearer_context_accept->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_modify_eps_bearer_context_reject(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_modify_eps_bearer_context_reject_t *modify_eps_bearer_context_reject = &message->esm.modify_eps_bearer_context_reject;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode MODIFY_EPS_BEARER_CONTEXT_REJECT\n");

    size = nas_encode_esm_cause(pkbuf, &modify_eps_bearer_context_reject->esm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (modify_eps_bearer_context_reject->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &modify_eps_bearer_context_reject->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_reject->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &modify_eps_bearer_context_reject->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (modify_eps_bearer_context_reject->presencemask & NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &modify_eps_bearer_context_reject->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_deactivate_eps_bearer_context_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_deactivate_eps_bearer_context_request_t *deactivate_eps_bearer_context_request = &message->esm.deactivate_eps_bearer_context_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST\n");

    size = nas_encode_esm_cause(pkbuf, &deactivate_eps_bearer_context_request->esm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (deactivate_eps_bearer_context_request->presencemask & NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &deactivate_eps_bearer_context_request->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (deactivate_eps_bearer_context_request->presencemask & NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_T3396_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_T3396_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_3(pkbuf, &deactivate_eps_bearer_context_request->t3396_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (deactivate_eps_bearer_context_request->presencemask & NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT)
    {
        deactivate_eps_bearer_context_request->wlan_offload_indication.type = (NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE >> 4);

        size = nas_encode_wlan_offload_acceptability(pkbuf, &deactivate_eps_bearer_context_request->wlan_offload_indication);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (deactivate_eps_bearer_context_request->presencemask & NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &deactivate_eps_bearer_context_request->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (deactivate_eps_bearer_context_request->presencemask & NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &deactivate_eps_bearer_context_request->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_deactivate_eps_bearer_context_accept(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_deactivate_eps_bearer_context_accept_t *deactivate_eps_bearer_context_accept = &message->esm.deactivate_eps_bearer_context_accept;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT\n");

    if (deactivate_eps_bearer_context_accept->presencemask & NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &deactivate_eps_bearer_context_accept->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (deactivate_eps_bearer_context_accept->presencemask & NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &deactivate_eps_bearer_context_accept->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_pdn_connectivity_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_pdn_connectivity_request_t *pdn_connectivity_request = &message->esm.pdn_connectivity_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode PDN_CONNECTIVITY_REQUEST\n");

    size = nas_encode_request_type(pkbuf, &pdn_connectivity_request->request_type);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (pdn_connectivity_request->presencemask & NAS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_PRESENT)
    {
        pdn_connectivity_request->esm_information_transfer_flag.type = (NAS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_TYPE >> 4);

        size = nas_encode_esm_information_transfer_flag(pkbuf, &pdn_connectivity_request->esm_information_transfer_flag);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (pdn_connectivity_request->presencemask & NAS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_access_point_name(pkbuf, &pdn_connectivity_request->access_point_name);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (pdn_connectivity_request->presencemask & NAS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &pdn_connectivity_request->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (pdn_connectivity_request->presencemask & NAS_PDN_CONNECTIVITY_REQUEST_DEVICE_PROPERTIES_PRESENT)
    {
        pdn_connectivity_request->device_properties.type = (NAS_PDN_CONNECTIVITY_REQUEST_DEVICE_PROPERTIES_TYPE >> 4);

        size = nas_encode_device_properties(pkbuf, &pdn_connectivity_request->device_properties);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (pdn_connectivity_request->presencemask & NAS_PDN_CONNECTIVITY_REQUEST_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_CONNECTIVITY_REQUEST_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &pdn_connectivity_request->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (pdn_connectivity_request->presencemask & NAS_PDN_CONNECTIVITY_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_CONNECTIVITY_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_header_compression_configuration(pkbuf, &pdn_connectivity_request->header_compression_configuration);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (pdn_connectivity_request->presencemask & NAS_PDN_CONNECTIVITY_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_CONNECTIVITY_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &pdn_connectivity_request->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_pdn_connectivity_reject(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_pdn_connectivity_reject_t *pdn_connectivity_reject = &message->esm.pdn_connectivity_reject;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode PDN_CONNECTIVITY_REJECT\n");

    size = nas_encode_esm_cause(pkbuf, &pdn_connectivity_reject->esm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (pdn_connectivity_reject->presencemask & NAS_PDN_CONNECTIVITY_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_CONNECTIVITY_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &pdn_connectivity_reject->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (pdn_connectivity_reject->presencemask & NAS_PDN_CONNECTIVITY_REJECT_BACK_OFF_TIMER_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_CONNECTIVITY_REJECT_BACK_OFF_TIMER_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_3(pkbuf, &pdn_connectivity_reject->back_off_timer_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (pdn_connectivity_reject->presencemask & NAS_PDN_CONNECTIVITY_REJECT_RE_ATTEMPT_INDICATOR_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_CONNECTIVITY_REJECT_RE_ATTEMPT_INDICATOR_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_re_attempt_indicator(pkbuf, &pdn_connectivity_reject->re_attempt_indicator);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (pdn_connectivity_reject->presencemask & NAS_PDN_CONNECTIVITY_REJECT_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_CONNECTIVITY_REJECT_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &pdn_connectivity_reject->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (pdn_connectivity_reject->presencemask & NAS_PDN_CONNECTIVITY_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_CONNECTIVITY_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &pdn_connectivity_reject->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_pdn_disconnect_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_pdn_disconnect_request_t *pdn_disconnect_request = &message->esm.pdn_disconnect_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode PDN_DISCONNECT_REQUEST\n");

    size = nas_encode_linked_eps_bearer_identity(pkbuf, &pdn_disconnect_request->linked_eps_bearer_identity);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (pdn_disconnect_request->presencemask & NAS_PDN_DISCONNECT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_DISCONNECT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &pdn_disconnect_request->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (pdn_disconnect_request->presencemask & NAS_PDN_DISCONNECT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_DISCONNECT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &pdn_disconnect_request->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_pdn_disconnect_reject(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_pdn_disconnect_reject_t *pdn_disconnect_reject = &message->esm.pdn_disconnect_reject;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode PDN_DISCONNECT_REJECT\n");

    size = nas_encode_esm_cause(pkbuf, &pdn_disconnect_reject->esm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (pdn_disconnect_reject->presencemask & NAS_PDN_DISCONNECT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_DISCONNECT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &pdn_disconnect_reject->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (pdn_disconnect_reject->presencemask & NAS_PDN_DISCONNECT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_PDN_DISCONNECT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &pdn_disconnect_reject->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_bearer_resource_allocation_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_bearer_resource_allocation_request_t *bearer_resource_allocation_request = &message->esm.bearer_resource_allocation_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode BEARER_RESOURCE_ALLOCATION_REQUEST\n");

    size = nas_encode_linked_eps_bearer_identity(pkbuf, &bearer_resource_allocation_request->linked_eps_bearer_identity);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_traffic_flow_aggregate_description(pkbuf, &bearer_resource_allocation_request->traffic_flow_aggregate);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_eps_quality_of_service(pkbuf, &bearer_resource_allocation_request->required_traffic_flow_qos);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (bearer_resource_allocation_request->presencemask & NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &bearer_resource_allocation_request->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_allocation_request->presencemask & NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_DEVICE_PROPERTIES_PRESENT)
    {
        bearer_resource_allocation_request->device_properties.type = (NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_DEVICE_PROPERTIES_TYPE >> 4);

        size = nas_encode_device_properties(pkbuf, &bearer_resource_allocation_request->device_properties);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_allocation_request->presencemask & NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &bearer_resource_allocation_request->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_allocation_request->presencemask & NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &bearer_resource_allocation_request->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_bearer_resource_allocation_reject(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_bearer_resource_allocation_reject_t *bearer_resource_allocation_reject = &message->esm.bearer_resource_allocation_reject;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode BEARER_RESOURCE_ALLOCATION_REJECT\n");

    size = nas_encode_esm_cause(pkbuf, &bearer_resource_allocation_reject->esm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (bearer_resource_allocation_reject->presencemask & NAS_BEARER_RESOURCE_ALLOCATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_ALLOCATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &bearer_resource_allocation_reject->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_allocation_reject->presencemask & NAS_BEARER_RESOURCE_ALLOCATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_ALLOCATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_3(pkbuf, &bearer_resource_allocation_reject->back_off_timer_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_allocation_reject->presencemask & NAS_BEARER_RESOURCE_ALLOCATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_ALLOCATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_re_attempt_indicator(pkbuf, &bearer_resource_allocation_reject->re_attempt_indicator);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_allocation_reject->presencemask & NAS_BEARER_RESOURCE_ALLOCATION_REJECT_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_ALLOCATION_REJECT_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &bearer_resource_allocation_reject->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_allocation_reject->presencemask & NAS_BEARER_RESOURCE_ALLOCATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_ALLOCATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &bearer_resource_allocation_reject->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_bearer_resource_modification_request(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_bearer_resource_modification_request_t *bearer_resource_modification_request = &message->esm.bearer_resource_modification_request;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode BEARER_RESOURCE_MODIFICATION_REQUEST\n");

    size = nas_encode_linked_eps_bearer_identity(pkbuf, &bearer_resource_modification_request->eps_bearer_identity_for_packet_filter);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    size = nas_encode_traffic_flow_aggregate_description(pkbuf, &bearer_resource_modification_request->traffic_flow_aggregate);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (bearer_resource_modification_request->presencemask & NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_eps_quality_of_service(pkbuf, &bearer_resource_modification_request->required_traffic_flow_qos);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_modification_request->presencemask & NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_ESM_CAUSE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_ESM_CAUSE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_esm_cause(pkbuf, &bearer_resource_modification_request->esm_cause);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_modification_request->presencemask & NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &bearer_resource_modification_request->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_modification_request->presencemask & NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_DEVICE_PROPERTIES_PRESENT)
    {
        bearer_resource_modification_request->device_properties.type = (NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_DEVICE_PROPERTIES_TYPE >> 4);

        size = nas_encode_device_properties(pkbuf, &bearer_resource_modification_request->device_properties);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_modification_request->presencemask & NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &bearer_resource_modification_request->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_modification_request->presencemask & NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_header_compression_configuration(pkbuf, &bearer_resource_modification_request->header_compression_configuration);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_modification_request->presencemask & NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &bearer_resource_modification_request->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_bearer_resource_modification_reject(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_bearer_resource_modification_reject_t *bearer_resource_modification_reject = &message->esm.bearer_resource_modification_reject;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode BEARER_RESOURCE_MODIFICATION_REJECT\n");

    size = nas_encode_esm_cause(pkbuf, &bearer_resource_modification_reject->esm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    if (bearer_resource_modification_reject->presencemask & NAS_BEARER_RESOURCE_MODIFICATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_MODIFICATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &bearer_resource_modification_reject->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_modification_reject->presencemask & NAS_BEARER_RESOURCE_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_3(pkbuf, &bearer_resource_modification_reject->back_off_timer_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_modification_reject->presencemask & NAS_BEARER_RESOURCE_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_re_attempt_indicator(pkbuf, &bearer_resource_modification_reject->re_attempt_indicator);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_modification_reject->presencemask & NAS_BEARER_RESOURCE_MODIFICATION_REJECT_NBIFOM_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_MODIFICATION_REJECT_NBIFOM_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_nbifom_container(pkbuf, &bearer_resource_modification_reject->nbifom_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (bearer_resource_modification_reject->presencemask & NAS_BEARER_RESOURCE_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_BEARER_RESOURCE_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &bearer_resource_modification_reject->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_esm_information_response(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_esm_information_response_t *esm_information_response = &message->esm.esm_information_response;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode ESM_INFORMATION_RESPONSE\n");

    if (esm_information_response->presencemask & NAS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_access_point_name(pkbuf, &esm_information_response->access_point_name);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (esm_information_response->presencemask & NAS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_protocol_configuration_options(pkbuf, &esm_information_response->protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (esm_information_response->presencemask & NAS_ESM_INFORMATION_RESPONSE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ESM_INFORMATION_RESPONSE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_protocol_configuration_options(pkbuf, &esm_information_response->extended_protocol_configuration_options);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_esm_status(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_esm_status_t *esm_status = &message->esm.esm_status;
    c_int32_t encoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Encode ESM_STATUS\n");

    size = nas_encode_esm_cause(pkbuf, &esm_status->esm_cause);
    d_assert(size >= 0, return -1, "encode failed");
    encoded += size;

    return encoded;
}

status_t nas_emm_encode(pkbuf_t **pkbuf, nas_message_t *message)
{
    status_t rv = CORE_ERROR;
    c_int32_t size = 0;
    c_int32_t encoded = 0;

    d_assert(message, return CORE_ERROR, "Null param");

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    *pkbuf = pkbuf_alloc(NAS_HEADROOM, MAX_SDU_LEN);
    d_assert(*pkbuf, return -1, "Null Param");

    size = sizeof(nas_emm_header_t);
    rv = pkbuf_header(*pkbuf, -size);
    d_assert(rv == CORE_OK, return CORE_ERROR, "pkbuf_header error");

    memcpy((*pkbuf)->payload - size, &message->emm.h, size);
    encoded += size;

    if (message->emm.h.security_header_type >=
            NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE)
    {
        d_assert(pkbuf_header(*pkbuf, 1) == CORE_OK, 
                return CORE_ERROR, "pkbuf_header error");
        encoded -= 1;
        size = nas_encode_service_request(*pkbuf, message);
        d_assert(size >= 0, return CORE_ERROR, "decode error");
        encoded += size;

        goto out;
    }

    switch(message->emm.h.message_type)
    {
        case NAS_ATTACH_REQUEST:
            size = nas_encode_attach_request(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_ATTACH_ACCEPT:
            size = nas_encode_attach_accept(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_ATTACH_COMPLETE:
            size = nas_encode_attach_complete(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_ATTACH_REJECT:
            size = nas_encode_attach_reject(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_DETACH_REQUEST:
            size = nas_encode_detach_request_to_ue(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_DETACH_ACCEPT:
            break;
        case NAS_TRACKING_AREA_UPDATE_REQUEST:
            size = nas_encode_tracking_area_update_request(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_TRACKING_AREA_UPDATE_ACCEPT:
            size = nas_encode_tracking_area_update_accept(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_TRACKING_AREA_UPDATE_COMPLETE:
            break;
        case NAS_TRACKING_AREA_UPDATE_REJECT:
            size = nas_encode_tracking_area_update_reject(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_EXTENDED_SERVICE_REQUEST:
            size = nas_encode_extended_service_request(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_SERVICE_REJECT:
            size = nas_encode_service_reject(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_GUTI_REALLOCATION_COMMAND:
            size = nas_encode_guti_reallocation_command(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_GUTI_REALLOCATION_COMPLETE:
            break;
        case NAS_AUTHENTICATION_REQUEST:
            size = nas_encode_authentication_request(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_AUTHENTICATION_RESPONSE:
            size = nas_encode_authentication_response(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_AUTHENTICATION_REJECT:
            break;
        case NAS_IDENTITY_REQUEST:
            size = nas_encode_identity_request(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_IDENTITY_RESPONSE:
            size = nas_encode_identity_response(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_AUTHENTICATION_FAILURE:
            size = nas_encode_authentication_failure(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_SECURITY_MODE_COMMAND:
            size = nas_encode_security_mode_command(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_SECURITY_MODE_COMPLETE:
            size = nas_encode_security_mode_complete(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_SECURITY_MODE_REJECT:
            size = nas_encode_security_mode_reject(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_EMM_STATUS:
            size = nas_encode_emm_status(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_EMM_INFORMATION:
            size = nas_encode_emm_information(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_DOWNLINK_NAS_TRANSPORT:
            size = nas_encode_downlink_nas_transport(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_UPLINK_NAS_TRANSPORT:
            size = nas_encode_uplink_nas_transport(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_CS_SERVICE_NOTIFICATION:
            size = nas_encode_cs_service_notification(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_UPLINK_GENERIC_NAS_TRANSPORT:
            size = nas_encode_uplink_generic_nas_transport(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_DOWNLINK_GENERIC_NAS_TRANSPORT:
            size = nas_encode_downlink_generic_nas_transport(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        default:
            d_error("Unknown message type (0x%x) or not implemented", 
                    message->emm.h.message_type);
            pkbuf_free((*pkbuf));
            return CORE_ERROR;
    }

out:
    rv = pkbuf_header(*pkbuf, encoded);
    d_assert(rv == CORE_OK, return CORE_ERROR, "pkbuf_header error");

    (*pkbuf)->len = encoded;

    return CORE_OK;
}

status_t nas_esm_encode(pkbuf_t **pkbuf, nas_message_t *message)
{
    status_t rv = CORE_ERROR;
    c_int32_t size = 0;
    c_int32_t encoded = 0;

    d_assert(message, return CORE_ERROR, "Null param");

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    *pkbuf = pkbuf_alloc(NAS_HEADROOM, MAX_SDU_LEN);
    d_assert(*pkbuf, return -1, "Null Param");

    size = sizeof(nas_esm_header_t);
    rv = pkbuf_header(*pkbuf, -size);
    d_assert(rv == CORE_OK, return CORE_ERROR, "pkbuf_header error");

    memcpy((*pkbuf)->payload - size, &message->esm.h, size);
    encoded += size;

    switch(message->esm.h.message_type)
    {
        case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST:
            size = nas_encode_activate_default_eps_bearer_context_request(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT:
            size = nas_encode_activate_default_eps_bearer_context_accept(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT:
            size = nas_encode_activate_default_eps_bearer_context_reject(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST:
            size = nas_encode_activate_dedicated_eps_bearer_context_request(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT:
            size = nas_encode_activate_dedicated_eps_bearer_context_accept(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT:
            size = nas_encode_activate_dedicated_eps_bearer_context_reject(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST:
            size = nas_encode_modify_eps_bearer_context_request(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT:
            size = nas_encode_modify_eps_bearer_context_accept(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT:
            size = nas_encode_modify_eps_bearer_context_reject(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST:
            size = nas_encode_deactivate_eps_bearer_context_request(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT:
            size = nas_encode_deactivate_eps_bearer_context_accept(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_PDN_CONNECTIVITY_REQUEST:
            size = nas_encode_pdn_connectivity_request(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_PDN_CONNECTIVITY_REJECT:
            size = nas_encode_pdn_connectivity_reject(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_PDN_DISCONNECT_REQUEST:
            size = nas_encode_pdn_disconnect_request(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_PDN_DISCONNECT_REJECT:
            size = nas_encode_pdn_disconnect_reject(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_BEARER_RESOURCE_ALLOCATION_REQUEST:
            size = nas_encode_bearer_resource_allocation_request(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_BEARER_RESOURCE_ALLOCATION_REJECT:
            size = nas_encode_bearer_resource_allocation_reject(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_BEARER_RESOURCE_MODIFICATION_REQUEST:
            size = nas_encode_bearer_resource_modification_request(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_BEARER_RESOURCE_MODIFICATION_REJECT:
            size = nas_encode_bearer_resource_modification_reject(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_ESM_INFORMATION_REQUEST:
            break;
        case NAS_ESM_INFORMATION_RESPONSE:
            size = nas_encode_esm_information_response(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_ESM_STATUS:
            size = nas_encode_esm_status(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        default:
            d_error("Unknown message type (0x%x) or not implemented", 
                    message->esm.h.message_type);
            pkbuf_free((*pkbuf));
            return CORE_ERROR;
    }

    rv = pkbuf_header(*pkbuf, encoded);
    d_assert(rv == CORE_OK, return CORE_ERROR, "pkbuf_header error");

    (*pkbuf)->len = encoded;

    return CORE_OK;
}

status_t nas_plain_encode(pkbuf_t **pkbuf, nas_message_t *message)
{
    d_assert(message, return CORE_ERROR, "Null param");

    d_assert(message->emm.h.protocol_discriminator ==
            message->esm.h.protocol_discriminator, 
            return CORE_ERROR, "check UNION for protocol");

    if (message->emm.h.protocol_discriminator == 
            NAS_PROTOCOL_DISCRIMINATOR_EMM)
        return nas_emm_encode(pkbuf, message);
    else if (message->emm.h.protocol_discriminator == 
            NAS_PROTOCOL_DISCRIMINATOR_ESM)
        return nas_esm_encode(pkbuf, message);

    d_assert(0, return CORE_ERROR, 
            "Invalid Protocol : %d", message->emm.h.protocol_discriminator);
}
