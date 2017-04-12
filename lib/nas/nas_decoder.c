/*
 * Copyright (c) 2017, CellWire Group
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
 * Created on: 2017-04-12 23:24:41.513620 by acetcom
 * from 24301-d80.docx
 ******************************************************************************/

#define TRACE_MODULE _nasdec

#include "core_debug.h"
#include "nas_message.h"

c_int32_t nas_decode_attach_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_attach_request_t *attach_request = &message->emm.attach_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

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
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

    return decoded;
}

c_int32_t nas_decode_attach_accept(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_attach_accept_t *attach_accept = &message->emm.attach_accept;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

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
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

    return decoded;
}

c_int32_t nas_decode_attach_complete(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_attach_complete_t *attach_complete = &message->emm.attach_complete;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

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
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

    return decoded;
}

c_int32_t nas_decode_authentication_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_authentication_request_t *authentication_request = &message->emm.authentication_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

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
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

    return decoded;
}

c_int32_t nas_decode_security_mode_command(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_security_mode_command_t *security_mode_command = &message->emm.security_mode_command;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

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
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

    return decoded;
}

c_int32_t nas_decode_security_mode_complete(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_security_mode_complete_t *security_mode_complete = &message->emm.security_mode_complete;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

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
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

    return decoded;
}

c_int32_t nas_decode_security_mode_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_security_mode_reject_t *security_mode_reject = &message->emm.security_mode_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    size = nas_decode_emm_cause(&security_mode_reject->emm_cause, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    return decoded;
}

c_int32_t nas_decode_activate_dedicated_eps_bearer_context_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_activate_dedicated_eps_bearer_context_request_t *activate_dedicated_eps_bearer_context_request = &message->esm.activate_dedicated_eps_bearer_context_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

    size = nas_decode_eps_quality_of_service(&activate_dedicated_eps_bearer_context_request->eps_qos, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_access_point_name(&activate_dedicated_eps_bearer_context_request->access_point_name, pkbuf);
    d_assert(size >= 0, return -1, "decode failed");
    decoded += size;

    size = nas_decode_pdn_address(&activate_dedicated_eps_bearer_context_request->pdn_address, pkbuf);
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
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_TYPE:
                 size = nas_decode_apn_aggregate_maximum_bit_rate(&activate_dedicated_eps_bearer_context_request->apn_ambr, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_TYPE:
                 size = nas_decode_esm_cause(&activate_dedicated_eps_bearer_context_request->esm_cause, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_protocol_configuration_options(&activate_dedicated_eps_bearer_context_request->protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_TYPE:
                 size = nas_decode_connectivity_type(&activate_dedicated_eps_bearer_context_request->connectivity_type, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_PRESENT;
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
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE:
                 size = nas_decode_header_compression_configuration(&activate_dedicated_eps_bearer_context_request->header_compression_configuration, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_TYPE:
                 size = nas_decode_control_plane_only_indication(&activate_dedicated_eps_bearer_context_request->control_plane_only_indication, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&activate_dedicated_eps_bearer_context_request->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_TYPE:
                 size = nas_decode_serving_plmn_rate_control(&activate_dedicated_eps_bearer_context_request->serving_plmn_rate_control, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_request->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_PRESENT;
                 decoded += size;
                 break;
             default:
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

    return decoded;
}

c_int32_t nas_decode_activate_dedicated_eps_bearer_context_accept(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_activate_dedicated_eps_bearer_context_accept_t *activate_dedicated_eps_bearer_context_accept = &message->esm.activate_dedicated_eps_bearer_context_accept;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

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
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&activate_dedicated_eps_bearer_context_accept->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_accept->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

    return decoded;
}

c_int32_t nas_decode_activate_dedicated_eps_bearer_context_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_activate_dedicated_eps_bearer_context_reject_t *activate_dedicated_eps_bearer_context_reject = &message->esm.activate_dedicated_eps_bearer_context_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

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
             case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
                 size = nas_decode_extended_protocol_configuration_options(&activate_dedicated_eps_bearer_context_reject->extended_protocol_configuration_options, pkbuf);
                 d_assert(size >= 0, return -1, "decode failed");
                 activate_dedicated_eps_bearer_context_reject->presencemask |= NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
                 decoded += size;
                 break;
             default:
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

    return decoded;
}

c_int32_t nas_decode_pdn_connectivity_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_pdn_connectivity_request_t *pdn_connectivity_request = &message->esm.pdn_connectivity_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

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
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

    return decoded;
}

c_int32_t nas_decode_pdn_connectivity_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_pdn_connectivity_reject_t *pdn_connectivity_reject = &message->esm.pdn_connectivity_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

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
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

    return decoded;
}

c_int32_t nas_decode_pdn_disconnect_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_pdn_disconnect_request_t *pdn_disconnect_request = &message->esm.pdn_disconnect_request;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

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
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

    return decoded;
}

c_int32_t nas_decode_pdn_disconnect_reject(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_pdn_disconnect_reject_t *pdn_disconnect_reject = &message->esm.pdn_disconnect_reject;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

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
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

    return decoded;
}

c_int32_t nas_decode_esm_information_response(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_esm_information_response_t *esm_information_response = &message->esm.esm_information_response;
    c_int32_t decoded = 0;
    c_int32_t size = 0;

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
                d_error("Unknown type(0x%x) or not implemented\n", type);
                return -1;
        }
    }

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
        default:
            d_error("Unknown message type (0x%x) or not implemented", 
                    message->emm.h.message_type);
            break;
    }

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
        case NAS_ESM_INFORMATION_REQUEST:
            break;
        case NAS_ESM_INFORMATION_RESPONSE:
            size = nas_decode_esm_information_response(message, pkbuf);
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
