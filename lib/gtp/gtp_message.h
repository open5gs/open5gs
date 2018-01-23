/*
 * Copyright (c) 2017, NextEPC Group
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*******************************************************************************
 * This file had been created by gtp_tlv.py script v0.1.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2018-01-23 16:32:53.055846 by acetcom
 * from 29274-d80.docx
 ******************************************************************************/

#ifndef __GTP_TLV_H__
#define __GTP_TLV_H__

#include "core_tlv_msg.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 5.1 General format */
#define GTPV1U_HEADER_LEN   8
#define GTPV2C_HEADER_LEN   12
#define GTP_TEID_LEN        4
typedef struct _gtp_header_t {
    union {
        struct {
        ED4(c_uint8_t version:3;,
            c_uint8_t piggybacked:1;,
            c_uint8_t teid_presence:1;,
            c_uint8_t spare1:3;)
        };
/* GTU-U flags */
#define GTPU_FLAGS_PN                       0x1
#define GTPU_FLAGS_S                        0x2
        c_uint8_t flags;
    };
    c_uint8_t type;
    c_uint16_t length;
    union {
        struct {
            c_uint32_t teid;
            /* sqn : 31bit ~ 8bit, spare : 7bit ~ 0bit */
#define GTP_XID_TO_SQN(__xid) htonl(((__xid) << 8))
#define GTP_SQN_TO_XID(__sqn) (ntohl(__sqn) >> 8)
            c_uint32_t sqn;
        };
        /* sqn : 31bit ~ 8bit, spare : 7bit ~ 0bit */
        c_uint32_t sqn_only;
    };
} __attribute__ ((packed)) gtp_header_t;

/* GTP-U message type, defined in 3GPP TS 29.281 Release 11 */
#define GTPU_MSGTYPE_ECHO_REQ               1
#define GTPU_MSGTYPE_ECHO_RSP               2
#define GTPU_MSGTYPE_ERR_IND                26
#define GTPU_MSGTYPE_SUPP_EXTHDR_NOTI       31
#define GTPU_MSGTYPE_END_MARKER             254
#define GTPU_MSGTYPE_GPDU                   255

/* GTPv2-C message type */
#define GTP_ECHO_REQUEST_TYPE 1
#define GTP_ECHO_RESPONSE_TYPE 2
#define GTP_VERSION_NOT_SUPPORTED_INDICATION_TYPE 3
#define GTP_CREATE_SESSION_REQUEST_TYPE 32
#define GTP_CREATE_SESSION_RESPONSE_TYPE 33
#define GTP_MODIFY_BEARER_REQUEST_TYPE 34
#define GTP_MODIFY_BEARER_RESPONSE_TYPE 35
#define GTP_DELETE_SESSION_REQUEST_TYPE 36
#define GTP_DELETE_SESSION_RESPONSE_TYPE 37
#define GTP_CHANGE_NOTIFICATION_REQUEST_TYPE 38
#define GTP_CHANGE_NOTIFICATION_RESPONSE_TYPE 39
#define GTP_REMOTE_UE_REPORT_NOTIFICATION_TYPE 40
#define GTP_REMOTE_UE_REPORT_ACKNOWLEDGE_TYPE 41
#define GTP_MODIFY_BEARER_COMMAND_TYPE 64
#define GTP_MODIFY_BEARER_FAILURE_INDICATION_TYPE 65
#define GTP_DELETE_BEARER_COMMAND_TYPE 66
#define GTP_DELETE_BEARER_FAILURE_INDICATION_TYPE 67
#define GTP_BEARER_RESOURCE_COMMAND_TYPE 68
#define GTP_BEARER_RESOURCE_FAILURE_INDICATION_TYPE 69
#define GTP_DOWNLINK_DATA_NOTIFICATION_FAILURE_INDICATION_TYPE 70
#define GTP_TRACE_SESSION_ACTIVATION_TYPE 71
#define GTP_TRACE_SESSION_DEACTIVATION_TYPE 72
#define GTP_STOP_PAGING_INDICATION_TYPE 73
#define GTP_CREATE_BEARER_REQUEST_TYPE 95
#define GTP_CREATE_BEARER_RESPONSE_TYPE 96
#define GTP_UPDATE_BEARER_REQUEST_TYPE 97
#define GTP_UPDATE_BEARER_RESPONSE_TYPE 98
#define GTP_DELETE_BEARER_REQUEST_TYPE 99
#define GTP_DELETE_BEARER_RESPONSE_TYPE 100
#define GTP_DELETE_PDN_CONNECTION_SET_REQUEST_TYPE 101
#define GTP_DELETE_PDN_CONNECTION_SET_RESPONSE_TYPE 102
#define GTP_PGW_DOWNLINK_TRIGGERING_NOTIFICATION_TYPE 103
#define GTP_PGW_DOWNLINK_TRIGGERING_ACKNOWLEDGE_TYPE 104
#define GTP_CREATE_FORWARDING_TUNNEL_REQUEST_TYPE 160
#define GTP_CREATE_FORWARDING_TUNNEL_RESPONSE_TYPE 161
#define GTP_SUSPEND_NOTIFICATION_TYPE 162
#define GTP_SUSPEND_ACKNOWLEDGE_TYPE 163
#define GTP_RESUME_NOTIFICATION_TYPE 164
#define GTP_RESUME_ACKNOWLEDGE_TYPE 165
#define GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE 166
#define GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE 167
#define GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE 168
#define GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE 169
#define GTP_RELEASE_ACCESS_BEARERS_REQUEST_TYPE 170
#define GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE 171
#define GTP_DOWNLINK_DATA_NOTIFICATION_TYPE 176
#define GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE 177
#define GTP_PGW_RESTART_NOTIFICATION_TYPE 179
#define GTP_PGW_RESTART_NOTIFICATION_ACKNOWLEDGE_TYPE 180
#define GTP_UPDATE_PDN_CONNECTION_SET_REQUEST_TYPE 200
#define GTP_UPDATE_PDN_CONNECTION_SET_RESPONSE_TYPE 201
#define GTP_MODIFY_ACCESS_BEARERS_REQUEST_TYPE 211
#define GTP_MODIFY_ACCESS_BEARERS_RESPONSE_TYPE 212

