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

/*******************************************************************************
 * This file had been created by pfcp-tlv.py script v0.1.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2022-03-30 17:00:53.600137 by pespin
 * from 29244-g10.docx
 ******************************************************************************/

#if !defined(OGS_PFCP_INSIDE) && !defined(OGS_PFCP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_PFCP_MESSAGE_H
#define OGS_PFCP_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/* 5.1 General format */
#define OGS_PFCP_HEADER_LEN 16
#define OGS_PFCP_SEID_LEN   8
typedef struct ogs_pfcp_header_s {
    union {
        struct {
        ED4(uint8_t version:3;,
            uint8_t spare1:3;,
            uint8_t mp:1;,
            uint8_t seid_presence:1;)
        };
        uint8_t flags;
    };
    uint8_t type;
    uint16_t length;
    union {
        struct {
            uint64_t seid;
            /* sqn : 31bit ~ 8bit, spare : 7bit ~ 0bit */
#define OGS_PFCP_XID_TO_SQN(__xid) htobe32(((__xid) << 8))
#define OGS_PFCP_SQN_TO_XID(__sqn) (be32toh(__sqn) >> 8)
            uint32_t sqn;
        };
        /* sqn : 31bit ~ 8bit, spare : 7bit ~ 0bit */
        uint32_t sqn_only;
    };
} __attribute__ ((packed)) ogs_pfcp_header_t;

/* PFCP message type */
#define OGS_PFCP_HEARTBEAT_REQUEST_TYPE 1
#define OGS_PFCP_HEARTBEAT_RESPONSE_TYPE 2
#define OGS_PFCP_PFD_MANAGEMENT_REQUEST_TYPE 3
#define OGS_PFCP_PFD_MANAGEMENT_RESPONSE_TYPE 4
#define OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE 5
#define OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE 6
#define OGS_PFCP_ASSOCIATION_UPDATE_REQUEST_TYPE 7
#define OGS_PFCP_ASSOCIATION_UPDATE_RESPONSE_TYPE 8
#define OGS_PFCP_ASSOCIATION_RELEASE_REQUEST_TYPE 9
#define OGS_PFCP_ASSOCIATION_RELEASE_RESPONSE_TYPE 10
#define OGS_PFCP_VERSION_NOT_SUPPORTED_RESPONSE_TYPE 11
#define OGS_PFCP_NODE_REPORT_REQUEST_TYPE 12
#define OGS_PFCP_NODE_REPORT_RESPONSE_TYPE 13
#define OGS_PFCP_SESSION_SET_DELETION_REQUEST_TYPE 14
#define OGS_PFCP_SESSION_SET_DELETION_RESPONSE_TYPE 15
#define OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE 50
#define OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE 51
#define OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE 52
#define OGS_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE 53
#define OGS_PFCP_SESSION_DELETION_REQUEST_TYPE 54
#define OGS_PFCP_SESSION_DELETION_RESPONSE_TYPE 55
#define OGS_PFCP_SESSION_REPORT_REQUEST_TYPE 56
#define OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE 57

