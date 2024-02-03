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
 * Created on: 2024-01-21 18:47:47.820801 by acetcom
 * from 24501-h90.docx
 ******************************************************************************/

#if !defined(OGS_NAS_INSIDE) && !defined(OGS_NAS_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_NAS_5GS_MESSAGE_H
#define OGS_NAS_5GS_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM 0x2e
#define OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM 0x7e

#define OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED 0

typedef struct ogs_nas_5gmm_header_s {
    uint8_t extended_protocol_discriminator;
    uint8_t security_header_type;
    uint8_t message_type;
} __attribute__ ((packed)) ogs_nas_5gmm_header_t;

typedef struct ogs_nas_5gsm_header_s {
    uint8_t extended_protocol_discriminator;
    uint8_t pdu_session_identity;
    uint8_t procedure_transaction_identity;
    uint8_t message_type;
} __attribute__ ((packed)) ogs_nas_5gsm_header_t;

typedef struct ogs_nas_5gs_security_header_s {
    uint8_t extended_protocol_discriminator;
    uint8_t security_header_type;
    uint32_t message_authentication_code;
    uint8_t sequence_number;
} __attribute__ ((packed)) ogs_nas_5gs_security_header_t;

#define OGS_NAS_5GS_REGISTRATION_REQUEST 65
#define OGS_NAS_5GS_REGISTRATION_ACCEPT 66
#define OGS_NAS_5GS_REGISTRATION_COMPLETE 67
#define OGS_NAS_5GS_REGISTRATION_REJECT 68
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_FROM_UE 69
#define OGS_NAS_5GS_DEREGISTRATION_ACCEPT_FROM_UE 70
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE 71
#define OGS_NAS_5GS_DEREGISTRATION_ACCEPT_TO_UE 72
#define OGS_NAS_5GS_SERVICE_REQUEST 76
#define OGS_NAS_5GS_SERVICE_REJECT 77
#define OGS_NAS_5GS_SERVICE_ACCEPT 78
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND 84
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMPLETE 85
#define OGS_NAS_5GS_AUTHENTICATION_REQUEST 86
#define OGS_NAS_5GS_AUTHENTICATION_RESPONSE 87
#define OGS_NAS_5GS_AUTHENTICATION_REJECT 88
#define OGS_NAS_5GS_AUTHENTICATION_FAILURE 89
#define OGS_NAS_5GS_AUTHENTICATION_RESULT 90
#define OGS_NAS_5GS_IDENTITY_REQUEST 91
#define OGS_NAS_5GS_IDENTITY_RESPONSE 92
#define OGS_NAS_5GS_SECURITY_MODE_COMMAND 93
#define OGS_NAS_5GS_SECURITY_MODE_COMPLETE 94
#define OGS_NAS_5GS_SECURITY_MODE_REJECT 95
#define OGS_NAS_5GS_5GMM_STATUS 100
#define OGS_NAS_5GS_NOTIFICATION 101
#define OGS_NAS_5GS_NOTIFICATION_RESPONSE 102
#define OGS_NAS_5GS_UL_NAS_TRANSPORT 103
#define OGS_NAS_5GS_DL_NAS_TRANSPORT 104
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST 193
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT 194
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT 195
#define OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMMAND 197
#define OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMPLETE 198
#define OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT 199
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST 201
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT 202
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND 203
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE 204
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT 205
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST 209
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT 210
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND 211
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE 212
#define OGS_NAS_5GS_5GSM_STATUS 214


/*******************************************************
 * REGISTRATION REQUEST
 ******************************************************/
#define OGS_NAS_5GS_REGISTRATION_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_NSSAI_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_LAST_VISITED_REGISTERED_TAI_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_UPLINK_DATA_STATUS_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_PDU_SESSION_STATUS_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_MICO_INDICATION_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_UE_STATUS_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_ADDITIONAL_GUTI_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_PRESENT ((uint64_t)1<<11)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_UE_USAGE_SETTING_PRESENT ((uint64_t)1<<12)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_DRX_PARAMETERS_PRESENT ((uint64_t)1<<13)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_PRESENT ((uint64_t)1<<14)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_LADN_INDICATION_PRESENT ((uint64_t)1<<15)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_PRESENT ((uint64_t)1<<16)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_PAYLOAD_CONTAINER_PRESENT ((uint64_t)1<<17)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_PRESENT ((uint64_t)1<<18)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_5GS_UPDATE_TYPE_PRESENT ((uint64_t)1<<19)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_MOBILE_STATION_CLASSMARK_2_PRESENT ((uint64_t)1<<20)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_SUPPORTED_CODECS_PRESENT ((uint64_t)1<<21)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT ((uint64_t)1<<22)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_EPS_BEARER_CONTEXT_STATUS_PRESENT ((uint64_t)1<<23)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_EXTENDED_DRX_PARAMETERS_PRESENT ((uint64_t)1<<24)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_T3324_VALUE_PRESENT ((uint64_t)1<<25)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_UE_RADIO_CAPABILITY_ID_PRESENT ((uint64_t)1<<26)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_MAPPED_NSSAI_PRESENT ((uint64_t)1<<27)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_PRESENT ((uint64_t)1<<28)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_WUS_ASSISTANCE_INFORMATION_PRESENT ((uint64_t)1<<29)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_N5GC_INDICATION_PRESENT ((uint64_t)1<<30)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_NB_N1_MODE_DRX_PARAMETERS_PRESENT ((uint64_t)1<<31)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_UE_REQUEST_TYPE_PRESENT ((uint64_t)1<<32)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_PAGING_RESTRICTION_PRESENT ((uint64_t)1<<33)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_SERVICE_LEVEL_AA_CONTAINER_PRESENT ((uint64_t)1<<34)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_NID_PRESENT ((uint64_t)1<<35)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_MS_DETERMINED_PLMN_WITH_DISASTER_CONDITION_PRESENT ((uint64_t)1<<36)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_PEIPS_ASSISTANCE_INFORMATION_PRESENT ((uint64_t)1<<37)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_T3512_VALUE_PRESENT ((uint64_t)1<<38)
#define OGS_NAS_5GS_REGISTRATION_REQUEST_NON_CURRENT_NATIVE_NAS_KEY_SET_IDENTIFIER_TYPE 0xC0
#define OGS_NAS_5GS_REGISTRATION_REQUEST_5GMM_CAPABILITY_TYPE 0x10
#define OGS_NAS_5GS_REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_TYPE 0x2E
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_NSSAI_TYPE 0x2F
#define OGS_NAS_5GS_REGISTRATION_REQUEST_LAST_VISITED_REGISTERED_TAI_TYPE 0x52
#define OGS_NAS_5GS_REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_TYPE 0x17
#define OGS_NAS_5GS_REGISTRATION_REQUEST_UPLINK_DATA_STATUS_TYPE 0x40
#define OGS_NAS_5GS_REGISTRATION_REQUEST_PDU_SESSION_STATUS_TYPE 0x50
#define OGS_NAS_5GS_REGISTRATION_REQUEST_MICO_INDICATION_TYPE 0xB0
#define OGS_NAS_5GS_REGISTRATION_REQUEST_UE_STATUS_TYPE 0x2B
#define OGS_NAS_5GS_REGISTRATION_REQUEST_ADDITIONAL_GUTI_TYPE 0x77
#define OGS_NAS_5GS_REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_TYPE 0x25
#define OGS_NAS_5GS_REGISTRATION_REQUEST_UE_USAGE_SETTING_TYPE 0x18
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_DRX_PARAMETERS_TYPE 0x51
#define OGS_NAS_5GS_REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_TYPE 0x70
#define OGS_NAS_5GS_REGISTRATION_REQUEST_LADN_INDICATION_TYPE 0x74
#define OGS_NAS_5GS_REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_TYPE 0x80
#define OGS_NAS_5GS_REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE 0x7B
#define OGS_NAS_5GS_REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_TYPE 0x90
#define OGS_NAS_5GS_REGISTRATION_REQUEST_5GS_UPDATE_TYPE_TYPE 0x53
#define OGS_NAS_5GS_REGISTRATION_REQUEST_MOBILE_STATION_CLASSMARK_2_TYPE 0x41
#define OGS_NAS_5GS_REGISTRATION_REQUEST_SUPPORTED_CODECS_TYPE 0x42
#define OGS_NAS_5GS_REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_TYPE 0x71
#define OGS_NAS_5GS_REGISTRATION_REQUEST_EPS_BEARER_CONTEXT_STATUS_TYPE 0x60
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_EXTENDED_DRX_PARAMETERS_TYPE 0x6E
#define OGS_NAS_5GS_REGISTRATION_REQUEST_T3324_VALUE_TYPE 0x6A
#define OGS_NAS_5GS_REGISTRATION_REQUEST_UE_RADIO_CAPABILITY_ID_TYPE 0x67
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_MAPPED_NSSAI_TYPE 0x35
#define OGS_NAS_5GS_REGISTRATION_REQUEST_ADDITIONAL_INFORMATION_REQUESTED_TYPE 0x48
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_WUS_ASSISTANCE_INFORMATION_TYPE 0x1A
#define OGS_NAS_5GS_REGISTRATION_REQUEST_N5GC_INDICATION_TYPE 0xA0
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_NB_N1_MODE_DRX_PARAMETERS_TYPE 0x30
#define OGS_NAS_5GS_REGISTRATION_REQUEST_UE_REQUEST_TYPE_TYPE 0x29
#define OGS_NAS_5GS_REGISTRATION_REQUEST_PAGING_RESTRICTION_TYPE 0x28
#define OGS_NAS_5GS_REGISTRATION_REQUEST_SERVICE_LEVEL_AA_CONTAINER_TYPE 0x72
#define OGS_NAS_5GS_REGISTRATION_REQUEST_NID_TYPE 0x32
#define OGS_NAS_5GS_REGISTRATION_REQUEST_MS_DETERMINED_PLMN_WITH_DISASTER_CONDITION_TYPE 0x16
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_PEIPS_ASSISTANCE_INFORMATION_TYPE 0x2A
#define OGS_NAS_5GS_REGISTRATION_REQUEST_REQUESTED_T3512_VALUE_TYPE 0x3B

