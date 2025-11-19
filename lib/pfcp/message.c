/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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
 * Created on: 2024-10-20 22:37:02.550243 by acetcom
 * from 29244-h71-modified.docx
 ******************************************************************************/

#include "ogs-pfcp.h"

ogs_tlv_desc_t ogs_pfcp_tlv_desc_cause =
{
    OGS_TLV_UINT8,
    "Cause",
    OGS_PFCP_CAUSE_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_cause_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_source_interface =
{
    OGS_TLV_UINT8,
    "Source Interface",
    OGS_PFCP_SOURCE_INTERFACE_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_source_interface_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_f_teid =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_PFCP_F_TEID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_network_instance =
{
    OGS_TLV_VAR_STR,
    "Network Instance",
    OGS_PFCP_NETWORK_INSTANCE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_network_instance_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_sdf_filter =
{
    OGS_TLV_VAR_STR,
    "SDF Filter",
    OGS_PFCP_SDF_FILTER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_sdf_filter_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_application_id =
{
    OGS_TLV_VAR_STR,
    "Application ID",
    OGS_PFCP_APPLICATION_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_application_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_gate_status =
{
    OGS_TLV_UINT8,
    "Gate Status",
    OGS_PFCP_GATE_STATUS_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_gate_status_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mbr =
{
    OGS_TLV_VAR_STR,
    "MBR",
    OGS_PFCP_MBR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mbr_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_gbr =
{
    OGS_TLV_VAR_STR,
    "GBR",
    OGS_PFCP_GBR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_gbr_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_qer_correlation_id =
{
    OGS_TLV_UINT32,
    "QER Correlation ID",
    OGS_PFCP_QER_CORRELATION_ID_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_qer_correlation_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_precedence =
{
    OGS_TLV_UINT32,
    "Precedence",
    OGS_PFCP_PRECEDENCE_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_precedence_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_transport_level_marking =
{
    OGS_TLV_VAR_STR,
    "Transport Level Marking",
    OGS_PFCP_TRANSPORT_LEVEL_MARKING_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_transport_level_marking_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_volume_threshold =
{
    OGS_TLV_VAR_STR,
    "Volume Threshold",
    OGS_PFCP_VOLUME_THRESHOLD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_volume_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_threshold =
{
    OGS_TLV_UINT32,
    "Time Threshold",
    OGS_PFCP_TIME_THRESHOLD_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_time_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_monitoring_time =
{
    OGS_TLV_VAR_STR,
    "Monitoring Time",
    OGS_PFCP_MONITORING_TIME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_monitoring_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_volume_threshold =
{
    OGS_TLV_VAR_STR,
    "Subsequent Volume Threshold",
    OGS_PFCP_SUBSEQUENT_VOLUME_THRESHOLD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_subsequent_volume_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_time_threshold =
{
    OGS_TLV_VAR_STR,
    "Subsequent Time Threshold",
    OGS_PFCP_SUBSEQUENT_TIME_THRESHOLD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_subsequent_time_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_inactivity_detection_time =
{
    OGS_TLV_VAR_STR,
    "Inactivity Detection Time",
    OGS_PFCP_INACTIVITY_DETECTION_TIME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_inactivity_detection_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_reporting_triggers =
{
    OGS_TLV_UINT24,
    "Reporting Triggers",
    OGS_PFCP_REPORTING_TRIGGERS_TYPE,
    3,
    0,
    sizeof(ogs_pfcp_tlv_reporting_triggers_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_redirect_information =
{
    OGS_TLV_VAR_STR,
    "Redirect Information",
    OGS_PFCP_REDIRECT_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_redirect_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_report_type =
{
    OGS_TLV_UINT8,
    "Report Type",
    OGS_PFCP_REPORT_TYPE_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_report_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_offending_ie =
{
    OGS_TLV_UINT16,
    "Offending IE",
    OGS_PFCP_OFFENDING_IE_TYPE,
    2,
    0,
    sizeof(ogs_pfcp_tlv_offending_ie_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_forwarding_policy =
{
    OGS_TLV_VAR_STR,
    "Forwarding Policy",
    OGS_PFCP_FORWARDING_POLICY_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_forwarding_policy_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_destination_interface =
{
    OGS_TLV_UINT8,
    "Destination Interface",
    OGS_PFCP_DESTINATION_INTERFACE_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_destination_interface_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_up_function_features =
{
    OGS_TLV_VAR_STR,
    "UP Function Features",
    OGS_PFCP_UP_FUNCTION_FEATURES_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_up_function_features_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_apply_action =
{
    OGS_TLV_UINT16,
    "Apply Action",
    OGS_PFCP_APPLY_ACTION_TYPE,
    2,
    0,
    sizeof(ogs_pfcp_tlv_apply_action_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_downlink_data_service_information =
{
    OGS_TLV_VAR_STR,
    "Downlink Data Service Information",
    OGS_PFCP_DOWNLINK_DATA_SERVICE_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_downlink_data_service_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_downlink_data_notification_delay =
{
    OGS_TLV_VAR_STR,
    "Downlink Data Notification Delay",
    OGS_PFCP_DOWNLINK_DATA_NOTIFICATION_DELAY_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_downlink_data_notification_delay_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_dl_buffering_duration =
{
    OGS_TLV_VAR_STR,
    "DL Buffering Duration",
    OGS_PFCP_DL_BUFFERING_DURATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_dl_buffering_duration_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_dl_buffering_suggested_packet_count =
{
    OGS_TLV_VAR_STR,
    "DL Buffering Suggested Packet Count",
    OGS_PFCP_DL_BUFFERING_SUGGESTED_PACKET_COUNT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_dl_buffering_suggested_packet_count_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpsmreq_flags =
{
    OGS_TLV_UINT8,
    "PFCPSMReq-Flags",
    OGS_PFCP_PFCPSMREQ_FLAGS_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_pfcpsmreq_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpsrrsp_flags =
{
    OGS_TLV_UINT8,
    "PFCPSRRsp-Flags",
    OGS_PFCP_PFCPSRRSP_FLAGS_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_pfcpsrrsp_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_sequence_number =
{
    OGS_TLV_VAR_STR,
    "Sequence Number",
    OGS_PFCP_SEQUENCE_NUMBER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_sequence_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_metric =
{
    OGS_TLV_VAR_STR,
    "Metric",
    OGS_PFCP_METRIC_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_metric_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_timer =
{
    OGS_TLV_VAR_STR,
    "Timer",
    OGS_PFCP_TIMER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_timer_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pdr_id =
{
    OGS_TLV_UINT16,
    "PDR ID",
    OGS_PFCP_PDR_ID_TYPE,
    2,
    0,
    sizeof(ogs_pfcp_tlv_pdr_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_f_seid =
{
    OGS_TLV_VAR_STR,
    "F-SEID",
    OGS_PFCP_F_SEID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_f_seid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_node_id =
{
    OGS_TLV_VAR_STR,
    "Node ID",
    OGS_PFCP_NODE_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_node_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfd_contents =
{
    OGS_TLV_VAR_STR,
    "PFD contents",
    OGS_PFCP_PFD_CONTENTS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pfd_contents_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_measurement_method =
{
    OGS_TLV_UINT8,
    "Measurement Method",
    OGS_PFCP_MEASUREMENT_METHOD_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_measurement_method_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_report_trigger =
{
    OGS_TLV_UINT24,
    "Usage Report Trigger",
    OGS_PFCP_USAGE_REPORT_TRIGGER_TYPE,
    3,
    0,
    sizeof(ogs_pfcp_tlv_usage_report_trigger_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_measurement_period =
{
    OGS_TLV_UINT32,
    "Measurement Period",
    OGS_PFCP_MEASUREMENT_PERIOD_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_measurement_period_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_fq_csid =
{
    OGS_TLV_VAR_STR,
    "FQ-CSID",
    OGS_PFCP_FQ_CSID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_fq_csid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_volume_measurement =
{
    OGS_TLV_VAR_STR,
    "Volume Measurement",
    OGS_PFCP_VOLUME_MEASUREMENT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_volume_measurement_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_duration_measurement =
{
    OGS_TLV_UINT32,
    "Duration Measurement",
    OGS_PFCP_DURATION_MEASUREMENT_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_duration_measurement_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_of_first_packet =
{
    OGS_TLV_UINT32,
    "Time of First Packet",
    OGS_PFCP_TIME_OF_FIRST_PACKET_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_time_of_first_packet_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_of_last_packet =
{
    OGS_TLV_UINT32,
    "Time of Last Packet",
    OGS_PFCP_TIME_OF_LAST_PACKET_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_time_of_last_packet_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_quota_holding_time =
{
    OGS_TLV_UINT32,
    "Quota Holding Time",
    OGS_PFCP_QUOTA_HOLDING_TIME_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_quota_holding_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_dropped_dl_traffic_threshold =
{
    OGS_TLV_VAR_STR,
    "Dropped DL Traffic Threshold",
    OGS_PFCP_DROPPED_DL_TRAFFIC_THRESHOLD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_dropped_dl_traffic_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_volume_quota =
{
    OGS_TLV_VAR_STR,
    "Volume Quota",
    OGS_PFCP_VOLUME_QUOTA_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_volume_quota_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_quota =
{
    OGS_TLV_UINT32,
    "Time Quota",
    OGS_PFCP_TIME_QUOTA_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_time_quota_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_start_time =
{
    OGS_TLV_UINT32,
    "Start Time",
    OGS_PFCP_START_TIME_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_start_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_end_time =
{
    OGS_TLV_UINT32,
    "End Time",
    OGS_PFCP_END_TIME_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_end_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_urr_id =
{
    OGS_TLV_UINT32,
    "URR ID",
    OGS_PFCP_URR_ID_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_urr_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_linked_urr_id =
{
    OGS_TLV_VAR_STR,
    "Linked URR ID",
    OGS_PFCP_LINKED_URR_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_linked_urr_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_outer_header_creation =
{
    OGS_TLV_VAR_STR,
    "Outer Header Creation",
    OGS_PFCP_OUTER_HEADER_CREATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_outer_header_creation_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_bar_id =
{
    OGS_TLV_UINT8,
    "BAR ID",
    OGS_PFCP_BAR_ID_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_bar_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_cp_function_features =
{
    OGS_TLV_UINT8,
    "CP Function Features",
    OGS_PFCP_CP_FUNCTION_FEATURES_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_cp_function_features_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_information =
{
    OGS_TLV_VAR_STR,
    "Usage Information",
    OGS_PFCP_USAGE_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_usage_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_application_instance_id =
{
    OGS_TLV_VAR_STR,
    "Application Instance ID",
    OGS_PFCP_APPLICATION_INSTANCE_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_application_instance_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_flow_information =
{
    OGS_TLV_VAR_STR,
    "Flow Information",
    OGS_PFCP_FLOW_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_flow_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ue_ip_address =
{
    OGS_TLV_VAR_STR,
    "UE IP Address",
    OGS_PFCP_UE_IP_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ue_ip_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_packet_rate =
{
    OGS_TLV_VAR_STR,
    "Packet Rate",
    OGS_PFCP_PACKET_RATE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_packet_rate_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_outer_header_removal =
{
    OGS_TLV_VAR_STR,
    "Outer Header Removal",
    OGS_PFCP_OUTER_HEADER_REMOVAL_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_outer_header_removal_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_recovery_time_stamp =
{
    OGS_TLV_UINT32,
    "Recovery Time Stamp",
    OGS_PFCP_RECOVERY_TIME_STAMP_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_recovery_time_stamp_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_dl_flow_level_marking =
{
    OGS_TLV_VAR_STR,
    "DL Flow Level Marking",
    OGS_PFCP_DL_FLOW_LEVEL_MARKING_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_dl_flow_level_marking_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_header_enrichment =
{
    OGS_TLV_VAR_STR,
    "Header Enrichment",
    OGS_PFCP_HEADER_ENRICHMENT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_header_enrichment_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_measurement_information =
{
    OGS_TLV_VAR_STR,
    "Measurement Information",
    OGS_PFCP_MEASUREMENT_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_measurement_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_node_report_type =
{
    OGS_TLV_VAR_STR,
    "Node Report Type",
    OGS_PFCP_NODE_REPORT_TYPE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_node_report_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remote_gtp_u_peer =
{
    OGS_TLV_VAR_STR,
    "Remote GTP-U Peer",
    OGS_PFCP_REMOTE_GTP_U_PEER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remote_gtp_u_peer_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ur_seqn =
{
    OGS_TLV_UINT32,
    "UR-SEQN",
    OGS_PFCP_UR_SEQN_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_ur_seqn_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_activate_predefined_rules =
{
    OGS_TLV_VAR_STR,
    "Activate Predefined Rules",
    OGS_PFCP_ACTIVATE_PREDEFINED_RULES_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_activate_predefined_rules_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_deactivate_predefined_rules =
{
    OGS_TLV_VAR_STR,
    "Deactivate Predefined Rules",
    OGS_PFCP_DEACTIVATE_PREDEFINED_RULES_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_deactivate_predefined_rules_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_far_id =
{
    OGS_TLV_UINT32,
    "FAR ID",
    OGS_PFCP_FAR_ID_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_far_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_qer_id =
{
    OGS_TLV_UINT32,
    "QER ID",
    OGS_PFCP_QER_ID_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_qer_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_oci_flags =
{
    OGS_TLV_VAR_STR,
    "OCI Flags",
    OGS_PFCP_OCI_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_oci_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_association_release_request =
{
    OGS_TLV_VAR_STR,
    "PFCP Association Release Request",
    OGS_PFCP_PFCP_ASSOCIATION_RELEASE_REQUEST_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pfcp_association_release_request_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_graceful_release_period =
{
    OGS_TLV_VAR_STR,
    "Graceful Release Period",
    OGS_PFCP_GRACEFUL_RELEASE_PERIOD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_graceful_release_period_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pdn_type =
{
    OGS_TLV_UINT8,
    "PDN Type",
    OGS_PFCP_PDN_TYPE_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_pdn_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_failed_rule_id =
{
    OGS_TLV_VAR_STR,
    "Failed Rule ID",
    OGS_PFCP_FAILED_RULE_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_failed_rule_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_quota_mechanism =
{
    OGS_TLV_VAR_STR,
    "Time Quota Mechanism",
    OGS_PFCP_TIME_QUOTA_MECHANISM_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_time_quota_mechanism_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_user_plane_ip_resource_information =
{
    OGS_TLV_VAR_STR,
    "User Plane IP Resource Information",
    OGS_PFCP_USER_PLANE_IP_RESOURCE_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_user_plane_ip_resource_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_user_plane_inactivity_timer =
{
    OGS_TLV_VAR_STR,
    "User Plane Inactivity Timer",
    OGS_PFCP_USER_PLANE_INACTIVITY_TIMER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_user_plane_inactivity_timer_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_aggregated_urrs =
{
    OGS_TLV_VAR_STR,
    "Aggregated URRs",
    OGS_PFCP_AGGREGATED_URRS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_aggregated_urrs_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_multiplier =
{
    OGS_TLV_VAR_STR,
    "Multiplier",
    OGS_PFCP_MULTIPLIER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_multiplier_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_aggregated_urr_id =
{
    OGS_TLV_VAR_STR,
    "Aggregated URR ID",
    OGS_PFCP_AGGREGATED_URR_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_aggregated_urr_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_volume_quota =
{
    OGS_TLV_VAR_STR,
    "Subsequent Volume Quota",
    OGS_PFCP_SUBSEQUENT_VOLUME_QUOTA_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_subsequent_volume_quota_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_time_quota =
{
    OGS_TLV_VAR_STR,
    "Subsequent Time Quota",
    OGS_PFCP_SUBSEQUENT_TIME_QUOTA_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_subsequent_time_quota_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_rqi =
{
    OGS_TLV_UINT8,
    "RQI",
    OGS_PFCP_RQI_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_rqi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_qfi =
{
    OGS_TLV_UINT8,
    "QFI",
    OGS_PFCP_QFI_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_qfi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_query_urr_reference =
{
    OGS_TLV_VAR_STR,
    "Query URR Reference",
    OGS_PFCP_QUERY_URR_REFERENCE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_query_urr_reference_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_additional_usage_reports_information =
{
    OGS_TLV_VAR_STR,
    "Additional Usage Reports Information",
    OGS_PFCP_ADDITIONAL_USAGE_REPORTS_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_additional_usage_reports_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_traffic_endpoint =
{
    OGS_TLV_VAR_STR,
    "Update Traffic Endpoint",
    OGS_PFCP_UPDATE_TRAFFIC_ENDPOINT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_traffic_endpoint_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_traffic_endpoint_id =
{
    OGS_TLV_VAR_STR,
    "Traffic Endpoint ID",
    OGS_PFCP_TRAFFIC_ENDPOINT_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_traffic_endpoint_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mac_address =
{
    OGS_TLV_VAR_STR,
    "MAC address",
    OGS_PFCP_MAC_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mac_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_c_tag =
{
    OGS_TLV_VAR_STR,
    "C-TAG",
    OGS_PFCP_C_TAG_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_c_tag_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_s_tag =
{
    OGS_TLV_VAR_STR,
    "S-TAG",
    OGS_PFCP_S_TAG_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_s_tag_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethertype =
{
    OGS_TLV_VAR_STR,
    "Ethertype",
    OGS_PFCP_ETHERTYPE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ethertype_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_proxying =
{
    OGS_TLV_VAR_STR,
    "Proxying",
    OGS_PFCP_PROXYING_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_proxying_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethernet_filter_id =
{
    OGS_TLV_VAR_STR,
    "Ethernet Filter ID",
    OGS_PFCP_ETHERNET_FILTER_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ethernet_filter_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethernet_filter_properties =
{
    OGS_TLV_VAR_STR,
    "Ethernet Filter Properties",
    OGS_PFCP_ETHERNET_FILTER_PROPERTIES_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ethernet_filter_properties_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_suggested_buffering_packets_count =
{
    OGS_TLV_VAR_STR,
    "Suggested Buffering Packets Count",
    OGS_PFCP_SUGGESTED_BUFFERING_PACKETS_COUNT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_suggested_buffering_packets_count_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_user_id =
{
    OGS_TLV_VAR_STR,
    "User ID",
    OGS_PFCP_USER_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_user_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethernet_pdu_session_information =
{
    OGS_TLV_VAR_STR,
    "Ethernet PDU Session Information",
    OGS_PFCP_ETHERNET_PDU_SESSION_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ethernet_pdu_session_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mac_addresses_detected =
{
    OGS_TLV_VAR_STR,
    "MAC Addresses Detected",
    OGS_PFCP_MAC_ADDRESSES_DETECTED_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mac_addresses_detected_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mac_addresses_removed =
{
    OGS_TLV_VAR_STR,
    "MAC Addresses Removed",
    OGS_PFCP_MAC_ADDRESSES_REMOVED_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mac_addresses_removed_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethernet_inactivity_timer =
{
    OGS_TLV_VAR_STR,
    "Ethernet Inactivity Timer",
    OGS_PFCP_ETHERNET_INACTIVITY_TIMER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ethernet_inactivity_timer_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_additional_monitoring_time =
{
    OGS_TLV_VAR_STR,
    "Additional Monitoring Time",
    OGS_PFCP_ADDITIONAL_MONITORING_TIME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_additional_monitoring_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_event_quota =
{
    OGS_TLV_UINT32,
    "Event Quota",
    OGS_PFCP_EVENT_QUOTA_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_event_quota_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_event_threshold =
{
    OGS_TLV_UINT32,
    "Event Threshold",
    OGS_PFCP_EVENT_THRESHOLD_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_event_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_event_quota =
{
    OGS_TLV_VAR_STR,
    "Subsequent Event Quota",
    OGS_PFCP_SUBSEQUENT_EVENT_QUOTA_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_subsequent_event_quota_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_event_threshold =
{
    OGS_TLV_VAR_STR,
    "Subsequent Event Threshold",
    OGS_PFCP_SUBSEQUENT_EVENT_THRESHOLD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_subsequent_event_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_trace_information =
{
    OGS_TLV_VAR_STR,
    "Trace Information",
    OGS_PFCP_TRACE_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_trace_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_framed_route =
{
    OGS_TLV_VAR_STR,
    "Framed-Route",
    OGS_PFCP_FRAMED_ROUTE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_framed_route_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_framed_routing =
{
    OGS_TLV_VAR_STR,
    "Framed-Routing",
    OGS_PFCP_FRAMED_ROUTING_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_framed_routing_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_framed_ipv6_route =
{
    OGS_TLV_VAR_STR,
    "Framed-IPv6-Route",
    OGS_PFCP_FRAMED_IPV6_ROUTE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_framed_ipv6_route_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_stamp =
{
    OGS_TLV_VAR_STR,
    "Time Stamp",
    OGS_PFCP_TIME_STAMP_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_time_stamp_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_averaging_window =
{
    OGS_TLV_UINT32,
    "Averaging Window",
    OGS_PFCP_AVERAGING_WINDOW_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_averaging_window_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_paging_policy_indicator =
{
    OGS_TLV_UINT8,
    "Paging Policy Indicator",
    OGS_PFCP_PAGING_POLICY_INDICATOR_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_paging_policy_indicator_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_apn_dnn =
{
    OGS_TLV_VAR_STR,
    "APN/DNN",
    OGS_PFCP_APN_DNN_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_apn_dnn_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc__interface_type =
{
    OGS_TLV_UINT8,
    "3GPP Interface Type",
    OGS_PFCP__INTERFACE_TYPE_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv__interface_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpsrreq_flags =
{
    OGS_TLV_UINT8,
    "PFCPSRReq-Flags",
    OGS_PFCP_PFCPSRREQ_FLAGS_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_pfcpsrreq_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpaureq_flags =
{
    OGS_TLV_UINT8,
    "PFCPAUReq-Flags",
    OGS_PFCP_PFCPAUREQ_FLAGS_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_pfcpaureq_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_activation_time =
{
    OGS_TLV_VAR_STR,
    "Activation Time",
    OGS_PFCP_ACTIVATION_TIME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_activation_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_deactivation_time =
{
    OGS_TLV_VAR_STR,
    "Deactivation Time",
    OGS_PFCP_DEACTIVATION_TIME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_deactivation_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mar_id =
{
    OGS_TLV_VAR_STR,
    "MAR ID",
    OGS_PFCP_MAR_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mar_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_steering_functionality =
{
    OGS_TLV_VAR_STR,
    "Steering Functionality",
    OGS_PFCP_STEERING_FUNCTIONALITY_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_steering_functionality_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_steering_mode =
{
    OGS_TLV_VAR_STR,
    "Steering Mode",
    OGS_PFCP_STEERING_MODE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_steering_mode_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_weight =
{
    OGS_TLV_VAR_STR,
    "Weight",
    OGS_PFCP_WEIGHT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_weight_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_priority =
{
    OGS_TLV_VAR_STR,
    "Priority",
    OGS_PFCP_PRIORITY_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_priority_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ue_ip_address_pool_identity =
{
    OGS_TLV_VAR_STR,
    "UE IP address Pool Identity",
    OGS_PFCP_UE_IP_ADDRESS_POOL_IDENTITY_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ue_ip_address_pool_identity_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_alternative_smf_ip_address =
{
    OGS_TLV_VAR_STR,
    "Alternative SMF IP Address",
    OGS_PFCP_ALTERNATIVE_SMF_IP_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_alternative_smf_ip_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_packet_replication_and_detection_carry_on_information =
{
    OGS_TLV_VAR_STR,
    "Packet Replication and Detection Carry-On Information",
    OGS_PFCP_PACKET_REPLICATION_AND_DETECTION_CARRY_ON_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_packet_replication_and_detection_carry_on_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_smf_set_id =
{
    OGS_TLV_VAR_STR,
    "SMF Set ID",
    OGS_PFCP_SMF_SET_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_smf_set_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_quota_validity_time =
{
    OGS_TLV_UINT32,
    "Quota Validity Time",
    OGS_PFCP_QUOTA_VALIDITY_TIME_TYPE,
    4,
    0,
    sizeof(ogs_pfcp_tlv_quota_validity_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_number_of_reports =
{
    OGS_TLV_VAR_STR,
    "Number of Reports",
    OGS_PFCP_NUMBER_OF_REPORTS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_number_of_reports_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpasrsp_flags =
{
    OGS_TLV_VAR_STR,
    "PFCPASRsp-Flags",
    OGS_PFCP_PFCPASRSP_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pfcpasrsp_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_cp_pfcp_entity_ip_address =
{
    OGS_TLV_VAR_STR,
    "CP PFCP Entity IP Address",
    OGS_PFCP_CP_PFCP_ENTITY_IP_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_cp_pfcp_entity_ip_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpsereq_flags =
{
    OGS_TLV_UINT8,
    "PFCPSEReq-Flags",
    OGS_PFCP_PFCPSEREQ_FLAGS_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_pfcpsereq_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ip_multicast_address =
{
    OGS_TLV_VAR_STR,
    "IP Multicast Address",
    OGS_PFCP_IP_MULTICAST_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ip_multicast_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_source_ip_address =
{
    OGS_TLV_VAR_STR,
    "Source IP Address",
    OGS_PFCP_SOURCE_IP_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_source_ip_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_packet_rate_status =
{
    OGS_TLV_VAR_STR,
    "Packet Rate Status",
    OGS_PFCP_PACKET_RATE_STATUS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_packet_rate_status_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_bridge_info_for_tsc =
{
    OGS_TLV_VAR_STR,
    "Create Bridge Info for TSC",
    OGS_PFCP_CREATE_BRIDGE_INFO_FOR_TSC_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_bridge_info_for_tsc_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ds_tt_port_number =
{
    OGS_TLV_VAR_STR,
    "DS-TT Port Number",
    OGS_PFCP_DS_TT_PORT_NUMBER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ds_tt_port_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_nw_tt_port_number =
{
    OGS_TLV_VAR_STR,
    "NW-TT Port Number",
    OGS_PFCP_NW_TT_PORT_NUMBER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_nw_tt_port_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_fivegs_user_plane_node =
{
    OGS_TLV_VAR_STR,
    "5GS User Plane Node",
    OGS_PFCP_FiveGS_USER_PLANE_NODE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_fivegs_user_plane_node_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_port_management_information_container =
{
    OGS_TLV_VAR_STR,
    "Port Management Information Container",
    OGS_PFCP_PORT_MANAGEMENT_INFORMATION_CONTAINER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_port_management_information_container_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_requested_clock_drift_information =
{
    OGS_TLV_VAR_STR,
    "Requested Clock Drift Information",
    OGS_PFCP_REQUESTED_CLOCK_DRIFT_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_requested_clock_drift_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_domain_number =
{
    OGS_TLV_VAR_STR,
    "Time Domain Number",
    OGS_PFCP_TIME_DOMAIN_NUMBER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_time_domain_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_offset_threshold =
{
    OGS_TLV_VAR_STR,
    "Time Offset Threshold",
    OGS_PFCP_TIME_OFFSET_THRESHOLD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_time_offset_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_cumulative_rateratio_threshold =
{
    OGS_TLV_VAR_STR,
    "Cumulative rateRatio Threshold",
    OGS_PFCP_CUMULATIVE_RATERATIO_THRESHOLD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_cumulative_rateratio_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_offset_measurement =
{
    OGS_TLV_VAR_STR,
    "Time Offset Measurement",
    OGS_PFCP_TIME_OFFSET_MEASUREMENT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_time_offset_measurement_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_cumulative_rateratio_measurement =
{
    OGS_TLV_VAR_STR,
    "Cumulative rateRatio Measurement",
    OGS_PFCP_CUMULATIVE_RATERATIO_MEASUREMENT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_cumulative_rateratio_measurement_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_srr_id =
{
    OGS_TLV_VAR_STR,
    "SRR ID",
    OGS_PFCP_SRR_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_srr_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_access_availability_control_information =
{
    OGS_TLV_VAR_STR,
    "Access Availability Control Information",
    OGS_PFCP_ACCESS_AVAILABILITY_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_access_availability_control_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_requested_access_availability_information =
{
    OGS_TLV_VAR_STR,
    "Requested Access Availability Information",
    OGS_PFCP_REQUESTED_ACCESS_AVAILABILITY_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_requested_access_availability_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_access_availability_information =
{
    OGS_TLV_VAR_STR,
    "Access Availability Information",
    OGS_PFCP_ACCESS_AVAILABILITY_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_access_availability_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mptcp_control_information =
{
    OGS_TLV_VAR_STR,
    "MPTCP Control Information",
    OGS_PFCP_MPTCP_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mptcp_control_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_atsss_ll_control_information =
{
    OGS_TLV_VAR_STR,
    "ATSSS-LL Control Information",
    OGS_PFCP_ATSSS_LL_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_atsss_ll_control_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pmf_control_information =
{
    OGS_TLV_VAR_STR,
    "PMF Control Information",
    OGS_PFCP_PMF_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pmf_control_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mptcp_address_information =
{
    OGS_TLV_VAR_STR,
    "MPTCP Address Information",
    OGS_PFCP_MPTCP_ADDRESS_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mptcp_address_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ue_link_specific_ip_address =
{
    OGS_TLV_VAR_STR,
    "UE Link-Specific IP Address",
    OGS_PFCP_UE_LINK_SPECIFIC_IP_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ue_link_specific_ip_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pmf_address_information =
{
    OGS_TLV_VAR_STR,
    "PMF Address Information",
    OGS_PFCP_PMF_ADDRESS_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pmf_address_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_atsss_ll_information =
{
    OGS_TLV_VAR_STR,
    "ATSSS-LL Information",
    OGS_PFCP_ATSSS_LL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_atsss_ll_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_data_network_access_identifier =
{
    OGS_TLV_VAR_STR,
    "Data Network Access Identifier",
    OGS_PFCP_DATA_NETWORK_ACCESS_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_data_network_access_identifier_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_average_packet_delay =
{
    OGS_TLV_VAR_STR,
    "Average Packet Delay",
    OGS_PFCP_AVERAGE_PACKET_DELAY_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_average_packet_delay_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_minimum_packet_delay =
{
    OGS_TLV_VAR_STR,
    "Minimum Packet Delay",
    OGS_PFCP_MINIMUM_PACKET_DELAY_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_minimum_packet_delay_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_maximum_packet_delay =
{
    OGS_TLV_VAR_STR,
    "Maximum Packet Delay",
    OGS_PFCP_MAXIMUM_PACKET_DELAY_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_maximum_packet_delay_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_qos_report_trigger =
{
    OGS_TLV_VAR_STR,
    "QoS Report Trigger",
    OGS_PFCP_QOS_REPORT_TRIGGER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_qos_report_trigger_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_gtp_u_path_qos_control_information =
{
    OGS_TLV_VAR_STR,
    "GTP-U Path QoS Control Information",
    OGS_PFCP_GTP_U_PATH_QOS_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_gtp_u_path_qos_control_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_gtp_u_path_interface_type =
{
    OGS_TLV_VAR_STR,
    "GTP-U Path Interface Type",
    OGS_PFCP_GTP_U_PATH_INTERFACE_TYPE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_gtp_u_path_interface_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_qos_monitoring_per_qos_flow_control_information =
{
    OGS_TLV_VAR_STR,
    "QoS Monitoring per QoS flow Control Information",
    OGS_PFCP_QOS_MONITORING_PER_QOS_FLOW_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_qos_monitoring_per_qos_flow_control_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_requested_qos_monitoring =
{
    OGS_TLV_VAR_STR,
    "Requested QoS Monitoring",
    OGS_PFCP_REQUESTED_QOS_MONITORING_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_requested_qos_monitoring_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_reporting_frequency =
{
    OGS_TLV_VAR_STR,
    "Reporting Frequency",
    OGS_PFCP_REPORTING_FREQUENCY_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_reporting_frequency_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_packet_delay_thresholds =
{
    OGS_TLV_VAR_STR,
    "Packet Delay Thresholds",
    OGS_PFCP_PACKET_DELAY_THRESHOLDS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_packet_delay_thresholds_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_minimum_wait_time =
{
    OGS_TLV_VAR_STR,
    "Minimum Wait Time",
    OGS_PFCP_MINIMUM_WAIT_TIME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_minimum_wait_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_qos_monitoring_measurement =
{
    OGS_TLV_VAR_STR,
    "QoS Monitoring Measurement",
    OGS_PFCP_QOS_MONITORING_MEASUREMENT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_qos_monitoring_measurement_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mt_edt_control_information =
{
    OGS_TLV_VAR_STR,
    "MT-EDT Control Information",
    OGS_PFCP_MT_EDT_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mt_edt_control_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_dl_data_packets_size =
{
    OGS_TLV_VAR_STR,
    "DL Data Packets Size",
    OGS_PFCP_DL_DATA_PACKETS_SIZE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_dl_data_packets_size_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_qer_control_indications =
{
    OGS_TLV_VAR_STR,
    "QER Control Indications",
    OGS_PFCP_QER_CONTROL_INDICATIONS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_qer_control_indications_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_nf_instance_id =
{
    OGS_TLV_VAR_STR,
    "NF Instance ID",
    OGS_PFCP_NF_INSTANCE_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_nf_instance_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_s_nssai =
{
    OGS_TLV_VAR_STR,
    "S-NSSAI",
    OGS_PFCP_S_NSSAI_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_s_nssai_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ip_version =
{
    OGS_TLV_VAR_STR,
    "IP version",
    OGS_PFCP_IP_VERSION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ip_version_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpasreq_flags =
{
    OGS_TLV_VAR_STR,
    "PFCPASReq-Flags",
    OGS_PFCP_PFCPASREQ_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pfcpasreq_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_data_status =
{
    OGS_TLV_UINT8,
    "Data Status",
    OGS_PFCP_DATA_STATUS_TYPE,
    1,
    0,
    sizeof(ogs_pfcp_tlv_data_status_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_rds_configuration_information =
{
    OGS_TLV_VAR_STR,
    "RDS Configuration Information",
    OGS_PFCP_RDS_CONFIGURATION_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_rds_configuration_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mptcp_applicable_indication =
{
    OGS_TLV_VAR_STR,
    "MPTCP Applicable Indication",
    OGS_PFCP_MPTCP_APPLICABLE_INDICATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mptcp_applicable_indication_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_bridge_management_information_container =
{
    OGS_TLV_VAR_STR,
    "Bridge Management Information Container",
    OGS_PFCP_BRIDGE_MANAGEMENT_INFORMATION_CONTAINER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_bridge_management_information_container_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_number_of_ue_ip_addresses =
{
    OGS_TLV_VAR_STR,
    "Number of UE IP Addresses",
    OGS_PFCP_NUMBER_OF_UE_IP_ADDRESSES_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_number_of_ue_ip_addresses_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_validity_timer =
{
    OGS_TLV_VAR_STR,
    "Validity Timer",
    OGS_PFCP_VALIDITY_TIMER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_validity_timer_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_spare =
{
    OGS_TLV_VAR_STR,
    "Spare",
    OGS_PFCP_SPARE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_spare_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_offending_ie_information =
{
    OGS_TLV_VAR_STR,
    "Offending IE Information",
    OGS_PFCP_OFFENDING_IE_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_offending_ie_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_rat_type =
{
    OGS_TLV_VAR_STR,
    "RAT Type",
    OGS_PFCP_RAT_TYPE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_rat_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_l2tp_user_authentication_ie =
{
    OGS_TLV_VAR_STR,
    "L2TP User Authentication IE",
    OGS_PFCP_L2TP_USER_AUTHENTICATION_IE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_l2tp_user_authentication_ie_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_lns_address =
{
    OGS_TLV_VAR_STR,
    "LNS Address",
    OGS_PFCP_LNS_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_lns_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_tunnel_preference =
{
    OGS_TLV_VAR_STR,
    "Tunnel Preference",
    OGS_PFCP_TUNNEL_PREFERENCE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_tunnel_preference_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_calling_number =
{
    OGS_TLV_VAR_STR,
    "Calling Number",
    OGS_PFCP_CALLING_NUMBER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_calling_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_called_number =
{
    OGS_TLV_VAR_STR,
    "Called Number",
    OGS_PFCP_CALLED_NUMBER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_called_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_l2tp_session_indications =
{
    OGS_TLV_VAR_STR,
    "L2TP Session Indications",
    OGS_PFCP_L2TP_SESSION_INDICATIONS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_l2tp_session_indications_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_dns_server_address =
{
    OGS_TLV_VAR_STR,
    "DNS Server Address",
    OGS_PFCP_DNS_SERVER_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_dns_server_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_nbns_server_address =
{
    OGS_TLV_VAR_STR,
    "NBNS Server Address",
    OGS_PFCP_NBNS_SERVER_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_nbns_server_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_maximum_receive_unit =
{
    OGS_TLV_VAR_STR,
    "Maximum Receive Unit",
    OGS_PFCP_MAXIMUM_RECEIVE_UNIT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_maximum_receive_unit_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_thresholds =
{
    OGS_TLV_VAR_STR,
    "Thresholds",
    OGS_PFCP_THRESHOLDS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_thresholds_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_steering_mode_indicator =
{
    OGS_TLV_VAR_STR,
    "Steering Mode Indicator",
    OGS_PFCP_STEERING_MODE_INDICATOR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_steering_mode_indicator_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_group_id =
{
    OGS_TLV_VAR_STR,
    "Group ID",
    OGS_PFCP_GROUP_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_group_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_cp_ip_address =
{
    OGS_TLV_VAR_STR,
    "CP IP Address",
    OGS_PFCP_CP_IP_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_cp_ip_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ip_address_and_port_number_replacement =
{
    OGS_TLV_VAR_STR,
    "IP Address and Port number Replacement",
    OGS_PFCP_IP_ADDRESS_AND_PORT_NUMBER_REPLACEMENT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ip_address_and_port_number_replacement_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_dns_query_filter =
{
    OGS_TLV_VAR_STR,
    "DNS Query Filter",
    OGS_PFCP_DNS_QUERY_FILTER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_dns_query_filter_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_direct_reporting_information =
{
    OGS_TLV_VAR_STR,
    "Direct Reporting Information",
    OGS_PFCP_DIRECT_REPORTING_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_direct_reporting_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_event_notification_uri =
{
    OGS_TLV_VAR_STR,
    "Event Notification URI",
    OGS_PFCP_EVENT_NOTIFICATION_URI_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_event_notification_uri_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_notification_correlation_id =
{
    OGS_TLV_VAR_STR,
    "Notification Correlation ID",
    OGS_PFCP_NOTIFICATION_CORRELATION_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_notification_correlation_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_reporting_flags =
{
    OGS_TLV_VAR_STR,
    "Reporting Flags",
    OGS_PFCP_REPORTING_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_reporting_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_predefined_rules_name =
{
    OGS_TLV_VAR_STR,
    "Predefined Rules Name",
    OGS_PFCP_PREDEFINED_RULES_NAME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_predefined_rules_name_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_mbs_unicast_parameters =
{
    OGS_TLV_VAR_STR,
    "Remove MBS Unicast Parameters",
    OGS_PFCP_REMOVE_MBS_UNICAST_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_mbs_unicast_parameters_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mbs_session_identifier =
{
    OGS_TLV_VAR_STR,
    "MBS Session Identifier",
    OGS_PFCP_MBS_SESSION_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mbs_session_identifier_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_multicast_transport_information =
{
    OGS_TLV_VAR_STR,
    "Multicast Transport Information",
    OGS_PFCP_MULTICAST_TRANSPORT_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_multicast_transport_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mbsn4mbreq_flags =
{
    OGS_TLV_VAR_STR,
    "MBSN4mbReq-Flags",
    OGS_PFCP_MBSN4MBREQ_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mbsn4mbreq_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_local_ingress_tunnel =
{
    OGS_TLV_VAR_STR,
    "Local Ingress Tunnel",
    OGS_PFCP_LOCAL_INGRESS_TUNNEL_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_local_ingress_tunnel_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mbs_unicast_parameters_id =
{
    OGS_TLV_VAR_STR,
    "MBS Unicast Parameters ID",
    OGS_PFCP_MBS_UNICAST_PARAMETERS_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mbs_unicast_parameters_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mbsn4resp_flags =
{
    OGS_TLV_VAR_STR,
    "MBSN4Resp-Flags",
    OGS_PFCP_MBSN4RESP_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mbsn4resp_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_tunnel_password =
{
    OGS_TLV_VAR_STR,
    "Tunnel Password",
    OGS_PFCP_TUNNEL_PASSWORD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_tunnel_password_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_area_session_id =
{
    OGS_TLV_VAR_STR,
    "Area Session ID",
    OGS_PFCP_AREA_SESSION_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_area_session_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_peer_up_restart_report =
{
    OGS_TLV_VAR_STR,
    "Peer UP Restart Report",
    OGS_PFCP_PEER_UP_RESTART_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_peer_up_restart_report_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_dscp_to_ppi_mapping_information =
{
    OGS_TLV_VAR_STR,
    "DSCP to PPI Mapping Information",
    OGS_PFCP_DSCP_TO_PPI_MAPPING_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_dscp_to_ppi_mapping_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpsdrsp_flags =
{
    OGS_TLV_VAR_STR,
    "PFCPSDRsp-Flags",
    OGS_PFCP_PFCPSDRSP_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pfcpsdrsp_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_qer_indications =
{
    OGS_TLV_VAR_STR,
    "QER Indications",
    OGS_PFCP_QER_INDICATIONS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_qer_indications_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_vendor_specific_node_report_type =
{
    OGS_TLV_VAR_STR,
    "Vendor-Specific Node Report Type",
    OGS_PFCP_VENDOR_SPECIFIC_NODE_REPORT_TYPE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_vendor_specific_node_report_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_configured_time_domain =
{
    OGS_TLV_VAR_STR,
    "Configured Time Domain",
    OGS_PFCP_CONFIGURED_TIME_DOMAIN_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_configured_time_domain_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethernet_packet_filter =
{
    OGS_TLV_COMPOUND,
    "Ethernet Packet Filter",
    OGS_PFCP_ETHERNET_PACKET_FILTER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ethernet_packet_filter_t),
    {
        &ogs_pfcp_tlv_desc_ethernet_filter_id,
        &ogs_pfcp_tlv_desc_ethernet_filter_properties,
        &ogs_pfcp_tlv_desc_mac_address,
        &ogs_pfcp_tlv_desc_ethertype,
        &ogs_pfcp_tlv_desc_c_tag,
        &ogs_pfcp_tlv_desc_s_tag,
        &ogs_pfcp_tlv_desc_sdf_filter,
        &ogs_tlv_desc_more8,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_redundant_transmission_parameters =
{
    OGS_TLV_COMPOUND,
    "Redundant Transmission Parameters",
    OGS_PFCP_REDUNDANT_TRANSMISSION_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_redundant_transmission_parameters_t),
    {
        &ogs_pfcp_tlv_desc_f_teid,
        &ogs_pfcp_tlv_desc_network_instance,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ip_multicast_addressing_info_within_pfcp_session_establishment_request =
{
    OGS_TLV_COMPOUND,
    "IP Multicast Addressing Info within PFCP Session Establishment Request",
    OGS_PFCP_IP_MULTICAST_ADDRESSING_INFO_WITHIN_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ip_multicast_addressing_info_within_pfcp_session_establishment_request_t),
    {
        &ogs_pfcp_tlv_desc_ip_multicast_address,
        &ogs_pfcp_tlv_desc_source_ip_address,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pdi =
{
    OGS_TLV_COMPOUND,
    "PDI",
    OGS_PFCP_PDI_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pdi_t),
    {
        &ogs_pfcp_tlv_desc_source_interface,
        &ogs_pfcp_tlv_desc_f_teid,
        &ogs_pfcp_tlv_desc_local_ingress_tunnel,
        &ogs_pfcp_tlv_desc_network_instance,
        &ogs_pfcp_tlv_desc_redundant_transmission_parameters,
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_traffic_endpoint_id,
        &ogs_pfcp_tlv_desc_sdf_filter,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_application_id,
        &ogs_pfcp_tlv_desc_ethernet_pdu_session_information,
        &ogs_pfcp_tlv_desc_ethernet_packet_filter,
        &ogs_pfcp_tlv_desc_qfi,
        &ogs_pfcp_tlv_desc_framed_route,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_framed_routing,
        &ogs_pfcp_tlv_desc_framed_ipv6_route,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc__interface_type,
        &ogs_pfcp_tlv_desc_ip_multicast_addressing_info_within_pfcp_session_establishment_request,
        &ogs_pfcp_tlv_desc_dns_query_filter,
        &ogs_pfcp_tlv_desc_mbs_session_identifier,
        &ogs_pfcp_tlv_desc_area_session_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_transport_delay_reporting =
{
    OGS_TLV_COMPOUND,
    "Transport Delay Reporting",
    OGS_PFCP_TRANSPORT_DELAY_REPORTING_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_transport_delay_reporting_t),
    {
        &ogs_pfcp_tlv_desc_remote_gtp_u_peer,
        &ogs_pfcp_tlv_desc_transport_level_marking,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_pdr =
{
    OGS_TLV_COMPOUND,
    "Create PDR",
    OGS_PFCP_CREATE_PDR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_pdr_t),
    {
        &ogs_pfcp_tlv_desc_pdr_id,
        &ogs_pfcp_tlv_desc_precedence,
        &ogs_pfcp_tlv_desc_pdi,
        &ogs_pfcp_tlv_desc_outer_header_removal,
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_urr_id,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_qer_id,
        &ogs_pfcp_tlv_desc_activate_predefined_rules,
        &ogs_pfcp_tlv_desc_activation_time,
        &ogs_pfcp_tlv_desc_deactivation_time,
        &ogs_pfcp_tlv_desc_mar_id,
        &ogs_pfcp_tlv_desc_packet_replication_and_detection_carry_on_information,
        &ogs_pfcp_tlv_desc_ip_multicast_addressing_info_within_pfcp_session_establishment_request,
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_mptcp_applicable_indication,
        &ogs_pfcp_tlv_desc_transport_delay_reporting,
        &ogs_pfcp_tlv_desc_rat_type,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_forwarding_parameters =
{
    OGS_TLV_COMPOUND,
    "Forwarding Parameters",
    OGS_PFCP_FORWARDING_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_forwarding_parameters_t),
    {
        &ogs_pfcp_tlv_desc_destination_interface,
        &ogs_pfcp_tlv_desc_network_instance,
        &ogs_pfcp_tlv_desc_redirect_information,
        &ogs_pfcp_tlv_desc_outer_header_creation,
        &ogs_pfcp_tlv_desc_transport_level_marking,
        &ogs_pfcp_tlv_desc_forwarding_policy,
        &ogs_pfcp_tlv_desc_header_enrichment,
        &ogs_pfcp_tlv_desc_traffic_endpoint_id,
        &ogs_pfcp_tlv_desc_proxying,
        &ogs_pfcp_tlv_desc__interface_type,
        &ogs_pfcp_tlv_desc_data_network_access_identifier,
        &ogs_pfcp_tlv_desc_ip_address_and_port_number_replacement,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_duplicating_parameters =
{
    OGS_TLV_COMPOUND,
    "Duplicating Parameters",
    OGS_PFCP_DUPLICATING_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_duplicating_parameters_t),
    {
        &ogs_pfcp_tlv_desc_destination_interface,
        &ogs_pfcp_tlv_desc_outer_header_creation,
        &ogs_pfcp_tlv_desc_transport_level_marking,
        &ogs_pfcp_tlv_desc_forwarding_policy,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_redundant_transmission_forwarding_parameters =
{
    OGS_TLV_COMPOUND,
    "Redundant Transmission Forwarding Parameters",
    OGS_PFCP_REDUNDANT_TRANSMISSION_FORWARDING_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_redundant_transmission_forwarding_parameters_t),
    {
        &ogs_pfcp_tlv_desc_outer_header_creation,
        &ogs_pfcp_tlv_desc_network_instance,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mbs_multicast_parameters =
{
    OGS_TLV_COMPOUND,
    "MBS Multicast Parameters",
    OGS_PFCP_MBS_MULTICAST_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mbs_multicast_parameters_t),
    {
        &ogs_pfcp_tlv_desc_destination_interface,
        &ogs_pfcp_tlv_desc_network_instance,
        &ogs_pfcp_tlv_desc_outer_header_creation,
        &ogs_pfcp_tlv_desc_transport_level_marking,
        &ogs_pfcp_tlv_desc__interface_type,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_add_mbs_unicast_parameters =
{
    OGS_TLV_COMPOUND,
    "Add MBS Unicast Parameters",
    OGS_PFCP_ADD_MBS_UNICAST_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_add_mbs_unicast_parameters_t),
    {
        &ogs_pfcp_tlv_desc_destination_interface,
        &ogs_pfcp_tlv_desc_mbs_unicast_parameters_id,
        &ogs_pfcp_tlv_desc_network_instance,
        &ogs_pfcp_tlv_desc_outer_header_creation,
        &ogs_pfcp_tlv_desc_transport_level_marking,
        &ogs_pfcp_tlv_desc__interface_type,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_far =
{
    OGS_TLV_COMPOUND,
    "Create FAR",
    OGS_PFCP_CREATE_FAR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_far_t),
    {
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_apply_action,
        &ogs_pfcp_tlv_desc_forwarding_parameters,
        &ogs_pfcp_tlv_desc_duplicating_parameters,
        &ogs_pfcp_tlv_desc_bar_id,
        &ogs_pfcp_tlv_desc_redundant_transmission_forwarding_parameters,
        &ogs_pfcp_tlv_desc_mbs_multicast_parameters,
        &ogs_pfcp_tlv_desc_add_mbs_unicast_parameters,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_forwarding_parameters =
{
    OGS_TLV_COMPOUND,
    "Update Forwarding Parameters",
    OGS_PFCP_UPDATE_FORWARDING_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_forwarding_parameters_t),
    {
        &ogs_pfcp_tlv_desc_destination_interface,
        &ogs_pfcp_tlv_desc_network_instance,
        &ogs_pfcp_tlv_desc_redirect_information,
        &ogs_pfcp_tlv_desc_outer_header_creation,
        &ogs_pfcp_tlv_desc_transport_level_marking,
        &ogs_pfcp_tlv_desc_forwarding_policy,
        &ogs_pfcp_tlv_desc_header_enrichment,
        &ogs_pfcp_tlv_desc_pfcpsmreq_flags,
        &ogs_pfcp_tlv_desc_traffic_endpoint_id,
        &ogs_pfcp_tlv_desc__interface_type,
        &ogs_pfcp_tlv_desc_data_network_access_identifier,
        &ogs_pfcp_tlv_desc_ip_address_and_port_number_replacement,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_duplicating_parameters =
{
    OGS_TLV_COMPOUND,
    "Update Duplicating Parameters",
    OGS_PFCP_UPDATE_DUPLICATING_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_duplicating_parameters_t),
    {
        &ogs_pfcp_tlv_desc_destination_interface,
        &ogs_pfcp_tlv_desc_outer_header_creation,
        &ogs_pfcp_tlv_desc_transport_level_marking,
        &ogs_pfcp_tlv_desc_forwarding_policy,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_far =
{
    OGS_TLV_COMPOUND,
    "Update FAR",
    OGS_PFCP_UPDATE_FAR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_far_t),
    {
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_apply_action,
        &ogs_pfcp_tlv_desc_update_forwarding_parameters,
        &ogs_pfcp_tlv_desc_update_duplicating_parameters,
        &ogs_pfcp_tlv_desc_redundant_transmission_forwarding_parameters,
        &ogs_pfcp_tlv_desc_bar_id,
        &ogs_pfcp_tlv_desc_add_mbs_unicast_parameters,
        &ogs_pfcp_tlv_desc_remove_mbs_unicast_parameters,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfd_context =
{
    OGS_TLV_COMPOUND,
    "PFD context",
    OGS_PFCP_PFD_CONTEXT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pfd_context_t),
    {
        &ogs_pfcp_tlv_desc_pfd_contents,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_application_id_s_pfds =
{
    OGS_TLV_COMPOUND,
    "Application ID's PFDs",
    OGS_PFCP_APPLICATION_ID_S_PFDS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_application_id_s_pfds_t),
    {
        &ogs_pfcp_tlv_desc_application_id,
        &ogs_pfcp_tlv_desc_pfd_context,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethernet_traffic_information =
{
    OGS_TLV_COMPOUND,
    "Ethernet Traffic Information",
    OGS_PFCP_ETHERNET_TRAFFIC_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ethernet_traffic_information_t),
    {
        &ogs_pfcp_tlv_desc_mac_addresses_detected,
        &ogs_pfcp_tlv_desc_mac_addresses_removed,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc__access_forwarding_action_information =
{
    OGS_TLV_COMPOUND,
    "3GPP Access Forwarding Action Information",
    OGS_PFCP__ACCESS_FORWARDING_ACTION_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv__access_forwarding_action_information_t),
    {
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_weight,
        &ogs_pfcp_tlv_desc_priority,
        &ogs_pfcp_tlv_desc_urr_id,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_rat_type,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_non__access_forwarding_action_information =
{
    OGS_TLV_COMPOUND,
    "Non-3GPP Access Forwarding Action Information",
    OGS_PFCP_NON__ACCESS_FORWARDING_ACTION_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_non__access_forwarding_action_information_t),
    {
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_weight,
        &ogs_pfcp_tlv_desc_priority,
        &ogs_pfcp_tlv_desc_urr_id,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_rat_type,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update__access_forwarding_action_information =
{
    OGS_TLV_COMPOUND,
    "Update 3GPP Access Forwarding Action Information",
    OGS_PFCP_UPDATE__ACCESS_FORWARDING_ACTION_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update__access_forwarding_action_information_t),
    {
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_weight,
        &ogs_pfcp_tlv_desc_priority,
        &ogs_pfcp_tlv_desc_urr_id,
        &ogs_pfcp_tlv_desc_rat_type,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_non__access_forwarding_action_information =
{
    OGS_TLV_COMPOUND,
    "Update Non-3GPP Access Forwarding Action Information",
    OGS_PFCP_UPDATE_NON__ACCESS_FORWARDING_ACTION_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_non__access_forwarding_action_information_t),
    {
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_weight,
        &ogs_pfcp_tlv_desc_priority,
        &ogs_pfcp_tlv_desc_urr_id,
        &ogs_pfcp_tlv_desc_rat_type,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_access_availability_report =
{
    OGS_TLV_COMPOUND,
    "Access Availability Report",
    OGS_PFCP_ACCESS_AVAILABILITY_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_access_availability_report_t),
    {
        &ogs_pfcp_tlv_desc_access_availability_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_qos_monitoring_report =
{
    OGS_TLV_COMPOUND,
    "QoS Monitoring Report",
    OGS_PFCP_QOS_MONITORING_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_qos_monitoring_report_t),
    {
        &ogs_pfcp_tlv_desc_qfi,
        &ogs_pfcp_tlv_desc_qos_monitoring_measurement,
        &ogs_pfcp_tlv_desc_time_stamp,
        &ogs_pfcp_tlv_desc_start_time,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mptcp_parameters =
{
    OGS_TLV_COMPOUND,
    "MPTCP Parameters",
    OGS_PFCP_MPTCP_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mptcp_parameters_t),
    {
        &ogs_pfcp_tlv_desc_mptcp_address_information,
        &ogs_pfcp_tlv_desc_ue_link_specific_ip_address,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_atsss_ll_parameters =
{
    OGS_TLV_COMPOUND,
    "ATSSS-LL Parameters",
    OGS_PFCP_ATSSS_LL_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_atsss_ll_parameters_t),
    {
        &ogs_pfcp_tlv_desc_atsss_ll_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pmf_parameters =
{
    OGS_TLV_COMPOUND,
    "PMF Parameters",
    OGS_PFCP_PMF_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pmf_parameters_t),
    {
        &ogs_pfcp_tlv_desc_pmf_address_information,
        &ogs_pfcp_tlv_desc_qfi,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_join_ip_multicast_information_ie_within_usage_report =
{
    OGS_TLV_COMPOUND,
    "Join IP Multicast Information IE within Usage Report",
    OGS_PFCP_JOIN_IP_MULTICAST_INFORMATION_IE_WITHIN_USAGE_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_join_ip_multicast_information_ie_within_usage_report_t),
    {
        &ogs_pfcp_tlv_desc_ip_multicast_address,
        &ogs_pfcp_tlv_desc_source_ip_address,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_leave_ip_multicast_information_ie_within_usage_report =
{
    OGS_TLV_COMPOUND,
    "Leave IP Multicast Information IE within Usage Report",
    OGS_PFCP_LEAVE_IP_MULTICAST_INFORMATION_IE_WITHIN_USAGE_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_leave_ip_multicast_information_ie_within_usage_report_t),
    {
        &ogs_pfcp_tlv_desc_ip_multicast_address,
        &ogs_pfcp_tlv_desc_source_ip_address,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_urr =
{
    OGS_TLV_COMPOUND,
    "Create URR",
    OGS_PFCP_CREATE_URR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_urr_t),
    {
        &ogs_pfcp_tlv_desc_urr_id,
        &ogs_pfcp_tlv_desc_measurement_method,
        &ogs_pfcp_tlv_desc_reporting_triggers,
        &ogs_pfcp_tlv_desc_measurement_period,
        &ogs_pfcp_tlv_desc_volume_threshold,
        &ogs_pfcp_tlv_desc_volume_quota,
        &ogs_pfcp_tlv_desc_event_threshold,
        &ogs_pfcp_tlv_desc_event_quota,
        &ogs_pfcp_tlv_desc_time_threshold,
        &ogs_pfcp_tlv_desc_time_quota,
        &ogs_pfcp_tlv_desc_quota_holding_time,
        &ogs_pfcp_tlv_desc_dropped_dl_traffic_threshold,
        &ogs_pfcp_tlv_desc_quota_validity_time,
        &ogs_pfcp_tlv_desc_monitoring_time,
        &ogs_pfcp_tlv_desc_subsequent_volume_threshold,
        &ogs_pfcp_tlv_desc_subsequent_time_threshold,
        &ogs_pfcp_tlv_desc_subsequent_volume_quota,
        &ogs_pfcp_tlv_desc_subsequent_time_quota,
        &ogs_pfcp_tlv_desc_subsequent_event_threshold,
        &ogs_pfcp_tlv_desc_subsequent_event_quota,
        &ogs_pfcp_tlv_desc_inactivity_detection_time,
        &ogs_pfcp_tlv_desc_linked_urr_id,
        &ogs_pfcp_tlv_desc_measurement_information,
        &ogs_pfcp_tlv_desc_time_quota_mechanism,
        &ogs_pfcp_tlv_desc_aggregated_urrs,
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_ethernet_inactivity_timer,
        &ogs_pfcp_tlv_desc_additional_monitoring_time,
        &ogs_pfcp_tlv_desc_number_of_reports,
        &ogs_pfcp_tlv_desc_application_id,
        &ogs_pfcp_tlv_desc_sdf_filter,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_user_plane_inactivity_timer,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_qer =
{
    OGS_TLV_COMPOUND,
    "Create QER",
    OGS_PFCP_CREATE_QER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_qer_t),
    {
        &ogs_pfcp_tlv_desc_qer_id,
        &ogs_pfcp_tlv_desc_qer_correlation_id,
        &ogs_pfcp_tlv_desc_gate_status,
        &ogs_pfcp_tlv_desc_mbr,
        &ogs_pfcp_tlv_desc_gbr,
        &ogs_pfcp_tlv_desc_packet_rate,
        &ogs_pfcp_tlv_desc_packet_rate_status,
        &ogs_pfcp_tlv_desc_dl_flow_level_marking,
        &ogs_pfcp_tlv_desc_qfi,
        &ogs_pfcp_tlv_desc_rqi,
        &ogs_pfcp_tlv_desc_paging_policy_indicator,
        &ogs_pfcp_tlv_desc_averaging_window,
        &ogs_pfcp_tlv_desc_qer_control_indications,
        &ogs_pfcp_tlv_desc_qer_indications,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_created_pdr =
{
    OGS_TLV_COMPOUND,
    "Created PDR",
    OGS_PFCP_CREATED_PDR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_created_pdr_t),
    {
        &ogs_pfcp_tlv_desc_pdr_id,
        &ogs_pfcp_tlv_desc_f_teid,
        &ogs_pfcp_tlv_desc_f_teid,
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_local_ingress_tunnel,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_pdr =
{
    OGS_TLV_COMPOUND,
    "Update PDR",
    OGS_PFCP_UPDATE_PDR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_pdr_t),
    {
        &ogs_pfcp_tlv_desc_pdr_id,
        &ogs_pfcp_tlv_desc_outer_header_removal,
        &ogs_pfcp_tlv_desc_precedence,
        &ogs_pfcp_tlv_desc_pdi,
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_urr_id,
        &ogs_pfcp_tlv_desc_qer_id,
        &ogs_pfcp_tlv_desc_activate_predefined_rules,
        &ogs_pfcp_tlv_desc_deactivate_predefined_rules,
        &ogs_pfcp_tlv_desc_activation_time,
        &ogs_pfcp_tlv_desc_deactivation_time,
        &ogs_pfcp_tlv_desc_ip_multicast_addressing_info_within_pfcp_session_establishment_request,
        &ogs_pfcp_tlv_desc_transport_delay_reporting,
        &ogs_pfcp_tlv_desc_rat_type,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_bar_pfcp_session_report_response =
{
    OGS_TLV_COMPOUND,
    "Update BAR PFCP Session Report Response",
    OGS_PFCP_UPDATE_BAR_PFCP_SESSION_REPORT_RESPONSE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_bar_pfcp_session_report_response_t),
    {
        &ogs_pfcp_tlv_desc_bar_id,
        &ogs_pfcp_tlv_desc_downlink_data_notification_delay,
        &ogs_pfcp_tlv_desc_dl_buffering_duration,
        &ogs_pfcp_tlv_desc_dl_buffering_suggested_packet_count,
        &ogs_pfcp_tlv_desc_suggested_buffering_packets_count,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_urr =
{
    OGS_TLV_COMPOUND,
    "Update URR",
    OGS_PFCP_UPDATE_URR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_urr_t),
    {
        &ogs_pfcp_tlv_desc_urr_id,
        &ogs_pfcp_tlv_desc_measurement_method,
        &ogs_pfcp_tlv_desc_reporting_triggers,
        &ogs_pfcp_tlv_desc_measurement_period,
        &ogs_pfcp_tlv_desc_volume_threshold,
        &ogs_pfcp_tlv_desc_volume_quota,
        &ogs_pfcp_tlv_desc_time_threshold,
        &ogs_pfcp_tlv_desc_time_quota,
        &ogs_pfcp_tlv_desc_event_threshold,
        &ogs_pfcp_tlv_desc_event_quota,
        &ogs_pfcp_tlv_desc_quota_holding_time,
        &ogs_pfcp_tlv_desc_dropped_dl_traffic_threshold,
        &ogs_pfcp_tlv_desc_quota_validity_time,
        &ogs_pfcp_tlv_desc_monitoring_time,
        &ogs_pfcp_tlv_desc_subsequent_volume_threshold,
        &ogs_pfcp_tlv_desc_subsequent_time_threshold,
        &ogs_pfcp_tlv_desc_subsequent_volume_quota,
        &ogs_pfcp_tlv_desc_subsequent_time_quota,
        &ogs_pfcp_tlv_desc_subsequent_event_threshold,
        &ogs_pfcp_tlv_desc_subsequent_event_quota,
        &ogs_pfcp_tlv_desc_inactivity_detection_time,
        &ogs_pfcp_tlv_desc_linked_urr_id,
        &ogs_pfcp_tlv_desc_measurement_information,
        &ogs_pfcp_tlv_desc_time_quota_mechanism,
        &ogs_pfcp_tlv_desc_aggregated_urrs,
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_ethernet_inactivity_timer,
        &ogs_pfcp_tlv_desc_additional_monitoring_time,
        &ogs_pfcp_tlv_desc_number_of_reports,
        &ogs_pfcp_tlv_desc_application_id,
        &ogs_pfcp_tlv_desc_sdf_filter,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_user_plane_inactivity_timer,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_qer =
{
    OGS_TLV_COMPOUND,
    "Update QER",
    OGS_PFCP_UPDATE_QER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_qer_t),
    {
        &ogs_pfcp_tlv_desc_qer_id,
        &ogs_pfcp_tlv_desc_qer_correlation_id,
        &ogs_pfcp_tlv_desc_gate_status,
        &ogs_pfcp_tlv_desc_mbr,
        &ogs_pfcp_tlv_desc_gbr,
        &ogs_pfcp_tlv_desc_packet_rate,
        &ogs_pfcp_tlv_desc_dl_flow_level_marking,
        &ogs_pfcp_tlv_desc_qfi,
        &ogs_pfcp_tlv_desc_rqi,
        &ogs_pfcp_tlv_desc_paging_policy_indicator,
        &ogs_pfcp_tlv_desc_averaging_window,
        &ogs_pfcp_tlv_desc_qer_control_indications,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_pdr =
{
    OGS_TLV_COMPOUND,
    "Remove PDR",
    OGS_PFCP_REMOVE_PDR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_pdr_t),
    {
        &ogs_pfcp_tlv_desc_pdr_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_far =
{
    OGS_TLV_COMPOUND,
    "Remove FAR",
    OGS_PFCP_REMOVE_FAR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_far_t),
    {
        &ogs_pfcp_tlv_desc_far_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_urr =
{
    OGS_TLV_COMPOUND,
    "Remove URR",
    OGS_PFCP_REMOVE_URR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_urr_t),
    {
        &ogs_pfcp_tlv_desc_urr_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_qer =
{
    OGS_TLV_COMPOUND,
    "Remove QER",
    OGS_PFCP_REMOVE_QER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_qer_t),
    {
        &ogs_pfcp_tlv_desc_qer_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_load_control_information =
{
    OGS_TLV_COMPOUND,
    "Load Control Information",
    OGS_PFCP_LOAD_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_load_control_information_t),
    {
        &ogs_pfcp_tlv_desc_sequence_number,
        &ogs_pfcp_tlv_desc_metric,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_overload_control_information =
{
    OGS_TLV_COMPOUND,
    "Overload Control Information",
    OGS_PFCP_OVERLOAD_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_overload_control_information_t),
    {
        &ogs_pfcp_tlv_desc_sequence_number,
        &ogs_pfcp_tlv_desc_metric,
        &ogs_pfcp_tlv_desc_timer,
        &ogs_pfcp_tlv_desc_oci_flags,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_application_detection_information =
{
    OGS_TLV_COMPOUND,
    "Application Detection Information",
    OGS_PFCP_APPLICATION_DETECTION_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_application_detection_information_t),
    {
        &ogs_pfcp_tlv_desc_application_id,
        &ogs_pfcp_tlv_desc_application_instance_id,
        &ogs_pfcp_tlv_desc_flow_information,
        &ogs_pfcp_tlv_desc_pdr_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_query_urr =
{
    OGS_TLV_COMPOUND,
    "Query URR",
    OGS_PFCP_QUERY_URR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_query_urr_t),
    {
        &ogs_pfcp_tlv_desc_urr_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_report_session_modification_response =
{
    OGS_TLV_COMPOUND,
    "Usage Report Session Modification Response",
    OGS_PFCP_USAGE_REPORT_SESSION_MODIFICATION_RESPONSE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_usage_report_session_modification_response_t),
    {
        &ogs_pfcp_tlv_desc_urr_id,
        &ogs_pfcp_tlv_desc_ur_seqn,
        &ogs_pfcp_tlv_desc_usage_report_trigger,
        &ogs_pfcp_tlv_desc_start_time,
        &ogs_pfcp_tlv_desc_end_time,
        &ogs_pfcp_tlv_desc_volume_measurement,
        &ogs_pfcp_tlv_desc_duration_measurement,
        &ogs_pfcp_tlv_desc_time_of_first_packet,
        &ogs_pfcp_tlv_desc_time_of_last_packet,
        &ogs_pfcp_tlv_desc_usage_information,
        &ogs_pfcp_tlv_desc_query_urr_reference,
        &ogs_pfcp_tlv_desc_ethernet_traffic_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_report_session_deletion_response =
{
    OGS_TLV_COMPOUND,
    "Usage Report Session Deletion Response",
    OGS_PFCP_USAGE_REPORT_SESSION_DELETION_RESPONSE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_usage_report_session_deletion_response_t),
    {
        &ogs_pfcp_tlv_desc_urr_id,
        &ogs_pfcp_tlv_desc_ur_seqn,
        &ogs_pfcp_tlv_desc_usage_report_trigger,
        &ogs_pfcp_tlv_desc_start_time,
        &ogs_pfcp_tlv_desc_end_time,
        &ogs_pfcp_tlv_desc_volume_measurement,
        &ogs_pfcp_tlv_desc_duration_measurement,
        &ogs_pfcp_tlv_desc_time_of_first_packet,
        &ogs_pfcp_tlv_desc_time_of_last_packet,
        &ogs_pfcp_tlv_desc_usage_information,
        &ogs_pfcp_tlv_desc_ethernet_traffic_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_report_session_report_request =
{
    OGS_TLV_COMPOUND,
    "Usage Report Session Report Request",
    OGS_PFCP_USAGE_REPORT_SESSION_REPORT_REQUEST_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_usage_report_session_report_request_t),
    {
        &ogs_pfcp_tlv_desc_urr_id,
        &ogs_pfcp_tlv_desc_ur_seqn,
        &ogs_pfcp_tlv_desc_usage_report_trigger,
        &ogs_pfcp_tlv_desc_start_time,
        &ogs_pfcp_tlv_desc_end_time,
        &ogs_pfcp_tlv_desc_volume_measurement,
        &ogs_pfcp_tlv_desc_duration_measurement,
        &ogs_pfcp_tlv_desc_application_detection_information,
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_network_instance,
        &ogs_pfcp_tlv_desc_time_of_first_packet,
        &ogs_pfcp_tlv_desc_time_of_last_packet,
        &ogs_pfcp_tlv_desc_usage_information,
        &ogs_pfcp_tlv_desc_query_urr_reference,
        &ogs_pfcp_tlv_desc_time_stamp,
        &ogs_pfcp_tlv_desc_ethernet_traffic_information,
        &ogs_pfcp_tlv_desc_join_ip_multicast_information_ie_within_usage_report,
        &ogs_pfcp_tlv_desc_leave_ip_multicast_information_ie_within_usage_report,
        &ogs_pfcp_tlv_desc_predefined_rules_name,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_downlink_data_report =
{
    OGS_TLV_COMPOUND,
    "Downlink Data Report",
    OGS_PFCP_DOWNLINK_DATA_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_downlink_data_report_t),
    {
        &ogs_pfcp_tlv_desc_pdr_id,
        &ogs_pfcp_tlv_desc_downlink_data_service_information,
        &ogs_pfcp_tlv_desc_dl_data_packets_size,
        &ogs_pfcp_tlv_desc_data_status,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_bar =
{
    OGS_TLV_COMPOUND,
    "Create BAR",
    OGS_PFCP_CREATE_BAR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_bar_t),
    {
        &ogs_pfcp_tlv_desc_bar_id,
        &ogs_pfcp_tlv_desc_downlink_data_notification_delay,
        &ogs_pfcp_tlv_desc_suggested_buffering_packets_count,
        &ogs_pfcp_tlv_desc_mt_edt_control_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_bar_session_modification_request =
{
    OGS_TLV_COMPOUND,
    "Update BAR Session Modification Request",
    OGS_PFCP_UPDATE_BAR_SESSION_MODIFICATION_REQUEST_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_bar_session_modification_request_t),
    {
        &ogs_pfcp_tlv_desc_bar_id,
        &ogs_pfcp_tlv_desc_downlink_data_notification_delay,
        &ogs_pfcp_tlv_desc_suggested_buffering_packets_count,
        &ogs_pfcp_tlv_desc_mt_edt_control_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_bar =
{
    OGS_TLV_COMPOUND,
    "Remove BAR",
    OGS_PFCP_REMOVE_BAR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_bar_t),
    {
        &ogs_pfcp_tlv_desc_bar_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_error_indication_report =
{
    OGS_TLV_COMPOUND,
    "Error Indication Report",
    OGS_PFCP_ERROR_INDICATION_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_error_indication_report_t),
    {
        &ogs_pfcp_tlv_desc_f_teid,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_user_plane_path_failure_report =
{
    OGS_TLV_COMPOUND,
    "User Plane Path Failure Report",
    OGS_PFCP_USER_PLANE_PATH_FAILURE_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_user_plane_path_failure_report_t),
    {
        &ogs_pfcp_tlv_desc_remote_gtp_u_peer,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_traffic_endpoint =
{
    OGS_TLV_COMPOUND,
    "Create Traffic Endpoint",
    OGS_PFCP_CREATE_TRAFFIC_ENDPOINT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_traffic_endpoint_t),
    {
        &ogs_pfcp_tlv_desc_traffic_endpoint_id,
        &ogs_pfcp_tlv_desc_f_teid,
        &ogs_pfcp_tlv_desc_network_instance,
        &ogs_pfcp_tlv_desc_redundant_transmission_parameters,
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_ethernet_pdu_session_information,
        &ogs_pfcp_tlv_desc_framed_route,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_framed_routing,
        &ogs_pfcp_tlv_desc_framed_ipv6_route,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_qfi,
        &ogs_pfcp_tlv_desc__interface_type,
        &ogs_pfcp_tlv_desc_local_ingress_tunnel,
        &ogs_pfcp_tlv_desc_ip_multicast_addressing_info_within_pfcp_session_establishment_request,
        &ogs_pfcp_tlv_desc_mbs_session_identifier,
        &ogs_pfcp_tlv_desc_area_session_id,
        &ogs_pfcp_tlv_desc_rat_type,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_created_traffic_endpoint =
{
    OGS_TLV_COMPOUND,
    "Created Traffic Endpoint",
    OGS_PFCP_CREATED_TRAFFIC_ENDPOINT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_created_traffic_endpoint_t),
    {
        &ogs_pfcp_tlv_desc_traffic_endpoint_id,
        &ogs_pfcp_tlv_desc_f_teid,
        &ogs_pfcp_tlv_desc_f_teid,
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_local_ingress_tunnel,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_traffic_endpoint =
{
    OGS_TLV_COMPOUND,
    "Remove Traffic Endpoint",
    OGS_PFCP_REMOVE_TRAFFIC_ENDPOINT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_traffic_endpoint_t),
    {
        &ogs_pfcp_tlv_desc_traffic_endpoint_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_mar =
{
    OGS_TLV_COMPOUND,
    "Create MAR",
    OGS_PFCP_CREATE_MAR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_mar_t),
    {
        &ogs_pfcp_tlv_desc_mar_id,
        &ogs_pfcp_tlv_desc_steering_functionality,
        &ogs_pfcp_tlv_desc_steering_mode,
        &ogs_pfcp_tlv_desc__access_forwarding_action_information,
        &ogs_pfcp_tlv_desc_non__access_forwarding_action_information,
        &ogs_pfcp_tlv_desc_thresholds,
        &ogs_pfcp_tlv_desc_steering_mode_indicator,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_mar =
{
    OGS_TLV_COMPOUND,
    "Remove MAR",
    OGS_PFCP_REMOVE_MAR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_mar_t),
    {
        &ogs_pfcp_tlv_desc_mar_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_mar =
{
    OGS_TLV_COMPOUND,
    "Update MAR",
    OGS_PFCP_UPDATE_MAR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_mar_t),
    {
        &ogs_pfcp_tlv_desc_mar_id,
        &ogs_pfcp_tlv_desc_steering_functionality,
        &ogs_pfcp_tlv_desc_steering_mode,
        &ogs_pfcp_tlv_desc_update__access_forwarding_action_information,
        &ogs_pfcp_tlv_desc_update_non__access_forwarding_action_information,
        &ogs_pfcp_tlv_desc__access_forwarding_action_information,
        &ogs_pfcp_tlv_desc_non__access_forwarding_action_information,
        &ogs_pfcp_tlv_desc_thresholds,
        &ogs_pfcp_tlv_desc_steering_mode_indicator,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_session_retention_information_within_pfcp_association_setup_request =
{
    OGS_TLV_COMPOUND,
    "PFCP Session Retention Information within PFCP Association Setup Request",
    OGS_PFCP_PFCP_SESSION_RETENTION_INFORMATION_WITHIN_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pfcp_session_retention_information_within_pfcp_association_setup_request_t),
    {
        &ogs_pfcp_tlv_desc_cp_pfcp_entity_ip_address,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_user_plane_path_recovery_report =
{
    OGS_TLV_COMPOUND,
    "User Plane Path Recovery Report",
    OGS_PFCP_USER_PLANE_PATH_RECOVERY_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_user_plane_path_recovery_report_t),
    {
        &ogs_pfcp_tlv_desc_remote_gtp_u_peer,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_created_bridge_info_for_tsc =
{
    OGS_TLV_COMPOUND,
    "Created Bridge Info for TSC",
    OGS_PFCP_CREATED_BRIDGE_INFO_FOR_TSC_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_created_bridge_info_for_tsc_t),
    {
        &ogs_pfcp_tlv_desc_ds_tt_port_number,
        &ogs_pfcp_tlv_desc_fivegs_user_plane_node,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_tsc_management_information_ie_within_pfcp_session_modification_request =
{
    OGS_TLV_COMPOUND,
    "TSC Management Information IE within PFCP Session Modification Request",
    OGS_PFCP_TSC_MANAGEMENT_INFORMATION_IE_WITHIN_PFCP_SESSION_MODIFICATION_REQUEST_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_tsc_management_information_ie_within_pfcp_session_modification_request_t),
    {
        &ogs_pfcp_tlv_desc_port_management_information_container,
        &ogs_pfcp_tlv_desc_bridge_management_information_container,
        &ogs_pfcp_tlv_desc_nw_tt_port_number,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_tsc_management_information_ie_within_pfcp_session_modification_response =
{
    OGS_TLV_COMPOUND,
    "TSC Management Information IE within PFCP Session Modification Response",
    OGS_PFCP_TSC_MANAGEMENT_INFORMATION_IE_WITHIN_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_tsc_management_information_ie_within_pfcp_session_modification_response_t),
    {
        &ogs_pfcp_tlv_desc_port_management_information_container,
        &ogs_pfcp_tlv_desc_bridge_management_information_container,
        &ogs_pfcp_tlv_desc_nw_tt_port_number,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_tsc_management_information_ie_within_pfcp_session_report_request =
{
    OGS_TLV_COMPOUND,
    "TSC Management Information IE within PFCP Session Report Request",
    OGS_PFCP_TSC_MANAGEMENT_INFORMATION_IE_WITHIN_PFCP_SESSION_REPORT_REQUEST_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_tsc_management_information_ie_within_pfcp_session_report_request_t),
    {
        &ogs_pfcp_tlv_desc_port_management_information_container,
        &ogs_pfcp_tlv_desc_bridge_management_information_container,
        &ogs_pfcp_tlv_desc_nw_tt_port_number,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_clock_drift_control_information =
{
    OGS_TLV_COMPOUND,
    "Clock Drift Control Information",
    OGS_PFCP_CLOCK_DRIFT_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_clock_drift_control_information_t),
    {
        &ogs_pfcp_tlv_desc_requested_clock_drift_information,
        &ogs_pfcp_tlv_desc_time_domain_number,
        &ogs_pfcp_tlv_desc_configured_time_domain,
        &ogs_pfcp_tlv_desc_time_offset_threshold,
        &ogs_pfcp_tlv_desc_cumulative_rateratio_threshold,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_clock_drift_report =
{
    OGS_TLV_COMPOUND,
    "Clock Drift Report",
    OGS_PFCP_CLOCK_DRIFT_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_clock_drift_report_t),
    {
        &ogs_pfcp_tlv_desc_time_domain_number,
        &ogs_pfcp_tlv_desc_time_offset_measurement,
        &ogs_pfcp_tlv_desc_cumulative_rateratio_measurement,
        &ogs_pfcp_tlv_desc_time_stamp,
        &ogs_pfcp_tlv_desc_network_instance,
        &ogs_pfcp_tlv_desc_apn_dnn,
        &ogs_pfcp_tlv_desc_s_nssai,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_srr =
{
    OGS_TLV_COMPOUND,
    "Remove SRR",
    OGS_PFCP_REMOVE_SRR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_srr_t),
    {
        &ogs_pfcp_tlv_desc_srr_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_srr =
{
    OGS_TLV_COMPOUND,
    "Create SRR",
    OGS_PFCP_CREATE_SRR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_srr_t),
    {
        &ogs_pfcp_tlv_desc_srr_id,
        &ogs_pfcp_tlv_desc_access_availability_control_information,
        &ogs_pfcp_tlv_desc_qos_monitoring_per_qos_flow_control_information,
        &ogs_pfcp_tlv_desc_direct_reporting_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_srr =
{
    OGS_TLV_COMPOUND,
    "Update SRR",
    OGS_PFCP_UPDATE_SRR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_srr_t),
    {
        &ogs_pfcp_tlv_desc_srr_id,
        &ogs_pfcp_tlv_desc_access_availability_control_information,
        &ogs_pfcp_tlv_desc_qos_monitoring_per_qos_flow_control_information,
        &ogs_pfcp_tlv_desc_direct_reporting_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_session_report =
{
    OGS_TLV_COMPOUND,
    "Session Report",
    OGS_PFCP_SESSION_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_session_report_t),
    {
        &ogs_pfcp_tlv_desc_srr_id,
        &ogs_pfcp_tlv_desc_access_availability_report,
        &ogs_pfcp_tlv_desc_qos_monitoring_report,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_provide_atsss_control_information =
{
    OGS_TLV_COMPOUND,
    "Provide ATSSS Control Information",
    OGS_PFCP_PROVIDE_ATSSS_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_provide_atsss_control_information_t),
    {
        &ogs_pfcp_tlv_desc_mptcp_control_information,
        &ogs_pfcp_tlv_desc_atsss_ll_control_information,
        &ogs_pfcp_tlv_desc_pmf_control_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_atsss_control_parameters =
{
    OGS_TLV_COMPOUND,
    "ATSSS Control Parameters",
    OGS_PFCP_ATSSS_CONTROL_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_atsss_control_parameters_t),
    {
        &ogs_pfcp_tlv_desc_mptcp_parameters,
        &ogs_pfcp_tlv_desc_atsss_ll_parameters,
        &ogs_pfcp_tlv_desc_pmf_parameters,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ue_ip_address_pool_information =
{
    OGS_TLV_COMPOUND,
    "UE IP address Pool Information",
    OGS_PFCP_UE_IP_ADDRESS_POOL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ue_ip_address_pool_information_t),
    {
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_network_instance,
        &ogs_pfcp_tlv_desc_s_nssai,
        &ogs_pfcp_tlv_desc_ip_version,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_gtp_u_path_qos_report_pfcp_node_report_request =
{
    OGS_TLV_COMPOUND,
    "GTP-U Path QoS Report PFCP Node Report Request",
    OGS_PFCP_GTP_U_PATH_QOS_REPORT_PFCP_NODE_REPORT_REQUEST_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_gtp_u_path_qos_report_pfcp_node_report_request_t),
    {
        &ogs_pfcp_tlv_desc_remote_gtp_u_peer,
        &ogs_pfcp_tlv_desc_gtp_u_path_interface_type,
        &ogs_pfcp_tlv_desc_qos_report_trigger,
        &ogs_pfcp_tlv_desc_transport_level_marking,
        &ogs_pfcp_tlv_desc_measurement_period,
        &ogs_pfcp_tlv_desc_average_packet_delay,
        &ogs_pfcp_tlv_desc_minimum_packet_delay,
        &ogs_pfcp_tlv_desc_maximum_packet_delay,
        &ogs_pfcp_tlv_desc_timer,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_qos_information_in_gtp_u_path_qos_report =
{
    OGS_TLV_COMPOUND,
    "QoS Information in GTP-U Path QoS Report",
    OGS_PFCP_QOS_INFORMATION_IN_GTP_U_PATH_QOS_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_qos_information_in_gtp_u_path_qos_report_t),
    {
        &ogs_pfcp_tlv_desc_average_packet_delay,
        &ogs_pfcp_tlv_desc_minimum_packet_delay,
        &ogs_pfcp_tlv_desc_maximum_packet_delay,
        &ogs_pfcp_tlv_desc_transport_level_marking,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_packet_rate_status_report =
{
    OGS_TLV_COMPOUND,
    "Packet Rate Status Report",
    OGS_PFCP_PACKET_RATE_STATUS_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_packet_rate_status_report_t),
    {
        &ogs_pfcp_tlv_desc_qer_id,
        &ogs_pfcp_tlv_desc_packet_rate_status,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ethernet_context_information =
{
    OGS_TLV_COMPOUND,
    "Ethernet Context Information",
    OGS_PFCP_ETHERNET_CONTEXT_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ethernet_context_information_t),
    {
        &ogs_pfcp_tlv_desc_mac_addresses_detected,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_updated_pdr =
{
    OGS_TLV_COMPOUND,
    "Updated PDR",
    OGS_PFCP_UPDATED_PDR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_updated_pdr_t),
    {
        &ogs_pfcp_tlv_desc_pdr_id,
        &ogs_pfcp_tlv_desc_f_teid,
        &ogs_pfcp_tlv_desc_f_teid,
        &ogs_pfcp_tlv_desc_ue_ip_address,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_provide_rds_configuration_information =
{
    OGS_TLV_COMPOUND,
    "Provide RDS Configuration Information",
    OGS_PFCP_PROVIDE_RDS_CONFIGURATION_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_provide_rds_configuration_information_t),
    {
        &ogs_pfcp_tlv_desc_rds_configuration_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_query_packet_rate_status_ie_within_pfcp_session_modification_request =
{
    OGS_TLV_COMPOUND,
    "Query Packet Rate Status IE within PFCP Session Modification Request",
    OGS_PFCP_QUERY_PACKET_RATE_STATUS_IE_WITHIN_PFCP_SESSION_MODIFICATION_REQUEST_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_query_packet_rate_status_ie_within_pfcp_session_modification_request_t),
    {
        &ogs_pfcp_tlv_desc_qer_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_packet_rate_status_report_ie_within_pfcp_session_modification_response =
{
    OGS_TLV_COMPOUND,
    "Packet Rate Status Report IE within PFCP Session Modification Response",
    OGS_PFCP_PACKET_RATE_STATUS_REPORT_IE_WITHIN_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_packet_rate_status_report_ie_within_pfcp_session_modification_response_t),
    {
        &ogs_pfcp_tlv_desc_qer_id,
        &ogs_pfcp_tlv_desc_packet_rate_status,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ue_ip_address_usage_information =
{
    OGS_TLV_COMPOUND,
    "UE IP Address Usage Information",
    OGS_PFCP_UE_IP_ADDRESS_USAGE_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ue_ip_address_usage_information_t),
    {
        &ogs_pfcp_tlv_desc_sequence_number,
        &ogs_pfcp_tlv_desc_metric,
        &ogs_pfcp_tlv_desc_validity_timer,
        &ogs_pfcp_tlv_desc_number_of_ue_ip_addresses,
        &ogs_pfcp_tlv_desc_network_instance,
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_s_nssai,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_partial_failure_information =
{
    OGS_TLV_COMPOUND,
    "Partial Failure Information",
    OGS_PFCP_PARTIAL_FAILURE_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_partial_failure_information_t),
    {
        &ogs_pfcp_tlv_desc_failed_rule_id,
        &ogs_pfcp_tlv_desc_cause,
        &ogs_pfcp_tlv_desc_offending_ie_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_l2tp_tunnel_information =
{
    OGS_TLV_COMPOUND,
    "L2TP Tunnel Information",
    OGS_PFCP_L2TP_TUNNEL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_l2tp_tunnel_information_t),
    {
        &ogs_pfcp_tlv_desc_lns_address,
        &ogs_pfcp_tlv_desc_tunnel_password,
        &ogs_pfcp_tlv_desc_tunnel_preference,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_l2tp_session_information =
{
    OGS_TLV_COMPOUND,
    "L2TP Session Information",
    OGS_PFCP_L2TP_SESSION_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_l2tp_session_information_t),
    {
        &ogs_pfcp_tlv_desc_calling_number,
        &ogs_pfcp_tlv_desc_called_number,
        &ogs_pfcp_tlv_desc_maximum_receive_unit,
        &ogs_pfcp_tlv_desc_l2tp_session_indications,
        &ogs_pfcp_tlv_desc_l2tp_user_authentication_ie,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_created_l2tp_session =
{
    OGS_TLV_COMPOUND,
    "Created L2TP Session",
    OGS_PFCP_CREATED_L2TP_SESSION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_created_l2tp_session_t),
    {
        &ogs_pfcp_tlv_desc_dns_server_address,
        &ogs_pfcp_tlv_desc_nbns_server_address,
        &ogs_pfcp_tlv_desc_lns_address,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_session_change_info =
{
    OGS_TLV_COMPOUND,
    "PFCP Session Change Info",
    OGS_PFCP_PFCP_SESSION_CHANGE_INFO_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pfcp_session_change_info_t),
    {
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_group_id,
        &ogs_pfcp_tlv_desc_cp_ip_address,
        &ogs_pfcp_tlv_desc_alternative_smf_ip_address,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mbs_session_n4mb_control_information =
{
    OGS_TLV_COMPOUND,
    "MBS Session N4mb Control Information",
    OGS_PFCP_MBS_SESSION_N4MB_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mbs_session_n4mb_control_information_t),
    {
        &ogs_pfcp_tlv_desc_mbs_session_identifier,
        &ogs_pfcp_tlv_desc_area_session_id,
        &ogs_pfcp_tlv_desc_mbsn4mbreq_flags,
        &ogs_pfcp_tlv_desc_multicast_transport_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mbs_session_n4mb_information =
{
    OGS_TLV_COMPOUND,
    "MBS Session N4mb Information",
    OGS_PFCP_MBS_SESSION_N4MB_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mbs_session_n4mb_information_t),
    {
        &ogs_pfcp_tlv_desc_multicast_transport_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mbs_session_n4_control_information =
{
    OGS_TLV_COMPOUND,
    "MBS Session N4 Control Information",
    OGS_PFCP_MBS_SESSION_N4_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mbs_session_n4_control_information_t),
    {
        &ogs_pfcp_tlv_desc_mbs_session_identifier,
        &ogs_pfcp_tlv_desc_area_session_id,
        &ogs_pfcp_tlv_desc_multicast_transport_information,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mbs_session_n4_information =
{
    OGS_TLV_COMPOUND,
    "MBS Session N4 Information",
    OGS_PFCP_MBS_SESSION_N4_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mbs_session_n4_information_t),
    {
        &ogs_pfcp_tlv_desc_mbs_session_identifier,
        &ogs_pfcp_tlv_desc_area_session_id,
        &ogs_pfcp_tlv_desc_f_teid,
        &ogs_pfcp_tlv_desc_mbsn4resp_flags,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_dscp_to_ppi_control_information =
{
    OGS_TLV_COMPOUND,
    "DSCP to PPI Control Information",
    OGS_PFCP_DSCP_TO_PPI_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_dscp_to_ppi_control_information_t),
    {
        &ogs_pfcp_tlv_desc_dscp_to_ppi_mapping_information,
        &ogs_pfcp_tlv_desc_qfi,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_heartbeat_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Heartbeat Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_recovery_time_stamp,
        &ogs_pfcp_tlv_desc_source_ip_address,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_heartbeat_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Heartbeat Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_recovery_time_stamp,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_pfd_management_request =
{
    OGS_TLV_MESSAGE,
    "PFCP PFD Management Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_application_id_s_pfds,
        &ogs_pfcp_tlv_desc_node_id,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_pfd_management_response =
{
    OGS_TLV_MESSAGE,
    "PFCP PFD Management Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_cause,
        &ogs_pfcp_tlv_desc_offending_ie,
        &ogs_pfcp_tlv_desc_node_id,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_association_setup_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Association Setup Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_recovery_time_stamp,
        &ogs_pfcp_tlv_desc_up_function_features,
        &ogs_pfcp_tlv_desc_cp_function_features,
        &ogs_pfcp_tlv_desc_user_plane_ip_resource_information,
        &ogs_tlv_desc_more4,
        &ogs_pfcp_tlv_desc_alternative_smf_ip_address,
        &ogs_pfcp_tlv_desc_smf_set_id,
        &ogs_pfcp_tlv_desc_pfcp_session_retention_information_within_pfcp_association_setup_request,
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_gtp_u_path_qos_control_information,
        &ogs_pfcp_tlv_desc_clock_drift_control_information,
        &ogs_pfcp_tlv_desc_nf_instance_id,
        &ogs_pfcp_tlv_desc_pfcpasreq_flags,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_association_setup_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Association Setup Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_cause,
        &ogs_pfcp_tlv_desc_recovery_time_stamp,
        &ogs_pfcp_tlv_desc_up_function_features,
        &ogs_pfcp_tlv_desc_cp_function_features,
        &ogs_pfcp_tlv_desc_user_plane_ip_resource_information,
        &ogs_tlv_desc_more4,
        &ogs_pfcp_tlv_desc_alternative_smf_ip_address,
        &ogs_pfcp_tlv_desc_smf_set_id,
        &ogs_pfcp_tlv_desc_pfcpasrsp_flags,
        &ogs_pfcp_tlv_desc_clock_drift_control_information,
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_gtp_u_path_qos_control_information,
        &ogs_pfcp_tlv_desc_nf_instance_id,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_association_update_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Association Update Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_up_function_features,
        &ogs_pfcp_tlv_desc_cp_function_features,
        &ogs_pfcp_tlv_desc_user_plane_ip_resource_information,
        &ogs_tlv_desc_more4,
        &ogs_pfcp_tlv_desc_pfcp_association_release_request,
        &ogs_pfcp_tlv_desc_graceful_release_period,
        &ogs_pfcp_tlv_desc_pfcpaureq_flags,
        &ogs_pfcp_tlv_desc_alternative_smf_ip_address,
        &ogs_pfcp_tlv_desc_smf_set_id,
        &ogs_pfcp_tlv_desc_clock_drift_control_information,
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_gtp_u_path_qos_control_information,
        &ogs_pfcp_tlv_desc_ue_ip_address_usage_information,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_association_update_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Association Update Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_cause,
        &ogs_pfcp_tlv_desc_up_function_features,
        &ogs_pfcp_tlv_desc_cp_function_features,
        &ogs_pfcp_tlv_desc_user_plane_ip_resource_information,
        &ogs_tlv_desc_more4,
        &ogs_pfcp_tlv_desc_ue_ip_address_usage_information,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_association_release_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Association Release Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_association_release_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Association Release Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_version_not_supported_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Version Not Supported Response",
    0, 0, 0, 0, {
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_node_report_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Node Report Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_node_report_type,
        &ogs_pfcp_tlv_desc_vendor_specific_node_report_type,
        &ogs_pfcp_tlv_desc_user_plane_path_failure_report,
        &ogs_pfcp_tlv_desc_user_plane_path_recovery_report,
        &ogs_pfcp_tlv_desc_clock_drift_report,
        &ogs_pfcp_tlv_desc_gtp_u_path_qos_report_pfcp_node_report_request,
        &ogs_pfcp_tlv_desc_peer_up_restart_report,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_node_report_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Node Report Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_cause,
        &ogs_pfcp_tlv_desc_offending_ie,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_set_deletion_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Set Deletion Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_set_deletion_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Set Deletion Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_cause,
        &ogs_pfcp_tlv_desc_offending_ie,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_set_modification_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Set Modification Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_pfcp_session_change_info,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_set_modification_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Set Modification Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_cause,
        &ogs_pfcp_tlv_desc_offending_ie,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_establishment_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Establishment Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_f_seid,
        &ogs_pfcp_tlv_desc_create_pdr,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_create_far,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_create_urr,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_create_qer,
        &ogs_tlv_desc_more4,
        &ogs_pfcp_tlv_desc_create_bar,
        &ogs_pfcp_tlv_desc_create_traffic_endpoint,
        &ogs_pfcp_tlv_desc_pdn_type,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_user_plane_inactivity_timer,
        &ogs_pfcp_tlv_desc_user_id,
        &ogs_pfcp_tlv_desc_trace_information,
        &ogs_pfcp_tlv_desc_apn_dnn,
        &ogs_pfcp_tlv_desc_create_mar,
        &ogs_pfcp_tlv_desc_pfcpsereq_flags,
        &ogs_pfcp_tlv_desc_create_bridge_info_for_tsc,
        &ogs_pfcp_tlv_desc_create_srr,
        &ogs_pfcp_tlv_desc_provide_atsss_control_information,
        &ogs_pfcp_tlv_desc_recovery_time_stamp,
        &ogs_pfcp_tlv_desc_s_nssai,
        &ogs_pfcp_tlv_desc_provide_rds_configuration_information,
        &ogs_pfcp_tlv_desc_rat_type,
        &ogs_pfcp_tlv_desc_l2tp_tunnel_information,
        &ogs_pfcp_tlv_desc_l2tp_session_information,
        &ogs_pfcp_tlv_desc_group_id,
        &ogs_pfcp_tlv_desc_mbs_session_n4mb_control_information,
        &ogs_pfcp_tlv_desc_mbs_session_n4_control_information,
        &ogs_pfcp_tlv_desc_dscp_to_ppi_control_information,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_establishment_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Establishment Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_cause,
        &ogs_pfcp_tlv_desc_offending_ie,
        &ogs_pfcp_tlv_desc_f_seid,
        &ogs_pfcp_tlv_desc_created_pdr,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_load_control_information,
        &ogs_pfcp_tlv_desc_overload_control_information,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_failed_rule_id,
        &ogs_pfcp_tlv_desc_created_traffic_endpoint,
        &ogs_pfcp_tlv_desc_created_bridge_info_for_tsc,
        &ogs_pfcp_tlv_desc_atsss_control_parameters,
        &ogs_pfcp_tlv_desc_rds_configuration_information,
        &ogs_pfcp_tlv_desc_partial_failure_information,
        &ogs_pfcp_tlv_desc_created_l2tp_session,
        &ogs_pfcp_tlv_desc_mbs_session_n4mb_information,
        &ogs_pfcp_tlv_desc_mbs_session_n4_information,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_modification_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Modification Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_f_seid,
        &ogs_pfcp_tlv_desc_remove_pdr,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_remove_far,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_remove_urr,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_remove_qer,
        &ogs_tlv_desc_more4,
        &ogs_pfcp_tlv_desc_remove_bar,
        &ogs_pfcp_tlv_desc_remove_traffic_endpoint,
        &ogs_pfcp_tlv_desc_create_pdr,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_create_far,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_create_urr,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_create_qer,
        &ogs_tlv_desc_more4,
        &ogs_pfcp_tlv_desc_create_bar,
        &ogs_pfcp_tlv_desc_create_traffic_endpoint,
        &ogs_pfcp_tlv_desc_update_pdr,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_update_far,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_update_urr,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_update_qer,
        &ogs_tlv_desc_more4,
        &ogs_pfcp_tlv_desc_update_bar_session_modification_request,
        &ogs_pfcp_tlv_desc_update_traffic_endpoint,
        &ogs_pfcp_tlv_desc_pfcpsmreq_flags,
        &ogs_pfcp_tlv_desc_query_urr,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_user_plane_inactivity_timer,
        &ogs_pfcp_tlv_desc_query_urr_reference,
        &ogs_pfcp_tlv_desc_trace_information,
        &ogs_pfcp_tlv_desc_remove_mar,
        &ogs_pfcp_tlv_desc_update_mar,
        &ogs_pfcp_tlv_desc_create_mar,
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_tsc_management_information_ie_within_pfcp_session_modification_request,
        &ogs_pfcp_tlv_desc_remove_srr,
        &ogs_pfcp_tlv_desc_create_srr,
        &ogs_pfcp_tlv_desc_update_srr,
        &ogs_pfcp_tlv_desc_provide_atsss_control_information,
        &ogs_pfcp_tlv_desc_ethernet_context_information,
        &ogs_pfcp_tlv_desc_access_availability_information,
        &ogs_pfcp_tlv_desc_query_packet_rate_status_ie_within_pfcp_session_modification_request,
        &ogs_pfcp_tlv_desc_s_nssai,
        &ogs_pfcp_tlv_desc_rat_type,
        &ogs_pfcp_tlv_desc_group_id,
        &ogs_pfcp_tlv_desc_mbs_session_n4_control_information,
        &ogs_pfcp_tlv_desc_dscp_to_ppi_control_information,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_modification_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Modification Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_cause,
        &ogs_pfcp_tlv_desc_offending_ie,
        &ogs_pfcp_tlv_desc_created_pdr,
        &ogs_tlv_desc_more16,
        &ogs_pfcp_tlv_desc_load_control_information,
        &ogs_pfcp_tlv_desc_overload_control_information,
        &ogs_pfcp_tlv_desc_usage_report_session_modification_response,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_failed_rule_id,
        &ogs_pfcp_tlv_desc_additional_usage_reports_information,
        &ogs_pfcp_tlv_desc_created_traffic_endpoint,
        &ogs_pfcp_tlv_desc_tsc_management_information_ie_within_pfcp_session_modification_request,
        &ogs_pfcp_tlv_desc_atsss_control_parameters,
        &ogs_pfcp_tlv_desc_updated_pdr,
        &ogs_pfcp_tlv_desc_packet_rate_status_report,
        &ogs_pfcp_tlv_desc_partial_failure_information,
        &ogs_pfcp_tlv_desc_mbs_session_n4_information,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_deletion_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Deletion Request",
    0, 0, 0, 0, {
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_deletion_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Deletion Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_cause,
        &ogs_pfcp_tlv_desc_offending_ie,
        &ogs_pfcp_tlv_desc_load_control_information,
        &ogs_pfcp_tlv_desc_overload_control_information,
        &ogs_pfcp_tlv_desc_usage_report_session_deletion_response,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_additional_usage_reports_information,
        &ogs_pfcp_tlv_desc_packet_rate_status_report,
        &ogs_pfcp_tlv_desc_session_report,
        &ogs_pfcp_tlv_desc_mbs_session_n4_information,
        &ogs_pfcp_tlv_desc_pfcpsdrsp_flags,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_report_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Report Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_report_type,
        &ogs_pfcp_tlv_desc_downlink_data_report,
        &ogs_pfcp_tlv_desc_usage_report_session_report_request,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_error_indication_report,
        &ogs_pfcp_tlv_desc_load_control_information,
        &ogs_pfcp_tlv_desc_overload_control_information,
        &ogs_pfcp_tlv_desc_additional_usage_reports_information,
        &ogs_pfcp_tlv_desc_pfcpsrreq_flags,
        &ogs_pfcp_tlv_desc_f_seid,
        &ogs_pfcp_tlv_desc_packet_rate_status_report,
        &ogs_pfcp_tlv_desc_tsc_management_information_ie_within_pfcp_session_modification_request,
        &ogs_pfcp_tlv_desc_session_report,
        &ogs_pfcp_tlv_desc_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_report_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Report Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_cause,
        &ogs_pfcp_tlv_desc_offending_ie,
        &ogs_pfcp_tlv_desc_update_bar_pfcp_session_report_response,
        &ogs_pfcp_tlv_desc_pfcpsrrsp_flags,
        &ogs_pfcp_tlv_desc_f_seid,
        &ogs_pfcp_tlv_desc_f_teid,
        &ogs_pfcp_tlv_desc_alternative_smf_ip_address,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_group_id,
        &ogs_pfcp_tlv_desc_node_id,
    NULL,
}};


ogs_pfcp_message_t *ogs_pfcp_parse_msg(ogs_pkbuf_t *pkbuf)
{
    int rv = OGS_ERROR;
    ogs_pfcp_header_t *h = NULL;
    uint16_t size = 0;

    ogs_pfcp_message_t *pfcp_message = NULL;

    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);

    h = (ogs_pfcp_header_t *)pkbuf->data;
    ogs_assert(h);

    pfcp_message = ogs_calloc(1, sizeof(*pfcp_message));
    if (!pfcp_message) {
        ogs_error("No memory");
        return NULL;
    }

    if (h->seid_presence)
        size = OGS_PFCP_HEADER_LEN;
    else
        size = OGS_PFCP_HEADER_LEN-OGS_PFCP_SEID_LEN;

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
        ogs_error("ogs_pkbuf_pull() failed [len:%d]", pkbuf->len);
        ogs_pfcp_message_free(pfcp_message);
        return NULL;
    }
    memcpy(&pfcp_message->h, pkbuf->data - size, size);

    if (h->seid_presence) {
        pfcp_message->h.seid = be64toh(pfcp_message->h.seid);
    } else {
        pfcp_message->h.sqn = pfcp_message->h.sqn_only;
    }

    if (pkbuf->len == 0)
        return pfcp_message;

    switch(pfcp_message->h.type)
    {
        case OGS_PFCP_HEARTBEAT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_heartbeat_request,
                    &ogs_pfcp_msg_desc_pfcp_heartbeat_request, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_HEARTBEAT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_heartbeat_response,
                    &ogs_pfcp_msg_desc_pfcp_heartbeat_response, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_PFD_MANAGEMENT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_pfd_management_request,
                    &ogs_pfcp_msg_desc_pfcp_pfd_management_request, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_PFD_MANAGEMENT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_pfd_management_response,
                    &ogs_pfcp_msg_desc_pfcp_pfd_management_response, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_setup_request,
                    &ogs_pfcp_msg_desc_pfcp_association_setup_request, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_setup_response,
                    &ogs_pfcp_msg_desc_pfcp_association_setup_response, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_ASSOCIATION_UPDATE_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_update_request,
                    &ogs_pfcp_msg_desc_pfcp_association_update_request, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_ASSOCIATION_UPDATE_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_update_response,
                    &ogs_pfcp_msg_desc_pfcp_association_update_response, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_ASSOCIATION_RELEASE_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_release_request,
                    &ogs_pfcp_msg_desc_pfcp_association_release_request, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_ASSOCIATION_RELEASE_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_release_response,
                    &ogs_pfcp_msg_desc_pfcp_association_release_response, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_VERSION_NOT_SUPPORTED_RESPONSE_TYPE:
            break;
        case OGS_PFCP_NODE_REPORT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_node_report_request,
                    &ogs_pfcp_msg_desc_pfcp_node_report_request, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_NODE_REPORT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_node_report_response,
                    &ogs_pfcp_msg_desc_pfcp_node_report_response, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_SESSION_SET_DELETION_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_set_deletion_request,
                    &ogs_pfcp_msg_desc_pfcp_session_set_deletion_request, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_SESSION_SET_DELETION_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_set_deletion_response,
                    &ogs_pfcp_msg_desc_pfcp_session_set_deletion_response, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_SESSION_SET_MODIFICATION_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_set_modification_request,
                    &ogs_pfcp_msg_desc_pfcp_session_set_modification_request, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_SESSION_SET_MODIFICATION_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_set_modification_response,
                    &ogs_pfcp_msg_desc_pfcp_session_set_modification_response, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_establishment_request,
                    &ogs_pfcp_msg_desc_pfcp_session_establishment_request, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_establishment_response,
                    &ogs_pfcp_msg_desc_pfcp_session_establishment_response, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_modification_request,
                    &ogs_pfcp_msg_desc_pfcp_session_modification_request, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_modification_response,
                    &ogs_pfcp_msg_desc_pfcp_session_modification_response, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_SESSION_DELETION_REQUEST_TYPE:
            break;
        case OGS_PFCP_SESSION_DELETION_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_deletion_response,
                    &ogs_pfcp_msg_desc_pfcp_session_deletion_response, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_SESSION_REPORT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_report_request,
                    &ogs_pfcp_msg_desc_pfcp_session_report_request, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        case OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_report_response,
                    &ogs_pfcp_msg_desc_pfcp_session_report_response, pkbuf, OGS_TLV_MODE_T2_L2);
            ogs_expect(rv == OGS_OK);
            break;
        default:
            ogs_warn("Not implemented(type:%d)", pfcp_message->h.type);
            break;
    }

    if (rv != OGS_OK) {
        ogs_pfcp_message_free(pfcp_message);
        return NULL;
    }

    return pfcp_message;
}

void ogs_pfcp_message_free(ogs_pfcp_message_t *pfcp_message)
{
    ogs_assert(pfcp_message);
    ogs_free(pfcp_message);
}

ogs_pkbuf_t *ogs_pfcp_build_msg(ogs_pfcp_message_t *pfcp_message)
{
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(pfcp_message);
    switch(pfcp_message->h.type)
    {
        case OGS_PFCP_HEARTBEAT_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_heartbeat_request,
                    &pfcp_message->pfcp_heartbeat_request, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_HEARTBEAT_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_heartbeat_response,
                    &pfcp_message->pfcp_heartbeat_response, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFD_MANAGEMENT_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_pfd_management_request,
                    &pfcp_message->pfcp_pfd_management_request, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFD_MANAGEMENT_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_pfd_management_response,
                    &pfcp_message->pfcp_pfd_management_response, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_association_setup_request,
                    &pfcp_message->pfcp_association_setup_request, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_association_setup_response,
                    &pfcp_message->pfcp_association_setup_response, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_ASSOCIATION_UPDATE_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_association_update_request,
                    &pfcp_message->pfcp_association_update_request, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_ASSOCIATION_UPDATE_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_association_update_response,
                    &pfcp_message->pfcp_association_update_response, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_ASSOCIATION_RELEASE_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_association_release_request,
                    &pfcp_message->pfcp_association_release_request, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_ASSOCIATION_RELEASE_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_association_release_response,
                    &pfcp_message->pfcp_association_release_response, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_VERSION_NOT_SUPPORTED_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_version_not_supported_response,
                    &pfcp_message->pfcp_version_not_supported_response, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_NODE_REPORT_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_node_report_request,
                    &pfcp_message->pfcp_node_report_request, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_NODE_REPORT_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_node_report_response,
                    &pfcp_message->pfcp_node_report_response, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_SET_DELETION_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_session_set_deletion_request,
                    &pfcp_message->pfcp_session_set_deletion_request, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_SET_DELETION_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_session_set_deletion_response,
                    &pfcp_message->pfcp_session_set_deletion_response, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_SET_MODIFICATION_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_session_set_modification_request,
                    &pfcp_message->pfcp_session_set_modification_request, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_SET_MODIFICATION_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_session_set_modification_response,
                    &pfcp_message->pfcp_session_set_modification_response, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_session_establishment_request,
                    &pfcp_message->pfcp_session_establishment_request, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_session_establishment_response,
                    &pfcp_message->pfcp_session_establishment_response, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_session_modification_request,
                    &pfcp_message->pfcp_session_modification_request, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_session_modification_response,
                    &pfcp_message->pfcp_session_modification_response, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_DELETION_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_session_deletion_request,
                    &pfcp_message->pfcp_session_deletion_request, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_DELETION_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_session_deletion_response,
                    &pfcp_message->pfcp_session_deletion_response, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_REPORT_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_session_report_request,
                    &pfcp_message->pfcp_session_report_request, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&ogs_pfcp_msg_desc_pfcp_session_report_response,
                    &pfcp_message->pfcp_session_report_response, OGS_TLV_MODE_T2_L2);
            break;
        default:
            ogs_warn("Not implemented(type:%d)", pfcp_message->h.type);
            break;
    }

    return pkbuf;
}

