/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef _NAS_TYPES_H__
#define _NAS_TYPES_H__

#include "base/types.h"
#include "base/context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define NAS_CLEAR_DATA(__dATA) \
    do { \
        ogs_assert((__dATA)); \
        if ((__dATA)->buffer) \
        { \
            ogs_free((__dATA)->buffer); \
            (__dATA)->buffer = NULL; \
            (__dATA)->length = 0; \
        } \
    } while(0)
#define NAS_STORE_DATA(__dST, __sRC) \
    do { \
        ogs_assert((__sRC)); \
        ogs_assert((__sRC)->buffer); \
        ogs_assert((__dST)); \
        NAS_CLEAR_DATA(__dST); \
        (__dST)->length = (__sRC)->length; \
        (__dST)->buffer = ogs_calloc((__dST)->length, sizeof(uint8_t)); \
        memcpy((__dST)->buffer, (__sRC)->buffer, (__dST)->length); \
    } while(0)

#define NAS_KSI_NO_KEY_IS_AVAILABLE             0x7

/**********************************
 * NAS PLMN_ID Structure             */
typedef struct nas_plmn_id_s {
ED2(uint8_t mcc2:4;,
    uint8_t mcc1:4;)
ED2(uint8_t mnc3:4;,
    uint8_t mcc3:4;)
ED2(uint8_t mnc2:4;,
    uint8_t mnc1:4;)
} __attribute__ ((packed)) nas_plmn_id_t;

void *nas_from_plmn_id(nas_plmn_id_t *nas_plmn_id, plmn_id_t *plmn_id);
void *nas_to_plmn_id(plmn_id_t *plmn_id, nas_plmn_id_t *nas_plmn_id);

typedef struct nas_guti_s {
    nas_plmn_id_t nas_plmn_id;
    uint16_t mme_gid;
    uint8_t mme_code;
    uint32_t m_tmsi;
} __attribute__ ((packed)) nas_guti_t;

/* 9.9.2.0 Additional information
 * O TLV 3-n */
#define NAX_MAX_ADDITIONAL_INFORMATION_LEN 255
typedef struct nas_additional_information_s {
    uint8_t length;
    uint8_t buffer[NAX_MAX_ADDITIONAL_INFORMATION_LEN];
} __attribute__ ((packed)) nas_additional_information_t;

/* 9.9.2.0A Device properties
 * See subclause 10.5.7.8 in 3GPP TS 24.008 [13].
 * O TV 1 */
typedef struct nas_device_properties_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t low_priority:1;)
} __attribute__ ((packed)) nas_device_properties_t;

/* 9.9.2.1 EPS bearer context status
 * O TLV 4 */
typedef struct nas_eps_bearer_context_status_s {
    uint8_t length;
ED8(uint8_t ebi7:1;,
    uint8_t ebi6:1;,
    uint8_t ebi5:1;,
    uint8_t ebi4:1;,
    uint8_t ebi3:1;,
    uint8_t ebi2:1;,
    uint8_t ebi1:1;,
    uint8_t ebi0:1;)
ED8(uint8_t ebi15:1;,
    uint8_t ebi14:1;,
    uint8_t ebi13:1;,
    uint8_t ebi12:1;,
    uint8_t ebi11:1;,
    uint8_t ebi10:1;,
    uint8_t ebi9:1;,
    uint8_t ebi8:1;)
} __attribute__ ((packed)) nas_eps_bearer_context_status_t;

/* 9.9.2.2 Location area identification
 * See subclause 10.5.1.3 in 3GPP TS 24.008 [13]
 * O TV 6 */
typedef struct nas_location_area_identification_s {
    nas_plmn_id_t nas_plmn_id;
    uint16_t lac;
} __attribute__ ((packed)) nas_location_area_identification_t;

typedef nas_location_area_identification_t nas_lai_t;

/* 9.9.2.3 Mobile identity
 * See subclause 10.5.1.4 in 3GPP TS 24.008 [13].
 * O TLV 7-10 */
#define NAS_MOBILE_IDENTITY_NONE 0
#define NAS_MOBILE_IDENTITY_IMSI 1
#define NAS_MOBILE_IDENTITY_IMEI 2
#define NAS_MOBILE_IDENTITY_IMEISV 3
#define NAS_MOBILE_IDENTITY_TMSI 4
#define NAS_MOBILE_IDENTITY_TMGI 5
#define NAS_MOBILE_IDENTITY_GUTI 6
typedef struct nas_mobile_identity_imsi_s {
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
} __attribute__ ((packed)) nas_mobile_identity_imsi_t;

typedef struct nas_mobile_identity_tmsi_s {
ED3(uint8_t spare:4;,
    uint8_t odd_even:1;,
    uint8_t type:3;)
    uint32_t tmsi;
} __attribute__ ((packed)) nas_mobile_identity_tmsi_t;

typedef struct nas_mobile_identity_tmgi_s {
ED5(uint8_t spare:2;,
    uint8_t mbms_session_id:1;,
    uint8_t mcc_mnc:1;,
    uint8_t odd_even:1;,
    uint8_t type:3;)
    uint8_t mbms_servicec_id[3];
    nas_plmn_id_t nas_plmn_id;
    uint8_t mbms_session_identity;
} __attribute__ ((packed)) nas_mobile_identity_tmgi_t;

typedef struct nas_mobile_identity_s {
    uint8_t length;
    union {
        nas_mobile_identity_imsi_t imsi;
        nas_mobile_identity_tmsi_t tmsi;
        nas_mobile_identity_tmgi_t tmgi;
    };
} nas_mobile_identity_t;

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
    uint8_t length;
ED5(uint8_t spare1:1;,
    uint8_t revision_level:2;, 
    uint8_t es_ind:1;,
    uint8_t a5_1:1;,
    uint8_t rf_power_capability:3;)
ED7(uint8_t spare:1;,
    uint8_t ps_capa:1;,
    uint8_t ss_screen_indicator:2;,
    uint8_t sm_capabi:1;,
    uint8_t vbs:1;,
    uint8_t vgcs:1;,
    uint8_t fc:1;)
ED8(uint8_t cm3:1;,
    uint8_t spare2:1;,
    uint8_t lcsva_cap:1;,
    uint8_t ucs2:1;,
    uint8_t solsa:1;,
    uint8_t cmsp:1;,
    uint8_t a5_3:1;,
    uint8_t a5_2:1;)
} __attribute__ ((packed)) nas_mobile_station_classmark_2_t;

/*9.9.2.5 Mobile station classmark 3
 * See subclause 10.5.1.7 in 3GPP TS 24.008 [13].
 * O TLV 2-34 */
#define NAS_MAX_MOBILE_STATION_CLASSMARK_3_LEN 32
typedef struct nas_mobile_station_classmark_3_s {
    uint8_t length;
    uint8_t buffer[NAS_MAX_MOBILE_STATION_CLASSMARK_3_LEN];
} __attribute__ ((packed)) nas_mobile_station_classmark_3_t;

/* 9.9.2.8 PLMN list
 * See subclause 10.5.1.13 in 3GPP TS 24.008 [13].
 * O TLV 5-47 */