#define TLV_IMSI_TYPE 1
#define TLV_CAUSE_TYPE 2
#define TLV_RECOVERY_TYPE 3
#define TLV_STN_SR_TYPE 51
#define TLV_APN_TYPE 71
#define TLV_AMBR_TYPE 72
#define TLV_EBI_TYPE 73
#define TLV_IP_ADDRESS_TYPE 74
#define TLV_MEI_TYPE 75
#define TLV_MSISDN_TYPE 76
#define TLV_INDICATION_TYPE 77
#define TLV_PCO_TYPE 78
#define TLV_PAA_TYPE 79
#define TLV_BEARER_QOS_TYPE 80
#define TLV_FLOW_QOS_TYPE 81
#define TLV_RAT_TYPE_TYPE 82
#define TLV_SERVING_NETWORK_TYPE 83
#define TLV_BEARER_TFT_TYPE 84
#define TLV_TAD_TYPE 85
#define TLV_ULI_TYPE 86
#define TLV_F_TEID_TYPE 87
#define TLV_TMSI_TYPE 88
#define TLV_GLOBAL_CN_ID_TYPE 89
#define TLV_S103PDF_TYPE 90
#define TLV_S1UDF_TYPE 91
#define TLV_DELAY_VALUE_TYPE 92
#define TLV_BEARER_CONTEXT_TYPE 93
#define TLV_CHARGING_ID_TYPE 94
#define TLV_CHARGING_CHARACTERISTICS_TYPE 95
#define TLV_TRACE_INFORMATION_TYPE 96
#define TLV_BEARER_FLAGS_TYPE 97
#define TLV_PDN_TYPE_TYPE 99
#define TLV_PTI_TYPE 100
#define TLV_MM_CONTEXT_TYPE 107
#define TLV_PDN_CONNECTION_TYPE 109
#define TLV_PDU_NUMBERS_TYPE 110
#define TLV_P_TMSI_TYPE 111
#define TLV_P_TMSI_SIGNATURE_TYPE 112
#define TLV_HOP_COUNTER_TYPE 113
#define TLV_UE_TIME_ZONE_TYPE 114
#define TLV_TRACE_REFERENCE_TYPE 115
#define TLV_COMPLETE_REQUEST_MESSAGE_TYPE 116
#define TLV_GUTI_TYPE 117
#define TLV_F_CONTAINER_TYPE 118
#define TLV_F_CAUSE_TYPE 119
#define TLV_PLMN_ID_TYPE 120
#define TLV_TARGET_IDENTIFICATION_TYPE 121
#define TLV_PACKET_FLOW_ID_TYPE 123
#define TLV_RAB_CONTEXT_TYPE 124
#define TLV_SOURCE_RNC_PDCP_CONTEXT_INFO_TYPE 125
#define TLV_PORT_NUMBER_TYPE 126
#define TLV_APN_RESTRICTION_TYPE 127
#define TLV_SELECTION_MODE_TYPE 128
#define TLV_SOURCE_IDENTIFICATION_TYPE 129
#define TLV_CHANGE_REPORTING_ACTION_TYPE 131
#define TLV_FQ_CSID_TYPE 132
#define TLV_CHANNEL_NEEDED_TYPE 133
#define TLV_EMLPP_PRIORITY_TYPE 134
#define TLV_NODE_TYPE_TYPE 135
#define TLV_FQDN_TYPE 136
#define TLV_TI_TYPE 137
#define TLV_MBMS_SESSION_DURATION_TYPE 138
#define TLV_MBMS_SERVICE_AREA_TYPE 139
#define TLV_MBMS_SESSION_IDENTIFIER_TYPE 140
#define TLV_MBMS_FLOW_IDENTIFIER_TYPE 141
#define TLV_MBMS_IP_MULTICAST_DISTRIBUTION_TYPE 142
#define TLV_MBMS_DISTRIBUTION_ACKNOWLEDGE_TYPE 143
#define TLV_RFSP_INDEX_TYPE 144
#define TLV_UCI_TYPE 145
#define TLV_CSG_INFORMATION_REPORTING_ACTION_TYPE 146
#define TLV_CSG_ID_TYPE 147
#define TLV_CMI_TYPE 148
#define TLV_SERVICE_INDICATOR_TYPE 149
#define TLV_DETACH_TYPE_TYPE 150
#define TLV_LDN_TYPE 151
#define TLV_NODE_FEATURES_TYPE 152
#define TLV_MBMS_TIME_TO_DATA_TRANSFER_TYPE 153
#define TLV_THROTTLING_TYPE 154
#define TLV_ARP_TYPE 155
#define TLV_EPC_TIMER_TYPE 156
#define TLV_SIGNALLING_PRIORITY_INDICATION_TYPE 157
#define TLV_TMGI_TYPE 158
#define TLV_ADDITIONAL_MM_CONTEXT_FOR_SRVCC_TYPE 159
#define TLV_ADDITIONAL_FLAGS_FOR_SRVCC_TYPE 160
#define TLV_MDT_CONFIGURATION_TYPE 162
#define TLV_APCO_TYPE 163
#define TLV_ABSOLUTE_TIME_OF_MBMS_DATA_TRANSFER_TYPE 164
#define TLV_ENB_INFORMATION_REPORTING_TYPE 165
#define TLV_IP4CP_TYPE 166
#define TLV_CHANGE_TO_REPORT_FLAGS_TYPE 167
#define TLV_ACTION_INDICATION_TYPE 168
#define TLV_TWAN_IDENTIFIER_TYPE 169
#define TLV_ULI_TIMESTAMP_TYPE 170
#define TLV_MBMS_FLAGS_TYPE 171
#define TLV_RAN_NAS_CAUSE_TYPE 172
#define TLV_CN_OPERATOR_SELECTION_ENTITY_TYPE 173
#define TLV_TWMI_TYPE 174
#define TLV_NODE_NUMBER_TYPE 175
#define TLV_NODE_IDENTIFIER_TYPE 176
#define TLV_PRESENCE_REPORTING_AREA_ACTION_TYPE 177
#define TLV_PRESENCE_REPORTING_AREA_INFORMATION_TYPE 178
#define TLV_TWAN_IDENTIFIER_TIMESTAMP_TYPE 179
#define TLV_OVERLOAD_CONTROL_INFORMATION_TYPE 180
#define TLV_LOAD_CONTROL_INFORMATION_TYPE 181
#define TLV_METRIC_TYPE 182
#define TLV_SEQUENCE_NUMBER_TYPE 183
#define TLV_APN_AND_RELATIVE_CAPACITY_TYPE 184
#define TLV_WLAN_OFFLOADABILITY_INDICATION_TYPE 185
#define TLV_PAGING_AND_SERVICE_INFORMATION_TYPE 186
#define TLV_INTEGER_NUMBER_TYPE 187
#define TLV_MILLISECOND_TIME_STAMP_TYPE 188
#define TLV_MONITORING_EVENT_INFORMATION_TYPE 189
#define TLV_ECGI_LIST_TYPE 190
#define TLV_REMOTE_UE_CONTEXT_TYPE 191
#define TLV_REMOTE_USER_ID_TYPE 192
#define TLV_REMOTE_UE_IP_INFORMATION_TYPE 193
#define TLV_CIOT_OPTIMIZATIONS_SUPPORT_INDICATION_TYPE 194
#define TLV_SCEF_PDN_CONNECTION_TYPE 195
#define TLV_HEADER_COMPRESSION_CONFIGURATION_TYPE 196
#define TLV_EPCO_TYPE 197
#define TLV_SERVING_PLMN_RATE_CONTROL_TYPE 198
#define TLV_COUNTER_TYPE 199

