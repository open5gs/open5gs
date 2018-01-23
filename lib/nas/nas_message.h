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
 * Created on: 2018-01-23 15:58:42.847411 by acetcom
 * from 24301-d80.docx
 ******************************************************************************/

#ifndef __NAS_MESSAGE_H__
#define __NAS_MESSAGE_H__

#include "nas_ies.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
 * When calculating AES_CMAC, we need to use the headroom of the packet. */
#define NAS_HEADROOM 16

#define NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE 0
#define NAS_SECURITY_HEADER_INTEGRITY_PROTECTED 1
#define NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED 2
#define NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT 3
#define NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHTERD_WITH_NEW_INTEGRITY_CONTEXT 4
#define NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_PARTICALLY_CIPHTERD 5
#define NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE 12

#define NAS_PROTOCOL_DISCRIMINATOR_ESM 0x2
#define NAS_PROTOCOL_DISCRIMINATOR_EMM 0x7

#define NAS_EPS_BEARER_IDENTITY_UNASSIGNED 0
#define NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED 0

typedef struct _nas_emm_header_t {
ED2(c_uint8_t security_header_type:4;,
    c_uint8_t protocol_discriminator:4;)
    c_uint8_t message_type;
} __attribute__ ((packed)) nas_emm_header_t;

typedef struct _nas_esm_header_t {
ED2(c_uint8_t eps_bearer_identity:4;,
    c_uint8_t protocol_discriminator:4;)
    c_uint8_t procedure_transaction_identity;
    c_uint8_t message_type;
} __attribute__ ((packed)) nas_esm_header_t;

typedef struct _nas_security_header_t {
ED2(c_uint8_t security_header_type:4;,
    c_uint8_t protocol_discriminator:4;)
    c_uint32_t message_authentication_code;
    c_uint8_t sequence_number;
} __attribute__ ((packed)) nas_security_header_t;

#define NAS_ATTACH_REQUEST 65
#define NAS_ATTACH_ACCEPT 66
#define NAS_ATTACH_COMPLETE 67
#define NAS_ATTACH_REJECT 68
#define NAS_DETACH_REQUEST 69
#define NAS_DETACH_ACCEPT 70
#define NAS_TRACKING_AREA_UPDATE_REQUEST 72
#define NAS_TRACKING_AREA_UPDATE_ACCEPT 73
#define NAS_TRACKING_AREA_UPDATE_COMPLETE 74
#define NAS_TRACKING_AREA_UPDATE_REJECT 75
#define NAS_EXTENDED_SERVICE_REQUEST 76
#define NAS_SERVICE_REJECT 78
#define NAS_GUTI_REALLOCATION_COMMAND 80
#define NAS_GUTI_REALLOCATION_COMPLETE 81
#define NAS_AUTHENTICATION_REQUEST 82
#define NAS_AUTHENTICATION_RESPONSE 83
#define NAS_AUTHENTICATION_REJECT 84
#define NAS_IDENTITY_REQUEST 85
#define NAS_IDENTITY_RESPONSE 86
#define NAS_AUTHENTICATION_FAILURE 92
#define NAS_SECURITY_MODE_COMMAND 93
#define NAS_SECURITY_MODE_COMPLETE 94
#define NAS_SECURITY_MODE_REJECT 95
#define NAS_EMM_STATUS 96
#define NAS_EMM_INFORMATION 97
#define NAS_DOWNLINK_NAS_TRANSPORT 98
#define NAS_UPLINK_NAS_TRANSPORT 99
#define NAS_CS_SERVICE_NOTIFICATION 100
#define NAS_UPLINK_GENERIC_NAS_TRANSPORT 101
#define NAS_DOWNLINK_GENERIC_NAS_TRANSPORT 104
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST 193
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT 194
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT 195
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST 197
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT 198
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT 199
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST 201
#define NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT 202
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT 203
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST 205
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT 206
#define NAS_PDN_CONNECTIVITY_REQUEST 208
#define NAS_PDN_CONNECTIVITY_REJECT 209
#define NAS_PDN_DISCONNECT_REQUEST 210
#define NAS_PDN_DISCONNECT_REJECT 211
#define NAS_BEARER_RESOURCE_ALLOCATION_REQUEST 212
#define NAS_BEARER_RESOURCE_ALLOCATION_REJECT 213
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST 214
#define NAS_BEARER_RESOURCE_MODIFICATION_REJECT 215
#define NAS_ESM_INFORMATION_REQUEST 217
#define NAS_ESM_INFORMATION_RESPONSE 218
#define NAS_ESM_STATUS 232


/*******************************************************
 * ATTACH REQUEST
 ******************************************************/
#define NAS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_PRESENT (1<<0)
#define NAS_ATTACH_REQUEST_ADDITIONAL_GUTI_PRESENT (1<<1)
#define NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT (1<<2)
#define NAS_ATTACH_REQUEST_DRX_PARAMETER_PRESENT (1<<3)
#define NAS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_PRESENT (1<<4)
#define NAS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT (1<<5)
#define NAS_ATTACH_REQUEST_TMSI_STATUS_PRESENT (1<<6)
#define NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT (1<<7)
#define NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT (1<<8)
#define NAS_ATTACH_REQUEST_SUPPORTED_CODECS_PRESENT (1<<9)
#define NAS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT (1<<10)
#define NAS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT (1<<11)
#define NAS_ATTACH_REQUEST_DEVICE_PROPERTIES_PRESENT (1<<12)
#define NAS_ATTACH_REQUEST_OLD_GUTI_TYPE_PRESENT (1<<13)
#define NAS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT (1<<14)
#define NAS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT (1<<15)
#define NAS_ATTACH_REQUEST_T3324_VALUE_PRESENT (1<<16)
#define NAS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_PRESENT (1<<17)
#define NAS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_PRESENT (1<<18)
#define NAS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE 0x19
#define NAS_ATTACH_REQUEST_ADDITIONAL_GUTI_TYPE 0x50
#define NAS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE 0x52
#define NAS_ATTACH_REQUEST_DRX_PARAMETER_TYPE 0x5C
#define NAS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_TYPE 0x31
#define NAS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_TYPE 0x13
#define NAS_ATTACH_REQUEST_TMSI_STATUS_TYPE 0x90
#define NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE 0x11
#define NAS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_TYPE 0x20
#define NAS_ATTACH_REQUEST_SUPPORTED_CODECS_TYPE 0x40
#define NAS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_TYPE 0xF0
#define NAS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_TYPE 0x5D
#define NAS_ATTACH_REQUEST_DEVICE_PROPERTIES_TYPE 0xD0
#define NAS_ATTACH_REQUEST_OLD_GUTI_TYPE_TYPE 0xE0
#define NAS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_TYPE 0xC0 
#define NAS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_TYPE 0x10
#define NAS_ATTACH_REQUEST_T3324_VALUE_TYPE 0x6A
#define NAS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_TYPE 0x5E
#define NAS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_TYPE 0x6E

