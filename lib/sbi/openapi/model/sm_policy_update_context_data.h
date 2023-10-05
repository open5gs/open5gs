/*
 * sm_policy_update_context_data.h
 *
 * Contains the policy control request trigger(s) that were met and the corresponding new value(s) or the error report of the policy enforcement.
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
#include "bridge_management_container.h"
#include "credit_management_status.h"
#include "ddd_traffic_descriptor.h"
#include "dl_data_delivery_status.h"
#include "invalid_param.h"
#include "ip_multicast_address_info.h"
#include "ma_pdu_indication.h"
#include "npcf_atsss_capability.h"
#include "nwdaf_data.h"
#include "pcf_ue_callback_info.h"
#include "plmn_id_nid.h"
#include "policy_control_request_trigger.h"
#include "policy_decision_failure_code.h"
#include "port_management_container.h"
#include "presence_info.h"
#include "qos_flow_usage.h"
#include "qos_monitoring_report.h"
#include "qos_notification_control_info.h"
#include "rat_type.h"
#include "rule_report.h"
#include "satellite_backhaul_category.h"
#include "serving_nf_identity.h"
#include "session_rule_report.h"
#include "subscribed_default_qos.h"
#include "trace_data.h"
#include "tsn_bridge_info.h"
#include "ue_initiated_resource_request.h"
#include "user_location.h"
#include "vplmn_qos.h"

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
    struct OpenAPI_vplmn_qos_s *vplmn_qos;
    bool is_vplmn_qos_not_app;
    int vplmn_qos_not_app;
    bool is_num_of_pack_filter;
    int num_of_pack_filter;
    OpenAPI_list_t *accu_usage_reports;
    bool is__3gpp_ps_data_off_status;
    int _3gpp_ps_data_off_status;
    OpenAPI_list_t *app_detection_infos;
    OpenAPI_list_t *rule_reports;
    OpenAPI_list_t *sess_rule_reports;
    OpenAPI_list_t *qnc_reports;
    OpenAPI_list_t *qos_mon_reports;
    char *user_location_info_time;
    OpenAPI_list_t* rep_pra_infos;
    struct OpenAPI_ue_initiated_resource_request_s *ue_init_res_req;
    bool is_ref_qos_indication;
    int ref_qos_indication;
    OpenAPI_qos_flow_usage_e qos_flow_usage;
    OpenAPI_credit_management_status_e credit_manage_status;
    struct OpenAPI_serving_nf_identity_s *serv_nf_id;
    bool is_trace_req_null;
    struct OpenAPI_trace_data_s *trace_req;
    OpenAPI_ma_pdu_indication_e ma_pdu_ind;
    OpenAPI_npcf_atsss_capability_e atsss_capab;
    struct OpenAPI_tsn_bridge_info_s *tsn_bridge_info;
    struct OpenAPI_bridge_management_container_s *tsn_bridge_man_cont;
    struct OpenAPI_port_management_container_s *tsn_port_man_cont_dstt;
    OpenAPI_list_t *tsn_port_man_cont_nwtts;
    OpenAPI_list_t *mul_addr_infos;
    OpenAPI_list_t *policy_dec_failure_reports;
    OpenAPI_list_t *invalid_policy_decs;
    OpenAPI_list_t *traffic_descriptors;
    char *pcc_rule_id;
    OpenAPI_list_t *types_of_notif;
    OpenAPI_list_t *inter_grp_ids;
    OpenAPI_satellite_backhaul_category_e sat_backhaul_category;
    bool is_pcf_ue_info_null;
    struct OpenAPI_pcf_ue_callback_info_s *pcf_ue_info;
    bool is_nwdaf_datas_null;
    OpenAPI_list_t *nwdaf_datas;
    bool is_an_gw_status;
    int an_gw_status;
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
    OpenAPI_vplmn_qos_t *vplmn_qos,
    bool is_vplmn_qos_not_app,
    int vplmn_qos_not_app,
    bool is_num_of_pack_filter,
    int num_of_pack_filter,
    OpenAPI_list_t *accu_usage_reports,
    bool is__3gpp_ps_data_off_status,
    int _3gpp_ps_data_off_status,
    OpenAPI_list_t *app_detection_infos,
    OpenAPI_list_t *rule_reports,
    OpenAPI_list_t *sess_rule_reports,
    OpenAPI_list_t *qnc_reports,
    OpenAPI_list_t *qos_mon_reports,
    char *user_location_info_time,
    OpenAPI_list_t* rep_pra_infos,
    OpenAPI_ue_initiated_resource_request_t *ue_init_res_req,
    bool is_ref_qos_indication,
    int ref_qos_indication,
    OpenAPI_qos_flow_usage_e qos_flow_usage,
    OpenAPI_credit_management_status_e credit_manage_status,
    OpenAPI_serving_nf_identity_t *serv_nf_id,
    bool is_trace_req_null,
    OpenAPI_trace_data_t *trace_req,
    OpenAPI_ma_pdu_indication_e ma_pdu_ind,
    OpenAPI_npcf_atsss_capability_e atsss_capab,
    OpenAPI_tsn_bridge_info_t *tsn_bridge_info,
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts,
    OpenAPI_list_t *mul_addr_infos,
    OpenAPI_list_t *policy_dec_failure_reports,
    OpenAPI_list_t *invalid_policy_decs,
    OpenAPI_list_t *traffic_descriptors,
    char *pcc_rule_id,
    OpenAPI_list_t *types_of_notif,
    OpenAPI_list_t *inter_grp_ids,
    OpenAPI_satellite_backhaul_category_e sat_backhaul_category,
    bool is_pcf_ue_info_null,
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_info,
    bool is_nwdaf_datas_null,
    OpenAPI_list_t *nwdaf_datas,
    bool is_an_gw_status,
    int an_gw_status
);
void OpenAPI_sm_policy_update_context_data_free(OpenAPI_sm_policy_update_context_data_t *sm_policy_update_context_data);
OpenAPI_sm_policy_update_context_data_t *OpenAPI_sm_policy_update_context_data_parseFromJSON(cJSON *sm_policy_update_context_dataJSON);
cJSON *OpenAPI_sm_policy_update_context_data_convertToJSON(OpenAPI_sm_policy_update_context_data_t *sm_policy_update_context_data);
OpenAPI_sm_policy_update_context_data_t *OpenAPI_sm_policy_update_context_data_copy(OpenAPI_sm_policy_update_context_data_t *dst, OpenAPI_sm_policy_update_context_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_policy_update_context_data_H_ */