/* Infomration Element TLV Descriptor */
extern tlv_desc_t tlv_desc_imsi_0;
extern tlv_desc_t tlv_desc_cause_0;
extern tlv_desc_t tlv_desc_recovery_0;
extern tlv_desc_t tlv_desc_stn_sr_0;
extern tlv_desc_t tlv_desc_apn_0;
extern tlv_desc_t tlv_desc_ambr_0;
extern tlv_desc_t tlv_desc_ebi_0;
extern tlv_desc_t tlv_desc_ebi_1;
extern tlv_desc_t tlv_desc_ip_address_0;
extern tlv_desc_t tlv_desc_ip_address_1;
extern tlv_desc_t tlv_desc_ip_address_2;
extern tlv_desc_t tlv_desc_ip_address_3;
extern tlv_desc_t tlv_desc_mei_0;
extern tlv_desc_t tlv_desc_msisdn_0;
extern tlv_desc_t tlv_desc_indication_0;
extern tlv_desc_t tlv_desc_pco_0;
extern tlv_desc_t tlv_desc_paa_0;
extern tlv_desc_t tlv_desc_bearer_qos_0;
extern tlv_desc_t tlv_desc_flow_qos_0;
extern tlv_desc_t tlv_desc_rat_type_0;
extern tlv_desc_t tlv_desc_serving_network_0;
extern tlv_desc_t tlv_desc_bearer_tft_0;
extern tlv_desc_t tlv_desc_tad_0;
extern tlv_desc_t tlv_desc_uli_0;
extern tlv_desc_t tlv_desc_f_teid_0;
extern tlv_desc_t tlv_desc_f_teid_1;
extern tlv_desc_t tlv_desc_f_teid_2;
extern tlv_desc_t tlv_desc_f_teid_3;
extern tlv_desc_t tlv_desc_f_teid_4;
extern tlv_desc_t tlv_desc_f_teid_5;
extern tlv_desc_t tlv_desc_f_teid_6;
extern tlv_desc_t tlv_desc_f_teid_7;
extern tlv_desc_t tlv_desc_f_teid_8;
extern tlv_desc_t tlv_desc_f_teid_9;
extern tlv_desc_t tlv_desc_f_teid_10;
extern tlv_desc_t tlv_desc_f_teid_11;
extern tlv_desc_t tlv_desc_tmsi_0;
extern tlv_desc_t tlv_desc_global_cn_id_0;
extern tlv_desc_t tlv_desc_s103pdf_0;
extern tlv_desc_t tlv_desc_s1udf_0;
extern tlv_desc_t tlv_desc_delay_value_0;
extern tlv_desc_t tlv_desc_charging_id_0;
extern tlv_desc_t tlv_desc_charging_characteristics_0;
extern tlv_desc_t tlv_desc_trace_information_0;
extern tlv_desc_t tlv_desc_bearer_flags_0;
extern tlv_desc_t tlv_desc_pdn_type_0;
extern tlv_desc_t tlv_desc_pti_0;
extern tlv_desc_t tlv_desc_mm_context_0;
extern tlv_desc_t tlv_desc_pdu_numbers_0;
extern tlv_desc_t tlv_desc_p_tmsi_0;
extern tlv_desc_t tlv_desc_p_tmsi_signature_0;
extern tlv_desc_t tlv_desc_hop_counter_0;
extern tlv_desc_t tlv_desc_ue_time_zone_0;
extern tlv_desc_t tlv_desc_trace_reference_0;
extern tlv_desc_t tlv_desc_complete_request_message_0;
extern tlv_desc_t tlv_desc_guti_0;
extern tlv_desc_t tlv_desc_f_container_0;
extern tlv_desc_t tlv_desc_f_cause_0;
extern tlv_desc_t tlv_desc_plmn_id_0;
extern tlv_desc_t tlv_desc_target_identification_0;
extern tlv_desc_t tlv_desc_packet_flow_id_0;
extern tlv_desc_t tlv_desc_rab_context_0;
extern tlv_desc_t tlv_desc_source_rnc_pdcp_context_info_0;
extern tlv_desc_t tlv_desc_port_number_0;
extern tlv_desc_t tlv_desc_port_number_1;
extern tlv_desc_t tlv_desc_port_number_2;
extern tlv_desc_t tlv_desc_apn_restriction_0;
extern tlv_desc_t tlv_desc_selection_mode_0;
extern tlv_desc_t tlv_desc_source_identification_0;
extern tlv_desc_t tlv_desc_change_reporting_action_0;
extern tlv_desc_t tlv_desc_fq_csid_0;
extern tlv_desc_t tlv_desc_fq_csid_1;
extern tlv_desc_t tlv_desc_fq_csid_2;
extern tlv_desc_t tlv_desc_fq_csid_3;
extern tlv_desc_t tlv_desc_channel_needed_0;
extern tlv_desc_t tlv_desc_emlpp_priority_0;
extern tlv_desc_t tlv_desc_node_type_0;
extern tlv_desc_t tlv_desc_fqdn_0;
extern tlv_desc_t tlv_desc_ti_0;
extern tlv_desc_t tlv_desc_mbms_session_duration_0;
extern tlv_desc_t tlv_desc_mbms_service_area_0;
extern tlv_desc_t tlv_desc_mbms_session_identifier_0;
extern tlv_desc_t tlv_desc_mbms_flow_identifier_0;
extern tlv_desc_t tlv_desc_mbms_ip_multicast_distribution_0;
extern tlv_desc_t tlv_desc_mbms_distribution_acknowledge_0;
extern tlv_desc_t tlv_desc_rfsp_index_0;
extern tlv_desc_t tlv_desc_uci_0;
extern tlv_desc_t tlv_desc_csg_information_reporting_action_0;
extern tlv_desc_t tlv_desc_csg_id_0;
extern tlv_desc_t tlv_desc_cmi_0;
extern tlv_desc_t tlv_desc_service_indicator_0;
extern tlv_desc_t tlv_desc_detach_type_0;
extern tlv_desc_t tlv_desc_ldn_0;
extern tlv_desc_t tlv_desc_ldn_1;
extern tlv_desc_t tlv_desc_ldn_2;
extern tlv_desc_t tlv_desc_ldn_3;
extern tlv_desc_t tlv_desc_node_features_0;
extern tlv_desc_t tlv_desc_mbms_time_to_data_transfer_0;
extern tlv_desc_t tlv_desc_throttling_0;
extern tlv_desc_t tlv_desc_arp_0;
extern tlv_desc_t tlv_desc_epc_timer_0;
extern tlv_desc_t tlv_desc_signalling_priority_indication_0;
extern tlv_desc_t tlv_desc_tmgi_0;
extern tlv_desc_t tlv_desc_additional_mm_context_for_srvcc_0;
extern tlv_desc_t tlv_desc_additional_flags_for_srvcc_0;
extern tlv_desc_t tlv_desc_mdt_configuration_0;
extern tlv_desc_t tlv_desc_apco_0;
extern tlv_desc_t tlv_desc_absolute_time_of_mbms_data_transfer_0;
extern tlv_desc_t tlv_desc_enb_information_reporting_0;
extern tlv_desc_t tlv_desc_ip4cp_0;
extern tlv_desc_t tlv_desc_change_to_report_flags_0;
extern tlv_desc_t tlv_desc_action_indication_0;
extern tlv_desc_t tlv_desc_twan_identifier_0;
extern tlv_desc_t tlv_desc_twan_identifier_1;
extern tlv_desc_t tlv_desc_uli_timestamp_0;
extern tlv_desc_t tlv_desc_mbms_flags_0;
extern tlv_desc_t tlv_desc_ran_nas_cause_0;
extern tlv_desc_t tlv_desc_cn_operator_selection_entity_0;
extern tlv_desc_t tlv_desc_twmi_0;
extern tlv_desc_t tlv_desc_node_number_0;
extern tlv_desc_t tlv_desc_node_identifier_0;
extern tlv_desc_t tlv_desc_presence_reporting_area_action_0;
extern tlv_desc_t tlv_desc_presence_reporting_area_information_0;
extern tlv_desc_t tlv_desc_twan_identifier_timestamp_0;
extern tlv_desc_t tlv_desc_twan_identifier_timestamp_1;
extern tlv_desc_t tlv_desc_metric_0;
extern tlv_desc_t tlv_desc_sequence_number_0;
extern tlv_desc_t tlv_desc_apn_and_relative_capacity_0;
extern tlv_desc_t tlv_desc_wlan_offloadability_indication_0;
extern tlv_desc_t tlv_desc_paging_and_service_information_0;
extern tlv_desc_t tlv_desc_integer_number_0;
extern tlv_desc_t tlv_desc_millisecond_time_stamp_0;
extern tlv_desc_t tlv_desc_monitoring_event_information_0;
extern tlv_desc_t tlv_desc_ecgi_list_0;
extern tlv_desc_t tlv_desc_remote_user_id_0;
extern tlv_desc_t tlv_desc_remote_ue_ip_information_0;
extern tlv_desc_t tlv_desc_ciot_optimizations_support_indication_0;
extern tlv_desc_t tlv_desc_header_compression_configuration_0;
extern tlv_desc_t tlv_desc_epco_0;
extern tlv_desc_t tlv_desc_serving_plmn_rate_control_0;
extern tlv_desc_t tlv_desc_counter_0;

/* Group Infomration Element TLV Descriptor */
extern tlv_desc_t tlv_desc_bearer_context_0;
extern tlv_desc_t tlv_desc_bearer_context_1;
extern tlv_desc_t tlv_desc_bearer_context_2;
extern tlv_desc_t tlv_desc_bearer_context_3;
extern tlv_desc_t tlv_desc_bearer_context_4;
extern tlv_desc_t tlv_desc_bearer_context_5;
extern tlv_desc_t tlv_desc_bearer_context_6;
extern tlv_desc_t tlv_desc_bearer_context_7;
extern tlv_desc_t tlv_desc_bearer_context_8;
extern tlv_desc_t tlv_desc_bearer_context_9;
extern tlv_desc_t tlv_desc_bearer_context_10;
extern tlv_desc_t tlv_desc_pdn_connection_0;
extern tlv_desc_t tlv_desc_overload_control_information_0;
extern tlv_desc_t tlv_desc_overload_control_information_1;
extern tlv_desc_t tlv_desc_overload_control_information_2;
extern tlv_desc_t tlv_desc_load_control_information_0;
extern tlv_desc_t tlv_desc_load_control_information_1;
extern tlv_desc_t tlv_desc_load_control_information_2;
extern tlv_desc_t tlv_desc_remote_ue_context_0;
extern tlv_desc_t tlv_desc_scef_pdn_connection_0;