typedef struct ogs_nas_5gs_registration_request_s {
    /* Mandatory fields */
    ogs_nas_5gs_registration_type_t registration_type;
    ogs_nas_5gs_mobile_identity_t mobile_identity;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_key_set_identifier_t non_current_native_nas_key_set_identifier;
    ogs_nas_5gmm_capability_t gmm_capability;
    ogs_nas_ue_security_capability_t ue_security_capability;
    ogs_nas_nssai_t requested_nssai;
    ogs_nas_5gs_tracking_area_identity_t last_visited_registered_tai;
    ogs_nas_s1_ue_network_capability_t s1_ue_network_capability;
    ogs_nas_uplink_data_status_t uplink_data_status;
    ogs_nas_pdu_session_status_t pdu_session_status;
    ogs_nas_mico_indication_t mico_indication;
    ogs_nas_ue_status_t ue_status;
    ogs_nas_5gs_mobile_identity_t additional_guti;
    ogs_nas_allowed_pdu_session_status_t allowed_pdu_session_status;
    ogs_nas_ue_usage_setting_t ue_usage_setting;
    ogs_nas_5gs_drx_parameters_t requested_drx_parameters;
    ogs_nas_eps_nas_message_container_t eps_nas_message_container;
    ogs_nas_ladn_indication_t ladn_indication;
    ogs_nas_payload_container_type_t payload_container_type;
    ogs_nas_payload_container_t payload_container;
    ogs_nas_network_slicing_indication_t network_slicing_indication;
    ogs_nas_5gs_update_type_t update_type;
    ogs_nas_mobile_station_classmark_2_t mobile_station_classmark_2;
    ogs_nas_supported_codec_list_t supported_codecs;
    ogs_nas_message_container_t nas_message_container;
    ogs_nas_eps_bearer_context_status_t eps_bearer_context_status;
    ogs_nas_extended_drx_parameters_t requested_extended_drx_parameters;
    ogs_nas_gprs_timer_3_t t3324_value;
    ogs_nas_ue_radio_capability_id_t ue_radio_capability_id;
    ogs_nas_mapped_nssai_t requested_mapped_nssai;
    ogs_nas_additional_information_requested_t additional_information_requested;
    ogs_nas_wus_assistance_information_t requested_wus_assistance_information;
    ogs_nas_n5gc_indication_t n5gc_indication;
    ogs_nas_nb_n1_mode_drx_parameters_t requested_nb_n1_mode_drx_parameters;
    ogs_nas_ue_request_type_t ue_request_type;
    ogs_nas_paging_restriction_t paging_restriction;
    ogs_nas_service_level_aa_container_t service_level_aa_container;
    ogs_nas_nid_t nid;
    ogs_nas_plmn_identity_t ms_determined_plmn_with_disaster_condition;
    ogs_nas_peips_assistance_information_t requested_peips_assistance_information;
    ogs_nas_gprs_timer_3_t requested_t3512_value;
} ogs_nas_5gs_registration_request_t;


/*******************************************************
 * REGISTRATION ACCEPT
 ******************************************************/
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_5G_GUTI_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EQUIVALENT_PLMNS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_TAI_LIST_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_ALLOWED_NSSAI_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_REJECTED_NSSAI_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_CONFIGURED_NSSAI_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_STATUS_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_LADN_INFORMATION_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_MICO_INDICATION_PRESENT ((uint64_t)1<<11)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_PRESENT ((uint64_t)1<<12)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_SERVICE_AREA_LIST_PRESENT ((uint64_t)1<<13)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_T3512_VALUE_PRESENT ((uint64_t)1<<14)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NON_3GPP_DE_REGISTRATION_TIMER_VALUE_PRESENT ((uint64_t)1<<15)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_T3502_VALUE_PRESENT ((uint64_t)1<<16)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT ((uint64_t)1<<17)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_PRESENT ((uint64_t)1<<18)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_SOR_TRANSPARENT_CONTAINER_PRESENT ((uint64_t)1<<19)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EAP_MESSAGE_PRESENT ((uint64_t)1<<20)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_PRESENT ((uint64_t)1<<21)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_PRESENT ((uint64_t)1<<22)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_DRX_PARAMETERS_PRESENT ((uint64_t)1<<23)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NON_3GPP_NW_POLICIES_PRESENT ((uint64_t)1<<24)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EPS_BEARER_CONTEXT_STATUS_PRESENT ((uint64_t)1<<25)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_EXTENDED_DRX_PARAMETERS_PRESENT ((uint64_t)1<<26)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_T3447_VALUE_PRESENT ((uint64_t)1<<27)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_T3448_VALUE_PRESENT ((uint64_t)1<<28)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_T3324_VALUE_PRESENT ((uint64_t)1<<29)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_UE_RADIO_CAPABILITY_ID_PRESENT ((uint64_t)1<<30)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT ((uint64_t)1<<31)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_PENDING_NSSAI_PRESENT ((uint64_t)1<<32)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_CIPHERING_KEY_DATA_PRESENT ((uint64_t)1<<33)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_CAG_INFORMATION_LIST_PRESENT ((uint64_t)1<<34)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_TRUNCATED_5G_S_TMSI_CONFIGURATION_PRESENT ((uint64_t)1<<35)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_WUS_ASSISTANCE_INFORMATION_PRESENT ((uint64_t)1<<36)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_NB_N1_MODE_DRX_PARAMETERS_PRESENT ((uint64_t)1<<37)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_REJECTED_NSSAI_PRESENT ((uint64_t)1<<38)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_SERVICE_LEVEL_AA_CONTAINER_PRESENT ((uint64_t)1<<39)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_PEIPS_ASSISTANCE_INFORMATION_PRESENT ((uint64_t)1<<40)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_5GS_ADDITIONAL_REQUEST_RESULT_PRESENT ((uint64_t)1<<41)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NSSRG_INFORMATION_PRESENT ((uint64_t)1<<42)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_DISASTER_ROAMING_WAIT_RANGE_PRESENT ((uint64_t)1<<43)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_DISASTER_RETURN_WAIT_RANGE_PRESENT ((uint64_t)1<<44)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_LIST_OF_PLMNS_TO_BE_USED_IN_DISASTER_CONDITION_PRESENT ((uint64_t)1<<45)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT ((uint64_t)1<<46)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT ((uint64_t)1<<47)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_CAG_INFORMATION_LIST_PRESENT ((uint64_t)1<<48)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NSAG_INFORMATION_PRESENT ((uint64_t)1<<49)
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_5G_GUTI_TYPE 0x77
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EQUIVALENT_PLMNS_TYPE 0x4A
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_TAI_LIST_TYPE 0x54
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_ALLOWED_NSSAI_TYPE 0x15
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_REJECTED_NSSAI_TYPE 0x11
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_CONFIGURED_NSSAI_TYPE 0x31
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_TYPE 0x21
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_STATUS_TYPE 0x50
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_TYPE 0x26
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_TYPE 0x72
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_LADN_INFORMATION_TYPE 0x79
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_MICO_INDICATION_TYPE 0xB0
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_TYPE 0x90
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_SERVICE_AREA_LIST_TYPE 0x27
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_T3512_VALUE_TYPE 0x5E
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NON_3GPP_DE_REGISTRATION_TIMER_VALUE_TYPE 0x5D
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_T3502_VALUE_TYPE 0x16
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EMERGENCY_NUMBER_LIST_TYPE 0x34
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_TYPE 0x7A
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_SOR_TRANSPARENT_CONTAINER_TYPE 0x73
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EAP_MESSAGE_TYPE 0x78
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_TYPE 0xA0
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_TYPE 0x76
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_DRX_PARAMETERS_TYPE 0x51
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NON_3GPP_NW_POLICIES_TYPE 0xD0
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EPS_BEARER_CONTEXT_STATUS_TYPE 0x60
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_EXTENDED_DRX_PARAMETERS_TYPE 0x6E
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_T3447_VALUE_TYPE 0x6C
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_T3448_VALUE_TYPE 0x6B
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_T3324_VALUE_TYPE 0x6A
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_UE_RADIO_CAPABILITY_ID_TYPE 0x67
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE 0xE0
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_PENDING_NSSAI_TYPE 0x39
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_CIPHERING_KEY_DATA_TYPE 0x74
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_CAG_INFORMATION_LIST_TYPE 0x75
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_TRUNCATED_5G_S_TMSI_CONFIGURATION_TYPE 0x1B
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_WUS_ASSISTANCE_INFORMATION_TYPE 0x1C
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_NB_N1_MODE_DRX_PARAMETERS_TYPE 0x29
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_REJECTED_NSSAI_TYPE 0x68
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_SERVICE_LEVEL_AA_CONTAINER_TYPE 0x7B
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NEGOTIATED_PEIPS_ASSISTANCE_INFORMATION_TYPE 0x33
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_5GS_ADDITIONAL_REQUEST_RESULT_TYPE 0x34
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NSSRG_INFORMATION_TYPE 0x70
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_DISASTER_ROAMING_WAIT_RANGE_TYPE 0x14
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_DISASTER_RETURN_WAIT_RANGE_TYPE 0x2C
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_LIST_OF_PLMNS_TO_BE_USED_IN_DISASTER_CONDITION_TYPE 0x13
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE 0x1D
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE 0x1E
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_EXTENDED_CAG_INFORMATION_LIST_TYPE 0x71
#define OGS_NAS_5GS_REGISTRATION_ACCEPT_NSAG_INFORMATION_TYPE 0x7C