typedef struct _nas_attach_request_t {
    /* Mandatory fields */
    nas_eps_attach_type_t eps_attach_type;
    nas_eps_mobile_identity_t eps_mobile_identity;
    nas_ue_network_capability_t ue_network_capability;
    nas_esm_message_container_t esm_message_container;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_p_tmsi_signature_t old_p_tmsi_signature;
    nas_eps_mobile_identity_t additional_guti;
    nas_tracking_area_identity_t last_visited_registered_tai;
    nas_drx_parameter_t drx_parameter;
    nas_ms_network_capability_t ms_network_capability;
    nas_location_area_identification_t old_location_area_identification;
    nas_tmsi_status_t tmsi_status;
    nas_mobile_station_classmark_2_t mobile_station_classmark_2;
    nas_mobile_station_classmark_3_t mobile_station_classmark_3;
    nas_supported_codec_list_t supported_codecs;
    nas_additional_update_type_t additional_update_type;
    nas_voice_domain_preference_and_ue_usage_setting_t voice_domain_preference_and_ue_usage_setting;
    nas_device_properties_t device_properties;
    nas_guti_type_t old_guti_type;
    nas_ms_network_feature_support_t ms_network_feature_support;
    nas_network_resource_identifier_container_t tmsi_based_nri_container;
    nas_gprs_timer_2_t t3324_value;
    nas_gprs_timer_3_t t3412_extended_value;
    nas_extended_drx_parameters_t extended_drx_parameters;
} nas_attach_request_t;


/*******************************************************
 * ATTACH ACCEPT
 ******************************************************/
#define NAS_ATTACH_ACCEPT_GUTI_PRESENT (1<<0)
#define NAS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT (1<<1)
#define NAS_ATTACH_ACCEPT_MS_IDENTITY_PRESENT (1<<2)
#define NAS_ATTACH_ACCEPT_EMM_CAUSE_PRESENT (1<<3)
#define NAS_ATTACH_ACCEPT_T3402_VALUE_PRESENT (1<<4)
#define NAS_ATTACH_ACCEPT_T3423_VALUE_PRESENT (1<<5)
#define NAS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_PRESENT (1<<6)
#define NAS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT (1<<7)
#define NAS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT (1<<8)
#define NAS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT (1<<9)
#define NAS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_PRESENT (1<<10)
#define NAS_ATTACH_ACCEPT_T3324_VALUE_PRESENT (1<<11)
#define NAS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT (1<<12)
#define NAS_ATTACH_ACCEPT_GUTI_TYPE 0x50
#define NAS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE 0x13
#define NAS_ATTACH_ACCEPT_MS_IDENTITY_TYPE 0x23
#define NAS_ATTACH_ACCEPT_EMM_CAUSE_TYPE 0x53
#define NAS_ATTACH_ACCEPT_T3402_VALUE_TYPE 0x17
#define NAS_ATTACH_ACCEPT_T3423_VALUE_TYPE 0x59
#define NAS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_TYPE 0x4A
#define NAS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE 0x34
#define NAS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE 0x64
#define NAS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE 0xF0
#define NAS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_TYPE 0x5E
#define NAS_ATTACH_ACCEPT_T3324_VALUE_TYPE 0x6A
#define NAS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE 0x6E

typedef struct _nas_attach_accept_t {
    /* Mandatory fields */
    nas_eps_attach_result_t eps_attach_result;
    nas_gprs_timer_t t3412_value;
    nas_tracking_area_identity_list_t tai_list;
    nas_esm_message_container_t esm_message_container;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_eps_mobile_identity_t guti;
    nas_location_area_identification_t location_area_identification;
    nas_mobile_identity_t ms_identity;
    nas_emm_cause_t emm_cause;
    nas_gprs_timer_t t3402_value;
    nas_gprs_timer_t t3423_value;
    nas_plmn_list_t equivalent_plmns;
    nas_emergency_number_list_t emergency_number_list;
    nas_eps_network_feature_support_t eps_network_feature_support;
    nas_additional_update_result_t additional_update_result;
    nas_gprs_timer_3_t t3412_extended_value;
    nas_gprs_timer_2_t t3324_value;
    nas_extended_drx_parameters_t extended_drx_parameters;
} nas_attach_accept_t;


/*******************************************************
 * ATTACH COMPLETE
 ******************************************************/

typedef struct _nas_attach_complete_t {
    /* Mandatory fields */
    nas_esm_message_container_t esm_message_container;
} nas_attach_complete_t;


/*******************************************************
 * ATTACH REJECT
 ******************************************************/
#define NAS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_PRESENT (1<<0)
#define NAS_ATTACH_REJECT_T3346_VALUE_PRESENT (1<<1)
#define NAS_ATTACH_REJECT_T3402_VALUE_PRESENT (1<<2)
#define NAS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_PRESENT (1<<3)
#define NAS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_TYPE 0x78
#define NAS_ATTACH_REJECT_T3346_VALUE_TYPE 0x5F
#define NAS_ATTACH_REJECT_T3402_VALUE_TYPE 0x16
#define NAS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_TYPE 0xA0

typedef struct _nas_attach_reject_t {
    /* Mandatory fields */
    nas_emm_cause_t emm_cause;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_esm_message_container_t esm_message_container;
    nas_gprs_timer_2_t t3346_value;
    nas_gprs_timer_2_t t3402_value;
    nas_extended_emm_cause_t extended_emm_cause;
} nas_attach_reject_t;


/*******************************************************
 * DETACH REQUEST FROM UE
 ******************************************************/

typedef struct _nas_detach_request_from_ue_t {
    /* Mandatory fields */
    nas_detach_type_t detach_type;
    nas_eps_mobile_identity_t eps_mobile_identity;
} nas_detach_request_from_ue_t;


/*******************************************************
 * DETACH REQUEST TO UE
 ******************************************************/
