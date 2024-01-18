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
 * Created on: 2023-12-11 17:21:37.071241 by pespin
 * from 29060-h40.docx
 ******************************************************************************/

#include "ogs-gtp.h"

ogs_tlv_desc_t ogs_gtp1_tlv_desc_cause =
{
    OGS_TV_UINT8,
    "Cause",
    OGS_GTP1_CAUSE_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_cause_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_imsi =
{
    OGS_TV_FIXED_STR,
    "IMSI",
    OGS_GTP1_IMSI_TYPE,
    8,
    0,
    sizeof(ogs_gtp1_tlv_imsi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_rai =
{
    OGS_TV_FIXED_STR,
    "RAI",
    OGS_GTP1_RAI_TYPE,
    6,
    0,
    sizeof(ogs_gtp1_tlv_rai_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_tlli =
{
    OGS_TV_UINT32,
    "TLLI",
    OGS_GTP1_TLLI_TYPE,
    4,
    0,
    sizeof(ogs_gtp1_tlv_tlli_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_p_tmsi =
{
    OGS_TV_UINT32,
    "P-TMSI",
    OGS_GTP1_P_TMSI_TYPE,
    4,
    0,
    sizeof(ogs_gtp1_tlv_p_tmsi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_reordering_required =
{
    OGS_TV_UINT8,
    "Reordering Required",
    OGS_GTP1_REORDERING_REQUIRED_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_reordering_required_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_authentication_triplet =
{
    OGS_TV_FIXED_STR,
    "Authentication Triplet",
    OGS_GTP1_AUTHENTICATION_TRIPLET_TYPE,
    28,
    0,
    sizeof(ogs_gtp1_tlv_authentication_triplet_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_map_cause =
{
    OGS_TV_FIXED_STR,
    "MAP Cause",
    OGS_GTP1_MAP_CAUSE_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_map_cause_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_p_tmsi_signature =
{
    OGS_TV_UINT24,
    "P-TMSI Signature",
    OGS_GTP1_P_TMSI_SIGNATURE_TYPE,
    3,
    0,
    sizeof(ogs_gtp1_tlv_p_tmsi_signature_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ms_validated =
{
    OGS_TV_UINT8,
    "MS Validated",
    OGS_GTP1_MS_VALIDATED_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_ms_validated_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_recovery =
{
    OGS_TV_UINT8,
    "Recovery",
    OGS_GTP1_RECOVERY_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_recovery_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_selection_mode =
{
    OGS_TV_UINT8,
    "Selection Mode",
    OGS_GTP1_SELECTION_MODE_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_selection_mode_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_data_i =
{
    OGS_TV_UINT32,
    "Tunnel Endpoint Identifier Data I",
    OGS_GTP1_TUNNEL_ENDPOINT_IDENTIFIER_DATA_I_TYPE,
    4,
    0,
    sizeof(ogs_gtp1_tlv_tunnel_endpoint_identifier_data_i_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane =
{
    OGS_TV_UINT32,
    "Tunnel Endpoint Identifier Control Plane",
    OGS_GTP1_TUNNEL_ENDPOINT_IDENTIFIER_CONTROL_PLANE_TYPE,
    4,
    0,
    sizeof(ogs_gtp1_tlv_tunnel_endpoint_identifier_control_plane_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_data_ii =
{
    OGS_TV_FIXED_STR,
    "Tunnel Endpoint Identifier Data II",
    OGS_GTP1_TUNNEL_ENDPOINT_IDENTIFIER_DATA_II_TYPE,
    5,
    0,
    sizeof(ogs_gtp1_tlv_tunnel_endpoint_identifier_data_ii_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_teardown_ind =
{
    OGS_TV_UINT8,
    "Teardown Ind",
    OGS_GTP1_TEARDOWN_IND_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_teardown_ind_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_nsapi =
{
    OGS_TV_UINT8,
    "NSAPI",
    OGS_GTP1_NSAPI_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_nsapi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ranap_cause =
{
    OGS_TV_UINT8,
    "RANAP Cause",
    OGS_GTP1_RANAP_CAUSE_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_ranap_cause_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_rab_context =
{
    OGS_TV_FIXED_STR,
    "RAB Context",
    OGS_GTP1_RAB_CONTEXT_TYPE,
    9,
    0,
    sizeof(ogs_gtp1_tlv_rab_context_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_radio_priority_sms =
{
    OGS_TV_UINT8,
    "Radio Priority SMS",
    OGS_GTP1_RADIO_PRIORITY_SMS_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_radio_priority_sms_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_radio_priority =
{
    OGS_TV_UINT8,
    "Radio Priority",
    OGS_GTP1_RADIO_PRIORITY_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_radio_priority_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_packet_flow_id =
{
    OGS_TV_UINT16,
    "Packet Flow Id",
    OGS_GTP1_PACKET_FLOW_ID_TYPE,
    2,
    0,
    sizeof(ogs_gtp1_tlv_packet_flow_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_charging_characteristics =
{
    OGS_TV_FIXED_STR,
    "Charging Characteristics",
    OGS_GTP1_CHARGING_CHARACTERISTICS_TYPE,
    2,
    0,
    sizeof(ogs_gtp1_tlv_charging_characteristics_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_trace_reference =
{
    OGS_TV_UINT16,
    "Trace Reference",
    OGS_GTP1_TRACE_REFERENCE_TYPE,
    2,
    0,
    sizeof(ogs_gtp1_tlv_trace_reference_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_trace_type =
{
    OGS_TV_UINT16,
    "Trace Type",
    OGS_GTP1_TRACE_TYPE_TYPE,
    2,
    0,
    sizeof(ogs_gtp1_tlv_trace_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ms_not_reachable_reason =
{
    OGS_TV_UINT8,
    "MS Not Reachable Reason",
    OGS_GTP1_MS_NOT_REACHABLE_REASON_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_ms_not_reachable_reason_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_charging_id =
{
    OGS_TV_UINT32,
    "Charging ID",
    OGS_GTP1_CHARGING_ID_TYPE,
    4,
    0,
    sizeof(ogs_gtp1_tlv_charging_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_end_user_address =
{
    OGS_TLV_VAR_STR,
    "End User Address",
    OGS_GTP1_END_USER_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_end_user_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mm_context =
{
    OGS_TLV_VAR_STR,
    "MM Context",
    OGS_GTP1_MM_CONTEXT_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_mm_context_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdp_context =
{
    OGS_TLV_VAR_STR,
    "PDP Context",
    OGS_GTP1_PDP_CONTEXT_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_pdp_context_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_access_point_name =
{
    OGS_TLV_VAR_STR,
    "Access Point Name",
    OGS_GTP1_ACCESS_POINT_NAME_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_access_point_name_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_protocol_configuration_options =
{
    OGS_TLV_VAR_STR,
    "Protocol Configuration Options",
    OGS_GTP1_PROTOCOL_CONFIGURATION_OPTIONS_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_protocol_configuration_options_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_gsn_address =
{
    OGS_TLV_VAR_STR,
    "GSN Address",
    OGS_GTP1_GSN_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_gsn_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_msisdn =
{
    OGS_TLV_VAR_STR,
    "MSISDN",
    OGS_GTP1_MSISDN_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_msisdn_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_quality_of_service_profile =
{
    OGS_TLV_VAR_STR,
    "Quality of Service Profile",
    OGS_GTP1_QUALITY_OF_SERVICE_PROFILE_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_quality_of_service_profile_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_authentication_quintuplet =
{
    OGS_TLV_VAR_STR,
    "Authentication Quintuplet",
    OGS_GTP1_AUTHENTICATION_QUINTUPLET_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_authentication_quintuplet_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_traffic_flow_template =
{
    OGS_TLV_VAR_STR,
    "Traffic Flow Template",
    OGS_GTP1_TRAFFIC_FLOW_TEMPLATE_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_traffic_flow_template_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_target_identification =
{
    OGS_TLV_VAR_STR,
    "Target Identification",
    OGS_GTP1_TARGET_IDENTIFICATION_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_target_identification_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_utran_transparent_container =
{
    OGS_TLV_VAR_STR,
    "UTRAN Transparent Container",
    OGS_GTP1_UTRAN_TRANSPARENT_CONTAINER_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_utran_transparent_container_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_rab_setup_information =
{
    OGS_TLV_VAR_STR,
    "RAB Setup Information",
    OGS_GTP1_RAB_SETUP_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_rab_setup_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_extension_header_type_list =
{
    OGS_TLV_VAR_STR,
    "Extension Header Type List",
    OGS_GTP1_EXTENSION_HEADER_TYPE_LIST_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_extension_header_type_list_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_trigger_id =
{
    OGS_TLV_VAR_STR,
    "Trigger Id",
    OGS_GTP1_TRIGGER_ID_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_trigger_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_omc_identity =
{
    OGS_TLV_VAR_STR,
    "OMC Identity",
    OGS_GTP1_OMC_IDENTITY_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_omc_identity_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ran_transparent_container =
{
    OGS_TLV_VAR_STR,
    "RAN Transparent Container",
    OGS_GTP1_RAN_TRANSPARENT_CONTAINER_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_ran_transparent_container_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdp_context_prioritization =
{
    OGS_TLV_FIXED_STR,
    "PDP Context Prioritization",
    OGS_GTP1_PDP_CONTEXT_PRIORITIZATION_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_pdp_context_prioritization_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_additional_rab_setup_information =
{
    OGS_TLV_VAR_STR,
    "Additional RAB Setup Information",
    OGS_GTP1_ADDITIONAL_RAB_SETUP_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_additional_rab_setup_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_sgsn_number =
{
    OGS_TLV_VAR_STR,
    "SGSN Number",
    OGS_GTP1_SGSN_NUMBER_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_sgsn_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_common_flags =
{
    OGS_TLV_FIXED_STR,
    "Common Flags",
    OGS_GTP1_COMMON_FLAGS_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_common_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_apn_restriction =
{
    OGS_TLV_UINT8,
    "APN Restriction",
    OGS_GTP1_APN_RESTRICTION_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_apn_restriction_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_radio_priority_lcs =
{
    OGS_TLV_FIXED_STR,
    "Radio Priority LCS",
    OGS_GTP1_RADIO_PRIORITY_LCS_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_radio_priority_lcs_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_rat_type =
{
    OGS_TLV_UINT8,
    "RAT Type",
    OGS_GTP1_RAT_TYPE_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_rat_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_user_location_information =
{
    OGS_TLV_VAR_STR,
    "User Location Information",
    OGS_GTP1_USER_LOCATION_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_user_location_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ms_time_zone =
{
    OGS_TLV_FIXED_STR,
    "MS Time Zone",
    OGS_GTP1_MS_TIME_ZONE_TYPE,
    2,
    0,
    sizeof(ogs_gtp1_tlv_ms_time_zone_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_sv =
{
    OGS_TLV_FIXED_STR,
    "SV",
    OGS_GTP1_SV_TYPE,
    8,
    0,
    sizeof(ogs_gtp1_tlv_sv_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_camel_charging_information_container =
{
    OGS_TLV_VAR_STR,
    "CAMEL Charging Information Container",
    OGS_GTP1_CAMEL_CHARGING_INFORMATION_CONTAINER_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_camel_charging_information_container_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_ue_context =
{
    OGS_TLV_VAR_STR,
    "MBMS UE Context",
    OGS_GTP1_MBMS_UE_CONTEXT_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_mbms_ue_context_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_tmgi =
{
    OGS_TLV_FIXED_STR,
    "TMGI",
    OGS_GTP1_TMGI_TYPE,
    6,
    0,
    sizeof(ogs_gtp1_tlv_tmgi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_rim_routing_address =
{
    OGS_TLV_VAR_STR,
    "RIM Routing Address",
    OGS_GTP1_RIM_ROUTING_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_rim_routing_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_protocol_configuration_options =
{
    OGS_TLV_VAR_STR,
    "MBMS Protocol Configuration Options",
    OGS_GTP1_MBMS_PROTOCOL_CONFIGURATION_OPTIONS_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_mbms_protocol_configuration_options_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_service_area =
{
    OGS_TLV_VAR_STR,
    "MBMS Service Area",
    OGS_GTP1_MBMS_SERVICE_AREA_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_mbms_service_area_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_source_rnc_pdcp_context_info =
{
    OGS_TLV_VAR_STR,
    "Source RNC PDCP context info",
    OGS_GTP1_SOURCE_RNC_PDCP_CONTEXT_INFO_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_source_rnc_pdcp_context_info_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_additional_trace_info =
{
    OGS_TLV_FIXED_STR,
    "Additional Trace Info",
    OGS_GTP1_ADDITIONAL_TRACE_INFO_TYPE,
    9,
    0,
    sizeof(ogs_gtp1_tlv_additional_trace_info_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_hop_counter =
{
    OGS_TLV_FIXED_STR,
    "Hop Counter",
    OGS_GTP1_HOP_COUNTER_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_hop_counter_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_selected_plmn_id =
{
    OGS_TLV_FIXED_STR,
    "Selected PLMN ID",
    OGS_GTP1_SELECTED_PLMN_ID_TYPE,
    3,
    0,
    sizeof(ogs_gtp1_tlv_selected_plmn_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_identifier =
{
    OGS_TLV_FIXED_STR,
    "MBMS Session Identifier",
    OGS_GTP1_MBMS_SESSION_IDENTIFIER_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_mbms_session_identifier_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_2g_3g_indicator =
{
    OGS_TLV_FIXED_STR,
    "MBMS 2G/3G Indicator",
    OGS_GTP1_MBMS_2G_3G_INDICATOR_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_mbms_2g_3g_indicator_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_enhanced_nsapi =
{
    OGS_TLV_FIXED_STR,
    "Enhanced NSAPI",
    OGS_GTP1_ENHANCED_NSAPI_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_enhanced_nsapi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_duration =
{
    OGS_TLV_FIXED_STR,
    "MBMS Session Duration",
    OGS_GTP1_MBMS_SESSION_DURATION_TYPE,
    3,
    0,
    sizeof(ogs_gtp1_tlv_mbms_session_duration_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_additional_mbms_trace_info =
{
    OGS_TLV_FIXED_STR,
    "Additional MBMS Trace Info",
    OGS_GTP1_ADDITIONAL_MBMS_TRACE_INFO_TYPE,
    8,
    0,
    sizeof(ogs_gtp1_tlv_additional_mbms_trace_info_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_repetition_number =
{
    OGS_TLV_FIXED_STR,
    "MBMS Session Repetition Number",
    OGS_GTP1_MBMS_SESSION_REPETITION_NUMBER_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_mbms_session_repetition_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_time_to_data_transfer =
{
    OGS_TLV_FIXED_STR,
    "MBMS Time To Data Transfer",
    OGS_GTP1_MBMS_TIME_TO_DATA_TRANSFER_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_mbms_time_to_data_transfer_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_bss_container =
{
    OGS_TLV_VAR_STR,
    "BSS Container",
    OGS_GTP1_BSS_CONTAINER_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_bss_container_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_cell_identification =
{
    OGS_TLV_FIXED_STR,
    "Cell Identification",
    OGS_GTP1_CELL_IDENTIFICATION_TYPE,
    17,
    0,
    sizeof(ogs_gtp1_tlv_cell_identification_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdu_numbers =
{
    OGS_TLV_FIXED_STR,
    "PDU Numbers",
    OGS_GTP1_PDU_NUMBERS_TYPE,
    9,
    0,
    sizeof(ogs_gtp1_tlv_pdu_numbers_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_bssgp_cause =
{
    OGS_TLV_FIXED_STR,
    "BSSGP Cause",
    OGS_GTP1_BSSGP_CAUSE_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_bssgp_cause_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_required_mbms_bearer_capabilities =
{
    OGS_TLV_VAR_STR,
    "Required MBMS bearer capabilities",
    OGS_GTP1_REQUIRED_MBMS_BEARER_CAPABILITIES_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_required_mbms_bearer_capabilities_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_rim_routing_address_discriminator =
{
    OGS_TLV_FIXED_STR,
    "RIM Routing Address Discriminator",
    OGS_GTP1_RIM_ROUTING_ADDRESS_DISCRIMINATOR_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_rim_routing_address_discriminator_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_list_of_set_up_pfcs =
{
    OGS_TLV_VAR_STR,
    "List of set-up PFCs",
    OGS_GTP1_LIST_OF_SET_UP_PFCS_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_list_of_set_up_pfcs_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ps_handover_xid_parameters =
{
    OGS_TLV_VAR_STR,
    "PS Handover XID Parameters",
    OGS_GTP1_PS_HANDOVER_XID_PARAMETERS_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_ps_handover_xid_parameters_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ms_info_change_reporting_action =
{
    OGS_TLV_FIXED_STR,
    "MS Info Change Reporting Action",
    OGS_GTP1_MS_INFO_CHANGE_REPORTING_ACTION_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_ms_info_change_reporting_action_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_direct_tunnel_flags =
{
    OGS_TLV_VAR_STR,
    "Direct Tunnel Flags",
    OGS_GTP1_DIRECT_TUNNEL_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_direct_tunnel_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_correlation_id =
{
    OGS_TLV_FIXED_STR,
    "Correlation-ID",
    OGS_GTP1_CORRELATION_ID_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_correlation_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_bearer_control_mode =
{
    OGS_TLV_FIXED_STR,
    "Bearer Control Mode",
    OGS_GTP1_BEARER_CONTROL_MODE_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_bearer_control_mode_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_flow_identifier =
{
    OGS_TLV_VAR_STR,
    "MBMS Flow Identifier",
    OGS_GTP1_MBMS_FLOW_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_mbms_flow_identifier_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_ip_multicast_distribution =
{
    OGS_TLV_VAR_STR,
    "MBMS IP Multicast Distribution",
    OGS_GTP1_MBMS_IP_MULTICAST_DISTRIBUTION_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_mbms_ip_multicast_distribution_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_distribution_acknowledgement =
{
    OGS_TLV_FIXED_STR,
    "MBMS Distribution Acknowledgement",
    OGS_GTP1_MBMS_DISTRIBUTION_ACKNOWLEDGEMENT_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_mbms_distribution_acknowledgement_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_reliable_inter_rat_handover_info =
{
    OGS_TLV_FIXED_STR,
    "Reliable INTER RAT HANDOVER INFO",
    OGS_GTP1_RELIABLE_INTER_RAT_HANDOVER_INFO_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_reliable_inter_rat_handover_info_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_rfsp_index =
{
    OGS_TLV_FIXED_STR,
    "RFSP Index",
    OGS_GTP1_RFSP_INDEX_TYPE,
    2,
    0,
    sizeof(ogs_gtp1_tlv_rfsp_index_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_fqdn =
{
    OGS_TLV_VAR_STR,
    "FQDN",
    OGS_GTP1_FQDN_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_fqdn_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_evolved_allocation_retention_priority_i =
{
    OGS_TLV_FIXED_STR,
    "Evolved Allocation/Retention Priority I",
    OGS_GTP1_EVOLVED_ALLOCATION_RETENTION_PRIORITY_I_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_evolved_allocation_retention_priority_i_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_evolved_allocation_retention_priority_ii =
{
    OGS_TLV_FIXED_STR,
    "Evolved Allocation/Retention Priority II",
    OGS_GTP1_EVOLVED_ALLOCATION_RETENTION_PRIORITY_II_TYPE,
    2,
    0,
    sizeof(ogs_gtp1_tlv_evolved_allocation_retention_priority_ii_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_extended_common_flags =
{
    OGS_TLV_VAR_STR,
    "Extended Common Flags",
    OGS_GTP1_EXTENDED_COMMON_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_extended_common_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_uci =
{
    OGS_TLV_FIXED_STR,
    "UCI",
    OGS_GTP1_UCI_TYPE,
    8,
    0,
    sizeof(ogs_gtp1_tlv_uci_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_csg_information_reporting_action =
{
    OGS_TLV_VAR_STR,
    "CSG Information Reporting Action",
    OGS_GTP1_CSG_INFORMATION_REPORTING_ACTION_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_csg_information_reporting_action_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_csg_id =
{
    OGS_TLV_FIXED_STR,
    "CSG ID",
    OGS_GTP1_CSG_ID_TYPE,
    4,
    0,
    sizeof(ogs_gtp1_tlv_csg_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_cmi =
{
    OGS_TLV_FIXED_STR,
    "CMI",
    OGS_GTP1_CMI_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_cmi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ambr =
{
    OGS_TLV_FIXED_STR,
    "AMBR",
    OGS_GTP1_AMBR_TYPE,
    8,
    0,
    sizeof(ogs_gtp1_tlv_ambr_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ue_network_capability =
{
    OGS_TLV_VAR_STR,
    "UE Network Capability",
    OGS_GTP1_UE_NETWORK_CAPABILITY_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_ue_network_capability_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ue_ambr =
{
    OGS_TLV_VAR_STR,
    "UE-AMBR",
    OGS_GTP1_UE_AMBR_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_ue_ambr_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_apn_ambr_with_nsapi =
{
    OGS_TLV_FIXED_STR,
    "APN-AMBR with NSAPI",
    OGS_GTP1_APN_AMBR_WITH_NSAPI_TYPE,
    9,
    0,
    sizeof(ogs_gtp1_tlv_apn_ambr_with_nsapi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ggsn_back_off_time =
{
    OGS_TLV_VAR_STR,
    "GGSN Back-Off Time",
    OGS_GTP1_GGSN_BACK_OFF_TIME_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_ggsn_back_off_time_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_signalling_priority_indication =
{
    OGS_TLV_VAR_STR,
    "Signalling Priority Indication",
    OGS_GTP1_SIGNALLING_PRIORITY_INDICATION_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_signalling_priority_indication_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_signalling_priority_indication_with_nsapi =
{
    OGS_TLV_VAR_STR,
    "Signalling Priority Indication with NSAPI",
    OGS_GTP1_SIGNALLING_PRIORITY_INDICATION_WITH_NSAPI_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_signalling_priority_indication_with_nsapi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_higher_bitrates_than_16_mbps_flag =
{
    OGS_TLV_FIXED_STR,
    "Higher bitrates than 16 Mbps flag",
    OGS_GTP1_HIGHER_BITRATES_THAN_16_MBPS_FLAG_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_higher_bitrates_than_16_mbps_flag_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_additional_mm_context_for_srvcc =
{
    OGS_TLV_VAR_STR,
    "Additional MM context for SRVCC",
    OGS_GTP1_ADDITIONAL_MM_CONTEXT_FOR_SRVCC_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_additional_mm_context_for_srvcc_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_additional_flags_for_srvcc =
{
    OGS_TLV_VAR_STR,
    "Additional flags for SRVCC",
    OGS_GTP1_ADDITIONAL_FLAGS_FOR_SRVCC_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_additional_flags_for_srvcc_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_stn_sr =
{
    OGS_TLV_VAR_STR,
    "STN-SR",
    OGS_GTP1_STN_SR_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_stn_sr_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_c_msisdn =
{
    OGS_TLV_VAR_STR,
    "C-MSISDN",
    OGS_GTP1_C_MSISDN_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_c_msisdn_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_extended_ranap_cause =
{
    OGS_TLV_VAR_STR,
    "Extended RANAP Cause",
    OGS_GTP1_EXTENDED_RANAP_CAUSE_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_extended_ranap_cause_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_enodeb_id =
{
    OGS_TLV_VAR_STR,
    "eNodeB ID",
    OGS_GTP1_ENODEB_ID_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_enodeb_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_selection_mode_with_nsapi =
{
    OGS_TLV_FIXED_STR,
    "Selection Mode with NSAPI",
    OGS_GTP1_SELECTION_MODE_WITH_NSAPI_TYPE,
    2,
    0,
    sizeof(ogs_gtp1_tlv_selection_mode_with_nsapi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_uli_timestamp =
{
    OGS_TLV_VAR_STR,
    "ULI Timestamp",
    OGS_GTP1_ULI_TIMESTAMP_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_uli_timestamp_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_lhn_id_with_nsapi =
{
    OGS_TLV_VAR_STR,
    "LHN-ID with NSAPI",
    OGS_GTP1_LHN_ID_WITH_NSAPI_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_lhn_id_with_nsapi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_cn_operator_selection_entity =
{
    OGS_TLV_VAR_STR,
    "CN Operator Selection Entity",
    OGS_GTP1_CN_OPERATOR_SELECTION_ENTITY_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_cn_operator_selection_entity_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ue_usage_type =
{
    OGS_TLV_VAR_STR,
    "UE Usage Type",
    OGS_GTP1_UE_USAGE_TYPE_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_ue_usage_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_extended_common_flags_ii =
{
    OGS_TLV_VAR_STR,
    "Extended Common Flags II",
    OGS_GTP1_EXTENDED_COMMON_FLAGS_II_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_extended_common_flags_ii_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_node_identifier =
{
    OGS_TLV_VAR_STR,
    "Node Identifier",
    OGS_GTP1_NODE_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_node_identifier_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ciot_optimizations_support_indication =
{
    OGS_TLV_VAR_STR,
    "CIoT Optimizations Support Indication",
    OGS_GTP1_CIOT_OPTIMIZATIONS_SUPPORT_INDICATION_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_ciot_optimizations_support_indication_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_scef_pdn_connection =
{
    OGS_TLV_VAR_STR,
    "SCEF PDN Connection",
    OGS_GTP1_SCEF_PDN_CONNECTION_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_scef_pdn_connection_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_iov_updates_counter =
{
    OGS_TLV_FIXED_STR,
    "IOV_updates counter",
    OGS_GTP1_IOV_UPDATES_COUNTER_TYPE,
    1,
    0,
    sizeof(ogs_gtp1_tlv_iov_updates_counter_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mapped_ue_usage_type =
{
    OGS_TLV_VAR_STR,
    "Mapped UE Usage Type",
    OGS_GTP1_MAPPED_UE_USAGE_TYPE_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_mapped_ue_usage_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_up_function_selection_indication_flags =
{
    OGS_TLV_VAR_STR,
    "UP Function Selection Indication Flags",
    OGS_GTP1_UP_FUNCTION_SELECTION_INDICATION_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_up_function_selection_indication_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_special_ie_type_for_ie_type_extension =
{
    OGS_TLV_VAR_STR,
    "Special IE type for IE Type Extension",
    OGS_GTP1_SPECIAL_IE_TYPE_FOR_IE_TYPE_EXTENSION_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_special_ie_type_for_ie_type_extension_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_charging_gateway_address =
{
    OGS_TLV_VAR_STR,
    "Charging Gateway Address",
    OGS_GTP1_CHARGING_GATEWAY_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_gtp1_tlv_charging_gateway_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_echo_request =
{
    OGS_TLV_MESSAGE,
    "Echo Request",
    0, 0, 0, 0, {
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_echo_response =
{
    OGS_TLV_MESSAGE,
    "Echo Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_recovery,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_create_pdp_context_request =
{
    OGS_TLV_MESSAGE,
    "Create PDP Context Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_rai,
        &ogs_gtp1_tlv_desc_recovery,
        &ogs_gtp1_tlv_desc_selection_mode,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_data_i,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_nsapi,
        &ogs_gtp1_tlv_desc_nsapi,
        &ogs_gtp1_tlv_desc_charging_characteristics,
        &ogs_gtp1_tlv_desc_trace_reference,
        &ogs_gtp1_tlv_desc_trace_type,
        &ogs_gtp1_tlv_desc_end_user_address,
        &ogs_gtp1_tlv_desc_access_point_name,
        &ogs_gtp1_tlv_desc_protocol_configuration_options,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_msisdn,
        &ogs_gtp1_tlv_desc_quality_of_service_profile,
        &ogs_gtp1_tlv_desc_traffic_flow_template,
        &ogs_gtp1_tlv_desc_trigger_id,
        &ogs_gtp1_tlv_desc_omc_identity,
        &ogs_gtp1_tlv_desc_common_flags,
        &ogs_gtp1_tlv_desc_apn_restriction,
        &ogs_gtp1_tlv_desc_rat_type,
        &ogs_gtp1_tlv_desc_user_location_information,
        &ogs_gtp1_tlv_desc_ms_time_zone,
        &ogs_gtp1_tlv_desc_sv,
        &ogs_gtp1_tlv_desc_camel_charging_information_container,
        &ogs_gtp1_tlv_desc_additional_trace_info,
        &ogs_gtp1_tlv_desc_correlation_id,
        &ogs_gtp1_tlv_desc_evolved_allocation_retention_priority_i,
        &ogs_gtp1_tlv_desc_extended_common_flags,
        &ogs_gtp1_tlv_desc_uci,
        &ogs_gtp1_tlv_desc_ambr,
        &ogs_gtp1_tlv_desc_signalling_priority_indication,
        &ogs_gtp1_tlv_desc_cn_operator_selection_entity,
        &ogs_gtp1_tlv_desc_mapped_ue_usage_type,
        &ogs_gtp1_tlv_desc_up_function_selection_indication_flags,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_create_pdp_context_response =
{
    OGS_TLV_MESSAGE,
    "Create PDP Context Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_reordering_required,
        &ogs_gtp1_tlv_desc_recovery,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_data_i,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_nsapi,
        &ogs_gtp1_tlv_desc_charging_id,
        &ogs_gtp1_tlv_desc_end_user_address,
        &ogs_gtp1_tlv_desc_protocol_configuration_options,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_quality_of_service_profile,
        &ogs_gtp1_tlv_desc_charging_gateway_address,
        &ogs_gtp1_tlv_desc_charging_gateway_address,
        &ogs_gtp1_tlv_desc_common_flags,
        &ogs_gtp1_tlv_desc_apn_restriction,
        &ogs_gtp1_tlv_desc_ms_info_change_reporting_action,
        &ogs_gtp1_tlv_desc_bearer_control_mode,
        &ogs_gtp1_tlv_desc_evolved_allocation_retention_priority_i,
        &ogs_gtp1_tlv_desc_extended_common_flags,
        &ogs_gtp1_tlv_desc_csg_information_reporting_action,
        &ogs_gtp1_tlv_desc_ambr,
        &ogs_gtp1_tlv_desc_ggsn_back_off_time,
        &ogs_gtp1_tlv_desc_extended_common_flags_ii,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_update_pdp_context_request =
{
    OGS_TLV_MESSAGE,
    "Update PDP Context Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_rai,
        &ogs_gtp1_tlv_desc_recovery,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_data_i,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_nsapi,
        &ogs_gtp1_tlv_desc_trace_reference,
        &ogs_gtp1_tlv_desc_trace_type,
        &ogs_gtp1_tlv_desc_protocol_configuration_options,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_quality_of_service_profile,
        &ogs_gtp1_tlv_desc_traffic_flow_template,
        &ogs_gtp1_tlv_desc_trigger_id,
        &ogs_gtp1_tlv_desc_omc_identity,
        &ogs_gtp1_tlv_desc_common_flags,
        &ogs_gtp1_tlv_desc_rat_type,
        &ogs_gtp1_tlv_desc_user_location_information,
        &ogs_gtp1_tlv_desc_ms_time_zone,
        &ogs_gtp1_tlv_desc_additional_trace_info,
        &ogs_gtp1_tlv_desc_direct_tunnel_flags,
        &ogs_gtp1_tlv_desc_evolved_allocation_retention_priority_i,
        &ogs_gtp1_tlv_desc_extended_common_flags,
        &ogs_gtp1_tlv_desc_uci,
        &ogs_gtp1_tlv_desc_ambr,
        &ogs_gtp1_tlv_desc_signalling_priority_indication,
        &ogs_gtp1_tlv_desc_cn_operator_selection_entity,
        &ogs_gtp1_tlv_desc_sv,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_update_pdp_context_response =
{
    OGS_TLV_MESSAGE,
    "Update PDP Context Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_recovery,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_data_i,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_charging_id,
        &ogs_gtp1_tlv_desc_protocol_configuration_options,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_quality_of_service_profile,
        &ogs_gtp1_tlv_desc_charging_gateway_address,
        &ogs_gtp1_tlv_desc_charging_gateway_address,
        &ogs_gtp1_tlv_desc_common_flags,
        &ogs_gtp1_tlv_desc_apn_restriction,
        &ogs_gtp1_tlv_desc_bearer_control_mode,
        &ogs_gtp1_tlv_desc_ms_info_change_reporting_action,
        &ogs_gtp1_tlv_desc_evolved_allocation_retention_priority_i,
        &ogs_gtp1_tlv_desc_csg_information_reporting_action,
        &ogs_gtp1_tlv_desc_ambr,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_delete_pdp_context_request =
{
    OGS_TLV_MESSAGE,
    "Delete PDP Context Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_teardown_ind,
        &ogs_gtp1_tlv_desc_nsapi,
        &ogs_gtp1_tlv_desc_protocol_configuration_options,
        &ogs_gtp1_tlv_desc_user_location_information,
        &ogs_gtp1_tlv_desc_ms_time_zone,
        &ogs_gtp1_tlv_desc_extended_common_flags,
        &ogs_gtp1_tlv_desc_uli_timestamp,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_delete_pdp_context_response =
{
    OGS_TLV_MESSAGE,
    "Delete PDP Context Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_protocol_configuration_options,
        &ogs_gtp1_tlv_desc_user_location_information,
        &ogs_gtp1_tlv_desc_ms_time_zone,
        &ogs_gtp1_tlv_desc_uli_timestamp,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_initiate_pdp_context_activation_request =
{
    OGS_TLV_MESSAGE,
    "Initiate PDP Context Activation Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_nsapi,
        &ogs_gtp1_tlv_desc_protocol_configuration_options,
        &ogs_gtp1_tlv_desc_quality_of_service_profile,
        &ogs_gtp1_tlv_desc_traffic_flow_template,
        &ogs_gtp1_tlv_desc_correlation_id,
        &ogs_gtp1_tlv_desc_evolved_allocation_retention_priority_i,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_initiate_pdp_context_activation_response =
{
    OGS_TLV_MESSAGE,
    "Initiate PDP Context Activation Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_protocol_configuration_options,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdu_notification_request =
{
    OGS_TLV_MESSAGE,
    "PDU Notification Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_end_user_address,
        &ogs_gtp1_tlv_desc_access_point_name,
        &ogs_gtp1_tlv_desc_protocol_configuration_options,
        &ogs_gtp1_tlv_desc_gsn_address,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdu_notification_response =
{
    OGS_TLV_MESSAGE,
    "PDU Notification Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdu_notification_reject_request =
{
    OGS_TLV_MESSAGE,
    "PDU Notification Reject Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_end_user_address,
        &ogs_gtp1_tlv_desc_access_point_name,
        &ogs_gtp1_tlv_desc_protocol_configuration_options,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_pdu_notification_reject_response =
{
    OGS_TLV_MESSAGE,
    "PDU Notification Reject Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_supported_extension_headers_notification =
{
    OGS_TLV_MESSAGE,
    "Supported Extension Headers Notification",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_extension_header_type_list,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_send_routeing_information_for_gprs_request =
{
    OGS_TLV_MESSAGE,
    "Send Routeing Information for GPRS Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_imsi,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_send_routeing_information_for_gprs_response =
{
    OGS_TLV_MESSAGE,
    "Send Routeing Information for GPRS Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_map_cause,
        &ogs_gtp1_tlv_desc_ms_not_reachable_reason,
        &ogs_gtp1_tlv_desc_gsn_address,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_failure_report_request =
{
    OGS_TLV_MESSAGE,
    "Failure Report Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_imsi,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_failure_report_response =
{
    OGS_TLV_MESSAGE,
    "Failure Report Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_map_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_note_ms_gprs_present_request =
{
    OGS_TLV_MESSAGE,
    "Note MS GPRS Present Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_gsn_address,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_note_ms_gprs_present_response =
{
    OGS_TLV_MESSAGE,
    "Note MS GPRS Present Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_identification_request =
{
    OGS_TLV_MESSAGE,
    "Identification Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_rai,
        &ogs_gtp1_tlv_desc_p_tmsi,
        &ogs_gtp1_tlv_desc_p_tmsi_signature,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_hop_counter,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_identification_response =
{
    OGS_TLV_MESSAGE,
    "Identification Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_authentication_triplet,
        &ogs_gtp1_tlv_desc_authentication_quintuplet,
        &ogs_gtp1_tlv_desc_ue_usage_type,
        &ogs_gtp1_tlv_desc_iov_updates_counter,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_sgsn_context_request =
{
    OGS_TLV_MESSAGE,
    "SGSN Context Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_rai,
        &ogs_gtp1_tlv_desc_tlli,
        &ogs_gtp1_tlv_desc_p_tmsi,
        &ogs_gtp1_tlv_desc_p_tmsi_signature,
        &ogs_gtp1_tlv_desc_ms_validated,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_sgsn_number,
        &ogs_gtp1_tlv_desc_rat_type,
        &ogs_gtp1_tlv_desc_hop_counter,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_sgsn_context_response =
{
    OGS_TLV_MESSAGE,
    "SGSN Context Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_rab_context,
        &ogs_gtp1_tlv_desc_radio_priority_sms,
        &ogs_gtp1_tlv_desc_radio_priority,
        &ogs_gtp1_tlv_desc_packet_flow_id,
        &ogs_gtp1_tlv_desc_charging_characteristics,
        &ogs_gtp1_tlv_desc_radio_priority_lcs,
        &ogs_gtp1_tlv_desc_mm_context,
        &ogs_gtp1_tlv_desc_pdp_context,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_pdp_context_prioritization,
        &ogs_gtp1_tlv_desc_mbms_ue_context,
        &ogs_gtp1_tlv_desc_rfsp_index,
        &ogs_gtp1_tlv_desc_rfsp_index,
        &ogs_gtp1_tlv_desc_fqdn,
        &ogs_gtp1_tlv_desc_evolved_allocation_retention_priority_ii,
        &ogs_gtp1_tlv_desc_extended_common_flags,
        &ogs_gtp1_tlv_desc_ue_network_capability,
        &ogs_gtp1_tlv_desc_ue_ambr,
        &ogs_gtp1_tlv_desc_apn_ambr_with_nsapi,
        &ogs_gtp1_tlv_desc_signalling_priority_indication_with_nsapi,
        &ogs_gtp1_tlv_desc_higher_bitrates_than_16_mbps_flag,
        &ogs_gtp1_tlv_desc_selection_mode_with_nsapi,
        &ogs_gtp1_tlv_desc_lhn_id_with_nsapi,
        &ogs_gtp1_tlv_desc_ue_usage_type,
        &ogs_gtp1_tlv_desc_extended_common_flags_ii,
        &ogs_gtp1_tlv_desc_scef_pdn_connection,
        &ogs_gtp1_tlv_desc_iov_updates_counter,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_sgsn_context_acknowledge =
{
    OGS_TLV_MESSAGE,
    "SGSN Context Acknowledge",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_data_ii,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_sgsn_number,
        &ogs_gtp1_tlv_desc_node_identifier,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_forward_relocation_request =
{
    OGS_TLV_MESSAGE,
    "Forward Relocation Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_ranap_cause,
        &ogs_gtp1_tlv_desc_packet_flow_id,
        &ogs_gtp1_tlv_desc_charging_characteristics,
        &ogs_gtp1_tlv_desc_mm_context,
        &ogs_gtp1_tlv_desc_pdp_context,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_target_identification,
        &ogs_gtp1_tlv_desc_utran_transparent_container,
        &ogs_gtp1_tlv_desc_pdp_context_prioritization,
        &ogs_gtp1_tlv_desc_mbms_ue_context,
        &ogs_gtp1_tlv_desc_selected_plmn_id,
        &ogs_gtp1_tlv_desc_bss_container,
        &ogs_gtp1_tlv_desc_cell_identification,
        &ogs_gtp1_tlv_desc_bssgp_cause,
        &ogs_gtp1_tlv_desc_ps_handover_xid_parameters,
        &ogs_gtp1_tlv_desc_direct_tunnel_flags,
        &ogs_gtp1_tlv_desc_reliable_inter_rat_handover_info,
        &ogs_gtp1_tlv_desc_rfsp_index,
        &ogs_gtp1_tlv_desc_rfsp_index,
        &ogs_gtp1_tlv_desc_fqdn,
        &ogs_gtp1_tlv_desc_evolved_allocation_retention_priority_ii,
        &ogs_gtp1_tlv_desc_extended_common_flags,
        &ogs_gtp1_tlv_desc_csg_id,
        &ogs_gtp1_tlv_desc_cmi,
        &ogs_gtp1_tlv_desc_ue_network_capability,
        &ogs_gtp1_tlv_desc_ue_ambr,
        &ogs_gtp1_tlv_desc_apn_ambr_with_nsapi,
        &ogs_gtp1_tlv_desc_signalling_priority_indication_with_nsapi,
        &ogs_gtp1_tlv_desc_higher_bitrates_than_16_mbps_flag,
        &ogs_gtp1_tlv_desc_additional_mm_context_for_srvcc,
        &ogs_gtp1_tlv_desc_additional_flags_for_srvcc,
        &ogs_gtp1_tlv_desc_stn_sr,
        &ogs_gtp1_tlv_desc_c_msisdn,
        &ogs_gtp1_tlv_desc_extended_ranap_cause,
        &ogs_gtp1_tlv_desc_enodeb_id,
        &ogs_gtp1_tlv_desc_selection_mode_with_nsapi,
        &ogs_gtp1_tlv_desc_ue_usage_type,
        &ogs_gtp1_tlv_desc_extended_common_flags_ii,
        &ogs_gtp1_tlv_desc_scef_pdn_connection,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_forward_relocation_response =
{
    OGS_TLV_MESSAGE,
    "Forward Relocation Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_data_ii,
        &ogs_gtp1_tlv_desc_ranap_cause,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_utran_transparent_container,
        &ogs_gtp1_tlv_desc_rab_setup_information,
        &ogs_gtp1_tlv_desc_additional_rab_setup_information,
        &ogs_gtp1_tlv_desc_sgsn_number,
        &ogs_gtp1_tlv_desc_bss_container,
        &ogs_gtp1_tlv_desc_bssgp_cause,
        &ogs_gtp1_tlv_desc_list_of_set_up_pfcs,
        &ogs_gtp1_tlv_desc_extended_ranap_cause,
        &ogs_gtp1_tlv_desc_node_identifier,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_forward_relocation_complete =
{
    OGS_TLV_MESSAGE,
    "Forward Relocation Complete",
    0, 0, 0, 0, {
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_relocation_cancel_request =
{
    OGS_TLV_MESSAGE,
    "Relocation Cancel Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_sv,
        &ogs_gtp1_tlv_desc_extended_common_flags,
        &ogs_gtp1_tlv_desc_extended_ranap_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_relocation_cancel_response =
{
    OGS_TLV_MESSAGE,
    "Relocation Cancel Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_forward_srns_context =
{
    OGS_TLV_MESSAGE,
    "Forward SRNS Context",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_rab_context,
        &ogs_gtp1_tlv_desc_source_rnc_pdcp_context_info,
        &ogs_gtp1_tlv_desc_pdu_numbers,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_forward_relocation_complete_acknowledge =
{
    OGS_TLV_MESSAGE,
    "Forward Relocation Complete Acknowledge",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_forward_srns_context_acknowledge =
{
    OGS_TLV_MESSAGE,
    "Forward SRNS Context Acknowledge",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ue_registration_query_request =
{
    OGS_TLV_MESSAGE,
    "UE Registration Query Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_imsi,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ue_registration_query_response =
{
    OGS_TLV_MESSAGE,
    "UE Registration Query Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_selected_plmn_id,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_ran_information_relay =
{
    OGS_TLV_MESSAGE,
    "RAN Information Relay",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_ran_transparent_container,
        &ogs_gtp1_tlv_desc_rim_routing_address,
        &ogs_gtp1_tlv_desc_rim_routing_address_discriminator,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_notification_request =
{
    OGS_TLV_MESSAGE,
    "MBMS Notification Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_nsapi,
        &ogs_gtp1_tlv_desc_end_user_address,
        &ogs_gtp1_tlv_desc_access_point_name,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_mbms_protocol_configuration_options,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_notification_response =
{
    OGS_TLV_MESSAGE,
    "MBMS Notification Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_notification_reject_request =
{
    OGS_TLV_MESSAGE,
    "MBMS Notification Reject Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_nsapi,
        &ogs_gtp1_tlv_desc_end_user_address,
        &ogs_gtp1_tlv_desc_access_point_name,
        &ogs_gtp1_tlv_desc_gsn_address,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_notification_reject_response =
{
    OGS_TLV_MESSAGE,
    "MBMS Notification Reject Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_create_mbms_context_request =
{
    OGS_TLV_MESSAGE,
    "Create MBMS Context Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_rai,
        &ogs_gtp1_tlv_desc_recovery,
        &ogs_gtp1_tlv_desc_selection_mode,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_trace_reference,
        &ogs_gtp1_tlv_desc_trace_type,
        &ogs_gtp1_tlv_desc_end_user_address,
        &ogs_gtp1_tlv_desc_access_point_name,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_msisdn,
        &ogs_gtp1_tlv_desc_trigger_id,
        &ogs_gtp1_tlv_desc_omc_identity,
        &ogs_gtp1_tlv_desc_rat_type,
        &ogs_gtp1_tlv_desc_user_location_information,
        &ogs_gtp1_tlv_desc_ms_time_zone,
        &ogs_gtp1_tlv_desc_sv,
        &ogs_gtp1_tlv_desc_mbms_protocol_configuration_options,
        &ogs_gtp1_tlv_desc_additional_trace_info,
        &ogs_gtp1_tlv_desc_enhanced_nsapi,
        &ogs_gtp1_tlv_desc_additional_mbms_trace_info,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_create_mbms_context_response =
{
    OGS_TLV_MESSAGE,
    "Create MBMS Context Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_recovery,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_charging_id,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_charging_gateway_address,
        &ogs_gtp1_tlv_desc_charging_gateway_address,
        &ogs_gtp1_tlv_desc_mbms_protocol_configuration_options,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_update_mbms_context_request =
{
    OGS_TLV_MESSAGE,
    "Update MBMS Context Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_rai,
        &ogs_gtp1_tlv_desc_recovery,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_trace_reference,
        &ogs_gtp1_tlv_desc_trace_type,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_trigger_id,
        &ogs_gtp1_tlv_desc_omc_identity,
        &ogs_gtp1_tlv_desc_rat_type,
        &ogs_gtp1_tlv_desc_user_location_information,
        &ogs_gtp1_tlv_desc_ms_time_zone,
        &ogs_gtp1_tlv_desc_additional_trace_info,
        &ogs_gtp1_tlv_desc_enhanced_nsapi,
        &ogs_gtp1_tlv_desc_additional_mbms_trace_info,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_update_mbms_context_response =
{
    OGS_TLV_MESSAGE,
    "Update MBMS Context Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_recovery,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_charging_id,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_charging_gateway_address,
        &ogs_gtp1_tlv_desc_charging_gateway_address,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_delete_mbms_context_request =
{
    OGS_TLV_MESSAGE,
    "Delete MBMS Context Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_imsi,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_end_user_address,
        &ogs_gtp1_tlv_desc_access_point_name,
        &ogs_gtp1_tlv_desc_mbms_protocol_configuration_options,
        &ogs_gtp1_tlv_desc_enhanced_nsapi,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_delete_mbms_context_response =
{
    OGS_TLV_MESSAGE,
    "Delete MBMS Context Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_mbms_protocol_configuration_options,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_registration_request =
{
    OGS_TLV_MESSAGE,
    "MBMS Registration Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_end_user_address,
        &ogs_gtp1_tlv_desc_access_point_name,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_registration_response =
{
    OGS_TLV_MESSAGE,
    "MBMS Registration Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_tmgi,
        &ogs_gtp1_tlv_desc_required_mbms_bearer_capabilities,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_de_registration_request =
{
    OGS_TLV_MESSAGE,
    "MBMS De-Registration Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_end_user_address,
        &ogs_gtp1_tlv_desc_access_point_name,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_de_registration_response =
{
    OGS_TLV_MESSAGE,
    "MBMS De-Registration Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_start_request =
{
    OGS_TLV_MESSAGE,
    "MBMS Session Start Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_recovery,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_end_user_address,
        &ogs_gtp1_tlv_desc_access_point_name,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_quality_of_service_profile,
        &ogs_gtp1_tlv_desc_common_flags,
        &ogs_gtp1_tlv_desc_tmgi,
        &ogs_gtp1_tlv_desc_mbms_service_area,
        &ogs_gtp1_tlv_desc_mbms_session_identifier,
        &ogs_gtp1_tlv_desc_mbms_2g_3g_indicator,
        &ogs_gtp1_tlv_desc_mbms_session_duration,
        &ogs_gtp1_tlv_desc_mbms_session_repetition_number,
        &ogs_gtp1_tlv_desc_mbms_time_to_data_transfer,
        &ogs_gtp1_tlv_desc_mbms_flow_identifier,
        &ogs_gtp1_tlv_desc_mbms_ip_multicast_distribution,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_start_response =
{
    OGS_TLV_MESSAGE,
    "MBMS Session Start Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_recovery,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_data_i,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_mbms_distribution_acknowledgement,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_stop_request =
{
    OGS_TLV_MESSAGE,
    "MBMS Session Stop Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_end_user_address,
        &ogs_gtp1_tlv_desc_access_point_name,
        &ogs_gtp1_tlv_desc_mbms_flow_identifier,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_stop_response =
{
    OGS_TLV_MESSAGE,
    "MBMS Session Stop Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_update_request =
{
    OGS_TLV_MESSAGE,
    "MBMS Session Update Request",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_end_user_address,
        &ogs_gtp1_tlv_desc_access_point_name,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_tmgi,
        &ogs_gtp1_tlv_desc_mbms_session_duration,
        &ogs_gtp1_tlv_desc_mbms_service_area,
        &ogs_gtp1_tlv_desc_mbms_session_identifier,
        &ogs_gtp1_tlv_desc_mbms_session_repetition_number,
        &ogs_gtp1_tlv_desc_mbms_flow_identifier,
    NULL,
}};

ogs_tlv_desc_t ogs_gtp1_tlv_desc_mbms_session_update_response =
{
    OGS_TLV_MESSAGE,
    "MBMS Session Update Response",
    0, 0, 0, 0, {
        &ogs_gtp1_tlv_desc_cause,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_data_i,
        &ogs_gtp1_tlv_desc_tunnel_endpoint_identifier_control_plane,
        &ogs_gtp1_tlv_desc_gsn_address,
        &ogs_gtp1_tlv_desc_gsn_address,
    NULL,
}};


int ogs_gtp1_parse_msg(ogs_gtp1_message_t *gtp1_message, ogs_pkbuf_t *pkbuf)
{
    int rv = OGS_ERROR;
    ogs_gtp1_header_t *h = NULL;
    uint16_t size = 0;

    ogs_assert(gtp1_message);
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);

    h = (ogs_gtp1_header_t *)pkbuf->data;
    ogs_assert(h);

    memset(gtp1_message, 0, sizeof(ogs_gtp1_message_t));

    if (h->e || h->s || h->pn)
        size = OGS_GTPV1C_HEADER_LEN;
    else
        size = OGS_GTPV1C_HEADER_LEN - 4;

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
        ogs_error("ogs_pkbuf_pull() failed [len:%d]", pkbuf->len);
        return OGS_ERROR;
    }
    memcpy(&gtp1_message->h, pkbuf->data - size, size);

    gtp1_message->h.teid = be32toh(gtp1_message->h.teid);

    if (pkbuf->len == 0) {
        ogs_assert(ogs_pkbuf_push(pkbuf, size));
        return OGS_OK;
    }

    switch(gtp1_message->h.type) {
    case OGS_GTP1_ECHO_REQUEST_TYPE:
        break;
    case OGS_GTP1_ECHO_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->echo_response,
                &ogs_gtp1_tlv_desc_echo_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_CREATE_PDP_CONTEXT_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->create_pdp_context_request,
                &ogs_gtp1_tlv_desc_create_pdp_context_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_CREATE_PDP_CONTEXT_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->create_pdp_context_response,
                &ogs_gtp1_tlv_desc_create_pdp_context_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_UPDATE_PDP_CONTEXT_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->update_pdp_context_request,
                &ogs_gtp1_tlv_desc_update_pdp_context_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_UPDATE_PDP_CONTEXT_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->update_pdp_context_response,
                &ogs_gtp1_tlv_desc_update_pdp_context_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_DELETE_PDP_CONTEXT_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->delete_pdp_context_request,
                &ogs_gtp1_tlv_desc_delete_pdp_context_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_DELETE_PDP_CONTEXT_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->delete_pdp_context_response,
                &ogs_gtp1_tlv_desc_delete_pdp_context_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_INITIATE_PDP_CONTEXT_ACTIVATION_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->initiate_pdp_context_activation_request,
                &ogs_gtp1_tlv_desc_initiate_pdp_context_activation_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_INITIATE_PDP_CONTEXT_ACTIVATION_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->initiate_pdp_context_activation_response,
                &ogs_gtp1_tlv_desc_initiate_pdp_context_activation_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_PDU_NOTIFICATION_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->pdu_notification_request,
                &ogs_gtp1_tlv_desc_pdu_notification_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_PDU_NOTIFICATION_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->pdu_notification_response,
                &ogs_gtp1_tlv_desc_pdu_notification_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_PDU_NOTIFICATION_REJECT_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->pdu_notification_reject_request,
                &ogs_gtp1_tlv_desc_pdu_notification_reject_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_PDU_NOTIFICATION_REJECT_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->pdu_notification_reject_response,
                &ogs_gtp1_tlv_desc_pdu_notification_reject_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_SUPPORTED_EXTENSION_HEADERS_NOTIFICATION_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->supported_extension_headers_notification,
                &ogs_gtp1_tlv_desc_supported_extension_headers_notification, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_SEND_ROUTEING_INFORMATION_FOR_GPRS_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->send_routeing_information_for_gprs_request,
                &ogs_gtp1_tlv_desc_send_routeing_information_for_gprs_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_SEND_ROUTEING_INFORMATION_FOR_GPRS_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->send_routeing_information_for_gprs_response,
                &ogs_gtp1_tlv_desc_send_routeing_information_for_gprs_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FAILURE_REPORT_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->failure_report_request,
                &ogs_gtp1_tlv_desc_failure_report_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FAILURE_REPORT_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->failure_report_response,
                &ogs_gtp1_tlv_desc_failure_report_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_NOTE_MS_GPRS_PRESENT_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->note_ms_gprs_present_request,
                &ogs_gtp1_tlv_desc_note_ms_gprs_present_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_NOTE_MS_GPRS_PRESENT_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->note_ms_gprs_present_response,
                &ogs_gtp1_tlv_desc_note_ms_gprs_present_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_IDENTIFICATION_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->identification_request,
                &ogs_gtp1_tlv_desc_identification_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_IDENTIFICATION_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->identification_response,
                &ogs_gtp1_tlv_desc_identification_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_SGSN_CONTEXT_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->sgsn_context_request,
                &ogs_gtp1_tlv_desc_sgsn_context_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_SGSN_CONTEXT_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->sgsn_context_response,
                &ogs_gtp1_tlv_desc_sgsn_context_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_SGSN_CONTEXT_ACKNOWLEDGE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->sgsn_context_acknowledge,
                &ogs_gtp1_tlv_desc_sgsn_context_acknowledge, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FORWARD_RELOCATION_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->forward_relocation_request,
                &ogs_gtp1_tlv_desc_forward_relocation_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FORWARD_RELOCATION_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->forward_relocation_response,
                &ogs_gtp1_tlv_desc_forward_relocation_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FORWARD_RELOCATION_COMPLETE_TYPE:
        break;
    case OGS_GTP1_RELOCATION_CANCEL_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->relocation_cancel_request,
                &ogs_gtp1_tlv_desc_relocation_cancel_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_RELOCATION_CANCEL_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->relocation_cancel_response,
                &ogs_gtp1_tlv_desc_relocation_cancel_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FORWARD_SRNS_CONTEXT_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->forward_srns_context,
                &ogs_gtp1_tlv_desc_forward_srns_context, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->forward_relocation_complete_acknowledge,
                &ogs_gtp1_tlv_desc_forward_relocation_complete_acknowledge, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FORWARD_SRNS_CONTEXT_ACKNOWLEDGE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->forward_srns_context_acknowledge,
                &ogs_gtp1_tlv_desc_forward_srns_context_acknowledge, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_UE_REGISTRATION_QUERY_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->ue_registration_query_request,
                &ogs_gtp1_tlv_desc_ue_registration_query_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_UE_REGISTRATION_QUERY_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->ue_registration_query_response,
                &ogs_gtp1_tlv_desc_ue_registration_query_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_RAN_INFORMATION_RELAY_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->ran_information_relay,
                &ogs_gtp1_tlv_desc_ran_information_relay, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_NOTIFICATION_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_notification_request,
                &ogs_gtp1_tlv_desc_mbms_notification_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_NOTIFICATION_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_notification_response,
                &ogs_gtp1_tlv_desc_mbms_notification_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_NOTIFICATION_REJECT_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_notification_reject_request,
                &ogs_gtp1_tlv_desc_mbms_notification_reject_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_NOTIFICATION_REJECT_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_notification_reject_response,
                &ogs_gtp1_tlv_desc_mbms_notification_reject_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_CREATE_MBMS_CONTEXT_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->create_mbms_context_request,
                &ogs_gtp1_tlv_desc_create_mbms_context_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_CREATE_MBMS_CONTEXT_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->create_mbms_context_response,
                &ogs_gtp1_tlv_desc_create_mbms_context_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_UPDATE_MBMS_CONTEXT_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->update_mbms_context_request,
                &ogs_gtp1_tlv_desc_update_mbms_context_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_UPDATE_MBMS_CONTEXT_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->update_mbms_context_response,
                &ogs_gtp1_tlv_desc_update_mbms_context_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_DELETE_MBMS_CONTEXT_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->delete_mbms_context_request,
                &ogs_gtp1_tlv_desc_delete_mbms_context_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_DELETE_MBMS_CONTEXT_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->delete_mbms_context_response,
                &ogs_gtp1_tlv_desc_delete_mbms_context_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_REGISTRATION_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_registration_request,
                &ogs_gtp1_tlv_desc_mbms_registration_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_REGISTRATION_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_registration_response,
                &ogs_gtp1_tlv_desc_mbms_registration_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_DE_REGISTRATION_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_de_registration_request,
                &ogs_gtp1_tlv_desc_mbms_de_registration_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_DE_REGISTRATION_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_de_registration_response,
                &ogs_gtp1_tlv_desc_mbms_de_registration_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_SESSION_START_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_session_start_request,
                &ogs_gtp1_tlv_desc_mbms_session_start_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_SESSION_START_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_session_start_response,
                &ogs_gtp1_tlv_desc_mbms_session_start_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_SESSION_STOP_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_session_stop_request,
                &ogs_gtp1_tlv_desc_mbms_session_stop_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_SESSION_STOP_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_session_stop_response,
                &ogs_gtp1_tlv_desc_mbms_session_stop_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_SESSION_UPDATE_REQUEST_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_session_update_request,
                &ogs_gtp1_tlv_desc_mbms_session_update_request, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_SESSION_UPDATE_RESPONSE_TYPE:
        rv = ogs_tlv_parse_msg_desc(&gtp1_message->mbms_session_update_response,
                &ogs_gtp1_tlv_desc_mbms_session_update_response, pkbuf, OGS_TLV_MODE_T1_L2);
        break;
    default:
        ogs_warn("Not implemented(type:%d)", gtp1_message->h.type);
        break;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, size));

    return rv;
}

ogs_pkbuf_t *ogs_gtp1_build_msg(ogs_gtp1_message_t *gtp1_message)
{
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(gtp1_message);
    switch(gtp1_message->h.type) {
    case OGS_GTP1_ECHO_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_echo_request,
                &gtp1_message->echo_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_ECHO_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_echo_response,
                &gtp1_message->echo_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_CREATE_PDP_CONTEXT_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_create_pdp_context_request,
                &gtp1_message->create_pdp_context_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_CREATE_PDP_CONTEXT_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_create_pdp_context_response,
                &gtp1_message->create_pdp_context_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_UPDATE_PDP_CONTEXT_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_update_pdp_context_request,
                &gtp1_message->update_pdp_context_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_UPDATE_PDP_CONTEXT_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_update_pdp_context_response,
                &gtp1_message->update_pdp_context_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_DELETE_PDP_CONTEXT_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_delete_pdp_context_request,
                &gtp1_message->delete_pdp_context_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_DELETE_PDP_CONTEXT_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_delete_pdp_context_response,
                &gtp1_message->delete_pdp_context_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_INITIATE_PDP_CONTEXT_ACTIVATION_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_initiate_pdp_context_activation_request,
                &gtp1_message->initiate_pdp_context_activation_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_INITIATE_PDP_CONTEXT_ACTIVATION_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_initiate_pdp_context_activation_response,
                &gtp1_message->initiate_pdp_context_activation_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_PDU_NOTIFICATION_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_pdu_notification_request,
                &gtp1_message->pdu_notification_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_PDU_NOTIFICATION_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_pdu_notification_response,
                &gtp1_message->pdu_notification_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_PDU_NOTIFICATION_REJECT_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_pdu_notification_reject_request,
                &gtp1_message->pdu_notification_reject_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_PDU_NOTIFICATION_REJECT_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_pdu_notification_reject_response,
                &gtp1_message->pdu_notification_reject_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_SUPPORTED_EXTENSION_HEADERS_NOTIFICATION_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_supported_extension_headers_notification,
                &gtp1_message->supported_extension_headers_notification, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_SEND_ROUTEING_INFORMATION_FOR_GPRS_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_send_routeing_information_for_gprs_request,
                &gtp1_message->send_routeing_information_for_gprs_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_SEND_ROUTEING_INFORMATION_FOR_GPRS_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_send_routeing_information_for_gprs_response,
                &gtp1_message->send_routeing_information_for_gprs_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FAILURE_REPORT_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_failure_report_request,
                &gtp1_message->failure_report_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FAILURE_REPORT_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_failure_report_response,
                &gtp1_message->failure_report_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_NOTE_MS_GPRS_PRESENT_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_note_ms_gprs_present_request,
                &gtp1_message->note_ms_gprs_present_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_NOTE_MS_GPRS_PRESENT_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_note_ms_gprs_present_response,
                &gtp1_message->note_ms_gprs_present_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_IDENTIFICATION_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_identification_request,
                &gtp1_message->identification_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_IDENTIFICATION_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_identification_response,
                &gtp1_message->identification_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_SGSN_CONTEXT_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_sgsn_context_request,
                &gtp1_message->sgsn_context_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_SGSN_CONTEXT_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_sgsn_context_response,
                &gtp1_message->sgsn_context_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_SGSN_CONTEXT_ACKNOWLEDGE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_sgsn_context_acknowledge,
                &gtp1_message->sgsn_context_acknowledge, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FORWARD_RELOCATION_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_forward_relocation_request,
                &gtp1_message->forward_relocation_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FORWARD_RELOCATION_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_forward_relocation_response,
                &gtp1_message->forward_relocation_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FORWARD_RELOCATION_COMPLETE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_forward_relocation_complete,
                &gtp1_message->forward_relocation_complete, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_RELOCATION_CANCEL_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_relocation_cancel_request,
                &gtp1_message->relocation_cancel_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_RELOCATION_CANCEL_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_relocation_cancel_response,
                &gtp1_message->relocation_cancel_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FORWARD_SRNS_CONTEXT_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_forward_srns_context,
                &gtp1_message->forward_srns_context, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_forward_relocation_complete_acknowledge,
                &gtp1_message->forward_relocation_complete_acknowledge, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_FORWARD_SRNS_CONTEXT_ACKNOWLEDGE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_forward_srns_context_acknowledge,
                &gtp1_message->forward_srns_context_acknowledge, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_UE_REGISTRATION_QUERY_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_ue_registration_query_request,
                &gtp1_message->ue_registration_query_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_UE_REGISTRATION_QUERY_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_ue_registration_query_response,
                &gtp1_message->ue_registration_query_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_RAN_INFORMATION_RELAY_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_ran_information_relay,
                &gtp1_message->ran_information_relay, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_NOTIFICATION_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_notification_request,
                &gtp1_message->mbms_notification_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_NOTIFICATION_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_notification_response,
                &gtp1_message->mbms_notification_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_NOTIFICATION_REJECT_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_notification_reject_request,
                &gtp1_message->mbms_notification_reject_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_NOTIFICATION_REJECT_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_notification_reject_response,
                &gtp1_message->mbms_notification_reject_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_CREATE_MBMS_CONTEXT_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_create_mbms_context_request,
                &gtp1_message->create_mbms_context_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_CREATE_MBMS_CONTEXT_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_create_mbms_context_response,
                &gtp1_message->create_mbms_context_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_UPDATE_MBMS_CONTEXT_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_update_mbms_context_request,
                &gtp1_message->update_mbms_context_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_UPDATE_MBMS_CONTEXT_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_update_mbms_context_response,
                &gtp1_message->update_mbms_context_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_DELETE_MBMS_CONTEXT_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_delete_mbms_context_request,
                &gtp1_message->delete_mbms_context_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_DELETE_MBMS_CONTEXT_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_delete_mbms_context_response,
                &gtp1_message->delete_mbms_context_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_REGISTRATION_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_registration_request,
                &gtp1_message->mbms_registration_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_REGISTRATION_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_registration_response,
                &gtp1_message->mbms_registration_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_DE_REGISTRATION_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_de_registration_request,
                &gtp1_message->mbms_de_registration_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_DE_REGISTRATION_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_de_registration_response,
                &gtp1_message->mbms_de_registration_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_SESSION_START_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_session_start_request,
                &gtp1_message->mbms_session_start_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_SESSION_START_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_session_start_response,
                &gtp1_message->mbms_session_start_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_SESSION_STOP_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_session_stop_request,
                &gtp1_message->mbms_session_stop_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_SESSION_STOP_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_session_stop_response,
                &gtp1_message->mbms_session_stop_response, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_SESSION_UPDATE_REQUEST_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_session_update_request,
                &gtp1_message->mbms_session_update_request, OGS_TLV_MODE_T1_L2);
        break;
    case OGS_GTP1_MBMS_SESSION_UPDATE_RESPONSE_TYPE:
        pkbuf = ogs_tlv_build_msg(&ogs_gtp1_tlv_desc_mbms_session_update_response,
                &gtp1_message->mbms_session_update_response, OGS_TLV_MODE_T1_L2);
        break;
    default:
        ogs_warn("Not implemented(type:%d)", gtp1_message->h.type);
        break;
    }

    return pkbuf;
}

