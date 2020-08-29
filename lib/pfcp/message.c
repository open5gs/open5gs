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
 * Created on: 2020-08-28 21:50:38.885547 by acetcom
 * from 29244-g10.docx
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
    OGS_TLV_VAR_STR,
    "Time Threshold",
    OGS_PFCP_TIME_THRESHOLD_TYPE,
    0,
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
    OGS_TLV_UINT8,
    "Reporting Triggers",
    OGS_PFCP_REPORTING_TRIGGERS_TYPE,
    1,
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
    OGS_TLV_UINT8,
    "Apply Action",
    OGS_PFCP_APPLY_ACTION_TYPE,
    1,
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
    OGS_TLV_VAR_STR,
    "Usage Report Trigger",
    OGS_PFCP_USAGE_REPORT_TRIGGER_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_usage_report_trigger_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_measurement_period =
{
    OGS_TLV_VAR_STR,
    "Measurement Period",
    OGS_PFCP_MEASUREMENT_PERIOD_TYPE,
    0,
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
    OGS_TLV_VAR_STR,
    "Duration Measurement",
    OGS_PFCP_DURATION_MEASUREMENT_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_duration_measurement_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_of_first_packet =
{
    OGS_TLV_VAR_STR,
    "Time of First Packet",
    OGS_PFCP_TIME_OF_FIRST_PACKET_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_time_of_first_packet_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_time_of_last_packet =
{
    OGS_TLV_VAR_STR,
    "Time of Last Packet",
    OGS_PFCP_TIME_OF_LAST_PACKET_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_time_of_last_packet_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_quota_holding_time =
{
    OGS_TLV_VAR_STR,
    "Quota Holding Time",
    OGS_PFCP_QUOTA_HOLDING_TIME_TYPE,
    0,
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
    OGS_TLV_VAR_STR,
    "Time Quota",
    OGS_PFCP_TIME_QUOTA_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_time_quota_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_start_time =
{
    OGS_TLV_VAR_STR,
    "Start Time",
    OGS_PFCP_START_TIME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_start_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_end_time =
{
    OGS_TLV_VAR_STR,
    "End Time",
    OGS_PFCP_END_TIME_TYPE,
    0,
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
    OGS_TLV_VAR_STR,
    "UR-SEQN",
    OGS_PFCP_UR_SEQN_TYPE,
    0,
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
    OGS_TLV_VAR_STR,
    "Event Quota",
    OGS_PFCP_EVENT_QUOTA_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_event_quota_t),
    { NULL }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_event_threshold =
{
    OGS_TLV_VAR_STR,
    "Event Threshold",
    OGS_PFCP_EVENT_THRESHOLD_TYPE,
    0,
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

ogs_tlv_desc_t ogs_pfcp_tlv_desc_event_time_stamp =
{
    OGS_TLV_VAR_STR,
    "Event Time Stamp",
    OGS_PFCP_EVENT_TIME_STAMP_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_event_time_stamp_t),
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
    OGS_TLV_VAR_STR,
    "3GPP Interface Type",
    OGS_PFCP__INTERFACE_TYPE_TYPE,
    0,
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
    OGS_TLV_VAR_STR,
    "Quota Validity Time",
    OGS_PFCP_QUOTA_VALIDITY_TIME_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_quota_validity_time_t),
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
        &ogs_pfcp_tlv_desc_network_instance,
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_traffic_endpoint_id,
        &ogs_pfcp_tlv_desc_sdf_filter,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_application_id,
        &ogs_pfcp_tlv_desc_ethernet_pdu_session_information,
        &ogs_pfcp_tlv_desc_ethernet_packet_filter,
        &ogs_pfcp_tlv_desc_qfi,
        &ogs_pfcp_tlv_desc_framed_route,
        &ogs_pfcp_tlv_desc_framed_routing,
        &ogs_pfcp_tlv_desc_framed_ipv6_route,
        &ogs_pfcp_tlv_desc__interface_type,
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
        &ogs_pfcp_tlv_desc_qer_id,
        &ogs_pfcp_tlv_desc_activate_predefined_rules,
        &ogs_pfcp_tlv_desc_activation_time,
        &ogs_pfcp_tlv_desc_deactivation_time,
        &ogs_pfcp_tlv_desc_mar_id,
        &ogs_pfcp_tlv_desc_packet_replication_and_detection_carry_on_information,
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
        &ogs_pfcp_tlv_desc_bar_id,
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

ogs_tlv_desc_t ogs_pfcp_tlv_desc_access_forwarding_action_information_1 =
{
    OGS_TLV_COMPOUND,
    "Access Forwarding Action Information 1",
    OGS_PFCP_ACCESS_FORWARDING_ACTION_INFORMATION_1_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_access_forwarding_action_information_1_t),
    {
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_weight,
        &ogs_pfcp_tlv_desc_priority,
        &ogs_pfcp_tlv_desc_urr_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_access_forwarding_action_information_2 =
{
    OGS_TLV_COMPOUND,
    "Access Forwarding Action Information 2",
    OGS_PFCP_ACCESS_FORWARDING_ACTION_INFORMATION_2_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_access_forwarding_action_information_2_t),
    {
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_weight,
        &ogs_pfcp_tlv_desc_priority,
        &ogs_pfcp_tlv_desc_urr_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_access_forwarding_action_information_1 =
{
    OGS_TLV_COMPOUND,
    "Update Access Forwarding Action Information 1",
    OGS_PFCP_UPDATE_ACCESS_FORWARDING_ACTION_INFORMATION_1_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_access_forwarding_action_information_1_t),
    {
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_weight,
        &ogs_pfcp_tlv_desc_priority,
        &ogs_pfcp_tlv_desc_urr_id,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_tlv_desc_update_access_forwarding_action_information_2 =
{
    OGS_TLV_COMPOUND,
    "Update Access Forwarding Action Information 2",
    OGS_PFCP_UPDATE_ACCESS_FORWARDING_ACTION_INFORMATION_2_TYPE,
    0,
    0,
    sizeof(ogs_pfcp_tlv_update_access_forwarding_action_information_2_t),
    {
        &ogs_pfcp_tlv_desc_far_id,
        &ogs_pfcp_tlv_desc_weight,
        &ogs_pfcp_tlv_desc_priority,
        &ogs_pfcp_tlv_desc_urr_id,
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
        &ogs_pfcp_tlv_desc_dl_flow_level_marking,
        &ogs_pfcp_tlv_desc_qfi,
        &ogs_pfcp_tlv_desc_rqi,
        &ogs_pfcp_tlv_desc_paging_policy_indicator,
        &ogs_pfcp_tlv_desc_averaging_window,
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
        &ogs_pfcp_tlv_desc_ue_ip_address,
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
        &ogs_pfcp_tlv_desc_event_time_stamp,
        &ogs_pfcp_tlv_desc_ethernet_traffic_information,
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
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_ethernet_pdu_session_information,
        &ogs_pfcp_tlv_desc_framed_route,
        &ogs_pfcp_tlv_desc_framed_routing,
        &ogs_pfcp_tlv_desc_framed_ipv6_route,
        &ogs_pfcp_tlv_desc_qfi,
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
        &ogs_pfcp_tlv_desc_ue_ip_address,
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
        &ogs_pfcp_tlv_desc_access_forwarding_action_information_1,
        &ogs_pfcp_tlv_desc_access_forwarding_action_information_2,
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
        &ogs_pfcp_tlv_desc_update_access_forwarding_action_information_1,
        &ogs_pfcp_tlv_desc_update_access_forwarding_action_information_2,
        &ogs_pfcp_tlv_desc_access_forwarding_action_information_1,
        &ogs_pfcp_tlv_desc_access_forwarding_action_information_2,
        NULL,
    }
};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_heartbeat_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Heartbeat Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_recovery_time_stamp,
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
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_pfd_management_response =
{
    OGS_TLV_MESSAGE,
    "PFCP PFD Management Response",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_cause,
        &ogs_pfcp_tlv_desc_offending_ie,
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
        &ogs_pfcp_tlv_desc_ue_ip_address,
        &ogs_pfcp_tlv_desc_alternative_smf_ip_address,
        &ogs_pfcp_tlv_desc_smf_set_id,
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
        &ogs_pfcp_tlv_desc_pfcp_association_release_request,
        &ogs_pfcp_tlv_desc_graceful_release_period,
        &ogs_pfcp_tlv_desc_user_plane_ip_resource_information,
        &ogs_tlv_desc_more4,
        &ogs_pfcp_tlv_desc_pfcpaureq_flags,
        &ogs_pfcp_tlv_desc_alternative_smf_ip_address,
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
        &ogs_pfcp_tlv_desc_user_plane_path_failure_report,
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

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_establishment_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Establishment Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_node_id,
        &ogs_pfcp_tlv_desc_f_seid,
        &ogs_pfcp_tlv_desc_create_pdr,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_create_far,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_create_urr,
        &ogs_tlv_desc_more2,
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
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_load_control_information,
        &ogs_pfcp_tlv_desc_overload_control_information,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_fq_csid,
        &ogs_pfcp_tlv_desc_failed_rule_id,
        &ogs_pfcp_tlv_desc_created_traffic_endpoint,
    NULL,
}};

ogs_tlv_desc_t ogs_pfcp_msg_desc_pfcp_session_modification_request =
{
    OGS_TLV_MESSAGE,
    "PFCP Session Modification Request",
    0, 0, 0, 0, {
        &ogs_pfcp_tlv_desc_f_seid,
        &ogs_pfcp_tlv_desc_remove_pdr,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_remove_far,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_remove_urr,
        &ogs_tlv_desc_more2,
        &ogs_pfcp_tlv_desc_remove_qer,
        &ogs_tlv_desc_more4,
        &ogs_pfcp_tlv_desc_remove_bar,
        &ogs_pfcp_tlv_desc_remove_traffic_endpoint,
        &ogs_pfcp_tlv_desc_create_pdr,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_create_far,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_create_urr,
        &ogs_tlv_desc_more2,
        &ogs_pfcp_tlv_desc_create_qer,
        &ogs_tlv_desc_more4,
        &ogs_pfcp_tlv_desc_create_bar,
        &ogs_pfcp_tlv_desc_create_traffic_endpoint,
        &ogs_pfcp_tlv_desc_update_pdr,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_update_far,
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_update_urr,
        &ogs_tlv_desc_more2,
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
        &ogs_tlv_desc_more8,
        &ogs_pfcp_tlv_desc_load_control_information,
        &ogs_pfcp_tlv_desc_overload_control_information,
        &ogs_pfcp_tlv_desc_usage_report_session_modification_response,
        &ogs_pfcp_tlv_desc_failed_rule_id,
        &ogs_pfcp_tlv_desc_additional_usage_reports_information,
        &ogs_pfcp_tlv_desc_created_traffic_endpoint,
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
        &ogs_pfcp_tlv_desc_error_indication_report,
        &ogs_pfcp_tlv_desc_load_control_information,
        &ogs_pfcp_tlv_desc_overload_control_information,
        &ogs_pfcp_tlv_desc_additional_usage_reports_information,
        &ogs_pfcp_tlv_desc_pfcpsrreq_flags,
        &ogs_pfcp_tlv_desc_f_seid,
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
    NULL,
}};


int ogs_pfcp_parse_msg(ogs_pfcp_message_t *pfcp_message, ogs_pkbuf_t *pkbuf)
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

    if (h->seid_presence)
        size = OGS_PFCP_HEADER_LEN;
    else
        size = OGS_PFCP_HEADER_LEN-OGS_PFCP_SEID_LEN;

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(&pfcp_message->h, pkbuf->data - size, size);

    if (h->seid_presence) {
        pfcp_message->h.seid = be64toh(pfcp_message->h.seid);
    } else {
        pfcp_message->h.sqn = pfcp_message->h.sqn_only;
        pfcp_message->h.sqn_only = pfcp_message->h.sqn_only;
    }

    if (pkbuf->len == 0)
        return OGS_OK;

    switch(pfcp_message->h.type)
    {
        case OGS_PFCP_HEARTBEAT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_heartbeat_request,
                    &ogs_pfcp_msg_desc_pfcp_heartbeat_request, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_HEARTBEAT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_heartbeat_response,
                    &ogs_pfcp_msg_desc_pfcp_heartbeat_response, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFD_MANAGEMENT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_pfd_management_request,
                    &ogs_pfcp_msg_desc_pfcp_pfd_management_request, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_PFD_MANAGEMENT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_pfd_management_response,
                    &ogs_pfcp_msg_desc_pfcp_pfd_management_response, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_ASSOCIATION_SETUP_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_setup_request,
                    &ogs_pfcp_msg_desc_pfcp_association_setup_request, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_ASSOCIATION_SETUP_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_setup_response,
                    &ogs_pfcp_msg_desc_pfcp_association_setup_response, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_ASSOCIATION_UPDATE_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_update_request,
                    &ogs_pfcp_msg_desc_pfcp_association_update_request, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_ASSOCIATION_UPDATE_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_update_response,
                    &ogs_pfcp_msg_desc_pfcp_association_update_response, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_ASSOCIATION_RELEASE_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_release_request,
                    &ogs_pfcp_msg_desc_pfcp_association_release_request, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_ASSOCIATION_RELEASE_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_association_release_response,
                    &ogs_pfcp_msg_desc_pfcp_association_release_response, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_VERSION_NOT_SUPPORTED_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_version_not_supported_response,
                    &ogs_pfcp_msg_desc_pfcp_version_not_supported_response, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_NODE_REPORT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_node_report_request,
                    &ogs_pfcp_msg_desc_pfcp_node_report_request, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_NODE_REPORT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_node_report_response,
                    &ogs_pfcp_msg_desc_pfcp_node_report_response, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_SET_DELETION_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_set_deletion_request,
                    &ogs_pfcp_msg_desc_pfcp_session_set_deletion_request, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_SET_DELETION_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_set_deletion_response,
                    &ogs_pfcp_msg_desc_pfcp_session_set_deletion_response, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_establishment_request,
                    &ogs_pfcp_msg_desc_pfcp_session_establishment_request, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_establishment_response,
                    &ogs_pfcp_msg_desc_pfcp_session_establishment_response, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_modification_request,
                    &ogs_pfcp_msg_desc_pfcp_session_modification_request, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_modification_response,
                    &ogs_pfcp_msg_desc_pfcp_session_modification_response, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_DELETION_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_deletion_request,
                    &ogs_pfcp_msg_desc_pfcp_session_deletion_request, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_DELETION_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_deletion_response,
                    &ogs_pfcp_msg_desc_pfcp_session_deletion_response, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_REPORT_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_report_request,
                    &ogs_pfcp_msg_desc_pfcp_session_report_request, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        case OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&pfcp_message->pfcp_session_report_response,
                    &ogs_pfcp_msg_desc_pfcp_session_report_response, pkbuf, OGS_TLV_MODE_T2_L2);
            break;
        default:
            ogs_warn("Not implmeneted(type:%d)", pfcp_message->h.type);
            break;
    }

    return rv;
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
            ogs_warn("Not implmeneted(type:%d)", pfcp_message->h.type);
            break;
    }

    return pkbuf;
}