#define NAS_DETACH_REQUEST_EMM_CAUSE_PRESENT (1<<0)
#define NAS_DETACH_REQUEST_EMM_CAUSE_TYPE 0x53

typedef struct _nas_detach_request_to_ue_t {
    /* Mandatory fields */
    nas_detach_type_t detach_type;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_emm_cause_t emm_cause;
} nas_detach_request_to_ue_t;


/*******************************************************
 * TRACKING AREA UPDATE REQUEST
 ******************************************************/
#define NAS_TRACKING_AREA_UPDATE_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_PRESENT (1<<0)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_GPRS_CIPHERING_KEY_SEQUENCE_NUMBER_PRESENT (1<<1)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_P_TMSI_SIGNATURE_PRESENT (1<<2)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_GUTI_PRESENT (1<<3)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_PRESENT (1<<4)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_PRESENT (1<<5)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT (1<<6)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_PRESENT (1<<7)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED_PRESENT (1<<8)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT (1<<9)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_PRESENT (1<<10)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT (1<<11)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_TMSI_STATUS_PRESENT (1<<12)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT (1<<13)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT (1<<14)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_SUPPORTED_CODECS_PRESENT (1<<15)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT (1<<16)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT (1<<17)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_PRESENT (1<<18)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_DEVICE_PROPERTIES_PRESENT (1<<19)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT (1<<20)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT (1<<21)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_T3324_VALUE_PRESENT (1<<22)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_T3412_EXTENDED_VALUE_PRESENT (1<<23)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_EXTENDED_DRX_PARAMETERS_PRESENT (1<<24)
#define NAS_TRACKING_AREA_UPDATE_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_TYPE 0xB0
#define NAS_TRACKING_AREA_UPDATE_REQUEST_GPRS_CIPHERING_KEY_SEQUENCE_NUMBER_TYPE 0x80
#define NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE 0x19
#define NAS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_GUTI_TYPE 0x50
#define NAS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_TYPE 0x55
#define NAS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_TYPE 0x58
#define NAS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE 0x52
#define NAS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_TYPE 0x5C
#define NAS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED_TYPE 0xA0
#define NAS_TRACKING_AREA_UPDATE_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE 0x57
#define NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_TYPE 0x31
#define NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_TYPE 0x13
#define NAS_TRACKING_AREA_UPDATE_REQUEST_TMSI_STATUS_TYPE 0x90
#define NAS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE 0x11
#define NAS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_3_TYPE 0x20
#define NAS_TRACKING_AREA_UPDATE_REQUEST_SUPPORTED_CODECS_TYPE 0x40
#define NAS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_UPDATE_TYPE_TYPE 0xF0
#define NAS_TRACKING_AREA_UPDATE_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_TYPE 0x5D
#define NAS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_TYPE 0xE0
#define NAS_TRACKING_AREA_UPDATE_REQUEST_DEVICE_PROPERTIES_TYPE 0xD0
#define NAS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_FEATURE_SUPPORT_TYPE 0xC0
#define NAS_TRACKING_AREA_UPDATE_REQUEST_TMSI_BASED_NRI_CONTAINER_TYPE 0x10
#define NAS_TRACKING_AREA_UPDATE_REQUEST_T3324_VALUE_TYPE 0x6A
#define NAS_TRACKING_AREA_UPDATE_REQUEST_T3412_EXTENDED_VALUE_TYPE 0x5E
#define NAS_TRACKING_AREA_UPDATE_REQUEST_EXTENDED_DRX_PARAMETERS_TYPE 0x6E

typedef struct _nas_tracking_area_update_request_t {
    /* Mandatory fields */
    nas_eps_update_type_t eps_update_type;
    nas_eps_mobile_identity_t old_guti;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_key_set_identifier_t non_current_native_nas_key_set_identifier;
    nas_ciphering_key_sequence_number_t gprs_ciphering_key_sequence_number;
    nas_p_tmsi_signature_t old_p_tmsi_signature;
    nas_eps_mobile_identity_t additional_guti;
    nas_nonce_t nonceue;
    nas_ue_network_capability_t ue_network_capability;
    nas_tracking_area_identity_t last_visited_registered_tai;
    nas_drx_parameter_t drx_parameter;
    nas_ue_radio_capability_information_update_needed_t ue_radio_capability_information_update_needed;
    nas_eps_bearer_context_status_t eps_bearer_context_status;
    nas_ms_network_capability_t ms_network_capability;
    nas_location_area_identification_t old_location_area_identification;
    nas_tmsi_status_t tmsi_status;
    nas_mobile_station_classmark_2_t mobile_station_classmark_2;
    nas_mobile_station_classmark_3_t mobile_station_classmark_3;
    nas_supported_codec_list_t supported_codecs;
    nas_additional_update_type_t additional_update_type;
    nas_voice_domain_preference_and_ue_usage_setting_t voice_domain_preference_and_ue_usage_setting;
    nas_guti_type_t old_guti_type;
    nas_device_properties_t device_properties;
    nas_ms_network_feature_support_t ms_network_feature_support;
    nas_network_resource_identifier_container_t tmsi_based_nri_container;
    nas_gprs_timer_2_t t3324_value;
    nas_gprs_timer_3_t t3412_extended_value;
    nas_extended_drx_parameters_t extended_drx_parameters;
} nas_tracking_area_update_request_t;


/*******************************************************
 * TRACKING AREA UPDATE ACCEPT
 ******************************************************/
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_PRESENT (1<<0)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_GUTI_PRESENT (1<<1)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_PRESENT (1<<2)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_PRESENT (1<<3)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT (1<<4)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_MS_IDENTITY_PRESENT (1<<5)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_EMM_CAUSE_PRESENT (1<<6)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_PRESENT (1<<7)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_PRESENT (1<<8)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_EQUIVALENT_PLMNS_PRESENT (1<<9)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT (1<<10)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT (1<<11)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT (1<<12)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_EXTENDED_VALUE_PRESENT (1<<13)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_T3324_VALUE_PRESENT (1<<14)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT (1<<15)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_STATUS_PRESENT (1<<16)
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_TYPE 0x5A
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_GUTI_TYPE 0x50
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_TYPE 0x54
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_TYPE 0x57
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE 0x13
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_MS_IDENTITY_TYPE 0x23
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_EMM_CAUSE_TYPE 0x53
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_TYPE 0x17
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_TYPE 0x59
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_EQUIVALENT_PLMNS_TYPE 0x4A
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE 0x34
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE 0x64
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE 0xF0
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_EXTENDED_VALUE_TYPE 0x5E
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_T3324_VALUE_TYPE 0x6A
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE 0x6E
#define NAS_TRACKING_AREA_UPDATE_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_STATUS_TYPE 0x68

