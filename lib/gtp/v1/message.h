/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

/*******************************************************************************
 * This file had been created by gtp1-tlv.py script v0.1.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2023-12-11 17:21:37.064347 by pespin
 * from 29060-h40.docx
 ******************************************************************************/

#if !defined(OGS_GTP_INSIDE) && !defined(OGS_GTP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_GTP1_MESSAGE_H
#define OGS_GTP1_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/* 5.1 General format */
#define OGS_GTPV1U_HEADER_LEN   8
#define OGS_GTPV1C_HEADER_LEN   12
#define OGS_GTP1_TEID_LEN        4
typedef struct ogs_gtp1_header_s {
    union {
        struct {
#define OGS_GTP1_VERSION_0 0
#define OGS_GTP1_VERSION_1 1
        ED6(uint8_t version:3;,
            uint8_t pt:1;,
            uint8_t spare1:1;,
            uint8_t e:1;,
            uint8_t s:1;,
            uint8_t pn:1;)
        };
/* GTU-U flags */
#define OGS_GTP1U_FLAGS_V                        0x20
#define OGS_GTP1U_FLAGS_PT                       0x10
#define OGS_GTP1U_FLAGS_E                        0x04
#define OGS_GTP1U_FLAGS_S                        0x02
#define OGS_GTP1U_FLAGS_PN                       0x01
        uint8_t flags;
    };
/* GTP-U message type, defined in 3GPP TS 29.281 Release 11 */
#define OGS_GTP1U_MSGTYPE_ECHO_REQ               1
#define OGS_GTP1U_MSGTYPE_ECHO_RSP               2
#define OGS_GTP1U_MSGTYPE_ERR_IND                26
#define OGS_GTP1U_MSGTYPE_SUPP_EXTHDR_NOTI       31
#define OGS_GTP1U_MSGTYPE_END_MARKER             254
#define OGS_GTP1U_MSGTYPE_GPDU                   255
    uint8_t type;
    uint16_t length;
    uint32_t teid;
    /* Optional based on flags: */
#define OGS_GTP1_XID_TO_SQN(__xid) htobe16((uint16_t)(__xid))
#define OGS_GTP1_SQN_TO_XID(__sqn) be16toh((uint16_t)__sqn)
    uint16_t sqn;
    uint8_t npdu;
    uint8_t ext_hdr_type;
} __attribute__ ((packed)) ogs_gtp1_header_t;

/* GTPv1-C message type */
#define OGS_GTP1_ECHO_REQUEST_TYPE 1
#define OGS_GTP1_ECHO_RESPONSE_TYPE 2
#define OGS_GTP1_VERSION_NOT_SUPPORTED_TYPE 3
#define OGS_GTP1_NODE_ALIVE_REQUEST_TYPE 4
#define OGS_GTP1_NODE_ALIVE_RESPONSE_TYPE 5
#define OGS_GTP1_REDIRECTION_REQUEST_TYPE 6
#define OGS_GTP1_REDIRECTION_RESPONSE_TYPE 7
#define OGS_GTP1_CREATE_PDP_CONTEXT_REQUEST_TYPE 16
#define OGS_GTP1_CREATE_PDP_CONTEXT_RESPONSE_TYPE 17
#define OGS_GTP1_UPDATE_PDP_CONTEXT_REQUEST_TYPE 18
#define OGS_GTP1_UPDATE_PDP_CONTEXT_RESPONSE_TYPE 19
#define OGS_GTP1_DELETE_PDP_CONTEXT_REQUEST_TYPE 20
#define OGS_GTP1_DELETE_PDP_CONTEXT_RESPONSE_TYPE 21
#define OGS_GTP1_INITIATE_PDP_CONTEXT_ACTIVATION_REQUEST_TYPE 22
#define OGS_GTP1_INITIATE_PDP_CONTEXT_ACTIVATION_RESPONSE_TYPE 23
#define OGS_GTP1_ERROR_INDICATION_TYPE 26
#define OGS_GTP1_PDU_NOTIFICATION_REQUEST_TYPE 27
#define OGS_GTP1_PDU_NOTIFICATION_RESPONSE_TYPE 28
#define OGS_GTP1_PDU_NOTIFICATION_REJECT_REQUEST_TYPE 29
#define OGS_GTP1_PDU_NOTIFICATION_REJECT_RESPONSE_TYPE 30
#define OGS_GTP1_SUPPORTED_EXTENSION_HEADERS_NOTIFICATION_TYPE 31
#define OGS_GTP1_SEND_ROUTEING_INFORMATION_FOR_GPRS_REQUEST_TYPE 32
#define OGS_GTP1_SEND_ROUTEING_INFORMATION_FOR_GPRS_RESPONSE_TYPE 33
#define OGS_GTP1_FAILURE_REPORT_REQUEST_TYPE 34
#define OGS_GTP1_FAILURE_REPORT_RESPONSE_TYPE 35
#define OGS_GTP1_NOTE_MS_GPRS_PRESENT_REQUEST_TYPE 36
#define OGS_GTP1_NOTE_MS_GPRS_PRESENT_RESPONSE_TYPE 37
#define OGS_GTP1_IDENTIFICATION_REQUEST_TYPE 48
#define OGS_GTP1_IDENTIFICATION_RESPONSE_TYPE 49
#define OGS_GTP1_SGSN_CONTEXT_REQUEST_TYPE 50
#define OGS_GTP1_SGSN_CONTEXT_RESPONSE_TYPE 51
#define OGS_GTP1_SGSN_CONTEXT_ACKNOWLEDGE_TYPE 52
#define OGS_GTP1_FORWARD_RELOCATION_REQUEST_TYPE 53
#define OGS_GTP1_FORWARD_RELOCATION_RESPONSE_TYPE 54
#define OGS_GTP1_FORWARD_RELOCATION_COMPLETE_TYPE 55
#define OGS_GTP1_RELOCATION_CANCEL_REQUEST_TYPE 56
#define OGS_GTP1_RELOCATION_CANCEL_RESPONSE_TYPE 57
#define OGS_GTP1_FORWARD_SRNS_CONTEXT_TYPE 58
#define OGS_GTP1_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE_TYPE 59
#define OGS_GTP1_FORWARD_SRNS_CONTEXT_ACKNOWLEDGE_TYPE 60
#define OGS_GTP1_UE_REGISTRATION_QUERY_REQUEST_TYPE 61
#define OGS_GTP1_UE_REGISTRATION_QUERY_RESPONSE_TYPE 62
#define OGS_GTP1_RAN_INFORMATION_RELAY_TYPE 70
#define OGS_GTP1_MBMS_NOTIFICATION_REQUEST_TYPE 96
#define OGS_GTP1_MBMS_NOTIFICATION_RESPONSE_TYPE 97
#define OGS_GTP1_MBMS_NOTIFICATION_REJECT_REQUEST_TYPE 98
#define OGS_GTP1_MBMS_NOTIFICATION_REJECT_RESPONSE_TYPE 99
#define OGS_GTP1_CREATE_MBMS_CONTEXT_REQUEST_TYPE 100
#define OGS_GTP1_CREATE_MBMS_CONTEXT_RESPONSE_TYPE 101
#define OGS_GTP1_UPDATE_MBMS_CONTEXT_REQUEST_TYPE 102
#define OGS_GTP1_UPDATE_MBMS_CONTEXT_RESPONSE_TYPE 103
#define OGS_GTP1_DELETE_MBMS_CONTEXT_REQUEST_TYPE 104
#define OGS_GTP1_DELETE_MBMS_CONTEXT_RESPONSE_TYPE 105
#define OGS_GTP1_MBMS_REGISTRATION_REQUEST_TYPE 112
#define OGS_GTP1_MBMS_REGISTRATION_RESPONSE_TYPE 113
#define OGS_GTP1_MBMS_DE_REGISTRATION_REQUEST_TYPE 114
#define OGS_GTP1_MBMS_DE_REGISTRATION_RESPONSE_TYPE 115
#define OGS_GTP1_MBMS_SESSION_START_REQUEST_TYPE 116
#define OGS_GTP1_MBMS_SESSION_START_RESPONSE_TYPE 117
#define OGS_GTP1_MBMS_SESSION_STOP_REQUEST_TYPE 118
#define OGS_GTP1_MBMS_SESSION_STOP_RESPONSE_TYPE 119
#define OGS_GTP1_MBMS_SESSION_UPDATE_REQUEST_TYPE 120
#define OGS_GTP1_MBMS_SESSION_UPDATE_RESPONSE_TYPE 121