typedef struct ogs_nas_5gs_registration_accept_s {
    /* Mandatory fields */
    ogs_nas_5gs_registration_result_t registration_result;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_5gs_mobile_identity_t guti;
    ogs_nas_plmn_list_t equivalent_plmns;
    ogs_nas_5gs_tracking_area_identity_list_t tai_list;
    ogs_nas_nssai_t allowed_nssai;
    ogs_nas_rejected_nssai_t rejected_nssai;
    ogs_nas_nssai_t configured_nssai;
    ogs_nas_5gs_network_feature_support_t network_feature_support;
    ogs_nas_pdu_session_status_t pdu_session_status;
    ogs_nas_pdu_session_reactivation_result_t pdu_session_reactivation_result;
    ogs_nas_pdu_session_reactivation_result_error_cause_t pdu_session_reactivation_result_error_cause;
    ogs_nas_ladn_information_t ladn_information;
    ogs_nas_mico_indication_t mico_indication;
    ogs_nas_network_slicing_indication_t network_slicing_indication;
    ogs_nas_service_area_list_t service_area_list;
    ogs_nas_gprs_timer_3_t t3512_value;
    ogs_nas_gprs_timer_2_t non_3gpp_de_registration_timer_value;
    ogs_nas_gprs_timer_2_t t3502_value;
    ogs_nas_emergency_number_list_t emergency_number_list;
    ogs_nas_extended_emergency_number_list_t extended_emergency_number_list;
    ogs_nas_sor_transparent_container_t sor_transparent_container;
    ogs_nas_eap_message_t eap_message;
    ogs_nas_nssai_inclusion_mode_t nssai_inclusion_mode;
    ogs_nas_operator_defined_access_category_definitions_t operator_defined_access_category_definitions;
    ogs_nas_5gs_drx_parameters_t negotiated_drx_parameters;
    ogs_nas_non_3gpp_nw_provided_policies_t non_3gpp_nw_policies;
    ogs_nas_eps_bearer_context_status_t eps_bearer_context_status;
    ogs_nas_extended_drx_parameters_t negotiated_extended_drx_parameters;
    ogs_nas_gprs_timer_3_t t3447_value;
    ogs_nas_gprs_timer_2_t t3448_value;
    ogs_nas_gprs_timer_3_t t3324_value;
    ogs_nas_ue_radio_capability_id_t ue_radio_capability_id;
    ogs_nas_ue_radio_capability_id_deletion_indication_t ue_radio_capability_id_deletion_indication;
    ogs_nas_nssai_t pending_nssai;
    ogs_nas_ciphering_key_data_t ciphering_key_data;
    ogs_nas_cag_information_list_t cag_information_list;
    ogs_nas_truncated_5g_s_tmsi_configuration_t truncated_s_tmsi_configuration;
    ogs_nas_wus_assistance_information_t negotiated_wus_assistance_information;
    ogs_nas_nb_n1_mode_drx_parameters_t negotiated_nb_n1_mode_drx_parameters;
    ogs_nas_extended_rejected_nssai_t extended_rejected_nssai;
    ogs_nas_service_level_aa_container_t service_level_aa_container;
    ogs_nas_peips_assistance_information_t negotiated_peips_assistance_information;
    ogs_nas_5gs_additional_request_result_t additional_request_result;
    ogs_nas_nssrg_information_t nssrg_information;
    ogs_nas_registration_wait_range_t disaster_roaming_wait_range;
    ogs_nas_registration_wait_range_t disaster_return_wait_range;
    ogs_nas_list_of_plmns_to_be_used_in_disaster_condition_t list_of_plmns_to_be_used_in_disaster_condition;
    ogs_nas_5gs_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming;
    ogs_nas_5gs_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service;
    ogs_nas_extended_cag_information_list_t extended_cag_information_list;
    ogs_nas_nsag_information_t nsag_information;
} ogs_nas_5gs_registration_accept_t;


/*******************************************************
 * REGISTRATION COMPLETE
 ******************************************************/
#define OGS_NAS_5GS_REGISTRATION_COMPLETE_SOR_TRANSPARENT_CONTAINER_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_REGISTRATION_COMPLETE_SOR_TRANSPARENT_CONTAINER_TYPE 0x73

typedef struct ogs_nas_5gs_registration_complete_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_sor_transparent_container_t sor_transparent_container;
} ogs_nas_5gs_registration_complete_t;


/*******************************************************
 * REGISTRATION REJECT
 ******************************************************/
#define OGS_NAS_5GS_REGISTRATION_REJECT_T3346_VALUE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_REGISTRATION_REJECT_T3502_VALUE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_REGISTRATION_REJECT_EAP_MESSAGE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_REGISTRATION_REJECT_REJECTED_NSSAI_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_REGISTRATION_REJECT_CAG_INFORMATION_LIST_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_REGISTRATION_REJECT_EXTENDED_REJECTED_NSSAI_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_REGISTRATION_REJECT_DISASTER_RETURN_WAIT_RANGE_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_5GS_REGISTRATION_REJECT_EXTENDED_CAG_INFORMATION_LIST_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_5GS_REGISTRATION_REJECT_LOWER_BOUND_TIMER_VALUE_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_5GS_REGISTRATION_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_5GS_REGISTRATION_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_5GS_REGISTRATION_REJECT_T3346_VALUE_TYPE 0x5F
#define OGS_NAS_5GS_REGISTRATION_REJECT_T3502_VALUE_TYPE 0x16
#define OGS_NAS_5GS_REGISTRATION_REJECT_EAP_MESSAGE_TYPE 0x78
#define OGS_NAS_5GS_REGISTRATION_REJECT_REJECTED_NSSAI_TYPE 0x69
#define OGS_NAS_5GS_REGISTRATION_REJECT_CAG_INFORMATION_LIST_TYPE 0x75
#define OGS_NAS_5GS_REGISTRATION_REJECT_EXTENDED_REJECTED_NSSAI_TYPE 0x68
#define OGS_NAS_5GS_REGISTRATION_REJECT_DISASTER_RETURN_WAIT_RANGE_TYPE 0x2C
#define OGS_NAS_5GS_REGISTRATION_REJECT_EXTENDED_CAG_INFORMATION_LIST_TYPE 0x71
#define OGS_NAS_5GS_REGISTRATION_REJECT_LOWER_BOUND_TIMER_VALUE_TYPE 0x3A
#define OGS_NAS_5GS_REGISTRATION_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE 0x1D
#define OGS_NAS_5GS_REGISTRATION_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE 0x1E

typedef struct ogs_nas_5gs_registration_reject_s {
    /* Mandatory fields */
    ogs_nas_5gmm_cause_t gmm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_gprs_timer_2_t t3346_value;
    ogs_nas_gprs_timer_2_t t3502_value;
    ogs_nas_eap_message_t eap_message;
    ogs_nas_rejected_nssai_t rejected_nssai;
    ogs_nas_cag_information_list_t cag_information_list;
    ogs_nas_extended_rejected_nssai_t extended_rejected_nssai;
    ogs_nas_registration_wait_range_t disaster_return_wait_range;
    ogs_nas_extended_cag_information_list_t extended_cag_information_list;
    ogs_nas_gprs_timer_3_t lower_bound_timer_value;
    ogs_nas_5gs_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming;
    ogs_nas_5gs_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service;
} ogs_nas_5gs_registration_reject_t;


/*******************************************************
 * DEREGISTRATION REQUEST FROM UE
 ******************************************************/

typedef struct ogs_nas_5gs_deregistration_request_from_ue_s {
    /* Mandatory fields */
    ogs_nas_de_registration_type_t de_registration_type;
    ogs_nas_5gs_mobile_identity_t mobile_identity;
} ogs_nas_5gs_deregistration_request_from_ue_t;


/*******************************************************
 * DEREGISTRATION REQUEST TO UE
 ******************************************************/
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_5GMM_CAUSE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_T3346_VALUE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_REJECTED_NSSAI_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_CAG_INFORMATION_LIST_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_EXTENDED_REJECTED_NSSAI_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_DISASTER_RETURN_WAIT_RANGE_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_EXTENDED_CAG_INFORMATION_LIST_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_LOWER_BOUND_TIMER_VALUE_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_5GMM_CAUSE_TYPE 0x58
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_T3346_VALUE_TYPE 0x5F
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_REJECTED_NSSAI_TYPE 0x6D
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_CAG_INFORMATION_LIST_TYPE 0x75
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_EXTENDED_REJECTED_NSSAI_TYPE 0x68
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_DISASTER_RETURN_WAIT_RANGE_TYPE 0x2C
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_EXTENDED_CAG_INFORMATION_LIST_TYPE 0x71
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_LOWER_BOUND_TIMER_VALUE_TYPE 0x3A
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE 0x1D
#define OGS_NAS_5GS_DEREGISTRATION_REQUEST_TO_UE_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE 0x1E

