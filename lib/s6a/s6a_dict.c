#define TRACE_MODULE _s6a_dict

#include "core_debug.h"

#include "s6a_app.h"

struct dict_object *s6a_vendor = NULL;
struct dict_object *s6a_appli = NULL;

struct dict_object *s6a_cmd_air = NULL;
struct dict_object *s6a_cmd_aia = NULL;
struct dict_object *s6a_cmd_ulr = NULL;
struct dict_object *s6a_cmd_ula = NULL;
struct dict_object *s6a_cmd_pur = NULL;
struct dict_object *s6a_cmd_pua = NULL;
struct dict_object *s6a_cmd_clr = NULL;
struct dict_object *s6a_cmd_cla = NULL;

struct dict_object *s6a_origin_host = NULL;
struct dict_object *s6a_origin_realm = NULL;
struct dict_object *s6a_destination_host = NULL;
struct dict_object *s6a_destination_realm = NULL;
struct dict_object *s6a_user_name = NULL;
struct dict_object *s6a_session_id = NULL;
struct dict_object *s6a_auth_session_state = NULL;
struct dict_object *s6a_result_code = NULL;
struct dict_object *s6a_experimental_result = NULL;
struct dict_object *s6a_vendor_id = NULL;
struct dict_object *s6a_experimental_result_code = NULL;

struct dict_object *s6a_visited_plmn_id = NULL;
struct dict_object *s6a_rat_type = NULL;
struct dict_object *s6a_ulr_flags = NULL;
struct dict_object *s6a_ula_flags = NULL;
struct dict_object *s6a_subscription_data = NULL;
struct dict_object *s6a_req_eutran_auth_info = NULL;
struct dict_object *s6a_number_of_requested_vectors = NULL;
struct dict_object *s6a_immediate_response_preferred = NULL;
struct dict_object *s6a_authentication_info = NULL;
struct dict_object *s6a_re_synchronization_info = NULL;
struct dict_object *s6a_service_selection = NULL;
struct dict_object *s6a_ue_srvcc_cap = NULL;
struct dict_object *s6a_imsi = NULL;
struct dict_object *s6a_imei = NULL;
struct dict_object *s6a_software_version = NULL;
struct dict_object *s6a_supported_features = NULL;
struct dict_object *s6a_req_e_utran_auth_info = NULL;
struct dict_object *s6a_req_resync_info = NULL;
struct dict_object *s6a_req_nb_of_req_vectors = NULL;
struct dict_object *s6a_req_geran_auth_info = NULL;
struct dict_object *s6a_e_utran_vector = NULL;
struct dict_object *s6a_rand = NULL;
struct dict_object *s6a_xres = NULL;
struct dict_object *s6a_autn = NULL;
struct dict_object *s6a_kasme = NULL;
struct dict_object *s6a_pur_flags = NULL;
struct dict_object *s6a_pua_flags = NULL;
struct dict_object *s6a_terminal_info = NULL;
struct dict_object *s6a_gmlc_addr = NULL;
struct dict_object *s6a_subscriber_status = NULL;
struct dict_object *s6a_msisdn = NULL;
struct dict_object *s6a_ambr = NULL;
struct dict_object *s6a_network_access_mode = NULL;
struct dict_object *s6a_access_restriction_data = NULL;
struct dict_object *s6a_apn_configuration_profile = NULL;
struct dict_object *s6a_subscribed_rau_tau_timer = NULL;
struct dict_object *s6a_context_identifier = NULL;
struct dict_object *s6a_all_apn_conf_inc_ind = NULL;
struct dict_object *s6a_apn_configuration = NULL;
struct dict_object *s6a_max_bandwidth_ul = NULL;
struct dict_object *s6a_max_bandwidth_dl = NULL;
struct dict_object *s6a_pdn_type = NULL;
struct dict_object *s6a_eps_subscribed_qos_profile = NULL;
struct dict_object *s6a_qos_class_identifier = NULL;
struct dict_object *s6a_allocation_retention_priority = NULL;
struct dict_object *s6a_priority_level = NULL;
struct dict_object *s6a_pre_emption_capability = NULL;
struct dict_object *s6a_pre_emption_vulnerability = NULL;
struct dict_object *s6a_served_party_ip_addr = NULL;

int s6a_ext_load()
{
    int ret;

    int fd_ext_dict_nas_mipv6_init(int major, int minor, char *conffile);
    int fd_ext_dict_s6a_init(int major, int minor, char *conffile);

    ret = fd_ext_dict_nas_mipv6_init(
            FD_PROJECT_VERSION_MAJOR, FD_PROJECT_VERSION_MINOR, NULL);
    if (ret != 0) 
    {
        d_error("fd_ext_init_nas_mipv6() failed");
        return ret;
    } 
    ret = fd_ext_dict_s6a_init(
            FD_PROJECT_VERSION_MAJOR, FD_PROJECT_VERSION_MINOR, NULL);
    if (ret != 0) 
    {
        d_error("fd_ext_init_dict_s6a() failed");
        return ret;
    } 

    return 0;
}