#define OGS_PFCP_CREATE_PDR_TYPE 1
#define OGS_PFCP_PDI_TYPE 2
#define OGS_PFCP_CREATE_FAR_TYPE 3
#define OGS_PFCP_FORWARDING_PARAMETERS_TYPE 4
#define OGS_PFCP_DUPLICATING_PARAMETERS_TYPE 5
#define OGS_PFCP_CREATE_URR_TYPE 6
#define OGS_PFCP_CREATE_QER_TYPE 7
#define OGS_PFCP_CREATED_PDR_TYPE 8
#define OGS_PFCP_UPDATE_PDR_TYPE 9
#define OGS_PFCP_UPDATE_FAR_TYPE 10
#define OGS_PFCP_UPDATE_FORWARDING_PARAMETERS_TYPE 11
#define OGS_PFCP_UPDATE_BAR_PFCP_SESSION_REPORT_RESPONSE_TYPE 12
#define OGS_PFCP_UPDATE_URR_TYPE 13
#define OGS_PFCP_UPDATE_QER_TYPE 14
#define OGS_PFCP_REMOVE_PDR_TYPE 15
#define OGS_PFCP_REMOVE_FAR_TYPE 16
#define OGS_PFCP_REMOVE_URR_TYPE 17
#define OGS_PFCP_REMOVE_QER_TYPE 18
#define OGS_PFCP_CAUSE_TYPE 19
#define OGS_PFCP_SOURCE_INTERFACE_TYPE 20
#define OGS_PFCP_F_TEID_TYPE 21
#define OGS_PFCP_NETWORK_INSTANCE_TYPE 22
#define OGS_PFCP_SDF_FILTER_TYPE 23
#define OGS_PFCP_APPLICATION_ID_TYPE 24
#define OGS_PFCP_GATE_STATUS_TYPE 25
#define OGS_PFCP_MBR_TYPE 26
#define OGS_PFCP_GBR_TYPE 27
#define OGS_PFCP_QER_CORRELATION_ID_TYPE 28
#define OGS_PFCP_PRECEDENCE_TYPE 29
#define OGS_PFCP_TRANSPORT_LEVEL_MARKING_TYPE 30
#define OGS_PFCP_VOLUME_THRESHOLD_TYPE 31
#define OGS_PFCP_TIME_THRESHOLD_TYPE 32
#define OGS_PFCP_MONITORING_TIME_TYPE 33
#define OGS_PFCP_SUBSEQUENT_VOLUME_THRESHOLD_TYPE 34
#define OGS_PFCP_SUBSEQUENT_TIME_THRESHOLD_TYPE 35
#define OGS_PFCP_INACTIVITY_DETECTION_TIME_TYPE 36
#define OGS_PFCP_REPORTING_TRIGGERS_TYPE 37
#define OGS_PFCP_REDIRECT_INFORMATION_TYPE 38
#define OGS_PFCP_REPORT_TYPE_TYPE 39
#define OGS_PFCP_OFFENDING_IE_TYPE 40
#define OGS_PFCP_FORWARDING_POLICY_TYPE 41
#define OGS_PFCP_DESTINATION_INTERFACE_TYPE 42
#define OGS_PFCP_UP_FUNCTION_FEATURES_TYPE 43
#define OGS_PFCP_APPLY_ACTION_TYPE 44
#define OGS_PFCP_DOWNLINK_DATA_SERVICE_INFORMATION_TYPE 45
#define OGS_PFCP_DOWNLINK_DATA_NOTIFICATION_DELAY_TYPE 46
#define OGS_PFCP_DL_BUFFERING_DURATION_TYPE 47
#define OGS_PFCP_DL_BUFFERING_SUGGESTED_PACKET_COUNT_TYPE 48
#define OGS_PFCP_PFCPSMREQ_FLAGS_TYPE 49
#define OGS_PFCP_PFCPSRRSP_FLAGS_TYPE 50
#define OGS_PFCP_LOAD_CONTROL_INFORMATION_TYPE 51
#define OGS_PFCP_SEQUENCE_NUMBER_TYPE 52
#define OGS_PFCP_METRIC_TYPE 53
#define OGS_PFCP_OVERLOAD_CONTROL_INFORMATION_TYPE 54
#define OGS_PFCP_TIMER_TYPE 55
#define OGS_PFCP_PDR_ID_TYPE 56
#define OGS_PFCP_F_SEID_TYPE 57
#define OGS_PFCP_APPLICATION_ID_S_PFDS_TYPE 58
#define OGS_PFCP_PFD_CONTEXT_TYPE 59
#define OGS_PFCP_NODE_ID_TYPE 60
#define OGS_PFCP_PFD_CONTENTS_TYPE 61
#define OGS_PFCP_MEASUREMENT_METHOD_TYPE 62
#define OGS_PFCP_USAGE_REPORT_TRIGGER_TYPE 63
#define OGS_PFCP_MEASUREMENT_PERIOD_TYPE 64
#define OGS_PFCP_FQ_CSID_TYPE 65
#define OGS_PFCP_VOLUME_MEASUREMENT_TYPE 66
#define OGS_PFCP_DURATION_MEASUREMENT_TYPE 67
#define OGS_PFCP_APPLICATION_DETECTION_INFORMATION_TYPE 68
#define OGS_PFCP_TIME_OF_FIRST_PACKET_TYPE 69
#define OGS_PFCP_TIME_OF_LAST_PACKET_TYPE 70
#define OGS_PFCP_QUOTA_HOLDING_TIME_TYPE 71
#define OGS_PFCP_DROPPED_DL_TRAFFIC_THRESHOLD_TYPE 72
#define OGS_PFCP_VOLUME_QUOTA_TYPE 73
#define OGS_PFCP_TIME_QUOTA_TYPE 74
#define OGS_PFCP_START_TIME_TYPE 75
#define OGS_PFCP_END_TIME_TYPE 76
#define OGS_PFCP_QUERY_URR_TYPE 77
#define OGS_PFCP_USAGE_REPORT_SESSION_MODIFICATION_RESPONSE_TYPE 78
#define OGS_PFCP_USAGE_REPORT_SESSION_DELETION_RESPONSE_TYPE 79
#define OGS_PFCP_USAGE_REPORT_SESSION_REPORT_REQUEST_TYPE 80
#define OGS_PFCP_URR_ID_TYPE 81
#define OGS_PFCP_LINKED_URR_ID_TYPE 82
#define OGS_PFCP_DOWNLINK_DATA_REPORT_TYPE 83
#define OGS_PFCP_OUTER_HEADER_CREATION_TYPE 84
#define OGS_PFCP_CREATE_BAR_TYPE 85
#define OGS_PFCP_UPDATE_BAR_SESSION_MODIFICATION_REQUEST_TYPE 86
#define OGS_PFCP_REMOVE_BAR_TYPE 87
#define OGS_PFCP_BAR_ID_TYPE 88
#define OGS_PFCP_CP_FUNCTION_FEATURES_TYPE 89
#define OGS_PFCP_USAGE_INFORMATION_TYPE 90
#define OGS_PFCP_APPLICATION_INSTANCE_ID_TYPE 91
#define OGS_PFCP_FLOW_INFORMATION_TYPE 92
#define OGS_PFCP_UE_IP_ADDRESS_TYPE 93
#define OGS_PFCP_PACKET_RATE_TYPE 94
#define OGS_PFCP_OUTER_HEADER_REMOVAL_TYPE 95
#define OGS_PFCP_RECOVERY_TIME_STAMP_TYPE 96
#define OGS_PFCP_DL_FLOW_LEVEL_MARKING_TYPE 97
#define OGS_PFCP_HEADER_ENRICHMENT_TYPE 98
#define OGS_PFCP_ERROR_INDICATION_REPORT_TYPE 99
#define OGS_PFCP_MEASUREMENT_INFORMATION_TYPE 100
#define OGS_PFCP_NODE_REPORT_TYPE_TYPE 101
#define OGS_PFCP_USER_PLANE_PATH_FAILURE_REPORT_TYPE 102
#define OGS_PFCP_REMOTE_GTP_U_PEER_TYPE 103
#define OGS_PFCP_UR_SEQN_TYPE 104
#define OGS_PFCP_UPDATE_DUPLICATING_PARAMETERS_TYPE 105
#define OGS_PFCP_ACTIVATE_PREDEFINED_RULES_TYPE 106
#define OGS_PFCP_DEACTIVATE_PREDEFINED_RULES_TYPE 107
#define OGS_PFCP_FAR_ID_TYPE 108
#define OGS_PFCP_QER_ID_TYPE 109
#define OGS_PFCP_OCI_FLAGS_TYPE 110
#define OGS_PFCP_PFCP_ASSOCIATION_RELEASE_REQUEST_TYPE 111
#define OGS_PFCP_GRACEFUL_RELEASE_PERIOD_TYPE 112
#define OGS_PFCP_PDN_TYPE_TYPE 113
#define OGS_PFCP_FAILED_RULE_ID_TYPE 114
#define OGS_PFCP_TIME_QUOTA_MECHANISM_TYPE 115
#define OGS_PFCP_USER_PLANE_IP_RESOURCE_INFORMATION_TYPE 116
#define OGS_PFCP_USER_PLANE_INACTIVITY_TIMER_TYPE 117
#define OGS_PFCP_AGGREGATED_URRS_TYPE 118
#define OGS_PFCP_MULTIPLIER_TYPE 119
#define OGS_PFCP_AGGREGATED_URR_ID_TYPE 120
#define OGS_PFCP_SUBSEQUENT_VOLUME_QUOTA_TYPE 121
#define OGS_PFCP_SUBSEQUENT_TIME_QUOTA_TYPE 122
#define OGS_PFCP_RQI_TYPE 123
#define OGS_PFCP_QFI_TYPE 124
#define OGS_PFCP_QUERY_URR_REFERENCE_TYPE 125
#define OGS_PFCP_ADDITIONAL_USAGE_REPORTS_INFORMATION_TYPE 126
#define OGS_PFCP_CREATE_TRAFFIC_ENDPOINT_TYPE 127
#define OGS_PFCP_CREATED_TRAFFIC_ENDPOINT_TYPE 128
#define OGS_PFCP_UPDATE_TRAFFIC_ENDPOINT_TYPE 129
#define OGS_PFCP_REMOVE_TRAFFIC_ENDPOINT_TYPE 130
#define OGS_PFCP_TRAFFIC_ENDPOINT_ID_TYPE 131
#define OGS_PFCP_ETHERNET_PACKET_FILTER_TYPE 132
#define OGS_PFCP_MAC_ADDRESS_TYPE 133
#define OGS_PFCP_C_TAG_TYPE 134
#define OGS_PFCP_S_TAG_TYPE 135
#define OGS_PFCP_ETHERTYPE_TYPE 136
#define OGS_PFCP_PROXYING_TYPE 137
#define OGS_PFCP_ETHERNET_FILTER_ID_TYPE 138
#define OGS_PFCP_ETHERNET_FILTER_PROPERTIES_TYPE 139
#define OGS_PFCP_SUGGESTED_BUFFERING_PACKETS_COUNT_TYPE 140
#define OGS_PFCP_USER_ID_TYPE 141
#define OGS_PFCP_ETHERNET_PDU_SESSION_INFORMATION_TYPE 142
#define OGS_PFCP_ETHERNET_TRAFFIC_INFORMATION_TYPE 143
#define OGS_PFCP_MAC_ADDRESSES_DETECTED_TYPE 144
#define OGS_PFCP_MAC_ADDRESSES_REMOVED_TYPE 145
#define OGS_PFCP_ETHERNET_INACTIVITY_TIMER_TYPE 146
#define OGS_PFCP_ADDITIONAL_MONITORING_TIME_TYPE 147
#define OGS_PFCP_EVENT_QUOTA_TYPE 148
#define OGS_PFCP_EVENT_THRESHOLD_TYPE 149
#define OGS_PFCP_SUBSEQUENT_EVENT_QUOTA_TYPE 150
#define OGS_PFCP_SUBSEQUENT_EVENT_THRESHOLD_TYPE 151
#define OGS_PFCP_TRACE_INFORMATION_TYPE 152
#define OGS_PFCP_FRAMED_ROUTE_TYPE 153
#define OGS_PFCP_FRAMED_ROUTING_TYPE 154
#define OGS_PFCP_FRAMED_IPV6_ROUTE_TYPE 155
#define OGS_PFCP_EVENT_TIME_STAMP_TYPE 156
#define OGS_PFCP_AVERAGING_WINDOW_TYPE 157
#define OGS_PFCP_PAGING_POLICY_INDICATOR_TYPE 158
#define OGS_PFCP_APN_DNN_TYPE 159
#define OGS_PFCP__INTERFACE_TYPE_TYPE 160
#define OGS_PFCP_PFCPSRREQ_FLAGS_TYPE 161
#define OGS_PFCP_PFCPAUREQ_FLAGS_TYPE 162
#define OGS_PFCP_ACTIVATION_TIME_TYPE 163
#define OGS_PFCP_DEACTIVATION_TIME_TYPE 164
#define OGS_PFCP_CREATE_MAR_TYPE 165
#define OGS_PFCP_ACCESS_FORWARDING_ACTION_INFORMATION_1_TYPE 166
#define OGS_PFCP_ACCESS_FORWARDING_ACTION_INFORMATION_2_TYPE 167
#define OGS_PFCP_REMOVE_MAR_TYPE 168
#define OGS_PFCP_UPDATE_MAR_TYPE 169
#define OGS_PFCP_MAR_ID_TYPE 170
#define OGS_PFCP_STEERING_FUNCTIONALITY_TYPE 171
#define OGS_PFCP_STEERING_MODE_TYPE 172
#define OGS_PFCP_WEIGHT_TYPE 173
#define OGS_PFCP_PRIORITY_TYPE 174
#define OGS_PFCP_UPDATE_ACCESS_FORWARDING_ACTION_INFORMATION_1_TYPE 175
#define OGS_PFCP_UPDATE_ACCESS_FORWARDING_ACTION_INFORMATION_2_TYPE 176
#define OGS_PFCP_UE_IP_ADDRESS_POOL_IDENTITY_TYPE 177
#define OGS_PFCP_ALTERNATIVE_SMF_IP_ADDRESS_TYPE 178
#define OGS_PFCP_PACKET_REPLICATION_AND_DETECTION_CARRY_ON_INFORMATION_TYPE 179
#define OGS_PFCP_SMF_SET_ID_TYPE 180
#define OGS_PFCP_QUOTA_VALIDITY_TIME_TYPE 181