#define OGS_GTP1_CAUSE_TYPE 1
#define OGS_GTP1_IMSI_TYPE 2
#define OGS_GTP1_RAI_TYPE 3
#define OGS_GTP1_TLLI_TYPE 4
#define OGS_GTP1_P_TMSI_TYPE 5
#define OGS_GTP1_REORDERING_REQUIRED_TYPE 8
#define OGS_GTP1_AUTHENTICATION_TRIPLET_TYPE 9
#define OGS_GTP1_MAP_CAUSE_TYPE 11
#define OGS_GTP1_P_TMSI_SIGNATURE_TYPE 12
#define OGS_GTP1_MS_VALIDATED_TYPE 13
#define OGS_GTP1_RECOVERY_TYPE 14
#define OGS_GTP1_SELECTION_MODE_TYPE 15
#define OGS_GTP1_TUNNEL_ENDPOINT_IDENTIFIER_DATA_I_TYPE 16
#define OGS_GTP1_TUNNEL_ENDPOINT_IDENTIFIER_CONTROL_PLANE_TYPE 17
#define OGS_GTP1_TUNNEL_ENDPOINT_IDENTIFIER_DATA_II_TYPE 18
#define OGS_GTP1_TEARDOWN_IND_TYPE 19
#define OGS_GTP1_NSAPI_TYPE 20
#define OGS_GTP1_RANAP_CAUSE_TYPE 21
#define OGS_GTP1_RAB_CONTEXT_TYPE 22
#define OGS_GTP1_RADIO_PRIORITY_SMS_TYPE 23
#define OGS_GTP1_RADIO_PRIORITY_TYPE 24
#define OGS_GTP1_PACKET_FLOW_ID_TYPE 25
#define OGS_GTP1_CHARGING_CHARACTERISTICS_TYPE 26
#define OGS_GTP1_TRACE_REFERENCE_TYPE 27
#define OGS_GTP1_TRACE_TYPE_TYPE 28
#define OGS_GTP1_MS_NOT_REACHABLE_REASON_TYPE 29
#define OGS_GTP1_CHARGING_ID_TYPE 127
#define OGS_GTP1_END_USER_ADDRESS_TYPE 128
#define OGS_GTP1_MM_CONTEXT_TYPE 129
#define OGS_GTP1_PDP_CONTEXT_TYPE 130
#define OGS_GTP1_ACCESS_POINT_NAME_TYPE 131
#define OGS_GTP1_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 132
#define OGS_GTP1_GSN_ADDRESS_TYPE 133
#define OGS_GTP1_MSISDN_TYPE 134
#define OGS_GTP1_QUALITY_OF_SERVICE_PROFILE_TYPE 135
#define OGS_GTP1_AUTHENTICATION_QUINTUPLET_TYPE 136
#define OGS_GTP1_TRAFFIC_FLOW_TEMPLATE_TYPE 137
#define OGS_GTP1_TARGET_IDENTIFICATION_TYPE 138
#define OGS_GTP1_UTRAN_TRANSPARENT_CONTAINER_TYPE 139
#define OGS_GTP1_RAB_SETUP_INFORMATION_TYPE 140
#define OGS_GTP1_EXTENSION_HEADER_TYPE_LIST_TYPE 141
#define OGS_GTP1_TRIGGER_ID_TYPE 142
#define OGS_GTP1_OMC_IDENTITY_TYPE 143
#define OGS_GTP1_RAN_TRANSPARENT_CONTAINER_TYPE 144
#define OGS_GTP1_PDP_CONTEXT_PRIORITIZATION_TYPE 145
#define OGS_GTP1_ADDITIONAL_RAB_SETUP_INFORMATION_TYPE 146
#define OGS_GTP1_SGSN_NUMBER_TYPE 147
#define OGS_GTP1_COMMON_FLAGS_TYPE 148
#define OGS_GTP1_APN_RESTRICTION_TYPE 149
#define OGS_GTP1_RADIO_PRIORITY_LCS_TYPE 150
#define OGS_GTP1_RAT_TYPE_TYPE 151
#define OGS_GTP1_USER_LOCATION_INFORMATION_TYPE 152
#define OGS_GTP1_MS_TIME_ZONE_TYPE 153
#define OGS_GTP1_SV_TYPE 154
#define OGS_GTP1_CAMEL_CHARGING_INFORMATION_CONTAINER_TYPE 155
#define OGS_GTP1_MBMS_UE_CONTEXT_TYPE 156
#define OGS_GTP1_TMGI_TYPE 157
#define OGS_GTP1_RIM_ROUTING_ADDRESS_TYPE 158
#define OGS_GTP1_MBMS_PROTOCOL_CONFIGURATION_OPTIONS_TYPE 159
#define OGS_GTP1_MBMS_SERVICE_AREA_TYPE 160
#define OGS_GTP1_SOURCE_RNC_PDCP_CONTEXT_INFO_TYPE 161
#define OGS_GTP1_ADDITIONAL_TRACE_INFO_TYPE 162
#define OGS_GTP1_HOP_COUNTER_TYPE 163
#define OGS_GTP1_SELECTED_PLMN_ID_TYPE 164
#define OGS_GTP1_MBMS_SESSION_IDENTIFIER_TYPE 165
#define OGS_GTP1_MBMS_2G_3G_INDICATOR_TYPE 166
#define OGS_GTP1_ENHANCED_NSAPI_TYPE 167
#define OGS_GTP1_MBMS_SESSION_DURATION_TYPE 168
#define OGS_GTP1_ADDITIONAL_MBMS_TRACE_INFO_TYPE 169
#define OGS_GTP1_MBMS_SESSION_REPETITION_NUMBER_TYPE 170
#define OGS_GTP1_MBMS_TIME_TO_DATA_TRANSFER_TYPE 171
#define OGS_GTP1_BSS_CONTAINER_TYPE 173
#define OGS_GTP1_CELL_IDENTIFICATION_TYPE 174
#define OGS_GTP1_PDU_NUMBERS_TYPE 175
#define OGS_GTP1_BSSGP_CAUSE_TYPE 176
#define OGS_GTP1_REQUIRED_MBMS_BEARER_CAPABILITIES_TYPE 177
#define OGS_GTP1_RIM_ROUTING_ADDRESS_DISCRIMINATOR_TYPE 178
#define OGS_GTP1_LIST_OF_SET_UP_PFCS_TYPE 179
#define OGS_GTP1_PS_HANDOVER_XID_PARAMETERS_TYPE 180
#define OGS_GTP1_MS_INFO_CHANGE_REPORTING_ACTION_TYPE 181
#define OGS_GTP1_DIRECT_TUNNEL_FLAGS_TYPE 182
#define OGS_GTP1_CORRELATION_ID_TYPE 183
#define OGS_GTP1_BEARER_CONTROL_MODE_TYPE 184
#define OGS_GTP1_MBMS_FLOW_IDENTIFIER_TYPE 185
#define OGS_GTP1_MBMS_IP_MULTICAST_DISTRIBUTION_TYPE 186
#define OGS_GTP1_MBMS_DISTRIBUTION_ACKNOWLEDGEMENT_TYPE 187
#define OGS_GTP1_RELIABLE_INTER_RAT_HANDOVER_INFO_TYPE 188
#define OGS_GTP1_RFSP_INDEX_TYPE 189
#define OGS_GTP1_FQDN_TYPE 190
#define OGS_GTP1_EVOLVED_ALLOCATION_RETENTION_PRIORITY_I_TYPE 191
#define OGS_GTP1_EVOLVED_ALLOCATION_RETENTION_PRIORITY_II_TYPE 192
#define OGS_GTP1_EXTENDED_COMMON_FLAGS_TYPE 193
#define OGS_GTP1_UCI_TYPE 194
#define OGS_GTP1_CSG_INFORMATION_REPORTING_ACTION_TYPE 195
#define OGS_GTP1_CSG_ID_TYPE 196
#define OGS_GTP1_CMI_TYPE 197
#define OGS_GTP1_AMBR_TYPE 198
#define OGS_GTP1_UE_NETWORK_CAPABILITY_TYPE 199
#define OGS_GTP1_UE_AMBR_TYPE 200
#define OGS_GTP1_APN_AMBR_WITH_NSAPI_TYPE 201
#define OGS_GTP1_GGSN_BACK_OFF_TIME_TYPE 202
#define OGS_GTP1_SIGNALLING_PRIORITY_INDICATION_TYPE 203
#define OGS_GTP1_SIGNALLING_PRIORITY_INDICATION_WITH_NSAPI_TYPE 204
#define OGS_GTP1_HIGHER_BITRATES_THAN_16_MBPS_FLAG_TYPE 205
#define OGS_GTP1_ADDITIONAL_MM_CONTEXT_FOR_SRVCC_TYPE 207
#define OGS_GTP1_ADDITIONAL_FLAGS_FOR_SRVCC_TYPE 208
#define OGS_GTP1_STN_SR_TYPE 209
#define OGS_GTP1_C_MSISDN_TYPE 210
#define OGS_GTP1_EXTENDED_RANAP_CAUSE_TYPE 211
#define OGS_GTP1_ENODEB_ID_TYPE 212
#define OGS_GTP1_SELECTION_MODE_WITH_NSAPI_TYPE 213
#define OGS_GTP1_ULI_TIMESTAMP_TYPE 214
#define OGS_GTP1_LHN_ID_WITH_NSAPI_TYPE 215
#define OGS_GTP1_CN_OPERATOR_SELECTION_ENTITY_TYPE 216
#define OGS_GTP1_UE_USAGE_TYPE_TYPE 217
#define OGS_GTP1_EXTENDED_COMMON_FLAGS_II_TYPE 218
#define OGS_GTP1_NODE_IDENTIFIER_TYPE 219
#define OGS_GTP1_CIOT_OPTIMIZATIONS_SUPPORT_INDICATION_TYPE 220
#define OGS_GTP1_SCEF_PDN_CONNECTION_TYPE 221
#define OGS_GTP1_IOV_UPDATES_COUNTER_TYPE 222
#define OGS_GTP1_MAPPED_UE_USAGE_TYPE_TYPE 223
#define OGS_GTP1_UP_FUNCTION_SELECTION_INDICATION_FLAGS_TYPE 224
#define OGS_GTP1_SPECIAL_IE_TYPE_FOR_IE_TYPE_EXTENSION_TYPE 238
#define OGS_GTP1_CHARGING_GATEWAY_ADDRESS_TYPE 251