typedef struct ogs_nas_5gs_deregistration_request_to_ue_s {
    /* Mandatory fields */
    ogs_nas_de_registration_type_t de_registration_type;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_5gmm_cause_t gmm_cause;
    ogs_nas_gprs_timer_2_t t3346_value;
    ogs_nas_rejected_nssai_t rejected_nssai;
    ogs_nas_cag_information_list_t cag_information_list;
    ogs_nas_extended_rejected_nssai_t extended_rejected_nssai;
    ogs_nas_registration_wait_range_t disaster_return_wait_range;
    ogs_nas_extended_cag_information_list_t extended_cag_information_list;
    ogs_nas_gprs_timer_3_t lower_bound_timer_value;
    ogs_nas_5gs_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming;
    ogs_nas_5gs_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service;
} ogs_nas_5gs_deregistration_request_to_ue_t;


/*******************************************************
 * SERVICE REQUEST
 ******************************************************/
#define OGS_NAS_5GS_SERVICE_REQUEST_UPLINK_DATA_STATUS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_SERVICE_REQUEST_PDU_SESSION_STATUS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_SERVICE_REQUEST_ALLOWED_PDU_SESSION_STATUS_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_SERVICE_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_SERVICE_REQUEST_UE_REQUEST_TYPE_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_SERVICE_REQUEST_PAGING_RESTRICTION_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_SERVICE_REQUEST_UPLINK_DATA_STATUS_TYPE 0x40
#define OGS_NAS_5GS_SERVICE_REQUEST_PDU_SESSION_STATUS_TYPE 0x50
#define OGS_NAS_5GS_SERVICE_REQUEST_ALLOWED_PDU_SESSION_STATUS_TYPE 0x25
#define OGS_NAS_5GS_SERVICE_REQUEST_NAS_MESSAGE_CONTAINER_TYPE 0x71
#define OGS_NAS_5GS_SERVICE_REQUEST_UE_REQUEST_TYPE_TYPE 0x29
#define OGS_NAS_5GS_SERVICE_REQUEST_PAGING_RESTRICTION_TYPE 0x28

typedef struct ogs_nas_5gs_service_request_s {
    /* Mandatory fields */
    ogs_nas_key_set_identifier_t ngksi;
    ogs_nas_5gs_mobile_identity_t s_tmsi;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_uplink_data_status_t uplink_data_status;
    ogs_nas_pdu_session_status_t pdu_session_status;
    ogs_nas_allowed_pdu_session_status_t allowed_pdu_session_status;
    ogs_nas_message_container_t nas_message_container;
    ogs_nas_ue_request_type_t ue_request_type;
    ogs_nas_paging_restriction_t paging_restriction;
} ogs_nas_5gs_service_request_t;


/*******************************************************
 * SERVICE REJECT
 ******************************************************/
#define OGS_NAS_5GS_SERVICE_REJECT_PDU_SESSION_STATUS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_SERVICE_REJECT_T3346_VALUE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_SERVICE_REJECT_EAP_MESSAGE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_SERVICE_REJECT_T3448_VALUE_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_SERVICE_REJECT_CAG_INFORMATION_LIST_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_SERVICE_REJECT_DISASTER_RETURN_WAIT_RANGE_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_SERVICE_REJECT_EXTENDED_CAG_INFORMATION_LIST_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_5GS_SERVICE_REJECT_LOWER_BOUND_TIMER_VALUE_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_5GS_SERVICE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_5GS_SERVICE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_5GS_SERVICE_REJECT_PDU_SESSION_STATUS_TYPE 0x50
#define OGS_NAS_5GS_SERVICE_REJECT_T3346_VALUE_TYPE 0x5F
#define OGS_NAS_5GS_SERVICE_REJECT_EAP_MESSAGE_TYPE 0x78
#define OGS_NAS_5GS_SERVICE_REJECT_T3448_VALUE_TYPE 0x6B
#define OGS_NAS_5GS_SERVICE_REJECT_CAG_INFORMATION_LIST_TYPE 0x75
#define OGS_NAS_5GS_SERVICE_REJECT_DISASTER_RETURN_WAIT_RANGE_TYPE 0x2C
#define OGS_NAS_5GS_SERVICE_REJECT_EXTENDED_CAG_INFORMATION_LIST_TYPE 0x71
#define OGS_NAS_5GS_SERVICE_REJECT_LOWER_BOUND_TIMER_VALUE_TYPE 0x3A
#define OGS_NAS_5GS_SERVICE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE 0x1D
#define OGS_NAS_5GS_SERVICE_REJECT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE 0x1E

typedef struct ogs_nas_5gs_service_reject_s {
    /* Mandatory fields */
    ogs_nas_5gmm_cause_t gmm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_pdu_session_status_t pdu_session_status;
    ogs_nas_gprs_timer_2_t t3346_value;
    ogs_nas_eap_message_t eap_message;
    ogs_nas_gprs_timer_2_t t3448_value;
    ogs_nas_cag_information_list_t cag_information_list;
    ogs_nas_registration_wait_range_t disaster_return_wait_range;
    ogs_nas_extended_cag_information_list_t extended_cag_information_list;
    ogs_nas_gprs_timer_3_t lower_bound_timer_value;
    ogs_nas_5gs_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming;
    ogs_nas_5gs_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service;
} ogs_nas_5gs_service_reject_t;


/*******************************************************
 * SERVICE ACCEPT
 ******************************************************/
#define OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_STATUS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_SERVICE_ACCEPT_EAP_MESSAGE_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_SERVICE_ACCEPT_T3448_VALUE_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_SERVICE_ACCEPT_5GS_ADDITIONAL_REQUEST_RESULT_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_SERVICE_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_5GS_SERVICE_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_STATUS_TYPE 0x50
#define OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_TYPE 0x26
#define OGS_NAS_5GS_SERVICE_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_TYPE 0x72
#define OGS_NAS_5GS_SERVICE_ACCEPT_EAP_MESSAGE_TYPE 0x78
#define OGS_NAS_5GS_SERVICE_ACCEPT_T3448_VALUE_TYPE 0x6B
#define OGS_NAS_5GS_SERVICE_ACCEPT_5GS_ADDITIONAL_REQUEST_RESULT_TYPE 0x34
#define OGS_NAS_5GS_SERVICE_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FOR_ROAMING_TYPE 0x1D
#define OGS_NAS_5GS_SERVICE_ACCEPT_FORBIDDEN_TAI_FOR_THE_LIST_OF_5GS_FORBIDDEN_TRACKING_AREAS_FORREGIONAL_PROVISION_OF_SERVICE_TYPE 0x1E

typedef struct ogs_nas_5gs_service_accept_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_pdu_session_status_t pdu_session_status;
    ogs_nas_pdu_session_reactivation_result_t pdu_session_reactivation_result;
    ogs_nas_pdu_session_reactivation_result_error_cause_t pdu_session_reactivation_result_error_cause;
    ogs_nas_eap_message_t eap_message;
    ogs_nas_gprs_timer_2_t t3448_value;
    ogs_nas_5gs_additional_request_result_t additional_request_result;
    ogs_nas_5gs_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_for_roaming;
    ogs_nas_5gs_tracking_area_identity_list_t forbidden_tai_for_the_list_of_forbidden_tracking_areas_forregional_provision_of_service;
} ogs_nas_5gs_service_accept_t;


