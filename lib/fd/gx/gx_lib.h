#ifndef __GX_LIB_H__
#define __GX_LIB_H__

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GX_APPLICATION_ID 16777238

extern struct dict_object *gx_application;

extern struct dict_object *gx_cmd_ccr;
extern struct dict_object *gx_cmd_cca;

#define GX_CC_REQUEST_TYPE_INITIAL_REQUEST      1
#define GX_CC_REQUEST_TYPE_UPDATE_REQUEST       2
#define GX_CC_REQUEST_TYPE_TERMINATION_REQUEST  3
#define GX_CC_REQUEST_TYPE_EVENT_REQUEST        4
extern struct dict_object *gx_cc_request_type;
extern struct dict_object *gx_cc_request_number;
extern struct dict_object *gx_network_request_support;
extern struct dict_object *gx_subscription_id;
extern struct dict_object *gx_supported_features;
extern struct dict_object *gx_framed_ip_address;
extern struct dict_object *gx_framed_ipv6_prefix;
extern struct dict_object *gx_ip_can_type;
extern struct dict_object *gx_rat_type;
extern struct dict_object *gx_qos_information;
extern struct dict_object *gx_3gpp_user_location_info;
extern struct dict_object *gx_called_station_id;
extern struct dict_object *gx_default_eps_bearer_qos;
extern struct dict_object *gx_3gpp_ms_timezone;
extern struct dict_object *gx_event_trigger;
extern struct dict_object *gx_bearer_control_mode;
extern struct dict_object *gx_charging_rule_install;
extern struct dict_object *gx_charging_rule_definition;
extern struct dict_object *gx_charging_rule_base_name;
extern struct dict_object *gx_charging_rule_name;
extern struct dict_object *gx_flow_information;
extern struct dict_object *gx_flow_direction;
extern struct dict_object *gx_flow_description;
extern struct dict_object *gx_flow_status;
extern struct dict_object *gx_precedence;
extern struct dict_object *gx_flows;

int gx_dict_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __GX_LIB_H__ */