/* Information Element TLV Descriptor */
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_cause;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_imsi;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_rai;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_tlli;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_p_tmsi;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_reordering_required;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_authentication_triplet;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_map_cause;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_p_tmsi_signature;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ms_validated;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_recovery;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_selection_mode;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_data_i;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_data_ii;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_teardown_ind;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_nsapi;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ranap_cause;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_rab_context;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_radio_priority_sms;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_radio_priority;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_packet_flow_id;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_charging_characteristics;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_trace_reference;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_trace_type;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ms_not_reachable_reason;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_charging_id;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_end_user_address;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mm_context;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdp_context;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_access_point_name;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_protocol_configuration_options;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_gsn_address;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_msisdn;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_quality_of_service_profile;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_authentication_quintuplet;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_traffic_flow_template;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_target_identification;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_utran_transparent_container;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_rab_setup_information;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_extension_header_type_list;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_trigger_id;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_omc_identity;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ran_transparent_container;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdp_context_prioritization;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_additional_rab_setup_information;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_sgsn_number;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_common_flags;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_apn_restriction;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_radio_priority_lcs;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_rat_type;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_user_location_information;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ms_time_zone;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_sv;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_camel_charging_information_container;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_ue_context;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_tmgi;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_rim_routing_address;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_protocol_configuration_options;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_service_area;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_source_rnc_pdcp_context_info;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_additional_trace_info;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_hop_counter;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_selected_plmn_id;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_identifier;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_2g_3g_indicator;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_enhanced_nsapi;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_duration;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_additional_mbms_trace_info;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_repetition_number;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_time_to_data_transfer;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_bss_container;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_cell_identification;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdu_numbers;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_bssgp_cause;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_required_mbms_bearer_capabilities;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_rim_routing_address_discriminator;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_list_of_set_up_pfcs;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ps_handover_xid_parameters;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ms_info_change_reporting_action;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_direct_tunnel_flags;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_correlation_id;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_bearer_control_mode;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_flow_identifier;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_ip_multicast_distribution;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_distribution_acknowledgement;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_reliable_inter_rat_handover_info;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_rfsp_index;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_fqdn;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_evolved_allocation_retention_priority_i;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_evolved_allocation_retention_priority_ii;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_extended_common_flags;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_uci;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_csg_information_reporting_action;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_csg_id;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_cmi;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ambr;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ue_network_capability;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ue_ambr;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_apn_ambr_with_nsapi;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ggsn_back_off_time;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_signalling_priority_indication;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_signalling_priority_indication_with_nsapi;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_higher_bitrates_than_16_mbps_flag;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_additional_mm_context_for_srvcc;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_additional_flags_for_srvcc;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_stn_sr;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_c_msisdn;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_extended_ranap_cause;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_enodeb_id;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_selection_mode_with_nsapi;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_uli_timestamp;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_lhn_id_with_nsapi;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_cn_operator_selection_entity;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ue_usage_type;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_extended_common_flags_ii;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_node_identifier;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ciot_optimizations_support_indication;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_scef_pdn_connection;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_iov_updates_counter;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mapped_ue_usage_type;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_up_function_selection_indication_flags;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_special_ie_type_for_ie_type_extension;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_charging_gateway_address;

/* Message Descriptor */
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_echo_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_echo_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_version_not_supported;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_node_alive_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_node_alive_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_redirection_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_redirection_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_create_pdp_context_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_create_pdp_context_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_update_pdp_context_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_update_pdp_context_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_delete_pdp_context_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_delete_pdp_context_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_initiate_pdp_context_activation_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_initiate_pdp_context_activation_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_error_indication;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdu_notification_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdu_notification_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdu_notification_reject_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdu_notification_reject_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_supported_extension_headers_notification;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_send_routeing_information_for_gprs_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_send_routeing_information_for_gprs_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_failure_report_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_failure_report_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_note_ms_gprs_present_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_note_ms_gprs_present_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_identification_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_identification_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_sgsn_context_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_sgsn_context_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_sgsn_context_acknowledge;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_forward_relocation_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_forward_relocation_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_forward_relocation_complete;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_relocation_cancel_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_relocation_cancel_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_forward_srns_context;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_forward_relocation_complete_acknowledge;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_forward_srns_context_acknowledge;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ue_registration_query_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ue_registration_query_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_ran_information_relay;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_notification_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_notification_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_notification_reject_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_notification_reject_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_create_mbms_context_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_create_mbms_context_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_update_mbms_context_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_update_mbms_context_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_delete_mbms_context_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_delete_mbms_context_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_registration_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_registration_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_de_registration_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_de_registration_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_start_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_start_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_stop_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_stop_response;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_update_request;
extern ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_update_response;