#define NAS_MAX_PLMN 15
typedef struct nas_plmn_list_s {
    uint8_t length;
    nas_plmn_id_t nas_plmn_id[NAS_MAX_PLMN];
} __attribute__ ((packed)) nas_plmn_list_t;

/* 9.9.2.10 Supported codec list
 * See subclause 10.5.4.32 in 3GPP TS 24.008 [13].
 * O TLV 5-n */
typedef struct nas_supported_codec_item_s {
    uint8_t system_identification;
    uint8_t length_of_bitmap;
    uint16_t codec_bitmap;
} __attribute__ ((packed)) nas_supported_codec_item_t;

#define NAS_MAX_SUPPORTED_CODECS 8
typedef struct nas_supported_codec_list_s {
    uint8_t length;
    nas_supported_codec_item_t item[NAS_MAX_SUPPORTED_CODECS];
} __attribute__ ((packed)) nas_supported_codec_list_t;

/* 9.9.3.0A Additional update result
 * O TV 1  */
typedef struct nas_additional_update_result_s {
ED3(uint8_t type:4;,
    uint8_t spare:2;,
    uint8_t additional_update_result_value:2;)
} __attribute__ ((packed)) nas_additional_update_result_t;

/* 9.9.3.0B Additional update type
 * O TV 1 */
#define NAS_ADDITIONAL_UPDATE_TYPE_CIOT_NONE 0
#define NAS_ADDITIONAL_UPDATE_TYPE_CIOT_CONTROL_PLANE 1
#define NAS_ADDITIONAL_UPDATE_TYPE_CIOT_DATA_PLANE 2
#define NAS_ADDITIONAL_UPDATE_TYPE_CIOT_RESERVED 3
typedef struct nas_additional_update_type_s {
ED4(uint8_t type:4;,
    uint8_t pnb_ciot:2;,
    uint8_t saf:1;,
    uint8_t autv:1;)
} __attribute__ ((packed)) nas_additional_update_type_t;

/* 9.9.3.1 Authentication failure parameter
 * See subclause 10.5.3.2.2 in 3GPP TS 24.008 [13].
 * O TLV 16 */
typedef struct nas_authentication_failure_parameter_s {
    uint8_t length;
    uint8_t auts[AUTS_LEN];
} __attribute__ ((packed)) nas_authentication_failure_parameter_t;

/* 9.9.3.2 Authentication parameter AUTN
 * See subclause 10.5.3.1.1 in 3GPP TS 24.008 [13].
 * M LV 17 */
typedef struct nas_authentication_parameter_autn_s {
    uint8_t length;
    uint8_t autn[AUTN_LEN];
} nas_authentication_parameter_autn_t;

/* 9.9.3.3 Authentication parameter RAND
 * See subclause 10.5.3.1 in 3GPP TS 24.008 [13].
 * M V 16 */
typedef struct nas_authentication_parameter_rand_s {
    uint8_t rand[RAND_LEN];
} nas_authentication_parameter_rand_t;

/* 9.9.3.4 Authentication response parameter
 * M LV 5-17 */
typedef struct nas_authentication_response_parameter_s {
    uint8_t length;
    uint8_t res[MAX_RES_LEN];
} nas_authentication_response_parameter_t;

/* 9.9.3.4a Ciphering key sequence number
 * See subclause 10.5.1.2 in 3GPP TS 24.008 [13].
 * O TV 1 */
#define NAS_CIPHERING_KEY_SEQUENCE_NUMBER_NO_KEY_FROM_MS 7
typedef struct nas_ciphering_key_sequence_number_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t key_sequence:3;)
} __attribute__ ((packed)) nas_ciphering_key_sequence_number_t;

/* 9.9.3.5 CSFB response
 * O TV 1 */
#define NAS_CS_FALLBACK_RESPONSE_REJECTED_BY_THE_UE     0
#define NAS_CS_FALLBACK_RESPONSE_ACCEPTED_BY_THE_UE     1
typedef struct nas_csfb_response_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t response:3;)
} __attribute__ ((packed)) nas_csfb_response_t;

/* 9.9.3.6 Daylight saving time
 * See subclause 10.5.3.12 in 3GPP TS 24.008 [13].
 * O TLV 3 */
#define NAS_NO_ADJUSTMENT_FOR_DAYLIGHT_SAVING_TIME              0
#define NAS_PLUS_1_HOUR_ADJUSTMENT_FOR_DAYLIGHT_SAVING_TIME     1
#define NAS_PLUS_2_HOURS_ADJUSTMENT_FOR_DAYLIGHT_SAVING_TIME    2
typedef struct nas_daylight_saving_time_s {
    uint8_t length;
ED2(uint8_t spare:6;,
    uint8_t value:2;)
} __attribute__ ((packed)) nas_daylight_saving_time_t;

/* 9.9.3.7 Detach type
 * M V 1/2 
 * 9.9.3.21 NAS key set identifier 
 * M V 1/2 */
#define NAS_DETACH_TYPE_FROM_UE_EPS_DETACH                  1
#define NAS_DETACH_TYPE_FROM_UE_IMSI_DETACH                 2
#define NAS_DETACH_TYPE_FROM_UE_COMBINED_EPS_IMSI_DETACH    3
#define NAS_DETACH_TYPE_TO_UE_RE_ATTACH_REQUIRED            1
#define NAS_DETACH_TYPE_TO_UE_RE_ATTACH_NOT_REQUIRED        2
#define NAS_DETACH_TYPE_TO_UE_IMSI_DETACH                   3
typedef struct nas_detach_type_s {
ED4(uint8_t tsc:1;,
    uint8_t nas_key_set_identifier:3;,
    uint8_t switch_off:1;,
    uint8_t detach_type:3;)
} __attribute__ ((packed)) nas_detach_type_t;

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
typedef struct nas_drx_parameter_s {
    uint8_t split_pg_cycle_code;
ED3(uint8_t cn_specific_drx_cycle_length_coefficient_and_drx_value_for_s1_mode:4;,
    uint8_t split_on_ccch:1;,
    uint8_t non_DRX_timer:3;)
} __attribute__ ((packed)) nas_drx_parameter_t;

/* 9.9.3.9 EMM cause
 * O TV 2 
 * Annex A (informative): Cause values for EPS mobility management 
 * A.1 Causes related to UE identification */
#define EMM_CAUSE_IMSI_UNKNOWN_IN_HSS 2
#define EMM_CAUSE_ILLEGAL_UE 3
#define EMM_CAUSE_IMSI_UNKNOWN_IN_VLR 4
#define EMM_CAUSE_ILLEGAL_ME 6
#define EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK 9
#define EMM_CAUSE_IMPLICITLY_DETACHED 10
/* A.2 Cause related to subscription options */
#define EMM_CAUSE_IMEI_NOT_ACCEPTED 5
#define EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED 7
#define EMM_CAUSE_EPS_SERVICES_AND_NON_EPS_SERVICES_NOT_ALLOWED 8
#define EMM_CAUSE_PLMN_NOT_ALLOWED 11
#define EMM_CAUSE_TRACKING_AREA_NOT_ALLOWED 12
#define EMM_CAUSE_ROAMING_NOT_ALLOWED_IN_THIS_TRACKING_AREA 13
#define EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED_IN_THIS_PLMN 14
#define EMM_CAUSE_NO_SUITABLE_CELLS_IN_TRACKING_AREA 15
#define EMM_CAUSE_REQUESTED_SERVICE_OPTION_NOT_AUTHORIZED_IN_THIS_PLMN 35
#define EMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED 40
/* A.3 Causes related to PLMN specific network failures and 
 *     congestion/authentication failures */
