#ifndef _GTPV2C_TYPES_H__
#define _GTPV2C_TYPES_H__

#include "3gpp_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 9.9.2.0A Device properties
 * See subclause 10.5.7.8 in 3GPP TS 24.008 [13].
 * O TV 1 */
typedef struct _nas_device_properties_t {
ED3(c_uint8_t type:4;,
    c_uint8_t spare:3;,
    c_uint8_t low_priority:1;)
} __attribute__ ((packed)) nas_device_properties_t;

/* 9.9.2.2 Location area identification
 * See subclause 10.5.1.3 in 3GPP TS 24.008 [13]
 * O TV 6 */
typedef struct _nas_location_area_identification_t {
    nas_plmn_t plmn;
    c_uint16_t lac;
} __attribute__ ((packed)) nas_location_area_identification_t;

/* 9.9.2.3 Mobile identity
 * See subclause 10.5.1.4 in 3GPP TS 24.008 [13].
 * O TLV 7-10 */
#define GTPV2C_MOBILE_IDENTITY_NONE 0
#define GTPV2C_MOBILE_IDENTITY_IMSI 1
#define GTPV2C_MOBILE_IDENTITY_IMEI 2
#define GTPV2C_MOBILE_IDENTITY_IMEISV 3
#define GTPV2C_MOBILE_IDENTITY_TMSI 4
#define GTPV2C_MOBILE_IDENTITY_TMGI 5
#define GTPV2C_MOBILE_IDENTITY_GUTI 6
typedef struct _nas_mobile_identity_imsi {
ED3(c_uint8_t digit1:4;,
    c_uint8_t odd_even:1;,
    c_uint8_t type_of_identity:3;)
ED2(c_uint8_t digit3:4;,
    c_uint8_t digit2:4;)
ED2(c_uint8_t digit5:4;,
    c_uint8_t digit4:4;)
ED2(c_uint8_t digit7:4;,
    c_uint8_t digit6:4;)
ED2(c_uint8_t digit9:4;,
    c_uint8_t digit8:4;)
ED2(c_uint8_t digit11:4;,
    c_uint8_t digit10:4;)
ED2(c_uint8_t digit13:4;,
    c_uint8_t digit12:4;)
ED2(c_uint8_t digit15:4;,
    c_uint8_t digit14:4;)
} __attribute__ ((packed)) nas_mobile_identity_imsi_t;

typedef struct _nas_mobile_identity_tmsi {
ED3(c_uint8_t spare:4;,
    c_uint8_t odd_even:1;,
    c_uint8_t type_of_identity:3;)
    c_uint32_t tmsi;
} __attribute__ ((packed)) nas_mobile_identity_tmsi_t;

typedef struct _nas_mobile_identity_tmgi {
ED5(c_uint8_t spare:2;,
    c_uint8_t mbms_session_id:1;,
    c_uint8_t mcc_mnc:1;,
    c_uint8_t odd_even:1;,
    c_uint8_t type_of_identity:3;)
    c_uint8_t mbms_servicec_id[3];
    nas_plmn_t plmn;
    c_uint8_t mbms_session_identity;
} __attribute__ ((packed)) nas_mobile_identity_tmgi_t;

typedef struct _nas_mobile_identity_t {
    c_uint8_t length;
    union {
        nas_mobile_identity_imsi_t imsi;
        nas_mobile_identity_tmsi_t tmsi;
        nas_mobile_identity_tmgi_t tmgi;
    };
} nas_mobile_identity_t;

/* 9.9.2.4 Mobile station classmark 2
 * See subclause 10.5.1.6 in 3GPP TS 24.008
 * O TLV 5 */
#define GTPV2C_MS_CLASSMARK_2_REVISION_GSM_PHASE1 0
#define GTPV2C_MS_CLASSMARK_2_REVISION_GSM_PHASE2 1
#define GTPV2C_MS_CLASSMARK_2_REVISION_R99 2
#define GTPV2C_MS_CLASSMARK_2_REVISION_RESERVED 2