/* Structure for Information Element */
typedef ogs_tlv_uint8_t ogs_gtp1_tlv_cause_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_imsi_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_rai_t;
typedef ogs_tlv_uint32_t ogs_gtp1_tlv_tlli_t;
typedef ogs_tlv_uint32_t ogs_gtp1_tlv_p_tmsi_t;
typedef ogs_tlv_uint8_t ogs_gtp1_tlv_reordering_required_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_authentication_triplet_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_map_cause_t;
typedef ogs_tlv_uint24_t ogs_gtp1_tlv_p_tmsi_signature_t;
typedef ogs_tlv_uint8_t ogs_gtp1_tlv_ms_validated_t;
typedef ogs_tlv_uint8_t ogs_gtp1_tlv_recovery_t;
typedef ogs_tlv_uint8_t ogs_gtp1_tlv_selection_mode_t;
typedef ogs_tlv_uint32_t ogs_gtp1_tlv_tunnel_endpoint_identifier_data_i_t;
typedef ogs_tlv_uint32_t ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_tunnel_endpoint_identifier_data_ii_t;
typedef ogs_tlv_uint8_t ogs_gtp1_tlv_teardown_ind_t;
typedef ogs_tlv_uint8_t ogs_gtp1_tlv_nsapi_t;
typedef ogs_tlv_uint8_t ogs_gtp1_tlv_ranap_cause_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_rab_context_t;
typedef ogs_tlv_uint8_t ogs_gtp1_tlv_radio_priority_sms_t;
typedef ogs_tlv_uint8_t ogs_gtp1_tlv_radio_priority_t;
typedef ogs_tlv_uint16_t ogs_gtp1_tlv_packet_flow_id_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_charging_characteristics_t;
typedef ogs_tlv_uint16_t ogs_gtp1_tlv_trace_reference_t;
typedef ogs_tlv_uint16_t ogs_gtp1_tlv_trace_type_t;
typedef ogs_tlv_uint8_t ogs_gtp1_tlv_ms_not_reachable_reason_t;
typedef ogs_tlv_uint32_t ogs_gtp1_tlv_charging_id_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_end_user_address_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_mm_context_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_pdp_context_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_access_point_name_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_protocol_configuration_options_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_gsn_address_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_msisdn_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_quality_of_service_profile_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_authentication_quintuplet_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_traffic_flow_template_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_target_identification_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_utran_transparent_container_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_rab_setup_information_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_extension_header_type_list_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_trigger_id_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_omc_identity_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_ran_transparent_container_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_pdp_context_prioritization_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_additional_rab_setup_information_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_sgsn_number_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_common_flags_t;
typedef ogs_tlv_uint8_t ogs_gtp1_tlv_apn_restriction_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_radio_priority_lcs_t;
typedef ogs_tlv_uint8_t ogs_gtp1_tlv_rat_type_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_user_location_information_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_ms_time_zone_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_sv_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_camel_charging_information_container_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_mbms_ue_context_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_tmgi_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_rim_routing_address_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_mbms_protocol_configuration_options_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_mbms_service_area_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_source_rnc_pdcp_context_info_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_additional_trace_info_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_hop_counter_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_selected_plmn_id_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_mbms_session_identifier_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_mbms_2g_3g_indicator_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_enhanced_nsapi_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_mbms_session_duration_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_additional_mbms_trace_info_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_mbms_session_repetition_number_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_mbms_time_to_data_transfer_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_bss_container_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_cell_identification_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_pdu_numbers_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_bssgp_cause_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_required_mbms_bearer_capabilities_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_rim_routing_address_discriminator_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_list_of_set_up_pfcs_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_ps_handover_xid_parameters_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_ms_info_change_reporting_action_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_direct_tunnel_flags_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_correlation_id_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_bearer_control_mode_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_mbms_flow_identifier_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_mbms_ip_multicast_distribution_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_mbms_distribution_acknowledgement_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_reliable_inter_rat_handover_info_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_rfsp_index_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_fqdn_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_evolved_allocation_retention_priority_i_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_evolved_allocation_retention_priority_ii_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_extended_common_flags_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_uci_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_csg_information_reporting_action_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_csg_id_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_cmi_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_ambr_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_ue_network_capability_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_ue_ambr_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_apn_ambr_with_nsapi_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_ggsn_back_off_time_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_signalling_priority_indication_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_signalling_priority_indication_with_nsapi_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_higher_bitrates_than_16_mbps_flag_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_additional_mm_context_for_srvcc_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_additional_flags_for_srvcc_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_stn_sr_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_c_msisdn_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_extended_ranap_cause_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_enodeb_id_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_selection_mode_with_nsapi_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_uli_timestamp_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_lhn_id_with_nsapi_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_cn_operator_selection_entity_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_ue_usage_type_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_extended_common_flags_ii_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_node_identifier_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_ciot_optimizations_support_indication_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_scef_pdn_connection_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_iov_updates_counter_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_mapped_ue_usage_type_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_up_function_selection_indication_flags_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_special_ie_type_for_ie_type_extension_t;
typedef ogs_tlv_octet_t ogs_gtp1_tlv_charging_gateway_address_t;

/* Structure for Message */
typedef struct ogs_gtp1_echo_request_s {
} ogs_gtp1_echo_request_t;

typedef struct ogs_gtp1_echo_response_s {
    ogs_gtp1_tlv_recovery_t recovery;
} ogs_gtp1_echo_response_t;

typedef struct ogs_gtp1_create_pdp_context_request_s {
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_rai_t routeing_area_identity;
    ogs_gtp1_tlv_recovery_t recovery;
    ogs_gtp1_tlv_selection_mode_t selection_mode;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_data_i_t tunnel_endpoint_identifier_data_i;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_nsapi_t nsapi;
    ogs_gtp1_tlv_nsapi_t linked_nsapi;
    ogs_gtp1_tlv_charging_characteristics_t charging_characteristics;
    ogs_gtp1_tlv_trace_reference_t trace_reference;
    ogs_gtp1_tlv_trace_type_t trace_type;
    ogs_gtp1_tlv_end_user_address_t end_user_address;
    ogs_gtp1_tlv_access_point_name_t access_point_name;
    ogs_gtp1_tlv_protocol_configuration_options_t protocol_configuration_options;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_signalling;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_user_traffic;
    ogs_gtp1_tlv_msisdn_t msisdn;
    ogs_gtp1_tlv_quality_of_service_profile_t quality_of_service_profile;
    ogs_gtp1_tlv_traffic_flow_template_t tft;
    ogs_gtp1_tlv_trigger_id_t trigger_id;
    ogs_gtp1_tlv_omc_identity_t omc_identity;
    ogs_gtp1_tlv_common_flags_t common_flags;
    ogs_gtp1_tlv_apn_restriction_t apn_restriction;
    ogs_gtp1_tlv_rat_type_t rat_type;
    ogs_gtp1_tlv_user_location_information_t user_location_information;
    ogs_gtp1_tlv_ms_time_zone_t ms_time_zone;
    ogs_gtp1_tlv_sv_t imei;
    ogs_gtp1_tlv_camel_charging_information_container_t camel_charging_information_container;
    ogs_gtp1_tlv_additional_trace_info_t additional_trace_info;
    ogs_gtp1_tlv_correlation_id_t correlation_id;
    ogs_gtp1_tlv_evolved_allocation_retention_priority_i_t evolved_allocation_retention_priority_i;
    ogs_gtp1_tlv_extended_common_flags_t extended_common_flags;
    ogs_gtp1_tlv_uci_t user_csg_information;
    ogs_gtp1_tlv_ambr_t apn_ambr;
    ogs_gtp1_tlv_signalling_priority_indication_t signalling_priority_indication;
    ogs_gtp1_tlv_cn_operator_selection_entity_t cn_operator_selection_entity;
    ogs_gtp1_tlv_mapped_ue_usage_type_t mapped_ue_usage_type;
    ogs_gtp1_tlv_up_function_selection_indication_flags_t up_function_selection_indication_flags;
} ogs_gtp1_create_pdp_context_request_t;

typedef struct ogs_gtp1_create_pdp_context_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_reordering_required_t reordering_required;
    ogs_gtp1_tlv_recovery_t recovery;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_data_i_t tunnel_endpoint_identifier_data_i;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_nsapi_t nsapi;
    ogs_gtp1_tlv_charging_id_t charging_id;
    ogs_gtp1_tlv_end_user_address_t end_user_address;
    ogs_gtp1_tlv_protocol_configuration_options_t protocol_configuration_options;
    ogs_gtp1_tlv_gsn_address_t ggsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t ggsn_address_for_user_traffic;
    ogs_gtp1_tlv_gsn_address_t alternative_ggsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t alternative_ggsn_address_for_user_traffic;
    ogs_gtp1_tlv_quality_of_service_profile_t quality_of_service_profile;
    ogs_gtp1_tlv_charging_gateway_address_t charging_gateway_address;
    ogs_gtp1_tlv_charging_gateway_address_t alternative_charging_gateway_address;
    ogs_gtp1_tlv_common_flags_t common_flags;
    ogs_gtp1_tlv_apn_restriction_t apn_restriction;
    ogs_gtp1_tlv_ms_info_change_reporting_action_t ms_info_change_reporting_action;
    ogs_gtp1_tlv_bearer_control_mode_t bearer_control_mode;
    ogs_gtp1_tlv_evolved_allocation_retention_priority_i_t evolved_allocation_retention_priority_i;
    ogs_gtp1_tlv_extended_common_flags_t extended_common_flag;
    ogs_gtp1_tlv_csg_information_reporting_action_t csg_information_reporting_action;
    ogs_gtp1_tlv_ambr_t apn_ambr;
    ogs_gtp1_tlv_ggsn_back_off_time_t ggsn_back_off_time;
    ogs_gtp1_tlv_extended_common_flags_ii_t extended_common_flags_ii;
} ogs_gtp1_create_pdp_context_response_t;

