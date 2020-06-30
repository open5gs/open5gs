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

#include "ogs-diameter-s6a.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )	\
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct dict_object *ogs_diam_s6a_application = NULL;

struct dict_object *ogs_diam_s6a_cmd_air = NULL;
struct dict_object *ogs_diam_s6a_cmd_aia = NULL;
struct dict_object *ogs_diam_s6a_cmd_ulr = NULL;
struct dict_object *ogs_diam_s6a_cmd_ula = NULL;
struct dict_object *ogs_diam_s6a_cmd_pur = NULL;
struct dict_object *ogs_diam_s6a_cmd_pua = NULL;

struct dict_object *ogs_diam_s6a_visited_plmn_id = NULL;
struct dict_object *ogs_diam_s6a_rat_type = NULL;
struct dict_object *ogs_diam_s6a_ulr_flags = NULL;
struct dict_object *ogs_diam_s6a_ula_flags = NULL;
struct dict_object *ogs_diam_s6a_subscription_data = NULL;
struct dict_object *ogs_diam_s6a_req_eutran_auth_info = NULL;
struct dict_object *ogs_diam_s6a_number_of_requested_vectors = NULL;
struct dict_object *ogs_diam_s6a_immediate_response_preferred = NULL;
struct dict_object *ogs_diam_s6a_authentication_info = NULL;
struct dict_object *ogs_diam_s6a_re_synchronization_info = NULL;
struct dict_object *ogs_diam_s6a_service_selection = NULL;
struct dict_object *ogs_diam_s6a_ue_srvcc_capability = NULL;
struct dict_object *ogs_diam_s6a_e_utran_vector = NULL;
struct dict_object *ogs_diam_s6a_rand = NULL;
struct dict_object *ogs_diam_s6a_xres = NULL;
struct dict_object *ogs_diam_s6a_autn = NULL;
struct dict_object *ogs_diam_s6a_kasme = NULL;
struct dict_object *ogs_diam_s6a_subscriber_status = NULL;
struct dict_object *ogs_diam_s6a_ambr = NULL;
struct dict_object *ogs_diam_s6a_network_access_mode = NULL;
struct dict_object *ogs_diam_s6a_access_restriction_data = NULL;
struct dict_object *ogs_diam_s6a_apn_configuration_profile = NULL;
struct dict_object *ogs_diam_s6a_subscribed_rau_tau_timer = NULL;
struct dict_object *ogs_diam_s6a_context_identifier = NULL;
struct dict_object *ogs_diam_s6a_all_apn_configuration_included_indicator = NULL;
struct dict_object *ogs_diam_s6a_apn_configuration = NULL;
struct dict_object *ogs_diam_s6a_max_bandwidth_ul = NULL;
struct dict_object *ogs_diam_s6a_max_bandwidth_dl = NULL;
struct dict_object *ogs_diam_s6a_pdn_type = NULL;
struct dict_object *ogs_diam_s6a_served_party_ip_address = NULL;
struct dict_object *ogs_diam_s6a_eps_subscribed_qos_profile = NULL;
struct dict_object *ogs_diam_s6a_qos_class_identifier = NULL;
struct dict_object *ogs_diam_s6a_allocation_retention_priority = NULL;
struct dict_object *ogs_diam_s6a_priority_level = NULL;
struct dict_object *ogs_diam_s6a_pre_emption_capability = NULL;
struct dict_object *ogs_diam_s6a_pre_emption_vulnerability = NULL;

struct dict_object *ogs_diam_s6a_terminal_information = NULL;
struct dict_object *ogs_diam_s6a_imei = NULL;
struct dict_object *ogs_diam_s6a_software_version = NULL;

struct dict_object *ogs_diam_s6a_msisdn = NULL;
struct dict_object *ogs_diam_s6a_a_msisdn = NULL;

extern int ogs_dict_s6a_entry(char *conffile);