#define GTPV2C_MS_CLASSMARK_2_RF_CLASS1 0
#define GTPV2C_MS_CLASSMARK_2_RF_CLASS2 1
#define GTPV2C_MS_CLASSMARK_2_RF_CLASS3 2
#define GTPV2C_MS_CLASSMARK_2_RF_CLASS4 3
#define GTPV2C_MS_CLASSMARK_2_RF_CLASS5 4
#define GTPV2C_MS_CLASSMARK_2_RF_IRRELEVANT 7
typedef struct nas_mobile_station_classmark_2_t {
    c_uint8_t length;
ED5(c_uint8_t spare1:1;,
    c_uint8_t revision_level:2;, 
    c_uint8_t es_ind:1;,
    c_uint8_t a5_1:1;,
    c_uint8_t rf_power_capability:3;)
ED7(c_uint8_t spare:1;,
    c_uint8_t ps_capa:1;,
    c_uint8_t ss_screen_indicator:2;,
    c_uint8_t sm_capabi:1;,
    c_uint8_t vbs:1;,
    c_uint8_t vgcs:1;,
    c_uint8_t fc:1;)
ED8(c_uint8_t cm3:1;,
    c_uint8_t spare2:1;,
    c_uint8_t lcsva_cap:1;,
    c_uint8_t ucs2:1;,
    c_uint8_t solsa:1;,
    c_uint8_t cmsp:1;,
    c_uint8_t a5_3:1;,
    c_uint8_t a5_2:1;)
} __attribute__ ((packed)) nas_mobile_station_classmark_2_t;

/*9.9.2.5 Mobile station classmark 3
 * See subclause 10.5.1.7 in 3GPP TS 24.008 [13].
 * O TLV 2-34 */
typedef struct _nas_mobile_station_classmark_3_t {
    c_uint8_t length;
    c_uint8_t todo[34];
} __attribute__ ((packed)) nas_mobile_station_classmark_3_t;

/* 9.9.2.8 PLMN list
 * See subclause 10.5.1.13 in 3GPP TS 24.008 [13].
 * O TLV 5-47 */
#define GTPV2C_MAX_PLMN 15
typedef struct _nas_plmn_list_t {
    c_uint8_t length;
    nas_plmn_t plmn[GTPV2C_MAX_PLMN];
} __attribute__ ((packed)) nas_plmn_list_t;

/* 9.9.2.10 Supported codec list
 * See subclause 10.5.4.32 in 3GPP TS 24.008 [13].
 * O TLV 5-n */
typedef struct _nas_supported_codec_item_t {
    c_uint8_t system_identification;
    c_uint8_t length_of_bitmap;
    c_uint16_t codec_bitmap;
} __attribute__ ((packed)) nas_supported_codec_item_t;

#define GTPV2C_MAX_SUPPORTED_CODECS 8
typedef struct _nas_supported_codec_list_t {
    c_uint8_t length;
    nas_supported_codec_item_t item[GTPV2C_MAX_SUPPORTED_CODECS];
} __attribute__ ((packed)) nas_supported_codec_list_t;

/* 9.9.3.0A Additional update result
 * O TV 1  */
typedef struct _nas_additional_update_result_t {
ED3(c_uint8_t type:4;,
    c_uint8_t spare:2;,
    c_uint8_t additional_update_result_value:2;)
} __attribute__ ((packed)) nas_additional_update_result_t;

/* 9.9.3.0B Additional update type
 * O TV 1 */
#define GTPV2C_ADDITIONAL_UPDATE_TYPE_CIOT_NONE 0
#define GTPV2C_ADDITIONAL_UPDATE_TYPE_CIOT_CONTROL_PLANE 1
#define GTPV2C_ADDITIONAL_UPDATE_TYPE_CIOT_DATA_PLANE 2
#define GTPV2C_ADDITIONAL_UPDATE_TYPE_CIOT_RESERVED 3
typedef struct _nas_additional_update_type_t {
ED4(c_uint8_t type:4;,
    c_uint8_t pnb_ciot:2;,
    c_uint8_t saf:1;,
    c_uint8_t autv:1;)
} __attribute__ ((packed)) nas_additional_update_type_t;

/* 9.9.3.1 Authentication failure parameter
 * See subclause 10.5.3.2.2 in 3GPP TS 24.008 [13].
 * O TLV 16 */
typedef struct _nas_authentication_failure_parameter_t {
    c_uint8_t length;
    c_uint8_t parameter[14];
} __attribute__ ((packed)) nas_authentication_failure_parameter_t;

/* 9.9.3.2 Authentication parameter AUTN
 * See subclause 10.5.3.1.1 in 3GPP TS 24.008 [13].
 * M LV 17 */
typedef struct _nas_authentication_parameter_autn_t {
    c_uint8_t length;
    c_uint8_t autn[MAX_AUTN_LEN];
} nas_authentication_parameter_autn_t;

