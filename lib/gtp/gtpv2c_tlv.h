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
 * Created on: 2017-03-15 16:25:39.781353 by acetcom
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

/* Infomration Element TLV Descriptor */
extern tlv_desc_t gtpv2c_desc_imsi_0;
extern tlv_desc_t gtpv2c_desc_cause_0;
extern tlv_desc_t gtpv2c_desc_recovery_0;
extern tlv_desc_t gtpv2c_desc_stn_sr_0;
extern tlv_desc_t gtpv2c_desc_apn_0;
extern tlv_desc_t gtpv2c_desc_ambr_0;
extern tlv_desc_t gtpv2c_desc_ebi_0;
extern tlv_desc_t gtpv2c_desc_ip_address_0;
extern tlv_desc_t gtpv2c_desc_ip_address_1;
extern tlv_desc_t gtpv2c_desc_ip_address_2;
extern tlv_desc_t gtpv2c_desc_ip_address_3;
extern tlv_desc_t gtpv2c_desc_mei_0;
extern tlv_desc_t gtpv2c_desc_msisdn_0;
extern tlv_desc_t gtpv2c_desc_indication_0;
extern tlv_desc_t gtpv2c_desc_pco_0;
extern tlv_desc_t gtpv2c_desc_paa_0;
extern tlv_desc_t gtpv2c_desc_bearer_qos_0;
extern tlv_desc_t gtpv2c_desc_flow_qos_0;
extern tlv_desc_t gtpv2c_desc_rat_type_0;
extern tlv_desc_t gtpv2c_desc_serving_network_0;
extern tlv_desc_t gtpv2c_desc_bearer_tft_0;
extern tlv_desc_t gtpv2c_desc_tad_0;
extern tlv_desc_t gtpv2c_desc_uli_0;
extern tlv_desc_t gtpv2c_desc_f_teid_0;
extern tlv_desc_t gtpv2c_desc_f_teid_1;
extern tlv_desc_t gtpv2c_desc_f_teid_2;
extern tlv_desc_t gtpv2c_desc_f_teid_3;
extern tlv_desc_t gtpv2c_desc_f_teid_4;
extern tlv_desc_t gtpv2c_desc_f_teid_5;
extern tlv_desc_t gtpv2c_desc_f_teid_6;
extern tlv_desc_t gtpv2c_desc_f_teid_7;
extern tlv_desc_t gtpv2c_desc_f_teid_8;
extern tlv_desc_t gtpv2c_desc_f_teid_9;
extern tlv_desc_t gtpv2c_desc_f_teid_10;
extern tlv_desc_t gtpv2c_desc_f_teid_11;
extern tlv_desc_t gtpv2c_desc_tmsi_0;
extern tlv_desc_t gtpv2c_desc_global_cn_id_0;
extern tlv_desc_t gtpv2c_desc_s103pdf_0;
extern tlv_desc_t gtpv2c_desc_s1udf_0;
extern tlv_desc_t gtpv2c_desc_delay_value_0;
extern tlv_desc_t gtpv2c_desc_charging_id_0;
extern tlv_desc_t gtpv2c_desc_charging_characteristics_0;
extern tlv_desc_t gtpv2c_desc_trace_information_0;
extern tlv_desc_t gtpv2c_desc_bearer_flags_0;
extern tlv_desc_t gtpv2c_desc_pdn_type_0;
extern tlv_desc_t gtpv2c_desc_procedure_transaction_id_0;
extern tlv_desc_t gtpv2c_desc_mm_context_0;
extern tlv_desc_t gtpv2c_desc_pdu_numbers_0;
extern tlv_desc_t gtpv2c_desc_p_tmsi_0;
extern tlv_desc_t gtpv2c_desc_p_tmsi_signature_0;
extern tlv_desc_t gtpv2c_desc_hop_counter_0;
extern tlv_desc_t gtpv2c_desc_ue_time_zone_0;
extern tlv_desc_t gtpv2c_desc_trace_reference_0;
extern tlv_desc_t gtpv2c_desc_complete_request_message_0;
extern tlv_desc_t gtpv2c_desc_guti_0;
extern tlv_desc_t gtpv2c_desc_f_container_0;
extern tlv_desc_t gtpv2c_desc_f_cause_0;
extern tlv_desc_t gtpv2c_desc_plmn_id_0;
extern tlv_desc_t gtpv2c_desc_target_identification_0;
extern tlv_desc_t gtpv2c_desc_packet_flow_id_0;
extern tlv_desc_t gtpv2c_desc_rab_context_0;
extern tlv_desc_t gtpv2c_desc_source_rnc_pdcp_context_info_0;
extern tlv_desc_t gtpv2c_desc_port_number_0;
extern tlv_desc_t gtpv2c_desc_port_number_1;
extern tlv_desc_t gtpv2c_desc_port_number_2;
extern tlv_desc_t gtpv2c_desc_apn_restriction_0;
extern tlv_desc_t gtpv2c_desc_selection_mode_0;
extern tlv_desc_t gtpv2c_desc_source_identification_0;
extern tlv_desc_t gtpv2c_desc_change_reporting_action_0;
extern tlv_desc_t gtpv2c_desc_fq_csid_0;
extern tlv_desc_t gtpv2c_desc_fq_csid_1;
extern tlv_desc_t gtpv2c_desc_fq_csid_2;
extern tlv_desc_t gtpv2c_desc_fq_csid_3;
extern tlv_desc_t gtpv2c_desc_channel_needed_0;
extern tlv_desc_t gtpv2c_desc_emlpp_priority_0;
extern tlv_desc_t gtpv2c_desc_node_type_0;
extern tlv_desc_t gtpv2c_desc_fqdn_0;
extern tlv_desc_t gtpv2c_desc_ti_0;
extern tlv_desc_t gtpv2c_desc_mbms_session_duration_0;
extern tlv_desc_t gtpv2c_desc_mbms_service_area_0;
extern tlv_desc_t gtpv2c_desc_mbms_session_identifier_0;
extern tlv_desc_t gtpv2c_desc_mbms_flow_identifier_0;
extern tlv_desc_t gtpv2c_desc_mbms_ip_multicast_distribution_0;
extern tlv_desc_t gtpv2c_desc_mbms_distribution_acknowledge_0;
extern tlv_desc_t gtpv2c_desc_rfsp_index_0;
extern tlv_desc_t gtpv2c_desc_uci_0;
extern tlv_desc_t gtpv2c_desc_csg_information_reporting_action_0;
extern tlv_desc_t gtpv2c_desc_csg_id_0;
extern tlv_desc_t gtpv2c_desc_cmi_0;
extern tlv_desc_t gtpv2c_desc_service_indicator_0;
extern tlv_desc_t gtpv2c_desc_detach_type_0;
extern tlv_desc_t gtpv2c_desc_ldn_0;
extern tlv_desc_t gtpv2c_desc_ldn_1;
extern tlv_desc_t gtpv2c_desc_ldn_2;
extern tlv_desc_t gtpv2c_desc_ldn_3;
extern tlv_desc_t gtpv2c_desc_node_features_0;
extern tlv_desc_t gtpv2c_desc_mbms_time_to_data_transfer_0;
extern tlv_desc_t gtpv2c_desc_throttling_0;
extern tlv_desc_t gtpv2c_desc_arp_0;
extern tlv_desc_t gtpv2c_desc_epc_timer_0;
extern tlv_desc_t gtpv2c_desc_signalling_priority_indication_0;
extern tlv_desc_t gtpv2c_desc_tmgi_0;
extern tlv_desc_t gtpv2c_desc_additional_mm_context_for_srvcc_0;
extern tlv_desc_t gtpv2c_desc_additional_flags_for_srvcc_0;
extern tlv_desc_t gtpv2c_desc_mdt_configuration_0;
extern tlv_desc_t gtpv2c_desc_apco_0;
extern tlv_desc_t gtpv2c_desc_absolute_time_of_mbms_data_transfer_0;
extern tlv_desc_t gtpv2c_desc_enb_information_reporting_0;
extern tlv_desc_t gtpv2c_desc_ip4cp_0;
extern tlv_desc_t gtpv2c_desc_change_to_report_flags_0;
extern tlv_desc_t gtpv2c_desc_action_indication_0;
extern tlv_desc_t gtpv2c_desc_twan_identifier_0;
extern tlv_desc_t gtpv2c_desc_twan_identifier_1;
extern tlv_desc_t gtpv2c_desc_uli_timestamp_0;
extern tlv_desc_t gtpv2c_desc_mbms_flags_0;
extern tlv_desc_t gtpv2c_desc_ran_nas_cause_0;
extern tlv_desc_t gtpv2c_desc_cn_operator_selection_entity_0;
extern tlv_desc_t gtpv2c_desc_twmi_0;
extern tlv_desc_t gtpv2c_desc_node_number_0;
extern tlv_desc_t gtpv2c_desc_node_identifier_0;
extern tlv_desc_t gtpv2c_desc_presence_reporting_area_action_0;
extern tlv_desc_t gtpv2c_desc_presence_reporting_area_information_0;
extern tlv_desc_t gtpv2c_desc_twan_identifier_timestamp_0;
extern tlv_desc_t gtpv2c_desc_metric_0;
extern tlv_desc_t gtpv2c_desc_sequence_number_0;
extern tlv_desc_t gtpv2c_desc_apn_and_relative_capacity_0;
extern tlv_desc_t gtpv2c_desc_wlan_offloadability_indication_0;
extern tlv_desc_t gtpv2c_desc_paging_and_service_information_0;
extern tlv_desc_t gtpv2c_desc_integer_number_0;
extern tlv_desc_t gtpv2c_desc_millisecond_time_stamp_0;
extern tlv_desc_t gtpv2c_desc_monitoring_event_information_0;
extern tlv_desc_t gtpv2c_desc_ecgi_list_0;
extern tlv_desc_t gtpv2c_desc_remote_user_id_0;
extern tlv_desc_t gtpv2c_desc_remote_ue_ip_information_0;
extern tlv_desc_t gtpv2c_desc_ciot_optimizations_support_indication_0;
extern tlv_desc_t gtpv2c_desc_header_compression_configuration_0;
extern tlv_desc_t gtpv2c_desc_epco_0;
extern tlv_desc_t gtpv2c_desc_serving_plmn_rate_control_0;
extern tlv_desc_t gtpv2c_desc_counter_0;

