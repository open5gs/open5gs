/*
 * The MIT License
 *
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*******************************************************************************
 * This file had been created by nas-message.py script v0.1.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2020-04-24 08:50:31.627584 by acetcom
 * from 24301-d80.docx
 ******************************************************************************/

#if !defined(OGS_NAS_INSIDE) && !defined(OGS_NAS_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_NAS_IES_H
#define OGS_NAS_IES_H

#ifdef __cplusplus
extern "C" {
#endif

int ogs_nas_encode_optional_type(ogs_pkbuf_t *pkbuf, uint8_t type);

int ogs_nas_decode_additional_information(ogs_nas_additional_information_t *additional_information, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_device_properties(ogs_nas_device_properties_t *device_properties, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_eps_bearer_context_status(ogs_nas_eps_bearer_context_status_t *eps_bearer_context_status, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_supported_codec_list(ogs_nas_supported_codec_list_t *supported_codec_list, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_location_area_identification(ogs_nas_location_area_identification_t *location_area_identification, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_mobile_identity(ogs_nas_mobile_identity_t *mobile_identity, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_mobile_station_classmark_2(ogs_nas_mobile_station_classmark_2_t *mobile_station_classmark_2, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_mobile_station_classmark_3(ogs_nas_mobile_station_classmark_3_t *mobile_station_classmark_3, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_plmn_list(ogs_nas_plmn_list_t *plmn_list, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_additional_update_result(ogs_nas_additional_update_result_t *additional_update_result, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_additional_update_type(ogs_nas_additional_update_type_t *additional_update_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_authentication_failure_parameter(ogs_nas_authentication_failure_parameter_t *authentication_failure_parameter, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_eps_attach_result(ogs_nas_eps_attach_result_t *eps_attach_result, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_eps_attach_type(ogs_nas_eps_attach_type_t *eps_attach_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_eps_mobile_identity(ogs_nas_eps_mobile_identity_t *eps_mobile_identity, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_eps_network_feature_support(ogs_nas_eps_network_feature_support_t *eps_network_feature_support, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_eps_update_result(ogs_nas_eps_update_result_t *eps_update_result, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_eps_update_type(ogs_nas_eps_update_type_t *eps_update_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_esm_message_container(ogs_nas_esm_message_container_t *esm_message_container, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_gprs_timer(ogs_nas_gprs_timer_t *gprs_timer, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_gprs_timer_2(ogs_nas_gprs_timer_2_t *gprs_timer_2, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_gprs_timer_3(ogs_nas_gprs_timer_3_t *gprs_timer_3, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_identity_type_2(ogs_nas_identity_type_2_t *identity_type_2, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_imeisv_request(ogs_nas_imeisv_request_t *imeisv_request, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_ksi_and_sequence_number(ogs_nas_ksi_and_sequence_number_t *ksi_and_sequence_number, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_authentication_parameter_autn(ogs_nas_authentication_parameter_autn_t *authentication_parameter_autn, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_ms_network_capability(ogs_nas_ms_network_capability_t *ms_network_capability, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_ms_network_feature_support(ogs_nas_ms_network_feature_support_t *ms_network_feature_support, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_key_set_identifier(ogs_nas_key_set_identifier_t *key_set_identifier, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_message_container(ogs_nas_message_container_t *message_container, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_security_algorithms(ogs_nas_security_algorithms_t *security_algorithms, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_network_name(ogs_nas_network_name_t *network_name, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_network_resource_identifier_container(ogs_nas_network_resource_identifier_container_t *network_resource_identifier_container, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_nonce(ogs_nas_nonce_t *nonce, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_paging_identity(ogs_nas_paging_identity_t *paging_identity, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_p_tmsi_signature(ogs_nas_p_tmsi_signature_t *p_tmsi_signature, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_extended_emm_cause(ogs_nas_extended_emm_cause_t *extended_emm_cause, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_service_type(ogs_nas_service_type_t *service_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_short_mac(ogs_nas_short_mac_t *short_mac, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_time_zone(ogs_nas_time_zone_t *time_zone, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_authentication_parameter_rand(ogs_nas_authentication_parameter_rand_t *authentication_parameter_rand, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_time_zone_and_time(ogs_nas_time_zone_and_time_t *time_zone_and_time, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_tmsi_status(ogs_nas_tmsi_status_t *tmsi_status, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_tracking_area_identity(ogs_nas_tracking_area_identity_t *tracking_area_identity, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_tracking_area_identity_list(ogs_nas_tracking_area_identity_list_t *tracking_area_identity_list, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_ue_network_capability(ogs_nas_ue_network_capability_t *ue_network_capability, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_ue_radio_capability_information_update_needed(ogs_nas_ue_radio_capability_information_update_needed_t *ue_radio_capability_information_update_needed, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_ue_security_capability(ogs_nas_ue_security_capability_t *ue_security_capability, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_emergency_number_list(ogs_nas_emergency_number_list_t *emergency_number_list, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_cli(ogs_nas_cli_t *cli, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_ss_code(ogs_nas_ss_code_t *ss_code, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_authentication_response_parameter(ogs_nas_authentication_response_parameter_t *authentication_response_parameter, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_lcs_indicator(ogs_nas_lcs_indicator_t *lcs_indicator, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_lcs_client_identity(ogs_nas_lcs_client_identity_t *lcs_client_identity, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_generic_message_container_type(ogs_nas_generic_message_container_type_t *generic_message_container_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_generic_message_container(ogs_nas_generic_message_container_t *generic_message_container, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_voice_domain_preference_and_ue_usage_setting(ogs_nas_voice_domain_preference_and_ue_usage_setting_t *voice_domain_preference_and_ue_usage_setting, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_guti_type(ogs_nas_guti_type_t *guti_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_extended_drx_parameters(ogs_nas_extended_drx_parameters_t *extended_drx_parameters, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_ciphering_key_sequence_number(ogs_nas_ciphering_key_sequence_number_t *ciphering_key_sequence_number, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_csfb_response(ogs_nas_csfb_response_t *csfb_response, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_daylight_saving_time(ogs_nas_daylight_saving_time_t *daylight_saving_time, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_detach_type(ogs_nas_detach_type_t *detach_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_drx_parameter(ogs_nas_drx_parameter_t *drx_parameter, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_emm_cause(ogs_nas_emm_cause_t *emm_cause, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_access_point_name(ogs_nas_access_point_name_t *access_point_name, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_protocol_configuration_options(ogs_nas_protocol_configuration_options_t *protocol_configuration_options, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_quality_of_service(ogs_nas_quality_of_service_t *quality_of_service, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_radio_priority(ogs_nas_radio_priority_t *radio_priority, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_re_attempt_indicator(ogs_nas_re_attempt_indicator_t *re_attempt_indicator, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_request_type(ogs_nas_request_type_t *request_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_traffic_flow_aggregate_description(ogs_nas_traffic_flow_aggregate_description_t *traffic_flow_aggregate_description, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_traffic_flow_template(ogs_nas_traffic_flow_template_t *traffic_flow_template, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_transaction_identifier(ogs_nas_transaction_identifier_t *transaction_identifier, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_wlan_offload_acceptability(ogs_nas_wlan_offload_acceptability_t *wlan_offload_acceptability, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_nbifom_container(ogs_nas_nbifom_container_t *nbifom_container, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_apn_aggregate_maximum_bit_rate(ogs_nas_apn_aggregate_maximum_bit_rate_t *apn_aggregate_maximum_bit_rate, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_header_compression_configuration(ogs_nas_header_compression_configuration_t *header_compression_configuration, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_control_plane_only_indication(ogs_nas_control_plane_only_indication_t *control_plane_only_indication, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_extended_protocol_configuration_options(ogs_nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_header_compression_configuration_status(ogs_nas_header_compression_configuration_status_t *header_compression_configuration_status, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_serving_plmn_rate_control(ogs_nas_serving_plmn_rate_control_t *serving_plmn_rate_control, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_connectivity_type(ogs_nas_connectivity_type_t *connectivity_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_eps_quality_of_service(ogs_nas_eps_quality_of_service_t *eps_quality_of_service, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_esm_cause(ogs_nas_esm_cause_t *esm_cause, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_esm_information_transfer_flag(ogs_nas_esm_information_transfer_flag_t *esm_information_transfer_flag, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_linked_eps_bearer_identity(ogs_nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_llc_service_access_point_identifier(ogs_nas_llc_service_access_point_identifier_t *llc_service_access_point_identifier, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_packet_flow_identifier(ogs_nas_packet_flow_identifier_t *packet_flow_identifier, ogs_pkbuf_t *pkbuf);
int ogs_nas_decode_pdn_address(ogs_nas_pdn_address_t *pdn_address, ogs_pkbuf_t *pkbuf);

int ogs_nas_encode_additional_information(ogs_pkbuf_t *pkbuf, ogs_nas_additional_information_t *additional_information);
int ogs_nas_encode_device_properties(ogs_pkbuf_t *pkbuf, ogs_nas_device_properties_t *device_properties);
int ogs_nas_encode_eps_bearer_context_status(ogs_pkbuf_t *pkbuf, ogs_nas_eps_bearer_context_status_t *eps_bearer_context_status);
int ogs_nas_encode_supported_codec_list(ogs_pkbuf_t *pkbuf, ogs_nas_supported_codec_list_t *supported_codec_list);
int ogs_nas_encode_location_area_identification(ogs_pkbuf_t *pkbuf, ogs_nas_location_area_identification_t *location_area_identification);
int ogs_nas_encode_mobile_identity(ogs_pkbuf_t *pkbuf, ogs_nas_mobile_identity_t *mobile_identity);
int ogs_nas_encode_mobile_station_classmark_2(ogs_pkbuf_t *pkbuf, ogs_nas_mobile_station_classmark_2_t *mobile_station_classmark_2);
int ogs_nas_encode_mobile_station_classmark_3(ogs_pkbuf_t *pkbuf, ogs_nas_mobile_station_classmark_3_t *mobile_station_classmark_3);
int ogs_nas_encode_plmn_list(ogs_pkbuf_t *pkbuf, ogs_nas_plmn_list_t *plmn_list);
int ogs_nas_encode_additional_update_result(ogs_pkbuf_t *pkbuf, ogs_nas_additional_update_result_t *additional_update_result);
int ogs_nas_encode_additional_update_type(ogs_pkbuf_t *pkbuf, ogs_nas_additional_update_type_t *additional_update_type);
int ogs_nas_encode_authentication_failure_parameter(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_failure_parameter_t *authentication_failure_parameter);
int ogs_nas_encode_eps_attach_result(ogs_pkbuf_t *pkbuf, ogs_nas_eps_attach_result_t *eps_attach_result);
int ogs_nas_encode_eps_attach_type(ogs_pkbuf_t *pkbuf, ogs_nas_eps_attach_type_t *eps_attach_type);
int ogs_nas_encode_eps_mobile_identity(ogs_pkbuf_t *pkbuf, ogs_nas_eps_mobile_identity_t *eps_mobile_identity);
int ogs_nas_encode_eps_network_feature_support(ogs_pkbuf_t *pkbuf, ogs_nas_eps_network_feature_support_t *eps_network_feature_support);
int ogs_nas_encode_eps_update_result(ogs_pkbuf_t *pkbuf, ogs_nas_eps_update_result_t *eps_update_result);
int ogs_nas_encode_eps_update_type(ogs_pkbuf_t *pkbuf, ogs_nas_eps_update_type_t *eps_update_type);
int ogs_nas_encode_esm_message_container(ogs_pkbuf_t *pkbuf, ogs_nas_esm_message_container_t *esm_message_container);
int ogs_nas_encode_gprs_timer(ogs_pkbuf_t *pkbuf, ogs_nas_gprs_timer_t *gprs_timer);
int ogs_nas_encode_gprs_timer_2(ogs_pkbuf_t *pkbuf, ogs_nas_gprs_timer_2_t *gprs_timer_2);
int ogs_nas_encode_gprs_timer_3(ogs_pkbuf_t *pkbuf, ogs_nas_gprs_timer_3_t *gprs_timer_3);
int ogs_nas_encode_identity_type_2(ogs_pkbuf_t *pkbuf, ogs_nas_identity_type_2_t *identity_type_2);
int ogs_nas_encode_imeisv_request(ogs_pkbuf_t *pkbuf, ogs_nas_imeisv_request_t *imeisv_request);
int ogs_nas_encode_ksi_and_sequence_number(ogs_pkbuf_t *pkbuf, ogs_nas_ksi_and_sequence_number_t *ksi_and_sequence_number);
int ogs_nas_encode_authentication_parameter_autn(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_parameter_autn_t *authentication_parameter_autn);
int ogs_nas_encode_ms_network_capability(ogs_pkbuf_t *pkbuf, ogs_nas_ms_network_capability_t *ms_network_capability);
int ogs_nas_encode_ms_network_feature_support(ogs_pkbuf_t *pkbuf, ogs_nas_ms_network_feature_support_t *ms_network_feature_support);
int ogs_nas_encode_key_set_identifier(ogs_pkbuf_t *pkbuf, ogs_nas_key_set_identifier_t *key_set_identifier);
int ogs_nas_encode_message_container(ogs_pkbuf_t *pkbuf, ogs_nas_message_container_t *message_container);
int ogs_nas_encode_security_algorithms(ogs_pkbuf_t *pkbuf, ogs_nas_security_algorithms_t *security_algorithms);
int ogs_nas_encode_network_name(ogs_pkbuf_t *pkbuf, ogs_nas_network_name_t *network_name);
int ogs_nas_encode_network_resource_identifier_container(ogs_pkbuf_t *pkbuf, ogs_nas_network_resource_identifier_container_t *network_resource_identifier_container);
int ogs_nas_encode_nonce(ogs_pkbuf_t *pkbuf, ogs_nas_nonce_t *nonce);
int ogs_nas_encode_paging_identity(ogs_pkbuf_t *pkbuf, ogs_nas_paging_identity_t *paging_identity);
int ogs_nas_encode_p_tmsi_signature(ogs_pkbuf_t *pkbuf, ogs_nas_p_tmsi_signature_t *p_tmsi_signature);
int ogs_nas_encode_extended_emm_cause(ogs_pkbuf_t *pkbuf, ogs_nas_extended_emm_cause_t *extended_emm_cause);
int ogs_nas_encode_service_type(ogs_pkbuf_t *pkbuf, ogs_nas_service_type_t *service_type);
int ogs_nas_encode_short_mac(ogs_pkbuf_t *pkbuf, ogs_nas_short_mac_t *short_mac);
int ogs_nas_encode_time_zone(ogs_pkbuf_t *pkbuf, ogs_nas_time_zone_t *time_zone);
int ogs_nas_encode_authentication_parameter_rand(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_parameter_rand_t *authentication_parameter_rand);
int ogs_nas_encode_time_zone_and_time(ogs_pkbuf_t *pkbuf, ogs_nas_time_zone_and_time_t *time_zone_and_time);
int ogs_nas_encode_tmsi_status(ogs_pkbuf_t *pkbuf, ogs_nas_tmsi_status_t *tmsi_status);
int ogs_nas_encode_tracking_area_identity(ogs_pkbuf_t *pkbuf, ogs_nas_tracking_area_identity_t *tracking_area_identity);
int ogs_nas_encode_tracking_area_identity_list(ogs_pkbuf_t *pkbuf, ogs_nas_tracking_area_identity_list_t *tracking_area_identity_list);
int ogs_nas_encode_ue_network_capability(ogs_pkbuf_t *pkbuf, ogs_nas_ue_network_capability_t *ue_network_capability);
int ogs_nas_encode_ue_radio_capability_information_update_needed(ogs_pkbuf_t *pkbuf, ogs_nas_ue_radio_capability_information_update_needed_t *ue_radio_capability_information_update_needed);
int ogs_nas_encode_ue_security_capability(ogs_pkbuf_t *pkbuf, ogs_nas_ue_security_capability_t *ue_security_capability);
int ogs_nas_encode_emergency_number_list(ogs_pkbuf_t *pkbuf, ogs_nas_emergency_number_list_t *emergency_number_list);
int ogs_nas_encode_cli(ogs_pkbuf_t *pkbuf, ogs_nas_cli_t *cli);
int ogs_nas_encode_ss_code(ogs_pkbuf_t *pkbuf, ogs_nas_ss_code_t *ss_code);
int ogs_nas_encode_authentication_response_parameter(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_response_parameter_t *authentication_response_parameter);
int ogs_nas_encode_lcs_indicator(ogs_pkbuf_t *pkbuf, ogs_nas_lcs_indicator_t *lcs_indicator);
int ogs_nas_encode_lcs_client_identity(ogs_pkbuf_t *pkbuf, ogs_nas_lcs_client_identity_t *lcs_client_identity);
int ogs_nas_encode_generic_message_container_type(ogs_pkbuf_t *pkbuf, ogs_nas_generic_message_container_type_t *generic_message_container_type);
int ogs_nas_encode_generic_message_container(ogs_pkbuf_t *pkbuf, ogs_nas_generic_message_container_t *generic_message_container);
int ogs_nas_encode_voice_domain_preference_and_ue_usage_setting(ogs_pkbuf_t *pkbuf, ogs_nas_voice_domain_preference_and_ue_usage_setting_t *voice_domain_preference_and_ue_usage_setting);
int ogs_nas_encode_guti_type(ogs_pkbuf_t *pkbuf, ogs_nas_guti_type_t *guti_type);
int ogs_nas_encode_extended_drx_parameters(ogs_pkbuf_t *pkbuf, ogs_nas_extended_drx_parameters_t *extended_drx_parameters);
int ogs_nas_encode_ciphering_key_sequence_number(ogs_pkbuf_t *pkbuf, ogs_nas_ciphering_key_sequence_number_t *ciphering_key_sequence_number);
int ogs_nas_encode_csfb_response(ogs_pkbuf_t *pkbuf, ogs_nas_csfb_response_t *csfb_response);
int ogs_nas_encode_daylight_saving_time(ogs_pkbuf_t *pkbuf, ogs_nas_daylight_saving_time_t *daylight_saving_time);
int ogs_nas_encode_detach_type(ogs_pkbuf_t *pkbuf, ogs_nas_detach_type_t *detach_type);
int ogs_nas_encode_drx_parameter(ogs_pkbuf_t *pkbuf, ogs_nas_drx_parameter_t *drx_parameter);
int ogs_nas_encode_emm_cause(ogs_pkbuf_t *pkbuf, ogs_nas_emm_cause_t *emm_cause);
int ogs_nas_encode_access_point_name(ogs_pkbuf_t *pkbuf, ogs_nas_access_point_name_t *access_point_name);
int ogs_nas_encode_protocol_configuration_options(ogs_pkbuf_t *pkbuf, ogs_nas_protocol_configuration_options_t *protocol_configuration_options);
int ogs_nas_encode_quality_of_service(ogs_pkbuf_t *pkbuf, ogs_nas_quality_of_service_t *quality_of_service);
int ogs_nas_encode_radio_priority(ogs_pkbuf_t *pkbuf, ogs_nas_radio_priority_t *radio_priority);
int ogs_nas_encode_re_attempt_indicator(ogs_pkbuf_t *pkbuf, ogs_nas_re_attempt_indicator_t *re_attempt_indicator);
int ogs_nas_encode_request_type(ogs_pkbuf_t *pkbuf, ogs_nas_request_type_t *request_type);
int ogs_nas_encode_traffic_flow_aggregate_description(ogs_pkbuf_t *pkbuf, ogs_nas_traffic_flow_aggregate_description_t *traffic_flow_aggregate_description);
int ogs_nas_encode_traffic_flow_template(ogs_pkbuf_t *pkbuf, ogs_nas_traffic_flow_template_t *traffic_flow_template);
int ogs_nas_encode_transaction_identifier(ogs_pkbuf_t *pkbuf, ogs_nas_transaction_identifier_t *transaction_identifier);
int ogs_nas_encode_wlan_offload_acceptability(ogs_pkbuf_t *pkbuf, ogs_nas_wlan_offload_acceptability_t *wlan_offload_acceptability);
int ogs_nas_encode_nbifom_container(ogs_pkbuf_t *pkbuf, ogs_nas_nbifom_container_t *nbifom_container);
int ogs_nas_encode_apn_aggregate_maximum_bit_rate(ogs_pkbuf_t *pkbuf, ogs_nas_apn_aggregate_maximum_bit_rate_t *apn_aggregate_maximum_bit_rate);
int ogs_nas_encode_header_compression_configuration(ogs_pkbuf_t *pkbuf, ogs_nas_header_compression_configuration_t *header_compression_configuration);
int ogs_nas_encode_control_plane_only_indication(ogs_pkbuf_t *pkbuf, ogs_nas_control_plane_only_indication_t *control_plane_only_indication);
int ogs_nas_encode_extended_protocol_configuration_options(ogs_pkbuf_t *pkbuf, ogs_nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options);
int ogs_nas_encode_header_compression_configuration_status(ogs_pkbuf_t *pkbuf, ogs_nas_header_compression_configuration_status_t *header_compression_configuration_status);
int ogs_nas_encode_serving_plmn_rate_control(ogs_pkbuf_t *pkbuf, ogs_nas_serving_plmn_rate_control_t *serving_plmn_rate_control);
int ogs_nas_encode_connectivity_type(ogs_pkbuf_t *pkbuf, ogs_nas_connectivity_type_t *connectivity_type);
int ogs_nas_encode_eps_quality_of_service(ogs_pkbuf_t *pkbuf, ogs_nas_eps_quality_of_service_t *eps_quality_of_service);
int ogs_nas_encode_esm_cause(ogs_pkbuf_t *pkbuf, ogs_nas_esm_cause_t *esm_cause);
int ogs_nas_encode_esm_information_transfer_flag(ogs_pkbuf_t *pkbuf, ogs_nas_esm_information_transfer_flag_t *esm_information_transfer_flag);
int ogs_nas_encode_linked_eps_bearer_identity(ogs_pkbuf_t *pkbuf, ogs_nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity);
int ogs_nas_encode_llc_service_access_point_identifier(ogs_pkbuf_t *pkbuf, ogs_nas_llc_service_access_point_identifier_t *llc_service_access_point_identifier);
int ogs_nas_encode_packet_flow_identifier(ogs_pkbuf_t *pkbuf, ogs_nas_packet_flow_identifier_t *packet_flow_identifier);
int ogs_nas_encode_pdn_address(ogs_pkbuf_t *pkbuf, ogs_nas_pdn_address_t *pdn_address);

#ifdef __cplusplus
}
#endif

#endif /* OGS_NAS_IES_H */