/* 9.9.3.3 Authentication parameter RAND
 * See subclause 10.5.3.1 in 3GPP TS 24.008 [13].
 * M V 16 */
typedef struct _nas_authentication_parameter_rand_t {
    c_uint8_t rand[MAX_RAND_LEN];
} nas_authentication_parameter_rand_t;

/* 9.9.3.4 Authentication response parameter
 * M LV 5-17 */
typedef struct _nas_authentication_response_parameter_t {
    c_uint8_t length;
    c_uint8_t res[MAX_RES_LEN];
} nas_authentication_response_parameter_t;

/* 9.9.3.8 DRX parameter
 * See subclause 10.5.5.6 in 3GPP TS 24.008
 * O TV 3 */
#define GTPV2C_DRX_PARAMETER_TIMER_MAX_1SEC 1
#define GTPV2C_DRX_PARAMETER_TIMER_MAX_2SEC 2
#define GTPV2C_DRX_PARAMETER_TIMER_MAX_4SEC 3
#define GTPV2C_DRX_PARAMETER_TIMER_MAX_8SEC 4
#define GTPV2C_DRX_PARAMETER_TIMER_MAX_16SEC 5
#define GTPV2C_DRX_PARAMETER_TIMER_MAX_32SEC 6
#define GTPV2C_DRX_PARAMETER_TIMER_MAX_64SEC 7

#define GTPV2C_DRX_PARAMETER_COEFFICIENT_T_NOT_SPECIFIED_BY_MS 0
#define GTPV2C_DRX_PARAMETER_COEFFICIENT_6_T_32 6
#define GTPV2C_DRX_PARAMETER_COEFFICIENT_7_T_64 7
#define GTPV2C_DRX_PARAMETER_COEFFICIENT_8_T_128 8
#define GTPV2C_DRX_PARAMETER_COEFFICIENT_9_T_256 9
typedef struct _nas_drx_parameter_t {
    c_uint8_t split_pg_cycle_code;
ED3(c_uint8_t cn_specific_drx_cycle_length_coefficient_and_drx_value_for_s1_mode:4;,
    c_uint8_t split_on_ccch:1;,
    c_uint8_t non_DRX_timer:3;)
} __attribute__ ((packed)) nas_drx_parameter_t;

/* 9.9.3.9 EMM cause
 * O TV 2 */
#define GTPV2C_EMM_CAUSE_IMSI_UNKNOWN_IN_HSS 0b00000010
#define GTPV2C_EMM_CAUSE_ILLEGAL_UE 0b00000011
#define GTPV2C_EMM_CAUSE_IMEI_NOT_ACCEPTED 0b00000101
#define GTPV2C_EMM_CAUSE_ILLEGAL_ME 0b00000110
#define GTPV2C_EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED 0b00000111
#define GTPV2C_EMM_CAUSE_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED 0b00001000
#define GTPV2C_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK 0b00001001
#define GTPV2C_EMM_CAUSE_IMPLICITLY_DETACHED 0b00001010
#define GTPV2C_EMM_CAUSE_PLMN_NOT_ALLOWED 0b00001011
#define GTPV2C_EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED 0b00001100
#define GTPV2C_EMM_CAUSE_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA 0b00001101
#define GTPV2C_EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED_IN_THIS_PLMN 0b00001110
#define GTPV2C_EMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA 0b00001111
#define GTPV2C_EMM_CAUSE_MSC_TEMPORARILY_NOT_REACHABLE 0b00010000
#define GTPV2C_EMM_CAUSE_NETWORK_FAILURE 0b00010001
#define GTPV2C_EMM_CAUSE_CS_DOMAIN_NOT_AVAILABLE 0b00010010
#define GTPV2C_EMM_CAUSE_ESM_FAILURE 0b00010011
#define GTPV2C_EMM_CAUSE_MAC_FAILURE 0b00010100
#define GTPV2C_EMM_CAUSE_SYNCH_FAILURE 0b00010101
#define GTPV2C_EMM_CAUSE_CONGESTION 0b00010110
#define GTPV2C_EMM_CAUSE_UE_SECURITY_CAPABILITIES_MISMATCH 0b00010111
#define GTPV2C_EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED 0b00011000
#define GTPV2C_EMM_CAUSE_NOT_AUTHORIZED_FOR_THIS_CSG 0b00011001
#define GTPV2C_EMM_CAUSE_NON_EPS_AUTHENTICATION_UNACCEPTABLE 0b00011010
#define GTPV2C_EMM_CAUSE_REQUESTED_SERVICE_OPTION_NOT_AUTHORIZED_IN_THIS_PLMN 0b00100011
#define GTPV2C_EMM_CAUSE_CS_SERVICE_TEMPORARILY_NOT_AVAILABLE 0b00100111
#define GTPV2C_EMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED 0b00101000
#define GTPV2C_EMM_CAUSE_SEVERE_NETWORK_FAILURE 0b00101010
#define GTPV2C_EMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE 0b01011111
#define GTPV2C_EMM_CAUSE_INVALID_MANDATORY_INFORMATION 0b01100000
#define GTPV2C_EMM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED 0b01100001
#define GTPV2C_EMM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE 0b01100010
#define GTPV2C_EMM_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED 0b01100011
#define GTPV2C_EMM_CAUSE_CONDITIONAL_IE_ERROR 0b01100100
#define GTPV2C_EMM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_THE_PROTOCOL_STATE 0b01100101
#define GTPV2C_EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED 0b01101111
typedef c_uint8_t nas_emm_cause_t;

