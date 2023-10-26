
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_update_context_data.h"

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
)
{
    OpenAPI_sm_policy_update_context_data_t *sm_policy_update_context_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_policy_update_context_data_t));
    ogs_assert(sm_policy_update_context_data_local_var);

    sm_policy_update_context_data_local_var->rep_policy_ctrl_req_triggers = rep_policy_ctrl_req_triggers;
    sm_policy_update_context_data_local_var->acc_net_ch_ids = acc_net_ch_ids;
    sm_policy_update_context_data_local_var->access_type = access_type;
    sm_policy_update_context_data_local_var->rat_type = rat_type;
    sm_policy_update_context_data_local_var->add_access_info = add_access_info;
    sm_policy_update_context_data_local_var->rel_access_info = rel_access_info;
    sm_policy_update_context_data_local_var->serving_network = serving_network;
    sm_policy_update_context_data_local_var->user_location_info = user_location_info;
    sm_policy_update_context_data_local_var->ue_time_zone = ue_time_zone;
    sm_policy_update_context_data_local_var->rel_ipv4_address = rel_ipv4_address;
    sm_policy_update_context_data_local_var->ipv4_address = ipv4_address;
    sm_policy_update_context_data_local_var->ip_domain = ip_domain;
    sm_policy_update_context_data_local_var->ipv6_address_prefix = ipv6_address_prefix;
    sm_policy_update_context_data_local_var->rel_ipv6_address_prefix = rel_ipv6_address_prefix;
    sm_policy_update_context_data_local_var->add_ipv6_addr_prefixes = add_ipv6_addr_prefixes;
    sm_policy_update_context_data_local_var->add_rel_ipv6_addr_prefixes = add_rel_ipv6_addr_prefixes;
    sm_policy_update_context_data_local_var->rel_ue_mac = rel_ue_mac;
    sm_policy_update_context_data_local_var->ue_mac = ue_mac;
    sm_policy_update_context_data_local_var->subs_sess_ambr = subs_sess_ambr;
    sm_policy_update_context_data_local_var->auth_prof_index = auth_prof_index;
    sm_policy_update_context_data_local_var->subs_def_qos = subs_def_qos;
    sm_policy_update_context_data_local_var->vplmn_qos = vplmn_qos;
    sm_policy_update_context_data_local_var->is_vplmn_qos_not_app = is_vplmn_qos_not_app;
    sm_policy_update_context_data_local_var->vplmn_qos_not_app = vplmn_qos_not_app;
    sm_policy_update_context_data_local_var->is_num_of_pack_filter = is_num_of_pack_filter;
    sm_policy_update_context_data_local_var->num_of_pack_filter = num_of_pack_filter;
    sm_policy_update_context_data_local_var->accu_usage_reports = accu_usage_reports;
    sm_policy_update_context_data_local_var->is__3gpp_ps_data_off_status = is__3gpp_ps_data_off_status;
    sm_policy_update_context_data_local_var->_3gpp_ps_data_off_status = _3gpp_ps_data_off_status;
    sm_policy_update_context_data_local_var->app_detection_infos = app_detection_infos;
    sm_policy_update_context_data_local_var->rule_reports = rule_reports;
    sm_policy_update_context_data_local_var->sess_rule_reports = sess_rule_reports;
    sm_policy_update_context_data_local_var->qnc_reports = qnc_reports;
    sm_policy_update_context_data_local_var->qos_mon_reports = qos_mon_reports;
    sm_policy_update_context_data_local_var->user_location_info_time = user_location_info_time;
    sm_policy_update_context_data_local_var->rep_pra_infos = rep_pra_infos;
    sm_policy_update_context_data_local_var->ue_init_res_req = ue_init_res_req;
    sm_policy_update_context_data_local_var->is_ref_qos_indication = is_ref_qos_indication;
    sm_policy_update_context_data_local_var->ref_qos_indication = ref_qos_indication;
    sm_policy_update_context_data_local_var->qos_flow_usage = qos_flow_usage;
    sm_policy_update_context_data_local_var->credit_manage_status = credit_manage_status;
    sm_policy_update_context_data_local_var->serv_nf_id = serv_nf_id;
    sm_policy_update_context_data_local_var->is_trace_req_null = is_trace_req_null;
    sm_policy_update_context_data_local_var->trace_req = trace_req;
    sm_policy_update_context_data_local_var->ma_pdu_ind = ma_pdu_ind;
    sm_policy_update_context_data_local_var->atsss_capab = atsss_capab;
    sm_policy_update_context_data_local_var->tsn_bridge_info = tsn_bridge_info;
    sm_policy_update_context_data_local_var->tsn_bridge_man_cont = tsn_bridge_man_cont;
    sm_policy_update_context_data_local_var->tsn_port_man_cont_dstt = tsn_port_man_cont_dstt;
    sm_policy_update_context_data_local_var->tsn_port_man_cont_nwtts = tsn_port_man_cont_nwtts;
    sm_policy_update_context_data_local_var->mul_addr_infos = mul_addr_infos;
    sm_policy_update_context_data_local_var->policy_dec_failure_reports = policy_dec_failure_reports;
    sm_policy_update_context_data_local_var->invalid_policy_decs = invalid_policy_decs;
    sm_policy_update_context_data_local_var->traffic_descriptors = traffic_descriptors;
    sm_policy_update_context_data_local_var->pcc_rule_id = pcc_rule_id;
    sm_policy_update_context_data_local_var->types_of_notif = types_of_notif;
    sm_policy_update_context_data_local_var->inter_grp_ids = inter_grp_ids;
    sm_policy_update_context_data_local_var->sat_backhaul_category = sat_backhaul_category;
    sm_policy_update_context_data_local_var->is_pcf_ue_info_null = is_pcf_ue_info_null;
    sm_policy_update_context_data_local_var->pcf_ue_info = pcf_ue_info;
    sm_policy_update_context_data_local_var->is_nwdaf_datas_null = is_nwdaf_datas_null;
    sm_policy_update_context_data_local_var->nwdaf_datas = nwdaf_datas;
    sm_policy_update_context_data_local_var->is_an_gw_status = is_an_gw_status;
    sm_policy_update_context_data_local_var->an_gw_status = an_gw_status;

    return sm_policy_update_context_data_local_var;
}