#define EMM_CAUSE_MSC_TEMPORARILY_NOT_REACHABLE 16
#define EMM_CAUSE_NETWORK_FAILURE 17
#define EMM_CAUSE_CS_DOMAIN_NOT_AVAILABLE 18
#define EMM_CAUSE_ESM_FAILURE 19
#define EMM_CAUSE_MAC_FAILURE 20
#define EMM_CAUSE_SYNCH_FAILURE 21
#define EMM_CAUSE_CONGESTION 22
#define EMM_CAUSE_UE_SECURITY_CAPABILITIES_MISMATCH 23
#define EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED 24
#define EMM_CAUSE_NON_EPS_AUTHENTICATION_UNACCEPTABLE 26
#define EMM_CAUSE_CS_SERVICE_TEMPORARILY_NOT_AVAILABLE 39
#define EMM_CAUSE_SEVERE_NETWORK_FAILURE 42
/* A.4 Causes related to nature of request 
 * NOTE:   This subclause has no entries in this version of the specification *
 * A.5 Causes related to invalid messages */
#define EMM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE 95
#define EMM_CAUSE_INVALID_MANDATORY_INFORMATION 96
#define EMM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED 97
#define EMM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE 98
#define EMM_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED 99
#define EMM_CAUSE_CONDITIONAL_IE_ERROR 100
#define EMM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE 101
#define EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED 111
typedef uint8_t nas_emm_cause_t;

/* 9.9.3.10 * EPS attach result
 * M V 1/2 */
#define NAS_ATTACH_RESULT_EPS_ONLY 1
#define NAS_ATTACH_RESULT_COMBINED_EPS_IMSI_ATTACH 2
typedef struct nas_eps_attach_result_s {
ED2(uint8_t spare:5;,
    uint8_t result:3;)
} __attribute__ ((packed)) nas_eps_attach_result_t;

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
typedef struct nas_eps_attach_type_s {
ED4(uint8_t tsc:1;,
    uint8_t nas_key_set_identifier:3;,
    uint8_t spare:1;,
    uint8_t attach_type:3;)
} __attribute__ ((packed)) nas_eps_attach_type_t;

/* 9.9.3.12 EPS mobile identity
 * M LV  5-12 */
#define NAS_EPS_MOBILE_IDENTITY_IMSI 1
#define NAS_EPS_MOBILE_IDENTITY_GUTI 6
#define NAS_EPS_MOBILE_IDENTITY_IMEI 3

#define NAS_EPS_MOBILE_IDENTITY_EVEN 0
#define NAS_EPS_MOBILE_IDENTITY_ODD 1
typedef struct nas_eps_mobile_identity_guti_s {
ED3(uint8_t spare:4;,
    uint8_t odd_even:1;,
    uint8_t type:3;)
    nas_plmn_id_t nas_plmn_id;
    uint16_t mme_gid;
    uint8_t mme_code;
    uint32_t m_tmsi;
} __attribute__ ((packed)) nas_eps_mobile_identity_guti_t;

typedef nas_mobile_identity_imsi_t nas_eps_mobile_identity_imsi_t;
typedef nas_eps_mobile_identity_imsi_t nas_eps_mobile_identity_imei_t;

typedef struct nas_eps_mobile_identity_s {
    uint8_t length;
    union {
        nas_eps_mobile_identity_imsi_t imsi;
        nas_eps_mobile_identity_guti_t guti;
        nas_eps_mobile_identity_imei_t imei;
    };
} __attribute__ ((packed)) nas_eps_mobile_identity_t;

/* 9.9.3.12A EPS network feature support 
 * O TLV 3 */
typedef struct nas_eps_network_feature_support_s {
    uint8_t length;
ED7(uint8_t cp_ciot:1;,
    uint8_t erw_opdn:1;,
    uint8_t esr_ps:1;,
    uint8_t cs_lcs:2;,
    uint8_t epc_lcs:1;,
    uint8_t emc_bs:1;,
    uint8_t ims_vops:1;)
ED5(uint8_t spare:4;,
    uint8_t e_pco:1;,
    uint8_t hc_cp_ciot:1;,
    uint8_t s1_u_data:1;,
    uint8_t up_ciot :1;)
} __attribute__ ((packed)) nas_eps_network_feature_support_t;

/* 9.9.3.13 EPS update result
 * M V 1/2 */
#define NAS_EPS_UPDATE_RESULT_TA_UPDATED                                0
#define NAS_EPS_UPDATE_RESULT_COMBINED_TA_LA_UPDATED                    1
#define NAS_EPS_UPDATE_RESULT_TA_UPDATED_AND_ISR_ACTIVATED              4
#define NAS_EPS_UPDATE_RESULT_COMBINED_TA_LA_UPDATED_AND_ISR_ACTIVATED  5
typedef struct nas_eps_update_result_s {
ED2(uint8_t spare:5;,
    uint8_t result:3;)
} __attribute__ ((packed)) nas_eps_update_result_t;

/* 9.9.3.14 EPS update type
 * M V 1/2 
 * 9.9.3.21 NAS key set identifier 
 * M V 1/2 */
#define NAS_EPS_UPDATE_TYPE_TA_UPDATING                                 0
#define NAS_EPS_UPDATE_TYPE_COMBINED_TA_LA_UPDATING                     1
#define NAS_EPS_UPDATE_TYPE_COMBINED_TA_LA_UPDATING_WITH_IMSI_ATTACH    2
#define NAS_EPS_UPDATE_TYPE_PERIODIC_UPDATING                           3
typedef struct nas_eps_update_type_s {
ED4(uint8_t tsc:1;,
    uint8_t nas_key_set_identifier:3;,
    uint8_t active_flag:1;,
    uint8_t update_type:3;)
} __attribute__ ((packed)) nas_eps_update_type_t;

/* 9.9.3.15 ESM message container
 * M LV-E 5-n */
typedef struct nas_esm_message_container_s {
    uint16_t length;
    uint8_t *buffer;
} nas_esm_message_container_t;

/* 9.9.3.16 GPRS timer
 * See subclause 10.5.7.3 in 3GPP TS 24.008 [13].
 * M V 1 or  O TV 2 */
#define NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_2_SS       0
#define NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_1_MM       1
#define NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH    2
#define NAS_GRPS_TIMER_UNIT_DEACTIVATED             7
typedef struct nas_gprs_timer_s {
ED2(uint8_t unit:3;,
    uint8_t value:5;)
} __attribute__ ((packed)) nas_gprs_timer_t;

/* 9.9.3.16A GPRS timer 2
 * See subclause 10.5.7.4 in 3GPP TS 24.008 [13].
 * O TLV 3 */
typedef struct nas_gprs_timer_2_s {
    uint8_t length;
    uint8_t gprs_timer_2_value;
} __attribute__ ((packed)) nas_gprs_timer_2_t;

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

