#include "s6a_dict.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )	\
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct dict_object *s6a_application = NULL;

struct dict_object *s6a_cmd_air = NULL;
struct dict_object *s6a_cmd_aia = NULL;
struct dict_object *s6a_cmd_ulr = NULL;
struct dict_object *s6a_cmd_ula = NULL;
struct dict_object *s6a_cmd_pur = NULL;
struct dict_object *s6a_cmd_pua = NULL;

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
struct dict_object *s6a_ue_srvcc_capability = NULL;
struct dict_object *s6a_e_utran_vector = NULL;
struct dict_object *s6a_rand = NULL;
struct dict_object *s6a_xres = NULL;
struct dict_object *s6a_autn = NULL;
struct dict_object *s6a_kasme = NULL;
struct dict_object *s6a_subscriber_status = NULL;
struct dict_object *s6a_ambr = NULL;
struct dict_object *s6a_network_access_mode = NULL;
struct dict_object *s6a_access_restriction_data = NULL;
struct dict_object *s6a_apn_configuration_profile = NULL;
struct dict_object *s6a_subscribed_rau_tau_timer = NULL;
struct dict_object *s6a_context_identifier = NULL;
struct dict_object *s6a_all_apn_configuration_included_indicator = NULL;
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

int s6a_dict_init(void)
{
    application_id_t id = S6A_APPLICATION_ID;

    CHECK_dict_search(DICT_APPLICATION, APPLICATION_BY_ID, (void *)&id, &s6a_application);

    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Authentication-Information-Request", &s6a_cmd_air);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Authentication-Information-Answer", &s6a_cmd_aia);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Update-Location-Request", &s6a_cmd_ulr);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Update-Location-Answer", &s6a_cmd_ula);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Purge-UE-Request", &s6a_cmd_pur);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Purge-UE-Answer", &s6a_cmd_pua);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Visited-PLMN-Id", &s6a_visited_plmn_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RAT-Type", &s6a_rat_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "ULR-Flags", &s6a_ulr_flags);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "ULA-Flags", &s6a_ula_flags);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "UE-SRVCC-Capability", &s6a_ue_srvcc_capability);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Requested-EUTRAN-Authentication-Info", &s6a_req_eutran_auth_info);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Number-Of-Requested-Vectors", &s6a_number_of_requested_vectors);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Immediate-Response-Preferred", &s6a_immediate_response_preferred);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Re-Synchronization-Info", &s6a_re_synchronization_info);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Authentication-Info", &s6a_authentication_info);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "E-UTRAN-Vector", &s6a_e_utran_vector);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RAND", &s6a_rand);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "XRES", &s6a_xres);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "AUTN", &s6a_autn);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "KASME", &s6a_kasme);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "EPS-Subscribed-QoS-Profile", &s6a_eps_subscribed_qos_profile);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "QoS-Class-Identifier", &s6a_qos_class_identifier);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Allocation-Retention-Priority", &s6a_allocation_retention_priority);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Priority-Level", &s6a_priority_level);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Pre-emption-Capability", &s6a_pre_emption_capability);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Pre-emption-Vulnerability", &s6a_pre_emption_vulnerability);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "AMBR", &s6a_ambr);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-UL", &s6a_max_bandwidth_ul);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-DL", &s6a_max_bandwidth_dl);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "APN-Configuration-Profile", &s6a_apn_configuration_profile);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Context-Identifier", &s6a_context_identifier);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "All-APN-Configurations-Included-Indicator", &s6a_all_apn_configuration_included_indicator);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "APN-Configuration", &s6a_apn_configuration);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Service-Selection", &s6a_service_selection);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "PDN-Type", &s6a_pdn_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscription-Data", &s6a_subscription_data);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscriber-Status", &s6a_subscriber_status);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Network-Access-Mode", &s6a_network_access_mode);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Access-Restriction-Data", &s6a_access_restriction_data);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscribed-Periodic-RAU-TAU-Timer", &s6a_subscribed_rau_tau_timer);

	return 0;
}
