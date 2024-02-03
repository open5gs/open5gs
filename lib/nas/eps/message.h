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
 * This file had been created by nas-message.py script v0.1.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2024-01-21 18:50:03.409367 by acetcom
 * from 24301-h90.docx
 ******************************************************************************/

#if !defined(OGS_NAS_INSIDE) && !defined(OGS_NAS_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_NAS_EPS_MESSAGE_H
#define OGS_NAS_EPS_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_NAS_PROTOCOL_DISCRIMINATOR_ESM 0x2
#define OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM 0x7

#define OGS_NAS_EPS_BEARER_IDENTITY_UNASSIGNED 0

typedef struct ogs_nas_emm_header_s {
ED2(uint8_t security_header_type:4;,
    uint8_t protocol_discriminator:4;)
    uint8_t message_type;
} __attribute__ ((packed)) ogs_nas_emm_header_t;

typedef struct ogs_nas_esm_header_s {
ED2(uint8_t eps_bearer_identity:4;,
    uint8_t protocol_discriminator:4;)
    uint8_t procedure_transaction_identity;
    uint8_t message_type;
} __attribute__ ((packed)) ogs_nas_esm_header_t;

typedef struct ogs_nas_eps_security_header_s {
ED2(uint8_t security_header_type:4;,
    uint8_t protocol_discriminator:4;)
    uint32_t message_authentication_code;
    uint8_t sequence_number;
} __attribute__ ((packed)) ogs_nas_eps_security_header_t;

#define OGS_NAS_EPS_ATTACH_REQUEST 65
#define OGS_NAS_EPS_ATTACH_ACCEPT 66
#define OGS_NAS_EPS_ATTACH_COMPLETE 67
#define OGS_NAS_EPS_ATTACH_REJECT 68
#define OGS_NAS_EPS_DETACH_REQUEST 69
#define OGS_NAS_EPS_DETACH_ACCEPT 70
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST 72
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT 73
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_COMPLETE 74
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT 75
#define OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST 76
#define OGS_NAS_EPS_SERVICE_REJECT 78
#define OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND 80
#define OGS_NAS_EPS_GUTI_REALLOCATION_COMPLETE 81
#define OGS_NAS_EPS_AUTHENTICATION_REQUEST 82
#define OGS_NAS_EPS_AUTHENTICATION_RESPONSE 83
#define OGS_NAS_EPS_AUTHENTICATION_REJECT 84
#define OGS_NAS_EPS_IDENTITY_REQUEST 85
#define OGS_NAS_EPS_IDENTITY_RESPONSE 86
#define OGS_NAS_EPS_AUTHENTICATION_FAILURE 92
#define OGS_NAS_EPS_SECURITY_MODE_COMMAND 93
#define OGS_NAS_EPS_SECURITY_MODE_COMPLETE 94
#define OGS_NAS_EPS_SECURITY_MODE_REJECT 95
#define OGS_NAS_EPS_EMM_STATUS 96
#define OGS_NAS_EPS_EMM_INFORMATION 97
#define OGS_NAS_EPS_DOWNLINK_NAS_TRANSPORT 98
#define OGS_NAS_EPS_UPLINK_NAS_TRANSPORT 99
#define OGS_NAS_EPS_CS_SERVICE_NOTIFICATION 100
#define OGS_NAS_EPS_UPLINK_GENERIC_NAS_TRANSPORT 101
#define OGS_NAS_EPS_DOWNLINK_GENERIC_NAS_TRANSPORT 104
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST 193
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT 194
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT 195
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST 197
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT 198
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT 199
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST 201
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT 202
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT 203
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST 205
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT 206
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST 208
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT 209
#define OGS_NAS_EPS_PDN_DISCONNECT_REQUEST 210
#define OGS_NAS_EPS_PDN_DISCONNECT_REJECT 211
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST 212
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT 213
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST 214
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT 215
#define OGS_NAS_EPS_ESM_INFORMATION_REQUEST 217
#define OGS_NAS_EPS_ESM_INFORMATION_RESPONSE 218
#define OGS_NAS_EPS_ESM_STATUS 232


/*******************************************************
 * ATTACH REQUEST
 ******************************************************/
#define OGS_NAS_EPS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_GUTI_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_ATTACH_REQUEST_DRX_PARAMETER_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_EPS_ATTACH_REQUEST_TMSI_STATUS_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_EPS_ATTACH_REQUEST_SUPPORTED_CODECS_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_EPS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT ((uint64_t)1<<11)
#define OGS_NAS_EPS_ATTACH_REQUEST_DEVICE_PROPERTIES_PRESENT ((uint64_t)1<<12)
#define OGS_NAS_EPS_ATTACH_REQUEST_OLD_GUTI_TYPE_PRESENT ((uint64_t)1<<13)
#define OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT ((uint64_t)1<<14)
#define OGS_NAS_EPS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT ((uint64_t)1<<15)
#define OGS_NAS_EPS_ATTACH_REQUEST_T3324_VALUE_PRESENT ((uint64_t)1<<16)
#define OGS_NAS_EPS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_PRESENT ((uint64_t)1<<17)
#define OGS_NAS_EPS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_PRESENT ((uint64_t)1<<18)
#define OGS_NAS_EPS_ATTACH_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_PRESENT ((uint64_t)1<<19)
#define OGS_NAS_EPS_ATTACH_REQUEST_UE_STATUS_PRESENT ((uint64_t)1<<20)
#define OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_PRESENT ((uint64_t)1<<21)
#define OGS_NAS_EPS_ATTACH_REQUEST_N1_UE_NETWORK_CAPABILITY_PRESENT ((uint64_t)1<<22)
#define OGS_NAS_EPS_ATTACH_REQUEST_UE_RADIO_CAPABILITY_ID_AVAILABILITY_PRESENT ((uint64_t)1<<23)
#define OGS_NAS_EPS_ATTACH_REQUEST_REQUESTED_WUS_ASSISTANCE_INFORMATION_PRESENT ((uint64_t)1<<24)
#define OGS_NAS_EPS_ATTACH_REQUEST_DRX_PARAMETER_IN_NB_S1_MODE_PRESENT ((uint64_t)1<<25)
#define OGS_NAS_EPS_ATTACH_REQUEST_REQUESTED_IMSI_OFFSET_PRESENT ((uint64_t)1<<26)
#define OGS_NAS_EPS_ATTACH_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE 0x19
#define OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_GUTI_TYPE 0x50
#define OGS_NAS_EPS_ATTACH_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE 0x52
#define OGS_NAS_EPS_ATTACH_REQUEST_DRX_PARAMETER_TYPE 0x5C
#define OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_CAPABILITY_TYPE 0x31
#define OGS_NAS_EPS_ATTACH_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_TYPE 0x13
#define OGS_NAS_EPS_ATTACH_REQUEST_TMSI_STATUS_TYPE 0x90
#define OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE 0x11
#define OGS_NAS_EPS_ATTACH_REQUEST_MOBILE_STATION_CLASSMARK_3_TYPE 0x20
#define OGS_NAS_EPS_ATTACH_REQUEST_SUPPORTED_CODECS_TYPE 0x40
#define OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_UPDATE_TYPE_TYPE 0xF0
#define OGS_NAS_EPS_ATTACH_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_TYPE 0x5D
#define OGS_NAS_EPS_ATTACH_REQUEST_DEVICE_PROPERTIES_TYPE 0xD0
#define OGS_NAS_EPS_ATTACH_REQUEST_OLD_GUTI_TYPE_TYPE 0xE0
#define OGS_NAS_EPS_ATTACH_REQUEST_MS_NETWORK_FEATURE_SUPPORT_TYPE 0xC0 
#define OGS_NAS_EPS_ATTACH_REQUEST_TMSI_BASED_NRI_CONTAINER_TYPE 0x10
#define OGS_NAS_EPS_ATTACH_REQUEST_T3324_VALUE_TYPE 0x6A
#define OGS_NAS_EPS_ATTACH_REQUEST_T3412_EXTENDED_VALUE_TYPE 0x5E
#define OGS_NAS_EPS_ATTACH_REQUEST_EXTENDED_DRX_PARAMETERS_TYPE 0x6E
#define OGS_NAS_EPS_ATTACH_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_TYPE 0x6F
#define OGS_NAS_EPS_ATTACH_REQUEST_UE_STATUS_TYPE 0x6D
#define OGS_NAS_EPS_ATTACH_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_TYPE 0x17
#define OGS_NAS_EPS_ATTACH_REQUEST_N1_UE_NETWORK_CAPABILITY_TYPE 0x32
#define OGS_NAS_EPS_ATTACH_REQUEST_UE_RADIO_CAPABILITY_ID_AVAILABILITY_TYPE 0x34
#define OGS_NAS_EPS_ATTACH_REQUEST_REQUESTED_WUS_ASSISTANCE_INFORMATION_TYPE 0x35
#define OGS_NAS_EPS_ATTACH_REQUEST_DRX_PARAMETER_IN_NB_S1_MODE_TYPE 0x36
#define OGS_NAS_EPS_ATTACH_REQUEST_REQUESTED_IMSI_OFFSET_TYPE 0x38

