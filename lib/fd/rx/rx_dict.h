#ifndef __RX_DICT_H__
#define __RX_DICT_H__

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RX_APPLICATION_ID 16777236

#define RX_AVP_CODE_SUBSCRIPTION_ID                 (443)
#define RX_AVP_CODE_SPECIFIC_ACTION                 (513)

#define RX_AVP_CODE_MEDIA_COMPONENT_DESCRIPTION     (517)
#define RX_AVP_CODE_MEDIA_TYPE                      (520)
#define RX_AVP_CODE_MAX_REQUESTED_BANDWIDTH_DL      (515)
#define RX_AVP_CODE_MAX_REQUESTED_BANDWIDTH_UL      (516)
#define RX_AVP_CODE_RR_BANDWIDTH                    (521)
#define RX_AVP_CODE_RS_BANDWIDTH                    (522)
#define RX_AVP_CODE_MIN_REQUESTED_BANDWIDTH_DL      (534)
#define RX_AVP_CODE_MIN_REQUESTED_BANDWIDTH_UL      (535)
#define RX_AVP_CODE_MEDIA_COMPONENT_NUMBER          (518)

#define RX_AVP_CODE_MEDIA_SUB_COMPONENT             (519)
#define RX_AVP_CODE_FLOW_DESCRIPTION                (507)
#define RX_AVP_CODE_FLOW_NUMBER                     (509)
#define RX_AVP_CODE_FLOW_USAGE                      (512)

extern struct dict_object *rx_application;

extern struct dict_object *rx_cmd_aar;
extern struct dict_object *rx_cmd_aaa;
extern struct dict_object *rx_cmd_asr;
extern struct dict_object *rx_cmd_asa;
extern struct dict_object *rx_cmd_str;
extern struct dict_object *rx_cmd_sta;

extern struct dict_object *rx_media_component_description;
extern struct dict_object *rx_media_component_number;
extern struct dict_object *rx_media_type;
extern struct dict_object *rx_max_requested_bandwidth_ul;
extern struct dict_object *rx_max_requested_bandwidth_dl;
extern struct dict_object *rx_min_requested_bandwidth_ul;
extern struct dict_object *rx_min_requested_bandwidth_dl;
extern struct dict_object *rx_rr_bandwidth;
extern struct dict_object *rx_rs_bandwidth;
#define RX_FLOW_STATUS_ENABLED_UPLINK       0
#define RX_FLOW_STATUS_ENABLED_DOWNLINK     1
#define RX_FLOW_STATUS_ENABLED              2
#define RX_FLOW_STATUS_DISABLED             3
extern struct dict_object *rx_flow_status;
extern struct dict_object *rx_codec_data;
extern struct dict_object *rx_media_sub_component;
extern struct dict_object *rx_flow_number;
extern struct dict_object *rx_flow_usage;
extern struct dict_object *rx_flow_description;
extern struct dict_object *rx_subscription_id;
#define RX_SUBSCRIPTION_ID_TYPE_END_USER_E164        0
#define RX_SUBSCRIPTION_ID_TYPE_END_USER_IMSI        1
#define RX_SUBSCRIPTION_ID_TYPE_END_USER_SIP_URI     2
#define RX_SUBSCRIPTION_ID_TYPE_END_USER_NAI         3
extern struct dict_object *rx_subscription_id_type;
extern struct dict_object *rx_subscription_id_data;
extern struct dict_object *rx_specific_action;
extern struct dict_object *rx_framed_ip_address;
extern struct dict_object *rx_framed_ipv6_prefix;
#define RX_IP_CAN_TYPE_3GPP_GPRS        0
#define RX_IP_CAN_TYPE_DOCSIS           1
#define RX_IP_CAN_TYPE_xDSL             2
#define RX_IP_CAN_TYPE_WiMAX            3
#define RX_IP_CAN_TYPE_3GPP2            4
#define RX_IP_CAN_TYPE_3GPP_EPS         5
#define RX_IP_CAN_TYPE_Non_3GPP_EPS     6
extern struct dict_object *rx_ip_can_type;
#define RX_RAT_TYPE_WLAN                0
#define RX_RAT_TYPE_VIRTUAL             1 
#define RX_RAT_TYPE_UTRAN               1000
#define RX_RAT_TYPE_GERAN               1001
#define RX_RAT_TYPE_GAN                 1002
#define RX_RAT_TYPE_HSPA_EVOLUTION      1003
#define RX_RAT_TYPE_EUTRAN              1004
#define RX_RAT_TYPE_EUTRAN_NB_IoT       1005
#define RX_RAT_TYPE_CDMA2000_1X         2000
#define RX_RAT_TYPE_HRPD                2001
#define RX_RAT_TYPE_UMB                 2002
#define RX_RAT_TYPE_EHRPD               2003
extern struct dict_object *rx_rat_type;
#define RX_ABORT_CAUSE_BEARER_RELEASED                          0
#define RX_ABORT_CAUSE_INSUFFICIENT_SERVER_RESOURCES            1
#define RX_ABORT_CAUSE_INSUFFICIENT_BEARER_RESOURCES            2
#define RX_ABORT_CAUSE_PS_TO_CS_HANDOVER                        3
#define RX_ABORT_CAUSE_SPONSORED_DATA_CONNECTIVITY_ DISALLOWED  4
extern struct dict_object *rx_abort_cause;
#define RX_TERMINATION_CAUSE_DIAMETER_LOGOUT                    1
#define RX_TERMINATION_CAUSE_DIAMETER_SERVICE_NOT_PROVIDED      2
#define RX_TERMINATION_CAUSE_DIAMETER_BAD_ANSWER                3
#define RX_TERMINATION_CAUSE_DIAMETER_DIAMETER_ADMINISTRATIVE   4
#define RX_TERMINATION_CAUSE_DIAMETER_LINK_BROKEN               5
#define RX_TERMINATION_CAUSE_DIAMETER_AUTH_EXPIRED              6
#define RX_TERMINATION_CAUSE_DIAMETER_USER_MOVED                7 
#define RX_TERMINATION_CAUSE_DIAMETER_SESSION_TIMEOUT           8
extern struct dict_object *rx_termination_cause;

int rx_dict_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RX_DICT_H__ */