void OpenAPI_sm_policy_update_context_data_free(OpenAPI_sm_policy_update_context_data_t *sm_policy_update_context_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_policy_update_context_data) {
        return;
    }
    if (sm_policy_update_context_data->rep_policy_ctrl_req_triggers) {
        OpenAPI_list_free(sm_policy_update_context_data->rep_policy_ctrl_req_triggers);
        sm_policy_update_context_data->rep_policy_ctrl_req_triggers = NULL;
    }
    if (sm_policy_update_context_data->acc_net_ch_ids) {
        OpenAPI_list_for_each(sm_policy_update_context_data->acc_net_ch_ids, node) {
            OpenAPI_acc_net_ch_id_free(node->data);
        }
        OpenAPI_list_free(sm_policy_update_context_data->acc_net_ch_ids);
        sm_policy_update_context_data->acc_net_ch_ids = NULL;
    }
    if (sm_policy_update_context_data->add_access_info) {
        OpenAPI_additional_access_info_free(sm_policy_update_context_data->add_access_info);
        sm_policy_update_context_data->add_access_info = NULL;
    }
    if (sm_policy_update_context_data->rel_access_info) {
        OpenAPI_additional_access_info_free(sm_policy_update_context_data->rel_access_info);
        sm_policy_update_context_data->rel_access_info = NULL;
    }
    if (sm_policy_update_context_data->serving_network) {
        OpenAPI_plmn_id_nid_free(sm_policy_update_context_data->serving_network);
        sm_policy_update_context_data->serving_network = NULL;
    }
    if (sm_policy_update_context_data->user_location_info) {
        OpenAPI_user_location_free(sm_policy_update_context_data->user_location_info);
        sm_policy_update_context_data->user_location_info = NULL;
    }
    if (sm_policy_update_context_data->ue_time_zone) {
        ogs_free(sm_policy_update_context_data->ue_time_zone);
        sm_policy_update_context_data->ue_time_zone = NULL;
    }
    if (sm_policy_update_context_data->rel_ipv4_address) {
        ogs_free(sm_policy_update_context_data->rel_ipv4_address);
        sm_policy_update_context_data->rel_ipv4_address = NULL;
    }
    if (sm_policy_update_context_data->ipv4_address) {
        ogs_free(sm_policy_update_context_data->ipv4_address);
        sm_policy_update_context_data->ipv4_address = NULL;
    }
    if (sm_policy_update_context_data->ip_domain) {
        ogs_free(sm_policy_update_context_data->ip_domain);
        sm_policy_update_context_data->ip_domain = NULL;
    }
    if (sm_policy_update_context_data->ipv6_address_prefix) {
        ogs_free(sm_policy_update_context_data->ipv6_address_prefix);
        sm_policy_update_context_data->ipv6_address_prefix = NULL;
    }
    if (sm_policy_update_context_data->rel_ipv6_address_prefix) {
        ogs_free(sm_policy_update_context_data->rel_ipv6_address_prefix);
        sm_policy_update_context_data->rel_ipv6_address_prefix = NULL;
    }
    if (sm_policy_update_context_data->add_ipv6_addr_prefixes) {
        ogs_free(sm_policy_update_context_data->add_ipv6_addr_prefixes);
        sm_policy_update_context_data->add_ipv6_addr_prefixes = NULL;
    }
    if (sm_policy_update_context_data->add_rel_ipv6_addr_prefixes) {
        ogs_free(sm_policy_update_context_data->add_rel_ipv6_addr_prefixes);
        sm_policy_update_context_data->add_rel_ipv6_addr_prefixes = NULL;
    }
    if (sm_policy_update_context_data->rel_ue_mac) {
        ogs_free(sm_policy_update_context_data->rel_ue_mac);
        sm_policy_update_context_data->rel_ue_mac = NULL;
    }
    if (sm_policy_update_context_data->ue_mac) {
        ogs_free(sm_policy_update_context_data->ue_mac);
        sm_policy_update_context_data->ue_mac = NULL;
    }
    if (sm_policy_update_context_data->subs_sess_ambr) {
        OpenAPI_ambr_free(sm_policy_update_context_data->subs_sess_ambr);
        sm_policy_update_context_data->subs_sess_ambr = NULL;
    }
    if (sm_policy_update_context_data->auth_prof_index) {
        ogs_free(sm_policy_update_context_data->auth_prof_index);
        sm_policy_update_context_data->auth_prof_index = NULL;
    }
    if (sm_policy_update_context_data->subs_def_qos) {
        OpenAPI_subscribed_default_qos_free(sm_policy_update_context_data->subs_def_qos);
        sm_policy_update_context_data->subs_def_qos = NULL;
    }
    if (sm_policy_update_context_data->vplmn_qos) {
        OpenAPI_vplmn_qos_free(sm_policy_update_context_data->vplmn_qos);
        sm_policy_update_context_data->vplmn_qos = NULL;
    }
    if (sm_policy_update_context_data->accu_usage_reports) {
        OpenAPI_list_for_each(sm_policy_update_context_data->accu_usage_reports, node) {
            OpenAPI_accu_usage_report_free(node->data);
        }
        OpenAPI_list_free(sm_policy_update_context_data->accu_usage_reports);
        sm_policy_update_context_data->accu_usage_reports = NULL;
    }
    if (sm_policy_update_context_data->app_detection_infos) {
        OpenAPI_list_for_each(sm_policy_update_context_data->app_detection_infos, node) {
            OpenAPI_app_detection_info_free(node->data);
        }
        OpenAPI_list_free(sm_policy_update_context_data->app_detection_infos);
        sm_policy_update_context_data->app_detection_infos = NULL;
    }
    if (sm_policy_update_context_data->rule_reports) {
        OpenAPI_list_for_each(sm_policy_update_context_data->rule_reports, node) {
            OpenAPI_rule_report_free(node->data);
        }
        OpenAPI_list_free(sm_policy_update_context_data->rule_reports);
        sm_policy_update_context_data->rule_reports = NULL;
    }
    if (sm_policy_update_context_data->sess_rule_reports) {
        OpenAPI_list_for_each(sm_policy_update_context_data->sess_rule_reports, node) {
            OpenAPI_session_rule_report_free(node->data);
        }
        OpenAPI_list_free(sm_policy_update_context_data->sess_rule_reports);
        sm_policy_update_context_data->sess_rule_reports = NULL;
    }
    if (sm_policy_update_context_data->qnc_reports) {
        OpenAPI_list_for_each(sm_policy_update_context_data->qnc_reports, node) {
            OpenAPI_qos_notification_control_info_free(node->data);
        }
        OpenAPI_list_free(sm_policy_update_context_data->qnc_reports);
        sm_policy_update_context_data->qnc_reports = NULL;
    }
    if (sm_policy_update_context_data->qos_mon_reports) {
        OpenAPI_list_for_each(sm_policy_update_context_data->qos_mon_reports, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(sm_policy_update_context_data->qos_mon_reports);
        sm_policy_update_context_data->qos_mon_reports = NULL;
    }
    if (sm_policy_update_context_data->user_location_info_time) {
        ogs_free(sm_policy_update_context_data->user_location_info_time);
        sm_policy_update_context_data->user_location_info_time = NULL;
    }
    if (sm_policy_update_context_data->rep_pra_infos) {
        OpenAPI_list_for_each(sm_policy_update_context_data->rep_pra_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_update_context_data->rep_pra_infos);
        sm_policy_update_context_data->rep_pra_infos = NULL;
    }
    if (sm_policy_update_context_data->ue_init_res_req) {
        OpenAPI_ue_initiated_resource_request_free(sm_policy_update_context_data->ue_init_res_req);
        sm_policy_update_context_data->ue_init_res_req = NULL;
    }
    if (sm_policy_update_context_data->serv_nf_id) {
        OpenAPI_serving_nf_identity_free(sm_policy_update_context_data->serv_nf_id);
        sm_policy_update_context_data->serv_nf_id = NULL;
    }
    if (sm_policy_update_context_data->trace_req) {
        OpenAPI_trace_data_free(sm_policy_update_context_data->trace_req);
        sm_policy_update_context_data->trace_req = NULL;
    }
    if (sm_policy_update_context_data->tsn_bridge_info) {
        OpenAPI_tsn_bridge_info_free(sm_policy_update_context_data->tsn_bridge_info);
        sm_policy_update_context_data->tsn_bridge_info = NULL;
    }
    if (sm_policy_update_context_data->tsn_bridge_man_cont) {
        OpenAPI_bridge_management_container_free(sm_policy_update_context_data->tsn_bridge_man_cont);
        sm_policy_update_context_data->tsn_bridge_man_cont = NULL;
    }
    if (sm_policy_update_context_data->tsn_port_man_cont_dstt) {
        OpenAPI_port_management_container_free(sm_policy_update_context_data->tsn_port_man_cont_dstt);
        sm_policy_update_context_data->tsn_port_man_cont_dstt = NULL;
    }
    if (sm_policy_update_context_data->tsn_port_man_cont_nwtts) {
        OpenAPI_list_for_each(sm_policy_update_context_data->tsn_port_man_cont_nwtts, node) {
            OpenAPI_port_management_container_free(node->data);
        }
        OpenAPI_list_free(sm_policy_update_context_data->tsn_port_man_cont_nwtts);
        sm_policy_update_context_data->tsn_port_man_cont_nwtts = NULL;
    }
    if (sm_policy_update_context_data->mul_addr_infos) {
        OpenAPI_list_for_each(sm_policy_update_context_data->mul_addr_infos, node) {
            OpenAPI_ip_multicast_address_info_free(node->data);
        }
        OpenAPI_list_free(sm_policy_update_context_data->mul_addr_infos);
        sm_policy_update_context_data->mul_addr_infos = NULL;
    }
    if (sm_policy_update_context_data->policy_dec_failure_reports) {
        OpenAPI_list_free(sm_policy_update_context_data->policy_dec_failure_reports);
        sm_policy_update_context_data->policy_dec_failure_reports = NULL;
    }
    if (sm_policy_update_context_data->invalid_policy_decs) {
        OpenAPI_list_for_each(sm_policy_update_context_data->invalid_policy_decs, node) {
            OpenAPI_invalid_param_free(node->data);
        }
        OpenAPI_list_free(sm_policy_update_context_data->invalid_policy_decs);
        sm_policy_update_context_data->invalid_policy_decs = NULL;
    }
    if (sm_policy_update_context_data->traffic_descriptors) {
        OpenAPI_list_for_each(sm_policy_update_context_data->traffic_descriptors, node) {
            OpenAPI_ddd_traffic_descriptor_free(node->data);
        }
        OpenAPI_list_free(sm_policy_update_context_data->traffic_descriptors);
        sm_policy_update_context_data->traffic_descriptors = NULL;
    }
    if (sm_policy_update_context_data->pcc_rule_id) {
        ogs_free(sm_policy_update_context_data->pcc_rule_id);
        sm_policy_update_context_data->pcc_rule_id = NULL;
    }
    if (sm_policy_update_context_data->types_of_notif) {
        OpenAPI_list_free(sm_policy_update_context_data->types_of_notif);
        sm_policy_update_context_data->types_of_notif = NULL;
    }
    if (sm_policy_update_context_data->inter_grp_ids) {
        OpenAPI_list_for_each(sm_policy_update_context_data->inter_grp_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_policy_update_context_data->inter_grp_ids);
        sm_policy_update_context_data->inter_grp_ids = NULL;
    }
    if (sm_policy_update_context_data->pcf_ue_info) {
        OpenAPI_pcf_ue_callback_info_free(sm_policy_update_context_data->pcf_ue_info);
        sm_policy_update_context_data->pcf_ue_info = NULL;
    }
    if (sm_policy_update_context_data->nwdaf_datas) {
        OpenAPI_list_for_each(sm_policy_update_context_data->nwdaf_datas, node) {
            OpenAPI_nwdaf_data_free(node->data);
        }
        OpenAPI_list_free(sm_policy_update_context_data->nwdaf_datas);
        sm_policy_update_context_data->nwdaf_datas = NULL;
    }
    ogs_free(sm_policy_update_context_data);
}