/* Message Descriptor */
extern tlv_desc_t tlv_desc_echo_request;
extern tlv_desc_t tlv_desc_echo_response;
extern tlv_desc_t tlv_desc_version_not_supported_indication;
extern tlv_desc_t tlv_desc_create_session_request;
extern tlv_desc_t tlv_desc_create_session_response;
extern tlv_desc_t tlv_desc_modify_bearer_request;
extern tlv_desc_t tlv_desc_modify_bearer_response;
extern tlv_desc_t tlv_desc_delete_session_request;
extern tlv_desc_t tlv_desc_delete_session_response;
extern tlv_desc_t tlv_desc_change_notification_request;
extern tlv_desc_t tlv_desc_change_notification_response;
extern tlv_desc_t tlv_desc_remote_ue_report_notification;
extern tlv_desc_t tlv_desc_remote_ue_report_acknowledge;
extern tlv_desc_t tlv_desc_modify_bearer_command;
extern tlv_desc_t tlv_desc_modify_bearer_failure_indication;
extern tlv_desc_t tlv_desc_delete_bearer_command;
extern tlv_desc_t tlv_desc_delete_bearer_failure_indication;
extern tlv_desc_t tlv_desc_bearer_resource_command;
extern tlv_desc_t tlv_desc_bearer_resource_failure_indication;
extern tlv_desc_t tlv_desc_downlink_data_notification_failure_indication;
extern tlv_desc_t tlv_desc_trace_session_activation;
extern tlv_desc_t tlv_desc_trace_session_deactivation;
extern tlv_desc_t tlv_desc_stop_paging_indication;
extern tlv_desc_t tlv_desc_create_bearer_request;
extern tlv_desc_t tlv_desc_create_bearer_response;
extern tlv_desc_t tlv_desc_update_bearer_request;
extern tlv_desc_t tlv_desc_update_bearer_response;
extern tlv_desc_t tlv_desc_delete_bearer_request;
extern tlv_desc_t tlv_desc_delete_bearer_response;
extern tlv_desc_t tlv_desc_delete_pdn_connection_set_request;
extern tlv_desc_t tlv_desc_delete_pdn_connection_set_response;
extern tlv_desc_t tlv_desc_pgw_downlink_triggering_notification;
extern tlv_desc_t tlv_desc_pgw_downlink_triggering_acknowledge;
extern tlv_desc_t tlv_desc_create_forwarding_tunnel_request;
extern tlv_desc_t tlv_desc_create_forwarding_tunnel_response;
extern tlv_desc_t tlv_desc_suspend_notification;
extern tlv_desc_t tlv_desc_suspend_acknowledge;
extern tlv_desc_t tlv_desc_resume_notification;
extern tlv_desc_t tlv_desc_resume_acknowledge;
extern tlv_desc_t tlv_desc_create_indirect_data_forwarding_tunnel_request;
extern tlv_desc_t tlv_desc_create_indirect_data_forwarding_tunnel_response;
extern tlv_desc_t tlv_desc_delete_indirect_data_forwarding_tunnel_request;
extern tlv_desc_t tlv_desc_delete_indirect_data_forwarding_tunnel_response;
extern tlv_desc_t tlv_desc_release_access_bearers_request;
extern tlv_desc_t tlv_desc_release_access_bearers_response;
extern tlv_desc_t tlv_desc_downlink_data_notification;
extern tlv_desc_t tlv_desc_downlink_data_notification_acknowledge;
extern tlv_desc_t tlv_desc_pgw_restart_notification;
extern tlv_desc_t tlv_desc_pgw_restart_notification_acknowledge;
extern tlv_desc_t tlv_desc_update_pdn_connection_set_request;
extern tlv_desc_t tlv_desc_update_pdn_connection_set_response;
extern tlv_desc_t tlv_desc_modify_access_bearers_request;
extern tlv_desc_t tlv_desc_modify_access_bearers_response;

/* Structure for Infomration Element */
typedef tlv_octet_t tlv_imsi_t;
typedef tlv_octet_t tlv_cause_t;
typedef tlv_uint8_t tlv_recovery_t;
typedef tlv_octet_t tlv_stn_sr_t;
typedef tlv_octet_t tlv_apn_t;
typedef tlv_octet_t tlv_ambr_t;
typedef tlv_uint8_t tlv_ebi_t;
typedef tlv_octet_t tlv_ip_address_t;
typedef tlv_octet_t tlv_mei_t;
typedef tlv_octet_t tlv_msisdn_t;
typedef tlv_octet_t tlv_indication_t;
typedef tlv_octet_t tlv_pco_t;
typedef tlv_octet_t tlv_paa_t;
typedef tlv_octet_t tlv_bearer_qos_t;
typedef tlv_octet_t tlv_flow_qos_t;
typedef tlv_uint8_t tlv_rat_type_t;
typedef tlv_octet_t tlv_serving_network_t;
typedef tlv_octet_t tlv_bearer_tft_t;
typedef tlv_octet_t tlv_tad_t;
typedef tlv_octet_t tlv_uli_t;
typedef tlv_octet_t tlv_f_teid_t;
typedef tlv_octet_t tlv_tmsi_t;
typedef tlv_octet_t tlv_global_cn_id_t;
typedef tlv_octet_t tlv_s103pdf_t;
typedef tlv_octet_t tlv_s1udf_t;
typedef tlv_octet_t tlv_delay_value_t;
typedef tlv_octet_t tlv_charging_id_t;
typedef tlv_octet_t tlv_charging_characteristics_t;
typedef tlv_octet_t tlv_trace_information_t;
typedef tlv_octet_t tlv_bearer_flags_t;
typedef tlv_uint8_t tlv_pdn_type_t;
typedef tlv_octet_t tlv_pti_t;
typedef tlv_octet_t tlv_mm_context_t;
typedef tlv_octet_t tlv_pdu_numbers_t;
typedef tlv_octet_t tlv_p_tmsi_t;
typedef tlv_octet_t tlv_p_tmsi_signature_t;
typedef tlv_octet_t tlv_hop_counter_t;
typedef tlv_octet_t tlv_ue_time_zone_t;
typedef tlv_octet_t tlv_trace_reference_t;
typedef tlv_octet_t tlv_complete_request_message_t;
typedef tlv_octet_t tlv_guti_t;
typedef tlv_octet_t tlv_f_container_t;
typedef tlv_octet_t tlv_f_cause_t;
typedef tlv_octet_t tlv_plmn_id_t;
typedef tlv_octet_t tlv_target_identification_t;
typedef tlv_octet_t tlv_packet_flow_id_t;
typedef tlv_octet_t tlv_rab_context_t;
typedef tlv_octet_t tlv_source_rnc_pdcp_context_info_t;
typedef tlv_uint16_t tlv_port_number_t;
typedef tlv_uint8_t tlv_apn_restriction_t;
typedef tlv_uint8_t tlv_selection_mode_t;
typedef tlv_octet_t tlv_source_identification_t;
typedef tlv_octet_t tlv_change_reporting_action_t;
typedef tlv_octet_t tlv_fq_csid_t;
typedef tlv_octet_t tlv_channel_needed_t;
typedef tlv_octet_t tlv_emlpp_priority_t;
typedef tlv_uint8_t tlv_node_type_t;
typedef tlv_octet_t tlv_fqdn_t;
typedef tlv_octet_t tlv_ti_t;
typedef tlv_octet_t tlv_mbms_session_duration_t;
typedef tlv_octet_t tlv_mbms_service_area_t;
typedef tlv_octet_t tlv_mbms_session_identifier_t;
typedef tlv_octet_t tlv_mbms_flow_identifier_t;
typedef tlv_octet_t tlv_mbms_ip_multicast_distribution_t;
typedef tlv_octet_t tlv_mbms_distribution_acknowledge_t;
typedef tlv_octet_t tlv_rfsp_index_t;
typedef tlv_octet_t tlv_uci_t;
typedef tlv_octet_t tlv_csg_information_reporting_action_t;
typedef tlv_octet_t tlv_csg_id_t;
typedef tlv_octet_t tlv_cmi_t;
typedef tlv_octet_t tlv_service_indicator_t;
typedef tlv_octet_t tlv_detach_type_t;
typedef tlv_octet_t tlv_ldn_t;
typedef tlv_octet_t tlv_node_features_t;
typedef tlv_octet_t tlv_mbms_time_to_data_transfer_t;
typedef tlv_octet_t tlv_throttling_t;
typedef tlv_octet_t tlv_arp_t;
typedef tlv_octet_t tlv_epc_timer_t;
typedef tlv_octet_t tlv_signalling_priority_indication_t;
typedef tlv_octet_t tlv_tmgi_t;
typedef tlv_octet_t tlv_additional_mm_context_for_srvcc_t;
typedef tlv_octet_t tlv_additional_flags_for_srvcc_t;
typedef tlv_octet_t tlv_mdt_configuration_t;
typedef tlv_octet_t tlv_apco_t;
typedef tlv_octet_t tlv_absolute_time_of_mbms_data_transfer_t;
typedef tlv_octet_t tlv_enb_information_reporting_t;
typedef tlv_octet_t tlv_ip4cp_t;
typedef tlv_octet_t tlv_change_to_report_flags_t;
typedef tlv_octet_t tlv_action_indication_t;
typedef tlv_octet_t tlv_twan_identifier_t;
typedef tlv_octet_t tlv_uli_timestamp_t;
typedef tlv_octet_t tlv_mbms_flags_t;
typedef tlv_octet_t tlv_ran_nas_cause_t;
typedef tlv_octet_t tlv_cn_operator_selection_entity_t;
typedef tlv_octet_t tlv_twmi_t;
typedef tlv_octet_t tlv_node_number_t;
typedef tlv_octet_t tlv_node_identifier_t;
typedef tlv_octet_t tlv_presence_reporting_area_action_t;
typedef tlv_octet_t tlv_presence_reporting_area_information_t;
typedef tlv_octet_t tlv_twan_identifier_timestamp_t;
typedef tlv_octet_t tlv_metric_t;
typedef tlv_octet_t tlv_sequence_number_t;
typedef tlv_octet_t tlv_apn_and_relative_capacity_t;
typedef tlv_octet_t tlv_wlan_offloadability_indication_t;
typedef tlv_octet_t tlv_paging_and_service_information_t;
typedef tlv_octet_t tlv_integer_number_t;
typedef tlv_octet_t tlv_millisecond_time_stamp_t;
typedef tlv_octet_t tlv_monitoring_event_information_t;
typedef tlv_octet_t tlv_ecgi_list_t;
typedef tlv_octet_t tlv_remote_user_id_t;
typedef tlv_octet_t tlv_remote_ue_ip_information_t;
typedef tlv_octet_t tlv_ciot_optimizations_support_indication_t;
typedef tlv_octet_t tlv_header_compression_configuration_t;
typedef tlv_octet_t tlv_epco_t;
typedef tlv_octet_t tlv_serving_plmn_rate_control_t;
typedef tlv_octet_t tlv_counter_t;