typedef struct ogs_nas_eps_attach_request_s {
    /* Mandatory fields */
    ogs_nas_eps_attach_type_t eps_attach_type;
    ogs_nas_eps_mobile_identity_t eps_mobile_identity;
    ogs_nas_ue_network_capability_t ue_network_capability;
    ogs_nas_esm_message_container_t esm_message_container;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_p_tmsi_signature_t old_p_tmsi_signature;
    ogs_nas_eps_mobile_identity_t additional_guti;
    ogs_nas_tracking_area_identity_t last_visited_registered_tai;
    ogs_nas_drx_parameter_t drx_parameter;
    ogs_nas_ms_network_capability_t ms_network_capability;
    ogs_nas_location_area_identification_t old_location_area_identification;
    ogs_nas_tmsi_status_t tmsi_status;
    ogs_nas_mobile_station_classmark_2_t mobile_station_classmark_2;
    ogs_nas_mobile_station_classmark_3_t mobile_station_classmark_3;
    ogs_nas_supported_codec_list_t supported_codecs;
    ogs_nas_additional_update_type_t additional_update_type;
    ogs_nas_voice_domain_preference_and_ue_usage_setting_t voice_domain_preference_and_ue_usage_setting;
    ogs_nas_device_properties_t device_properties;
    ogs_nas_guti_type_t old_guti_type;
    ogs_nas_ms_network_feature_support_t ms_network_feature_support;
    ogs_nas_network_resource_identifier_container_t tmsi_based_nri_container;
    ogs_nas_gprs_timer_2_t t3324_value;
    ogs_nas_gprs_timer_3_t t3412_extended_value;
    ogs_nas_extended_drx_parameters_t extended_drx_parameters;
    ogs_nas_ue_additional_security_capability_t ue_additional_security_capability;
    ogs_nas_ue_status_t ue_status;
    ogs_nas_additional_information_requested_t additional_information_requested;
    ogs_nas_n1_ue_network_capability_t n1_ue_network_capability;
    ogs_nas_ue_radio_capability_id_availability_t ue_radio_capability_id_availability;
    ogs_nas_wus_assistance_information_t requested_wus_assistance_information;
    ogs_nas_nb_s1_drx_parameter_t drx_parameter_in_nb_s1_mode;
    ogs_nas_imsi_offset_t requested_imsi_offset;
} ogs_nas_eps_attach_request_t;


/*******************************************************
 * ATTACH ACCEPT
 ******************************************************/
#define OGS_NAS_EPS_ATTACH_ACCEPT_GUTI_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_ATTACH_ACCEPT_MS_IDENTITY_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_ATTACH_ACCEPT_EMM_CAUSE_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_ATTACH_ACCEPT_T3402_VALUE_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_ATTACH_ACCEPT_T3423_VALUE_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_EPS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_EPS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_EPS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_EPS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_EPS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_EPS_ATTACH_ACCEPT_T3324_VALUE_PRESENT ((uint64_t)1<<11)
#define OGS_NAS_EPS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT ((uint64_t)1<<12)
#define OGS_NAS_EPS_ATTACH_ACCEPT_DCN_ID_PRESENT ((uint64_t)1<<13)
#define OGS_NAS_EPS_ATTACH_ACCEPT_SMS_SERVICES_STATUS_PRESENT ((uint64_t)1<<14)
#define OGS_NAS_EPS_ATTACH_ACCEPT_NON__NW_PROVIDED_POLICIES_PRESENT ((uint64_t)1<<15)
#define OGS_NAS_EPS_ATTACH_ACCEPT_T3448_VALUE_PRESENT ((uint64_t)1<<16)
#define OGS_NAS_EPS_ATTACH_ACCEPT_NETWORK_POLICY_PRESENT ((uint64_t)1<<17)
#define OGS_NAS_EPS_ATTACH_ACCEPT_T3447_VALUE_PRESENT ((uint64_t)1<<18)
#define OGS_NAS_EPS_ATTACH_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_PRESENT ((uint64_t)1<<19)
#define OGS_NAS_EPS_ATTACH_ACCEPT_CIPHERING_KEY_DATA_PRESENT ((uint64_t)1<<20)
#define OGS_NAS_EPS_ATTACH_ACCEPT_UE_RADIO_CAPABILITY_ID_PRESENT ((uint64_t)1<<21)
#define OGS_NAS_EPS_ATTACH_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT ((uint64_t)1<<22)
#define OGS_NAS_EPS_ATTACH_ACCEPT_NEGOTIATED_WUS_ASSISTANCE_INFORMATION_PRESENT ((uint64_t)1<<23)
#define OGS_NAS_EPS_ATTACH_ACCEPT_NEGOTIATED_DRX_PARAMETER_IN_NB_S1_MODE_PRESENT ((uint64_t)1<<24)
#define OGS_NAS_EPS_ATTACH_ACCEPT_NEGOTIATED_IMSI_OFFSET_PRESENT ((uint64_t)1<<25)
#define OGS_NAS_EPS_ATTACH_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT ((uint64_t)1<<26)
#define OGS_NAS_EPS_ATTACH_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT ((uint64_t)1<<27)
#define OGS_NAS_EPS_ATTACH_ACCEPT_GUTI_TYPE 0x50
#define OGS_NAS_EPS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE 0x13
#define OGS_NAS_EPS_ATTACH_ACCEPT_MS_IDENTITY_TYPE 0x23
#define OGS_NAS_EPS_ATTACH_ACCEPT_EMM_CAUSE_TYPE 0x53
#define OGS_NAS_EPS_ATTACH_ACCEPT_T3402_VALUE_TYPE 0x17
#define OGS_NAS_EPS_ATTACH_ACCEPT_T3423_VALUE_TYPE 0x59
#define OGS_NAS_EPS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_TYPE 0x4A
#define OGS_NAS_EPS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE 0x34
#define OGS_NAS_EPS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE 0x64
#define OGS_NAS_EPS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE 0xF0
#define OGS_NAS_EPS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_TYPE 0x5E
#define OGS_NAS_EPS_ATTACH_ACCEPT_T3324_VALUE_TYPE 0x6A
#define OGS_NAS_EPS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE 0x6E
#define OGS_NAS_EPS_ATTACH_ACCEPT_DCN_ID_TYPE 0x65
#define OGS_NAS_EPS_ATTACH_ACCEPT_SMS_SERVICES_STATUS_TYPE 0xE0
#define OGS_NAS_EPS_ATTACH_ACCEPT_NON__NW_PROVIDED_POLICIES_TYPE 0xD0
#define OGS_NAS_EPS_ATTACH_ACCEPT_T3448_VALUE_TYPE 0x6B
#define OGS_NAS_EPS_ATTACH_ACCEPT_NETWORK_POLICY_TYPE 0xC0
#define OGS_NAS_EPS_ATTACH_ACCEPT_T3447_VALUE_TYPE 0x6C
#define OGS_NAS_EPS_ATTACH_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_TYPE 0x7A
#define OGS_NAS_EPS_ATTACH_ACCEPT_CIPHERING_KEY_DATA_TYPE 0x7C
#define OGS_NAS_EPS_ATTACH_ACCEPT_UE_RADIO_CAPABILITY_ID_TYPE 0x66
#define OGS_NAS_EPS_ATTACH_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE 0xB0
#define OGS_NAS_EPS_ATTACH_ACCEPT_NEGOTIATED_WUS_ASSISTANCE_INFORMATION_TYPE 0x35
#define OGS_NAS_EPS_ATTACH_ACCEPT_NEGOTIATED_DRX_PARAMETER_IN_NB_S1_MODE_TYPE 0x36
#define OGS_NAS_EPS_ATTACH_ACCEPT_NEGOTIATED_IMSI_OFFSET_TYPE 0x38
#define OGS_NAS_EPS_ATTACH_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE 0x1D
#define OGS_NAS_EPS_ATTACH_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE 0x1E