/* Information Element TLV Descriptor */
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_cause;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_source_interface;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_f_teid;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_network_instance;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_sdf_filter;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_application_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_gate_status;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_mbr;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_gbr;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_qer_correlation_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_precedence;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_transport_level_marking;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_volume_threshold;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_threshold;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_monitoring_time;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_volume_threshold;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_time_threshold;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_inactivity_detection_time;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_reporting_triggers;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_redirect_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_report_type;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_offending_ie;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_forwarding_policy;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_destination_interface;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_up_function_features;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_apply_action;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_downlink_data_service_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_downlink_data_notification_delay;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_dl_buffering_duration;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_dl_buffering_suggested_packet_count;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpsmreq_flags;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpsrrsp_flags;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_sequence_number;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_metric;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_timer;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_pdr_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_f_seid;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_node_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfd_contents;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_measurement_method;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_report_trigger;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_measurement_period;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_fq_csid;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_volume_measurement;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_duration_measurement;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_of_first_packet;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_of_last_packet;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_quota_holding_time;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_dropped_dl_traffic_threshold;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_volume_quota;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_quota;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_start_time;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_end_time;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_urr_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_linked_urr_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_outer_header_creation;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_bar_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_cp_function_features;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_application_instance_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_flow_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_ue_ip_address;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_packet_rate;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_outer_header_removal;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_recovery_time_stamp;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_dl_flow_level_marking;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_header_enrichment;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_measurement_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_node_report_type;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_remote_gtp_u_peer;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_ur_seqn;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_activate_predefined_rules;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_deactivate_predefined_rules;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_far_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_qer_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_oci_flags;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_association_release_request;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_graceful_release_period;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_pdn_type;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_failed_rule_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_quota_mechanism;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_user_plane_ip_resource_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_user_plane_inactivity_timer;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_aggregated_urrs;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_multiplier;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_aggregated_urr_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_volume_quota;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_time_quota;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_rqi;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_qfi;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_query_urr_reference;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_additional_usage_reports_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_traffic_endpoint;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_traffic_endpoint_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_mac_address;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_c_tag;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_s_tag;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethertype;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_proxying;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethernet_filter_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethernet_filter_properties;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_suggested_buffering_packets_count;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_user_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethernet_pdu_session_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_mac_addresses_detected;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_mac_addresses_removed;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethernet_inactivity_timer;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_additional_monitoring_time;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_event_quota;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_event_threshold;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_event_quota;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_event_threshold;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_trace_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_framed_route;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_framed_routing;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_framed_ipv6_route;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_event_time_stamp;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_averaging_window;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_paging_policy_indicator;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_apn_dnn;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc__interface_type;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpsrreq_flags;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpaureq_flags;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_activation_time;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_deactivation_time;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_mar_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_steering_functionality;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_steering_mode;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_weight;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_priority;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_ue_ip_address_pool_identity;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_alternative_smf_ip_address;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_packet_replication_and_detection_carry_on_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_smf_set_id;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_quota_validity_time;

/* Group Information Element TLV Descriptor */
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethernet_packet_filter;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_pdi;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_pdr;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_forwarding_parameters;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_duplicating_parameters;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_far;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_forwarding_parameters;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_duplicating_parameters;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_far;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfd_context;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_application_id_s_pfds;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethernet_traffic_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_access_forwarding_action_information_1;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_access_forwarding_action_information_2;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_access_forwarding_action_information_1;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_access_forwarding_action_information_2;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_urr;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_qer;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_created_pdr;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_pdr;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_bar_pfcp_session_report_response;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_urr;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_qer;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_pdr;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_far;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_urr;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_qer;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_load_control_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_overload_control_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_application_detection_information;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_query_urr;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_report_session_modification_response;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_report_session_deletion_response;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_report_session_report_request;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_downlink_data_report;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_bar;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_bar_session_modification_request;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_bar;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_error_indication_report;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_user_plane_path_failure_report;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_traffic_endpoint;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_created_traffic_endpoint;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_traffic_endpoint;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_mar;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_mar;
extern ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_mar;

/* Message Descriptor */
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_heartbeat_request;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_heartbeat_response;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_pfd_management_request;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_pfd_management_response;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_association_setup_request;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_association_setup_response;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_association_update_request;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_association_update_response;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_association_release_request;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_association_release_response;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_version_not_supported_response;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_node_report_request;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_node_report_response;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_set_deletion_request;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_set_deletion_response;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_establishment_request;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_establishment_response;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_modification_request;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_modification_response;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_deletion_request;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_deletion_response;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_report_request;
extern ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_report_response;

/* Structure for Information Element */
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_cause_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_source_interface_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_f_teid_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_network_instance_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_sdf_filter_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_application_id_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_gate_status_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_mbr_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_gbr_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_qer_correlation_id_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_precedence_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_transport_level_marking_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_volume_threshold_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_time_threshold_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_monitoring_time_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_subsequent_volume_threshold_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_subsequent_time_threshold_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_inactivity_detection_time_t;
typedef ogs_tlv_uint24_t ogs_pfcp_tlv_reporting_triggers_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_redirect_information_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_report_type_t;
typedef ogs_tlv_uint16_t ogs_pfcp_tlv_offending_ie_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_forwarding_policy_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_destination_interface_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_up_function_features_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_apply_action_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_downlink_data_service_information_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_downlink_data_notification_delay_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_dl_buffering_duration_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_dl_buffering_suggested_packet_count_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_pfcpsmreq_flags_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_pfcpsrrsp_flags_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_sequence_number_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_metric_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_timer_t;
typedef ogs_tlv_uint16_t ogs_pfcp_tlv_pdr_id_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_f_seid_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_node_id_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_pfd_contents_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_measurement_method_t;
typedef ogs_tlv_uint24_t ogs_pfcp_tlv_usage_report_trigger_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_measurement_period_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_fq_csid_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_volume_measurement_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_duration_measurement_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_time_of_first_packet_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_time_of_last_packet_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_quota_holding_time_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_dropped_dl_traffic_threshold_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_volume_quota_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_time_quota_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_start_time_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_end_time_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_urr_id_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_linked_urr_id_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_outer_header_creation_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_bar_id_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_cp_function_features_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_usage_information_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_application_instance_id_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_flow_information_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_ue_ip_address_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_packet_rate_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_outer_header_removal_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_recovery_time_stamp_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_dl_flow_level_marking_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_header_enrichment_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_measurement_information_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_node_report_type_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_remote_gtp_u_peer_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_ur_seqn_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_activate_predefined_rules_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_deactivate_predefined_rules_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_far_id_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_qer_id_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_oci_flags_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_pfcp_association_release_request_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_graceful_release_period_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_pdn_type_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_failed_rule_id_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_time_quota_mechanism_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_user_plane_ip_resource_information_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_user_plane_inactivity_timer_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_aggregated_urrs_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_multiplier_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_aggregated_urr_id_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_subsequent_volume_quota_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_subsequent_time_quota_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_rqi_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_qfi_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_query_urr_reference_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_additional_usage_reports_information_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_update_traffic_endpoint_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_traffic_endpoint_id_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_mac_address_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_c_tag_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_s_tag_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_ethertype_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_proxying_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_ethernet_filter_id_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_ethernet_filter_properties_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_suggested_buffering_packets_count_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_user_id_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_ethernet_pdu_session_information_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_mac_addresses_detected_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_mac_addresses_removed_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_ethernet_inactivity_timer_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_additional_monitoring_time_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_event_quota_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_event_threshold_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_subsequent_event_quota_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_subsequent_event_threshold_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_trace_information_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_framed_route_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_framed_routing_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_framed_ipv6_route_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_event_time_stamp_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_averaging_window_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_paging_policy_indicator_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_apn_dnn_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv__interface_type_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_pfcpsrreq_flags_t;
typedef ogs_tlv_uint8_t ogs_pfcp_tlv_pfcpaureq_flags_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_activation_time_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_deactivation_time_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_mar_id_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_steering_functionality_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_steering_mode_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_weight_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_priority_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_ue_ip_address_pool_identity_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_alternative_smf_ip_address_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_packet_replication_and_detection_carry_on_information_t;
typedef ogs_tlv_octet_t ogs_pfcp_tlv_smf_set_id_t;
typedef ogs_tlv_uint32_t ogs_pfcp_tlv_quota_validity_time_t;

