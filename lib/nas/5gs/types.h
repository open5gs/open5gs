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

#ifndef OGS_NAS_5GS_TYPES_H
#define OGS_NAS_5GS_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/**********************
 * NAS GUTI Structure */
typedef struct ogs_nas_5gs_guti_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    ogs_amf_id_t amf_id;
    uint32_t m_tmsi;
} __attribute__ ((packed)) ogs_nas_5gs_guti_t;

/* 9.11.2.1A DNN
 * O TLV 3-102 */
typedef struct ogs_nas_dnn_s {
    uint8_t length;
    char value[OGS_MAX_DNN_LEN+1];
} ogs_nas_dnn_t;

/* 9.11.2.2 EAP message
 * O TLV-E 7-1503 */
typedef struct ogs_nas_eap_message_s {
    uint16_t length;
    void *buffer;
} ogs_nas_eap_message_t;

/* 9.11.2.8 S-NSSAI
 * O TLV 3-10 */
#define OGS_NAS_S_NSSAI_SST_LEN 1
#define OGS_NAS_S_NSSAI_SST_AND_MAPPED_HPLMN_SST_LEN 2
#define OGS_NAS_S_NSSAI_SST_AND_SD 4
#define OGS_NAS_S_NSSAI_SST_SD_AND_MAPPED_HPLMN_SST_LEN 5
#define OGS_NAS_S_NSSAI_SST_SD_AND_MAPPED_HPLMN_SST_SD_LEN 8
typedef struct ogs_nas_s_nssai_s {
    uint8_t length;
    uint8_t buffer[OGS_NAS_S_NSSAI_SST_SD_AND_MAPPED_HPLMN_SST_SD_LEN];
} __attribute__ ((packed)) ogs_nas_s_nssai_t;

typedef struct ogs_nas_s_nssai_ie_s {
    uint8_t sst;
    ogs_uint24_t sd;
    uint8_t mapped_hplmn_sst;
    bool mapped_hplmn_sst_presence;
    ogs_uint24_t mapped_hplmn_sd;
} __attribute__ ((packed)) ogs_nas_s_nssai_ie_t;

void ogs_nas_build_s_nssai(
        ogs_nas_s_nssai_t *nas_s_nssai, ogs_nas_s_nssai_ie_t *nas_s_nssai_ie);
void ogs_nas_build_s_nssai2(
        ogs_nas_s_nssai_t *nas_s_nssai,
        ogs_s_nssai_t *s_nssai, ogs_s_nssai_t *mapped_hplmn);
int ogs_nas_parse_s_nssai(
        ogs_nas_s_nssai_ie_t *nas_s_nssai_ie, ogs_nas_s_nssai_t *nas_s_nssai);

/* 9.11.2.10 Service-level-AA container
 * O TLV-E 6-n */
typedef struct ogs_nas_service_level_aa_container_s {
    uint16_t length;
    void *buffer;
} __attribute__ ((packed)) ogs_nas_service_level_aa_container_t;

/* 9.11.3.1 5GMM capability
 * O TLV 3-15 */
typedef struct ogs_nas_5gmm_capability_s {
    uint8_t length;
ED8(uint8_t service_gap_control:1;,
    uint8_t iphc_cp_ciot_5gs_optimization:1;,
    uint8_t n3_data_trasfer:1;,
    uint8_t cp_ciot_5gs_optimization:1;,
    uint8_t restrict_on_use_of_enhanced_coverage_support:1;,
    uint8_t lte_positioning_protocol_capability:1;,
    uint8_t ho_attach:1;,
    uint8_t s1_mode:1;)
ED8(uint8_t radio_capability_signalling_optimization_capability:1;,
    uint8_t nssaa:1;,
    uint8_t lcs_5g_notification_mechanisms_capability:1;,
    uint8_t v2x_communication_over_nr_pc5_capability:1;,
    uint8_t v2x_communication_over_e_utra_pc5_capability:1;,
    uint8_t v2x_capability:1;,
    uint8_t up_ciot_5gs_optimization:1;,
    uint8_t srvcc_5g_capability:1;)
ED8(uint8_t fiveg_prose_layer_2_ue_to_network_relay:1;,
    uint8_t fiveg_prose_direct_communication:1;,
    uint8_t fiveg_prose_direct_discovery:1;,
    uint8_t extended_rejected_nssai_support:1;,
    uint8_t ethernet_header_compression_cp_ciot_5gs_optimization:1;,
    uint8_t multiple_user_plane_resource_support:1;,
    uint8_t wusa_information_reception_capability:1;,
    uint8_t closed_access_group_capability:1;)
ED8(uint8_t paging_restriction:1;,
    uint8_t reject_paging_request:1;,
    uint8_t paging_indication_for_voice_services:1;,
    uint8_t n1_nas_signalling_connection_release:1;,
    uint8_t nr_paging_subgroup_support_indication:1;,
    uint8_t five_prose_layer_3_ue_to_network_remote:1;,
    uint8_t five_prose_layer_2_ue_to_network_remote:1;,
    uint8_t five_prose_layer_3_ue_to_network_relay:1;)
ED7(uint8_t spare1:2;,
    uint8_t nsag:1;,
    uint8_t extended_cag_information_list_support:1;,
    uint8_t sor_snpn_si:1;,
    uint8_t event_notification:1;,
    uint8_t minimization_of_service_interruption:1;,
    uint8_t nssrg:1;)
    uint8_t spare2[8];
} __attribute__ ((packed)) ogs_nas_5gmm_capability_t;

/* 9.11.3.2 5GMM cause
 * M V 1 */

/* REQUEST_ACCEPTED(16) cause is defined by Open5GS */
#define OGS_5GMM_CAUSE_REQUEST_ACCEPTED 16

/* Annex A (informative): Cause values for 5GS mobility management
 * A.1 Causes related to UE identification */
#define OGS_5GMM_CAUSE_ILLEGAL_UE 3
#define OGS_5GMM_CAUSE_ILLEGAL_ME 6
#define OGS_5GMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK 9
#define OGS_5GMM_CAUSE_IMPLICITLY_DE_REGISTERED 10

/* A.2 Cause related to subscription options */
#define OGS_5GMM_CAUSE_PEI_NOT_ACCEPTED 5
#define OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED 7
#define OGS_5GMM_CAUSE_PLMN_NOT_ALLOWED 11
#define OGS_5GMM_CAUSE_TRACKING_AREA_NOT_ALLOWED 12
#define OGS_5GMM_CAUSE_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA 13
#define OGS_5GMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA 15
#define OGS_5GMM_CAUSE_N1_MODE_NOT_ALLOWED 27
#define OGS_5GMM_CAUSE_REDIRECTION_TO_EPC_REQUIRED 31
#define OGS_5GMM_CAUSE_NON_3GPP_ACCESS_TO_5GCN_NOT_ALLOWED 72
#define OGS_5GMM_CAUSE_TEMPORARILY_NOT_AUTHORIZED_FOR_THIS_SNPN 74
#define OGS_5GMM_CAUSE_PERMANENTLY_NOT_AUTHORIZED_FOR_THIS_SNPN 75
#define OGS_5GMM_CAUSE_NOT_AUTHORIZED_FOR_THIS_CAG_OR_AUITHORIZED_FOR_CAG_CELLS_ONLY 76
#define WIRELESS_ACCESS_AREA_NOT_ALLOWED 76