typedef struct _nas_tracking_area_update_accept_t {
    /* Mandatory fields */
    nas_eps_update_result_t eps_update_result;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_gprs_timer_t t3412_value;
    nas_eps_mobile_identity_t guti;
    nas_tracking_area_identity_list_t tai_list;
    nas_eps_bearer_context_status_t eps_bearer_context_status;
    nas_location_area_identification_t location_area_identification;
    nas_mobile_identity_t ms_identity;
    nas_emm_cause_t emm_cause;
    nas_gprs_timer_t t3402_value;
    nas_gprs_timer_t t3423_value;
    nas_plmn_list_t equivalent_plmns;
    nas_emergency_number_list_t emergency_number_list;
    nas_eps_network_feature_support_t eps_network_feature_support;
    nas_additional_update_result_t additional_update_result;
    nas_gprs_timer_3_t t3412_extended_value;
    nas_gprs_timer_2_t t3324_value;
    nas_extended_drx_parameters_t extended_drx_parameters;
    nas_header_compression_configuration_status_t header_compression_configuration_status;
} nas_tracking_area_update_accept_t;


/*******************************************************
 * TRACKING AREA UPDATE REJECT
 ******************************************************/
#define NAS_TRACKING_AREA_UPDATE_REJECT_T3346_VALUE_PRESENT (1<<0)
#define NAS_TRACKING_AREA_UPDATE_REJECT_EXTENDED_EMM_CAUSE_PRESENT (1<<1)
#define NAS_TRACKING_AREA_UPDATE_REJECT_T3346_VALUE_TYPE 0x5F
#define NAS_TRACKING_AREA_UPDATE_REJECT_EXTENDED_EMM_CAUSE_TYPE 0xA0

typedef struct _nas_tracking_area_update_reject_t {
    /* Mandatory fields */
    nas_emm_cause_t emm_cause;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_gprs_timer_2_t t3346_value;
    nas_extended_emm_cause_t extended_emm_cause;
} nas_tracking_area_update_reject_t;


/*******************************************************
 * EXTENDED SERVICE REQUEST
 ******************************************************/
#define NAS_EXTENDED_SERVICE_REQUEST_CSFB_RESPONSE_PRESENT (1<<0)
#define NAS_EXTENDED_SERVICE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT (1<<1)
#define NAS_EXTENDED_SERVICE_REQUEST_DEVICE_PROPERTIES_PRESENT (1<<2)
#define NAS_EXTENDED_SERVICE_REQUEST_CSFB_RESPONSE_TYPE 0xB0
#define NAS_EXTENDED_SERVICE_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE 0x57
#define NAS_EXTENDED_SERVICE_REQUEST_DEVICE_PROPERTIES_TYPE 0xD0

typedef struct _nas_extended_service_request_t {
    /* Mandatory fields */
    nas_service_type_t service_type;
    nas_mobile_identity_t m_tmsi;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_csfb_response_t csfb_response;
    nas_eps_bearer_context_status_t eps_bearer_context_status;
    nas_device_properties_t device_properties;
} nas_extended_service_request_t;


/*******************************************************
 * SERVICE REQUEST
 ******************************************************/

typedef struct _nas_service_request_t {
    /* Mandatory fields */
    nas_ksi_and_sequence_number_t ksi_and_sequence_number;
    nas_short_mac_t message_authentication_code;
} nas_service_request_t;


/*******************************************************
 * SERVICE REJECT
 ******************************************************/
#define NAS_SERVICE_REJECT_T3346_VALUE_PRESENT (1<<0)
#define NAS_SERVICE_REJECT_T3346_VALUE_TYPE 0x5F

typedef struct _nas_service_reject_t {
    /* Mandatory fields */
    nas_emm_cause_t emm_cause;
    nas_gprs_timer_t t3442_value;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_gprs_timer_2_t t3346_value;
} nas_service_reject_t;


/*******************************************************
 * GUTI REALLOCATION COMMAND
 ******************************************************/
#define NAS_GUTI_REALLOCATION_COMMAND_TAI_LIST_PRESENT (1<<0)
#define NAS_GUTI_REALLOCATION_COMMAND_TAI_LIST_TYPE 0x54

typedef struct _nas_guti_reallocation_command_t {
    /* Mandatory fields */
    nas_eps_mobile_identity_t guti;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_tracking_area_identity_list_t tai_list;
} nas_guti_reallocation_command_t;


/*******************************************************
 * AUTHENTICATION REQUEST
 ******************************************************/

typedef struct _nas_authentication_request_t {
    /* Mandatory fields */
    nas_key_set_identifier_t nas_key_set_identifierasme;
    nas_authentication_parameter_rand_t authentication_parameter_rand;
    nas_authentication_parameter_autn_t authentication_parameter_autn;
} nas_authentication_request_t;


/*******************************************************
 * AUTHENTICATION RESPONSE
 ******************************************************/

typedef struct _nas_authentication_response_t {
    /* Mandatory fields */
    nas_authentication_response_parameter_t authentication_response_parameter;
} nas_authentication_response_t;


/*******************************************************
 * IDENTITY REQUEST
 ******************************************************/

typedef struct _nas_identity_request_t {
    /* Mandatory fields */
    nas_identity_type_2_t identity_type;
} nas_identity_request_t;


/*******************************************************
 * IDENTITY RESPONSE
 ******************************************************/

typedef struct _nas_identity_response_t {
    /* Mandatory fields */
    nas_mobile_identity_t mobile_identity;
} nas_identity_response_t;


/*******************************************************
 * AUTHENTICATION FAILURE
 ******************************************************/
#define NAS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_PRESENT (1<<0)
#define NAS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_TYPE 0x30

typedef struct _nas_authentication_failure_t {
    /* Mandatory fields */
    nas_emm_cause_t emm_cause;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_authentication_failure_parameter_t authentication_failure_parameter;
} nas_authentication_failure_t;


/*******************************************************
 * SECURITY MODE COMMAND
 ******************************************************/