typedef struct ogs_nas_eps_attach_accept_s {
    /* Mandatory fields */
    ogs_nas_eps_attach_result_t eps_attach_result;
    ogs_nas_gprs_timer_t t3412_value;
    ogs_nas_tracking_area_identity_list_t tai_list;
    ogs_nas_esm_message_container_t esm_message_container;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_eps_mobile_identity_t guti;
    ogs_nas_location_area_identification_t location_area_identification;
    ogs_nas_mobile_identity_t ms_identity;
    ogs_nas_emm_cause_t emm_cause;
    ogs_nas_gprs_timer_t t3402_value;
    ogs_nas_gprs_timer_t t3423_value;
    ogs_nas_plmn_list_t equivalent_plmns;
    ogs_nas_emergency_number_list_t emergency_number_list;
    ogs_nas_eps_network_feature_support_t eps_network_feature_support;
    ogs_nas_additional_update_result_t additional_update_result;
    ogs_nas_gprs_timer_3_t t3412_extended_value;
    ogs_nas_gprs_timer_2_t t3324_value;
    ogs_nas_extended_drx_parameters_t extended_drx_parameters;
    ogs_nas_dcn_id_t dcn_id;
    ogs_nas_sms_services_status_t sms_services_status;
    ogs_nas_non__nw_provided_policies_t non__nw_provided_policies;
    ogs_nas_gprs_timer_2_t t3448_value;
    ogs_nas_network_policy_t network_policy;
    ogs_nas_gprs_timer_3_t t3447_value;
    ogs_nas_extended_emergency_number_list_t extended_emergency_number_list;
    ogs_nas_ciphering_key_data_t ciphering_key_data;
    ogs_nas_ue_radio_capability_id_t ue_radio_capability_id;
    ogs_nas_ue_radio_capability_id_deletion_indication_t ue_radio_capability_id_deletion_indication;
    ogs_nas_wus_assistance_information_t negotiated_wus_assistance_information;
    ogs_nas_nb_s1_drx_parameter_t negotiated_drx_parameter_in_nb_s1_mode;
    ogs_nas_imsi_offset_t negotiated_imsi_offset;
    ogs_nas_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming;
    ogs_nas_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service;
} ogs_nas_eps_attach_accept_t;


/*******************************************************
 * ATTACH COMPLETE
 ******************************************************/

typedef struct ogs_nas_eps_attach_complete_s {
    /* Mandatory fields */
    ogs_nas_esm_message_container_t esm_message_container;
} ogs_nas_eps_attach_complete_t;


/*******************************************************
 * ATTACH REJECT
 ******************************************************/
#define OGS_NAS_EPS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_ATTACH_REJECT_T3346_VALUE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_ATTACH_REJECT_T3402_VALUE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_ATTACH_REJECT_LOWER_BOUND_TIMER_VALUE_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_ATTACH_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_EPS_ATTACH_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_EPS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_TYPE 0x78
#define OGS_NAS_EPS_ATTACH_REJECT_T3346_VALUE_TYPE 0x5F
#define OGS_NAS_EPS_ATTACH_REJECT_T3402_VALUE_TYPE 0x16
#define OGS_NAS_EPS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_TYPE 0xA0
#define OGS_NAS_EPS_ATTACH_REJECT_LOWER_BOUND_TIMER_VALUE_TYPE 0x1C
#define OGS_NAS_EPS_ATTACH_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE 0x1D
#define OGS_NAS_EPS_ATTACH_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE 0x1E

typedef struct ogs_nas_eps_attach_reject_s {
    /* Mandatory fields */
    ogs_nas_emm_cause_t emm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_esm_message_container_t esm_message_container;
    ogs_nas_gprs_timer_2_t t3346_value;
    ogs_nas_gprs_timer_2_t t3402_value;
    ogs_nas_extended_emm_cause_t extended_emm_cause;
    ogs_nas_gprs_timer_3_t lower_bound_timer_value;
    ogs_nas_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming;
    ogs_nas_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service;
} ogs_nas_eps_attach_reject_t;


/*******************************************************
 * DETACH REQUEST FROM UE
 ******************************************************/

typedef struct ogs_nas_eps_detach_request_from_ue_s {
    /* Mandatory fields */
    ogs_nas_detach_type_t detach_type;
    ogs_nas_eps_mobile_identity_t eps_mobile_identity;
} ogs_nas_eps_detach_request_from_ue_t;


/*******************************************************
 * DETACH REQUEST TO UE
 ******************************************************/
#define OGS_NAS_EPS_DETACH_REQUEST_EMM_CAUSE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_DETACH_REQUEST_LOWER_BOUND_TIMER_VALUE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_DETACH_REQUEST_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_DETACH_REQUEST_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_DETACH_REQUEST_EMM_CAUSE_TYPE 0x53
#define OGS_NAS_EPS_DETACH_REQUEST_LOWER_BOUND_TIMER_VALUE_TYPE 0x1C
#define OGS_NAS_EPS_DETACH_REQUEST_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE 0x1D
#define OGS_NAS_EPS_DETACH_REQUEST_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE 0x1E

typedef struct ogs_nas_eps_detach_request_to_ue_s {
    /* Mandatory fields */
    ogs_nas_detach_type_t detach_type;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_emm_cause_t emm_cause;
    ogs_nas_gprs_timer_3_t lower_bound_timer_value;
    ogs_nas_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming;
    ogs_nas_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service;
} ogs_nas_eps_detach_request_to_ue_t;


/*******************************************************
 * TRACKING AREA UPDATE REQUEST
 ******************************************************/
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_GPRS_CIPHERING_KEY_SEQUENCE_NUMBER_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_P_TMSI_SIGNATURE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_GUTI_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_PRESENT ((uint64_t)1<<11)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_STATUS_PRESENT ((uint64_t)1<<12)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT ((uint64_t)1<<13)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_3_PRESENT ((uint64_t)1<<14)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_SUPPORTED_CODECS_PRESENT ((uint64_t)1<<15)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_UPDATE_TYPE_PRESENT ((uint64_t)1<<16)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_PRESENT ((uint64_t)1<<17)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_PRESENT ((uint64_t)1<<18)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DEVICE_PROPERTIES_PRESENT ((uint64_t)1<<19)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_FEATURE_SUPPORT_PRESENT ((uint64_t)1<<20)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_BASED_NRI_CONTAINER_PRESENT ((uint64_t)1<<21)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_T3324_VALUE_PRESENT ((uint64_t)1<<22)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_T3412_EXTENDED_VALUE_PRESENT ((uint64_t)1<<23)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_EXTENDED_DRX_PARAMETERS_PRESENT ((uint64_t)1<<24)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_PRESENT ((uint64_t)1<<25)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_STATUS_PRESENT ((uint64_t)1<<26)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_PRESENT ((uint64_t)1<<27)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_N1_UE_NETWORK_CAPABILITY_PRESENT ((uint64_t)1<<28)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_ID_AVAILABILITY_PRESENT ((uint64_t)1<<29)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_REQUESTED_WUS_ASSISTANCE_INFORMATION_PRESENT ((uint64_t)1<<30)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_IN_NB_S1_MODE_PRESENT ((uint64_t)1<<31)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_REQUESTED_IMSI_OFFSET_PRESENT ((uint64_t)1<<32)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_REQUEST_TYPE_PRESENT ((uint64_t)1<<33)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_PAGING_RESTRICTION_PRESENT ((uint64_t)1<<34)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_TYPE 0xB0
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_GPRS_CIPHERING_KEY_SEQUENCE_NUMBER_TYPE 0x80
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_P_TMSI_SIGNATURE_TYPE 0x19
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_GUTI_TYPE 0x50
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_NONCEUE_TYPE 0x55
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_NETWORK_CAPABILITY_TYPE 0x58
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE 0x52
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_TYPE 0x5C
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED_TYPE 0xA0
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE 0x57
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_CAPABILITY_TYPE 0x31
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_LOCATION_AREA_IDENTIFICATION_TYPE 0x13
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_STATUS_TYPE 0x90
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE 0x11
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MOBILE_STATION_CLASSMARK_3_TYPE 0x20
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_SUPPORTED_CODECS_TYPE 0x40
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_UPDATE_TYPE_TYPE 0xF0
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING_TYPE 0x5D
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_OLD_GUTI_TYPE_TYPE 0xE0
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DEVICE_PROPERTIES_TYPE 0xD0
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_MS_NETWORK_FEATURE_SUPPORT_TYPE 0xC0
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_TMSI_BASED_NRI_CONTAINER_TYPE 0x10
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_T3324_VALUE_TYPE 0x6A
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_T3412_EXTENDED_VALUE_TYPE 0x5E
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_EXTENDED_DRX_PARAMETERS_TYPE 0x6E
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_ADDITIONAL_SECURITY_CAPABILITY_TYPE 0x6F
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_STATUS_TYPE 0x6D
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_TYPE 0x17
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_N1_UE_NETWORK_CAPABILITY_TYPE 0x32
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_RADIO_CAPABILITY_ID_AVAILABILITY_TYPE 0x34
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_REQUESTED_WUS_ASSISTANCE_INFORMATION_TYPE 0x35
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_DRX_PARAMETER_IN_NB_S1_MODE_TYPE 0x36
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_REQUESTED_IMSI_OFFSET_TYPE 0x38
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_UE_REQUEST_TYPE_TYPE 0x29
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST_PAGING_RESTRICTION_TYPE 0x28

