/*
 * sm_policy_update_context_data.h
 *
 *
 */

#ifndef _OpenAPI_sm_policy_update_context_data_H_
#define _OpenAPI_sm_policy_update_context_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "acc_net_ch_id.h"
#include "access_type.h"
#include "accu_usage_report.h"
#include "additional_access_info.h"
#include "ambr.h"
#include "app_detection_info.h"
#include "atsss_capability.h"
#include "credit_management_status.h"
#include "ip_multicast_address_info.h"
#include "ma_pdu_indication.h"
#include "plmn_id_nid.h"
#include "policy_control_request_trigger.h"
#include "port_management_container.h"
#include "presence_info.h"
#include "qos_flow_usage.h"
#include "qos_monitoring_report.h"
#include "qos_notification_control_info.h"
#include "rat_type.h"
#include "rule_report.h"
#include "serving_nf_identity.h"
#include "session_rule_report.h"
#include "subscribed_default_qos.h"
#include "trace_data.h"
#include "tsn_bridge_info.h"
#include "ue_initiated_resource_request.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_policy_update_context_data_s OpenAPI_sm_policy_update_context_data_t;
typedef struct OpenAPI_sm_policy_update_context_data_s {
    OpenAPI_list_t *rep_policy_ctrl_req_triggers;
    OpenAPI_list_t *acc_net_ch_ids;
    OpenAPI_access_type_e access_type;
    OpenAPI_rat_type_e rat_type;
    struct OpenAPI_additional_access_info_s *add_access_info;
    struct OpenAPI_additional_access_info_s *rel_access_info;
    struct OpenAPI_plmn_id_nid_s *serving_network;
    struct OpenAPI_user_location_s *user_location_info;
    char *ue_time_zone;
    char *rel_ipv4_address;
    char *ipv4_address;
    char *ip_domain;
    char *ipv6_address_prefix;
    char *rel_ipv6_address_prefix;
    char *add_ipv6_addr_prefixes;
    char *add_rel_ipv6_addr_prefixes;
    char *rel_ue_mac;
    char *ue_mac;
    struct OpenAPI_ambr_s *subs_sess_ambr;
    char *auth_prof_index;
    struct OpenAPI_subscribed_default_qos_s *subs_def_qos;
    int num_of_pack_filter;
    OpenAPI_list_t *accu_usage_reports;
    int _3gpp_ps_data_off_status;
    OpenAPI_list_t *app_detection_infos;
    OpenAPI_list_t *rule_reports;
    OpenAPI_list_t *sess_rule_reports;
    OpenAPI_list_t *qnc_reports;
    OpenAPI_list_t *qos_mon_reports;
    char *user_location_info_time;
    OpenAPI_list_t* rep_pra_infos;
    struct OpenAPI_ue_initiated_resource_request_s *ue_init_res_req;
    int ref_qos_indication;
    struct OpenAPI_qos_flow_usage_s *qos_flow_usage;
    struct OpenAPI_credit_management_status_s *credit_manage_status;
    struct OpenAPI_serving_nf_identity_s *serv_nf_id;
    struct OpenAPI_trace_data_s *trace_req;
    struct OpenAPI_ma_pdu_indication_s *ma_pdu_ind;
    struct OpenAPI_atsss_capability_s *atsss_capab;
    struct OpenAPI_tsn_bridge_info_s *tsn_bridge_info;
    struct OpenAPI_port_management_container_s *tsn_port_man_cont_dstt;
    OpenAPI_list_t *tsn_port_man_cont_nwtts;
    OpenAPI_list_t *mul_addr_infos;
} OpenAPI_sm_policy_update_context_data_t;

OpenAPI_sm_policy_update_context_data_t *OpenAPI_sm_policy_update_context_data_create(
    OpenAPI_list_t *rep_policy_ctrl_req_triggers,
    OpenAPI_list_t *acc_net_ch_ids,
    OpenAPI_access_type_e access_type,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_additional_access_info_t *add_access_info,
    OpenAPI_additional_access_info_t *rel_access_info,
    OpenAPI_plmn_id_nid_t *serving_network,
    OpenAPI_user_location_t *user_location_info,
    char *ue_time_zone,
    char *rel_ipv4_address,
    char *ipv4_address,
    char *ip_domain,
    char *ipv6_address_prefix,
    char *rel_ipv6_address_prefix,
    char *add_ipv6_addr_prefixes,
    char *add_rel_ipv6_addr_prefixes,
    char *rel_ue_mac,
    char *ue_mac,
    OpenAPI_ambr_t *subs_sess_ambr,
    char *auth_prof_index,
    OpenAPI_subscribed_default_qos_t *subs_def_qos,
    int num_of_pack_filter,
    OpenAPI_list_t *accu_usage_reports,
    int _3gpp_ps_data_off_status,
    OpenAPI_list_t *app_detection_infos,
    OpenAPI_list_t *rule_reports,
    OpenAPI_list_t *sess_rule_reports,
    OpenAPI_list_t *qnc_reports,
    OpenAPI_list_t *qos_mon_reports,
    char *user_location_info_time,
    OpenAPI_list_t* rep_pra_infos,
    OpenAPI_ue_initiated_resource_request_t *ue_init_res_req,
    int ref_qos_indication,
    OpenAPI_qos_flow_usage_t *qos_flow_usage,
    OpenAPI_credit_management_status_t *credit_manage_status,
    OpenAPI_serving_nf_identity_t *serv_nf_id,
    OpenAPI_trace_data_t *trace_req,
    OpenAPI_ma_pdu_indication_t *ma_pdu_ind,
    OpenAPI_atsss_capability_t *atsss_capab,
    OpenAPI_tsn_bridge_info_t *tsn_bridge_info,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts,
    OpenAPI_list_t *mul_addr_infos
    );
void OpenAPI_sm_policy_update_context_data_free(OpenAPI_sm_policy_update_context_data_t *sm_policy_update_context_data);
OpenAPI_sm_policy_update_context_data_t *OpenAPI_sm_policy_update_context_data_parseFromJSON(cJSON *sm_policy_update_context_dataJSON);
cJSON *OpenAPI_sm_policy_update_context_data_convertToJSON(OpenAPI_sm_policy_update_context_data_t *sm_policy_update_context_data);
OpenAPI_sm_policy_update_context_data_t *OpenAPI_sm_policy_update_context_data_copy(OpenAPI_sm_policy_update_context_data_t *dst, OpenAPI_sm_policy_update_context_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_policy_update_context_data_H_ */