/* A.3 Causes related to PLMN or SNPN specific network failures
 * and congestion/authentication failures */
#define OGS_5GMM_CAUSE_MAC_FAILURE 20
#define OGS_5GMM_CAUSE_SYNCH_FAILURE 21
#define OGS_5GMM_CAUSE_CONGESTION 22
#define OGS_5GMM_CAUSE_UE_SECURITY_CAPABILITIES_MISMATCH 23
#define OGS_5GMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED 24
#define OGS_5GMM_CAUSE_NON_5G_AUTHENTICATION_UNACCEPTABLE 26
#define OGS_5GMM_CAUSE_RESTRICTED_SERVICE_AREA 28
#define OGS_5GMM_CAUSE_LADN_NOT_AVAILABLE 43
#define OGS_5GMM_CAUSE_NO_NETWORK_SLICES_AVAILABLE 62
#define OGS_5GMM_CAUSE_MAXIMUM_NUMBER_OF_PDU_SESSIONS_REACHED 65
#define OGS_5GMM_CAUSE_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE_AND_DNN 67
#define OGS_5GMM_CAUSE_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE 69
#define OGS_5GMM_CAUSE_NGKSI_ALREADY_IN_USE 71
#define OGS_5GMM_CAUSE_SERVING_NETWORK_NOT_AUTHORIZED 73
#define OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED 90
#define OGS_5GMM_CAUSE_DNN_NOT_SUPPORTED_OR_NOT_SUBSCRIBED_IN_THE_SLICE 91
#define OGS_5GMM_CAUSE_INSUFFICIENT_USER_PLANE_RESOURCES_FOR_THE_PDU_SESSION 92

/* A.4 Causes related to invalid messages */
#define OGS_5GMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE 95
#define OGS_5GMM_CAUSE_INVALID_MANDATORY_INFORMATION 96
#define OGS_5GMM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED 97
#define OGS_5GMM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE 98
#define OGS_5GMM_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED 99
#define OGS_5GMM_CAUSE_CONDITIONAL_IE_ERROR 100
#define OGS_5GMM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE 101
#define OGS_5GMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED 111
typedef uint8_t ogs_nas_5gmm_cause_t;

/* 9.11.3.2A 5GS DRX parameters
 * O TLV 3 */
#define OGS_NAS_DRX_CYCLE_PARAMETER_T_32 1
#define OGS_NAS_DRX_CYCLE_PARAMETER_T_64 2
#define OGS_NAS_DRX_CYCLE_PARAMETER_T_128 3
#define OGS_NAS_DRX_CYCLE_PARAMETER_T_256 4
typedef struct ogs_nas_5gs_drx_parameters_s {
    uint8_t length;
ED2(uint8_t spare:4;,
    uint8_t value:4;)
} __attribute__ ((packed)) ogs_nas_5gs_drx_parameters_t;

/* 9.11.3.3 5GS identity type
 * M V 1/2 */
#define OGS_NAS_5GS_MOBILE_IDENTITY_IS_NOT_AVAILABLE 0
#define OGS_NAS_5GS_MOBILE_IDENTITY_SUCI 1
#define OGS_NAS_5GS_MOBILE_IDENTITY_GUTI 2
#define OGS_NAS_5GS_MOBILE_IDENTITY_IMEI 3
#define OGS_NAS_5GS_MOBILE_IDENTITY_S_TMSI 4
#define OGS_NAS_5GS_MOBILE_IDENTITY_IMEISV 5
typedef struct ogs_nas_5gs_identity_type_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t value:3;)
} __attribute__ ((packed)) ogs_nas_5gs_identity_type_t;

/* 9.11.3.4 5GS mobile identity
 * M LV-E 6-n */
typedef struct ogs_nas_5gs_mobile_identity_header_s {
#define OGS_NAS_5GS_SUPI_FORMAT_IMSI 0
#define OGS_NAS_5GS_SUPI_FORMAT_NETWORK_SPECIFIC_IDENTIFIER 1
ED3(uint8_t supi_format:4;,
    uint8_t odd_even:1;,
    uint8_t type:3;)
} __attribute__ ((packed)) ogs_nas_5gs_mobile_identity_header_t;
#define OGS_NAS_5GS_MOBILE_IDENTITY_SUCI_MIN_SIZE 8
typedef struct ogs_nas_5gs_mobile_identity_suci_s {
    ogs_nas_5gs_mobile_identity_header_t h;
    ogs_nas_plmn_id_t nas_plmn_id;
    ED2(uint8_t routing_indicator2:4;,
        uint8_t routing_indicator1:4;)
    ED2(uint8_t routing_indicator4:4;,
        uint8_t routing_indicator3:4;)
ED2(uint8_t spare3:4;,
    uint8_t protection_scheme_id:4;)
    uint8_t home_network_pki_value;
} __attribute__ ((packed)) ogs_nas_5gs_mobile_identity_suci_t;
typedef struct ogs_nas_5gs_mobile_identity_guti_s {
    ogs_nas_5gs_mobile_identity_header_t h;
    ogs_nas_plmn_id_t nas_plmn_id;
    ogs_amf_id_t amf_id;
    uint32_t m_tmsi;
} __attribute__ ((packed)) ogs_nas_5gs_mobile_identity_guti_t;
typedef struct ogs_nas_5gs_mobile_identity_s_tmsi_s {
    ogs_nas_5gs_mobile_identity_header_t h;
    uint8_t set1;
ED2(uint8_t set2:2;,
    uint8_t pointer:6;)
    uint32_t m_tmsi;
} __attribute__ ((packed)) ogs_nas_5gs_mobile_identity_s_tmsi_t;
typedef struct ogs_nas_5gs_mobile_identity_s {
    uint16_t length;
    void *buffer;
} ogs_nas_5gs_mobile_identity_t;

/* 9.11.3.5 5GS network feature support
 * M LV 2 */
