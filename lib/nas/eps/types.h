/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined(OGS_NAS_INSIDE) && !defined(OGS_NAS_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_NAS_EPS_TYPES_H
#define OGS_NAS_EPS_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/**********************
 * NAS GUTI Structure */
typedef struct ogs_nas_eps_guti_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    uint16_t mme_gid;
    uint8_t mme_code;
    uint32_t m_tmsi;
} __attribute__ ((packed)) ogs_nas_eps_guti_t;

/* 9.9.2.0A Device properties
 * See subclause 10.5.7.8 in 3GPP TS 24.008 [13].
 * O TV 1 */
typedef struct ogs_nas_device_properties_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t low_priority:1;)
} __attribute__ ((packed)) ogs_nas_device_properties_t;

/* 9.9.2.2 Location area identification
 * See subclause 10.5.1.3 in 3GPP TS 24.008 [13]
 * O TV 6 */
typedef struct ogs_nas_location_area_identification_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    uint16_t lac;
} __attribute__ ((packed)) ogs_nas_location_area_identification_t;

typedef ogs_nas_location_area_identification_t ogs_nas_lai_t;

/* 3GPP TS 24.008 10.5.5.15 Routing area identification */
typedef struct ogs_nas_routing_area_identification_s {
    ogs_nas_location_area_identification_t lai;
    uint8_t rac;
} __attribute__ ((packed)) ogs_nas_routing_area_identification_t;

typedef ogs_nas_routing_area_identification_t ogs_nas_rai_t;

/* 9.9.2.3 Mobile identity
 * See subclause 10.5.1.4 in 3GPP TS 24.008 [13].
 * O TLV 7-10 */
#define OGS_NAS_MOBILE_IDENTITY_NONE 0
#define OGS_NAS_MOBILE_IDENTITY_IMSI 1
#define OGS_NAS_MOBILE_IDENTITY_IMEI 2
#define OGS_NAS_MOBILE_IDENTITY_IMEISV 3
#define OGS_NAS_MOBILE_IDENTITY_TMSI 4
#define OGS_NAS_MOBILE_IDENTITY_TMGI 5
#define OGS_NAS_MOBILE_IDENTITY_GUTI 6

#define OGS_NAS_MOBILE_IDENTITY_EVEN 0
#define OGS_NAS_MOBILE_IDENTITY_ODD 1
typedef struct ogs_nas_mobile_identity_imsi_s {
ED3(uint8_t digit1:4;,
    uint8_t odd_even:1;,
    uint8_t type:3;)
ED2(uint8_t digit3:4;,
    uint8_t digit2:4;)
ED2(uint8_t digit5:4;,
    uint8_t digit4:4;)
ED2(uint8_t digit7:4;,
    uint8_t digit6:4;)
ED2(uint8_t digit9:4;,
    uint8_t digit8:4;)
ED2(uint8_t digit11:4;,
    uint8_t digit10:4;)
ED2(uint8_t digit13:4;,
    uint8_t digit12:4;)
ED2(uint8_t digit15:4;,
    uint8_t digit14:4;)
} __attribute__ ((packed)) ogs_nas_mobile_identity_imsi_t;

typedef ogs_nas_mobile_identity_imsi_t ogs_nas_mobile_identity_imei_t;

typedef struct ogs_nas_mobile_identity_tmsi_s {
ED3(uint8_t spare:4;,
    uint8_t odd_even:1;,
    uint8_t type:3;)
    uint32_t tmsi;
} __attribute__ ((packed)) ogs_nas_mobile_identity_tmsi_t;

typedef struct ogs_nas_mobile_identity_tmgi_s {
ED5(uint8_t spare:2;,
    uint8_t mbms_session_id:1;,
    uint8_t mcc_mnc:1;,
    uint8_t odd_even:1;,
    uint8_t type:3;)
    uint8_t mbms_servicec_id[3];
    ogs_nas_plmn_id_t nas_plmn_id;
    uint8_t mbms_session_identity;
} __attribute__ ((packed)) ogs_nas_mobile_identity_tmgi_t;

typedef struct ogs_nas_mobile_identity_s {
    uint8_t length;
    union {
        ogs_nas_mobile_identity_imsi_t imsi;
        ogs_nas_mobile_identity_tmsi_t tmsi;
        ogs_nas_mobile_identity_tmgi_t tmgi;
        ogs_nas_mobile_identity_imeisv_t imeisv;
    };
} ogs_nas_mobile_identity_t;

/*9.9.2.5 Mobile station classmark 3
 * See subclause 10.5.1.7 in 3GPP TS 24.008 [13].
 * O TLV 2-34 */
#define OGS_NAS_MAX_MOBILE_STATION_CLASSMARK_3_LEN 33
typedef struct ogs_nas_mobile_station_classmark_3_s {
    uint8_t length;
    uint8_t buffer[OGS_NAS_MAX_MOBILE_STATION_CLASSMARK_3_LEN];
} __attribute__ ((packed)) ogs_nas_mobile_station_classmark_3_t;

/* 9.9.3.0A Additional update result
 * O TV 1  */
typedef struct ogs_nas_additional_update_result_s {
ED3(uint8_t type:4;,
    uint8_t spare:2;,
    uint8_t additional_update_result_value:2;)
} __attribute__ ((packed)) ogs_nas_additional_update_result_t;

/* 9.9.3.0B Additional update type
 * O TV 1 */
#define OGS_NAS_ADDITIONAL_UPDATE_TYPE_CIOT_NONE 0
#define OGS_NAS_ADDITIONAL_UPDATE_TYPE_CIOT_CONTROL_PLANE 1
#define OGS_NAS_ADDITIONAL_UPDATE_TYPE_CIOT_DATA_PLANE 2
#define OGS_NAS_ADDITIONAL_UPDATE_TYPE_CIOT_RESERVED 3
typedef struct ogs_nas_additional_update_type_s {
ED4(uint8_t type:4;,
    uint8_t preferred_ciot_network_behaviour:2;,
    uint8_t signalling_active_flag:1;,
    uint8_t additional_update_type_value:1;)
} __attribute__ ((packed)) ogs_nas_additional_update_type_t;

