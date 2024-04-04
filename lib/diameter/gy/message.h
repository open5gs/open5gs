/* Gy Interface, 3GPP TS 32.299
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#if !defined(OGS_DIAMETER_INSIDE) && !defined(OGS_DIAMETER_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_DIAM_GY_MESSAGE_H
#define OGS_DIAM_GY_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Gy interface, 3GPP TS 32.299 */

#define OGS_DIAM_GY_APPLICATION_ID 4

#define OGS_DIAM_GY_AVP_CODE_RE_AUTH_REQUEST_TYPE           (285)
#define OGS_DIAM_GY_AVP_CODE_CC_INPUT_OCTETS                (412)
#define OGS_DIAM_GY_AVP_CODE_CC_OUTPUT_OCTETS               (414)
#define OGS_DIAM_GY_AVP_CODE_CC_REQUEST_NUMBER              (415)
#define OGS_DIAM_GY_AVP_CODE_CC_REQUEST_TYPE                (416)
#define OGS_DIAM_GY_AVP_CODE_CC_TIME                        (420)
#define OGS_DIAM_GY_AVP_CODE_CC_TOTAL_OCTETS                (421)
#define OGS_DIAM_GY_AVP_CODE_FINAL_UNIT_INDICATION          (430)
#define OGS_DIAM_GY_AVP_CODE_GRANTED_SERVICE_UNIT           (431)
#define OGS_DIAM_GY_AVP_CODE_VALIDITY_TIME                  (448)
#define OGS_DIAM_GY_AVP_CODE_FINAL_UNIT_ACTION              (449)
#define OGS_DIAM_GY_AVP_CODE_MULTIPLE_SERVICES_CREDIT_CONTROL (456)
#define OGS_DIAM_GY_AVP_CODE_SUPPORTED_FEATURES             (628)
#define OGS_DIAM_GY_AVP_CODE_TIME_QUOTA_THRESHOLD           (868)
#define OGS_DIAM_GY_AVP_CODE_VOLUME_QUOTA_THRESHOLD         (869)
#define OGS_DIAM_GY_AVP_CODE_CHARGING_RULE_BASE_NAME        (1004)
#define OGS_DIAM_GY_AVP_CODE_FLOW_INFORMATION               (1058)
#define OGS_DIAM_GY_AVP_CODE_QOS_INFORMATION                (1016)

extern struct dict_object *ogs_diam_gy_application;

extern struct dict_object *ogs_diam_gy_cmd_ccr;
extern struct dict_object *ogs_diam_gy_cmd_cca;
extern struct dict_object *ogs_diam_gy_cmd_rar;
extern struct dict_object *ogs_diam_gy_cmd_raa;