cJSON *OpenAPI_sm_policy_update_context_data_convertToJSON(OpenAPI_sm_policy_update_context_data_t *sm_policy_update_context_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_policy_update_context_data == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [SmPolicyUpdateContextData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_policy_update_context_data->rep_policy_ctrl_req_triggers != OpenAPI_policy_control_request_trigger_NULL) {
    cJSON *rep_policy_ctrl_req_triggersList = cJSON_AddArrayToObject(item, "repPolicyCtrlReqTriggers");
    if (rep_policy_ctrl_req_triggersList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rep_policy_ctrl_req_triggers]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->rep_policy_ctrl_req_triggers, node) {
        if (cJSON_AddStringToObject(rep_policy_ctrl_req_triggersList, "", OpenAPI_policy_control_request_trigger_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rep_policy_ctrl_req_triggers]");
            goto end;
        }
    }
    }

    if (sm_policy_update_context_data->acc_net_ch_ids) {
    cJSON *acc_net_ch_idsList = cJSON_AddArrayToObject(item, "accNetChIds");
    if (acc_net_ch_idsList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [acc_net_ch_ids]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->acc_net_ch_ids, node) {
        cJSON *itemLocal = OpenAPI_acc_net_ch_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [acc_net_ch_ids]");
            goto end;
        }
        cJSON_AddItemToArray(acc_net_ch_idsList, itemLocal);
    }
    }

    if (sm_policy_update_context_data->access_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(sm_policy_update_context_data->access_type)) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [access_type]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(sm_policy_update_context_data->rat_type)) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->add_access_info) {
    cJSON *add_access_info_local_JSON = OpenAPI_additional_access_info_convertToJSON(sm_policy_update_context_data->add_access_info);
    if (add_access_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [add_access_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "addAccessInfo", add_access_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [add_access_info]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->rel_access_info) {
    cJSON *rel_access_info_local_JSON = OpenAPI_additional_access_info_convertToJSON(sm_policy_update_context_data->rel_access_info);
    if (rel_access_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rel_access_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "relAccessInfo", rel_access_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rel_access_info]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->serving_network) {
    cJSON *serving_network_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(sm_policy_update_context_data->serving_network);
    if (serving_network_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetwork", serving_network_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->user_location_info) {
    cJSON *user_location_info_local_JSON = OpenAPI_user_location_convertToJSON(sm_policy_update_context_data->user_location_info);
    if (user_location_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [user_location_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationInfo", user_location_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [user_location_info]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", sm_policy_update_context_data->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->rel_ipv4_address) {
    if (cJSON_AddStringToObject(item, "relIpv4Address", sm_policy_update_context_data->rel_ipv4_address) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rel_ipv4_address]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->ipv4_address) {
    if (cJSON_AddStringToObject(item, "ipv4Address", sm_policy_update_context_data->ipv4_address) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [ipv4_address]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->ip_domain) {
    if (cJSON_AddStringToObject(item, "ipDomain", sm_policy_update_context_data->ip_domain) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [ip_domain]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->ipv6_address_prefix) {
    if (cJSON_AddStringToObject(item, "ipv6AddressPrefix", sm_policy_update_context_data->ipv6_address_prefix) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [ipv6_address_prefix]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->rel_ipv6_address_prefix) {
    if (cJSON_AddStringToObject(item, "relIpv6AddressPrefix", sm_policy_update_context_data->rel_ipv6_address_prefix) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rel_ipv6_address_prefix]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->add_ipv6_addr_prefixes) {
    if (cJSON_AddStringToObject(item, "addIpv6AddrPrefixes", sm_policy_update_context_data->add_ipv6_addr_prefixes) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [add_ipv6_addr_prefixes]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->add_rel_ipv6_addr_prefixes) {
    if (cJSON_AddStringToObject(item, "addRelIpv6AddrPrefixes", sm_policy_update_context_data->add_rel_ipv6_addr_prefixes) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [add_rel_ipv6_addr_prefixes]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->rel_ue_mac) {
    if (cJSON_AddStringToObject(item, "relUeMac", sm_policy_update_context_data->rel_ue_mac) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rel_ue_mac]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->ue_mac) {
    if (cJSON_AddStringToObject(item, "ueMac", sm_policy_update_context_data->ue_mac) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [ue_mac]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->subs_sess_ambr) {
    cJSON *subs_sess_ambr_local_JSON = OpenAPI_ambr_convertToJSON(sm_policy_update_context_data->subs_sess_ambr);
    if (subs_sess_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [subs_sess_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "subsSessAmbr", subs_sess_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [subs_sess_ambr]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->auth_prof_index) {
    if (cJSON_AddStringToObject(item, "authProfIndex", sm_policy_update_context_data->auth_prof_index) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [auth_prof_index]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->subs_def_qos) {
    cJSON *subs_def_qos_local_JSON = OpenAPI_subscribed_default_qos_convertToJSON(sm_policy_update_context_data->subs_def_qos);
    if (subs_def_qos_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [subs_def_qos]");
        goto end;
    }
    cJSON_AddItemToObject(item, "subsDefQos", subs_def_qos_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [subs_def_qos]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->vplmn_qos) {
    cJSON *vplmn_qos_local_JSON = OpenAPI_vplmn_qos_convertToJSON(sm_policy_update_context_data->vplmn_qos);
    if (vplmn_qos_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [vplmn_qos]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vplmnQos", vplmn_qos_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [vplmn_qos]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->is_vplmn_qos_not_app) {
    if (cJSON_AddBoolToObject(item, "vplmnQosNotApp", sm_policy_update_context_data->vplmn_qos_not_app) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [vplmn_qos_not_app]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->is_num_of_pack_filter) {
    if (cJSON_AddNumberToObject(item, "numOfPackFilter", sm_policy_update_context_data->num_of_pack_filter) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [num_of_pack_filter]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->accu_usage_reports) {
    cJSON *accu_usage_reportsList = cJSON_AddArrayToObject(item, "accuUsageReports");
    if (accu_usage_reportsList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [accu_usage_reports]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->accu_usage_reports, node) {
        cJSON *itemLocal = OpenAPI_accu_usage_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [accu_usage_reports]");
            goto end;
        }
        cJSON_AddItemToArray(accu_usage_reportsList, itemLocal);
    }
    }

    if (sm_policy_update_context_data->is__3gpp_ps_data_off_status) {
    if (cJSON_AddBoolToObject(item, "3gppPsDataOffStatus", sm_policy_update_context_data->_3gpp_ps_data_off_status) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [_3gpp_ps_data_off_status]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->app_detection_infos) {
    cJSON *app_detection_infosList = cJSON_AddArrayToObject(item, "appDetectionInfos");
    if (app_detection_infosList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [app_detection_infos]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->app_detection_infos, node) {
        cJSON *itemLocal = OpenAPI_app_detection_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [app_detection_infos]");
            goto end;
        }
        cJSON_AddItemToArray(app_detection_infosList, itemLocal);
    }
    }

    if (sm_policy_update_context_data->rule_reports) {
    cJSON *rule_reportsList = cJSON_AddArrayToObject(item, "ruleReports");
    if (rule_reportsList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rule_reports]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->rule_reports, node) {
        cJSON *itemLocal = OpenAPI_rule_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rule_reports]");
            goto end;
        }
        cJSON_AddItemToArray(rule_reportsList, itemLocal);
    }
    }

    if (sm_policy_update_context_data->sess_rule_reports) {
    cJSON *sess_rule_reportsList = cJSON_AddArrayToObject(item, "sessRuleReports");
    if (sess_rule_reportsList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [sess_rule_reports]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->sess_rule_reports, node) {
        cJSON *itemLocal = OpenAPI_session_rule_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [sess_rule_reports]");
            goto end;
        }
        cJSON_AddItemToArray(sess_rule_reportsList, itemLocal);
    }
    }

    if (sm_policy_update_context_data->qnc_reports) {
    cJSON *qnc_reportsList = cJSON_AddArrayToObject(item, "qncReports");
    if (qnc_reportsList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [qnc_reports]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->qnc_reports, node) {
        cJSON *itemLocal = OpenAPI_qos_notification_control_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [qnc_reports]");
            goto end;
        }
        cJSON_AddItemToArray(qnc_reportsList, itemLocal);
    }
    }

    if (sm_policy_update_context_data->qos_mon_reports) {
    cJSON *qos_mon_reportsList = cJSON_AddArrayToObject(item, "qosMonReports");
    if (qos_mon_reportsList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [qos_mon_reports]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->qos_mon_reports, node) {
        cJSON *itemLocal = OpenAPI_qos_monitoring_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [qos_mon_reports]");
            goto end;
        }
        cJSON_AddItemToArray(qos_mon_reportsList, itemLocal);
    }
    }

    if (sm_policy_update_context_data->user_location_info_time) {
    if (cJSON_AddStringToObject(item, "userLocationInfoTime", sm_policy_update_context_data->user_location_info_time) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [user_location_info_time]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->rep_pra_infos) {
    cJSON *rep_pra_infos = cJSON_AddObjectToObject(item, "repPraInfos");
    if (rep_pra_infos == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rep_pra_infos]");
        goto end;
    }
    cJSON *localMapObject = rep_pra_infos;
    if (sm_policy_update_context_data->rep_pra_infos) {
        OpenAPI_list_for_each(sm_policy_update_context_data->rep_pra_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rep_pra_infos]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rep_pra_infos]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (sm_policy_update_context_data->ue_init_res_req) {
    cJSON *ue_init_res_req_local_JSON = OpenAPI_ue_initiated_resource_request_convertToJSON(sm_policy_update_context_data->ue_init_res_req);
    if (ue_init_res_req_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [ue_init_res_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueInitResReq", ue_init_res_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [ue_init_res_req]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->is_ref_qos_indication) {
    if (cJSON_AddBoolToObject(item, "refQosIndication", sm_policy_update_context_data->ref_qos_indication) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [ref_qos_indication]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->qos_flow_usage != OpenAPI_qos_flow_usage_NULL) {
    if (cJSON_AddStringToObject(item, "qosFlowUsage", OpenAPI_qos_flow_usage_ToString(sm_policy_update_context_data->qos_flow_usage)) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [qos_flow_usage]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->credit_manage_status != OpenAPI_credit_management_status_NULL) {
    if (cJSON_AddStringToObject(item, "creditManageStatus", OpenAPI_credit_management_status_ToString(sm_policy_update_context_data->credit_manage_status)) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [credit_manage_status]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->serv_nf_id) {
    cJSON *serv_nf_id_local_JSON = OpenAPI_serving_nf_identity_convertToJSON(sm_policy_update_context_data->serv_nf_id);
    if (serv_nf_id_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [serv_nf_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servNfId", serv_nf_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [serv_nf_id]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->trace_req) {
    cJSON *trace_req_local_JSON = OpenAPI_trace_data_convertToJSON(sm_policy_update_context_data->trace_req);
    if (trace_req_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [trace_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceReq", trace_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [trace_req]");
        goto end;
    }
    } else if (sm_policy_update_context_data->is_trace_req_null) {
        if (cJSON_AddNullToObject(item, "traceReq") == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [trace_req]");
            goto end;
        }
    }

    if (sm_policy_update_context_data->ma_pdu_ind != OpenAPI_ma_pdu_indication_NULL) {
    if (cJSON_AddStringToObject(item, "maPduInd", OpenAPI_ma_pdu_indication_ToString(sm_policy_update_context_data->ma_pdu_ind)) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [ma_pdu_ind]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->atsss_capab != OpenAPI_npcf_atsss_capability_NULL) {
    if (cJSON_AddStringToObject(item, "atsssCapab", OpenAPI_npcf_atsss_capability_ToString(sm_policy_update_context_data->atsss_capab)) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [atsss_capab]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->tsn_bridge_info) {
    cJSON *tsn_bridge_info_local_JSON = OpenAPI_tsn_bridge_info_convertToJSON(sm_policy_update_context_data->tsn_bridge_info);
    if (tsn_bridge_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [tsn_bridge_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnBridgeInfo", tsn_bridge_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [tsn_bridge_info]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->tsn_bridge_man_cont) {
    cJSON *tsn_bridge_man_cont_local_JSON = OpenAPI_bridge_management_container_convertToJSON(sm_policy_update_context_data->tsn_bridge_man_cont);
    if (tsn_bridge_man_cont_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnBridgeManCont", tsn_bridge_man_cont_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->tsn_port_man_cont_dstt) {
    cJSON *tsn_port_man_cont_dstt_local_JSON = OpenAPI_port_management_container_convertToJSON(sm_policy_update_context_data->tsn_port_man_cont_dstt);
    if (tsn_port_man_cont_dstt_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnPortManContDstt", tsn_port_man_cont_dstt_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->tsn_port_man_cont_nwtts) {
    cJSON *tsn_port_man_cont_nwttsList = cJSON_AddArrayToObject(item, "tsnPortManContNwtts");
    if (tsn_port_man_cont_nwttsList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [tsn_port_man_cont_nwtts]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->tsn_port_man_cont_nwtts, node) {
        cJSON *itemLocal = OpenAPI_port_management_container_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }
        cJSON_AddItemToArray(tsn_port_man_cont_nwttsList, itemLocal);
    }
    }

    if (sm_policy_update_context_data->mul_addr_infos) {
    cJSON *mul_addr_infosList = cJSON_AddArrayToObject(item, "mulAddrInfos");
    if (mul_addr_infosList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [mul_addr_infos]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->mul_addr_infos, node) {
        cJSON *itemLocal = OpenAPI_ip_multicast_address_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [mul_addr_infos]");
            goto end;
        }
        cJSON_AddItemToArray(mul_addr_infosList, itemLocal);
    }
    }

    if (sm_policy_update_context_data->policy_dec_failure_reports != OpenAPI_policy_decision_failure_code_NULL) {
    cJSON *policy_dec_failure_reportsList = cJSON_AddArrayToObject(item, "policyDecFailureReports");
    if (policy_dec_failure_reportsList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [policy_dec_failure_reports]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->policy_dec_failure_reports, node) {
        if (cJSON_AddStringToObject(policy_dec_failure_reportsList, "", OpenAPI_policy_decision_failure_code_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [policy_dec_failure_reports]");
            goto end;
        }
    }
    }

    if (sm_policy_update_context_data->invalid_policy_decs) {
    cJSON *invalid_policy_decsList = cJSON_AddArrayToObject(item, "invalidPolicyDecs");
    if (invalid_policy_decsList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [invalid_policy_decs]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->invalid_policy_decs, node) {
        cJSON *itemLocal = OpenAPI_invalid_param_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [invalid_policy_decs]");
            goto end;
        }
        cJSON_AddItemToArray(invalid_policy_decsList, itemLocal);
    }
    }

    if (sm_policy_update_context_data->traffic_descriptors) {
    cJSON *traffic_descriptorsList = cJSON_AddArrayToObject(item, "trafficDescriptors");
    if (traffic_descriptorsList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [traffic_descriptors]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->traffic_descriptors, node) {
        cJSON *itemLocal = OpenAPI_ddd_traffic_descriptor_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [traffic_descriptors]");
            goto end;
        }
        cJSON_AddItemToArray(traffic_descriptorsList, itemLocal);
    }
    }

    if (sm_policy_update_context_data->pcc_rule_id) {
    if (cJSON_AddStringToObject(item, "pccRuleId", sm_policy_update_context_data->pcc_rule_id) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [pcc_rule_id]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->types_of_notif != OpenAPI_dl_data_delivery_status_NULL) {
    cJSON *types_of_notifList = cJSON_AddArrayToObject(item, "typesOfNotif");
    if (types_of_notifList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [types_of_notif]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->types_of_notif, node) {
        if (cJSON_AddStringToObject(types_of_notifList, "", OpenAPI_dl_data_delivery_status_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [types_of_notif]");
            goto end;
        }
    }
    }

    if (sm_policy_update_context_data->inter_grp_ids) {
    cJSON *inter_grp_idsList = cJSON_AddArrayToObject(item, "interGrpIds");
    if (inter_grp_idsList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [inter_grp_ids]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->inter_grp_ids, node) {
        if (cJSON_AddStringToObject(inter_grp_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [inter_grp_ids]");
            goto end;
        }
    }
    }

    if (sm_policy_update_context_data->sat_backhaul_category != OpenAPI_satellite_backhaul_category_NULL) {
    if (cJSON_AddStringToObject(item, "satBackhaulCategory", OpenAPI_satellite_backhaul_category_ToString(sm_policy_update_context_data->sat_backhaul_category)) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [sat_backhaul_category]");
        goto end;
    }
    }

    if (sm_policy_update_context_data->pcf_ue_info) {
    cJSON *pcf_ue_info_local_JSON = OpenAPI_pcf_ue_callback_info_convertToJSON(sm_policy_update_context_data->pcf_ue_info);
    if (pcf_ue_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [pcf_ue_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pcfUeInfo", pcf_ue_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [pcf_ue_info]");
        goto end;
    }
    } else if (sm_policy_update_context_data->is_pcf_ue_info_null) {
        if (cJSON_AddNullToObject(item, "pcfUeInfo") == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [pcf_ue_info]");
            goto end;
        }
    }

    if (sm_policy_update_context_data->nwdaf_datas) {
    cJSON *nwdaf_datasList = cJSON_AddArrayToObject(item, "nwdafDatas");
    if (nwdaf_datasList == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [nwdaf_datas]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_update_context_data->nwdaf_datas, node) {
        cJSON *itemLocal = OpenAPI_nwdaf_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [nwdaf_datas]");
            goto end;
        }
        cJSON_AddItemToArray(nwdaf_datasList, itemLocal);
    }
    } else if (sm_policy_update_context_data->is_nwdaf_datas_null) {
        if (cJSON_AddNullToObject(item, "nwdafDatas") == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [nwdaf_datas]");
            goto end;
        }
    }

    if (sm_policy_update_context_data->is_an_gw_status) {
    if (cJSON_AddBoolToObject(item, "anGwStatus", sm_policy_update_context_data->an_gw_status) == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [an_gw_status]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_policy_update_context_data_t *OpenAPI_sm_policy_update_context_data_parseFromJSON(cJSON *sm_policy_update_context_dataJSON)
{
    OpenAPI_sm_policy_update_context_data_t *sm_policy_update_context_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rep_policy_ctrl_req_triggers = NULL;
    OpenAPI_list_t *rep_policy_ctrl_req_triggersList = NULL;
    cJSON *acc_net_ch_ids = NULL;
    OpenAPI_list_t *acc_net_ch_idsList = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    cJSON *rat_type = NULL;
    OpenAPI_rat_type_e rat_typeVariable = 0;
    cJSON *add_access_info = NULL;
    OpenAPI_additional_access_info_t *add_access_info_local_nonprim = NULL;
    cJSON *rel_access_info = NULL;
    OpenAPI_additional_access_info_t *rel_access_info_local_nonprim = NULL;
    cJSON *serving_network = NULL;
    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    cJSON *user_location_info = NULL;
    OpenAPI_user_location_t *user_location_info_local_nonprim = NULL;
    cJSON *ue_time_zone = NULL;
    cJSON *rel_ipv4_address = NULL;
    cJSON *ipv4_address = NULL;
    cJSON *ip_domain = NULL;
    cJSON *ipv6_address_prefix = NULL;
    cJSON *rel_ipv6_address_prefix = NULL;
    cJSON *add_ipv6_addr_prefixes = NULL;
    cJSON *add_rel_ipv6_addr_prefixes = NULL;
    cJSON *rel_ue_mac = NULL;
    cJSON *ue_mac = NULL;
    cJSON *subs_sess_ambr = NULL;
    OpenAPI_ambr_t *subs_sess_ambr_local_nonprim = NULL;
    cJSON *auth_prof_index = NULL;
    cJSON *subs_def_qos = NULL;
    OpenAPI_subscribed_default_qos_t *subs_def_qos_local_nonprim = NULL;
    cJSON *vplmn_qos = NULL;
    OpenAPI_vplmn_qos_t *vplmn_qos_local_nonprim = NULL;
    cJSON *vplmn_qos_not_app = NULL;
    cJSON *num_of_pack_filter = NULL;
    cJSON *accu_usage_reports = NULL;
    OpenAPI_list_t *accu_usage_reportsList = NULL;
    cJSON *_3gpp_ps_data_off_status = NULL;
    cJSON *app_detection_infos = NULL;
    OpenAPI_list_t *app_detection_infosList = NULL;
    cJSON *rule_reports = NULL;
    OpenAPI_list_t *rule_reportsList = NULL;
    cJSON *sess_rule_reports = NULL;
    OpenAPI_list_t *sess_rule_reportsList = NULL;
    cJSON *qnc_reports = NULL;
    OpenAPI_list_t *qnc_reportsList = NULL;
    cJSON *qos_mon_reports = NULL;
    OpenAPI_list_t *qos_mon_reportsList = NULL;
    cJSON *user_location_info_time = NULL;
    cJSON *rep_pra_infos = NULL;
    OpenAPI_list_t *rep_pra_infosList = NULL;
    cJSON *ue_init_res_req = NULL;
    OpenAPI_ue_initiated_resource_request_t *ue_init_res_req_local_nonprim = NULL;
    cJSON *ref_qos_indication = NULL;
    cJSON *qos_flow_usage = NULL;
    OpenAPI_qos_flow_usage_e qos_flow_usageVariable = 0;
    cJSON *credit_manage_status = NULL;
    OpenAPI_credit_management_status_e credit_manage_statusVariable = 0;
    cJSON *serv_nf_id = NULL;
    OpenAPI_serving_nf_identity_t *serv_nf_id_local_nonprim = NULL;
    cJSON *trace_req = NULL;
    OpenAPI_trace_data_t *trace_req_local_nonprim = NULL;
    cJSON *ma_pdu_ind = NULL;
    OpenAPI_ma_pdu_indication_e ma_pdu_indVariable = 0;
    cJSON *atsss_capab = NULL;
    OpenAPI_npcf_atsss_capability_e atsss_capabVariable = 0;
    cJSON *tsn_bridge_info = NULL;
    OpenAPI_tsn_bridge_info_t *tsn_bridge_info_local_nonprim = NULL;
    cJSON *tsn_bridge_man_cont = NULL;
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont_local_nonprim = NULL;
    cJSON *tsn_port_man_cont_dstt = NULL;
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt_local_nonprim = NULL;
    cJSON *tsn_port_man_cont_nwtts = NULL;
    OpenAPI_list_t *tsn_port_man_cont_nwttsList = NULL;
    cJSON *mul_addr_infos = NULL;
    OpenAPI_list_t *mul_addr_infosList = NULL;
    cJSON *policy_dec_failure_reports = NULL;
    OpenAPI_list_t *policy_dec_failure_reportsList = NULL;
    cJSON *invalid_policy_decs = NULL;
    OpenAPI_list_t *invalid_policy_decsList = NULL;
    cJSON *traffic_descriptors = NULL;
    OpenAPI_list_t *traffic_descriptorsList = NULL;
    cJSON *pcc_rule_id = NULL;
    cJSON *types_of_notif = NULL;
    OpenAPI_list_t *types_of_notifList = NULL;
    cJSON *inter_grp_ids = NULL;
    OpenAPI_list_t *inter_grp_idsList = NULL;
    cJSON *sat_backhaul_category = NULL;
    OpenAPI_satellite_backhaul_category_e sat_backhaul_categoryVariable = 0;
    cJSON *pcf_ue_info = NULL;
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_info_local_nonprim = NULL;
    cJSON *nwdaf_datas = NULL;
    OpenAPI_list_t *nwdaf_datasList = NULL;
    cJSON *an_gw_status = NULL;
    rep_policy_ctrl_req_triggers = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "repPolicyCtrlReqTriggers");
    if (rep_policy_ctrl_req_triggers) {
        cJSON *rep_policy_ctrl_req_triggers_local = NULL;
        if (!cJSON_IsArray(rep_policy_ctrl_req_triggers)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rep_policy_ctrl_req_triggers]");
            goto end;
        }

        rep_policy_ctrl_req_triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(rep_policy_ctrl_req_triggers_local, rep_policy_ctrl_req_triggers) {
            OpenAPI_policy_control_request_trigger_e localEnum = OpenAPI_policy_control_request_trigger_NULL;
            if (!cJSON_IsString(rep_policy_ctrl_req_triggers_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rep_policy_ctrl_req_triggers]");
                goto end;
            }
            localEnum = OpenAPI_policy_control_request_trigger_FromString(rep_policy_ctrl_req_triggers_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"rep_policy_ctrl_req_triggers\" is not supported. Ignoring it ...",
                         rep_policy_ctrl_req_triggers_local->valuestring);
            } else {
                OpenAPI_list_add(rep_policy_ctrl_req_triggersList, (void *)localEnum);
            }
        }
        if (rep_policy_ctrl_req_triggersList->count == 0) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed: Expected rep_policy_ctrl_req_triggersList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    acc_net_ch_ids = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "accNetChIds");
    if (acc_net_ch_ids) {
        cJSON *acc_net_ch_ids_local = NULL;
        if (!cJSON_IsArray(acc_net_ch_ids)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [acc_net_ch_ids]");
            goto end;
        }

        acc_net_ch_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(acc_net_ch_ids_local, acc_net_ch_ids) {
            if (!cJSON_IsObject(acc_net_ch_ids_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [acc_net_ch_ids]");
                goto end;
            }
            OpenAPI_acc_net_ch_id_t *acc_net_ch_idsItem = OpenAPI_acc_net_ch_id_parseFromJSON(acc_net_ch_ids_local);
            if (!acc_net_ch_idsItem) {
                ogs_error("No acc_net_ch_idsItem");
                goto end;
            }
            OpenAPI_list_add(acc_net_ch_idsList, acc_net_ch_idsItem);
        }
    }

    access_type = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "accessType");
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    rat_type = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ratType");
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    add_access_info = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "addAccessInfo");
    if (add_access_info) {
    add_access_info_local_nonprim = OpenAPI_additional_access_info_parseFromJSON(add_access_info);
    if (!add_access_info_local_nonprim) {
        ogs_error("OpenAPI_additional_access_info_parseFromJSON failed [add_access_info]");
        goto end;
    }
    }

    rel_access_info = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "relAccessInfo");
    if (rel_access_info) {
    rel_access_info_local_nonprim = OpenAPI_additional_access_info_parseFromJSON(rel_access_info);
    if (!rel_access_info_local_nonprim) {
        ogs_error("OpenAPI_additional_access_info_parseFromJSON failed [rel_access_info]");
        goto end;
    }
    }

    serving_network = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "servingNetwork");
    if (serving_network) {
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    if (!serving_network_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [serving_network]");
        goto end;
    }
    }

    user_location_info = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "userLocationInfo");
    if (user_location_info) {
    user_location_info_local_nonprim = OpenAPI_user_location_parseFromJSON(user_location_info);
    if (!user_location_info_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [user_location_info]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    rel_ipv4_address = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "relIpv4Address");
    if (rel_ipv4_address) {
    if (!cJSON_IsString(rel_ipv4_address) && !cJSON_IsNull(rel_ipv4_address)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rel_ipv4_address]");
        goto end;
    }
    }

    ipv4_address = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ipv4Address");
    if (ipv4_address) {
    if (!cJSON_IsString(ipv4_address) && !cJSON_IsNull(ipv4_address)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [ipv4_address]");
        goto end;
    }
    }

    ip_domain = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ipDomain");
    if (ip_domain) {
    if (!cJSON_IsString(ip_domain) && !cJSON_IsNull(ip_domain)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [ip_domain]");
        goto end;
    }
    }

    ipv6_address_prefix = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ipv6AddressPrefix");
    if (ipv6_address_prefix) {
    if (!cJSON_IsString(ipv6_address_prefix) && !cJSON_IsNull(ipv6_address_prefix)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [ipv6_address_prefix]");
        goto end;
    }
    }

    rel_ipv6_address_prefix = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "relIpv6AddressPrefix");
    if (rel_ipv6_address_prefix) {
    if (!cJSON_IsString(rel_ipv6_address_prefix) && !cJSON_IsNull(rel_ipv6_address_prefix)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rel_ipv6_address_prefix]");
        goto end;
    }
    }

    add_ipv6_addr_prefixes = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "addIpv6AddrPrefixes");
    if (add_ipv6_addr_prefixes) {
    if (!cJSON_IsString(add_ipv6_addr_prefixes) && !cJSON_IsNull(add_ipv6_addr_prefixes)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [add_ipv6_addr_prefixes]");
        goto end;
    }
    }

    add_rel_ipv6_addr_prefixes = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "addRelIpv6AddrPrefixes");
    if (add_rel_ipv6_addr_prefixes) {
    if (!cJSON_IsString(add_rel_ipv6_addr_prefixes) && !cJSON_IsNull(add_rel_ipv6_addr_prefixes)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [add_rel_ipv6_addr_prefixes]");
        goto end;
    }
    }

    rel_ue_mac = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "relUeMac");
    if (rel_ue_mac) {
    if (!cJSON_IsString(rel_ue_mac) && !cJSON_IsNull(rel_ue_mac)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rel_ue_mac]");
        goto end;
    }
    }

    ue_mac = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ueMac");
    if (ue_mac) {
    if (!cJSON_IsString(ue_mac) && !cJSON_IsNull(ue_mac)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [ue_mac]");
        goto end;
    }
    }

    subs_sess_ambr = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "subsSessAmbr");
    if (subs_sess_ambr) {
    subs_sess_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(subs_sess_ambr);
    if (!subs_sess_ambr_local_nonprim) {
        ogs_error("OpenAPI_ambr_parseFromJSON failed [subs_sess_ambr]");
        goto end;
    }
    }

    auth_prof_index = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "authProfIndex");
    if (auth_prof_index) {
    if (!cJSON_IsString(auth_prof_index) && !cJSON_IsNull(auth_prof_index)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [auth_prof_index]");
        goto end;
    }
    }

    subs_def_qos = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "subsDefQos");
    if (subs_def_qos) {
    subs_def_qos_local_nonprim = OpenAPI_subscribed_default_qos_parseFromJSON(subs_def_qos);
    if (!subs_def_qos_local_nonprim) {
        ogs_error("OpenAPI_subscribed_default_qos_parseFromJSON failed [subs_def_qos]");
        goto end;
    }
    }

    vplmn_qos = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "vplmnQos");
    if (vplmn_qos) {
    vplmn_qos_local_nonprim = OpenAPI_vplmn_qos_parseFromJSON(vplmn_qos);
    if (!vplmn_qos_local_nonprim) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON failed [vplmn_qos]");
        goto end;
    }
    }

    vplmn_qos_not_app = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "vplmnQosNotApp");
    if (vplmn_qos_not_app) {
    if (!cJSON_IsBool(vplmn_qos_not_app)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [vplmn_qos_not_app]");
        goto end;
    }
    }

    num_of_pack_filter = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "numOfPackFilter");
    if (num_of_pack_filter) {
    if (!cJSON_IsNumber(num_of_pack_filter)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [num_of_pack_filter]");
        goto end;
    }
    }

    accu_usage_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "accuUsageReports");
    if (accu_usage_reports) {
        cJSON *accu_usage_reports_local = NULL;
        if (!cJSON_IsArray(accu_usage_reports)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [accu_usage_reports]");
            goto end;
        }

        accu_usage_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(accu_usage_reports_local, accu_usage_reports) {
            if (!cJSON_IsObject(accu_usage_reports_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [accu_usage_reports]");
                goto end;
            }
            OpenAPI_accu_usage_report_t *accu_usage_reportsItem = OpenAPI_accu_usage_report_parseFromJSON(accu_usage_reports_local);
            if (!accu_usage_reportsItem) {
                ogs_error("No accu_usage_reportsItem");
                goto end;
            }
            OpenAPI_list_add(accu_usage_reportsList, accu_usage_reportsItem);
        }
    }

    _3gpp_ps_data_off_status = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "3gppPsDataOffStatus");
    if (_3gpp_ps_data_off_status) {
    if (!cJSON_IsBool(_3gpp_ps_data_off_status)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [_3gpp_ps_data_off_status]");
        goto end;
    }
    }

    app_detection_infos = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "appDetectionInfos");
    if (app_detection_infos) {
        cJSON *app_detection_infos_local = NULL;
        if (!cJSON_IsArray(app_detection_infos)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [app_detection_infos]");
            goto end;
        }

        app_detection_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_detection_infos_local, app_detection_infos) {
            if (!cJSON_IsObject(app_detection_infos_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [app_detection_infos]");
                goto end;
            }
            OpenAPI_app_detection_info_t *app_detection_infosItem = OpenAPI_app_detection_info_parseFromJSON(app_detection_infos_local);
            if (!app_detection_infosItem) {
                ogs_error("No app_detection_infosItem");
                goto end;
            }
            OpenAPI_list_add(app_detection_infosList, app_detection_infosItem);
        }
    }

    rule_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ruleReports");
    if (rule_reports) {
        cJSON *rule_reports_local = NULL;
        if (!cJSON_IsArray(rule_reports)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rule_reports]");
            goto end;
        }

        rule_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(rule_reports_local, rule_reports) {
            if (!cJSON_IsObject(rule_reports_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rule_reports]");
                goto end;
            }
            OpenAPI_rule_report_t *rule_reportsItem = OpenAPI_rule_report_parseFromJSON(rule_reports_local);
            if (!rule_reportsItem) {
                ogs_error("No rule_reportsItem");
                goto end;
            }
            OpenAPI_list_add(rule_reportsList, rule_reportsItem);
        }
    }

    sess_rule_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "sessRuleReports");
    if (sess_rule_reports) {
        cJSON *sess_rule_reports_local = NULL;
        if (!cJSON_IsArray(sess_rule_reports)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [sess_rule_reports]");
            goto end;
        }

        sess_rule_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(sess_rule_reports_local, sess_rule_reports) {
            if (!cJSON_IsObject(sess_rule_reports_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [sess_rule_reports]");
                goto end;
            }
            OpenAPI_session_rule_report_t *sess_rule_reportsItem = OpenAPI_session_rule_report_parseFromJSON(sess_rule_reports_local);
            if (!sess_rule_reportsItem) {
                ogs_error("No sess_rule_reportsItem");
                goto end;
            }
            OpenAPI_list_add(sess_rule_reportsList, sess_rule_reportsItem);
        }
    }

    qnc_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "qncReports");
    if (qnc_reports) {
        cJSON *qnc_reports_local = NULL;
        if (!cJSON_IsArray(qnc_reports)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [qnc_reports]");
            goto end;
        }

        qnc_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qnc_reports_local, qnc_reports) {
            if (!cJSON_IsObject(qnc_reports_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [qnc_reports]");
                goto end;
            }
            OpenAPI_qos_notification_control_info_t *qnc_reportsItem = OpenAPI_qos_notification_control_info_parseFromJSON(qnc_reports_local);
            if (!qnc_reportsItem) {
                ogs_error("No qnc_reportsItem");
                goto end;
            }
            OpenAPI_list_add(qnc_reportsList, qnc_reportsItem);
        }
    }

    qos_mon_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "qosMonReports");
    if (qos_mon_reports) {
        cJSON *qos_mon_reports_local = NULL;
        if (!cJSON_IsArray(qos_mon_reports)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [qos_mon_reports]");
            goto end;
        }

        qos_mon_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_mon_reports_local, qos_mon_reports) {
            if (!cJSON_IsObject(qos_mon_reports_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [qos_mon_reports]");
                goto end;
            }
            OpenAPI_qos_monitoring_report_t *qos_mon_reportsItem = OpenAPI_qos_monitoring_report_parseFromJSON(qos_mon_reports_local);
            if (!qos_mon_reportsItem) {
                ogs_error("No qos_mon_reportsItem");
                goto end;
            }
            OpenAPI_list_add(qos_mon_reportsList, qos_mon_reportsItem);
        }
    }

    user_location_info_time = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "userLocationInfoTime");
    if (user_location_info_time) {
    if (!cJSON_IsString(user_location_info_time) && !cJSON_IsNull(user_location_info_time)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [user_location_info_time]");
        goto end;
    }
    }

    rep_pra_infos = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "repPraInfos");
    if (rep_pra_infos) {
        cJSON *rep_pra_infos_local_map = NULL;
        if (!cJSON_IsObject(rep_pra_infos) && !cJSON_IsNull(rep_pra_infos)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rep_pra_infos]");
            goto end;
        }
        if (cJSON_IsObject(rep_pra_infos)) {
            rep_pra_infosList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(rep_pra_infos_local_map, rep_pra_infos) {
                cJSON *localMapObject = rep_pra_infos_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(rep_pra_infosList, localMapKeyPair);
            }
        }
    }

    ue_init_res_req = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ueInitResReq");
    if (ue_init_res_req) {
    ue_init_res_req_local_nonprim = OpenAPI_ue_initiated_resource_request_parseFromJSON(ue_init_res_req);
    if (!ue_init_res_req_local_nonprim) {
        ogs_error("OpenAPI_ue_initiated_resource_request_parseFromJSON failed [ue_init_res_req]");
        goto end;
    }
    }

    ref_qos_indication = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "refQosIndication");
    if (ref_qos_indication) {
    if (!cJSON_IsBool(ref_qos_indication)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [ref_qos_indication]");
        goto end;
    }
    }

    qos_flow_usage = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "qosFlowUsage");
    if (qos_flow_usage) {
    if (!cJSON_IsString(qos_flow_usage)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [qos_flow_usage]");
        goto end;
    }
    qos_flow_usageVariable = OpenAPI_qos_flow_usage_FromString(qos_flow_usage->valuestring);
    }

    credit_manage_status = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "creditManageStatus");
    if (credit_manage_status) {
    if (!cJSON_IsString(credit_manage_status)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [credit_manage_status]");
        goto end;
    }
    credit_manage_statusVariable = OpenAPI_credit_management_status_FromString(credit_manage_status->valuestring);
    }

    serv_nf_id = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "servNfId");
    if (serv_nf_id) {
    serv_nf_id_local_nonprim = OpenAPI_serving_nf_identity_parseFromJSON(serv_nf_id);
    if (!serv_nf_id_local_nonprim) {
        ogs_error("OpenAPI_serving_nf_identity_parseFromJSON failed [serv_nf_id]");
        goto end;
    }
    }

    trace_req = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "traceReq");
    if (trace_req) {
    if (!cJSON_IsNull(trace_req)) {
    trace_req_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_req);
    if (!trace_req_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_req]");
        goto end;
    }
    }
    }

    ma_pdu_ind = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "maPduInd");
    if (ma_pdu_ind) {
    if (!cJSON_IsString(ma_pdu_ind)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [ma_pdu_ind]");
        goto end;
    }
    ma_pdu_indVariable = OpenAPI_ma_pdu_indication_FromString(ma_pdu_ind->valuestring);
    }

    atsss_capab = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "atsssCapab");
    if (atsss_capab) {
    if (!cJSON_IsString(atsss_capab)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [atsss_capab]");
        goto end;
    }
    atsss_capabVariable = OpenAPI_npcf_atsss_capability_FromString(atsss_capab->valuestring);
    }

    tsn_bridge_info = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "tsnBridgeInfo");
    if (tsn_bridge_info) {
    tsn_bridge_info_local_nonprim = OpenAPI_tsn_bridge_info_parseFromJSON(tsn_bridge_info);
    if (!tsn_bridge_info_local_nonprim) {
        ogs_error("OpenAPI_tsn_bridge_info_parseFromJSON failed [tsn_bridge_info]");
        goto end;
    }
    }

    tsn_bridge_man_cont = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "tsnBridgeManCont");
    if (tsn_bridge_man_cont) {
    tsn_bridge_man_cont_local_nonprim = OpenAPI_bridge_management_container_parseFromJSON(tsn_bridge_man_cont);
    if (!tsn_bridge_man_cont_local_nonprim) {
        ogs_error("OpenAPI_bridge_management_container_parseFromJSON failed [tsn_bridge_man_cont]");
        goto end;
    }
    }

    tsn_port_man_cont_dstt = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "tsnPortManContDstt");
    if (tsn_port_man_cont_dstt) {
    tsn_port_man_cont_dstt_local_nonprim = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_dstt);
    if (!tsn_port_man_cont_dstt_local_nonprim) {
        ogs_error("OpenAPI_port_management_container_parseFromJSON failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    }

    tsn_port_man_cont_nwtts = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "tsnPortManContNwtts");
    if (tsn_port_man_cont_nwtts) {
        cJSON *tsn_port_man_cont_nwtts_local = NULL;
        if (!cJSON_IsArray(tsn_port_man_cont_nwtts)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }

        tsn_port_man_cont_nwttsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tsn_port_man_cont_nwtts_local, tsn_port_man_cont_nwtts) {
            if (!cJSON_IsObject(tsn_port_man_cont_nwtts_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
                goto end;
            }
            OpenAPI_port_management_container_t *tsn_port_man_cont_nwttsItem = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_nwtts_local);
            if (!tsn_port_man_cont_nwttsItem) {
                ogs_error("No tsn_port_man_cont_nwttsItem");
                goto end;
            }
            OpenAPI_list_add(tsn_port_man_cont_nwttsList, tsn_port_man_cont_nwttsItem);
        }
    }

    mul_addr_infos = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "mulAddrInfos");
    if (mul_addr_infos) {
        cJSON *mul_addr_infos_local = NULL;
        if (!cJSON_IsArray(mul_addr_infos)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [mul_addr_infos]");
            goto end;
        }

        mul_addr_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(mul_addr_infos_local, mul_addr_infos) {
            if (!cJSON_IsObject(mul_addr_infos_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [mul_addr_infos]");
                goto end;
            }
            OpenAPI_ip_multicast_address_info_t *mul_addr_infosItem = OpenAPI_ip_multicast_address_info_parseFromJSON(mul_addr_infos_local);
            if (!mul_addr_infosItem) {
                ogs_error("No mul_addr_infosItem");
                goto end;
            }
            OpenAPI_list_add(mul_addr_infosList, mul_addr_infosItem);
        }
    }

    policy_dec_failure_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "policyDecFailureReports");
    if (policy_dec_failure_reports) {
        cJSON *policy_dec_failure_reports_local = NULL;
        if (!cJSON_IsArray(policy_dec_failure_reports)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [policy_dec_failure_reports]");
            goto end;
        }

        policy_dec_failure_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(policy_dec_failure_reports_local, policy_dec_failure_reports) {
            OpenAPI_policy_decision_failure_code_e localEnum = OpenAPI_policy_decision_failure_code_NULL;
            if (!cJSON_IsString(policy_dec_failure_reports_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [policy_dec_failure_reports]");
                goto end;
            }
            localEnum = OpenAPI_policy_decision_failure_code_FromString(policy_dec_failure_reports_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"policy_dec_failure_reports\" is not supported. Ignoring it ...",
                         policy_dec_failure_reports_local->valuestring);
            } else {
                OpenAPI_list_add(policy_dec_failure_reportsList, (void *)localEnum);
            }
        }
        if (policy_dec_failure_reportsList->count == 0) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed: Expected policy_dec_failure_reportsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    invalid_policy_decs = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "invalidPolicyDecs");
    if (invalid_policy_decs) {
        cJSON *invalid_policy_decs_local = NULL;
        if (!cJSON_IsArray(invalid_policy_decs)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [invalid_policy_decs]");
            goto end;
        }

        invalid_policy_decsList = OpenAPI_list_create();

        cJSON_ArrayForEach(invalid_policy_decs_local, invalid_policy_decs) {
            if (!cJSON_IsObject(invalid_policy_decs_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [invalid_policy_decs]");
                goto end;
            }
            OpenAPI_invalid_param_t *invalid_policy_decsItem = OpenAPI_invalid_param_parseFromJSON(invalid_policy_decs_local);
            if (!invalid_policy_decsItem) {
                ogs_error("No invalid_policy_decsItem");
                goto end;
            }
            OpenAPI_list_add(invalid_policy_decsList, invalid_policy_decsItem);
        }
    }

    traffic_descriptors = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "trafficDescriptors");
    if (traffic_descriptors) {
        cJSON *traffic_descriptors_local = NULL;
        if (!cJSON_IsArray(traffic_descriptors)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [traffic_descriptors]");
            goto end;
        }

        traffic_descriptorsList = OpenAPI_list_create();

        cJSON_ArrayForEach(traffic_descriptors_local, traffic_descriptors) {
            if (!cJSON_IsObject(traffic_descriptors_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [traffic_descriptors]");
                goto end;
            }
            OpenAPI_ddd_traffic_descriptor_t *traffic_descriptorsItem = OpenAPI_ddd_traffic_descriptor_parseFromJSON(traffic_descriptors_local);
            if (!traffic_descriptorsItem) {
                ogs_error("No traffic_descriptorsItem");
                goto end;
            }
            OpenAPI_list_add(traffic_descriptorsList, traffic_descriptorsItem);
        }
    }

    pcc_rule_id = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "pccRuleId");
    if (pcc_rule_id) {
    if (!cJSON_IsString(pcc_rule_id) && !cJSON_IsNull(pcc_rule_id)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [pcc_rule_id]");
        goto end;
    }
    }

    types_of_notif = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "typesOfNotif");
    if (types_of_notif) {
        cJSON *types_of_notif_local = NULL;
        if (!cJSON_IsArray(types_of_notif)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [types_of_notif]");
            goto end;
        }

        types_of_notifList = OpenAPI_list_create();

        cJSON_ArrayForEach(types_of_notif_local, types_of_notif) {
            OpenAPI_dl_data_delivery_status_e localEnum = OpenAPI_dl_data_delivery_status_NULL;
            if (!cJSON_IsString(types_of_notif_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [types_of_notif]");
                goto end;
            }
            localEnum = OpenAPI_dl_data_delivery_status_FromString(types_of_notif_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"types_of_notif\" is not supported. Ignoring it ...",
                         types_of_notif_local->valuestring);
            } else {
                OpenAPI_list_add(types_of_notifList, (void *)localEnum);
            }
        }
        if (types_of_notifList->count == 0) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed: Expected types_of_notifList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    inter_grp_ids = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "interGrpIds");
    if (inter_grp_ids) {
        cJSON *inter_grp_ids_local = NULL;
        if (!cJSON_IsArray(inter_grp_ids)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [inter_grp_ids]");
            goto end;
        }

        inter_grp_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(inter_grp_ids_local, inter_grp_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(inter_grp_ids_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [inter_grp_ids]");
                goto end;
            }
            OpenAPI_list_add(inter_grp_idsList, ogs_strdup(inter_grp_ids_local->valuestring));
        }
    }

    sat_backhaul_category = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "satBackhaulCategory");
    if (sat_backhaul_category) {
    if (!cJSON_IsString(sat_backhaul_category)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [sat_backhaul_category]");
        goto end;
    }
    sat_backhaul_categoryVariable = OpenAPI_satellite_backhaul_category_FromString(sat_backhaul_category->valuestring);
    }

    pcf_ue_info = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "pcfUeInfo");
    if (pcf_ue_info) {
    if (!cJSON_IsNull(pcf_ue_info)) {
    pcf_ue_info_local_nonprim = OpenAPI_pcf_ue_callback_info_parseFromJSON(pcf_ue_info);
    if (!pcf_ue_info_local_nonprim) {
        ogs_error("OpenAPI_pcf_ue_callback_info_parseFromJSON failed [pcf_ue_info]");
        goto end;
    }
    }
    }

    nwdaf_datas = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "nwdafDatas");
    if (nwdaf_datas) {
    if (!cJSON_IsNull(nwdaf_datas)) {
        cJSON *nwdaf_datas_local = NULL;
        if (!cJSON_IsArray(nwdaf_datas)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [nwdaf_datas]");
            goto end;
        }

        nwdaf_datasList = OpenAPI_list_create();

        cJSON_ArrayForEach(nwdaf_datas_local, nwdaf_datas) {
            if (!cJSON_IsObject(nwdaf_datas_local)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [nwdaf_datas]");
                goto end;
            }
            OpenAPI_nwdaf_data_t *nwdaf_datasItem = OpenAPI_nwdaf_data_parseFromJSON(nwdaf_datas_local);
            if (!nwdaf_datasItem) {
                ogs_error("No nwdaf_datasItem");
                goto end;
            }
            OpenAPI_list_add(nwdaf_datasList, nwdaf_datasItem);
        }
    }
    }

    an_gw_status = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "anGwStatus");
    if (an_gw_status) {
    if (!cJSON_IsBool(an_gw_status)) {
        ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [an_gw_status]");
        goto end;
    }
    }

    sm_policy_update_context_data_local_var = OpenAPI_sm_policy_update_context_data_create (
        rep_policy_ctrl_req_triggers ? rep_policy_ctrl_req_triggersList : NULL,
        acc_net_ch_ids ? acc_net_ch_idsList : NULL,
        access_type ? access_typeVariable : 0,
        rat_type ? rat_typeVariable : 0,
        add_access_info ? add_access_info_local_nonprim : NULL,
        rel_access_info ? rel_access_info_local_nonprim : NULL,
        serving_network ? serving_network_local_nonprim : NULL,
        user_location_info ? user_location_info_local_nonprim : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        rel_ipv4_address && !cJSON_IsNull(rel_ipv4_address) ? ogs_strdup(rel_ipv4_address->valuestring) : NULL,
        ipv4_address && !cJSON_IsNull(ipv4_address) ? ogs_strdup(ipv4_address->valuestring) : NULL,
        ip_domain && !cJSON_IsNull(ip_domain) ? ogs_strdup(ip_domain->valuestring) : NULL,
        ipv6_address_prefix && !cJSON_IsNull(ipv6_address_prefix) ? ogs_strdup(ipv6_address_prefix->valuestring) : NULL,
        rel_ipv6_address_prefix && !cJSON_IsNull(rel_ipv6_address_prefix) ? ogs_strdup(rel_ipv6_address_prefix->valuestring) : NULL,
        add_ipv6_addr_prefixes && !cJSON_IsNull(add_ipv6_addr_prefixes) ? ogs_strdup(add_ipv6_addr_prefixes->valuestring) : NULL,
        add_rel_ipv6_addr_prefixes && !cJSON_IsNull(add_rel_ipv6_addr_prefixes) ? ogs_strdup(add_rel_ipv6_addr_prefixes->valuestring) : NULL,
        rel_ue_mac && !cJSON_IsNull(rel_ue_mac) ? ogs_strdup(rel_ue_mac->valuestring) : NULL,
        ue_mac && !cJSON_IsNull(ue_mac) ? ogs_strdup(ue_mac->valuestring) : NULL,
        subs_sess_ambr ? subs_sess_ambr_local_nonprim : NULL,
        auth_prof_index && !cJSON_IsNull(auth_prof_index) ? ogs_strdup(auth_prof_index->valuestring) : NULL,
        subs_def_qos ? subs_def_qos_local_nonprim : NULL,
        vplmn_qos ? vplmn_qos_local_nonprim : NULL,
        vplmn_qos_not_app ? true : false,
        vplmn_qos_not_app ? vplmn_qos_not_app->valueint : 0,
        num_of_pack_filter ? true : false,
        num_of_pack_filter ? num_of_pack_filter->valuedouble : 0,
        accu_usage_reports ? accu_usage_reportsList : NULL,
        _3gpp_ps_data_off_status ? true : false,
        _3gpp_ps_data_off_status ? _3gpp_ps_data_off_status->valueint : 0,
        app_detection_infos ? app_detection_infosList : NULL,
        rule_reports ? rule_reportsList : NULL,
        sess_rule_reports ? sess_rule_reportsList : NULL,
        qnc_reports ? qnc_reportsList : NULL,
        qos_mon_reports ? qos_mon_reportsList : NULL,
        user_location_info_time && !cJSON_IsNull(user_location_info_time) ? ogs_strdup(user_location_info_time->valuestring) : NULL,
        rep_pra_infos ? rep_pra_infosList : NULL,
        ue_init_res_req ? ue_init_res_req_local_nonprim : NULL,
        ref_qos_indication ? true : false,
        ref_qos_indication ? ref_qos_indication->valueint : 0,
        qos_flow_usage ? qos_flow_usageVariable : 0,
        credit_manage_status ? credit_manage_statusVariable : 0,
        serv_nf_id ? serv_nf_id_local_nonprim : NULL,
        trace_req && cJSON_IsNull(trace_req) ? true : false,
        trace_req ? trace_req_local_nonprim : NULL,
        ma_pdu_ind ? ma_pdu_indVariable : 0,
        atsss_capab ? atsss_capabVariable : 0,
        tsn_bridge_info ? tsn_bridge_info_local_nonprim : NULL,
        tsn_bridge_man_cont ? tsn_bridge_man_cont_local_nonprim : NULL,
        tsn_port_man_cont_dstt ? tsn_port_man_cont_dstt_local_nonprim : NULL,
        tsn_port_man_cont_nwtts ? tsn_port_man_cont_nwttsList : NULL,
        mul_addr_infos ? mul_addr_infosList : NULL,
        policy_dec_failure_reports ? policy_dec_failure_reportsList : NULL,
        invalid_policy_decs ? invalid_policy_decsList : NULL,
        traffic_descriptors ? traffic_descriptorsList : NULL,
        pcc_rule_id && !cJSON_IsNull(pcc_rule_id) ? ogs_strdup(pcc_rule_id->valuestring) : NULL,
        types_of_notif ? types_of_notifList : NULL,
        inter_grp_ids ? inter_grp_idsList : NULL,
        sat_backhaul_category ? sat_backhaul_categoryVariable : 0,
        pcf_ue_info && cJSON_IsNull(pcf_ue_info) ? true : false,
        pcf_ue_info ? pcf_ue_info_local_nonprim : NULL,
        nwdaf_datas && cJSON_IsNull(nwdaf_datas) ? true : false,
        nwdaf_datas ? nwdaf_datasList : NULL,
        an_gw_status ? true : false,
        an_gw_status ? an_gw_status->valueint : 0
    );

    return sm_policy_update_context_data_local_var;
