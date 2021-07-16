/*
 * sm_policy_decision.h
 *
 * 
 */

#ifndef _OpenAPI_sm_policy_decision_H_
#define _OpenAPI_sm_policy_decision_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "bridge_management_container.h"
#include "charging_data.h"
#include "charging_information.h"
#include "condition_data.h"
#include "pcc_rule.h"
#include "policy_control_request_trigger.h"
#include "port_management_container.h"
#include "presence_info_rm.h"
#include "qos_characteristics.h"
#include "qos_data.h"
#include "qos_flow_usage.h"
#include "qos_monitoring_data.h"
#include "requested_rule_data.h"
#include "requested_usage_data.h"
#include "session_rule.h"
#include "sm_policy_association_release_cause.h"
#include "traffic_control_data.h"
#include "usage_monitoring_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_policy_decision_s OpenAPI_sm_policy_decision_t;
typedef struct OpenAPI_sm_policy_decision_s {
    OpenAPI_list_t* sess_rules;
    OpenAPI_list_t* pcc_rules;
    bool is_pcscf_rest_indication;
    int pcscf_rest_indication;
    OpenAPI_list_t* qos_decs;
    OpenAPI_list_t* chg_decs;
    struct OpenAPI_charging_information_s *charging_info;
    OpenAPI_list_t* traff_cont_decs;
    OpenAPI_list_t* um_decs;
    OpenAPI_list_t* qos_chars;
    OpenAPI_list_t* qos_mon_decs;
    bool is_reflective_qo_s_timer;
    int reflective_qo_s_timer;
    OpenAPI_list_t* conds;
    char *revalidation_time;
    bool is_offline;
    int offline;
    bool is_online;
    int online;
    OpenAPI_list_t *policy_ctrl_req_triggers;
    OpenAPI_list_t *last_req_rule_data;
    struct OpenAPI_requested_usage_data_s *last_req_usage_data;
    OpenAPI_list_t* pra_infos;
    bool is_ipv4_index;
    int ipv4_index;
    bool is_ipv6_index;
    int ipv6_index;
    OpenAPI_qos_flow_usage_e qos_flow_usage;
    OpenAPI_sm_policy_association_release_cause_e rel_cause;
    char *supp_feat;
    struct OpenAPI_bridge_management_container_s *tsn_bridge_man_cont;
    struct OpenAPI_port_management_container_s *tsn_port_man_cont_dstt;
    OpenAPI_list_t *tsn_port_man_cont_nwtts;
    bool is_red_sess_indication;
    int red_sess_indication;
} OpenAPI_sm_policy_decision_t;

OpenAPI_sm_policy_decision_t *OpenAPI_sm_policy_decision_create(
    OpenAPI_list_t* sess_rules,
    OpenAPI_list_t* pcc_rules,
    bool is_pcscf_rest_indication,
    int pcscf_rest_indication,
    OpenAPI_list_t* qos_decs,
    OpenAPI_list_t* chg_decs,
    OpenAPI_charging_information_t *charging_info,
    OpenAPI_list_t* traff_cont_decs,
    OpenAPI_list_t* um_decs,
    OpenAPI_list_t* qos_chars,
    OpenAPI_list_t* qos_mon_decs,
    bool is_reflective_qo_s_timer,
    int reflective_qo_s_timer,
    OpenAPI_list_t* conds,
    char *revalidation_time,
    bool is_offline,
    int offline,
    bool is_online,
    int online,
    OpenAPI_list_t *policy_ctrl_req_triggers,
    OpenAPI_list_t *last_req_rule_data,
    OpenAPI_requested_usage_data_t *last_req_usage_data,
    OpenAPI_list_t* pra_infos,
    bool is_ipv4_index,
    int ipv4_index,
    bool is_ipv6_index,
    int ipv6_index,
    OpenAPI_qos_flow_usage_e qos_flow_usage,
    OpenAPI_sm_policy_association_release_cause_e rel_cause,
    char *supp_feat,
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts,
    bool is_red_sess_indication,
    int red_sess_indication
);
void OpenAPI_sm_policy_decision_free(OpenAPI_sm_policy_decision_t *sm_policy_decision);
OpenAPI_sm_policy_decision_t *OpenAPI_sm_policy_decision_parseFromJSON(cJSON *sm_policy_decisionJSON);
cJSON *OpenAPI_sm_policy_decision_convertToJSON(OpenAPI_sm_policy_decision_t *sm_policy_decision);
OpenAPI_sm_policy_decision_t *OpenAPI_sm_policy_decision_copy(OpenAPI_sm_policy_decision_t *dst, OpenAPI_sm_policy_decision_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_policy_decision_H_ */