/*******************************************************
 * CONFIGURATION UPDATE COMMAND
 ******************************************************/
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CONFIGURATION_UPDATE_INDICATION_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_5G_GUTI_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_TAI_LIST_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_ALLOWED_NSSAI_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SERVICE_AREA_LIST_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_FULL_NAME_FOR_NETWORK_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SHORT_NAME_FOR_NETWORK_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LOCAL_TIME_ZONE_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NETWORK_DAYLIGHT_SAVING_TIME_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LADN_INFORMATION_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_MICO_INDICATION_PRESENT ((uint64_t)1<<11)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NETWORK_SLICING_INDICATION_PRESENT ((uint64_t)1<<12)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CONFIGURED_NSSAI_PRESENT ((uint64_t)1<<13)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_REJECTED_NSSAI_PRESENT ((uint64_t)1<<14)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_PRESENT ((uint64_t)1<<15)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SMS_INDICATION_PRESENT ((uint64_t)1<<16)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_T3447_VALUE_PRESENT ((uint64_t)1<<17)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CAG_INFORMATION_LIST_PRESENT ((uint64_t)1<<18)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UE_RADIO_CAPABILITY_ID_PRESENT ((uint64_t)1<<19)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_PRESENT ((uint64_t)1<<20)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_5GS_REGISTRATION_RESULT_PRESENT ((uint64_t)1<<21)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_TRUNCATED_5G_S_TMSI_CONFIGURATION_PRESENT ((uint64_t)1<<22)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_ADDITIONAL_CONFIGURATION_INDICATION_PRESENT ((uint64_t)1<<23)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_EXTENDED_REJECTED_NSSAI_PRESENT ((uint64_t)1<<24)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SERVICE_LEVEL_AA_CONTAINER_PRESENT ((uint64_t)1<<25)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NSSRG_INFORMATION_PRESENT ((uint64_t)1<<26)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_DISASTER_ROAMING_WAIT_RANGE_PRESENT ((uint64_t)1<<27)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_DISASTER_RETURN_WAIT_RANGE_PRESENT ((uint64_t)1<<28)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LIST_OF_PLMNS_TO_BE_USED_IN_DISASTER_CONDITION_PRESENT ((uint64_t)1<<29)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_EXTENDED_CAG_INFORMATION_LIST_PRESENT ((uint64_t)1<<30)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UPDATED_PEIPS_ASSISTANCE_INFORMATION_PRESENT ((uint64_t)1<<31)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NSAG_INFORMATION_PRESENT ((uint64_t)1<<32)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_PRIORITY_INDICATOR_PRESENT ((uint64_t)1<<33)
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CONFIGURATION_UPDATE_INDICATION_TYPE 0xD0
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_5G_GUTI_TYPE 0x77
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_TAI_LIST_TYPE 0x54
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_ALLOWED_NSSAI_TYPE 0x15
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SERVICE_AREA_LIST_TYPE 0x27
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_FULL_NAME_FOR_NETWORK_TYPE 0x43
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SHORT_NAME_FOR_NETWORK_TYPE 0x45
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LOCAL_TIME_ZONE_TYPE 0x46
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_TYPE 0x47
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NETWORK_DAYLIGHT_SAVING_TIME_TYPE 0x49
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LADN_INFORMATION_TYPE 0x79
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_MICO_INDICATION_TYPE 0xB0
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NETWORK_SLICING_INDICATION_TYPE 0x90
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CONFIGURED_NSSAI_TYPE 0x31
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_REJECTED_NSSAI_TYPE 0x11
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_TYPE 0x76
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SMS_INDICATION_TYPE 0xF0
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_T3447_VALUE_TYPE 0x6C
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_CAG_INFORMATION_LIST_TYPE 0x75
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UE_RADIO_CAPABILITY_ID_TYPE 0x67
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UE_RADIO_CAPABILITY_ID_DELETION_INDICATION_TYPE 0xA0
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_5GS_REGISTRATION_RESULT_TYPE 0x44
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_TRUNCATED_5G_S_TMSI_CONFIGURATION_TYPE 0x1B
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_ADDITIONAL_CONFIGURATION_INDICATION_TYPE 0xC0
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_EXTENDED_REJECTED_NSSAI_TYPE 0x68
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_SERVICE_LEVEL_AA_CONTAINER_TYPE 0x72
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NSSRG_INFORMATION_TYPE 0x70
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_DISASTER_ROAMING_WAIT_RANGE_TYPE 0x14
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_DISASTER_RETURN_WAIT_RANGE_TYPE 0x2C
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_LIST_OF_PLMNS_TO_BE_USED_IN_DISASTER_CONDITION_TYPE 0x13
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_EXTENDED_CAG_INFORMATION_LIST_TYPE 0x71
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_UPDATED_PEIPS_ASSISTANCE_INFORMATION_TYPE 0x1F
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_NSAG_INFORMATION_TYPE 0x73
#define OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND_PRIORITY_INDICATOR_TYPE 0xE0

typedef struct ogs_nas_5gs_configuration_update_command_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_configuration_update_indication_t configuration_update_indication;
    ogs_nas_5gs_mobile_identity_t guti;
    ogs_nas_5gs_tracking_area_identity_list_t tai_list;
    ogs_nas_nssai_t allowed_nssai;
    ogs_nas_service_area_list_t service_area_list;
    ogs_nas_network_name_t full_name_for_network;
    ogs_nas_network_name_t short_name_for_network;
    ogs_nas_time_zone_t local_time_zone;
    ogs_nas_time_zone_and_time_t universal_time_and_local_time_zone;
    ogs_nas_daylight_saving_time_t network_daylight_saving_time;
    ogs_nas_ladn_information_t ladn_information;
    ogs_nas_mico_indication_t mico_indication;
    ogs_nas_network_slicing_indication_t network_slicing_indication;
    ogs_nas_nssai_t configured_nssai;
    ogs_nas_rejected_nssai_t rejected_nssai;
    ogs_nas_operator_defined_access_category_definitions_t operator_defined_access_category_definitions;
    ogs_nas_sms_indication_t sms_indication;
    ogs_nas_gprs_timer_3_t t3447_value;
    ogs_nas_cag_information_list_t cag_information_list;
    ogs_nas_ue_radio_capability_id_t ue_radio_capability_id;
    ogs_nas_ue_radio_capability_id_deletion_indication_t ue_radio_capability_id_deletion_indication;
    ogs_nas_5gs_registration_result_t registration_result;
    ogs_nas_truncated_5g_s_tmsi_configuration_t truncated_s_tmsi_configuration;
    ogs_nas_additional_configuration_indication_t additional_configuration_indication;
    ogs_nas_extended_rejected_nssai_t extended_rejected_nssai;
    ogs_nas_service_level_aa_container_t service_level_aa_container;
    ogs_nas_nssrg_information_t nssrg_information;
    ogs_nas_registration_wait_range_t disaster_roaming_wait_range;
    ogs_nas_registration_wait_range_t disaster_return_wait_range;
    ogs_nas_list_of_plmns_to_be_used_in_disaster_condition_t list_of_plmns_to_be_used_in_disaster_condition;
    ogs_nas_extended_cag_information_list_t extended_cag_information_list;
    ogs_nas_peips_assistance_information_t updated_peips_assistance_information;
    ogs_nas_nsag_information_t nsag_information;
    ogs_nas_priority_indicator_t priority_indicator;
} ogs_nas_5gs_configuration_update_command_t;


/*******************************************************
 * AUTHENTICATION REQUEST
 ******************************************************/
#define OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_RAND_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_AUTN_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_AUTHENTICATION_REQUEST_EAP_MESSAGE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_RAND_TYPE 0x21
#define OGS_NAS_5GS_AUTHENTICATION_REQUEST_AUTHENTICATION_PARAMETER_AUTN_TYPE 0x20
#define OGS_NAS_5GS_AUTHENTICATION_REQUEST_EAP_MESSAGE_TYPE 0x78

typedef struct ogs_nas_5gs_authentication_request_s {
    /* Mandatory fields */
    ogs_nas_key_set_identifier_t ngksi;
    ogs_nas_abba_t abba;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_authentication_parameter_rand_t authentication_parameter_rand;
    ogs_nas_authentication_parameter_autn_t authentication_parameter_autn;
    ogs_nas_eap_message_t eap_message;
} ogs_nas_5gs_authentication_request_t;


/*******************************************************
 * AUTHENTICATION RESPONSE
 ******************************************************/
#define OGS_NAS_5GS_AUTHENTICATION_RESPONSE_AUTHENTICATION_RESPONSE_PARAMETER_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_AUTHENTICATION_RESPONSE_EAP_MESSAGE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_AUTHENTICATION_RESPONSE_AUTHENTICATION_RESPONSE_PARAMETER_TYPE 0x2D
#define OGS_NAS_5GS_AUTHENTICATION_RESPONSE_EAP_MESSAGE_TYPE 0x78

typedef struct ogs_nas_5gs_authentication_response_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_authentication_response_parameter_t authentication_response_parameter;
    ogs_nas_eap_message_t eap_message;
} ogs_nas_5gs_authentication_response_t;


/*******************************************************
 * AUTHENTICATION REJECT
 ******************************************************/
#define OGS_NAS_5GS_AUTHENTICATION_REJECT_EAP_MESSAGE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_AUTHENTICATION_REJECT_EAP_MESSAGE_TYPE 0x78

typedef struct ogs_nas_5gs_authentication_reject_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_eap_message_t eap_message;
} ogs_nas_5gs_authentication_reject_t;


/*******************************************************
 * AUTHENTICATION FAILURE
 ******************************************************/
#define OGS_NAS_5GS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_TYPE 0x30

typedef struct ogs_nas_5gs_authentication_failure_s {
    /* Mandatory fields */
    ogs_nas_5gmm_cause_t gmm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_authentication_failure_parameter_t authentication_failure_parameter;
} ogs_nas_5gs_authentication_failure_t;


/*******************************************************
 * AUTHENTICATION RESULT
 ******************************************************/
#define OGS_NAS_5GS_AUTHENTICATION_RESULT_ABBA_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_AUTHENTICATION_RESULT_ABBA_TYPE 0x38

typedef struct ogs_nas_5gs_authentication_result_s {
    /* Mandatory fields */
    ogs_nas_key_set_identifier_t ngksi;
    ogs_nas_eap_message_t eap_message;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_abba_t abba;
} ogs_nas_5gs_authentication_result_t;


/*******************************************************
 * IDENTITY REQUEST
 ******************************************************/

typedef struct ogs_nas_5gs_identity_request_s {
    /* Mandatory fields */
    ogs_nas_5gs_identity_type_t identity_type;
} ogs_nas_5gs_identity_request_t;


/*******************************************************
 * IDENTITY RESPONSE
 ******************************************************/

typedef struct ogs_nas_5gs_identity_response_s {
    /* Mandatory fields */
    ogs_nas_5gs_mobile_identity_t mobile_identity;
} ogs_nas_5gs_identity_response_t;


/*******************************************************
 * SECURITY MODE COMMAND
 ******************************************************/
#define OGS_NAS_5GS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_SECURITY_MODE_COMMAND_SELECTED_EPS_NAS_SECURITY_ALGORITHMS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_SECURITY_MODE_COMMAND_EAP_MESSAGE_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_SECURITY_MODE_COMMAND_ABBA_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_SECURITY_MODE_COMMAND_REPLAYED_S1_UE_SECURITY_CAPABILITIES_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_SECURITY_MODE_COMMAND_IMEISV_REQUEST_TYPE 0xE0
#define OGS_NAS_5GS_SECURITY_MODE_COMMAND_SELECTED_EPS_NAS_SECURITY_ALGORITHMS_TYPE 0x57
#define OGS_NAS_5GS_SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_TYPE 0x36
#define OGS_NAS_5GS_SECURITY_MODE_COMMAND_EAP_MESSAGE_TYPE 0x78
#define OGS_NAS_5GS_SECURITY_MODE_COMMAND_ABBA_TYPE 0x38
#define OGS_NAS_5GS_SECURITY_MODE_COMMAND_REPLAYED_S1_UE_SECURITY_CAPABILITIES_TYPE 0x19

