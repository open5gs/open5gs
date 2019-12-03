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
 * Created on: 2019-12-01 21:40:00.638320 by acetcom
 * from 29244-f40.docx
 ******************************************************************************/

#include "ogs-pfcp.h"

ogs_tlv_desc_t ogs_pfcp_tlv_desc_cause_0 =
{
    OGS_TLV_VAR_STR,
    "Cause",
    OGS_TLV_PFCP_CAUSE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_cause_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_source_interface_0 =
{
    OGS_TLV_VAR_STR,
    "Source Interface",
    OGS_TLV_PFCP_SOURCE_INTERFACE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_source_interface_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_f_teid_0 =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_TLV_PFCP_F_TEID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_network_instance_0 =
{
    OGS_TLV_VAR_STR,
    "Network Instance",
    OGS_TLV_PFCP_NETWORK_INSTANCE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_network_instance_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_sdf_filter_0 =
{
    OGS_TLV_VAR_STR,
    "SDF Filter",
    OGS_TLV_PFCP_SDF_FILTER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_sdf_filter_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_application_id_0 =
{
    OGS_TLV_VAR_STR,
    "Application ID",
    OGS_TLV_PFCP_APPLICATION_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_application_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_gate_status_0 =
{
    OGS_TLV_VAR_STR,
    "Gate Status",
    OGS_TLV_PFCP_GATE_STATUS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_gate_status_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_mbr_0 =
{
    OGS_TLV_VAR_STR,
    "MBR",
    OGS_TLV_PFCP_MBR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_mbr_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_gbr_0 =
{
    OGS_TLV_VAR_STR,
    "GBR",
    OGS_TLV_PFCP_GBR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_gbr_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_qer_correlation_id_0 =
{
    OGS_TLV_VAR_STR,
    "QER Correlation ID",
    OGS_TLV_PFCP_QER_CORRELATION_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_qer_correlation_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_precedence_0 =
{
    OGS_TLV_VAR_STR,
    "Precedence",
    OGS_TLV_PFCP_PRECEDENCE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_precedence_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_transport_level_marking_0 =
{
    OGS_TLV_VAR_STR,
    "Transport Level Marking",
    OGS_TLV_PFCP_TRANSPORT_LEVEL_MARKING_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_transport_level_marking_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_volume_threshold_0 =
{
    OGS_TLV_VAR_STR,
    "Volume Threshold",
    OGS_TLV_PFCP_VOLUME_THRESHOLD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_volume_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_threshold_0 =
{
    OGS_TLV_VAR_STR,
    "Time Threshold",
    OGS_TLV_PFCP_TIME_THRESHOLD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_time_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_monitoring_time_0 =
{
    OGS_TLV_VAR_STR,
    "Monitoring Time",
    OGS_TLV_PFCP_MONITORING_TIME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_monitoring_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_volume_threshold_0 =
{
    OGS_TLV_VAR_STR,
    "Subsequent Volume Threshold",
    OGS_TLV_PFCP_SUBSEQUENT_VOLUME_THRESHOLD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_subsequent_volume_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_time_threshold_0 =
{
    OGS_TLV_VAR_STR,
    "Subsequent Time Threshold",
    OGS_TLV_PFCP_SUBSEQUENT_TIME_THRESHOLD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_subsequent_time_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_inactivity_detection_time_0 =
{
    OGS_TLV_VAR_STR,
    "Inactivity Detection Time",
    OGS_TLV_PFCP_INACTIVITY_DETECTION_TIME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_inactivity_detection_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_reporting_triggers_0 =
{
    OGS_TLV_VAR_STR,
    "Reporting Triggers",
    OGS_TLV_PFCP_REPORTING_TRIGGERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_reporting_triggers_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_redirect_information_0 =
{
    OGS_TLV_VAR_STR,
    "Redirect Information",
    OGS_TLV_PFCP_REDIRECT_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_redirect_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_report_type_0 =
{
    OGS_TLV_VAR_STR,
    "Report Type",
    OGS_TLV_PFCP_REPORT_TYPE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_report_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_offending_ie_0 =
{
    OGS_TLV_VAR_STR,
    "Offending IE",
    OGS_TLV_PFCP_OFFENDING_IE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_offending_ie_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_forwarding_policy_0 =
{
    OGS_TLV_VAR_STR,
    "Forwarding Policy",
    OGS_TLV_PFCP_FORWARDING_POLICY_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_forwarding_policy_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_destination_interface_0 =
{
    OGS_TLV_VAR_STR,
    "Destination Interface",
    OGS_TLV_PFCP_DESTINATION_INTERFACE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_destination_interface_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_up_function_features_0 =
{
    OGS_TLV_VAR_STR,
    "UP Function Features",
    OGS_TLV_PFCP_UP_FUNCTION_FEATURES_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_up_function_features_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_apply_action_0 =
{
    OGS_TLV_VAR_STR,
    "Apply Action",
    OGS_TLV_PFCP_APPLY_ACTION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_apply_action_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_downlink_data_service_information_0 =
{
    OGS_TLV_VAR_STR,
    "Downlink Data Service Information",
    OGS_TLV_PFCP_DOWNLINK_DATA_SERVICE_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_downlink_data_service_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_downlink_data_notification_delay_0 =
{
    OGS_TLV_VAR_STR,
    "Downlink Data Notification Delay",
    OGS_TLV_PFCP_DOWNLINK_DATA_NOTIFICATION_DELAY_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_downlink_data_notification_delay_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_dl_buffering_duration_0 =
{
    OGS_TLV_VAR_STR,
    "DL Buffering Duration",
    OGS_TLV_PFCP_DL_BUFFERING_DURATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_dl_buffering_duration_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_dl_buffering_suggested_packet_count_0 =
{
    OGS_TLV_VAR_STR,
    "DL Buffering Suggested Packet Count",
    OGS_TLV_PFCP_DL_BUFFERING_SUGGESTED_PACKET_COUNT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_dl_buffering_suggested_packet_count_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpsmreq_flags_0 =
{
    OGS_TLV_VAR_STR,
    "PFCPSMReq-Flags",
    OGS_TLV_PFCP_PFCPSMREQ_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pfcpsmreq_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcpsrrsp_flags_0 =
{
    OGS_TLV_VAR_STR,
    "PFCPSRRsp-Flags",
    OGS_TLV_PFCP_PFCPSRRSP_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pfcpsrrsp_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_sequence_number_0 =
{
    OGS_TLV_VAR_STR,
    "Sequence Number",
    OGS_TLV_PFCP_SEQUENCE_NUMBER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_sequence_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_metricp_0 =
{
    OGS_TLV_VAR_STR,
    "Metricp",
    OGS_TLV_PFCP_METRICP_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_metricp_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_timer_0 =
{
    OGS_TLV_VAR_STR,
    "Timer",
    OGS_TLV_PFCP_TIMER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_timer_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_packet_detection_rule_id_0 =
{
    OGS_TLV_VAR_STR,
    "Packet Detection Rule ID",
    OGS_TLV_PFCP_PACKET_DETECTION_RULE_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_packet_detection_rule_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_f_seid_0 =
{
    OGS_TLV_VAR_STR,
    "F-SEID",
    OGS_TLV_PFCP_F_SEID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_f_seid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_node_id_0 =
{
    OGS_TLV_VAR_STR,
    "Node ID",
    OGS_TLV_PFCP_NODE_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_node_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfd_contents_0 =
{
    OGS_TLV_VAR_STR,
    "PFD contents",
    OGS_TLV_PFCP_PFD_CONTENTS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pfd_contents_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_measurement_method_0 =
{
    OGS_TLV_VAR_STR,
    "Measurement Method",
    OGS_TLV_PFCP_MEASUREMENT_METHOD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_measurement_method_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_report_trigger_0 =
{
    OGS_TLV_VAR_STR,
    "Usage Report Trigger",
    OGS_TLV_PFCP_USAGE_REPORT_TRIGGER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_usage_report_trigger_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_measurement_period_0 =
{
    OGS_TLV_VAR_STR,
    "Measurement Period",
    OGS_TLV_PFCP_MEASUREMENT_PERIOD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_measurement_period_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_fq_csidp_0 =
{
    OGS_TLV_VAR_STR,
    "FQ-CSIDp",
    OGS_TLV_PFCP_FQ_CSIDP_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_fq_csidp_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_volume_measurement_0 =
{
    OGS_TLV_VAR_STR,
    "Volume Measurement",
    OGS_TLV_PFCP_VOLUME_MEASUREMENT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_volume_measurement_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_duration_measurement_0 =
{
    OGS_TLV_VAR_STR,
    "Duration Measurement",
    OGS_TLV_PFCP_DURATION_MEASUREMENT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_duration_measurement_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_of_first_packet_0 =
{
    OGS_TLV_VAR_STR,
    "Time of First Packet",
    OGS_TLV_PFCP_TIME_OF_FIRST_PACKET_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_time_of_first_packet_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_of_last_packet_0 =
{
    OGS_TLV_VAR_STR,
    "Time of Last Packet",
    OGS_TLV_PFCP_TIME_OF_LAST_PACKET_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_time_of_last_packet_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_quota_holding_time_0 =
{
    OGS_TLV_VAR_STR,
    "Quota Holding Time",
    OGS_TLV_PFCP_QUOTA_HOLDING_TIME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_quota_holding_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_dropped_dl_traffic_threshold_0 =
{
    OGS_TLV_VAR_STR,
    "Dropped DL Traffic Threshold",
    OGS_TLV_PFCP_DROPPED_DL_TRAFFIC_THRESHOLD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_dropped_dl_traffic_threshold_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_volume_quota_0 =
{
    OGS_TLV_VAR_STR,
    "Volume Quota",
    OGS_TLV_PFCP_VOLUME_QUOTA_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_volume_quota_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_quota_0 =
{
    OGS_TLV_VAR_STR,
    "Time Quota",
    OGS_TLV_PFCP_TIME_QUOTA_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_time_quota_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_start_time_0 =
{
    OGS_TLV_VAR_STR,
    "Start Time",
    OGS_TLV_PFCP_START_TIME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_start_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_end_time_0 =
{
    OGS_TLV_VAR_STR,
    "End Time",
    OGS_TLV_PFCP_END_TIME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_end_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_urr_id_0 =
{
    OGS_TLV_VAR_STR,
    "URR ID",
    OGS_TLV_PFCP_URR_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_urr_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_linked_urr_id_0 =
{
    OGS_TLV_VAR_STR,
    "Linked URR ID",
    OGS_TLV_PFCP_LINKED_URR_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_linked_urr_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_outer_header_creation_0 =
{
    OGS_TLV_VAR_STR,
    "Outer Header Creation",
    OGS_TLV_PFCP_OUTER_HEADER_CREATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_outer_header_creation_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_bar_id_0 =
{
    OGS_TLV_VAR_STR,
    "BAR ID",
    OGS_TLV_PFCP_BAR_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_bar_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_cp_function_features_0 =
{
    OGS_TLV_VAR_STR,
    "CP Function Features",
    OGS_TLV_PFCP_CP_FUNCTION_FEATURES_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_cp_function_features_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_information_0 =
{
    OGS_TLV_VAR_STR,
    "Usage Information",
    OGS_TLV_PFCP_USAGE_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_usage_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_application_instance_id_0 =
{
    OGS_TLV_VAR_STR,
    "Application Instance ID",
    OGS_TLV_PFCP_APPLICATION_INSTANCE_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_application_instance_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_flow_information_0 =
{
    OGS_TLV_VAR_STR,
    "Flow Information",
    OGS_TLV_PFCP_FLOW_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_flow_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ue_ip_address_0 =
{
    OGS_TLV_VAR_STR,
    "UE IP Address",
    OGS_TLV_PFCP_UE_IP_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ue_ip_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_packet_rate_0 =
{
    OGS_TLV_VAR_STR,
    "Packet Rate",
    OGS_TLV_PFCP_PACKET_RATE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_packet_rate_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_outer_header_removal_0 =
{
    OGS_TLV_VAR_STR,
    "Outer Header Removal",
    OGS_TLV_PFCP_OUTER_HEADER_REMOVAL_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_outer_header_removal_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_recovery_time_stamp_0 =
{
    OGS_TLV_VAR_STR,
    "Recovery Time Stamp",
    OGS_TLV_PFCP_RECOVERY_TIME_STAMP_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_recovery_time_stamp_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_dl_flow_level_marking_0 =
{
    OGS_TLV_VAR_STR,
    "DL Flow Level Marking",
    OGS_TLV_PFCP_DL_FLOW_LEVEL_MARKING_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_dl_flow_level_marking_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_header_enrichment_0 =
{
    OGS_TLV_VAR_STR,
    "Header Enrichment",
    OGS_TLV_PFCP_HEADER_ENRICHMENT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_header_enrichment_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_measurement_information_0 =
{
    OGS_TLV_VAR_STR,
    "Measurement Information",
    OGS_TLV_PFCP_MEASUREMENT_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_measurement_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_node_report_type_0 =
{
    OGS_TLV_VAR_STR,
    "Node Report Type",
    OGS_TLV_PFCP_NODE_REPORT_TYPE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_node_report_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_user_plane_path_failure_report_0 =
{
    OGS_TLV_VAR_STR,
    "User Plane Path Failure Report",
    OGS_TLV_PFCP_USER_PLANE_PATH_FAILURE_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_user_plane_path_failure_report_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remote_gtp_u_peer_0 =
{
    OGS_TLV_VAR_STR,
    "Remote GTP-U Peer",
    OGS_TLV_PFCP_REMOTE_GTP_U_PEER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remote_gtp_u_peer_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_ur_seqn_0 =
{
    OGS_TLV_VAR_STR,
    "UR-SEQN",
    OGS_TLV_PFCP_UR_SEQN_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_ur_seqn_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_activate_predefined_rules_0 =
{
    OGS_TLV_VAR_STR,
    "Activate Predefined Rules",
    OGS_TLV_PFCP_ACTIVATE_PREDEFINED_RULES_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_activate_predefined_rules_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_deactivate_predefined_rules_0 =
{
    OGS_TLV_VAR_STR,
    "Deactivate Predefined Rules",
    OGS_TLV_PFCP_DEACTIVATE_PREDEFINED_RULES_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_deactivate_predefined_rules_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_far_id_0 =
{
    OGS_TLV_VAR_STR,
    "FAR ID",
    OGS_TLV_PFCP_FAR_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_far_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_qer_id_0 =
{
    OGS_TLV_VAR_STR,
    "QER ID",
    OGS_TLV_PFCP_QER_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_qer_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_oci_flags_0 =
{
    OGS_TLV_VAR_STR,
    "OCI Flags",
    OGS_TLV_PFCP_OCI_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_oci_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_association_release_request_0 =
{
    OGS_TLV_VAR_STR,
    "PFCP Association Release Request",
    OGS_TLV_PFCP_PFCP_ASSOCIATION_RELEASE_REQUEST_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pfcp_association_release_request_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_graceful_release_period_0 =
{
    OGS_TLV_VAR_STR,
    "Graceful Release Period",
    OGS_TLV_PFCP_GRACEFUL_RELEASE_PERIOD_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_graceful_release_period_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pdn_typep_0 =
{
    OGS_TLV_VAR_STR,
    "PDN Typep",
    OGS_TLV_PFCP_PDN_TYPEP_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pdn_typep_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_failed_rule_id_0 =
{
    OGS_TLV_VAR_STR,
    "Failed Rule ID",
    OGS_TLV_PFCP_FAILED_RULE_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_failed_rule_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_quota_mechanism_0 =
{
    OGS_TLV_VAR_STR,
    "Time Quota Mechanism",
    OGS_TLV_PFCP_TIME_QUOTA_MECHANISM_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_time_quota_mechanism_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_user_plane_ip_resource_information_0 =
{
    OGS_TLV_VAR_STR,
    "User Plane IP Resource Information",
    OGS_TLV_PFCP_USER_PLANE_IP_RESOURCE_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_user_plane_ip_resource_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_user_plane_inactivity_timer_0 =
{
    OGS_TLV_VAR_STR,
    "User Plane Inactivity Timer",
    OGS_TLV_PFCP_USER_PLANE_INACTIVITY_TIMER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_user_plane_inactivity_timer_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_aggregated_urrs_0 =
{
    OGS_TLV_VAR_STR,
    "Aggregated URRs",
    OGS_TLV_PFCP_AGGREGATED_URRS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_aggregated_urrs_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_multiplier_0 =
{
    OGS_TLV_VAR_STR,
    "Multiplier",
    OGS_TLV_PFCP_MULTIPLIER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_multiplier_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_aggregated_urr_id_0 =
{
    OGS_TLV_VAR_STR,
    "Aggregated URR ID",
    OGS_TLV_PFCP_AGGREGATED_URR_ID_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_aggregated_urr_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_volume_quota_0 =
{
    OGS_TLV_VAR_STR,
    "Subsequent Volume Quota",
    OGS_TLV_PFCP_SUBSEQUENT_VOLUME_QUOTA_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_subsequent_volume_quota_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_subsequent_time_quota_0 =
{
    OGS_TLV_VAR_STR,
    "Subsequent Time Quota",
    OGS_TLV_PFCP_SUBSEQUENT_TIME_QUOTA_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_subsequent_time_quota_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_rqi_0 =
{
    OGS_TLV_VAR_STR,
    "RQI",
    OGS_TLV_PFCP_RQI_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_rqi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_qfi_0 =
{
    OGS_TLV_VAR_STR,
    "QFI",
    OGS_TLV_PFCP_QFI_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_qfi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_pdr_0 =
{
    OGS_TLV_COMPOUND,
    "Create PDR",
    OGS_PFCP_CREATE_PDR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_pdr_t),
    {
        &ogs_pfcp_tlv_desc_packet_detection_rule_id_0,
        &ogs_pfcp_tlv_desc_precedence_0,
        &ogs_pfcp_tlv_desc_pdi_0,
        &ogs_pfcp_tlv_desc_outer_header_removal_0,
        &ogs_pfcp_tlv_desc_far_id_0,
        &ogs_pfcp_tlv_desc_urr_id_0,
        &ogs_pfcp_tlv_desc_qer_id_0,
        &ogs_pfcp_tlv_desc_activate_predefined_rules_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_pdr_1 =
{
    OGS_TLV_COMPOUND,
    "Create PDR",
    OGS_PFCP_CREATE_PDR_TYPE,
    0,
    1,
    sizeof(ogs_pfcp_tlv_create_pdr_t),
    {
        &ogs_pfcp_tlv_desc_packet_detection_rule_id_0,
        &ogs_pfcp_tlv_desc_precedence_0,
        &ogs_pfcp_tlv_desc_pdi_0,
        &ogs_pfcp_tlv_desc_outer_header_removal_0,
        &ogs_pfcp_tlv_desc_far_id_0,
        &ogs_pfcp_tlv_desc_urr_id_0,
        &ogs_pfcp_tlv_desc_qer_id_0,
        &ogs_pfcp_tlv_desc_activate_predefined_rules_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pdi_0 =
{
    OGS_TLV_COMPOUND,
    "PDI",
    OGS_PFCP_PDI_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pdi_t),
    {
        &ogs_pfcp_tlv_desc_source_interface_0,
        &ogs_pfcp_tlv_desc_pfcpf_teid_0,
        &ogs_pfcp_tlv_desc_network_instance_0,
        &ogs_pfcp_tlv_desc_ue_ip_address_0,
        &ogs_pfcp_tlv_desc_sdf_filter_0,
        &ogs_pfcp_tlv_desc_application_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_far_0 =
{
    OGS_TLV_COMPOUND,
    "Create FAR",
    OGS_PFCP_CREATE_FAR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_far_t),
    {
        &ogs_pfcp_tlv_desc_far_id_0,
        &ogs_pfcp_tlv_desc_apply_action_0,
        &ogs_pfcp_tlv_desc_forwarding_parameters_0,
        &ogs_pfcp_tlv_desc_duplicating_parameters_0,
        &ogs_pfcp_tlv_desc_bar_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_far_1 =
{
    OGS_TLV_COMPOUND,
    "Create FAR",
    OGS_PFCP_CREATE_FAR_TYPE,
    0,
    1,
    sizeof(ogs_pfcp_tlv_create_far_t),
    {
        &ogs_pfcp_tlv_desc_far_id_0,
        &ogs_pfcp_tlv_desc_apply_action_0,
        &ogs_pfcp_tlv_desc_forwarding_parameters_0,
        &ogs_pfcp_tlv_desc_duplicating_parameters_0,
        &ogs_pfcp_tlv_desc_bar_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_forwarding_parameters_0 =
{
    OGS_TLV_COMPOUND,
    "Forwarding Parameters",
    OGS_PFCP_FORWARDING_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_forwarding_parameters_t),
    {
        &ogs_pfcp_tlv_desc_destination_interface_0,
        &ogs_pfcp_tlv_desc_network_instance_0,
        &ogs_pfcp_tlv_desc_redirect_information_0,
        &ogs_pfcp_tlv_desc_outer_header_creation_0,
        &ogs_pfcp_tlv_desc_transport_level_marking_0,
        &ogs_pfcp_tlv_desc_forwarding_policy_0,
        &ogs_pfcp_tlv_desc_header_enrichment_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_duplicating_parameters_0 =
{
    OGS_TLV_COMPOUND,
    "Duplicating Parameters",
    OGS_PFCP_DUPLICATING_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_duplicating_parameters_t),
    {
        &ogs_pfcp_tlv_desc_destination_interface_0,
        &ogs_pfcp_tlv_desc_outer_header_creation_0,
        &ogs_pfcp_tlv_desc_transport_level_marking_0,
        &ogs_pfcp_tlv_desc_forwarding_policy_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_urr_0 =
{
    OGS_TLV_COMPOUND,
    "Create URR",
    OGS_PFCP_CREATE_URR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_urr_t),
    {
        &ogs_pfcp_tlv_desc_urr_id_0,
        &ogs_pfcp_tlv_desc_measurement_method_0,
        &ogs_pfcp_tlv_desc_reporting_triggers_0,
        &ogs_pfcp_tlv_desc_measurement_period_0,
        &ogs_pfcp_tlv_desc_volume_threshold_0,
        &ogs_pfcp_tlv_desc_volume_quota_0,
        &ogs_pfcp_tlv_desc_time_threshold_0,
        &ogs_pfcp_tlv_desc_time_quota_0,
        &ogs_pfcp_tlv_desc_quota_holding_time_0,
        &ogs_pfcp_tlv_desc_dropped_dl_traffic_threshold_0,
        &ogs_pfcp_tlv_desc_monitoring_time_0,
        &ogs_pfcp_tlv_desc_subsequent_volume_threshold_0,
        &ogs_pfcp_tlv_desc_subsequent_time_threshold_0,
        &ogs_pfcp_tlv_desc_subsequent_volume_quota_0,
        &ogs_pfcp_tlv_desc_subsequent_time_quota_0,
        &ogs_pfcp_tlv_desc_inactivity_detection_time_0,
        &ogs_pfcp_tlv_desc_linked_urr_id_0,
        &ogs_pfcp_tlv_desc_measurement_information_0,
        &ogs_pfcp_tlv_desc_time_quota_mechanism_0,
        &ogs_pfcp_tlv_desc_aggregated_urrs_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_qer_0 =
{
    OGS_TLV_COMPOUND,
    "Create QER",
    OGS_PFCP_CREATE_QER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_qer_t),
    {
        &ogs_pfcp_tlv_desc_qer_id_0,
        &ogs_pfcp_tlv_desc_qer_correlation_id_0,
        &ogs_pfcp_tlv_desc_gate_status_0,
        &ogs_pfcp_tlv_desc_mbr_0,
        &ogs_pfcp_tlv_desc_gbr_0,
        &ogs_pfcp_tlv_desc_packet_rate_0,
        &ogs_pfcp_tlv_desc_dl_flow_level_marking_0,
        &ogs_pfcp_tlv_desc_qfi_0,
        &ogs_pfcp_tlv_desc_rqi_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_created_pdr_0 =
{
    OGS_TLV_COMPOUND,
    "Created PDR",
    OGS_PFCP_CREATED_PDR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_created_pdr_t),
    {
        &ogs_pfcp_tlv_desc_packet_detection_rule_id_0,
        &ogs_pfcp_tlv_desc_pfcpf_teid_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_pdr_0 =
{
    OGS_TLV_COMPOUND,
    "Update PDR",
    OGS_PFCP_UPDATE_PDR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_pdr_t),
    {
        &ogs_pfcp_tlv_desc_packet_detection_rule_id_0,
        &ogs_pfcp_tlv_desc_outer_header_removal_0,
        &ogs_pfcp_tlv_desc_precedence_0,
        &ogs_pfcp_tlv_desc_pdi_0,
        &ogs_pfcp_tlv_desc_far_id_0,
        &ogs_pfcp_tlv_desc_urr_id_0,
        &ogs_pfcp_tlv_desc_qer_id_0,
        &ogs_pfcp_tlv_desc_activate_predefined_rules_0,
        &ogs_pfcp_tlv_desc_deactivate_predefined_rules_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_pdr_1 =
{
    OGS_TLV_COMPOUND,
    "Update PDR",
    OGS_PFCP_UPDATE_PDR_TYPE,
    0,
    1,
    sizeof(ogs_pfcp_tlv_update_pdr_t),
    {
        &ogs_pfcp_tlv_desc_packet_detection_rule_id_0,
        &ogs_pfcp_tlv_desc_outer_header_removal_0,
        &ogs_pfcp_tlv_desc_precedence_0,
        &ogs_pfcp_tlv_desc_pdi_0,
        &ogs_pfcp_tlv_desc_far_id_0,
        &ogs_pfcp_tlv_desc_urr_id_0,
        &ogs_pfcp_tlv_desc_qer_id_0,
        &ogs_pfcp_tlv_desc_activate_predefined_rules_0,
        &ogs_pfcp_tlv_desc_deactivate_predefined_rules_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_far_0 =
{
    OGS_TLV_COMPOUND,
    "Update FAR",
    OGS_PFCP_UPDATE_FAR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_far_t),
    {
        &ogs_pfcp_tlv_desc_far_id_0,
        &ogs_pfcp_tlv_desc_apply_action_0,
        &ogs_pfcp_tlv_desc_update_forwarding_parameters_0,
        &ogs_pfcp_tlv_desc_update_duplicating_parameters_0,
        &ogs_pfcp_tlv_desc_bar_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_forwarding_parameters_0 =
{
    OGS_TLV_COMPOUND,
    "Update Forwarding Parameters",
    OGS_PFCP_UPDATE_FORWARDING_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_forwarding_parameters_t),
    {
        &ogs_pfcp_tlv_desc_destination_interface_0,
        &ogs_pfcp_tlv_desc_network_instance_0,
        &ogs_pfcp_tlv_desc_redirect_information_0,
        &ogs_pfcp_tlv_desc_outer_header_creation_0,
        &ogs_pfcp_tlv_desc_transport_level_marking_0,
        &ogs_pfcp_tlv_desc_forwarding_policy_0,
        &ogs_pfcp_tlv_desc_header_enrichment_0,
        &ogs_pfcp_tlv_desc_pfcpsmreq_flags_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_bar_pfcp_session_report_response_0 =
{
    OGS_TLV_COMPOUND,
    "Update BAR PFCP Session Report Response",
    OGS_PFCP_UPDATE_BAR_PFCP_SESSION_REPORT_RESPONSE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_bar_pfcp_session_report_response_t),
    {
        &ogs_pfcp_tlv_desc_bar_id_0,
        &ogs_pfcp_tlv_desc_downlink_data_notification_delay_0,
        &ogs_pfcp_tlv_desc_dl_buffering_duration_0,
        &ogs_pfcp_tlv_desc_dl_buffering_suggested_packet_count_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_urr_0 =
{
    OGS_TLV_COMPOUND,
    "Update URR",
    OGS_PFCP_UPDATE_URR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_urr_t),
    {
        &ogs_pfcp_tlv_desc_urr_id_0,
        &ogs_pfcp_tlv_desc_measurement_method_0,
        &ogs_pfcp_tlv_desc_reporting_triggers_0,
        &ogs_pfcp_tlv_desc_measurement_period_0,
        &ogs_pfcp_tlv_desc_volume_threshold_0,
        &ogs_pfcp_tlv_desc_volume_quota_0,
        &ogs_pfcp_tlv_desc_time_threshold_0,
        &ogs_pfcp_tlv_desc_time_quota_0,
        &ogs_pfcp_tlv_desc_quota_holding_time_0,
        &ogs_pfcp_tlv_desc_dropped_dl_traffic_threshold_0,
        &ogs_pfcp_tlv_desc_monitoring_time_0,
        &ogs_pfcp_tlv_desc_subsequent_volume_threshold_0,
        &ogs_pfcp_tlv_desc_subsequent_time_threshold_0,
        &ogs_pfcp_tlv_desc_subsequent_volume_quota_0,
        &ogs_pfcp_tlv_desc_subsequent_time_quota_0,
        &ogs_pfcp_tlv_desc_inactivity_detection_time_0,
        &ogs_pfcp_tlv_desc_linked_urr_id_0,
        &ogs_pfcp_tlv_desc_measurement_information_0,
        &ogs_pfcp_tlv_desc_time_quota_mechanism_0,
        &ogs_pfcp_tlv_desc_aggregated_urrs_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_qer_0 =
{
    OGS_TLV_COMPOUND,
    "Update QER",
    OGS_PFCP_UPDATE_QER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_qer_t),
    {
        &ogs_pfcp_tlv_desc_qer_id_0,
        &ogs_pfcp_tlv_desc_qer_correlation_id_0,
        &ogs_pfcp_tlv_desc_gate_status_0,
        &ogs_pfcp_tlv_desc_mbr_0,
        &ogs_pfcp_tlv_desc_gbr_0,
        &ogs_pfcp_tlv_desc_packet_rate_0,
        &ogs_pfcp_tlv_desc_dl_flow_level_marking_0,
        &ogs_pfcp_tlv_desc_qfi_0,
        &ogs_pfcp_tlv_desc_rqi_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_pdr_0 =
{
    OGS_TLV_COMPOUND,
    "Remove PDR",
    OGS_PFCP_REMOVE_PDR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_pdr_t),
    {
        &ogs_pfcp_tlv_desc_packet_detection_rule_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_far_0 =
{
    OGS_TLV_COMPOUND,
    "Remove FAR",
    OGS_PFCP_REMOVE_FAR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_far_t),
    {
        &ogs_pfcp_tlv_desc_far_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_urr_0 =
{
    OGS_TLV_COMPOUND,
    "Remove URR",
    OGS_PFCP_REMOVE_URR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_urr_t),
    {
        &ogs_pfcp_tlv_desc_urr_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_qer_0 =
{
    OGS_TLV_COMPOUND,
    "Remove QER",
    OGS_PFCP_REMOVE_QER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_qer_t),
    {
        &ogs_pfcp_tlv_desc_qer_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_load_control_informationp_0 =
{
    OGS_TLV_COMPOUND,
    "Load Control Informationp",
    OGS_PFCP_LOAD_CONTROL_INFORMATIONP_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_load_control_informationp_t),
    {
        &ogs_pfcp_tlv_desc_pfcpsequence_number_0,
        &ogs_pfcp_tlv_desc_metricp_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_overload_control_informationp_0 =
{
    OGS_TLV_COMPOUND,
    "Overload Control Informationp",
    OGS_PFCP_OVERLOAD_CONTROL_INFORMATIONP_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_overload_control_informationp_t),
    {
        &ogs_pfcp_tlv_desc_pfcpsequence_number_0,
        &ogs_pfcp_tlv_desc_metricp_0,
        &ogs_pfcp_tlv_desc_timer_0,
        &ogs_pfcp_tlv_desc_oci_flags_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_application_id_s_pfds_0 =
{
    OGS_TLV_COMPOUND,
    "Application ID's PFDs",
    OGS_PFCP_APPLICATION_ID_S_PFDS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_application_id_s_pfds_t),
    {
        &ogs_pfcp_tlv_desc_application_id_0,
        &ogs_pfcp_tlv_desc_pfd_context_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfd_context_0 =
{
    OGS_TLV_COMPOUND,
    "PFD context",
    OGS_PFCP_PFD_CONTEXT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_pfd_context_t),
    {
        &ogs_pfcp_tlv_desc_pfd_contents_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_application_detection_information_0 =
{
    OGS_TLV_COMPOUND,
    "Application Detection Information",
    OGS_PFCP_APPLICATION_DETECTION_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_application_detection_information_t),
    {
        &ogs_pfcp_tlv_desc_application_id_0,
        &ogs_pfcp_tlv_desc_application_instance_id_0,
        &ogs_pfcp_tlv_desc_flow_information_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_query_urr_0 =
{
    OGS_TLV_COMPOUND,
    "Query URR",
    OGS_PFCP_QUERY_URR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_query_urr_t),
    {
        &ogs_pfcp_tlv_desc_urr_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_report_in_session_modification_response_0 =
{
    OGS_TLV_COMPOUND,
    "Usage Report in Session Modification Response",
    OGS_PFCP_USAGE_REPORT_IN_SESSION_MODIFICATION_RESPONSE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_usage_report_in_session_modification_response_t),
    {
        &ogs_pfcp_tlv_desc_urr_id_0,
        &ogs_pfcp_tlv_desc_ur_seqn_0,
        &ogs_pfcp_tlv_desc_usage_report_trigger_0,
        &ogs_pfcp_tlv_desc_start_time_0,
        &ogs_pfcp_tlv_desc_end_time_0,
        &ogs_pfcp_tlv_desc_volume_measurement_0,
        &ogs_pfcp_tlv_desc_duration_measurement_0,
        &ogs_pfcp_tlv_desc_time_of_first_packet_0,
        &ogs_pfcp_tlv_desc_time_of_last_packet_0,
        &ogs_pfcp_tlv_desc_usage_information_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_report_session_deletion_response_0 =
{
    OGS_TLV_COMPOUND,
    "Usage Report Session Deletion Response",
    OGS_PFCP_USAGE_REPORT_SESSION_DELETION_RESPONSE_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_usage_report_session_deletion_response_t),
    {
        &ogs_pfcp_tlv_desc_urr_id_0,
        &ogs_pfcp_tlv_desc_ur_seqn_0,
        &ogs_pfcp_tlv_desc_usage_report_trigger_0,
        &ogs_pfcp_tlv_desc_start_time_0,
        &ogs_pfcp_tlv_desc_end_time_0,
        &ogs_pfcp_tlv_desc_volume_measurement_0,
        &ogs_pfcp_tlv_desc_duration_measurement_0,
        &ogs_pfcp_tlv_desc_time_of_first_packet_0,
        &ogs_pfcp_tlv_desc_time_of_last_packet_0,
        &ogs_pfcp_tlv_desc_usage_information_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_usage_report_session_report_request_0 =
{
    OGS_TLV_COMPOUND,
    "Usage Report Session Report Request",
    OGS_PFCP_USAGE_REPORT_SESSION_REPORT_REQUEST_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_usage_report_session_report_request_t),
    {
        &ogs_pfcp_tlv_desc_urr_id_0,
        &ogs_pfcp_tlv_desc_ur_seqn_0,
        &ogs_pfcp_tlv_desc_usage_report_trigger_0,
        &ogs_pfcp_tlv_desc_start_time_0,
        &ogs_pfcp_tlv_desc_end_time_0,
        &ogs_pfcp_tlv_desc_volume_measurement_0,
        &ogs_pfcp_tlv_desc_duration_measurement_0,
        &ogs_pfcp_tlv_desc_application_detection_information_0,
        &ogs_pfcp_tlv_desc_ue_ip_address_0,
        &ogs_pfcp_tlv_desc_network_instance_0,
        &ogs_pfcp_tlv_desc_time_of_first_packet_0,
        &ogs_pfcp_tlv_desc_time_of_last_packet_0,
        &ogs_pfcp_tlv_desc_usage_information_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_downlink_data_report_0 =
{
    OGS_TLV_COMPOUND,
    "Downlink Data Report",
    OGS_PFCP_DOWNLINK_DATA_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_downlink_data_report_t),
    {
        &ogs_pfcp_tlv_desc_packet_detection_rule_id_0,
        &ogs_pfcp_tlv_desc_downlink_data_service_information_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_create_bar_0 =
{
    OGS_TLV_COMPOUND,
    "Create BAR",
    OGS_PFCP_CREATE_BAR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_create_bar_t),
    {
        &ogs_pfcp_tlv_desc_bar_id_0,
        &ogs_pfcp_tlv_desc_downlink_data_notification_delay_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_bar_session_modification_request_0 =
{
    OGS_TLV_COMPOUND,
    "Update BAR Session Modification Request",
    OGS_PFCP_UPDATE_BAR_SESSION_MODIFICATION_REQUEST_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_bar_session_modification_request_t),
    {
        &ogs_pfcp_tlv_desc_bar_id_0,
        &ogs_pfcp_tlv_desc_downlink_data_notification_delay_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_remove_bar_0 =
{
    OGS_TLV_COMPOUND,
    "Remove BAR",
    OGS_PFCP_REMOVE_BAR_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_remove_bar_t),
    {
        &ogs_pfcp_tlv_desc_bar_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_error_indication_report_0 =
{
    OGS_TLV_COMPOUND,
    "Error Indication Report",
    OGS_PFCP_ERROR_INDICATION_REPORT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_error_indication_report_t),
    {
        &ogs_pfcp_tlv_desc_pfcpf_teid_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_duplicating_parameters_0 =
{
    OGS_TLV_COMPOUND,
    "Update Duplicating Parameters",
    OGS_PFCP_UPDATE_DUPLICATING_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_duplicating_parameters_t),
    {
        &ogs_pfcp_tlv_desc_destination_interface_0,
        &ogs_pfcp_tlv_desc_outer_header_creation_0,
        &ogs_pfcp_tlv_desc_transport_level_marking_0,
        &ogs_pfcp_tlv_desc_forwarding_policy_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_heartbeat_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Heartbeat Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_recovery_time_stamp_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_heartbeat_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Heartbeat Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_recovery_time_stamp_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_association_setup_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Association Setup Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id_0,
        &ogs_pfcp_tlv_desc_recovery_time_stamp_0,
        &ogs_pfcp_tlv_desc_up_function_features_0,
        &ogs_pfcp_tlv_desc_cp_function_features_0,
        &ogs_pfcp_tlv_desc_user_plane_ip_resource_information_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_association_setup_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Association Setup Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id_0,
        &ogs_pfcp_tlv_desc_pfcpcause_0,
        &ogs_pfcp_tlv_desc_recovery_time_stamp_0,
        &ogs_pfcp_tlv_desc_up_function_features_0,
        &ogs_pfcp_tlv_desc_cp_function_features_0,
        &ogs_pfcp_tlv_desc_user_plane_ip_resource_information_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_association_update_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Association Update Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id_0,
        &ogs_pfcp_tlv_desc_up_function_features_0,
        &ogs_pfcp_tlv_desc_cp_function_features_0,
        &ogs_pfcp_tlv_desc_pfcp_association_release_request_0,
        &ogs_pfcp_tlv_desc_graceful_release_period_0,
        &ogs_pfcp_tlv_desc_user_plane_ip_resource_information_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_association_update_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Association Update Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id_0,
        &ogs_pfcp_tlv_desc_pfcpcause_0,
        &ogs_pfcp_tlv_desc_up_function_features_0,
        &ogs_pfcp_tlv_desc_cp_function_features_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_association_release_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Association Release Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_association_release_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Association Release Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id_0,
        &ogs_pfcp_tlv_desc_pfcpcause_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_version_not_supported_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Version Not Supported Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_pfcpcause_0,
        &ogs_pfcp_tlv_desc_offending_ie_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_node_report_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Node Report Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id_0,
        &ogs_pfcp_tlv_desc_node_report_type_0,
        &ogs_pfcp_tlv_desc_user_plane_path_failure_report_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_node_report_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Node Report Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id_0,
        &ogs_pfcp_tlv_desc_pfcpcause_0,
        &ogs_pfcp_tlv_desc_offending_ie_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_session_set_deletion_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Set Deletion Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id_0,
        &ogs_pfcp_tlv_desc_fq_csidp_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_session_set_deletion_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Set Deletion Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id_0,
        &ogs_pfcp_tlv_desc_pfcpcause_0,
        &ogs_pfcp_tlv_desc_offending_ie_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_session_establishment_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Establishment Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id_0,
        &ogs_pfcp_tlv_desc_f_seid_0,
        &ogs_pfcp_tlv_desc_create_pdr_0,
        &ogs_pfcp_tlv_desc_create_pdr_1,
        &ogs_pfcp_tlv_desc_create_far_0,
        &ogs_pfcp_tlv_desc_create_far_1,
        &ogs_pfcp_tlv_desc_create_urr_0,
        &ogs_pfcp_tlv_desc_create_qer_0,
        &ogs_pfcp_tlv_desc_create_bar_0,
        &ogs_pfcp_tlv_desc_pdn_typep_0,
        &ogs_pfcp_tlv_desc_fq_csidp_0,
        &ogs_pfcp_tlv_desc_user_plane_inactivity_timer_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_session_establishment_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Establishment Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id_0,
        &ogs_pfcp_tlv_desc_pfcpcause_0,
        &ogs_pfcp_tlv_desc_offending_ie_0,
        &ogs_pfcp_tlv_desc_f_seid_0,
        &ogs_pfcp_tlv_desc_created_pdr_0,
        &ogs_pfcp_tlv_desc_load_control_informationp_0,
        &ogs_pfcp_tlv_desc_overload_control_informationp_0,
        &ogs_pfcp_tlv_desc_fq_csidp_0,
        &ogs_pfcp_tlv_desc_failed_rule_id_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_session_modification_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Modification Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_f_seid_0,
        &ogs_pfcp_tlv_desc_remove_pdr_0,
        &ogs_pfcp_tlv_desc_remove_far_0,
        &ogs_pfcp_tlv_desc_remove_urr_0,
        &ogs_pfcp_tlv_desc_remove_qer_0,
        &ogs_pfcp_tlv_desc_remove_bar_0,
        &ogs_pfcp_tlv_desc_create_pdr_0,
        &ogs_pfcp_tlv_desc_create_pdr_1,
        &ogs_pfcp_tlv_desc_create_far_0,
        &ogs_pfcp_tlv_desc_create_far_1,
        &ogs_pfcp_tlv_desc_create_urr_0,
        &ogs_pfcp_tlv_desc_create_qer_0,
        &ogs_pfcp_tlv_desc_create_bar_0,
        &ogs_pfcp_tlv_desc_update_pdr_0,
        &ogs_pfcp_tlv_desc_update_pdr_1,
        &ogs_pfcp_tlv_desc_update_far_0,
        &ogs_pfcp_tlv_desc_update_urr_0,
        &ogs_pfcp_tlv_desc_update_qer_0,
        &ogs_pfcp_tlv_desc_update_bar_session_modification_request_0,
        &ogs_pfcp_tlv_desc_pfcpsmreq_flags_0,
        &ogs_pfcp_tlv_desc_query_urr_0,
        &ogs_pfcp_tlv_desc_fq_csidp_0,
        &ogs_pfcp_tlv_desc_user_plane_inactivity_timer_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_session_modification_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Modification Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_pfcpcause_0,
        &ogs_pfcp_tlv_desc_offending_ie_0,
        &ogs_pfcp_tlv_desc_created_pdr_0,
        &ogs_pfcp_tlv_desc_load_control_informationp_0,
        &ogs_pfcp_tlv_desc_overload_control_informationp_0,
        &ogs_pfcp_tlv_desc_usage_report_in_session_modification_response_0,
        &ogs_pfcp_tlv_desc_failed_rule_id_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_session_deletion_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Deletion Request",
    0, 0, 0, 0, {
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_session_report_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Report Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_report_type_0,
        &ogs_pfcp_tlv_desc_downlink_data_report_0,
        &ogs_pfcp_tlv_desc_usage_report_session_report_request_0,
        &ogs_pfcp_tlv_desc_error_indication_report_0,
        &ogs_pfcp_tlv_desc_load_control_informationp_0,
        &ogs_pfcp_tlv_desc_overload_control_informationp_0,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_pfcp_session_report_response =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Report Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_pfcpcause_0,
        &ogs_pfcp_tlv_desc_offending_ie_0,
        &ogs_pfcp_tlv_desc_update_bar_pfcp_session_report_response_0,
        &ogs_pfcp_tlv_desc_pfcpsrrsp_flags_0,
    NULL,
}};


int ogs_pfcp_parse_msg(ogs_pfcp_message_t *pfcp_message, pkbuf_t *pkbuf)
{
    int rv = OGS_ERROR;
    ogs_pfcp_header_t *h = NULL;
    uint16_t size = 0;

    ogs_assert(pfcp_message);
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);

    h = (ogs_pfcp_header_t *)pkbuf->data;
    ogs_assert(h);
    
    memset(pfcp_message, 0, sizeof(ogs_pfcp_message_t));

    if (h->seid_p)
        size = OGS_PFCP_HEADER_LEN;
    else
        size = OGS_PFCP_HEADER_LEN-OGS_PFCP_SEID_LEN;

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(&pcfp_message->h, pkbuf->data - size, size);

    if (h->seid_p) {
        pfcp_message->h.seid = be64toh(pfcp_message->h.seid);
    } else {
        pfcp_message->h.sqn = pfcp_message->h.sqn_only;
        pfcp_message->h.sqn_only = pfcp_message->h.sqn_only;
    }

    if (pkbuf->len == 0)
        return OGS_OK;

    switch(pfcp_message->h.type)
    {
        case OGS_PFCP_PFCP_HEARTBEAT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_heartbeat_request,
                    &ogs_pfcp_tlv_desc_pfcp_heartbeat_request, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_HEARTBEAT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_heartbeat_response,
                    &ogs_pfcp_tlv_desc_pfcp_heartbeat_response, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_setup_request,
                    &ogs_pfcp_tlv_desc_pfcp_association_setup_request, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_setup_response,
                    &ogs_pfcp_tlv_desc_pfcp_association_setup_response, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_ASSOCIATION_UPDATE_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_update_request,
                    &ogs_pfcp_tlv_desc_pfcp_association_update_request, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_ASSOCIATION_UPDATE_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_update_response,
                    &ogs_pfcp_tlv_desc_pfcp_association_update_response, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_ASSOCIATION_RELEASE_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_release_request,
                    &ogs_pfcp_tlv_desc_pfcp_association_release_request, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_ASSOCIATION_RELEASE_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_release_response,
                    &ogs_pfcp_tlv_desc_pfcp_association_release_response, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_VERSION_NOT_SUPPORTED_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_version_not_supported_response,
                    &ogs_pfcp_tlv_desc_pfcp_version_not_supported_response, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_NODE_REPORT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_node_report_request,
                    &ogs_pfcp_tlv_desc_pfcp_node_report_request, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_NODE_REPORT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_node_report_response,
                    &ogs_pfcp_tlv_desc_pfcp_node_report_response, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_SET_DELETION_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_set_deletion_request,
                    &ogs_pfcp_tlv_desc_pfcp_session_set_deletion_request, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_SET_DELETION_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_set_deletion_response,
                    &ogs_pfcp_tlv_desc_pfcp_session_set_deletion_response, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_establishment_request,
                    &ogs_pfcp_tlv_desc_pfcp_session_establishment_request, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_establishment_response,
                    &ogs_pfcp_tlv_desc_pfcp_session_establishment_response, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_modification_request,
                    &ogs_pfcp_tlv_desc_pfcp_session_modification_request, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_modification_response,
                    &ogs_pfcp_tlv_desc_pfcp_session_modification_response, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_DELETION_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_deletion_request,
                    &ogs_pfcp_tlv_desc_pfcp_session_deletion_request, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_REPORT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_report_request,
                    &ogs_pfcp_tlv_desc_pfcp_session_report_request, pkbuf, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_REPORT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_report_response,
                    &ogs_pfcp_tlv_desc_pfcp_session_report_response, pkbuf, TLV_MODE_T2_L2);
            break;
        default:
            ogs_warn("Not implmeneted(type:%d)", pfcp_message->h.type);
            break;
    }

    return rv;
}

ogs_pkbuf_t *ogs_pfcp_build_msg(pfcp_message_t *pfcp_message)
{
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(pfcp_message);
    switch(pfcp_message->h.type)
    {
        case OGS_PFCP_PFCP_HEARTBEAT_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_heartbeat_request,
                    &pfcp_message->pfcp_heartbeat_request, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_HEARTBEAT_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_heartbeat_response,
                    &pfcp_message->pfcp_heartbeat_response, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_association_setup_request,
                    &pfcp_message->pfcp_association_setup_request, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_association_setup_response,
                    &pfcp_message->pfcp_association_setup_response, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_ASSOCIATION_UPDATE_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_association_update_request,
                    &pfcp_message->pfcp_association_update_request, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_ASSOCIATION_UPDATE_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_association_update_response,
                    &pfcp_message->pfcp_association_update_response, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_ASSOCIATION_RELEASE_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_association_release_request,
                    &pfcp_message->pfcp_association_release_request, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_ASSOCIATION_RELEASE_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_association_release_response,
                    &pfcp_message->pfcp_association_release_response, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_VERSION_NOT_SUPPORTED_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_version_not_supported_response,
                    &pfcp_message->pfcp_version_not_supported_response, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_NODE_REPORT_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_node_report_request,
                    &pfcp_message->pfcp_node_report_request, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_NODE_REPORT_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_node_report_response,
                    &pfcp_message->pfcp_node_report_response, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_SET_DELETION_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_session_set_deletion_request,
                    &pfcp_message->pfcp_session_set_deletion_request, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_SET_DELETION_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_session_set_deletion_response,
                    &pfcp_message->pfcp_session_set_deletion_response, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_session_establishment_request,
                    &pfcp_message->pfcp_session_establishment_request, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_session_establishment_response,
                    &pfcp_message->pfcp_session_establishment_response, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_session_modification_request,
                    &pfcp_message->pfcp_session_modification_request, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_session_modification_response,
                    &pfcp_message->pfcp_session_modification_response, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_DELETION_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_session_deletion_request,
                    &pfcp_message->pfcp_session_deletion_request, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_REPORT_REQUEST_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_session_report_request,
                    &pfcp_message->pfcp_session_report_request, TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFCP_SESSION_REPORT_RESPONSE_TYPE:
            pkbuf = ogs_tlv_build_msg(&tlv_desc_pfcp_session_report_response,
                    &pfcp_message->pfcp_session_report_response, TLV_MODE_T2_L2);
            break;
        default:
            ogs_warn("Not implmeneted(type:%d)", pfcp_message->h.type);
            break;
    }

    return pkbuf;
}

