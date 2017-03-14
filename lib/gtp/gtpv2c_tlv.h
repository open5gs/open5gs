/*
 * Copyright (c) 2017, CellWire Group
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
 * This file had been created by gtpv2c_tlv.py script v0.1.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2017-03-14 17:23:42.957665 by acetcom
 * from 29274-d80.docx
 ******************************************************************************/

#ifndef __GTPV2C_TLV_H__
#define __GTPV2C_TLV_H__

#include "core_tlv_msg.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTPV2C_MSG_ECHO_REQUEST_TYPE 1
#define GTPV2C_MSG_ECHO_RESPONSE_TYPE 2
#define GTPV2C_MSG_VERSION_NOT_SUPPORTED_INDICATION_TYPE 3
#define GTPV2C_MSG_CREATE_SESSION_REQUEST_TYPE 32
#define GTPV2C_MSG_CREATE_SESSION_RESPONSE_TYPE 33
#define GTPV2C_MSG_MODIFY_BEARER_REQUEST_TYPE 34
#define GTPV2C_MSG_MODIFY_BEARER_RESPONSE_TYPE 35
#define GTPV2C_MSG_DELETE_SESSION_REQUEST_TYPE 36
#define GTPV2C_MSG_DELETE_SESSION_RESPONSE_TYPE 37
#define GTPV2C_MSG_CHANGE_NOTIFICATION_REQUEST_TYPE 38
#define GTPV2C_MSG_CHANGE_NOTIFICATION_RESPONSE_TYPE 39
#define GTPV2C_MSG_REMOTE_UE_REPORT_NOTIFICATION_TYPE 40
#define GTPV2C_MSG_REMOTE_UE_REPORT_ACKNOWLEDGE_TYPE 41
#define GTPV2C_MSG_MODIFY_BEARER_COMMAND_TYPE 64
#define GTPV2C_MSG_MODIFY_BEARER_FAILURE_INDICATION_TYPE 65
#define GTPV2C_MSG_DELETE_BEARER_COMMAND_TYPE 66
#define GTPV2C_MSG_DELETE_BEARER_FAILURE_INDICATION_TYPE 67
#define GTPV2C_MSG_BEARER_RESOURCE_COMMAND_TYPE 68
#define GTPV2C_MSG_BEARER_RESOURCE_FAILURE_INDICATION_TYPE 69
#define GTPV2C_MSG_DOWNLINK_DATA_NOTIFICATION_FAILURE_INDICATION_TYPE 70
#define GTPV2C_MSG_TRACE_SESSION_ACTIVATION_TYPE 71
#define GTPV2C_MSG_TRACE_SESSION_DEACTIVATION_TYPE 72
#define GTPV2C_MSG_STOP_PAGING_INDICATION_TYPE 73
#define GTPV2C_MSG_CREATE_BEARER_REQUEST_TYPE 95
#define GTPV2C_MSG_CREATE_BEARER_RESPONSE_TYPE 96
#define GTPV2C_MSG_UPDATE_BEARER_REQUEST_TYPE 97
#define GTPV2C_MSG_UPDATE_BEARER_RESPONSE_TYPE 98
#define GTPV2C_MSG_DELETE_BEARER_REQUEST_TYPE 99
#define GTPV2C_MSG_DELETE_BEARER_RESPONSE_TYPE 100
#define GTPV2C_MSG_DELETE_PDN_CONNECTION_SET_REQUEST_TYPE 101
#define GTPV2C_MSG_DELETE_PDN_CONNECTION_SET_RESPONSE_TYPE 102
#define GTPV2C_MSG_PGW_DOWNLINK_TRIGGERING_NOTIFICATION_TYPE 103
#define GTPV2C_MSG_PGW_DOWNLINK_TRIGGERING_ACKNOWLEDGE_TYPE 104
#define GTPV2C_MSG_CREATE_FORWARDING_TUNNEL_REQUEST_TYPE 160
#define GTPV2C_MSG_CREATE_FORWARDING_TUNNEL_RESPONSE_TYPE 161
#define GTPV2C_MSG_SUSPEND_NOTIFICATION_TYPE 162
#define GTPV2C_MSG_SUSPEND_ACKNOWLEDGE_TYPE 163
#define GTPV2C_MSG_RESUME_NOTIFICATION_TYPE 164
#define GTPV2C_MSG_RESUME_ACKNOWLEDGE_TYPE 165
#define GTPV2C_MSG_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE 166
#define GTPV2C_MSG_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE 167
#define GTPV2C_MSG_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE 168
#define GTPV2C_MSG_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE 169
#define GTPV2C_MSG_RELEASE_ACCESS_BEARERS_REQUEST_TYPE 170
#define GTPV2C_MSG_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE 171
#define GTPV2C_MSG_DOWNLINK_DATA_NOTIFICATION_TYPE 176
#define GTPV2C_MSG_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE 177
#define GTPV2C_MSG_PGW_RESTART_NOTIFICATION_TYPE 179
#define GTPV2C_MSG_PGW_RESTART_NOTIFICATION_ACKNOWLEDGE_TYPE 180
#define GTPV2C_MSG_UPDATE_PDN_CONNECTION_SET_REQUEST_TYPE 200
#define GTPV2C_MSG_UPDATE_PDN_CONNECTION_SET_RESPONSE_TYPE 201
#define GTPV2C_MSG_MODIFY_ACCESS_BEARERS_REQUEST_TYPE 211
#define GTPV2C_MSG_MODIFY_ACCESS_BEARERS_RESPONSE_TYPE 212