typedef struct ogs_gtp1_update_pdp_context_request_s {
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_rai_t routeing_area_identity;
    ogs_gtp1_tlv_recovery_t recovery;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_data_i_t tunnel_endpoint_identifier_data_i;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_nsapi_t nsapi;
    ogs_gtp1_tlv_trace_reference_t trace_reference;
    ogs_gtp1_tlv_trace_type_t trace_type;
    ogs_gtp1_tlv_protocol_configuration_options_t protocol_configuration_options;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_user_traffic;
    ogs_gtp1_tlv_gsn_address_t alternative_sgsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t alternative_sgsn_address_for_user_traffic;
    ogs_gtp1_tlv_quality_of_service_profile_t quality_of_service_profile;
    ogs_gtp1_tlv_traffic_flow_template_t tft;
    ogs_gtp1_tlv_trigger_id_t trigger_id;
    ogs_gtp1_tlv_omc_identity_t omc_identity;
    ogs_gtp1_tlv_common_flags_t common_flags;
    ogs_gtp1_tlv_rat_type_t rat_type;
    ogs_gtp1_tlv_user_location_information_t user_location_information;
    ogs_gtp1_tlv_ms_time_zone_t ms_time_zone;
    ogs_gtp1_tlv_additional_trace_info_t additonal_trace_info;
    ogs_gtp1_tlv_direct_tunnel_flags_t direct_tunnel_flags;
    ogs_gtp1_tlv_evolved_allocation_retention_priority_i_t evolved_allocation_retention_priority_i;
    ogs_gtp1_tlv_extended_common_flags_t extended_common_flags;
    ogs_gtp1_tlv_uci_t user_csg_information;
    ogs_gtp1_tlv_ambr_t apn_ambr;
    ogs_gtp1_tlv_signalling_priority_indication_t signalling_priority_indication;
    ogs_gtp1_tlv_cn_operator_selection_entity_t cn_operator_selection_entity;
    ogs_gtp1_tlv_sv_t imei;
} ogs_gtp1_update_pdp_context_request_t;

typedef struct ogs_gtp1_update_pdp_context_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_recovery_t recovery;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_data_i_t tunnel_endpoint_identifier_data_i;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_charging_id_t charging_id;
    ogs_gtp1_tlv_protocol_configuration_options_t protocol_configuration_options;
    ogs_gtp1_tlv_gsn_address_t ggsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t ggsn_address_for_user_traffic;
    ogs_gtp1_tlv_gsn_address_t alternative_ggsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t alternative_ggsn_address_for_user_traffic;
    ogs_gtp1_tlv_quality_of_service_profile_t quality_of_service_profile;
    ogs_gtp1_tlv_charging_gateway_address_t charging_gateway_address;
    ogs_gtp1_tlv_charging_gateway_address_t alternative_charging_gateway_address;
    ogs_gtp1_tlv_common_flags_t common_flags;
    ogs_gtp1_tlv_apn_restriction_t apn_restriction;
    ogs_gtp1_tlv_bearer_control_mode_t bearer_control_mode;
    ogs_gtp1_tlv_ms_info_change_reporting_action_t ms_info_change_reporting_action;
    ogs_gtp1_tlv_evolved_allocation_retention_priority_i_t evolved_allocation_retention_priority_i;
    ogs_gtp1_tlv_csg_information_reporting_action_t csg_information_reporting_action;
    ogs_gtp1_tlv_ambr_t apn_ambr;
} ogs_gtp1_update_pdp_context_response_t;

typedef struct ogs_gtp1_delete_pdp_context_request_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_teardown_ind_t teardown_ind;
    ogs_gtp1_tlv_nsapi_t nsapi;
    ogs_gtp1_tlv_protocol_configuration_options_t protocol_configuration_options;
    ogs_gtp1_tlv_user_location_information_t user_location_information;
    ogs_gtp1_tlv_ms_time_zone_t ms_time_zone;
    ogs_gtp1_tlv_extended_common_flags_t extended_common_flags;
    ogs_gtp1_tlv_uli_timestamp_t uli_timestamp;
} ogs_gtp1_delete_pdp_context_request_t;

typedef struct ogs_gtp1_delete_pdp_context_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_protocol_configuration_options_t protocol_configuration_options;
    ogs_gtp1_tlv_user_location_information_t user_location_information;
    ogs_gtp1_tlv_ms_time_zone_t ms_time_zone;
    ogs_gtp1_tlv_uli_timestamp_t uli_timestamp;
} ogs_gtp1_delete_pdp_context_response_t;

typedef struct ogs_gtp1_initiate_pdp_context_activation_request_s {
    ogs_gtp1_tlv_nsapi_t linked_nsapi;
    ogs_gtp1_tlv_protocol_configuration_options_t protocol_configuration_options;
    ogs_gtp1_tlv_quality_of_service_profile_t quality_of_service_profile;
    ogs_gtp1_tlv_traffic_flow_template_t tft;
    ogs_gtp1_tlv_correlation_id_t correlation_id;
    ogs_gtp1_tlv_evolved_allocation_retention_priority_i_t evolved_allocation_retention_priority_i;
} ogs_gtp1_initiate_pdp_context_activation_request_t;

typedef struct ogs_gtp1_initiate_pdp_context_activation_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_protocol_configuration_options_t protocol_configuration_options;
} ogs_gtp1_initiate_pdp_context_activation_response_t;

typedef struct ogs_gtp1_pdu_notification_request_s {
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_end_user_address_t end_user_address;
    ogs_gtp1_tlv_access_point_name_t access_point_name;
    ogs_gtp1_tlv_protocol_configuration_options_t protocol_configuration_options;
    ogs_gtp1_tlv_gsn_address_t ggsn_address_for_control_plane;
} ogs_gtp1_pdu_notification_request_t;

typedef struct ogs_gtp1_pdu_notification_response_s {
    ogs_gtp1_tlv_cause_t cause;
} ogs_gtp1_pdu_notification_response_t;

typedef struct ogs_gtp1_pdu_notification_reject_request_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_end_user_address_t end_user_address;
    ogs_gtp1_tlv_access_point_name_t access_point_name;
    ogs_gtp1_tlv_protocol_configuration_options_t protocol_configuration_options;
} ogs_gtp1_pdu_notification_reject_request_t;

typedef struct ogs_gtp1_pdu_notification_reject_response_s {
    ogs_gtp1_tlv_cause_t cause;
} ogs_gtp1_pdu_notification_reject_response_t;

typedef struct ogs_gtp1_supported_extension_headers_notification_s {
    ogs_gtp1_tlv_extension_header_type_list_t extension_header_type_list;
} ogs_gtp1_supported_extension_headers_notification_t;

typedef struct ogs_gtp1_send_routeing_information_for_gprs_request_s {
    ogs_gtp1_tlv_imsi_t imsi;
} ogs_gtp1_send_routeing_information_for_gprs_request_t;

typedef struct ogs_gtp1_send_routeing_information_for_gprs_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_map_cause_t map_cause;
    ogs_gtp1_tlv_ms_not_reachable_reason_t ms_not_reachable_reason;
    ogs_gtp1_tlv_gsn_address_t gsn_address;
} ogs_gtp1_send_routeing_information_for_gprs_response_t;

typedef struct ogs_gtp1_failure_report_request_s {
    ogs_gtp1_tlv_imsi_t imsi;
} ogs_gtp1_failure_report_request_t;

typedef struct ogs_gtp1_failure_report_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_map_cause_t map_cause;
} ogs_gtp1_failure_report_response_t;

typedef struct ogs_gtp1_note_ms_gprs_present_request_s {
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_gsn_address_t gsn_address;
} ogs_gtp1_note_ms_gprs_present_request_t;

typedef struct ogs_gtp1_note_ms_gprs_present_response_s {
    ogs_gtp1_tlv_cause_t cause;
} ogs_gtp1_note_ms_gprs_present_response_t;

typedef struct ogs_gtp1_identification_request_s {
    ogs_gtp1_tlv_rai_t routeing_area_identity;
    ogs_gtp1_tlv_p_tmsi_t packet_tmsi;
    ogs_gtp1_tlv_p_tmsi_signature_t p_tmsi_signature;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_control_plane;
    ogs_gtp1_tlv_hop_counter_t hop_counter;
} ogs_gtp1_identification_request_t;

typedef struct ogs_gtp1_identification_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_authentication_triplet_t authentication_triplet;
    ogs_gtp1_tlv_authentication_quintuplet_t authentication_quintuplet;
    ogs_gtp1_tlv_ue_usage_type_t ue_usage_type;
    ogs_gtp1_tlv_iov_updates_counter_t iov_updates_counter;
} ogs_gtp1_identification_response_t;

