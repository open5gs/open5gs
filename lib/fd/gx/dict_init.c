#define TRACE_MODULE _gx_dict

#include "gx_dict.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )	\
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct dict_object *gx_application = NULL;

struct dict_object *gx_cmd_ccr = NULL;
struct dict_object *gx_cmd_cca = NULL;
struct dict_object *gx_cmd_rar = NULL;
struct dict_object *gx_cmd_raa = NULL;

struct dict_object *gx_cc_request_type = NULL;
struct dict_object *gx_cc_request_number = NULL;
struct dict_object *gx_network_request_support = NULL;
struct dict_object *gx_subscription_id = NULL;
struct dict_object *gx_subscription_id_type = NULL;
struct dict_object *gx_subscription_id_data = NULL;
struct dict_object *gx_supported_features = NULL;
struct dict_object *gx_feature_list_id = NULL;
struct dict_object *gx_feature_list = NULL;
struct dict_object *gx_framed_ip_address = NULL;
struct dict_object *gx_framed_ipv6_prefix = NULL;
struct dict_object *gx_ip_can_type = NULL;
struct dict_object *gx_rat_type = NULL;
struct dict_object *gx_qos_information = NULL;
struct dict_object *gx_qos_class_identifier = NULL;
struct dict_object *gx_max_requested_bandwidth_ul = NULL;
struct dict_object *gx_max_requested_bandwidth_dl = NULL;
struct dict_object *gx_min_requested_bandwidth_ul = NULL;
struct dict_object *gx_min_requested_bandwidth_dl = NULL;
struct dict_object *gx_guaranteed_bitrate_ul = NULL;
struct dict_object *gx_guaranteed_bitrate_dl = NULL;
struct dict_object *gx_allocation_retention_priority = NULL;
struct dict_object *gx_priority_level = NULL;
struct dict_object *gx_pre_emption_capability = NULL;
struct dict_object *gx_pre_emption_vulnerability = NULL;
struct dict_object *gx_apn_aggregate_max_bitrate_ul = NULL;
struct dict_object *gx_apn_aggregate_max_bitrate_dl = NULL;
struct dict_object *gx_3gpp_user_location_info = NULL;
struct dict_object *gx_called_station_id = NULL;
struct dict_object *gx_default_eps_bearer_qos = NULL;
struct dict_object *gx_3gpp_ms_timezone = NULL;
struct dict_object *gx_event_trigger = NULL;
struct dict_object *gx_bearer_control_mode = NULL;
struct dict_object *gx_charging_rule_install = NULL;
struct dict_object *gx_charging_rule_remove = NULL;
struct dict_object *gx_charging_rule_definition = NULL;
struct dict_object *gx_charging_rule_base_name = NULL;
struct dict_object *gx_charging_rule_name = NULL;
struct dict_object *gx_flow_information = NULL;
struct dict_object *gx_flow_direction = NULL;
struct dict_object *gx_flow_description = NULL;
struct dict_object *gx_flow_status = NULL;
struct dict_object *gx_precedence = NULL;
struct dict_object *gx_flows = NULL;
struct dict_object *gx_media_component_description = NULL;
struct dict_object *gx_media_component_number = NULL;
struct dict_object *gx_media_type = NULL;
struct dict_object *gx_rr_bandwidth = NULL;
struct dict_object *gx_rs_bandwidth = NULL;
struct dict_object *gx_codec_data = NULL;
struct dict_object *gx_media_sub_component = NULL;
struct dict_object *gx_flow_number = NULL;
struct dict_object *gx_flow_usage = NULL;

int gx_dict_init(void)
{
    application_id_t id = GX_APPLICATION_ID;

    CHECK_dict_search(DICT_APPLICATION, APPLICATION_BY_ID, (void *)&id, &gx_application);

    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Credit-Control-Request", &gx_cmd_ccr);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Credit-Control-Answer", &gx_cmd_cca);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Re-Auth-Request", &gx_cmd_rar);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Re-Auth-Answer", &gx_cmd_raa);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "CC-Request-Type", &gx_cc_request_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "CC-Request-Number", &gx_cc_request_number);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Network-Request-Support", &gx_network_request_support);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscription-Id", &gx_subscription_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscription-Id-Type", &gx_subscription_id_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscription-Id-Data", &gx_subscription_id_data);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Supported-Features", &gx_supported_features);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Feature-List-ID", &gx_feature_list_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Feature-List", &gx_feature_list);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Framed-IP-Address", &gx_framed_ip_address);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Framed-IPv6-Prefix", &gx_framed_ipv6_prefix);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "IP-CAN-Type", &gx_ip_can_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RAT-Type", &gx_rat_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "QoS-Information", &gx_qos_information);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "QoS-Class-Identifier" , &gx_qos_class_identifier);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-UL" , &gx_max_requested_bandwidth_ul);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-DL" , &gx_max_requested_bandwidth_dl);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Min-Requested-Bandwidth-UL" , &gx_min_requested_bandwidth_ul);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Min-Requested-Bandwidth-DL" , &gx_min_requested_bandwidth_dl);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Guaranteed-Bitrate-UL" , &gx_guaranteed_bitrate_ul);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Guaranteed-Bitrate-DL" , &gx_guaranteed_bitrate_dl);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Allocation-Retention-Priority" , &gx_allocation_retention_priority);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Priority-Level", &gx_priority_level);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Pre-emption-Capability", &gx_pre_emption_capability);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Pre-emption-Vulnerability", &gx_pre_emption_vulnerability);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "APN-Aggregate-Max-Bitrate-UL" , &gx_apn_aggregate_max_bitrate_ul);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "APN-Aggregate-Max-Bitrate-DL" , &gx_apn_aggregate_max_bitrate_dl);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-User-Location-Info", &gx_3gpp_user_location_info);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Called-Station-Id", &gx_called_station_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Default-EPS-Bearer-QoS", &gx_default_eps_bearer_qos);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "3GPP-MS-TimeZone", &gx_3gpp_ms_timezone);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Event-Trigger", &gx_event_trigger);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Bearer-Control-Mode", &gx_bearer_control_mode);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Charging-Rule-Install", &gx_charging_rule_install);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Charging-Rule-Remove", &gx_charging_rule_remove);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Charging-Rule-Definition", &gx_charging_rule_definition);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Charging-Rule-Base-Name", &gx_charging_rule_base_name);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Charging-Rule-Name", &gx_charging_rule_name);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Information", &gx_flow_information);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Direction", &gx_flow_direction);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Description", &gx_flow_description);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Status", &gx_flow_status);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Precedence", &gx_precedence);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flows", &gx_flows);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Component-Description", &gx_media_component_description);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Component-Number", &gx_media_component_number);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Type", &gx_media_type);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RR-Bandwidth" , &gx_rr_bandwidth);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RS-Bandwidth" , &gx_rs_bandwidth);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Codec-Data", &gx_codec_data);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Sub-Component", &gx_media_sub_component);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Number", &gx_flow_number);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Usage", &gx_flow_usage);

    return 0;
}