/* 9.9.3.4a Ciphering key sequence number
 * See subclause 10.5.1.2 in 3GPP TS 24.008 [13].
 * O TV 1 */
#define OGS_NAS_CIPHERING_KEY_SEQUENCE_NUMBER_NO_KEY_FROM_MS 7
typedef struct ogs_nas_ciphering_key_sequence_number_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t key_sequence:3;)
} __attribute__ ((packed)) ogs_nas_ciphering_key_sequence_number_t;

/* 9.9.3.4B SMS services status
 * O TV 1 */
#define OGS_NAS_SMS_SERVICES_STATUS_NOT_AVAILABLE 0
#define OGS_NAS_SMS_SERVICES_STATUS_NOT_AVAILABLE_IN_THIS_PLMN 1
#define OGS_NAS_SMS_SERVICES_STATUS_NETWORK_FAILURE 2
#define OGS_NAS_SMS_SERVICES_STATUS_CONGESTION 3
typedef struct ogs_nas_sms_services_status_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t value:3;)
} __attribute__ ((packed)) ogs_nas_sms_services_status_t;

/* 9.9.3.5 CSFB response
 * O TV 1 */
#define OGS_NAS_CS_FALLBACK_RESPONSE_REJECTED_BY_THE_UE     0
#define OGS_NAS_CS_FALLBACK_RESPONSE_ACCEPTED_BY_THE_UE     1
typedef struct ogs_nas_csfb_response_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t response:3;)
} __attribute__ ((packed)) ogs_nas_csfb_response_t;

/* 9.9.3.7 Detach type
 * M V 1/2
 * 9.9.3.21 NAS key set identifier
 * M V 1/2 */
#define OGS_NAS_DETACH_TYPE_FROM_UE_EPS_DETACH                  1
#define OGS_NAS_DETACH_TYPE_FROM_UE_IMSI_DETACH                 2
#define OGS_NAS_DETACH_TYPE_FROM_UE_COMBINED_EPS_IMSI_DETACH    3
#define OGS_NAS_DETACH_TYPE_TO_UE_RE_ATTACH_REQUIRED            1
#define OGS_NAS_DETACH_TYPE_TO_UE_RE_ATTACH_NOT_REQUIRED        2
#define OGS_NAS_DETACH_TYPE_TO_UE_IMSI_DETACH                   3
typedef struct ogs_nas_detach_type_s {
ED4(uint8_t tsc:1;,
    uint8_t nas_key_set_identifier:3;,
    uint8_t switch_off:1;,
    uint8_t value:3;)
} __attribute__ ((packed)) ogs_nas_detach_type_t;

/* 9.9.3.8 DRX parameter
 * See subclause 10.5.5.6 in 3GPP TS 24.008
 * O TV 3 */
#define OGS_NAS_DRX_PARAMETER_TIMER_MAX_1SEC 1
#define OGS_NAS_DRX_PARAMETER_TIMER_MAX_2SEC 2
#define OGS_NAS_DRX_PARAMETER_TIMER_MAX_4SEC 3
#define OGS_NAS_DRX_PARAMETER_TIMER_MAX_8SEC 4
#define OGS_NAS_DRX_PARAMETER_TIMER_MAX_16SEC 5
#define OGS_NAS_DRX_PARAMETER_TIMER_MAX_32SEC 6
#define OGS_NAS_DRX_PARAMETER_TIMER_MAX_64SEC 7

#define OGS_NAS_DRX_PARAMETER_COEFFICIENT_T_NOT_SPECIFIED_BY_MS 0
#define OGS_NAS_DRX_PARAMETER_COEFFICIENT_6_T_32 6
#define OGS_NAS_DRX_PARAMETER_COEFFICIENT_7_T_64 7
#define OGS_NAS_DRX_PARAMETER_COEFFICIENT_8_T_128 8
#define OGS_NAS_DRX_PARAMETER_COEFFICIENT_9_T_256 9
typedef struct ogs_nas_drx_parameter_s {
    uint8_t split_pg_cycle_code;
ED3(uint8_t cn_specific_drx_cycle_length_coefficient_and_drx_value_for_s1_mode:4;,
    uint8_t split_on_ccch:1;,
    uint8_t non_DRX_timer:3;)
} __attribute__ ((packed)) ogs_nas_drx_parameter_t;

/* 9.9.3.9 EMM cause
 * O TV 2 */

/* REQUEST_ACCEPTED(16) cause is defined by Open5GS */
#define OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED 16

/* Annex A (informative) Cause values for EPS mobility management
 * A.1 Causes related to UE identification */
#define OGS_NAS_EMM_CAUSE_IMSI_UNKNOWN_IN_HSS 2
#define OGS_NAS_EMM_CAUSE_ILLEGAL_UE 3
#define OGS_NAS_EMM_CAUSE_IMSI_UNKNOWN_IN_VLR 4
#define OGS_NAS_EMM_CAUSE_ILLEGAL_ME 6
#define OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK 9
#define OGS_NAS_EMM_CAUSE_IMPLICITLY_DETACHED 10
/* A.2 Cause related to subscription options */
#define OGS_NAS_EMM_CAUSE_IMEI_NOT_ACCEPTED 5
#define OGS_NAS_EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED 7
#define OGS_NAS_EMM_CAUSE_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED 8
#define OGS_NAS_EMM_CAUSE_PLMN_NOT_ALLOWED 11
#define OGS_NAS_EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED 12
#define OGS_NAS_EMM_CAUSE_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA 13
#define OGS_NAS_EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED_IN_THIS_PLMN 14
#define OGS_NAS_EMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA 15
#define OGS_NAS_EMM_CAUSE_REQUESTED_SERVICE_OPTION_NOT_AUTHORIZED_IN_THIS_PLMN 35
#define OGS_NAS_EMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED 40
/* A.3 Causes related to PLMN specific network failures and
 *     congestion/authentication failures */
