#ifndef _NAS_IES_H__
#define _NAS_IES_H__

#include "core_pkbuf.h"

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

CORE_DECLARE(status_t) nas_decode_device_properties(
    nas_device_properties_t *device_properties, pkbuf_t *pkbuf);

/* 9.9.2.2 Location area identification
 * See subclause 10.5.1.3 in 3GPP TS 24.008 [13]
 * O TV 6 */
typedef struct _nas_location_area_identification_t {
ED2(c_uint8_t mcc_digit2:4;,
    c_uint8_t mcc_digit1:4;)
ED2(c_uint8_t mnc_digit3:4;,
    c_uint8_t mcc_digit3:4;)
ED2(c_uint8_t mnc_digit2:4;,
    c_uint8_t mnc_digit1:4;)
    c_uint16_t lac;
} __attribute__ ((packed)) nas_location_area_identification_t;

CORE_DECLARE(status_t) nas_decode_location_area_identification(
    nas_location_area_identification_t *old_location_area_identification, 
    pkbuf_t *pkbuf);

/* 9.9.2.4 Mobile station classmark 2
 * See subclause 10.5.1.6 in 3GPP TS 24.008
 * O TLV 5 */
#define NAS_MS_CLASSMARK_2_REVISION_GSM_PHASE1 0
#define NAS_MS_CLASSMARK_2_REVISION_GSM_PHASE2 1
#define NAS_MS_CLASSMARK_2_REVISION_R99 2
#define NAS_MS_CLASSMARK_2_REVISION_RESERVED 2

#define NAS_MS_CLASSMARK_2_RF_CLASS1 0
#define NAS_MS_CLASSMARK_2_RF_CLASS2 1
#define NAS_MS_CLASSMARK_2_RF_CLASS3 2
#define NAS_MS_CLASSMARK_2_RF_CLASS4 3
#define NAS_MS_CLASSMARK_2_RF_CLASS5 4
#define NAS_MS_CLASSMARK_2_RF_IRRELEVANT 7
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

CORE_DECLARE(status_t) nas_decode_mobile_station_classmark_2(
    nas_mobile_station_classmark_2_t *mobile_station_classmark_2, 
    pkbuf_t *pkbuf);

/*9.9.2.5 Mobile station classmark 3
 * See subclause 10.5.1.7 in 3GPP TS 24.008 [13].
 * O TLV 2-34 */
typedef struct _nas_mobile_station_classmark_3_t {
    c_uint8_t length;
    /* TODO */
} __attribute__ ((packed)) nas_mobile_station_classmark_3_t;

CORE_DECLARE(status_t) nas_decode_mobile_station_classmark_3(
    nas_mobile_station_classmark_3_t *mobile_station_classmark_3, 
    pkbuf_t *pkbuf);

/* 9.9.2.10 Supported codec list
 * See subclause 10.5.4.32 in 3GPP TS 24.008 [13].
 * O TLV 5-n */
typedef struct _nas_supported_codec_item_t {
    c_uint8_t system_identification;
    c_uint8_t length_of_bitmap;
    c_uint16_t codec_bitmap;
} __attribute__ ((packed)) nas_supported_codec_item_t;

#define NAS_MAX_SUPPORTED_CODECS 4
typedef struct _nas_supported_codec_list_t {
    c_uint8_t length;
    nas_supported_codec_item_t item[NAS_MAX_SUPPORTED_CODECS];
} __attribute__ ((packed)) nas_supported_codec_list_t;

CORE_DECLARE(status_t) nas_decode_supported_codec_list(
    nas_supported_codec_list_t *supported_codecs, pkbuf_t *pkbuf);

/* 9.9.3.0B Additional update type
 * O TV 1 */