/* Structure for Group Infomration Element */
typedef struct _tlv_bearer_context_t {
    tlv_presence_t presence;
    tlv_ebi_t eps_bearer_id;
    tlv_bearer_tft_t tft;
    tlv_f_teid_t s1_u_enodeb_f_teid; /* Instance : 0 */
    tlv_f_teid_t s4_u_sgsn_f_teid; /* Instance : 1 */
    tlv_f_teid_t s5_s8_u_sgw_f_teid; /* Instance : 2 */
    tlv_f_teid_t s5_s8_u_pgw_f_teid; /* Instance : 3 */
    tlv_f_teid_t s12_rnc_f_teid; /* Instance : 4 */
    tlv_f_teid_t s2b_u_epdg_f_teid_5; /* Instance : 5 */
    tlv_f_teid_t s2a_u_twan_f_teid_6; /* Instance : 6 */
    tlv_bearer_qos_t bearer_level_qos;
    tlv_f_teid_t s11_u_mme_f_teid; /* Instance : 7 */
    tlv_cause_t cause;
    tlv_charging_id_t charging_id;
    tlv_bearer_flags_t bearer_flags;
    tlv_pco_t protocol_configuration_options;
    tlv_epco_t extended_protocol_configuration_options;
    tlv_f_teid_t s2b_u_epdg_f_teid_8; /* Instance : 8 */
    tlv_f_teid_t s2b_u_pgw_f_teid; /* Instance : 9 */
    tlv_f_teid_t s2a_u_twan_f_teid_10; /* Instance : 10 */
    tlv_f_teid_t s2a_u_pgw_f_teid; /* Instance : 11 */
    tlv_ran_nas_cause_t ran_nas_cause;
    tlv_apco_t additional_protocol_configuration_options;
    tlv_f_container_t bss_container;
    tlv_ti_t transaction_identifier;
    tlv_packet_flow_id_t packet_flow_id;
} tlv_bearer_context_t;

typedef struct _tlv_pdn_connection_t {
    tlv_presence_t presence;
} tlv_pdn_connection_t;

typedef struct _tlv_overload_control_information_t {
    tlv_presence_t presence;
    tlv_sequence_number_t overload_control_sequence_number;
    tlv_metric_t overload_reduction_metric;
    tlv_epc_timer_t period_of_validity;
    tlv_apn_t list_of_access_point_name;
} tlv_overload_control_information_t;

typedef struct _tlv_load_control_information_t {
    tlv_presence_t presence;
    tlv_sequence_number_t load_control_sequence_number;
    tlv_metric_t load_metric;
    tlv_apn_and_relative_capacity_t list_of_apn_and_relative_capacity;
} tlv_load_control_information_t;

typedef struct _tlv_remote_ue_context_t {
    tlv_presence_t presence;
    tlv_remote_user_id_t remote_user_id;
    tlv_remote_ue_ip_information_t remote_ue_ip_information;
} tlv_remote_ue_context_t;

typedef struct _tlv_scef_pdn_connection_t {
    tlv_presence_t presence;
    tlv_apn_t apn;
    tlv_ebi_t default_eps_bearer_id;
    tlv_node_identifier_t scef_id;
} tlv_scef_pdn_connection_t;

/* Structure for Message */
typedef struct _gtp_echo_request_t {
    tlv_recovery_t recovery;
    tlv_node_features_t sending_node_features;
} gtp_echo_request_t;

typedef struct _gtp_echo_response_t {
    tlv_recovery_t recovery;
    tlv_node_features_t sending_node_features;
} gtp_echo_response_t;