#define OGS_NAS_EMM_CAUSE_MSC_TEMPORARILY_NOT_REACHABLE 16
#define OGS_NAS_EMM_CAUSE_NETWORK_FAILURE 17
#define OGS_NAS_EMM_CAUSE_CS_DOMAIN_NOT_AVAILABLE 18
#define OGS_NAS_EMM_CAUSE_ESM_FAILURE 19
#define OGS_NAS_EMM_CAUSE_MAC_FAILURE 20
#define OGS_NAS_EMM_CAUSE_SYNCH_FAILURE 21
#define OGS_NAS_EMM_CAUSE_CONGESTION 22
#define OGS_NAS_EMM_CAUSE_UE_SECURITY_CAPABILITIES_MISMATCH 23
#define OGS_NAS_EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED 24
#define OGS_NAS_EMM_CAUSE_NON_EPS_AUTHENTICATION_UNACCEPTABLE 26
#define OGS_NAS_EMM_CAUSE_CS_SERVICE_TEMPORARILY_NOT_AVAILABLE 39
#define OGS_NAS_EMM_CAUSE_SEVERE_NETWORK_FAILURE 42
/* A.4 Causes related to nature of request
 * NOTE:   This subclause has no entries in this version of the specification *
 * A.5 Causes related to invalid messages */
#define OGS_NAS_EMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE 95
#define OGS_NAS_EMM_CAUSE_INVALID_MANDATORY_INFORMATION 96
#define OGS_NAS_EMM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED 97
#define OGS_NAS_EMM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE 98
#define OGS_NAS_EMM_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED 99
#define OGS_NAS_EMM_CAUSE_CONDITIONAL_IE_ERROR 100
#define OGS_NAS_EMM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE 101
#define OGS_NAS_EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED 111
typedef uint8_t ogs_nas_emm_cause_t;

/* 9.9.3.10 * EPS attach result
 * M V 1/2 */
#define OGS_NAS_ATTACH_RESULT_EPS_ONLY 1
#define OGS_NAS_ATTACH_RESULT_COMBINED_EPS_IMSI_ATTACH 2
typedef struct ogs_nas_eps_attach_result_s {
ED2(uint8_t spare:5;,
    uint8_t result:3;)
} __attribute__ ((packed)) ogs_nas_eps_attach_result_t;

/* 9.9.3.11 EPS attach type
 * M V 1/2
 * 9.9.3.21 NAS key set identifier
 * M V 1/2 */
#define OGS_NAS_KEY_SET_IDENTIFIER_NATIVE 0
#define OGS_NAS_KEY_SET_IDENTIFIER_MAPPED 1
#define OGS_NAS_KEY_SET_IDENTIFIER_NOT_AVAILABLE 0x111

#define OGS_NAS_ATTACH_TYPE_EPS_ATTACH 1
#define OGS_NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTACH 2
#define OGS_NAS_ATTACH_TYPE_EPS_EMERGENCY_ATTACH 3
#define OGS_NAS_ATTACH_TYPE_RESERVED 4
typedef struct ogs_nas_eps_attach_type_s {
ED4(uint8_t tsc:1;,
    uint8_t nas_key_set_identifier:3;,
    uint8_t spare:1;,
    uint8_t value:3;)
} __attribute__ ((packed)) ogs_nas_eps_attach_type_t;

/* 9.9.3.12 EPS mobile identity
 * M LV  5-12 */
#define OGS_NAS_EPS_MOBILE_IDENTITY_IMSI 1
#define OGS_NAS_EPS_MOBILE_IDENTITY_GUTI 6
#define OGS_NAS_EPS_MOBILE_IDENTITY_IMEI 3
typedef struct ogs_nas_eps_mobile_identity_guti_s {
ED3(uint8_t spare:4;,
    uint8_t odd_even:1;,
    uint8_t type:3;)
    ogs_nas_plmn_id_t nas_plmn_id;
    uint16_t mme_gid;
    uint8_t mme_code;
    uint32_t m_tmsi;
} __attribute__ ((packed)) ogs_nas_eps_mobile_identity_guti_t;
typedef struct ogs_nas_eps_mobile_identity_s {
    uint8_t length;
    union {
        ogs_nas_mobile_identity_imsi_t imsi;
        ogs_nas_eps_mobile_identity_guti_t guti;
        ogs_nas_mobile_identity_imei_t imei;
    };
} __attribute__ ((packed)) ogs_nas_eps_mobile_identity_t;

/* 9.9.3.12A EPS network feature support
 * O TLV 3 */
typedef struct ogs_nas_eps_network_feature_support_s {
    uint8_t length;
ED7(uint8_t control_plane_ciot_eps_optimization:1;,
    uint8_t emm_registered_without_pdn_connectivity:1;,
    uint8_t support_of_extended_service_request_for_packet_services:1;,
    uint8_t cs_lcs_no_information_about_support_of_location_service_via_cs_domain_is_available:2;,
    uint8_t location_service_via_epc:1;,
    uint8_t emergency_bearer_services_in_s1_mode:1;,
    uint8_t ims_voice_over_ps_session_in_s1_mode:1;)
ED8(uint8_t signalling_for_a_maximum_number_of_16_eps_bearer_contexts:1;,
    uint8_t interworking_without_n26_interface:1;,
    uint8_t restriction_on_the_use_of_dual_connectivity_with_nr:1;,
    uint8_t restriction_on_enhanced_coverage:1;,
    uint8_t extended_protocol_configuration_options:1;,
    uint8_t header_compression_for_control_plan_ciot_eps_optimization:1;,
    uint8_t s1_u_data_transfer:1;,
    uint8_t user_plane_ciot_eps_optimization :1;)
ED6(uint8_t spare:3;,
    uint8_t paging_timing_collision_control:1;,
    uint8_t paging_restriction:1;,
    uint8_t reject_paging_request:1;,
    uint8_t paging_indication_for_voice_services:1;,
    uint8_t nas_signalling_connection_release:1;)
} __attribute__ ((packed)) ogs_nas_eps_network_feature_support_t;

/* 9.9.3.13 EPS update result
 * M V 1/2 */
#define OGS_NAS_EPS_UPDATE_RESULT_TA_UPDATED                                0
#define OGS_NAS_EPS_UPDATE_RESULT_COMBINED_TA_LA_UPDATED                    1
#define OGS_NAS_EPS_UPDATE_RESULT_TA_UPDATED_AND_ISR_ACTIVATED              4
#define OGS_NAS_EPS_UPDATE_RESULT_COMBINED_TA_LA_UPDATED_AND_ISR_ACTIVATED  5
typedef struct ogs_nas_eps_update_result_s {
ED2(uint8_t spare:5;,
    uint8_t result:3;)
} __attribute__ ((packed)) ogs_nas_eps_update_result_t;