typedef struct nas_gprs_timer_3_s {
    uint8_t length;
ED2(uint8_t unit:3;,
    uint8_t timer_value:5;)
} __attribute__ ((packed)) nas_gprs_timer_3_t;

/* 9.9.3.17 Identity type 2
 * See subclause 10.5.5.9 in 3GPP TS 24.008 [13].
 * M V 1/2 */
#define NAS_IDENTITY_TYPE_2_IMSI            1
#define NAS_IDENTITY_TYPE_2_IMEI            2
#define NAS_IDENTITY_TYPE_2_IMEISV          3
#define NAS_IDENTITY_TYPE_2_TMSI            4
typedef struct nas_identity_type_2_s {
ED2(uint8_t spare:5;,
    uint8_t type:3;)
} __attribute__ ((packed)) nas_identity_type_2_t;

/* 9.9.3.18 IMEISV request
 * See subclause 10.5.5.10 in 3GPP TS 24.008 [13].
 * O TV 1 */
typedef struct nas_imeisv_request_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t imeisv_request_value:3;)
} __attribute__ ((packed)) nas_imeisv_request_t;

/* 9.9.3.19 KSI and sequence number
 * M V 1 */
typedef struct nas_ksi_and_sequence_number_s {
ED2(uint8_t ksi:3;,
    uint8_t sequence_number:5;)
} __attribute__ ((packed)) nas_ksi_and_sequence_number_t;

/* 9.9.3.20 MS network capability
 * See subclause 10.5.5.12 in 3GPP TS 24.008
 * O TLV 4-10 */
#define NAS_MS_NETWORK_CAPABILITY_GEA2 0b01000000
#define NAS_MS_NETWORK_CAPABILITY_GEA3 0b00100000
#define NAS_MS_NETWORK_CAPABILITY_GEA4 0b00010000
#define NAS_MS_NETWORK_CAPABILITY_GEA5 0b00001000
#define NAS_MS_NETWORK_CAPABILITY_GEA6 0b00000100
#define NAS_MS_NETWORK_CAPABILITY_GEA7 0b00000010
typedef struct nas_ms_network_capability_s {
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
ED6(uint8_t user_plane_integrity_protection_support:1;,
    uint8_t gia4:1;,
    uint8_t gia5:1;,
    uint8_t gia6:1;,
    uint8_t gia7:1;,
    uint8_t spare:3;)
} __attribute__ ((packed)) nas_ms_network_capability_t;

/* 9.9.3.20A MS network feature support 
 * See subclause 10.5.1.15 in 3GPP TS 24.008 [13].
 * O TV 1 */
typedef struct nas_ms_network_feature_support_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t extended_periodic_timers:1;)
} __attribute__ ((packed)) nas_ms_network_feature_support_t;

/* 9.9.3.21 NAS key set identifier
 * M V 1/2
 * 9.9.2.9 Spare half octet
 * M V 1/2 */
typedef struct nas_key_set_identifier_s {
ED3(uint8_t type:4;,
    uint8_t tsc:1;,
    uint8_t nas_key_set_identifier:3;)
} __attribute__ ((packed)) nas_key_set_identifier_t;

/* 9.9.3.22 message container
 * M LV 3-252 */
#define NAS_MAX_MESSAGE_CONTAINER_LEN 250
typedef struct nas_message_container_s {
    uint8_t length;
    uint8_t buffer[NAS_MAX_MESSAGE_CONTAINER_LEN];
} __attribute__ ((packed)) nas_message_container_t;

/* 9.9.3.23 NAS security algorithms
 * M V 1 */
#define NAS_SECURITY_ALGORITHMS_EIA0        0
#define NAS_SECURITY_ALGORITHMS_128_EIA1    1
#define NAS_SECURITY_ALGORITHMS_128_EIA2    2
#define NAS_SECURITY_ALGORITHMS_128_EIA3    3
#define NAS_SECURITY_ALGORITHMS_EEA0        0
#define NAS_SECURITY_ALGORITHMS_128_EEA1    1
#define NAS_SECURITY_ALGORITHMS_128_EEA2    2
#define NAS_SECURITY_ALGORITHMS_128_EEA3    3
typedef struct nas_security_algorithms_s {
ED4(uint8_t spare1:1;,
    uint8_t type_of_ciphering_algorithm:3;,
    uint8_t spare2:1;,
    uint8_t type_of_integrity_protection_algorithm:3;)
} __attribute__ ((packed)) nas_security_algorithms_t;

/* 9.9.3.24 Network name
 * See subclause 10.5.3.5a in 3GPP TS 24.008 [13].
 * O TLV 3-n */
#define NAS_MAX_NETWORK_NAME_LEN 255
typedef struct nas_network_name_s {
    uint8_t length;
ED4(uint8_t ext:1;,
    uint8_t coding_scheme:3;,
    uint8_t add_ci:1;,
    uint8_t number_of_spare_bits_in_last_octet:3;)
    uint8_t name[NAS_MAX_NETWORK_NAME_LEN];
} __attribute__ ((packed)) nas_network_name_t;

/* 9.9.3.24A Network resource identifier container
 * See subclause 10.5.5.31 in 3GPP TS 24.008 [13].
 * O TLV 4 */
typedef struct nas_network_resource_identifier_container_s {
    uint8_t length;
    uint8_t nri_container_value1;
ED2(uint8_t nri_container_value2:2;,
    uint8_t spare:6;)
} __attribute__ ((packed)) nas_network_resource_identifier_container_t;

/* 9.9.3.25 Nonce
 * O TV 5 */
typedef uint32_t nas_nonce_t;

/* 9.9.3.25A Paging identity
 * M V 1 */
#define NAS_PAGING_IDENTITY_IMSI        0
#define NAS_PAGING_IDENTITY_TMSI        1
typedef struct nas_paging_identity_s {
ED2(uint8_t spare:7;,
    uint8_t identity:1;)
} nas_paging_identity_t;

/* 9.9.3.26 P-TMSI signature 
 * See subclause 10.5.5.8 in 3GPP TS 24.008
 * O TV 4 */
typedef uint32_t nas_p_tmsi_signature_t; /* TV : 4bytes */

/* 9.9.3.26A Extended EMM cause 
 * O TV 1 */
typedef struct nas_extended_emm_cause_s {
ED4(uint8_t type:4;,
    uint8_t spare:2;,
    uint8_t eps_optimization_info:1;,
    uint8_t e_utran_allowed:1;)
} nas_extended_emm_cause_t;

/* 9.9.3.27 Service type
 * M V 1/2 
 * 9.9.3.21 NAS key set identifier 
 * M V 1/2 */
#define NAS_SERVICE_TYPE_CS_FALLBACK_FROM_UE 0
#define NAS_SERVICE_TYPE_CS_FALLBACK_TO_UE 1
#define NAS_SERVICE_TYPE_CS_FALLBACK_EMERGENCY_CALL_FROM_UE 2
typedef struct nas_service_type_s {
ED3(uint8_t tsc:1;,
    uint8_t nas_key_set_identifier:3;,
    uint8_t service_type:4;)
} __attribute__ ((packed)) nas_service_type_t;

/* 9.9.3.28 Short MAC
 * M V 2 */
typedef uint16_t nas_short_mac_t;