/* Group Infomration Element TLV Descriptor */
extern tlv_desc_t gtpv2c_desc_bearer_context_0;
extern tlv_desc_t gtpv2c_desc_bearer_context_1;
extern tlv_desc_t gtpv2c_desc_pdn_connection_0;
extern tlv_desc_t gtpv2c_desc_overload_control_information_0;
extern tlv_desc_t gtpv2c_desc_overload_control_information_1;
extern tlv_desc_t gtpv2c_desc_overload_control_information_2;
extern tlv_desc_t gtpv2c_desc_load_control_information_0;
extern tlv_desc_t gtpv2c_desc_remote_ue_context_0;
extern tlv_desc_t gtpv2c_desc_scef_pdn_connection_0;

/* Message Descriptor */
extern tlv_desc_t gtpv2c_desc_echo_request;
extern tlv_desc_t gtpv2c_desc_echo_response;
extern tlv_desc_t gtpv2c_desc_version_not_supported_indication;
extern tlv_desc_t gtpv2c_desc_create_session_request;
extern tlv_desc_t gtpv2c_desc_create_session_response;
extern tlv_desc_t gtpv2c_desc_modify_bearer_request;
extern tlv_desc_t gtpv2c_desc_modify_bearer_response;
extern tlv_desc_t gtpv2c_desc_delete_session_request;
extern tlv_desc_t gtpv2c_desc_delete_session_response;
extern tlv_desc_t gtpv2c_desc_change_notification_request;
extern tlv_desc_t gtpv2c_desc_change_notification_response;
extern tlv_desc_t gtpv2c_desc_remote_ue_report_notification;
extern tlv_desc_t gtpv2c_desc_remote_ue_report_acknowledge;
extern tlv_desc_t gtpv2c_desc_modify_bearer_command;
extern tlv_desc_t gtpv2c_desc_modify_bearer_failure_indication;
extern tlv_desc_t gtpv2c_desc_delete_bearer_command;
extern tlv_desc_t gtpv2c_desc_delete_bearer_failure_indication;
extern tlv_desc_t gtpv2c_desc_bearer_resource_command;
extern tlv_desc_t gtpv2c_desc_bearer_resource_failure_indication;
extern tlv_desc_t gtpv2c_desc_downlink_data_notification_failure_indication;
extern tlv_desc_t gtpv2c_desc_trace_session_activation;
extern tlv_desc_t gtpv2c_desc_trace_session_deactivation;
extern tlv_desc_t gtpv2c_desc_stop_paging_indication;
extern tlv_desc_t gtpv2c_desc_create_bearer_request;
extern tlv_desc_t gtpv2c_desc_create_bearer_response;
extern tlv_desc_t gtpv2c_desc_update_bearer_request;
extern tlv_desc_t gtpv2c_desc_update_bearer_response;
extern tlv_desc_t gtpv2c_desc_delete_bearer_request;
extern tlv_desc_t gtpv2c_desc_delete_bearer_response;
extern tlv_desc_t gtpv2c_desc_delete_pdn_connection_set_request;
extern tlv_desc_t gtpv2c_desc_delete_pdn_connection_set_response;
extern tlv_desc_t gtpv2c_desc_pgw_downlink_triggering_notification;
extern tlv_desc_t gtpv2c_desc_pgw_downlink_triggering_acknowledge;
extern tlv_desc_t gtpv2c_desc_create_forwarding_tunnel_request;
extern tlv_desc_t gtpv2c_desc_create_forwarding_tunnel_response;
extern tlv_desc_t gtpv2c_desc_suspend_notification;
extern tlv_desc_t gtpv2c_desc_suspend_acknowledge;
extern tlv_desc_t gtpv2c_desc_resume_notification;
extern tlv_desc_t gtpv2c_desc_resume_acknowledge;
extern tlv_desc_t gtpv2c_desc_create_indirect_data_forwarding_tunnel_request;
extern tlv_desc_t gtpv2c_desc_create_indirect_data_forwarding_tunnel_response;
extern tlv_desc_t gtpv2c_desc_delete_indirect_data_forwarding_tunnel_request;
extern tlv_desc_t gtpv2c_desc_delete_indirect_data_forwarding_tunnel_response;
extern tlv_desc_t gtpv2c_desc_release_access_bearers_request;
extern tlv_desc_t gtpv2c_desc_release_access_bearers_response;
extern tlv_desc_t gtpv2c_desc_downlink_data_notification;
extern tlv_desc_t gtpv2c_desc_downlink_data_notification_acknowledge;
extern tlv_desc_t gtpv2c_desc_pgw_restart_notification;
extern tlv_desc_t gtpv2c_desc_pgw_restart_notification_acknowledge;
extern tlv_desc_t gtpv2c_desc_update_pdn_connection_set_request;
extern tlv_desc_t gtpv2c_desc_update_pdn_connection_set_response;
extern tlv_desc_t gtpv2c_desc_modify_access_bearers_request;
extern tlv_desc_t gtpv2c_desc_modify_access_bearers_response;