end:
    if (rep_policy_ctrl_req_triggersList) {
        OpenAPI_list_free(rep_policy_ctrl_req_triggersList);
        rep_policy_ctrl_req_triggersList = NULL;
    }
    if (acc_net_ch_idsList) {
        OpenAPI_list_for_each(acc_net_ch_idsList, node) {
            OpenAPI_acc_net_ch_id_free(node->data);
        }
        OpenAPI_list_free(acc_net_ch_idsList);
        acc_net_ch_idsList = NULL;
    }
    if (add_access_info_local_nonprim) {
        OpenAPI_additional_access_info_free(add_access_info_local_nonprim);
        add_access_info_local_nonprim = NULL;
    }
    if (rel_access_info_local_nonprim) {
        OpenAPI_additional_access_info_free(rel_access_info_local_nonprim);
        rel_access_info_local_nonprim = NULL;
    }
    if (serving_network_local_nonprim) {
        OpenAPI_plmn_id_nid_free(serving_network_local_nonprim);
        serving_network_local_nonprim = NULL;
    }
    if (user_location_info_local_nonprim) {
        OpenAPI_user_location_free(user_location_info_local_nonprim);
        user_location_info_local_nonprim = NULL;
    }
    if (subs_sess_ambr_local_nonprim) {
        OpenAPI_ambr_free(subs_sess_ambr_local_nonprim);
        subs_sess_ambr_local_nonprim = NULL;
    }
    if (subs_def_qos_local_nonprim) {
        OpenAPI_subscribed_default_qos_free(subs_def_qos_local_nonprim);
        subs_def_qos_local_nonprim = NULL;
    }
    if (vplmn_qos_local_nonprim) {
        OpenAPI_vplmn_qos_free(vplmn_qos_local_nonprim);
        vplmn_qos_local_nonprim = NULL;
    }
    if (accu_usage_reportsList) {
        OpenAPI_list_for_each(accu_usage_reportsList, node) {
            OpenAPI_accu_usage_report_free(node->data);
        }
        OpenAPI_list_free(accu_usage_reportsList);
        accu_usage_reportsList = NULL;
    }
    if (app_detection_infosList) {
        OpenAPI_list_for_each(app_detection_infosList, node) {
            OpenAPI_app_detection_info_free(node->data);
        }
        OpenAPI_list_free(app_detection_infosList);
        app_detection_infosList = NULL;
    }
    if (rule_reportsList) {
        OpenAPI_list_for_each(rule_reportsList, node) {
            OpenAPI_rule_report_free(node->data);
        }
        OpenAPI_list_free(rule_reportsList);
        rule_reportsList = NULL;
    }
    if (sess_rule_reportsList) {
        OpenAPI_list_for_each(sess_rule_reportsList, node) {
            OpenAPI_session_rule_report_free(node->data);
        }
        OpenAPI_list_free(sess_rule_reportsList);
        sess_rule_reportsList = NULL;
    }
    if (qnc_reportsList) {
        OpenAPI_list_for_each(qnc_reportsList, node) {
            OpenAPI_qos_notification_control_info_free(node->data);
        }
        OpenAPI_list_free(qnc_reportsList);
        qnc_reportsList = NULL;
    }
    if (qos_mon_reportsList) {
        OpenAPI_list_for_each(qos_mon_reportsList, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(qos_mon_reportsList);
        qos_mon_reportsList = NULL;
    }
    if (rep_pra_infosList) {
        OpenAPI_list_for_each(rep_pra_infosList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(rep_pra_infosList);
        rep_pra_infosList = NULL;
    }
    if (ue_init_res_req_local_nonprim) {
        OpenAPI_ue_initiated_resource_request_free(ue_init_res_req_local_nonprim);
        ue_init_res_req_local_nonprim = NULL;
    }
    if (serv_nf_id_local_nonprim) {
        OpenAPI_serving_nf_identity_free(serv_nf_id_local_nonprim);
        serv_nf_id_local_nonprim = NULL;
    }
    if (trace_req_local_nonprim) {
        OpenAPI_trace_data_free(trace_req_local_nonprim);
        trace_req_local_nonprim = NULL;
    }
    if (tsn_bridge_info_local_nonprim) {
        OpenAPI_tsn_bridge_info_free(tsn_bridge_info_local_nonprim);
        tsn_bridge_info_local_nonprim = NULL;
    }
    if (tsn_bridge_man_cont_local_nonprim) {
        OpenAPI_bridge_management_container_free(tsn_bridge_man_cont_local_nonprim);
        tsn_bridge_man_cont_local_nonprim = NULL;
    }
    if (tsn_port_man_cont_dstt_local_nonprim) {
        OpenAPI_port_management_container_free(tsn_port_man_cont_dstt_local_nonprim);
        tsn_port_man_cont_dstt_local_nonprim = NULL;
    }
    if (tsn_port_man_cont_nwttsList) {
        OpenAPI_list_for_each(tsn_port_man_cont_nwttsList, node) {
            OpenAPI_port_management_container_free(node->data);
        }
        OpenAPI_list_free(tsn_port_man_cont_nwttsList);
        tsn_port_man_cont_nwttsList = NULL;
    }
    if (mul_addr_infosList) {
        OpenAPI_list_for_each(mul_addr_infosList, node) {
            OpenAPI_ip_multicast_address_info_free(node->data);
        }
        OpenAPI_list_free(mul_addr_infosList);
        mul_addr_infosList = NULL;
    }
    if (policy_dec_failure_reportsList) {
        OpenAPI_list_free(policy_dec_failure_reportsList);
        policy_dec_failure_reportsList = NULL;
    }
    if (invalid_policy_decsList) {
        OpenAPI_list_for_each(invalid_policy_decsList, node) {
            OpenAPI_invalid_param_free(node->data);
        }
        OpenAPI_list_free(invalid_policy_decsList);
        invalid_policy_decsList = NULL;
    }
    if (traffic_descriptorsList) {
        OpenAPI_list_for_each(traffic_descriptorsList, node) {
            OpenAPI_ddd_traffic_descriptor_free(node->data);
        }
        OpenAPI_list_free(traffic_descriptorsList);
        traffic_descriptorsList = NULL;
    }
    if (types_of_notifList) {
        OpenAPI_list_free(types_of_notifList);
        types_of_notifList = NULL;
    }
    if (inter_grp_idsList) {
        OpenAPI_list_for_each(inter_grp_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(inter_grp_idsList);
        inter_grp_idsList = NULL;
    }
    if (pcf_ue_info_local_nonprim) {
        OpenAPI_pcf_ue_callback_info_free(pcf_ue_info_local_nonprim);
        pcf_ue_info_local_nonprim = NULL;
    }
    if (nwdaf_datasList) {
        OpenAPI_list_for_each(nwdaf_datasList, node) {
            OpenAPI_nwdaf_data_free(node->data);
        }
        OpenAPI_list_free(nwdaf_datasList);
        nwdaf_datasList = NULL;
    }
    return NULL;
}

OpenAPI_sm_policy_update_context_data_t *OpenAPI_sm_policy_update_context_data_copy(OpenAPI_sm_policy_update_context_data_t *dst, OpenAPI_sm_policy_update_context_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_policy_update_context_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_sm_policy_update_context_data_free(dst);
    dst = OpenAPI_sm_policy_update_context_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

