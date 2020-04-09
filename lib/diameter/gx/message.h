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

#if !defined(OGS_DIAMETER_INSIDE) && !defined(OGS_DIAMETER_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_DIAM_GX_MESSAGE_H
#define OGS_DIAM_GX_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_DIAM_GX_APPLICATION_ID 16777238

#define OGS_DIAM_GX_AVP_CODE_RE_AUTH_REQUEST_TYPE           (285)
#define OGS_DIAM_GX_AVP_CODE_CC_REQUEST_NUMBER              (415)
#define OGS_DIAM_GX_AVP_CODE_CC_REQUEST_TYPE                (416)
#define OGS_DIAM_GX_AVP_CODE_DEFAULT_EPS_BEARER_QOS         (1049)
#define OGS_DIAM_GX_AVP_CODE_SUPPORTED_FEATURES             (628)
#define OGS_DIAM_GX_AVP_CODE_CHARGING_RULE_INSTALL          (1001)
#define OGS_DIAM_GX_AVP_CODE_CHARGING_RULE_REMOVE           (1002)
#define OGS_DIAM_GX_AVP_CODE_CHARGING_RULE_DEFINITION       (1003)
#define OGS_DIAM_GX_AVP_CODE_CHARGING_RULE_NAME             (1005)
#define OGS_DIAM_GX_AVP_CODE_FLOW_INFORMATION               (1058)
#define OGS_DIAM_GX_AVP_CODE_FLOW_STATUS                    (511)
#define OGS_DIAM_GX_AVP_CODE_QOS_INFORMATION                (1016)
#define OGS_DIAM_GX_AVP_CODE_PRECEDENCE                     (1010)

extern struct dict_object *ogs_diam_gx_application;

extern struct dict_object *ogs_diam_gx_cmd_ccr;
extern struct dict_object *ogs_diam_gx_cmd_cca;
extern struct dict_object *ogs_diam_gx_cmd_rar;
extern struct dict_object *ogs_diam_gx_cmd_raa;

extern struct dict_object *ogs_diam_gx_cc_request_type;
extern struct dict_object *ogs_diam_gx_cc_request_number;
extern struct dict_object *ogs_diam_gx_network_request_support;
extern struct dict_object *ogs_diam_gx_subscription_id;
#define OGS_DIAM_GX_SUBSCRIPTION_ID_TYPE_END_USER_E164      0
#define OGS_DIAM_GX_SUBSCRIPTION_ID_TYPE_END_USER_IMSI      1
#define OGS_DIAM_GX_SUBSCRIPTION_ID_TYPE_END_USER_SIP_URI   2
#define OGS_DIAM_GX_SUBSCRIPTION_ID_TYPE_END_USER_NAI       3
extern struct dict_object *ogs_diam_gx_subscription_id_type;
extern struct dict_object *ogs_diam_gx_subscription_id_data;
extern struct dict_object *ogs_diam_gx_supported_features;
extern struct dict_object *ogs_diam_gx_feature_list_id;
extern struct dict_object *ogs_diam_gx_feature_list;
extern struct dict_object *ogs_diam_gx_framed_ip_address;
extern struct dict_object *ogs_diam_gx_framed_ipv6_prefix;
#define OGS_DIAM_GX_IP_CAN_TYPE_3GPP_GPRS                   0
#define OGS_DIAM_GX_IP_CAN_TYPE_DOCSIS                      1
#define OGS_DIAM_GX_IP_CAN_TYPE_xDSL                        2
#define OGS_DIAM_GX_IP_CAN_TYPE_WiMAX                       3
#define OGS_DIAM_GX_IP_CAN_TYPE_3GPP2                       4
#define OGS_DIAM_GX_IP_CAN_TYPE_3GPP_EPS                    5
#define OGS_DIAM_GX_IP_CAN_TYPE_Non_3GPP_EPS                6
extern struct dict_object *ogs_diam_gx_ip_can_type;
#define OGS_DIAM_GX_RAT_TYPE_WLAN                           0
#define OGS_DIAM_GX_RAT_TYPE_VIRTUAL                        1 
#define OGS_DIAM_GX_RAT_TYPE_UTRAN                          1000
#define OGS_DIAM_GX_RAT_TYPE_GERAN                          1001
#define OGS_DIAM_GX_RAT_TYPE_GAN                            1002
#define OGS_DIAM_GX_RAT_TYPE_HSPA_EVOLUTION                 1003
#define OGS_DIAM_GX_RAT_TYPE_EUTRAN                         1004
#define OGS_DIAM_GX_RAT_TYPE_EUTRAN_NB_IoT                  1005
#define OGS_DIAM_GX_RAT_TYPE_CDMA2000_1X                    2000
#define OGS_DIAM_GX_RAT_TYPE_HRPD                           2001
#define OGS_DIAM_GX_RAT_TYPE_UMB                            2002
#define OGS_DIAM_GX_RAT_TYPE_EHRPD                          2003
extern struct dict_object *ogs_diam_gx_rat_type;
extern struct dict_object *ogs_diam_gx_qos_information;
extern struct dict_object *ogs_diam_gx_qos_class_identifier;
extern struct dict_object *ogs_diam_gx_max_requested_bandwidth_ul;
extern struct dict_object *ogs_diam_gx_max_requested_bandwidth_dl;
extern struct dict_object *ogs_diam_gx_guaranteed_bitrate_ul;
extern struct dict_object *ogs_diam_gx_guaranteed_bitrate_dl;
extern struct dict_object *ogs_diam_gx_allocation_retention_priority;
extern struct dict_object *ogs_diam_gx_priority_level;
extern struct dict_object *ogs_diam_gx_pre_emption_capability;
extern struct dict_object *ogs_diam_gx_pre_emption_vulnerability;
extern struct dict_object *ogs_diam_gx_apn_aggregate_max_bitrate_ul;
extern struct dict_object *ogs_diam_gx_apn_aggregate_max_bitrate_dl;
#define OGS_DIAM_GX_3GPP_USER_LOCATION_INFO_TYPE_TAI            128
#define OGS_DIAM_GX_3GPP_USER_LOCATION_INFO_TYPE_ECGI           129
#define OGS_DIAM_GX_3GPP_USER_LOCATION_INFO_TYPE_TAI_AND_ECGI   130
extern struct dict_object *ogs_diam_gx_3gpp_user_location_info;
extern struct dict_object *ogs_diam_gx_called_station_id;
extern struct dict_object *ogs_diam_gx_default_eps_bearer_qos;
extern struct dict_object *ogs_diam_gx_3gpp_ms_timezone;
extern struct dict_object *ogs_diam_gx_event_trigger;
extern struct dict_object *ogs_diam_gx_bearer_control_mode;
extern struct dict_object *ogs_diam_gx_charging_rule_install;
extern struct dict_object *ogs_diam_gx_charging_rule_remove;
extern struct dict_object *ogs_diam_gx_charging_rule_definition;
extern struct dict_object *ogs_diam_gx_charging_rule_base_name;
extern struct dict_object *ogs_diam_gx_charging_rule_name;
extern struct dict_object *ogs_diam_gx_flow_information;
extern struct dict_object *ogs_diam_gx_flow_direction;
extern struct dict_object *ogs_diam_gx_flow_description;
extern struct dict_object *ogs_diam_gx_flow_status;
extern struct dict_object *ogs_diam_gx_precedence;
extern struct dict_object *ogs_diam_gx_flows;
extern struct dict_object *ogs_diam_gx_media_component_description;
extern struct dict_object *ogs_diam_gx_media_component_number;
extern struct dict_object *ogs_diam_gx_media_type;
extern struct dict_object *ogs_diam_gx_rr_bandwidth;
extern struct dict_object *ogs_diam_gx_rs_bandwidth;
extern struct dict_object *ogs_diam_gx_codec_data;
extern struct dict_object *ogs_diam_gx_media_sub_component;
extern struct dict_object *ogs_diam_gx_flow_number;
extern struct dict_object *ogs_diam_gx_flow_usage;