#define NAS_ADDITIONAL_UPDATE_TYPE_CIOT_NONE 0
#define NAS_ADDITIONAL_UPDATE_TYPE_CIOT_CONTROL_PLANE 1
#define NAS_ADDITIONAL_UPDATE_TYPE_CIOT_DATA_PLANE 2
#define NAS_ADDITIONAL_UPDATE_TYPE_CIOT_RESERVED 3
typedef struct _nas_additional_update_type_t {
ED4(c_uint8_t type:4;,
    c_uint8_t pnb_ciot:2;,
    c_uint8_t saf:1;,
    c_uint8_t autv:1;)
} __attribute__ ((packed)) nas_additional_update_type_t;

CORE_DECLARE(status_t) nas_decode_additional_update_type(
    nas_additional_update_type_t *additional_update_type, pkbuf_t *pkbuf);

/* 9.9.3.8 DRX parameter
 * See subclause 10.5.5.6 in 3GPP TS 24.008
 * O TV 3 */
#define NAS_DRX_PARAMETER_TIMER_MAX_1SEC 1
#define NAS_DRX_PARAMETER_TIMER_MAX_2SEC 2
#define NAS_DRX_PARAMETER_TIMER_MAX_4SEC 3
#define NAS_DRX_PARAMETER_TIMER_MAX_8SEC 4
#define NAS_DRX_PARAMETER_TIMER_MAX_16SEC 5
#define NAS_DRX_PARAMETER_TIMER_MAX_32SEC 6
#define NAS_DRX_PARAMETER_TIMER_MAX_64SEC 7

#define NAS_DRX_PARAMETER_COEFFICIENT_T_NOT_SPECIFIED_BY_MS 0
#define NAS_DRX_PARAMETER_COEFFICIENT_6_T_32 6
#define NAS_DRX_PARAMETER_COEFFICIENT_7_T_64 7
#define NAS_DRX_PARAMETER_COEFFICIENT_8_T_128 8
#define NAS_DRX_PARAMETER_COEFFICIENT_9_T_256 9
typedef struct _nas_drx_parameter_t {
    c_uint8_t split_pg_cycle_code;
ED3(c_uint8_t cn_specific_drx_cycle_length_coefficient_and_drx_value_for_s1_mode:4;,
    c_uint8_t split_on_ccch:1;,
    c_uint8_t non_DRX_timer:3;)
} __attribute__ ((packed)) nas_drx_parameter_t;

CORE_DECLARE(status_t) nas_decode_drx_parameter(
    nas_drx_parameter_t *drx_parameter, pkbuf_t *pkbuf);

/* 9.9.3.11 EPS attach type
 * M V 1/2
 * 9.9.3.21 NAS key set identifier 
 * M V 1/2 */
#define NAS_KEY_SET_IDENTIFIER_NATIVE 0
#define NAS_KEY_SET_IDENTIFIER_MAPPED 1
#define NAS_KEY_SET_IDENTIFIER_NOT_AVAILABLE 0x111

#define NAS_ATTACH_TYPE_EPS_ATTACH 1
#define NAS_ATTACH_TYPE_COMBINED_EPS_IMSI_ATTAACH 2
#define NAS_ATTACH_TYPE_EPS_ERMERGENCY_ATTCH 3
#define NAS_ATTACH_TYPE_RESERVED 4
typedef struct _nas_attach_info_t {
ED4(c_uint8_t tsc:1;,
    c_uint8_t nas_key_set_identifier:3;,
    c_uint8_t spare:1;,
    c_uint8_t attach_type:3;)
} __attribute__ ((packed)) nas_attach_info_t;

CORE_DECLARE(status_t) nas_decode_attach_info(
    nas_attach_info_t *attach_info, pkbuf_t *pkbuf);

/* 9.9.3.12 EPS mobile identity
 * M LV  5-12 */
#define NAS_EPS_MOBILE_IDENTITY_IMSI 1
#define NAS_EPS_MOBILE_IDENTITY_GUTI 6
#define NAS_EPS_MOBILE_IDENTITY_IMEI 3