int s6a_dict_init(void)
{
    CHECK_FCT(s6a_ext_load());

    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_VENDOR, 
        VENDOR_BY_ID, (void *)&s6a_config->vendor_id, &s6a_vendor, ENOENT));

    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_APPLICATION, 
        APPLICATION_BY_ID, (void *)&s6a_config->appli_id, &s6a_appli, ENOENT));

    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Authentication-Information-Request", &s6a_cmd_air, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Authentication-Information-Answer", &s6a_cmd_aia, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Update-Location-Request", &s6a_cmd_ulr, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Update-Location-Answer", &s6a_cmd_ula, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Purge-UE-Request", &s6a_cmd_pur, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Purge-UE-Answer", &s6a_cmd_pua, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Cancel-Location-Request", &s6a_cmd_clr, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Cancel-Location-Answer", &s6a_cmd_cla, ENOENT));
	
    CHECK_FCT(fd_dict_search (fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
                "Origin-Host", &s6a_origin_host, ENOENT));
    CHECK_FCT(fd_dict_search (fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
                "Origin-Realm", &s6a_origin_realm, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Destination-Host", &s6a_destination_host, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Destination-Realm", &s6a_destination_realm, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "User-Name", &s6a_user_name, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Session-Id", &s6a_session_id, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Auth-Session-State", &s6a_auth_session_state, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Result-Code", &s6a_result_code, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Experimental-Result", &s6a_experimental_result, ENOENT));
    CHECK_FCT(fd_dict_search (fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Vendor-Id", &s6a_vendor_id, ENOENT));
    CHECK_FCT(fd_dict_search (fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Experimental-Result-Code", &s6a_experimental_result_code, ENOENT));

    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Visited-PLMN-Id", 
            &s6a_visited_plmn_id, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "RAT-Type", &s6a_rat_type, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "ULR-Flags", &s6a_ulr_flags, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "ULA-Flags", &s6a_ula_flags, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Subscription-Data", 
            &s6a_subscription_data, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Requested-EUTRAN-Authentication-Info", 
            &s6a_req_eutran_auth_info, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Number-Of-Requested-Vectors", 
            &s6a_number_of_requested_vectors, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Immediate-Response-Preferred", 
            &s6a_immediate_response_preferred, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Authentication-Info", 
            &s6a_authentication_info, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Re-Synchronization-Info", 
            &s6a_re_synchronization_info, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Service-Selection", 
            &s6a_service_selection, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "UE-SRVCC-Capability", 
            &s6a_ue_srvcc_cap, ENOENT));
    CHECK_FCT(fd_dict_search (fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "User-Name", &s6a_imsi, ENOENT));
    CHECK_FCT(fd_dict_search (fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "IMEI", &s6a_imei, ENOENT));
    CHECK_FCT(fd_dict_search (fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Software-Version", 
            &s6a_software_version, ENOENT));
    CHECK_FCT(fd_dict_search (fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Supported-Features", 
            &s6a_supported_features, ENOENT));
    CHECK_FCT(fd_dict_search (fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Requested-EUTRAN-Authentication-Info", 
            &s6a_req_e_utran_auth_info, ENOENT));
    CHECK_FCT(fd_dict_search (fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Number-Of-Requested-Vectors", 
            &s6a_req_nb_of_req_vectors, ENOENT));
    CHECK_FCT(fd_dict_search (fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Re-Synchronization-Info", 
            &s6a_req_resync_info, ENOENT));
    CHECK_FCT(fd_dict_search (fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Requested-UTRAN-GERAN-Authentication-Info", 
            &s6a_req_geran_auth_info, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "E-UTRAN-Vector", 
            &s6a_e_utran_vector, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "RAND", &s6a_rand, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "XRES", &s6a_xres, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "AUTN", &s6a_autn, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "KASME", &s6a_kasme, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "PUR-Flags", &s6a_pur_flags, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "PUA-Flags", &s6a_pua_flags, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Terminal-Information",
            &s6a_terminal_info, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "GMLC-Address", &s6a_gmlc_addr, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Subscriber-Status",
            &s6a_subscriber_status, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "MSISDN", &s6a_msisdn, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "AMBR", &s6a_ambr, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Network-Access-Mode",
            &s6a_network_access_mode, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Access-Restriction-Data",
            &s6a_access_restriction_data, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "APN-Configuration-Profile",
            &s6a_apn_configuration_profile, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Subscribed-Periodic-RAU-TAU-Timer",
            &s6a_subscribed_rau_tau_timer, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Context-Identifier",
            &s6a_context_identifier, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "All-APN-Configurations-Included-Indicator",
            &s6a_all_apn_conf_inc_ind, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "APN-Configuration",
            &s6a_apn_configuration, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-UL",
            &s6a_max_bandwidth_ul, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-DL",
            &s6a_max_bandwidth_dl, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "PDN-Type", &s6a_pdn_type, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "EPS-Subscribed-QoS-Profile",
            &s6a_eps_subscribed_qos_profile, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "QoS-Class-Identifier",
            &s6a_qos_class_identifier, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Allocation-Retention-Priority",
            &s6a_allocation_retention_priority, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Priority-Level",
            &s6a_priority_level, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Pre-emption-Capability",
            &s6a_pre_emption_capability, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Pre-emption-Vulnerability",
            &s6a_pre_emption_vulnerability, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP,
        AVP_BY_NAME_ALL_VENDORS, "Served-Party-IP-Address",
            &s6a_served_party_ip_addr, ENOENT));
	
	return 0;
}