/* Structure for Group Information Element */
typedef struct ogs_pfcp_tlv_ethernet_packet_filter_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_ethernet_filter_id_t ethernet_filter_id;
    ogs_pfcp_tlv_ethernet_filter_properties_t ethernet_filter_properties;
    ogs_pfcp_tlv_mac_address_t mac_address;
    ogs_pfcp_tlv_ethertype_t ethertype;
    ogs_pfcp_tlv_c_tag_t c_tag;
    ogs_pfcp_tlv_s_tag_t s_tag;
    ogs_pfcp_tlv_sdf_filter_t sdf_filter[8];
} ogs_pfcp_tlv_ethernet_packet_filter_t;

typedef struct ogs_pfcp_tlv_pdi_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_source_interface_t source_interface;
    ogs_pfcp_tlv_f_teid_t local_f_teid;
    ogs_pfcp_tlv_network_instance_t network_instance;
    ogs_pfcp_tlv_ue_ip_address_t ue_ip_address;
    ogs_pfcp_tlv_traffic_endpoint_id_t traffic_endpoint_id;
    ogs_pfcp_tlv_sdf_filter_t sdf_filter[8];
    ogs_pfcp_tlv_application_id_t application_id;
    ogs_pfcp_tlv_ethernet_pdu_session_information_t ethernet_pdu_session_information;
    ogs_pfcp_tlv_ethernet_packet_filter_t ethernet_packet_filter;
    ogs_pfcp_tlv_qfi_t qfi;
    ogs_pfcp_tlv_framed_route_t framed_route;
    ogs_pfcp_tlv_framed_routing_t framed_routing;
    ogs_pfcp_tlv_framed_ipv6_route_t framed_ipv6_route;
    ogs_pfcp_tlv__interface_type_t source_interface_type;
} ogs_pfcp_tlv_pdi_t;

typedef struct ogs_pfcp_tlv_create_pdr_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_pdr_id_t pdr_id;
    ogs_pfcp_tlv_precedence_t precedence;
    ogs_pfcp_tlv_pdi_t pdi;
    ogs_pfcp_tlv_outer_header_removal_t outer_header_removal;
    ogs_pfcp_tlv_far_id_t far_id;
    ogs_pfcp_tlv_urr_id_t urr_id[8];
    ogs_pfcp_tlv_qer_id_t qer_id;
    ogs_pfcp_tlv_activate_predefined_rules_t activate_predefined_rules;
    ogs_pfcp_tlv_activation_time_t activation_time;
    ogs_pfcp_tlv_deactivation_time_t deactivation_time;
    ogs_pfcp_tlv_mar_id_t mar_id;
    ogs_pfcp_tlv_packet_replication_and_detection_carry_on_information_t packet_replication_and_detection_carry_on_information;
} ogs_pfcp_tlv_create_pdr_t;

typedef struct ogs_pfcp_tlv_forwarding_parameters_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_destination_interface_t destination_interface;
    ogs_pfcp_tlv_network_instance_t network_instance;
    ogs_pfcp_tlv_redirect_information_t redirect_information;
    ogs_pfcp_tlv_outer_header_creation_t outer_header_creation;
    ogs_pfcp_tlv_transport_level_marking_t transport_level_marking;
    ogs_pfcp_tlv_forwarding_policy_t forwarding_policy;
    ogs_pfcp_tlv_header_enrichment_t header_enrichment;
    ogs_pfcp_tlv_traffic_endpoint_id_t linked_traffic_endpoint_id;
    ogs_pfcp_tlv_proxying_t proxying;
    ogs_pfcp_tlv__interface_type_t destination_interface_type;
} ogs_pfcp_tlv_forwarding_parameters_t;

typedef struct ogs_pfcp_tlv_duplicating_parameters_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_destination_interface_t destination_interface;
    ogs_pfcp_tlv_outer_header_creation_t outer_header_creation;
    ogs_pfcp_tlv_transport_level_marking_t transport_level_marking;
    ogs_pfcp_tlv_forwarding_policy_t forwarding_policy;
} ogs_pfcp_tlv_duplicating_parameters_t;

typedef struct ogs_pfcp_tlv_create_far_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_far_id_t far_id;
    ogs_pfcp_tlv_apply_action_t apply_action;
    ogs_pfcp_tlv_forwarding_parameters_t forwarding_parameters;
    ogs_pfcp_tlv_duplicating_parameters_t duplicating_parameters;
    ogs_pfcp_tlv_bar_id_t bar_id;
} ogs_pfcp_tlv_create_far_t;

typedef struct ogs_pfcp_tlv_update_forwarding_parameters_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_destination_interface_t destination_interface;
    ogs_pfcp_tlv_network_instance_t network_instance;
    ogs_pfcp_tlv_redirect_information_t redirect_information;
    ogs_pfcp_tlv_outer_header_creation_t outer_header_creation;
    ogs_pfcp_tlv_transport_level_marking_t transport_level_marking;
    ogs_pfcp_tlv_forwarding_policy_t forwarding_policy;
    ogs_pfcp_tlv_header_enrichment_t header_enrichment;
    ogs_pfcp_tlv_pfcpsmreq_flags_t pfcpsmreq_flags;
    ogs_pfcp_tlv_traffic_endpoint_id_t linked_traffic_endpoint_id;
    ogs_pfcp_tlv__interface_type_t destination_interface_type;
} ogs_pfcp_tlv_update_forwarding_parameters_t;

typedef struct ogs_pfcp_tlv_update_duplicating_parameters_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_destination_interface_t destination_interface;
    ogs_pfcp_tlv_outer_header_creation_t outer_header_creation;
    ogs_pfcp_tlv_transport_level_marking_t transport_level_marking;
    ogs_pfcp_tlv_forwarding_policy_t forwarding_policy;
} ogs_pfcp_tlv_update_duplicating_parameters_t;

typedef struct ogs_pfcp_tlv_update_far_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_far_id_t far_id;
    ogs_pfcp_tlv_apply_action_t apply_action;
    ogs_pfcp_tlv_update_forwarding_parameters_t update_forwarding_parameters;
    ogs_pfcp_tlv_update_duplicating_parameters_t update_duplicating_parameters;
    ogs_pfcp_tlv_bar_id_t bar_id;
} ogs_pfcp_tlv_update_far_t;

typedef struct ogs_pfcp_tlv_pfd_context_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_pfd_contents_t pfd_contents;
} ogs_pfcp_tlv_pfd_context_t;

typedef struct ogs_pfcp_tlv_application_id_s_pfds_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_application_id_t application_id;
    ogs_pfcp_tlv_pfd_context_t pfd_context;
} ogs_pfcp_tlv_application_id_s_pfds_t;

typedef struct ogs_pfcp_tlv_ethernet_traffic_information_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_mac_addresses_detected_t mac_addresses_detected;
    ogs_pfcp_tlv_mac_addresses_removed_t mac_addresses_removed;
} ogs_pfcp_tlv_ethernet_traffic_information_t;

typedef struct ogs_pfcp_tlv_access_forwarding_action_information_1_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_far_id_t far_id;
    ogs_pfcp_tlv_weight_t weight;
    ogs_pfcp_tlv_priority_t priority;
    ogs_pfcp_tlv_urr_id_t urr_id[8];
} ogs_pfcp_tlv_access_forwarding_action_information_1_t;

typedef struct ogs_pfcp_tlv_access_forwarding_action_information_2_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_far_id_t far_id;
    ogs_pfcp_tlv_weight_t weight;
    ogs_pfcp_tlv_priority_t priority;
    ogs_pfcp_tlv_urr_id_t urr_id[8];
} ogs_pfcp_tlv_access_forwarding_action_information_2_t;

typedef struct ogs_pfcp_tlv_update_access_forwarding_action_information_1_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_far_id_t far_id;
    ogs_pfcp_tlv_weight_t weight;
    ogs_pfcp_tlv_priority_t priority;
    ogs_pfcp_tlv_urr_id_t urr_id;
} ogs_pfcp_tlv_update_access_forwarding_action_information_1_t;

