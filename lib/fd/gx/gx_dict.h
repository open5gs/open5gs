#ifndef __GX_DICT_H__
#define __GX_DICT_H__

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GX_APPLICATION_ID 16777238

#define GX_AVP_CODE_RE_AUTH_REQUEST_TYPE            (285)
#define GX_AVP_CODE_CC_REQUEST_NUMBER               (415)
#define GX_AVP_CODE_CC_REQUEST_TYPE                 (416)
#define GX_AVP_CODE_DEFAULT_EPS_BEARER_QOS          (1049)
#define GX_AVP_CODE_SUPPORTED_FEATURES              (628)
#define GX_AVP_CODE_CHARGING_RULE_INSTALL           (1001)
#define GX_AVP_CODE_CHARGING_RULE_REMOVE            (1002)
#define GX_AVP_CODE_CHARGING_RULE_DEFINITION        (1003)
#define GX_AVP_CODE_CHARGING_RULE_NAME              (1005)
#define GX_AVP_CODE_FLOW_INFORMATION                (1058)
#define GX_AVP_CODE_FLOW_STATUS                     (511)
#define GX_AVP_CODE_QOS_INFORMATION                 (1016)
#define GX_AVP_CODE_PRECEDENCE                      (1010)

extern struct dict_object *gx_application;

extern struct dict_object *gx_cmd_ccr;
extern struct dict_object *gx_cmd_cca;
extern struct dict_object *gx_cmd_rar;
extern struct dict_object *gx_cmd_raa;

extern struct dict_object *gx_cc_request_type;
extern struct dict_object *gx_cc_request_number;
extern struct dict_object *gx_network_request_support;
extern struct dict_object *gx_subscription_id;
#define GX_SUBSCRIPTION_ID_TYPE_END_USER_E164        0
#define GX_SUBSCRIPTION_ID_TYPE_END_USER_IMSI        1
#define GX_SUBSCRIPTION_ID_TYPE_END_USER_SIP_URI     2
#define GX_SUBSCRIPTION_ID_TYPE_END_USER_NAI         3
extern struct dict_object *gx_subscription_id_type;
extern struct dict_object *gx_subscription_id_data;
extern struct dict_object *gx_supported_features;
extern struct dict_object *gx_feature_list_id;
extern struct dict_object *gx_feature_list;
extern struct dict_object *gx_framed_ip_address;
extern struct dict_object *gx_framed_ipv6_prefix;
#define GX_IP_CAN_TYPE_3GPP_GPRS        0
#define GX_IP_CAN_TYPE_DOCSIS           1
#define GX_IP_CAN_TYPE_xDSL             2
#define GX_IP_CAN_TYPE_WiMAX            3
#define GX_IP_CAN_TYPE_3GPP2            4
#define GX_IP_CAN_TYPE_3GPP_EPS         5
#define GX_IP_CAN_TYPE_Non_3GPP_EPS     6
extern struct dict_object *gx_ip_can_type;
#define GX_RAT_TYPE_WLAN                0
#define GX_RAT_TYPE_VIRTUAL             1 
#define GX_RAT_TYPE_UTRAN               1000
#define GX_RAT_TYPE_GERAN               1001
#define GX_RAT_TYPE_GAN                 1002
#define GX_RAT_TYPE_HSPA_EVOLUTION      1003
#define GX_RAT_TYPE_EUTRAN              1004
#define GX_RAT_TYPE_EUTRAN_NB_IoT       1005
#define GX_RAT_TYPE_CDMA2000_1X         2000
#define GX_RAT_TYPE_HRPD                2001
#define GX_RAT_TYPE_UMB                 2002
#define GX_RAT_TYPE_EHRPD               2003
extern struct dict_object *gx_rat_type;
extern struct dict_object *gx_qos_information;
extern struct dict_object *gx_qos_class_identifier;
extern struct dict_object *gx_max_requested_bandwidth_ul;
extern struct dict_object *gx_max_requested_bandwidth_dl;
extern struct dict_object *gx_guaranteed_bitrate_ul;
extern struct dict_object *gx_guaranteed_bitrate_dl;
extern struct dict_object *gx_allocation_retention_priority;
extern struct dict_object *gx_priority_level;
extern struct dict_object *gx_pre_emption_capability;
extern struct dict_object *gx_pre_emption_vulnerability;
extern struct dict_object *gx_apn_aggregate_max_bitrate_ul;
extern struct dict_object *gx_apn_aggregate_max_bitrate_dl;
#define GX_3GPP_USER_LOCATION_INFO_TYPE_TAI_AND_ECGI 130
extern struct dict_object *gx_3gpp_user_location_info;
extern struct dict_object *gx_called_station_id;
extern struct dict_object *gx_default_eps_bearer_qos;
extern struct dict_object *gx_3gpp_ms_timezone;
extern struct dict_object *gx_event_trigger;
extern struct dict_object *gx_bearer_control_mode;
extern struct dict_object *gx_charging_rule_install;
extern struct dict_object *gx_charging_rule_remove;
extern struct dict_object *gx_charging_rule_definition;
extern struct dict_object *gx_charging_rule_base_name;
extern struct dict_object *gx_charging_rule_name;
extern struct dict_object *gx_flow_information;
extern struct dict_object *gx_flow_direction;
extern struct dict_object *gx_flow_description;
extern struct dict_object *gx_flow_status;
extern struct dict_object *gx_precedence;
extern struct dict_object *gx_flows;
extern struct dict_object *gx_media_component_description;
extern struct dict_object *gx_media_component_number;
extern struct dict_object *gx_media_type;
extern struct dict_object *gx_rr_bandwidth;
extern struct dict_object *gx_rs_bandwidth;
extern struct dict_object *gx_codec_data;
extern struct dict_object *gx_media_sub_component;
extern struct dict_object *gx_flow_number;
extern struct dict_object *gx_flow_usage;

int gx_dict_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GX_DICT_H__ */