#define NAS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT (1<<0)
#define NAS_SECURITY_MODE_COMMAND_REPLAYED_NONCEUE_PRESENT (1<<1)
#define NAS_SECURITY_MODE_COMMAND_NONCEMME_PRESENT (1<<2)
#define NAS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_TYPE 0xC0
#define NAS_SECURITY_MODE_COMMAND_REPLAYED_NONCEUE_TYPE 0x55
#define NAS_SECURITY_MODE_COMMAND_NONCEMME_TYPE 0x56

typedef struct _nas_security_mode_command_t {
    /* Mandatory fields */
    nas_security_algorithms_t selected_nas_security_algorithms;
    nas_key_set_identifier_t nas_key_set_identifier;
    nas_ue_security_capability_t replayed_ue_security_capabilities;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_imeisv_request_t imeisv_request;
    nas_nonce_t replayed_nonceue;
    nas_nonce_t noncemme;
} nas_security_mode_command_t;


/*******************************************************
 * SECURITY MODE COMPLETE
 ******************************************************/
#define NAS_SECURITY_MODE_COMPLETE_IMEISV_PRESENT (1<<0)
#define NAS_SECURITY_MODE_COMPLETE_IMEISV_TYPE 0x23

typedef struct _nas_security_mode_complete_t {

    /* Optional fields */
    c_uint32_t presencemask;
    nas_mobile_identity_t imeisv;
} nas_security_mode_complete_t;


/*******************************************************
 * SECURITY MODE REJECT
 ******************************************************/

typedef struct _nas_security_mode_reject_t {
    /* Mandatory fields */
    nas_emm_cause_t emm_cause;
} nas_security_mode_reject_t;


/*******************************************************
 * EMM STATUS
 ******************************************************/

typedef struct _nas_emm_status_t {
    /* Mandatory fields */
    nas_emm_cause_t emm_cause;
} nas_emm_status_t;


/*******************************************************
 * EMM INFORMATION
 ******************************************************/
#define NAS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_PRESENT (1<<0)
#define NAS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_PRESENT (1<<1)
#define NAS_EMM_INFORMATION_LOCAL_TIME_ZONE_PRESENT (1<<2)
#define NAS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT (1<<3)
#define NAS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT (1<<4)
#define NAS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_TYPE 0x43
#define NAS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_TYPE 0x45
#define NAS_EMM_INFORMATION_LOCAL_TIME_ZONE_TYPE 0x46
#define NAS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_TYPE 0x47
#define NAS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_TYPE 0x49

typedef struct _nas_emm_information_t {

    /* Optional fields */
    c_uint32_t presencemask;
    nas_network_name_t full_name_for_network;
    nas_network_name_t short_name_for_network;
    nas_time_zone_t local_time_zone;
    nas_time_zone_and_time_t universal_time_and_local_time_zone;
    nas_daylight_saving_time_t network_daylight_saving_time;
} nas_emm_information_t;


/*******************************************************
 * DOWNLINK NAS TRANSPORT
 ******************************************************/

typedef struct _nas_downlink_nas_transport_t {
    /* Mandatory fields */
    nas_message_container_t nas_message_container;
} nas_downlink_nas_transport_t;


/*******************************************************
 * UPLINK NAS TRANSPORT
 ******************************************************/

typedef struct _nas_uplink_nas_transport_t {
    /* Mandatory fields */
    nas_message_container_t nas_message_container;
} nas_uplink_nas_transport_t;


/*******************************************************
 * CS SERVICE NOTIFICATION
 ******************************************************/
#define NAS_CS_SERVICE_NOTIFICATION_CLI_PRESENT (1<<0)
#define NAS_CS_SERVICE_NOTIFICATION_SS_CODE_PRESENT (1<<1)
#define NAS_CS_SERVICE_NOTIFICATION_LCS_INDICATOR_PRESENT (1<<2)
#define NAS_CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_PRESENT (1<<3)
#define NAS_CS_SERVICE_NOTIFICATION_CLI_TYPE 0x60
#define NAS_CS_SERVICE_NOTIFICATION_SS_CODE_TYPE 0x61
#define NAS_CS_SERVICE_NOTIFICATION_LCS_INDICATOR_TYPE 0x62
#define NAS_CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_TYPE 0x63

typedef struct _nas_cs_service_notification_t {
    /* Mandatory fields */
    nas_paging_identity_t paging_identity;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_cli_t cli;
    nas_ss_code_t ss_code;
    nas_lcs_indicator_t lcs_indicator;
    nas_lcs_client_identity_t lcs_client_identity;
} nas_cs_service_notification_t;


/*******************************************************
 * UPLINK GENERIC NAS TRANSPORT
 ******************************************************/
#define NAS_UPLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT (1<<0)
#define NAS_UPLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE 0x65

typedef struct _nas_uplink_generic_nas_transport_t {
    /* Mandatory fields */
    nas_generic_message_container_type_t generic_message_container_type;
    nas_generic_message_container_t generic_message_container;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_additional_information_t additional_information;
} nas_uplink_generic_nas_transport_t;


/*******************************************************
 * DOWNLINK GENERIC NAS TRANSPORT
 ******************************************************/
#define NAS_DOWNLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT (1<<0)
#define NAS_DOWNLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE 0x65

typedef struct _nas_downlink_generic_nas_transport_t {
    /* Mandatory fields */
    nas_generic_message_container_type_t generic_message_container_type;
    nas_generic_message_container_t generic_message_container;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_additional_information_t additional_information;
} nas_downlink_generic_nas_transport_t;


/*******************************************************
 * ACTIVATE DEFAULT EPS BEARER CONTEXT REQUEST
 ******************************************************/
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_PRESENT (1<<0)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_PRESENT (1<<1)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT (1<<2)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT (1<<3)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT (1<<4)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT (1<<5)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_PRESENT (1<<6)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<7)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_PRESENT (1<<8)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT (1<<9)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT (1<<10)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT (1<<11)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_PRESENT (1<<12)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<13)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_PRESENT (1<<14)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_TYPE 0x5D
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_TYPE 0x30
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE 0x32
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE 0x80
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE 0x34
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_TYPE 0x5E
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_TYPE 0x58
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_TYPE 0xB0
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE 0xC0
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x66
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_TYPE 0x90
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_TYPE 0x6E