typedef struct ogs_pfcp_tlv_update_access_forwarding_action_information_2_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_far_id_t far_id;
    ogs_pfcp_tlv_weight_t weight;
    ogs_pfcp_tlv_priority_t priority;
    ogs_pfcp_tlv_urr_id_t urr_id;
} ogs_pfcp_tlv_update_access_forwarding_action_information_2_t;

typedef struct ogs_pfcp_tlv_create_urr_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_urr_id_t urr_id;
    ogs_pfcp_tlv_measurement_method_t measurement_method;
    ogs_pfcp_tlv_reporting_triggers_t reporting_triggers;
    ogs_pfcp_tlv_measurement_period_t measurement_period;
    ogs_pfcp_tlv_volume_threshold_t volume_threshold;
    ogs_pfcp_tlv_volume_quota_t volume_quota;
    ogs_pfcp_tlv_event_threshold_t event_threshold;
    ogs_pfcp_tlv_event_quota_t event_quota;
    ogs_pfcp_tlv_time_threshold_t time_threshold;
    ogs_pfcp_tlv_time_quota_t time_quota;
    ogs_pfcp_tlv_quota_holding_time_t quota_holding_time;
    ogs_pfcp_tlv_dropped_dl_traffic_threshold_t dropped_dl_traffic_threshold;
    ogs_pfcp_tlv_quota_validity_time_t quota_validity_time;
    ogs_pfcp_tlv_monitoring_time_t monitoring_time;
    ogs_pfcp_tlv_subsequent_volume_threshold_t subsequent_volume_threshold;
    ogs_pfcp_tlv_subsequent_time_threshold_t subsequent_time_threshold;
    ogs_pfcp_tlv_subsequent_volume_quota_t subsequent_volume_quota;
    ogs_pfcp_tlv_subsequent_time_quota_t subsequent_time_quota;
    ogs_pfcp_tlv_subsequent_event_threshold_t subsequent_event_threshold;
    ogs_pfcp_tlv_subsequent_event_quota_t subsequent_event_quota;
    ogs_pfcp_tlv_inactivity_detection_time_t inactivity_detection_time;
    ogs_pfcp_tlv_linked_urr_id_t linked_urr_id;
    ogs_pfcp_tlv_measurement_information_t measurement_information;
    ogs_pfcp_tlv_time_quota_mechanism_t time_quota_mechanism;
    ogs_pfcp_tlv_aggregated_urrs_t aggregated_urrs;
    ogs_pfcp_tlv_far_id_t far_id_for_quota_action;
    ogs_pfcp_tlv_ethernet_inactivity_timer_t ethernet_inactivity_timer;
    ogs_pfcp_tlv_additional_monitoring_time_t additional_monitoring_time;
} ogs_pfcp_tlv_create_urr_t;

typedef struct ogs_pfcp_tlv_create_qer_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_qer_id_t qer_id;
    ogs_pfcp_tlv_qer_correlation_id_t qer_correlation_id;
    ogs_pfcp_tlv_gate_status_t gate_status;
    ogs_pfcp_tlv_mbr_t maximum_bitrate;
    ogs_pfcp_tlv_gbr_t guaranteed_bitrate;
    ogs_pfcp_tlv_packet_rate_t packet_rate;
    ogs_pfcp_tlv_dl_flow_level_marking_t dl_flow_level_marking;
    ogs_pfcp_tlv_qfi_t qos_flow_identifier;
    ogs_pfcp_tlv_rqi_t reflective_qos;
    ogs_pfcp_tlv_paging_policy_indicator_t paging_policy_indicator;
    ogs_pfcp_tlv_averaging_window_t averaging_window;
} ogs_pfcp_tlv_create_qer_t;

typedef struct ogs_pfcp_tlv_created_pdr_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_pdr_id_t pdr_id;
    ogs_pfcp_tlv_f_teid_t local_f_teid;
    ogs_pfcp_tlv_ue_ip_address_t ue_ip_address;
} ogs_pfcp_tlv_created_pdr_t;

typedef struct ogs_pfcp_tlv_update_pdr_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_pdr_id_t pdr_id;
    ogs_pfcp_tlv_outer_header_removal_t outer_header_removal;
    ogs_pfcp_tlv_precedence_t precedence;
    ogs_pfcp_tlv_pdi_t pdi;
    ogs_pfcp_tlv_far_id_t far_id;
    ogs_pfcp_tlv_urr_id_t urr_id;
    ogs_pfcp_tlv_qer_id_t qer_id;
    ogs_pfcp_tlv_activate_predefined_rules_t activate_predefined_rules;
    ogs_pfcp_tlv_deactivate_predefined_rules_t deactivate_predefined_rules;
    ogs_pfcp_tlv_activation_time_t activation_time;
    ogs_pfcp_tlv_deactivation_time_t deactivation_time;
} ogs_pfcp_tlv_update_pdr_t;

typedef struct ogs_pfcp_tlv_update_bar_pfcp_session_report_response_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_bar_id_t bar_id;
    ogs_pfcp_tlv_downlink_data_notification_delay_t downlink_data_notification_delay;
    ogs_pfcp_tlv_dl_buffering_duration_t dl_buffering_duration;
    ogs_pfcp_tlv_dl_buffering_suggested_packet_count_t dl_buffering_suggested_packet_count;
    ogs_pfcp_tlv_suggested_buffering_packets_count_t suggested_buffering_packets_count;
} ogs_pfcp_tlv_update_bar_pfcp_session_report_response_t;

typedef struct ogs_pfcp_tlv_update_urr_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_urr_id_t urr_id;
    ogs_pfcp_tlv_measurement_method_t measurement_method;
    ogs_pfcp_tlv_reporting_triggers_t reporting_triggers;
    ogs_pfcp_tlv_measurement_period_t measurement_period;
    ogs_pfcp_tlv_volume_threshold_t volume_threshold;
    ogs_pfcp_tlv_volume_quota_t volume_quota;
    ogs_pfcp_tlv_time_threshold_t time_threshold;
    ogs_pfcp_tlv_time_quota_t time_quota;
    ogs_pfcp_tlv_event_threshold_t event_threshold;
    ogs_pfcp_tlv_event_quota_t event_quota;
    ogs_pfcp_tlv_quota_holding_time_t quota_holding_time;
    ogs_pfcp_tlv_dropped_dl_traffic_threshold_t dropped_dl_traffic_threshold;
    ogs_pfcp_tlv_quota_validity_time_t quota_validity_time;
    ogs_pfcp_tlv_monitoring_time_t monitoring_time;
    ogs_pfcp_tlv_subsequent_volume_threshold_t subsequent_volume_threshold;
    ogs_pfcp_tlv_subsequent_time_threshold_t subsequent_time_threshold;
    ogs_pfcp_tlv_subsequent_volume_quota_t subsequent_volume_quota;
    ogs_pfcp_tlv_subsequent_time_quota_t subsequent_time_quota;
    ogs_pfcp_tlv_subsequent_event_threshold_t subsequent_event_threshold;
    ogs_pfcp_tlv_subsequent_event_quota_t subsequent_event_quota;
    ogs_pfcp_tlv_inactivity_detection_time_t inactivity_detection_time;
    ogs_pfcp_tlv_linked_urr_id_t linked_urr_id;
    ogs_pfcp_tlv_measurement_information_t measurement_information;
    ogs_pfcp_tlv_time_quota_mechanism_t time_quota_mechanism;
    ogs_pfcp_tlv_aggregated_urrs_t aggregated_urrs;
    ogs_pfcp_tlv_far_id_t far_id_for_quota_action;
    ogs_pfcp_tlv_ethernet_inactivity_timer_t ethernet_inactivity_timer;
    ogs_pfcp_tlv_additional_monitoring_time_t additional_monitoring_time;
} ogs_pfcp_tlv_update_urr_t;

typedef struct ogs_pfcp_tlv_update_qer_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_qer_id_t qer_id;
    ogs_pfcp_tlv_qer_correlation_id_t qer_correlation_id;
    ogs_pfcp_tlv_gate_status_t gate_status;
    ogs_pfcp_tlv_mbr_t maximum_bitrate;
    ogs_pfcp_tlv_gbr_t guaranteed_bitrate;
    ogs_pfcp_tlv_packet_rate_t packet_rate;
    ogs_pfcp_tlv_dl_flow_level_marking_t dl_flow_level_marking;
    ogs_pfcp_tlv_qfi_t qos_flow_identifier;
    ogs_pfcp_tlv_rqi_t reflective_qos;
    ogs_pfcp_tlv_paging_policy_indicator_t paging_policy_indicator;
    ogs_pfcp_tlv_averaging_window_t averaging_window;
} ogs_pfcp_tlv_update_qer_t;