typedef struct ogs_nas_5gs_security_mode_command_s {
    /* Mandatory fields */
    ogs_nas_security_algorithms_t selected_nas_security_algorithms;
    ogs_nas_key_set_identifier_t ngksi;
    ogs_nas_ue_security_capability_t replayed_ue_security_capabilities;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_imeisv_request_t imeisv_request;
    ogs_nas_eps_nas_security_algorithms_t selected_eps_nas_security_algorithms;
    ogs_nas_additional_5g_security_information_t additional_security_information;
    ogs_nas_eap_message_t eap_message;
    ogs_nas_abba_t abba;
    ogs_nas_s1_ue_security_capability_t replayed_s1_ue_security_capabilities;
} ogs_nas_5gs_security_mode_command_t;


/*******************************************************
 * SECURITY MODE COMPLETE
 ******************************************************/
#define OGS_NAS_5GS_SECURITY_MODE_COMPLETE_IMEISV_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NAS_MESSAGE_CONTAINER_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NON_IMEISV_PEI_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_SECURITY_MODE_COMPLETE_IMEISV_TYPE 0x77
#define OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NAS_MESSAGE_CONTAINER_TYPE 0x71
#define OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NON_IMEISV_PEI_TYPE 0x78

typedef struct ogs_nas_5gs_security_mode_complete_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_5gs_mobile_identity_t imeisv;
    ogs_nas_message_container_t nas_message_container;
    ogs_nas_5gs_mobile_identity_t non_imeisv_pei;
} ogs_nas_5gs_security_mode_complete_t;


/*******************************************************
 * SECURITY MODE REJECT
 ******************************************************/

typedef struct ogs_nas_5gs_security_mode_reject_s {
    /* Mandatory fields */
    ogs_nas_5gmm_cause_t gmm_cause;
} ogs_nas_5gs_security_mode_reject_t;


/*******************************************************
 * 5GMM STATUS
 ******************************************************/

typedef struct ogs_nas_5gs_5gmm_status_s {
    /* Mandatory fields */
    ogs_nas_5gmm_cause_t gmm_cause;
} ogs_nas_5gs_5gmm_status_t;


/*******************************************************
 * NOTIFICATION
 ******************************************************/

typedef struct ogs_nas_5gs_notification_s {
    /* Mandatory fields */
    ogs_nas_access_type_t access_type;
} ogs_nas_5gs_notification_t;


/*******************************************************
 * NOTIFICATION RESPONSE
 ******************************************************/
#define OGS_NAS_5GS_NOTIFICATION_RESPONSE_PDU_SESSION_STATUS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_NOTIFICATION_RESPONSE_PDU_SESSION_STATUS_TYPE 0x50

typedef struct ogs_nas_5gs_notification_response_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_pdu_session_status_t pdu_session_status;
} ogs_nas_5gs_notification_response_t;


/*******************************************************
 * UL NAS TRANSPORT
 ******************************************************/
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_PDU_SESSION_ID_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_OLD_PDU_SESSION_ID_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_REQUEST_TYPE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_S_NSSAI_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_DNN_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_MA_PDU_SESSION_INFORMATION_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_RELEASE_ASSISTANCE_INDICATION_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_PDU_SESSION_ID_TYPE 0x12
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_OLD_PDU_SESSION_ID_TYPE 0x59
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_REQUEST_TYPE_TYPE 0x80
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_S_NSSAI_TYPE 0x22
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_DNN_TYPE 0x25
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE 0x24
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_MA_PDU_SESSION_INFORMATION_TYPE 0xA0
#define OGS_NAS_5GS_UL_NAS_TRANSPORT_RELEASE_ASSISTANCE_INDICATION_TYPE 0xF0

typedef struct ogs_nas_5gs_ul_nas_transport_s {
    /* Mandatory fields */
    ogs_nas_payload_container_type_t payload_container_type;
    ogs_nas_payload_container_t payload_container;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_pdu_session_identity_2_t pdu_session_id;
    ogs_nas_pdu_session_identity_2_t old_pdu_session_id;
    ogs_nas_request_type_t request_type;
    ogs_nas_s_nssai_t s_nssai;
    ogs_nas_dnn_t dnn;
    ogs_nas_additional_information_t additional_information;
    ogs_nas_ma_pdu_session_information_t ma_pdu_session_information;
    ogs_nas_release_assistance_indication_t release_assistance_indication;
} ogs_nas_5gs_ul_nas_transport_t;


/*******************************************************
 * DL NAS TRANSPORT
 ******************************************************/
#define OGS_NAS_5GS_DL_NAS_TRANSPORT_PDU_SESSION_ID_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_DL_NAS_TRANSPORT_ADDITIONAL_INFORMATION_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_DL_NAS_TRANSPORT_5GMM_CAUSE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_DL_NAS_TRANSPORT_BACK_OFF_TIMER_VALUE_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_DL_NAS_TRANSPORT_LOWER_BOUND_TIMER_VALUE_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_DL_NAS_TRANSPORT_PDU_SESSION_ID_TYPE 0x12
#define OGS_NAS_5GS_DL_NAS_TRANSPORT_ADDITIONAL_INFORMATION_TYPE 0x24
#define OGS_NAS_5GS_DL_NAS_TRANSPORT_5GMM_CAUSE_TYPE 0x58
#define OGS_NAS_5GS_DL_NAS_TRANSPORT_BACK_OFF_TIMER_VALUE_TYPE 0x37
#define OGS_NAS_5GS_DL_NAS_TRANSPORT_LOWER_BOUND_TIMER_VALUE_TYPE 0x3A

typedef struct ogs_nas_5gs_dl_nas_transport_s {
    /* Mandatory fields */
    ogs_nas_payload_container_type_t payload_container_type;
    ogs_nas_payload_container_t payload_container;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_pdu_session_identity_2_t pdu_session_id;
    ogs_nas_additional_information_t additional_information;
    ogs_nas_5gmm_cause_t gmm_cause;
    ogs_nas_gprs_timer_3_t back_off_timer_value;
    ogs_nas_gprs_timer_3_t lower_bound_timer_value;
} ogs_nas_5gs_dl_nas_transport_t;


/*******************************************************
 * PDU SESSION ESTABLISHMENT REQUEST
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_TYPE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SSC_MODE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_5GSM_CAPABILITY_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SM_PDU_DN_REQUEST_CONTAINER_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_IP_HEADER_COMPRESSION_CONFIGURATION_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_DS_TT_ETHERNET_PORT_MAC_ADDRESS_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_UE_DS_TT_RESIDENCE_TIME_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PORT_MANAGEMENT_INFORMATION_CONTAINER_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_PRESENT ((uint64_t)1<<11)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SUGGESTED_INTERFACE_IDENTIFIER_PRESENT ((uint64_t)1<<12)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SERVICE_LEVEL_AA_CONTAINER_PRESENT ((uint64_t)1<<13)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_REQUESTED_MBS_CONTAINER_PRESENT ((uint64_t)1<<14)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_PAIR_ID_PRESENT ((uint64_t)1<<15)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_RSN_PRESENT ((uint64_t)1<<16)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_TYPE_TYPE 0x90
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SSC_MODE_TYPE 0xA0
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_5GSM_CAPABILITY_TYPE 0x28
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_TYPE 0x55
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_TYPE 0xB0
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SM_PDU_DN_REQUEST_CONTAINER_TYPE 0x39
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_IP_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x66
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_DS_TT_ETHERNET_PORT_MAC_ADDRESS_TYPE 0x6E
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_UE_DS_TT_RESIDENCE_TIME_TYPE 0x6F
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE 0x74
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x1F
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SUGGESTED_INTERFACE_IDENTIFIER_TYPE 0x29
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SERVICE_LEVEL_AA_CONTAINER_TYPE 0x72
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_REQUESTED_MBS_CONTAINER_TYPE 0x70
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_PAIR_ID_TYPE 0x34
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_RSN_TYPE 0x35

typedef struct ogs_nas_5gs_pdu_session_establishment_request_s {
    /* Mandatory fields */
    ogs_nas_integrity_protection_maximum_data_rate_t integrity_protection_maximum_data_rate;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_pdu_session_type_t pdu_session_type;
    ogs_nas_ssc_mode_t ssc_mode;
    ogs_nas_5gsm_capability_t gsm_capability;
    ogs_nas_maximum_number_of_supported_packet_filters_t maximum_number_of_supported_packet_filters;
    ogs_nas_always_on_pdu_session_requested_t always_on_pdu_session_requested;
    ogs_nas_sm_pdu_dn_request_container_t sm_pdu_dn_request_container;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    ogs_nas_ip_header_compression_configuration_t ip_header_compression_configuration;
    ogs_nas_ds_tt_ethernet_port_mac_address_t ds_tt_ethernet_port_mac_address;
    ogs_nas_ue_ds_tt_residence_time_t ue_ds_tt_residence_time;
    ogs_nas_port_management_information_container_t port_management_information_container;
    ogs_nas_ethernet_header_compression_configuration_t ethernet_header_compression_configuration;
    ogs_nas_pdu_address_t suggested_interface_identifier;
    ogs_nas_service_level_aa_container_t service_level_aa_container;
    ogs_nas_requested_mbs_container_t requested_mbs_container;
    ogs_nas_pdu_session_pair_id_t pdu_session_pair_id;
    ogs_nas_rsn_t rsn;
} ogs_nas_5gs_pdu_session_establishment_request_t;