typedef struct ogs_nas_eps_tracking_area_update_request_s {
    /* Mandatory fields */
    ogs_nas_eps_update_type_t eps_update_type;
    ogs_nas_eps_mobile_identity_t old_guti;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_key_set_identifier_t non_current_native_nas_key_set_identifier;
    ogs_nas_ciphering_key_sequence_number_t gprs_ciphering_key_sequence_number;
    ogs_nas_p_tmsi_signature_t old_p_tmsi_signature;
    ogs_nas_eps_mobile_identity_t additional_guti;
    ogs_nas_nonce_t nonceue;
    ogs_nas_ue_network_capability_t ue_network_capability;
    ogs_nas_tracking_area_identity_t last_visited_registered_tai;
    ogs_nas_drx_parameter_t drx_parameter;
    ogs_nas_ue_radio_capability_information_update_needed_t ue_radio_capability_information_update_needed;
    ogs_nas_eps_bearer_context_status_t eps_bearer_context_status;
    ogs_nas_ms_network_capability_t ms_network_capability;
    ogs_nas_location_area_identification_t old_location_area_identification;
    ogs_nas_tmsi_status_t tmsi_status;
    ogs_nas_mobile_station_classmark_2_t mobile_station_classmark_2;
    ogs_nas_mobile_station_classmark_3_t mobile_station_classmark_3;
    ogs_nas_supported_codec_list_t supported_codecs;
    ogs_nas_additional_update_type_t additional_update_type;
    ogs_nas_voice_domain_preference_and_ue_usage_setting_t voice_domain_preference_and_ue_usage_setting;
    ogs_nas_guti_type_t old_guti_type;
    ogs_nas_device_properties_t device_properties;
    ogs_nas_ms_network_feature_support_t ms_network_feature_support;
    ogs_nas_network_resource_identifier_container_t tmsi_based_nri_container;
    ogs_nas_gprs_timer_2_t t3324_value;
    ogs_nas_gprs_timer_3_t t3412_extended_value;
    ogs_nas_extended_drx_parameters_t extended_drx_parameters;
    ogs_nas_ue_additional_security_capability_t ue_additional_security_capability;
    ogs_nas_ue_status_t ue_status;
    ogs_nas_additional_information_requested_t additional_information_requested;
    ogs_nas_n1_ue_network_capability_t n1_ue_network_capability;
    ogs_nas_ue_radio_capability_id_availability_t ue_radio_capability_id_availability;
    ogs_nas_wus_assistance_information_t requested_wus_assistance_information;
    ogs_nas_nb_s1_drx_parameter_t drx_parameter_in_nb_s1_mode;
    ogs_nas_imsi_offset_t requested_imsi_offset;
    ogs_nas_ue_request_type_t ue_request_type;
    ogs_nas_paging_restriction_t paging_restriction;
} ogs_nas_eps_tracking_area_update_request_t;


/*******************************************************
 * TRACKING AREA UPDATE ACCEPT
 ******************************************************/
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_GUTI_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_MS_IDENTITY_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EMM_CAUSE_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EQUIVALENT_PLMNS_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT ((uint64_t)1<<11)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT ((uint64_t)1<<12)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3412_EXTENDED_VALUE_PRESENT ((uint64_t)1<<13)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3324_VALUE_PRESENT ((uint64_t)1<<14)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT ((uint64_t)1<<15)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_STATUS_PRESENT ((uint64_t)1<<16)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_DCN_ID_PRESENT ((uint64_t)1<<17)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_SMS_SERVICES_STATUS_PRESENT ((uint64_t)1<<18)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NON__NW_POLICIES_PRESENT ((uint64_t)1<<19)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3448_VALUE_PRESENT ((uint64_t)1<<20)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NETWORK_POLICY_PRESENT ((uint64_t)1<<21)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3447_VALUE_PRESENT ((uint64_t)1<<22)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_PRESENT ((uint64_t)1<<23)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_CIPHERING_KEY_DATA_PRESENT ((uint64_t)1<<24)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_UE_RADIO_CAPABILITY_ID_PRESENT ((uint64_t)1<<25)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT ((uint64_t)1<<26)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NEGOTIATED_WUS_ASSISTANCE_INFORMATION_PRESENT ((uint64_t)1<<27)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NEGOTIATED_DRX_PARAMETER_IN_NB_S1_MODE_PRESENT ((uint64_t)1<<28)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NEGOTIATED_IMSI_OFFSET_PRESENT ((uint64_t)1<<29)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_ADDITIONAL_REQUEST_RESULT_PRESENT ((uint64_t)1<<30)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT ((uint64_t)1<<31)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT ((uint64_t)1<<32)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_TYPE 0x5A
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_GUTI_TYPE 0x50
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_TYPE 0x54
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_TYPE 0x57
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE 0x13
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_MS_IDENTITY_TYPE 0x23
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EMM_CAUSE_TYPE 0x53
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_TYPE 0x17
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_TYPE 0x59
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EQUIVALENT_PLMNS_TYPE 0x4A
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE 0x34
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE 0x64
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE 0xF0
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3412_EXTENDED_VALUE_TYPE 0x5E
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3324_VALUE_TYPE 0x6A
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE 0x6E
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_HEADER_COMPRESSION_CONFIGURATION_STATUS_TYPE 0x68
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_DCN_ID_TYPE 0x65
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_SMS_SERVICES_STATUS_TYPE 0xE0
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NON__NW_POLICIES_TYPE 0xD0
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3448_VALUE_TYPE 0x6B
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NETWORK_POLICY_TYPE 0xC0
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_T3447_VALUE_TYPE 0x6C
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_TYPE 0x7A
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_CIPHERING_KEY_DATA_TYPE 0x7C
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_UE_RADIO_CAPABILITY_ID_TYPE 0x66
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE 0xB0
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NEGOTIATED_WUS_ASSISTANCE_INFORMATION_TYPE 0x35
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NEGOTIATED_DRX_PARAMETER_IN_NB_S1_MODE_TYPE 0x36
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_NEGOTIATED_IMSI_OFFSET_TYPE 0x38
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_EPS_ADDITIONAL_REQUEST_RESULT_TYPE 0x37
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE 0x1D
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE 0x1E