typedef struct _nas_activate_default_eps_bearer_context_request_t {
    /* Mandatory fields */
    nas_eps_quality_of_service_t eps_qos;
    nas_access_point_name_t access_point_name;
    nas_pdn_address_t pdn_address;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_transaction_identifier_t transaction_identifier;
    nas_quality_of_service_t negotiated_qos;
    nas_llc_service_access_point_identifier_t negotiated_llc_sapi;
    nas_radio_priority_t radio_priority;
    nas_packet_flow_identifier_t packet_flow_identifier;
    nas_apn_aggregate_maximum_bit_rate_t apn_ambr;
    nas_esm_cause_t esm_cause;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_connectivity_type_t connectivity_type;
    nas_wlan_offload_acceptability_t wlan_offload_indication;
    nas_nbifom_container_t nbifom_container;
    nas_header_compression_configuration_t header_compression_configuration;
    nas_control_plane_only_indication_t control_plane_only_indication;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    nas_serving_plmn_rate_control_t serving_plmn_rate_control;
} nas_activate_default_eps_bearer_context_request_t;


/*******************************************************
 * ACTIVATE DEFAULT EPS BEARER CONTEXT ACCEPT
 ******************************************************/
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<1)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_activate_default_eps_bearer_context_accept_t {

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_activate_default_eps_bearer_context_accept_t;


/*******************************************************
 * ACTIVATE DEFAULT EPS BEARER CONTEXT REJECT
 ******************************************************/
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<1)
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_activate_default_eps_bearer_context_reject_t {
    /* Mandatory fields */
    nas_esm_cause_t esm_cause;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_activate_default_eps_bearer_context_reject_t;


/*******************************************************
 * ACTIVATE DEDICATED EPS BEARER CONTEXT REQUEST
 ******************************************************/
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_PRESENT (1<<0)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_PRESENT (1<<1)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT (1<<2)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT (1<<3)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT (1<<4)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<5)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT (1<<6)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT (1<<7)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<8)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_TYPE 0x5D
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_TYPE 0x30
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE 0x32
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE 0x80
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE 0x34
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE 0xC0
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_activate_dedicated_eps_bearer_context_request_t {
    /* Mandatory fields */
    nas_linked_eps_bearer_identity_t linked_eps_bearer_identity;
    nas_eps_quality_of_service_t eps_qos;
    nas_traffic_flow_template_t tft;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_transaction_identifier_t transaction_identifier;
    nas_quality_of_service_t negotiated_qos;
    nas_llc_service_access_point_identifier_t negotiated_llc_sapi;
    nas_radio_priority_t radio_priority;
    nas_packet_flow_identifier_t packet_flow_identifier;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_wlan_offload_acceptability_t wlan_offload_indication;
    nas_nbifom_container_t nbifom_container;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_activate_dedicated_eps_bearer_context_request_t;


/*******************************************************
 * ACTIVATE DEDICATED EPS BEARER CONTEXT ACCEPT
 ******************************************************/
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_PRESENT (1<<1)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<2)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_activate_dedicated_eps_bearer_context_accept_t {

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_nbifom_container_t nbifom_container;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_activate_dedicated_eps_bearer_context_accept_t;


/*******************************************************
 * ACTIVATE DEDICATED EPS BEARER CONTEXT REJECT
 ******************************************************/
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_PRESENT (1<<1)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<2)
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_activate_dedicated_eps_bearer_context_reject_t {
    /* Mandatory fields */
    nas_esm_cause_t esm_cause;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_nbifom_container_t nbifom_container;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_activate_dedicated_eps_bearer_context_reject_t;


/*******************************************************
 * MODIFY EPS BEARER CONTEXT REQUEST
 ******************************************************/
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_EPS_QOS_PRESENT (1<<0)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_TFT_PRESENT (1<<1)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_QOS_PRESENT (1<<2)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT (1<<3)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT (1<<4)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT (1<<5)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT (1<<6)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<7)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT (1<<8)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT (1<<9)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT (1<<10)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<11)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_EPS_QOS_TYPE 0x5B
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_TFT_TYPE 0x36
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_QOS_TYPE 0x30
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE 0x32
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE 0x80
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE 0x34
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_TYPE 0x5E
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE 0xC0
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x66
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_modify_eps_bearer_context_request_t {

    /* Optional fields */
    c_uint32_t presencemask;
    nas_eps_quality_of_service_t new_eps_qos;
    nas_traffic_flow_template_t tft;
    nas_quality_of_service_t new_qos;
    nas_llc_service_access_point_identifier_t negotiated_llc_sapi;
    nas_radio_priority_t radio_priority;
    nas_packet_flow_identifier_t packet_flow_identifier;
    nas_apn_aggregate_maximum_bit_rate_t apn_ambr;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_wlan_offload_acceptability_t wlan_offload_indication;
    nas_nbifom_container_t nbifom_container;
    nas_header_compression_configuration_t header_compression_configuration;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_modify_eps_bearer_context_request_t;


/*******************************************************
 * MODIFY EPS BEARER CONTEXT ACCEPT
 ******************************************************/
#define NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_PRESENT (1<<1)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<2)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_modify_eps_bearer_context_accept_t {

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_nbifom_container_t nbifom_container;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_modify_eps_bearer_context_accept_t;


/*******************************************************
 * MODIFY EPS BEARER CONTEXT REJECT
 ******************************************************/
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_PRESENT (1<<1)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<2)
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_modify_eps_bearer_context_reject_t {
    /* Mandatory fields */
    nas_esm_cause_t esm_cause;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_nbifom_container_t nbifom_container;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_modify_eps_bearer_context_reject_t;


/*******************************************************
 * DEACTIVATE EPS BEARER CONTEXT REQUEST
 ******************************************************/
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_T3396_VALUE_PRESENT (1<<1)
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT (1<<2)
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT (1<<3)
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<4)
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_T3396_VALUE_TYPE 0x37
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE 0xC0
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_deactivate_eps_bearer_context_request_t {
    /* Mandatory fields */
    nas_esm_cause_t esm_cause;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_gprs_timer_3_t t3396_value;
    nas_wlan_offload_acceptability_t wlan_offload_indication;
    nas_nbifom_container_t nbifom_container;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_deactivate_eps_bearer_context_request_t;


/*******************************************************
 * DEACTIVATE EPS BEARER CONTEXT ACCEPT
 ******************************************************/
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<1)
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_deactivate_eps_bearer_context_accept_t {

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_deactivate_eps_bearer_context_accept_t;


/*******************************************************
 * PDN CONNECTIVITY REQUEST
 ******************************************************/
#define NAS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_PRESENT (1<<0)
#define NAS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT (1<<1)
#define NAS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<2)
#define NAS_PDN_CONNECTIVITY_REQUEST_DEVICE_PROPERTIES_PRESENT (1<<3)
#define NAS_PDN_CONNECTIVITY_REQUEST_NBIFOM_CONTAINER_PRESENT (1<<4)
#define NAS_PDN_CONNECTIVITY_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT (1<<5)
#define NAS_PDN_CONNECTIVITY_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<6)
#define NAS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_TYPE 0xD0
#define NAS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_TYPE 0x28
#define NAS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_PDN_CONNECTIVITY_REQUEST_DEVICE_PROPERTIES_TYPE 0xC0
#define NAS_PDN_CONNECTIVITY_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_PDN_CONNECTIVITY_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x66
#define NAS_PDN_CONNECTIVITY_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_pdn_connectivity_request_t {
    /* Mandatory fields */
    nas_request_type_t request_type;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_esm_information_transfer_flag_t esm_information_transfer_flag;
    nas_access_point_name_t access_point_name;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_device_properties_t device_properties;
    nas_nbifom_container_t nbifom_container;
    nas_header_compression_configuration_t header_compression_configuration;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_pdn_connectivity_request_t;


/*******************************************************
 * PDN CONNECTIVITY REJECT
 ******************************************************/
#define NAS_PDN_CONNECTIVITY_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_PDN_CONNECTIVITY_REJECT_BACK_OFF_TIMER_VALUE_PRESENT (1<<1)
#define NAS_PDN_CONNECTIVITY_REJECT_RE_ATTEMPT_INDICATOR_PRESENT (1<<2)
#define NAS_PDN_CONNECTIVITY_REJECT_NBIFOM_CONTAINER_PRESENT (1<<3)
#define NAS_PDN_CONNECTIVITY_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<4)
#define NAS_PDN_CONNECTIVITY_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_PDN_CONNECTIVITY_REJECT_BACK_OFF_TIMER_VALUE_TYPE 0x37
#define NAS_PDN_CONNECTIVITY_REJECT_RE_ATTEMPT_INDICATOR_TYPE 0x6B
#define NAS_PDN_CONNECTIVITY_REJECT_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_PDN_CONNECTIVITY_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_pdn_connectivity_reject_t {
    /* Mandatory fields */
    nas_esm_cause_t esm_cause;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_gprs_timer_3_t back_off_timer_value;
    nas_re_attempt_indicator_t re_attempt_indicator;
    nas_nbifom_container_t nbifom_container;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_pdn_connectivity_reject_t;


/*******************************************************
 * PDN DISCONNECT REQUEST
 ******************************************************/
#define NAS_PDN_DISCONNECT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_PDN_DISCONNECT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<1)
#define NAS_PDN_DISCONNECT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_PDN_DISCONNECT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_pdn_disconnect_request_t {
    /* Mandatory fields */
    nas_linked_eps_bearer_identity_t linked_eps_bearer_identity;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_pdn_disconnect_request_t;


/*******************************************************
 * PDN DISCONNECT REJECT
 ******************************************************/
#define NAS_PDN_DISCONNECT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_PDN_DISCONNECT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<1)
#define NAS_PDN_DISCONNECT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_PDN_DISCONNECT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_pdn_disconnect_reject_t {
    /* Mandatory fields */
    nas_esm_cause_t esm_cause;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_pdn_disconnect_reject_t;


/*******************************************************
 * BEARER RESOURCE ALLOCATION REQUEST
 ******************************************************/
#define NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_DEVICE_PROPERTIES_PRESENT (1<<1)
#define NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_NBIFOM_CONTAINER_PRESENT (1<<2)
#define NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<3)
#define NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_DEVICE_PROPERTIES_TYPE 0xC0
#define NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_bearer_resource_allocation_request_t {
    /* Mandatory fields */
    nas_linked_eps_bearer_identity_t linked_eps_bearer_identity;
    nas_traffic_flow_aggregate_description_t traffic_flow_aggregate;
    nas_eps_quality_of_service_t required_traffic_flow_qos;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_device_properties_t device_properties;
    nas_nbifom_container_t nbifom_container;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_bearer_resource_allocation_request_t;


/*******************************************************
 * BEARER RESOURCE ALLOCATION REJECT
 ******************************************************/
#define NAS_BEARER_RESOURCE_ALLOCATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_BEARER_RESOURCE_ALLOCATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT (1<<1)
#define NAS_BEARER_RESOURCE_ALLOCATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT (1<<2)
#define NAS_BEARER_RESOURCE_ALLOCATION_REJECT_NBIFOM_CONTAINER_PRESENT (1<<3)
#define NAS_BEARER_RESOURCE_ALLOCATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<4)
#define NAS_BEARER_RESOURCE_ALLOCATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_BEARER_RESOURCE_ALLOCATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE 0x37
#define NAS_BEARER_RESOURCE_ALLOCATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE 0x6B
#define NAS_BEARER_RESOURCE_ALLOCATION_REJECT_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_BEARER_RESOURCE_ALLOCATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_bearer_resource_allocation_reject_t {
    /* Mandatory fields */
    nas_esm_cause_t esm_cause;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_gprs_timer_3_t back_off_timer_value;
    nas_re_attempt_indicator_t re_attempt_indicator;
    nas_nbifom_container_t nbifom_container;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_bearer_resource_allocation_reject_t;


/*******************************************************
 * BEARER RESOURCE MODIFICATION REQUEST
 ******************************************************/
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_PRESENT (1<<0)
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_ESM_CAUSE_PRESENT (1<<1)
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<2)
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_DEVICE_PROPERTIES_PRESENT (1<<3)
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_NBIFOM_CONTAINER_PRESENT (1<<4)
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT (1<<5)
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<6)
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_TYPE 0x5B
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_ESM_CAUSE_TYPE 0x58
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_DEVICE_PROPERTIES_TYPE 0xC0
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x66
#define NAS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_bearer_resource_modification_request_t {
    /* Mandatory fields */
    nas_linked_eps_bearer_identity_t eps_bearer_identity_for_packet_filter;
    nas_traffic_flow_aggregate_description_t traffic_flow_aggregate;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_eps_quality_of_service_t required_traffic_flow_qos;
    nas_esm_cause_t esm_cause;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_device_properties_t device_properties;
    nas_nbifom_container_t nbifom_container;
    nas_header_compression_configuration_t header_compression_configuration;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_bearer_resource_modification_request_t;