/* 9.9.3.29 Time zone
 * See subclause 10.5.3.8 in 3GPP TS 24.008 [13].
 * O TV 2 */
typedef uint8_t nas_time_zone_t;

/* 9.9.3.30 Time zone and time
 * See subclause 10.5.3.9 in 3GPP TS 24.008 [13].
 * 9.2.3.11 TPServiceCentreTimeStamp (TPSCTS) in 3GPP TS 23.040 [90]
 * O TV 8 */
#define NAS_TIME_TO_BCD(x) TIME_TO_BCD(x)
typedef struct nas_time_zone_and_time_s {
    uint8_t year;
    uint8_t mon;
    uint8_t mday;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    /* The Time Zone indicates the difference, expressed in quarters of an hour,
     * between the local time and GMT. In the first of the two semi-octets, 
     * the first bit (bit 3 of the seventh octet of 
     * the TP-Service-Centre-Time-Stamp field) represents 
     * the algebraic sign of this difference (0: positive, 1: negative). */
    uint8_t timezone;  
} nas_time_zone_and_time_t;

/* 9.9.3.31 TMSI status
 * See subclause 10.5.5.4 in 3GPP TS 24.008 [13]
 * O TV 1 */
typedef struct nas_tmsi_status_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t tmsi_flag:1;)
} __attribute__ ((packed)) nas_tmsi_status_t;

/* 9.9.3.32 Tracking area identity
 * O TV 6 */
typedef struct nas_tracking_area_identity_s {
    nas_plmn_id_t nas_plmn_id;
    uint16_t tac;
} __attribute__ ((packed)) nas_tracking_area_identity_t;

typedef nas_tracking_area_identity_t nas_tai_t;

/* 9.9.3.33 Tracking area identity list
 * M LV 7-97 */
#define NAS_MAX_TAI_LIST_LEN        96
#define TAI0_TYPE                   0
#define TAI1_TYPE                   1
#define TAI2_TYPE                   2
typedef struct tai0_list_s {
    struct {
    ED3(uint8_t spare:1;,
        uint8_t type:2;,
        uint8_t num:5;)
        /*
         * Do not change 'plmn_id_t' to 'nas_plmn_id_t'.
         * Use 'plmn_id_t' for easy implementation.
         * nas_tai_list_build() changes to NAS format(nas_plmn_id_t)
         * and is sent to the UE.
         */
        plmn_id_t plmn_id;
        uint16_t tac[MAX_NUM_OF_TAI];
    } __attribute__ ((packed)) tai[MAX_NUM_OF_TAI];
} __attribute__ ((packed)) tai0_list_t;

typedef struct tai2_list_s {
ED3(uint8_t spare:1;,
    uint8_t type:2;,
    uint8_t num:5;)
    /*
     * Do not change 'tai_t' to 'nas_tracking_area_identity_t'.
     * Use 'tai_t' for easy implementation.
     * nas_tai_list_build() changes to NAS format(nas_tracking_area_identity_t)
     * and is sent to the UE.
     */
    tai_t tai[MAX_NUM_OF_TAI];
} __attribute__ ((packed)) tai2_list_t;

typedef struct nas_tracking_area_identity_list_s {
    uint8_t length;
    uint8_t buffer[NAS_MAX_TAI_LIST_LEN];
} __attribute__ ((packed)) nas_tracking_area_identity_list_t;

void nas_tai_list_build(
        nas_tracking_area_identity_list_t *target,
        tai0_list_t *source0, tai2_list_t *source2);

/* 9.9.3.34 UE network capability
 * M LV  3-14 */
typedef struct nas_ue_network_capability_s {
    uint8_t length;
    union { 
        struct {
        ED8(uint8_t eea0:1;,
            uint8_t eea1:1;,
            uint8_t eea2:1;,
            uint8_t eea3:1;,
            uint8_t eea4:1;,
            uint8_t eea5:1;,
            uint8_t eea6:1;,
            uint8_t eea7:1;)
        };
        uint8_t eea; 
    };
    union { 
        struct {
        ED8(uint8_t eia0:1;,
            uint8_t eia1:1;,
            uint8_t eia2:1;,
            uint8_t eia3:1;,
            uint8_t eia4:1;,
            uint8_t eia5:1;,
            uint8_t eia6:1;,
            uint8_t eia7:1;)
        };
        uint8_t eia; 
    };
    union { 
        struct {
        ED8(uint8_t uea0:1;,
            uint8_t uea1:1;,
            uint8_t uea2:1;,
            uint8_t uea3:1;,
            uint8_t uea4:1;,
            uint8_t uea5:1;,
            uint8_t uea6:1;,
            uint8_t uea7:1;)
        };
        uint8_t uea; 
    };
    union { 
        struct {
        ED8(uint8_t ucs2:1;,
            uint8_t uia1:1;,
            uint8_t uia2:1;,
            uint8_t uia3:1;,
            uint8_t uia4:1;,
            uint8_t uia5:1;,
            uint8_t uia6:1;,
            uint8_t uia7:1;)
        };
        uint8_t uia; 
    };
ED8(uint8_t prose_dd:1;,
    uint8_t prose:1;,
    uint8_t h_245_ash:1;,
    uint8_t acc_csfb:1;,
    uint8_t lpp:1;,
    uint8_t lcs:1;,
    uint8_t srvcc:1;,
    uint8_t nf:1;)
ED8(uint8_t epco:1;,
    uint8_t hc_cp_ciot:1;,
    uint8_t erw_opdn:1;,
    uint8_t s1u_data:1;,
    uint8_t up_ciot:1;,
    uint8_t cp_ciot:1;,
    uint8_t prose_relay:1;,
    uint8_t prose_dc:1;)
ED2(uint8_t spare:7;,
    uint8_t multiple_drb:1;)
} __attribute__ ((packed)) nas_ue_network_capability_t;

/* 9.9.3.35 UE radio capability information update needed
 * O TV 1 */
typedef struct nas_ue_radio_capability_information_update_needed_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t update_needed:1;)
} __attribute__ ((packed)) nas_ue_radio_capability_information_update_needed_t;

/* 9.9.3.36 UE security capability
 * M LV 3-6 */
typedef struct nas_ue_security_capability_s {
    uint8_t length;
    union { 
        struct {
        ED8(uint8_t eea0:1;,
            uint8_t eea1:1;,
            uint8_t eea2:1;,
            uint8_t eea3:1;,
            uint8_t eea4:1;,
            uint8_t eea5:1;,
            uint8_t eea6:1;,
            uint8_t eea7:1;)
        };
        uint8_t eea; 
    };
    union { 
        struct {
        ED8(uint8_t eia0:1;,
            uint8_t eia1:1;,
            uint8_t eia2:1;,
            uint8_t eia3:1;,
            uint8_t eia4:1;,
            uint8_t eia5:1;,
            uint8_t eia6:1;,
            uint8_t eia7:1;)
        };
        uint8_t eia; 
    };
    union { 
        struct {
        ED8(uint8_t uea0:1;,
            uint8_t uea1:1;,
            uint8_t uea2:1;,
            uint8_t uea3:1;,
            uint8_t uea4:1;,
            uint8_t uea5:1;,
            uint8_t uea6:1;,
            uint8_t uea7:1;)
        };
        uint8_t uea; 
    };
    union { 
        struct {
        ED8(uint8_t spare1:1;,
            uint8_t uia1:1;,
            uint8_t uia2:1;,
            uint8_t uia3:1;,
            uint8_t uia4:1;,
            uint8_t uia5:1;,
            uint8_t uia6:1;,
            uint8_t uia7:1;)
        };
        uint8_t uia; 
    };
    union { 
        struct {
        ED8(uint8_t spare2:1;,
            uint8_t gea1:1;,
            uint8_t gea2:1;,
            uint8_t gea3:1;,
            uint8_t gea4:1;,
            uint8_t gea5:1;,
            uint8_t gea6:1;,
            uint8_t gea7:1;)
        };
        uint8_t gea; 
    };
} __attribute__ ((packed)) nas_ue_security_capability_t;

