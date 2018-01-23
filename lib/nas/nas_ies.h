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
 * This file had been created by gtpv2c_tlv.py script v0.1.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2018-01-23 15:58:42.823738 by acetcom
 * from 24301-d80.docx
 ******************************************************************************/

#ifndef __NAS_IES_H__
#define __NAS_IES_H__

#include "core_pkbuf.h"
#include "nas_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(c_int16_t) nas_encode_optional_type(pkbuf_t *pkbuf, c_uint8_t type);

CORE_DECLARE(c_int16_t) nas_decode_additional_information(nas_additional_information_t *additional_information, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_device_properties(nas_device_properties_t *device_properties, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_eps_bearer_context_status(nas_eps_bearer_context_status_t *eps_bearer_context_status, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_supported_codec_list(nas_supported_codec_list_t *supported_codec_list, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_location_area_identification(nas_location_area_identification_t *location_area_identification, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_mobile_identity(nas_mobile_identity_t *mobile_identity, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_mobile_station_classmark_2(nas_mobile_station_classmark_2_t *mobile_station_classmark_2, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_mobile_station_classmark_3(nas_mobile_station_classmark_3_t *mobile_station_classmark_3, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_plmn_list(nas_plmn_list_t *plmn_list, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_additional_update_result(nas_additional_update_result_t *additional_update_result, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_additional_update_type(nas_additional_update_type_t *additional_update_type, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_authentication_failure_parameter(nas_authentication_failure_parameter_t *authentication_failure_parameter, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_eps_attach_result(nas_eps_attach_result_t *eps_attach_result, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_eps_attach_type(nas_eps_attach_type_t *eps_attach_type, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_eps_mobile_identity(nas_eps_mobile_identity_t *eps_mobile_identity, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_eps_network_feature_support(nas_eps_network_feature_support_t *eps_network_feature_support, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_eps_update_result(nas_eps_update_result_t *eps_update_result, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_eps_update_type(nas_eps_update_type_t *eps_update_type, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_esm_message_container(nas_esm_message_container_t *esm_message_container, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_gprs_timer(nas_gprs_timer_t *gprs_timer, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_gprs_timer_2(nas_gprs_timer_2_t *gprs_timer_2, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_gprs_timer_3(nas_gprs_timer_3_t *gprs_timer_3, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_identity_type_2(nas_identity_type_2_t *identity_type_2, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_imeisv_request(nas_imeisv_request_t *imeisv_request, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_ksi_and_sequence_number(nas_ksi_and_sequence_number_t *ksi_and_sequence_number, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_authentication_parameter_autn(nas_authentication_parameter_autn_t *authentication_parameter_autn, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_ms_network_capability(nas_ms_network_capability_t *ms_network_capability, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_ms_network_feature_support(nas_ms_network_feature_support_t *ms_network_feature_support, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_key_set_identifier(nas_key_set_identifier_t *key_set_identifier, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_message_container(nas_message_container_t *message_container, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_security_algorithms(nas_security_algorithms_t *security_algorithms, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_network_name(nas_network_name_t *network_name, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_network_resource_identifier_container(nas_network_resource_identifier_container_t *network_resource_identifier_container, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_nonce(nas_nonce_t *nonce, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_paging_identity(nas_paging_identity_t *paging_identity, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_p_tmsi_signature(nas_p_tmsi_signature_t *p_tmsi_signature, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_extended_emm_cause(nas_extended_emm_cause_t *extended_emm_cause, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_service_type(nas_service_type_t *service_type, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_short_mac(nas_short_mac_t *short_mac, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_time_zone(nas_time_zone_t *time_zone, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_authentication_parameter_rand(nas_authentication_parameter_rand_t *authentication_parameter_rand, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_time_zone_and_time(nas_time_zone_and_time_t *time_zone_and_time, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_tmsi_status(nas_tmsi_status_t *tmsi_status, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_tracking_area_identity(nas_tracking_area_identity_t *tracking_area_identity, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_tracking_area_identity_list(nas_tracking_area_identity_list_t *tracking_area_identity_list, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_ue_network_capability(nas_ue_network_capability_t *ue_network_capability, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_ue_radio_capability_information_update_needed(nas_ue_radio_capability_information_update_needed_t *ue_radio_capability_information_update_needed, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_ue_security_capability(nas_ue_security_capability_t *ue_security_capability, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_emergency_number_list(nas_emergency_number_list_t *emergency_number_list, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_cli(nas_cli_t *cli, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_ss_code(nas_ss_code_t *ss_code, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_authentication_response_parameter(nas_authentication_response_parameter_t *authentication_response_parameter, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_lcs_indicator(nas_lcs_indicator_t *lcs_indicator, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_lcs_client_identity(nas_lcs_client_identity_t *lcs_client_identity, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_generic_message_container_type(nas_generic_message_container_type_t *generic_message_container_type, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_generic_message_container(nas_generic_message_container_t *generic_message_container, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_voice_domain_preference_and_ue_usage_setting(nas_voice_domain_preference_and_ue_usage_setting_t *voice_domain_preference_and_ue_usage_setting, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_guti_type(nas_guti_type_t *guti_type, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_extended_drx_parameters(nas_extended_drx_parameters_t *extended_drx_parameters, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_ciphering_key_sequence_number(nas_ciphering_key_sequence_number_t *ciphering_key_sequence_number, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_csfb_response(nas_csfb_response_t *csfb_response, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_daylight_saving_time(nas_daylight_saving_time_t *daylight_saving_time, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_detach_type(nas_detach_type_t *detach_type, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_drx_parameter(nas_drx_parameter_t *drx_parameter, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_emm_cause(nas_emm_cause_t *emm_cause, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_access_point_name(nas_access_point_name_t *access_point_name, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_protocol_configuration_options(nas_protocol_configuration_options_t *protocol_configuration_options, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_quality_of_service(nas_quality_of_service_t *quality_of_service, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_radio_priority(nas_radio_priority_t *radio_priority, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_re_attempt_indicator(nas_re_attempt_indicator_t *re_attempt_indicator, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_request_type(nas_request_type_t *request_type, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_traffic_flow_aggregate_description(nas_traffic_flow_aggregate_description_t *traffic_flow_aggregate_description, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_traffic_flow_template(nas_traffic_flow_template_t *traffic_flow_template, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_transaction_identifier(nas_transaction_identifier_t *transaction_identifier, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_wlan_offload_acceptability(nas_wlan_offload_acceptability_t *wlan_offload_acceptability, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_nbifom_container(nas_nbifom_container_t *nbifom_container, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_apn_aggregate_maximum_bit_rate(nas_apn_aggregate_maximum_bit_rate_t *apn_aggregate_maximum_bit_rate, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_header_compression_configuration(nas_header_compression_configuration_t *header_compression_configuration, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_control_plane_only_indication(nas_control_plane_only_indication_t *control_plane_only_indication, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_extended_protocol_configuration_options(nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_header_compression_configuration_status(nas_header_compression_configuration_status_t *header_compression_configuration_status, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_serving_plmn_rate_control(nas_serving_plmn_rate_control_t *serving_plmn_rate_control, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_connectivity_type(nas_connectivity_type_t *connectivity_type, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_eps_quality_of_service(nas_eps_quality_of_service_t *eps_quality_of_service, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_esm_cause(nas_esm_cause_t *esm_cause, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_esm_information_transfer_flag(nas_esm_information_transfer_flag_t *esm_information_transfer_flag, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_linked_eps_bearer_identity(nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_llc_service_access_point_identifier(nas_llc_service_access_point_identifier_t *llc_service_access_point_identifier, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_packet_flow_identifier(nas_packet_flow_identifier_t *packet_flow_identifier, pkbuf_t *pkbuf);
CORE_DECLARE(c_int16_t) nas_decode_pdn_address(nas_pdn_address_t *pdn_address, pkbuf_t *pkbuf);

CORE_DECLARE(c_int16_t) nas_encode_additional_information(pkbuf_t *pkbuf, nas_additional_information_t *additional_information);
CORE_DECLARE(c_int16_t) nas_encode_device_properties(pkbuf_t *pkbuf, nas_device_properties_t *device_properties);
CORE_DECLARE(c_int16_t) nas_encode_eps_bearer_context_status(pkbuf_t *pkbuf, nas_eps_bearer_context_status_t *eps_bearer_context_status);
CORE_DECLARE(c_int16_t) nas_encode_supported_codec_list(pkbuf_t *pkbuf, nas_supported_codec_list_t *supported_codec_list);
CORE_DECLARE(c_int16_t) nas_encode_location_area_identification(pkbuf_t *pkbuf, nas_location_area_identification_t *location_area_identification);
CORE_DECLARE(c_int16_t) nas_encode_mobile_identity(pkbuf_t *pkbuf, nas_mobile_identity_t *mobile_identity);
CORE_DECLARE(c_int16_t) nas_encode_mobile_station_classmark_2(pkbuf_t *pkbuf, nas_mobile_station_classmark_2_t *mobile_station_classmark_2);
CORE_DECLARE(c_int16_t) nas_encode_mobile_station_classmark_3(pkbuf_t *pkbuf, nas_mobile_station_classmark_3_t *mobile_station_classmark_3);
CORE_DECLARE(c_int16_t) nas_encode_plmn_list(pkbuf_t *pkbuf, nas_plmn_list_t *plmn_list);
CORE_DECLARE(c_int16_t) nas_encode_additional_update_result(pkbuf_t *pkbuf, nas_additional_update_result_t *additional_update_result);
CORE_DECLARE(c_int16_t) nas_encode_additional_update_type(pkbuf_t *pkbuf, nas_additional_update_type_t *additional_update_type);
CORE_DECLARE(c_int16_t) nas_encode_authentication_failure_parameter(pkbuf_t *pkbuf, nas_authentication_failure_parameter_t *authentication_failure_parameter);
CORE_DECLARE(c_int16_t) nas_encode_eps_attach_result(pkbuf_t *pkbuf, nas_eps_attach_result_t *eps_attach_result);
CORE_DECLARE(c_int16_t) nas_encode_eps_attach_type(pkbuf_t *pkbuf, nas_eps_attach_type_t *eps_attach_type);
CORE_DECLARE(c_int16_t) nas_encode_eps_mobile_identity(pkbuf_t *pkbuf, nas_eps_mobile_identity_t *eps_mobile_identity);
CORE_DECLARE(c_int16_t) nas_encode_eps_network_feature_support(pkbuf_t *pkbuf, nas_eps_network_feature_support_t *eps_network_feature_support);
CORE_DECLARE(c_int16_t) nas_encode_eps_update_result(pkbuf_t *pkbuf, nas_eps_update_result_t *eps_update_result);
CORE_DECLARE(c_int16_t) nas_encode_eps_update_type(pkbuf_t *pkbuf, nas_eps_update_type_t *eps_update_type);
CORE_DECLARE(c_int16_t) nas_encode_esm_message_container(pkbuf_t *pkbuf, nas_esm_message_container_t *esm_message_container);
CORE_DECLARE(c_int16_t) nas_encode_gprs_timer(pkbuf_t *pkbuf, nas_gprs_timer_t *gprs_timer);
CORE_DECLARE(c_int16_t) nas_encode_gprs_timer_2(pkbuf_t *pkbuf, nas_gprs_timer_2_t *gprs_timer_2);
CORE_DECLARE(c_int16_t) nas_encode_gprs_timer_3(pkbuf_t *pkbuf, nas_gprs_timer_3_t *gprs_timer_3);
CORE_DECLARE(c_int16_t) nas_encode_identity_type_2(pkbuf_t *pkbuf, nas_identity_type_2_t *identity_type_2);
CORE_DECLARE(c_int16_t) nas_encode_imeisv_request(pkbuf_t *pkbuf, nas_imeisv_request_t *imeisv_request);
CORE_DECLARE(c_int16_t) nas_encode_ksi_and_sequence_number(pkbuf_t *pkbuf, nas_ksi_and_sequence_number_t *ksi_and_sequence_number);
CORE_DECLARE(c_int16_t) nas_encode_authentication_parameter_autn(pkbuf_t *pkbuf, nas_authentication_parameter_autn_t *authentication_parameter_autn);
CORE_DECLARE(c_int16_t) nas_encode_ms_network_capability(pkbuf_t *pkbuf, nas_ms_network_capability_t *ms_network_capability);
CORE_DECLARE(c_int16_t) nas_encode_ms_network_feature_support(pkbuf_t *pkbuf, nas_ms_network_feature_support_t *ms_network_feature_support);
CORE_DECLARE(c_int16_t) nas_encode_key_set_identifier(pkbuf_t *pkbuf, nas_key_set_identifier_t *key_set_identifier);
CORE_DECLARE(c_int16_t) nas_encode_message_container(pkbuf_t *pkbuf, nas_message_container_t *message_container);
CORE_DECLARE(c_int16_t) nas_encode_security_algorithms(pkbuf_t *pkbuf, nas_security_algorithms_t *security_algorithms);
CORE_DECLARE(c_int16_t) nas_encode_network_name(pkbuf_t *pkbuf, nas_network_name_t *network_name);
CORE_DECLARE(c_int16_t) nas_encode_network_resource_identifier_container(pkbuf_t *pkbuf, nas_network_resource_identifier_container_t *network_resource_identifier_container);
CORE_DECLARE(c_int16_t) nas_encode_nonce(pkbuf_t *pkbuf, nas_nonce_t *nonce);
CORE_DECLARE(c_int16_t) nas_encode_paging_identity(pkbuf_t *pkbuf, nas_paging_identity_t *paging_identity);
CORE_DECLARE(c_int16_t) nas_encode_p_tmsi_signature(pkbuf_t *pkbuf, nas_p_tmsi_signature_t *p_tmsi_signature);
CORE_DECLARE(c_int16_t) nas_encode_extended_emm_cause(pkbuf_t *pkbuf, nas_extended_emm_cause_t *extended_emm_cause);
CORE_DECLARE(c_int16_t) nas_encode_service_type(pkbuf_t *pkbuf, nas_service_type_t *service_type);
CORE_DECLARE(c_int16_t) nas_encode_short_mac(pkbuf_t *pkbuf, nas_short_mac_t *short_mac);
CORE_DECLARE(c_int16_t) nas_encode_time_zone(pkbuf_t *pkbuf, nas_time_zone_t *time_zone);
CORE_DECLARE(c_int16_t) nas_encode_authentication_parameter_rand(pkbuf_t *pkbuf, nas_authentication_parameter_rand_t *authentication_parameter_rand);
CORE_DECLARE(c_int16_t) nas_encode_time_zone_and_time(pkbuf_t *pkbuf, nas_time_zone_and_time_t *time_zone_and_time);
CORE_DECLARE(c_int16_t) nas_encode_tmsi_status(pkbuf_t *pkbuf, nas_tmsi_status_t *tmsi_status);
CORE_DECLARE(c_int16_t) nas_encode_tracking_area_identity(pkbuf_t *pkbuf, nas_tracking_area_identity_t *tracking_area_identity);
CORE_DECLARE(c_int16_t) nas_encode_tracking_area_identity_list(pkbuf_t *pkbuf, nas_tracking_area_identity_list_t *tracking_area_identity_list);
CORE_DECLARE(c_int16_t) nas_encode_ue_network_capability(pkbuf_t *pkbuf, nas_ue_network_capability_t *ue_network_capability);
CORE_DECLARE(c_int16_t) nas_encode_ue_radio_capability_information_update_needed(pkbuf_t *pkbuf, nas_ue_radio_capability_information_update_needed_t *ue_radio_capability_information_update_needed);
CORE_DECLARE(c_int16_t) nas_encode_ue_security_capability(pkbuf_t *pkbuf, nas_ue_security_capability_t *ue_security_capability);
CORE_DECLARE(c_int16_t) nas_encode_emergency_number_list(pkbuf_t *pkbuf, nas_emergency_number_list_t *emergency_number_list);
CORE_DECLARE(c_int16_t) nas_encode_cli(pkbuf_t *pkbuf, nas_cli_t *cli);
CORE_DECLARE(c_int16_t) nas_encode_ss_code(pkbuf_t *pkbuf, nas_ss_code_t *ss_code);
CORE_DECLARE(c_int16_t) nas_encode_authentication_response_parameter(pkbuf_t *pkbuf, nas_authentication_response_parameter_t *authentication_response_parameter);
CORE_DECLARE(c_int16_t) nas_encode_lcs_indicator(pkbuf_t *pkbuf, nas_lcs_indicator_t *lcs_indicator);
CORE_DECLARE(c_int16_t) nas_encode_lcs_client_identity(pkbuf_t *pkbuf, nas_lcs_client_identity_t *lcs_client_identity);
CORE_DECLARE(c_int16_t) nas_encode_generic_message_container_type(pkbuf_t *pkbuf, nas_generic_message_container_type_t *generic_message_container_type);
CORE_DECLARE(c_int16_t) nas_encode_generic_message_container(pkbuf_t *pkbuf, nas_generic_message_container_t *generic_message_container);
CORE_DECLARE(c_int16_t) nas_encode_voice_domain_preference_and_ue_usage_setting(pkbuf_t *pkbuf, nas_voice_domain_preference_and_ue_usage_setting_t *voice_domain_preference_and_ue_usage_setting);
CORE_DECLARE(c_int16_t) nas_encode_guti_type(pkbuf_t *pkbuf, nas_guti_type_t *guti_type);
CORE_DECLARE(c_int16_t) nas_encode_extended_drx_parameters(pkbuf_t *pkbuf, nas_extended_drx_parameters_t *extended_drx_parameters);
CORE_DECLARE(c_int16_t) nas_encode_ciphering_key_sequence_number(pkbuf_t *pkbuf, nas_ciphering_key_sequence_number_t *ciphering_key_sequence_number);
CORE_DECLARE(c_int16_t) nas_encode_csfb_response(pkbuf_t *pkbuf, nas_csfb_response_t *csfb_response);
CORE_DECLARE(c_int16_t) nas_encode_daylight_saving_time(pkbuf_t *pkbuf, nas_daylight_saving_time_t *daylight_saving_time);
CORE_DECLARE(c_int16_t) nas_encode_detach_type(pkbuf_t *pkbuf, nas_detach_type_t *detach_type);
CORE_DECLARE(c_int16_t) nas_encode_drx_parameter(pkbuf_t *pkbuf, nas_drx_parameter_t *drx_parameter);
CORE_DECLARE(c_int16_t) nas_encode_emm_cause(pkbuf_t *pkbuf, nas_emm_cause_t *emm_cause);
CORE_DECLARE(c_int16_t) nas_encode_access_point_name(pkbuf_t *pkbuf, nas_access_point_name_t *access_point_name);
CORE_DECLARE(c_int16_t) nas_encode_protocol_configuration_options(pkbuf_t *pkbuf, nas_protocol_configuration_options_t *protocol_configuration_options);
CORE_DECLARE(c_int16_t) nas_encode_quality_of_service(pkbuf_t *pkbuf, nas_quality_of_service_t *quality_of_service);
CORE_DECLARE(c_int16_t) nas_encode_radio_priority(pkbuf_t *pkbuf, nas_radio_priority_t *radio_priority);
CORE_DECLARE(c_int16_t) nas_encode_re_attempt_indicator(pkbuf_t *pkbuf, nas_re_attempt_indicator_t *re_attempt_indicator);
CORE_DECLARE(c_int16_t) nas_encode_request_type(pkbuf_t *pkbuf, nas_request_type_t *request_type);
CORE_DECLARE(c_int16_t) nas_encode_traffic_flow_aggregate_description(pkbuf_t *pkbuf, nas_traffic_flow_aggregate_description_t *traffic_flow_aggregate_description);
CORE_DECLARE(c_int16_t) nas_encode_traffic_flow_template(pkbuf_t *pkbuf, nas_traffic_flow_template_t *traffic_flow_template);
CORE_DECLARE(c_int16_t) nas_encode_transaction_identifier(pkbuf_t *pkbuf, nas_transaction_identifier_t *transaction_identifier);
CORE_DECLARE(c_int16_t) nas_encode_wlan_offload_acceptability(pkbuf_t *pkbuf, nas_wlan_offload_acceptability_t *wlan_offload_acceptability);
CORE_DECLARE(c_int16_t) nas_encode_nbifom_container(pkbuf_t *pkbuf, nas_nbifom_container_t *nbifom_container);
CORE_DECLARE(c_int16_t) nas_encode_apn_aggregate_maximum_bit_rate(pkbuf_t *pkbuf, nas_apn_aggregate_maximum_bit_rate_t *apn_aggregate_maximum_bit_rate);
CORE_DECLARE(c_int16_t) nas_encode_header_compression_configuration(pkbuf_t *pkbuf, nas_header_compression_configuration_t *header_compression_configuration);
CORE_DECLARE(c_int16_t) nas_encode_control_plane_only_indication(pkbuf_t *pkbuf, nas_control_plane_only_indication_t *control_plane_only_indication);
CORE_DECLARE(c_int16_t) nas_encode_extended_protocol_configuration_options(pkbuf_t *pkbuf, nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options);
CORE_DECLARE(c_int16_t) nas_encode_header_compression_configuration_status(pkbuf_t *pkbuf, nas_header_compression_configuration_status_t *header_compression_configuration_status);
CORE_DECLARE(c_int16_t) nas_encode_serving_plmn_rate_control(pkbuf_t *pkbuf, nas_serving_plmn_rate_control_t *serving_plmn_rate_control);
CORE_DECLARE(c_int16_t) nas_encode_connectivity_type(pkbuf_t *pkbuf, nas_connectivity_type_t *connectivity_type);
CORE_DECLARE(c_int16_t) nas_encode_eps_quality_of_service(pkbuf_t *pkbuf, nas_eps_quality_of_service_t *eps_quality_of_service);
CORE_DECLARE(c_int16_t) nas_encode_esm_cause(pkbuf_t *pkbuf, nas_esm_cause_t *esm_cause);
CORE_DECLARE(c_int16_t) nas_encode_esm_information_transfer_flag(pkbuf_t *pkbuf, nas_esm_information_transfer_flag_t *esm_information_transfer_flag);
CORE_DECLARE(c_int16_t) nas_encode_linked_eps_bearer_identity(pkbuf_t *pkbuf, nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity);
CORE_DECLARE(c_int16_t) nas_encode_llc_service_access_point_identifier(pkbuf_t *pkbuf, nas_llc_service_access_point_identifier_t *llc_service_access_point_identifier);
CORE_DECLARE(c_int16_t) nas_encode_packet_flow_identifier(pkbuf_t *pkbuf, nas_packet_flow_identifier_t *packet_flow_identifier);
CORE_DECLARE(c_int16_t) nas_encode_pdn_address(pkbuf_t *pkbuf, nas_pdn_address_t *pdn_address);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAS_IES_H__ */

