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

extern struct dict_object *gx_cc_request_typeg;
extern struct dict_object *gx_cc_request_numberg;
extern struct dict_object *gx_network_request_supportg;
extern struct dict_object *gx_subscription_idg;
extern struct dict_object *gx_supported_featuresg;
extern struct dict_object *gx_framed_ip_addressg;
extern struct dict_object *gx_framed_ipv6_prefixg;
extern struct dict_object *gx_ip_can_typeg;
extern struct dict_object *gx_rat_typeg;
extern struct dict_object *gx_qos_informationg;
extern struct dict_object *gx_3gpp_user_location_infog;
extern struct dict_object *gx_called_station_idg;
extern struct dict_object *gx_default_eps_bearer_qosg;
extern struct dict_object *gx_3gpp_ms_timezoneg;
extern struct dict_object *gx_event_triggerg;
extern struct dict_object *gx_bearer_control_modeg;
extern struct dict_object *gx_charging_rule_installg;
extern struct dict_object *gx_charging_rule_definitiong;
extern struct dict_object *gx_charging_rule_base_nameg;
extern struct dict_object *gx_charging_rule_nameg;
extern struct dict_object *gx_flow_informationg;
extern struct dict_object *gx_flow_directiong;
extern struct dict_object *gx_flow_descriptiong;
extern struct dict_object *gx_flow_statusg;
extern struct dict_object *gx_precedenceg;
extern struct dict_object *gx_flowsg;

int gx_dict_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __GX_LIB_H__ */