/* 9.9.3.14 EPS update type
 * M V 1/2
 * 9.9.3.21 NAS key set identifier
 * M V 1/2 */
#define OGS_NAS_EPS_UPDATE_TYPE_TA_UPDATING                                 0
#define OGS_NAS_EPS_UPDATE_TYPE_COMBINED_TA_LA_UPDATING                     1
#define OGS_NAS_EPS_UPDATE_TYPE_COMBINED_TA_LA_UPDATING_WITH_IMSI_ATTACH    2
#define OGS_NAS_EPS_UPDATE_TYPE_PERIODIC_UPDATING                           3
typedef struct ogs_nas_eps_update_type_s {
ED4(uint8_t tsc:1;,
    uint8_t nas_key_set_identifier:3;,
    uint8_t active_flag:1;,
    uint8_t value:3;)
} __attribute__ ((packed)) ogs_nas_eps_update_type_t;

/* 9.9.3.15 ESM message container
 * M LV-E 5-n */
typedef struct ogs_nas_esm_message_container_s {
    uint16_t length;
    void *buffer;
} ogs_nas_esm_message_container_t;

/* 9.9.3.17 Identity type 2
 * See subclause 10.5.5.9 in 3GPP TS 24.008 [13].
 * M V 1/2 */
#define OGS_NAS_IDENTITY_TYPE_2_IMSI            1
#define OGS_NAS_IDENTITY_TYPE_2_IMEI            2
#define OGS_NAS_IDENTITY_TYPE_2_IMEISV          3
#define OGS_NAS_IDENTITY_TYPE_2_TMSI            4
typedef struct ogs_nas_identity_type_2_s {
ED2(uint8_t spare:5;,
    uint8_t type:3;)
} __attribute__ ((packed)) ogs_nas_identity_type_2_t;

/* 9.9.3.19 KSI and sequence number
 * M V 1 */
typedef struct ogs_nas_ksi_and_sequence_number_s {
ED2(uint8_t ksi:3;,
    uint8_t sequence_number:5;)
} __attribute__ ((packed)) ogs_nas_ksi_and_sequence_number_t;

/* 9.9.3.20 MS network capability
 * See subclause 10.5.5.12 in 3GPP TS 24.008
 * O TLV 4-10 */
#define OGS_NAS_MS_NETWORK_CAPABILITY_GEA2 0b01000000
#define OGS_NAS_MS_NETWORK_CAPABILITY_GEA3 0b00100000
#define OGS_NAS_MS_NETWORK_CAPABILITY_GEA4 0b00010000
#define OGS_NAS_MS_NETWORK_CAPABILITY_GEA5 0b00001000
#define OGS_NAS_MS_NETWORK_CAPABILITY_GEA6 0b00000100
#define OGS_NAS_MS_NETWORK_CAPABILITY_GEA7 0b00000010
typedef struct ogs_nas_ms_network_capability_s {
    uint8_t length;
ED7(uint8_t gea1:1;,
    uint8_t sm_capabilities_via_dedicated_channels:1;,
    uint8_t sm_capabilities_via_gprs_channels:1;,
    uint8_t ucs2:1;,
    uint8_t ss_screening_indicator:2;,
    uint8_t solsa_capability:1;,
    uint8_t revision_level_indicator:1;)
ED3(uint8_t pfc_feature_mode:1;,
    uint8_t extended_gea:6;,
    uint8_t lcs_va_capability:1;)
ED8(uint8_t ps_inter_rat_ho_from_geran_to_utran_iu_mode_capability:1;,
    uint8_t ps_inter_rat_ho_from_geran_to_e_utran_s1_mode_capability:1;,
    uint8_t emm_combined_procedures_capability:1;,
    uint8_t isr_support:1;,
    uint8_t srvcc_to_geran_utran_capability:1;,
    uint8_t epc_capability:1;,
    uint8_t nf_capability:1;,
    uint8_t geran_network_sharing_capability:1;)
ED8(uint8_t user_plane_integrity_protection_support:1;,
    uint8_t gia4:1;,
    uint8_t gia5:1;,
    uint8_t gia6:1;,
    uint8_t gia7:1;,
    uint8_t epco_ie_indicator:1;,
    uint8_t restriction_on_usex_of_enhanced_coverage_capability:1;,
    uint8_t dual_connectivity_of_e_utra_with_nr_capability:1;)
    uint8_t spare2[4];
} __attribute__ ((packed)) ogs_nas_ms_network_capability_t;

/* 9.9.3.20A MS network feature support
 * See subclause 10.5.1.15 in 3GPP TS 24.008 [13].
 * O TV 1 */
typedef struct ogs_nas_ms_network_feature_support_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t extended_periodic_timers:1;)
} __attribute__ ((packed)) ogs_nas_ms_network_feature_support_t;

/* 9.9.3.22 message container
 * M LV 3-252 */
#define OGS_NAS_MAX_MESSAGE_CONTAINER_LEN 250
typedef struct ogs_nas_eps_message_container_s {
    uint8_t length;
    uint8_t buffer[OGS_NAS_MAX_MESSAGE_CONTAINER_LEN];
} __attribute__ ((packed)) ogs_nas_eps_message_container_t;

/* 9.9.3.24A Network resource identifier container
 * See subclause 10.5.5.31 in 3GPP TS 24.008 [13].
 * O TLV 4 */
typedef struct ogs_nas_network_resource_identifier_container_s {
    uint8_t length;
    uint8_t nri_container_value1;
ED2(uint8_t nri_container_value2:2;,
    uint8_t spare:6;)
} __attribute__ ((packed)) ogs_nas_network_resource_identifier_container_t;

/* 9.9.3.25 Nonce
 * O TV 5 */
typedef uint32_t ogs_nas_nonce_t;

/* 9.9.3.25A Paging identity
 * M V 1 */