typedef struct ogs_gtp1_sgsn_context_request_s {
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_rai_t routeing_area_identity;
    ogs_gtp1_tlv_tlli_t temporary_logical_link_identifier;
    ogs_gtp1_tlv_p_tmsi_t packet_tmsi;
    ogs_gtp1_tlv_p_tmsi_signature_t p_tmsi_signature;
    ogs_gtp1_tlv_ms_validated_t ms_validated;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t alternative_sgsn_address_for_control_plane;
    ogs_gtp1_tlv_sgsn_number_t sgsn_number;
    ogs_gtp1_tlv_rat_type_t rat_type;
    ogs_gtp1_tlv_hop_counter_t hop_counter;
} ogs_gtp1_sgsn_context_request_t;

typedef struct ogs_gtp1_sgsn_context_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_rab_context_t rab_context;
    ogs_gtp1_tlv_radio_priority_sms_t radio_priority_sms;
    ogs_gtp1_tlv_radio_priority_t radio_priority;
    ogs_gtp1_tlv_packet_flow_id_t packet_flow_id;
    ogs_gtp1_tlv_charging_characteristics_t charging_characteristics;
    ogs_gtp1_tlv_radio_priority_lcs_t radio_priority_lcs;
    ogs_gtp1_tlv_mm_context_t mm_context;
    ogs_gtp1_tlv_pdp_context_t pdp_context;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t alternative_ggsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t alternative_ggsn_address_for_user_traffic;
    ogs_gtp1_tlv_pdp_context_prioritization_t pdp_context_prioritization;
    ogs_gtp1_tlv_mbms_ue_context_t mbms_ue_context;
    ogs_gtp1_tlv_rfsp_index_t subscribed_rfsp_index;
    ogs_gtp1_tlv_rfsp_index_t rfsp_index_in_use;
    ogs_gtp1_tlv_fqdn_t co_located_ggsn_pgw_fqdn;
    ogs_gtp1_tlv_evolved_allocation_retention_priority_ii_t evolved_allocation_retention_priority_ii;
    ogs_gtp1_tlv_extended_common_flags_t extended_common_flags;
    ogs_gtp1_tlv_ue_network_capability_t ue_network_capability;
    ogs_gtp1_tlv_ue_ambr_t ue_ambr;
    ogs_gtp1_tlv_apn_ambr_with_nsapi_t apn_ambr_with_nsapi;
    ogs_gtp1_tlv_signalling_priority_indication_with_nsapi_t signalling_priority_indication_with_nsapi;
    ogs_gtp1_tlv_higher_bitrates_than_16_mbps_flag_t higher_bitrates_than_16_mbps_flag;
    ogs_gtp1_tlv_selection_mode_with_nsapi_t selection_mode_with_nsapi;
    ogs_gtp1_tlv_lhn_id_with_nsapi_t local_home_network_id_with_nsapi;
    ogs_gtp1_tlv_ue_usage_type_t ue_usage_type;
    ogs_gtp1_tlv_extended_common_flags_ii_t extended_common_flags_ii;
    ogs_gtp1_tlv_scef_pdn_connection_t ue_scef_pdn_connection;
    ogs_gtp1_tlv_iov_updates_counter_t iov_updates_counter;
} ogs_gtp1_sgsn_context_response_t;

typedef struct ogs_gtp1_sgsn_context_acknowledge_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_data_ii_t tunnel_endpoint_identifier_data_ii;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_user_traffic;
    ogs_gtp1_tlv_sgsn_number_t sgsn_number;
    ogs_gtp1_tlv_node_identifier_t node_identifier;
} ogs_gtp1_sgsn_context_acknowledge_t;

typedef struct ogs_gtp1_forward_relocation_request_s {
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_ranap_cause_t ranap_cause;
    ogs_gtp1_tlv_packet_flow_id_t packet_flow_id;
    ogs_gtp1_tlv_charging_characteristics_t charging_characteristics;
    ogs_gtp1_tlv_mm_context_t mm_context;
    ogs_gtp1_tlv_pdp_context_t pdp_context;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t alternative_ggsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t alternative_ggsn_address_for_user_traffic;
    ogs_gtp1_tlv_target_identification_t target_identification;
    ogs_gtp1_tlv_utran_transparent_container_t utran_transparent_container;
    ogs_gtp1_tlv_pdp_context_prioritization_t pdp_context_prioritization;
    ogs_gtp1_tlv_mbms_ue_context_t mbms_ue_context;
    ogs_gtp1_tlv_selected_plmn_id_t selected_plmn_id;
    ogs_gtp1_tlv_bss_container_t bss_container;
    ogs_gtp1_tlv_cell_identification_t cell_identification;
    ogs_gtp1_tlv_bssgp_cause_t bssgp_cause;
    ogs_gtp1_tlv_ps_handover_xid_parameters_t ps_handover_xid_parameters;
    ogs_gtp1_tlv_direct_tunnel_flags_t direct_tunnel_flags;
    ogs_gtp1_tlv_reliable_inter_rat_handover_info_t reliable_inter_rat_handover_info;
    ogs_gtp1_tlv_rfsp_index_t subscribed_rfsp_index;
    ogs_gtp1_tlv_rfsp_index_t rfsp_index_in_use;
    ogs_gtp1_tlv_fqdn_t co_located_ggsn_pgw_fqdn;
    ogs_gtp1_tlv_evolved_allocation_retention_priority_ii_t evolved_allocation_retention_priority_ii;
    ogs_gtp1_tlv_extended_common_flags_t extended_common_flags;
    ogs_gtp1_tlv_csg_id_t csg_id;
    ogs_gtp1_tlv_cmi_t csg_membership_indication;
    ogs_gtp1_tlv_ue_network_capability_t ue_network_capability;
    ogs_gtp1_tlv_ue_ambr_t ue_ambr;
    ogs_gtp1_tlv_apn_ambr_with_nsapi_t apn_ambr_with_nsapi;
    ogs_gtp1_tlv_signalling_priority_indication_with_nsapi_t signalling_priority_indication_with_nsapi;
    ogs_gtp1_tlv_higher_bitrates_than_16_mbps_flag_t higher_bitrates_than_16_mbps_flag;
    ogs_gtp1_tlv_additional_mm_context_for_srvcc_t additional_mm_context_for_srvcc;
    ogs_gtp1_tlv_additional_flags_for_srvcc_t additional_flags_for_srvcc;
    ogs_gtp1_tlv_stn_sr_t stn_sr;
    ogs_gtp1_tlv_c_msisdn_t c_msisdn;
    ogs_gtp1_tlv_extended_ranap_cause_t extended_ranap_cause;
    ogs_gtp1_tlv_enodeb_id_t enodeb_id;
    ogs_gtp1_tlv_selection_mode_with_nsapi_t selection_mode_with_nsapi;
    ogs_gtp1_tlv_ue_usage_type_t ue_usage_type;
    ogs_gtp1_tlv_extended_common_flags_ii_t extended_common_flags_ii;
    ogs_gtp1_tlv_scef_pdn_connection_t ue_scef_pdn_connection;
} ogs_gtp1_forward_relocation_request_t;

typedef struct ogs_gtp1_forward_relocation_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_data_ii_t tunnel_endpoint_identifier_data_ii;
    ogs_gtp1_tlv_ranap_cause_t ranap_cause;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_user_traffic;
    ogs_gtp1_tlv_utran_transparent_container_t utran_transparent_container;
    ogs_gtp1_tlv_rab_setup_information_t rab_setup_information;
    ogs_gtp1_tlv_additional_rab_setup_information_t additional_rab_setup_information;
    ogs_gtp1_tlv_sgsn_number_t sgsn_number;
    ogs_gtp1_tlv_bss_container_t bss_container;
    ogs_gtp1_tlv_bssgp_cause_t bssgp_cause;
    ogs_gtp1_tlv_list_of_set_up_pfcs_t list_of_set_up_pfcs;
    ogs_gtp1_tlv_extended_ranap_cause_t extended_ranap_cause;
    ogs_gtp1_tlv_node_identifier_t node_identfiier;
} ogs_gtp1_forward_relocation_response_t;

typedef struct ogs_gtp1_forward_relocation_complete_s {
} ogs_gtp1_forward_relocation_complete_t;

typedef struct ogs_gtp1_relocation_cancel_request_s {
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_sv_t imei;
    ogs_gtp1_tlv_extended_common_flags_t extended_common_flags;
    ogs_gtp1_tlv_extended_ranap_cause_t extended_ranap_cause;
} ogs_gtp1_relocation_cancel_request_t;