extern struct dict_object *ogs_diam_gy_cc_request_type;
extern struct dict_object *ogs_diam_gy_cc_request_number;
extern struct dict_object *ogs_diam_gy_requested_action;
#define OGS_DIAM_GY_REQUESTED_ACTION_DIRECT_DEBITING   0
#define OGS_DIAM_GY_REQUESTED_ACTION_REFUND_ACCOUNT    1
#define OGS_DIAM_GY_REQUESTED_ACTION_CHECK_BALANCE     2
#define OGS_DIAM_GY_REQUESTED_ACTION_PRICE_ENQUIRY     3
extern struct dict_object *ogs_diam_gy_aoc_request_type;
#define OGS_DIAM_GY_AoC_NOT_REQUESTED   0
#define OGS_DIAM_GY_AoC_FULL            1
#define OGS_DIAM_GY_AoC_COST_ONLY       2
#define OGS_DIAM_GY_AoC_TARIFF_ONLY     3
extern struct dict_object *ogs_diam_gy_multiple_services_ind;
#define OGS_DIAM_GY_MULTIPLE_SERVICES_NOT_SUPPORTED 0
#define OGS_DIAM_GY_MULTIPLE_SERVICES_SUPPORTED 1
extern struct dict_object *ogs_diam_gy_multiple_services_cc;
extern struct dict_object *ogs_diam_gy_requested_service_unit;
extern struct dict_object *ogs_diam_gy_used_service_unit;
extern struct dict_object *ogs_diam_gy_cc_time;
extern struct dict_object *ogs_diam_gy_cc_total_octets;
extern struct dict_object *ogs_diam_gy_cc_input_octets;
extern struct dict_object *ogs_diam_gy_cc_output_octets;
extern struct dict_object *ogs_diam_gy_cc_service_specific_units;
extern struct dict_object *ogs_diam_gy_reporting_reason;
#define OGS_DIAM_GY_REPORTING_REASON_THRESHOLD                  0
#define OGS_DIAM_GY_REPORTING_REASON_QHT                        1
#define OGS_DIAM_GY_REPORTING_REASON_FINAL                      2
#define OGS_DIAM_GY_REPORTING_REASON_QUOTA_EXHAUSTED            3
#define OGS_DIAM_GY_REPORTING_REASON_VALIDITY_TIME              4
#define OGS_DIAM_GY_REPORTING_REASON_OTHER_QUOTA_TYPE           5
#define OGS_DIAM_GY_REPORTING_REASON_RATING_CONDITION_CHANGE    6
#define OGS_DIAM_GY_REPORTING_REASON_FORCED_REAUTHORISATION     7
#define OGS_DIAM_GY_REPORTING_REASON_POOL_EXHAUSTED             8
#define OGS_DIAM_GY_REPORTING_REASON_UNUSED_QUOTA_TIMER         9
extern struct dict_object *ogs_diam_gy_service_id;
extern struct dict_object *ogs_diam_gy_service_information;
extern struct dict_object *ogs_diam_gy_ps_information;
extern struct dict_object *ogs_diam_gy_3gpp_charging_id;
extern struct dict_object *ogs_diam_gy_3gpp_pdp_type;
#define OGS_DIAM_GY_3GPP_PDP_TYPE_IPv4     0
#define OGS_DIAM_GY_3GPP_PDP_TYPE_PPP      1
#define OGS_DIAM_GY_3GPP_PDP_TYPE_IPv6     2
#define OGS_DIAM_GY_3GPP_PDP_TYPE_IPv4v6   3
#define OGS_DIAM_GY_3GPP_PDP_TYPE_NON_IP   4
#define OGS_DIAM_GY_3GPP_PDP_TYPE_UNSTRUCTURED 5
#define OGS_DIAM_GY_3GPP_PDP_TYPE_ETHERNET 6
extern struct dict_object *ogs_diam_gy_pdp_address;
extern struct dict_object *ogs_diam_gy_sgsn_address;
extern struct dict_object *ogs_diam_gy_ggsn_address;
extern struct dict_object *ogs_diam_gy_3gpp_nsapi;
extern struct dict_object *ogs_diam_gy_3gpp_selection_mode;
extern struct dict_object *ogs_diam_gy_3gpp_charging_characteristics;
extern struct dict_object *ogs_diam_gy_user_equipment_info;
extern struct dict_object *ogs_diam_gy_user_equipment_info_type;
extern struct dict_object *ogs_diam_gy_user_equipment_info_value;
extern struct dict_object *ogs_diam_gy_feature_list_id;
extern struct dict_object *ogs_diam_gy_feature_list;
extern struct dict_object *ogs_diam_gy_qos_information;
extern struct dict_object *ogs_diam_gy_qos_class_identifier;
extern struct dict_object *ogs_diam_gy_max_requested_bandwidth_ul;
extern struct dict_object *ogs_diam_gy_max_requested_bandwidth_dl;
extern struct dict_object *ogs_diam_gy_guaranteed_bitrate_ul;
extern struct dict_object *ogs_diam_gy_guaranteed_bitrate_dl;
extern struct dict_object *ogs_diam_gy_allocation_retention_priority;
extern struct dict_object *ogs_diam_gy_priority_level;
extern struct dict_object *ogs_diam_gy_pre_emption_capability;
extern struct dict_object *ogs_diam_gy_pre_emption_vulnerability;
extern struct dict_object *ogs_diam_gy_apn_aggregate_max_bitrate_ul;
extern struct dict_object *ogs_diam_gy_apn_aggregate_max_bitrate_dl;
extern struct dict_object *ogs_diam_gy_3gpp_rat_type;
#define OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_CGI                0
#define OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_SAI                1
#define OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_RAI                2
#define OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_TAI                128
#define OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_ECGI               129
#define OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_TAI_AND_ECGI       130
#define OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_ENODEB_ID          131
#define OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_TAI_AND_ENODEB_ID  132
#define OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_EXT_ENODEB_ID      133
#define OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_TAI_AND_EXT_ENODEB_ID 134
extern struct dict_object *ogs_diam_gy_3gpp_user_location_info;
extern struct dict_object *ogs_diam_gy_called_station_id;
extern struct dict_object *ogs_diam_gy_3gpp_ms_timezone;
extern struct dict_object *ogs_diam_gy_charging_rule_base_name;;
extern struct dict_object *ogs_diam_gy_flows;
extern struct dict_object *ogs_diam_gy_3gpp_sgsn_mcc_mnc;