typedef struct ogs_nas_5gs_network_feature_support_s {
    uint8_t length;
ED6(uint8_t mps_indicator:1;,
    uint8_t interworking_without_n26_interface_indicator:1;,
    uint8_t emergency_services_fallback_indicator_for_3gpp_access:2;,
    uint8_t emergency_service_support_indicator_for_3gpp_access:2;,
    uint8_t ims_voice_over_ps_session_over_non_3gpp_access_indicator:1;,
    uint8_t ims_voice_over_ps_session_over_3gpp_access_indicator:1;)
ED7(uint8_t user_plane_ciot_5gs_optimization:1;,
    uint8_t ip_header_compression_for_control_plane_ciot_5gs_optimization:1;,
    uint8_t n3_data_transfer:1;,
    uint8_t control_plane_ciot_5gs_optimization:1;,
    uint8_t restriction_on_enhanced_coverage:2;,
    uint8_t mcs_indicator:1;,
    uint8_t emergency_service_support_for_non_3gpp_access_indicator:1;)
ED8(uint8_t spare:1;,
    uint8_t paging_restriction:1;,
    uint8_t reject_paging_request:1;,
    uint8_t paging_indication_for_voice_services:1;,
    uint8_t n1_nas_signalling_connection_release:1;,
    uint8_t ethernet_header_compression_for_control_plane_ciot_5gs_optimization:1;,
    uint8_t atsss_support_indicator:1;,
    uint8_t location_services_indicator_in_5gc:1;)
} ogs_nas_5gs_network_feature_support_t;

/* 9.11.3.6 5GS registration result
 * O TLV 3-5 */
typedef struct ogs_nas_5gs_registration_result_s {
    uint8_t length;
ED6(uint8_t spare:1;,
    uint8_t disaster_roaming_registration_result_value:1;,
    uint8_t emergency_registered:1;,
    uint8_t network_slice_specific_authentication_and_authorization_is_to_be_performed:1;,
    uint8_t sms_over_nas_transport_allowed:1;,
    uint8_t value:3;)
} ogs_nas_5gs_registration_result_t;

/* 9.11.3.7 5GS registration type
 * M V 1/2 */
#define OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL 1
#define OGS_NAS_5GS_REGISTRATION_TYPE_MOBILITY_UPDATING 2
#define OGS_NAS_5GS_REGISTRATION_TYPE_PERIODIC_UPDATING 3
#define OGS_NAS_5GS_REGISTRATION_TYPE_EMERGENCY 4
typedef struct ogs_nas_5gs_registration_type_s {
    union {
        struct {
        ED4(uint8_t tsc:1;,
            uint8_t ksi:3;,
            uint8_t follow_on_request:1;,
            uint8_t value:3;)
        };
        uint8_t data;
    };
} ogs_nas_5gs_registration_type_t;

/* 9.11.3.8 5GS tracking area identity
 * O TV 6 */
typedef struct ogs_nas_5gs_tracking_area_identity_s {
    ogs_nas_plmn_id_t nas_plmn_id;
    ogs_uint24_t tac;
} __attribute__ ((packed)) ogs_nas_5gs_tracking_area_identity_t;

typedef ogs_nas_5gs_tracking_area_identity_t ogs_nas_5gs_tai_t;

/* 9.11.3.9 5GS tracking area identity list
 * O TLV 9-114 */
#define OGS_NAS_5GS_MAX_TAI_LIST_LEN    114
typedef struct ogs_5gs_tai0_list_s {
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
        ogs_uint24_t tac[OGS_MAX_NUM_OF_TAI];
    } __attribute__ ((packed)) tai[OGS_MAX_NUM_OF_TAI];
} __attribute__ ((packed)) ogs_5gs_tai0_list_t;

typedef struct ogs_5gs_tai1_list_s {
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
        ogs_uint24_t tac;
    } __attribute__ ((packed)) tai[OGS_MAX_NUM_OF_TAI];
} __attribute__ ((packed)) ogs_5gs_tai1_list_t;

typedef struct ogs_5gs_tai2_list_s {
ED3(uint8_t spare:1;,
    uint8_t type:2;,
    uint8_t num:5;)
    /*
     * Do not change 'ogs_5gs_tai_t' to 'ogs_nas_tracking_area_identity_t'.
     * Use 'ogs_5gs_tai_t' for easy implementation.
     * ogs_nas_tai_list_build() changes to NAS
     * format(ogs_nas_tracking_area_identity_t)
     * and is sent to the UE.
     */
    ogs_5gs_tai_t tai[OGS_MAX_NUM_OF_TAI];
} __attribute__ ((packed)) ogs_5gs_tai2_list_t;

typedef struct ogs_nas_5gs_tracking_area_identity_list_s {
    uint8_t length;
    uint8_t buffer[OGS_NAS_5GS_MAX_TAI_LIST_LEN];
} __attribute__ ((packed)) ogs_nas_5gs_tracking_area_identity_list_t;

int ogs_nas_5gs_tai_list_build(
        ogs_nas_5gs_tracking_area_identity_list_t *target,
        ogs_5gs_tai0_list_t *source0,
        ogs_5gs_tai1_list_t *source1,
        ogs_5gs_tai2_list_t *source2);

/* 9.11.3.9A 5GS update type
 * O TLV 3 */
typedef struct ogs_nas_5gs_update_type_s {
    uint8_t length;
ED7(uint8_t spare:2;,
    uint8_t user_plane_ciot_eps_optimization:1;,
    uint8_t control_plane_ciot_eps_optimization:1;,
    uint8_t user_plane_ciot_5gs_optimization:1;,
    uint8_t control_plane_ciot_5gs_optimization:1;,
    uint8_t ng_ran_radio_capability_update_needed:1;,
    uint8_t sms_over_nas_supported:1;)
} __attribute__ ((packed)) ogs_nas_5gs_update_type_t;

/* 9.11.3.10 ABBA
 * M LV 3-n */
#define OGS_NAS_MAX_ABBA_LEN 255
typedef struct ogs_nas_abba_s {
    uint8_t length;
    uint8_t value[OGS_NAS_MAX_ABBA_LEN];
} ogs_nas_abba_t;

/* 9.11.3.11 Access Type
 * M V 1/2 */
typedef struct ogs_nas_access_type_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t value:3;)
} __attribute__ ((packed)) ogs_nas_access_type_t;

/* 9.11.3.12 Additional 5G security information
 * O TLV 3 */
typedef struct ogs_nas_additional_5g_security_information_s {
    uint8_t length;
ED3(uint8_t spare:6;,
    uint8_t retransmission_of_initial_nas_message_request:1;,
    uint8_t horizontal_derivation_parameter:1;)
} __attribute__ ((packed)) ogs_nas_additional_5g_security_information_t;

/* 9.11.3.13 Allowed PDU session status
 * O TLV 4-34 */
typedef struct ogs_nas_allowed_pdu_session_status_s {
    uint8_t length;
    uint16_t psi;
    uint8_t spare[30];
} __attribute__ ((packed)) ogs_nas_allowed_pdu_session_status_t;

/* 9.11.3.18 Configuration update indication
 * O TV 1 */
typedef struct ogs_nas_configuration_update_indication_s {
ED4(uint8_t type:4;,
    uint8_t spare:2;,
    uint8_t registration_requested:1;,
    uint8_t acknowledgement_requested:1;)
} __attribute__ ((packed)) ogs_nas_configuration_update_indication_t;

/* 9.11.3.18A CAG information list
 * O TLV-E 3-n */
typedef struct ogs_nas_cag_information_list_s {
    uint16_t length;
    void *buffer;
} ogs_nas_cag_information_list_t;

/* 9.11.3.18B CIoT small data container
 * O TLV 4-257 */