/*******************************************************
 * BEARER RESOURCE MODIFICATION REJECT
 ******************************************************/
#define NAS_BEARER_RESOURCE_MODIFICATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<0)
#define NAS_BEARER_RESOURCE_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT (1<<1)
#define NAS_BEARER_RESOURCE_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT (1<<2)
#define NAS_BEARER_RESOURCE_MODIFICATION_REJECT_NBIFOM_CONTAINER_PRESENT (1<<3)
#define NAS_BEARER_RESOURCE_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<4)
#define NAS_BEARER_RESOURCE_MODIFICATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_BEARER_RESOURCE_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE 0x37
#define NAS_BEARER_RESOURCE_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE 0x6B
#define NAS_BEARER_RESOURCE_MODIFICATION_REJECT_NBIFOM_CONTAINER_TYPE 0x33
#define NAS_BEARER_RESOURCE_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_bearer_resource_modification_reject_t {
    /* Mandatory fields */
    nas_esm_cause_t esm_cause;

    /* Optional fields */
    c_uint32_t presencemask;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_gprs_timer_3_t back_off_timer_value;
    nas_re_attempt_indicator_t re_attempt_indicator;
    nas_nbifom_container_t nbifom_container;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_bearer_resource_modification_reject_t;


/*******************************************************
 * ESM INFORMATION RESPONSE
 ******************************************************/
