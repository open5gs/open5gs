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

#include "ogs-diameter-gx.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )	\
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct dict_object *ogs_diam_gx_application = NULL;

struct dict_object *ogs_diam_gx_cmd_ccr = NULL;
struct dict_object *ogs_diam_gx_cmd_cca = NULL;
struct dict_object *ogs_diam_gx_cmd_rar = NULL;
struct dict_object *ogs_diam_gx_cmd_raa = NULL;

struct dict_object *ogs_diam_gx_cc_request_type = NULL;
struct dict_object *ogs_diam_gx_cc_request_number = NULL;
struct dict_object *ogs_diam_gx_network_request_support = NULL;
struct dict_object *ogs_diam_gx_supported_features = NULL;
struct dict_object *ogs_diam_gx_feature_list_id = NULL;
struct dict_object *ogs_diam_gx_feature_list = NULL;
struct dict_object *ogs_diam_gx_framed_ip_address = NULL;
struct dict_object *ogs_diam_gx_framed_ipv6_prefix = NULL;
struct dict_object *ogs_diam_gx_ip_can_type = NULL;
struct dict_object *ogs_diam_gx_qos_information = NULL;
struct dict_object *ogs_diam_gx_qos_class_identifier = NULL;
struct dict_object *ogs_diam_gx_max_requested_bandwidth_ul = NULL;
struct dict_object *ogs_diam_gx_max_requested_bandwidth_dl = NULL;
struct dict_object *ogs_diam_gx_min_requested_bandwidth_ul = NULL;
struct dict_object *ogs_diam_gx_min_requested_bandwidth_dl = NULL;
struct dict_object *ogs_diam_gx_guaranteed_bitrate_ul = NULL;
struct dict_object *ogs_diam_gx_guaranteed_bitrate_dl = NULL;
struct dict_object *ogs_diam_gx_allocation_retention_priority = NULL;
struct dict_object *ogs_diam_gx_priority_level = NULL;
struct dict_object *ogs_diam_gx_pre_emption_capability = NULL;
struct dict_object *ogs_diam_gx_pre_emption_vulnerability = NULL;
struct dict_object *ogs_diam_gx_apn_aggregate_max_bitrate_ul = NULL;
struct dict_object *ogs_diam_gx_apn_aggregate_max_bitrate_dl = NULL;
struct dict_object *ogs_diam_gx_3gpp_user_location_info = NULL;
struct dict_object *ogs_diam_gx_called_station_id = NULL;
struct dict_object *ogs_diam_gx_default_eps_bearer_qos = NULL;
struct dict_object *ogs_diam_gx_3gpp_ms_timezone = NULL;
struct dict_object *ogs_diam_gx_event_trigger = NULL;
struct dict_object *ogs_diam_gx_bearer_control_mode = NULL;
struct dict_object *ogs_diam_gx_charging_rule_install = NULL;
struct dict_object *ogs_diam_gx_charging_rule_remove = NULL;
struct dict_object *ogs_diam_gx_charging_rule_definition = NULL;
struct dict_object *ogs_diam_gx_charging_rule_base_name = NULL;
struct dict_object *ogs_diam_gx_charging_rule_name = NULL;
struct dict_object *ogs_diam_gx_flow_information = NULL;
struct dict_object *ogs_diam_gx_flow_direction = NULL;
struct dict_object *ogs_diam_gx_flow_description = NULL;
struct dict_object *ogs_diam_gx_flow_status = NULL;
struct dict_object *ogs_diam_gx_precedence = NULL;
struct dict_object *ogs_diam_gx_flows = NULL;
struct dict_object *ogs_diam_gx_media_component_description = NULL;
struct dict_object *ogs_diam_gx_media_component_number = NULL;
struct dict_object *ogs_diam_gx_media_type = NULL;
struct dict_object *ogs_diam_gx_rr_bandwidth = NULL;
struct dict_object *ogs_diam_gx_rs_bandwidth = NULL;
struct dict_object *ogs_diam_gx_codec_data = NULL;
struct dict_object *ogs_diam_gx_media_sub_component = NULL;
struct dict_object *ogs_diam_gx_flow_number = NULL;
struct dict_object *ogs_diam_gx_flow_usage = NULL;
struct dict_object *ogs_diam_gx_3gpp_sgsn_mcc_mnc = NULL;
struct dict_object *ogs_diam_gx_an_gw_address = NULL;
struct dict_object *ogs_diam_gx_online = NULL;
struct dict_object *ogs_diam_gx_offline = NULL;
struct dict_object *ogs_diam_gx_access_network_charging_address = NULL;
struct dict_object *ogs_diam_gx_access_network_charging_identifier_gx = NULL;
struct dict_object *ogs_diam_gx_access_network_charging_identifier_value = NULL;
struct dict_object *ogs_diam_gx_an_trusted = NULL;

extern int ogs_dict_gx_entry(char *conffile);