/* Structure for Infomration Element */
typedef tlv_octet_t gtpv2c_imsi_t;
typedef tlv_octet_t gtpv2c_cause_t;
typedef tlv_octet_t gtpv2c_recovery_t;
typedef tlv_octet_t gtpv2c_stn_sr_t;
typedef tlv_octet_t gtpv2c_apn_t;
typedef tlv_octet_t gtpv2c_ambr_t;
typedef tlv_octet_t gtpv2c_ebi_t;
typedef tlv_octet_t gtpv2c_ip_address_t;
typedef tlv_octet_t gtpv2c_mei_t;
typedef tlv_octet_t gtpv2c_msisdn_t;
typedef tlv_octet_t gtpv2c_indication_t;
typedef tlv_octet_t gtpv2c_pco_t;
typedef tlv_octet_t gtpv2c_paa_t;
typedef tlv_octet_t gtpv2c_bearer_qos_t;
typedef tlv_octet_t gtpv2c_flow_qos_t;
typedef tlv_octet_t gtpv2c_rat_type_t;
typedef tlv_octet_t gtpv2c_serving_network_t;
typedef tlv_octet_t gtpv2c_bearer_tft_t;
typedef tlv_octet_t gtpv2c_tad_t;
typedef tlv_octet_t gtpv2c_uli_t;
typedef tlv_octet_t gtpv2c_f_teid_t;
typedef tlv_octet_t gtpv2c_tmsi_t;
typedef tlv_octet_t gtpv2c_global_cn_id_t;
typedef tlv_octet_t gtpv2c_s103pdf_t;
typedef tlv_octet_t gtpv2c_s1udf_t;
typedef tlv_octet_t gtpv2c_delay_value_t;
typedef tlv_octet_t gtpv2c_charging_id_t;
typedef tlv_octet_t gtpv2c_charging_characteristics_t;
typedef tlv_octet_t gtpv2c_trace_information_t;
typedef tlv_octet_t gtpv2c_bearer_flags_t;
typedef tlv_octet_t gtpv2c_pdn_type_t;
typedef tlv_octet_t gtpv2c_procedure_transaction_id_t;
typedef tlv_octet_t gtpv2c_mm_context_t;
typedef tlv_octet_t gtpv2c_pdu_numbers_t;
typedef tlv_octet_t gtpv2c_p_tmsi_t;
typedef tlv_octet_t gtpv2c_p_tmsi_signature_t;
typedef tlv_octet_t gtpv2c_hop_counter_t;
typedef tlv_octet_t gtpv2c_ue_time_zone_t;
typedef tlv_octet_t gtpv2c_trace_reference_t;
typedef tlv_octet_t gtpv2c_complete_request_message_t;
typedef tlv_octet_t gtpv2c_guti_t;
typedef tlv_octet_t gtpv2c_f_container_t;
typedef tlv_octet_t gtpv2c_f_cause_t;
typedef tlv_octet_t gtpv2c_plmn_id_t;
typedef tlv_octet_t gtpv2c_target_identification_t;
typedef tlv_octet_t gtpv2c_packet_flow_id_t;
typedef tlv_octet_t gtpv2c_rab_context_t;
typedef tlv_octet_t gtpv2c_source_rnc_pdcp_context_info_t;
typedef tlv_octet_t gtpv2c_port_number_t;
typedef tlv_octet_t gtpv2c_apn_restriction_t;
typedef tlv_octet_t gtpv2c_selection_mode_t;
typedef tlv_octet_t gtpv2c_source_identification_t;
typedef tlv_octet_t gtpv2c_change_reporting_action_t;
typedef tlv_octet_t gtpv2c_fq_csid_t;
typedef tlv_octet_t gtpv2c_channel_needed_t;
typedef tlv_octet_t gtpv2c_emlpp_priority_t;
typedef tlv_octet_t gtpv2c_node_type_t;
typedef tlv_octet_t gtpv2c_fqdn_t;
typedef tlv_octet_t gtpv2c_ti_t;
typedef tlv_octet_t gtpv2c_mbms_session_duration_t;
typedef tlv_octet_t gtpv2c_mbms_service_area_t;
typedef tlv_octet_t gtpv2c_mbms_session_identifier_t;
typedef tlv_octet_t gtpv2c_mbms_flow_identifier_t;
typedef tlv_octet_t gtpv2c_mbms_ip_multicast_distribution_t;
typedef tlv_octet_t gtpv2c_mbms_distribution_acknowledge_t;
typedef tlv_octet_t gtpv2c_rfsp_index_t;
typedef tlv_octet_t gtpv2c_uci_t;
typedef tlv_octet_t gtpv2c_csg_information_reporting_action_t;
typedef tlv_octet_t gtpv2c_csg_id_t;
typedef tlv_octet_t gtpv2c_cmi_t;
typedef tlv_octet_t gtpv2c_service_indicator_t;
typedef tlv_octet_t gtpv2c_detach_type_t;
typedef tlv_octet_t gtpv2c_ldn_t;
typedef tlv_octet_t gtpv2c_node_features_t;
typedef tlv_octet_t gtpv2c_mbms_time_to_data_transfer_t;
typedef tlv_octet_t gtpv2c_throttling_t;
typedef tlv_octet_t gtpv2c_arp_t;
typedef tlv_octet_t gtpv2c_epc_timer_t;
typedef tlv_octet_t gtpv2c_signalling_priority_indication_t;
typedef tlv_octet_t gtpv2c_tmgi_t;
typedef tlv_octet_t gtpv2c_additional_mm_context_for_srvcc_t;
typedef tlv_octet_t gtpv2c_additional_flags_for_srvcc_t;
typedef tlv_octet_t gtpv2c_mdt_configuration_t;
typedef tlv_octet_t gtpv2c_apco_t;
typedef tlv_octet_t gtpv2c_absolute_time_of_mbms_data_transfer_t;
typedef tlv_octet_t gtpv2c_enb_information_reporting_t;
typedef tlv_octet_t gtpv2c_ip4cp_t;
typedef tlv_octet_t gtpv2c_change_to_report_flags_t;
typedef tlv_octet_t gtpv2c_action_indication_t;
typedef tlv_octet_t gtpv2c_twan_identifier_t;
typedef tlv_octet_t gtpv2c_uli_timestamp_t;
typedef tlv_octet_t gtpv2c_mbms_flags_t;
typedef tlv_octet_t gtpv2c_ran_nas_cause_t;
typedef tlv_octet_t gtpv2c_cn_operator_selection_entity_t;
typedef tlv_octet_t gtpv2c_twmi_t;
typedef tlv_octet_t gtpv2c_node_number_t;
typedef tlv_octet_t gtpv2c_node_identifier_t;
typedef tlv_octet_t gtpv2c_presence_reporting_area_action_t;
typedef tlv_octet_t gtpv2c_presence_reporting_area_information_t;
typedef tlv_octet_t gtpv2c_twan_identifier_timestamp_t;
typedef tlv_octet_t gtpv2c_metric_t;
typedef tlv_octet_t gtpv2c_sequence_number_t;
typedef tlv_octet_t gtpv2c_apn_and_relative_capacity_t;
typedef tlv_octet_t gtpv2c_wlan_offloadability_indication_t;
typedef tlv_octet_t gtpv2c_paging_and_service_information_t;
typedef tlv_octet_t gtpv2c_integer_number_t;
typedef tlv_octet_t gtpv2c_millisecond_time_stamp_t;
typedef tlv_octet_t gtpv2c_monitoring_event_information_t;
typedef tlv_octet_t gtpv2c_ecgi_list_t;
typedef tlv_octet_t gtpv2c_remote_user_id_t;
typedef tlv_octet_t gtpv2c_remote_ue_ip_information_t;
typedef tlv_octet_t gtpv2c_ciot_optimizations_support_indication_t;
typedef tlv_octet_t gtpv2c_header_compression_configuration_t;
typedef tlv_octet_t gtpv2c_epco_t;
typedef tlv_octet_t gtpv2c_serving_plmn_rate_control_t;
typedef tlv_octet_t gtpv2c_counter_t;

