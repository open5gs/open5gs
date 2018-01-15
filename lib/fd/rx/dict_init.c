#define TRACE_MODULE _rx_dict

#include "rx_dict.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )	\
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

struct dict_object *rx_application = NULL;

struct dict_object *rx_cmd_aar = NULL;
struct dict_object *rx_cmd_aaa = NULL;
struct dict_object *rx_cmd_asr = NULL;
struct dict_object *rx_cmd_asa = NULL;
struct dict_object *rx_cmd_str = NULL;
struct dict_object *rx_cmd_sta = NULL;

struct dict_object *rx_media_component_description = NULL;
struct dict_object *rx_media_component_number = NULL;
struct dict_object *rx_media_type = NULL;
struct dict_object *rx_max_requested_bandwidth_ul = NULL;
struct dict_object *rx_max_requested_bandwidth_dl = NULL;
struct dict_object *rx_min_requested_bandwidth_ul = NULL;
struct dict_object *rx_min_requested_bandwidth_dl = NULL;
struct dict_object *rx_rr_bandwidth = NULL;
struct dict_object *rx_rs_bandwidth = NULL;
struct dict_object *rx_flow_status = NULL;
struct dict_object *rx_codec_data = NULL;
struct dict_object *rx_media_sub_component = NULL;
struct dict_object *rx_flow_number = NULL;
struct dict_object *rx_flow_usage = NULL;
struct dict_object *rx_flow_description = NULL;
struct dict_object *rx_subscription_id = NULL;
struct dict_object *rx_subscription_id_type = NULL;
struct dict_object *rx_subscription_id_data = NULL;
struct dict_object *rx_specific_action = NULL;
struct dict_object *rx_framed_ip_address = NULL;
struct dict_object *rx_framed_ipv6_prefix = NULL;
struct dict_object *rx_ip_can_type = NULL;
struct dict_object *rx_rat_type = NULL;
struct dict_object *rx_abort_cause = NULL;
struct dict_object *rx_termination_cause = NULL;

int rx_dict_init(void)
{
    application_id_t id = RX_APPLICATION_ID;

    CHECK_dict_search(DICT_APPLICATION, APPLICATION_BY_ID, (void *)&id, &rx_application);

    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "AA-Request", &rx_cmd_aar);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "AA-Answer", &rx_cmd_aaa);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Abort-Session-Request", &rx_cmd_asr);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Abort-Session-Answer", &rx_cmd_asa);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Session-Termination-Request", &rx_cmd_str);
    CHECK_dict_search(DICT_COMMAND, CMD_BY_NAME, "Session-Termination-Answer", &rx_cmd_sta);

    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Component-Description", &rx_media_component_description);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Component-Number", &rx_media_component_number);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Type", &rx_media_type);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-UL" , &rx_max_requested_bandwidth_ul);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Max-Requested-Bandwidth-DL" , &rx_max_requested_bandwidth_dl);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Min-Requested-Bandwidth-UL" , &rx_min_requested_bandwidth_ul);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Min-Requested-Bandwidth-DL" , &rx_min_requested_bandwidth_dl);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RR-Bandwidth" , &rx_rr_bandwidth);
	CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RS-Bandwidth" , &rx_rs_bandwidth);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Status", &rx_flow_status);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Codec-Data", &rx_codec_data);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Media-Sub-Component", &rx_media_sub_component);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Number", &rx_flow_number);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Usage", &rx_flow_usage);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Flow-Description", &rx_flow_description);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscription-Id", &rx_subscription_id);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscription-Id-Type", &rx_subscription_id_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Subscription-Id-Data", &rx_subscription_id_data);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Specific-Action", &rx_specific_action);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Framed-IP-Address", &rx_framed_ip_address);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Framed-IPv6-Prefix", &rx_framed_ipv6_prefix);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "IP-CAN-Type", &rx_ip_can_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "RAT-Type", &rx_rat_type);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Abort-Cause", &rx_abort_cause);
    CHECK_dict_search(DICT_AVP, AVP_BY_NAME_ALL_VENDORS, "Termination-Cause", &rx_termination_cause);

    return 0;
}