/* 9.9.3.10 * EPS attach result
 * M V 1/2 */
#define GTPV2C_ATTACH_RESULT_EPS_ONLY 1
#define GTPV2C_ATTACH_RESULT_COMBINED_EPS_IMSI_ATTACH 2
typedef struct _nas_eps_attach_result_t {
ED2(c_uint8_t spare:5;,
    c_uint8_t result:3;)
} __attribute__ ((packed)) nas_eps_attach_result_t;

/* 9.9.3.11 EPS attach type
 * M V 1/2
 * 9.9.3.21 NAS key set identifier 
 * M V 1/2 */
#define GTPV2C_KEY_SET_IDENTIFIER_NATIVE 0
#define GTPV2C_KEY_SET_IDENTIFIER_MAPPED 1
#define GTPV2C_KEY_SET_IDENTIFIER_NOT_AVAILABLE 0x111

#define GTPV2C_ATTACH_TYPE_EPS_ATTACH 1
#define GTPV2C_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTAACH 2
#define GTPV2C_ATTACH_TYPE_EPS_ERMERGENCY_ATTCH 3
#define GTPV2C_ATTACH_TYPE_RESERVED 4
typedef struct _nas_eps_attach_type_t {
ED4(c_uint8_t tsc:1;,
    c_uint8_t nas_key_set_identifier:3;,
    c_uint8_t spare:1;,
    c_uint8_t attach_type:3;)
} __attribute__ ((packed)) nas_eps_attach_type_t;

/* 9.9.3.12 EPS mobile identity
 * M LV  5-12 */
#define GTPV2C_EPS_MOBILE_IDENTITY_IMSI 1
#define GTPV2C_EPS_MOBILE_IDENTITY_GUTI 6
#define GTPV2C_EPS_MOBILE_IDENTITY_IMEI 3

#define GTPV2C_EPS_MOBILE_IDENTITY_EVEN 0
#define GTPV2C_EPS_MOBILE_IDENTITY_ODD 1
typedef struct _nas_eps_mobile_identity_guti_t {
ED3(c_uint8_t spare:4;,
    c_uint8_t odd_even:1;,
    c_uint8_t type_of_identity:3;)
    nas_plmn_t plmn;
    c_uint16_t mme_group_id;
    c_uint8_t mme_code;
    c_uint32_t m_tmsi;
} __attribute__ ((packed)) nas_eps_mobile_identity_guti_t;

typedef nas_mobile_identity_imsi_t nas_eps_mobile_identity_imsi_t;
typedef nas_eps_mobile_identity_imsi_t nas_eps_mobile_identity_imei_t;

typedef struct _nas_eps_mobile_identity_t {
    c_uint8_t length;
    union {
        nas_eps_mobile_identity_imsi_t imsi;
        nas_eps_mobile_identity_guti_t guti;
        nas_eps_mobile_identity_imei_t imei;
    };
} __attribute__ ((packed)) nas_eps_mobile_identity_t;

/* 9.9.3.12A EPS network feature support 
 * O TLV 3 */
typedef struct _nas_eps_network_feature_support_t {
    c_uint8_t length;
ED7(c_uint8_t cp_ciot:1;,
    c_uint8_t erw_opdn:1;,
    c_uint8_t esr_ps:1;,
    c_uint8_t cs_lcs:2;,
    c_uint8_t epc_lcs:1;,
    c_uint8_t emc_bs:1;,
    c_uint8_t ims_vops:1;)
ED5(c_uint8_t spare:4;,
    c_uint8_t e_pco:1;,
    c_uint8_t hc_cp_ciot:1;,
    c_uint8_t s1_u_data:1;,
    c_uint8_t up_ciot :1;)
} __attribute__ ((packed)) nas_eps_network_feature_support_t;

