
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
    )
{
    OpenAPI_sm_policy_update_context_data_t *sm_policy_update_context_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_sm_policy_update_context_data_t));
    if (!sm_policy_update_context_data_local_var) {
        return NULL;
    }
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
    sm_policy_update_context_data_local_var->num_of_pack_filter = num_of_pack_filter;
    sm_policy_update_context_data_local_var->accu_usage_reports = accu_usage_reports;
    sm_policy_update_context_data_local_var->_3gpp_ps_data_off_status = _3gpp_ps_data_off_status;
    sm_policy_update_context_data_local_var->app_detection_infos = app_detection_infos;
    sm_policy_update_context_data_local_var->rule_reports = rule_reports;
    sm_policy_update_context_data_local_var->sess_rule_reports = sess_rule_reports;
    sm_policy_update_context_data_local_var->qnc_reports = qnc_reports;
    sm_policy_update_context_data_local_var->qos_mon_reports = qos_mon_reports;
    sm_policy_update_context_data_local_var->user_location_info_time = user_location_info_time;
    sm_policy_update_context_data_local_var->rep_pra_infos = rep_pra_infos;
    sm_policy_update_context_data_local_var->ue_init_res_req = ue_init_res_req;
    sm_policy_update_context_data_local_var->ref_qos_indication = ref_qos_indication;
    sm_policy_update_context_data_local_var->qos_flow_usage = qos_flow_usage;
    sm_policy_update_context_data_local_var->credit_manage_status = credit_manage_status;
    sm_policy_update_context_data_local_var->serv_nf_id = serv_nf_id;
    sm_policy_update_context_data_local_var->trace_req = trace_req;
    sm_policy_update_context_data_local_var->ma_pdu_ind = ma_pdu_ind;
    sm_policy_update_context_data_local_var->atsss_capab = atsss_capab;
    sm_policy_update_context_data_local_var->tsn_bridge_info = tsn_bridge_info;
    sm_policy_update_context_data_local_var->tsn_port_man_cont_dstt = tsn_port_man_cont_dstt;
    sm_policy_update_context_data_local_var->tsn_port_man_cont_nwtts = tsn_port_man_cont_nwtts;
    sm_policy_update_context_data_local_var->mul_addr_infos = mul_addr_infos;

    return sm_policy_update_context_data_local_var;
}