/*******************************************************
 * PDU SESSION ESTABLISHMENT ACCEPT
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_PDU_ADDRESS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RQ_TIMER_VALUE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_S_NSSAI_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ALWAYS_ON_PDU_SESSION_INDICATION_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_MAPPED_EPS_BEARER_CONTEXTS_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EAP_MESSAGE_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_DNN_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_NETWORK_FEATURE_SUPPORT_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_SERVING_PLMN_RATE_CONTROL_PRESENT ((uint64_t)1<<11)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ATSSS_CONTAINER_PRESENT ((uint64_t)1<<12)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_CONTROL_PLANE_ONLY_INDICATION_PRESENT ((uint64_t)1<<13)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_IP_HEADER_COMPRESSION_CONFIGURATION_PRESENT ((uint64_t)1<<14)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_PRESENT ((uint64_t)1<<15)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_SERVICE_LEVEL_AA_CONTAINER_PRESENT ((uint64_t)1<<16)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RECEIVED_MBS_CONTAINER_PRESENT ((uint64_t)1<<17)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_TYPE 0x59
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_PDU_ADDRESS_TYPE 0x29
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RQ_TIMER_VALUE_TYPE 0x56
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_S_NSSAI_TYPE 0x22
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ALWAYS_ON_PDU_SESSION_INDICATION_TYPE 0x80
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_MAPPED_EPS_BEARER_CONTEXTS_TYPE 0x75
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EAP_MESSAGE_TYPE 0x78
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_TYPE 0x79
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_DNN_TYPE 0x25
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_NETWORK_FEATURE_SUPPORT_TYPE 0x17
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_SERVING_PLMN_RATE_CONTROL_TYPE 0x18
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ATSSS_CONTAINER_TYPE 0x77
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_CONTROL_PLANE_ONLY_INDICATION_TYPE 0xC0
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_IP_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x66
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x1F
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_SERVICE_LEVEL_AA_CONTAINER_TYPE 0x72
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RECEIVED_MBS_CONTAINER_TYPE 0x71

typedef struct ogs_nas_5gs_pdu_session_establishment_accept_s {
    /* Mandatory fields */
    ogs_nas_pdu_session_type_t selected_pdu_session_type;
    ogs_nas_qos_rules_t authorized_qos_rules;
    ogs_nas_session_ambr_t session_ambr;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_5gsm_cause_t gsm_cause;
    ogs_nas_pdu_address_t pdu_address;
    ogs_nas_gprs_timer_t rq_timer_value;
    ogs_nas_s_nssai_t s_nssai;
    ogs_nas_always_on_pdu_session_indication_t always_on_pdu_session_indication;
    ogs_nas_mapped_eps_bearer_contexts_t mapped_eps_bearer_contexts;
    ogs_nas_eap_message_t eap_message;
    ogs_nas_qos_flow_descriptions_t authorized_qos_flow_descriptions;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    ogs_nas_dnn_t dnn;
    ogs_nas_5gsm_network_feature_support_t gsm_network_feature_support;
    ogs_nas_serving_plmn_rate_control_t serving_plmn_rate_control;
    ogs_nas_atsss_container_t atsss_container;
    ogs_nas_control_plane_only_indication_t control_plane_only_indication;
    ogs_nas_ip_header_compression_configuration_t ip_header_compression_configuration;
    ogs_nas_ethernet_header_compression_configuration_t ethernet_header_compression_configuration;
    ogs_nas_service_level_aa_container_t service_level_aa_container;
    ogs_nas_received_mbs_container_t received_mbs_container;
} ogs_nas_5gs_pdu_session_establishment_accept_t;


/*******************************************************
 * PDU SESSION ESTABLISHMENT REJECT
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_BACK_OFF_TIMER_VALUE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_RE_ATTEMPT_INDICATOR_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_SERVICE_LEVEL_AA_CONTAINER_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_BACK_OFF_TIMER_VALUE_TYPE 0x37
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_TYPE 0xF0
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_TYPE 0x78
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_TYPE 0x61
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_RE_ATTEMPT_INDICATOR_TYPE 0x1D
#define OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_SERVICE_LEVEL_AA_CONTAINER_TYPE 0x72

typedef struct ogs_nas_5gs_pdu_session_establishment_reject_s {
    /* Mandatory fields */
    ogs_nas_5gsm_cause_t gsm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_gprs_timer_3_t back_off_timer_value;
    ogs_nas_allowed_ssc_mode_t allowed_ssc_mode;
    ogs_nas_eap_message_t eap_message;
    ogs_nas_5gsm_congestion_re_attempt_indicator_t gsm_congestion_re_attempt_indicator;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    ogs_nas_re_attempt_indicator_t re_attempt_indicator;
    ogs_nas_service_level_aa_container_t service_level_aa_container;
} ogs_nas_5gs_pdu_session_establishment_reject_t;


/*******************************************************
 * PDU SESSION AUTHENTICATION COMMAND
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_5gs_pdu_session_authentication_command_s {
    /* Mandatory fields */
    ogs_nas_eap_message_t eap_message;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_5gs_pdu_session_authentication_command_t;


/*******************************************************
 * PDU SESSION AUTHENTICATION COMPLETE
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_5gs_pdu_session_authentication_complete_s {
    /* Mandatory fields */
    ogs_nas_eap_message_t eap_message;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_5gs_pdu_session_authentication_complete_t;


/*******************************************************
 * PDU SESSION AUTHENTICATION RESULT
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT_EAP_MESSAGE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT_EAP_MESSAGE_TYPE 0x78
#define OGS_NAS_5GS_PDU_SESSION_AUTHENTICATION_RESULT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_5gs_pdu_session_authentication_result_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_eap_message_t eap_message;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_5gs_pdu_session_authentication_result_t;


/*******************************************************
 * PDU SESSION MODIFICATION REQUEST
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAPABILITY_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAPPED_EPS_BEARER_CONTEXTS_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_PORT_MANAGEMENT_INFORMATION_CONTAINER_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_IP_HEADER_COMPRESSION_CONFIGURATION_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_PRESENT ((uint64_t)1<<11)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_MBS_CONTAINER_PRESENT ((uint64_t)1<<12)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_SERVICE_LEVEL_AA_CONTAINER_PRESENT ((uint64_t)1<<13)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAPABILITY_TYPE 0x28
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_TYPE 0x59
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_TYPE 0x55
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_ALWAYS_ON_PDU_SESSION_REQUESTED_TYPE 0xB0
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_TYPE 0x13
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_TYPE 0x7A
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_TYPE 0x79
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAPPED_EPS_BEARER_CONTEXTS_TYPE 0x75
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE 0x74
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_IP_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x66
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x1F
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_MBS_CONTAINER_TYPE 0x70
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_SERVICE_LEVEL_AA_CONTAINER_TYPE 0x72

typedef struct ogs_nas_5gs_pdu_session_modification_request_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_5gsm_capability_t gsm_capability;
    ogs_nas_5gsm_cause_t gsm_cause;
    ogs_nas_maximum_number_of_supported_packet_filters_t maximum_number_of_supported_packet_filters;
    ogs_nas_always_on_pdu_session_requested_t always_on_pdu_session_requested;
    ogs_nas_integrity_protection_maximum_data_rate_t integrity_protection_maximum_data_rate;
    ogs_nas_qos_rules_t requested_qos_rules;
    ogs_nas_qos_flow_descriptions_t requested_qos_flow_descriptions;
    ogs_nas_mapped_eps_bearer_contexts_t mapped_eps_bearer_contexts;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    ogs_nas_port_management_information_container_t port_management_information_container;
    ogs_nas_header_compression_configuration_t ip_header_compression_configuration;
    ogs_nas_ethernet_header_compression_configuration_t ethernet_header_compression_configuration;
    ogs_nas_requested_mbs_container_t requested_mbs_container;
    ogs_nas_service_level_aa_container_t service_level_aa_container;
} ogs_nas_5gs_pdu_session_modification_request_t;


/*******************************************************
 * PDU SESSION MODIFICATION REJECT
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_BACK_OFF_TIMER_VALUE_TYPE 0x37
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_TYPE 0x61
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_RE_ATTEMPT_INDICATOR_TYPE 0x1D

typedef struct ogs_nas_5gs_pdu_session_modification_reject_s {
    /* Mandatory fields */
    ogs_nas_5gsm_cause_t gsm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_gprs_timer_3_t back_off_timer_value;
    ogs_nas_5gsm_congestion_re_attempt_indicator_t gsm_congestion_re_attempt_indicator;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    ogs_nas_re_attempt_indicator_t re_attempt_indicator;
} ogs_nas_5gs_pdu_session_modification_reject_t;