#define OGS_NAS_PAGING_IDENTITY_IMSI        0
#define OGS_NAS_PAGING_IDENTITY_TMSI        1
typedef struct ogs_nas_paging_identity_s {
ED2(uint8_t spare:7;,
    uint8_t identity:1;)
} ogs_nas_paging_identity_t;

/* 9.9.3.26 P-TMSI signature
 * See subclause 10.5.5.8 in 3GPP TS 24.008
 * O TV 4 */
typedef uint32_t ogs_nas_p_tmsi_signature_t; /* TV : 4bytes */

/* 9.9.3.26A Extended EMM cause
 * O TV 1 */
typedef struct ogs_nas_extended_emm_cause_s {
ED4(uint8_t type:4;,
    uint8_t spare:2;,
    uint8_t eps_optimization_info:1;,
    uint8_t e_utran_allowed:1;)
} ogs_nas_extended_emm_cause_t;

/* 9.9.3.27 Service type
 * M V 1/2
 * 9.9.3.21 NAS key set identifier
 * M V 1/2 */
#define OGS_NAS_SERVICE_TYPE_CS_FALLBACK_FROM_UE 0
#define OGS_NAS_SERVICE_TYPE_CS_FALLBACK_TO_UE 1
#define OGS_NAS_SERVICE_TYPE_CS_FALLBACK_EMERGENCY_CALL_FROM_UE 2

/* 9.9.3.28 Short MAC
 * M V 2 */
typedef uint16_t ogs_nas_short_mac_t;

/* 9.9.3.31 TMSI status
 * See subclause 10.5.5.4 in 3GPP TS 24.008 [13]
 * O TV 1 */
typedef struct ogs_nas_tmsi_status_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t tmsi_flag:1;)
} __attribute__ ((packed)) ogs_nas_tmsi_status_t;

/* 9.9.3.32 Tracking area identity
 * O TV 6 */
typedef struct ogs_nas_tracking_area_identity_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    uint16_t tac;
} __attribute__ ((packed)) ogs_nas_tracking_area_identity_t;

typedef ogs_nas_tracking_area_identity_t ogs_nas_eps_tai_t;

/* 9.9.3.33 Tracking area identity list
 * M LV 7-97 */
#define OGS_NAS_EPS_MAX_TAI_LIST_LEN    96
typedef struct ogs_eps_tai0_list_s {
    struct {
    ED3(uint8_t spare:1;,
        uint8_t type:2;,
        uint8_t num:5;)
        /*
         * Do not change 'ogs_plmn_id_t' to 'ogs_nas_plmn_id_t'.
         * Use 'ogs_plmn_id_t' for easy implementation.
         * ogs_nas_tai_list_build() changes to NAS format(ogs_nas_plmn_id_t)
         * and is sent to the UE.
         */
        ogs_plmn_id_t plmn_id;
        uint16_t tac[OGS_MAX_NUM_OF_TAI];
    } __attribute__ ((packed)) tai[OGS_MAX_NUM_OF_TAI];
} __attribute__ ((packed)) ogs_eps_tai0_list_t;

typedef struct ogs_eps_tai1_list_s {
    struct {
    ED3(uint8_t spare:1;,
        uint8_t type:2;,
        uint8_t num:5;)
        /*
         * Do not change 'ogs_plmn_id_t' to 'ogs_nas_plmn_id_t'.
         * Use 'ogs_plmn_id_t' for easy implementation.
         * ogs_nas_tai_list_build() changes to NAS format(ogs_nas_plmn_id_t)
         * and is sent to the UE.
         */
        ogs_plmn_id_t plmn_id;
        uint16_t tac;
    } __attribute__ ((packed)) tai[OGS_MAX_NUM_OF_TAI];
} __attribute__ ((packed)) ogs_eps_tai1_list_t;

typedef struct ogs_eps_tai2_list_s {
ED3(uint8_t spare:1;,
    uint8_t type:2;,
    uint8_t num:5;)
    /*
     * Do not change 'ogs_eps_tai_t' to 'ogs_nas_tracking_area_identity_t'.
     * Use 'ogs_eps_tai_t' for easy implementation.
     * ogs_nas_tai_list_build() changes to NAS
     * format(ogs_nas_tracking_area_identity_t)
     * and is sent to the UE.
     */
    ogs_eps_tai_t tai[OGS_MAX_NUM_OF_TAI];
} __attribute__ ((packed)) ogs_eps_tai2_list_t;

typedef struct ogs_nas_tracking_area_identity_list_s {
    uint8_t length;
    uint8_t buffer[OGS_NAS_EPS_MAX_TAI_LIST_LEN];
} __attribute__ ((packed)) ogs_nas_tracking_area_identity_list_t;

int ogs_nas_tai_list_build(
        ogs_nas_tracking_area_identity_list_t *target,
        ogs_eps_tai0_list_t *source0,
        ogs_eps_tai1_list_t *source1,
        ogs_eps_tai2_list_t *source2);


/* 9.9.3.35 UE radio capability information update needed
 * O TV 1 */
typedef struct ogs_nas_ue_radio_capability_information_update_needed_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t update_needed:1;)
} __attribute__ ((packed)) ogs_nas_ue_radio_capability_information_update_needed_t;

/* 9.9.3.38 CLI
 * O TLV 3-14
 * The coding of the CLI value part is the same as for
 * octets 3 to 14 of the Calling party BCD number information element
 * defined in subclause 10.5.4.9 of 3GPP TS 24.008 [13]. */
#define NAX_MAX_CLI_LEN 12
typedef struct ogs_nas_cli_s {
    uint8_t length;
    uint8_t buffer[NAX_MAX_CLI_LEN];
} __attribute__ ((packed)) ogs_nas_cli_t;

/* 9.9.3.39 SS Code
 * O TV 2
 * The coding of the SS Code value is given in subclause 17.7.5 of
 * 3GPP TS 29.002 [15C] */
typedef uint8_t ogs_nas_ss_code_t;

/* 9.9.3.40 LCS indicator
 * O TV 2 */
#define OGS_NAS_LCS_INDICATOR_MT_LR     1
typedef uint8_t ogs_nas_lcs_indicator_t;

/* 9.9.3.41 LCS client identity
 * O TLV 3-257 */