/* Structure for Group Infomration Element */
typedef struct _gtpv2c_bearer_context_t {
    tlv_header_t h;
    gtpv2c_ebi_t eps_bearer_id;
    gtpv2c_bearer_tft_t tft;
    gtpv2c_f_teid_t s1_u_enodeb_f_teid; /* Instance : 0 */
    gtpv2c_f_teid_t s4_u_sgsn_f_teid; /* Instance : 1 */
    gtpv2c_f_teid_t s5_s8_u_sgw_f_teid; /* Instance : 2 */
    gtpv2c_f_teid_t s5_s8_u_pgw_f_teid; /* Instance : 3 */
    gtpv2c_f_teid_t s12_rnc_f_teid; /* Instance : 4 */
    gtpv2c_f_teid_t s2b_u_epdg_f_teid_5; /* Instance : 5 */
    gtpv2c_f_teid_t s2a_u_twan_f_teid_6; /* Instance : 6 */
    gtpv2c_bearer_qos_t bearer_level_qos;
    gtpv2c_f_teid_t s11_u_mme_f_teid; /* Instance : 7 */
    gtpv2c_cause_t cause;
    gtpv2c_charging_id_t charging_id;
    gtpv2c_bearer_flags_t bearer_flags;
    gtpv2c_pco_t protocol_configuration_options;
    gtpv2c_epco_t extended_protocol_configuration_options;
    gtpv2c_f_teid_t s2b_u_epdg_f_teid_8; /* Instance : 8 */
    gtpv2c_f_teid_t s2b_u_pgw_f_teid; /* Instance : 9 */
    gtpv2c_f_teid_t s2a_u_twan_f_teid_10; /* Instance : 10 */
    gtpv2c_f_teid_t s2a_u_pgw_f_teid; /* Instance : 11 */
    gtpv2c_ran_nas_cause_t ran_nas_cause;
    gtpv2c_apco_t additional_protocol_configuration_options;
    gtpv2c_f_container_t bss_container;
    gtpv2c_ti_t transaction_identifier;
    gtpv2c_packet_flow_id_t packet_flow_id;
} gtpv2c_bearer_context_t;

