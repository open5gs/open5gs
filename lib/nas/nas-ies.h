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
 * This file had been created by nas-message.py script v0.1.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2019-06-21 14:10:51.884139 by acetcom
 * from 24301-d80.docx
 ******************************************************************************/

#ifndef NAS_IES_H
#define NAS_IES_H

#include "nas-types.h"

#ifdef __cplusplus
extern "C" {
#endif

int nas_encode_optional_type(ogs_pkbuf_t *pkbuf, uint8_t type);

int nas_decode_additional_information(nas_additional_information_t *additional_information, ogs_pkbuf_t *pkbuf);
int nas_decode_device_properties(nas_device_properties_t *device_properties, ogs_pkbuf_t *pkbuf);
int nas_decode_eps_bearer_context_status(nas_eps_bearer_context_status_t *eps_bearer_context_status, ogs_pkbuf_t *pkbuf);
int nas_decode_supported_codec_list(nas_supported_codec_list_t *supported_codec_list, ogs_pkbuf_t *pkbuf);
int nas_decode_location_area_identification(nas_location_area_identification_t *location_area_identification, ogs_pkbuf_t *pkbuf);
int nas_decode_mobile_identity(nas_mobile_identity_t *mobile_identity, ogs_pkbuf_t *pkbuf);
int nas_decode_mobile_station_classmark_2(nas_mobile_station_classmark_2_t *mobile_station_classmark_2, ogs_pkbuf_t *pkbuf);
int nas_decode_mobile_station_classmark_3(nas_mobile_station_classmark_3_t *mobile_station_classmark_3, ogs_pkbuf_t *pkbuf);
int nas_decode_plmn_list(nas_plmn_list_t *plmn_list, ogs_pkbuf_t *pkbuf);
int nas_decode_additional_update_result(nas_additional_update_result_t *additional_update_result, ogs_pkbuf_t *pkbuf);
int nas_decode_additional_update_type(nas_additional_update_type_t *additional_update_type, ogs_pkbuf_t *pkbuf);
int nas_decode_authentication_failure_parameter(nas_authentication_failure_parameter_t *authentication_failure_parameter, ogs_pkbuf_t *pkbuf);
int nas_decode_eps_attach_result(nas_eps_attach_result_t *eps_attach_result, ogs_pkbuf_t *pkbuf);
int nas_decode_eps_attach_type(nas_eps_attach_type_t *eps_attach_type, ogs_pkbuf_t *pkbuf);
int nas_decode_eps_mobile_identity(nas_eps_mobile_identity_t *eps_mobile_identity, ogs_pkbuf_t *pkbuf);
int nas_decode_eps_network_feature_support(nas_eps_network_feature_support_t *eps_network_feature_support, ogs_pkbuf_t *pkbuf);
int nas_decode_eps_update_result(nas_eps_update_result_t *eps_update_result, ogs_pkbuf_t *pkbuf);
int nas_decode_eps_update_type(nas_eps_update_type_t *eps_update_type, ogs_pkbuf_t *pkbuf);
int nas_decode_esm_message_container(nas_esm_message_container_t *esm_message_container, ogs_pkbuf_t *pkbuf);
int nas_decode_gprs_timer(nas_gprs_timer_t *gprs_timer, ogs_pkbuf_t *pkbuf);
int nas_decode_gprs_timer_2(nas_gprs_timer_2_t *gprs_timer_2, ogs_pkbuf_t *pkbuf);
int nas_decode_gprs_timer_3(nas_gprs_timer_3_t *gprs_timer_3, ogs_pkbuf_t *pkbuf);
int nas_decode_identity_type_2(nas_identity_type_2_t *identity_type_2, ogs_pkbuf_t *pkbuf);
int nas_decode_imeisv_request(nas_imeisv_request_t *imeisv_request, ogs_pkbuf_t *pkbuf);
int nas_decode_ksi_and_sequence_number(nas_ksi_and_sequence_number_t *ksi_and_sequence_number, ogs_pkbuf_t *pkbuf);
int nas_decode_authentication_parameter_autn(nas_authentication_parameter_autn_t *authentication_parameter_autn, ogs_pkbuf_t *pkbuf);
int nas_decode_ms_network_capability(nas_ms_network_capability_t *ms_network_capability, ogs_pkbuf_t *pkbuf);
int nas_decode_ms_network_feature_support(nas_ms_network_feature_support_t *ms_network_feature_support, ogs_pkbuf_t *pkbuf);
int nas_decode_key_set_identifier(nas_key_set_identifier_t *key_set_identifier, ogs_pkbuf_t *pkbuf);
int nas_decode_message_container(nas_message_container_t *message_container, ogs_pkbuf_t *pkbuf);
int nas_decode_security_algorithms(nas_security_algorithms_t *security_algorithms, ogs_pkbuf_t *pkbuf);
int nas_decode_network_name(nas_network_name_t *network_name, ogs_pkbuf_t *pkbuf);
int nas_decode_network_resource_identifier_container(nas_network_resource_identifier_container_t *network_resource_identifier_container, ogs_pkbuf_t *pkbuf);
int nas_decode_nonce(nas_nonce_t *nonce, ogs_pkbuf_t *pkbuf);
int nas_decode_paging_identity(nas_paging_identity_t *paging_identity, ogs_pkbuf_t *pkbuf);
int nas_decode_p_tmsi_signature(nas_p_tmsi_signature_t *p_tmsi_signature, ogs_pkbuf_t *pkbuf);
int nas_decode_extended_emm_cause(nas_extended_emm_cause_t *extended_emm_cause, ogs_pkbuf_t *pkbuf);
int nas_decode_service_type(nas_service_type_t *service_type, ogs_pkbuf_t *pkbuf);
int nas_decode_short_mac(nas_short_mac_t *short_mac, ogs_pkbuf_t *pkbuf);
int nas_decode_time_zone(nas_time_zone_t *time_zone, ogs_pkbuf_t *pkbuf);
int nas_decode_authentication_parameter_rand(nas_authentication_parameter_rand_t *authentication_parameter_rand, ogs_pkbuf_t *pkbuf);
int nas_decode_time_zone_and_time(nas_time_zone_and_time_t *time_zone_and_time, ogs_pkbuf_t *pkbuf);
int nas_decode_tmsi_status(nas_tmsi_status_t *tmsi_status, ogs_pkbuf_t *pkbuf);
int nas_decode_tracking_area_identity(nas_tracking_area_identity_t *tracking_area_identity, ogs_pkbuf_t *pkbuf);
int nas_decode_tracking_area_identity_list(nas_tracking_area_identity_list_t *tracking_area_identity_list, ogs_pkbuf_t *pkbuf);
int nas_decode_ue_network_capability(nas_ue_network_capability_t *ue_network_capability, ogs_pkbuf_t *pkbuf);
int nas_decode_ue_radio_capability_information_update_needed(nas_ue_radio_capability_information_update_needed_t *ue_radio_capability_information_update_needed, ogs_pkbuf_t *pkbuf);
int nas_decode_ue_security_capability(nas_ue_security_capability_t *ue_security_capability, ogs_pkbuf_t *pkbuf);
int nas_decode_emergency_number_list(nas_emergency_number_list_t *emergency_number_list, ogs_pkbuf_t *pkbuf);
int nas_decode_cli(nas_cli_t *cli, ogs_pkbuf_t *pkbuf);
int nas_decode_ss_code(nas_ss_code_t *ss_code, ogs_pkbuf_t *pkbuf);
int nas_decode_authentication_response_parameter(nas_authentication_response_parameter_t *authentication_response_parameter, ogs_pkbuf_t *pkbuf);
int nas_decode_lcs_indicator(nas_lcs_indicator_t *lcs_indicator, ogs_pkbuf_t *pkbuf);
int nas_decode_lcs_client_identity(nas_lcs_client_identity_t *lcs_client_identity, ogs_pkbuf_t *pkbuf);
int nas_decode_generic_message_container_type(nas_generic_message_container_type_t *generic_message_container_type, ogs_pkbuf_t *pkbuf);
int nas_decode_generic_message_container(nas_generic_message_container_t *generic_message_container, ogs_pkbuf_t *pkbuf);
int nas_decode_voice_domain_preference_and_ue_usage_setting(nas_voice_domain_preference_and_ue_usage_setting_t *voice_domain_preference_and_ue_usage_setting, ogs_pkbuf_t *pkbuf);
int nas_decode_guti_type(nas_guti_type_t *guti_type, ogs_pkbuf_t *pkbuf);
int nas_decode_extended_drx_parameters(nas_extended_drx_parameters_t *extended_drx_parameters, ogs_pkbuf_t *pkbuf);
int nas_decode_ciphering_key_sequence_number(nas_ciphering_key_sequence_number_t *ciphering_key_sequence_number, ogs_pkbuf_t *pkbuf);
int nas_decode_csfb_response(nas_csfb_response_t *csfb_response, ogs_pkbuf_t *pkbuf);
int nas_decode_daylight_saving_time(nas_daylight_saving_time_t *daylight_saving_time, ogs_pkbuf_t *pkbuf);
int nas_decode_detach_type(nas_detach_type_t *detach_type, ogs_pkbuf_t *pkbuf);
int nas_decode_drx_parameter(nas_drx_parameter_t *drx_parameter, ogs_pkbuf_t *pkbuf);
int nas_decode_emm_cause(nas_emm_cause_t *emm_cause, ogs_pkbuf_t *pkbuf);
int nas_decode_access_point_name(nas_access_point_name_t *access_point_name, ogs_pkbuf_t *pkbuf);
int nas_decode_protocol_configuration_options(nas_protocol_configuration_options_t *protocol_configuration_options, ogs_pkbuf_t *pkbuf);
int nas_decode_quality_of_service(nas_quality_of_service_t *quality_of_service, ogs_pkbuf_t *pkbuf);
int nas_decode_radio_priority(nas_radio_priority_t *radio_priority, ogs_pkbuf_t *pkbuf);
int nas_decode_re_attempt_indicator(nas_re_attempt_indicator_t *re_attempt_indicator, ogs_pkbuf_t *pkbuf);
int nas_decode_request_type(nas_request_type_t *request_type, ogs_pkbuf_t *pkbuf);
int nas_decode_traffic_flow_aggregate_description(nas_traffic_flow_aggregate_description_t *traffic_flow_aggregate_description, ogs_pkbuf_t *pkbuf);
int nas_decode_traffic_flow_template(nas_traffic_flow_template_t *traffic_flow_template, ogs_pkbuf_t *pkbuf);
int nas_decode_transaction_identifier(nas_transaction_identifier_t *transaction_identifier, ogs_pkbuf_t *pkbuf);
int nas_decode_wlan_offload_acceptability(nas_wlan_offload_acceptability_t *wlan_offload_acceptability, ogs_pkbuf_t *pkbuf);
int nas_decode_nbifom_container(nas_nbifom_container_t *nbifom_container, ogs_pkbuf_t *pkbuf);
int nas_decode_apn_aggregate_maximum_bit_rate(nas_apn_aggregate_maximum_bit_rate_t *apn_aggregate_maximum_bit_rate, ogs_pkbuf_t *pkbuf);
int nas_decode_header_compression_configuration(nas_header_compression_configuration_t *header_compression_configuration, ogs_pkbuf_t *pkbuf);
int nas_decode_control_plane_only_indication(nas_control_plane_only_indication_t *control_plane_only_indication, ogs_pkbuf_t *pkbuf);
int nas_decode_extended_protocol_configuration_options(nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options, ogs_pkbuf_t *pkbuf);
int nas_decode_header_compression_configuration_status(nas_header_compression_configuration_status_t *header_compression_configuration_status, ogs_pkbuf_t *pkbuf);
int nas_decode_serving_plmn_rate_control(nas_serving_plmn_rate_control_t *serving_plmn_rate_control, ogs_pkbuf_t *pkbuf);
int nas_decode_connectivity_type(nas_connectivity_type_t *connectivity_type, ogs_pkbuf_t *pkbuf);
int nas_decode_eps_quality_of_service(nas_eps_quality_of_service_t *eps_quality_of_service, ogs_pkbuf_t *pkbuf);
int nas_decode_esm_cause(nas_esm_cause_t *esm_cause, ogs_pkbuf_t *pkbuf);
int nas_decode_esm_information_transfer_flag(nas_esm_information_transfer_flag_t *esm_information_transfer_flag, ogs_pkbuf_t *pkbuf);
int nas_decode_linked_eps_bearer_identity(nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity, ogs_pkbuf_t *pkbuf);
int nas_decode_llc_service_access_point_identifier(nas_llc_service_access_point_identifier_t *llc_service_access_point_identifier, ogs_pkbuf_t *pkbuf);
int nas_decode_packet_flow_identifier(nas_packet_flow_identifier_t *packet_flow_identifier, ogs_pkbuf_t *pkbuf);
int nas_decode_pdn_address(nas_pdn_address_t *pdn_address, ogs_pkbuf_t *pkbuf);

int nas_encode_additional_information(ogs_pkbuf_t *pkbuf, nas_additional_information_t *additional_information);
int nas_encode_device_properties(ogs_pkbuf_t *pkbuf, nas_device_properties_t *device_properties);
int nas_encode_eps_bearer_context_status(ogs_pkbuf_t *pkbuf, nas_eps_bearer_context_status_t *eps_bearer_context_status);
int nas_encode_supported_codec_list(ogs_pkbuf_t *pkbuf, nas_supported_codec_list_t *supported_codec_list);
int nas_encode_location_area_identification(ogs_pkbuf_t *pkbuf, nas_location_area_identification_t *location_area_identification);
int nas_encode_mobile_identity(ogs_pkbuf_t *pkbuf, nas_mobile_identity_t *mobile_identity);
int nas_encode_mobile_station_classmark_2(ogs_pkbuf_t *pkbuf, nas_mobile_station_classmark_2_t *mobile_station_classmark_2);
int nas_encode_mobile_station_classmark_3(ogs_pkbuf_t *pkbuf, nas_mobile_station_classmark_3_t *mobile_station_classmark_3);
int nas_encode_plmn_list(ogs_pkbuf_t *pkbuf, nas_plmn_list_t *plmn_list);
int nas_encode_additional_update_result(ogs_pkbuf_t *pkbuf, nas_additional_update_result_t *additional_update_result);
int nas_encode_additional_update_type(ogs_pkbuf_t *pkbuf, nas_additional_update_type_t *additional_update_type);
int nas_encode_authentication_failure_parameter(ogs_pkbuf_t *pkbuf, nas_authentication_failure_parameter_t *authentication_failure_parameter);
int nas_encode_eps_attach_result(ogs_pkbuf_t *pkbuf, nas_eps_attach_result_t *eps_attach_result);
int nas_encode_eps_attach_type(ogs_pkbuf_t *pkbuf, nas_eps_attach_type_t *eps_attach_type);
int nas_encode_eps_mobile_identity(ogs_pkbuf_t *pkbuf, nas_eps_mobile_identity_t *eps_mobile_identity);
int nas_encode_eps_network_feature_support(ogs_pkbuf_t *pkbuf, nas_eps_network_feature_support_t *eps_network_feature_support);
int nas_encode_eps_update_result(ogs_pkbuf_t *pkbuf, nas_eps_update_result_t *eps_update_result);
int nas_encode_eps_update_type(ogs_pkbuf_t *pkbuf, nas_eps_update_type_t *eps_update_type);
int nas_encode_esm_message_container(ogs_pkbuf_t *pkbuf, nas_esm_message_container_t *esm_message_container);
int nas_encode_gprs_timer(ogs_pkbuf_t *pkbuf, nas_gprs_timer_t *gprs_timer);
int nas_encode_gprs_timer_2(ogs_pkbuf_t *pkbuf, nas_gprs_timer_2_t *gprs_timer_2);
int nas_encode_gprs_timer_3(ogs_pkbuf_t *pkbuf, nas_gprs_timer_3_t *gprs_timer_3);
int nas_encode_identity_type_2(ogs_pkbuf_t *pkbuf, nas_identity_type_2_t *identity_type_2);
int nas_encode_imeisv_request(ogs_pkbuf_t *pkbuf, nas_imeisv_request_t *imeisv_request);
int nas_encode_ksi_and_sequence_number(ogs_pkbuf_t *pkbuf, nas_ksi_and_sequence_number_t *ksi_and_sequence_number);
int nas_encode_authentication_parameter_autn(ogs_pkbuf_t *pkbuf, nas_authentication_parameter_autn_t *authentication_parameter_autn);
int nas_encode_ms_network_capability(ogs_pkbuf_t *pkbuf, nas_ms_network_capability_t *ms_network_capability);
int nas_encode_ms_network_feature_support(ogs_pkbuf_t *pkbuf, nas_ms_network_feature_support_t *ms_network_feature_support);
int nas_encode_key_set_identifier(ogs_pkbuf_t *pkbuf, nas_key_set_identifier_t *key_set_identifier);
int nas_encode_message_container(ogs_pkbuf_t *pkbuf, nas_message_container_t *message_container);
int nas_encode_security_algorithms(ogs_pkbuf_t *pkbuf, nas_security_algorithms_t *security_algorithms);
int nas_encode_network_name(ogs_pkbuf_t *pkbuf, nas_network_name_t *network_name);
int nas_encode_network_resource_identifier_container(ogs_pkbuf_t *pkbuf, nas_network_resource_identifier_container_t *network_resource_identifier_container);
int nas_encode_nonce(ogs_pkbuf_t *pkbuf, nas_nonce_t *nonce);
int nas_encode_paging_identity(ogs_pkbuf_t *pkbuf, nas_paging_identity_t *paging_identity);
int nas_encode_p_tmsi_signature(ogs_pkbuf_t *pkbuf, nas_p_tmsi_signature_t *p_tmsi_signature);
int nas_encode_extended_emm_cause(ogs_pkbuf_t *pkbuf, nas_extended_emm_cause_t *extended_emm_cause);
int nas_encode_service_type(ogs_pkbuf_t *pkbuf, nas_service_type_t *service_type);
int nas_encode_short_mac(ogs_pkbuf_t *pkbuf, nas_short_mac_t *short_mac);
int nas_encode_time_zone(ogs_pkbuf_t *pkbuf, nas_time_zone_t *time_zone);
int nas_encode_authentication_parameter_rand(ogs_pkbuf_t *pkbuf, nas_authentication_parameter_rand_t *authentication_parameter_rand);
int nas_encode_time_zone_and_time(ogs_pkbuf_t *pkbuf, nas_time_zone_and_time_t *time_zone_and_time);
int nas_encode_tmsi_status(ogs_pkbuf_t *pkbuf, nas_tmsi_status_t *tmsi_status);
int nas_encode_tracking_area_identity(ogs_pkbuf_t *pkbuf, nas_tracking_area_identity_t *tracking_area_identity);
int nas_encode_tracking_area_identity_list(ogs_pkbuf_t *pkbuf, nas_tracking_area_identity_list_t *tracking_area_identity_list);
int nas_encode_ue_network_capability(ogs_pkbuf_t *pkbuf, nas_ue_network_capability_t *ue_network_capability);
int nas_encode_ue_radio_capability_information_update_needed(ogs_pkbuf_t *pkbuf, nas_ue_radio_capability_information_update_needed_t *ue_radio_capability_information_update_needed);
int nas_encode_ue_security_capability(ogs_pkbuf_t *pkbuf, nas_ue_security_capability_t *ue_security_capability);
int nas_encode_emergency_number_list(ogs_pkbuf_t *pkbuf, nas_emergency_number_list_t *emergency_number_list);
int nas_encode_cli(ogs_pkbuf_t *pkbuf, nas_cli_t *cli);
int nas_encode_ss_code(ogs_pkbuf_t *pkbuf, nas_ss_code_t *ss_code);
int nas_encode_authentication_response_parameter(ogs_pkbuf_t *pkbuf, nas_authentication_response_parameter_t *authentication_response_parameter);
int nas_encode_lcs_indicator(ogs_pkbuf_t *pkbuf, nas_lcs_indicator_t *lcs_indicator);
int nas_encode_lcs_client_identity(ogs_pkbuf_t *pkbuf, nas_lcs_client_identity_t *lcs_client_identity);
int nas_encode_generic_message_container_type(ogs_pkbuf_t *pkbuf, nas_generic_message_container_type_t *generic_message_container_type);
int nas_encode_generic_message_container(ogs_pkbuf_t *pkbuf, nas_generic_message_container_t *generic_message_container);
int nas_encode_voice_domain_preference_and_ue_usage_setting(ogs_pkbuf_t *pkbuf, nas_voice_domain_preference_and_ue_usage_setting_t *voice_domain_preference_and_ue_usage_setting);
int nas_encode_guti_type(ogs_pkbuf_t *pkbuf, nas_guti_type_t *guti_type);
int nas_encode_extended_drx_parameters(ogs_pkbuf_t *pkbuf, nas_extended_drx_parameters_t *extended_drx_parameters);
int nas_encode_ciphering_key_sequence_number(ogs_pkbuf_t *pkbuf, nas_ciphering_key_sequence_number_t *ciphering_key_sequence_number);
int nas_encode_csfb_response(ogs_pkbuf_t *pkbuf, nas_csfb_response_t *csfb_response);
int nas_encode_daylight_saving_time(ogs_pkbuf_t *pkbuf, nas_daylight_saving_time_t *daylight_saving_time);
int nas_encode_detach_type(ogs_pkbuf_t *pkbuf, nas_detach_type_t *detach_type);
int nas_encode_drx_parameter(ogs_pkbuf_t *pkbuf, nas_drx_parameter_t *drx_parameter);
int nas_encode_emm_cause(ogs_pkbuf_t *pkbuf, nas_emm_cause_t *emm_cause);
int nas_encode_access_point_name(ogs_pkbuf_t *pkbuf, nas_access_point_name_t *access_point_name);
int nas_encode_protocol_configuration_options(ogs_pkbuf_t *pkbuf, nas_protocol_configuration_options_t *protocol_configuration_options);
int nas_encode_quality_of_service(ogs_pkbuf_t *pkbuf, nas_quality_of_service_t *quality_of_service);
int nas_encode_radio_priority(ogs_pkbuf_t *pkbuf, nas_radio_priority_t *radio_priority);
int nas_encode_re_attempt_indicator(ogs_pkbuf_t *pkbuf, nas_re_attempt_indicator_t *re_attempt_indicator);
int nas_encode_request_type(ogs_pkbuf_t *pkbuf, nas_request_type_t *request_type);
int nas_encode_traffic_flow_aggregate_description(ogs_pkbuf_t *pkbuf, nas_traffic_flow_aggregate_description_t *traffic_flow_aggregate_description);
int nas_encode_traffic_flow_template(ogs_pkbuf_t *pkbuf, nas_traffic_flow_template_t *traffic_flow_template);
int nas_encode_transaction_identifier(ogs_pkbuf_t *pkbuf, nas_transaction_identifier_t *transaction_identifier);
int nas_encode_wlan_offload_acceptability(ogs_pkbuf_t *pkbuf, nas_wlan_offload_acceptability_t *wlan_offload_acceptability);
int nas_encode_nbifom_container(ogs_pkbuf_t *pkbuf, nas_nbifom_container_t *nbifom_container);
int nas_encode_apn_aggregate_maximum_bit_rate(ogs_pkbuf_t *pkbuf, nas_apn_aggregate_maximum_bit_rate_t *apn_aggregate_maximum_bit_rate);
int nas_encode_header_compression_configuration(ogs_pkbuf_t *pkbuf, nas_header_compression_configuration_t *header_compression_configuration);
int nas_encode_control_plane_only_indication(ogs_pkbuf_t *pkbuf, nas_control_plane_only_indication_t *control_plane_only_indication);
int nas_encode_extended_protocol_configuration_options(ogs_pkbuf_t *pkbuf, nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options);
int nas_encode_header_compression_configuration_status(ogs_pkbuf_t *pkbuf, nas_header_compression_configuration_status_t *header_compression_configuration_status);
int nas_encode_serving_plmn_rate_control(ogs_pkbuf_t *pkbuf, nas_serving_plmn_rate_control_t *serving_plmn_rate_control);
int nas_encode_connectivity_type(ogs_pkbuf_t *pkbuf, nas_connectivity_type_t *connectivity_type);
int nas_encode_eps_quality_of_service(ogs_pkbuf_t *pkbuf, nas_eps_quality_of_service_t *eps_quality_of_service);
int nas_encode_esm_cause(ogs_pkbuf_t *pkbuf, nas_esm_cause_t *esm_cause);
int nas_encode_esm_information_transfer_flag(ogs_pkbuf_t *pkbuf, nas_esm_information_transfer_flag_t *esm_information_transfer_flag);
int nas_encode_linked_eps_bearer_identity(ogs_pkbuf_t *pkbuf, nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity);
int nas_encode_llc_service_access_point_identifier(ogs_pkbuf_t *pkbuf, nas_llc_service_access_point_identifier_t *llc_service_access_point_identifier);
int nas_encode_packet_flow_identifier(ogs_pkbuf_t *pkbuf, nas_packet_flow_identifier_t *packet_flow_identifier);
int nas_encode_pdn_address(ogs_pkbuf_t *pkbuf, nas_pdn_address_t *pdn_address);

#ifdef __cplusplus
}
#endif

#endif /* NAS_IES_H */