#define OGS_NAS_MAX_LCS_CLIENT_IDENTITY_LEN 255
typedef struct ogs_nas_lcs_client_identity_s {
    uint8_t length;
    uint8_t buffer[OGS_NAS_MAX_LCS_CLIENT_IDENTITY_LEN];
} __attribute__ ((packed)) ogs_nas_lcs_client_identity_t;

/* 9.9.3.42 Generic message container type
 * M V 1 */
#define OGS_NAS_GENERIC_MESSAGE_CONTAINER_TYPE_LTE_POSITIONING_PROTOCOL         1
#define OGS_NAS_GENERIC_MESSAGE_CONTAINER_TYPE_LTE_LOCATION_SERVICES_MESSAGE    2
typedef uint8_t ogs_nas_generic_message_container_type_t;

/* 9.9.3.43 Generic message container
 * M LV-E 3-n */
typedef struct ogs_nas_generic_message_container_s {
    uint16_t length;
    void *buffer;
} ogs_nas_generic_message_container_t;

/* 9.9.3.44 Voice domain preference and UE's usage setting
 * See subclause 10.5.5.28 in 3GPP TS 24.008 [13].
 * O TLV 3 */
typedef struct ogs_nas_voice_domain_preference_and_ue_usage_setting_s {
    uint8_t length;
ED3(uint8_t spare:5;,
    uint8_t ue_usage_setting:1;,
    uint8_t voice_domain_preference_for_e_utran:2;)
} __attribute__ ((packed)) ogs_nas_voice_domain_preference_and_ue_usage_setting_t;

/* 9.9.3.45 GUTI type
 * O TV 1 */
typedef struct ogs_nas_guti_type_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t guti_type:1;)
} __attribute__ ((packed)) ogs_nas_guti_type_t;

/* 9.9.3.48 DCN-ID
 * See subclause 10.5.5.35 in 3GPP TS 24.008.
 * O TLV 4
 *
 * This field contains the 16 bit DCN-ID.
 * The coding of the DCN-ID value part is defined in 3GPP TS 23.003 [2]
 */
typedef struct ogs_nas_dcn_id_s {
    uint8_t length;
    uint16_t value;
} ogs_nas_dcn_id_t;

/* 9.9.3.49 Non-3GPP NW provided policies
 * See subclause 10.5.5.37 in 3GPP TS 24.008.
 * O TV 1 */
typedef struct ogs_nas_non__nw_provided_policies_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t use_of_non_3gpp_emergency_numbers_permitted:1;)
} __attribute__ ((packed)) ogs_nas_non__nw_provided_policies_t;

/* 9.9.3.50 HashMME
 * O TLV 10 */
typedef struct ogs_nas_hashmme_s {
    uint8_t length;
    uint8_t value[OGS_HASH_MME_LEN];
} ogs_nas_hashmme_t;

/* 9.9.3.51 Replayed NAS message container
 * O TLV-E 3-n */
typedef struct ogs_nas_replayed_nas_message_container_s {
    uint16_t length;
    void *buffer;
} ogs_nas_replayed_nas_message_container_t;

/* 9.9.3.52 Network policy
 * O TV 1 */
typedef struct ogs_nas_network_policy_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t unsecured_redirection_to_geran_not_allowed:1;)
} __attribute__ ((packed)) ogs_nas_network_policy_t;

/* 9.9.3.53 UE additional security capability
 * O TLV 6 */
typedef struct ogs_nas_ue_additional_security_capability_s {
    uint8_t length;
    union {
        struct {
        ED8(uint8_t nea0:1;,
            uint8_t nea1:1;,
            uint8_t nea2:1;,
            uint8_t nea3:1;,
            uint8_t nea4:1;,
            uint8_t nea5:1;,
            uint8_t nea6:1;,
            uint8_t nea7:1;)
        };
        uint8_t nea;
    };
    union {
        struct {
        ED8(uint8_t nea8:1;,
            uint8_t nea9:1;,
            uint8_t nea10:1;,
            uint8_t nea11:1;,
            uint8_t nea12:1;,
            uint8_t nea13:1;,
            uint8_t nea14:1;,
            uint8_t nea15:1;)
        };
        uint8_t nea_8_15;
    };
    union {
        struct {
        ED8(uint8_t nia0:1;,
            uint8_t nia1:1;,
            uint8_t nia2:1;,
            uint8_t nia3:1;,
            uint8_t nia4:1;,
            uint8_t nia5:1;,
            uint8_t nia6:1;,
            uint8_t nia7:1;)
        };
        uint8_t nia;
    };
    union {
        struct {
        ED8(uint8_t nia8:1;,
            uint8_t nia9:1;,
            uint8_t nia10:1;,
            uint8_t nia11:1;,
            uint8_t nia12:1;,
            uint8_t nia13:1;,
            uint8_t nia14:1;,
            uint8_t nia15:1;)
        };
        uint8_t nia_8_15;
    };
} __attribute__ ((packed)) ogs_nas_ue_additional_security_capability_t;

/* 9.9.3.57 N1 UE network capability
 * O TLV 3-15 */
typedef struct ogs_nas_n1_ue_network_capability_s {
    uint8_t length;
    struct {
    ED7(uint8_t spare:2;,
        uint8_t user_plane_ciot_5gs_optimization:1;,
        uint8_t control_plane_ciot_5gs_optimization:1;,
        uint8_t user_plane_ciot_5gs_optimization_supported:1;,
        uint8_t header_compression_for_control_plane_ciot_5gs_optimization_supported:1;,
        uint8_t n3_data_transfer_not_supported:1;,
        uint8_t control_plane_ciot_5gs_optimization_supported:1;)
    };
} ogs_nas_n1_ue_network_capability_t;

/* 9.9.3.58 UE radio capability ID availability
 * O TLV 3 */
typedef struct ogs_nas_ue_radio_capability_id_availability_s {
    uint8_t length;
ED2(uint8_t spare:5;,
    uint8_t value:3;)
} __attribute__ ((packed)) ogs_nas_ue_radio_capability_id_availability_t;

/* 9.9.3.59 UE radio capability ID request
 * O TLV 3 */