typedef struct ogs_diam_gy_service_unit_s {
    bool cc_time_present;
    uint32_t cc_time;
    bool cc_total_octets_present;
    uint64_t cc_total_octets;
    bool cc_input_octets_present;
    uint64_t cc_input_octets;
    bool cc_output_octets_present;
    uint64_t cc_output_octets;
} ogs_diam_gy_service_unit_t;

typedef struct gs_diam_gy_final_unit_s {
    bool cc_final_action_present;
#define OGS_DIAM_GY_FINAL_UNIT_ACTION_TERMINATE                 0
#define OGS_DIAM_GY_FINAL_UNIT_ACTION_REDIRECT                  1
#define OGS_DIAM_GY_FINAL_UNIT_ACTION_REDIRECT_ACCESS           2
    int32_t cc_final_action;
} ogs_diam_gy_final_unit_t;

typedef struct ogs_diam_gy_message_s {
#define OGS_DIAM_GY_CMD_CODE_CREDIT_CONTROL                         272
#define OGS_DIAM_GY_CMD_RE_AUTH                                     258
    uint16_t            cmd_code;

/* Experimental-Result-Codes */
#define OGS_DIAM_GY_DIAMETER_ERROR_LATE_OVERLAPPING_REQUEST         5453
#define OGS_DIAM_GY_DIAMETER_ERROR_TIMED_OUT_REQUEST                5454
#define OGS_DIAM_GY_DIAMETER_ERROR_INITIAL_PARAMETERS               5140
#define OGS_DIAM_GY_DIAMETER_ERROR_TRIGGER_EVENT                    5141
#define OGS_DIAM_GY_DIAMETER_PCC_RULE_EVENT                         5142
#define OGS_DIAM_GY_DIAMETER_ERROR_BEARER_NOT_AUTHORIZED            5143
#define OGS_DIAM_GY_DIAMETER_ERROR_TRAFFIC_MAPPING_INFO_REJECTED    5144
#define OGS_DIAM_GY_DIAMETER_ERROR_CONFLICTING_REQUEST              5147
#define OGS_DIAM_GY_DIAMETER_ADC_RULE_EVENT                         5148
#define OGS_DIAM_GY_DIAMETER_ERROR_NBIFOM_NOT_AUTHORIZED            5149
    uint32_t            result_code;
    uint32_t            *err;
    uint32_t            *exp_err;

#define OGS_DIAM_GY_CC_REQUEST_TYPE_INITIAL_REQUEST                 1
#define OGS_DIAM_GY_CC_REQUEST_TYPE_UPDATE_REQUEST                  2
#define OGS_DIAM_GY_CC_REQUEST_TYPE_TERMINATION_REQUEST             3
#define OGS_DIAM_GY_CC_REQUEST_TYPE_EVENT_REQUEST                   4
    uint32_t            cc_request_type;

    union {
        struct {
            uint32_t validity_time;
            uint32_t time_threshold;
            uint32_t volume_threshold;
            ogs_diam_gy_service_unit_t granted;
            ogs_diam_gy_final_unit_t final;
            uint32_t result_code;
            uint32_t *err;
        } cca;
    };
} ogs_diam_gy_message_t;

int ogs_diam_gy_init(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_GY_MESSAGE_H */