#define NAS_EPS_MOBILE_IDENTITY_EVEN 0
#define NAS_EPS_MOBILE_IDENTITY_ODD 1
typedef struct _nas_eps_mobile_identity_guti_t {
ED2(c_uint8_t mcc_digit2:4;,
    c_uint8_t mcc_digit1:4;)
ED2(c_uint8_t mnc_digit3:4;,
    c_uint8_t mcc_digit3:4;)
ED2(c_uint8_t mnc_digit2:4;,
    c_uint8_t mnc_digit1:4;)
    c_uint16_t mme_group_id;
    c_uint8_t mme_code;
    c_uint32_t m_tmsi;
} __attribute__ ((packed)) nas_eps_mobile_identity_guti_t;

typedef struct _nas_eps_mobile_identity_imsi_t {
ED2(c_uint8_t digit2:4;,
    c_uint8_t digit3:4;)
ED2(c_uint8_t digit4:4;,
    c_uint8_t digit5:4;)
ED2(c_uint8_t digit6:4;,
    c_uint8_t digit7:4;)
ED2(c_uint8_t digit8:4;,
    c_uint8_t digit9:4;)
ED2(c_uint8_t digit10:4;,
    c_uint8_t digit11:4;)
ED2(c_uint8_t digit12:4;,
    c_uint8_t digit13:4;)
ED2(c_uint8_t digit14:4;,
    c_uint8_t digit15:4;)
} __attribute__ ((packed)) nas_eps_mobile_identity_imsi_t;

typedef nas_eps_mobile_identity_imsi_t nas_eps_mobile_identity_imei_t;

typedef struct _nas_eps_mobile_identity_t {
    c_uint8_t length;
ED3(c_uint8_t digit1:4;,
    c_uint8_t odd_even:1;,
    c_uint8_t type_of_identity:3;)
    union {
        nas_eps_mobile_identity_imsi_t imsi;
        nas_eps_mobile_identity_guti_t guti;
        nas_eps_mobile_identity_imei_t imei;
    } u;
} __attribute__ ((packed)) nas_eps_mobile_identity_t;

CORE_DECLARE(status_t) nas_decode_eps_mobile_identity(
    nas_eps_mobile_identity_t *eps_mobile_identity, pkbuf_t *pkbuf);

/* 9.9.3.15 ESM message container
 * M LV-E 5-n */
typedef struct _nas_esm_message_container_t {
    c_uint16_t length;
    c_uint8_t *buffer;
} nas_esm_message_container_t;

CORE_DECLARE(status_t) nas_decode_esm_message_container(
    nas_esm_message_container_t *esm_message_container, pkbuf_t *pkbuf);

/* 9.9.3.16A GPRS timer 2
 * See subclause 10.5.7.4 in 3GPP TS 24.008 [13].
 * O TLV 3 */
typedef struct _nas_gprs_timer_2_t {
    c_uint8_t length;
    c_uint8_t gprs_timer_2_value;
} __attribute__ ((packed)) nas_gprs_timer_2_t;

CORE_DECLARE(status_t) nas_decode_gprs_timer_2(
    nas_gprs_timer_2_t *t3324_value, pkbuf_t *pkbuf);

/* 9.9.3.16B GPRS timer 3
 * See subclause 10.5.7.4a in 3GPP TS 24.008 [13].
 * O TLV 3 */
#define NAS_GRPS_TIMER_3_UNIT_MULTIPLES_OF_10_MM    0
#define NAS_GRPS_TIMER_3_UNIT_MULTIPLES_OF_1_HH     1
#define NAS_GRPS_TIMER_3_UNIT_MULTIPLES_OF_10_HH    2
#define NAS_GRPS_TIMER_3_UNIT_MULTIPLES_OF_2_SS     3 
#define NAS_GRPS_TIMER_3_UNIT_MULTIPLES_OF_30_SS    4
#define NAS_GRPS_TIMER_3_UNIT_MULTIPLES_OF_1_MM     5
#define NAS_GRPS_TIMER_3_UNIT_MULTIPLES_OF_320_HH   6
#define NAS_GRPS_TIMER_3_UNIT_DEACTIVATED           7