typedef struct _gtpv2c_pdn_connection_t {
    tlv_header_t h;
} gtpv2c_pdn_connection_t;

typedef struct _gtpv2c_overload_control_information_t {
    tlv_header_t h;
    gtpv2c_sequence_number_t overload_control_sequence_number;
    gtpv2c_metric_t overload_reduction_metric;
    gtpv2c_epc_timer_t period_of_validity;
    gtpv2c_apn_t list_of_access_point_name;
} gtpv2c_overload_control_information_t;

typedef struct _gtpv2c_load_control_information_t {
    tlv_header_t h;
    gtpv2c_sequence_number_t load_control_sequence_number;
    gtpv2c_metric_t load_metric;
    gtpv2c_apn_and_relative_capacity_t list_of_apn_and_relative_capacity;
} gtpv2c_load_control_information_t;

typedef struct _gtpv2c_remote_ue_context_t {
    tlv_header_t h;
    gtpv2c_remote_user_id_t remote_user_id;
    gtpv2c_remote_ue_ip_information_t remote_ue_ip_information;
} gtpv2c_remote_ue_context_t;

typedef struct _gtpv2c_scef_pdn_connection_t {
    tlv_header_t h;
    gtpv2c_apn_t apn;
    gtpv2c_ebi_t default_eps_bearer_id;
    gtpv2c_node_identifier_t scef_id;
} gtpv2c_scef_pdn_connection_t;