int ogs_diam_s6a_init(void)
{
    application_id_t id = OGS_DIAM_S6A_APPLICATION_ID;

    ogs_assert(ogs_dict_s6a_entry(NULL) == 0);

    CHECK_dict_search(DICT_APPLICATION, APPLICATION_BY_ID, (void *)&id, &ogs_diam_s6a_application);

    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Authentication-Information-Request", &ogs_diam_s6a_cmd_air);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Authentication-Information-Answer", &ogs_diam_s6a_cmd_aia);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Update-Location-Request", &ogs_diam_s6a_cmd_ulr);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Update-Location-Answer", &ogs_diam_s6a_cmd_ula);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Purge-UE-Request", &ogs_diam_s6a_cmd_pur);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Purge-UE-Answer", &ogs_diam_s6a_cmd_pua);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Visited-PLMN-Id", &ogs_diam_s6a_visited_plmn_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RAT-Type", &ogs_diam_s6a_rat_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "ULR-Flags", &ogs_diam_s6a_ulr_flags);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "ULA-Flags", &ogs_diam_s6a_ula_flags);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "UE-SRVCC-Capability", &ogs_diam_s6a_ue_srvcc_capability);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Requested-EUTRAN-Authentication-Info", &ogs_diam_s6a_req_eutran_auth_info);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Number-Of-Requested-Vectors", &ogs_diam_s6a_number_of_requested_vectors);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Immediate-Response-Preferred", &ogs_diam_s6a_immediate_response_preferred);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Re-Synchronization-Info", &ogs_diam_s6a_re_synchronization_info);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Authentication-Info", &ogs_diam_s6a_authentication_info);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "E-UTRAN-Vector", &ogs_diam_s6a_e_utran_vector);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RAND", &ogs_diam_s6a_rand);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "XRES", &ogs_diam_s6a_xres);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "AUTN", &ogs_diam_s6a_autn);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "KASME", &ogs_diam_s6a_kasme);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "EPS-Subscribed-QoS-Profile", &ogs_diam_s6a_eps_subscribed_qos_profile);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "QoS-Class-Identifier", &ogs_diam_s6a_qos_class_identifier);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Allocation-Retention-Priority", &ogs_diam_s6a_allocation_retention_priority);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Priority-Level", &ogs_diam_s6a_priority_level);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Pre-emption-Capability", &ogs_diam_s6a_pre_emption_capability);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Pre-emption-Vulnerability", &ogs_diam_s6a_pre_emption_vulnerability);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "AMBR", &ogs_diam_s6a_ambr);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-UL", &ogs_diam_s6a_max_bandwidth_ul);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-DL", &ogs_diam_s6a_max_bandwidth_dl);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "APN-Configuration-Profile", &ogs_diam_s6a_apn_configuration_profile);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Context-Identifier", &ogs_diam_s6a_context_identifier);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "All-APN-Configurations-Included-Indicator", &ogs_diam_s6a_all_apn_configuration_included_indicator);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "APN-Configuration", &ogs_diam_s6a_apn_configuration);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Service-Selection", &ogs_diam_s6a_service_selection);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "PDN-Type", &ogs_diam_s6a_pdn_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Served-Party-IP-Address", &ogs_diam_s6a_served_party_ip_address);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscription-Data", &ogs_diam_s6a_subscription_data);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscriber-Status", &ogs_diam_s6a_subscriber_status);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Network-Access-Mode", &ogs_diam_s6a_network_access_mode);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Access-Restriction-Data", &ogs_diam_s6a_access_restriction_data);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscribed-Periodic-RAU-TAU-Timer", &ogs_diam_s6a_subscribed_rau_tau_timer);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Terminal-Information", &ogs_diam_s6a_terminal_information);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "IMEI", &ogs_diam_s6a_imei);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Software-Version", &ogs_diam_s6a_software_version);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "MSISDN", &ogs_diam_s6a_msisdn);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "A-MSISDN", &ogs_diam_s6a_a_msisdn);

	return 0;
}