typedef struct ogs_gtp1_relocation_cancel_response_s {
    ogs_gtp1_tlv_cause_t cause;
} ogs_gtp1_relocation_cancel_response_t;

typedef struct ogs_gtp1_forward_srns_context_s {
    ogs_gtp1_tlv_rab_context_t rab_context;
    ogs_gtp1_tlv_source_rnc_pdcp_context_info_t source_rnc_pdcp_context_info;
    ogs_gtp1_tlv_pdu_numbers_t pdu_numbers;
} ogs_gtp1_forward_srns_context_t;

typedef struct ogs_gtp1_forward_relocation_complete_acknowledge_s {
    ogs_gtp1_tlv_cause_t cause;
} ogs_gtp1_forward_relocation_complete_acknowledge_t;

typedef struct ogs_gtp1_forward_srns_context_acknowledge_s {
    ogs_gtp1_tlv_cause_t cause;
} ogs_gtp1_forward_srns_context_acknowledge_t;

typedef struct ogs_gtp1_ue_registration_query_request_s {
    ogs_gtp1_tlv_imsi_t imsi;
} ogs_gtp1_ue_registration_query_request_t;

typedef struct ogs_gtp1_ue_registration_query_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_selected_plmn_id_t selected_plmn_id;
} ogs_gtp1_ue_registration_query_response_t;

typedef struct ogs_gtp1_ran_information_relay_s {
    ogs_gtp1_tlv_ran_transparent_container_t ran_transparent_container;
    ogs_gtp1_tlv_rim_routing_address_t rim_routing_address;
    ogs_gtp1_tlv_rim_routing_address_discriminator_t rim_routing_address_discriminator;
} ogs_gtp1_ran_information_relay_t;

typedef struct ogs_gtp1_mbms_notification_request_s {
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_nsapi_t nsapi;
    ogs_gtp1_tlv_end_user_address_t end_user_address;
    ogs_gtp1_tlv_access_point_name_t access_point_name;
    ogs_gtp1_tlv_gsn_address_t ggsn_address_for_control_plane;
    ogs_gtp1_tlv_mbms_protocol_configuration_options_t mbms_protocol_configuration_options;
} ogs_gtp1_mbms_notification_request_t;

typedef struct ogs_gtp1_mbms_notification_response_s {
    ogs_gtp1_tlv_cause_t cause;
} ogs_gtp1_mbms_notification_response_t;

typedef struct ogs_gtp1_mbms_notification_reject_request_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_nsapi_t nsapi;
    ogs_gtp1_tlv_end_user_address_t end_user_address;
    ogs_gtp1_tlv_access_point_name_t access_point_name;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_control_plane;
} ogs_gtp1_mbms_notification_reject_request_t;

typedef struct ogs_gtp1_mbms_notification_reject_response_s {
    ogs_gtp1_tlv_cause_t cause;
} ogs_gtp1_mbms_notification_reject_response_t;

typedef struct ogs_gtp1_create_mbms_context_request_s {
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_rai_t routeing_area_identity;
    ogs_gtp1_tlv_recovery_t recovery;
    ogs_gtp1_tlv_selection_mode_t selection_mode;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_trace_reference_t trace_reference;
    ogs_gtp1_tlv_trace_type_t trace_type;
    ogs_gtp1_tlv_end_user_address_t end_user_address;
    ogs_gtp1_tlv_access_point_name_t access_point_name;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_signalling;
    ogs_gtp1_tlv_msisdn_t msisdn;
    ogs_gtp1_tlv_trigger_id_t trigger_id;
    ogs_gtp1_tlv_omc_identity_t omc_identity;
    ogs_gtp1_tlv_rat_type_t rat_type;
    ogs_gtp1_tlv_user_location_information_t user_location_information;
    ogs_gtp1_tlv_ms_time_zone_t ms_time_zone;
    ogs_gtp1_tlv_sv_t imei;
    ogs_gtp1_tlv_mbms_protocol_configuration_options_t mbms_protocol_configuration_options;
    ogs_gtp1_tlv_additional_trace_info_t additonal_trace_info;
    ogs_gtp1_tlv_enhanced_nsapi_t enhanced_nsapi;
    ogs_gtp1_tlv_additional_mbms_trace_info_t additional_mbms_trace_info;
} ogs_gtp1_create_mbms_context_request_t;

typedef struct ogs_gtp1_create_mbms_context_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_recovery_t recovery;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_charging_id_t charging_id;
    ogs_gtp1_tlv_gsn_address_t ggsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t alternative_ggsn_address_for_control_plane;
    ogs_gtp1_tlv_charging_gateway_address_t charging_gateway_address;
    ogs_gtp1_tlv_charging_gateway_address_t alternative_charging_gateway_address;
    ogs_gtp1_tlv_mbms_protocol_configuration_options_t mbms_protocol_configuration_options;
} ogs_gtp1_create_mbms_context_response_t;

typedef struct ogs_gtp1_update_mbms_context_request_s {
    ogs_gtp1_tlv_rai_t routeing_area_identity;
    ogs_gtp1_tlv_recovery_t recovery;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_trace_reference_t trace_reference;
    ogs_gtp1_tlv_trace_type_t trace_type;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t alternative_sgsn_address_for_control_plane;
    ogs_gtp1_tlv_trigger_id_t trigger_id;
    ogs_gtp1_tlv_omc_identity_t omc_identity;
    ogs_gtp1_tlv_rat_type_t rat_type;
    ogs_gtp1_tlv_user_location_information_t user_location_information;
    ogs_gtp1_tlv_ms_time_zone_t ms_time_zone;
    ogs_gtp1_tlv_additional_trace_info_t additional_trace_info;
    ogs_gtp1_tlv_enhanced_nsapi_t enhanced_nsapi;
    ogs_gtp1_tlv_additional_mbms_trace_info_t additional_mbms_trace_info;
} ogs_gtp1_update_mbms_context_request_t;

typedef struct ogs_gtp1_update_mbms_context_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_recovery_t recovery;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_charging_id_t charging_id;
    ogs_gtp1_tlv_gsn_address_t ggsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t alternative_ggsn_address_for_control_plane;
    ogs_gtp1_tlv_charging_gateway_address_t charging_gateway_address;
    ogs_gtp1_tlv_charging_gateway_address_t alternative_charging_gateway_address;
} ogs_gtp1_update_mbms_context_response_t;

typedef struct ogs_gtp1_delete_mbms_context_request_s {
    ogs_gtp1_tlv_imsi_t imsi;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_end_user_address_t end_user_address;
    ogs_gtp1_tlv_access_point_name_t access_point_name;
    ogs_gtp1_tlv_mbms_protocol_configuration_options_t mbms_protocol_configuration_options;
    ogs_gtp1_tlv_enhanced_nsapi_t enhanced_nsapi;
} ogs_gtp1_delete_mbms_context_request_t;

typedef struct ogs_gtp1_delete_mbms_context_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_mbms_protocol_configuration_options_t mbms_protocol_configuration_options;
} ogs_gtp1_delete_mbms_context_response_t;

typedef struct ogs_gtp1_mbms_registration_request_s {
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_end_user_address_t end_user_address;
    ogs_gtp1_tlv_access_point_name_t access_point_name;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t alternative_sgsn_address_for_control_plane;
} ogs_gtp1_mbms_registration_request_t;

typedef struct ogs_gtp1_mbms_registration_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_gsn_address_t ggsn_address_for_control_plane;
    ogs_gtp1_tlv_tmgi_t temporary_mobile_group_identity;
    ogs_gtp1_tlv_required_mbms_bearer_capabilities_t required_mbms_bearer_capabilities;
} ogs_gtp1_mbms_registration_response_t;

typedef struct ogs_gtp1_mbms_de_registration_request_s {
    ogs_gtp1_tlv_end_user_address_t end_user_address;
    ogs_gtp1_tlv_access_point_name_t access_point_name;
} ogs_gtp1_mbms_de_registration_request_t;

typedef struct ogs_gtp1_mbms_de_registration_response_s {
    ogs_gtp1_tlv_cause_t cause;
} ogs_gtp1_mbms_de_registration_response_t;