/* Structure for Message */
typedef struct _gtpv2c_echo_request_t {
    tlv_header_t h;
    gtpv2c_recovery_t recovery;
    gtpv2c_node_features_t sending_node_features;
} gtpv2c_echo_request_t;

typedef struct _gtpv2c_echo_response_t {
    tlv_header_t h;
    gtpv2c_recovery_t recovery;
    gtpv2c_node_features_t sending_node_features;
} gtpv2c_echo_response_t;

typedef struct _gtpv2c_version_not_supported_indication_t {
    tlv_header_t h;
} gtpv2c_version_not_supported_indication_t;

typedef struct _gtpv2c_create_session_request_t {
    tlv_header_t h;
    gtpv2c_imsi_t imsi;
    gtpv2c_msisdn_t msisdn;
    gtpv2c_mei_t me_identity;
    gtpv2c_uli_t user_location_information;
    gtpv2c_serving_network_t serving_network;
    gtpv2c_rat_type_t rat_type;
    gtpv2c_indication_t indication_flags;
    gtpv2c_f_teid_t sender_f_teid_for_control_plane;
    gtpv2c_f_teid_t pgw_s5_s8_address_for_control_plane_or_pmip;
    gtpv2c_apn_t access_point_name;
    gtpv2c_selection_mode_t selection_mode;
    gtpv2c_pdn_type_t pdn_type;
    gtpv2c_paa_t pdn_address_allocation;
    gtpv2c_apn_restriction_t maximum_apn_restriction;
    gtpv2c_ambr_t aggregate_maximum_bit_rate;
    gtpv2c_ebi_t linked_eps_bearer_id;
    gtpv2c_twmi_t trusted_wlan_mode_indication;
    gtpv2c_pco_t protocol_configuration_options;
    gtpv2c_bearer_context_t bearer_contexts_to_be_created;
    gtpv2c_bearer_context_t bearer_contexts_to_be_removed;
    gtpv2c_trace_information_t trace_information;
    gtpv2c_recovery_t recovery;
    gtpv2c_fq_csid_t mme_fq_csid;
    gtpv2c_fq_csid_t sgw_fq_csid;
    gtpv2c_fq_csid_t epdg_fq_csid;
    gtpv2c_fq_csid_t twan_fq_csid;
    gtpv2c_ue_time_zone_t ue_time_zone;
    gtpv2c_uci_t user_csg_information;
    gtpv2c_charging_characteristics_t charging_characteristics;
    gtpv2c_ldn_t mme_s4_sgsn_ldn;
    gtpv2c_ldn_t sgw_ldn;
    gtpv2c_ldn_t epdg_ldn;
    gtpv2c_ldn_t twan_ldn;
    gtpv2c_signalling_priority_indication_t signalling_priority_indication;
    gtpv2c_ip_address_t ue_local_ip_address;
    gtpv2c_port_number_t ue_udp_port;
    gtpv2c_apco_t additional_protocol_configuration_options;
    gtpv2c_ip_address_t hnb_local_ip_address;
    gtpv2c_port_number_t hnb_udp_port;
    gtpv2c_ip_address_t mme_s4_sgsn_identifier;
    gtpv2c_twan_identifier_t twan_identifier;
    gtpv2c_ip_address_t epdg_ip_address;
    gtpv2c_cn_operator_selection_entity_t cn_operator_selection_entity;
    gtpv2c_presence_reporting_area_information_t presence_reporting_area_information;
    gtpv2c_overload_control_information_t mme_s4_sgsn_s_overload_control_information;
    gtpv2c_overload_control_information_t sgw_s_overload_control_information;
    gtpv2c_overload_control_information_t twan_epdg_s_overload_control_information;
    gtpv2c_millisecond_time_stamp_t origination_time_stamp;
    gtpv2c_integer_number_t maximum_wait_time;
    gtpv2c_twan_identifier_t wlan_location_information;
    gtpv2c_twan_identifier_timestamp_t wlan_location_timestamp;
    gtpv2c_f_container_t nbifom_container;
    gtpv2c_remote_ue_context_t remote_ue_context_connected;
    gtpv2c_node_identifier_t _aaa_server_identifier;
    gtpv2c_epco_t extended_protocol_configuration_options;
    gtpv2c_serving_plmn_rate_control_t serving_plmn_rate_control;
    gtpv2c_counter_t mo_exception_data_counter;
    gtpv2c_port_number_t ue_tcp_port;
} gtpv2c_create_session_request_t;

