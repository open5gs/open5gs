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

#ifndef OGS_DIAM_RX_MESSAGE_H
#define OGS_DIAM_RX_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_DIAM_RX_APPLICATION_ID 16777236

#define OGS_DIAM_RX_AVP_CODE_SUBSCRIPTION_ID                (443)
#define OGS_DIAM_RX_AVP_CODE_SPECIFIC_ACTION                (513)

#define OGS_DIAM_RX_AVP_CODE_MEDIA_COMPONENT_DESCRIPTION    (517)
#define OGS_DIAM_RX_AVP_CODE_MEDIA_TYPE                     (520)
#define OGS_DIAM_RX_AVP_CODE_MAX_REQUESTED_BANDWIDTH_DL     (515)
#define OGS_DIAM_RX_AVP_CODE_MAX_REQUESTED_BANDWIDTH_UL     (516)
#define OGS_DIAM_RX_AVP_CODE_RR_BANDWIDTH                   (521)
#define OGS_DIAM_RX_AVP_CODE_RS_BANDWIDTH                   (522)
#define OGS_DIAM_RX_AVP_CODE_MIN_REQUESTED_BANDWIDTH_DL     (534)
#define OGS_DIAM_RX_AVP_CODE_MIN_REQUESTED_BANDWIDTH_UL     (535)
#define OGS_DIAM_RX_AVP_CODE_MEDIA_COMPONENT_NUMBER         (518)

#define OGS_DIAM_RX_AVP_CODE_MEDIA_SUB_COMPONENT            (519)
#define OGS_DIAM_RX_AVP_CODE_FLOW_DESCRIPTION               (507)
#define OGS_DIAM_RX_AVP_CODE_FLOW_NUMBER                    (509)
#define OGS_DIAM_RX_AVP_CODE_FLOW_STATUS                    (511)
#define OGS_DIAM_RX_AVP_CODE_FLOW_USAGE                     (512)

extern struct dict_object *ogs_diam_rx_application;

extern struct dict_object *ogs_diam_rx_cmd_aar;
extern struct dict_object *ogs_diam_rx_cmd_aaa;
extern struct dict_object *ogs_diam_rx_cmd_asr;
extern struct dict_object *ogs_diam_rx_cmd_asa;
extern struct dict_object *ogs_diam_rx_cmd_str;
extern struct dict_object *ogs_diam_rx_cmd_sta;

