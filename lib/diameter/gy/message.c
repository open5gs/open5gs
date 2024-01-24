/* Gy Interface, 3GPP TS 32.299
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#include "ogs-diameter-gy.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )    \
    CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct dict_object *ogs_diam_gy_application = NULL;

struct dict_object *ogs_diam_gy_cmd_ccr = NULL;
struct dict_object *ogs_diam_gy_cmd_cca = NULL;
struct dict_object *ogs_diam_gy_cmd_rar = NULL;
struct dict_object *ogs_diam_gy_cmd_raa = NULL;

struct dict_object *ogs_diam_gy_cc_request_type = NULL;
struct dict_object *ogs_diam_gy_cc_request_number = NULL;
struct dict_object *ogs_diam_gy_requested_action = NULL;
struct dict_object *ogs_diam_gy_aoc_request_type = NULL;
struct dict_object *ogs_diam_gy_multiple_services_ind = NULL;
struct dict_object *ogs_diam_gy_multiple_services_cc = NULL;
struct dict_object *ogs_diam_gy_requested_service_unit = NULL;
struct dict_object *ogs_diam_gy_used_service_unit = NULL;
struct dict_object *ogs_diam_gy_cc_time = NULL;
struct dict_object *ogs_diam_gy_cc_total_octets = NULL;
struct dict_object *ogs_diam_gy_cc_input_octets = NULL;
struct dict_object *ogs_diam_gy_cc_output_octets = NULL;
struct dict_object *ogs_diam_gy_cc_service_specific_units = NULL;
struct dict_object *ogs_diam_gy_reporting_reason = NULL;
struct dict_object *ogs_diam_gy_service_id = NULL;

struct dict_object *ogs_diam_gy_service_information = NULL;
struct dict_object *ogs_diam_gy_ps_information = NULL;
struct dict_object *ogs_diam_gy_3gpp_charging_id = NULL;
struct dict_object *ogs_diam_gy_3gpp_pdp_type = NULL;
struct dict_object *ogs_diam_gy_pdp_address = NULL;
struct dict_object *ogs_diam_gy_sgsn_address = NULL;
struct dict_object *ogs_diam_gy_ggsn_address = NULL;
struct dict_object *ogs_diam_gy_3gpp_nsapi = NULL;
struct dict_object *ogs_diam_gy_3gpp_selection_mode = NULL;
struct dict_object *ogs_diam_gy_3gpp_charging_characteristics = NULL;
struct dict_object *ogs_diam_gy_user_equipment_info = NULL;
struct dict_object *ogs_diam_gy_user_equipment_info_type = NULL;
struct dict_object *ogs_diam_gy_user_equipment_info_value = NULL;

struct dict_object *ogs_diam_gy_feature_list_id = NULL;
struct dict_object *ogs_diam_gy_feature_list = NULL;
struct dict_object *ogs_diam_gy_qos_information = NULL;
struct dict_object *ogs_diam_gy_qos_class_identifier = NULL;
struct dict_object *ogs_diam_gy_max_requested_bandwidth_ul = NULL;
struct dict_object *ogs_diam_gy_max_requested_bandwidth_dl = NULL;
struct dict_object *ogs_diam_gy_guaranteed_bitrate_ul = NULL;
struct dict_object *ogs_diam_gy_guaranteed_bitrate_dl = NULL;
struct dict_object *ogs_diam_gy_allocation_retention_priority = NULL;
struct dict_object *ogs_diam_gy_priority_level = NULL;
struct dict_object *ogs_diam_gy_pre_emption_capability = NULL;
struct dict_object *ogs_diam_gy_pre_emption_vulnerability = NULL;
struct dict_object *ogs_diam_gy_apn_aggregate_max_bitrate_ul = NULL;
struct dict_object *ogs_diam_gy_apn_aggregate_max_bitrate_dl = NULL;
struct dict_object *ogs_diam_gy_3gpp_rat_type = NULL;
struct dict_object *ogs_diam_gy_3gpp_user_location_info = NULL;
struct dict_object *ogs_diam_gy_called_station_id = NULL;
struct dict_object *ogs_diam_gy_3gpp_ms_timezone = NULL;
struct dict_object *ogs_diam_gy_charging_rule_base_name = NULL;
struct dict_object *ogs_diam_gy_flows = NULL;
struct dict_object *ogs_diam_gy_3gpp_sgsn_mcc_mnc = NULL;

extern int ogs_dict_gy_entry(char *conffile);

int ogs_diam_gy_init(void)
{
    application_id_t id = OGS_DIAM_GY_APPLICATION_ID;

    ogs_assert(ogs_dict_gy_entry(NULL) == 0);

    CHECK_dict_search(DICT_APPLICATION, APPLICATION_BY_ID, (void *)&id, &ogs_diam_gy_application);

    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Credit-Control-Request", &ogs_diam_gy_cmd_ccr);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Credit-Control-Answer", &ogs_diam_gy_cmd_cca);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Re-Auth-Request", &ogs_diam_gy_cmd_rar);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Re-Auth-Answer", &ogs_diam_gy_cmd_raa);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "CC-Request-Type", &ogs_diam_gy_cc_request_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "CC-Request-Number", &ogs_diam_gy_cc_request_number);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Requested-Action", &ogs_diam_gy_requested_action);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "AoC-Request-Type", &ogs_diam_gy_aoc_request_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Multiple-Services-Indicator", &ogs_diam_gy_multiple_services_ind);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Multiple-Services-Credit-Control", &ogs_diam_gy_multiple_services_cc);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Requested-Service-Unit", &ogs_diam_gy_requested_service_unit);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Used-Service-Unit", &ogs_diam_gy_used_service_unit);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "CC-Time", &ogs_diam_gy_cc_time);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "CC-Total-Octets", &ogs_diam_gy_cc_total_octets);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "CC-Input-Octets", &ogs_diam_gy_cc_input_octets);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "CC-Output-Octets", &ogs_diam_gy_cc_output_octets);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "CC-Service-Specific-Units", &ogs_diam_gy_cc_service_specific_units);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Reporting-Reason", &ogs_diam_gy_reporting_reason);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Service-Identifier", &ogs_diam_gy_service_id);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Service-Information", &ogs_diam_gy_service_information);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "PS-Information", &ogs_diam_gy_ps_information);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-Charging-Id", &ogs_diam_gy_3gpp_charging_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-PDP-Type", &ogs_diam_gy_3gpp_pdp_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "PDP-Address", &ogs_diam_gy_pdp_address);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "SGSN-Address", &ogs_diam_gy_sgsn_address);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "GGSN-Address", &ogs_diam_gy_ggsn_address);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-NSAPI", &ogs_diam_gy_3gpp_nsapi);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-Selection-Mode", &ogs_diam_gy_3gpp_selection_mode);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-Charging-Characteristics", &ogs_diam_gy_3gpp_charging_characteristics);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "User-Equipment-Info", &ogs_diam_gy_user_equipment_info);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "User-Equipment-Info-Type", &ogs_diam_gy_user_equipment_info_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "User-Equipment-Info-Value", &ogs_diam_gy_user_equipment_info_value);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Feature-List-ID", &ogs_diam_gy_feature_list_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Feature-List", &ogs_diam_gy_feature_list);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "QoS-Information", &ogs_diam_gy_qos_information);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "QoS-Class-Identifier" , &ogs_diam_gy_qos_class_identifier);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-UL" , &ogs_diam_gy_max_requested_bandwidth_ul);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-DL" , &ogs_diam_gy_max_requested_bandwidth_dl);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Guaranteed-Bitrate-UL" , &ogs_diam_gy_guaranteed_bitrate_ul);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Guaranteed-Bitrate-DL" , &ogs_diam_gy_guaranteed_bitrate_dl);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Allocation-Retention-Priority" , &ogs_diam_gy_allocation_retention_priority);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Priority-Level", &ogs_diam_gy_priority_level);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Pre-emption-Capability", &ogs_diam_gy_pre_emption_capability);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Pre-emption-Vulnerability", &ogs_diam_gy_pre_emption_vulnerability);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "APN-Aggregate-Max-Bitrate-UL" , &ogs_diam_gy_apn_aggregate_max_bitrate_ul);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "APN-Aggregate-Max-Bitrate-DL" , &ogs_diam_gy_apn_aggregate_max_bitrate_dl);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-RAT-Type" , &ogs_diam_gy_3gpp_rat_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-User-Location-Info", &ogs_diam_gy_3gpp_user_location_info);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Called-Station-Id", &ogs_diam_gy_called_station_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-MS-TimeZone", &ogs_diam_gy_3gpp_ms_timezone);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Charging-Rule-Base-Name", &ogs_diam_gy_charging_rule_base_name);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flows", &ogs_diam_gy_flows);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-SGSN-MCC-MNC", &ogs_diam_gy_3gpp_sgsn_mcc_mnc);

    return 0;
}