/* 9.9.3.15 ESM message container
 * M LV-E 5-n */
typedef struct _nas_esm_message_container_t {
    c_uint16_t length;
    c_uint8_t *buffer;
} nas_esm_message_container_t;

/* 9.9.3.16 GPRS timer
 * See subclause 10.5.7.3 in 3GPP TS 24.008 [13].
 * M V 1 or  O TV 2 */
#define GTPV2C_GRPS_TIMER_UNIT_MULTIPLES_OF_2_SS       0
#define GTPV2C_GRPS_TIMER_UNIT_MULTIPLES_OF_1_MM       1
#define GTPV2C_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH    2
#define GTPV2C_GRPS_TIMER_UNIT_DEACTIVATED             7
typedef struct _nas_gprs_timer_t {
ED2(c_uint8_t unit:3;,
    c_uint8_t timer_value:5;)
} __attribute__ ((packed)) nas_gprs_timer_t;

/* 9.9.3.16A GPRS timer 2
 * See subclause 10.5.7.4 in 3GPP TS 24.008 [13].
 * O TLV 3 */
typedef struct _nas_gprs_timer_2_t {
    c_uint8_t length;
    c_uint8_t gprs_timer_2_value;
} __attribute__ ((packed)) nas_gprs_timer_2_t;

/* 9.9.3.16B GPRS timer 3
 * See subclause 10.5.7.4a in 3GPP TS 24.008 [13].
 * O TLV 3 */
#define GTPV2C_GRPS_TIMER_3_UNIT_MULTIPLES_OF_10_MM    0
#define GTPV2C_GRPS_TIMER_3_UNIT_MULTIPLES_OF_1_HH     1
#define GTPV2C_GRPS_TIMER_3_UNIT_MULTIPLES_OF_10_HH    2
#define GTPV2C_GRPS_TIMER_3_UNIT_MULTIPLES_OF_2_SS     3 
#define GTPV2C_GRPS_TIMER_3_UNIT_MULTIPLES_OF_30_SS    4
#define GTPV2C_GRPS_TIMER_3_UNIT_MULTIPLES_OF_1_MM     5
#define GTPV2C_GRPS_TIMER_3_UNIT_MULTIPLES_OF_320_HH   6
#define GTPV2C_GRPS_TIMER_3_UNIT_DEACTIVATED           7

typedef struct _nas_gprs_timer_3_t {
    c_uint8_t length;
ED2(c_uint8_t unit:3;,
    c_uint8_t timer_value:5;)
} __attribute__ ((packed)) nas_gprs_timer_3_t;

/* 9.9.3.17 Identity type 2
 * See subclause 10.5.5.9 in 3GPP TS 24.008 [13].
 * M V 1/2 */
#define GTPV2C_IDENTITY_TYPE_2_IMSI            1
#define GTPV2C_IDENTITY_TYPE_2_IMEI            2
#define GTPV2C_IDENTITY_TYPE_2_IMEISV          3
#define GTPV2C_IDENTITY_TYPE_2_TMSI            4
typedef struct _nas_identity_type_2_t {
ED2(c_uint8_t spare:5;,
    c_uint8_t type_of_identity:3;)
} __attribute__ ((packed)) nas_identity_type_2_t;

/* 9.9.3.18 IMEISV request
 * See subclause 10.5.5.10 in 3GPP TS 24.008 [13].
 * O TV 1 */
typedef struct _nas_imeisv_request_t {
ED3(c_uint8_t type:4;,
    c_uint8_t spare:1;,
    c_uint8_t imeisv_request_value:3;)
} __attribute__ ((packed)) nas_imeisv_request_t;

/* 9.9.3.20 MS network capability
 * See subclause 10.5.5.12 in 3GPP TS 24.008
 * O TLV 4-10 */