#define OGS_NAS_MAX_CIOT_SMALL_DATA_CONTAINER_LEN 254
typedef struct ogs_nas_ciot_small_data_container_s {
    uint8_t length;
ED3(uint8_t data_type:3;,
    uint8_t ddx:2;,
    uint8_t pdu_session_identity:1;)
    uint8_t buffer[OGS_NAS_MAX_CIOT_SMALL_DATA_CONTAINER_LEN];
} __attribute__ ((packed)) ogs_nas_ciot_small_data_container_t;

/* 9.11.3.18D Control plane service type
 * M V 1/2 */
#define OGS_NAS_5GS_CONTROL_PLANE_SERVICE_TYPE_MO 0
#define OGS_NAS_5GS_CONTROL_PLANE_SERVICE_TYPE_MT 1
typedef struct ogs_nas_control_plane_service_type_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t value:1;)
} __attribute__ ((packed)) ogs_nas_control_plane_service_type_t;

/* 9.11.3.20 De-registration type
 * M V 1/2 */
typedef struct ogs_nas_de_registration_type_s {
ED5(uint8_t tsc:1;,
    uint8_t ksi:3;,
    uint8_t switch_off:1;,
    uint8_t re_registration_required:1;,
    uint8_t access_type:2;)
} __attribute__ ((packed)) ogs_nas_de_registration_type_t;

/* 9.11.3.24 EPS NAS message container
 * O TLV-E 4-n */
typedef struct ogs_nas_eps_nas_message_container_s {
    uint16_t length;
    void *buffer;
} ogs_nas_eps_nas_message_container_t;

/* 9.11.3.25 EPS NAS security algorithms
 * See subclause 9.9.3.23 in 3GPP TS 24.301 [15].
 * O TV 2 */
typedef ogs_nas_security_algorithms_t ogs_nas_eps_nas_security_algorithms_t;

/* 9.11.3.29 LADN indication
 * O TLV-E 3-811 */
typedef struct ogs_nas_ladn_indication_s {
    uint16_t length;
    void *buffer;
} ogs_nas_ladn_indication_t;

/* 9.11.3.30 LADN information
 * O TLV-E 12-1715 */
typedef struct ogs_nas_ladn_information_s {
    uint16_t length;
    void *buffer;
} ogs_nas_ladn_information_t;

/* 9.11.3.31 MICO indication
 * O TV 1 */
typedef struct ogs_nas_mico_indication_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t raai:1;)
} __attribute__ ((packed)) ogs_nas_mico_indication_t;

/* 9.11.3.31A MA PDU session information
 * O TV 1 */
typedef struct ogs_nas_ma_pdu_session_information_s {
ED2(uint8_t type:4;,
    uint8_t value:4;)
} __attribute__ ((packed)) ogs_nas_ma_pdu_session_information_t;

/* 9.11.3.31B Mapped NSSAI
 * O TLV 3-42 */
#define OGS_NAS_MAX_MAPPED_NSSAI_LEN 40
typedef struct ogs_nas_mapped_nssai_s {
    uint8_t length;
    char buffer[OGS_NAS_MAX_MAPPED_NSSAI_LEN];
} ogs_nas_mapped_nssai_t;

/* 9.11.3.33 message container
 * O TLV-E 4-n */
typedef struct ogs_nas_message_container_s {
    uint16_t length;
    void *buffer;
} __attribute__ ((packed)) ogs_nas_message_container_t;

/* 9.11.3.36 Network slicing indication
 * O TV 1 */
typedef struct ogs_nas_network_slicing_indication_s {
ED4(uint8_t type:4;,
    uint8_t spare:2;,
    uint8_t default_configured_nssai_indication:1;,
    uint8_t network_slicing_subscription_change_indication:1;)
}  __attribute__ ((packed)) ogs_nas_network_slicing_indication_t;

/* 9.11.3.37 NSSAI
 * O TLV 4-72 */
#define OGS_NAS_MAX_NSSAI_LEN 142
typedef struct ogs_nas_nssai_s {
    uint8_t length;
    uint8_t buffer[OGS_NAS_MAX_NSSAI_LEN];
} __attribute__ ((packed)) ogs_nas_nssai_t;

void ogs_nas_build_nssai(ogs_nas_nssai_t *nas_nssai,
        ogs_nas_s_nssai_ie_t *nas_s_nssai_ie, int num_of_nas_s_nssai_ie);
int ogs_nas_parse_nssai(
        ogs_nas_s_nssai_ie_t *nas_s_nssai_ie, ogs_nas_nssai_t *nas_nssai);

/* 9.11.3.37A NSSAI inclusion mode
 * O TV 1 */
#define OGS_NAS_NSSAI_INCLUSION_MODE_A 0
#define OGS_NAS_NSSAI_INCLUSION_MODE_B 1
#define OGS_NAS_NSSAI_INCLUSION_MODE_C 2
#define OGS_NAS_NSSAI_INCLUSION_MODE_D 3
typedef struct ogs_nas_nssai_inclusion_mode_s {
ED3(uint8_t type:4;,
    uint8_t spare:2;,
    uint8_t value:2;)
} __attribute__ ((packed)) ogs_nas_nssai_inclusion_mode_t;

/* 9.11.3.38 Operator-defined access category definitions
 * O TLV-E 3-n */
typedef struct ogs_nas_operator_defined_access_category_definitions_s {
    uint16_t length;
    void *buffer;
} ogs_nas_operator_defined_access_category_definitions_t;

/* 9.11.3.39 Payload container
 * O TLV-E 4-65535 */
typedef struct ogs_nas_payload_container_s {
    uint16_t length;
    void *buffer;
} ogs_nas_payload_container_t;

/* 9.11.3.40 Payload container type
 * O TV 1 */
#define OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION 1
#define OGS_NAS_PAYLOAD_CONTAINER_SMS 2
#define OGS_NAS_PAYLOAD_CONTAINER_LPP 3
#define OGS_NAS_PAYLOAD_CONTAINER_SOR 4
#define OGS_NAS_PAYLOAD_CONTAINER_UE_POLICY 5
#define OGS_NAS_PAYLOAD_CONTAINER_UE_PARAMETER_UPDATE 6
#define OGS_NAS_PAYLOAD_CONTAINER_MULTIPLE 7
typedef struct ogs_nas_payload_container_type_s {
ED2(uint8_t type:4;,
    uint8_t value:4;)
}  __attribute__ ((packed)) ogs_nas_payload_container_type_t;

/* 9.11.3.41 PDU session identity 2
 * C TV 2 */
typedef uint8_t ogs_nas_pdu_session_identity_2_t;

/* 9.11.3.42 PDU session reactivation result
 * O TLV 4-34 */
typedef ogs_nas_allowed_pdu_session_status_t ogs_nas_pdu_session_reactivation_result_t;

/* 9.11.3.43 PDU session reactivation result error cause
 * O TLV-E 5-515 */