#define NAS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT (1<<0)
#define NAS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<1)
#define NAS_ESM_INFORMATION_RESPONSE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT (1<<2)
#define NAS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_TYPE 0x28
#define NAS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define NAS_ESM_INFORMATION_RESPONSE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct _nas_esm_information_response_t {

    /* Optional fields */
    c_uint32_t presencemask;
    nas_access_point_name_t access_point_name;
    nas_protocol_configuration_options_t protocol_configuration_options;
    nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} nas_esm_information_response_t;


/*******************************************************
 * ESM STATUS
 ******************************************************/

typedef struct _nas_esm_status_t {
    /* Mandatory fields */
    nas_esm_cause_t esm_cause;
} nas_esm_status_t;


typedef struct _nas_emm_message_t {
    nas_emm_header_t h;
    union {
        nas_attach_request_t attach_request;
        nas_attach_accept_t attach_accept;
        nas_attach_complete_t attach_complete;
        nas_attach_reject_t attach_reject;
        nas_detach_request_from_ue_t detach_request_from_ue;
        nas_detach_request_to_ue_t detach_request_to_ue;
        nas_tracking_area_update_request_t tracking_area_update_request;
        nas_tracking_area_update_accept_t tracking_area_update_accept;
        nas_tracking_area_update_reject_t tracking_area_update_reject;
        nas_extended_service_request_t extended_service_request;
        nas_service_request_t service_request;
        nas_service_reject_t service_reject;
        nas_guti_reallocation_command_t guti_reallocation_command;
        nas_authentication_request_t authentication_request;
        nas_authentication_response_t authentication_response;
        nas_identity_request_t identity_request;
        nas_identity_response_t identity_response;
        nas_authentication_failure_t authentication_failure;
        nas_security_mode_command_t security_mode_command;
        nas_security_mode_complete_t security_mode_complete;
        nas_security_mode_reject_t security_mode_reject;
        nas_emm_status_t emm_status;
        nas_emm_information_t emm_information;
        nas_downlink_nas_transport_t downlink_nas_transport;
        nas_uplink_nas_transport_t uplink_nas_transport;
        nas_cs_service_notification_t cs_service_notification;
        nas_uplink_generic_nas_transport_t uplink_generic_nas_transport;
        nas_downlink_generic_nas_transport_t downlink_generic_nas_transport;
    };
} nas_emm_message_t;

typedef struct _nas_esm_message_t {
    nas_esm_header_t h;
    union {
        nas_activate_default_eps_bearer_context_request_t activate_default_eps_bearer_context_request;
        nas_activate_default_eps_bearer_context_accept_t activate_default_eps_bearer_context_accept;
        nas_activate_default_eps_bearer_context_reject_t activate_default_eps_bearer_context_reject;
        nas_activate_dedicated_eps_bearer_context_request_t activate_dedicated_eps_bearer_context_request;
        nas_activate_dedicated_eps_bearer_context_accept_t activate_dedicated_eps_bearer_context_accept;
        nas_activate_dedicated_eps_bearer_context_reject_t activate_dedicated_eps_bearer_context_reject;
        nas_modify_eps_bearer_context_request_t modify_eps_bearer_context_request;
        nas_modify_eps_bearer_context_accept_t modify_eps_bearer_context_accept;
        nas_modify_eps_bearer_context_reject_t modify_eps_bearer_context_reject;
        nas_deactivate_eps_bearer_context_request_t deactivate_eps_bearer_context_request;
        nas_deactivate_eps_bearer_context_accept_t deactivate_eps_bearer_context_accept;
        nas_pdn_connectivity_request_t pdn_connectivity_request;
        nas_pdn_connectivity_reject_t pdn_connectivity_reject;
        nas_pdn_disconnect_request_t pdn_disconnect_request;
        nas_pdn_disconnect_reject_t pdn_disconnect_reject;
        nas_bearer_resource_allocation_request_t bearer_resource_allocation_request;
        nas_bearer_resource_allocation_reject_t bearer_resource_allocation_reject;
        nas_bearer_resource_modification_request_t bearer_resource_modification_request;
        nas_bearer_resource_modification_reject_t bearer_resource_modification_reject;
        nas_esm_information_response_t esm_information_response;
        nas_esm_status_t esm_status;
    };
} nas_esm_message_t;

typedef struct _nas_message_t {
    nas_security_header_t h;
    union {
        nas_emm_message_t emm;
        nas_esm_message_t esm;
    };
} nas_message_t;

CORE_DECLARE(status_t) nas_emm_decode(nas_message_t *message, pkbuf_t *pkbuf);
CORE_DECLARE(status_t) nas_esm_decode(nas_message_t *message, pkbuf_t *pkbuf);
CORE_DECLARE(status_t) nas_plain_encode(
        pkbuf_t **pkbuf, nas_message_t *message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAS_MESSAGE_H__ */