typedef struct ogs_nas_eps_tracking_area_update_accept_s {
    /* Mandatory fields */
    ogs_nas_eps_update_result_t eps_update_result;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_gprs_timer_t t3412_value;
    ogs_nas_eps_mobile_identity_t guti;
    ogs_nas_tracking_area_identity_list_t tai_list;
    ogs_nas_eps_bearer_context_status_t eps_bearer_context_status;
    ogs_nas_location_area_identification_t location_area_identification;
    ogs_nas_mobile_identity_t ms_identity;
    ogs_nas_emm_cause_t emm_cause;
    ogs_nas_gprs_timer_t t3402_value;
    ogs_nas_gprs_timer_t t3423_value;
    ogs_nas_plmn_list_t equivalent_plmns;
    ogs_nas_emergency_number_list_t emergency_number_list;
    ogs_nas_eps_network_feature_support_t eps_network_feature_support;
    ogs_nas_additional_update_result_t additional_update_result;
    ogs_nas_gprs_timer_3_t t3412_extended_value;
    ogs_nas_gprs_timer_2_t t3324_value;
    ogs_nas_extended_drx_parameters_t extended_drx_parameters;
    ogs_nas_header_compression_configuration_status_t header_compression_configuration_status;
    ogs_nas_dcn_id_t dcn_id;
    ogs_nas_sms_services_status_t sms_services_status;
    ogs_nas_non__nw_provided_policies_t non__nw_policies;
    ogs_nas_gprs_timer_2_t t3448_value;
    ogs_nas_network_policy_t network_policy;
    ogs_nas_gprs_timer_3_t t3447_value;
    ogs_nas_extended_emergency_number_list_t extended_emergency_number_list;
    ogs_nas_ciphering_key_data_t ciphering_key_data;
    ogs_nas_ue_radio_capability_id_t ue_radio_capability_id;
    ogs_nas_ue_radio_capability_id_deletion_indication_t ue_radio_capability_id_deletion_indication;
    ogs_nas_wus_assistance_information_t negotiated_wus_assistance_information;
    ogs_nas_nb_s1_drx_parameter_t negotiated_drx_parameter_in_nb_s1_mode;
    ogs_nas_imsi_offset_t negotiated_imsi_offset;
    ogs_nas_eps_additional_request_result_t eps_additional_request_result;
    ogs_nas_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming;
    ogs_nas_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service;
} ogs_nas_eps_tracking_area_update_accept_t;


/*******************************************************
 * TRACKING AREA UPDATE REJECT
 ******************************************************/
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_T3346_VALUE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_EXTENDED_EMM_CAUSE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_LOWER_BOUND_TIMER_VALUE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_T3346_VALUE_TYPE 0x5F
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_EXTENDED_EMM_CAUSE_TYPE 0xA0
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_LOWER_BOUND_TIMER_VALUE_TYPE 0x1C
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE 0x1D
#define OGS_NAS_EPS_TRACKING_AREA_UPDATE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE 0x1E

typedef struct ogs_nas_eps_tracking_area_update_reject_s {
    /* Mandatory fields */
    ogs_nas_emm_cause_t emm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_gprs_timer_2_t t3346_value;
    ogs_nas_extended_emm_cause_t extended_emm_cause;
    ogs_nas_gprs_timer_3_t lower_bound_timer_value;
    ogs_nas_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming;
    ogs_nas_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service;
} ogs_nas_eps_tracking_area_update_reject_t;


/*******************************************************
 * EXTENDED SERVICE REQUEST
 ******************************************************/
#define OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_CSFB_RESPONSE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_DEVICE_PROPERTIES_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_UE_REQUEST_TYPE_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_PAGING_RESTRICTION_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_CSFB_RESPONSE_TYPE 0xB0
#define OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE 0x57
#define OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_DEVICE_PROPERTIES_TYPE 0xD0
#define OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_UE_REQUEST_TYPE_TYPE 0x29
#define OGS_NAS_EPS_EXTENDED_SERVICE_REQUEST_PAGING_RESTRICTION_TYPE 0x28

typedef struct ogs_nas_eps_extended_service_request_s {
    /* Mandatory fields */
    ogs_nas_service_type_t service_type;
    ogs_nas_mobile_identity_t m_tmsi;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_csfb_response_t csfb_response;
    ogs_nas_eps_bearer_context_status_t eps_bearer_context_status;
    ogs_nas_device_properties_t device_properties;
    ogs_nas_ue_request_type_t ue_request_type;
    ogs_nas_paging_restriction_t paging_restriction;
} ogs_nas_eps_extended_service_request_t;


/*******************************************************
 * SERVICE REQUEST
 ******************************************************/

typedef struct ogs_nas_eps_service_request_s {
    /* Mandatory fields */
    ogs_nas_ksi_and_sequence_number_t ksi_and_sequence_number;
    ogs_nas_short_mac_t message_authentication_code;
} ogs_nas_eps_service_request_t;


/*******************************************************
 * SERVICE REJECT
 ******************************************************/
#define OGS_NAS_EPS_SERVICE_REJECT_T3442_VALUE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_SERVICE_REJECT_T3346_VALUE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_SERVICE_REJECT_T3448_VALUE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_SERVICE_REJECT_LOWER_BOUND_TIMER_VALUE_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_SERVICE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_SERVICE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_EPS_SERVICE_REJECT_T3442_VALUE_TYPE 0x5B
#define OGS_NAS_EPS_SERVICE_REJECT_T3346_VALUE_TYPE 0x5F
#define OGS_NAS_EPS_SERVICE_REJECT_T3448_VALUE_TYPE 0x6B
#define OGS_NAS_EPS_SERVICE_REJECT_LOWER_BOUND_TIMER_VALUE_TYPE 0x1C
#define OGS_NAS_EPS_SERVICE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE 0x1D
#define OGS_NAS_EPS_SERVICE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE 0x1E

typedef struct ogs_nas_eps_service_reject_s {
    /* Mandatory fields */
    ogs_nas_emm_cause_t emm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_gprs_timer_t t3442_value;
    ogs_nas_gprs_timer_2_t t3346_value;
    ogs_nas_gprs_timer_2_t t3448_value;
    ogs_nas_gprs_timer_3_t lower_bound_timer_value;
    ogs_nas_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming;
    ogs_nas_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service;
} ogs_nas_eps_service_reject_t;


/*******************************************************
 * GUTI REALLOCATION COMMAND
 ******************************************************/
#define OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_TAI_LIST_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_DCN_ID_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_UE_RADIO_CAPABILITY_ID_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_TAI_LIST_TYPE 0x54
#define OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_DCN_ID_TYPE 0x65
#define OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_UE_RADIO_CAPABILITY_ID_TYPE 0x66
#define OGS_NAS_EPS_GUTI_REALLOCATION_COMMAND_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE 0xB0

typedef struct ogs_nas_eps_guti_reallocation_command_s {
    /* Mandatory fields */
    ogs_nas_eps_mobile_identity_t guti;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_tracking_area_identity_list_t tai_list;
    ogs_nas_dcn_id_t dcn_id;
    ogs_nas_ue_radio_capability_id_t ue_radio_capability_id;
    ogs_nas_ue_radio_capability_id_deletion_indication_t ue_radio_capability_id_deletion_indication;
} ogs_nas_eps_guti_reallocation_command_t;


/*******************************************************
 * AUTHENTICATION REQUEST
 ******************************************************/

typedef struct ogs_nas_eps_authentication_request_s {
    /* Mandatory fields */
    ogs_nas_key_set_identifier_t nas_key_set_identifierasme;
    ogs_nas_authentication_parameter_rand_t authentication_parameter_rand;
    ogs_nas_authentication_parameter_autn_t authentication_parameter_autn;
} ogs_nas_eps_authentication_request_t;


/*******************************************************
 * AUTHENTICATION RESPONSE
 ******************************************************/

typedef struct ogs_nas_eps_authentication_response_s {
    /* Mandatory fields */
    ogs_nas_authentication_response_parameter_t authentication_response_parameter;
} ogs_nas_eps_authentication_response_t;


/*******************************************************
 * IDENTITY REQUEST
 ******************************************************/

typedef struct ogs_nas_eps_identity_request_s {
    /* Mandatory fields */
    ogs_nas_identity_type_2_t identity_type;
} ogs_nas_eps_identity_request_t;


/*******************************************************
 * IDENTITY RESPONSE
 ******************************************************/

typedef struct ogs_nas_eps_identity_response_s {
    /* Mandatory fields */
    ogs_nas_mobile_identity_t mobile_identity;
} ogs_nas_eps_identity_response_t;


/*******************************************************
 * AUTHENTICATION FAILURE
 ******************************************************/
#define OGS_NAS_EPS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_TYPE 0x30

typedef struct ogs_nas_eps_authentication_failure_s {
    /* Mandatory fields */
    ogs_nas_emm_cause_t emm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_authentication_failure_parameter_t authentication_failure_parameter;
} ogs_nas_eps_authentication_failure_t;


/*******************************************************
 * SECURITY MODE COMMAND
 ******************************************************/
