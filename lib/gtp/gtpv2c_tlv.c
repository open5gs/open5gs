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
 * Created on: 2017-03-15 21:13:48.788521 by acetcom
 * from 29274-d80.docx
 ******************************************************************************/

#include "gtpv2c_tlv.h"

tlv_desc_t gtpv2c_desc_imsi_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_IMSI_TYPE,
    0,
    0,
    sizeof(gtpv2c_imsi_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_cause_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_CAUSE_TYPE,
    0,
    0,
    sizeof(gtpv2c_cause_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_recovery_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_RECOVERY_TYPE,
    0,
    0,
    sizeof(gtpv2c_recovery_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_stn_sr_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_STN_SR_TYPE,
    0,
    0,
    sizeof(gtpv2c_stn_sr_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_apn_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_APN_TYPE,
    0,
    0,
    sizeof(gtpv2c_apn_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ambr_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_AMBR_TYPE,
    0,
    0,
    sizeof(gtpv2c_ambr_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ebi_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_EBI_TYPE,
    0,
    0,
    sizeof(gtpv2c_ebi_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ip_address_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_IP_ADDRESS_TYPE,
    0,
    0,
    sizeof(gtpv2c_ip_address_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ip_address_1 =
{
    TLV_VAR_STR,
    GTPV2C_IE_IP_ADDRESS_TYPE,
    0,
    1,
    sizeof(gtpv2c_ip_address_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ip_address_2 =
{
    TLV_VAR_STR,
    GTPV2C_IE_IP_ADDRESS_TYPE,
    0,
    2,
    sizeof(gtpv2c_ip_address_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ip_address_3 =
{
    TLV_VAR_STR,
    GTPV2C_IE_IP_ADDRESS_TYPE,
    0,
    3,
    sizeof(gtpv2c_ip_address_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_mei_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MEI_TYPE,
    0,
    0,
    sizeof(gtpv2c_mei_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_msisdn_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MSISDN_TYPE,
    0,
    0,
    sizeof(gtpv2c_msisdn_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_indication_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_INDICATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_indication_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_pco_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_PCO_TYPE,
    0,
    0,
    sizeof(gtpv2c_pco_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_paa_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_PAA_TYPE,
    0,
    0,
    sizeof(gtpv2c_paa_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_bearer_qos_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_BEARER_QOS_TYPE,
    0,
    0,
    sizeof(gtpv2c_bearer_qos_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_flow_qos_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_FLOW_QOS_TYPE,
    0,
    0,
    sizeof(gtpv2c_flow_qos_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_rat_type_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_RAT_TYPE_TYPE,
    0,
    0,
    sizeof(gtpv2c_rat_type_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_serving_network_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_SERVING_NETWORK_TYPE,
    0,
    0,
    sizeof(gtpv2c_serving_network_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_bearer_tft_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_BEARER_TFT_TYPE,
    0,
    0,
    sizeof(gtpv2c_bearer_tft_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_tad_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_TAD_TYPE,
    0,
    0,
    sizeof(gtpv2c_tad_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_uli_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_ULI_TYPE,
    0,
    0,
    sizeof(gtpv2c_uli_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_teid_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_TEID_TYPE,
    0,
    0,
    sizeof(gtpv2c_f_teid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_teid_1 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_TEID_TYPE,
    0,
    1,
    sizeof(gtpv2c_f_teid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_teid_2 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_TEID_TYPE,
    0,
    2,
    sizeof(gtpv2c_f_teid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_teid_3 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_TEID_TYPE,
    0,
    3,
    sizeof(gtpv2c_f_teid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_teid_4 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_TEID_TYPE,
    0,
    4,
    sizeof(gtpv2c_f_teid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_teid_5 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_TEID_TYPE,
    0,
    5,
    sizeof(gtpv2c_f_teid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_teid_6 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_TEID_TYPE,
    0,
    6,
    sizeof(gtpv2c_f_teid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_teid_7 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_TEID_TYPE,
    0,
    7,
    sizeof(gtpv2c_f_teid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_teid_8 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_TEID_TYPE,
    0,
    8,
    sizeof(gtpv2c_f_teid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_teid_9 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_TEID_TYPE,
    0,
    9,
    sizeof(gtpv2c_f_teid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_teid_10 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_TEID_TYPE,
    0,
    10,
    sizeof(gtpv2c_f_teid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_teid_11 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_TEID_TYPE,
    0,
    11,
    sizeof(gtpv2c_f_teid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_tmsi_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_TMSI_TYPE,
    0,
    0,
    sizeof(gtpv2c_tmsi_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_global_cn_id_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_GLOBAL_CN_ID_TYPE,
    0,
    0,
    sizeof(gtpv2c_global_cn_id_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_s103pdf_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_S103PDF_TYPE,
    0,
    0,
    sizeof(gtpv2c_s103pdf_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_s1udf_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_S1UDF_TYPE,
    0,
    0,
    sizeof(gtpv2c_s1udf_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_delay_value_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_DELAY_VALUE_TYPE,
    0,
    0,
    sizeof(gtpv2c_delay_value_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_charging_id_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_CHARGING_ID_TYPE,
    0,
    0,
    sizeof(gtpv2c_charging_id_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_charging_characteristics_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_CHARGING_CHARACTERISTICS_TYPE,
    0,
    0,
    sizeof(gtpv2c_charging_characteristics_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_trace_information_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_TRACE_INFORMATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_trace_information_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_bearer_flags_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_BEARER_FLAGS_TYPE,
    0,
    0,
    sizeof(gtpv2c_bearer_flags_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_pdn_type_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_PDN_TYPE_TYPE,
    0,
    0,
    sizeof(gtpv2c_pdn_type_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_procedure_transaction_id_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_PROCEDURE_TRANSACTION_ID_TYPE,
    0,
    0,
    sizeof(gtpv2c_procedure_transaction_id_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_mm_context_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MM_CONTEXT_TYPE,
    0,
    0,
    sizeof(gtpv2c_mm_context_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_pdu_numbers_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_PDU_NUMBERS_TYPE,
    0,
    0,
    sizeof(gtpv2c_pdu_numbers_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_p_tmsi_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_P_TMSI_TYPE,
    0,
    0,
    sizeof(gtpv2c_p_tmsi_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_p_tmsi_signature_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_P_TMSI_SIGNATURE_TYPE,
    0,
    0,
    sizeof(gtpv2c_p_tmsi_signature_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_hop_counter_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_HOP_COUNTER_TYPE,
    0,
    0,
    sizeof(gtpv2c_hop_counter_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ue_time_zone_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_UE_TIME_ZONE_TYPE,
    0,
    0,
    sizeof(gtpv2c_ue_time_zone_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_trace_reference_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_TRACE_REFERENCE_TYPE,
    0,
    0,
    sizeof(gtpv2c_trace_reference_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_complete_request_message_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_COMPLETE_REQUEST_MESSAGE_TYPE,
    0,
    0,
    sizeof(gtpv2c_complete_request_message_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_guti_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_GUTI_TYPE,
    0,
    0,
    sizeof(gtpv2c_guti_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_container_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_CONTAINER_TYPE,
    0,
    0,
    sizeof(gtpv2c_f_container_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_f_cause_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_F_CAUSE_TYPE,
    0,
    0,
    sizeof(gtpv2c_f_cause_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_plmn_id_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_PLMN_ID_TYPE,
    0,
    0,
    sizeof(gtpv2c_plmn_id_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_target_identification_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_TARGET_IDENTIFICATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_target_identification_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_packet_flow_id_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_PACKET_FLOW_ID_TYPE,
    0,
    0,
    sizeof(gtpv2c_packet_flow_id_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_rab_context_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_RAB_CONTEXT_TYPE,
    0,
    0,
    sizeof(gtpv2c_rab_context_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_source_rnc_pdcp_context_info_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_SOURCE_RNC_PDCP_CONTEXT_INFO_TYPE,
    0,
    0,
    sizeof(gtpv2c_source_rnc_pdcp_context_info_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_port_number_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_PORT_NUMBER_TYPE,
    0,
    0,
    sizeof(gtpv2c_port_number_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_port_number_1 =
{
    TLV_VAR_STR,
    GTPV2C_IE_PORT_NUMBER_TYPE,
    0,
    1,
    sizeof(gtpv2c_port_number_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_port_number_2 =
{
    TLV_VAR_STR,
    GTPV2C_IE_PORT_NUMBER_TYPE,
    0,
    2,
    sizeof(gtpv2c_port_number_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_apn_restriction_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_APN_RESTRICTION_TYPE,
    0,
    0,
    sizeof(gtpv2c_apn_restriction_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_selection_mode_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_SELECTION_MODE_TYPE,
    0,
    0,
    sizeof(gtpv2c_selection_mode_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_source_identification_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_SOURCE_IDENTIFICATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_source_identification_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_change_reporting_action_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_CHANGE_REPORTING_ACTION_TYPE,
    0,
    0,
    sizeof(gtpv2c_change_reporting_action_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_fq_csid_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_FQ_CSID_TYPE,
    0,
    0,
    sizeof(gtpv2c_fq_csid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_fq_csid_1 =
{
    TLV_VAR_STR,
    GTPV2C_IE_FQ_CSID_TYPE,
    0,
    1,
    sizeof(gtpv2c_fq_csid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_fq_csid_2 =
{
    TLV_VAR_STR,
    GTPV2C_IE_FQ_CSID_TYPE,
    0,
    2,
    sizeof(gtpv2c_fq_csid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_fq_csid_3 =
{
    TLV_VAR_STR,
    GTPV2C_IE_FQ_CSID_TYPE,
    0,
    3,
    sizeof(gtpv2c_fq_csid_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_channel_needed_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_CHANNEL_NEEDED_TYPE,
    0,
    0,
    sizeof(gtpv2c_channel_needed_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_emlpp_priority_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_EMLPP_PRIORITY_TYPE,
    0,
    0,
    sizeof(gtpv2c_emlpp_priority_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_node_type_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_NODE_TYPE_TYPE,
    0,
    0,
    sizeof(gtpv2c_node_type_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_fqdn_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_FQDN_TYPE,
    0,
    0,
    sizeof(gtpv2c_fqdn_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ti_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_TI_TYPE,
    0,
    0,
    sizeof(gtpv2c_ti_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_mbms_session_duration_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MBMS_SESSION_DURATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_mbms_session_duration_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_mbms_service_area_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MBMS_SERVICE_AREA_TYPE,
    0,
    0,
    sizeof(gtpv2c_mbms_service_area_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_mbms_session_identifier_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MBMS_SESSION_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(gtpv2c_mbms_session_identifier_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_mbms_flow_identifier_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MBMS_FLOW_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(gtpv2c_mbms_flow_identifier_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_mbms_ip_multicast_distribution_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MBMS_IP_MULTICAST_DISTRIBUTION_TYPE,
    0,
    0,
    sizeof(gtpv2c_mbms_ip_multicast_distribution_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_mbms_distribution_acknowledge_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MBMS_DISTRIBUTION_ACKNOWLEDGE_TYPE,
    0,
    0,
    sizeof(gtpv2c_mbms_distribution_acknowledge_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_rfsp_index_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_RFSP_INDEX_TYPE,
    0,
    0,
    sizeof(gtpv2c_rfsp_index_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_uci_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_UCI_TYPE,
    0,
    0,
    sizeof(gtpv2c_uci_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_csg_information_reporting_action_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_CSG_INFORMATION_REPORTING_ACTION_TYPE,
    0,
    0,
    sizeof(gtpv2c_csg_information_reporting_action_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_csg_id_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_CSG_ID_TYPE,
    0,
    0,
    sizeof(gtpv2c_csg_id_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_cmi_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_CMI_TYPE,
    0,
    0,
    sizeof(gtpv2c_cmi_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_service_indicator_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_SERVICE_INDICATOR_TYPE,
    0,
    0,
    sizeof(gtpv2c_service_indicator_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_detach_type_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_DETACH_TYPE_TYPE,
    0,
    0,
    sizeof(gtpv2c_detach_type_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ldn_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_LDN_TYPE,
    0,
    0,
    sizeof(gtpv2c_ldn_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ldn_1 =
{
    TLV_VAR_STR,
    GTPV2C_IE_LDN_TYPE,
    0,
    1,
    sizeof(gtpv2c_ldn_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ldn_2 =
{
    TLV_VAR_STR,
    GTPV2C_IE_LDN_TYPE,
    0,
    2,
    sizeof(gtpv2c_ldn_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ldn_3 =
{
    TLV_VAR_STR,
    GTPV2C_IE_LDN_TYPE,
    0,
    3,
    sizeof(gtpv2c_ldn_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_node_features_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_NODE_FEATURES_TYPE,
    0,
    0,
    sizeof(gtpv2c_node_features_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_mbms_time_to_data_transfer_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MBMS_TIME_TO_DATA_TRANSFER_TYPE,
    0,
    0,
    sizeof(gtpv2c_mbms_time_to_data_transfer_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_throttling_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_THROTTLING_TYPE,
    0,
    0,
    sizeof(gtpv2c_throttling_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_arp_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_ARP_TYPE,
    0,
    0,
    sizeof(gtpv2c_arp_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_epc_timer_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_EPC_TIMER_TYPE,
    0,
    0,
    sizeof(gtpv2c_epc_timer_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_signalling_priority_indication_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_SIGNALLING_PRIORITY_INDICATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_signalling_priority_indication_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_tmgi_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_TMGI_TYPE,
    0,
    0,
    sizeof(gtpv2c_tmgi_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_additional_mm_context_for_srvcc_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_ADDITIONAL_MM_CONTEXT_FOR_SRVCC_TYPE,
    0,
    0,
    sizeof(gtpv2c_additional_mm_context_for_srvcc_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_additional_flags_for_srvcc_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_ADDITIONAL_FLAGS_FOR_SRVCC_TYPE,
    0,
    0,
    sizeof(gtpv2c_additional_flags_for_srvcc_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_mdt_configuration_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MDT_CONFIGURATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_mdt_configuration_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_apco_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_APCO_TYPE,
    0,
    0,
    sizeof(gtpv2c_apco_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_absolute_time_of_mbms_data_transfer_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_ABSOLUTE_TIME_OF_MBMS_DATA_TRANSFER_TYPE,
    0,
    0,
    sizeof(gtpv2c_absolute_time_of_mbms_data_transfer_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_enb_information_reporting_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_ENB_INFORMATION_REPORTING_TYPE,
    0,
    0,
    sizeof(gtpv2c_enb_information_reporting_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ip4cp_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_IP4CP_TYPE,
    0,
    0,
    sizeof(gtpv2c_ip4cp_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_change_to_report_flags_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_CHANGE_TO_REPORT_FLAGS_TYPE,
    0,
    0,
    sizeof(gtpv2c_change_to_report_flags_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_action_indication_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_ACTION_INDICATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_action_indication_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_twan_identifier_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_TWAN_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(gtpv2c_twan_identifier_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_twan_identifier_1 =
{
    TLV_VAR_STR,
    GTPV2C_IE_TWAN_IDENTIFIER_TYPE,
    0,
    1,
    sizeof(gtpv2c_twan_identifier_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_uli_timestamp_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_ULI_TIMESTAMP_TYPE,
    0,
    0,
    sizeof(gtpv2c_uli_timestamp_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_mbms_flags_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MBMS_FLAGS_TYPE,
    0,
    0,
    sizeof(gtpv2c_mbms_flags_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ran_nas_cause_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_RAN_NAS_CAUSE_TYPE,
    0,
    0,
    sizeof(gtpv2c_ran_nas_cause_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_cn_operator_selection_entity_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_CN_OPERATOR_SELECTION_ENTITY_TYPE,
    0,
    0,
    sizeof(gtpv2c_cn_operator_selection_entity_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_twmi_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_TWMI_TYPE,
    0,
    0,
    sizeof(gtpv2c_twmi_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_node_number_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_NODE_NUMBER_TYPE,
    0,
    0,
    sizeof(gtpv2c_node_number_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_node_identifier_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_NODE_IDENTIFIER_TYPE,
    0,
    0,
    sizeof(gtpv2c_node_identifier_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_presence_reporting_area_action_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_PRESENCE_REPORTING_AREA_ACTION_TYPE,
    0,
    0,
    sizeof(gtpv2c_presence_reporting_area_action_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_presence_reporting_area_information_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_PRESENCE_REPORTING_AREA_INFORMATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_presence_reporting_area_information_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_twan_identifier_timestamp_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_TWAN_IDENTIFIER_TIMESTAMP_TYPE,
    0,
    0,
    sizeof(gtpv2c_twan_identifier_timestamp_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_metric_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_METRIC_TYPE,
    0,
    0,
    sizeof(gtpv2c_metric_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_sequence_number_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_SEQUENCE_NUMBER_TYPE,
    0,
    0,
    sizeof(gtpv2c_sequence_number_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_apn_and_relative_capacity_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_APN_AND_RELATIVE_CAPACITY_TYPE,
    0,
    0,
    sizeof(gtpv2c_apn_and_relative_capacity_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_wlan_offloadability_indication_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_WLAN_OFFLOADABILITY_INDICATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_wlan_offloadability_indication_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_paging_and_service_information_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_PAGING_AND_SERVICE_INFORMATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_paging_and_service_information_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_integer_number_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_INTEGER_NUMBER_TYPE,
    0,
    0,
    sizeof(gtpv2c_integer_number_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_millisecond_time_stamp_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MILLISECOND_TIME_STAMP_TYPE,
    0,
    0,
    sizeof(gtpv2c_millisecond_time_stamp_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_monitoring_event_information_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_MONITORING_EVENT_INFORMATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_monitoring_event_information_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ecgi_list_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_ECGI_LIST_TYPE,
    0,
    0,
    sizeof(gtpv2c_ecgi_list_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_remote_user_id_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_REMOTE_USER_ID_TYPE,
    0,
    0,
    sizeof(gtpv2c_remote_user_id_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_remote_ue_ip_information_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_REMOTE_UE_IP_INFORMATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_remote_ue_ip_information_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_ciot_optimizations_support_indication_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_CIOT_OPTIMIZATIONS_SUPPORT_INDICATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_ciot_optimizations_support_indication_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_header_compression_configuration_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_HEADER_COMPRESSION_CONFIGURATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_header_compression_configuration_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_epco_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_EPCO_TYPE,
    0,
    0,
    sizeof(gtpv2c_epco_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_serving_plmn_rate_control_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_SERVING_PLMN_RATE_CONTROL_TYPE,
    0,
    0,
    sizeof(gtpv2c_serving_plmn_rate_control_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_counter_0 =
{
    TLV_VAR_STR,
    GTPV2C_IE_COUNTER_TYPE,
    0,
    0,
    sizeof(gtpv2c_counter_t),
    { NULL }
};

tlv_desc_t gtpv2c_desc_bearer_context_0 =
{
    TLV_COMPOUND,
    GTPV2C_IE_BEARER_CONTEXT_TYPE,
    0,
    0,
    sizeof(gtpv2c_bearer_context_t),
    {
        &gtpv2c_desc_ebi_0,
        &gtpv2c_desc_bearer_tft_0,
        &gtpv2c_desc_f_teid_0,
        &gtpv2c_desc_f_teid_1,
        &gtpv2c_desc_f_teid_2,
        &gtpv2c_desc_f_teid_3,
        &gtpv2c_desc_f_teid_4,
        &gtpv2c_desc_f_teid_5,
        &gtpv2c_desc_f_teid_6,
        &gtpv2c_desc_bearer_qos_0,
        &gtpv2c_desc_f_teid_7,
        &gtpv2c_desc_cause_0,
        &gtpv2c_desc_charging_id_0,
        &gtpv2c_desc_bearer_flags_0,
        &gtpv2c_desc_pco_0,
        &gtpv2c_desc_epco_0,
        &gtpv2c_desc_f_teid_8,
        &gtpv2c_desc_f_teid_9,
        &gtpv2c_desc_f_teid_10,
        &gtpv2c_desc_f_teid_11,
        &gtpv2c_desc_ran_nas_cause_0,
        &gtpv2c_desc_apco_0,
        &gtpv2c_desc_f_container_0,
        &gtpv2c_desc_ti_0,
        &gtpv2c_desc_packet_flow_id_0,
        NULL,
    }
};

tlv_desc_t gtpv2c_desc_bearer_context_1 =
{
    TLV_COMPOUND,
    GTPV2C_IE_BEARER_CONTEXT_TYPE,
    0,
    1,
    sizeof(gtpv2c_bearer_context_t),
    {
        &gtpv2c_desc_ebi_0,
        &gtpv2c_desc_bearer_tft_0,
        &gtpv2c_desc_f_teid_0,
        &gtpv2c_desc_f_teid_1,
        &gtpv2c_desc_f_teid_2,
        &gtpv2c_desc_f_teid_3,
        &gtpv2c_desc_f_teid_4,
        &gtpv2c_desc_f_teid_5,
        &gtpv2c_desc_f_teid_6,
        &gtpv2c_desc_bearer_qos_0,
        &gtpv2c_desc_f_teid_7,
        &gtpv2c_desc_cause_0,
        &gtpv2c_desc_charging_id_0,
        &gtpv2c_desc_bearer_flags_0,
        &gtpv2c_desc_pco_0,
        &gtpv2c_desc_epco_0,
        &gtpv2c_desc_f_teid_8,
        &gtpv2c_desc_f_teid_9,
        &gtpv2c_desc_f_teid_10,
        &gtpv2c_desc_f_teid_11,
        &gtpv2c_desc_ran_nas_cause_0,
        &gtpv2c_desc_apco_0,
        &gtpv2c_desc_f_container_0,
        &gtpv2c_desc_ti_0,
        &gtpv2c_desc_packet_flow_id_0,
        NULL,
    }
};

tlv_desc_t gtpv2c_desc_pdn_connection_0 =
{
    TLV_COMPOUND,
    GTPV2C_IE_PDN_CONNECTION_TYPE,
    0,
    0,
    sizeof(gtpv2c_pdn_connection_t),
    {
        NULL,
    }
};

tlv_desc_t gtpv2c_desc_overload_control_information_0 =
{
    TLV_COMPOUND,
    GTPV2C_IE_OVERLOAD_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_overload_control_information_t),
    {
        &gtpv2c_desc_sequence_number_0,
        &gtpv2c_desc_metric_0,
        &gtpv2c_desc_epc_timer_0,
        &gtpv2c_desc_apn_0,
        NULL,
    }
};

tlv_desc_t gtpv2c_desc_overload_control_information_1 =
{
    TLV_COMPOUND,
    GTPV2C_IE_OVERLOAD_CONTROL_INFORMATION_TYPE,
    0,
    1,
    sizeof(gtpv2c_overload_control_information_t),
    {
        &gtpv2c_desc_sequence_number_0,
        &gtpv2c_desc_metric_0,
        &gtpv2c_desc_epc_timer_0,
        &gtpv2c_desc_apn_0,
        NULL,
    }
};

tlv_desc_t gtpv2c_desc_overload_control_information_2 =
{
    TLV_COMPOUND,
    GTPV2C_IE_OVERLOAD_CONTROL_INFORMATION_TYPE,
    0,
    2,
    sizeof(gtpv2c_overload_control_information_t),
    {
        &gtpv2c_desc_sequence_number_0,
        &gtpv2c_desc_metric_0,
        &gtpv2c_desc_epc_timer_0,
        &gtpv2c_desc_apn_0,
        NULL,
    }
};

tlv_desc_t gtpv2c_desc_load_control_information_0 =
{
    TLV_COMPOUND,
    GTPV2C_IE_LOAD_CONTROL_INFORMATION_TYPE,
    0,
    0,
    sizeof(gtpv2c_load_control_information_t),
    {
        &gtpv2c_desc_sequence_number_0,
        &gtpv2c_desc_metric_0,
        &gtpv2c_desc_apn_and_relative_capacity_0,
        NULL,
    }
};

tlv_desc_t gtpv2c_desc_remote_ue_context_0 =
{
    TLV_COMPOUND,
    GTPV2C_IE_REMOTE_UE_CONTEXT_TYPE,
    0,
    0,
    sizeof(gtpv2c_remote_ue_context_t),
    {
        &gtpv2c_desc_remote_user_id_0,
        &gtpv2c_desc_remote_ue_ip_information_0,
        NULL,
    }
};

tlv_desc_t gtpv2c_desc_scef_pdn_connection_0 =
{
    TLV_COMPOUND,
    GTPV2C_IE_SCEF_PDN_CONNECTION_TYPE,
    0,
    0,
    sizeof(gtpv2c_scef_pdn_connection_t),
    {
        &gtpv2c_desc_apn_0,
        &gtpv2c_desc_ebi_0,
        &gtpv2c_desc_node_identifier_0,
        NULL,
    }
};

tlv_desc_t gtpv2c_desc_echo_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
        &gtpv2c_desc_recovery_0,
        &gtpv2c_desc_node_features_0,
    NULL,
}};

tlv_desc_t gtpv2c_desc_echo_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
        &gtpv2c_desc_recovery_0,
        &gtpv2c_desc_node_features_0,
    NULL,
}};

tlv_desc_t gtpv2c_desc_version_not_supported_indication =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_create_session_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
        &gtpv2c_desc_imsi_0,
        &gtpv2c_desc_msisdn_0,
        &gtpv2c_desc_mei_0,
        &gtpv2c_desc_uli_0,
        &gtpv2c_desc_serving_network_0,
        &gtpv2c_desc_rat_type_0,
        &gtpv2c_desc_indication_0,
        &gtpv2c_desc_f_teid_0,
        &gtpv2c_desc_f_teid_1,
        &gtpv2c_desc_apn_0,
        &gtpv2c_desc_selection_mode_0,
        &gtpv2c_desc_pdn_type_0,
        &gtpv2c_desc_paa_0,
        &gtpv2c_desc_apn_restriction_0,
        &gtpv2c_desc_ambr_0,
        &gtpv2c_desc_ebi_0,
        &gtpv2c_desc_twmi_0,
        &gtpv2c_desc_pco_0,
        &gtpv2c_desc_bearer_context_0,
        &gtpv2c_desc_bearer_context_1,
        &gtpv2c_desc_trace_information_0,
        &gtpv2c_desc_recovery_0,
        &gtpv2c_desc_fq_csid_0,
        &gtpv2c_desc_fq_csid_1,
        &gtpv2c_desc_fq_csid_2,
        &gtpv2c_desc_fq_csid_3,
        &gtpv2c_desc_ue_time_zone_0,
        &gtpv2c_desc_uci_0,
        &gtpv2c_desc_charging_characteristics_0,
        &gtpv2c_desc_ldn_0,
        &gtpv2c_desc_ldn_1,
        &gtpv2c_desc_ldn_2,
        &gtpv2c_desc_ldn_3,
        &gtpv2c_desc_signalling_priority_indication_0,
        &gtpv2c_desc_ip_address_0,
        &gtpv2c_desc_port_number_0,
        &gtpv2c_desc_apco_0,
        &gtpv2c_desc_ip_address_1,
        &gtpv2c_desc_port_number_1,
        &gtpv2c_desc_ip_address_2,
        &gtpv2c_desc_twan_identifier_0,
        &gtpv2c_desc_ip_address_3,
        &gtpv2c_desc_cn_operator_selection_entity_0,
        &gtpv2c_desc_presence_reporting_area_information_0,
        &gtpv2c_desc_overload_control_information_0,
        &gtpv2c_desc_overload_control_information_1,
        &gtpv2c_desc_overload_control_information_2,
        &gtpv2c_desc_millisecond_time_stamp_0,
        &gtpv2c_desc_integer_number_0,
        &gtpv2c_desc_twan_identifier_1,
        &gtpv2c_desc_twan_identifier_timestamp_0,
        &gtpv2c_desc_f_container_0,
        &gtpv2c_desc_remote_ue_context_0,
        &gtpv2c_desc_node_identifier_0,
        &gtpv2c_desc_epco_0,
        &gtpv2c_desc_serving_plmn_rate_control_0,
        &gtpv2c_desc_counter_0,
        &gtpv2c_desc_port_number_2,
    NULL,
}};

tlv_desc_t gtpv2c_desc_create_session_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_modify_bearer_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_modify_bearer_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_delete_session_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_delete_session_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_change_notification_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_change_notification_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_remote_ue_report_notification =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_remote_ue_report_acknowledge =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_modify_bearer_command =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_modify_bearer_failure_indication =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_delete_bearer_command =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_delete_bearer_failure_indication =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_bearer_resource_command =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_bearer_resource_failure_indication =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_downlink_data_notification_failure_indication =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_trace_session_activation =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_trace_session_deactivation =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_stop_paging_indication =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_create_bearer_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_create_bearer_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_update_bearer_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_update_bearer_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_delete_bearer_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_delete_bearer_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_delete_pdn_connection_set_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_delete_pdn_connection_set_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_pgw_downlink_triggering_notification =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_pgw_downlink_triggering_acknowledge =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_create_forwarding_tunnel_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_create_forwarding_tunnel_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_suspend_notification =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_suspend_acknowledge =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_resume_notification =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_resume_acknowledge =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_create_indirect_data_forwarding_tunnel_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_create_indirect_data_forwarding_tunnel_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_delete_indirect_data_forwarding_tunnel_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_delete_indirect_data_forwarding_tunnel_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_release_access_bearers_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_release_access_bearers_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_downlink_data_notification =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_downlink_data_notification_acknowledge =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_pgw_restart_notification =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_pgw_restart_notification_acknowledge =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_update_pdn_connection_set_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_update_pdn_connection_set_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_modify_access_bearers_request =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};

tlv_desc_t gtpv2c_desc_modify_access_bearers_response =
{
    TLV_MESSAGE, 0, 0, 0, 0, {
    NULL,
}};