#define GTPV2C_IE_IMSI_TYPE 1
#define GTPV2C_IE_CAUSE_TYPE 2
#define GTPV2C_IE_RECOVERY_TYPE 3
#define GTPV2C_IE_STN_SR_TYPE 51
#define GTPV2C_IE_APN_TYPE 71
#define GTPV2C_IE_AMBR_TYPE 72
#define GTPV2C_IE_EBI_TYPE 73
#define GTPV2C_IE_IP_ADDRESS_TYPE 74
#define GTPV2C_IE_MEI_TYPE 75
#define GTPV2C_IE_MSISDN_TYPE 76
#define GTPV2C_IE_INDICATION_TYPE 77
#define GTPV2C_IE_PCO_TYPE 78
#define GTPV2C_IE_PAA_TYPE 79
#define GTPV2C_IE_BEARER_QOS_TYPE 80
#define GTPV2C_IE_FLOW_QOS_TYPE 81
#define GTPV2C_IE_RAT_TYPE_TYPE 82
#define GTPV2C_IE_SERVING_NETWORK_TYPE 83
#define GTPV2C_IE_BEARER_TFT_TYPE 84
#define GTPV2C_IE_TAD_TYPE 85
#define GTPV2C_IE_ULI_TYPE 86
#define GTPV2C_IE_F_TEID_TYPE 87
#define GTPV2C_IE_TMSI_TYPE 88
#define GTPV2C_IE_GLOBAL_CN_ID_TYPE 89
#define GTPV2C_IE_S103PDF_TYPE 90
#define GTPV2C_IE_S1UDF_TYPE 91
#define GTPV2C_IE_DELAY_VALUE_TYPE 92
#define GTPV2C_IE_BEARER_CONTEXT_TYPE 93
#define GTPV2C_IE_CHARGING_ID_TYPE 94
#define GTPV2C_IE_CHARGING_CHARACTERISTICS_TYPE 95
#define GTPV2C_IE_TRACE_INFORMATION_TYPE 96
#define GTPV2C_IE_BEARER_FLAGS_TYPE 97
#define GTPV2C_IE_PDN_TYPE_TYPE 99
#define GTPV2C_IE_PROCEDURE_TRANSACTION_ID_TYPE 100
#define GTPV2C_IE_MM_CONTEXT_TYPE 107
#define GTPV2C_IE_PDN_CONNECTION_TYPE 109
#define GTPV2C_IE_PDU_NUMBERS_TYPE 110
#define GTPV2C_IE_P_TMSI_TYPE 111
#define GTPV2C_IE_P_TMSI_SIGNATURE_TYPE 112
#define GTPV2C_IE_HOP_COUNTER_TYPE 113
#define GTPV2C_IE_UE_TIME_ZONE_TYPE 114
#define GTPV2C_IE_TRACE_REFERENCE_TYPE 115
#define GTPV2C_IE_COMPLETE_REQUEST_MESSAGE_TYPE 116
#define GTPV2C_IE_GUTI_TYPE 117
#define GTPV2C_IE_F_CONTAINER_TYPE 118
#define GTPV2C_IE_F_CAUSE_TYPE 119
#define GTPV2C_IE_PLMN_ID_TYPE 120
#define GTPV2C_IE_TARGET_IDENTIFICATION_TYPE 121
#define GTPV2C_IE_PACKET_FLOW_ID_TYPE 123
#define GTPV2C_IE_RAB_CONTEXT_TYPE 124
#define GTPV2C_IE_SOURCE_RNC_PDCP_CONTEXT_INFO_TYPE 125
#define GTPV2C_IE_PORT_NUMBER_TYPE 126
#define GTPV2C_IE_APN_RESTRICTION_TYPE 127
#define GTPV2C_IE_SELECTION_MODE_TYPE 128
#define GTPV2C_IE_SOURCE_IDENTIFICATION_TYPE 129
#define GTPV2C_IE_CHANGE_REPORTING_ACTION_TYPE 131
#define GTPV2C_IE_FQ_CSID_TYPE 132
#define GTPV2C_IE_CHANNEL_NEEDED_TYPE 133
#define GTPV2C_IE_EMLPP_PRIORITY_TYPE 134
#define GTPV2C_IE_NODE_TYPE_TYPE 135
#define GTPV2C_IE_FQDN_TYPE 136
#define GTPV2C_IE_TI_TYPE 137
#define GTPV2C_IE_MBMS_SESSION_DURATION_TYPE 138
#define GTPV2C_IE_MBMS_SERVICE_AREA_TYPE 139
#define GTPV2C_IE_MBMS_SESSION_IDENTIFIER_TYPE 140
#define GTPV2C_IE_MBMS_FLOW_IDENTIFIER_TYPE 141
#define GTPV2C_IE_MBMS_IP_MULTICAST_DISTRIBUTION_TYPE 142
#define GTPV2C_IE_MBMS_DISTRIBUTION_ACKNOWLEDGE_TYPE 143
#define GTPV2C_IE_RFSP_INDEX_TYPE 144
#define GTPV2C_IE_UCI_TYPE 145
#define GTPV2C_IE_CSG_INFORMATION_REPORTING_ACTION_TYPE 146
#define GTPV2C_IE_CSG_ID_TYPE 147
#define GTPV2C_IE_CMI_TYPE 148
#define GTPV2C_IE_SERVICE_INDICATOR_TYPE 149
#define GTPV2C_IE_DETACH_TYPE_TYPE 150
#define GTPV2C_IE_LDN_TYPE 151
#define GTPV2C_IE_NODE_FEATURES_TYPE 152
#define GTPV2C_IE_MBMS_TIME_TO_DATA_TRANSFER_TYPE 153
#define GTPV2C_IE_THROTTLING_TYPE 154
#define GTPV2C_IE_ARP_TYPE 155
#define GTPV2C_IE_EPC_TIMER_TYPE 156
#define GTPV2C_IE_SIGNALLING_PRIORITY_INDICATION_TYPE 157
#define GTPV2C_IE_TMGI_TYPE 158
#define GTPV2C_IE_ADDITIONAL_MM_CONTEXT_FOR_SRVCC_TYPE 159
#define GTPV2C_IE_ADDITIONAL_FLAGS_FOR_SRVCC_TYPE 160
#define GTPV2C_IE_MDT_CONFIGURATION_TYPE 162
#define GTPV2C_IE_APCO_TYPE 163
#define GTPV2C_IE_ABSOLUTE_TIME_OF_MBMS_DATA_TRANSFER_TYPE 164
#define GTPV2C_IE_ENB_INFORMATION_REPORTING_TYPE 165
#define GTPV2C_IE_IP4CP_TYPE 166
#define GTPV2C_IE_CHANGE_TO_REPORT_FLAGS_TYPE 167
#define GTPV2C_IE_ACTION_INDICATION_TYPE 168
#define GTPV2C_IE_TWAN_IDENTIFIER_TYPE 169
#define GTPV2C_IE_ULI_TIMESTAMP_TYPE 170
#define GTPV2C_IE_MBMS_FLAGS_TYPE 171
#define GTPV2C_IE_RAN_NAS_CAUSE_TYPE 172
#define GTPV2C_IE_CN_OPERATOR_SELECTION_ENTITY_TYPE 173
#define GTPV2C_IE_TWMI_TYPE 174
#define GTPV2C_IE_NODE_NUMBER_TYPE 175
#define GTPV2C_IE_NODE_IDENTIFIER_TYPE 176
#define GTPV2C_IE_PRESENCE_REPORTING_AREA_ACTION_TYPE 177
#define GTPV2C_IE_PRESENCE_REPORTING_AREA_INFORMATION_TYPE 178
#define GTPV2C_IE_TWAN_IDENTIFIER_TIMESTAMP_TYPE 179
#define GTPV2C_IE_OVERLOAD_CONTROL_INFORMATION_TYPE 180
#define GTPV2C_IE_LOAD_CONTROL_INFORMATION_TYPE 181
#define GTPV2C_IE_METRIC_TYPE 182
#define GTPV2C_IE_SEQUENCE_NUMBER_TYPE 183
#define GTPV2C_IE_APN_AND_RELATIVE_CAPACITY_TYPE 184
#define GTPV2C_IE_WLAN_OFFLOADABILITY_INDICATION_TYPE 185
#define GTPV2C_IE_PAGING_AND_SERVICE_INFORMATION_TYPE 186
#define GTPV2C_IE_INTEGER_NUMBER_TYPE 187
#define GTPV2C_IE_MILLISECOND_TIME_STAMP_TYPE 188
#define GTPV2C_IE_MONITORING_EVENT_INFORMATION_TYPE 189
#define GTPV2C_IE_ECGI_LIST_TYPE 190
#define GTPV2C_IE_REMOTE_UE_CONTEXT_TYPE 191
#define GTPV2C_IE_REMOTE_USER_ID_TYPE 192
#define GTPV2C_IE_REMOTE_UE_IP_INFORMATION_TYPE 193
#define GTPV2C_IE_CIOT_OPTIMIZATIONS_SUPPORT_INDICATION_TYPE 194
#define GTPV2C_IE_SCEF_PDN_CONNECTION_TYPE 195
#define GTPV2C_IE_HEADER_COMPRESSION_CONFIGURATION_TYPE 196
#define GTPV2C_IE_EPCO_TYPE 197
#define GTPV2C_IE_SERVING_PLMN_RATE_CONTROL_TYPE 198
#define GTPV2C_IE_COUNTER_TYPE 199