#define OGS_NAS_EPS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_SECURITY_MODE_COMMAND_REPLAYED_NONCEUE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_SECURITY_MODE_COMMAND_NONCEMME_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_SECURITY_MODE_COMMAND_HASHMME_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_SECURITY_MODE_COMMAND_REPLAYED_UE_ADDITIONAL_SECURITY_CAPABILITY_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_SECURITY_MODE_COMMAND_UE_RADIO_CAPABILITY_ID_REQUEST_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_EPS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_TYPE 0xC0
#define OGS_NAS_EPS_SECURITY_MODE_COMMAND_REPLAYED_NONCEUE_TYPE 0x55
#define OGS_NAS_EPS_SECURITY_MODE_COMMAND_NONCEMME_TYPE 0x56
#define OGS_NAS_EPS_SECURITY_MODE_COMMAND_HASHMME_TYPE 0x4F
#define OGS_NAS_EPS_SECURITY_MODE_COMMAND_REPLAYED_UE_ADDITIONAL_SECURITY_CAPABILITY_TYPE 0x6F
#define OGS_NAS_EPS_SECURITY_MODE_COMMAND_UE_RADIO_CAPABILITY_ID_REQUEST_TYPE 0x37

typedef struct ogs_nas_eps_security_mode_command_s {
    /* Mandatory fields */
    ogs_nas_security_algorithms_t selected_nas_security_algorithms;
    ogs_nas_key_set_identifier_t nas_key_set_identifier;
    ogs_nas_ue_security_capability_t replayed_ue_security_capabilities;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_imeisv_request_t imeisv_request;
    ogs_nas_nonce_t replayed_nonceue;
    ogs_nas_nonce_t noncemme;
    ogs_nas_hashmme_t hashmme;
    ogs_nas_ue_additional_security_capability_t replayed_ue_additional_security_capability;
    ogs_nas_ue_radio_capability_id_request_t ue_radio_capability_id_request;
} ogs_nas_eps_security_mode_command_t;


/*******************************************************
 * SECURITY MODE COMPLETE
 ******************************************************/
#define OGS_NAS_EPS_SECURITY_MODE_COMPLETE_IMEISV_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_SECURITY_MODE_COMPLETE_REPLAYED_NAS_MESSAGE_CONTAINER_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_SECURITY_MODE_COMPLETE_UE_RADIO_CAPABILITY_ID_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_SECURITY_MODE_COMPLETE_IMEISV_TYPE 0x23
#define OGS_NAS_EPS_SECURITY_MODE_COMPLETE_REPLAYED_NAS_MESSAGE_CONTAINER_TYPE 0x79
#define OGS_NAS_EPS_SECURITY_MODE_COMPLETE_UE_RADIO_CAPABILITY_ID_TYPE 0x66

typedef struct ogs_nas_eps_security_mode_complete_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_mobile_identity_t imeisv;
    ogs_nas_replayed_nas_message_container_t replayed_nas_message_container;
    ogs_nas_ue_radio_capability_id_t ue_radio_capability_id;
} ogs_nas_eps_security_mode_complete_t;


/*******************************************************
 * SECURITY MODE REJECT
 ******************************************************/

typedef struct ogs_nas_eps_security_mode_reject_s {
    /* Mandatory fields */
    ogs_nas_emm_cause_t emm_cause;
} ogs_nas_eps_security_mode_reject_t;


/*******************************************************
 * EMM STATUS
 ******************************************************/

typedef struct ogs_nas_eps_emm_status_s {
    /* Mandatory fields */
    ogs_nas_emm_cause_t emm_cause;
} ogs_nas_eps_emm_status_t;


/*******************************************************
 * EMM INFORMATION
 ******************************************************/
#define OGS_NAS_EPS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_EMM_INFORMATION_LOCAL_TIME_ZONE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_EMM_INFORMATION_FULL_NAME_FOR_NETWORK_TYPE 0x43
#define OGS_NAS_EPS_EMM_INFORMATION_SHORT_NAME_FOR_NETWORK_TYPE 0x45
#define OGS_NAS_EPS_EMM_INFORMATION_LOCAL_TIME_ZONE_TYPE 0x46
#define OGS_NAS_EPS_EMM_INFORMATION_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_TYPE 0x47
#define OGS_NAS_EPS_EMM_INFORMATION_NETWORK_DAYLIGHT_SAVING_TIME_TYPE 0x49

typedef struct ogs_nas_eps_emm_information_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_network_name_t full_name_for_network;
    ogs_nas_network_name_t short_name_for_network;
    ogs_nas_time_zone_t local_time_zone;
    ogs_nas_time_zone_and_time_t universal_time_and_local_time_zone;
    ogs_nas_daylight_saving_time_t network_daylight_saving_time;
} ogs_nas_eps_emm_information_t;


/*******************************************************
 * DOWNLINK NAS TRANSPORT
 ******************************************************/

typedef struct ogs_nas_eps_downlink_nas_transport_s {
    /* Mandatory fields */
    ogs_nas_eps_message_container_t nas_message_container;
} ogs_nas_eps_downlink_nas_transport_t;


/*******************************************************
 * UPLINK NAS TRANSPORT
 ******************************************************/

typedef struct ogs_nas_eps_uplink_nas_transport_s {
    /* Mandatory fields */
    ogs_nas_eps_message_container_t nas_message_container;
} ogs_nas_eps_uplink_nas_transport_t;


/*******************************************************
 * CS SERVICE NOTIFICATION
 ******************************************************/
#define OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_CLI_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_SS_CODE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_LCS_INDICATOR_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_CLI_TYPE 0x60
#define OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_SS_CODE_TYPE 0x61
#define OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_LCS_INDICATOR_TYPE 0x62
#define OGS_NAS_EPS_CS_SERVICE_NOTIFICATION_LCS_CLIENT_IDENTITY_TYPE 0x63

typedef struct ogs_nas_eps_cs_service_notification_s {
    /* Mandatory fields */
    ogs_nas_paging_identity_t paging_identity;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_cli_t cli;
    ogs_nas_ss_code_t ss_code;
    ogs_nas_lcs_indicator_t lcs_indicator;
    ogs_nas_lcs_client_identity_t lcs_client_identity;
} ogs_nas_eps_cs_service_notification_t;


/*******************************************************
 * UPLINK GENERIC NAS TRANSPORT
 ******************************************************/
#define OGS_NAS_EPS_UPLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_UPLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE 0x65

typedef struct ogs_nas_eps_uplink_generic_nas_transport_s {
    /* Mandatory fields */
    ogs_nas_generic_message_container_type_t generic_message_container_type;
    ogs_nas_generic_message_container_t generic_message_container;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_additional_information_t additional_information;
} ogs_nas_eps_uplink_generic_nas_transport_t;


/*******************************************************
 * DOWNLINK GENERIC NAS TRANSPORT
 ******************************************************/
#define OGS_NAS_EPS_DOWNLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_DOWNLINK_GENERIC_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE 0x65

typedef struct ogs_nas_eps_downlink_generic_nas_transport_s {
    /* Mandatory fields */
    ogs_nas_generic_message_container_type_t generic_message_container_type;
    ogs_nas_generic_message_container_t generic_message_container;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_additional_information_t additional_information;
} ogs_nas_eps_downlink_generic_nas_transport_t;


/*******************************************************
 * ACTIVATE DEFAULT EPS BEARER CONTEXT REQUEST
 ******************************************************/
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT ((uint64_t)1<<11)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_PRESENT ((uint64_t)1<<12)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<13)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_PRESENT ((uint64_t)1<<14)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_APN_AMBR_PRESENT ((uint64_t)1<<15)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_TYPE 0x5D
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_TYPE 0x30
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE 0x32
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE 0x80
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE 0x34
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_TYPE 0x5E
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_ESM_CAUSE_TYPE 0x58
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONNECTIVITY_TYPE_TYPE 0xB0
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE 0xC0
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x66
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_CONTROL_PLANE_ONLY_INDICATION_TYPE 0x90
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_SERVING_PLMN_RATE_CONTROL_TYPE 0x6E
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_APN_AMBR_TYPE 0x5F

typedef struct ogs_nas_eps_activate_default_eps_bearer_context_request_s {
    /* Mandatory fields */
    ogs_nas_eps_quality_of_service_t eps_qos;
    ogs_nas_access_point_name_t access_point_name;
    ogs_nas_pdn_address_t pdn_address;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_transaction_identifier_t transaction_identifier;
    ogs_nas_quality_of_service_t negotiated_qos;
    ogs_nas_llc_service_access_point_identifier_t negotiated_llc_sapi;
    ogs_nas_radio_priority_t radio_priority;
    ogs_nas_packet_flow_identifier_t packet_flow_identifier;
    ogs_nas_apn_aggregate_maximum_bit_rate_t apn_ambr;
    ogs_nas_esm_cause_t esm_cause;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_connectivity_type_t connectivity_type;
    ogs_nas_wlan_offload_acceptability_t wlan_offload_indication;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_header_compression_configuration_t header_compression_configuration;
    ogs_nas_control_plane_only_indication_t control_plane_only_indication;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    ogs_nas_serving_plmn_rate_control_t serving_plmn_rate_control;
    ogs_nas_extended_apn_aggregate_maximum_bit_rate_t extended_apn_ambr;
} ogs_nas_eps_activate_default_eps_bearer_context_request_t;