/* buffer : 9.9.3.37 Emergency number list
 * See subclause 10.5.3.13 in 3GPP TS 24.008 [13].
 * O TLV 5-50 */
#define NAS_MAX_EMERGENCY_NUMBER_LIST_LEN 48
typedef struct nas_emergency_number_list_s {
    uint16_t length;
    uint8_t buffer[NAS_MAX_EMERGENCY_NUMBER_LIST_LEN];
} __attribute__ ((packed)) nas_emergency_number_list_t;

/* 9.9.3.38 CLI
 * O TLV 3-14 
 * The coding of the CLI value part is the same as for 
 * octets 3 to 14 of the Calling party BCD number information element 
 * defined in subclause 10.5.4.9 of 3GPP TS 24.008 [13]. */
#define NAX_MAX_CLI_LEN 12
typedef struct nas_cli_s {
    uint8_t length;
    uint8_t buffer[NAX_MAX_CLI_LEN];
} __attribute__ ((packed)) nas_cli_t;

/* 9.9.3.39 SS Code
 * O TV 2 
 * The coding of the SS Code value is given in subclause 17.7.5 of 
 * 3GPP TS 29.002 [15C] */
typedef uint8_t nas_ss_code_t;

/* 9.9.3.40 LCS indicator
 * O TV 2 */
#define NAS_LCS_INDICATOR_MT_LR     1
typedef uint8_t nas_lcs_indicator_t;

/* 9.9.3.41 LCS client identity
 * O TLV 3-257 */
#define NAS_MAX_LCS_CLIENT_IDENTITY_LEN 255
typedef struct nas_lcs_client_identity_s {
    uint8_t length;
    uint8_t buffer[NAS_MAX_LCS_CLIENT_IDENTITY_LEN];
} __attribute__ ((packed)) nas_lcs_client_identity_t;

/* 9.9.3.42 Generic message container type
 * M V 1 */
#define NAS_GENERIC_MESSAGE_CONTAINER_TYPE_LTE_POSITIONING_PROTOCOL         1
#define NAS_GENERIC_MESSAGE_CONTAINER_TYPE_LTE_LOCATION_SERVICES_MESSAGE    2
typedef uint8_t nas_generic_message_container_type_t;

/* 9.9.3.43 Generic message container
 * M LV-E 3-n */
typedef struct nas_generic_message_container_s {
    uint16_t length;
    uint8_t *buffer;
} nas_generic_message_container_t;

/* 9.9.3.44 Voice domain preference and UE's usage setting
 * See subclause 10.5.5.28 in 3GPP TS 24.008 [13].
 * O TLV 3 */
typedef struct nas_voice_domain_preference_and_ue_usage_setting_s {
    uint8_t length;
ED3(uint8_t spare:5;,
    uint8_t ue_usage_setting:1;, 
    uint8_t voice_domain_preference_for_e_utran:2;)
} __attribute__ ((packed)) nas_voice_domain_preference_and_ue_usage_setting_t;

/* 9.9.3.45 GUTI type 
 * O TV 1 */
typedef struct nas_guti_type_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t guti_type:1;)
} __attribute__ ((packed)) nas_guti_type_t;

/* 9.9.3.46 Extended DRX parameters
 * See subclause 10.5.5.32 in 3GPP TS 24.008 [13].
 * O TLV 3 */
typedef struct nas_extended_drx_parameters_s {
    uint8_t length;
ED2(uint8_t paging_time_window:4;,
    uint8_t e_drx_value:4;)
} __attribute__ ((packed)) nas_extended_drx_parameters_t;

/* 9.9.4.1 Access point name
 * See subclause 10.5.6.1 in 3GPP TS 24.008 [13].
 * O TLV 3-102 */
typedef struct nas_access_point_name_s {
    uint8_t length;
    char apn[MAX_APN_LEN];
} __attribute__ ((packed)) nas_access_point_name_t;

/* 9.9.4.2 APN aggregate maximum bit rate
 * O TLV 4-8  */
typedef struct nas_apn_aggregate_maximum_bit_rate_s {
    uint8_t length;
    uint8_t dl_apn_ambr;
    uint8_t ul_apn_ambr;
    uint8_t dl_apn_ambr_extended;
    uint8_t ul_apn_ambr_extended;
    uint8_t dl_apn_ambr_extended2;
    uint8_t ul_apn_ambr_extended2;
} __attribute__ ((packed)) nas_apn_aggregate_maximum_bit_rate_t;

void apn_ambr_build(
    nas_apn_aggregate_maximum_bit_rate_t * apn_aggregate_maximum_bit_rate,
    uint32_t dl_ambr, uint32_t ul_ambr);

/* 9.9.4.2A Connectivity type
 * See subclause 10.5.6.19 in 3GPP TS 24.008 [13].
 * O TV 1 */
typedef struct nas_connectivity_type_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t considered_lipa_pdn_connection:1;)
} __attribute__ ((packed)) nas_connectivity_type_t;

/* 9.9.4.3 EPS quality of service
 * M LV 2-14 */
typedef struct nas_eps_quality_of_service_s {
    uint8_t length;
    uint8_t qci;
    uint8_t ul_mbr;
    uint8_t dl_mbr;
    uint8_t ul_gbr;
    uint8_t dl_gbr;
    uint8_t ul_mbr_extended;
    uint8_t dl_mbr_extended;
    uint8_t ul_gbr_extended;
    uint8_t dl_gbr_extended;
    uint8_t ul_mbr_extended2;
    uint8_t dl_mbr_extended2;
    uint8_t ul_gbr_extended2;
    uint8_t dl_gbr_extended2;
} nas_eps_quality_of_service_t;

void eps_qos_build(nas_eps_quality_of_service_t *eps_qos,
    uint8_t qci,
    uint64_t dl_mbr, uint64_t ul_mbr, uint64_t dl_gbr, uint64_t ul_gbr);

/* 9.9.4.4 ESM cause
 * M V 1 
 * Annex B (informative): Cause values for EPS session management 
   B.1 Causes related to nature of request */