typedef struct _gtp_create_session_request_t {
    tlv_imsi_t imsi;
    tlv_msisdn_t msisdn;
    tlv_mei_t me_identity;
    tlv_uli_t user_location_information;
    tlv_serving_network_t serving_network;
    tlv_rat_type_t rat_type;
    tlv_indication_t indication_flags;
    tlv_f_teid_t sender_f_teid_for_control_plane;
    tlv_f_teid_t pgw_s5_s8_address_for_control_plane_or_pmip;
    tlv_apn_t access_point_name;
    tlv_selection_mode_t selection_mode;
    tlv_pdn_type_t pdn_type;
    tlv_paa_t pdn_address_allocation;
    tlv_apn_restriction_t maximum_apn_restriction;
    tlv_ambr_t aggregate_maximum_bit_rate;
    tlv_ebi_t linked_eps_bearer_id;
    tlv_twmi_t trusted_wlan_mode_indication;
    tlv_pco_t protocol_configuration_options;
    tlv_bearer_context_t bearer_contexts_to_be_created;
    tlv_bearer_context_t bearer_contexts_to_be_removed;
    tlv_trace_information_t trace_information;
    tlv_recovery_t recovery;
    tlv_fq_csid_t mme_fq_csid;
    tlv_fq_csid_t sgw_fq_csid;
    tlv_fq_csid_t epdg_fq_csid;
    tlv_fq_csid_t twan_fq_csid;
    tlv_ue_time_zone_t ue_time_zone;
    tlv_uci_t user_csg_information;
    tlv_charging_characteristics_t charging_characteristics;
    tlv_ldn_t mme_s4_sgsn_ldn;
    tlv_ldn_t sgw_ldn;
    tlv_ldn_t epdg_ldn;
    tlv_ldn_t twan_ldn;
    tlv_signalling_priority_indication_t signalling_priority_indication;
    tlv_ip_address_t ue_local_ip_address;
    tlv_port_number_t ue_udp_port;
    tlv_apco_t additional_protocol_configuration_options;
    tlv_ip_address_t hnb_local_ip_address;
    tlv_port_number_t hnb_udp_port;
    tlv_ip_address_t mme_s4_sgsn_identifier;
    tlv_twan_identifier_t twan_identifier;
    tlv_ip_address_t epdg_ip_address;
    tlv_cn_operator_selection_entity_t cn_operator_selection_entity;
    tlv_presence_reporting_area_information_t presence_reporting_area_information;
    tlv_overload_control_information_t mme_s4_sgsn_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_overload_control_information_t twan_epdg_s_overload_control_information;
    tlv_millisecond_time_stamp_t origination_time_stamp;
    tlv_integer_number_t maximum_wait_time;
    tlv_twan_identifier_t wlan_location_information;
    tlv_twan_identifier_timestamp_t wlan_location_timestamp;
    tlv_f_container_t nbifom_container;
    tlv_remote_ue_context_t remote_ue_context_connected;
    tlv_node_identifier_t _aaa_server_identifier;
    tlv_epco_t extended_protocol_configuration_options;
    tlv_serving_plmn_rate_control_t serving_plmn_rate_control;
    tlv_counter_t mo_exception_data_counter;
    tlv_port_number_t ue_tcp_port;
} gtp_create_session_request_t;

typedef struct _gtp_create_session_response_t {
    tlv_cause_t cause;
    tlv_change_reporting_action_t change_reporting_action_;
    tlv_csg_information_reporting_action_t csg_information_reporting_action;
    tlv_enb_information_reporting_t hnb_information_reporting;
    tlv_f_teid_t sender_f_teid_for_control_plane;
    tlv_f_teid_t pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface;
    tlv_paa_t pdn_address_allocation;
    tlv_apn_restriction_t apn_restriction;
    tlv_ambr_t aggregate_maximum_bit_rate;
    tlv_ebi_t linked_eps_bearer_id;
    tlv_pco_t protocol_configuration_options;
    tlv_bearer_context_t bearer_contexts_created;
    tlv_bearer_context_t bearer_contexts_marked_for_removal;
    tlv_recovery_t recovery;
    tlv_fqdn_t charging_gateway_name;
    tlv_ip_address_t charging_gateway_address;
    tlv_fq_csid_t pgw_fq_csid;
    tlv_fq_csid_t sgw_fq_csid;
    tlv_ldn_t sgw_ldn;
    tlv_ldn_t pgw_ldn;
    tlv_epc_timer_t pgw_back_off_time;
    tlv_apco_t additional_protocol_configuration_options;
    tlv_ip4cp_t trusted_wlan_ipv4_parameters_;
    tlv_indication_t indication_flags;
    tlv_presence_reporting_area_action_t presence_reporting_area_action;
    tlv_load_control_information_t pgw_s_node_level_load_control_information;
    tlv_load_control_information_t pgw_s_apn_level_load_control_information;
    tlv_load_control_information_t sgw_s_node_level_load_control_information;
    tlv_overload_control_information_t pgw_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_f_container_t nbifom_container;
    tlv_charging_id_t pdn_connection_charging_id;
    tlv_epco_t extended_protocol_configuration_options;
} gtp_create_session_response_t;

typedef struct _gtp_modify_bearer_request_t {
    tlv_mei_t me_identity;
    tlv_uli_t user_location_information;
    tlv_serving_network_t serving_network;
    tlv_rat_type_t rat_type;
    tlv_indication_t indication_flags;
    tlv_f_teid_t sender_f_teid_for_control_plane;
    tlv_ambr_t aggregate_maximum_bit_rate;
    tlv_delay_value_t delay_downlink_packet_notification_request;
    tlv_bearer_context_t bearer_contexts_to_be_modified;
    tlv_bearer_context_t bearer_contexts_to_be_removed;
    tlv_recovery_t recovery;
    tlv_ue_time_zone_t ue_time_zone;
    tlv_fq_csid_t mme_fq_csid;
    tlv_uci_t user_csg_information;
    tlv_ip_address_t ue_local_ip_address;
    tlv_port_number_t ue_udp_port;
    tlv_ldn_t mme_s4_sgsn_ldn;
    tlv_ldn_t sgw_ldn;
    tlv_ip_address_t hnb_local_ip_address;
    tlv_port_number_t hnb_udp_port;
    tlv_ip_address_t mme_s4_sgsn_identifier;
    tlv_cn_operator_selection_entity_t cn_operator_selection_entity;
    tlv_presence_reporting_area_information_t presence_reporting_area_information;
    tlv_overload_control_information_t mme_s4_sgsn_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_overload_control_information_t epdg_s_overload_control_information;
    tlv_serving_plmn_rate_control_t serving_plmn_rate_control;
    tlv_counter_t mo_exception_data_counter;
} gtp_modify_bearer_request_t;

typedef struct _gtp_modify_bearer_response_t {
    tlv_cause_t cause;
    tlv_msisdn_t msisdn;
    tlv_ebi_t linked_eps_bearer_id;
    tlv_apn_restriction_t apn_restriction;
    tlv_pco_t protocol_configuration_options;
    tlv_bearer_context_t bearer_contexts_modified;
    tlv_bearer_context_t bearer_contexts_marked_for_removal;
    tlv_change_reporting_action_t change_reporting_action;
    tlv_csg_information_reporting_action_t csg_information_reporting_action;
    tlv_enb_information_reporting_t hnb_information_reporting_;
    tlv_fqdn_t charging_gateway_name;
    tlv_ip_address_t charging_gateway_address;
    tlv_fq_csid_t pgw_fq_csid;
    tlv_fq_csid_t sgw_fq_csid;
    tlv_recovery_t recovery;
    tlv_ldn_t sgw_ldn;
    tlv_ldn_t pgw_ldn;
    tlv_indication_t indication_flags;
    tlv_presence_reporting_area_action_t presence_reporting_area_action;
    tlv_load_control_information_t pgw_s_node_level_load_control_information;
    tlv_load_control_information_t pgw_s_apn_level_load_control_information;
    tlv_load_control_information_t sgw_s_node_level_load_control_information;
    tlv_overload_control_information_t pgw_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_charging_id_t pdn_connection_charging_id;
} gtp_modify_bearer_response_t;

typedef struct _gtp_delete_session_request_t {
    tlv_cause_t cause;
    tlv_ebi_t linked_eps_bearer_id;
    tlv_uli_t user_location_information;
    tlv_indication_t indication_flags;
    tlv_pco_t protocol_configuration_options;
    tlv_node_type_t originating_node;
    tlv_f_teid_t sender_f_teid_for_control_plane;
    tlv_ue_time_zone_t ue_time_zone;
    tlv_uli_timestamp_t uli_timestamp;
    tlv_ran_nas_cause_t ran_nas_release_cause;
    tlv_twan_identifier_t twan_identifier;
    tlv_twan_identifier_timestamp_t twan_identifier_timestamp;
    tlv_overload_control_information_t mme_s4_sgsn_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_overload_control_information_t twan_epdg_s_overload_control_information;
    tlv_twan_identifier_t wlan_location_information;
    tlv_twan_identifier_timestamp_t wlan_location_timestamp;
    tlv_ip_address_t ue_local_ip_address;
    tlv_port_number_t ue_udp_port;
    tlv_epco_t extended_protocol_configuration_options;
    tlv_port_number_t ue_tcp_port;
} gtp_delete_session_request_t;

