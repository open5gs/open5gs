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
 * Created on: 2018-01-23 16:32:53.073144 by acetcom
 * from 29274-d80.docx
 ******************************************************************************/

#define TRACE_MODULE _gtp_message
#include "core_debug.h"
#include "gtp_message.h"

tlv_desc_t tlv_desc_imsi_0 =
{
    TLV_VAR_STR,
    "IMSI",
    TLV_IMSI_TYPE,
    0,
    0,
    sizeof(tlv_imsi_t),
    { NULL }
};

tlv_desc_t tlv_desc_cause_0 =
{
    TLV_VAR_STR,
    "Cause",
    TLV_CAUSE_TYPE,
    0,
    0,
    sizeof(tlv_cause_t),
    { NULL }
};

tlv_desc_t tlv_desc_recovery_0 =
{
    TLV_UINT8,
    "Recovery",
    TLV_RECOVERY_TYPE,
    1,
    0,
    sizeof(tlv_recovery_t),
    { NULL }
};

tlv_desc_t tlv_desc_stn_sr_0 =
{
    TLV_VAR_STR,
    "STN-SR",
    TLV_STN_SR_TYPE,
    0,
    0,
    sizeof(tlv_stn_sr_t),
    { NULL }
};

tlv_desc_t tlv_desc_apn_0 =
{
    TLV_VAR_STR,
    "APN",
    TLV_APN_TYPE,
    0,
    0,
    sizeof(tlv_apn_t),
    { NULL }
};

tlv_desc_t tlv_desc_ambr_0 =
{
    TLV_VAR_STR,
    "AMBR",
    TLV_AMBR_TYPE,
    0,
    0,
    sizeof(tlv_ambr_t),
    { NULL }
};

tlv_desc_t tlv_desc_ebi_0 =
{
    TLV_UINT8,
    "EBI",
    TLV_EBI_TYPE,
    1,
    0,
    sizeof(tlv_ebi_t),
    { NULL }
};

tlv_desc_t tlv_desc_ebi_1 =
{
    TLV_UINT8,
    "EBI",
    TLV_EBI_TYPE,
    1,
    1,
    sizeof(tlv_ebi_t),
    { NULL }
};

tlv_desc_t tlv_desc_ip_address_0 =
{
    TLV_VAR_STR,
    "IP Address",
    TLV_IP_ADDRESS_TYPE,
    0,
    0,
    sizeof(tlv_ip_address_t),
    { NULL }
};

tlv_desc_t tlv_desc_ip_address_1 =
{
    TLV_VAR_STR,
    "IP Address",
    TLV_IP_ADDRESS_TYPE,
    0,
    1,
    sizeof(tlv_ip_address_t),
    { NULL }
};

tlv_desc_t tlv_desc_ip_address_2 =
{
    TLV_VAR_STR,
    "IP Address",
    TLV_IP_ADDRESS_TYPE,
    0,
    2,
    sizeof(tlv_ip_address_t),
    { NULL }
};

tlv_desc_t tlv_desc_ip_address_3 =
{
    TLV_VAR_STR,
    "IP Address",
    TLV_IP_ADDRESS_TYPE,
    0,
    3,
    sizeof(tlv_ip_address_t),
    { NULL }
};

tlv_desc_t tlv_desc_mei_0 =
{
    TLV_VAR_STR,
    "MEI",
    TLV_MEI_TYPE,
    0,
    0,
    sizeof(tlv_mei_t),
    { NULL }
};

tlv_desc_t tlv_desc_msisdn_0 =
{
    TLV_VAR_STR,
    "MSISDN",
    TLV_MSISDN_TYPE,
    0,
    0,
    sizeof(tlv_msisdn_t),
    { NULL }
};

tlv_desc_t tlv_desc_indication_0 =
{
    TLV_VAR_STR,
    "Indication",
    TLV_INDICATION_TYPE,
    0,
    0,
    sizeof(tlv_indication_t),
    { NULL }
};

tlv_desc_t tlv_desc_pco_0 =
{
    TLV_VAR_STR,
    "PCO",
    TLV_PCO_TYPE,
    0,
    0,
    sizeof(tlv_pco_t),
    { NULL }
};

tlv_desc_t tlv_desc_paa_0 =
{
    TLV_VAR_STR,
    "PAA",
    TLV_PAA_TYPE,
    0,
    0,
    sizeof(tlv_paa_t),
    { NULL }
};

tlv_desc_t tlv_desc_bearer_qos_0 =
{
    TLV_VAR_STR,
    "Bearer QoS",
    TLV_BEARER_QOS_TYPE,
    0,
    0,
    sizeof(tlv_bearer_qos_t),
    { NULL }
};

tlv_desc_t tlv_desc_flow_qos_0 =
{
    TLV_VAR_STR,
    "Flow QoS",
    TLV_FLOW_QOS_TYPE,
    0,
    0,
    sizeof(tlv_flow_qos_t),
    { NULL }
};

tlv_desc_t tlv_desc_rat_type_0 =
{
    TLV_UINT8,
    "RAT Type",
    TLV_RAT_TYPE_TYPE,
    1,
    0,
    sizeof(tlv_rat_type_t),
    { NULL }
};

tlv_desc_t tlv_desc_serving_network_0 =
{
    TLV_VAR_STR,
    "Serving Network",
    TLV_SERVING_NETWORK_TYPE,
    0,
    0,
    sizeof(tlv_serving_network_t),
    { NULL }
};

tlv_desc_t tlv_desc_bearer_tft_0 =
{
    TLV_VAR_STR,
    "Bearer TFT",
    TLV_BEARER_TFT_TYPE,
    0,
    0,
    sizeof(tlv_bearer_tft_t),
    { NULL }
};

tlv_desc_t tlv_desc_tad_0 =
{
    TLV_VAR_STR,
    "TAD",
    TLV_TAD_TYPE,
    0,
    0,
    sizeof(tlv_tad_t),
    { NULL }
};