#define ESM_CAUSE_OPERATOR_DETERMINED_BARRING 8
#define ESM_CAUSE_INSUFFICIENT_RESOURCES 26
#define ESM_CAUSE_MISSING_OR_UNKNOWN_APN 27
#define ESM_CAUSE_UNKNOWN_PDN_TYPE 28
#define ESM_CAUSE_USER_AUTHENTICATION_FAILED 29
#define ESM_CAUSE_REQUEST_REJECTED_BY_SERVING_GW_OR_PDN_GW 30
#define ESM_CAUSE_REQUEST_REJECTED_UNSPECIFIED 31
#define ESM_CAUSE_SERVICE_OPTION_NOT_SUPPORTED 32
#define ESM_CAUSE_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED 33
#define ESM_CAUSE_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER 34
#define ESM_CAUSE_PTI_ALREADY_IN_USE 35
#define ESM_CAUSE_REGULAR_DEACTIVATION 36
#define ESM_CAUSE_EPS_QOS_NOT_ACCEPTED 37
#define ESM_CAUSE_NETWORK_FAILURE 38
#define ESM_CAUSE_REACTIVATION_REQUESTED 39
#define ESM_CAUSE_SEMANTIC_ERROR_IN_THE_TFT_OPERATION  41
#define ESM_CAUSE_SYNTACTICAL_ERROR_IN_THE_TFT_OPERATION 42
#define ESM_CAUSE_INVALID_EPS_BEARER_IDENTITY 43
#define ESM_CAUSE_SEMANTIC_ERRORS_IN_PACKET_FILTERS 44
#define ESM_CAUSE_SYNTACTICAL_ERROR_IN_PACKET_FILTERS 45
#define ESM_CAUSE_PTI_MISMATCH 47
#define ESM_CAUSE_LAST_PDN_DISCONNECTION_NOT_ALLOWED 49
#define ESM_CAUSE_PDN_TYPE_IPV4_ONLY_ALLOWED 50
#define ESM_CAUSE_PDN_TYPE_IPV6_ONLY_ALLOWED 51
#define ESM_CAUSE_SINGLE_ADDRESS_BEARERS_ONLY_ALLOWED 52
#define ESM_CAUSE_ESM_INFORMATION_NOT_RECEIVED 53
#define ESM_CAUSE_PDN_CONNECTION_DOES_NOT_EXIST 54
#define ESM_CAUSE_MULTIPLE_PDN_CONNECTIONS_FOR_A_GIVEN_APN_NOT_ALLOWED 55
#define ESM_CAUSE_COLLISION_WITH_NETWORK_INITIATED_REQUEST 56
#define ESM_CAUSE_PDN_TYPE_IPV4V6_ONLY_ALLOWED 57
#define ESM_CAUSE_PDN_TYPE_NON_IP_ONLY_ALLOWED 58
#define ESM_CAUSE_UNSUPPORTED_QCI_VALUE 59
#define ESM_CAUSE_BEARER_HANDLING_NOT_SUPPORTED 60
#define ESM_CAUSE_MAXIMUM_NUMBER_OF_EPS_BEARERS_REACHED 65
#define ESM_CAUSE_REQUESTED_APN_NOT_SUPPORTED_IN_CURRENT_RAT_AND_PLMN_COMBINATION 66
#define ESM_CAUSE_INVALID_PTI_VALUE 81
#define ESM_CAUSE_APN_RESTRICTION_VALUE_INCOMPATIBLE_WITH_ACTIVE_EPS_BEARER_CONTEXT 112
#define ESM_CAUSE_MULTIPLE_ACCESSES_TO_A_PDN_CONNECTION_NOT_ALLOWED 113
/* B.2 Protocol errors (e.g., unknown message) class */
#define ESM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE 95
#define ESM_CAUSE_INVALID_MANDATORY_INFORMATION 96
#define ESM_CAUSE_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED 97
#define ESM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE 98
#define ESM_CAUSE_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED 99
#define ESM_CAUSE_CONDITIONAL_IE_ERROR 100
#define ESM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE 101
#define ESM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED 111
typedef uint8_t nas_esm_cause_t;

/* 9.9.4.5 ESM information transfer flag
 * O TV 1 */
typedef struct nas_esm_information_transfer_flag_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t security_protected_required:1;)
} __attribute__ ((packed)) nas_esm_information_transfer_flag_t;

/* 9.9.4.6 Linked EPS bearer identity
 * M V 1/2 */
typedef struct nas_linked_eps_bearer_identity_s {
ED2(uint8_t spare:4;,
    uint8_t eps_bearer_identity:4;)
} __attribute__ ((packed)) nas_linked_eps_bearer_identity_t;

/* 9.9.4.7 LLC service access point identifier
 * See subclause 10.5.6.9 in 3GPP TS 24.008 [13].
 * O TV 2 */
typedef uint8_t nas_llc_service_access_point_identifier_t;

/* 9.9.4.8 Packet flow Identifier
 * See subclause 10.5.6.11 in 3GPP TS 24.008 [13].
 * O TLV 3 */
#define NAS_PACKET_FLOW_IDENTIFIER_BEST_EFFORT          0
#define NAS_PACKET_FLOW_IDENTIFIER_SIGNALLING           1
#define NAS_PACKET_FLOW_IDENTIFIER_SMS                  2
#define NAS_PACKET_FLOW_IDENTIFIER_TOM8                 3
#define NAS_PACKET_FLOW_IDENTIFIER_DYNAMIC_ASSIGN_MIN   8
#define NAS_PACKET_FLOW_IDENTIFIER_DYNAMIC_ASSIGN_MAX   0xef
typedef struct nas_packet_flow_identifier_s {
    uint8_t length;
ED2(uint8_t spare:1;,
    uint8_t value:7;)
} __attribute__ ((packed)) nas_packet_flow_identifier_t;

/* 9.9.4.13 Radio priority
 * See subclause 10.5.7.2 in 3GPP TS 24.008 [13].
 * O TV 1 */
typedef struct nas_radio_priority_s {
ED3(uint8_t type:4;,
    uint8_t spare:1;,
    uint8_t value:3;)
} __attribute__ ((packed)) nas_radio_priority_t;

/* 9.9.4.9 PDN address
 * M LV 6-14 */
#define NAS_PDN_ADDRESS_IPV4_LEN 5
#define NAS_PDN_ADDRESS_IPV6_LEN 9
#define NAS_PDN_ADDRESS_IPV4V6_LEN 13
typedef struct nas_pdn_address_s {
    uint8_t length;
ED2(uint8_t reserved:5;,
    uint8_t pdn_type:3;)
    union {
        uint32_t addr;      
        struct {
            uint8_t addr6[IPV6_LEN>>1]; /* Interface Identifer Only */
        };
        struct {
            uint8_t addr6[IPV6_LEN>>1]; /* Interface Identifer Only */
            uint32_t addr;      
        } both;
    };
} __attribute__ ((packed)) nas_pdn_address_t;

/* 9.9.4.11 Protocol configuration options
 * See subclause 10.5.6.3 in 3GPP TS 24.008 [13].
 * O TLV 3-253 */ 
typedef struct nas_protocol_configuration_options_s {
    uint8_t length;
    uint8_t buffer[MAX_PCO_LEN];
} __attribute__ ((packed)) nas_protocol_configuration_options_t;

/* 9.9.4.12 Quality of service
 * See subclause 10.5.6.5 in 3GPP TS 24.008 [13].
 * O TLV 14-22 */
#define NAS_QOS_LEN 20
typedef struct nas_quality_of_service_s {
    uint8_t length;
    uint8_t buffer[NAS_QOS_LEN];
} __attribute__ ((packed)) nas_quality_of_service_t;