typedef struct _gtp_delete_session_response_t {
    tlv_cause_t cause;
    tlv_recovery_t recovery;
    tlv_pco_t protocol_configuration_options;
    tlv_indication_t indication_flags;
    tlv_load_control_information_t pgw_s_node_level_load_control_information;
    tlv_load_control_information_t pgw_s_apn_level_load_control_information;
    tlv_load_control_information_t sgw_s_node_level_load_control_information;
    tlv_overload_control_information_t pgw_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_epco_t extended_protocol_configuration_options;
} gtp_delete_session_response_t;

typedef struct _gtp_modify_bearer_command_t {
    tlv_ambr_t apn_aggregate_maximum_bit_rate;
    tlv_bearer_context_t bearer_context;
    tlv_overload_control_information_t mme_s4_sgsn_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_overload_control_information_t twan_epdg_s_overload_control_information;
    tlv_f_teid_t sender_f_teid_for_control_plane;
} gtp_modify_bearer_command_t;

typedef struct _gtp_modify_bearer_failure_indication_t {
    tlv_cause_t cause;
    tlv_recovery_t recovery;
    tlv_indication_t indication_flags;
    tlv_overload_control_information_t pgw_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
} gtp_modify_bearer_failure_indication_t;

typedef struct _gtp_delete_bearer_command_t {
    tlv_bearer_context_t bearer_contexts;
    tlv_uli_t user_location_information;
    tlv_uli_timestamp_t uli_timestamp;
    tlv_ue_time_zone_t ue_time_zone;
    tlv_overload_control_information_t mme_s4_sgsn_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_f_teid_t sender_f_teid_for_control_plane;
} gtp_delete_bearer_command_t;

typedef struct _gtp_delete_bearer_failure_indication_t {
    tlv_cause_t cause;
    tlv_bearer_context_t bearer_context;
    tlv_recovery_t recovery;
    tlv_indication_t indication_flags;
    tlv_overload_control_information_t pgw_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
} gtp_delete_bearer_failure_indication_t;

typedef struct _gtp_downlink_data_notification_failure_indication_t {
    tlv_cause_t cause;
    tlv_node_type_t originating_node;
    tlv_imsi_t imsi;
} gtp_downlink_data_notification_failure_indication_t;

typedef struct _gtp_create_bearer_request_t {
    tlv_pti_t procedure_transaction_id;
    tlv_ebi_t linked_eps_bearer_id;
    tlv_pco_t protocol_configuration_options;
    tlv_bearer_context_t bearer_contexts;
    tlv_fq_csid_t pgw_fq_csid;
    tlv_fq_csid_t sgw_fq_csid;
    tlv_change_reporting_action_t change_reporting_action;
    tlv_csg_information_reporting_action_t csg_information_reporting_action;
    tlv_enb_information_reporting_t hnb_information_reporting;
    tlv_presence_reporting_area_action_t presence_reporting_area_action;
    tlv_indication_t indication_flags;
    tlv_load_control_information_t pgw_s_node_level_load_control_information;
    tlv_load_control_information_t pgw_s_apn_level_load_control_information;
    tlv_load_control_information_t sgw_s_node_level_load_control_information;
    tlv_overload_control_information_t pgw_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_f_container_t nbifom_container;
} gtp_create_bearer_request_t;

typedef struct _gtp_create_bearer_response_t {
    tlv_cause_t cause;
    tlv_bearer_context_t bearer_contexts;
    tlv_recovery_t recovery;
    tlv_fq_csid_t mme_fq_csid;
    tlv_fq_csid_t epdg_fq_csid;
    tlv_fq_csid_t twan_fq_csid;
    tlv_pco_t protocol_configuration_options;
    tlv_ue_time_zone_t ue_time_zone;
    tlv_uli_t user_location_information;
    tlv_twan_identifier_t twan_identifier;
    tlv_overload_control_information_t mme_s4_sgsn_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_presence_reporting_area_information_t presence_reporting_area_information;
    tlv_ip_address_t mme_s4_sgsn_identifier;
    tlv_overload_control_information_t twan_epdg_s_overload_control_information;
    tlv_twan_identifier_t wlan_location_information;
    tlv_twan_identifier_timestamp_t wlan_location_timestamp;
    tlv_port_number_t ue_udp_port;
    tlv_f_container_t nbifom_container;
    tlv_port_number_t ue_tcp_port;
} gtp_create_bearer_response_t;

typedef struct _gtp_update_bearer_request_t {
    tlv_bearer_context_t bearer_contexts;
    tlv_pti_t procedure_transaction_id;
    tlv_pco_t protocol_configuration_options;
    tlv_ambr_t aggregate_maximum_bit_rate;
    tlv_change_reporting_action_t change_reporting_action;
    tlv_csg_information_reporting_action_t csg_information_reporting_action;
    tlv_enb_information_reporting_t hnb_information_reporting_;
    tlv_indication_t indication_flags;
    tlv_fq_csid_t pgw_fq_csid;
    tlv_fq_csid_t sgw_fq_csid;
    tlv_presence_reporting_area_action_t presence_reporting_area_action;
    tlv_load_control_information_t pgw_s_node_level_load_control_information;
    tlv_load_control_information_t pgw_s_apn_level_load_control_information;
    tlv_load_control_information_t sgw_s_node_level_load_control_information;
    tlv_overload_control_information_t pgw_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_f_container_t nbifom_container;
} gtp_update_bearer_request_t;

typedef struct _gtp_update_bearer_response_t {
    tlv_cause_t cause;
    tlv_bearer_context_t bearer_contexts;
    tlv_pco_t protocol_configuration_options;
    tlv_recovery_t recovery;
    tlv_fq_csid_t mme_fq_csid;
    tlv_fq_csid_t sgw_fq_csid;
    tlv_fq_csid_t epdg_fq_csid;
    tlv_fq_csid_t twan_fq_csid;
    tlv_indication_t indication_flags;
    tlv_ue_time_zone_t ue_time_zone;
    tlv_uli_t user_location_information;
    tlv_twan_identifier_t twan_identifier;
    tlv_overload_control_information_t mme_s4_sgsn_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_presence_reporting_area_information_t presence_reporting_area_information;
    tlv_ip_address_t mme_s4_sgsn_identifier;
    tlv_overload_control_information_t twan_epdg_s_overload_control_information;
    tlv_twan_identifier_t wlan_location_information;
    tlv_twan_identifier_timestamp_t wlan_location_timestamp;
    tlv_port_number_t ue_udp_port;
    tlv_f_container_t nbifom_container;
    tlv_port_number_t ue_tcp_port;
} gtp_update_bearer_response_t;

typedef struct _gtp_delete_bearer_request_t {
    tlv_ebi_t linked_eps_bearer_id;
    tlv_ebi_t eps_bearer_ids;
    tlv_bearer_context_t failed_bearer_contexts;
    tlv_pti_t procedure_transaction_id;
    tlv_pco_t protocol_configuration_options;
    tlv_fq_csid_t pgw_fq_csid;
    tlv_fq_csid_t sgw_fq_csid;
    tlv_cause_t cause;
    tlv_indication_t indication_flags;
    tlv_load_control_information_t pgw_s_node_level_load_control_information;
    tlv_load_control_information_t pgw_s_apn_level_load_control_information;
    tlv_load_control_information_t sgw_s_node_level_load_control_information;
    tlv_overload_control_information_t pgw_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_f_container_t nbifom_container;
    tlv_epco_t extended_protocol_configuration_options;
} gtp_delete_bearer_request_t;