int ogs_diam_gx_init(void)
{
    application_id_t id = OGS_DIAM_GX_APPLICATION_ID;

    ogs_assert(ogs_dict_gx_entry(NULL) == 0);

    CHECK_dict_search(DICT_APPLICATION, APPLICATION_BY_ID, (void *)&id, &ogs_diam_gx_application);

    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Credit-Control-Request", &ogs_diam_gx_cmd_ccr);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Credit-Control-Answer", &ogs_diam_gx_cmd_cca);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Re-Auth-Request", &ogs_diam_gx_cmd_rar);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Re-Auth-Answer", &ogs_diam_gx_cmd_raa);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "CC-Request-Type", &ogs_diam_gx_cc_request_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "CC-Request-Number", &ogs_diam_gx_cc_request_number);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Network-Request-Support", &ogs_diam_gx_network_request_support);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Supported-Features", &ogs_diam_gx_supported_features);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Feature-List-ID", &ogs_diam_gx_feature_list_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Feature-List", &ogs_diam_gx_feature_list);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Framed-IP-Address", &ogs_diam_gx_framed_ip_address);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Framed-IPv6-Prefix", &ogs_diam_gx_framed_ipv6_prefix);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "IP-CAN-Type", &ogs_diam_gx_ip_can_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "QoS-Information", &ogs_diam_gx_qos_information);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "QoS-Class-Identifier" , &ogs_diam_gx_qos_class_identifier);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-UL" , &ogs_diam_gx_max_requested_bandwidth_ul);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-DL" , &ogs_diam_gx_max_requested_bandwidth_dl);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Min-Requested-Bandwidth-UL" , &ogs_diam_gx_min_requested_bandwidth_ul);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Min-Requested-Bandwidth-DL" , &ogs_diam_gx_min_requested_bandwidth_dl);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Guaranteed-Bitrate-UL" , &ogs_diam_gx_guaranteed_bitrate_ul);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Guaranteed-Bitrate-DL" , &ogs_diam_gx_guaranteed_bitrate_dl);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Allocation-Retention-Priority" , &ogs_diam_gx_allocation_retention_priority);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Priority-Level", &ogs_diam_gx_priority_level);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Pre-emption-Capability", &ogs_diam_gx_pre_emption_capability);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Pre-emption-Vulnerability", &ogs_diam_gx_pre_emption_vulnerability);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "APN-Aggregate-Max-Bitrate-UL" , &ogs_diam_gx_apn_aggregate_max_bitrate_ul);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "APN-Aggregate-Max-Bitrate-DL" , &ogs_diam_gx_apn_aggregate_max_bitrate_dl);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-User-Location-Info", &ogs_diam_gx_3gpp_user_location_info);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Called-Station-Id", &ogs_diam_gx_called_station_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Default-EPS-Bearer-QoS", &ogs_diam_gx_default_eps_bearer_qos);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-MS-TimeZone", &ogs_diam_gx_3gpp_ms_timezone);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Event-Trigger", &ogs_diam_gx_event_trigger);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Bearer-Control-Mode", &ogs_diam_gx_bearer_control_mode);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Charging-Rule-Install", &ogs_diam_gx_charging_rule_install);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Charging-Rule-Remove", &ogs_diam_gx_charging_rule_remove);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Charging-Rule-Definition", &ogs_diam_gx_charging_rule_definition);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Charging-Rule-Base-Name", &ogs_diam_gx_charging_rule_base_name);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Charging-Rule-Name", &ogs_diam_gx_charging_rule_name);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Information", &ogs_diam_gx_flow_information);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Direction", &ogs_diam_gx_flow_direction);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Description", &ogs_diam_gx_flow_description);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Status", &ogs_diam_gx_flow_status);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Precedence", &ogs_diam_gx_precedence);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flows", &ogs_diam_gx_flows);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Component-Description", &ogs_diam_gx_media_component_description);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Component-Number", &ogs_diam_gx_media_component_number);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Type", &ogs_diam_gx_media_type);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RR-Bandwidth" , &ogs_diam_gx_rr_bandwidth);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RS-Bandwidth" , &ogs_diam_gx_rs_bandwidth);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Codec-Data", &ogs_diam_gx_codec_data);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Sub-Component", &ogs_diam_gx_media_sub_component);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Number", &ogs_diam_gx_flow_number);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Usage", &ogs_diam_gx_flow_usage);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-SGSN-MCC-MNC", &ogs_diam_gx_3gpp_sgsn_mcc_mnc);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "AN-GW-Address", &ogs_diam_gx_an_gw_address);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Online", &ogs_diam_gx_online);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Offline", &ogs_diam_gx_offline);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Access-Network-Charging-Address", &ogs_diam_gx_access_network_charging_address);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Access-Network-Charging-Identifier-Gx", &ogs_diam_gx_access_network_charging_identifier_gx);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Access-Network-Charging-Identifier-Value", &ogs_diam_gx_access_network_charging_identifier_value);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "AN-Trusted", &ogs_diam_gx_an_trusted);

    return 0;
}