typedef struct ogs_gtp1_mbms_session_start_request_s {
    ogs_gtp1_tlv_recovery_t recovery;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_end_user_address_t end_user_address;
    ogs_gtp1_tlv_access_point_name_t access_point_name;
    ogs_gtp1_tlv_gsn_address_t ggsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t alternative_ggsn_address_for_control_plane;
    ogs_gtp1_tlv_quality_of_service_profile_t quality_of_service_profile;
    ogs_gtp1_tlv_common_flags_t common_flags;
    ogs_gtp1_tlv_tmgi_t temporary_mobile_group_identity;
    ogs_gtp1_tlv_mbms_service_area_t mbms_service_area;
    ogs_gtp1_tlv_mbms_session_identifier_t mbms_session_identifier;
    ogs_gtp1_tlv_mbms_2g_3g_indicator_t mbms_2g_3g_indicator;
    ogs_gtp1_tlv_mbms_session_duration_t mbms_session_duration;
    ogs_gtp1_tlv_mbms_session_repetition_number_t mbms_session_repetition_number;
    ogs_gtp1_tlv_mbms_time_to_data_transfer_t mbms_time_to_data_transfer;
    ogs_gtp1_tlv_mbms_flow_identifier_t mbms_flow_identifier;
    ogs_gtp1_tlv_mbms_ip_multicast_distribution_t mbms_ip_multicast_distribution;
} ogs_gtp1_mbms_session_start_request_t;

typedef struct ogs_gtp1_mbms_session_start_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_recovery_t recovery;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_data_i_t tunnel_endpoint_identifier_data_i;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_control_plane;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_user_traffic;
    ogs_gtp1_tlv_gsn_address_t alternative_sgsn_address_for_user_traffic;
    ogs_gtp1_tlv_mbms_distribution_acknowledgement_t mbms_distribution_acknowledgement;
} ogs_gtp1_mbms_session_start_response_t;

typedef struct ogs_gtp1_mbms_session_stop_request_s {
    ogs_gtp1_tlv_end_user_address_t end_user_address;
    ogs_gtp1_tlv_access_point_name_t access_point_name;
    ogs_gtp1_tlv_mbms_flow_identifier_t mbms_flow_identifier;
} ogs_gtp1_mbms_session_stop_request_t;

typedef struct ogs_gtp1_mbms_session_stop_response_s {
    ogs_gtp1_tlv_cause_t cause;
} ogs_gtp1_mbms_session_stop_response_t;

typedef struct ogs_gtp1_mbms_session_update_request_s {
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_end_user_address_t end_user_address;
    ogs_gtp1_tlv_access_point_name_t access_point_name;
    ogs_gtp1_tlv_gsn_address_t ggsn_address_for_control_plane;
    ogs_gtp1_tlv_tmgi_t temporary_mobile_group_identity;
    ogs_gtp1_tlv_mbms_session_duration_t mbms_session_duration;
    ogs_gtp1_tlv_mbms_service_area_t mbms_service_area;
    ogs_gtp1_tlv_mbms_session_identifier_t mbms_session_identifier;
    ogs_gtp1_tlv_mbms_session_repetition_number_t mbms_session_repetition_number;
    ogs_gtp1_tlv_mbms_flow_identifier_t mbms_flow_identifier;
} ogs_gtp1_mbms_session_update_request_t;

typedef struct ogs_gtp1_mbms_session_update_response_s {
    ogs_gtp1_tlv_cause_t cause;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_data_i_t tunnel_endpoint_identifier_data_i;
    ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t tunnel_endpoint_identifier_control_plane;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_data_i;
    ogs_gtp1_tlv_gsn_address_t sgsn_address_for_control_plane;
} ogs_gtp1_mbms_session_update_response_t;

typedef struct ogs_gtp1_message_s {
   ogs_gtp1_header_t h;
   union {
        ogs_gtp1_echo_request_t echo_request;
        ogs_gtp1_echo_response_t echo_response;
        ogs_gtp1_create_pdp_context_request_t create_pdp_context_request;
        ogs_gtp1_create_pdp_context_response_t create_pdp_context_response;
        ogs_gtp1_update_pdp_context_request_t update_pdp_context_request;
        ogs_gtp1_update_pdp_context_response_t update_pdp_context_response;
        ogs_gtp1_delete_pdp_context_request_t delete_pdp_context_request;
        ogs_gtp1_delete_pdp_context_response_t delete_pdp_context_response;
        ogs_gtp1_initiate_pdp_context_activation_request_t initiate_pdp_context_activation_request;
        ogs_gtp1_initiate_pdp_context_activation_response_t initiate_pdp_context_activation_response;
        ogs_gtp1_pdu_notification_request_t pdu_notification_request;
        ogs_gtp1_pdu_notification_response_t pdu_notification_response;
        ogs_gtp1_pdu_notification_reject_request_t pdu_notification_reject_request;
        ogs_gtp1_pdu_notification_reject_response_t pdu_notification_reject_response;
        ogs_gtp1_supported_extension_headers_notification_t supported_extension_headers_notification;
        ogs_gtp1_send_routeing_information_for_gprs_request_t send_routeing_information_for_gprs_request;
        ogs_gtp1_send_routeing_information_for_gprs_response_t send_routeing_information_for_gprs_response;
        ogs_gtp1_failure_report_request_t failure_report_request;
        ogs_gtp1_failure_report_response_t failure_report_response;
        ogs_gtp1_note_ms_gprs_present_request_t note_ms_gprs_present_request;
        ogs_gtp1_note_ms_gprs_present_response_t note_ms_gprs_present_response;
        ogs_gtp1_identification_request_t identification_request;
        ogs_gtp1_identification_response_t identification_response;
        ogs_gtp1_sgsn_context_request_t sgsn_context_request;
        ogs_gtp1_sgsn_context_response_t sgsn_context_response;
        ogs_gtp1_sgsn_context_acknowledge_t sgsn_context_acknowledge;
        ogs_gtp1_forward_relocation_request_t forward_relocation_request;
        ogs_gtp1_forward_relocation_response_t forward_relocation_response;
        ogs_gtp1_forward_relocation_complete_t forward_relocation_complete;
        ogs_gtp1_relocation_cancel_request_t relocation_cancel_request;
        ogs_gtp1_relocation_cancel_response_t relocation_cancel_response;
        ogs_gtp1_forward_srns_context_t forward_srns_context;
        ogs_gtp1_forward_relocation_complete_acknowledge_t forward_relocation_complete_acknowledge;
        ogs_gtp1_forward_srns_context_acknowledge_t forward_srns_context_acknowledge;
        ogs_gtp1_ue_registration_query_request_t ue_registration_query_request;
        ogs_gtp1_ue_registration_query_response_t ue_registration_query_response;
        ogs_gtp1_ran_information_relay_t ran_information_relay;
        ogs_gtp1_mbms_notification_request_t mbms_notification_request;
        ogs_gtp1_mbms_notification_response_t mbms_notification_response;
        ogs_gtp1_mbms_notification_reject_request_t mbms_notification_reject_request;
        ogs_gtp1_mbms_notification_reject_response_t mbms_notification_reject_response;
        ogs_gtp1_create_mbms_context_request_t create_mbms_context_request;
        ogs_gtp1_create_mbms_context_response_t create_mbms_context_response;
        ogs_gtp1_update_mbms_context_request_t update_mbms_context_request;
        ogs_gtp1_update_mbms_context_response_t update_mbms_context_response;
        ogs_gtp1_delete_mbms_context_request_t delete_mbms_context_request;
        ogs_gtp1_delete_mbms_context_response_t delete_mbms_context_response;
        ogs_gtp1_mbms_registration_request_t mbms_registration_request;
        ogs_gtp1_mbms_registration_response_t mbms_registration_response;
        ogs_gtp1_mbms_de_registration_request_t mbms_de_registration_request;
        ogs_gtp1_mbms_de_registration_response_t mbms_de_registration_response;
        ogs_gtp1_mbms_session_start_request_t mbms_session_start_request;
        ogs_gtp1_mbms_session_start_response_t mbms_session_start_response;
        ogs_gtp1_mbms_session_stop_request_t mbms_session_stop_request;
        ogs_gtp1_mbms_session_stop_response_t mbms_session_stop_response;
        ogs_gtp1_mbms_session_update_request_t mbms_session_update_request;
        ogs_gtp1_mbms_session_update_response_t mbms_session_update_response;
   };
} ogs_gtp1_message_t;

int ogs_gtp1_parse_msg(ogs_gtp1_message_t *gtp1_message, ogs_pkbuf_t *pkbuf);
ogs_pkbuf_t *ogs_gtp1_build_msg(ogs_gtp1_message_t *gtp1_message);

#ifdef __cplusplus
}
#endif

#endif /* OGS_GTP1_MESSAGE_H */