typedef struct ogs_nas_pdu_session_reactivation_result_error_cause_s {
    uint16_t length;
    void *buffer;
} ogs_nas_pdu_session_reactivation_result_error_cause_t;

/* 9.11.3.44 PDU session status
 * O TLV 4-34 */
typedef ogs_nas_allowed_pdu_session_status_t ogs_nas_pdu_session_status_t;

/* 9.11.3.46 Rejected NSSAI
 * O TLV 4-42 */
#define OGS_NAS_MAX_REJECTED_NSSAI_LEN 40
typedef struct ogs_nas_rejected_nssai_s {
    uint8_t length;
    char buffer[OGS_NAS_MAX_REJECTED_NSSAI_LEN];
} ogs_nas_rejected_nssai_t;

typedef struct ogs_nas_rejected_s_nssai_s {
#define OGS_NAS_REJECTED_S_NSSAI_NOT_AVIALABLE_IN_PLMN 0
#define OGS_NAS_REJECTED_S_NSSAI_NOT_AVIALABLE_IN_TAI 1
#define OGS_NAS_REJECTED_S_NSSAI_NOT_AVIALABLE_DUE_TO_AUTH_FAILURE 22
ED2(uint8_t length_of_rejected_s_nssai:4;,
    uint8_t cause_value:4;)
    uint8_t sst;
    ogs_uint24_t sd;
} __attribute__ ((packed)) ogs_nas_rejected_s_nssai_t;

void ogs_nas_build_rejected_nssai(
        ogs_nas_rejected_nssai_t *rejected_nssai,
        ogs_nas_rejected_s_nssai_t *rejected_s_nssai,
        int num_of_rejected_s_nssai);
int ogs_nas_parse_rejected_nssai(
        ogs_nas_rejected_s_nssai_t *rejected_s_nssai,
        ogs_nas_rejected_nssai_t *rejected_nssai);

/* 9.11.3.49 Service area list
 * O TLV 6-114 */
#define OGS_NAS_MAX_SERVICE_AREA_LIST_LEN 112
typedef struct ogs_nas_service_area_list_s {
    uint8_t length;
    uint8_t buffer[112];
} ogs_nas_service_area_list_t;

/* 9.11.3.50 Service type
 * M V 1/2 */
#define OGS_NAS_SERVICE_TYPE_SIGNALLING 0
#define OGS_NAS_SERVICE_TYPE_DATA 1
#define OGS_NAS_SERVICE_TYPE_MOBILE_TERMINATED_SERVICES 2
#define OGS_NAS_SERVICE_TYPE_EMERGENCY_SERVICES 3
#define OGS_NAS_SERVICE_TYPE_EMERGENCY_SERVICES_FALLBACK 4
#define OGS_NAS_SERVICE_TYPE_HIGH_PRIORITY_ACCESS 5
#define OGS_NAS_SERVICE_TYPE_ELEVATED_SIGNALLING 6
/* 7(unused) shall be interpreted as "signalling", if received by the network
 * 8(unused) shall be interpreted as "signalling", if received by the network
 * 9(unused) shall be interpreted as "signalling", if received by the network
 * 10(unused) shall be interpreted as "data", if received by the network
 * 11(unused) shall be interpreted as "data", if received by the network */

/* 9.11.3.50A SMS indication
 * O TV 1 */
typedef struct ogs_nas_sms_indication_s {
ED2(uint8_t type:4;,
    uint8_t sai:4;)
} ogs_nas_sms_indication_t;

/* 9.11.3.51 SOR transparent container
 * O TLV-E 20-n */
typedef struct ogs_nas_sor_transparent_container_s {
    uint16_t length;
    void *buffer;
} ogs_nas_sor_transparent_container_t;

/* 9.11.3.48 S1 UE network capability
 * See subclause 9.9.3.34 in 3GPP TS 24.301 [15].
 * O TLV 4-15 */
typedef ogs_nas_ue_network_capability_t ogs_nas_s1_ue_network_capability_t;

/* 9.11.3.48A S1 UE security capability
 * O TLV 4-7
 * 9.9.3.36 UE security capability
 * M LV 3-6 */
typedef ogs_nas_ue_security_capability_t ogs_nas_s1_ue_security_capability_t;

/* 9.11.3.55 UE usage setting
 * O TLV 3 */
typedef struct ogs_nas_ue_usage_setting_s {
    uint8_t length;
ED2(uint8_t spare:7;,
    uint8_t data_centric:1;)
} __attribute__ ((packed)) ogs_nas_ue_usage_setting_t;

/* 9.11.3.57 Uplink data status
 * O TLV 4-34 */
typedef ogs_nas_allowed_pdu_session_status_t ogs_nas_uplink_data_status_t;

/* 9.11.3.70 Truncated 5G-S-TMSI configuration
 * O TLV 3 */
typedef struct ogs_nas_truncated_5g_s_tmsi_configuration_s {
    uint8_t length;
ED2(uint8_t amf_set_id:4;,
    uint8_t amf_pointer_value:4;)
} __attribute__ ((packed))  ogs_nas_truncated_5g_s_tmsi_configuration_t;

/* 9.11.3.72 N5GC indication
 * O T 1 */
typedef struct ogs_nas_n5gc_indication_s {
    uint8_t type;
} __attribute__ ((packed)) ogs_nas_n5gc_indication_t;

/* 9.11.3.73 NB-N1 mode DRX parameters
 * O TLV 3 */
typedef struct ogs_nas_nb_n1_mode_drx_parameters_s {
    uint8_t length;
ED2(uint8_t type:4;,
    uint8_t value:4;)
} __attribute__ ((packed)) ogs_nas_nb_n1_mode_drx_parameters_t;

/* 9.11.3.74 Additional configuration indication
 * O T 1 */
typedef struct ogs_nas_additional_configuration_indication_s {
    uint8_t type;
} __attribute__ ((packed)) ogs_nas_additional_configuration_indication_t;

/* 9.11.3.75 Extended rejected NSSAI
 * O TLV 5-90 */
#define OGS_NAS_MAX_EXTENDED_REJECTED_NSSAI_LEN 88
typedef struct ogs_nas_extended_rejected_nssai_s {
    uint8_t length;
    uint8_t buffer[OGS_NAS_MAX_EXTENDED_REJECTED_NSSAI_LEN];
} __attribute__ ((packed)) ogs_nas_extended_rejected_nssai_t;

/* 9.11.3.79 NID
 * See subclause 9.2.7 in 3GPP TS 24.502 [18]
 * O TLV 8 */
typedef struct ogs_nas_nid_s {
    uint8_t length;
ED2(uint8_t digit1:4;,
    uint8_t assignment_mode:4;)
ED2(uint8_t digit3:4;,
    uint8_t digit2:4;)
ED2(uint8_t digit5:4;,
    uint8_t digit4:4;)
ED2(uint8_t digit7:4;,
    uint8_t digit6:4;)
ED2(uint8_t digit9:4;,
    uint8_t digit8:4;)
ED2(uint8_t spare:4;,
    uint8_t digit10:4;)
} __attribute__ ((packed)) ogs_nas_nid_t;