/*******************************************************
 * ACTIVATE DEFAULT EPS BEARER CONTEXT ACCEPT
 ******************************************************/
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_activate_default_eps_bearer_context_accept_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_activate_default_eps_bearer_context_accept_t;


/*******************************************************
 * ACTIVATE DEFAULT EPS BEARER CONTEXT REJECT
 ******************************************************/
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_activate_default_eps_bearer_context_reject_s {
    /* Mandatory fields */
    ogs_nas_esm_cause_t esm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_activate_default_eps_bearer_context_reject_t;


/*******************************************************
 * ACTIVATE DEDICATED EPS BEARER CONTEXT REQUEST
 ******************************************************/
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_EPS_QOS_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_TRANSACTION_IDENTIFIER_TYPE 0x5D
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_QOS_TYPE 0x30
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE 0x32
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE 0x80
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE 0x34
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE 0xC0
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_EPS_QOS_TYPE 0x5C

typedef struct ogs_nas_eps_activate_dedicated_eps_bearer_context_request_s {
    /* Mandatory fields */
    ogs_nas_linked_eps_bearer_identity_t linked_eps_bearer_identity;
    ogs_nas_eps_quality_of_service_t eps_qos;
    ogs_nas_traffic_flow_template_t tft;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_transaction_identifier_t transaction_identifier;
    ogs_nas_quality_of_service_t negotiated_qos;
    ogs_nas_llc_service_access_point_identifier_t negotiated_llc_sapi;
    ogs_nas_radio_priority_t radio_priority;
    ogs_nas_packet_flow_identifier_t packet_flow_identifier;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_wlan_offload_acceptability_t wlan_offload_indication;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    ogs_nas_extended_quality_of_service_t extended_eps_qos;
} ogs_nas_eps_activate_dedicated_eps_bearer_context_request_t;


/*******************************************************
 * ACTIVATE DEDICATED EPS BEARER CONTEXT ACCEPT
 ******************************************************/
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_activate_dedicated_eps_bearer_context_accept_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_activate_dedicated_eps_bearer_context_accept_t;


/*******************************************************
 * ACTIVATE DEDICATED EPS BEARER CONTEXT REJECT
 ******************************************************/
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_activate_dedicated_eps_bearer_context_reject_s {
    /* Mandatory fields */
    ogs_nas_esm_cause_t esm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_activate_dedicated_eps_bearer_context_reject_t;


/*******************************************************
 * MODIFY EPS BEARER CONTEXT REQUEST
 ******************************************************/
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_EPS_QOS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_TFT_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_QOS_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<11)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_APN_AMBR_PRESENT ((uint64_t)1<<12)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_EPS_QOS_PRESENT ((uint64_t)1<<13)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_EPS_QOS_TYPE 0x5B
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_TFT_TYPE 0x36
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_QOS_TYPE 0x30
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEGOTIATED_LLC_SAPI_TYPE 0x32
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_RADIO_PRIORITY_TYPE 0x80
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PACKET_FLOW_IDENTIFIER_TYPE 0x34
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_TYPE 0x5E
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE 0xC0
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x66
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_APN_AMBR_TYPE 0x5F
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_EPS_QOS_TYPE 0x5C

typedef struct ogs_nas_eps_modify_eps_bearer_context_request_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_eps_quality_of_service_t new_eps_qos;
    ogs_nas_traffic_flow_template_t tft;
    ogs_nas_quality_of_service_t new_qos;
    ogs_nas_llc_service_access_point_identifier_t negotiated_llc_sapi;
    ogs_nas_radio_priority_t radio_priority;
    ogs_nas_packet_flow_identifier_t packet_flow_identifier;
    ogs_nas_apn_aggregate_maximum_bit_rate_t apn_ambr;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_wlan_offload_acceptability_t wlan_offload_indication;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_header_compression_configuration_t header_compression_configuration;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    ogs_nas_extended_apn_aggregate_maximum_bit_rate_t extended_apn_ambr;
    ogs_nas_extended_quality_of_service_t extended_eps_qos;
} ogs_nas_eps_modify_eps_bearer_context_request_t;


/*******************************************************
 * MODIFY EPS BEARER CONTEXT ACCEPT
 ******************************************************/
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_modify_eps_bearer_context_accept_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_modify_eps_bearer_context_accept_t;