/*******************************************************
 * PDU SESSION MODIFICATION COMMAND
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_5GSM_CAUSE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RQ_TIMER_VALUE_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ALWAYS_ON_PDU_SESSION_INDICATION_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_RULES_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT ((uint64_t)1<<6)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<7)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ATSSS_CONTAINER_PRESENT ((uint64_t)1<<8)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_IP_HEADER_COMPRESSION_CONFIGURATION_PRESENT ((uint64_t)1<<9)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_PORT_MANAGEMENT_INFORMATION_CONTAINER_PRESENT ((uint64_t)1<<10)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SERVING_PLMN_RATE_CONTROL_PRESENT ((uint64_t)1<<11)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_PRESENT ((uint64_t)1<<12)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RECEIVED_MBS_CONTAINER_PRESENT ((uint64_t)1<<13)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SERVICE_LEVEL_AA_CONTAINER_PRESENT ((uint64_t)1<<14)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_5GSM_CAUSE_TYPE 0x59
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_TYPE 0x2A
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RQ_TIMER_VALUE_TYPE 0x56
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ALWAYS_ON_PDU_SESSION_INDICATION_TYPE 0x80
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_RULES_TYPE 0x7A
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_TYPE 0x75
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_TYPE 0x79
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ATSSS_CONTAINER_TYPE 0x77
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_IP_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x66
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE 0x74
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SERVING_PLMN_RATE_CONTROL_TYPE 0x1E
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ETHERNET_HEADER_COMPRESSION_CONFIGURATION_TYPE 0x1F
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RECEIVED_MBS_CONTAINER_TYPE 0x71
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_SERVICE_LEVEL_AA_CONTAINER_TYPE 0x72

typedef struct ogs_nas_5gs_pdu_session_modification_command_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_5gsm_cause_t gsm_cause;
    ogs_nas_session_ambr_t session_ambr;
    ogs_nas_gprs_timer_t rq_timer_value;
    ogs_nas_always_on_pdu_session_indication_t always_on_pdu_session_indication;
    ogs_nas_qos_rules_t authorized_qos_rules;
    ogs_nas_mapped_eps_bearer_contexts_t mapped_eps_bearer_contexts;
    ogs_nas_qos_flow_descriptions_t authorized_qos_flow_descriptions;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    ogs_nas_atsss_container_t atsss_container;
    ogs_nas_ip_header_compression_configuration_t ip_header_compression_configuration;
    ogs_nas_port_management_information_container_t port_management_information_container;
    ogs_nas_serving_plmn_rate_control_t serving_plmn_rate_control;
    ogs_nas_ethernet_header_compression_configuration_t ethernet_header_compression_configuration;
    ogs_nas_received_mbs_container_t received_mbs_container;
    ogs_nas_service_level_aa_container_t service_level_aa_container;
} ogs_nas_5gs_pdu_session_modification_command_t;


/*******************************************************
 * PDU SESSION MODIFICATION COMPLETE
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_PORT_MANAGEMENT_INFORMATION_CONTAINER_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE 0x74

typedef struct ogs_nas_5gs_pdu_session_modification_complete_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    ogs_nas_port_management_information_container_t port_management_information_container;
} ogs_nas_5gs_pdu_session_modification_complete_t;


/*******************************************************
 * PDU SESSION MODIFICATION COMMAND REJECT
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_5gs_pdu_session_modification_command_reject_s {
    /* Mandatory fields */
    ogs_nas_5gsm_cause_t gsm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_5gs_pdu_session_modification_command_reject_t;


/*******************************************************
 * PDU SESSION RELEASE REQUEST
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_5GSM_CAUSE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_5GSM_CAUSE_TYPE 0x59
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_5gs_pdu_session_release_request_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_5gsm_cause_t gsm_cause;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_5gs_pdu_session_release_request_t;


/*******************************************************
 * PDU SESSION RELEASE REJECT
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_5gs_pdu_session_release_reject_s {
    /* Mandatory fields */
    ogs_nas_5gsm_cause_t gsm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_5gs_pdu_session_release_reject_t;


/*******************************************************
 * PDU SESSION RELEASE COMMAND
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_BACK_OFF_TIMER_VALUE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EAP_MESSAGE_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_PRESENT ((uint64_t)1<<2)
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<3)
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_ACCESS_TYPE_PRESENT ((uint64_t)1<<4)
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_SERVICE_LEVEL_AA_CONTAINER_PRESENT ((uint64_t)1<<5)
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_BACK_OFF_TIMER_VALUE_TYPE 0x37
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EAP_MESSAGE_TYPE 0x78
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_5GSM_CONGESTION_RE_ATTEMPT_INDICATOR_TYPE 0x61
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_ACCESS_TYPE_TYPE 0xD0
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_SERVICE_LEVEL_AA_CONTAINER_TYPE 0x72

typedef struct ogs_nas_5gs_pdu_session_release_command_s {
    /* Mandatory fields */
    ogs_nas_5gsm_cause_t gsm_cause;

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_gprs_timer_3_t back_off_timer_value;
    ogs_nas_eap_message_t eap_message;
    ogs_nas_5gsm_congestion_re_attempt_indicator_t gsm_congestion_re_attempt_indicator;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
    ogs_nas_access_type_t access_type;
    ogs_nas_service_level_aa_container_t service_level_aa_container;
} ogs_nas_5gs_pdu_session_release_command_t;


/*******************************************************
 * PDU SESSION RELEASE COMPLETE
 ******************************************************/
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_5GSM_CAUSE_PRESENT ((uint64_t)1<<0)
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT ((uint64_t)1<<1)
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_5GSM_CAUSE_TYPE 0x59
#define OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 0x7B

typedef struct ogs_nas_5gs_pdu_session_release_complete_s {

    /* Optional fields */
    uint64_t presencemask;
    ogs_nas_5gsm_cause_t gsm_cause;
    ogs_nas_extended_protocol_configuration_options_t extended_protocol_configuration_options;
} ogs_nas_5gs_pdu_session_release_complete_t;


/*******************************************************
 * 5GSM STATUS
 ******************************************************/

typedef struct ogs_nas_5gs_5gsm_status_s {
    /* Mandatory fields */
    ogs_nas_5gsm_cause_t gsm_cause;
} ogs_nas_5gs_5gsm_status_t;


typedef struct ogs_nas_5gmm_message_s {
    ogs_nas_5gmm_header_t h;
    union {
        ogs_nas_5gs_registration_request_t registration_request;
        ogs_nas_5gs_registration_accept_t registration_accept;
        ogs_nas_5gs_registration_complete_t registration_complete;
        ogs_nas_5gs_registration_reject_t registration_reject;
        ogs_nas_5gs_deregistration_request_from_ue_t deregistration_request_from_ue;
        ogs_nas_5gs_deregistration_request_to_ue_t deregistration_request_to_ue;
        ogs_nas_5gs_service_request_t service_request;
        ogs_nas_5gs_service_reject_t service_reject;
        ogs_nas_5gs_service_accept_t service_accept;
        ogs_nas_5gs_configuration_update_command_t configuration_update_command;
        ogs_nas_5gs_authentication_request_t authentication_request;
        ogs_nas_5gs_authentication_response_t authentication_response;
        ogs_nas_5gs_authentication_reject_t authentication_reject;
        ogs_nas_5gs_authentication_failure_t authentication_failure;
        ogs_nas_5gs_authentication_result_t authentication_result;
        ogs_nas_5gs_identity_request_t identity_request;
        ogs_nas_5gs_identity_response_t identity_response;
        ogs_nas_5gs_security_mode_command_t security_mode_command;
        ogs_nas_5gs_security_mode_complete_t security_mode_complete;
        ogs_nas_5gs_security_mode_reject_t security_mode_reject;
        ogs_nas_5gs_5gmm_status_t gmm_status;
        ogs_nas_5gs_notification_t notification;
        ogs_nas_5gs_notification_response_t notification_response;
        ogs_nas_5gs_ul_nas_transport_t ul_nas_transport;
        ogs_nas_5gs_dl_nas_transport_t dl_nas_transport;
    };
} ogs_nas_5gmm_message_t;

typedef struct ogs_nas_5gsm_message_s {
    ogs_nas_5gsm_header_t h;
    union {
        ogs_nas_5gs_pdu_session_establishment_request_t pdu_session_establishment_request;
        ogs_nas_5gs_pdu_session_establishment_accept_t pdu_session_establishment_accept;
        ogs_nas_5gs_pdu_session_establishment_reject_t pdu_session_establishment_reject;
        ogs_nas_5gs_pdu_session_authentication_command_t pdu_session_authentication_command;
        ogs_nas_5gs_pdu_session_authentication_complete_t pdu_session_authentication_complete;
        ogs_nas_5gs_pdu_session_authentication_result_t pdu_session_authentication_result;
        ogs_nas_5gs_pdu_session_modification_request_t pdu_session_modification_request;
        ogs_nas_5gs_pdu_session_modification_reject_t pdu_session_modification_reject;
        ogs_nas_5gs_pdu_session_modification_command_t pdu_session_modification_command;
        ogs_nas_5gs_pdu_session_modification_complete_t pdu_session_modification_complete;
        ogs_nas_5gs_pdu_session_modification_command_reject_t pdu_session_modification_command_reject;
        ogs_nas_5gs_pdu_session_release_request_t pdu_session_release_request;
        ogs_nas_5gs_pdu_session_release_reject_t pdu_session_release_reject;
        ogs_nas_5gs_pdu_session_release_command_t pdu_session_release_command;
        ogs_nas_5gs_pdu_session_release_complete_t pdu_session_release_complete;
        ogs_nas_5gs_5gsm_status_t gsm_status;
    };
} ogs_nas_5gsm_message_t;

typedef struct ogs_nas_5gs_message_s {
    ogs_nas_5gs_security_header_t h;
    union {
        ogs_nas_5gmm_message_t gmm;
        ogs_nas_5gsm_message_t gsm;
    };
} ogs_nas_5gs_message_t;

ogs_pkbuf_t *ogs_nas_5gmm_encode(ogs_nas_5gs_message_t *message);
ogs_pkbuf_t *ogs_nas_5gsm_encode(ogs_nas_5gs_message_t *message);
int ogs_nas_5gmm_decode(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gsm_decode(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf);
ogs_pkbuf_t *ogs_nas_5gs_plain_encode(ogs_nas_5gs_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* OGS_NAS_5GS_MESSAGE_H */