typedef struct ogs_nas_ue_radio_capability_id_request_s {
    uint8_t length;
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t ue_radio_capability_id_available:1;)
} __attribute__ ((packed)) ogs_nas_ue_radio_capability_id_request_t;

/* 9.9.3.63 NB-S1 DRX parameter
 * O TLV 3 */
typedef struct ogs_nas_nb_s1_drx_parameter_s {
    uint8_t length;
ED2(uint8_t spare:4;,
    uint8_t value:4;)
} __attribute__ ((packed)) ogs_nas_nb_s1_drx_parameter_t;

/* 9.9.3.64 IMSI offset
 * O TLV 4 */
typedef struct ogs_nas_imsi_offset_s {
    uint8_t length;
    uint16_t value;
} __attribute__ ((packed)) ogs_nas_imsi_offset_t;

/* 9.9.3.67 EPS additional request result
 * O TLV 3 */
typedef struct ogs_nas_additional_request_result_s ogs_nas_eps_additional_request_result_t;

/* 9.9.4.1 Access point name
 * See subclause 10.5.6.1 in 3GPP TS 24.008 [13].
 * O TLV 3-102 */
typedef struct ogs_nas_access_point_name_s {
    uint8_t length;
    char apn[OGS_MAX_APN_LEN];
} __attribute__ ((packed)) ogs_nas_access_point_name_t;

/* 9.9.4.2A Connectivity type
 * See subclause 10.5.6.19 in 3GPP TS 24.008 [13].
 * O TV 1 */
typedef struct ogs_nas_connectivity_type_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t considered_lipa_pdn_connection:1;)
} __attribute__ ((packed)) ogs_nas_connectivity_type_t;

/* 9.9.4.4 ESM cause
 * M V 1
 * Annex B (informative) Cause values for EPS session management
   B.1 Causes related to nature of request */
#define OGS_NAS_ESM_CAUSE_OPERATOR_DETERMINED_BARRING 8
#define OGS_NAS_ESM_CAUSE_INSUFFICIENT_RESOURCES 26
#define OGS_NAS_ESM_CAUSE_MISSING_OR_UNKNOWN_APN 27
#define OGS_NAS_ESM_CAUSE_UNKNOWN_PDN_TYPE 28
#define OGS_NAS_ESM_CAUSE_USER_AUTHENTICATION_FAILED 29
#define OGS_NAS_ESM_CAUSE_REQUEST_REJECTED_BY_SERVING_GW_OR_PDN_GW 30
#define OGS_NAS_ESM_CAUSE_REQUEST_REJECTED_UNSPECIFIED 31
#define OGS_NAS_ESM_CAUSE_SERVICE_OPTION_NOT_SUPPORTED 32
#define OGS_NAS_ESM_CAUSE_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED 33
#define OGS_NAS_ESM_CAUSE_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER 34
#define OGS_NAS_ESM_CAUSE_PTI_ALREADY_IN_USE 35
#define OGS_NAS_ESM_CAUSE_REGULAR_DEACTIVATION 36
#define OGS_NAS_ESM_CAUSE_EPS_QOS_NOT_ACCEPTED 37
#define OGS_NAS_ESM_CAUSE_NETWORK_FAILURE 38
#define OGS_NAS_ESM_CAUSE_REACTIVATION_REQUESTED 39
#define OGS_NAS_ESM_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION  41
#define OGS_NAS_ESM_CAUSE_SYNTACTICAL_ERROR_IN_THE_TFT_OPERATION 42
#define OGS_NAS_ESM_CAUSE_INVALID_EPS_BEARER_IDENTITY 43
#define OGS_NAS_ESM_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTERS 44
#define OGS_NAS_ESM_CAUSE_SYNTACTICAL_ERROR_IN_PACKET_FILTERS 45
#define OGS_NAS_ESM_CAUSE_PTI_MISMATCH 47
#define OGS_NAS_ESM_CAUSE_LAST_PDN_DISCONNECTION_NOT_ALLOWED 49
#define OGS_NAS_ESM_CAUSE_PDN_TYPE_IPV4_ONLY_ALLOWED 50
#define OGS_NAS_ESM_CAUSE_PDN_TYPE_IPV6_ONLY_ALLOWED 51
#define OGS_NAS_ESM_CAUSE_SINGLE_ADDRESS_BEARERS_ONLY_ALLOWED 52
#define OGS_NAS_ESM_CAUSE_ESM_INFORMATION_NOT_RECEIVED 53
#define OGS_NAS_ESM_CAUSE_PDN_CONNECTION_DOES_NOT_EXIST 54
#define OGS_NAS_ESM_CAUSE_MULTIPLE_PDN_CONNECTIONS_FOR_A_GIVEN_APN_NOT_ALLOWED 55
#define OGS_NAS_ESM_CAUSE_COLLISION_WITH_NETWORK_INITIATED_REQUEST 56
#define OGS_NAS_ESM_CAUSE_PDN_TYPE_IPV4V6_ONLY_ALLOWED 57
#define OGS_NAS_ESM_CAUSE_PDN_TYPE_NON_IP_ONLY_ALLOWED 58
#define OGS_NAS_ESM_CAUSE_UNSUPPORTED_QCI_VALUE 59
#define OGS_NAS_ESM_CAUSE_BEARER_HANDLING_NOT_SUPPORTED 60
#define OGS_NAS_ESM_CAUSE_MAXIMUM_NUMBER_OF_EPS_BEARERS_REACHED 65
#define OGS_NAS_ESM_CAUSE_REQUESTED_APN_NOT_SUPPORTED_IN_CURRENT_RAT_AND_PLMN_COMBINATION 66
#define OGS_NAS_ESM_CAUSE_INVALID_PTI_VALUE 81
#define OGS_NAS_ESM_CAUSE_APN_RESTRICTION_VALUE_INCOMPATIBLE_WITH_ACTIVE_EPS_BEARER_CONTEXT 112
#define OGS_NAS_ESM_CAUSE_MULTIPLE_ACCESSES_TO_A_PDN_CONNECTION_NOT_ALLOWED 113
/* B.2 Protocol errors (e.g., unknown message) class */
#define OGS_NAS_ESM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE 95
#define OGS_NAS_ESM_CAUSE_INVALID_MANDATORY_INFORMATION 96
#define OGS_NAS_ESM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED 97
#define OGS_NAS_ESM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE 98
#define OGS_NAS_ESM_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED 99
#define OGS_NAS_ESM_CAUSE_CONDITIONAL_IE_ERROR 100
#define OGS_NAS_ESM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE 101
#define OGS_NAS_ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED 111
typedef uint8_t ogs_nas_esm_cause_t;