/* 9.11.3.80 PEIPS assistance information
 * O TLV 3-n */
typedef struct ogs_nas_peips_assistance_information_s {
    uint8_t length;
    uint8_t spare[255];
} __attribute__ ((packed)) ogs_nas_peips_assistance_information_t;

/* 9.11.3.81 5GS additional request result
 * O TLV 3 */
typedef struct ogs_nas_additional_request_result_s ogs_nas_5gs_additional_request_result_t;

/* 9.11.3.82 NSSRG information
 * O TLV-E 7-65538 */
typedef struct ogs_nas_nssrg_information_s {
    uint16_t length;
    void *buffer;
} __attribute__ ((packed)) ogs_nas_nssrg_information_t;

/* 9.11.3.83 List of PLMNs to be used in disaster condition
 * O TLV 2-n */
typedef struct ogs_nas_list_of_plmns_to_be_used_in_disaster_condition_s {
    uint8_t length;
    uint8_t spare[255];
} __attribute__ ((packed)) ogs_nas_list_of_plmns_to_be_used_in_disaster_condition_t;

/* 9.11.3.84 Registration wait range
 * O TLV 4 */
typedef struct ogs_nas_registration_wait_range_s {
    uint8_t length;
    uint8_t min_time;
    uint8_t max_time;
} __attribute__ ((packed)) ogs_nas_registration_wait_range_t;

/* 9.11.3.85 PLMN identity
 * O TLV 5 */
typedef struct ogs_nas_plmn_identity_s {
    uint8_t length;
    ogs_nas_plmn_id_t nas_plmn_id;
} __attribute__ ((packed)) ogs_nas_plmn_identity_t;

/* 9.11.3.86 Extended CAG information list
 * O TLV-E 3-n */
typedef struct ogs_nas_extended_cag_information_list_s {
    uint8_t length;
    void *buffer;
} __attribute__ ((packed)) ogs_nas_extended_cag_information_list_t;

/* 9.11.3.87 NSAG information
 * O TLV-E 10-n */
typedef struct ogs_nas_nsag_information_s {
    uint8_t length;
    void *buffer;
} __attribute__ ((packed)) ogs_nas_nsag_information_t;

/* 9.11.3.91 Priority indicator
 * O TV 1 */
typedef struct ogs_nas_priority_indicator_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t mps_indicator:3;)
} __attribute__ ((packed)) ogs_nas_priority_indicator_t;

/* 9.11.4.1 5GSM capability
 * O TLV 3-15 */
typedef struct ogs_nas_5gsm_capability_s {
    uint8_t length;
ED5(uint8_t transfer_of_port_management_information_containers:1;,
    uint8_t supported_atsss_steering_functionalities_and_steering_modes:4;,
    uint8_t ethernet_pdn_type_in_s1_mode:1;,
    uint8_t multi_homed_ipv6_pdu_session:1;,
    uint8_t reflective_QoS:1;)
ED2(uint8_t spare1:7;,
    uint8_t access_performance_measurements_per_qos_flow_rule:1;)
    uint8_t spare2[11];
} __attribute__ ((packed)) ogs_nas_5gsm_capability_t;

/* 9.11.4.2 5GSM cause
 * O TV 2 */

/* Annex B (informative): Cause values for 5GS session management
 * B.1 Causes related to nature of request */
#define OGS_5GSM_CAUSE_OPERATOR_DETERMINED_BARRING 8
#define OGS_5GSM_CAUSE_INSUFFICIENT_RESOURCES 26
#define OGS_5GSM_CAUSE_MISSING_OR_UNKNOWN_DNN 27
#define OGS_5GSM_CAUSE_UNKNOWN_PDU_SESSION_TYPE 28
#define OGS_5GSM_CAUSE_USER_AUTHENTICATION_OR_AUTHORIZATION_FAILED 29
#define OGS_5GSM_CAUSE_REQUEST_REJECTED_UNSPECIFIED 31
#define OGS_5GSM_CAUSE_SERVICE_OPTION_NOT_SUPPORTED 32
#define OGS_5GSM_CAUSE_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED 33
#define OGS_5GSM_CAUSE_PTI_ALREADY_IN_USE 35
#define OGS_5GSM_CAUSE_REGULAR_DEACTIVATION 36
#define OGS_5GSM_CAUSE_NETWORK_FAILURE 38
#define OGS_5GSM_CAUSE_REACTIVATION_REQUESTED 39
#define OGS_5GSM_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION 41
#define OGS_5GSM_CAUSE_SYNTACTICAL_ERROR_IN_THE_TFT_OPERATION 42
#define OGS_5GSM_CAUSE_INVALID_PDU_SESSION_IDENTITY 43
#define OGS_5GSM_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTERS 44
#define OGS_5GSM_CAUSE_SYNTACTICAL_ERROR_IN_PACKET_FILTERS 45
#define OGS_5GSM_CAUSE_OUT_OF_LADN_SERVICE_AREA  46
#define OGS_5GSM_CAUSE_PTI_MISMATCH 47
#define OGS_5GSM_CAUSE_PDU_SESSION_TYPE_IPV4_ONLY_ALLOWED 50
#define OGS_5GSM_CAUSE_PDU_SESSION_TYPE_IPV6_ONLY_ALLOWED 51
#define OGS_5GSM_CAUSE_PDU_SESSION_DOES_NOT_EXIST 54
#define OGS_5GSM_CAUSE_PDU_SESSION_TYPE_IPV4V6_ONLY_ALLOWED 57
#define OGS_5GSM_CAUSE_PDU_SESSION_TYPE_UNSTRUCTURED_ONLY_ALLOWED 58
#define OGS_5GSM_CAUSE_UNSUPPORTED_5QI_VALUE 59
#define OGS_5GSM_CAUSE_PDU_SESSION_TYPE_ETHERNET_ONLY_ALLOWED 61
#define OGS_5GSM_CAUSE_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE_AND_DNN 67
#define OGS_5GSM_CAUSE_NOT_SUPPORTED_SSC_MODE 68
#define OGS_5GSM_CAUSE_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE 69
#define OGS_5GSM_CAUSE_MISSING_OR_UNKNOWN_DNN_IN_A_SLICE 70
#define OGS_5GSM_CAUSE_INVALID_PTI_VALUE 81
#define OGS_5GSM_CAUSE_MAXIMUM_DATA_RATE_PER_UE_FOR_USER_PLANE_INTEGRITY_PROTECTION_IS_TOO_LOW 82
#define OGS_5GSM_CAUSE_SEMANTIC_ERROR_IN_THE_QOS_OPERATION 83
#define OGS_5GSM_CAUSE_SYNTACTICAL_ERROR_IN_THE_QOS_OPERATION 84
#define OGS_5GSM_CAUSE_INVALID_MAPPED_EPS_BEARER_IDENTITY 85