typedef tlv_octet_t tlv_imsi_t;
typedef tlv_octet_t tlv_cause_t;
typedef tlv_octet_t tlv_recovery_t;
typedef tlv_octet_t tlv_stn_sr_t;
typedef tlv_octet_t tlv_apn_t;
typedef tlv_octet_t tlv_ambr_t;
typedef tlv_octet_t tlv_ebi_t;
typedef tlv_octet_t tlv_ip_address_t;
typedef tlv_octet_t tlv_mei_t;
typedef tlv_octet_t tlv_msisdn_t;
typedef tlv_octet_t tlv_indication_t;
typedef tlv_octet_t tlv_pco_t;
typedef tlv_octet_t tlv_paa_t;
typedef tlv_octet_t tlv_bearer_qos_t;
typedef tlv_octet_t tlv_flow_qos_t;
typedef tlv_octet_t tlv_rat_type_t;
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
typedef tlv_octet_t tlv_bearer_context_t;
typedef tlv_octet_t tlv_charging_id_t;
typedef tlv_octet_t tlv_charging_characteristics_t;
typedef tlv_octet_t tlv_trace_information_t;
typedef tlv_octet_t tlv_bearer_flags_t;
typedef tlv_octet_t tlv_pdn_type_t;
typedef tlv_octet_t tlv_procedure_transaction_id_t;
typedef tlv_octet_t tlv_mm_context_t;
typedef tlv_octet_t tlv_pdn_connection_t;
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
typedef tlv_octet_t tlv_port_number_t;
typedef tlv_octet_t tlv_apn_restriction_t;
typedef tlv_octet_t tlv_selection_mode_t;
typedef tlv_octet_t tlv_source_identification_t;
typedef tlv_octet_t tlv_change_reporting_action_t;
typedef tlv_octet_t tlv_fq_csid_t;
typedef tlv_octet_t tlv_channel_needed_t;
typedef tlv_octet_t tlv_emlpp_priority_t;
typedef tlv_octet_t tlv_node_type_t;
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
typedef tlv_octet_t tlv_overload_control_information_t;
typedef tlv_octet_t tlv_load_control_information_t;
typedef tlv_octet_t tlv_metric_t;
typedef tlv_octet_t tlv_sequence_number_t;
typedef tlv_octet_t tlv_apn_and_relative_capacity_t;
typedef tlv_octet_t tlv_wlan_offloadability_indication_t;
typedef tlv_octet_t tlv_paging_and_service_information_t;
typedef tlv_octet_t tlv_integer_number_t;
typedef tlv_octet_t tlv_millisecond_time_stamp_t;
typedef tlv_octet_t tlv_monitoring_event_information_t;
typedef tlv_octet_t tlv_ecgi_list_t;
typedef tlv_octet_t tlv_remote_ue_context_t;
typedef tlv_octet_t tlv_remote_user_id_t;
typedef tlv_octet_t tlv_remote_ue_ip_information_t;
typedef tlv_octet_t tlv_ciot_optimizations_support_indication_t;
typedef tlv_octet_t tlv_scef_pdn_connection_t;
typedef tlv_octet_t tlv_header_compression_configuration_t;
typedef tlv_octet_t tlv_epco_t;
typedef tlv_octet_t tlv_serving_plmn_rate_control_t;
typedef tlv_octet_t tlv_counter_t;