extern struct dict_object *ogs_diam_rx_af_application_identifier;
extern struct dict_object *ogs_diam_rx_media_component_description;
extern struct dict_object *ogs_diam_rx_media_component_number;
#define OGS_DIAM_RX_MEDIA_TYPE_AUDIO                0
#define OGS_DIAM_RX_MEDIA_TYPE_VIDEO                1
#define OGS_DIAM_RX_MEDIA_TYPE_DATA                 2
#define OGS_DIAM_RX_MEDIA_TYPE_APPLICATION          3
#define OGS_DIAM_RX_MEDIA_TYPE_CONTROL              4
#define OGS_DIAM_RX_MEDIA_TYPE_TEXT                 5
#define OGS_DIAM_RX_MEDIA_TYPE_MESSAGE              6
#define OGS_DIAM_RX_MEDIA_TYPE_OTHER                0xFFFFFFFF
extern struct dict_object *ogs_diam_rx_media_type;
extern struct dict_object *ogs_diam_rx_max_requested_bandwidth_ul;
extern struct dict_object *ogs_diam_rx_max_requested_bandwidth_dl;
extern struct dict_object *ogs_diam_rx_min_requested_bandwidth_ul;
extern struct dict_object *ogs_diam_rx_min_requested_bandwidth_dl;
extern struct dict_object *ogs_diam_rx_rr_bandwidth;
extern struct dict_object *ogs_diam_rx_rs_bandwidth;
#define OGS_DIAM_RX_FLOW_STATUS_ENABLED_UPLINK      0
#define OGS_DIAM_RX_FLOW_STATUS_ENABLED_DOWNLINK    1
#define OGS_DIAM_RX_FLOW_STATUS_ENABLED             2
#define OGS_DIAM_RX_FLOW_STATUS_DISABLED            3
extern struct dict_object *ogs_diam_rx_flow_status;
extern struct dict_object *ogs_diam_rx_codec_data;
extern struct dict_object *ogs_diam_rx_media_sub_component;
extern struct dict_object *ogs_diam_rx_flow_number;
#define OGS_DIAM_RX_FLOW_USAGE_NO_INFORMATION       0
#define OGS_DIAM_RX_FLOW_USAGE_RTCP                 1
#define OGS_DIAM_RX_FLOW_USAGE_AF_SIGNALLING        2
extern struct dict_object *ogs_diam_rx_flow_usage;
extern struct dict_object *ogs_diam_rx_flow_description;
extern struct dict_object *ogs_diam_rx_subscription_id;
#define OGS_DIAM_RX_SUBSCRIPTION_ID_TYPE_END_USER_E164          0
#define OGS_DIAM_RX_SUBSCRIPTION_ID_TYPE_END_USER_IMSI          1
#define OGS_DIAM_RX_SUBSCRIPTION_ID_TYPE_END_USER_SIP_URI       2
#define OGS_DIAM_RX_SUBSCRIPTION_ID_TYPE_END_USER_NAI           3
extern struct dict_object *ogs_diam_rx_subscription_id_type;
extern struct dict_object *ogs_diam_rx_subscription_id_data;
extern struct dict_object *ogs_diam_rx_reservation_priority;
#define OGS_DIAM_RX_SPECIFIC_ACTION_CHARGING_CORRELATION_EXCHANGE 1
#define OGS_DIAM_RX_SPECIFIC_ACTION_INDICATION_OF_LOSS_OF_BEARER 2
#define OGS_DIAM_RX_SPECIFIC_ACTION_INDICATION_OF_RECOVERY_OF_BEARER 3
#define OGS_DIAM_RX_SPECIFIC_ACTION_INDICATION_OF_RELEASE_OF_BEARER 4
#define OGS_DIAM_RX_SPECIFIC_ACTION_INDICATION_OF_ESTABLISHMENT_OF_BEARER 5
#define OGS_DIAM_RX_SPECIFIC_ACTION_IP_CAN_CHANGE 6
#define OGS_DIAM_RX_SPECIFIC_ACTION_ACCESS_NETWORK_INFO_REPORT 12
extern struct dict_object *ogs_diam_rx_specific_action;
extern struct dict_object *ogs_diam_rx_framed_ip_address;
extern struct dict_object *ogs_diam_rx_framed_ipv6_prefix;
#define OGS_DIAM_RX_IP_CAN_TYPE_3GPP_GPRS           0
#define OGS_DIAM_RX_IP_CAN_TYPE_DOCSIS              1
#define OGS_DIAM_RX_IP_CAN_TYPE_xDSL                2
#define OGS_DIAM_RX_IP_CAN_TYPE_WiMAX               3
#define OGS_DIAM_RX_IP_CAN_TYPE_3GPP2               4
#define OGS_DIAM_RX_IP_CAN_TYPE_3GPP_EPS            5
#define OGS_DIAM_RX_IP_CAN_TYPE_Non_3GPP_EPS        6
extern struct dict_object *ogs_diam_rx_ip_can_type;
#define OGS_DIAM_RX_ABORT_CAUSE_BEARER_RELEASED                         0
#define OGS_DIAM_RX_ABORT_CAUSE_INSUFFICIENT_SERVER_RESOURCES           1
#define OGS_DIAM_RX_ABORT_CAUSE_INSUFFICIENT_BEARER_RESOURCES           2
#define OGS_DIAM_RX_ABORT_CAUSE_PS_TO_CS_HANDOVER                       3
#define OGS_DIAM_RX_ABORT_CAUSE_SPONSORED_DATA_CONNECTIVITY_DISALLOWED  4
extern struct dict_object *ogs_diam_rx_abort_cause;

typedef struct ogs_diam_rx_message_s {
#define OGS_DIAM_RX_CMD_CODE_AA                     265
#define OGS_DIAM_RX_CMD_CODE_SESSION_TERMINATION    275
    uint16_t          cmd_code;

/* Experimental-Result-Codes */
#define OGS_DIAM_RX_DIAMETER_INVALID_SERVICE_INFORMATION        5061
#define OGS_DIAM_RX_DIAMETER_FILTER_RESTRICTIONS                5062
#define OGS_DIAM_RX_DIAMETER_REQUESTED_SERVICE_NOT_AUTHORIZED   5063
#define OGS_DIAM_RX_DIAMETER_DUPLICATED_AF_SESSION              5064
#define OGS_DIAM_RX_DIAMETER_IP_CAN_SESSION_NOT_AVAILABLE       5065
#define OGS_DIAM_RX_DIAMETER_UNAUTHORIZED_NON_EMERGENCY_SESSION 5066
#define OGS_DIAM_RX_DIAMETER_UNAUTHORIZED_SPONSORED_DATA_CONNECTIVITY 5067
#define OGS_DIAM_RX_DIAMETER_TEMPORARY_NETWORK_FAILURE          5068
    uint32_t          result_code;

    ogs_ims_data_t    ims_data;
} ogs_diam_rx_message_t;

int ogs_diam_rx_init(void);

#ifdef __cplusplus
}
#endif

#endif /* OGS_DIAM_OGS_DIAM_RX_MESSAGE_H */