#define GTPV2C_MS_NETWORK_CAPABILITY_GEA2 0b01000000
#define GTPV2C_MS_NETWORK_CAPABILITY_GEA3 0b00100000
#define GTPV2C_MS_NETWORK_CAPABILITY_GEA4 0b00010000
#define GTPV2C_MS_NETWORK_CAPABILITY_GEA5 0b00001000
#define GTPV2C_MS_NETWORK_CAPABILITY_GEA6 0b00000100
#define GTPV2C_MS_NETWORK_CAPABILITY_GEA7 0b00000010
typedef struct _nas_ms_network_capability_t {
    c_uint8_t length;
ED7(c_uint8_t gea1:1;,
    c_uint8_t sm_capabilities_via_dedicated_channels:1;,
    c_uint8_t sm_capabilities_via_gprs_channels:1;,
    c_uint8_t ucs2:1;,
    c_uint8_t ss_screening_indicator:2;,
    c_uint8_t solsa_capability:1;,
    c_uint8_t revision_level_indicator:1;)
ED3(c_uint8_t pfc_feature_mode:1;,
    c_uint8_t extended_gea:6;,
    c_uint8_t lcs_va_capability:1;)
ED8(c_uint8_t ps_inter_rat_ho_from_geran_to_utran_iu_mode_capability:1;,
    c_uint8_t ps_inter_rat_ho_from_geran_to_e_utran_s1_mode_capability:1;,
    c_uint8_t emm_combined_procedures_capability:1;,
    c_uint8_t isr_support:1;,
    c_uint8_t srvcc_to_geran_utran_capability:1;,
    c_uint8_t epc_capability:1;,
    c_uint8_t nf_capability:1;,
    c_uint8_t geran_network_sharing_capability:1;)
ED6(c_uint8_t user_plane_integrity_protection_support:1;,
    c_uint8_t gia4:1;,
    c_uint8_t gia5:1;,
    c_uint8_t gia6:1;,
    c_uint8_t gia7:1;,
    c_uint8_t spare:3;)
} __attribute__ ((packed)) nas_ms_network_capability_t;

/* 9.9.3.20A MS network feature support 
 * See subclause 10.5.1.15 in 3GPP TS 24.008 [13].
 * O TV 1 */
typedef struct _nas_ms_network_feature_support_t {
ED3(c_uint8_t type:4;,
    c_uint8_t spare:3;,
    c_uint8_t extended_periodic_timers:1;)
} __attribute__ ((packed)) nas_ms_network_feature_support_t;

/* 9.9.3.21 NAS key set identifier
 * M V 1/2
 * 9.9.2.9 Spare half octet
 * M V 1/2 */
typedef struct _nas_key_set_identifier_t {
ED3(c_uint8_t spare:4;,
    c_uint8_t tsc:1;,
    c_uint8_t nas_key_set_identifier:3;)
} __attribute__ ((packed)) nas_key_set_identifier_t;

/* 9.9.3.23 NAS security algorithms
 * M V 1 */
#define GTPV2C_SECURITY_ALGORITHMS_EIA0        0
#define GTPV2C_SECURITY_ALGORITHMS_128_EIA1    1
#define GTPV2C_SECURITY_ALGORITHMS_128_EIA2    2
#define GTPV2C_SECURITY_ALGORITHMS_128_EIA3    3
#define GTPV2C_SECURITY_ALGORITHMS_EEA0        0
#define GTPV2C_SECURITY_ALGORITHMS_128_EEA1    1
#define GTPV2C_SECURITY_ALGORITHMS_128_EEA2    2
#define GTPV2C_SECURITY_ALGORITHMS_128_EEA3    3
typedef struct _nas_security_algorithms_t {
ED4(c_uint8_t spare1:1;,
    c_uint8_t type_of_ciphering_algorithm:3;,
    c_uint8_t spare2:1;,
    c_uint8_t type_of_integrity_protection_algorithm:3;)
} __attribute__ ((packed)) nas_security_algorithms_t;

/* 9.9.3.24A Network resource identifier container
 * See subclause 10.5.5.31 in 3GPP TS 24.008 [13].
 * O TLV 4 */
typedef struct _nas_network_resource_identifier_container_t {
    c_uint8_t length;
    c_uint8_t nri_container_value1;
ED2(c_uint8_t nri_container_value2:2;,
    c_uint8_t spare:6;)
} __attribute__ ((packed)) nas_network_resource_identifier_container_t;

/* 9.9.3.25 Nonce
 * O TV 5 */
typedef c_uint32_t nas_nonce_t;

/* 9.9.3.26 P-TMSI signature 
 * See subclause 10.5.5.8 in 3GPP TS 24.008
 * O TV 4 */
typedef c_uint32_t nas_p_tmsi_signature_t; /* TV : 4bytes */

/* 9.9.3.26A Extended EMM cause 
 * O TV 1 */
