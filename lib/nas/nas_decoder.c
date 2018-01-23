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
 * Created on: 2018-01-23 15:58:42.874567 by acetcom
 * from 24301-d80.docx
 ******************************************************************************/

#define TRACE_MODULE _nas_decoder

#include "core_debug.h"
#include "nas_message.h"

c_int32_t nas_decode_attach_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_attach_request_t *attach_request = &message->emm.attach_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode ATTACH_REQUEST\n");

    size = nas_decode_eps_attach_type(&attach_request->eps_attach_type, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_eps_mobile_identity(&attach_request->eps_mobile_identity, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_ue_network_capability(&attach_request->ue_network_capability, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_esm_message_container(&attach_request->esm_message_container, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE:
                 size = nas_decode_p_tmsi_signature(&attach_request->old_p_tmsi_signature, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_ADDITIONAL_GUTI_TYPE:
                 size = nas_decode_eps_mobile_identity(&attach_request->additional_guti, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_ADDITIONAL_GUTI_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE:
                 size = nas_decode_tracking_area_identity(&attach_request->last_visited_registered_tai, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_DRX_PARAMETER_TYPE:
                 size = nas_decode_drx_parameter(&attach_request->drx_parameter, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_DRX_PARAMETER_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_TYPE:
                 size = nas_decode_ms_network_capability(&attach_request->ms_network_capability, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_TYPE:
                 size = nas_decode_location_area_identification(&attach_request->old_location_area_identification, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_TMSI_STATUS_TYPE:
                 size = nas_decode_tmsi_status(&attach_request->tmsi_status, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_TMSI_STATUS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE:
                 size = nas_decode_mobile_station_classmark_2(&attach_request->mobile_station_classmark_2, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_TYPE:
                 size = nas_decode_mobile_station_classmark_3(&attach_request->mobile_station_classmark_3, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_SUPPORTED_CODECS_TYPE:
                 size = nas_decode_supported_codec_list(&attach_request->supported_codecs, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_SUPPORTED_CODECS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_TYPE:
                 size = nas_decode_additional_update_type(&attach_request->additional_update_type, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_TYPE:
                 size = nas_decode_voice_domain_preference_and_ue_usage_setting(&attach_request->voice_domain_preference_and_ue_usage_setting, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_DEVICE_PROPERTIES_TYPE:
                 size = nas_decode_device_properties(&attach_request->device_properties, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_DEVICE_PROPERTIES_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_OLD_GUTI_TYPE_TYPE:
                 size = nas_decode_guti_type(&attach_request->old_guti_type, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_OLD_GUTI_TYPE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_TYPE:
                 size = nas_decode_ms_network_feature_support(&attach_request->ms_network_feature_support, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_TYPE:
                 size = nas_decode_network_resource_identifier_container(&attach_request->tmsi_based_nri_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_T3324_VALUE_TYPE:
                 size = nas_decode_gprs_timer_2(&attach_request->t3324_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_T3324_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_TYPE:
                 size = nas_decode_gprs_timer_3(&attach_request->t3412_extended_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_TYPE:
                 size = nas_decode_extended_drx_parameters(&attach_request->extended_drx_parameters, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_request->presencemask |= NAS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_attach_accept(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_attach_accept_t *attach_accept = &message->emm.attach_accept;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode ATTACH_ACCEPT\n");

    size = nas_decode_eps_attach_result(&attach_accept->eps_attach_result, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_gprs_timer(&attach_accept->t3412_value, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_tracking_area_identity_list(&attach_accept->tai_list, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_esm_message_container(&attach_accept->esm_message_container, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_ATTACH_ACCEPT_GUTI_TYPE:
                 size = nas_decode_eps_mobile_identity(&attach_accept->guti, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_accept->presencemask |= NAS_ATTACH_ACCEPT_GUTI_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE:
                 size = nas_decode_location_area_identification(&attach_accept->location_area_identification, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_accept->presencemask |= NAS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_ACCEPT_MS_IDENTITY_TYPE:
                 size = nas_decode_mobile_identity(&attach_accept->ms_identity, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_accept->presencemask |= NAS_ATTACH_ACCEPT_MS_IDENTITY_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_ACCEPT_EMM_CAUSE_TYPE:
                 size = nas_decode_emm_cause(&attach_accept->emm_cause, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_accept->presencemask |= NAS_ATTACH_ACCEPT_EMM_CAUSE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_ACCEPT_T3402_VALUE_TYPE:
                 size = nas_decode_gprs_timer(&attach_accept->t3402_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_accept->presencemask |= NAS_ATTACH_ACCEPT_T3402_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_ACCEPT_T3423_VALUE_TYPE:
                 size = nas_decode_gprs_timer(&attach_accept->t3423_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_accept->presencemask |= NAS_ATTACH_ACCEPT_T3423_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_TYPE:
                 size = nas_decode_plmn_list(&attach_accept->equivalent_plmns, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_accept->presencemask |= NAS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE:
                 size = nas_decode_emergency_number_list(&attach_accept->emergency_number_list, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_accept->presencemask |= NAS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE:
                 size = nas_decode_eps_network_feature_support(&attach_accept->eps_network_feature_support, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_accept->presencemask |= NAS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE:
                 size = nas_decode_additional_update_result(&attach_accept->additional_update_result, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_accept->presencemask |= NAS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_TYPE:
                 size = nas_decode_gprs_timer_3(&attach_accept->t3412_extended_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_accept->presencemask |= NAS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_ACCEPT_T3324_VALUE_TYPE:
                 size = nas_decode_gprs_timer_2(&attach_accept->t3324_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_accept->presencemask |= NAS_ATTACH_ACCEPT_T3324_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE:
                 size = nas_decode_extended_drx_parameters(&attach_accept->extended_drx_parameters, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_accept->presencemask |= NAS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_attach_complete(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_attach_complete_t *attach_complete = &message->emm.attach_complete;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode ATTACH_COMPLETE\n");

    size = nas_decode_esm_message_container(&attach_complete->esm_message_container, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    return decoded;
}

c_int32_t nas_decode_attach_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_attach_reject_t *attach_reject = &message->emm.attach_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode ATTACH_REJECT\n");

    size = nas_decode_emm_cause(&attach_reject->emm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_TYPE:
                 size = nas_decode_esm_message_container(&attach_reject->esm_message_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_reject->presencemask |= NAS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REJECT_T3346_VALUE_TYPE:
                 size = nas_decode_gprs_timer_2(&attach_reject->t3346_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_reject->presencemask |= NAS_ATTACH_REJECT_T3346_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REJECT_T3402_VALUE_TYPE:
                 size = nas_decode_gprs_timer_2(&attach_reject->t3402_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_reject->presencemask |= NAS_ATTACH_REJECT_T3402_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_TYPE:
                 size = nas_decode_extended_emm_cause(&attach_reject->extended_emm_cause, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 attach_reject->presencemask |= NAS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_detach_request_from_ue(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_detach_request_from_ue_t *detach_request_from_ue = &message->emm.detach_request_from_ue;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode DETACH_REQUEST\n");

    size = nas_decode_detach_type(&detach_request_from_ue->detach_type, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_eps_mobile_identity(&detach_request_from_ue->eps_mobile_identity, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    return decoded;
}

c_int32_t nas_decode_detach_request_to_ue(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_detach_request_to_ue_t *detach_request_to_ue = &message->emm.detach_request_to_ue;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode DETACH_REQUEST\n");

    size = nas_decode_detach_type(&detach_request_to_ue->detach_type, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_DETACH_REQUEST_EMM_CAUSE_TYPE:
                 size = nas_decode_emm_cause(&detach_request_to_ue->emm_cause, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 detach_request_to_ue->presencemask |= NAS_DETACH_REQUEST_EMM_CAUSE_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_tracking_area_update_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_tracking_area_update_request_t *tracking_area_update_request = &message->emm.tracking_area_update_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode TRACKING_AREA_UPDATE_REQUEST\n");

    size = nas_decode_eps_update_type(&tracking_area_update_request->eps_update_type, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_eps_mobile_identity(&tracking_area_update_request->old_guti, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_TRACKING_AREA_UPDATE_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_TYPE:
                 size = nas_decode_key_set_identifier(&tracking_area_update_request->non_current_native_nas_key_set_identifier, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_GPRS_CIPHERING_KEY_SEQUENCE_NUMBER_TYPE:
                 size = nas_decode_ciphering_key_sequence_number(&tracking_area_update_request->gprs_ciphering_key_sequence_number, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_GPRS_CIPHERING_KEY_SEQUENCE_NUMBER_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE:
                 size = nas_decode_p_tmsi_signature(&tracking_area_update_request->old_p_tmsi_signature, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_P_TMSI_SIGNATURE_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_GUTI_TYPE:
                 size = nas_decode_eps_mobile_identity(&tracking_area_update_request->additional_guti, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_GUTI_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_TYPE:
                 size = nas_decode_nonce(&tracking_area_update_request->nonceue, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_TYPE:
                 size = nas_decode_ue_network_capability(&tracking_area_update_request->ue_network_capability, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE:
                 size = nas_decode_tracking_area_identity(&tracking_area_update_request->last_visited_registered_tai, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_TYPE:
                 size = nas_decode_drx_parameter(&tracking_area_update_request->drx_parameter, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED_TYPE:
                 size = nas_decode_ue_radio_capability_information_update_needed(&tracking_area_update_request->ue_radio_capability_information_update_needed, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE:
                 size = nas_decode_eps_bearer_context_status(&tracking_area_update_request->eps_bearer_context_status, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_TYPE:
                 size = nas_decode_ms_network_capability(&tracking_area_update_request->ms_network_capability, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_TYPE:
                 size = nas_decode_location_area_identification(&tracking_area_update_request->old_location_area_identification, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_TMSI_STATUS_TYPE:
                 size = nas_decode_tmsi_status(&tracking_area_update_request->tmsi_status, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_TMSI_STATUS_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE:
                 size = nas_decode_mobile_station_classmark_2(&tracking_area_update_request->mobile_station_classmark_2, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_3_TYPE:
                 size = nas_decode_mobile_station_classmark_3(&tracking_area_update_request->mobile_station_classmark_3, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_SUPPORTED_CODECS_TYPE:
                 size = nas_decode_supported_codec_list(&tracking_area_update_request->supported_codecs, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_SUPPORTED_CODECS_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_UPDATE_TYPE_TYPE:
                 size = nas_decode_additional_update_type(&tracking_area_update_request->additional_update_type, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_TYPE:
                 size = nas_decode_voice_domain_preference_and_ue_usage_setting(&tracking_area_update_request->voice_domain_preference_and_ue_usage_setting, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_TYPE:
                 size = nas_decode_guti_type(&tracking_area_update_request->old_guti_type, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_DEVICE_PROPERTIES_TYPE:
                 size = nas_decode_device_properties(&tracking_area_update_request->device_properties, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_DEVICE_PROPERTIES_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_FEATURE_SUPPORT_TYPE:
                 size = nas_decode_ms_network_feature_support(&tracking_area_update_request->ms_network_feature_support, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_TMSI_BASED_NRI_CONTAINER_TYPE:
                 size = nas_decode_network_resource_identifier_container(&tracking_area_update_request->tmsi_based_nri_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_T3324_VALUE_TYPE:
                 size = nas_decode_gprs_timer_2(&tracking_area_update_request->t3324_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_T3324_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_T3412_EXTENDED_VALUE_TYPE:
                 size = nas_decode_gprs_timer_3(&tracking_area_update_request->t3412_extended_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_T3412_EXTENDED_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REQUEST_EXTENDED_DRX_PARAMETERS_TYPE:
                 size = nas_decode_extended_drx_parameters(&tracking_area_update_request->extended_drx_parameters, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_request->presencemask |= NAS_TRACKING_AREA_UPDATE_REQUEST_EXTENDED_DRX_PARAMETERS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_tracking_area_update_accept(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_tracking_area_update_accept_t *tracking_area_update_accept = &message->emm.tracking_area_update_accept;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode TRACKING_AREA_UPDATE_ACCEPT\n");

    size = nas_decode_eps_update_result(&tracking_area_update_accept->eps_update_result, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_TYPE:
                 size = nas_decode_gprs_timer(&tracking_area_update_accept->t3412_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_GUTI_TYPE:
                 size = nas_decode_eps_mobile_identity(&tracking_area_update_accept->guti, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_GUTI_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_TYPE:
                 size = nas_decode_tracking_area_identity_list(&tracking_area_update_accept->tai_list, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_TYPE:
                 size = nas_decode_eps_bearer_context_status(&tracking_area_update_accept->eps_bearer_context_status, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE:
                 size = nas_decode_location_area_identification(&tracking_area_update_accept->location_area_identification, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_MS_IDENTITY_TYPE:
                 size = nas_decode_mobile_identity(&tracking_area_update_accept->ms_identity, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_MS_IDENTITY_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_EMM_CAUSE_TYPE:
                 size = nas_decode_emm_cause(&tracking_area_update_accept->emm_cause, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_EMM_CAUSE_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_TYPE:
                 size = nas_decode_gprs_timer(&tracking_area_update_accept->t3402_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_TYPE:
                 size = nas_decode_gprs_timer(&tracking_area_update_accept->t3423_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_EQUIVALENT_PLMNS_TYPE:
                 size = nas_decode_plmn_list(&tracking_area_update_accept->equivalent_plmns, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_EQUIVALENT_PLMNS_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE:
                 size = nas_decode_emergency_number_list(&tracking_area_update_accept->emergency_number_list, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE:
                 size = nas_decode_eps_network_feature_support(&tracking_area_update_accept->eps_network_feature_support, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE:
                 size = nas_decode_additional_update_result(&tracking_area_update_accept->additional_update_result, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_EXTENDED_VALUE_TYPE:
                 size = nas_decode_gprs_timer_3(&tracking_area_update_accept->t3412_extended_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_EXTENDED_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_T3324_VALUE_TYPE:
                 size = nas_decode_gprs_timer_2(&tracking_area_update_accept->t3324_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_T3324_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE:
                 size = nas_decode_extended_drx_parameters(&tracking_area_update_accept->extended_drx_parameters, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_STATUS_TYPE:
                 size = nas_decode_header_compression_configuration_status(&tracking_area_update_accept->header_compression_configuration_status, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_accept->presencemask |= NAS_TRACKING_AREA_UPDATE_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_STATUS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_tracking_area_update_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_tracking_area_update_reject_t *tracking_area_update_reject = &message->emm.tracking_area_update_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode TRACKING_AREA_UPDATE_REJECT\n");

    size = nas_decode_emm_cause(&tracking_area_update_reject->emm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_TRACKING_AREA_UPDATE_REJECT_T3346_VALUE_TYPE:
                 size = nas_decode_gprs_timer_2(&tracking_area_update_reject->t3346_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_reject->presencemask |= NAS_TRACKING_AREA_UPDATE_REJECT_T3346_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_TRACKING_AREA_UPDATE_REJECT_EXTENDED_EMM_CAUSE_TYPE:
                 size = nas_decode_extended_emm_cause(&tracking_area_update_reject->extended_emm_cause, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 tracking_area_update_reject->presencemask |= NAS_TRACKING_AREA_UPDATE_REJECT_EXTENDED_EMM_CAUSE_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_extended_service_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_extended_service_request_t *extended_service_request = &message->emm.extended_service_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode EXTENDED_SERVICE_REQUEST\n");

    size = nas_decode_service_type(&extended_service_request->service_type, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_mobile_identity(&extended_service_request->m_tmsi, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_EXTENDED_SERVICE_REQUEST_CSFB_RESPONSE_TYPE:
                 size = nas_decode_csfb_response(&extended_service_request->csfb_response, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 extended_service_request->presencemask |= NAS_EXTENDED_SERVICE_REQUEST_CSFB_RESPONSE_PRESENT;
                 decoded += size;
                 break;
             case NAS_EXTENDED_SERVICE_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE:
                 size = nas_decode_eps_bearer_context_status(&extended_service_request->eps_bearer_context_status, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 extended_service_request->presencemask |= NAS_EXTENDED_SERVICE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT;
                 decoded += size;
                 break;
             case NAS_EXTENDED_SERVICE_REQUEST_DEVICE_PROPERTIES_TYPE:
                 size = nas_decode_device_properties(&extended_service_request->device_properties, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 extended_service_request->presencemask |= NAS_EXTENDED_SERVICE_REQUEST_DEVICE_PROPERTIES_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_service_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_service_request_t *service_request = &message->emm.service_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode SERVICE_REQUEST\n");

    size = nas_decode_ksi_and_sequence_number(&service_request->ksi_and_sequence_number, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_short_mac(&service_request->message_authentication_code, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    return decoded;
}

c_int32_t nas_decode_service_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_service_reject_t *service_reject = &message->emm.service_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode SERVICE_REJECT\n");

    size = nas_decode_emm_cause(&service_reject->emm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_SERVICE_REJECT_T3346_VALUE_TYPE:
                 size = nas_decode_gprs_timer_2(&service_reject->t3346_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 service_reject->presencemask |= NAS_SERVICE_REJECT_T3346_VALUE_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_guti_reallocation_command(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_guti_reallocation_command_t *guti_reallocation_command = &message->emm.guti_reallocation_command;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode GUTI_REALLOCATION_COMMAND\n");

    size = nas_decode_eps_mobile_identity(&guti_reallocation_command->guti, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_GUTI_REALLOCATION_COMMAND_TAI_LIST_TYPE:
                 size = nas_decode_tracking_area_identity_list(&guti_reallocation_command->tai_list, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 guti_reallocation_command->presencemask |= NAS_GUTI_REALLOCATION_COMMAND_TAI_LIST_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_authentication_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_authentication_request_t *authentication_request = &message->emm.authentication_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode AUTHENTICATION_REQUEST\n");

    size = nas_decode_key_set_identifier(&authentication_request->nas_key_set_identifierasme, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_authentication_parameter_rand(&authentication_request->authentication_parameter_rand, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_authentication_parameter_autn(&authentication_request->authentication_parameter_autn, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    return decoded;
}

c_int32_t nas_decode_authentication_response(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_authentication_response_t *authentication_response = &message->emm.authentication_response;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode AUTHENTICATION_RESPONSE\n");

    size = nas_decode_authentication_response_parameter(&authentication_response->authentication_response_parameter, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    return decoded;
}

c_int32_t nas_decode_identity_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_identity_request_t *identity_request = &message->emm.identity_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode IDENTITY_REQUEST\n");

    size = nas_decode_identity_type_2(&identity_request->identity_type, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    return decoded;
}

c_int32_t nas_decode_identity_response(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_identity_response_t *identity_response = &message->emm.identity_response;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode IDENTITY_RESPONSE\n");

    size = nas_decode_mobile_identity(&identity_response->mobile_identity, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    return decoded;
}

c_int32_t nas_decode_authentication_failure(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_authentication_failure_t *authentication_failure = &message->emm.authentication_failure;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode AUTHENTICATION_FAILURE\n");

    size = nas_decode_emm_cause(&authentication_failure->emm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_TYPE:
                 size = nas_decode_authentication_failure_parameter(&authentication_failure->authentication_failure_parameter, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 authentication_failure->presencemask |= NAS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_security_mode_command(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_security_mode_command_t *security_mode_command = &message->emm.security_mode_command;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode SECURITY_MODE_COMMAND\n");

    size = nas_decode_security_algorithms(&security_mode_command->selected_nas_security_algorithms, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_key_set_identifier(&security_mode_command->nas_key_set_identifier, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_ue_security_capability(&security_mode_command->replayed_ue_security_capabilities, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_TYPE:
                 size = nas_decode_imeisv_request(&security_mode_command->imeisv_request, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 security_mode_command->presencemask |= NAS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT;
                 decoded += size;
                 break;
             case NAS_SECURITY_MODE_COMMAND_REPLAYED_NONCEUE_TYPE:
                 size = nas_decode_nonce(&security_mode_command->replayed_nonceue, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 security_mode_command->presencemask |= NAS_SECURITY_MODE_COMMAND_REPLAYED_NONCEUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_SECURITY_MODE_COMMAND_NONCEMME_TYPE:
                 size = nas_decode_nonce(&security_mode_command->noncemme, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 security_mode_command->presencemask |= NAS_SECURITY_MODE_COMMAND_NONCEMME_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_security_mode_complete(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_security_mode_complete_t *security_mode_complete = &message->emm.security_mode_complete;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode SECURITY_MODE_COMPLETE\n");

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_SECURITY_MODE_COMPLETE_IMEISV_TYPE:
                 size = nas_decode_mobile_identity(&security_mode_complete->imeisv, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 security_mode_complete->presencemask |= NAS_SECURITY_MODE_COMPLETE_IMEISV_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_security_mode_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_security_mode_reject_t *security_mode_reject = &message->emm.security_mode_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode SECURITY_MODE_REJECT\n");

    size = nas_decode_emm_cause(&security_mode_reject->emm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    return decoded;
}

c_int32_t nas_decode_emm_status(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_emm_status_t *emm_status = &message->emm.emm_status;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode EMM_STATUS\n");

    size = nas_decode_emm_cause(&emm_status->emm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    return decoded;
}

c_int32_t nas_decode_emm_information(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_emm_information_t *emm_information = &message->emm.emm_information;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode EMM_INFORMATION\n");

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_TYPE:
                 size = nas_decode_network_name(&emm_information->full_name_for_network, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 emm_information->presencemask |= NAS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_PRESENT;
                 decoded += size;
                 break;
             case NAS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_TYPE:
                 size = nas_decode_network_name(&emm_information->short_name_for_network, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 emm_information->presencemask |= NAS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_PRESENT;
                 decoded += size;
                 break;
             case NAS_EMM_INFORMATION_LOCAL_TIME_ZONE_TYPE:
                 size = nas_decode_time_zone(&emm_information->local_time_zone, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 emm_information->presencemask |= NAS_EMM_INFORMATION_LOCAL_TIME_ZONE_PRESENT;
                 decoded += size;
                 break;
             case NAS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_TYPE:
                 size = nas_decode_time_zone_and_time(&emm_information->universal_time_and_local_time_zone, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 emm_information->presencemask |= NAS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT;
                 decoded += size;
                 break;
             case NAS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_TYPE:
                 size = nas_decode_daylight_saving_time(&emm_information->network_daylight_saving_time, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 emm_information->presencemask |= NAS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_downlink_nas_transport(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_downlink_nas_transport_t *downlink_nas_transport = &message->emm.downlink_nas_transport;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode DOWNLINK_NAS_TRANSPORT\n");

    size = nas_decode_message_container(&downlink_nas_transport->nas_message_container, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    return decoded;
}

c_int32_t nas_decode_uplink_nas_transport(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_uplink_nas_transport_t *uplink_nas_transport = &message->emm.uplink_nas_transport;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode UPLINK_NAS_TRANSPORT\n");

    size = nas_decode_message_container(&uplink_nas_transport->nas_message_container, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    return decoded;
}

c_int32_t nas_decode_cs_service_notification(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_cs_service_notification_t *cs_service_notification = &message->emm.cs_service_notification;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode CS_SERVICE_NOTIFICATION\n");

    size = nas_decode_paging_identity(&cs_service_notification->paging_identity, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_CS_SERVICE_NOTIFICATION_CLI_TYPE:
                 size = nas_decode_cli(&cs_service_notification->cli, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 cs_service_notification->presencemask |= NAS_CS_SERVICE_NOTIFICATION_CLI_PRESENT;
                 decoded += size;
                 break;
             case NAS_CS_SERVICE_NOTIFICATION_SS_CODE_TYPE:
                 size = nas_decode_ss_code(&cs_service_notification->ss_code, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 cs_service_notification->presencemask |= NAS_CS_SERVICE_NOTIFICATION_SS_CODE_PRESENT;
                 decoded += size;
                 break;
             case NAS_CS_SERVICE_NOTIFICATION_LCS_INDICATOR_TYPE:
                 size = nas_decode_lcs_indicator(&cs_service_notification->lcs_indicator, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 cs_service_notification->presencemask |= NAS_CS_SERVICE_NOTIFICATION_LCS_INDICATOR_PRESENT;
                 decoded += size;
                 break;
             case NAS_CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_TYPE:
                 size = nas_decode_lcs_client_identity(&cs_service_notification->lcs_client_identity, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 cs_service_notification->presencemask |= NAS_CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_uplink_generic_nas_transport(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_uplink_generic_nas_transport_t *uplink_generic_nas_transport = &message->emm.uplink_generic_nas_transport;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode UPLINK_GENERIC_NAS_TRANSPORT\n");

    size = nas_decode_generic_message_container_type(&uplink_generic_nas_transport->generic_message_container_type, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_generic_message_container(&uplink_generic_nas_transport->generic_message_container, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_UPLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE:
                 size = nas_decode_additional_information(&uplink_generic_nas_transport->additional_information, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 uplink_generic_nas_transport->presencemask |= NAS_UPLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_downlink_generic_nas_transport(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_downlink_generic_nas_transport_t *downlink_generic_nas_transport = &message->emm.downlink_generic_nas_transport;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode DOWNLINK_GENERIC_NAS_TRANSPORT\n");

    size = nas_decode_generic_message_container_type(&downlink_generic_nas_transport->generic_message_container_type, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_generic_message_container(&downlink_generic_nas_transport->generic_message_container, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_DOWNLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE:
                 size = nas_decode_additional_information(&downlink_generic_nas_transport->additional_information, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 downlink_generic_nas_transport->presencemask |= NAS_DOWNLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_activate_default_eps_bearer_context_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_activate_default_eps_bearer_context_request_t *activate_default_eps_bearer_context_request = &message->esm.activate_default_eps_bearer_context_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST\n");

    size = nas_decode_eps_quality_of_service(&activate_default_eps_bearer_context_request->eps_qos, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_access_point_name(&activate_default_eps_bearer_context_request->access_point_name, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_pdn_address(&activate_default_eps_bearer_context_request->pdn_address, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_TYPE:
                 size = nas_decode_transaction_identifier(&activate_default_eps_bearer_context_request->transaction_identifier, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_TYPE:
                 size = nas_decode_quality_of_service(&activate_default_eps_bearer_context_request->negotiated_qos, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE:
                 size = nas_decode_llc_service_access_point_identifier(&activate_default_eps_bearer_context_request->negotiated_llc_sapi, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE:
                 size = nas_decode_radio_priority(&activate_default_eps_bearer_context_request->radio_priority, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE:
                 size = nas_decode_packet_flow_identifier(&activate_default_eps_bearer_context_request->packet_flow_identifier, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_TYPE:
                 size = nas_decode_apn_aggregate_maximum_bit_rate(&activate_default_eps_bearer_context_request->apn_ambr, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_TYPE:
                 size = nas_decode_esm_cause(&activate_default_eps_bearer_context_request->esm_cause, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&activate_default_eps_bearer_context_request->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_TYPE:
                 size = nas_decode_connectivity_type(&activate_default_eps_bearer_context_request->connectivity_type, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE:
                 size = nas_decode_wlan_offload_acceptability(&activate_default_eps_bearer_context_request->wlan_offload_indication, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&activate_default_eps_bearer_context_request->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE:
                 size = nas_decode_header_compression_configuration(&activate_default_eps_bearer_context_request->header_compression_configuration, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_TYPE:
                 size = nas_decode_control_plane_only_indication(&activate_default_eps_bearer_context_request->control_plane_only_indication, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&activate_default_eps_bearer_context_request->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_TYPE:
                 size = nas_decode_serving_plmn_rate_control(&activate_default_eps_bearer_context_request->serving_plmn_rate_control, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_activate_default_eps_bearer_context_accept(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_activate_default_eps_bearer_context_accept_t *activate_default_eps_bearer_context_accept = &message->esm.activate_default_eps_bearer_context_accept;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT\n");

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&activate_default_eps_bearer_context_accept->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_accept->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&activate_default_eps_bearer_context_accept->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_accept->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_activate_default_eps_bearer_context_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_activate_default_eps_bearer_context_reject_t *activate_default_eps_bearer_context_reject = &message->esm.activate_default_eps_bearer_context_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT\n");

    size = nas_decode_esm_cause(&activate_default_eps_bearer_context_reject->esm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&activate_default_eps_bearer_context_reject->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_reject->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&activate_default_eps_bearer_context_reject->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_default_eps_bearer_context_reject->presencemask |= NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_activate_dedicated_eps_bearer_context_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_activate_dedicated_eps_bearer_context_request_t *activate_dedicated_eps_bearer_context_request = &message->esm.activate_dedicated_eps_bearer_context_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST\n");

    size = nas_decode_linked_eps_bearer_identity(&activate_dedicated_eps_bearer_context_request->linked_eps_bearer_identity, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_eps_quality_of_service(&activate_dedicated_eps_bearer_context_request->eps_qos, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_traffic_flow_template(&activate_dedicated_eps_bearer_context_request->tft, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_TYPE:
                 size = nas_decode_transaction_identifier(&activate_dedicated_eps_bearer_context_request->transaction_identifier, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_TYPE:
                 size = nas_decode_quality_of_service(&activate_dedicated_eps_bearer_context_request->negotiated_qos, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE:
                 size = nas_decode_llc_service_access_point_identifier(&activate_dedicated_eps_bearer_context_request->negotiated_llc_sapi, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE:
                 size = nas_decode_radio_priority(&activate_dedicated_eps_bearer_context_request->radio_priority, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE:
                 size = nas_decode_packet_flow_identifier(&activate_dedicated_eps_bearer_context_request->packet_flow_identifier, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&activate_dedicated_eps_bearer_context_request->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE:
                 size = nas_decode_wlan_offload_acceptability(&activate_dedicated_eps_bearer_context_request->wlan_offload_indication, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&activate_dedicated_eps_bearer_context_request->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&activate_dedicated_eps_bearer_context_request->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_activate_dedicated_eps_bearer_context_accept(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_activate_dedicated_eps_bearer_context_accept_t *activate_dedicated_eps_bearer_context_accept = &message->esm.activate_dedicated_eps_bearer_context_accept;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT\n");

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&activate_dedicated_eps_bearer_context_accept->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_accept->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&activate_dedicated_eps_bearer_context_accept->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_accept->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&activate_dedicated_eps_bearer_context_accept->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_accept->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_activate_dedicated_eps_bearer_context_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_activate_dedicated_eps_bearer_context_reject_t *activate_dedicated_eps_bearer_context_reject = &message->esm.activate_dedicated_eps_bearer_context_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT\n");

    size = nas_decode_esm_cause(&activate_dedicated_eps_bearer_context_reject->esm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&activate_dedicated_eps_bearer_context_reject->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_reject->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&activate_dedicated_eps_bearer_context_reject->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_reject->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&activate_dedicated_eps_bearer_context_reject->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_reject->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_modify_eps_bearer_context_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_modify_eps_bearer_context_request_t *modify_eps_bearer_context_request = &message->esm.modify_eps_bearer_context_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode MODIFY_EPS_BEARER_CONTEXT_REQUEST\n");

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_EPS_QOS_TYPE:
                 size = nas_decode_eps_quality_of_service(&modify_eps_bearer_context_request->new_eps_qos, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_request->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_EPS_QOS_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_TFT_TYPE:
                 size = nas_decode_traffic_flow_template(&modify_eps_bearer_context_request->tft, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_request->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_TFT_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_QOS_TYPE:
                 size = nas_decode_quality_of_service(&modify_eps_bearer_context_request->new_qos, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_request->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_QOS_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE:
                 size = nas_decode_llc_service_access_point_identifier(&modify_eps_bearer_context_request->negotiated_llc_sapi, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_request->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE:
                 size = nas_decode_radio_priority(&modify_eps_bearer_context_request->radio_priority, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_request->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE:
                 size = nas_decode_packet_flow_identifier(&modify_eps_bearer_context_request->packet_flow_identifier, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_request->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_TYPE:
                 size = nas_decode_apn_aggregate_maximum_bit_rate(&modify_eps_bearer_context_request->apn_ambr, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_request->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&modify_eps_bearer_context_request->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_request->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE:
                 size = nas_decode_wlan_offload_acceptability(&modify_eps_bearer_context_request->wlan_offload_indication, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_request->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&modify_eps_bearer_context_request->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_request->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE:
                 size = nas_decode_header_compression_configuration(&modify_eps_bearer_context_request->header_compression_configuration, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_request->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&modify_eps_bearer_context_request->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_request->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_modify_eps_bearer_context_accept(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_modify_eps_bearer_context_accept_t *modify_eps_bearer_context_accept = &message->esm.modify_eps_bearer_context_accept;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode MODIFY_EPS_BEARER_CONTEXT_ACCEPT\n");

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&modify_eps_bearer_context_accept->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_accept->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&modify_eps_bearer_context_accept->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_accept->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&modify_eps_bearer_context_accept->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_accept->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_modify_eps_bearer_context_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_modify_eps_bearer_context_reject_t *modify_eps_bearer_context_reject = &message->esm.modify_eps_bearer_context_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode MODIFY_EPS_BEARER_CONTEXT_REJECT\n");

    size = nas_decode_esm_cause(&modify_eps_bearer_context_reject->esm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&modify_eps_bearer_context_reject->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_reject->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&modify_eps_bearer_context_reject->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_reject->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&modify_eps_bearer_context_reject->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 modify_eps_bearer_context_reject->presencemask |= NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_deactivate_eps_bearer_context_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_deactivate_eps_bearer_context_request_t *deactivate_eps_bearer_context_request = &message->esm.deactivate_eps_bearer_context_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST\n");

    size = nas_decode_esm_cause(&deactivate_eps_bearer_context_request->esm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&deactivate_eps_bearer_context_request->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 deactivate_eps_bearer_context_request->presencemask |= NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_T3396_VALUE_TYPE:
                 size = nas_decode_gprs_timer_3(&deactivate_eps_bearer_context_request->t3396_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 deactivate_eps_bearer_context_request->presencemask |= NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_T3396_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE:
                 size = nas_decode_wlan_offload_acceptability(&deactivate_eps_bearer_context_request->wlan_offload_indication, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 deactivate_eps_bearer_context_request->presencemask |= NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&deactivate_eps_bearer_context_request->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 deactivate_eps_bearer_context_request->presencemask |= NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&deactivate_eps_bearer_context_request->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 deactivate_eps_bearer_context_request->presencemask |= NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_deactivate_eps_bearer_context_accept(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_deactivate_eps_bearer_context_accept_t *deactivate_eps_bearer_context_accept = &message->esm.deactivate_eps_bearer_context_accept;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT\n");

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&deactivate_eps_bearer_context_accept->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 deactivate_eps_bearer_context_accept->presencemask |= NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&deactivate_eps_bearer_context_accept->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 deactivate_eps_bearer_context_accept->presencemask |= NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_pdn_connectivity_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_pdn_connectivity_request_t *pdn_connectivity_request = &message->esm.pdn_connectivity_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode PDN_CONNECTIVITY_REQUEST\n");

    size = nas_decode_request_type(&pdn_connectivity_request->request_type, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_TYPE:
                 size = nas_decode_esm_information_transfer_flag(&pdn_connectivity_request->esm_information_transfer_flag, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_connectivity_request->presencemask |= NAS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_PRESENT;
                 decoded += size;
                 break;
             case NAS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_TYPE:
                 size = nas_decode_access_point_name(&pdn_connectivity_request->access_point_name, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_connectivity_request->presencemask |= NAS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT;
                 decoded += size;
                 break;
             case NAS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&pdn_connectivity_request->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_connectivity_request->presencemask |= NAS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_PDN_CONNECTIVITY_REQUEST_DEVICE_PROPERTIES_TYPE:
                 size = nas_decode_device_properties(&pdn_connectivity_request->device_properties, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_connectivity_request->presencemask |= NAS_PDN_CONNECTIVITY_REQUEST_DEVICE_PROPERTIES_PRESENT;
                 decoded += size;
                 break;
             case NAS_PDN_CONNECTIVITY_REQUEST_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&pdn_connectivity_request->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_connectivity_request->presencemask |= NAS_PDN_CONNECTIVITY_REQUEST_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_PDN_CONNECTIVITY_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE:
                 size = nas_decode_header_compression_configuration(&pdn_connectivity_request->header_compression_configuration, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_connectivity_request->presencemask |= NAS_PDN_CONNECTIVITY_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_PDN_CONNECTIVITY_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&pdn_connectivity_request->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_connectivity_request->presencemask |= NAS_PDN_CONNECTIVITY_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_pdn_connectivity_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_pdn_connectivity_reject_t *pdn_connectivity_reject = &message->esm.pdn_connectivity_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode PDN_CONNECTIVITY_REJECT\n");

    size = nas_decode_esm_cause(&pdn_connectivity_reject->esm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_PDN_CONNECTIVITY_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&pdn_connectivity_reject->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_connectivity_reject->presencemask |= NAS_PDN_CONNECTIVITY_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_PDN_CONNECTIVITY_REJECT_BACK_OFF_TIMER_VALUE_TYPE:
                 size = nas_decode_gprs_timer_3(&pdn_connectivity_reject->back_off_timer_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_connectivity_reject->presencemask |= NAS_PDN_CONNECTIVITY_REJECT_BACK_OFF_TIMER_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_PDN_CONNECTIVITY_REJECT_RE_ATTEMPT_INDICATOR_TYPE:
                 size = nas_decode_re_attempt_indicator(&pdn_connectivity_reject->re_attempt_indicator, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_connectivity_reject->presencemask |= NAS_PDN_CONNECTIVITY_REJECT_RE_ATTEMPT_INDICATOR_PRESENT;
                 decoded += size;
                 break;
             case NAS_PDN_CONNECTIVITY_REJECT_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&pdn_connectivity_reject->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_connectivity_reject->presencemask |= NAS_PDN_CONNECTIVITY_REJECT_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_PDN_CONNECTIVITY_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&pdn_connectivity_reject->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_connectivity_reject->presencemask |= NAS_PDN_CONNECTIVITY_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_pdn_disconnect_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_pdn_disconnect_request_t *pdn_disconnect_request = &message->esm.pdn_disconnect_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode PDN_DISCONNECT_REQUEST\n");

    size = nas_decode_linked_eps_bearer_identity(&pdn_disconnect_request->linked_eps_bearer_identity, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_PDN_DISCONNECT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&pdn_disconnect_request->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_disconnect_request->presencemask |= NAS_PDN_DISCONNECT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_PDN_DISCONNECT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&pdn_disconnect_request->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_disconnect_request->presencemask |= NAS_PDN_DISCONNECT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_pdn_disconnect_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_pdn_disconnect_reject_t *pdn_disconnect_reject = &message->esm.pdn_disconnect_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode PDN_DISCONNECT_REJECT\n");

    size = nas_decode_esm_cause(&pdn_disconnect_reject->esm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_PDN_DISCONNECT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&pdn_disconnect_reject->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_disconnect_reject->presencemask |= NAS_PDN_DISCONNECT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_PDN_DISCONNECT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&pdn_disconnect_reject->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 pdn_disconnect_reject->presencemask |= NAS_PDN_DISCONNECT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_bearer_resource_allocation_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_bearer_resource_allocation_request_t *bearer_resource_allocation_request = &message->esm.bearer_resource_allocation_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode BEARER_RESOURCE_ALLOCATION_REQUEST\n");

    size = nas_decode_linked_eps_bearer_identity(&bearer_resource_allocation_request->linked_eps_bearer_identity, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_traffic_flow_aggregate_description(&bearer_resource_allocation_request->traffic_flow_aggregate, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_eps_quality_of_service(&bearer_resource_allocation_request->required_traffic_flow_qos, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&bearer_resource_allocation_request->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_allocation_request->presencemask |= NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_DEVICE_PROPERTIES_TYPE:
                 size = nas_decode_device_properties(&bearer_resource_allocation_request->device_properties, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_allocation_request->presencemask |= NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_DEVICE_PROPERTIES_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&bearer_resource_allocation_request->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_allocation_request->presencemask |= NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&bearer_resource_allocation_request->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_allocation_request->presencemask |= NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_bearer_resource_allocation_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_bearer_resource_allocation_reject_t *bearer_resource_allocation_reject = &message->esm.bearer_resource_allocation_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode BEARER_RESOURCE_ALLOCATION_REJECT\n");

    size = nas_decode_esm_cause(&bearer_resource_allocation_reject->esm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_BEARER_RESOURCE_ALLOCATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&bearer_resource_allocation_reject->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_allocation_reject->presencemask |= NAS_BEARER_RESOURCE_ALLOCATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_ALLOCATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE:
                 size = nas_decode_gprs_timer_3(&bearer_resource_allocation_reject->back_off_timer_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_allocation_reject->presencemask |= NAS_BEARER_RESOURCE_ALLOCATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_ALLOCATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE:
                 size = nas_decode_re_attempt_indicator(&bearer_resource_allocation_reject->re_attempt_indicator, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_allocation_reject->presencemask |= NAS_BEARER_RESOURCE_ALLOCATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_ALLOCATION_REJECT_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&bearer_resource_allocation_reject->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_allocation_reject->presencemask |= NAS_BEARER_RESOURCE_ALLOCATION_REJECT_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_ALLOCATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&bearer_resource_allocation_reject->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_allocation_reject->presencemask |= NAS_BEARER_RESOURCE_ALLOCATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_bearer_resource_modification_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_bearer_resource_modification_request_t *bearer_resource_modification_request = &message->esm.bearer_resource_modification_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode BEARER_RESOURCE_MODIFICATION_REQUEST\n");

    size = nas_decode_linked_eps_bearer_identity(&bearer_resource_modification_request->eps_bearer_identity_for_packet_filter, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_traffic_flow_aggregate_description(&bearer_resource_modification_request->traffic_flow_aggregate, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_TYPE:
                 size = nas_decode_eps_quality_of_service(&bearer_resource_modification_request->required_traffic_flow_qos, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_modification_request->presencemask |= NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_ESM_CAUSE_TYPE:
                 size = nas_decode_esm_cause(&bearer_resource_modification_request->esm_cause, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_modification_request->presencemask |= NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_ESM_CAUSE_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&bearer_resource_modification_request->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_modification_request->presencemask |= NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_DEVICE_PROPERTIES_TYPE:
                 size = nas_decode_device_properties(&bearer_resource_modification_request->device_properties, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_modification_request->presencemask |= NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_DEVICE_PROPERTIES_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&bearer_resource_modification_request->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_modification_request->presencemask |= NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE:
                 size = nas_decode_header_compression_configuration(&bearer_resource_modification_request->header_compression_configuration, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_modification_request->presencemask |= NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&bearer_resource_modification_request->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_modification_request->presencemask |= NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_bearer_resource_modification_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_bearer_resource_modification_reject_t *bearer_resource_modification_reject = &message->esm.bearer_resource_modification_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode BEARER_RESOURCE_MODIFICATION_REJECT\n");

    size = nas_decode_esm_cause(&bearer_resource_modification_reject->esm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_BEARER_RESOURCE_MODIFICATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&bearer_resource_modification_reject->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_modification_reject->presencemask |= NAS_BEARER_RESOURCE_MODIFICATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE:
                 size = nas_decode_gprs_timer_3(&bearer_resource_modification_reject->back_off_timer_value, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_modification_reject->presencemask |= NAS_BEARER_RESOURCE_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE:
                 size = nas_decode_re_attempt_indicator(&bearer_resource_modification_reject->re_attempt_indicator, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_modification_reject->presencemask |= NAS_BEARER_RESOURCE_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_MODIFICATION_REJECT_NBIFOM_CONTAINER_TYPE:
                 size = nas_decode_nbifom_container(&bearer_resource_modification_reject->nbifom_container, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_modification_reject->presencemask |= NAS_BEARER_RESOURCE_MODIFICATION_REJECT_NBIFOM_CONTAINER_PRESENT;
                 decoded += size;
                 break;
             case NAS_BEARER_RESOURCE_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&bearer_resource_modification_reject->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 bearer_resource_modification_reject->presencemask |= NAS_BEARER_RESOURCE_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_esm_information_response(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_esm_information_response_t *esm_information_response = &message->esm.esm_information_response;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode ESM_INFORMATION_RESPONSE\n");

    while(pkbuf->len > 0) 
    {
        c_uint8_t *buffer = pkbuf->payload;
        c_uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(c_uint8_t);
        d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, 
                "pkbuf_header error");
        decoded += size;

        switch(type)
        {
             case NAS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_TYPE:
                 size = nas_decode_access_point_name(&esm_information_response->access_point_name, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 esm_information_response->presencemask |= NAS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT;
                 decoded += size;
                 break;
             case NAS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&esm_information_response->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 esm_information_response->presencemask |= NAS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ESM_INFORMATION_RESPONSE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&esm_information_response->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 esm_information_response->presencemask |= NAS_ESM_INFORMATION_RESPONSE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_warn("Unknown type(0x%x) or not implemented\n", type);
                break;
        }
    }

    return decoded;
}

c_int32_t nas_decode_esm_status(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_esm_status_t *esm_status = &message->esm.esm_status;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    d_trace(25, "[NAS] Decode ESM_STATUS\n");

    size = nas_decode_esm_cause(&esm_status->esm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    return decoded;
}

status_t nas_emm_decode(nas_message_t *message, pkbuf_t *pkbuf)
{
    status_t rv = CORE_ERROR;
    c_uint16_t size = 0;
    c_uint16_t decoded = 0;

    d_assert(pkbuf, return CORE_ERROR, "Null param");
    d_assert(pkbuf->payload, return CORE_ERROR, "Null param");

    memset(message, 0, sizeof(nas_message_t));

    size = sizeof(nas_emm_header_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return CORE_ERROR, "pkbuf_header error");
    memcpy(&message->emm.h, pkbuf->payload - size, size);
    decoded += size;

    if (message->emm.h.security_header_type >=
            NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE)
    {
        d_assert(pkbuf_header(pkbuf, 1) == CORE_OK, 
                return CORE_ERROR, "pkbuf_header error");
        decoded -= 1;
        size = nas_decode_service_request(message, pkbuf);
        d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
        decoded += size;

        goto out;
    }

    switch(message->emm.h.message_type)
    {
        case NAS_ATTACH_REQUEST:
            size = nas_decode_attach_request(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_ATTACH_ACCEPT:
            size = nas_decode_attach_accept(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_ATTACH_COMPLETE:
            size = nas_decode_attach_complete(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_ATTACH_REJECT:
            size = nas_decode_attach_reject(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_DETACH_REQUEST:
            size = nas_decode_detach_request_from_ue(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_DETACH_ACCEPT:
            break;
        case NAS_TRACKING_AREA_UPDATE_REQUEST:
            size = nas_decode_tracking_area_update_request(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_TRACKING_AREA_UPDATE_ACCEPT:
            size = nas_decode_tracking_area_update_accept(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_TRACKING_AREA_UPDATE_COMPLETE:
            break;
        case NAS_TRACKING_AREA_UPDATE_REJECT:
            size = nas_decode_tracking_area_update_reject(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_EXTENDED_SERVICE_REQUEST:
            size = nas_decode_extended_service_request(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_SERVICE_REJECT:
            size = nas_decode_service_reject(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_GUTI_REALLOCATION_COMMAND:
            size = nas_decode_guti_reallocation_command(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_GUTI_REALLOCATION_COMPLETE:
            break;
        case NAS_AUTHENTICATION_REQUEST:
            size = nas_decode_authentication_request(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_AUTHENTICATION_RESPONSE:
            size = nas_decode_authentication_response(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_AUTHENTICATION_REJECT:
            break;
        case NAS_IDENTITY_REQUEST:
            size = nas_decode_identity_request(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_IDENTITY_RESPONSE:
            size = nas_decode_identity_response(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_AUTHENTICATION_FAILURE:
            size = nas_decode_authentication_failure(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_SECURITY_MODE_COMMAND:
            size = nas_decode_security_mode_command(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_SECURITY_MODE_COMPLETE:
            size = nas_decode_security_mode_complete(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_SECURITY_MODE_REJECT:
            size = nas_decode_security_mode_reject(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_EMM_STATUS:
            size = nas_decode_emm_status(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_EMM_INFORMATION:
            size = nas_decode_emm_information(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_DOWNLINK_NAS_TRANSPORT:
            size = nas_decode_downlink_nas_transport(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_UPLINK_NAS_TRANSPORT:
            size = nas_decode_uplink_nas_transport(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_CS_SERVICE_NOTIFICATION:
            size = nas_decode_cs_service_notification(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_UPLINK_GENERIC_NAS_TRANSPORT:
            size = nas_decode_uplink_generic_nas_transport(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_DOWNLINK_GENERIC_NAS_TRANSPORT:
            size = nas_decode_downlink_generic_nas_transport(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        default:
            d_error("Unknown message type (0x%x) or not implemented", 
                    message->emm.h.message_type);
            break;
    }

out:
    rv = pkbuf_header(pkbuf, decoded);
    d_assert(rv == CORE_OK, return CORE_ERROR, "pkbuf_header error");

    return CORE_OK;
}
status_t nas_esm_decode(nas_message_t *message, pkbuf_t *pkbuf)
{
    status_t rv = CORE_ERROR;
    c_uint16_t size = 0;
    c_uint16_t decoded = 0;

    d_assert(pkbuf, return CORE_ERROR, "Null param");
    d_assert(pkbuf->payload, return CORE_ERROR, "Null param");

    memset(message, 0, sizeof(nas_message_t));

    size = sizeof(nas_esm_header_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return CORE_ERROR, "pkbuf_header error");
    memcpy(&message->esm.h, pkbuf->payload - size, size);
    decoded += size;

    switch(message->esm.h.message_type)
    {
        case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST:
            size = nas_decode_activate_default_eps_bearer_context_request(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT:
            size = nas_decode_activate_default_eps_bearer_context_accept(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT:
            size = nas_decode_activate_default_eps_bearer_context_reject(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST:
            size = nas_decode_activate_dedicated_eps_bearer_context_request(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT:
            size = nas_decode_activate_dedicated_eps_bearer_context_accept(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT:
            size = nas_decode_activate_dedicated_eps_bearer_context_reject(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST:
            size = nas_decode_modify_eps_bearer_context_request(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT:
            size = nas_decode_modify_eps_bearer_context_accept(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT:
            size = nas_decode_modify_eps_bearer_context_reject(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST:
            size = nas_decode_deactivate_eps_bearer_context_request(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT:
            size = nas_decode_deactivate_eps_bearer_context_accept(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_PDN_CONNECTIVITY_REQUEST:
            size = nas_decode_pdn_connectivity_request(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_PDN_CONNECTIVITY_REJECT:
            size = nas_decode_pdn_connectivity_reject(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_PDN_DISCONNECT_REQUEST:
            size = nas_decode_pdn_disconnect_request(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_PDN_DISCONNECT_REJECT:
            size = nas_decode_pdn_disconnect_reject(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_BEARER_RESOURCE_ALLOCATION_REQUEST:
            size = nas_decode_bearer_resource_allocation_request(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_BEARER_RESOURCE_ALLOCATION_REJECT:
            size = nas_decode_bearer_resource_allocation_reject(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_BEARER_RESOURCE_MODIFICATION_REQUEST:
            size = nas_decode_bearer_resource_modification_request(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_BEARER_RESOURCE_MODIFICATION_REJECT:
            size = nas_decode_bearer_resource_modification_reject(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_ESM_INFORMATION_REQUEST:
            break;
        case NAS_ESM_INFORMATION_RESPONSE:
            size = nas_decode_esm_information_response(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        case NAS_ESM_STATUS:
            size = nas_decode_esm_status(message, pkbuf);
            d_assert(size >= CORE_OK, return CORE_ERROR, "decode error");
            decoded += size;
            break;
        default:
            d_error("Unknown message type (0x%x) or not implemented", 
                    message->esm.h.message_type);
            break;
    }

    rv = pkbuf_header(pkbuf, decoded);
    d_assert(rv == CORE_OK, return CORE_ERROR, "pkbuf_header error");

    return CORE_OK;
}

#if 0 /* deprecated */
status_t nas_plain_decode(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_security_header_t *h = NULL;

    d_assert(pkbuf, return CORE_ERROR, "Null param");
    h = pkbuf->payload;
    d_assert(h, return CORE_ERROR, "Null param");

    if (h->protocol_discriminator == NAS_PROTOCOL_DISCRIMINATOR_EMM)
        return nas_emm_decode(message, pkbuf);
    else if (h->protocol_discriminator == NAS_PROTOCOL_DISCRIMINATOR_ESM)
        return nas_esm_decode(message, pkbuf);

    d_assert(0, return CORE_ERROR, 
            "Invalid Protocol : %d", h->protocol_discriminator);
}
#endif