/*******************************************************
 * MODIFY EPS BEARER CONTEXT REJECT
 ******************************************************/
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_MODIFY_EPS_BEARER_CONTEXT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_modify_eps_bearer_context_reject_s {
    /* Mandatory fields */
    ogs_nas_esm_cause_t esm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_modify_eps_bearer_context_reject_t;


/*******************************************************
 * DEACTIVATE EPS BEARER CONTEXT REQUEST
 ******************************************************/
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_T3396_VALUE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_T3396_VALUE_TYPE 0x37
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_WLAN_OFFLOAD_INDICATION_TYPE 0xC0
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_deactivate_eps_bearer_context_request_s {
    /* Mandatory fields */
    ogs_nas_esm_cause_t esm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_gprs_timer_3_t t3396_value;
    ogs_nas_wlan_offload_acceptability_t wlan_offload_indication;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_deactivate_eps_bearer_context_request_t;


/*******************************************************
 * DEACTIVATE EPS BEARER CONTEXT ACCEPT
 ******************************************************/
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_deactivate_eps_bearer_context_accept_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_deactivate_eps_bearer_context_accept_t;


/*******************************************************
 * PDN CONNECTIVITY REQUEST
 ******************************************************/
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_DEVICE_PROPERTIES_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_TYPE 0xD0
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_TYPE 0x28
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_DEVICE_PROPERTIES_TYPE 0xC0
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x66
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_pdn_connectivity_request_s {
    /* Mandatory fields */
    ogs_nas_request_type_t request_type;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_esm_information_transfer_flag_t esm_information_transfer_flag;
    ogs_nas_access_point_name_t access_point_name;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_device_properties_t device_properties;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_header_compression_configuration_t header_compression_configuration;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_pdn_connectivity_request_t;


/*******************************************************
 * PDN CONNECTIVITY REJECT
 ******************************************************/
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_BACK_OFF_TIMER_VALUE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_RE_ATTEMPT_INDICATOR_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_BACK_OFF_TIMER_VALUE_TYPE 0x37
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_RE_ATTEMPT_INDICATOR_TYPE 0x6B
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_PDN_CONNECTIVITY_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_pdn_connectivity_reject_s {
    /* Mandatory fields */
    ogs_nas_esm_cause_t esm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_gprs_timer_3_t back_off_timer_value;
    ogs_nas_re_attempt_indicator_t re_attempt_indicator;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_pdn_connectivity_reject_t;


/*******************************************************
 * PDN DISCONNECT REQUEST
 ******************************************************/
#define OGS_NAS_EPS_PDN_DISCONNECT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_PDN_DISCONNECT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_PDN_DISCONNECT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_PDN_DISCONNECT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_pdn_disconnect_request_s {
    /* Mandatory fields */
    ogs_nas_linked_eps_bearer_identity_t linked_eps_bearer_identity;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_pdn_disconnect_request_t;


/*******************************************************
 * PDN DISCONNECT REJECT
 ******************************************************/
#define OGS_NAS_EPS_PDN_DISCONNECT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_PDN_DISCONNECT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_PDN_DISCONNECT_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_PDN_DISCONNECT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_pdn_disconnect_reject_s {
    /* Mandatory fields */
    ogs_nas_esm_cause_t esm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_pdn_disconnect_reject_t;


/*******************************************************
 * BEARER RESOURCE ALLOCATION REQUEST
 ******************************************************/
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_DEVICE_PROPERTIES_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_EPS_QOS_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_DEVICE_PROPERTIES_TYPE 0xC0
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REQUEST_EXTENDED_EPS_QOS_TYPE 0x5C

typedef struct ogs_nas_eps_bearer_resource_allocation_request_s {
    /* Mandatory fields */
    ogs_nas_linked_eps_bearer_identity_t linked_eps_bearer_identity;
    ogs_nas_traffic_flow_aggregate_description_t traffic_flow_aggregate;
    ogs_nas_eps_quality_of_service_t required_traffic_flow_qos;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_device_properties_t device_properties;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    ogs_nas_extended_quality_of_service_t extended_eps_qos;
} ogs_nas_eps_bearer_resource_allocation_request_t;


/*******************************************************
 * BEARER RESOURCE ALLOCATION REJECT
 ******************************************************/
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE 0x37
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE 0x6B
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_BEARER_RESOURCE_ALLOCATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_bearer_resource_allocation_reject_s {
    /* Mandatory fields */
    ogs_nas_esm_cause_t esm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_gprs_timer_3_t back_off_timer_value;
    ogs_nas_re_attempt_indicator_t re_attempt_indicator;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_bearer_resource_allocation_reject_t;


/*******************************************************
 * BEARER RESOURCE MODIFICATION REQUEST
 ******************************************************/
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_ESM_CAUSE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_DEVICE_PROPERTIES_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_EPS_QOS_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_REQUIRED_TRAFFIC_FLOW_QOS_TYPE 0x5B
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_ESM_CAUSE_TYPE 0x58
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_DEVICE_PROPERTIES_TYPE 0xC0
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x66
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REQUEST_EXTENDED_EPS_QOS_TYPE 0x5C

typedef struct ogs_nas_eps_bearer_resource_modification_request_s {
    /* Mandatory fields */
    ogs_nas_linked_eps_bearer_identity_t eps_bearer_identity_for_packet_filter;
    ogs_nas_traffic_flow_aggregate_description_t traffic_flow_aggregate;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_eps_quality_of_service_t required_traffic_flow_qos;
    ogs_nas_esm_cause_t esm_cause;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_device_properties_t device_properties;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_header_compression_configuration_t header_compression_configuration;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    ogs_nas_extended_quality_of_service_t extended_eps_qos;
} ogs_nas_eps_bearer_resource_modification_request_t;


/*******************************************************
 * BEARER RESOURCE MODIFICATION REJECT
 ******************************************************/
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_NBIFOM_CONTAINER_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE 0x37
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE 0x6B
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_NBIFOM_CONTAINER_TYPE 0x33
#define OGS_NAS_EPS_BEARER_RESOURCE_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_bearer_resource_modification_reject_s {
    /* Mandatory fields */
    ogs_nas_esm_cause_t esm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_gprs_timer_3_t back_off_timer_value;
    ogs_nas_re_attempt_indicator_t re_attempt_indicator;
    ogs_nas_nbifom_container_t nbifom_container;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_bearer_resource_modification_reject_t;


/*******************************************************
 * ESM INFORMATION RESPONSE
 ******************************************************/
#define OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_TYPE 0x28
#define OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x27
#define OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_eps_esm_information_response_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_access_point_name_t access_point_name;
    ogs_nas_protocol_configuration_options_t protocol_configuration_options;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_eps_esm_information_response_t;


/*******************************************************
 * ESM STATUS
 ******************************************************/

typedef struct ogs_nas_eps_esm_status_s {
    /* Mandatory fields */
    ogs_nas_esm_cause_t esm_cause;
} ogs_nas_eps_esm_status_t;


typedef struct ogs_nas_emm_message_s {
    ogs_nas_emm_header_t h;
    union {
        ogs_nas_eps_attach_request_t attach_request;
        ogs_nas_eps_attach_accept_t attach_accept;
        ogs_nas_eps_attach_complete_t attach_complete;
        ogs_nas_eps_attach_reject_t attach_reject;
        ogs_nas_eps_detach_request_from_ue_t detach_request_from_ue;
        ogs_nas_eps_detach_request_to_ue_t detach_request_to_ue;
        ogs_nas_eps_tracking_area_update_request_t tracking_area_update_request;
        ogs_nas_eps_tracking_area_update_accept_t tracking_area_update_accept;
        ogs_nas_eps_tracking_area_update_reject_t tracking_area_update_reject;
        ogs_nas_eps_extended_service_request_t extended_service_request;
        ogs_nas_eps_service_request_t service_request;
        ogs_nas_eps_service_reject_t service_reject;
        ogs_nas_eps_guti_reallocation_command_t guti_reallocation_command;
        ogs_nas_eps_authentication_request_t authentication_request;
        ogs_nas_eps_authentication_response_t authentication_response;
        ogs_nas_eps_identity_request_t identity_request;
        ogs_nas_eps_identity_response_t identity_response;
        ogs_nas_eps_authentication_failure_t authentication_failure;
        ogs_nas_eps_security_mode_command_t security_mode_command;
        ogs_nas_eps_security_mode_complete_t security_mode_complete;
        ogs_nas_eps_security_mode_reject_t security_mode_reject;
        ogs_nas_eps_emm_status_t emm_status;
        ogs_nas_eps_emm_information_t emm_information;
        ogs_nas_eps_downlink_nas_transport_t downlink_nas_transport;
        ogs_nas_eps_uplink_nas_transport_t uplink_nas_transport;
        ogs_nas_eps_cs_service_notification_t cs_service_notification;
        ogs_nas_eps_uplink_generic_nas_transport_t uplink_generic_nas_transport;
        ogs_nas_eps_downlink_generic_nas_transport_t downlink_generic_nas_transport;
    };
} ogs_nas_emm_message_t;

typedef struct ogs_nas_esm_message_s {
    ogs_nas_esm_header_t h;
    union {
        ogs_nas_eps_activate_default_eps_bearer_context_request_t activate_default_eps_bearer_context_request;
        ogs_nas_eps_activate_default_eps_bearer_context_accept_t activate_default_eps_bearer_context_accept;
        ogs_nas_eps_activate_default_eps_bearer_context_reject_t activate_default_eps_bearer_context_reject;
        ogs_nas_eps_activate_dedicated_eps_bearer_context_request_t activate_dedicated_eps_bearer_context_request;
        ogs_nas_eps_activate_dedicated_eps_bearer_context_accept_t activate_dedicated_eps_bearer_context_accept;
        ogs_nas_eps_activate_dedicated_eps_bearer_context_reject_t activate_dedicated_eps_bearer_context_reject;
        ogs_nas_eps_modify_eps_bearer_context_request_t modify_eps_bearer_context_request;
        ogs_nas_eps_modify_eps_bearer_context_accept_t modify_eps_bearer_context_accept;
        ogs_nas_eps_modify_eps_bearer_context_reject_t modify_eps_bearer_context_reject;
        ogs_nas_eps_deactivate_eps_bearer_context_request_t deactivate_eps_bearer_context_request;
        ogs_nas_eps_deactivate_eps_bearer_context_accept_t deactivate_eps_bearer_context_accept;
        ogs_nas_eps_pdn_connectivity_request_t pdn_connectivity_request;
        ogs_nas_eps_pdn_connectivity_reject_t pdn_connectivity_reject;
        ogs_nas_eps_pdn_disconnect_request_t pdn_disconnect_request;
        ogs_nas_eps_pdn_disconnect_reject_t pdn_disconnect_reject;
        ogs_nas_eps_bearer_resource_allocation_request_t bearer_resource_allocation_request;
        ogs_nas_eps_bearer_resource_allocation_reject_t bearer_resource_allocation_reject;
        ogs_nas_eps_bearer_resource_modification_request_t bearer_resource_modification_request;
        ogs_nas_eps_bearer_resource_modification_reject_t bearer_resource_modification_reject;
        ogs_nas_eps_esm_information_response_t esm_information_response;
        ogs_nas_eps_esm_status_t esm_status;
    };
} ogs_nas_esm_message_t;

typedef struct ogs_nas_eps_message_s {
    ogs_nas_eps_security_header_t h;
    union {
        ogs_nas_emm_message_t emm;
        ogs_nas_esm_message_t esm;
    };
} ogs_nas_eps_message_t;

ogs_pkbuf_t *ogs_nas_emm_encode(ogs_nas_eps_message_t *message);
ogs_pkbuf_t *ogs_nas_esm_encode(ogs_nas_eps_message_t *message);
int ogs_nas_emm_decode(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_esm_decode(ogs_nas_eps_message_t *message, ogs_pkbuf_t *pkbuf);
ogs_pkbuf_t *ogs_nas_eps_plain_encode(ogs_nas_eps_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* OGS_NAS_EPS_MESSAGE_H */