typedef struct ogs_pfcp_tlv_remove_pdr_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_pdr_id_t pdr_id;
} ogs_pfcp_tlv_remove_pdr_t;

typedef struct ogs_pfcp_tlv_remove_far_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_far_id_t far_id;
} ogs_pfcp_tlv_remove_far_t;

typedef struct ogs_pfcp_tlv_remove_urr_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_urr_id_t urr_id;
} ogs_pfcp_tlv_remove_urr_t;

typedef struct ogs_pfcp_tlv_remove_qer_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_qer_id_t qer_id;
} ogs_pfcp_tlv_remove_qer_t;

typedef struct ogs_pfcp_tlv_load_control_information_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_sequence_number_t load_control_sequence_number;
    ogs_pfcp_tlv_metric_t load_metric;
} ogs_pfcp_tlv_load_control_information_t;

typedef struct ogs_pfcp_tlv_overload_control_information_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_sequence_number_t overload_control_sequence_number;
    ogs_pfcp_tlv_metric_t overload_reduction_metric;
    ogs_pfcp_tlv_timer_t period_of_validity;
    ogs_pfcp_tlv_oci_flags_t overload_control_information_flags;
} ogs_pfcp_tlv_overload_control_information_t;

typedef struct ogs_pfcp_tlv_application_detection_information_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_application_id_t application_id;
    ogs_pfcp_tlv_application_instance_id_t application_instance_id;
    ogs_pfcp_tlv_flow_information_t flow_information;
} ogs_pfcp_tlv_application_detection_information_t;

typedef struct ogs_pfcp_tlv_query_urr_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_urr_id_t urr_id;
} ogs_pfcp_tlv_query_urr_t;

typedef struct ogs_pfcp_tlv_usage_report_session_modification_response_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_urr_id_t urr_id;
    ogs_pfcp_tlv_ur_seqn_t ur_seqn;
    ogs_pfcp_tlv_usage_report_trigger_t usage_report_trigger;
    ogs_pfcp_tlv_start_time_t start_time;
    ogs_pfcp_tlv_end_time_t end_time;
    ogs_pfcp_tlv_volume_measurement_t volume_measurement;
    ogs_pfcp_tlv_duration_measurement_t duration_measurement;
    ogs_pfcp_tlv_time_of_first_packet_t time_of_first_packet;
    ogs_pfcp_tlv_time_of_last_packet_t time_of_last_packet;
    ogs_pfcp_tlv_usage_information_t usage_information;
    ogs_pfcp_tlv_query_urr_reference_t query_urr_reference;
    ogs_pfcp_tlv_ethernet_traffic_information_t ethernet_traffic_information;
} ogs_pfcp_tlv_usage_report_session_modification_response_t;

typedef struct ogs_pfcp_tlv_usage_report_session_deletion_response_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_urr_id_t urr_id;
    ogs_pfcp_tlv_ur_seqn_t ur_seqn;
    ogs_pfcp_tlv_usage_report_trigger_t usage_report_trigger;
    ogs_pfcp_tlv_start_time_t start_time;
    ogs_pfcp_tlv_end_time_t end_time;
    ogs_pfcp_tlv_volume_measurement_t volume_measurement;
    ogs_pfcp_tlv_duration_measurement_t duration_measurement;
    ogs_pfcp_tlv_time_of_first_packet_t time_of_first_packet;
    ogs_pfcp_tlv_time_of_last_packet_t time_of_last_packet;
    ogs_pfcp_tlv_usage_information_t usage_information;
    ogs_pfcp_tlv_ethernet_traffic_information_t ethernet_traffic_information;
} ogs_pfcp_tlv_usage_report_session_deletion_response_t;

typedef struct ogs_pfcp_tlv_usage_report_session_report_request_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_urr_id_t urr_id;
    ogs_pfcp_tlv_ur_seqn_t ur_seqn;
    ogs_pfcp_tlv_usage_report_trigger_t usage_report_trigger;
    ogs_pfcp_tlv_start_time_t start_time;
    ogs_pfcp_tlv_end_time_t end_time;
    ogs_pfcp_tlv_volume_measurement_t volume_measurement;
    ogs_pfcp_tlv_duration_measurement_t duration_measurement;
    ogs_pfcp_tlv_application_detection_information_t application_detection_information;
    ogs_pfcp_tlv_ue_ip_address_t ue_ip_address;
    ogs_pfcp_tlv_network_instance_t network_instance;
    ogs_pfcp_tlv_time_of_first_packet_t time_of_first_packet;
    ogs_pfcp_tlv_time_of_last_packet_t time_of_last_packet;
    ogs_pfcp_tlv_usage_information_t usage_information;
    ogs_pfcp_tlv_query_urr_reference_t query_urr_reference;
    ogs_pfcp_tlv_event_time_stamp_t event_time_stamp;
    ogs_pfcp_tlv_ethernet_traffic_information_t ethernet_traffic_information;
} ogs_pfcp_tlv_usage_report_session_report_request_t;

typedef struct ogs_pfcp_tlv_downlink_data_report_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_pdr_id_t pdr_id;
    ogs_pfcp_tlv_downlink_data_service_information_t downlink_data_service_information;
} ogs_pfcp_tlv_downlink_data_report_t;

typedef struct ogs_pfcp_tlv_create_bar_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_bar_id_t bar_id;
    ogs_pfcp_tlv_downlink_data_notification_delay_t downlink_data_notification_delay;
    ogs_pfcp_tlv_suggested_buffering_packets_count_t suggested_buffering_packets_count;
} ogs_pfcp_tlv_create_bar_t;

typedef struct ogs_pfcp_tlv_update_bar_session_modification_request_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_bar_id_t bar_id;
    ogs_pfcp_tlv_downlink_data_notification_delay_t downlink_data_notification_delay;
    ogs_pfcp_tlv_suggested_buffering_packets_count_t suggested_buffering_packets_count;
} ogs_pfcp_tlv_update_bar_session_modification_request_t;

typedef struct ogs_pfcp_tlv_remove_bar_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_bar_id_t bar_id;
} ogs_pfcp_tlv_remove_bar_t;

typedef struct ogs_pfcp_tlv_error_indication_report_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_f_teid_t remote_f_teid;
} ogs_pfcp_tlv_error_indication_report_t;

typedef struct ogs_pfcp_tlv_user_plane_path_failure_report_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_remote_gtp_u_peer_t remote_gtp_u_peer_;
} ogs_pfcp_tlv_user_plane_path_failure_report_t;

typedef struct ogs_pfcp_tlv_create_traffic_endpoint_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_traffic_endpoint_id_t traffic_endpoint_id;
    ogs_pfcp_tlv_f_teid_t local_f_teid;
    ogs_pfcp_tlv_network_instance_t network_instance;
    ogs_pfcp_tlv_ue_ip_address_t ue_ip_address;
    ogs_pfcp_tlv_ethernet_pdu_session_information_t ethernet_pdu_session_information;
    ogs_pfcp_tlv_framed_route_t framed_route;
    ogs_pfcp_tlv_framed_routing_t framed_routing;
    ogs_pfcp_tlv_framed_ipv6_route_t framed_ipv6_route;
    ogs_pfcp_tlv_qfi_t qfi;
} ogs_pfcp_tlv_create_traffic_endpoint_t;

typedef struct ogs_pfcp_tlv_created_traffic_endpoint_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_traffic_endpoint_id_t traffic_endpoint_id;
    ogs_pfcp_tlv_f_teid_t local_f_teid;
    ogs_pfcp_tlv_ue_ip_address_t ue_ip_address;
} ogs_pfcp_tlv_created_traffic_endpoint_t;

typedef struct ogs_pfcp_tlv_remove_traffic_endpoint_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_traffic_endpoint_id_t traffic_endpoint_id;
} ogs_pfcp_tlv_remove_traffic_endpoint_t;

typedef struct ogs_pfcp_tlv_create_mar_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_mar_id_t mar_id;
    ogs_pfcp_tlv_steering_functionality_t steering_functionality;
    ogs_pfcp_tlv_steering_mode_t steering_mode;
    ogs_pfcp_tlv_access_forwarding_action_information_1_t access_forwarding_action_information_1;
    ogs_pfcp_tlv_access_forwarding_action_information_2_t access_forwarding_action_information_2;
} ogs_pfcp_tlv_create_mar_t;