/* B.2 Protocol errors (e.g., unknown message) */
#define OGS_5GSM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE 95
#define OGS_5GSM_CAUSE_INVALID_MANDATORY_INFORMATION 96
#define OGS_5GSM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED 97
#define OGS_5GSM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE 98
#define OGS_5GSM_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED 99
#define OGS_5GSM_CAUSE_CONDITIONAL_IE_ERROR 100
#define OGS_5GSM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE 101
#define OGS_5GSM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED 111
typedef uint8_t ogs_nas_5gsm_cause_t;

/* 9.11.4.21 5GSM congestion re-attempt indicator
 * O TLV 3 */
typedef struct ogs_nas_5gsm_congestion_re_attempt_indicator_s {
    uint8_t length;
ED3(uint8_t spare:6;,
    uint8_t current_access_type_back_off_timer:1;,
    uint8_t all_plmns_back_off_timer:1;)
} __attribute__ ((packed)) ogs_nas_5gsm_congestion_re_attempt_indicator_t;

/* 9.11.4.3 Always-on PDU session indication
 * O TV 1 */
typedef struct ogs_nas_always_on_pdu_session_indication_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t always_on_pdu_session_indication:1;)
} __attribute__ ((packed)) ogs_nas_always_on_pdu_session_indication_t;

/* 9.11.4.4 Always-on PDU session requested
 * O TV 1 */
typedef struct ogs_nas_always_on_pdu_session_requested_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t always_on_pdu_session_requested:1;)
} __attribute__ ((packed)) ogs_nas_always_on_pdu_session_requested_t;

/* 9.11.4.5 Allowed SSC mode
 * O TV 1 */
typedef struct ogs_nas_allowed_ssc_mode_s {
ED5(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t ssc3:1;,
    uint8_t ssc2:1;,
    uint8_t ssc1:1;)
} __attribute__ ((packed)) ogs_nas_allowed_ssc_mode_t;

/* 9.11.4.7 Integrity protection maximum data rate
 * M V 2 */
typedef struct ogs_nas_integrity_protection_maximum_data_rate_s {
    uint8_t ul;
    uint8_t dl;
} __attribute__ ((packed)) ogs_nas_integrity_protection_maximum_data_rate_t;

/* 9.11.4.8 Mapped EPS bearer contexts
 * O TLV-E 7-65535 */
typedef struct ogs_nas_mapped_eps_bearer_contexts_s {
    uint16_t length;
    void *buffer;
} __attribute__ ((packed)) ogs_nas_mapped_eps_bearer_contexts_t;

/* 9.11.4.9 Maximum number of supported packet filters
 * O TV 3 */
typedef struct ogs_nas_maximum_number_of_supported_packet_filters_s {
    uint8_t max1;
ED2(uint8_t max2:3;,
    uint8_t spare:5;)
} __attribute__ ((packed)) ogs_nas_maximum_number_of_supported_packet_filters_t;

/* 9.11.4.11 PDU session type
 * O TV 1 */
typedef struct ogs_nas_pdu_session_type_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t value:3;)
} __attribute__ ((packed)) ogs_nas_pdu_session_type_t;

/* 9.11.4.12 QoS flow descriptions
 * O TLV-E 6-65535 */
typedef struct ogs_nas_qos_flow_parameter_s {
#define OGS_NAX_QOS_FLOW_PARAMETER_ID_5QI                   1
#define OGS_NAX_QOS_FLOW_PARAMETER_ID_GFBR_UPLINK           2
#define OGS_NAX_QOS_FLOW_PARAMETER_ID_GFBR_DOWNLINK         3
#define OGS_NAX_QOS_FLOW_PARAMETER_ID_MFBR_UPLINK           4
#define OGS_NAX_QOS_FLOW_PARAMETER_ID_MFBR_DOWNLINK         5
#define OGS_NAX_QOS_FLOW_PARAMETER_ID_AVERAGING_WINDOW      6
#define OGS_NAX_QOS_FLOW_PARAMETER_ID_EPS_BEARER_IDENTITY   7
    uint8_t identifier;
    uint8_t len;
    union {
        uint8_t qos_index;
        ogs_nas_bitrate_t br;
    };
} ogs_nas_qos_flow_parameter_t;

#define OGS_NAS_MAX_NUM_OF_QOS_FLOW_DESCRIPTION 8
#define OGS_NAS_MAX_NUM_OF_QOS_FLOW_PARAMETER 8
typedef struct ogs_nas_qos_flow_description_s {
ED2(uint8_t spare1:2;,
    uint8_t identifier:6;)
#define OGS_NAS_CREATE_NEW_QOS_FLOW_DESCRIPTION 1
#define OGS_NAS_DELETE_NEW_QOS_FLOW_DESCRIPTION 2
#define OGS_NAS_MODIFY_NEW_QOS_FLOW_DESCRIPTION 3
ED2(uint8_t code:3;,
    uint8_t spare2:5;)
ED3(uint8_t spare3:1;,
/*
 * For the "create new QoS flow description" operation,
 * the E bit is encoded as follows:
 *   0 reserved
 *   1 parameters list is included
 *
 * For the "Delete existing QoS flow description" operation,
 *   0 parameters list is not included
 *   1 reserved
 *
 * For the "modify existing QoS flow description" operation,
 *   0 extension of previously provided parameters
 *   1 replacement of all previously provided parameters
 */
    uint8_t E_bit:1;,
    uint8_t num_of_parameter:6;)

    ogs_nas_qos_flow_parameter_t param[OGS_NAS_MAX_NUM_OF_QOS_FLOW_PARAMETER];
} ogs_nas_qos_flow_description_t;

#define OGS_NAS_MAX_QOS_FLOW_DESCRIPTIONS_LEN 65535
typedef struct ogs_nas_qos_flow_descriptions_s {
    uint16_t length;
    void *buffer;
} ogs_nas_qos_flow_descriptions_t;

int ogs_nas_build_qos_flow_descriptions(
    ogs_nas_qos_flow_descriptions_t *flow_descriptions,
    ogs_nas_qos_flow_description_t *flow_description,
    int num_of_flow_description);

int ogs_nas_parse_qos_flow_descriptions(
        ogs_nas_qos_flow_description_t *description,
        ogs_nas_qos_flow_descriptions_t *descriptions);

/* 9.11.4.13 QoS rules
 * M LV-E 6-65535 */