/* 9.9.4.5 ESM information transfer flag
 * O TV 1 */
typedef struct ogs_nas_esm_information_transfer_flag_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t security_protected_required:1;)
} __attribute__ ((packed)) ogs_nas_esm_information_transfer_flag_t;

/* 9.9.4.6 Linked EPS bearer identity
 * M V 1/2 */
typedef struct ogs_nas_linked_eps_bearer_identity_s {
ED2(uint8_t spare:4;,
    uint8_t eps_bearer_identity:4;)
} __attribute__ ((packed)) ogs_nas_linked_eps_bearer_identity_t;

/* 9.9.4.7 LLC service access point identifier
 * See subclause 10.5.6.9 in 3GPP TS 24.008 [13].
 * O TV 2 */
typedef uint8_t ogs_nas_llc_service_access_point_identifier_t;

/* 9.9.4.8 Packet flow Identifier
 * See subclause 10.5.6.11 in 3GPP TS 24.008 [13].
 * O TLV 3 */
#define OGS_NAS_PACKET_FLOW_IDENTIFIER_BEST_EFFORT          0
#define OGS_NAS_PACKET_FLOW_IDENTIFIER_SIGNALLING           1
#define OGS_NAS_PACKET_FLOW_IDENTIFIER_SMS                  2
#define OGS_NAS_PACKET_FLOW_IDENTIFIER_TOM8                 3
#define OGS_NAS_PACKET_FLOW_IDENTIFIER_DYNAMIC_ASSIGN_MIN   8
#define OGS_NAS_PACKET_FLOW_IDENTIFIER_DYNAMIC_ASSIGN_MAX   0xef
typedef struct ogs_nas_packet_flow_identifier_s {
    uint8_t length;
ED2(uint8_t spare:1;,
    uint8_t value:7;)
} __attribute__ ((packed)) ogs_nas_packet_flow_identifier_t;

/* 9.9.4.13 Radio priority
 * See subclause 10.5.7.2 in 3GPP TS 24.008 [13].
 * O TV 1 */
typedef struct ogs_nas_radio_priority_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t value:3;)
} __attribute__ ((packed)) ogs_nas_radio_priority_t;

/* 9.9.4.9 PDN address
 * M LV 6-14 */
typedef ogs_nas_pdu_address_t ogs_nas_pdn_address_t;

/* 9.9.4.11 Protocol configuration options
 * See subclause 10.5.6.3 in 3GPP TS 24.008 [13].
 * O TLV 3-253 */
typedef struct ogs_nas_protocol_configuration_options_s {
    uint8_t length;
    uint8_t buffer[OGS_MAX_PCO_LEN];
} __attribute__ ((packed)) ogs_nas_protocol_configuration_options_t;

/* 9.9.4.12 Quality of service
 * See subclause 10.5.6.5 in 3GPP TS 24.008 [13].
 * O TLV 14-22 */
#define OGS_NAS_QOS_LEN 20
typedef struct ogs_nas_quality_of_service_s {
    uint8_t length;
    uint8_t buffer[OGS_NAS_QOS_LEN];
} __attribute__ ((packed)) ogs_nas_quality_of_service_t;

/* 9.9.4.15 Traffic flow aggregate description
 * see subclause 10.5.6.12 in 3GPP TS 24.008 [13]
 * M LV 2-256 */
#define OGS_NAS_MAX_TRAFFIC_FLOW_AGGREGATE_DESCRIPTION 255
typedef struct ogs_nas_traffic_flow_aggregate_description_s {
    uint8_t length;
    uint8_t buffer[OGS_NAS_MAX_TRAFFIC_FLOW_AGGREGATE_DESCRIPTION];
} __attribute__ ((packed)) ogs_nas_traffic_flow_aggregate_description_t;

/* 9.9.4.16 Traffic flow template
 * See subclause 10.5.6.12 in 3GPP TS 24.008 [13].
 * M LV 2-256 */
#define OGS_NAS_MAX_TRAFFIC_FLOW_TEMPLATE 255
typedef struct ogs_nas_traffic_flow_template_s {
    uint8_t length;
    uint8_t buffer[OGS_NAS_MAX_TRAFFIC_FLOW_TEMPLATE];
} __attribute__ ((packed)) ogs_nas_traffic_flow_template_t;

/* 9.9.4.17 Transaction identifier
 * 3GPP TS 24.008 [13], subclause 10.5.6.7.
 * O TLV 3-4 */
typedef struct ogs_nas_transaction_identifier_s {
    uint8_t length;
    uint16_t linked_ti;
} __attribute__ ((packed)) ogs_nas_transaction_identifier_t;

/* 9.9.4.18 WLAN offload acceptability
 * 3GPP TS 24.008 [13], subclause 10.5.6.20
 * O TV 1 */
typedef struct ogs_nas_wlan_offload_acceptability_s {
ED4(uint8_t type:4;,
    uint8_t spare:2;,
    uint8_t utran_via_wlan_acceptable:1;,
    uint8_t e_utran_via_wlan_acceptable:1;)
} __attribute__ ((packed)) ogs_nas_wlan_offload_acceptability_t;

/* 9.9.4.27 Header compression configuration status
 * O TLV 4 */
typedef struct ogs_nas_header_compression_configuration_status_s {
    uint8_t length;
    uint16_t value;
} __attribute__ ((packed)) ogs_nas_header_compression_configuration_status_t;

/* 9.9.4.29 Extended APN aggregate maximum bit rate
 * O TLV 8 */
typedef ogs_nas_session_ambr_t ogs_nas_extended_apn_aggregate_maximum_bit_rate_t;

#ifdef __cplusplus
}
#endif

#endif /* OGS_NAS_EPS_TYPES_H */