/* 9.9.4.13A Re-attempt indicator
 * O TLV 3 */
typedef struct nas_re_attempt_indicator_s {
    uint8_t length;
ED3(uint8_t spare:3;,  /* allowed in A/Gb mode or Iu mode */
    uint8_t eplmnc:1;, /* allowed in an equivalent PLMN */
    uint8_t ratc:1;) 
} __attribute__ ((packed)) nas_re_attempt_indicator_t;

/* 9.9.4.14 Request type
 * M V 1/2
 * See subclause 10.5.6.17 in 3GPP TS 24.008 [13].
 * 9.9.4.10 PDN type "PDN type
 * M V 1/2 */
#define NAS_PDN_CONNECTIVITY_REQUEST_TYPE_INITIAL   1
#define NAS_PDN_CONNECTIVITY_REQUEST_TYPE_HANDOVER  2
#define NAS_PDN_CONNECTIVITY_REQUEST_TYPE_EMERGENCY 4
#define NAS_PDN_CONNECTIVITY_PDN_TYPE_IPV4          1
#define NAS_PDN_CONNECTIVITY_PDN_TYPE_IPV6          2
#define NAS_PDN_CONNECTIVITY_PDN_TYPE_IPV4V6        3
#define NAS_PDN_CONNECTIVITY_PDN_TYPE_NON_IP        5
typedef struct nas_request_type_s {
ED4(uint8_t spare1:1;,
    uint8_t pdn_type:3;,
    uint8_t spare2:1;,
    uint8_t request_type:3;)
} __attribute__ ((packed)) nas_request_type_t;

/* 9.9.4.15 Traffic flow aggregate description
 * see subclause 10.5.6.12 in 3GPP TS 24.008 [13]
 * M LV 2-256 */
#define NAS_MAX_TRAFFIC_FLOW_AGGREGATE_DESCRIPTION 255
typedef struct nas_traffic_flow_aggregate_description_s {
    uint8_t length;
    uint8_t buffer[NAS_MAX_TRAFFIC_FLOW_AGGREGATE_DESCRIPTION];
} __attribute__ ((packed)) nas_traffic_flow_aggregate_description_t;

/* 9.9.4.16 Traffic flow template
 * See subclause 10.5.6.12 in 3GPP TS 24.008 [13].
 * M LV 2-256 */
#define NAS_MAX_TRAFFIC_FLOW_TEMPLATE 255
typedef struct nas_traffic_flow_template_s {
    uint8_t length;
    uint8_t buffer[NAS_MAX_TRAFFIC_FLOW_TEMPLATE];
} __attribute__ ((packed)) nas_traffic_flow_template_t;

/* 9.9.4.17 Transaction identifier
 * 3GPP TS 24.008 [13], subclause 10.5.6.7.
 * O TLV 3-4 */
typedef struct nas_transaction_identifier_s {
    uint8_t length;
    uint16_t linked_ti;
} __attribute__ ((packed)) nas_transaction_identifier_t;

/* 9.9.4.18 WLAN offload acceptability
 * 3GPP TS 24.008 [13], subclause 10.5.6.20
 * O TV 1 */
typedef struct nas_wlan_offload_acceptability_s {
ED4(uint8_t type:4;,
    uint8_t spare:2;,
    uint8_t utran_via_wlan_acceptable:1;,
    uint8_t e_utran_via_wlan_acceptable:1;)
} __attribute__ ((packed)) nas_wlan_offload_acceptability_t;

/* 9.9.4.19 NBIFOM container
 * See subclause 10.5.6.21 in 3GPP TS 24.008 [4].
 * O TLV 3-257 */
#define MAX_NAS_NBIFOM_CONTAINER_LEN 255
typedef struct nas_nbifom_container_s {
    uint8_t length;
    uint8_t buffer[MAX_NAS_NBIFOM_CONTAINER_LEN];
} __attribute__ ((packed)) nas_nbifom_container_t;

/* 9.9.4.22 Header compression configuration
 * O TLV 5-257 */
#define MAX_NAS_HEADER_COMPRESSION_CONTAINER_LEN 251
typedef struct nas_header_compression_configuration_s {
    uint8_t length;
ED8(uint8_t spare:1;,
    uint8_t profile0x0104:1;,
    uint8_t profile0x0103:1;,
    uint8_t profile0x0102:1;,
    uint8_t profile0x0006:1;,
    uint8_t profile0x0004:1;,
    uint8_t profile0x0003:1;,
    uint8_t profile0x0002:1;)
    uint16_t max_cid;
#define NAS_HEADER_COMPRESSION_NO_COMPRESSION           0
#define NAS_HEADER_COMPRESSION_PROFILE_0x0002_UDP_IP    1
#define NAS_HEADER_COMPRESSION_PROFILE_0x0003_ESP_IP    2
#define NAS_HEADER_COMPRESSION_PROFILE_0x0004           3
#define NAS_HEADER_COMPRESSION_PROFILE_0x0006           4
#define NAS_HEADER_COMPRESSION_PROFILE_0x0102_UDP_IP    5
#define NAS_HEADER_COMPRESSION_PROFILE_0x0103_ESP_IP    6
#define NAS_HEADER_COMPRESSION_PROFILE_0x0104_IP        7
    uint8_t type;
    uint8_t container[MAX_NAS_NBIFOM_CONTAINER_LEN];
} __attribute__ ((packed)) nas_header_compression_configuration_t;

/* 9.9.4.23 Control plane only indication
 * O TV 1 */
typedef struct nas_control_plane_only_indication_s {
ED3(uint8_t type:4;,
    uint8_t spare:3;,
    uint8_t ciot_eps_optimization:1;)
} __attribute__ ((packed)) nas_control_plane_only_indication_t;

/* 9.9.4.26 Extended protocol configuration options
 * O TLV-E 4-65538 */
typedef struct nas_extended_protocol_configuration_options_s {
    uint16_t length;
    uint8_t *buffer;
} __attribute__ ((packed)) nas_extended_protocol_configuration_options_t;

/* 9.9.4.27 Header compression configuration status
 * O TLV 4 */
typedef struct nas_header_compression_configuration_status_s {
    uint8_t length;
    uint16_t value;
} __attribute__ ((packed)) nas_header_compression_configuration_status_t;

/* 9.9.4.28 Serving PLMN rate control
 * O TLV 4 */
typedef struct nas_serving_plmn_rate_control_s {
    uint8_t length;
ED8(uint8_t ebi7:1;,
    uint8_t ebi6:1;,
    uint8_t ebi5:1;,
    uint8_t ebi4:1;,
    uint8_t ebi3:1;,
    uint8_t ebi2:1;,
    uint8_t ebi1:1;,
    uint8_t ebi0:1;)
ED8(uint8_t ebi15:1;,
    uint8_t ebi14:1;,
    uint8_t ebi13:1;,
    uint8_t ebi12:1;,
    uint8_t ebi11:1;,
    uint8_t ebi10:1;,
    uint8_t ebi9:1;,
    uint8_t ebi8:1;)
} __attribute__ ((packed)) nas_serving_plmn_rate_control_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAS_TYPES_H__ */