typedef struct _gtpv2c_create_session_response_t {
    tlv_header_t h;
} gtpv2c_create_session_response_t;

typedef struct _gtpv2c_modify_bearer_request_t {
    tlv_header_t h;
} gtpv2c_modify_bearer_request_t;

typedef struct _gtpv2c_modify_bearer_response_t {
    tlv_header_t h;
} gtpv2c_modify_bearer_response_t;

typedef struct _gtpv2c_delete_session_request_t {
    tlv_header_t h;
} gtpv2c_delete_session_request_t;

typedef struct _gtpv2c_delete_session_response_t {
    tlv_header_t h;
} gtpv2c_delete_session_response_t;

typedef struct _gtpv2c_change_notification_request_t {
    tlv_header_t h;
} gtpv2c_change_notification_request_t;

typedef struct _gtpv2c_change_notification_response_t {
    tlv_header_t h;
} gtpv2c_change_notification_response_t;

typedef struct _gtpv2c_remote_ue_report_notification_t {
    tlv_header_t h;
} gtpv2c_remote_ue_report_notification_t;

typedef struct _gtpv2c_remote_ue_report_acknowledge_t {
    tlv_header_t h;
} gtpv2c_remote_ue_report_acknowledge_t;

typedef struct _gtpv2c_modify_bearer_command_t {
    tlv_header_t h;
} gtpv2c_modify_bearer_command_t;

typedef struct _gtpv2c_modify_bearer_failure_indication_t {
    tlv_header_t h;
} gtpv2c_modify_bearer_failure_indication_t;

typedef struct _gtpv2c_delete_bearer_command_t {
    tlv_header_t h;
} gtpv2c_delete_bearer_command_t;

typedef struct _gtpv2c_delete_bearer_failure_indication_t {
    tlv_header_t h;
} gtpv2c_delete_bearer_failure_indication_t;

typedef struct _gtpv2c_bearer_resource_command_t {
    tlv_header_t h;
} gtpv2c_bearer_resource_command_t;

typedef struct _gtpv2c_bearer_resource_failure_indication_t {
    tlv_header_t h;
} gtpv2c_bearer_resource_failure_indication_t;

typedef struct _gtpv2c_downlink_data_notification_failure_indication_t {
    tlv_header_t h;
} gtpv2c_downlink_data_notification_failure_indication_t;

typedef struct _gtpv2c_trace_session_activation_t {
    tlv_header_t h;
} gtpv2c_trace_session_activation_t;

typedef struct _gtpv2c_trace_session_deactivation_t {
    tlv_header_t h;
} gtpv2c_trace_session_deactivation_t;

