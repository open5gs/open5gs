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
 * This file had been created by nas-message.py script v0.2.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2020-06-30 08:13:45.429071 by acetcom
 * from 24501-g41.docx
 ******************************************************************************/

#if !defined(OGS_NAS_INSIDE) && !defined(OGS_NAS_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_NAS_5GS_IES_H
#define OGS_NAS_5GS_IES_H

#ifdef __cplusplus
extern "C" {
#endif

int ogs_nas_5gs_encode_optional_type(ogs_pkbuf_t *pkbuf, uint8_t type);

int ogs_nas_5gs_decode_additional_information(ogs_nas_additional_information_t *additional_information, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_access_type(ogs_nas_access_type_t *access_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_dnn(ogs_nas_dnn_t *dnn, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_eap_message(ogs_nas_eap_message_t *eap_message, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_gprs_timer(ogs_nas_gprs_timer_t *gprs_timer, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_gprs_timer_2(ogs_nas_gprs_timer_2_t *gprs_timer_2, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_gprs_timer_3(ogs_nas_gprs_timer_3_t *gprs_timer_3, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_s_nssai(ogs_nas_s_nssai_t *s_nssai, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gmm_capability(ogs_nas_5gmm_capability_t *gmm_capability, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_abba(ogs_nas_abba_t *abba, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_additional_5g_security_information(ogs_nas_additional_5g_security_information_t *additional_security_information, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_additional_information_requested(ogs_nas_additional_information_requested_t *additional_information_requested, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_allowed_pdu_session_status(ogs_nas_allowed_pdu_session_status_t *allowed_pdu_session_status, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_authentication_failure_parameter(ogs_nas_authentication_failure_parameter_t *authentication_failure_parameter, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_authentication_parameter_autn(ogs_nas_authentication_parameter_autn_t *authentication_parameter_autn, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_authentication_parameter_rand(ogs_nas_authentication_parameter_rand_t *authentication_parameter_rand, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_authentication_response_parameter(ogs_nas_authentication_response_parameter_t *authentication_response_parameter, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_configuration_update_indication(ogs_nas_configuration_update_indication_t *configuration_update_indication, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_cag_information_list(ogs_nas_cag_information_list_t *cag_information_list, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_ciphering_key_data(ogs_nas_ciphering_key_data_t *ciphering_key_data, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_daylight_saving_time(ogs_nas_daylight_saving_time_t *daylight_saving_time, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gmm_cause(ogs_nas_5gmm_cause_t *gmm_cause, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_de_registration_type(ogs_nas_de_registration_type_t *de_registration_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_emergency_number_list(ogs_nas_emergency_number_list_t *emergency_number_list, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_eps_bearer_context_status(ogs_nas_eps_bearer_context_status_t *eps_bearer_context_status, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_eps_nas_message_container(ogs_nas_eps_nas_message_container_t *eps_nas_message_container, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_eps_nas_security_algorithms(ogs_nas_eps_nas_security_algorithms_t *eps_nas_security_algorithms, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_extended_emergency_number_list(ogs_nas_extended_emergency_number_list_t *extended_emergency_number_list, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_extended_drx_parameters(ogs_nas_extended_drx_parameters_t *extended_drx_parameters, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_imeisv_request(ogs_nas_imeisv_request_t *imeisv_request, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_ladn_indication(ogs_nas_ladn_indication_t *ladn_indication, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gs_drx_parameters(ogs_nas_5gs_drx_parameters_t *drx_parameters, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gs_identity_type(ogs_nas_5gs_identity_type_t *identity_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_ladn_information(ogs_nas_ladn_information_t *ladn_information, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_mico_indication(ogs_nas_mico_indication_t *mico_indication, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_ma_pdu_session_information(ogs_nas_ma_pdu_session_information_t *ma_pdu_session_information, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_mapped_nssai(ogs_nas_mapped_nssai_t *mapped_nssai, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_mobile_station_classmark_2(ogs_nas_mobile_station_classmark_2_t *mobile_station_classmark_2, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_key_set_identifier(ogs_nas_key_set_identifier_t *key_set_identifier, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_message_container(ogs_nas_message_container_t *message_container, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_security_algorithms(ogs_nas_security_algorithms_t *security_algorithms, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_network_name(ogs_nas_network_name_t *network_name, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_network_slicing_indication(ogs_nas_network_slicing_indication_t *network_slicing_indication, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_non_3gpp_nw_provided_policies(ogs_nas_non_3gpp_nw_provided_policies_t *non_3gpp_nw_provided_policies, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_nssai(ogs_nas_nssai_t *nssai, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_nssai_inclusion_mode(ogs_nas_nssai_inclusion_mode_t *nssai_inclusion_mode, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_operator_defined_access_category_definitions(ogs_nas_operator_defined_access_category_definitions_t *operator_defined_access_category_definitions, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_payload_container(ogs_nas_payload_container_t *payload_container, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gs_mobile_identity(ogs_nas_5gs_mobile_identity_t *mobile_identity, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_payload_container_type(ogs_nas_payload_container_type_t *payload_container_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_identity_2(ogs_nas_pdu_session_identity_2_t *pdu_session_identity_2, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_reactivation_result(ogs_nas_pdu_session_reactivation_result_t *pdu_session_reactivation_result, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_reactivation_result_error_cause(ogs_nas_pdu_session_reactivation_result_error_cause_t *pdu_session_reactivation_result_error_cause, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_status(ogs_nas_pdu_session_status_t *pdu_session_status, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_plmn_list(ogs_nas_plmn_list_t *plmn_list, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_rejected_nssai(ogs_nas_rejected_nssai_t *rejected_nssai, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_release_assistance_indication(ogs_nas_release_assistance_indication_t *release_assistance_indication, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_request_type(ogs_nas_request_type_t *request_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_s1_ue_network_capability(ogs_nas_s1_ue_network_capability_t *s1_ue_network_capability, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_s1_ue_security_capability(ogs_nas_s1_ue_security_capability_t *s1_ue_security_capability, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_service_area_list(ogs_nas_service_area_list_t *service_area_list, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gs_network_feature_support(ogs_nas_5gs_network_feature_support_t *network_feature_support, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_sms_indication(ogs_nas_sms_indication_t *sms_indication, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_sor_transparent_container(ogs_nas_sor_transparent_container_t *sor_transparent_container, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_supported_codec_list(ogs_nas_supported_codec_list_t *supported_codec_list, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_time_zone(ogs_nas_time_zone_t *time_zone, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_time_zone_and_time(ogs_nas_time_zone_and_time_t *time_zone_and_time, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_ue_security_capability(ogs_nas_ue_security_capability_t *ue_security_capability, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_ue_usage_setting(ogs_nas_ue_usage_setting_t *ue_usage_setting, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_ue_status(ogs_nas_ue_status_t *ue_status, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_uplink_data_status(ogs_nas_uplink_data_status_t *uplink_data_status, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gs_registration_result(ogs_nas_5gs_registration_result_t *registration_result, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_ue_radio_capability_id(ogs_nas_ue_radio_capability_id_t *ue_radio_capability_id, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_ue_radio_capability_id_deletion_indication(ogs_nas_ue_radio_capability_id_deletion_indication_t *ue_radio_capability_id_deletion_indication, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gs_registration_type(ogs_nas_5gs_registration_type_t *registration_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_truncated_5g_s_tmsi_configuration(ogs_nas_truncated_5g_s_tmsi_configuration_t *truncated_s_tmsi_configuration, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_wus_assistance_information(ogs_nas_wus_assistance_information_t *wus_assistance_information, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_n5gc_indication(ogs_nas_n5gc_indication_t *n5gc_indication, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gs_tracking_area_identity(ogs_nas_5gs_tracking_area_identity_t *tracking_area_identity, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gs_tracking_area_identity_list(ogs_nas_5gs_tracking_area_identity_list_t *tracking_area_identity_list, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gs_update_type(ogs_nas_5gs_update_type_t *update_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gsm_capability(ogs_nas_5gsm_capability_t *gsm_capability, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_address(ogs_nas_pdu_address_t *pdu_address, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_pdu_session_type(ogs_nas_pdu_session_type_t *pdu_session_type, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_qos_flow_descriptions(ogs_nas_qos_flow_descriptions_t *qos_flow_descriptions, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_qos_rules(ogs_nas_qos_rules_t *qos_rules, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_session_ambr(ogs_nas_session_ambr_t *session_ambr, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_sm_pdu_dn_request_container(ogs_nas_sm_pdu_dn_request_container_t *sm_pdu_dn_request_container, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_ssc_mode(ogs_nas_ssc_mode_t *ssc_mode, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_re_attempt_indicator(ogs_nas_re_attempt_indicator_t *re_attempt_indicator, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gsm_network_feature_support(ogs_nas_5gsm_network_feature_support_t *gsm_network_feature_support, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gsm_cause(ogs_nas_5gsm_cause_t *gsm_cause, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_serving_plmn_rate_control(ogs_nas_serving_plmn_rate_control_t *serving_plmn_rate_control, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_5gsm_congestion_re_attempt_indicator(ogs_nas_5gsm_congestion_re_attempt_indicator_t *gsm_congestion_re_attempt_indicator, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_atsss_container(ogs_nas_atsss_container_t *atsss_container, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_control_plane_only_indication(ogs_nas_control_plane_only_indication_t *control_plane_only_indication, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_header_compression_configuration(ogs_nas_header_compression_configuration_t *header_compression_configuration, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_ds_tt_ethernet_port_mac_address(ogs_nas_ds_tt_ethernet_port_mac_address_t *ds_tt_ethernet_port_mac_address, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_ue_ds_tt_residence_time(ogs_nas_ue_ds_tt_residence_time_t *ue_ds_tt_residence_time, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_port_management_information_container(ogs_nas_port_management_information_container_t *port_management_information_container, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_always_on_pdu_session_indication(ogs_nas_always_on_pdu_session_indication_t *always_on_pdu_session_indication, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_always_on_pdu_session_requested(ogs_nas_always_on_pdu_session_requested_t *always_on_pdu_session_requested, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_allowed_ssc_mode(ogs_nas_allowed_ssc_mode_t *allowed_ssc_mode, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_extended_protocol_configuration_options(ogs_nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_integrity_protection_maximum_data_rate(ogs_nas_integrity_protection_maximum_data_rate_t *integrity_protection_maximum_data_rate, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_mapped_eps_bearer_contexts(ogs_nas_mapped_eps_bearer_contexts_t *mapped_eps_bearer_contexts, ogs_pkbuf_t *pkbuf);
int ogs_nas_5gs_decode_maximum_number_of_supported_packet_filters(ogs_nas_maximum_number_of_supported_packet_filters_t *maximum_number_of_supported_packet_filters, ogs_pkbuf_t *pkbuf);

int ogs_nas_5gs_encode_additional_information(ogs_pkbuf_t *pkbuf, ogs_nas_additional_information_t *additional_information);
int ogs_nas_5gs_encode_access_type(ogs_pkbuf_t *pkbuf, ogs_nas_access_type_t *access_type);
int ogs_nas_5gs_encode_dnn(ogs_pkbuf_t *pkbuf, ogs_nas_dnn_t *dnn);
int ogs_nas_5gs_encode_eap_message(ogs_pkbuf_t *pkbuf, ogs_nas_eap_message_t *eap_message);
int ogs_nas_5gs_encode_gprs_timer(ogs_pkbuf_t *pkbuf, ogs_nas_gprs_timer_t *gprs_timer);
int ogs_nas_5gs_encode_gprs_timer_2(ogs_pkbuf_t *pkbuf, ogs_nas_gprs_timer_2_t *gprs_timer_2);
int ogs_nas_5gs_encode_gprs_timer_3(ogs_pkbuf_t *pkbuf, ogs_nas_gprs_timer_3_t *gprs_timer_3);
int ogs_nas_5gs_encode_s_nssai(ogs_pkbuf_t *pkbuf, ogs_nas_s_nssai_t *s_nssai);
int ogs_nas_5gs_encode_5gmm_capability(ogs_pkbuf_t *pkbuf, ogs_nas_5gmm_capability_t *gmm_capability);
int ogs_nas_5gs_encode_abba(ogs_pkbuf_t *pkbuf, ogs_nas_abba_t *abba);
int ogs_nas_5gs_encode_additional_5g_security_information(ogs_pkbuf_t *pkbuf, ogs_nas_additional_5g_security_information_t *additional_security_information);
int ogs_nas_5gs_encode_additional_information_requested(ogs_pkbuf_t *pkbuf, ogs_nas_additional_information_requested_t *additional_information_requested);
int ogs_nas_5gs_encode_allowed_pdu_session_status(ogs_pkbuf_t *pkbuf, ogs_nas_allowed_pdu_session_status_t *allowed_pdu_session_status);
int ogs_nas_5gs_encode_authentication_failure_parameter(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_failure_parameter_t *authentication_failure_parameter);
int ogs_nas_5gs_encode_authentication_parameter_autn(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_parameter_autn_t *authentication_parameter_autn);
int ogs_nas_5gs_encode_authentication_parameter_rand(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_parameter_rand_t *authentication_parameter_rand);
int ogs_nas_5gs_encode_authentication_response_parameter(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_response_parameter_t *authentication_response_parameter);
int ogs_nas_5gs_encode_configuration_update_indication(ogs_pkbuf_t *pkbuf, ogs_nas_configuration_update_indication_t *configuration_update_indication);
int ogs_nas_5gs_encode_cag_information_list(ogs_pkbuf_t *pkbuf, ogs_nas_cag_information_list_t *cag_information_list);
int ogs_nas_5gs_encode_ciphering_key_data(ogs_pkbuf_t *pkbuf, ogs_nas_ciphering_key_data_t *ciphering_key_data);
int ogs_nas_5gs_encode_daylight_saving_time(ogs_pkbuf_t *pkbuf, ogs_nas_daylight_saving_time_t *daylight_saving_time);
int ogs_nas_5gs_encode_5gmm_cause(ogs_pkbuf_t *pkbuf, ogs_nas_5gmm_cause_t *gmm_cause);
int ogs_nas_5gs_encode_de_registration_type(ogs_pkbuf_t *pkbuf, ogs_nas_de_registration_type_t *de_registration_type);
int ogs_nas_5gs_encode_emergency_number_list(ogs_pkbuf_t *pkbuf, ogs_nas_emergency_number_list_t *emergency_number_list);
int ogs_nas_5gs_encode_eps_bearer_context_status(ogs_pkbuf_t *pkbuf, ogs_nas_eps_bearer_context_status_t *eps_bearer_context_status);
int ogs_nas_5gs_encode_eps_nas_message_container(ogs_pkbuf_t *pkbuf, ogs_nas_eps_nas_message_container_t *eps_nas_message_container);
int ogs_nas_5gs_encode_eps_nas_security_algorithms(ogs_pkbuf_t *pkbuf, ogs_nas_eps_nas_security_algorithms_t *eps_nas_security_algorithms);
int ogs_nas_5gs_encode_extended_emergency_number_list(ogs_pkbuf_t *pkbuf, ogs_nas_extended_emergency_number_list_t *extended_emergency_number_list);
int ogs_nas_5gs_encode_extended_drx_parameters(ogs_pkbuf_t *pkbuf, ogs_nas_extended_drx_parameters_t *extended_drx_parameters);
int ogs_nas_5gs_encode_imeisv_request(ogs_pkbuf_t *pkbuf, ogs_nas_imeisv_request_t *imeisv_request);
int ogs_nas_5gs_encode_ladn_indication(ogs_pkbuf_t *pkbuf, ogs_nas_ladn_indication_t *ladn_indication);
int ogs_nas_5gs_encode_5gs_drx_parameters(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_drx_parameters_t *drx_parameters);
int ogs_nas_5gs_encode_5gs_identity_type(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_identity_type_t *identity_type);
int ogs_nas_5gs_encode_ladn_information(ogs_pkbuf_t *pkbuf, ogs_nas_ladn_information_t *ladn_information);
int ogs_nas_5gs_encode_mico_indication(ogs_pkbuf_t *pkbuf, ogs_nas_mico_indication_t *mico_indication);
int ogs_nas_5gs_encode_ma_pdu_session_information(ogs_pkbuf_t *pkbuf, ogs_nas_ma_pdu_session_information_t *ma_pdu_session_information);
int ogs_nas_5gs_encode_mapped_nssai(ogs_pkbuf_t *pkbuf, ogs_nas_mapped_nssai_t *mapped_nssai);
int ogs_nas_5gs_encode_mobile_station_classmark_2(ogs_pkbuf_t *pkbuf, ogs_nas_mobile_station_classmark_2_t *mobile_station_classmark_2);
int ogs_nas_5gs_encode_key_set_identifier(ogs_pkbuf_t *pkbuf, ogs_nas_key_set_identifier_t *key_set_identifier);
int ogs_nas_5gs_encode_message_container(ogs_pkbuf_t *pkbuf, ogs_nas_message_container_t *message_container);
int ogs_nas_5gs_encode_security_algorithms(ogs_pkbuf_t *pkbuf, ogs_nas_security_algorithms_t *security_algorithms);
int ogs_nas_5gs_encode_network_name(ogs_pkbuf_t *pkbuf, ogs_nas_network_name_t *network_name);
int ogs_nas_5gs_encode_network_slicing_indication(ogs_pkbuf_t *pkbuf, ogs_nas_network_slicing_indication_t *network_slicing_indication);
int ogs_nas_5gs_encode_non_3gpp_nw_provided_policies(ogs_pkbuf_t *pkbuf, ogs_nas_non_3gpp_nw_provided_policies_t *non_3gpp_nw_provided_policies);
int ogs_nas_5gs_encode_nssai(ogs_pkbuf_t *pkbuf, ogs_nas_nssai_t *nssai);
int ogs_nas_5gs_encode_nssai_inclusion_mode(ogs_pkbuf_t *pkbuf, ogs_nas_nssai_inclusion_mode_t *nssai_inclusion_mode);
int ogs_nas_5gs_encode_operator_defined_access_category_definitions(ogs_pkbuf_t *pkbuf, ogs_nas_operator_defined_access_category_definitions_t *operator_defined_access_category_definitions);
int ogs_nas_5gs_encode_payload_container(ogs_pkbuf_t *pkbuf, ogs_nas_payload_container_t *payload_container);
int ogs_nas_5gs_encode_5gs_mobile_identity(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_mobile_identity_t *mobile_identity);
int ogs_nas_5gs_encode_payload_container_type(ogs_pkbuf_t *pkbuf, ogs_nas_payload_container_type_t *payload_container_type);
int ogs_nas_5gs_encode_pdu_session_identity_2(ogs_pkbuf_t *pkbuf, ogs_nas_pdu_session_identity_2_t *pdu_session_identity_2);
int ogs_nas_5gs_encode_pdu_session_reactivation_result(ogs_pkbuf_t *pkbuf, ogs_nas_pdu_session_reactivation_result_t *pdu_session_reactivation_result);
int ogs_nas_5gs_encode_pdu_session_reactivation_result_error_cause(ogs_pkbuf_t *pkbuf, ogs_nas_pdu_session_reactivation_result_error_cause_t *pdu_session_reactivation_result_error_cause);
int ogs_nas_5gs_encode_pdu_session_status(ogs_pkbuf_t *pkbuf, ogs_nas_pdu_session_status_t *pdu_session_status);
int ogs_nas_5gs_encode_plmn_list(ogs_pkbuf_t *pkbuf, ogs_nas_plmn_list_t *plmn_list);
int ogs_nas_5gs_encode_rejected_nssai(ogs_pkbuf_t *pkbuf, ogs_nas_rejected_nssai_t *rejected_nssai);
int ogs_nas_5gs_encode_release_assistance_indication(ogs_pkbuf_t *pkbuf, ogs_nas_release_assistance_indication_t *release_assistance_indication);
int ogs_nas_5gs_encode_request_type(ogs_pkbuf_t *pkbuf, ogs_nas_request_type_t *request_type);
int ogs_nas_5gs_encode_s1_ue_network_capability(ogs_pkbuf_t *pkbuf, ogs_nas_s1_ue_network_capability_t *s1_ue_network_capability);
int ogs_nas_5gs_encode_s1_ue_security_capability(ogs_pkbuf_t *pkbuf, ogs_nas_s1_ue_security_capability_t *s1_ue_security_capability);
int ogs_nas_5gs_encode_service_area_list(ogs_pkbuf_t *pkbuf, ogs_nas_service_area_list_t *service_area_list);
int ogs_nas_5gs_encode_5gs_network_feature_support(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_network_feature_support_t *network_feature_support);
int ogs_nas_5gs_encode_sms_indication(ogs_pkbuf_t *pkbuf, ogs_nas_sms_indication_t *sms_indication);
int ogs_nas_5gs_encode_sor_transparent_container(ogs_pkbuf_t *pkbuf, ogs_nas_sor_transparent_container_t *sor_transparent_container);
int ogs_nas_5gs_encode_supported_codec_list(ogs_pkbuf_t *pkbuf, ogs_nas_supported_codec_list_t *supported_codec_list);
int ogs_nas_5gs_encode_time_zone(ogs_pkbuf_t *pkbuf, ogs_nas_time_zone_t *time_zone);
int ogs_nas_5gs_encode_time_zone_and_time(ogs_pkbuf_t *pkbuf, ogs_nas_time_zone_and_time_t *time_zone_and_time);
int ogs_nas_5gs_encode_ue_security_capability(ogs_pkbuf_t *pkbuf, ogs_nas_ue_security_capability_t *ue_security_capability);
int ogs_nas_5gs_encode_ue_usage_setting(ogs_pkbuf_t *pkbuf, ogs_nas_ue_usage_setting_t *ue_usage_setting);
int ogs_nas_5gs_encode_ue_status(ogs_pkbuf_t *pkbuf, ogs_nas_ue_status_t *ue_status);
int ogs_nas_5gs_encode_uplink_data_status(ogs_pkbuf_t *pkbuf, ogs_nas_uplink_data_status_t *uplink_data_status);
int ogs_nas_5gs_encode_5gs_registration_result(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_registration_result_t *registration_result);
int ogs_nas_5gs_encode_ue_radio_capability_id(ogs_pkbuf_t *pkbuf, ogs_nas_ue_radio_capability_id_t *ue_radio_capability_id);
int ogs_nas_5gs_encode_ue_radio_capability_id_deletion_indication(ogs_pkbuf_t *pkbuf, ogs_nas_ue_radio_capability_id_deletion_indication_t *ue_radio_capability_id_deletion_indication);
int ogs_nas_5gs_encode_5gs_registration_type(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_registration_type_t *registration_type);
int ogs_nas_5gs_encode_truncated_5g_s_tmsi_configuration(ogs_pkbuf_t *pkbuf, ogs_nas_truncated_5g_s_tmsi_configuration_t *truncated_s_tmsi_configuration);
int ogs_nas_5gs_encode_wus_assistance_information(ogs_pkbuf_t *pkbuf, ogs_nas_wus_assistance_information_t *wus_assistance_information);
int ogs_nas_5gs_encode_n5gc_indication(ogs_pkbuf_t *pkbuf, ogs_nas_n5gc_indication_t *n5gc_indication);
int ogs_nas_5gs_encode_5gs_tracking_area_identity(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_tracking_area_identity_t *tracking_area_identity);
int ogs_nas_5gs_encode_5gs_tracking_area_identity_list(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_tracking_area_identity_list_t *tracking_area_identity_list);
int ogs_nas_5gs_encode_5gs_update_type(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_update_type_t *update_type);
int ogs_nas_5gs_encode_5gsm_capability(ogs_pkbuf_t *pkbuf, ogs_nas_5gsm_capability_t *gsm_capability);
int ogs_nas_5gs_encode_pdu_address(ogs_pkbuf_t *pkbuf, ogs_nas_pdu_address_t *pdu_address);
int ogs_nas_5gs_encode_pdu_session_type(ogs_pkbuf_t *pkbuf, ogs_nas_pdu_session_type_t *pdu_session_type);
int ogs_nas_5gs_encode_qos_flow_descriptions(ogs_pkbuf_t *pkbuf, ogs_nas_qos_flow_descriptions_t *qos_flow_descriptions);
int ogs_nas_5gs_encode_qos_rules(ogs_pkbuf_t *pkbuf, ogs_nas_qos_rules_t *qos_rules);
int ogs_nas_5gs_encode_session_ambr(ogs_pkbuf_t *pkbuf, ogs_nas_session_ambr_t *session_ambr);
int ogs_nas_5gs_encode_sm_pdu_dn_request_container(ogs_pkbuf_t *pkbuf, ogs_nas_sm_pdu_dn_request_container_t *sm_pdu_dn_request_container);
int ogs_nas_5gs_encode_ssc_mode(ogs_pkbuf_t *pkbuf, ogs_nas_ssc_mode_t *ssc_mode);
int ogs_nas_5gs_encode_re_attempt_indicator(ogs_pkbuf_t *pkbuf, ogs_nas_re_attempt_indicator_t *re_attempt_indicator);
int ogs_nas_5gs_encode_5gsm_network_feature_support(ogs_pkbuf_t *pkbuf, ogs_nas_5gsm_network_feature_support_t *gsm_network_feature_support);
int ogs_nas_5gs_encode_5gsm_cause(ogs_pkbuf_t *pkbuf, ogs_nas_5gsm_cause_t *gsm_cause);
int ogs_nas_5gs_encode_serving_plmn_rate_control(ogs_pkbuf_t *pkbuf, ogs_nas_serving_plmn_rate_control_t *serving_plmn_rate_control);
int ogs_nas_5gs_encode_5gsm_congestion_re_attempt_indicator(ogs_pkbuf_t *pkbuf, ogs_nas_5gsm_congestion_re_attempt_indicator_t *gsm_congestion_re_attempt_indicator);
int ogs_nas_5gs_encode_atsss_container(ogs_pkbuf_t *pkbuf, ogs_nas_atsss_container_t *atsss_container);
int ogs_nas_5gs_encode_control_plane_only_indication(ogs_pkbuf_t *pkbuf, ogs_nas_control_plane_only_indication_t *control_plane_only_indication);
int ogs_nas_5gs_encode_header_compression_configuration(ogs_pkbuf_t *pkbuf, ogs_nas_header_compression_configuration_t *header_compression_configuration);
int ogs_nas_5gs_encode_ds_tt_ethernet_port_mac_address(ogs_pkbuf_t *pkbuf, ogs_nas_ds_tt_ethernet_port_mac_address_t *ds_tt_ethernet_port_mac_address);
int ogs_nas_5gs_encode_ue_ds_tt_residence_time(ogs_pkbuf_t *pkbuf, ogs_nas_ue_ds_tt_residence_time_t *ue_ds_tt_residence_time);
int ogs_nas_5gs_encode_port_management_information_container(ogs_pkbuf_t *pkbuf, ogs_nas_port_management_information_container_t *port_management_information_container);
int ogs_nas_5gs_encode_always_on_pdu_session_indication(ogs_pkbuf_t *pkbuf, ogs_nas_always_on_pdu_session_indication_t *always_on_pdu_session_indication);
int ogs_nas_5gs_encode_always_on_pdu_session_requested(ogs_pkbuf_t *pkbuf, ogs_nas_always_on_pdu_session_requested_t *always_on_pdu_session_requested);
int ogs_nas_5gs_encode_allowed_ssc_mode(ogs_pkbuf_t *pkbuf, ogs_nas_allowed_ssc_mode_t *allowed_ssc_mode);
int ogs_nas_5gs_encode_extended_protocol_configuration_options(ogs_pkbuf_t *pkbuf, ogs_nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options);
int ogs_nas_5gs_encode_integrity_protection_maximum_data_rate(ogs_pkbuf_t *pkbuf, ogs_nas_integrity_protection_maximum_data_rate_t *integrity_protection_maximum_data_rate);
int ogs_nas_5gs_encode_mapped_eps_bearer_contexts(ogs_pkbuf_t *pkbuf, ogs_nas_mapped_eps_bearer_contexts_t *mapped_eps_bearer_contexts);
int ogs_nas_5gs_encode_maximum_number_of_supported_packet_filters(ogs_pkbuf_t *pkbuf, ogs_nas_maximum_number_of_supported_packet_filters_t *maximum_number_of_supported_packet_filters);

#ifdef __cplusplus
}
#endif

#endif /* OGS_NAS_5GS_IES_H */