typedef struct _nas_gprs_timer_3_t {
    c_uint8_t length;
ED2(c_uint8_t unit:3;,
    c_uint8_t timer_value:6;)
} __attribute__ ((packed)) nas_gprs_timer_3_t;

CORE_DECLARE(status_t) nas_decode_gprs_timer_3(
    nas_gprs_timer_3_t *t3412_extended_value, pkbuf_t *pkbuf);

/* 9.9.3.20 MS network capability
 * See subclause 10.5.5.12 in 3GPP TS 24.008
 * O TLV 4-10 */
#define NAS_MS_NETWORK_CAPABILITY_GEA2 0b01000000
#define NAS_MS_NETWORK_CAPABILITY_GEA3 0b00100000
#define NAS_MS_NETWORK_CAPABILITY_GEA4 0b00010000
#define NAS_MS_NETWORK_CAPABILITY_GEA5 0b00001000
#define NAS_MS_NETWORK_CAPABILITY_GEA6 0b00000100
#define NAS_MS_NETWORK_CAPABILITY_GEA7 0b00000010
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

CORE_DECLARE(status_t) nas_decode_ms_network_capability(
    nas_ms_network_capability_t *ms_network_capability, pkbuf_t *pkbuf);

/* 9.9.3.20A MS network feature support 
 * See subclause 10.5.1.15 in 3GPP TS 24.008 [13].
 * O TV 1 */
typedef struct _nas_ms_network_feature_support_t {
ED3(c_uint8_t type:4;,
    c_uint8_t spare:3;,
    c_uint8_t extended_periodic_timers:1;)
} __attribute__ ((packed)) nas_ms_network_feature_support_t;

CORE_DECLARE(status_t) nas_decode_ms_network_feature_support(
    nas_ms_network_feature_support_t *ms_network_feature_support, 
    pkbuf_t *pkbuf);

/* 9.9.3.24A Network resource identifier container
 * See subclause 10.5.5.31 in 3GPP TS 24.008 [13].
 * O TLV 4 */
typedef struct _nas_network_resource_identifier_container_t {
    c_uint8_t length;
    c_uint8_t nri_container_value1;
ED2(c_uint8_t nri_container_value2:2;,
    c_uint8_t spare:6;)
} __attribute__ ((packed)) nas_network_resource_identifier_container_t;

CORE_DECLARE(status_t) nas_decode_network_resource_identifier_container(
    nas_network_resource_identifier_container_t *tmsi_based_nri_container, 
    pkbuf_t *pkbuf);

/* 9.9.3.26 P-TMSI signature 
 * See subclause 10.5.5.8 in 3GPP TS 24.008
 * O TV 4 */
typedef c_uint32_t nas_p_tmsi_signature_t; /* TV : 4bytes */

CORE_DECLARE(status_t) nas_decode_p_tmsi_signature(
    nas_p_tmsi_signature_t *old_p_tmsi_signature, pkbuf_t *pkbuf);

/* 9.9.3.31 TMSI status
 * See subclause 10.5.5.4 in 3GPP TS 24.008 [13]
 * O TV 1 */
typedef struct _nas_tmsi_status_t {
ED3(c_uint8_t type:4;,
    c_uint8_t spare:3;,
    c_uint8_t tmsi_flag:1;)
} __attribute__ ((packed)) nas_tmsi_status_t;

CORE_DECLARE(status_t) nas_decode_tmsi_status(
    nas_tmsi_status_t *tmsi_status, pkbuf_t *pkbuf);

/* 9.9.3.32 Tracking area identity
 * O TV 6 */