typedef struct ogs_pfcp_tlv_remove_mar_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_mar_id_t mar_id;
} ogs_pfcp_tlv_remove_mar_t;

typedef struct ogs_pfcp_tlv_update_mar_s {
    ogs_tlv_presence_t presence;
    ogs_pfcp_tlv_mar_id_t mar_id;
    ogs_pfcp_tlv_steering_functionality_t steering_functionality;
    ogs_pfcp_tlv_steering_mode_t steering_mode;
    ogs_pfcp_tlv_update_access_forwarding_action_information_1_t update_access_forwarding_action_information_1;
    ogs_pfcp_tlv_update_access_forwarding_action_information_2_t update_access_forwarding_action_information_2;
    ogs_pfcp_tlv_access_forwarding_action_information_1_t access_forwarding_action_information_1;
    ogs_pfcp_tlv_access_forwarding_action_information_2_t access_forwarding_action_information_2;
} ogs_pfcp_tlv_update_mar_t;

/* Structure for Message */
typedef struct ogs_pfcp_heartbeat_request_s {
    ogs_pfcp_tlv_recovery_time_stamp_t recovery_time_stamp;
} ogs_pfcp_heartbeat_request_t;

typedef struct ogs_pfcp_heartbeat_response_s {
    ogs_pfcp_tlv_recovery_time_stamp_t recovery_time_stamp;
} ogs_pfcp_heartbeat_response_t;

typedef struct ogs_pfcp_pfd_management_request_s {
    ogs_pfcp_tlv_application_id_s_pfds_t application_id_s_pfds;
} ogs_pfcp_pfd_management_request_t;

typedef struct ogs_pfcp_pfd_management_response_s {
    ogs_pfcp_tlv_cause_t cause;
    ogs_pfcp_tlv_offending_ie_t offending_ie;
} ogs_pfcp_pfd_management_response_t;

typedef struct ogs_pfcp_association_setup_request_s {
    ogs_pfcp_tlv_node_id_t node_id;
    ogs_pfcp_tlv_recovery_time_stamp_t recovery_time_stamp;
    ogs_pfcp_tlv_up_function_features_t up_function_features;
    ogs_pfcp_tlv_cp_function_features_t cp_function_features;
    ogs_pfcp_tlv_user_plane_ip_resource_information_t user_plane_ip_resource_information[4];
    ogs_pfcp_tlv_ue_ip_address_t ue_ip_address_pool_identity;
    ogs_pfcp_tlv_alternative_smf_ip_address_t alternative_smf_ip_address;
    ogs_pfcp_tlv_smf_set_id_t smf_set_id;
} ogs_pfcp_association_setup_request_t;

typedef struct ogs_pfcp_association_setup_response_s {
    ogs_pfcp_tlv_node_id_t node_id;
    ogs_pfcp_tlv_cause_t cause;
    ogs_pfcp_tlv_recovery_time_stamp_t recovery_time_stamp;
    ogs_pfcp_tlv_up_function_features_t up_function_features;
    ogs_pfcp_tlv_cp_function_features_t cp_function_features;
    ogs_pfcp_tlv_user_plane_ip_resource_information_t user_plane_ip_resource_information[4];
    ogs_pfcp_tlv_alternative_smf_ip_address_t alternative_smf_ip_address;
} ogs_pfcp_association_setup_response_t;

typedef struct ogs_pfcp_association_update_request_s {
    ogs_pfcp_tlv_node_id_t node_id;
    ogs_pfcp_tlv_up_function_features_t up_function_features;
    ogs_pfcp_tlv_cp_function_features_t cp_function_features;
    ogs_pfcp_tlv_pfcp_association_release_request_t pfcp_association_release_request;
    ogs_pfcp_tlv_graceful_release_period_t graceful_release_period;
    ogs_pfcp_tlv_user_plane_ip_resource_information_t user_plane_ip_resource_information[4];
    ogs_pfcp_tlv_pfcpaureq_flags_t pfcpaureq_flags;
    ogs_pfcp_tlv_alternative_smf_ip_address_t alternative_smf_ip_address;
} ogs_pfcp_association_update_request_t;

typedef struct ogs_pfcp_association_update_response_s {
    ogs_pfcp_tlv_node_id_t node_id;
    ogs_pfcp_tlv_cause_t cause;
    ogs_pfcp_tlv_up_function_features_t up_function_features;
    ogs_pfcp_tlv_cp_function_features_t cp_function_features;
} ogs_pfcp_association_update_response_t;

typedef struct ogs_pfcp_association_release_request_s {
    ogs_pfcp_tlv_node_id_t node_id;
} ogs_pfcp_association_release_request_t;

typedef struct ogs_pfcp_association_release_response_s {
    ogs_pfcp_tlv_node_id_t node_id;
    ogs_pfcp_tlv_cause_t cause;
} ogs_pfcp_association_release_response_t;

typedef struct ogs_pfcp_version_not_supported_response_s {
} ogs_pfcp_version_not_supported_response_t;

typedef struct ogs_pfcp_node_report_request_s {
    ogs_pfcp_tlv_node_id_t node_id;
    ogs_pfcp_tlv_node_report_type_t node_report_type;
    ogs_pfcp_tlv_user_plane_path_failure_report_t user_plane_path_failure_report;
} ogs_pfcp_node_report_request_t;

typedef struct ogs_pfcp_node_report_response_s {
    ogs_pfcp_tlv_node_id_t node_id;
    ogs_pfcp_tlv_cause_t cause;
    ogs_pfcp_tlv_offending_ie_t offending_ie;
} ogs_pfcp_node_report_response_t;

typedef struct ogs_pfcp_session_set_deletion_request_s {
    ogs_pfcp_tlv_node_id_t node_id;
    ogs_pfcp_tlv_fq_csid_t sgw_c_fq_csid;
    ogs_pfcp_tlv_fq_csid_t pgw_c_fq_csid;
    ogs_pfcp_tlv_fq_csid_t sgw_u_fq_csid;
    ogs_pfcp_tlv_fq_csid_t pgw_u_fq_csid;
    ogs_pfcp_tlv_fq_csid_t twan_fq_csid;
    ogs_pfcp_tlv_fq_csid_t epdg_fq_csid;
    ogs_pfcp_tlv_fq_csid_t mme_fq_csid;
} ogs_pfcp_session_set_deletion_request_t;

typedef struct ogs_pfcp_session_set_deletion_response_s {
    ogs_pfcp_tlv_node_id_t node_id;
    ogs_pfcp_tlv_cause_t cause;
    ogs_pfcp_tlv_offending_ie_t offending_ie;
} ogs_pfcp_session_set_deletion_response_t;

typedef struct ogs_pfcp_session_establishment_request_s {
    ogs_pfcp_tlv_node_id_t node_id;
    ogs_pfcp_tlv_f_seid_t cp_f_seid;
    ogs_pfcp_tlv_create_pdr_t create_pdr[8];
    ogs_pfcp_tlv_create_far_t create_far[8];
    ogs_pfcp_tlv_create_urr_t create_urr[2];
    ogs_pfcp_tlv_create_qer_t create_qer[4];
    ogs_pfcp_tlv_create_bar_t create_bar;
    ogs_pfcp_tlv_create_traffic_endpoint_t create_traffic_endpoint;
    ogs_pfcp_tlv_pdn_type_t pdn_type;
    ogs_pfcp_tlv_fq_csid_t sgw_c_fq_csid;
    ogs_pfcp_tlv_fq_csid_t mme_fq_csid;
    ogs_pfcp_tlv_fq_csid_t pgw_c_fq_csid;
    ogs_pfcp_tlv_fq_csid_t epdg_fq_csid;
    ogs_pfcp_tlv_fq_csid_t twan_fq_csid;
    ogs_pfcp_tlv_user_plane_inactivity_timer_t user_plane_inactivity_timer;
    ogs_pfcp_tlv_user_id_t user_id;
    ogs_pfcp_tlv_trace_information_t trace_information;
    ogs_pfcp_tlv_apn_dnn_t apn_dnn;
    ogs_pfcp_tlv_create_mar_t create_mar;
} ogs_pfcp_session_establishment_request_t;