void OpenAPI_sm_policy_update_context_data_free(OpenAPI_sm_policy_update_context_data_t *sm_policy_update_context_data)
{
    if (NULL == sm_policy_update_context_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(sm_policy_update_context_data->rep_policy_ctrl_req_triggers, node) {
        OpenAPI_policy_control_request_trigger_free(node->data);
    }
    OpenAPI_list_free(sm_policy_update_context_data->rep_policy_ctrl_req_triggers);
    OpenAPI_list_for_each(sm_policy_update_context_data->acc_net_ch_ids, node) {
        OpenAPI_acc_net_ch_id_free(node->data);
    }
    OpenAPI_list_free(sm_policy_update_context_data->acc_net_ch_ids);
    OpenAPI_additional_access_info_free(sm_policy_update_context_data->add_access_info);
    OpenAPI_additional_access_info_free(sm_policy_update_context_data->rel_access_info);
    OpenAPI_plmn_id_nid_free(sm_policy_update_context_data->serving_network);
    OpenAPI_user_location_free(sm_policy_update_context_data->user_location_info);
    ogs_free(sm_policy_update_context_data->ue_time_zone);
    ogs_free(sm_policy_update_context_data->rel_ipv4_address);
    ogs_free(sm_policy_update_context_data->ipv4_address);
    ogs_free(sm_policy_update_context_data->ip_domain);
    ogs_free(sm_policy_update_context_data->ipv6_address_prefix);
    ogs_free(sm_policy_update_context_data->rel_ipv6_address_prefix);
    ogs_free(sm_policy_update_context_data->add_ipv6_addr_prefixes);
    ogs_free(sm_policy_update_context_data->add_rel_ipv6_addr_prefixes);
    ogs_free(sm_policy_update_context_data->rel_ue_mac);
    ogs_free(sm_policy_update_context_data->ue_mac);
    OpenAPI_ambr_free(sm_policy_update_context_data->subs_sess_ambr);
    ogs_free(sm_policy_update_context_data->auth_prof_index);
    OpenAPI_subscribed_default_qos_free(sm_policy_update_context_data->subs_def_qos);
    OpenAPI_list_for_each(sm_policy_update_context_data->accu_usage_reports, node) {
        OpenAPI_accu_usage_report_free(node->data);
    }
    OpenAPI_list_free(sm_policy_update_context_data->accu_usage_reports);
    OpenAPI_list_for_each(sm_policy_update_context_data->app_detection_infos, node) {
        OpenAPI_app_detection_info_free(node->data);
    }
    OpenAPI_list_free(sm_policy_update_context_data->app_detection_infos);
    OpenAPI_list_for_each(sm_policy_update_context_data->rule_reports, node) {
        OpenAPI_rule_report_free(node->data);
    }
    OpenAPI_list_free(sm_policy_update_context_data->rule_reports);
    OpenAPI_list_for_each(sm_policy_update_context_data->sess_rule_reports, node) {
        OpenAPI_session_rule_report_free(node->data);
    }
    OpenAPI_list_free(sm_policy_update_context_data->sess_rule_reports);
    OpenAPI_list_for_each(sm_policy_update_context_data->qnc_reports, node) {
        OpenAPI_qos_notification_control_info_free(node->data);
    }
    OpenAPI_list_free(sm_policy_update_context_data->qnc_reports);
    OpenAPI_list_for_each(sm_policy_update_context_data->qos_mon_reports, node) {
        OpenAPI_qos_monitoring_report_free(node->data);
    }
    OpenAPI_list_free(sm_policy_update_context_data->qos_mon_reports);
    ogs_free(sm_policy_update_context_data->user_location_info_time);
    OpenAPI_list_for_each(sm_policy_update_context_data->rep_pra_infos, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_presence_info_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_update_context_data->rep_pra_infos);
    OpenAPI_ue_initiated_resource_request_free(sm_policy_update_context_data->ue_init_res_req);
    OpenAPI_qos_flow_usage_free(sm_policy_update_context_data->qos_flow_usage);
    OpenAPI_credit_management_status_free(sm_policy_update_context_data->credit_manage_status);
    OpenAPI_serving_nf_identity_free(sm_policy_update_context_data->serv_nf_id);
    OpenAPI_trace_data_free(sm_policy_update_context_data->trace_req);
    OpenAPI_ma_pdu_indication_free(sm_policy_update_context_data->ma_pdu_ind);
    OpenAPI_atsss_capability_free(sm_policy_update_context_data->atsss_capab);
    OpenAPI_tsn_bridge_info_free(sm_policy_update_context_data->tsn_bridge_info);
    OpenAPI_port_management_container_free(sm_policy_update_context_data->tsn_port_man_cont_dstt);
    OpenAPI_list_for_each(sm_policy_update_context_data->tsn_port_man_cont_nwtts, node) {
        OpenAPI_port_management_container_free(node->data);
    }
    OpenAPI_list_free(sm_policy_update_context_data->tsn_port_man_cont_nwtts);
    OpenAPI_list_for_each(sm_policy_update_context_data->mul_addr_infos, node) {
        OpenAPI_ip_multicast_address_info_free(node->data);
    }
    OpenAPI_list_free(sm_policy_update_context_data->mul_addr_infos);
    ogs_free(sm_policy_update_context_data);
}

cJSON *OpenAPI_sm_policy_update_context_data_convertToJSON(OpenAPI_sm_policy_update_context_data_t *sm_policy_update_context_data)
{
    cJSON *item = NULL;

    if (sm_policy_update_context_data == NULL) {
        ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [SmPolicyUpdateContextData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_policy_update_context_data->rep_policy_ctrl_req_triggers) {
        cJSON *rep_policy_ctrl_req_triggersList = cJSON_AddArrayToObject(item, "repPolicyCtrlReqTriggers");
        if (rep_policy_ctrl_req_triggersList == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rep_policy_ctrl_req_triggers]");
            goto end;
        }

        OpenAPI_lnode_t *rep_policy_ctrl_req_triggers_node;
        if (sm_policy_update_context_data->rep_policy_ctrl_req_triggers) {
            OpenAPI_list_for_each(sm_policy_update_context_data->rep_policy_ctrl_req_triggers, rep_policy_ctrl_req_triggers_node) {
                cJSON *itemLocal = OpenAPI_policy_control_request_trigger_convertToJSON(rep_policy_ctrl_req_triggers_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rep_policy_ctrl_req_triggers]");
                    goto end;
                }
                cJSON_AddItemToArray(rep_policy_ctrl_req_triggersList, itemLocal);
            }
        }
    }

    if (sm_policy_update_context_data->acc_net_ch_ids) {
        cJSON *acc_net_ch_idsList = cJSON_AddArrayToObject(item, "accNetChIds");
        if (acc_net_ch_idsList == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [acc_net_ch_ids]");
            goto end;
        }

        OpenAPI_lnode_t *acc_net_ch_ids_node;
        if (sm_policy_update_context_data->acc_net_ch_ids) {
            OpenAPI_list_for_each(sm_policy_update_context_data->acc_net_ch_ids, acc_net_ch_ids_node) {
                cJSON *itemLocal = OpenAPI_acc_net_ch_id_convertToJSON(acc_net_ch_ids_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [acc_net_ch_ids]");
                    goto end;
                }
                cJSON_AddItemToArray(acc_net_ch_idsList, itemLocal);
            }
        }
    }

    if (sm_policy_update_context_data->access_type) {
        if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(sm_policy_update_context_data->access_type)) == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [access_type]");
            goto end;
        }
    }

    if (sm_policy_update_context_data->rat_type) {
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

    if (sm_policy_update_context_data->num_of_pack_filter) {
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

        OpenAPI_lnode_t *accu_usage_reports_node;
        if (sm_policy_update_context_data->accu_usage_reports) {
            OpenAPI_list_for_each(sm_policy_update_context_data->accu_usage_reports, accu_usage_reports_node) {
                cJSON *itemLocal = OpenAPI_accu_usage_report_convertToJSON(accu_usage_reports_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [accu_usage_reports]");
                    goto end;
                }
                cJSON_AddItemToArray(accu_usage_reportsList, itemLocal);
            }
        }
    }

    if (sm_policy_update_context_data->_3gpp_ps_data_off_status) {
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

        OpenAPI_lnode_t *app_detection_infos_node;
        if (sm_policy_update_context_data->app_detection_infos) {
            OpenAPI_list_for_each(sm_policy_update_context_data->app_detection_infos, app_detection_infos_node) {
                cJSON *itemLocal = OpenAPI_app_detection_info_convertToJSON(app_detection_infos_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [app_detection_infos]");
                    goto end;
                }
                cJSON_AddItemToArray(app_detection_infosList, itemLocal);
            }
        }
    }

    if (sm_policy_update_context_data->rule_reports) {
        cJSON *rule_reportsList = cJSON_AddArrayToObject(item, "ruleReports");
        if (rule_reportsList == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rule_reports]");
            goto end;
        }

        OpenAPI_lnode_t *rule_reports_node;
        if (sm_policy_update_context_data->rule_reports) {
            OpenAPI_list_for_each(sm_policy_update_context_data->rule_reports, rule_reports_node) {
                cJSON *itemLocal = OpenAPI_rule_report_convertToJSON(rule_reports_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rule_reports]");
                    goto end;
                }
                cJSON_AddItemToArray(rule_reportsList, itemLocal);
            }
        }
    }

    if (sm_policy_update_context_data->sess_rule_reports) {
        cJSON *sess_rule_reportsList = cJSON_AddArrayToObject(item, "sessRuleReports");
        if (sess_rule_reportsList == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [sess_rule_reports]");
            goto end;
        }

        OpenAPI_lnode_t *sess_rule_reports_node;
        if (sm_policy_update_context_data->sess_rule_reports) {
            OpenAPI_list_for_each(sm_policy_update_context_data->sess_rule_reports, sess_rule_reports_node) {
                cJSON *itemLocal = OpenAPI_session_rule_report_convertToJSON(sess_rule_reports_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [sess_rule_reports]");
                    goto end;
                }
                cJSON_AddItemToArray(sess_rule_reportsList, itemLocal);
            }
        }
    }

    if (sm_policy_update_context_data->qnc_reports) {
        cJSON *qnc_reportsList = cJSON_AddArrayToObject(item, "qncReports");
        if (qnc_reportsList == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [qnc_reports]");
            goto end;
        }

        OpenAPI_lnode_t *qnc_reports_node;
        if (sm_policy_update_context_data->qnc_reports) {
            OpenAPI_list_for_each(sm_policy_update_context_data->qnc_reports, qnc_reports_node) {
                cJSON *itemLocal = OpenAPI_qos_notification_control_info_convertToJSON(qnc_reports_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [qnc_reports]");
                    goto end;
                }
                cJSON_AddItemToArray(qnc_reportsList, itemLocal);
            }
        }
    }

    if (sm_policy_update_context_data->qos_mon_reports) {
        cJSON *qos_mon_reportsList = cJSON_AddArrayToObject(item, "qosMonReports");
        if (qos_mon_reportsList == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [qos_mon_reports]");
            goto end;
        }

        OpenAPI_lnode_t *qos_mon_reports_node;
        if (sm_policy_update_context_data->qos_mon_reports) {
            OpenAPI_list_for_each(sm_policy_update_context_data->qos_mon_reports, qos_mon_reports_node) {
                cJSON *itemLocal = OpenAPI_qos_monitoring_report_convertToJSON(qos_mon_reports_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [qos_mon_reports]");
                    goto end;
                }
                cJSON_AddItemToArray(qos_mon_reportsList, itemLocal);
            }
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
        OpenAPI_lnode_t *rep_pra_infos_node;
        if (sm_policy_update_context_data->rep_pra_infos) {
            OpenAPI_list_for_each(sm_policy_update_context_data->rep_pra_infos, rep_pra_infos_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)rep_pra_infos_node->data;
                cJSON *itemLocal = OpenAPI_presence_info_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [rep_pra_infos]");
                    goto end;
                }
                cJSON_AddItemToObject(rep_pra_infos, localKeyValue->key, itemLocal);
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

    if (sm_policy_update_context_data->ref_qos_indication) {
        if (cJSON_AddBoolToObject(item, "refQosIndication", sm_policy_update_context_data->ref_qos_indication) == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [ref_qos_indication]");
            goto end;
        }
    }

    if (sm_policy_update_context_data->qos_flow_usage) {
        cJSON *qos_flow_usage_local_JSON = OpenAPI_qos_flow_usage_convertToJSON(sm_policy_update_context_data->qos_flow_usage);
        if (qos_flow_usage_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [qos_flow_usage]");
            goto end;
        }
        cJSON_AddItemToObject(item, "qosFlowUsage", qos_flow_usage_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [qos_flow_usage]");
            goto end;
        }
    }

    if (sm_policy_update_context_data->credit_manage_status) {
        cJSON *credit_manage_status_local_JSON = OpenAPI_credit_management_status_convertToJSON(sm_policy_update_context_data->credit_manage_status);
        if (credit_manage_status_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [credit_manage_status]");
            goto end;
        }
        cJSON_AddItemToObject(item, "creditManageStatus", credit_manage_status_local_JSON);
        if (item->child == NULL) {
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
    }

    if (sm_policy_update_context_data->ma_pdu_ind) {
        cJSON *ma_pdu_ind_local_JSON = OpenAPI_ma_pdu_indication_convertToJSON(sm_policy_update_context_data->ma_pdu_ind);
        if (ma_pdu_ind_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [ma_pdu_ind]");
            goto end;
        }
        cJSON_AddItemToObject(item, "maPduInd", ma_pdu_ind_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [ma_pdu_ind]");
            goto end;
        }
    }

    if (sm_policy_update_context_data->atsss_capab) {
        cJSON *atsss_capab_local_JSON = OpenAPI_atsss_capability_convertToJSON(sm_policy_update_context_data->atsss_capab);
        if (atsss_capab_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [atsss_capab]");
            goto end;
        }
        cJSON_AddItemToObject(item, "atsssCapab", atsss_capab_local_JSON);
        if (item->child == NULL) {
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

        OpenAPI_lnode_t *tsn_port_man_cont_nwtts_node;
        if (sm_policy_update_context_data->tsn_port_man_cont_nwtts) {
            OpenAPI_list_for_each(sm_policy_update_context_data->tsn_port_man_cont_nwtts, tsn_port_man_cont_nwtts_node) {
                cJSON *itemLocal = OpenAPI_port_management_container_convertToJSON(tsn_port_man_cont_nwtts_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [tsn_port_man_cont_nwtts]");
                    goto end;
                }
                cJSON_AddItemToArray(tsn_port_man_cont_nwttsList, itemLocal);
            }
        }
    }

    if (sm_policy_update_context_data->mul_addr_infos) {
        cJSON *mul_addr_infosList = cJSON_AddArrayToObject(item, "mulAddrInfos");
        if (mul_addr_infosList == NULL) {
            ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [mul_addr_infos]");
            goto end;
        }

        OpenAPI_lnode_t *mul_addr_infos_node;
        if (sm_policy_update_context_data->mul_addr_infos) {
            OpenAPI_list_for_each(sm_policy_update_context_data->mul_addr_infos, mul_addr_infos_node) {
                cJSON *itemLocal = OpenAPI_ip_multicast_address_info_convertToJSON(mul_addr_infos_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_update_context_data_convertToJSON() failed [mul_addr_infos]");
                    goto end;
                }
                cJSON_AddItemToArray(mul_addr_infosList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_sm_policy_update_context_data_t *OpenAPI_sm_policy_update_context_data_parseFromJSON(cJSON *sm_policy_update_context_dataJSON)
{
    OpenAPI_sm_policy_update_context_data_t *sm_policy_update_context_data_local_var = NULL;
    cJSON *rep_policy_ctrl_req_triggers = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "repPolicyCtrlReqTriggers");

    OpenAPI_list_t *rep_policy_ctrl_req_triggersList;
    if (rep_policy_ctrl_req_triggers) {
        cJSON *rep_policy_ctrl_req_triggers_local_nonprimitive;
        if (!cJSON_IsArray(rep_policy_ctrl_req_triggers)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rep_policy_ctrl_req_triggers]");
            goto end;
        }

        rep_policy_ctrl_req_triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(rep_policy_ctrl_req_triggers_local_nonprimitive, rep_policy_ctrl_req_triggers ) {
            if (!cJSON_IsObject(rep_policy_ctrl_req_triggers_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rep_policy_ctrl_req_triggers]");
                goto end;
            }
            OpenAPI_policy_control_request_trigger_t *rep_policy_ctrl_req_triggersItem = OpenAPI_policy_control_request_trigger_parseFromJSON(rep_policy_ctrl_req_triggers_local_nonprimitive);

            OpenAPI_list_add(rep_policy_ctrl_req_triggersList, rep_policy_ctrl_req_triggersItem);
        }
    }

    cJSON *acc_net_ch_ids = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "accNetChIds");

    OpenAPI_list_t *acc_net_ch_idsList;
    if (acc_net_ch_ids) {
        cJSON *acc_net_ch_ids_local_nonprimitive;
        if (!cJSON_IsArray(acc_net_ch_ids)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [acc_net_ch_ids]");
            goto end;
        }

        acc_net_ch_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(acc_net_ch_ids_local_nonprimitive, acc_net_ch_ids ) {
            if (!cJSON_IsObject(acc_net_ch_ids_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [acc_net_ch_ids]");
                goto end;
            }
            OpenAPI_acc_net_ch_id_t *acc_net_ch_idsItem = OpenAPI_acc_net_ch_id_parseFromJSON(acc_net_ch_ids_local_nonprimitive);

            OpenAPI_list_add(acc_net_ch_idsList, acc_net_ch_idsItem);
        }
    }

    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "accessType");

    OpenAPI_access_type_e access_typeVariable;
    if (access_type) {
        if (!cJSON_IsString(access_type)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [access_type]");
            goto end;
        }
        access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    cJSON *rat_type = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ratType");

    OpenAPI_rat_type_e rat_typeVariable;
    if (rat_type) {
        if (!cJSON_IsString(rat_type)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rat_type]");
            goto end;
        }
        rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    cJSON *add_access_info = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "addAccessInfo");

    OpenAPI_additional_access_info_t *add_access_info_local_nonprim = NULL;
    if (add_access_info) {
        add_access_info_local_nonprim = OpenAPI_additional_access_info_parseFromJSON(add_access_info);
    }

    cJSON *rel_access_info = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "relAccessInfo");

    OpenAPI_additional_access_info_t *rel_access_info_local_nonprim = NULL;
    if (rel_access_info) {
        rel_access_info_local_nonprim = OpenAPI_additional_access_info_parseFromJSON(rel_access_info);
    }

    cJSON *serving_network = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "servingNetwork");

    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    if (serving_network) {
        serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    }

    cJSON *user_location_info = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "userLocationInfo");

    OpenAPI_user_location_t *user_location_info_local_nonprim = NULL;
    if (user_location_info) {
        user_location_info_local_nonprim = OpenAPI_user_location_parseFromJSON(user_location_info);
    }

    cJSON *ue_time_zone = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ueTimeZone");

    if (ue_time_zone) {
        if (!cJSON_IsString(ue_time_zone)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [ue_time_zone]");
            goto end;
        }
    }

    cJSON *rel_ipv4_address = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "relIpv4Address");

    if (rel_ipv4_address) {
        if (!cJSON_IsString(rel_ipv4_address)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rel_ipv4_address]");
            goto end;
        }
    }

    cJSON *ipv4_address = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ipv4Address");

    if (ipv4_address) {
        if (!cJSON_IsString(ipv4_address)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [ipv4_address]");
            goto end;
        }
    }

    cJSON *ip_domain = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ipDomain");

    if (ip_domain) {
        if (!cJSON_IsString(ip_domain)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [ip_domain]");
            goto end;
        }
    }

    cJSON *ipv6_address_prefix = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ipv6AddressPrefix");

    if (ipv6_address_prefix) {
        if (!cJSON_IsString(ipv6_address_prefix)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [ipv6_address_prefix]");
            goto end;
        }
    }

    cJSON *rel_ipv6_address_prefix = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "relIpv6AddressPrefix");

    if (rel_ipv6_address_prefix) {
        if (!cJSON_IsString(rel_ipv6_address_prefix)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rel_ipv6_address_prefix]");
            goto end;
        }
    }

    cJSON *add_ipv6_addr_prefixes = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "addIpv6AddrPrefixes");

    if (add_ipv6_addr_prefixes) {
        if (!cJSON_IsString(add_ipv6_addr_prefixes)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [add_ipv6_addr_prefixes]");
            goto end;
        }
    }

    cJSON *add_rel_ipv6_addr_prefixes = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "addRelIpv6AddrPrefixes");

    if (add_rel_ipv6_addr_prefixes) {
        if (!cJSON_IsString(add_rel_ipv6_addr_prefixes)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [add_rel_ipv6_addr_prefixes]");
            goto end;
        }
    }

    cJSON *rel_ue_mac = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "relUeMac");

    if (rel_ue_mac) {
        if (!cJSON_IsString(rel_ue_mac)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rel_ue_mac]");
            goto end;
        }
    }

    cJSON *ue_mac = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ueMac");

    if (ue_mac) {
        if (!cJSON_IsString(ue_mac)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [ue_mac]");
            goto end;
        }
    }

    cJSON *subs_sess_ambr = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "subsSessAmbr");

    OpenAPI_ambr_t *subs_sess_ambr_local_nonprim = NULL;
    if (subs_sess_ambr) {
        subs_sess_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(subs_sess_ambr);
    }

    cJSON *auth_prof_index = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "authProfIndex");

    if (auth_prof_index) {
        if (!cJSON_IsString(auth_prof_index)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [auth_prof_index]");
            goto end;
        }
    }

    cJSON *subs_def_qos = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "subsDefQos");

    OpenAPI_subscribed_default_qos_t *subs_def_qos_local_nonprim = NULL;
    if (subs_def_qos) {
        subs_def_qos_local_nonprim = OpenAPI_subscribed_default_qos_parseFromJSON(subs_def_qos);
    }

    cJSON *num_of_pack_filter = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "numOfPackFilter");

    if (num_of_pack_filter) {
        if (!cJSON_IsNumber(num_of_pack_filter)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [num_of_pack_filter]");
            goto end;
        }
    }

    cJSON *accu_usage_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "accuUsageReports");

    OpenAPI_list_t *accu_usage_reportsList;
    if (accu_usage_reports) {
        cJSON *accu_usage_reports_local_nonprimitive;
        if (!cJSON_IsArray(accu_usage_reports)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [accu_usage_reports]");
            goto end;
        }

        accu_usage_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(accu_usage_reports_local_nonprimitive, accu_usage_reports ) {
            if (!cJSON_IsObject(accu_usage_reports_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [accu_usage_reports]");
                goto end;
            }
            OpenAPI_accu_usage_report_t *accu_usage_reportsItem = OpenAPI_accu_usage_report_parseFromJSON(accu_usage_reports_local_nonprimitive);

            OpenAPI_list_add(accu_usage_reportsList, accu_usage_reportsItem);
        }
    }

    cJSON *_3gpp_ps_data_off_status = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "3gppPsDataOffStatus");

    if (_3gpp_ps_data_off_status) {
        if (!cJSON_IsBool(_3gpp_ps_data_off_status)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [_3gpp_ps_data_off_status]");
            goto end;
        }
    }

    cJSON *app_detection_infos = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "appDetectionInfos");

    OpenAPI_list_t *app_detection_infosList;
    if (app_detection_infos) {
        cJSON *app_detection_infos_local_nonprimitive;
        if (!cJSON_IsArray(app_detection_infos)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [app_detection_infos]");
            goto end;
        }

        app_detection_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_detection_infos_local_nonprimitive, app_detection_infos ) {
            if (!cJSON_IsObject(app_detection_infos_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [app_detection_infos]");
                goto end;
            }
            OpenAPI_app_detection_info_t *app_detection_infosItem = OpenAPI_app_detection_info_parseFromJSON(app_detection_infos_local_nonprimitive);

            OpenAPI_list_add(app_detection_infosList, app_detection_infosItem);
        }
    }

    cJSON *rule_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ruleReports");

    OpenAPI_list_t *rule_reportsList;
    if (rule_reports) {
        cJSON *rule_reports_local_nonprimitive;
        if (!cJSON_IsArray(rule_reports)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rule_reports]");
            goto end;
        }

        rule_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(rule_reports_local_nonprimitive, rule_reports ) {
            if (!cJSON_IsObject(rule_reports_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rule_reports]");
                goto end;
            }
            OpenAPI_rule_report_t *rule_reportsItem = OpenAPI_rule_report_parseFromJSON(rule_reports_local_nonprimitive);

            OpenAPI_list_add(rule_reportsList, rule_reportsItem);
        }
    }

    cJSON *sess_rule_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "sessRuleReports");

    OpenAPI_list_t *sess_rule_reportsList;
    if (sess_rule_reports) {
        cJSON *sess_rule_reports_local_nonprimitive;
        if (!cJSON_IsArray(sess_rule_reports)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [sess_rule_reports]");
            goto end;
        }

        sess_rule_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(sess_rule_reports_local_nonprimitive, sess_rule_reports ) {
            if (!cJSON_IsObject(sess_rule_reports_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [sess_rule_reports]");
                goto end;
            }
            OpenAPI_session_rule_report_t *sess_rule_reportsItem = OpenAPI_session_rule_report_parseFromJSON(sess_rule_reports_local_nonprimitive);

            OpenAPI_list_add(sess_rule_reportsList, sess_rule_reportsItem);
        }
    }

    cJSON *qnc_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "qncReports");

    OpenAPI_list_t *qnc_reportsList;
    if (qnc_reports) {
        cJSON *qnc_reports_local_nonprimitive;
        if (!cJSON_IsArray(qnc_reports)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [qnc_reports]");
            goto end;
        }

        qnc_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qnc_reports_local_nonprimitive, qnc_reports ) {
            if (!cJSON_IsObject(qnc_reports_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [qnc_reports]");
                goto end;
            }
            OpenAPI_qos_notification_control_info_t *qnc_reportsItem = OpenAPI_qos_notification_control_info_parseFromJSON(qnc_reports_local_nonprimitive);

            OpenAPI_list_add(qnc_reportsList, qnc_reportsItem);
        }
    }

    cJSON *qos_mon_reports = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "qosMonReports");

    OpenAPI_list_t *qos_mon_reportsList;
    if (qos_mon_reports) {
        cJSON *qos_mon_reports_local_nonprimitive;
        if (!cJSON_IsArray(qos_mon_reports)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [qos_mon_reports]");
            goto end;
        }

        qos_mon_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_mon_reports_local_nonprimitive, qos_mon_reports ) {
            if (!cJSON_IsObject(qos_mon_reports_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [qos_mon_reports]");
                goto end;
            }
            OpenAPI_qos_monitoring_report_t *qos_mon_reportsItem = OpenAPI_qos_monitoring_report_parseFromJSON(qos_mon_reports_local_nonprimitive);

            OpenAPI_list_add(qos_mon_reportsList, qos_mon_reportsItem);
        }
    }

    cJSON *user_location_info_time = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "userLocationInfoTime");

    if (user_location_info_time) {
        if (!cJSON_IsString(user_location_info_time)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [user_location_info_time]");
            goto end;
        }
    }

    cJSON *rep_pra_infos = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "repPraInfos");

    OpenAPI_list_t *rep_pra_infosList;
    if (rep_pra_infos) {
        cJSON *rep_pra_infos_local_map;
        if (!cJSON_IsObject(rep_pra_infos)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rep_pra_infos]");
            goto end;
        }
        rep_pra_infosList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(rep_pra_infos_local_map, rep_pra_infos) {
            cJSON *localMapObject = rep_pra_infos_local_map;
            if (!cJSON_IsObject(rep_pra_infos_local_map)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [rep_pra_infos]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_presence_info_parseFromJSON(localMapObject));
            OpenAPI_list_add(rep_pra_infosList, localMapKeyPair);
        }
    }

    cJSON *ue_init_res_req = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "ueInitResReq");

    OpenAPI_ue_initiated_resource_request_t *ue_init_res_req_local_nonprim = NULL;
    if (ue_init_res_req) {
        ue_init_res_req_local_nonprim = OpenAPI_ue_initiated_resource_request_parseFromJSON(ue_init_res_req);
    }

    cJSON *ref_qos_indication = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "refQosIndication");

    if (ref_qos_indication) {
        if (!cJSON_IsBool(ref_qos_indication)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [ref_qos_indication]");
            goto end;
        }
    }

    cJSON *qos_flow_usage = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "qosFlowUsage");

    OpenAPI_qos_flow_usage_t *qos_flow_usage_local_nonprim = NULL;
    if (qos_flow_usage) {
        qos_flow_usage_local_nonprim = OpenAPI_qos_flow_usage_parseFromJSON(qos_flow_usage);
    }

    cJSON *credit_manage_status = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "creditManageStatus");

    OpenAPI_credit_management_status_t *credit_manage_status_local_nonprim = NULL;
    if (credit_manage_status) {
        credit_manage_status_local_nonprim = OpenAPI_credit_management_status_parseFromJSON(credit_manage_status);
    }

    cJSON *serv_nf_id = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "servNfId");

    OpenAPI_serving_nf_identity_t *serv_nf_id_local_nonprim = NULL;
    if (serv_nf_id) {
        serv_nf_id_local_nonprim = OpenAPI_serving_nf_identity_parseFromJSON(serv_nf_id);
    }

    cJSON *trace_req = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "traceReq");

    OpenAPI_trace_data_t *trace_req_local_nonprim = NULL;
    if (trace_req) {
        trace_req_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_req);
    }

    cJSON *ma_pdu_ind = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "maPduInd");

    OpenAPI_ma_pdu_indication_t *ma_pdu_ind_local_nonprim = NULL;
    if (ma_pdu_ind) {
        ma_pdu_ind_local_nonprim = OpenAPI_ma_pdu_indication_parseFromJSON(ma_pdu_ind);
    }

    cJSON *atsss_capab = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "atsssCapab");

    OpenAPI_atsss_capability_t *atsss_capab_local_nonprim = NULL;
    if (atsss_capab) {
        atsss_capab_local_nonprim = OpenAPI_atsss_capability_parseFromJSON(atsss_capab);
    }

    cJSON *tsn_bridge_info = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "tsnBridgeInfo");

    OpenAPI_tsn_bridge_info_t *tsn_bridge_info_local_nonprim = NULL;
    if (tsn_bridge_info) {
        tsn_bridge_info_local_nonprim = OpenAPI_tsn_bridge_info_parseFromJSON(tsn_bridge_info);
    }

    cJSON *tsn_port_man_cont_dstt = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "tsnPortManContDstt");

    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt_local_nonprim = NULL;
    if (tsn_port_man_cont_dstt) {
        tsn_port_man_cont_dstt_local_nonprim = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_dstt);
    }

    cJSON *tsn_port_man_cont_nwtts = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "tsnPortManContNwtts");

    OpenAPI_list_t *tsn_port_man_cont_nwttsList;
    if (tsn_port_man_cont_nwtts) {
        cJSON *tsn_port_man_cont_nwtts_local_nonprimitive;
        if (!cJSON_IsArray(tsn_port_man_cont_nwtts)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }

        tsn_port_man_cont_nwttsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tsn_port_man_cont_nwtts_local_nonprimitive, tsn_port_man_cont_nwtts ) {
            if (!cJSON_IsObject(tsn_port_man_cont_nwtts_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
                goto end;
            }
            OpenAPI_port_management_container_t *tsn_port_man_cont_nwttsItem = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_nwtts_local_nonprimitive);

            OpenAPI_list_add(tsn_port_man_cont_nwttsList, tsn_port_man_cont_nwttsItem);
        }
    }

    cJSON *mul_addr_infos = cJSON_GetObjectItemCaseSensitive(sm_policy_update_context_dataJSON, "mulAddrInfos");

    OpenAPI_list_t *mul_addr_infosList;
    if (mul_addr_infos) {
        cJSON *mul_addr_infos_local_nonprimitive;
        if (!cJSON_IsArray(mul_addr_infos)) {
            ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [mul_addr_infos]");
            goto end;
        }

        mul_addr_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(mul_addr_infos_local_nonprimitive, mul_addr_infos ) {
            if (!cJSON_IsObject(mul_addr_infos_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_policy_update_context_data_parseFromJSON() failed [mul_addr_infos]");
                goto end;
            }
            OpenAPI_ip_multicast_address_info_t *mul_addr_infosItem = OpenAPI_ip_multicast_address_info_parseFromJSON(mul_addr_infos_local_nonprimitive);

            OpenAPI_list_add(mul_addr_infosList, mul_addr_infosItem);
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
        ue_time_zone ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        rel_ipv4_address ? ogs_strdup(rel_ipv4_address->valuestring) : NULL,
        ipv4_address ? ogs_strdup(ipv4_address->valuestring) : NULL,
        ip_domain ? ogs_strdup(ip_domain->valuestring) : NULL,
        ipv6_address_prefix ? ogs_strdup(ipv6_address_prefix->valuestring) : NULL,
        rel_ipv6_address_prefix ? ogs_strdup(rel_ipv6_address_prefix->valuestring) : NULL,
        add_ipv6_addr_prefixes ? ogs_strdup(add_ipv6_addr_prefixes->valuestring) : NULL,
        add_rel_ipv6_addr_prefixes ? ogs_strdup(add_rel_ipv6_addr_prefixes->valuestring) : NULL,
        rel_ue_mac ? ogs_strdup(rel_ue_mac->valuestring) : NULL,
        ue_mac ? ogs_strdup(ue_mac->valuestring) : NULL,
        subs_sess_ambr ? subs_sess_ambr_local_nonprim : NULL,
        auth_prof_index ? ogs_strdup(auth_prof_index->valuestring) : NULL,
        subs_def_qos ? subs_def_qos_local_nonprim : NULL,
        num_of_pack_filter ? num_of_pack_filter->valuedouble : 0,
        accu_usage_reports ? accu_usage_reportsList : NULL,
        _3gpp_ps_data_off_status ? _3gpp_ps_data_off_status->valueint : 0,
        app_detection_infos ? app_detection_infosList : NULL,
        rule_reports ? rule_reportsList : NULL,
        sess_rule_reports ? sess_rule_reportsList : NULL,
        qnc_reports ? qnc_reportsList : NULL,
        qos_mon_reports ? qos_mon_reportsList : NULL,
        user_location_info_time ? ogs_strdup(user_location_info_time->valuestring) : NULL,
        rep_pra_infos ? rep_pra_infosList : NULL,
        ue_init_res_req ? ue_init_res_req_local_nonprim : NULL,
        ref_qos_indication ? ref_qos_indication->valueint : 0,
        qos_flow_usage ? qos_flow_usage_local_nonprim : NULL,
        credit_manage_status ? credit_manage_status_local_nonprim : NULL,
        serv_nf_id ? serv_nf_id_local_nonprim : NULL,
        trace_req ? trace_req_local_nonprim : NULL,
        ma_pdu_ind ? ma_pdu_ind_local_nonprim : NULL,
        atsss_capab ? atsss_capab_local_nonprim : NULL,
        tsn_bridge_info ? tsn_bridge_info_local_nonprim : NULL,
        tsn_port_man_cont_dstt ? tsn_port_man_cont_dstt_local_nonprim : NULL,
        tsn_port_man_cont_nwtts ? tsn_port_man_cont_nwttsList : NULL,
        mul_addr_infos ? mul_addr_infosList : NULL
        );

    return sm_policy_update_context_data_local_var;
end:
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