#define OGS_NAS_MAX_NUM_OF_QOS_RULE 8
#define OGS_NAS_MAX_NUM_OF_PACKET_FILTER_COMPONENT 16
typedef struct ogs_nas_qos_rule_s {
    uint8_t identifier;
    uint16_t length;
    union {
        struct {
#define OGS_NAS_QOS_CODE_CREATE_NEW_QOS_RULE 1
#define OGS_NAS_QOS_CODE_DELETE_EXISTING_QOS_RULE 2
#define OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS 3
#define OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_REPLACE_ALL_PACKET_FILTERS 4
#define OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_DELETE_PACKET_FILTERS 5
#define OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_WITHOUT_MODIFYING_PACKET_FILTERS 6
        ED3(uint8_t code:3;,
            uint8_t DQR_bit:1;,
            uint8_t num_of_packet_filter:4;)
        };
        uint8_t flags;
    };
    struct {
        union {
            struct {
            ED3(uint8_t spare:2;,
#define OGS_NAS_QOS_DIRECTION_DOWNLINK 1
#define OGS_NAS_QOS_DIRECTION_UPLINK 2
#define OGS_NAS_QOS_DIRECTION_BIDIRECTIONAL 3
                uint8_t direction:2;,
                uint8_t identifier:4;)
            };
            uint8_t flags;
        };
        ogs_pf_content_t content;
    } pf[OGS_MAX_NUM_OF_FLOW_IN_NAS];

    uint8_t precedence;
    union {
        struct {
        ED3(uint8_t spare:1;,
            uint8_t segregation:1;,
            uint8_t identifier:6;)
        };
        uint8_t flags;
    } flow;
} ogs_nas_qos_rule_t;

#define OGS_NAS_MAX_QOS_RULES_LEN 65535
typedef struct ogs_nas_qos_rules_s {
    uint16_t length;
    void *buffer;
} __attribute__ ((packed)) ogs_nas_qos_rules_t;

int ogs_nas_build_qos_rules(ogs_nas_qos_rules_t *rules,
        ogs_nas_qos_rule_t *rule, int num_of_rule);
int ogs_nas_parse_qos_rules(
        ogs_nas_qos_rule_t *rule, ogs_nas_qos_rules_t *rules);

/* 9.11.4.15 SM PDU DN request container
 * O TLV 3-255 */
#define OGS_NAX_MAX_SM_PDU_DN_REQUESTER_CONTAINER 255
typedef struct ogs_nas_sm_pdu_dn_request_container_s {
    uint8_t length;
    uint8_t buffer[OGS_NAX_MAX_SM_PDU_DN_REQUESTER_CONTAINER];
} ogs_nas_sm_pdu_dn_request_container_t;

/* 9.11.4.16 SSC mode
 * O TV 1 */
#define OGS_NAS_SSC_MODE_1 1
#define OGS_NAS_SSC_MODE_2 2
#define OGS_NAS_SSC_MODE_3 3
/* 4(unused) shall be interpreted as "SSC mode 1", if received by the network */
/* 5(unused) shall be interpreted as "SSC mode 2", if received by the network */
/* 6(unused) shall be interpreted as "SSC mode 3", if received by the network */
typedef struct ogs_nas_ssc_mode_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t value:3;)
} __attribute__ ((packed)) ogs_nas_ssc_mode_t;

/* 9.11.4.18 5GSM network feature support
 * O TLV 3-15 */
typedef struct ogs_nas_5gsm_network_feature_support_s {
    uint8_t length;
ED2(uint8_t spare1:7;,
    uint8_t ethernet_pdn_type_in_s1_mode:1;)
    uint8_t spare2[12];
} __attribute__ ((packed)) ogs_nas_5gsm_network_feature_support_t;

/* 9.11.4.22 ATSSS container
 * O TLV-E 3-65535 */
typedef struct ogs_nas_atsss_container_s {
    uint16_t length;
    void *buffer;
} ogs_nas_atsss_container_t;

/* 9.9.4.25 Release assistance indication
 * O TV 1
 *
 * Release assistance indication value
 *
 * Downlink data expected (DDX)
 *
 * Bits
 * 0 0 No information regarding DDX is conveyed by the information element.
 *     If received it shall be interpreted as 'neither value "01" nor "10"
 *     applies
 * 0 1 No further uplink or downlink data transmission subsequent to the
 *     uplink data transmission is expected
 * 1 0 Only a single downlink data transmission and no further uplink data
 *     transmission subsequent to the uplink data transmission is expected
 * 1 1 reserved */
typedef struct ogs_nas_release_assistance_indication_s {
ED3(uint8_t type:4;,
    uint8_t spare:2;,
    uint8_t value:2;)
} __attribute__ ((packed)) ogs_nas_release_assistance_indication_t;

/* 9.11.4.24 IP header compression configuration
 * O TLV 5-257 */
typedef struct ogs_nas_ip_header_compression_configuration_s {
    uint8_t length;
    uint8_t spare[255];
} __attribute__ ((packed)) ogs_nas_ip_header_compression_configuration_t;

/* 9.11.4.25 DS-TT Ethernet port MAC address
 * O TLV 8 */
#define OGS_NAX_MAX_EHTERNET_MAC_ADDRESS_LEN 6
typedef struct ogs_nas_ds_tt_ethernet_port_mac_address_s {
    uint8_t length;
    uint8_t buffer[OGS_NAX_MAX_EHTERNET_MAC_ADDRESS_LEN];
} ogs_nas_ds_tt_ethernet_port_mac_address_t;

/* 9.11.4.26 UE-DS-TT residence time
 * O TLV 10 */
#define OGS_NAS_MAX_UE_DS_TT_REDISENCE_TIME_LEN 8
typedef struct ogs_nas_ue_ds_tt_residence_time_s {
    uint8_t length;
    uint8_t buffer[OGS_NAS_MAX_UE_DS_TT_REDISENCE_TIME_LEN];
} ogs_nas_ue_ds_tt_residence_time_t;

/* 9.11.4.27 Port management information container
 * O TLV-E 4-65535 */
typedef struct ogs_nas_port_management_information_container_s {
    uint16_t length;
    void *buffer;
} ogs_nas_port_management_information_container_t;

/* 9.11.4.28 Ethernet header compression configuration
 * O TLV 3 */
typedef struct ogs_nas_ethernet_header_compression_configuration_s {
    uint8_t length;
ED2(uint8_t spare:6;,
    uint8_t cid_length:2;)
} __attribute__ ((packed)) ogs_nas_ethernet_header_compression_configuration_t;

/* 9.11.4.30 Requested MBS container
 * O TLV-E 8-65538 */
typedef struct ogs_nas_requested_mbs_container_s {
    uint16_t length;
    void *buffer;
} __attribute__ ((packed)) ogs_nas_requested_mbs_container_t;

/* 9.11.4.31 Received MBS container
 * O TLV-E 9-65538 */
typedef struct ogs_nas_received_mbs_container_s {
    uint8_t length;
    void *buffer;
} __attribute__ ((packed)) ogs_nas_received_mbs_container_t;

/* 9.11.4.32 PDU session pair ID
 * O TLV 3 */
typedef struct ogs_nas_pdu_session_pair_id_s {
    uint8_t length;
    uint8_t value;
} __attribute__ ((packed)) ogs_nas_pdu_session_pair_id_t;

/* 9.11.4.33 RSN
 * O TLV 3 */
typedef struct ogs_nas_rsn_s {
    uint8_t length;
    uint8_t value;
} __attribute__ ((packed)) ogs_nas_rsn_t;

#ifdef __cplusplus
}
#endif

#endif /* OGS_NAS_5GS_TYPES_H */