extern tlv_desc_t tlv_desc_imsi;
extern tlv_desc_t tlv_desc_cause;
extern tlv_desc_t tlv_desc_recovery;
extern tlv_desc_t tlv_desc_stn_sr;
extern tlv_desc_t tlv_desc_apn;
extern tlv_desc_t tlv_desc_ambr;
extern tlv_desc_t tlv_desc_ebi;
extern tlv_desc_t tlv_desc_ip_address;
extern tlv_desc_t tlv_desc_mei;
extern tlv_desc_t tlv_desc_msisdn;
extern tlv_desc_t tlv_desc_indication;
extern tlv_desc_t tlv_desc_pco;
extern tlv_desc_t tlv_desc_paa;
extern tlv_desc_t tlv_desc_bearer_qos;
extern tlv_desc_t tlv_desc_flow_qos;
extern tlv_desc_t tlv_desc_rat_type;
extern tlv_desc_t tlv_desc_serving_network;
extern tlv_desc_t tlv_desc_bearer_tft;
extern tlv_desc_t tlv_desc_tad;
extern tlv_desc_t tlv_desc_uli;
extern tlv_desc_t tlv_desc_f_teid;
extern tlv_desc_t tlv_desc_tmsi;
extern tlv_desc_t tlv_desc_global_cn_id;
extern tlv_desc_t tlv_desc_s103pdf;
extern tlv_desc_t tlv_desc_s1udf;
extern tlv_desc_t tlv_desc_delay_value;
extern tlv_desc_t tlv_desc_bearer_context;
extern tlv_desc_t tlv_desc_charging_id;
extern tlv_desc_t tlv_desc_charging_characteristics;
extern tlv_desc_t tlv_desc_trace_information;
extern tlv_desc_t tlv_desc_bearer_flags;
extern tlv_desc_t tlv_desc_pdn_type;
extern tlv_desc_t tlv_desc_procedure_transaction_id;
extern tlv_desc_t tlv_desc_mm_context;
extern tlv_desc_t tlv_desc_pdn_connection;
extern tlv_desc_t tlv_desc_pdu_numbers;
extern tlv_desc_t tlv_desc_p_tmsi;
extern tlv_desc_t tlv_desc_p_tmsi_signature;
extern tlv_desc_t tlv_desc_hop_counter;
extern tlv_desc_t tlv_desc_ue_time_zone;
extern tlv_desc_t tlv_desc_trace_reference;
extern tlv_desc_t tlv_desc_complete_request_message;
extern tlv_desc_t tlv_desc_guti;
extern tlv_desc_t tlv_desc_f_container;
extern tlv_desc_t tlv_desc_f_cause;
extern tlv_desc_t tlv_desc_plmn_id;
extern tlv_desc_t tlv_desc_target_identification;
extern tlv_desc_t tlv_desc_packet_flow_id;
extern tlv_desc_t tlv_desc_rab_context;
extern tlv_desc_t tlv_desc_source_rnc_pdcp_context_info;
extern tlv_desc_t tlv_desc_port_number;
extern tlv_desc_t tlv_desc_apn_restriction;
extern tlv_desc_t tlv_desc_selection_mode;
extern tlv_desc_t tlv_desc_source_identification;
extern tlv_desc_t tlv_desc_change_reporting_action;
extern tlv_desc_t tlv_desc_fq_csid;
extern tlv_desc_t tlv_desc_channel_needed;
extern tlv_desc_t tlv_desc_emlpp_priority;
extern tlv_desc_t tlv_desc_node_type;
extern tlv_desc_t tlv_desc_fqdn;
extern tlv_desc_t tlv_desc_ti;
extern tlv_desc_t tlv_desc_mbms_session_duration;
extern tlv_desc_t tlv_desc_mbms_service_area;
extern tlv_desc_t tlv_desc_mbms_session_identifier;
extern tlv_desc_t tlv_desc_mbms_flow_identifier;
extern tlv_desc_t tlv_desc_mbms_ip_multicast_distribution;
extern tlv_desc_t tlv_desc_mbms_distribution_acknowledge;
extern tlv_desc_t tlv_desc_rfsp_index;
extern tlv_desc_t tlv_desc_uci;
extern tlv_desc_t tlv_desc_csg_information_reporting_action;
extern tlv_desc_t tlv_desc_csg_id;
extern tlv_desc_t tlv_desc_cmi;
extern tlv_desc_t tlv_desc_service_indicator;
extern tlv_desc_t tlv_desc_detach_type;
extern tlv_desc_t tlv_desc_ldn;
extern tlv_desc_t tlv_desc_node_features;
extern tlv_desc_t tlv_desc_mbms_time_to_data_transfer;
extern tlv_desc_t tlv_desc_throttling;
extern tlv_desc_t tlv_desc_arp;
extern tlv_desc_t tlv_desc_epc_timer;
extern tlv_desc_t tlv_desc_signalling_priority_indication;
extern tlv_desc_t tlv_desc_tmgi;
extern tlv_desc_t tlv_desc_additional_mm_context_for_srvcc;
extern tlv_desc_t tlv_desc_additional_flags_for_srvcc;
extern tlv_desc_t tlv_desc_mdt_configuration;
extern tlv_desc_t tlv_desc_apco;
extern tlv_desc_t tlv_desc_absolute_time_of_mbms_data_transfer;
extern tlv_desc_t tlv_desc_enb_information_reporting;
extern tlv_desc_t tlv_desc_ip4cp;
extern tlv_desc_t tlv_desc_change_to_report_flags;
extern tlv_desc_t tlv_desc_action_indication;
extern tlv_desc_t tlv_desc_twan_identifier;
extern tlv_desc_t tlv_desc_uli_timestamp;
extern tlv_desc_t tlv_desc_mbms_flags;
extern tlv_desc_t tlv_desc_ran_nas_cause;
extern tlv_desc_t tlv_desc_cn_operator_selection_entity;
extern tlv_desc_t tlv_desc_twmi;
extern tlv_desc_t tlv_desc_node_number;
extern tlv_desc_t tlv_desc_node_identifier;
extern tlv_desc_t tlv_desc_presence_reporting_area_action;
extern tlv_desc_t tlv_desc_presence_reporting_area_information;
extern tlv_desc_t tlv_desc_twan_identifier_timestamp;
extern tlv_desc_t tlv_desc_overload_control_information;
extern tlv_desc_t tlv_desc_load_control_information;
extern tlv_desc_t tlv_desc_metric;
extern tlv_desc_t tlv_desc_sequence_number;
extern tlv_desc_t tlv_desc_apn_and_relative_capacity;
extern tlv_desc_t tlv_desc_wlan_offloadability_indication;
extern tlv_desc_t tlv_desc_paging_and_service_information;
extern tlv_desc_t tlv_desc_integer_number;
extern tlv_desc_t tlv_desc_millisecond_time_stamp;
extern tlv_desc_t tlv_desc_monitoring_event_information;
extern tlv_desc_t tlv_desc_ecgi_list;
extern tlv_desc_t tlv_desc_remote_ue_context;
extern tlv_desc_t tlv_desc_remote_user_id;
extern tlv_desc_t tlv_desc_remote_ue_ip_information;
extern tlv_desc_t tlv_desc_ciot_optimizations_support_indication;
extern tlv_desc_t tlv_desc_scef_pdn_connection;
extern tlv_desc_t tlv_desc_header_compression_configuration;
extern tlv_desc_t tlv_desc_epco;
extern tlv_desc_t tlv_desc_serving_plmn_rate_control;
extern tlv_desc_t tlv_desc_counter;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTPV2C_TLV_H__ */
