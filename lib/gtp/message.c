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
 * This file had been created by gtp-tlv.py script v0.1.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2019-09-26 22:10:18.839117 by acetcom
 * from 29274-d80.docx
 ******************************************************************************/

#include "ogs-gtp.h"

ogs_tlv_desc_t ogs_tlv_desc_imsi_0 =
{
    OGS_TLV_VAR_STR,
    "IMSI",
    OGS_TLV_IMSI_TYPE,
    0,
    0,
    sizeof(ogs_tlv_imsi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_cause_0 =
{
    OGS_TLV_VAR_STR,
    "Cause",
    OGS_TLV_CAUSE_TYPE,
    0,
    0,
    sizeof(ogs_tlv_cause_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_recovery_0 =
{
    OGS_TLV_UINT8,
    "Recovery",
    OGS_TLV_RECOVERY_TYPE,
    1,
    0,
    sizeof(ogs_tlv_recovery_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_stn_sr_0 =
{
    OGS_TLV_VAR_STR,
    "STN-SR",
    OGS_TLV_STN_SR_TYPE,
    0,
    0,
    sizeof(ogs_tlv_stn_sr_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_apn_0 =
{
    OGS_TLV_VAR_STR,
    "APN",
    OGS_TLV_APN_TYPE,
    0,
    0,
    sizeof(ogs_tlv_apn_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ambr_0 =
{
    OGS_TLV_VAR_STR,
    "AMBR",
    OGS_TLV_AMBR_TYPE,
    0,
    0,
    sizeof(ogs_tlv_ambr_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ebi_0 =
{
    OGS_TLV_UINT8,
    "EBI",
    OGS_TLV_EBI_TYPE,
    1,
    0,
    sizeof(ogs_tlv_ebi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ebi_1 =
{
    OGS_TLV_UINT8,
    "EBI",
    OGS_TLV_EBI_TYPE,
    1,
    1,
    sizeof(ogs_tlv_ebi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ip_address_0 =
{
    OGS_TLV_VAR_STR,
    "IP Address",
    OGS_TLV_IP_ADDRESS_TYPE,
    0,
    0,
    sizeof(ogs_tlv_ip_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ip_address_1 =
{
    OGS_TLV_VAR_STR,
    "IP Address",
    OGS_TLV_IP_ADDRESS_TYPE,
    0,
    1,
    sizeof(ogs_tlv_ip_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ip_address_2 =
{
    OGS_TLV_VAR_STR,
    "IP Address",
    OGS_TLV_IP_ADDRESS_TYPE,
    0,
    2,
    sizeof(ogs_tlv_ip_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ip_address_3 =
{
    OGS_TLV_VAR_STR,
    "IP Address",
    OGS_TLV_IP_ADDRESS_TYPE,
    0,
    3,
    sizeof(ogs_tlv_ip_address_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_mei_0 =
{
    OGS_TLV_VAR_STR,
    "MEI",
    OGS_TLV_MEI_TYPE,
    0,
    0,
    sizeof(ogs_tlv_mei_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_msisdn_0 =
{
    OGS_TLV_VAR_STR,
    "MSISDN",
    OGS_TLV_MSISDN_TYPE,
    0,
    0,
    sizeof(ogs_tlv_msisdn_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_indication_0 =
{
    OGS_TLV_VAR_STR,
    "Indication",
    OGS_TLV_INDICATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_indication_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_pco_0 =
{
    OGS_TLV_VAR_STR,
    "PCO",
    OGS_TLV_PCO_TYPE,
    0,
    0,
    sizeof(ogs_tlv_pco_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_paa_0 =
{
    OGS_TLV_VAR_STR,
    "PAA",
    OGS_TLV_PAA_TYPE,
    0,
    0,
    sizeof(ogs_tlv_paa_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_qos_0 =
{
    OGS_TLV_VAR_STR,
    "Bearer QoS",
    OGS_TLV_BEARER_QOS_TYPE,
    0,
    0,
    sizeof(ogs_tlv_bearer_qos_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_flow_qos_0 =
{
    OGS_TLV_VAR_STR,
    "Flow QoS",
    OGS_TLV_FLOW_QOS_TYPE,
    0,
    0,
    sizeof(ogs_tlv_flow_qos_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_rat_type_0 =
{
    OGS_TLV_UINT8,
    "RAT Type",
    OGS_TLV_RAT_TYPE_TYPE,
    1,
    0,
    sizeof(ogs_tlv_rat_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_serving_network_0 =
{
    OGS_TLV_VAR_STR,
    "Serving Network",
    OGS_TLV_SERVING_NETWORK_TYPE,
    0,
    0,
    sizeof(ogs_tlv_serving_network_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_tft_0 =
{
    OGS_TLV_VAR_STR,
    "Bearer TFT",
    OGS_TLV_BEARER_TFT_TYPE,
    0,
    0,
    sizeof(ogs_tlv_bearer_tft_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_tad_0 =
{
    OGS_TLV_VAR_STR,
    "TAD",
    OGS_TLV_TAD_TYPE,
    0,
    0,
    sizeof(ogs_tlv_tad_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_uli_0 =
{
    OGS_TLV_VAR_STR,
    "ULI",
    OGS_TLV_ULI_TYPE,
    0,
    0,
    sizeof(ogs_tlv_uli_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_teid_0 =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_TLV_F_TEID_TYPE,
    0,
    0,
    sizeof(ogs_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_teid_1 =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_TLV_F_TEID_TYPE,
    0,
    1,
    sizeof(ogs_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_teid_2 =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_TLV_F_TEID_TYPE,
    0,
    2,
    sizeof(ogs_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_teid_3 =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_TLV_F_TEID_TYPE,
    0,
    3,
    sizeof(ogs_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_teid_4 =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_TLV_F_TEID_TYPE,
    0,
    4,
    sizeof(ogs_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_teid_5 =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_TLV_F_TEID_TYPE,
    0,
    5,
    sizeof(ogs_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_teid_6 =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_TLV_F_TEID_TYPE,
    0,
    6,
    sizeof(ogs_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_teid_7 =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_TLV_F_TEID_TYPE,
    0,
    7,
    sizeof(ogs_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_teid_8 =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_TLV_F_TEID_TYPE,
    0,
    8,
    sizeof(ogs_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_teid_9 =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_TLV_F_TEID_TYPE,
    0,
    9,
    sizeof(ogs_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_teid_10 =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_TLV_F_TEID_TYPE,
    0,
    10,
    sizeof(ogs_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_teid_11 =
{
    OGS_TLV_VAR_STR,
    "F-TEID",
    OGS_TLV_F_TEID_TYPE,
    0,
    11,
    sizeof(ogs_tlv_f_teid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_tmsi_0 =
{
    OGS_TLV_VAR_STR,
    "TMSI",
    OGS_TLV_TMSI_TYPE,
    0,
    0,
    sizeof(ogs_tlv_tmsi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_global_cn_id_0 =
{
    OGS_TLV_VAR_STR,
    "Global CN-Id",
    OGS_TLV_GLOBAL_CN_ID_TYPE,
    0,
    0,
    sizeof(ogs_tlv_global_cn_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_s103pdf_0 =
{
    OGS_TLV_VAR_STR,
    "S103PDF",
    OGS_TLV_S103PDF_TYPE,
    0,
    0,
    sizeof(ogs_tlv_s103pdf_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_s1udf_0 =
{
    OGS_TLV_VAR_STR,
    "S1UDF",
    OGS_TLV_S1UDF_TYPE,
    0,
    0,
    sizeof(ogs_tlv_s1udf_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_delay_value_0 =
{
    OGS_TLV_VAR_STR,
    "Delay Value",
    OGS_TLV_DELAY_VALUE_TYPE,
    0,
    0,
    sizeof(ogs_tlv_delay_value_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_charging_id_0 =
{
    OGS_TLV_VAR_STR,
    "Charging ID",
    OGS_TLV_CHARGING_ID_TYPE,
    0,
    0,
    sizeof(ogs_tlv_charging_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_charging_characteristics_0 =
{
    OGS_TLV_VAR_STR,
    "Charging Characteristics",
    OGS_TLV_CHARGING_CHARACTERISTICS_TYPE,
    0,
    0,
    sizeof(ogs_tlv_charging_characteristics_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_trace_information_0 =
{
    OGS_TLV_VAR_STR,
    "Trace Information",
    OGS_TLV_TRACE_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_trace_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_flags_0 =
{
    OGS_TLV_VAR_STR,
    "Bearer Flags",
    OGS_TLV_BEARER_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_tlv_bearer_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_pdn_type_0 =
{
    OGS_TLV_UINT8,
    "PDN Type",
    OGS_TLV_PDN_TYPE_TYPE,
    1,
    0,
    sizeof(ogs_tlv_pdn_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_pti_0 =
{
    OGS_TLV_VAR_STR,
    "PTI",
    OGS_TLV_PTI_TYPE,
    0,
    0,
    sizeof(ogs_tlv_pti_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_mm_context_0 =
{
    OGS_TLV_VAR_STR,
    "MM Context",
    OGS_TLV_MM_CONTEXT_TYPE,
    0,
    0,
    sizeof(ogs_tlv_mm_context_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_pdu_numbers_0 =
{
    OGS_TLV_VAR_STR,
    "PDU Numbers",
    OGS_TLV_PDU_NUMBERS_TYPE,
    0,
    0,
    sizeof(ogs_tlv_pdu_numbers_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_p_tmsi_0 =
{
    OGS_TLV_VAR_STR,
    "P-TMSI",
    OGS_TLV_P_TMSI_TYPE,
    0,
    0,
    sizeof(ogs_tlv_p_tmsi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_p_tmsi_signature_0 =
{
    OGS_TLV_VAR_STR,
    "P-TMSI Signature",
    OGS_TLV_P_TMSI_SIGNATURE_TYPE,
    0,
    0,
    sizeof(ogs_tlv_p_tmsi_signature_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_hop_counter_0 =
{
    OGS_TLV_VAR_STR,
    "Hop Counter",
    OGS_TLV_HOP_COUNTER_TYPE,
    0,
    0,
    sizeof(ogs_tlv_hop_counter_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ue_time_zone_0 =
{
    OGS_TLV_VAR_STR,
    "UE Time Zone",
    OGS_TLV_UE_TIME_ZONE_TYPE,
    0,
    0,
    sizeof(ogs_tlv_ue_time_zone_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_trace_reference_0 =
{
    OGS_TLV_VAR_STR,
    "Trace Reference",
    OGS_TLV_TRACE_REFERENCE_TYPE,
    0,
    0,
    sizeof(ogs_tlv_trace_reference_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_complete_request_message_0 =
{
    OGS_TLV_VAR_STR,
    "Complete Request Message",
    OGS_TLV_COMPLETE_REQUEST_MESSAGE_TYPE,
    0,
    0,
    sizeof(ogs_tlv_complete_request_message_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_guti_0 =
{
    OGS_TLV_VAR_STR,
    "GUTI",
    OGS_TLV_GUTI_TYPE,
    0,
    0,
    sizeof(ogs_tlv_guti_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_container_0 =
{
    OGS_TLV_VAR_STR,
    "F-Container",
    OGS_TLV_F_CONTAINER_TYPE,
    0,
    0,
    sizeof(ogs_tlv_f_container_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_f_cause_0 =
{
    OGS_TLV_VAR_STR,
    "F-Cause",
    OGS_TLV_F_CAUSE_TYPE,
    0,
    0,
    sizeof(ogs_tlv_f_cause_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_plmn_id_0 =
{
    OGS_TLV_VAR_STR,
    "PLMN ID",
    OGS_TLV_PLMN_ID_TYPE,
    0,
    0,
    sizeof(ogs_tlv_plmn_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_target_identification_0 =
{
    OGS_TLV_VAR_STR,
    "Target Identification",
    OGS_TLV_TARGET_IDENTIFICATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_target_identification_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_packet_flow_id_0 =
{
    OGS_TLV_VAR_STR,
    "Packet Flow ID",
    OGS_TLV_PACKET_FLOW_ID_TYPE,
    0,
    0,
    sizeof(ogs_tlv_packet_flow_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_rab_context_0 =
{
    OGS_TLV_VAR_STR,
    "RAB Context",
    OGS_TLV_RAB_CONTEXT_TYPE,
    0,
    0,
    sizeof(ogs_tlv_rab_context_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_source_rnc_pdcp_context_info_0 =
{
    OGS_TLV_VAR_STR,
    "Source RNC PDCP Context Info",
    OGS_TLV_SOURCE_RNC_PDCP_CONTEXT_INFO_TYPE,
    0,
    0,
    sizeof(ogs_tlv_source_rnc_pdcp_context_info_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_port_number_0 =
{
    OGS_TLV_UINT16,
    "Port Number",
    OGS_TLV_PORT_NUMBER_TYPE,
    2,
    0,
    sizeof(ogs_tlv_port_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_port_number_1 =
{
    OGS_TLV_UINT16,
    "Port Number",
    OGS_TLV_PORT_NUMBER_TYPE,
    2,
    1,
    sizeof(ogs_tlv_port_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_port_number_2 =
{
    OGS_TLV_UINT16,
    "Port Number",
    OGS_TLV_PORT_NUMBER_TYPE,
    2,
    2,
    sizeof(ogs_tlv_port_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_apn_restriction_0 =
{
    OGS_TLV_UINT8,
    "APN Restriction",
    OGS_TLV_APN_RESTRICTION_TYPE,
    1,
    0,
    sizeof(ogs_tlv_apn_restriction_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_selection_mode_0 =
{
    OGS_TLV_UINT8,
    "Selection Mode",
    OGS_TLV_SELECTION_MODE_TYPE,
    1,
    0,
    sizeof(ogs_tlv_selection_mode_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_source_identification_0 =
{
    OGS_TLV_VAR_STR,
    "Source Identification",
    OGS_TLV_SOURCE_IDENTIFICATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_source_identification_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_change_reporting_action_0 =
{
    OGS_TLV_VAR_STR,
    "Change Reporting Action",
    OGS_TLV_CHANGE_REPORTING_ACTION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_change_reporting_action_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_fq_csid_0 =
{
    OGS_TLV_VAR_STR,
    "FQ-CSID",
    OGS_TLV_FQ_CSID_TYPE,
    0,
    0,
    sizeof(ogs_tlv_fq_csid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_fq_csid_1 =
{
    OGS_TLV_VAR_STR,
    "FQ-CSID",
    OGS_TLV_FQ_CSID_TYPE,
    0,
    1,
    sizeof(ogs_tlv_fq_csid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_fq_csid_2 =
{
    OGS_TLV_VAR_STR,
    "FQ-CSID",
    OGS_TLV_FQ_CSID_TYPE,
    0,
    2,
    sizeof(ogs_tlv_fq_csid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_fq_csid_3 =
{
    OGS_TLV_VAR_STR,
    "FQ-CSID",
    OGS_TLV_FQ_CSID_TYPE,
    0,
    3,
    sizeof(ogs_tlv_fq_csid_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_channel_needed_0 =
{
    OGS_TLV_VAR_STR,
    "Channel needed",
    OGS_TLV_CHANNEL_NEEDED_TYPE,
    0,
    0,
    sizeof(ogs_tlv_channel_needed_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_emlpp_priority_0 =
{
    OGS_TLV_VAR_STR,
    "eMLPP Priority",
    OGS_TLV_EMLPP_PRIORITY_TYPE,
    0,
    0,
    sizeof(ogs_tlv_emlpp_priority_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_node_type_0 =
{
    OGS_TLV_UINT8,
    "Node Type",
    OGS_TLV_NODE_TYPE_TYPE,
    1,
    0,
    sizeof(ogs_tlv_node_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_fqdn_0 =
{
    OGS_TLV_VAR_STR,
    "FQDN",
    OGS_TLV_FQDN_TYPE,
    0,
    0,
    sizeof(ogs_tlv_fqdn_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ti_0 =
{
    OGS_TLV_VAR_STR,
    "TI",
    OGS_TLV_TI_TYPE,
    0,
    0,
    sizeof(ogs_tlv_ti_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_mbms_session_duration_0 =
{
    OGS_TLV_VAR_STR,
    "MBMS Session Duration",
    OGS_TLV_MBMS_SESSION_DURATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_mbms_session_duration_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_mbms_service_area_0 =
{
    OGS_TLV_VAR_STR,
    "MBMS Service Area",
    OGS_TLV_MBMS_SERVICE_AREA_TYPE,
    0,
    0,
    sizeof(ogs_tlv_mbms_service_area_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_mbms_session_identifier_0 =
{
    OGS_TLV_VAR_STR,
    "MBMS Session Identifier",
    OGS_TLV_MBMS_SESSION_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(ogs_tlv_mbms_session_identifier_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_mbms_flow_identifier_0 =
{
    OGS_TLV_VAR_STR,
    "MBMS Flow Identifier",
    OGS_TLV_MBMS_FLOW_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(ogs_tlv_mbms_flow_identifier_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_mbms_ip_multicast_distribution_0 =
{
    OGS_TLV_VAR_STR,
    "MBMS IP Multicast Distribution",
    OGS_TLV_MBMS_IP_MULTICAST_DISTRIBUTION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_mbms_ip_multicast_distribution_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_mbms_distribution_acknowledge_0 =
{
    OGS_TLV_VAR_STR,
    "MBMS Distribution Acknowledge",
    OGS_TLV_MBMS_DISTRIBUTION_ACKNOWLEDGE_TYPE,
    0,
    0,
    sizeof(ogs_tlv_mbms_distribution_acknowledge_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_rfsp_index_0 =
{
    OGS_TLV_VAR_STR,
    "RFSP Index",
    OGS_TLV_RFSP_INDEX_TYPE,
    0,
    0,
    sizeof(ogs_tlv_rfsp_index_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_uci_0 =
{
    OGS_TLV_VAR_STR,
    "UCI",
    OGS_TLV_UCI_TYPE,
    0,
    0,
    sizeof(ogs_tlv_uci_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_csg_information_reporting_action_0 =
{
    OGS_TLV_VAR_STR,
    "CSG Information Reporting Action",
    OGS_TLV_CSG_INFORMATION_REPORTING_ACTION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_csg_information_reporting_action_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_csg_id_0 =
{
    OGS_TLV_VAR_STR,
    "CSG ID",
    OGS_TLV_CSG_ID_TYPE,
    0,
    0,
    sizeof(ogs_tlv_csg_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_cmi_0 =
{
    OGS_TLV_VAR_STR,
    "CMI",
    OGS_TLV_CMI_TYPE,
    0,
    0,
    sizeof(ogs_tlv_cmi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_service_indicator_0 =
{
    OGS_TLV_VAR_STR,
    "Service indicator",
    OGS_TLV_SERVICE_INDICATOR_TYPE,
    0,
    0,
    sizeof(ogs_tlv_service_indicator_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_detach_type_0 =
{
    OGS_TLV_VAR_STR,
    "Detach Type",
    OGS_TLV_DETACH_TYPE_TYPE,
    0,
    0,
    sizeof(ogs_tlv_detach_type_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ldn_0 =
{
    OGS_TLV_VAR_STR,
    "LDN",
    OGS_TLV_LDN_TYPE,
    0,
    0,
    sizeof(ogs_tlv_ldn_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ldn_1 =
{
    OGS_TLV_VAR_STR,
    "LDN",
    OGS_TLV_LDN_TYPE,
    0,
    1,
    sizeof(ogs_tlv_ldn_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ldn_2 =
{
    OGS_TLV_VAR_STR,
    "LDN",
    OGS_TLV_LDN_TYPE,
    0,
    2,
    sizeof(ogs_tlv_ldn_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ldn_3 =
{
    OGS_TLV_VAR_STR,
    "LDN",
    OGS_TLV_LDN_TYPE,
    0,
    3,
    sizeof(ogs_tlv_ldn_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_node_features_0 =
{
    OGS_TLV_VAR_STR,
    "Node Features",
    OGS_TLV_NODE_FEATURES_TYPE,
    0,
    0,
    sizeof(ogs_tlv_node_features_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_mbms_time_to_data_transfer_0 =
{
    OGS_TLV_VAR_STR,
    "MBMS Time to Data Transfer",
    OGS_TLV_MBMS_TIME_TO_DATA_TRANSFER_TYPE,
    0,
    0,
    sizeof(ogs_tlv_mbms_time_to_data_transfer_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_throttling_0 =
{
    OGS_TLV_VAR_STR,
    "Throttling",
    OGS_TLV_THROTTLING_TYPE,
    0,
    0,
    sizeof(ogs_tlv_throttling_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_arp_0 =
{
    OGS_TLV_VAR_STR,
    "ARP",
    OGS_TLV_ARP_TYPE,
    0,
    0,
    sizeof(ogs_tlv_arp_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_epc_timer_0 =
{
    OGS_TLV_VAR_STR,
    "EPC Timer",
    OGS_TLV_EPC_TIMER_TYPE,
    0,
    0,
    sizeof(ogs_tlv_epc_timer_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_signalling_priority_indication_0 =
{
    OGS_TLV_VAR_STR,
    "Signalling Priority Indication",
    OGS_TLV_SIGNALLING_PRIORITY_INDICATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_signalling_priority_indication_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_tmgi_0 =
{
    OGS_TLV_VAR_STR,
    "TMGI",
    OGS_TLV_TMGI_TYPE,
    0,
    0,
    sizeof(ogs_tlv_tmgi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_additional_mm_context_for_srvcc_0 =
{
    OGS_TLV_VAR_STR,
    "Additional MM context for SRVCC",
    OGS_TLV_ADDITIONAL_MM_CONTEXT_FOR_SRVCC_TYPE,
    0,
    0,
    sizeof(ogs_tlv_additional_mm_context_for_srvcc_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_additional_flags_for_srvcc_0 =
{
    OGS_TLV_VAR_STR,
    "Additional flags for SRVCC",
    OGS_TLV_ADDITIONAL_FLAGS_FOR_SRVCC_TYPE,
    0,
    0,
    sizeof(ogs_tlv_additional_flags_for_srvcc_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_mdt_configuration_0 =
{
    OGS_TLV_VAR_STR,
    "MDT Configuration",
    OGS_TLV_MDT_CONFIGURATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_mdt_configuration_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_apco_0 =
{
    OGS_TLV_VAR_STR,
    "APCO",
    OGS_TLV_APCO_TYPE,
    0,
    0,
    sizeof(ogs_tlv_apco_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_absolute_time_of_mbms_data_transfer_0 =
{
    OGS_TLV_VAR_STR,
    "Absolute Time of MBMS Data Transfer",
    OGS_TLV_ABSOLUTE_TIME_OF_MBMS_DATA_TRANSFER_TYPE,
    0,
    0,
    sizeof(ogs_tlv_absolute_time_of_mbms_data_transfer_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_enb_information_reporting_0 =
{
    OGS_TLV_VAR_STR,
    "eNB Information Reporting",
    OGS_TLV_ENB_INFORMATION_REPORTING_TYPE,
    0,
    0,
    sizeof(ogs_tlv_enb_information_reporting_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ip4cp_0 =
{
    OGS_TLV_VAR_STR,
    "IP4CP",
    OGS_TLV_IP4CP_TYPE,
    0,
    0,
    sizeof(ogs_tlv_ip4cp_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_change_to_report_flags_0 =
{
    OGS_TLV_VAR_STR,
    "Change to Report Flags",
    OGS_TLV_CHANGE_TO_REPORT_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_tlv_change_to_report_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_action_indication_0 =
{
    OGS_TLV_VAR_STR,
    "Action Indication",
    OGS_TLV_ACTION_INDICATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_action_indication_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_twan_identifier_0 =
{
    OGS_TLV_VAR_STR,
    "TWAN Identifier",
    OGS_TLV_TWAN_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(ogs_tlv_twan_identifier_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_twan_identifier_1 =
{
    OGS_TLV_VAR_STR,
    "TWAN Identifier",
    OGS_TLV_TWAN_IDENTIFIER_TYPE,
    0,
    1,
    sizeof(ogs_tlv_twan_identifier_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_uli_timestamp_0 =
{
    OGS_TLV_VAR_STR,
    "ULI Timestamp",
    OGS_TLV_ULI_TIMESTAMP_TYPE,
    0,
    0,
    sizeof(ogs_tlv_uli_timestamp_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_mbms_flags_0 =
{
    OGS_TLV_VAR_STR,
    "MBMS Flags",
    OGS_TLV_MBMS_FLAGS_TYPE,
    0,
    0,
    sizeof(ogs_tlv_mbms_flags_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ran_nas_cause_0 =
{
    OGS_TLV_VAR_STR,
    "RAN/NAS Cause",
    OGS_TLV_RAN_NAS_CAUSE_TYPE,
    0,
    0,
    sizeof(ogs_tlv_ran_nas_cause_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_cn_operator_selection_entity_0 =
{
    OGS_TLV_VAR_STR,
    "CN Operator Selection Entity",
    OGS_TLV_CN_OPERATOR_SELECTION_ENTITY_TYPE,
    0,
    0,
    sizeof(ogs_tlv_cn_operator_selection_entity_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_twmi_0 =
{
    OGS_TLV_VAR_STR,
    "TWMI",
    OGS_TLV_TWMI_TYPE,
    0,
    0,
    sizeof(ogs_tlv_twmi_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_node_number_0 =
{
    OGS_TLV_VAR_STR,
    "Node Number",
    OGS_TLV_NODE_NUMBER_TYPE,
    0,
    0,
    sizeof(ogs_tlv_node_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_node_identifier_0 =
{
    OGS_TLV_VAR_STR,
    "Node Identifier",
    OGS_TLV_NODE_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(ogs_tlv_node_identifier_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_presence_reporting_area_action_0 =
{
    OGS_TLV_VAR_STR,
    "Presence Reporting Area Action",
    OGS_TLV_PRESENCE_REPORTING_AREA_ACTION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_presence_reporting_area_action_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_presence_reporting_area_information_0 =
{
    OGS_TLV_VAR_STR,
    "Presence Reporting Area Information",
    OGS_TLV_PRESENCE_REPORTING_AREA_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_presence_reporting_area_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_twan_identifier_timestamp_0 =
{
    OGS_TLV_VAR_STR,
    "TWAN Identifier Timestamp",
    OGS_TLV_TWAN_IDENTIFIER_TIMESTAMP_TYPE,
    0,
    0,
    sizeof(ogs_tlv_twan_identifier_timestamp_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_twan_identifier_timestamp_1 =
{
    OGS_TLV_VAR_STR,
    "TWAN Identifier Timestamp",
    OGS_TLV_TWAN_IDENTIFIER_TIMESTAMP_TYPE,
    0,
    1,
    sizeof(ogs_tlv_twan_identifier_timestamp_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_metric_0 =
{
    OGS_TLV_VAR_STR,
    "Metric",
    OGS_TLV_METRIC_TYPE,
    0,
    0,
    sizeof(ogs_tlv_metric_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_sequence_number_0 =
{
    OGS_TLV_VAR_STR,
    "Sequence Number",
    OGS_TLV_SEQUENCE_NUMBER_TYPE,
    0,
    0,
    sizeof(ogs_tlv_sequence_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_apn_and_relative_capacity_0 =
{
    OGS_TLV_VAR_STR,
    "APN and Relative Capacity",
    OGS_TLV_APN_AND_RELATIVE_CAPACITY_TYPE,
    0,
    0,
    sizeof(ogs_tlv_apn_and_relative_capacity_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_wlan_offloadability_indication_0 =
{
    OGS_TLV_VAR_STR,
    "WLAN Offloadability Indication",
    OGS_TLV_WLAN_OFFLOADABILITY_INDICATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_wlan_offloadability_indication_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_paging_and_service_information_0 =
{
    OGS_TLV_VAR_STR,
    "Paging and Service Information",
    OGS_TLV_PAGING_AND_SERVICE_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_paging_and_service_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_integer_number_0 =
{
    OGS_TLV_VAR_STR,
    "Integer Number",
    OGS_TLV_INTEGER_NUMBER_TYPE,
    0,
    0,
    sizeof(ogs_tlv_integer_number_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_millisecond_time_stamp_0 =
{
    OGS_TLV_VAR_STR,
    "Millisecond Time Stamp",
    OGS_TLV_MILLISECOND_TIME_STAMP_TYPE,
    0,
    0,
    sizeof(ogs_tlv_millisecond_time_stamp_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_monitoring_event_information_0 =
{
    OGS_TLV_VAR_STR,
    "Monitoring Event Information",
    OGS_TLV_MONITORING_EVENT_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_monitoring_event_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ecgi_list_0 =
{
    OGS_TLV_VAR_STR,
    "ECGI List",
    OGS_TLV_ECGI_LIST_TYPE,
    0,
    0,
    sizeof(ogs_tlv_ecgi_list_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_remote_user_id_0 =
{
    OGS_TLV_VAR_STR,
    "Remote User ID",
    OGS_TLV_REMOTE_USER_ID_TYPE,
    0,
    0,
    sizeof(ogs_tlv_remote_user_id_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_remote_ue_ip_information_0 =
{
    OGS_TLV_VAR_STR,
    "Remote UE IP Information",
    OGS_TLV_REMOTE_UE_IP_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_remote_ue_ip_information_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_ciot_optimizations_support_indication_0 =
{
    OGS_TLV_VAR_STR,
    "CIoT Optimizations Support Indication",
    OGS_TLV_CIOT_OPTIMIZATIONS_SUPPORT_INDICATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_ciot_optimizations_support_indication_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_header_compression_configuration_0 =
{
    OGS_TLV_VAR_STR,
    "Header Compression Configuration",
    OGS_TLV_HEADER_COMPRESSION_CONFIGURATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_header_compression_configuration_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_epco_0 =
{
    OGS_TLV_VAR_STR,
    "ePCO",
    OGS_TLV_EPCO_TYPE,
    0,
    0,
    sizeof(ogs_tlv_epco_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_serving_plmn_rate_control_0 =
{
    OGS_TLV_VAR_STR,
    "Serving PLMN Rate Control",
    OGS_TLV_SERVING_PLMN_RATE_CONTROL_TYPE,
    0,
    0,
    sizeof(ogs_tlv_serving_plmn_rate_control_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_counter_0 =
{
    OGS_TLV_VAR_STR,
    "Counter",
    OGS_TLV_COUNTER_TYPE,
    0,
    0,
    sizeof(ogs_tlv_counter_t),
    { NULL }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_context_0 =
{
    OGS_TLV_COMPOUND,
    "Bearer Context",
    OGS_TLV_BEARER_CONTEXT_TYPE,
    0,
    0,
    sizeof(ogs_tlv_bearer_context_t),
    {
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_bearer_tft_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_f_teid_1,
        &ogs_tlv_desc_f_teid_2,
        &ogs_tlv_desc_f_teid_3,
        &ogs_tlv_desc_f_teid_4,
        &ogs_tlv_desc_f_teid_5,
        &ogs_tlv_desc_f_teid_6,
        &ogs_tlv_desc_bearer_qos_0,
        &ogs_tlv_desc_f_teid_7,
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_charging_id_0,
        &ogs_tlv_desc_bearer_flags_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_epco_0,
        &ogs_tlv_desc_f_teid_8,
        &ogs_tlv_desc_f_teid_9,
        &ogs_tlv_desc_f_teid_10,
        &ogs_tlv_desc_f_teid_11,
        &ogs_tlv_desc_ran_nas_cause_0,
        &ogs_tlv_desc_apco_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_ti_0,
        &ogs_tlv_desc_packet_flow_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_context_1 =
{
    OGS_TLV_COMPOUND,
    "Bearer Context",
    OGS_TLV_BEARER_CONTEXT_TYPE,
    0,
    1,
    sizeof(ogs_tlv_bearer_context_t),
    {
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_bearer_tft_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_f_teid_1,
        &ogs_tlv_desc_f_teid_2,
        &ogs_tlv_desc_f_teid_3,
        &ogs_tlv_desc_f_teid_4,
        &ogs_tlv_desc_f_teid_5,
        &ogs_tlv_desc_f_teid_6,
        &ogs_tlv_desc_bearer_qos_0,
        &ogs_tlv_desc_f_teid_7,
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_charging_id_0,
        &ogs_tlv_desc_bearer_flags_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_epco_0,
        &ogs_tlv_desc_f_teid_8,
        &ogs_tlv_desc_f_teid_9,
        &ogs_tlv_desc_f_teid_10,
        &ogs_tlv_desc_f_teid_11,
        &ogs_tlv_desc_ran_nas_cause_0,
        &ogs_tlv_desc_apco_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_ti_0,
        &ogs_tlv_desc_packet_flow_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_context_2 =
{
    OGS_TLV_COMPOUND,
    "Bearer Context",
    OGS_TLV_BEARER_CONTEXT_TYPE,
    0,
    2,
    sizeof(ogs_tlv_bearer_context_t),
    {
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_bearer_tft_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_f_teid_1,
        &ogs_tlv_desc_f_teid_2,
        &ogs_tlv_desc_f_teid_3,
        &ogs_tlv_desc_f_teid_4,
        &ogs_tlv_desc_f_teid_5,
        &ogs_tlv_desc_f_teid_6,
        &ogs_tlv_desc_bearer_qos_0,
        &ogs_tlv_desc_f_teid_7,
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_charging_id_0,
        &ogs_tlv_desc_bearer_flags_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_epco_0,
        &ogs_tlv_desc_f_teid_8,
        &ogs_tlv_desc_f_teid_9,
        &ogs_tlv_desc_f_teid_10,
        &ogs_tlv_desc_f_teid_11,
        &ogs_tlv_desc_ran_nas_cause_0,
        &ogs_tlv_desc_apco_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_ti_0,
        &ogs_tlv_desc_packet_flow_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_context_3 =
{
    OGS_TLV_COMPOUND,
    "Bearer Context",
    OGS_TLV_BEARER_CONTEXT_TYPE,
    0,
    3,
    sizeof(ogs_tlv_bearer_context_t),
    {
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_bearer_tft_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_f_teid_1,
        &ogs_tlv_desc_f_teid_2,
        &ogs_tlv_desc_f_teid_3,
        &ogs_tlv_desc_f_teid_4,
        &ogs_tlv_desc_f_teid_5,
        &ogs_tlv_desc_f_teid_6,
        &ogs_tlv_desc_bearer_qos_0,
        &ogs_tlv_desc_f_teid_7,
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_charging_id_0,
        &ogs_tlv_desc_bearer_flags_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_epco_0,
        &ogs_tlv_desc_f_teid_8,
        &ogs_tlv_desc_f_teid_9,
        &ogs_tlv_desc_f_teid_10,
        &ogs_tlv_desc_f_teid_11,
        &ogs_tlv_desc_ran_nas_cause_0,
        &ogs_tlv_desc_apco_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_ti_0,
        &ogs_tlv_desc_packet_flow_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_context_4 =
{
    OGS_TLV_COMPOUND,
    "Bearer Context",
    OGS_TLV_BEARER_CONTEXT_TYPE,
    0,
    4,
    sizeof(ogs_tlv_bearer_context_t),
    {
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_bearer_tft_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_f_teid_1,
        &ogs_tlv_desc_f_teid_2,
        &ogs_tlv_desc_f_teid_3,
        &ogs_tlv_desc_f_teid_4,
        &ogs_tlv_desc_f_teid_5,
        &ogs_tlv_desc_f_teid_6,
        &ogs_tlv_desc_bearer_qos_0,
        &ogs_tlv_desc_f_teid_7,
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_charging_id_0,
        &ogs_tlv_desc_bearer_flags_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_epco_0,
        &ogs_tlv_desc_f_teid_8,
        &ogs_tlv_desc_f_teid_9,
        &ogs_tlv_desc_f_teid_10,
        &ogs_tlv_desc_f_teid_11,
        &ogs_tlv_desc_ran_nas_cause_0,
        &ogs_tlv_desc_apco_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_ti_0,
        &ogs_tlv_desc_packet_flow_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_context_5 =
{
    OGS_TLV_COMPOUND,
    "Bearer Context",
    OGS_TLV_BEARER_CONTEXT_TYPE,
    0,
    5,
    sizeof(ogs_tlv_bearer_context_t),
    {
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_bearer_tft_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_f_teid_1,
        &ogs_tlv_desc_f_teid_2,
        &ogs_tlv_desc_f_teid_3,
        &ogs_tlv_desc_f_teid_4,
        &ogs_tlv_desc_f_teid_5,
        &ogs_tlv_desc_f_teid_6,
        &ogs_tlv_desc_bearer_qos_0,
        &ogs_tlv_desc_f_teid_7,
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_charging_id_0,
        &ogs_tlv_desc_bearer_flags_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_epco_0,
        &ogs_tlv_desc_f_teid_8,
        &ogs_tlv_desc_f_teid_9,
        &ogs_tlv_desc_f_teid_10,
        &ogs_tlv_desc_f_teid_11,
        &ogs_tlv_desc_ran_nas_cause_0,
        &ogs_tlv_desc_apco_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_ti_0,
        &ogs_tlv_desc_packet_flow_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_context_6 =
{
    OGS_TLV_COMPOUND,
    "Bearer Context",
    OGS_TLV_BEARER_CONTEXT_TYPE,
    0,
    6,
    sizeof(ogs_tlv_bearer_context_t),
    {
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_bearer_tft_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_f_teid_1,
        &ogs_tlv_desc_f_teid_2,
        &ogs_tlv_desc_f_teid_3,
        &ogs_tlv_desc_f_teid_4,
        &ogs_tlv_desc_f_teid_5,
        &ogs_tlv_desc_f_teid_6,
        &ogs_tlv_desc_bearer_qos_0,
        &ogs_tlv_desc_f_teid_7,
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_charging_id_0,
        &ogs_tlv_desc_bearer_flags_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_epco_0,
        &ogs_tlv_desc_f_teid_8,
        &ogs_tlv_desc_f_teid_9,
        &ogs_tlv_desc_f_teid_10,
        &ogs_tlv_desc_f_teid_11,
        &ogs_tlv_desc_ran_nas_cause_0,
        &ogs_tlv_desc_apco_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_ti_0,
        &ogs_tlv_desc_packet_flow_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_context_7 =
{
    OGS_TLV_COMPOUND,
    "Bearer Context",
    OGS_TLV_BEARER_CONTEXT_TYPE,
    0,
    7,
    sizeof(ogs_tlv_bearer_context_t),
    {
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_bearer_tft_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_f_teid_1,
        &ogs_tlv_desc_f_teid_2,
        &ogs_tlv_desc_f_teid_3,
        &ogs_tlv_desc_f_teid_4,
        &ogs_tlv_desc_f_teid_5,
        &ogs_tlv_desc_f_teid_6,
        &ogs_tlv_desc_bearer_qos_0,
        &ogs_tlv_desc_f_teid_7,
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_charging_id_0,
        &ogs_tlv_desc_bearer_flags_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_epco_0,
        &ogs_tlv_desc_f_teid_8,
        &ogs_tlv_desc_f_teid_9,
        &ogs_tlv_desc_f_teid_10,
        &ogs_tlv_desc_f_teid_11,
        &ogs_tlv_desc_ran_nas_cause_0,
        &ogs_tlv_desc_apco_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_ti_0,
        &ogs_tlv_desc_packet_flow_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_context_8 =
{
    OGS_TLV_COMPOUND,
    "Bearer Context",
    OGS_TLV_BEARER_CONTEXT_TYPE,
    0,
    8,
    sizeof(ogs_tlv_bearer_context_t),
    {
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_bearer_tft_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_f_teid_1,
        &ogs_tlv_desc_f_teid_2,
        &ogs_tlv_desc_f_teid_3,
        &ogs_tlv_desc_f_teid_4,
        &ogs_tlv_desc_f_teid_5,
        &ogs_tlv_desc_f_teid_6,
        &ogs_tlv_desc_bearer_qos_0,
        &ogs_tlv_desc_f_teid_7,
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_charging_id_0,
        &ogs_tlv_desc_bearer_flags_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_epco_0,
        &ogs_tlv_desc_f_teid_8,
        &ogs_tlv_desc_f_teid_9,
        &ogs_tlv_desc_f_teid_10,
        &ogs_tlv_desc_f_teid_11,
        &ogs_tlv_desc_ran_nas_cause_0,
        &ogs_tlv_desc_apco_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_ti_0,
        &ogs_tlv_desc_packet_flow_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_context_9 =
{
    OGS_TLV_COMPOUND,
    "Bearer Context",
    OGS_TLV_BEARER_CONTEXT_TYPE,
    0,
    9,
    sizeof(ogs_tlv_bearer_context_t),
    {
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_bearer_tft_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_f_teid_1,
        &ogs_tlv_desc_f_teid_2,
        &ogs_tlv_desc_f_teid_3,
        &ogs_tlv_desc_f_teid_4,
        &ogs_tlv_desc_f_teid_5,
        &ogs_tlv_desc_f_teid_6,
        &ogs_tlv_desc_bearer_qos_0,
        &ogs_tlv_desc_f_teid_7,
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_charging_id_0,
        &ogs_tlv_desc_bearer_flags_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_epco_0,
        &ogs_tlv_desc_f_teid_8,
        &ogs_tlv_desc_f_teid_9,
        &ogs_tlv_desc_f_teid_10,
        &ogs_tlv_desc_f_teid_11,
        &ogs_tlv_desc_ran_nas_cause_0,
        &ogs_tlv_desc_apco_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_ti_0,
        &ogs_tlv_desc_packet_flow_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_bearer_context_10 =
{
    OGS_TLV_COMPOUND,
    "Bearer Context",
    OGS_TLV_BEARER_CONTEXT_TYPE,
    0,
    10,
    sizeof(ogs_tlv_bearer_context_t),
    {
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_bearer_tft_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_f_teid_1,
        &ogs_tlv_desc_f_teid_2,
        &ogs_tlv_desc_f_teid_3,
        &ogs_tlv_desc_f_teid_4,
        &ogs_tlv_desc_f_teid_5,
        &ogs_tlv_desc_f_teid_6,
        &ogs_tlv_desc_bearer_qos_0,
        &ogs_tlv_desc_f_teid_7,
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_charging_id_0,
        &ogs_tlv_desc_bearer_flags_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_epco_0,
        &ogs_tlv_desc_f_teid_8,
        &ogs_tlv_desc_f_teid_9,
        &ogs_tlv_desc_f_teid_10,
        &ogs_tlv_desc_f_teid_11,
        &ogs_tlv_desc_ran_nas_cause_0,
        &ogs_tlv_desc_apco_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_ti_0,
        &ogs_tlv_desc_packet_flow_id_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_pdn_connection_0 =
{
    OGS_TLV_COMPOUND,
    "PDN Connection",
    OGS_TLV_PDN_CONNECTION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_pdn_connection_t),
    {
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_overload_control_information_0 =
{
    OGS_TLV_COMPOUND,
    "Overload Control Information",
    OGS_TLV_OVERLOAD_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_overload_control_information_t),
    {
        &ogs_tlv_desc_sequence_number_0,
        &ogs_tlv_desc_metric_0,
        &ogs_tlv_desc_epc_timer_0,
        &ogs_tlv_desc_apn_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_overload_control_information_1 =
{
    OGS_TLV_COMPOUND,
    "Overload Control Information",
    OGS_TLV_OVERLOAD_CONTROL_INFORMATION_TYPE,
    0,
    1,
    sizeof(ogs_tlv_overload_control_information_t),
    {
        &ogs_tlv_desc_sequence_number_0,
        &ogs_tlv_desc_metric_0,
        &ogs_tlv_desc_epc_timer_0,
        &ogs_tlv_desc_apn_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_overload_control_information_2 =
{
    OGS_TLV_COMPOUND,
    "Overload Control Information",
    OGS_TLV_OVERLOAD_CONTROL_INFORMATION_TYPE,
    0,
    2,
    sizeof(ogs_tlv_overload_control_information_t),
    {
        &ogs_tlv_desc_sequence_number_0,
        &ogs_tlv_desc_metric_0,
        &ogs_tlv_desc_epc_timer_0,
        &ogs_tlv_desc_apn_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_load_control_information_0 =
{
    OGS_TLV_COMPOUND,
    "Load Control Information",
    OGS_TLV_LOAD_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_load_control_information_t),
    {
        &ogs_tlv_desc_sequence_number_0,
        &ogs_tlv_desc_metric_0,
        &ogs_tlv_desc_apn_and_relative_capacity_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_load_control_information_1 =
{
    OGS_TLV_COMPOUND,
    "Load Control Information",
    OGS_TLV_LOAD_CONTROL_INFORMATION_TYPE,
    0,
    1,
    sizeof(ogs_tlv_load_control_information_t),
    {
        &ogs_tlv_desc_sequence_number_0,
        &ogs_tlv_desc_metric_0,
        &ogs_tlv_desc_apn_and_relative_capacity_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_load_control_information_2 =
{
    OGS_TLV_COMPOUND,
    "Load Control Information",
    OGS_TLV_LOAD_CONTROL_INFORMATION_TYPE,
    0,
    2,
    sizeof(ogs_tlv_load_control_information_t),
    {
        &ogs_tlv_desc_sequence_number_0,
        &ogs_tlv_desc_metric_0,
        &ogs_tlv_desc_apn_and_relative_capacity_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_remote_ue_context_0 =
{
    OGS_TLV_COMPOUND,
    "Remote UE Context",
    OGS_TLV_REMOTE_UE_CONTEXT_TYPE,
    0,
    0,
    sizeof(ogs_tlv_remote_ue_context_t),
    {
        &ogs_tlv_desc_remote_user_id_0,
        &ogs_tlv_desc_remote_ue_ip_information_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_scef_pdn_connection_0 =
{
    OGS_TLV_COMPOUND,
    "SCEF PDN Connection",
    OGS_TLV_SCEF_PDN_CONNECTION_TYPE,
    0,
    0,
    sizeof(ogs_tlv_scef_pdn_connection_t),
    {
        &ogs_tlv_desc_apn_0,
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_node_identifier_0,
        NULL,
    }
};

ogs_tlv_desc_t ogs_tlv_desc_echo_request =
{
    OGS_TLV_MESSAGE,
    "Echo Request",
    0, 0, 0, 0, {
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_node_features_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_echo_response =
{
    OGS_TLV_MESSAGE,
    "Echo Response",
    0, 0, 0, 0, {
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_node_features_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_create_session_request =
{
    OGS_TLV_MESSAGE,
    "Create Session Request",
    0, 0, 0, 0, {
        &ogs_tlv_desc_imsi_0,
        &ogs_tlv_desc_msisdn_0,
        &ogs_tlv_desc_mei_0,
        &ogs_tlv_desc_uli_0,
        &ogs_tlv_desc_serving_network_0,
        &ogs_tlv_desc_rat_type_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_f_teid_1,
        &ogs_tlv_desc_apn_0,
        &ogs_tlv_desc_selection_mode_0,
        &ogs_tlv_desc_pdn_type_0,
        &ogs_tlv_desc_paa_0,
        &ogs_tlv_desc_apn_restriction_0,
        &ogs_tlv_desc_ambr_0,
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_twmi_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_bearer_context_1,
        &ogs_tlv_desc_trace_information_0,
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_fq_csid_0,
        &ogs_tlv_desc_fq_csid_1,
        &ogs_tlv_desc_fq_csid_2,
        &ogs_tlv_desc_fq_csid_3,
        &ogs_tlv_desc_ue_time_zone_0,
        &ogs_tlv_desc_uci_0,
        &ogs_tlv_desc_charging_characteristics_0,
        &ogs_tlv_desc_ldn_0,
        &ogs_tlv_desc_ldn_1,
        &ogs_tlv_desc_ldn_2,
        &ogs_tlv_desc_ldn_3,
        &ogs_tlv_desc_signalling_priority_indication_0,
        &ogs_tlv_desc_ip_address_0,
        &ogs_tlv_desc_port_number_0,
        &ogs_tlv_desc_apco_0,
        &ogs_tlv_desc_ip_address_1,
        &ogs_tlv_desc_port_number_1,
        &ogs_tlv_desc_ip_address_2,
        &ogs_tlv_desc_twan_identifier_0,
        &ogs_tlv_desc_ip_address_3,
        &ogs_tlv_desc_cn_operator_selection_entity_0,
        &ogs_tlv_desc_presence_reporting_area_information_0,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_overload_control_information_2,
        &ogs_tlv_desc_millisecond_time_stamp_0,
        &ogs_tlv_desc_integer_number_0,
        &ogs_tlv_desc_twan_identifier_1,
        &ogs_tlv_desc_twan_identifier_timestamp_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_remote_ue_context_0,
        &ogs_tlv_desc_node_identifier_0,
        &ogs_tlv_desc_epco_0,
        &ogs_tlv_desc_serving_plmn_rate_control_0,
        &ogs_tlv_desc_counter_0,
        &ogs_tlv_desc_port_number_2,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_create_session_response =
{
    OGS_TLV_MESSAGE,
    "Create Session Response",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_change_reporting_action_0,
        &ogs_tlv_desc_csg_information_reporting_action_0,
        &ogs_tlv_desc_enb_information_reporting_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_f_teid_1,
        &ogs_tlv_desc_paa_0,
        &ogs_tlv_desc_apn_restriction_0,
        &ogs_tlv_desc_ambr_0,
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_bearer_context_1,
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_fqdn_0,
        &ogs_tlv_desc_ip_address_0,
        &ogs_tlv_desc_fq_csid_0,
        &ogs_tlv_desc_fq_csid_1,
        &ogs_tlv_desc_ldn_0,
        &ogs_tlv_desc_ldn_1,
        &ogs_tlv_desc_epc_timer_0,
        &ogs_tlv_desc_apco_0,
        &ogs_tlv_desc_ip4cp_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_presence_reporting_area_action_0,
        &ogs_tlv_desc_load_control_information_0,
        &ogs_tlv_desc_load_control_information_1,
        &ogs_tlv_desc_load_control_information_2,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_charging_id_0,
        &ogs_tlv_desc_epco_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_modify_bearer_request =
{
    OGS_TLV_MESSAGE,
    "Modify Bearer Request",
    0, 0, 0, 0, {
        &ogs_tlv_desc_mei_0,
        &ogs_tlv_desc_uli_0,
        &ogs_tlv_desc_serving_network_0,
        &ogs_tlv_desc_rat_type_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_ambr_0,
        &ogs_tlv_desc_delay_value_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_bearer_context_1,
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_ue_time_zone_0,
        &ogs_tlv_desc_fq_csid_0,
        &ogs_tlv_desc_uci_0,
        &ogs_tlv_desc_ip_address_1,
        &ogs_tlv_desc_port_number_1,
        &ogs_tlv_desc_ldn_0,
        &ogs_tlv_desc_ldn_1,
        &ogs_tlv_desc_ip_address_0,
        &ogs_tlv_desc_port_number_0,
        &ogs_tlv_desc_ip_address_2,
        &ogs_tlv_desc_cn_operator_selection_entity_0,
        &ogs_tlv_desc_presence_reporting_area_information_0,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_overload_control_information_2,
        &ogs_tlv_desc_serving_plmn_rate_control_0,
        &ogs_tlv_desc_counter_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_modify_bearer_response =
{
    OGS_TLV_MESSAGE,
    "Modify Bearer Response",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_msisdn_0,
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_apn_restriction_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_bearer_context_1,
        &ogs_tlv_desc_change_reporting_action_0,
        &ogs_tlv_desc_csg_information_reporting_action_0,
        &ogs_tlv_desc_enb_information_reporting_0,
        &ogs_tlv_desc_fqdn_0,
        &ogs_tlv_desc_ip_address_0,
        &ogs_tlv_desc_fq_csid_0,
        &ogs_tlv_desc_fq_csid_1,
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_ldn_0,
        &ogs_tlv_desc_ldn_1,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_presence_reporting_area_action_0,
        &ogs_tlv_desc_load_control_information_0,
        &ogs_tlv_desc_load_control_information_1,
        &ogs_tlv_desc_load_control_information_2,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_charging_id_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_delete_session_request =
{
    OGS_TLV_MESSAGE,
    "Delete Session Request",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_uli_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_node_type_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_ue_time_zone_0,
        &ogs_tlv_desc_uli_timestamp_0,
        &ogs_tlv_desc_ran_nas_cause_0,
        &ogs_tlv_desc_twan_identifier_0,
        &ogs_tlv_desc_twan_identifier_timestamp_0,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_overload_control_information_2,
        &ogs_tlv_desc_twan_identifier_1,
        &ogs_tlv_desc_twan_identifier_timestamp_1,
        &ogs_tlv_desc_ip_address_0,
        &ogs_tlv_desc_port_number_0,
        &ogs_tlv_desc_epco_0,
        &ogs_tlv_desc_port_number_1,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_delete_session_response =
{
    OGS_TLV_MESSAGE,
    "Delete Session Response",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_load_control_information_0,
        &ogs_tlv_desc_load_control_information_1,
        &ogs_tlv_desc_load_control_information_2,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_epco_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_modify_bearer_command =
{
    OGS_TLV_MESSAGE,
    "Modify Bearer Command",
    0, 0, 0, 0, {
        &ogs_tlv_desc_ambr_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_overload_control_information_2,
        &ogs_tlv_desc_f_teid_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_modify_bearer_failure_indication =
{
    OGS_TLV_MESSAGE,
    "Modify Bearer Failure Indication",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_delete_bearer_command =
{
    OGS_TLV_MESSAGE,
    "Delete Bearer Command",
    0, 0, 0, 0, {
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_uli_0,
        &ogs_tlv_desc_uli_timestamp_0,
        &ogs_tlv_desc_ue_time_zone_0,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_f_teid_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_delete_bearer_failure_indication =
{
    OGS_TLV_MESSAGE,
    "Delete Bearer Failure Indication",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_downlink_data_notification_failure_indication =
{
    OGS_TLV_MESSAGE,
    "Downlink Data Notification Failure Indication",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_node_type_0,
        &ogs_tlv_desc_imsi_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_create_bearer_request =
{
    OGS_TLV_MESSAGE,
    "Create Bearer Request",
    0, 0, 0, 0, {
        &ogs_tlv_desc_pti_0,
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_fq_csid_0,
        &ogs_tlv_desc_fq_csid_1,
        &ogs_tlv_desc_change_reporting_action_0,
        &ogs_tlv_desc_csg_information_reporting_action_0,
        &ogs_tlv_desc_enb_information_reporting_0,
        &ogs_tlv_desc_presence_reporting_area_action_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_load_control_information_0,
        &ogs_tlv_desc_load_control_information_1,
        &ogs_tlv_desc_load_control_information_2,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_f_container_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_create_bearer_response =
{
    OGS_TLV_MESSAGE,
    "Create Bearer Response",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_fq_csid_0,
        &ogs_tlv_desc_fq_csid_2,
        &ogs_tlv_desc_fq_csid_3,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_ue_time_zone_0,
        &ogs_tlv_desc_uli_0,
        &ogs_tlv_desc_twan_identifier_0,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_presence_reporting_area_information_0,
        &ogs_tlv_desc_ip_address_0,
        &ogs_tlv_desc_overload_control_information_2,
        &ogs_tlv_desc_twan_identifier_1,
        &ogs_tlv_desc_twan_identifier_timestamp_1,
        &ogs_tlv_desc_port_number_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_port_number_1,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_update_bearer_request =
{
    OGS_TLV_MESSAGE,
    "Update Bearer Request",
    0, 0, 0, 0, {
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_pti_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_ambr_0,
        &ogs_tlv_desc_change_reporting_action_0,
        &ogs_tlv_desc_csg_information_reporting_action_0,
        &ogs_tlv_desc_enb_information_reporting_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_fq_csid_0,
        &ogs_tlv_desc_fq_csid_1,
        &ogs_tlv_desc_presence_reporting_area_action_0,
        &ogs_tlv_desc_load_control_information_0,
        &ogs_tlv_desc_load_control_information_1,
        &ogs_tlv_desc_load_control_information_2,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_f_container_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_update_bearer_response =
{
    OGS_TLV_MESSAGE,
    "Update Bearer Response",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_fq_csid_0,
        &ogs_tlv_desc_fq_csid_1,
        &ogs_tlv_desc_fq_csid_2,
        &ogs_tlv_desc_fq_csid_3,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_ue_time_zone_0,
        &ogs_tlv_desc_uli_0,
        &ogs_tlv_desc_twan_identifier_0,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_presence_reporting_area_information_0,
        &ogs_tlv_desc_ip_address_0,
        &ogs_tlv_desc_overload_control_information_2,
        &ogs_tlv_desc_twan_identifier_1,
        &ogs_tlv_desc_twan_identifier_timestamp_1,
        &ogs_tlv_desc_port_number_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_port_number_1,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_delete_bearer_request =
{
    OGS_TLV_MESSAGE,
    "Delete Bearer Request",
    0, 0, 0, 0, {
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_ebi_1,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_pti_0,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_fq_csid_0,
        &ogs_tlv_desc_fq_csid_1,
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_load_control_information_0,
        &ogs_tlv_desc_load_control_information_1,
        &ogs_tlv_desc_load_control_information_2,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_epco_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_delete_bearer_response =
{
    OGS_TLV_MESSAGE,
    "Delete Bearer Response",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_fq_csid_0,
        &ogs_tlv_desc_fq_csid_1,
        &ogs_tlv_desc_fq_csid_2,
        &ogs_tlv_desc_fq_csid_3,
        &ogs_tlv_desc_pco_0,
        &ogs_tlv_desc_ue_time_zone_0,
        &ogs_tlv_desc_uli_0,
        &ogs_tlv_desc_uli_timestamp_0,
        &ogs_tlv_desc_twan_identifier_0,
        &ogs_tlv_desc_twan_identifier_timestamp_0,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_overload_control_information_1,
        &ogs_tlv_desc_ip_address_0,
        &ogs_tlv_desc_overload_control_information_2,
        &ogs_tlv_desc_twan_identifier_1,
        &ogs_tlv_desc_twan_identifier_timestamp_1,
        &ogs_tlv_desc_port_number_0,
        &ogs_tlv_desc_f_container_0,
        &ogs_tlv_desc_port_number_1,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_create_indirect_data_forwarding_tunnel_request =
{
    OGS_TLV_MESSAGE,
    "Create Indirect Data Forwarding Tunnel Request",
    0, 0, 0, 0, {
        &ogs_tlv_desc_imsi_0,
        &ogs_tlv_desc_mei_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_bearer_context_1,
        &ogs_tlv_desc_bearer_context_2,
        &ogs_tlv_desc_bearer_context_3,
        &ogs_tlv_desc_bearer_context_4,
        &ogs_tlv_desc_bearer_context_5,
        &ogs_tlv_desc_bearer_context_6,
        &ogs_tlv_desc_bearer_context_7,
        &ogs_tlv_desc_bearer_context_8,
        &ogs_tlv_desc_bearer_context_9,
        &ogs_tlv_desc_bearer_context_10,
        &ogs_tlv_desc_recovery_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_create_indirect_data_forwarding_tunnel_response =
{
    OGS_TLV_MESSAGE,
    "Create Indirect Data Forwarding Tunnel Response",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_bearer_context_1,
        &ogs_tlv_desc_bearer_context_2,
        &ogs_tlv_desc_bearer_context_3,
        &ogs_tlv_desc_bearer_context_4,
        &ogs_tlv_desc_bearer_context_5,
        &ogs_tlv_desc_bearer_context_6,
        &ogs_tlv_desc_bearer_context_7,
        &ogs_tlv_desc_bearer_context_8,
        &ogs_tlv_desc_bearer_context_9,
        &ogs_tlv_desc_bearer_context_10,
        &ogs_tlv_desc_recovery_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_delete_indirect_data_forwarding_tunnel_request =
{
    OGS_TLV_MESSAGE,
    "Delete Indirect Data Forwarding Tunnel Request",
    0, 0, 0, 0, {
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_delete_indirect_data_forwarding_tunnel_response =
{
    OGS_TLV_MESSAGE,
    "Delete Indirect Data Forwarding Tunnel Response",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_recovery_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_release_access_bearers_request =
{
    OGS_TLV_MESSAGE,
    "Release Access Bearers Request",
    0, 0, 0, 0, {
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_node_type_0,
        &ogs_tlv_desc_indication_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_release_access_bearers_response =
{
    OGS_TLV_MESSAGE,
    "Release Access Bearers Response",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_load_control_information_0,
        &ogs_tlv_desc_overload_control_information_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_downlink_data_notification =
{
    OGS_TLV_MESSAGE,
    "Downlink Data Notification",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_ebi_0,
        &ogs_tlv_desc_arp_0,
        &ogs_tlv_desc_imsi_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_load_control_information_0,
        &ogs_tlv_desc_overload_control_information_0,
        &ogs_tlv_desc_paging_and_service_information_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_downlink_data_notification_acknowledge =
{
    OGS_TLV_MESSAGE,
    "Downlink Data Notification Acknowledge",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_delay_value_0,
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_throttling_0,
        &ogs_tlv_desc_imsi_0,
        &ogs_tlv_desc_epc_timer_0,
        &ogs_tlv_desc_integer_number_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_modify_access_bearers_request =
{
    OGS_TLV_MESSAGE,
    "Modify Access Bearers Request",
    0, 0, 0, 0, {
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_f_teid_0,
        &ogs_tlv_desc_delay_value_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_bearer_context_1,
        &ogs_tlv_desc_recovery_0,
    NULL,
}};

ogs_tlv_desc_t ogs_tlv_desc_modify_access_bearers_response =
{
    OGS_TLV_MESSAGE,
    "Modify Access Bearers Response",
    0, 0, 0, 0, {
        &ogs_tlv_desc_cause_0,
        &ogs_tlv_desc_bearer_context_0,
        &ogs_tlv_desc_bearer_context_1,
        &ogs_tlv_desc_recovery_0,
        &ogs_tlv_desc_indication_0,
        &ogs_tlv_desc_load_control_information_0,
        &ogs_tlv_desc_overload_control_information_0,
    NULL,
}};


int ogs_gtp_parse_msg(ogs_gtp_message_t *gtp_message, ogs_pkbuf_t *pkbuf)
{
    int rv = OGS_ERROR;
    ogs_gtp_header_t *h = NULL;
    uint16_t size = 0;

    ogs_assert(gtp_message);
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);

    h = (ogs_gtp_header_t *)pkbuf->data;
    ogs_assert(h);
    
    memset(gtp_message, 0, sizeof(ogs_gtp_message_t));

    if (h->teid_presence)
        size = OGS_GTPV2C_HEADER_LEN;
    else
        size = OGS_GTPV2C_HEADER_LEN-OGS_GTP_TEID_LEN;

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(&gtp_message->h, pkbuf->data - size, size);

    if (h->teid_presence)
        gtp_message->h.teid = ntohl(gtp_message->h.teid);

    if (pkbuf->len == 0)
        return OGS_OK;

    switch(gtp_message->h.type)
    {
        case OGS_GTP_ECHO_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->echo_request,
                    &ogs_tlv_desc_echo_request, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_ECHO_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->echo_response,
                    &ogs_tlv_desc_echo_response, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_CREATE_SESSION_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->create_session_request,
                    &ogs_tlv_desc_create_session_request, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_CREATE_SESSION_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->create_session_response,
                    &ogs_tlv_desc_create_session_response, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_MODIFY_BEARER_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->modify_bearer_request,
                    &ogs_tlv_desc_modify_bearer_request, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_MODIFY_BEARER_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->modify_bearer_response,
                    &ogs_tlv_desc_modify_bearer_response, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_SESSION_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->delete_session_request,
                    &ogs_tlv_desc_delete_session_request, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_SESSION_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->delete_session_response,
                    &ogs_tlv_desc_delete_session_response, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_MODIFY_BEARER_COMMAND_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->modify_bearer_command,
                    &ogs_tlv_desc_modify_bearer_command, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_MODIFY_BEARER_FAILURE_INDICATION_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->modify_bearer_failure_indication,
                    &ogs_tlv_desc_modify_bearer_failure_indication, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_BEARER_COMMAND_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->delete_bearer_command,
                    &ogs_tlv_desc_delete_bearer_command, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_BEARER_FAILURE_INDICATION_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->delete_bearer_failure_indication,
                    &ogs_tlv_desc_delete_bearer_failure_indication, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DOWNLINK_DATA_NOTIFICATION_FAILURE_INDICATION_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->downlink_data_notification_failure_indication,
                    &ogs_tlv_desc_downlink_data_notification_failure_indication, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_CREATE_BEARER_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->create_bearer_request,
                    &ogs_tlv_desc_create_bearer_request, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_CREATE_BEARER_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->create_bearer_response,
                    &ogs_tlv_desc_create_bearer_response, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_UPDATE_BEARER_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->update_bearer_request,
                    &ogs_tlv_desc_update_bearer_request, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_UPDATE_BEARER_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->update_bearer_response,
                    &ogs_tlv_desc_update_bearer_response, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_BEARER_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->delete_bearer_request,
                    &ogs_tlv_desc_delete_bearer_request, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_BEARER_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->delete_bearer_response,
                    &ogs_tlv_desc_delete_bearer_response, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->create_indirect_data_forwarding_tunnel_request,
                    &ogs_tlv_desc_create_indirect_data_forwarding_tunnel_request, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->create_indirect_data_forwarding_tunnel_response,
                    &ogs_tlv_desc_create_indirect_data_forwarding_tunnel_response, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->delete_indirect_data_forwarding_tunnel_request,
                    &ogs_tlv_desc_delete_indirect_data_forwarding_tunnel_request, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->delete_indirect_data_forwarding_tunnel_response,
                    &ogs_tlv_desc_delete_indirect_data_forwarding_tunnel_response, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_RELEASE_ACCESS_BEARERS_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->release_access_bearers_request,
                    &ogs_tlv_desc_release_access_bearers_request, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->release_access_bearers_response,
                    &ogs_tlv_desc_release_access_bearers_response, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DOWNLINK_DATA_NOTIFICATION_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->downlink_data_notification,
                    &ogs_tlv_desc_downlink_data_notification, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->downlink_data_notification_acknowledge,
                    &ogs_tlv_desc_downlink_data_notification_acknowledge, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_MODIFY_ACCESS_BEARERS_REQUEST_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->modify_access_bearers_request,
                    &ogs_tlv_desc_modify_access_bearers_request, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_MODIFY_ACCESS_BEARERS_RESPONSE_TYPE:
            rv = ogs_tlv_parse_msg(&gtp_message->modify_access_bearers_response,
                    &ogs_tlv_desc_modify_access_bearers_response, pkbuf, OGS_TLV_MODE_T1_L2_I1);
            break;
        default:
            ogs_warn("Not implmeneted(type:%d)", gtp_message->h.type);
            break;
    }

    return rv;
}

int ogs_gtp_build_msg(ogs_pkbuf_t **pkbuf, ogs_gtp_message_t *gtp_message)
{
    int rv = OGS_ERROR;

    ogs_assert(gtp_message);
    switch(gtp_message->h.type)
    {
        case OGS_GTP_ECHO_REQUEST_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_echo_request,
                    &gtp_message->echo_request, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_ECHO_RESPONSE_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_echo_response,
                    &gtp_message->echo_response, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_CREATE_SESSION_REQUEST_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_create_session_request,
                    &gtp_message->create_session_request, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_CREATE_SESSION_RESPONSE_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_create_session_response,
                    &gtp_message->create_session_response, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_MODIFY_BEARER_REQUEST_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_modify_bearer_request,
                    &gtp_message->modify_bearer_request, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_MODIFY_BEARER_RESPONSE_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_modify_bearer_response,
                    &gtp_message->modify_bearer_response, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_SESSION_REQUEST_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_delete_session_request,
                    &gtp_message->delete_session_request, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_SESSION_RESPONSE_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_delete_session_response,
                    &gtp_message->delete_session_response, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_MODIFY_BEARER_COMMAND_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_modify_bearer_command,
                    &gtp_message->modify_bearer_command, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_MODIFY_BEARER_FAILURE_INDICATION_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_modify_bearer_failure_indication,
                    &gtp_message->modify_bearer_failure_indication, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_BEARER_COMMAND_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_delete_bearer_command,
                    &gtp_message->delete_bearer_command, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_BEARER_FAILURE_INDICATION_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_delete_bearer_failure_indication,
                    &gtp_message->delete_bearer_failure_indication, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DOWNLINK_DATA_NOTIFICATION_FAILURE_INDICATION_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_downlink_data_notification_failure_indication,
                    &gtp_message->downlink_data_notification_failure_indication, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_CREATE_BEARER_REQUEST_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_create_bearer_request,
                    &gtp_message->create_bearer_request, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_CREATE_BEARER_RESPONSE_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_create_bearer_response,
                    &gtp_message->create_bearer_response, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_UPDATE_BEARER_REQUEST_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_update_bearer_request,
                    &gtp_message->update_bearer_request, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_UPDATE_BEARER_RESPONSE_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_update_bearer_response,
                    &gtp_message->update_bearer_response, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_BEARER_REQUEST_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_delete_bearer_request,
                    &gtp_message->delete_bearer_request, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_BEARER_RESPONSE_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_delete_bearer_response,
                    &gtp_message->delete_bearer_response, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_create_indirect_data_forwarding_tunnel_request,
                    &gtp_message->create_indirect_data_forwarding_tunnel_request, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_create_indirect_data_forwarding_tunnel_response,
                    &gtp_message->create_indirect_data_forwarding_tunnel_response, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_delete_indirect_data_forwarding_tunnel_request,
                    &gtp_message->delete_indirect_data_forwarding_tunnel_request, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_delete_indirect_data_forwarding_tunnel_response,
                    &gtp_message->delete_indirect_data_forwarding_tunnel_response, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_RELEASE_ACCESS_BEARERS_REQUEST_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_release_access_bearers_request,
                    &gtp_message->release_access_bearers_request, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_release_access_bearers_response,
                    &gtp_message->release_access_bearers_response, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DOWNLINK_DATA_NOTIFICATION_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_downlink_data_notification,
                    &gtp_message->downlink_data_notification, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_downlink_data_notification_acknowledge,
                    &gtp_message->downlink_data_notification_acknowledge, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_MODIFY_ACCESS_BEARERS_REQUEST_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_modify_access_bearers_request,
                    &gtp_message->modify_access_bearers_request, OGS_TLV_MODE_T1_L2_I1);
            break;
        case OGS_GTP_MODIFY_ACCESS_BEARERS_RESPONSE_TYPE:
            rv = ogs_tlv_build_msg(pkbuf, &ogs_tlv_desc_modify_access_bearers_response,
                    &gtp_message->modify_access_bearers_response, OGS_TLV_MODE_T1_L2_I1);
            break;
        default:
            ogs_warn("Not implmeneted(type:%d)", gtp_message->h.type);
            break;
    }

    return rv;
}