tlv_desc_t tlv_desc_uli_0 =
{
    TLV_VAR_STR,
    "ULI",
    TLV_ULI_TYPE,
    0,
    0,
    sizeof(tlv_uli_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_teid_0 =
{
    TLV_VAR_STR,
    "F-TEID",
    TLV_F_TEID_TYPE,
    0,
    0,
    sizeof(tlv_f_teid_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_teid_1 =
{
    TLV_VAR_STR,
    "F-TEID",
    TLV_F_TEID_TYPE,
    0,
    1,
    sizeof(tlv_f_teid_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_teid_2 =
{
    TLV_VAR_STR,
    "F-TEID",
    TLV_F_TEID_TYPE,
    0,
    2,
    sizeof(tlv_f_teid_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_teid_3 =
{
    TLV_VAR_STR,
    "F-TEID",
    TLV_F_TEID_TYPE,
    0,
    3,
    sizeof(tlv_f_teid_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_teid_4 =
{
    TLV_VAR_STR,
    "F-TEID",
    TLV_F_TEID_TYPE,
    0,
    4,
    sizeof(tlv_f_teid_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_teid_5 =
{
    TLV_VAR_STR,
    "F-TEID",
    TLV_F_TEID_TYPE,
    0,
    5,
    sizeof(tlv_f_teid_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_teid_6 =
{
    TLV_VAR_STR,
    "F-TEID",
    TLV_F_TEID_TYPE,
    0,
    6,
    sizeof(tlv_f_teid_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_teid_7 =
{
    TLV_VAR_STR,
    "F-TEID",
    TLV_F_TEID_TYPE,
    0,
    7,
    sizeof(tlv_f_teid_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_teid_8 =
{
    TLV_VAR_STR,
    "F-TEID",
    TLV_F_TEID_TYPE,
    0,
    8,
    sizeof(tlv_f_teid_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_teid_9 =
{
    TLV_VAR_STR,
    "F-TEID",
    TLV_F_TEID_TYPE,
    0,
    9,
    sizeof(tlv_f_teid_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_teid_10 =
{
    TLV_VAR_STR,
    "F-TEID",
    TLV_F_TEID_TYPE,
    0,
    10,
    sizeof(tlv_f_teid_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_teid_11 =
{
    TLV_VAR_STR,
    "F-TEID",
    TLV_F_TEID_TYPE,
    0,
    11,
    sizeof(tlv_f_teid_t),
    { NULL }
};

tlv_desc_t tlv_desc_tmsi_0 =
{
    TLV_VAR_STR,
    "TMSI",
    TLV_TMSI_TYPE,
    0,
    0,
    sizeof(tlv_tmsi_t),
    { NULL }
};

tlv_desc_t tlv_desc_global_cn_id_0 =
{
    TLV_VAR_STR,
    "Global CN-Id",
    TLV_GLOBAL_CN_ID_TYPE,
    0,
    0,
    sizeof(tlv_global_cn_id_t),
    { NULL }
};

tlv_desc_t tlv_desc_s103pdf_0 =
{
    TLV_VAR_STR,
    "S103PDF",
    TLV_S103PDF_TYPE,
    0,
    0,
    sizeof(tlv_s103pdf_t),
    { NULL }
};

tlv_desc_t tlv_desc_s1udf_0 =
{
    TLV_VAR_STR,
    "S1UDF",
    TLV_S1UDF_TYPE,
    0,
    0,
    sizeof(tlv_s1udf_t),
    { NULL }
};

tlv_desc_t tlv_desc_delay_value_0 =
{
    TLV_VAR_STR,
    "Delay Value",
    TLV_DELAY_VALUE_TYPE,
    0,
    0,
    sizeof(tlv_delay_value_t),
    { NULL }
};

tlv_desc_t tlv_desc_charging_id_0 =
{
    TLV_VAR_STR,
    "Charging ID",
    TLV_CHARGING_ID_TYPE,
    0,
    0,
    sizeof(tlv_charging_id_t),
    { NULL }
};

tlv_desc_t tlv_desc_charging_characteristics_0 =
{
    TLV_VAR_STR,
    "Charging Characteristics",
    TLV_CHARGING_CHARACTERISTICS_TYPE,
    0,
    0,
    sizeof(tlv_charging_characteristics_t),
    { NULL }
};

tlv_desc_t tlv_desc_trace_information_0 =
{
    TLV_VAR_STR,
    "Trace Information",
    TLV_TRACE_INFORMATION_TYPE,
    0,
    0,
    sizeof(tlv_trace_information_t),
    { NULL }
};

tlv_desc_t tlv_desc_bearer_flags_0 =
{
    TLV_VAR_STR,
    "Bearer Flags",
    TLV_BEARER_FLAGS_TYPE,
    0,
    0,
    sizeof(tlv_bearer_flags_t),
    { NULL }
};

tlv_desc_t tlv_desc_pdn_type_0 =
{
    TLV_UINT8,
    "PDN Type",
    TLV_PDN_TYPE_TYPE,
    1,
    0,
    sizeof(tlv_pdn_type_t),
    { NULL }
};

tlv_desc_t tlv_desc_pti_0 =
{
    TLV_VAR_STR,
    "PTI",
    TLV_PTI_TYPE,
    0,
    0,
    sizeof(tlv_pti_t),
    { NULL }
};

tlv_desc_t tlv_desc_mm_context_0 =
{
    TLV_VAR_STR,
    "MM Context",
    TLV_MM_CONTEXT_TYPE,
    0,
    0,
    sizeof(tlv_mm_context_t),
    { NULL }
};

tlv_desc_t tlv_desc_pdu_numbers_0 =
{
    TLV_VAR_STR,
    "PDU Numbers",
    TLV_PDU_NUMBERS_TYPE,
    0,
    0,
    sizeof(tlv_pdu_numbers_t),
    { NULL }
};

tlv_desc_t tlv_desc_p_tmsi_0 =
{
    TLV_VAR_STR,
    "P-TMSI",
    TLV_P_TMSI_TYPE,
    0,
    0,
    sizeof(tlv_p_tmsi_t),
    { NULL }
};

tlv_desc_t tlv_desc_p_tmsi_signature_0 =
{
    TLV_VAR_STR,
    "P-TMSI Signature",
    TLV_P_TMSI_SIGNATURE_TYPE,
    0,
    0,
    sizeof(tlv_p_tmsi_signature_t),
    { NULL }
};

tlv_desc_t tlv_desc_hop_counter_0 =
{
    TLV_VAR_STR,
    "Hop Counter",
    TLV_HOP_COUNTER_TYPE,
    0,
    0,
    sizeof(tlv_hop_counter_t),
    { NULL }
};

tlv_desc_t tlv_desc_ue_time_zone_0 =
{
    TLV_VAR_STR,
    "UE Time Zone",
    TLV_UE_TIME_ZONE_TYPE,
    0,
    0,
    sizeof(tlv_ue_time_zone_t),
    { NULL }
};

tlv_desc_t tlv_desc_trace_reference_0 =
{
    TLV_VAR_STR,
    "Trace Reference",
    TLV_TRACE_REFERENCE_TYPE,
    0,
    0,
    sizeof(tlv_trace_reference_t),
    { NULL }
};

tlv_desc_t tlv_desc_complete_request_message_0 =
{
    TLV_VAR_STR,
    "Complete Request Message",
    TLV_COMPLETE_REQUEST_MESSAGE_TYPE,
    0,
    0,
    sizeof(tlv_complete_request_message_t),
    { NULL }
};

tlv_desc_t tlv_desc_guti_0 =
{
    TLV_VAR_STR,
    "GUTI",
    TLV_GUTI_TYPE,
    0,
    0,
    sizeof(tlv_guti_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_container_0 =
{
    TLV_VAR_STR,
    "F-Container",
    TLV_F_CONTAINER_TYPE,
    0,
    0,
    sizeof(tlv_f_container_t),
    { NULL }
};

tlv_desc_t tlv_desc_f_cause_0 =
{
    TLV_VAR_STR,
    "F-Cause",
    TLV_F_CAUSE_TYPE,
    0,
    0,
    sizeof(tlv_f_cause_t),
    { NULL }
};

tlv_desc_t tlv_desc_plmn_id_0 =
{
    TLV_VAR_STR,
    "PLMN ID",
    TLV_PLMN_ID_TYPE,
    0,
    0,
    sizeof(tlv_plmn_id_t),
    { NULL }
};

tlv_desc_t tlv_desc_target_identification_0 =
{
    TLV_VAR_STR,
    "Target Identification",
    TLV_TARGET_IDENTIFICATION_TYPE,
    0,
    0,
    sizeof(tlv_target_identification_t),
    { NULL }
};

tlv_desc_t tlv_desc_packet_flow_id_0 =
{
    TLV_VAR_STR,
    "Packet Flow ID",
    TLV_PACKET_FLOW_ID_TYPE,
    0,
    0,
    sizeof(tlv_packet_flow_id_t),
    { NULL }
};

tlv_desc_t tlv_desc_rab_context_0 =
{
    TLV_VAR_STR,
    "RAB Context",
    TLV_RAB_CONTEXT_TYPE,
    0,
    0,
    sizeof(tlv_rab_context_t),
    { NULL }
};

tlv_desc_t tlv_desc_source_rnc_pdcp_context_info_0 =
{
    TLV_VAR_STR,
    "Source RNC PDCP Context Info",
    TLV_SOURCE_RNC_PDCP_CONTEXT_INFO_TYPE,
    0,
    0,
    sizeof(tlv_source_rnc_pdcp_context_info_t),
    { NULL }
};

tlv_desc_t tlv_desc_port_number_0 =
{
    TLV_UINT16,
    "Port Number",
    TLV_PORT_NUMBER_TYPE,
    2,
    0,
    sizeof(tlv_port_number_t),
    { NULL }
};

tlv_desc_t tlv_desc_port_number_1 =
{
    TLV_UINT16,
    "Port Number",
    TLV_PORT_NUMBER_TYPE,
    2,
    1,
    sizeof(tlv_port_number_t),
    { NULL }
};

tlv_desc_t tlv_desc_port_number_2 =
{
    TLV_UINT16,
    "Port Number",
    TLV_PORT_NUMBER_TYPE,
    2,
    2,
    sizeof(tlv_port_number_t),
    { NULL }
};

tlv_desc_t tlv_desc_apn_restriction_0 =
{
    TLV_UINT8,
    "APN Restriction",
    TLV_APN_RESTRICTION_TYPE,
    1,
    0,
    sizeof(tlv_apn_restriction_t),
    { NULL }
};

tlv_desc_t tlv_desc_selection_mode_0 =
{
    TLV_UINT8,
    "Selection Mode",
    TLV_SELECTION_MODE_TYPE,
    1,
    0,
    sizeof(tlv_selection_mode_t),
    { NULL }
};

tlv_desc_t tlv_desc_source_identification_0 =
{
    TLV_VAR_STR,
    "Source Identification",
    TLV_SOURCE_IDENTIFICATION_TYPE,
    0,
    0,
    sizeof(tlv_source_identification_t),
    { NULL }
};

tlv_desc_t tlv_desc_change_reporting_action_0 =
{
    TLV_VAR_STR,
    "Change Reporting Action",
    TLV_CHANGE_REPORTING_ACTION_TYPE,
    0,
    0,
    sizeof(tlv_change_reporting_action_t),
    { NULL }
};

tlv_desc_t tlv_desc_fq_csid_0 =
{
    TLV_VAR_STR,
    "FQ-CSID",
    TLV_FQ_CSID_TYPE,
    0,
    0,
    sizeof(tlv_fq_csid_t),
    { NULL }
};

tlv_desc_t tlv_desc_fq_csid_1 =
{
    TLV_VAR_STR,
    "FQ-CSID",
    TLV_FQ_CSID_TYPE,
    0,
    1,
    sizeof(tlv_fq_csid_t),
    { NULL }
};

tlv_desc_t tlv_desc_fq_csid_2 =
{
    TLV_VAR_STR,
    "FQ-CSID",
    TLV_FQ_CSID_TYPE,
    0,
    2,
    sizeof(tlv_fq_csid_t),
    { NULL }
};

tlv_desc_t tlv_desc_fq_csid_3 =
{
    TLV_VAR_STR,
    "FQ-CSID",
    TLV_FQ_CSID_TYPE,
    0,
    3,
    sizeof(tlv_fq_csid_t),
    { NULL }
};

tlv_desc_t tlv_desc_channel_needed_0 =
{
    TLV_VAR_STR,
    "Channel needed",
    TLV_CHANNEL_NEEDED_TYPE,
    0,
    0,
    sizeof(tlv_channel_needed_t),
    { NULL }
};

tlv_desc_t tlv_desc_emlpp_priority_0 =
{
    TLV_VAR_STR,
    "eMLPP Priority",
    TLV_EMLPP_PRIORITY_TYPE,
    0,
    0,
    sizeof(tlv_emlpp_priority_t),
    { NULL }
};

tlv_desc_t tlv_desc_node_type_0 =
{
    TLV_UINT8,
    "Node Type",
    TLV_NODE_TYPE_TYPE,
    1,
    0,
    sizeof(tlv_node_type_t),
    { NULL }
};

tlv_desc_t tlv_desc_fqdn_0 =
{
    TLV_VAR_STR,
    "FQDN",
    TLV_FQDN_TYPE,
    0,
    0,
    sizeof(tlv_fqdn_t),
    { NULL }
};

tlv_desc_t tlv_desc_ti_0 =
{
    TLV_VAR_STR,
    "TI",
    TLV_TI_TYPE,
    0,
    0,
    sizeof(tlv_ti_t),
    { NULL }
};

tlv_desc_t tlv_desc_mbms_session_duration_0 =
{
    TLV_VAR_STR,
    "MBMS Session Duration",
    TLV_MBMS_SESSION_DURATION_TYPE,
    0,
    0,
    sizeof(tlv_mbms_session_duration_t),
    { NULL }
};

tlv_desc_t tlv_desc_mbms_service_area_0 =
{
    TLV_VAR_STR,
    "MBMS Service Area",
    TLV_MBMS_SERVICE_AREA_TYPE,
    0,
    0,
    sizeof(tlv_mbms_service_area_t),
    { NULL }
};

tlv_desc_t tlv_desc_mbms_session_identifier_0 =
{
    TLV_VAR_STR,
    "MBMS Session Identifier",
    TLV_MBMS_SESSION_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(tlv_mbms_session_identifier_t),
    { NULL }
};

tlv_desc_t tlv_desc_mbms_flow_identifier_0 =
{
    TLV_VAR_STR,
    "MBMS Flow Identifier",
    TLV_MBMS_FLOW_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(tlv_mbms_flow_identifier_t),
    { NULL }
};

tlv_desc_t tlv_desc_mbms_ip_multicast_distribution_0 =
{
    TLV_VAR_STR,
    "MBMS IP Multicast Distribution",
    TLV_MBMS_IP_MULTICAST_DISTRIBUTION_TYPE,
    0,
    0,
    sizeof(tlv_mbms_ip_multicast_distribution_t),
    { NULL }
};

tlv_desc_t tlv_desc_mbms_distribution_acknowledge_0 =
{
    TLV_VAR_STR,
    "MBMS Distribution Acknowledge",
    TLV_MBMS_DISTRIBUTION_ACKNOWLEDGE_TYPE,
    0,
    0,
    sizeof(tlv_mbms_distribution_acknowledge_t),
    { NULL }
};

tlv_desc_t tlv_desc_rfsp_index_0 =
{
    TLV_VAR_STR,
    "RFSP Index",
    TLV_RFSP_INDEX_TYPE,
    0,
    0,
    sizeof(tlv_rfsp_index_t),
    { NULL }
};

tlv_desc_t tlv_desc_uci_0 =
{
    TLV_VAR_STR,
    "UCI",
    TLV_UCI_TYPE,
    0,
    0,
    sizeof(tlv_uci_t),
    { NULL }
};

tlv_desc_t tlv_desc_csg_information_reporting_action_0 =
{
    TLV_VAR_STR,
    "CSG Information Reporting Action",
    TLV_CSG_INFORMATION_REPORTING_ACTION_TYPE,
    0,
    0,
    sizeof(tlv_csg_information_reporting_action_t),
    { NULL }
};

tlv_desc_t tlv_desc_csg_id_0 =
{
    TLV_VAR_STR,
    "CSG ID",
    TLV_CSG_ID_TYPE,
    0,
    0,
    sizeof(tlv_csg_id_t),
    { NULL }
};

tlv_desc_t tlv_desc_cmi_0 =
{
    TLV_VAR_STR,
    "CMI",
    TLV_CMI_TYPE,
    0,
    0,
    sizeof(tlv_cmi_t),
    { NULL }
};

tlv_desc_t tlv_desc_service_indicator_0 =
{
    TLV_VAR_STR,
    "Service indicator",
    TLV_SERVICE_INDICATOR_TYPE,
    0,
    0,
    sizeof(tlv_service_indicator_t),
    { NULL }
};

tlv_desc_t tlv_desc_detach_type_0 =
{
    TLV_VAR_STR,
    "Detach Type",
    TLV_DETACH_TYPE_TYPE,
    0,
    0,
    sizeof(tlv_detach_type_t),
    { NULL }
};

tlv_desc_t tlv_desc_ldn_0 =
{
    TLV_VAR_STR,
    "LDN",
    TLV_LDN_TYPE,
    0,
    0,
    sizeof(tlv_ldn_t),
    { NULL }
};

tlv_desc_t tlv_desc_ldn_1 =
{
    TLV_VAR_STR,
    "LDN",
    TLV_LDN_TYPE,
    0,
    1,
    sizeof(tlv_ldn_t),
    { NULL }
};

tlv_desc_t tlv_desc_ldn_2 =
{
    TLV_VAR_STR,
    "LDN",
    TLV_LDN_TYPE,
    0,
    2,
    sizeof(tlv_ldn_t),
    { NULL }
};

tlv_desc_t tlv_desc_ldn_3 =
{
    TLV_VAR_STR,
    "LDN",
    TLV_LDN_TYPE,
    0,
    3,
    sizeof(tlv_ldn_t),
    { NULL }
};

tlv_desc_t tlv_desc_node_features_0 =
{
    TLV_VAR_STR,
    "Node Features",
    TLV_NODE_FEATURES_TYPE,
    0,
    0,
    sizeof(tlv_node_features_t),
    { NULL }
};

tlv_desc_t tlv_desc_mbms_time_to_data_transfer_0 =
{
    TLV_VAR_STR,
    "MBMS Time to Data Transfer",
    TLV_MBMS_TIME_TO_DATA_TRANSFER_TYPE,
    0,
    0,
    sizeof(tlv_mbms_time_to_data_transfer_t),
    { NULL }
};

tlv_desc_t tlv_desc_throttling_0 =
{
    TLV_VAR_STR,
    "Throttling",
    TLV_THROTTLING_TYPE,
    0,
    0,
    sizeof(tlv_throttling_t),
    { NULL }
};

tlv_desc_t tlv_desc_arp_0 =
{
    TLV_VAR_STR,
    "ARP",
    TLV_ARP_TYPE,
    0,
    0,
    sizeof(tlv_arp_t),
    { NULL }
};

tlv_desc_t tlv_desc_epc_timer_0 =
{
    TLV_VAR_STR,
    "EPC Timer",
    TLV_EPC_TIMER_TYPE,
    0,
    0,
    sizeof(tlv_epc_timer_t),
    { NULL }
};

tlv_desc_t tlv_desc_signalling_priority_indication_0 =
{
    TLV_VAR_STR,
    "Signalling Priority Indication",
    TLV_SIGNALLING_PRIORITY_INDICATION_TYPE,
    0,
    0,
    sizeof(tlv_signalling_priority_indication_t),
    { NULL }
};

tlv_desc_t tlv_desc_tmgi_0 =
{
    TLV_VAR_STR,
    "TMGI",
    TLV_TMGI_TYPE,
    0,
    0,
    sizeof(tlv_tmgi_t),
    { NULL }
};

tlv_desc_t tlv_desc_additional_mm_context_for_srvcc_0 =
{
    TLV_VAR_STR,
    "Additional MM context for SRVCC",
    TLV_ADDITIONAL_MM_CONTEXT_FOR_SRVCC_TYPE,
    0,
    0,
    sizeof(tlv_additional_mm_context_for_srvcc_t),
    { NULL }
};

tlv_desc_t tlv_desc_additional_flags_for_srvcc_0 =
{
    TLV_VAR_STR,
    "Additional flags for SRVCC",
    TLV_ADDITIONAL_FLAGS_FOR_SRVCC_TYPE,
    0,
    0,
    sizeof(tlv_additional_flags_for_srvcc_t),
    { NULL }
};

tlv_desc_t tlv_desc_mdt_configuration_0 =
{
    TLV_VAR_STR,
    "MDT Configuration",
    TLV_MDT_CONFIGURATION_TYPE,
    0,
    0,
    sizeof(tlv_mdt_configuration_t),
    { NULL }
};

tlv_desc_t tlv_desc_apco_0 =
{
    TLV_VAR_STR,
    "APCO",
    TLV_APCO_TYPE,
    0,
    0,
    sizeof(tlv_apco_t),
    { NULL }
};

tlv_desc_t tlv_desc_absolute_time_of_mbms_data_transfer_0 =
{
    TLV_VAR_STR,
    "Absolute Time of MBMS Data Transfer",
    TLV_ABSOLUTE_TIME_OF_MBMS_DATA_TRANSFER_TYPE,
    0,
    0,
    sizeof(tlv_absolute_time_of_mbms_data_transfer_t),
    { NULL }
};

tlv_desc_t tlv_desc_enb_information_reporting_0 =
{
    TLV_VAR_STR,
    "eNB Information Reporting",
    TLV_ENB_INFORMATION_REPORTING_TYPE,
    0,
    0,
    sizeof(tlv_enb_information_reporting_t),
    { NULL }
};

tlv_desc_t tlv_desc_ip4cp_0 =
{
    TLV_VAR_STR,
    "IP4CP",
    TLV_IP4CP_TYPE,
    0,
    0,
    sizeof(tlv_ip4cp_t),
    { NULL }
};

tlv_desc_t tlv_desc_change_to_report_flags_0 =
{
    TLV_VAR_STR,
    "Change to Report Flags",
    TLV_CHANGE_TO_REPORT_FLAGS_TYPE,
    0,
    0,
    sizeof(tlv_change_to_report_flags_t),
    { NULL }
};

tlv_desc_t tlv_desc_action_indication_0 =
{
    TLV_VAR_STR,
    "Action Indication",
    TLV_ACTION_INDICATION_TYPE,
    0,
    0,
    sizeof(tlv_action_indication_t),
    { NULL }
};

tlv_desc_t tlv_desc_twan_identifier_0 =
{
    TLV_VAR_STR,
    "TWAN Identifier",
    TLV_TWAN_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(tlv_twan_identifier_t),
    { NULL }
};

tlv_desc_t tlv_desc_twan_identifier_1 =
{
    TLV_VAR_STR,
    "TWAN Identifier",
    TLV_TWAN_IDENTIFIER_TYPE,
    0,
    1,
    sizeof(tlv_twan_identifier_t),
    { NULL }
};

tlv_desc_t tlv_desc_uli_timestamp_0 =
{
    TLV_VAR_STR,
    "ULI Timestamp",
    TLV_ULI_TIMESTAMP_TYPE,
    0,
    0,
    sizeof(tlv_uli_timestamp_t),
    { NULL }
};

tlv_desc_t tlv_desc_mbms_flags_0 =
{
    TLV_VAR_STR,
    "MBMS Flags",
    TLV_MBMS_FLAGS_TYPE,
    0,
    0,
    sizeof(tlv_mbms_flags_t),
    { NULL }
};

tlv_desc_t tlv_desc_ran_nas_cause_0 =
{
    TLV_VAR_STR,
    "RAN/NAS Cause",
    TLV_RAN_NAS_CAUSE_TYPE,
    0,
    0,
    sizeof(tlv_ran_nas_cause_t),
    { NULL }
};

tlv_desc_t tlv_desc_cn_operator_selection_entity_0 =
{
    TLV_VAR_STR,
    "CN Operator Selection Entity",
    TLV_CN_OPERATOR_SELECTION_ENTITY_TYPE,
    0,
    0,
    sizeof(tlv_cn_operator_selection_entity_t),
    { NULL }
};

tlv_desc_t tlv_desc_twmi_0 =
{
    TLV_VAR_STR,
    "TWMI",
    TLV_TWMI_TYPE,
    0,
    0,
    sizeof(tlv_twmi_t),
    { NULL }
};

tlv_desc_t tlv_desc_node_number_0 =
{
    TLV_VAR_STR,
    "Node Number",
    TLV_NODE_NUMBER_TYPE,
    0,
    0,
    sizeof(tlv_node_number_t),
    { NULL }
};

tlv_desc_t tlv_desc_node_identifier_0 =
{
    TLV_VAR_STR,
    "Node Identifier",
    TLV_NODE_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(tlv_node_identifier_t),
    { NULL }
};

tlv_desc_t tlv_desc_presence_reporting_area_action_0 =
{
    TLV_VAR_STR,
    "Presence Reporting Area Action",
    TLV_PRESENCE_REPORTING_AREA_ACTION_TYPE,
    0,
    0,
    sizeof(tlv_presence_reporting_area_action_t),
    { NULL }
};

tlv_desc_t tlv_desc_presence_reporting_area_information_0 =
{
    TLV_VAR_STR,
    "Presence Reporting Area Information",
    TLV_PRESENCE_REPORTING_AREA_INFORMATION_TYPE,
    0,
    0,
    sizeof(tlv_presence_reporting_area_information_t),
    { NULL }
};

tlv_desc_t tlv_desc_twan_identifier_timestamp_0 =
{
    TLV_VAR_STR,
    "TWAN Identifier Timestamp",
    TLV_TWAN_IDENTIFIER_TIMESTAMP_TYPE,
    0,
    0,
    sizeof(tlv_twan_identifier_timestamp_t),
    { NULL }
};

tlv_desc_t tlv_desc_twan_identifier_timestamp_1 =
{
    TLV_VAR_STR,
    "TWAN Identifier Timestamp",
    TLV_TWAN_IDENTIFIER_TIMESTAMP_TYPE,
    0,
    1,
    sizeof(tlv_twan_identifier_timestamp_t),
    { NULL }
};

tlv_desc_t tlv_desc_metric_0 =
{
    TLV_VAR_STR,
    "Metric",
    TLV_METRIC_TYPE,
    0,
    0,
    sizeof(tlv_metric_t),
    { NULL }
};

tlv_desc_t tlv_desc_sequence_number_0 =
{
    TLV_VAR_STR,
    "Sequence Number",
    TLV_SEQUENCE_NUMBER_TYPE,
    0,
    0,
    sizeof(tlv_sequence_number_t),
    { NULL }
};

tlv_desc_t tlv_desc_apn_and_relative_capacity_0 =
{
    TLV_VAR_STR,
    "APN and Relative Capacity",
    TLV_APN_AND_RELATIVE_CAPACITY_TYPE,
    0,
    0,
    sizeof(tlv_apn_and_relative_capacity_t),
    { NULL }
};

tlv_desc_t tlv_desc_wlan_offloadability_indication_0 =
{
    TLV_VAR_STR,
    "WLAN Offloadability Indication",
    TLV_WLAN_OFFLOADABILITY_INDICATION_TYPE,
    0,
    0,
    sizeof(tlv_wlan_offloadability_indication_t),
    { NULL }
};

tlv_desc_t tlv_desc_paging_and_service_information_0 =
{
    TLV_VAR_STR,
    "Paging and Service Information",
    TLV_PAGING_AND_SERVICE_INFORMATION_TYPE,
    0,
    0,
    sizeof(tlv_paging_and_service_information_t),
    { NULL }
};

tlv_desc_t tlv_desc_integer_number_0 =
{
    TLV_VAR_STR,
    "Integer Number",
    TLV_INTEGER_NUMBER_TYPE,
    0,
    0,
    sizeof(tlv_integer_number_t),
    { NULL }
};

tlv_desc_t tlv_desc_millisecond_time_stamp_0 =
{
    TLV_VAR_STR,
    "Millisecond Time Stamp",
    TLV_MILLISECOND_TIME_STAMP_TYPE,
    0,
    0,
    sizeof(tlv_millisecond_time_stamp_t),
    { NULL }
};

tlv_desc_t tlv_desc_monitoring_event_information_0 =
{
    TLV_VAR_STR,
    "Monitoring Event Information",
    TLV_MONITORING_EVENT_INFORMATION_TYPE,
    0,
    0,
    sizeof(tlv_monitoring_event_information_t),
    { NULL }
};

tlv_desc_t tlv_desc_ecgi_list_0 =
{
    TLV_VAR_STR,
    "ECGI List",
    TLV_ECGI_LIST_TYPE,
    0,
    0,
    sizeof(tlv_ecgi_list_t),
    { NULL }
};

tlv_desc_t tlv_desc_remote_user_id_0 =
{
    TLV_VAR_STR,
    "Remote User ID",
    TLV_REMOTE_USER_ID_TYPE,
    0,
    0,
    sizeof(tlv_remote_user_id_t),
    { NULL }
};

tlv_desc_t tlv_desc_remote_ue_ip_information_0 =
{
    TLV_VAR_STR,
    "Remote UE IP Information",
    TLV_REMOTE_UE_IP_INFORMATION_TYPE,
    0,
    0,
    sizeof(tlv_remote_ue_ip_information_t),
    { NULL }
};

tlv_desc_t tlv_desc_ciot_optimizations_support_indication_0 =
{
    TLV_VAR_STR,
    "CIoT Optimizations Support Indication",
    TLV_CIOT_OPTIMIZATIONS_SUPPORT_INDICATION_TYPE,
    0,
    0,
    sizeof(tlv_ciot_optimizations_support_indication_t),
    { NULL }
};

tlv_desc_t tlv_desc_header_compression_configuration_0 =
{
    TLV_VAR_STR,
    "Header Compression Configuration",
    TLV_HEADER_COMPRESSION_CONFIGURATION_TYPE,
    0,
    0,
    sizeof(tlv_header_compression_configuration_t),
    { NULL }
};

tlv_desc_t tlv_desc_epco_0 =
{
    TLV_VAR_STR,
    "ePCO",
    TLV_EPCO_TYPE,
    0,
    0,
    sizeof(tlv_epco_t),
    { NULL }
};

tlv_desc_t tlv_desc_serving_plmn_rate_control_0 =
{
    TLV_VAR_STR,
    "Serving PLMN Rate Control",
    TLV_SERVING_PLMN_RATE_CONTROL_TYPE,
    0,
    0,
    sizeof(tlv_serving_plmn_rate_control_t),
    { NULL }
};

tlv_desc_t tlv_desc_counter_0 =
{
    TLV_VAR_STR,
    "Counter",
    TLV_COUNTER_TYPE,
    0,
    0,
    sizeof(tlv_counter_t),
    { NULL }
};

tlv_desc_t tlv_desc_bearer_context_0 =
{
    TLV_COMPOUND,
    "Bearer Context",
    TLV_BEARER_CONTEXT_TYPE,
    0,
    0,
    sizeof(tlv_bearer_context_t),
    {
        &tlv_desc_ebi_0,
        &tlv_desc_bearer_tft_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_f_teid_1,
        &tlv_desc_f_teid_2,
        &tlv_desc_f_teid_3,
        &tlv_desc_f_teid_4,
        &tlv_desc_f_teid_5,
        &tlv_desc_f_teid_6,
        &tlv_desc_bearer_qos_0,
        &tlv_desc_f_teid_7,
        &tlv_desc_cause_0,
        &tlv_desc_charging_id_0,
        &tlv_desc_bearer_flags_0,
        &tlv_desc_pco_0,
        &tlv_desc_epco_0,
        &tlv_desc_f_teid_8,
        &tlv_desc_f_teid_9,
        &tlv_desc_f_teid_10,
        &tlv_desc_f_teid_11,
        &tlv_desc_ran_nas_cause_0,
        &tlv_desc_apco_0,
        &tlv_desc_f_container_0,
        &tlv_desc_ti_0,
        &tlv_desc_packet_flow_id_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_bearer_context_1 =
{
    TLV_COMPOUND,
    "Bearer Context",
    TLV_BEARER_CONTEXT_TYPE,
    0,
    1,
    sizeof(tlv_bearer_context_t),
    {
        &tlv_desc_ebi_0,
        &tlv_desc_bearer_tft_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_f_teid_1,
        &tlv_desc_f_teid_2,
        &tlv_desc_f_teid_3,
        &tlv_desc_f_teid_4,
        &tlv_desc_f_teid_5,
        &tlv_desc_f_teid_6,
        &tlv_desc_bearer_qos_0,
        &tlv_desc_f_teid_7,
        &tlv_desc_cause_0,
        &tlv_desc_charging_id_0,
        &tlv_desc_bearer_flags_0,
        &tlv_desc_pco_0,
        &tlv_desc_epco_0,
        &tlv_desc_f_teid_8,
        &tlv_desc_f_teid_9,
        &tlv_desc_f_teid_10,
        &tlv_desc_f_teid_11,
        &tlv_desc_ran_nas_cause_0,
        &tlv_desc_apco_0,
        &tlv_desc_f_container_0,
        &tlv_desc_ti_0,
        &tlv_desc_packet_flow_id_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_bearer_context_2 =
{
    TLV_COMPOUND,
    "Bearer Context",
    TLV_BEARER_CONTEXT_TYPE,
    0,
    2,
    sizeof(tlv_bearer_context_t),
    {
        &tlv_desc_ebi_0,
        &tlv_desc_bearer_tft_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_f_teid_1,
        &tlv_desc_f_teid_2,
        &tlv_desc_f_teid_3,
        &tlv_desc_f_teid_4,
        &tlv_desc_f_teid_5,
        &tlv_desc_f_teid_6,
        &tlv_desc_bearer_qos_0,
        &tlv_desc_f_teid_7,
        &tlv_desc_cause_0,
        &tlv_desc_charging_id_0,
        &tlv_desc_bearer_flags_0,
        &tlv_desc_pco_0,
        &tlv_desc_epco_0,
        &tlv_desc_f_teid_8,
        &tlv_desc_f_teid_9,
        &tlv_desc_f_teid_10,
        &tlv_desc_f_teid_11,
        &tlv_desc_ran_nas_cause_0,
        &tlv_desc_apco_0,
        &tlv_desc_f_container_0,
        &tlv_desc_ti_0,
        &tlv_desc_packet_flow_id_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_bearer_context_3 =
{
    TLV_COMPOUND,
    "Bearer Context",
    TLV_BEARER_CONTEXT_TYPE,
    0,
    3,
    sizeof(tlv_bearer_context_t),
    {
        &tlv_desc_ebi_0,
        &tlv_desc_bearer_tft_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_f_teid_1,
        &tlv_desc_f_teid_2,
        &tlv_desc_f_teid_3,
        &tlv_desc_f_teid_4,
        &tlv_desc_f_teid_5,
        &tlv_desc_f_teid_6,
        &tlv_desc_bearer_qos_0,
        &tlv_desc_f_teid_7,
        &tlv_desc_cause_0,
        &tlv_desc_charging_id_0,
        &tlv_desc_bearer_flags_0,
        &tlv_desc_pco_0,
        &tlv_desc_epco_0,
        &tlv_desc_f_teid_8,
        &tlv_desc_f_teid_9,
        &tlv_desc_f_teid_10,
        &tlv_desc_f_teid_11,
        &tlv_desc_ran_nas_cause_0,
        &tlv_desc_apco_0,
        &tlv_desc_f_container_0,
        &tlv_desc_ti_0,
        &tlv_desc_packet_flow_id_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_bearer_context_4 =
{
    TLV_COMPOUND,
    "Bearer Context",
    TLV_BEARER_CONTEXT_TYPE,
    0,
    4,
    sizeof(tlv_bearer_context_t),
    {
        &tlv_desc_ebi_0,
        &tlv_desc_bearer_tft_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_f_teid_1,
        &tlv_desc_f_teid_2,
        &tlv_desc_f_teid_3,
        &tlv_desc_f_teid_4,
        &tlv_desc_f_teid_5,
        &tlv_desc_f_teid_6,
        &tlv_desc_bearer_qos_0,
        &tlv_desc_f_teid_7,
        &tlv_desc_cause_0,
        &tlv_desc_charging_id_0,
        &tlv_desc_bearer_flags_0,
        &tlv_desc_pco_0,
        &tlv_desc_epco_0,
        &tlv_desc_f_teid_8,
        &tlv_desc_f_teid_9,
        &tlv_desc_f_teid_10,
        &tlv_desc_f_teid_11,
        &tlv_desc_ran_nas_cause_0,
        &tlv_desc_apco_0,
        &tlv_desc_f_container_0,
        &tlv_desc_ti_0,
        &tlv_desc_packet_flow_id_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_bearer_context_5 =
{
    TLV_COMPOUND,
    "Bearer Context",
    TLV_BEARER_CONTEXT_TYPE,
    0,
    5,
    sizeof(tlv_bearer_context_t),
    {
        &tlv_desc_ebi_0,
        &tlv_desc_bearer_tft_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_f_teid_1,
        &tlv_desc_f_teid_2,
        &tlv_desc_f_teid_3,
        &tlv_desc_f_teid_4,
        &tlv_desc_f_teid_5,
        &tlv_desc_f_teid_6,
        &tlv_desc_bearer_qos_0,
        &tlv_desc_f_teid_7,
        &tlv_desc_cause_0,
        &tlv_desc_charging_id_0,
        &tlv_desc_bearer_flags_0,
        &tlv_desc_pco_0,
        &tlv_desc_epco_0,
        &tlv_desc_f_teid_8,
        &tlv_desc_f_teid_9,
        &tlv_desc_f_teid_10,
        &tlv_desc_f_teid_11,
        &tlv_desc_ran_nas_cause_0,
        &tlv_desc_apco_0,
        &tlv_desc_f_container_0,
        &tlv_desc_ti_0,
        &tlv_desc_packet_flow_id_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_bearer_context_6 =
{
    TLV_COMPOUND,
    "Bearer Context",
    TLV_BEARER_CONTEXT_TYPE,
    0,
    6,
    sizeof(tlv_bearer_context_t),
    {
        &tlv_desc_ebi_0,
        &tlv_desc_bearer_tft_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_f_teid_1,
        &tlv_desc_f_teid_2,
        &tlv_desc_f_teid_3,
        &tlv_desc_f_teid_4,
        &tlv_desc_f_teid_5,
        &tlv_desc_f_teid_6,
        &tlv_desc_bearer_qos_0,
        &tlv_desc_f_teid_7,
        &tlv_desc_cause_0,
        &tlv_desc_charging_id_0,
        &tlv_desc_bearer_flags_0,
        &tlv_desc_pco_0,
        &tlv_desc_epco_0,
        &tlv_desc_f_teid_8,
        &tlv_desc_f_teid_9,
        &tlv_desc_f_teid_10,
        &tlv_desc_f_teid_11,
        &tlv_desc_ran_nas_cause_0,
        &tlv_desc_apco_0,
        &tlv_desc_f_container_0,
        &tlv_desc_ti_0,
        &tlv_desc_packet_flow_id_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_bearer_context_7 =
{
    TLV_COMPOUND,
    "Bearer Context",
    TLV_BEARER_CONTEXT_TYPE,
    0,
    7,
    sizeof(tlv_bearer_context_t),
    {
        &tlv_desc_ebi_0,
        &tlv_desc_bearer_tft_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_f_teid_1,
        &tlv_desc_f_teid_2,
        &tlv_desc_f_teid_3,
        &tlv_desc_f_teid_4,
        &tlv_desc_f_teid_5,
        &tlv_desc_f_teid_6,
        &tlv_desc_bearer_qos_0,
        &tlv_desc_f_teid_7,
        &tlv_desc_cause_0,
        &tlv_desc_charging_id_0,
        &tlv_desc_bearer_flags_0,
        &tlv_desc_pco_0,
        &tlv_desc_epco_0,
        &tlv_desc_f_teid_8,
        &tlv_desc_f_teid_9,
        &tlv_desc_f_teid_10,
        &tlv_desc_f_teid_11,
        &tlv_desc_ran_nas_cause_0,
        &tlv_desc_apco_0,
        &tlv_desc_f_container_0,
        &tlv_desc_ti_0,
        &tlv_desc_packet_flow_id_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_bearer_context_8 =
{
    TLV_COMPOUND,
    "Bearer Context",
    TLV_BEARER_CONTEXT_TYPE,
    0,
    8,
    sizeof(tlv_bearer_context_t),
    {
        &tlv_desc_ebi_0,
        &tlv_desc_bearer_tft_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_f_teid_1,
        &tlv_desc_f_teid_2,
        &tlv_desc_f_teid_3,
        &tlv_desc_f_teid_4,
        &tlv_desc_f_teid_5,
        &tlv_desc_f_teid_6,
        &tlv_desc_bearer_qos_0,
        &tlv_desc_f_teid_7,
        &tlv_desc_cause_0,
        &tlv_desc_charging_id_0,
        &tlv_desc_bearer_flags_0,
        &tlv_desc_pco_0,
        &tlv_desc_epco_0,
        &tlv_desc_f_teid_8,
        &tlv_desc_f_teid_9,
        &tlv_desc_f_teid_10,
        &tlv_desc_f_teid_11,
        &tlv_desc_ran_nas_cause_0,
        &tlv_desc_apco_0,
        &tlv_desc_f_container_0,
        &tlv_desc_ti_0,
        &tlv_desc_packet_flow_id_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_bearer_context_9 =
{
    TLV_COMPOUND,
    "Bearer Context",
    TLV_BEARER_CONTEXT_TYPE,
    0,
    9,
    sizeof(tlv_bearer_context_t),
    {
        &tlv_desc_ebi_0,
        &tlv_desc_bearer_tft_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_f_teid_1,
        &tlv_desc_f_teid_2,
        &tlv_desc_f_teid_3,
        &tlv_desc_f_teid_4,
        &tlv_desc_f_teid_5,
        &tlv_desc_f_teid_6,
        &tlv_desc_bearer_qos_0,
        &tlv_desc_f_teid_7,
        &tlv_desc_cause_0,
        &tlv_desc_charging_id_0,
        &tlv_desc_bearer_flags_0,
        &tlv_desc_pco_0,
        &tlv_desc_epco_0,
        &tlv_desc_f_teid_8,
        &tlv_desc_f_teid_9,
        &tlv_desc_f_teid_10,
        &tlv_desc_f_teid_11,
        &tlv_desc_ran_nas_cause_0,
        &tlv_desc_apco_0,
        &tlv_desc_f_container_0,
        &tlv_desc_ti_0,
        &tlv_desc_packet_flow_id_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_bearer_context_10 =
{
    TLV_COMPOUND,
    "Bearer Context",
    TLV_BEARER_CONTEXT_TYPE,
    0,
    10,
    sizeof(tlv_bearer_context_t),
    {
        &tlv_desc_ebi_0,
        &tlv_desc_bearer_tft_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_f_teid_1,
        &tlv_desc_f_teid_2,
        &tlv_desc_f_teid_3,
        &tlv_desc_f_teid_4,
        &tlv_desc_f_teid_5,
        &tlv_desc_f_teid_6,
        &tlv_desc_bearer_qos_0,
        &tlv_desc_f_teid_7,
        &tlv_desc_cause_0,
        &tlv_desc_charging_id_0,
        &tlv_desc_bearer_flags_0,
        &tlv_desc_pco_0,
        &tlv_desc_epco_0,
        &tlv_desc_f_teid_8,
        &tlv_desc_f_teid_9,
        &tlv_desc_f_teid_10,
        &tlv_desc_f_teid_11,
        &tlv_desc_ran_nas_cause_0,
        &tlv_desc_apco_0,
        &tlv_desc_f_container_0,
        &tlv_desc_ti_0,
        &tlv_desc_packet_flow_id_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_pdn_connection_0 =
{
    TLV_COMPOUND,
    "PDN Connection",
    TLV_PDN_CONNECTION_TYPE,
    0,
    0,
    sizeof(tlv_pdn_connection_t),
    {
        NULL,
    }
};

tlv_desc_t tlv_desc_overload_control_information_0 =
{
    TLV_COMPOUND,
    "Overload Control Information",
    TLV_OVERLOAD_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(tlv_overload_control_information_t),
    {
        &tlv_desc_sequence_number_0,
        &tlv_desc_metric_0,
        &tlv_desc_epc_timer_0,
        &tlv_desc_apn_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_overload_control_information_1 =
{
    TLV_COMPOUND,
    "Overload Control Information",
    TLV_OVERLOAD_CONTROL_INFORMATION_TYPE,
    0,
    1,
    sizeof(tlv_overload_control_information_t),
    {
        &tlv_desc_sequence_number_0,
        &tlv_desc_metric_0,
        &tlv_desc_epc_timer_0,
        &tlv_desc_apn_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_overload_control_information_2 =
{
    TLV_COMPOUND,
    "Overload Control Information",
    TLV_OVERLOAD_CONTROL_INFORMATION_TYPE,
    0,
    2,
    sizeof(tlv_overload_control_information_t),
    {
        &tlv_desc_sequence_number_0,
        &tlv_desc_metric_0,
        &tlv_desc_epc_timer_0,
        &tlv_desc_apn_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_load_control_information_0 =
{
    TLV_COMPOUND,
    "Load Control Information",
    TLV_LOAD_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(tlv_load_control_information_t),
    {
        &tlv_desc_sequence_number_0,
        &tlv_desc_metric_0,
        &tlv_desc_apn_and_relative_capacity_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_load_control_information_1 =
{
    TLV_COMPOUND,
    "Load Control Information",
    TLV_LOAD_CONTROL_INFORMATION_TYPE,
    0,
    1,
    sizeof(tlv_load_control_information_t),
    {
        &tlv_desc_sequence_number_0,
        &tlv_desc_metric_0,
        &tlv_desc_apn_and_relative_capacity_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_load_control_information_2 =
{
    TLV_COMPOUND,
    "Load Control Information",
    TLV_LOAD_CONTROL_INFORMATION_TYPE,
    0,
    2,
    sizeof(tlv_load_control_information_t),
    {
        &tlv_desc_sequence_number_0,
        &tlv_desc_metric_0,
        &tlv_desc_apn_and_relative_capacity_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_remote_ue_context_0 =
{
    TLV_COMPOUND,
    "Remote UE Context",
    TLV_REMOTE_UE_CONTEXT_TYPE,
    0,
    0,
    sizeof(tlv_remote_ue_context_t),
    {
        &tlv_desc_remote_user_id_0,
        &tlv_desc_remote_ue_ip_information_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_scef_pdn_connection_0 =
{
    TLV_COMPOUND,
    "SCEF PDN Connection",
    TLV_SCEF_PDN_CONNECTION_TYPE,
    0,
    0,
    sizeof(tlv_scef_pdn_connection_t),
    {
        &tlv_desc_apn_0,
        &tlv_desc_ebi_0,
        &tlv_desc_node_identifier_0,
        NULL,
    }
};

tlv_desc_t tlv_desc_echo_request =
{
    TLV_MESSAGE,
    "Echo Request",
    0, 0, 0, 0, {
        &tlv_desc_recovery_0,
        &tlv_desc_node_features_0,
    NULL,
}};

tlv_desc_t tlv_desc_echo_response =
{
    TLV_MESSAGE,
    "Echo Response",
    0, 0, 0, 0, {
        &tlv_desc_recovery_0,
        &tlv_desc_node_features_0,
    NULL,
}};

tlv_desc_t tlv_desc_create_session_request =
{
    TLV_MESSAGE,
    "Create Session Request",
    0, 0, 0, 0, {
        &tlv_desc_imsi_0,
        &tlv_desc_msisdn_0,
        &tlv_desc_mei_0,
        &tlv_desc_uli_0,
        &tlv_desc_serving_network_0,
        &tlv_desc_rat_type_0,
        &tlv_desc_indication_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_f_teid_1,
        &tlv_desc_apn_0,
        &tlv_desc_selection_mode_0,
        &tlv_desc_pdn_type_0,
        &tlv_desc_paa_0,
        &tlv_desc_apn_restriction_0,
        &tlv_desc_ambr_0,
        &tlv_desc_ebi_0,
        &tlv_desc_twmi_0,
        &tlv_desc_pco_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_bearer_context_1,
        &tlv_desc_trace_information_0,
        &tlv_desc_recovery_0,
        &tlv_desc_fq_csid_0,
        &tlv_desc_fq_csid_1,
        &tlv_desc_fq_csid_2,
        &tlv_desc_fq_csid_3,
        &tlv_desc_ue_time_zone_0,
        &tlv_desc_uci_0,
        &tlv_desc_charging_characteristics_0,
        &tlv_desc_ldn_0,
        &tlv_desc_ldn_1,
        &tlv_desc_ldn_2,
        &tlv_desc_ldn_3,
        &tlv_desc_signalling_priority_indication_0,
        &tlv_desc_ip_address_0,
        &tlv_desc_port_number_0,
        &tlv_desc_apco_0,
        &tlv_desc_ip_address_1,
        &tlv_desc_port_number_1,
        &tlv_desc_ip_address_2,
        &tlv_desc_twan_identifier_0,
        &tlv_desc_ip_address_3,
        &tlv_desc_cn_operator_selection_entity_0,
        &tlv_desc_presence_reporting_area_information_0,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_overload_control_information_2,
        &tlv_desc_millisecond_time_stamp_0,
        &tlv_desc_integer_number_0,
        &tlv_desc_twan_identifier_1,
        &tlv_desc_twan_identifier_timestamp_0,
        &tlv_desc_f_container_0,
        &tlv_desc_remote_ue_context_0,
        &tlv_desc_node_identifier_0,
        &tlv_desc_epco_0,
        &tlv_desc_serving_plmn_rate_control_0,
        &tlv_desc_counter_0,
        &tlv_desc_port_number_2,
    NULL,
}};

tlv_desc_t tlv_desc_create_session_response =
{
    TLV_MESSAGE,
    "Create Session Response",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_change_reporting_action_0,
        &tlv_desc_csg_information_reporting_action_0,
        &tlv_desc_enb_information_reporting_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_f_teid_1,
        &tlv_desc_paa_0,
        &tlv_desc_apn_restriction_0,
        &tlv_desc_ambr_0,
        &tlv_desc_ebi_0,
        &tlv_desc_pco_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_bearer_context_1,
        &tlv_desc_recovery_0,
        &tlv_desc_fqdn_0,
        &tlv_desc_ip_address_0,
        &tlv_desc_fq_csid_0,
        &tlv_desc_fq_csid_1,
        &tlv_desc_ldn_0,
        &tlv_desc_ldn_1,
        &tlv_desc_epc_timer_0,
        &tlv_desc_apco_0,
        &tlv_desc_ip4cp_0,
        &tlv_desc_indication_0,
        &tlv_desc_presence_reporting_area_action_0,
        &tlv_desc_load_control_information_0,
        &tlv_desc_load_control_information_1,
        &tlv_desc_load_control_information_2,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_f_container_0,
        &tlv_desc_charging_id_0,
        &tlv_desc_epco_0,
    NULL,
}};

tlv_desc_t tlv_desc_modify_bearer_request =
{
    TLV_MESSAGE,
    "Modify Bearer Request",
    0, 0, 0, 0, {
        &tlv_desc_mei_0,
        &tlv_desc_uli_0,
        &tlv_desc_serving_network_0,
        &tlv_desc_rat_type_0,
        &tlv_desc_indication_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_ambr_0,
        &tlv_desc_delay_value_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_bearer_context_1,
        &tlv_desc_recovery_0,
        &tlv_desc_ue_time_zone_0,
        &tlv_desc_fq_csid_0,
        &tlv_desc_uci_0,
        &tlv_desc_ip_address_1,
        &tlv_desc_port_number_1,
        &tlv_desc_ldn_0,
        &tlv_desc_ldn_1,
        &tlv_desc_ip_address_0,
        &tlv_desc_port_number_0,
        &tlv_desc_ip_address_2,
        &tlv_desc_cn_operator_selection_entity_0,
        &tlv_desc_presence_reporting_area_information_0,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_overload_control_information_2,
        &tlv_desc_serving_plmn_rate_control_0,
        &tlv_desc_counter_0,
    NULL,
}};

tlv_desc_t tlv_desc_modify_bearer_response =
{
    TLV_MESSAGE,
    "Modify Bearer Response",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_msisdn_0,
        &tlv_desc_ebi_0,
        &tlv_desc_apn_restriction_0,
        &tlv_desc_pco_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_bearer_context_1,
        &tlv_desc_change_reporting_action_0,
        &tlv_desc_csg_information_reporting_action_0,
        &tlv_desc_enb_information_reporting_0,
        &tlv_desc_fqdn_0,
        &tlv_desc_ip_address_0,
        &tlv_desc_fq_csid_0,
        &tlv_desc_fq_csid_1,
        &tlv_desc_recovery_0,
        &tlv_desc_ldn_0,
        &tlv_desc_ldn_1,
        &tlv_desc_indication_0,
        &tlv_desc_presence_reporting_area_action_0,
        &tlv_desc_load_control_information_0,
        &tlv_desc_load_control_information_1,
        &tlv_desc_load_control_information_2,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_charging_id_0,
    NULL,
}};

tlv_desc_t tlv_desc_delete_session_request =
{
    TLV_MESSAGE,
    "Delete Session Request",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_ebi_0,
        &tlv_desc_uli_0,
        &tlv_desc_indication_0,
        &tlv_desc_pco_0,
        &tlv_desc_node_type_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_ue_time_zone_0,
        &tlv_desc_uli_timestamp_0,
        &tlv_desc_ran_nas_cause_0,
        &tlv_desc_twan_identifier_0,
        &tlv_desc_twan_identifier_timestamp_0,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_overload_control_information_2,
        &tlv_desc_twan_identifier_1,
        &tlv_desc_twan_identifier_timestamp_1,
        &tlv_desc_ip_address_0,
        &tlv_desc_port_number_0,
        &tlv_desc_epco_0,
        &tlv_desc_port_number_1,
    NULL,
}};

tlv_desc_t tlv_desc_delete_session_response =
{
    TLV_MESSAGE,
    "Delete Session Response",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_recovery_0,
        &tlv_desc_pco_0,
        &tlv_desc_indication_0,
        &tlv_desc_load_control_information_0,
        &tlv_desc_load_control_information_1,
        &tlv_desc_load_control_information_2,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_epco_0,
    NULL,
}};

tlv_desc_t tlv_desc_modify_bearer_command =
{
    TLV_MESSAGE,
    "Modify Bearer Command",
    0, 0, 0, 0, {
        &tlv_desc_ambr_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_overload_control_information_2,
        &tlv_desc_f_teid_0,
    NULL,
}};

tlv_desc_t tlv_desc_modify_bearer_failure_indication =
{
    TLV_MESSAGE,
    "Modify Bearer Failure Indication",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_recovery_0,
        &tlv_desc_indication_0,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
    NULL,
}};

tlv_desc_t tlv_desc_delete_bearer_command =
{
    TLV_MESSAGE,
    "Delete Bearer Command",
    0, 0, 0, 0, {
        &tlv_desc_bearer_context_0,
        &tlv_desc_uli_0,
        &tlv_desc_uli_timestamp_0,
        &tlv_desc_ue_time_zone_0,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_f_teid_0,
    NULL,
}};

tlv_desc_t tlv_desc_delete_bearer_failure_indication =
{
    TLV_MESSAGE,
    "Delete Bearer Failure Indication",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_recovery_0,
        &tlv_desc_indication_0,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
    NULL,
}};

tlv_desc_t tlv_desc_downlink_data_notification_failure_indication =
{
    TLV_MESSAGE,
    "Downlink Data Notification Failure Indication",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_node_type_0,
        &tlv_desc_imsi_0,
    NULL,
}};

tlv_desc_t tlv_desc_create_bearer_request =
{
    TLV_MESSAGE,
    "Create Bearer Request",
    0, 0, 0, 0, {
        &tlv_desc_pti_0,
        &tlv_desc_ebi_0,
        &tlv_desc_pco_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_fq_csid_0,
        &tlv_desc_fq_csid_1,
        &tlv_desc_change_reporting_action_0,
        &tlv_desc_csg_information_reporting_action_0,
        &tlv_desc_enb_information_reporting_0,
        &tlv_desc_presence_reporting_area_action_0,
        &tlv_desc_indication_0,
        &tlv_desc_load_control_information_0,
        &tlv_desc_load_control_information_1,
        &tlv_desc_load_control_information_2,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_f_container_0,
    NULL,
}};

tlv_desc_t tlv_desc_create_bearer_response =
{
    TLV_MESSAGE,
    "Create Bearer Response",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_recovery_0,
        &tlv_desc_fq_csid_0,
        &tlv_desc_fq_csid_2,
        &tlv_desc_fq_csid_3,
        &tlv_desc_pco_0,
        &tlv_desc_ue_time_zone_0,
        &tlv_desc_uli_0,
        &tlv_desc_twan_identifier_0,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_presence_reporting_area_information_0,
        &tlv_desc_ip_address_0,
        &tlv_desc_overload_control_information_2,
        &tlv_desc_twan_identifier_1,
        &tlv_desc_twan_identifier_timestamp_1,
        &tlv_desc_port_number_0,
        &tlv_desc_f_container_0,
        &tlv_desc_port_number_1,
    NULL,
}};

tlv_desc_t tlv_desc_update_bearer_request =
{
    TLV_MESSAGE,
    "Update Bearer Request",
    0, 0, 0, 0, {
        &tlv_desc_bearer_context_0,
        &tlv_desc_pti_0,
        &tlv_desc_pco_0,
        &tlv_desc_ambr_0,
        &tlv_desc_change_reporting_action_0,
        &tlv_desc_csg_information_reporting_action_0,
        &tlv_desc_enb_information_reporting_0,
        &tlv_desc_indication_0,
        &tlv_desc_fq_csid_0,
        &tlv_desc_fq_csid_1,
        &tlv_desc_presence_reporting_area_action_0,
        &tlv_desc_load_control_information_0,
        &tlv_desc_load_control_information_1,
        &tlv_desc_load_control_information_2,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_f_container_0,
    NULL,
}};

tlv_desc_t tlv_desc_update_bearer_response =
{
    TLV_MESSAGE,
    "Update Bearer Response",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_pco_0,
        &tlv_desc_recovery_0,
        &tlv_desc_fq_csid_0,
        &tlv_desc_fq_csid_1,
        &tlv_desc_fq_csid_2,
        &tlv_desc_fq_csid_3,
        &tlv_desc_indication_0,
        &tlv_desc_ue_time_zone_0,
        &tlv_desc_uli_0,
        &tlv_desc_twan_identifier_0,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_presence_reporting_area_information_0,
        &tlv_desc_ip_address_0,
        &tlv_desc_overload_control_information_2,
        &tlv_desc_twan_identifier_1,
        &tlv_desc_twan_identifier_timestamp_1,
        &tlv_desc_port_number_0,
        &tlv_desc_f_container_0,
        &tlv_desc_port_number_1,
    NULL,
}};

tlv_desc_t tlv_desc_delete_bearer_request =
{
    TLV_MESSAGE,
    "Delete Bearer Request",
    0, 0, 0, 0, {
        &tlv_desc_ebi_0,
        &tlv_desc_ebi_1,
        &tlv_desc_bearer_context_0,
        &tlv_desc_pti_0,
        &tlv_desc_pco_0,
        &tlv_desc_fq_csid_0,
        &tlv_desc_fq_csid_1,
        &tlv_desc_cause_0,
        &tlv_desc_indication_0,
        &tlv_desc_load_control_information_0,
        &tlv_desc_load_control_information_1,
        &tlv_desc_load_control_information_2,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_f_container_0,
        &tlv_desc_epco_0,
    NULL,
}};

tlv_desc_t tlv_desc_delete_bearer_response =
{
    TLV_MESSAGE,
    "Delete Bearer Response",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_ebi_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_recovery_0,
        &tlv_desc_fq_csid_0,
        &tlv_desc_fq_csid_1,
        &tlv_desc_fq_csid_2,
        &tlv_desc_fq_csid_3,
        &tlv_desc_pco_0,
        &tlv_desc_ue_time_zone_0,
        &tlv_desc_uli_0,
        &tlv_desc_uli_timestamp_0,
        &tlv_desc_twan_identifier_0,
        &tlv_desc_twan_identifier_timestamp_0,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_overload_control_information_1,
        &tlv_desc_ip_address_0,
        &tlv_desc_overload_control_information_2,
        &tlv_desc_twan_identifier_1,
        &tlv_desc_twan_identifier_timestamp_1,
        &tlv_desc_port_number_0,
        &tlv_desc_f_container_0,
        &tlv_desc_port_number_1,
    NULL,
}};

tlv_desc_t tlv_desc_create_indirect_data_forwarding_tunnel_request =
{
    TLV_MESSAGE,
    "Create Indirect Data Forwarding Tunnel Request",
    0, 0, 0, 0, {
        &tlv_desc_imsi_0,
        &tlv_desc_mei_0,
        &tlv_desc_indication_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_bearer_context_1,
        &tlv_desc_bearer_context_2,
        &tlv_desc_bearer_context_3,
        &tlv_desc_bearer_context_4,
        &tlv_desc_bearer_context_5,
        &tlv_desc_bearer_context_6,
        &tlv_desc_bearer_context_7,
        &tlv_desc_bearer_context_8,
        &tlv_desc_bearer_context_9,
        &tlv_desc_bearer_context_10,
        &tlv_desc_recovery_0,
    NULL,
}};

tlv_desc_t tlv_desc_create_indirect_data_forwarding_tunnel_response =
{
    TLV_MESSAGE,
    "Create Indirect Data Forwarding Tunnel Response",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_bearer_context_1,
        &tlv_desc_bearer_context_2,
        &tlv_desc_bearer_context_3,
        &tlv_desc_bearer_context_4,
        &tlv_desc_bearer_context_5,
        &tlv_desc_bearer_context_6,
        &tlv_desc_bearer_context_7,
        &tlv_desc_bearer_context_8,
        &tlv_desc_bearer_context_9,
        &tlv_desc_bearer_context_10,
        &tlv_desc_recovery_0,
    NULL,
}};

tlv_desc_t tlv_desc_delete_indirect_data_forwarding_tunnel_request =
{
    TLV_MESSAGE,
    "Delete Indirect Data Forwarding Tunnel Request",
    0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t tlv_desc_delete_indirect_data_forwarding_tunnel_response =
{
    TLV_MESSAGE,
    "Delete Indirect Data Forwarding Tunnel Response",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_recovery_0,
    NULL,
}};

tlv_desc_t tlv_desc_release_access_bearers_request =
{
    TLV_MESSAGE,
    "Release Access Bearers Request",
    0, 0, 0, 0, {
        &tlv_desc_ebi_0,
        &tlv_desc_node_type_0,
        &tlv_desc_indication_0,
    NULL,
}};

tlv_desc_t tlv_desc_release_access_bearers_response =
{
    TLV_MESSAGE,
    "Release Access Bearers Response",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_recovery_0,
        &tlv_desc_indication_0,
        &tlv_desc_load_control_information_0,
        &tlv_desc_overload_control_information_0,
    NULL,
}};

tlv_desc_t tlv_desc_downlink_data_notification =
{
    TLV_MESSAGE,
    "Downlink Data Notification",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_ebi_0,
        &tlv_desc_arp_0,
        &tlv_desc_imsi_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_indication_0,
        &tlv_desc_load_control_information_0,
        &tlv_desc_overload_control_information_0,
        &tlv_desc_paging_and_service_information_0,
    NULL,
}};

tlv_desc_t tlv_desc_downlink_data_notification_acknowledge =
{
    TLV_MESSAGE,
    "Downlink Data Notification Acknowledge",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_delay_value_0,
        &tlv_desc_recovery_0,
        &tlv_desc_throttling_0,
        &tlv_desc_imsi_0,
        &tlv_desc_epc_timer_0,
        &tlv_desc_integer_number_0,
    NULL,
}};

tlv_desc_t tlv_desc_modify_access_bearers_request =
{
    TLV_MESSAGE,
    "Modify Access Bearers Request",
    0, 0, 0, 0, {
        &tlv_desc_indication_0,
        &tlv_desc_f_teid_0,
        &tlv_desc_delay_value_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_bearer_context_1,
        &tlv_desc_recovery_0,
    NULL,
}};

tlv_desc_t tlv_desc_modify_access_bearers_response =
{
    TLV_MESSAGE,
    "Modify Access Bearers Response",
    0, 0, 0, 0, {
        &tlv_desc_cause_0,
        &tlv_desc_bearer_context_0,
        &tlv_desc_bearer_context_1,
        &tlv_desc_recovery_0,
        &tlv_desc_indication_0,
        &tlv_desc_load_control_information_0,
        &tlv_desc_overload_control_information_0,
    NULL,
}};


status_t gtp_parse_msg(gtp_message_t *gtp_message, pkbuf_t *pkbuf)
{
    status_t rv = CORE_ERROR;
    gtp_header_t *h = NULL;
    c_uint16_t size = 0;

    d_assert(gtp_message, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");
    d_assert(pkbuf->payload, return CORE_ERROR, "Null param");

    d_trace(50, "[GTPv2] RECV : ");
    d_trace_hex(50, pkbuf->payload, pkbuf->len);

    h = pkbuf->payload;
    d_assert(h, return CORE_ERROR, "Null param");
    
    memset(gtp_message, 0, sizeof(gtp_message_t));

    if (h->teid_presence)
        size = GTPV2C_HEADER_LEN;
    else
        size = GTPV2C_HEADER_LEN-GTP_TEID_LEN;

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK,
            return CORE_ERROR, "pkbuf_header error");
    memcpy(&gtp_message->h, pkbuf->payload - size, size);

    if (h->teid_presence)
        gtp_message->h.teid = ntohl(gtp_message->h.teid);

    if (pkbuf->len == 0)
        return CORE_OK;

    switch(gtp_message->h.type)
    {
        case GTP_ECHO_REQUEST_TYPE:
            rv = tlv_parse_msg(&gtp_message->echo_request,
                    &tlv_desc_echo_request, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_ECHO_RESPONSE_TYPE:
            rv = tlv_parse_msg(&gtp_message->echo_response,
                    &tlv_desc_echo_response, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_CREATE_SESSION_REQUEST_TYPE:
            rv = tlv_parse_msg(&gtp_message->create_session_request,
                    &tlv_desc_create_session_request, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_CREATE_SESSION_RESPONSE_TYPE:
            rv = tlv_parse_msg(&gtp_message->create_session_response,
                    &tlv_desc_create_session_response, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_MODIFY_BEARER_REQUEST_TYPE:
            rv = tlv_parse_msg(&gtp_message->modify_bearer_request,
                    &tlv_desc_modify_bearer_request, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_MODIFY_BEARER_RESPONSE_TYPE:
            rv = tlv_parse_msg(&gtp_message->modify_bearer_response,
                    &tlv_desc_modify_bearer_response, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_SESSION_REQUEST_TYPE:
            rv = tlv_parse_msg(&gtp_message->delete_session_request,
                    &tlv_desc_delete_session_request, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_SESSION_RESPONSE_TYPE:
            rv = tlv_parse_msg(&gtp_message->delete_session_response,
                    &tlv_desc_delete_session_response, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_MODIFY_BEARER_COMMAND_TYPE:
            rv = tlv_parse_msg(&gtp_message->modify_bearer_command,
                    &tlv_desc_modify_bearer_command, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_MODIFY_BEARER_FAILURE_INDICATION_TYPE:
            rv = tlv_parse_msg(&gtp_message->modify_bearer_failure_indication,
                    &tlv_desc_modify_bearer_failure_indication, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_BEARER_COMMAND_TYPE:
            rv = tlv_parse_msg(&gtp_message->delete_bearer_command,
                    &tlv_desc_delete_bearer_command, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_BEARER_FAILURE_INDICATION_TYPE:
            rv = tlv_parse_msg(&gtp_message->delete_bearer_failure_indication,
                    &tlv_desc_delete_bearer_failure_indication, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DOWNLINK_DATA_NOTIFICATION_FAILURE_INDICATION_TYPE:
            rv = tlv_parse_msg(&gtp_message->downlink_data_notification_failure_indication,
                    &tlv_desc_downlink_data_notification_failure_indication, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_CREATE_BEARER_REQUEST_TYPE:
            rv = tlv_parse_msg(&gtp_message->create_bearer_request,
                    &tlv_desc_create_bearer_request, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_CREATE_BEARER_RESPONSE_TYPE:
            rv = tlv_parse_msg(&gtp_message->create_bearer_response,
                    &tlv_desc_create_bearer_response, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_UPDATE_BEARER_REQUEST_TYPE:
            rv = tlv_parse_msg(&gtp_message->update_bearer_request,
                    &tlv_desc_update_bearer_request, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_UPDATE_BEARER_RESPONSE_TYPE:
            rv = tlv_parse_msg(&gtp_message->update_bearer_response,
                    &tlv_desc_update_bearer_response, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_BEARER_REQUEST_TYPE:
            rv = tlv_parse_msg(&gtp_message->delete_bearer_request,
                    &tlv_desc_delete_bearer_request, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_BEARER_RESPONSE_TYPE:
            rv = tlv_parse_msg(&gtp_message->delete_bearer_response,
                    &tlv_desc_delete_bearer_response, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
            rv = tlv_parse_msg(&gtp_message->create_indirect_data_forwarding_tunnel_request,
                    &tlv_desc_create_indirect_data_forwarding_tunnel_request, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
            rv = tlv_parse_msg(&gtp_message->create_indirect_data_forwarding_tunnel_response,
                    &tlv_desc_create_indirect_data_forwarding_tunnel_response, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
            rv = tlv_parse_msg(&gtp_message->delete_indirect_data_forwarding_tunnel_request,
                    &tlv_desc_delete_indirect_data_forwarding_tunnel_request, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
            rv = tlv_parse_msg(&gtp_message->delete_indirect_data_forwarding_tunnel_response,
                    &tlv_desc_delete_indirect_data_forwarding_tunnel_response, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_RELEASE_ACCESS_BEARERS_REQUEST_TYPE:
            rv = tlv_parse_msg(&gtp_message->release_access_bearers_request,
                    &tlv_desc_release_access_bearers_request, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE:
            rv = tlv_parse_msg(&gtp_message->release_access_bearers_response,
                    &tlv_desc_release_access_bearers_response, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DOWNLINK_DATA_NOTIFICATION_TYPE:
            rv = tlv_parse_msg(&gtp_message->downlink_data_notification,
                    &tlv_desc_downlink_data_notification, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE:
            rv = tlv_parse_msg(&gtp_message->downlink_data_notification_acknowledge,
                    &tlv_desc_downlink_data_notification_acknowledge, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_MODIFY_ACCESS_BEARERS_REQUEST_TYPE:
            rv = tlv_parse_msg(&gtp_message->modify_access_bearers_request,
                    &tlv_desc_modify_access_bearers_request, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        case GTP_MODIFY_ACCESS_BEARERS_RESPONSE_TYPE:
            rv = tlv_parse_msg(&gtp_message->modify_access_bearers_response,
                    &tlv_desc_modify_access_bearers_response, pkbuf, TLV_MODE_T1_L2_I1);
            break;
        default:
            d_warn("Not implmeneted(type:%d)", gtp_message->h.type);
            break;
    }

    return rv;
}

status_t gtp_build_msg(pkbuf_t **pkbuf, gtp_message_t *gtp_message)
{
    status_t rv = CORE_ERROR;

    d_assert(gtp_message, return rv, "Null param");
    switch(gtp_message->h.type)
    {
        case GTP_ECHO_REQUEST_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_echo_request,
                    &gtp_message->echo_request, TLV_MODE_T1_L2_I1);
            break;
        case GTP_ECHO_RESPONSE_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_echo_response,
                    &gtp_message->echo_response, TLV_MODE_T1_L2_I1);
            break;
        case GTP_CREATE_SESSION_REQUEST_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_create_session_request,
                    &gtp_message->create_session_request, TLV_MODE_T1_L2_I1);
            break;
        case GTP_CREATE_SESSION_RESPONSE_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_create_session_response,
                    &gtp_message->create_session_response, TLV_MODE_T1_L2_I1);
            break;
        case GTP_MODIFY_BEARER_REQUEST_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_modify_bearer_request,
                    &gtp_message->modify_bearer_request, TLV_MODE_T1_L2_I1);
            break;
        case GTP_MODIFY_BEARER_RESPONSE_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_modify_bearer_response,
                    &gtp_message->modify_bearer_response, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_SESSION_REQUEST_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_delete_session_request,
                    &gtp_message->delete_session_request, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_SESSION_RESPONSE_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_delete_session_response,
                    &gtp_message->delete_session_response, TLV_MODE_T1_L2_I1);
            break;
        case GTP_MODIFY_BEARER_COMMAND_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_modify_bearer_command,
                    &gtp_message->modify_bearer_command, TLV_MODE_T1_L2_I1);
            break;
        case GTP_MODIFY_BEARER_FAILURE_INDICATION_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_modify_bearer_failure_indication,
                    &gtp_message->modify_bearer_failure_indication, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_BEARER_COMMAND_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_delete_bearer_command,
                    &gtp_message->delete_bearer_command, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_BEARER_FAILURE_INDICATION_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_delete_bearer_failure_indication,
                    &gtp_message->delete_bearer_failure_indication, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DOWNLINK_DATA_NOTIFICATION_FAILURE_INDICATION_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_downlink_data_notification_failure_indication,
                    &gtp_message->downlink_data_notification_failure_indication, TLV_MODE_T1_L2_I1);
            break;
        case GTP_CREATE_BEARER_REQUEST_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_create_bearer_request,
                    &gtp_message->create_bearer_request, TLV_MODE_T1_L2_I1);
            break;
        case GTP_CREATE_BEARER_RESPONSE_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_create_bearer_response,
                    &gtp_message->create_bearer_response, TLV_MODE_T1_L2_I1);
            break;
        case GTP_UPDATE_BEARER_REQUEST_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_update_bearer_request,
                    &gtp_message->update_bearer_request, TLV_MODE_T1_L2_I1);
            break;
        case GTP_UPDATE_BEARER_RESPONSE_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_update_bearer_response,
                    &gtp_message->update_bearer_response, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_BEARER_REQUEST_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_delete_bearer_request,
                    &gtp_message->delete_bearer_request, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_BEARER_RESPONSE_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_delete_bearer_response,
                    &gtp_message->delete_bearer_response, TLV_MODE_T1_L2_I1);
            break;
        case GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_create_indirect_data_forwarding_tunnel_request,
                    &gtp_message->create_indirect_data_forwarding_tunnel_request, TLV_MODE_T1_L2_I1);
            break;
        case GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_create_indirect_data_forwarding_tunnel_response,
                    &gtp_message->create_indirect_data_forwarding_tunnel_response, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_delete_indirect_data_forwarding_tunnel_request,
                    &gtp_message->delete_indirect_data_forwarding_tunnel_request, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_delete_indirect_data_forwarding_tunnel_response,
                    &gtp_message->delete_indirect_data_forwarding_tunnel_response, TLV_MODE_T1_L2_I1);
            break;
        case GTP_RELEASE_ACCESS_BEARERS_REQUEST_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_release_access_bearers_request,
                    &gtp_message->release_access_bearers_request, TLV_MODE_T1_L2_I1);
            break;
        case GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_release_access_bearers_response,
                    &gtp_message->release_access_bearers_response, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DOWNLINK_DATA_NOTIFICATION_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_downlink_data_notification,
                    &gtp_message->downlink_data_notification, TLV_MODE_T1_L2_I1);
            break;
        case GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_downlink_data_notification_acknowledge,
                    &gtp_message->downlink_data_notification_acknowledge, TLV_MODE_T1_L2_I1);
            break;
        case GTP_MODIFY_ACCESS_BEARERS_REQUEST_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_modify_access_bearers_request,
                    &gtp_message->modify_access_bearers_request, TLV_MODE_T1_L2_I1);
            break;
        case GTP_MODIFY_ACCESS_BEARERS_RESPONSE_TYPE:
            rv = tlv_build_msg(pkbuf, &tlv_desc_modify_access_bearers_response,
                    &gtp_message->modify_access_bearers_response, TLV_MODE_T1_L2_I1);
            break;
        default:
            d_warn("Not implmeneted(type:%d)", gtp_message->h.type);
            break;
    }

    if ((*pkbuf) && (*pkbuf)->payload)
    {
        d_trace(50, "[GTPv2] SEND : ");
        d_trace_hex(50, (*pkbuf)->payload, (*pkbuf)->len);
    }

    return rv;
}