typedef struct _nas_tracking_area_identity_t {
ED2(c_uint8_t mcc_digit2:4;,
    c_uint8_t mcc_digit1:4;)
ED2(c_uint8_t mnc_digit3:4;,
    c_uint8_t mcc_digit3:4;)
ED2(c_uint8_t mnc_digit2:4;,
    c_uint8_t mnc_digit1:4;)
    c_uint16_t tac;
} __attribute__ ((packed)) nas_tracking_area_identity_t;

CORE_DECLARE(status_t) nas_decode_tracking_area_identity(
    nas_tracking_area_identity_t *last_visited_registered_tai, pkbuf_t *pkbuf);

/* 9.9.3.34 UE network capability
 * M LV  3-14 */
typedef struct _nas_ue_network_capability_t {
    c_uint8_t length;
ED8(c_uint8_t eea0:1;,
    c_uint8_t eea1:1;,
    c_uint8_t eea2:1;,
    c_uint8_t eea3:1;,
    c_uint8_t eea4:1;,
    c_uint8_t eea5:1;,
    c_uint8_t eea6:1;,
    c_uint8_t eea7:1;)
ED8(c_uint8_t eia0:1;,
    c_uint8_t eia1:1;,
    c_uint8_t eia2:1;,
    c_uint8_t eia3:1;,
    c_uint8_t eia4:1;,
    c_uint8_t eia5:1;,
    c_uint8_t eia6:1;,
    c_uint8_t eia7:1;)
ED8(c_uint8_t uea0:1;,
    c_uint8_t uea1:1;,
    c_uint8_t uea2:1;,
    c_uint8_t uea3:1;,
    c_uint8_t uea4:1;,
    c_uint8_t uea5:1;,
    c_uint8_t uea6:1;,
    c_uint8_t uea7:1;)
ED8(c_uint8_t ucs2:1;,
    c_uint8_t uia1:1;,
    c_uint8_t uia2:1;,
    c_uint8_t uia3:1;,
    c_uint8_t uia4:1;,
    c_uint8_t uia5:1;,
    c_uint8_t uia6:1;,
    c_uint8_t uia7:1;)
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

CORE_DECLARE(status_t) nas_decode_ue_network_capability(
    nas_ue_network_capability_t *ue_network_capability, pkbuf_t *pkbuf);

/* 9.9.3.44 Voice domain preference and UE's usage setting
 * See subclause 10.5.5.28 in 3GPP TS 24.008 [13].
 * O TLV 3 */
typedef struct _nas_voice_domain_preference_and_ue_usage_setting_t {
    c_uint8_t length;
ED3(c_uint8_t spare:5;,
    c_uint8_t ue_usage_setting:1;, 
    c_uint8_t voice_domain_preference_for_e_utran:2;)
} __attribute__ ((packed)) nas_voice_domain_preference_and_ue_usage_setting_t;

CORE_DECLARE(status_t) nas_decode_voice_domain_preference_and_ue_usage_setting(
    nas_voice_domain_preference_and_ue_usage_setting_t *
        voice_domain_preference_and_ue_usage_setting, 
    pkbuf_t *pkbuf);

/* 9.9.3.45 GUTI type 
 * O TV 1 */
typedef struct _nas_guti_type_t {
ED3(c_uint8_t type:4;,
    c_uint8_t spare:3;,
    c_uint8_t guti_type:1;)
} __attribute__ ((packed)) nas_guti_type_t;

CORE_DECLARE(status_t) nas_decode_guti_type(
    nas_guti_type_t *old_guti_type, pkbuf_t *pkbuf);

/* 9.9.3.46 Extended DRX parameters
 * See subclause 10.5.5.32 in 3GPP TS 24.008 [13].
 * O TLV 3 */
typedef struct _nas_extended_drx_parameters_t {
    c_uint8_t length;
ED2(c_uint8_t paging_time_window:4;,
    c_uint8_t e_drx_value:4;)
} __attribute__ ((packed)) nas_extended_drx_parameters_t;

CORE_DECLARE(status_t) nas_decode_extended_drx_parameters(
    nas_extended_drx_parameters_t *extended_drx_parameters, pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