typedef struct _nas_extended_emm_cause_t {
ED4(c_uint8_t type:4;,
    c_uint8_t spare:2;,
    c_uint8_t eps_optimization_info:1;,
    c_uint8_t e_utran_allowed:1;)
} nas_extended_emm_cause_t;

/* 9.9.3.31 TMSI status
 * See subclause 10.5.5.4 in 3GPP TS 24.008 [13]
 * O TV 1 */
typedef struct _nas_tmsi_status_t {
ED3(c_uint8_t type:4;,
    c_uint8_t spare:3;,
    c_uint8_t tmsi_flag:1;)
} __attribute__ ((packed)) nas_tmsi_status_t;

/* 9.9.3.32 Tracking area identity
 * O TV 6 */
typedef struct _nas_tracking_area_identity_t {
    plmn_t plmn_id;
    c_uint16_t tac;
} __attribute__ ((packed)) nas_tracking_area_identity_t;

/* 9.9.3.33 Tracking area identity list
 * M LV 7-97 */
#define GTPV2C_MAX_TRACKING_AREA_IDENTITY 16
#define GTPV2C_TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_NON_CONSECUTIVE_TACS 0
#define GTPV2C_TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_CONSECUTIVE_TACS     1
#define GTPV2C_TRACKING_AREA_IDENTITY_LIST_MANY_PLMNS                    2
typedef struct _nas_tracking_area_identity_type0 {
    nas_plmn_t plmn;
    c_uint16_t tac[GTPV2C_MAX_TRACKING_AREA_IDENTITY];
} __attribute__ ((packed)) nas_tracking_area_identity_type0;

typedef nas_tracking_area_identity_t nas_tracking_area_identity_type1;

typedef struct _nas_tracking_area_identity_type2 {
    nas_tracking_area_identity_type1 tai[GTPV2C_MAX_TRACKING_AREA_IDENTITY];
} __attribute__ ((packed)) nas_tracking_area_identity_type2;

typedef struct nas_tracking_area_identity_list_t {
    c_uint8_t length;
ED3(c_uint8_t spare:1;,
    c_uint8_t type_of_list:2;,
    c_uint8_t number_of_elements:5;)
    union {
        nas_tracking_area_identity_type0 type0;
        nas_tracking_area_identity_type1 type1;
        nas_tracking_area_identity_type2 type2;
    };
} __attribute__ ((packed)) nas_tracking_area_identity_list_t;

/* 9.9.3.34 UE network capability
 * M LV  3-14 */
typedef struct _nas_ue_network_capability_t {
    c_uint8_t length;
    union { 
        struct {
        ED8(c_uint8_t eea0:1;,
            c_uint8_t eea1:1;,
            c_uint8_t eea2:1;,
            c_uint8_t eea3:1;,
            c_uint8_t eea4:1;,
            c_uint8_t eea5:1;,
            c_uint8_t eea6:1;,
            c_uint8_t eea7:1;)
        };
        c_uint8_t eea; 
    };
    union { 
        struct {
        ED8(c_uint8_t eia0:1;,
            c_uint8_t eia1:1;,
            c_uint8_t eia2:1;,
            c_uint8_t eia3:1;,
            c_uint8_t eia4:1;,
            c_uint8_t eia5:1;,
            c_uint8_t eia6:1;,
            c_uint8_t eia7:1;)
        };
        c_uint8_t eia; 
    };
    union { 
        struct {
        ED8(c_uint8_t uea0:1;,
            c_uint8_t uea1:1;,
            c_uint8_t uea2:1;,
            c_uint8_t uea3:1;,
            c_uint8_t uea4:1;,
            c_uint8_t uea5:1;,
            c_uint8_t uea6:1;,
            c_uint8_t uea7:1;)
        };
        c_uint8_t uea; 
    };
    union { 
        struct {
        ED8(c_uint8_t ucs2:1;,
            c_uint8_t uia1:1;,
            c_uint8_t uia2:1;,
            c_uint8_t uia3:1;,
            c_uint8_t uia4:1;,
            c_uint8_t uia5:1;,
            c_uint8_t uia6:1;,
            c_uint8_t uia7:1;)
        };
        c_uint8_t uia; 
    };
ED8(c_uint8_t prose_dd:1;,
    c_uint8_t prose:1;,
    c_uint8_t h_245_ash:1;,
    c_uint8_t acc_csfb:1;,
    c_uint8_t lpp:1;,
    c_uint8_t lcs:1;,
    c_uint8_t srvcc:1;,
    c_uint8_t nf:1;)
ED8(c_uint8_t epco:1;,
    c_uint8_t hc_cp_ciot:1;,
    c_uint8_t erw_opdn:1;,
    c_uint8_t s1u_data:1;,
    c_uint8_t up_ciot:1;,
    c_uint8_t cp_ciot:1;,
    c_uint8_t prose_relay:1;,
    c_uint8_t prose_dc:1;)
ED2(c_uint8_t spare:7;,
    c_uint8_t multiple_drb:1;)
} __attribute__ ((packed)) nas_ue_network_capability_t;