typedef struct ogs_diam_gx_message_s {
#define OGS_DIAM_GX_CMD_CODE_CREDIT_CONTROL                         272
#define OGS_DIAM_GX_CMD_RE_AUTH                                     258
    uint16_t            cmd_code;

/* Experimental-Result-Codes */
#define OGS_DIAM_GX_DIAMETER_ERROR_LATE_OVERLAPPING_REQUEST         5453
#define OGS_DIAM_GX_DIAMETER_ERROR_TIMED_OUT_REQUEST                5454
#define OGS_DIAM_GX_DIAMETER_ERROR_INITIAL_PARAMETERS               5140
#define OGS_DIAM_GX_DIAMETER_ERROR_TRIGGER_EVENT                    5141
#define OGS_DIAM_GX_DIAMETER_PCC_RULE_EVENT                         5142
#define OGS_DIAM_GX_DIAMETER_ERROR_BEARER_NOT_AUTHORIZED            5143
#define OGS_DIAM_GX_DIAMETER_ERROR_TRAFFIC_MAPPING_INFO_REJECTED    5144
#define OGS_DIAM_GX_DIAMETER_ERROR_CONFLICTING_REQUEST              5147
#define OGS_DIAM_GX_DIAMETER_ADC_RULE_EVENT                         5148
#define OGS_DIAM_GX_DIAMETER_ERROR_NBIFOM_NOT_AUTHORIZED            5149
    uint32_t            result_code;

#define OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST                 1
#define OGS_DIAM_GX_CC_REQUEST_TYPE_UPDATE_REQUEST                  2
#define OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST             3
#define OGS_DIAM_GX_CC_REQUEST_TYPE_EVENT_REQUEST                   4
    uint32_t            cc_request_type;

    ogs_pdn_t           pdn;
    ogs_pcc_rule_t      pcc_rule[OGS_MAX_NUM_OF_PCC_RULE];
    int                 num_of_pcc_rule;
} ogs_diam_gx_message_t;

int ogs_diam_gx_init(void);
void ogs_diam_gx_message_free(ogs_diam_gx_message_t *gx_message);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_GX_MESSAGE_H */