typedef struct ogs_pfcp_session_establishment_response_s {
    ogs_pfcp_tlv_node_id_t node_id;
    ogs_pfcp_tlv_cause_t cause;
    ogs_pfcp_tlv_offending_ie_t offending_ie;
    ogs_pfcp_tlv_f_seid_t up_f_seid;
    ogs_pfcp_tlv_created_pdr_t created_pdr[8];
    ogs_pfcp_tlv_load_control_information_t load_control_information;
    ogs_pfcp_tlv_overload_control_information_t overload_control_information;
    ogs_pfcp_tlv_fq_csid_t sgw_u_fq_csid;
    ogs_pfcp_tlv_fq_csid_t pgw_u_fq_csid;
    ogs_pfcp_tlv_failed_rule_id_t failed_rule_id;
    ogs_pfcp_tlv_created_traffic_endpoint_t created_traffic_endpoint;
} ogs_pfcp_session_establishment_response_t;

typedef struct ogs_pfcp_session_modification_request_s {
    ogs_pfcp_tlv_f_seid_t cp_f_seid;
    ogs_pfcp_tlv_remove_pdr_t remove_pdr[8];
    ogs_pfcp_tlv_remove_far_t remove_far[8];
    ogs_pfcp_tlv_remove_urr_t remove_urr[2];
    ogs_pfcp_tlv_remove_qer_t remove_qer[4];
    ogs_pfcp_tlv_remove_bar_t remove_bar;
    ogs_pfcp_tlv_remove_traffic_endpoint_t remove_traffic_endpoint;
    ogs_pfcp_tlv_create_pdr_t create_pdr[8];
    ogs_pfcp_tlv_create_far_t create_far[8];
    ogs_pfcp_tlv_create_urr_t create_urr[2];
    ogs_pfcp_tlv_create_qer_t create_qer[4];
    ogs_pfcp_tlv_create_bar_t create_bar;
    ogs_pfcp_tlv_create_traffic_endpoint_t create_traffic_endpoint;
    ogs_pfcp_tlv_update_pdr_t update_pdr[8];
    ogs_pfcp_tlv_update_far_t update_far[8];
    ogs_pfcp_tlv_update_urr_t update_urr[2];
    ogs_pfcp_tlv_update_qer_t update_qer[4];
    ogs_pfcp_tlv_update_bar_session_modification_request_t update_bar;
    ogs_pfcp_tlv_update_traffic_endpoint_t update_traffic_endpoint;
    ogs_pfcp_tlv_pfcpsmreq_flags_t pfcpsmreq_flags;
    ogs_pfcp_tlv_query_urr_t query_urr;
    ogs_pfcp_tlv_fq_csid_t pgw_c_fq_csid;
    ogs_pfcp_tlv_fq_csid_t sgw_c_fq_csid;
    ogs_pfcp_tlv_fq_csid_t mme_fq_csid;
    ogs_pfcp_tlv_fq_csid_t epdg_fq_csid;
    ogs_pfcp_tlv_fq_csid_t twan_fq_csid;
    ogs_pfcp_tlv_user_plane_inactivity_timer_t user_plane_inactivity_timer;
    ogs_pfcp_tlv_query_urr_reference_t query_urr_reference;
    ogs_pfcp_tlv_trace_information_t trace_information;
    ogs_pfcp_tlv_remove_mar_t remove_mar;
    ogs_pfcp_tlv_update_mar_t update_mar;
    ogs_pfcp_tlv_create_mar_t create_mar;
    ogs_pfcp_tlv_node_id_t node_id;
} ogs_pfcp_session_modification_request_t;

typedef struct ogs_pfcp_session_modification_response_s {
    ogs_pfcp_tlv_cause_t cause;
    ogs_pfcp_tlv_offending_ie_t offending_ie;
    ogs_pfcp_tlv_created_pdr_t created_pdr[8];
    ogs_pfcp_tlv_load_control_information_t load_control_information;
    ogs_pfcp_tlv_overload_control_information_t overload_control_information;
    ogs_pfcp_tlv_usage_report_session_modification_response_t usage_report[8];
    ogs_pfcp_tlv_failed_rule_id_t failed_rule_id;
    ogs_pfcp_tlv_additional_usage_reports_information_t additional_usage_reports_information;
    ogs_pfcp_tlv_created_traffic_endpoint_t created_updated_traffic_endpoint;
} ogs_pfcp_session_modification_response_t;

typedef struct ogs_pfcp_session_deletion_request_s {
} ogs_pfcp_session_deletion_request_t;

typedef struct ogs_pfcp_session_deletion_response_s {
    ogs_pfcp_tlv_cause_t cause;
    ogs_pfcp_tlv_offending_ie_t offending_ie;
    ogs_pfcp_tlv_load_control_information_t load_control_information;
    ogs_pfcp_tlv_overload_control_information_t overload_control_information;
    ogs_pfcp_tlv_usage_report_session_deletion_response_t usage_report[8];
} ogs_pfcp_session_deletion_response_t;

typedef struct ogs_pfcp_session_report_request_s {
    ogs_pfcp_tlv_report_type_t report_type;
    ogs_pfcp_tlv_downlink_data_report_t downlink_data_report;
    ogs_pfcp_tlv_usage_report_session_report_request_t usage_report[8];
    ogs_pfcp_tlv_error_indication_report_t error_indication_report;
    ogs_pfcp_tlv_load_control_information_t load_control_information;
    ogs_pfcp_tlv_overload_control_information_t overload_control_information;
    ogs_pfcp_tlv_additional_usage_reports_information_t additional_usage_reports_information;
    ogs_pfcp_tlv_pfcpsrreq_flags_t pfcpsrreq_flags;
    ogs_pfcp_tlv_f_seid_t old_cp_f_seid;
} ogs_pfcp_session_report_request_t;

typedef struct ogs_pfcp_session_report_response_s {
    ogs_pfcp_tlv_cause_t cause;
    ogs_pfcp_tlv_offending_ie_t offending_ie;
    ogs_pfcp_tlv_update_bar_pfcp_session_report_response_t update_bar;
    ogs_pfcp_tlv_pfcpsrrsp_flags_t pfcpsrrsp_flags;
    ogs_pfcp_tlv_f_seid_t cp_f_seid;
    ogs_pfcp_tlv_f_teid_t n4_u_f_teid;
    ogs_pfcp_tlv_alternative_smf_ip_address_t alternative_smf_ip_address;
} ogs_pfcp_session_report_response_t;

typedef struct ogs_pfcp_message_s {
   ogs_pfcp_header_t h;
   union {
        ogs_pfcp_heartbeat_request_t pfcp_heartbeat_request;
        ogs_pfcp_heartbeat_response_t pfcp_heartbeat_response;
        ogs_pfcp_pfd_management_request_t pfcp_pfd_management_request;
        ogs_pfcp_pfd_management_response_t pfcp_pfd_management_response;
        ogs_pfcp_association_setup_request_t pfcp_association_setup_request;
        ogs_pfcp_association_setup_response_t pfcp_association_setup_response;
        ogs_pfcp_association_update_request_t pfcp_association_update_request;
        ogs_pfcp_association_update_response_t pfcp_association_update_response;
        ogs_pfcp_association_release_request_t pfcp_association_release_request;
        ogs_pfcp_association_release_response_t pfcp_association_release_response;
        ogs_pfcp_version_not_supported_response_t pfcp_version_not_supported_response;
        ogs_pfcp_node_report_request_t pfcp_node_report_request;
        ogs_pfcp_node_report_response_t pfcp_node_report_response;
        ogs_pfcp_session_set_deletion_request_t pfcp_session_set_deletion_request;
        ogs_pfcp_session_set_deletion_response_t pfcp_session_set_deletion_response;
        ogs_pfcp_session_establishment_request_t pfcp_session_establishment_request;
        ogs_pfcp_session_establishment_response_t pfcp_session_establishment_response;
        ogs_pfcp_session_modification_request_t pfcp_session_modification_request;
        ogs_pfcp_session_modification_response_t pfcp_session_modification_response;
        ogs_pfcp_session_deletion_request_t pfcp_session_deletion_request;
        ogs_pfcp_session_deletion_response_t pfcp_session_deletion_response;
        ogs_pfcp_session_report_request_t pfcp_session_report_request;
        ogs_pfcp_session_report_response_t pfcp_session_report_response;
   };
} ogs_pfcp_message_t;

int ogs_pfcp_parse_msg(ogs_pfcp_message_t *pfcp_message, ogs_pkbuf_t *pkbuf);
ogs_pkbuf_t *ogs_pfcp_build_msg(ogs_pfcp_message_t *pfcp_message);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PFCP_MESSAGE_H */