typedef struct _gtp_delete_bearer_response_t {
    tlv_cause_t cause;
    tlv_ebi_t linked_eps_bearer_id;
    tlv_bearer_context_t bearer_contexts;
    tlv_recovery_t recovery;
    tlv_fq_csid_t mme_fq_csid;
    tlv_fq_csid_t sgw_fq_csid;
    tlv_fq_csid_t epdg_fq_csid;
    tlv_fq_csid_t twan_fq_csid;
    tlv_pco_t protocol_configuration_options;
    tlv_ue_time_zone_t ue_time_zone;
    tlv_uli_t user_location_information;
    tlv_uli_timestamp_t uli_timestamp;
    tlv_twan_identifier_t twan_identifier;
    tlv_twan_identifier_timestamp_t twan_identifier_timestamp;
    tlv_overload_control_information_t mme_s4_sgsn_s_overload_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_ip_address_t mme_s4_sgsn_identifier;
    tlv_overload_control_information_t twan_epdg_s_overload_control_information;
    tlv_twan_identifier_t wlan_location_information;
    tlv_twan_identifier_timestamp_t wlan_location_timestamp;
    tlv_port_number_t ue_udp_port;
    tlv_f_container_t nbifom_container;
    tlv_port_number_t ue_tcp_port	;
} gtp_delete_bearer_response_t;

typedef struct _gtp_create_indirect_data_forwarding_tunnel_request_t {
    tlv_imsi_t imsi;
    tlv_mei_t me_identity;
    tlv_indication_t indication_flags;
    tlv_f_teid_t sender_f_teid_for_control_plane;
    tlv_bearer_context_t bearer_context_0;
    tlv_bearer_context_t bearer_context_1;
    tlv_bearer_context_t bearer_context_2;
    tlv_bearer_context_t bearer_context_3;
    tlv_bearer_context_t bearer_context_4;
    tlv_bearer_context_t bearer_context_5;
    tlv_bearer_context_t bearer_context_6;
    tlv_bearer_context_t bearer_context_7;
    tlv_bearer_context_t bearer_context_8;
    tlv_bearer_context_t bearer_context_9;
    tlv_bearer_context_t bearer_context_10;
    tlv_recovery_t recovery;
} gtp_create_indirect_data_forwarding_tunnel_request_t;

typedef struct _gtp_create_indirect_data_forwarding_tunnel_response_t {
    tlv_cause_t cause;
    tlv_f_teid_t sender_f_teid_for_control_plane;
    tlv_bearer_context_t bearer_context_0;
    tlv_bearer_context_t bearer_context_1;
    tlv_bearer_context_t bearer_context_2;
    tlv_bearer_context_t bearer_context_3;
    tlv_bearer_context_t bearer_context_4;
    tlv_bearer_context_t bearer_context_5;
    tlv_bearer_context_t bearer_context_6;
    tlv_bearer_context_t bearer_context_7;
    tlv_bearer_context_t bearer_context_8;
    tlv_bearer_context_t bearer_context_9;
    tlv_bearer_context_t bearer_context_10;
    tlv_recovery_t recovery;
} gtp_create_indirect_data_forwarding_tunnel_response_t;

typedef struct _gtp_delete_indirect_data_forwarding_tunnel_request_t {
} gtp_delete_indirect_data_forwarding_tunnel_request_t;

typedef struct _gtp_delete_indirect_data_forwarding_tunnel_response_t {
    tlv_cause_t cause;
    tlv_recovery_t recovery;
} gtp_delete_indirect_data_forwarding_tunnel_response_t;

typedef struct _gtp_release_access_bearers_request_t {
    tlv_ebi_t list_of_rabs;
    tlv_node_type_t originating_node;
    tlv_indication_t indication_flags;
} gtp_release_access_bearers_request_t;

typedef struct _gtp_release_access_bearers_response_t {
    tlv_cause_t cause;
    tlv_recovery_t recovery;
    tlv_indication_t indication_flags;
    tlv_load_control_information_t sgw_s_node_level_load_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
} gtp_release_access_bearers_response_t;

typedef struct _gtp_downlink_data_notification_t {
    tlv_cause_t cause;
    tlv_ebi_t eps_bearer_id;
    tlv_arp_t allocation_retention_priority;
    tlv_imsi_t imsi;
    tlv_f_teid_t sender_f_teid_for_control_plane;
    tlv_indication_t indication_flags;
    tlv_load_control_information_t sgw_s_node_level_load_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
    tlv_paging_and_service_information_t paging_and_service_information;
} gtp_downlink_data_notification_t;

typedef struct _gtp_downlink_data_notification_acknowledge_t {
    tlv_cause_t cause;
    tlv_delay_value_t data_notification_delay;
    tlv_recovery_t recovery;
    tlv_throttling_t dl_low_priority_traffic_throttling_;
    tlv_imsi_t imsi;
    tlv_epc_timer_t dl_buffering_duration;
    tlv_integer_number_t dl_buffering_suggested_packet_count;
} gtp_downlink_data_notification_acknowledge_t;

typedef struct _gtp_modify_access_bearers_request_t {
    tlv_indication_t indication_flags;
    tlv_f_teid_t sender_f_teid_for_control_plane;
    tlv_delay_value_t delay_downlink_packet_notification_request;
    tlv_bearer_context_t bearer_contexts_to_be_modified;
    tlv_bearer_context_t bearer_contexts_to_be_removed;
    tlv_recovery_t recovery;
} gtp_modify_access_bearers_request_t;

typedef struct _gtp_modify_access_bearers_response_t {
    tlv_cause_t cause;
    tlv_bearer_context_t bearer_contexts_modified;
    tlv_bearer_context_t bearer_contexts_marked_for_removal;
    tlv_recovery_t recovery;
    tlv_indication_t indication_flags;
    tlv_load_control_information_t sgw_s_node_level_load_control_information;
    tlv_overload_control_information_t sgw_s_overload_control_information;
} gtp_modify_access_bearers_response_t;

typedef struct _gtp_message_t {
   gtp_header_t h;
   union {
        gtp_echo_request_t echo_request;
        gtp_echo_response_t echo_response;
        gtp_create_session_request_t create_session_request;
        gtp_create_session_response_t create_session_response;
        gtp_modify_bearer_request_t modify_bearer_request;
        gtp_modify_bearer_response_t modify_bearer_response;
        gtp_delete_session_request_t delete_session_request;
        gtp_delete_session_response_t delete_session_response;
        gtp_modify_bearer_command_t modify_bearer_command;
        gtp_modify_bearer_failure_indication_t modify_bearer_failure_indication;
        gtp_delete_bearer_command_t delete_bearer_command;
        gtp_delete_bearer_failure_indication_t delete_bearer_failure_indication;
        gtp_downlink_data_notification_failure_indication_t downlink_data_notification_failure_indication;
        gtp_create_bearer_request_t create_bearer_request;
        gtp_create_bearer_response_t create_bearer_response;
        gtp_update_bearer_request_t update_bearer_request;
        gtp_update_bearer_response_t update_bearer_response;
        gtp_delete_bearer_request_t delete_bearer_request;
        gtp_delete_bearer_response_t delete_bearer_response;
        gtp_create_indirect_data_forwarding_tunnel_request_t create_indirect_data_forwarding_tunnel_request;
        gtp_create_indirect_data_forwarding_tunnel_response_t create_indirect_data_forwarding_tunnel_response;
        gtp_delete_indirect_data_forwarding_tunnel_request_t delete_indirect_data_forwarding_tunnel_request;
        gtp_delete_indirect_data_forwarding_tunnel_response_t delete_indirect_data_forwarding_tunnel_response;
        gtp_release_access_bearers_request_t release_access_bearers_request;
        gtp_release_access_bearers_response_t release_access_bearers_response;
        gtp_downlink_data_notification_t downlink_data_notification;
        gtp_downlink_data_notification_acknowledge_t downlink_data_notification_acknowledge;
        gtp_modify_access_bearers_request_t modify_access_bearers_request;
        gtp_modify_access_bearers_response_t modify_access_bearers_response;
   };
} gtp_message_t;

CORE_DECLARE(status_t) gtp_parse_msg(
        gtp_message_t *gtp_message, pkbuf_t *pkbuf);
CORE_DECLARE(status_t) gtp_build_msg(
        pkbuf_t **pkbuf, gtp_message_t *gtp_message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_TLV_H__ */