typedef struct _gtpv2c_stop_paging_indication_t {
    tlv_header_t h;
} gtpv2c_stop_paging_indication_t;

typedef struct _gtpv2c_create_bearer_request_t {
    tlv_header_t h;
} gtpv2c_create_bearer_request_t;

typedef struct _gtpv2c_create_bearer_response_t {
    tlv_header_t h;
} gtpv2c_create_bearer_response_t;

typedef struct _gtpv2c_update_bearer_request_t {
    tlv_header_t h;
} gtpv2c_update_bearer_request_t;

typedef struct _gtpv2c_update_bearer_response_t {
    tlv_header_t h;
} gtpv2c_update_bearer_response_t;

typedef struct _gtpv2c_delete_bearer_request_t {
    tlv_header_t h;
} gtpv2c_delete_bearer_request_t;

typedef struct _gtpv2c_delete_bearer_response_t {
    tlv_header_t h;
} gtpv2c_delete_bearer_response_t;

typedef struct _gtpv2c_delete_pdn_connection_set_request_t {
    tlv_header_t h;
} gtpv2c_delete_pdn_connection_set_request_t;

typedef struct _gtpv2c_delete_pdn_connection_set_response_t {
    tlv_header_t h;
} gtpv2c_delete_pdn_connection_set_response_t;

typedef struct _gtpv2c_pgw_downlink_triggering_notification_t {
    tlv_header_t h;
} gtpv2c_pgw_downlink_triggering_notification_t;

typedef struct _gtpv2c_pgw_downlink_triggering_acknowledge_t {
    tlv_header_t h;
} gtpv2c_pgw_downlink_triggering_acknowledge_t;

typedef struct _gtpv2c_create_forwarding_tunnel_request_t {
    tlv_header_t h;
} gtpv2c_create_forwarding_tunnel_request_t;

typedef struct _gtpv2c_create_forwarding_tunnel_response_t {
    tlv_header_t h;
} gtpv2c_create_forwarding_tunnel_response_t;

typedef struct _gtpv2c_suspend_notification_t {
    tlv_header_t h;
} gtpv2c_suspend_notification_t;

typedef struct _gtpv2c_suspend_acknowledge_t {
    tlv_header_t h;
} gtpv2c_suspend_acknowledge_t;

typedef struct _gtpv2c_resume_notification_t {
    tlv_header_t h;
} gtpv2c_resume_notification_t;

typedef struct _gtpv2c_resume_acknowledge_t {
    tlv_header_t h;
} gtpv2c_resume_acknowledge_t;

typedef struct _gtpv2c_create_indirect_data_forwarding_tunnel_request_t {
    tlv_header_t h;
} gtpv2c_create_indirect_data_forwarding_tunnel_request_t;

typedef struct _gtpv2c_create_indirect_data_forwarding_tunnel_response_t {
    tlv_header_t h;
} gtpv2c_create_indirect_data_forwarding_tunnel_response_t;

typedef struct _gtpv2c_delete_indirect_data_forwarding_tunnel_request_t {
    tlv_header_t h;
} gtpv2c_delete_indirect_data_forwarding_tunnel_request_t;

typedef struct _gtpv2c_delete_indirect_data_forwarding_tunnel_response_t {
    tlv_header_t h;
} gtpv2c_delete_indirect_data_forwarding_tunnel_response_t;

typedef struct _gtpv2c_release_access_bearers_request_t {
    tlv_header_t h;
} gtpv2c_release_access_bearers_request_t;

typedef struct _gtpv2c_release_access_bearers_response_t {
    tlv_header_t h;
} gtpv2c_release_access_bearers_response_t;

typedef struct _gtpv2c_downlink_data_notification_t {
    tlv_header_t h;
} gtpv2c_downlink_data_notification_t;

typedef struct _gtpv2c_downlink_data_notification_acknowledge_t {
    tlv_header_t h;
} gtpv2c_downlink_data_notification_acknowledge_t;

typedef struct _gtpv2c_pgw_restart_notification_t {
    tlv_header_t h;
} gtpv2c_pgw_restart_notification_t;

typedef struct _gtpv2c_pgw_restart_notification_acknowledge_t {
    tlv_header_t h;
} gtpv2c_pgw_restart_notification_acknowledge_t;

typedef struct _gtpv2c_update_pdn_connection_set_request_t {
    tlv_header_t h;
} gtpv2c_update_pdn_connection_set_request_t;

typedef struct _gtpv2c_update_pdn_connection_set_response_t {
    tlv_header_t h;
} gtpv2c_update_pdn_connection_set_response_t;

typedef struct _gtpv2c_modify_access_bearers_request_t {
    tlv_header_t h;
} gtpv2c_modify_access_bearers_request_t;

typedef struct _gtpv2c_modify_access_bearers_response_t {
    tlv_header_t h;
} gtpv2c_modify_access_bearers_response_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTPV2C_TLV_H__ */