/* 9.9.3.36 UE security capability
 * M LV 3-6 */
typedef struct _nas_ue_security_capability_t {
    c_uint8_t length;
    union { 
        struct {
        ED8(c_uint8_t eea0:1;,
            c_uint8_t eea1:1;,
            c_uint8_t eea2:1;,
            c_uint8_t eea3:1;,
            c_uint8_t eea4:1;,
            c_uint8_t eea5:1;,
            c_uint8_t eea6:1;,
            c_uint8_t eea7:1;)
        };
        c_uint8_t eea; 
    };
    union { 
        struct {
        ED8(c_uint8_t eia0:1;,
            c_uint8_t eia1:1;,
            c_uint8_t eia2:1;,
            c_uint8_t eia3:1;,
            c_uint8_t eia4:1;,
            c_uint8_t eia5:1;,
            c_uint8_t eia6:1;,
            c_uint8_t eia7:1;)
        };
        c_uint8_t eia; 
    };
    union { 
        struct {
        ED8(c_uint8_t uea0:1;,
            c_uint8_t uea1:1;,
            c_uint8_t uea2:1;,
            c_uint8_t uea3:1;,
            c_uint8_t uea4:1;,
            c_uint8_t uea5:1;,
            c_uint8_t uea6:1;,
            c_uint8_t uea7:1;)
        };
        c_uint8_t uea; 
    };
    union { 
        struct {
        ED8(c_uint8_t spare1:1;,
            c_uint8_t uia1:1;,
            c_uint8_t uia2:1;,
            c_uint8_t uia3:1;,
            c_uint8_t uia4:1;,
            c_uint8_t uia5:1;,
            c_uint8_t uia6:1;,
            c_uint8_t uia7:1;)
        };
        c_uint8_t uia; 
    };
    union { 
        struct {
        ED8(c_uint8_t spare2:1;,
            c_uint8_t gea1:1;,
            c_uint8_t gea2:1;,
            c_uint8_t gea3:1;,
            c_uint8_t gea4:1;,
            c_uint8_t gea5:1;,
            c_uint8_t gea6:1;,
            c_uint8_t gea7:1;)
        };
        c_uint8_t gea; 
    };
} __attribute__ ((packed)) nas_ue_security_capability_t;

/* TODO : 9.9.3.37 Emergency number list
 * See subclause 10.5.3.13 in 3GPP TS 24.008 [13].
 * O TLV 5-50 */
typedef struct _nas_emergency_number_list_t {
    c_uint16_t length;
    c_uint8_t todo[50];
} __attribute__ ((packed)) nas_emergency_number_list_t;

/* 9.9.3.44 Voice domain preference and UE's usage setting
 * See subclause 10.5.5.28 in 3GPP TS 24.008 [13].
 * O TLV 3 */
typedef struct _nas_voice_domain_preference_and_ue_usage_setting_t {
    c_uint8_t length;
ED3(c_uint8_t spare:5;,
    c_uint8_t ue_usage_setting:1;, 
    c_uint8_t voice_domain_preference_for_e_utran:2;)
} __attribute__ ((packed)) nas_voice_domain_preference_and_ue_usage_setting_t;

/* 9.9.3.45 GUTI type 
 * O TV 1 */
typedef struct _nas_guti_type_t {
ED3(c_uint8_t type:4;,
    c_uint8_t spare:3;,
    c_uint8_t guti_type:1;)
} __attribute__ ((packed)) nas_guti_type_t;

/* 9.9.3.46 Extended DRX parameters
 * See subclause 10.5.5.32 in 3GPP TS 24.008 [13].
 * O TLV 3 */
typedef struct _nas_extended_drx_parameters_t {
    c_uint8_t length;
ED2(c_uint8_t paging_time_window:4;,
    c_uint8_t e_drx_value:4;)
} __attribute__ ((packed)) nas_extended_drx_parameters_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTPV2C_TYPES_H__ */

