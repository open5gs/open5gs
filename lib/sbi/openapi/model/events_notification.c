
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "events_notification.h"

OpenAPI_events_notification_t *OpenAPI_events_notification_create(
    OpenAPI_list_t *ad_reports,
    OpenAPI_access_type_e access_type,
    OpenAPI_additional_access_info_t *add_access_info,
    OpenAPI_additional_access_info_t *rel_access_info,
    bool is_an_charg_addr_null,
    OpenAPI_acc_net_charging_address_t *an_charg_addr,
    OpenAPI_list_t *an_charg_ids,
    bool is_an_gw_addr_null,
    OpenAPI_an_gw_address_t *an_gw_addr,
    OpenAPI_list_t *l4s_reports,
    char *ev_subs_uri,
    OpenAPI_list_t *ev_notifs,
    OpenAPI_list_t *failed_resourc_alloc_reports,
    OpenAPI_list_t *succ_resourc_alloc_reports,
    OpenAPI_net_loc_access_support_e no_net_loc_supp,
    OpenAPI_list_t *out_of_cred_reports,
    OpenAPI_plmn_id_nid_t *plmn_id,
    OpenAPI_list_t *qnc_reports,
    OpenAPI_list_t *qos_mon_reports,
    OpenAPI_list_t *qos_mon_dat_rate_reps,
    OpenAPI_list_t *pdv_mon_reports,
    OpenAPI_list_t *congest_reports,
    OpenAPI_list_t *rtt_mon_reports,
    OpenAPI_list_t* qos_mon_cap_repos,
    OpenAPI_list_t *ran_nas_rel_causes,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_satellite_backhaul_category_e sat_backhaul_category,
    OpenAPI_user_location_t *ue_loc,
    char *ue_loc_time,
    char *ue_time_zone,
    OpenAPI_accumulated_usage_t *usg_rep,
    char *ursp_enf_rep,
    OpenAPI_ssc_mode_e ssc_mode,
    char *ue_req_dnn,
    OpenAPI_pdu_session_type_e ue_req_pdu_session_type,
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts,
    OpenAPI_list_t *ipv4_addr_list,
    OpenAPI_list_t *ipv6_prefix_list,
    OpenAPI_bat_offset_info_t *bat_offset_info,
    OpenAPI_ue_reachability_status_e ue_reach_status,
    bool is_retry_after,
    int retry_after,
    char *serv_sat_id,
    OpenAPI_rate_limit_repo_t *rate_limit_repo,
    OpenAPI_dnai_change_type_e dnai_chg_type,
    char *source_dnai,
    char *target_dnai,
    OpenAPI_pc_session_recovery_status_e session_recov_status
)
{
    OpenAPI_events_notification_t *events_notification_local_var = ogs_malloc(sizeof(OpenAPI_events_notification_t));
    ogs_assert(events_notification_local_var);

    events_notification_local_var->ad_reports = ad_reports;
    events_notification_local_var->access_type = access_type;
    events_notification_local_var->add_access_info = add_access_info;
    events_notification_local_var->rel_access_info = rel_access_info;
    events_notification_local_var->is_an_charg_addr_null = is_an_charg_addr_null;
    events_notification_local_var->an_charg_addr = an_charg_addr;
    events_notification_local_var->an_charg_ids = an_charg_ids;
    events_notification_local_var->is_an_gw_addr_null = is_an_gw_addr_null;
    events_notification_local_var->an_gw_addr = an_gw_addr;
    events_notification_local_var->l4s_reports = l4s_reports;
    events_notification_local_var->ev_subs_uri = ev_subs_uri;
    events_notification_local_var->ev_notifs = ev_notifs;
    events_notification_local_var->failed_resourc_alloc_reports = failed_resourc_alloc_reports;
    events_notification_local_var->succ_resourc_alloc_reports = succ_resourc_alloc_reports;
    events_notification_local_var->no_net_loc_supp = no_net_loc_supp;
    events_notification_local_var->out_of_cred_reports = out_of_cred_reports;
    events_notification_local_var->plmn_id = plmn_id;
    events_notification_local_var->qnc_reports = qnc_reports;
    events_notification_local_var->qos_mon_reports = qos_mon_reports;
    events_notification_local_var->qos_mon_dat_rate_reps = qos_mon_dat_rate_reps;
    events_notification_local_var->pdv_mon_reports = pdv_mon_reports;
    events_notification_local_var->congest_reports = congest_reports;
    events_notification_local_var->rtt_mon_reports = rtt_mon_reports;
    events_notification_local_var->qos_mon_cap_repos = qos_mon_cap_repos;
    events_notification_local_var->ran_nas_rel_causes = ran_nas_rel_causes;
    events_notification_local_var->rat_type = rat_type;
    events_notification_local_var->sat_backhaul_category = sat_backhaul_category;
    events_notification_local_var->ue_loc = ue_loc;
    events_notification_local_var->ue_loc_time = ue_loc_time;
    events_notification_local_var->ue_time_zone = ue_time_zone;
    events_notification_local_var->usg_rep = usg_rep;
    events_notification_local_var->ursp_enf_rep = ursp_enf_rep;
    events_notification_local_var->ssc_mode = ssc_mode;
    events_notification_local_var->ue_req_dnn = ue_req_dnn;
    events_notification_local_var->ue_req_pdu_session_type = ue_req_pdu_session_type;
    events_notification_local_var->tsn_bridge_man_cont = tsn_bridge_man_cont;
    events_notification_local_var->tsn_port_man_cont_dstt = tsn_port_man_cont_dstt;
    events_notification_local_var->tsn_port_man_cont_nwtts = tsn_port_man_cont_nwtts;
    events_notification_local_var->ipv4_addr_list = ipv4_addr_list;
    events_notification_local_var->ipv6_prefix_list = ipv6_prefix_list;
    events_notification_local_var->bat_offset_info = bat_offset_info;
    events_notification_local_var->ue_reach_status = ue_reach_status;
    events_notification_local_var->is_retry_after = is_retry_after;
    events_notification_local_var->retry_after = retry_after;
    events_notification_local_var->serv_sat_id = serv_sat_id;
    events_notification_local_var->rate_limit_repo = rate_limit_repo;
    events_notification_local_var->dnai_chg_type = dnai_chg_type;
    events_notification_local_var->source_dnai = source_dnai;
    events_notification_local_var->target_dnai = target_dnai;
    events_notification_local_var->session_recov_status = session_recov_status;

    return events_notification_local_var;
}

void OpenAPI_events_notification_free(OpenAPI_events_notification_t *events_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == events_notification) {
        return;
    }
    if (events_notification->ad_reports) {
        OpenAPI_list_for_each(events_notification->ad_reports, node) {
            OpenAPI_app_detection_report_free(node->data);
        }
        OpenAPI_list_free(events_notification->ad_reports);
        events_notification->ad_reports = NULL;
    }
    if (events_notification->add_access_info) {
        OpenAPI_additional_access_info_free(events_notification->add_access_info);
        events_notification->add_access_info = NULL;
    }
    if (events_notification->rel_access_info) {
        OpenAPI_additional_access_info_free(events_notification->rel_access_info);
        events_notification->rel_access_info = NULL;
    }
    if (events_notification->an_charg_addr) {
        OpenAPI_acc_net_charging_address_free(events_notification->an_charg_addr);
        events_notification->an_charg_addr = NULL;
    }
    if (events_notification->an_charg_ids) {
        OpenAPI_list_for_each(events_notification->an_charg_ids, node) {
            OpenAPI_access_net_charging_identifier_free(node->data);
        }
        OpenAPI_list_free(events_notification->an_charg_ids);
        events_notification->an_charg_ids = NULL;
    }
    if (events_notification->an_gw_addr) {
        OpenAPI_an_gw_address_free(events_notification->an_gw_addr);
        events_notification->an_gw_addr = NULL;
    }
    if (events_notification->l4s_reports) {
        OpenAPI_list_for_each(events_notification->l4s_reports, node) {
            OpenAPI_l4s_support_free(node->data);
        }
        OpenAPI_list_free(events_notification->l4s_reports);
        events_notification->l4s_reports = NULL;
    }
    if (events_notification->ev_subs_uri) {
        ogs_free(events_notification->ev_subs_uri);
        events_notification->ev_subs_uri = NULL;
    }
    if (events_notification->ev_notifs) {
        OpenAPI_list_for_each(events_notification->ev_notifs, node) {
            OpenAPI_af_event_notification_free(node->data);
        }
        OpenAPI_list_free(events_notification->ev_notifs);
        events_notification->ev_notifs = NULL;
    }
    if (events_notification->failed_resourc_alloc_reports) {
        OpenAPI_list_for_each(events_notification->failed_resourc_alloc_reports, node) {
            OpenAPI_resources_allocation_info_free(node->data);
        }
        OpenAPI_list_free(events_notification->failed_resourc_alloc_reports);
        events_notification->failed_resourc_alloc_reports = NULL;
    }
    if (events_notification->succ_resourc_alloc_reports) {
        OpenAPI_list_for_each(events_notification->succ_resourc_alloc_reports, node) {
            OpenAPI_resources_allocation_info_free(node->data);
        }
        OpenAPI_list_free(events_notification->succ_resourc_alloc_reports);
        events_notification->succ_resourc_alloc_reports = NULL;
    }
    if (events_notification->out_of_cred_reports) {
        OpenAPI_list_for_each(events_notification->out_of_cred_reports, node) {
            OpenAPI_out_of_credit_information_free(node->data);
        }
        OpenAPI_list_free(events_notification->out_of_cred_reports);
        events_notification->out_of_cred_reports = NULL;
    }
    if (events_notification->plmn_id) {
        OpenAPI_plmn_id_nid_free(events_notification->plmn_id);
        events_notification->plmn_id = NULL;
    }
    if (events_notification->qnc_reports) {
        OpenAPI_list_for_each(events_notification->qnc_reports, node) {
            OpenAPI_qos_notification_control_info_free(node->data);
        }
        OpenAPI_list_free(events_notification->qnc_reports);
        events_notification->qnc_reports = NULL;
    }
    if (events_notification->qos_mon_reports) {
        OpenAPI_list_for_each(events_notification->qos_mon_reports, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(events_notification->qos_mon_reports);
        events_notification->qos_mon_reports = NULL;
    }
    if (events_notification->qos_mon_dat_rate_reps) {
        OpenAPI_list_for_each(events_notification->qos_mon_dat_rate_reps, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(events_notification->qos_mon_dat_rate_reps);
        events_notification->qos_mon_dat_rate_reps = NULL;
    }
    if (events_notification->pdv_mon_reports) {
        OpenAPI_list_for_each(events_notification->pdv_mon_reports, node) {
            OpenAPI_pdv_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(events_notification->pdv_mon_reports);
        events_notification->pdv_mon_reports = NULL;
    }
    if (events_notification->congest_reports) {
        OpenAPI_list_for_each(events_notification->congest_reports, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(events_notification->congest_reports);
        events_notification->congest_reports = NULL;
    }
    if (events_notification->rtt_mon_reports) {
        OpenAPI_list_for_each(events_notification->rtt_mon_reports, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(events_notification->rtt_mon_reports);
        events_notification->rtt_mon_reports = NULL;
    }
    if (events_notification->qos_mon_cap_repos) {
        OpenAPI_list_for_each(events_notification->qos_mon_cap_repos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_capability_report_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(events_notification->qos_mon_cap_repos);
        events_notification->qos_mon_cap_repos = NULL;
    }
    if (events_notification->ran_nas_rel_causes) {
        OpenAPI_list_for_each(events_notification->ran_nas_rel_causes, node) {
            OpenAPI_ran_nas_rel_cause_free(node->data);
        }
        OpenAPI_list_free(events_notification->ran_nas_rel_causes);
        events_notification->ran_nas_rel_causes = NULL;
    }
    if (events_notification->ue_loc) {
        OpenAPI_user_location_free(events_notification->ue_loc);
        events_notification->ue_loc = NULL;
    }
    if (events_notification->ue_loc_time) {
        ogs_free(events_notification->ue_loc_time);
        events_notification->ue_loc_time = NULL;
    }
    if (events_notification->ue_time_zone) {
        ogs_free(events_notification->ue_time_zone);
        events_notification->ue_time_zone = NULL;
    }
    if (events_notification->usg_rep) {
        OpenAPI_accumulated_usage_free(events_notification->usg_rep);
        events_notification->usg_rep = NULL;
    }
    if (events_notification->ursp_enf_rep) {
        ogs_free(events_notification->ursp_enf_rep);
        events_notification->ursp_enf_rep = NULL;
    }
    if (events_notification->ue_req_dnn) {
        ogs_free(events_notification->ue_req_dnn);
        events_notification->ue_req_dnn = NULL;
    }
    if (events_notification->tsn_bridge_man_cont) {
        OpenAPI_bridge_management_container_free(events_notification->tsn_bridge_man_cont);
        events_notification->tsn_bridge_man_cont = NULL;
    }
    if (events_notification->tsn_port_man_cont_dstt) {
        OpenAPI_port_management_container_free(events_notification->tsn_port_man_cont_dstt);
        events_notification->tsn_port_man_cont_dstt = NULL;
    }
    if (events_notification->tsn_port_man_cont_nwtts) {
        OpenAPI_list_for_each(events_notification->tsn_port_man_cont_nwtts, node) {
            OpenAPI_port_management_container_free(node->data);
        }
        OpenAPI_list_free(events_notification->tsn_port_man_cont_nwtts);
        events_notification->tsn_port_man_cont_nwtts = NULL;
    }
    if (events_notification->ipv4_addr_list) {
        OpenAPI_list_for_each(events_notification->ipv4_addr_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(events_notification->ipv4_addr_list);
        events_notification->ipv4_addr_list = NULL;
    }
    if (events_notification->ipv6_prefix_list) {
        OpenAPI_list_for_each(events_notification->ipv6_prefix_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(events_notification->ipv6_prefix_list);
        events_notification->ipv6_prefix_list = NULL;
    }
    if (events_notification->bat_offset_info) {
        OpenAPI_bat_offset_info_free(events_notification->bat_offset_info);
        events_notification->bat_offset_info = NULL;
    }
    if (events_notification->serv_sat_id) {
        ogs_free(events_notification->serv_sat_id);
        events_notification->serv_sat_id = NULL;
    }
    if (events_notification->rate_limit_repo) {
        OpenAPI_rate_limit_repo_free(events_notification->rate_limit_repo);
        events_notification->rate_limit_repo = NULL;
    }
    if (events_notification->source_dnai) {
        ogs_free(events_notification->source_dnai);
        events_notification->source_dnai = NULL;
    }
    if (events_notification->target_dnai) {
        ogs_free(events_notification->target_dnai);
        events_notification->target_dnai = NULL;
    }
    ogs_free(events_notification);
}

cJSON *OpenAPI_events_notification_convertToJSON(OpenAPI_events_notification_t *events_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (events_notification == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [EventsNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (events_notification->ad_reports) {
    cJSON *ad_reportsList = cJSON_AddArrayToObject(item, "adReports");
    if (ad_reportsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ad_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->ad_reports, node) {
        cJSON *itemLocal = OpenAPI_app_detection_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [ad_reports]");
            goto end;
        }
        cJSON_AddItemToArray(ad_reportsList, itemLocal);
    }
    }

    if (events_notification->access_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(events_notification->access_type)) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [access_type]");
        goto end;
    }
    }

    if (events_notification->add_access_info) {
    cJSON *add_access_info_local_JSON = OpenAPI_additional_access_info_convertToJSON(events_notification->add_access_info);
    if (add_access_info_local_JSON == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [add_access_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "addAccessInfo", add_access_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [add_access_info]");
        goto end;
    }
    }

    if (events_notification->rel_access_info) {
    cJSON *rel_access_info_local_JSON = OpenAPI_additional_access_info_convertToJSON(events_notification->rel_access_info);
    if (rel_access_info_local_JSON == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [rel_access_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "relAccessInfo", rel_access_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [rel_access_info]");
        goto end;
    }
    }

    if (events_notification->an_charg_addr) {
    cJSON *an_charg_addr_local_JSON = OpenAPI_acc_net_charging_address_convertToJSON(events_notification->an_charg_addr);
    if (an_charg_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [an_charg_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "anChargAddr", an_charg_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [an_charg_addr]");
        goto end;
    }
    } else if (events_notification->is_an_charg_addr_null) {
        if (cJSON_AddNullToObject(item, "anChargAddr") == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [an_charg_addr]");
            goto end;
        }
    }

    if (events_notification->an_charg_ids) {
    cJSON *an_charg_idsList = cJSON_AddArrayToObject(item, "anChargIds");
    if (an_charg_idsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [an_charg_ids]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->an_charg_ids, node) {
        cJSON *itemLocal = OpenAPI_access_net_charging_identifier_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [an_charg_ids]");
            goto end;
        }
        cJSON_AddItemToArray(an_charg_idsList, itemLocal);
    }
    }

    if (events_notification->an_gw_addr) {
    cJSON *an_gw_addr_local_JSON = OpenAPI_an_gw_address_convertToJSON(events_notification->an_gw_addr);
    if (an_gw_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [an_gw_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "anGwAddr", an_gw_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [an_gw_addr]");
        goto end;
    }
    } else if (events_notification->is_an_gw_addr_null) {
        if (cJSON_AddNullToObject(item, "anGwAddr") == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [an_gw_addr]");
            goto end;
        }
    }

    if (events_notification->l4s_reports) {
    cJSON *l4s_reportsList = cJSON_AddArrayToObject(item, "l4sReports");
    if (l4s_reportsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [l4s_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->l4s_reports, node) {
        cJSON *itemLocal = OpenAPI_l4s_support_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [l4s_reports]");
            goto end;
        }
        cJSON_AddItemToArray(l4s_reportsList, itemLocal);
    }
    }

    if (!events_notification->ev_subs_uri) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ev_subs_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "evSubsUri", events_notification->ev_subs_uri) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ev_subs_uri]");
        goto end;
    }

    if (!events_notification->ev_notifs) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ev_notifs]");
        return NULL;
    }
    cJSON *ev_notifsList = cJSON_AddArrayToObject(item, "evNotifs");
    if (ev_notifsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ev_notifs]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->ev_notifs, node) {
        cJSON *itemLocal = OpenAPI_af_event_notification_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [ev_notifs]");
            goto end;
        }
        cJSON_AddItemToArray(ev_notifsList, itemLocal);
    }

    if (events_notification->failed_resourc_alloc_reports) {
    cJSON *failed_resourc_alloc_reportsList = cJSON_AddArrayToObject(item, "failedResourcAllocReports");
    if (failed_resourc_alloc_reportsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [failed_resourc_alloc_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->failed_resourc_alloc_reports, node) {
        cJSON *itemLocal = OpenAPI_resources_allocation_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [failed_resourc_alloc_reports]");
            goto end;
        }
        cJSON_AddItemToArray(failed_resourc_alloc_reportsList, itemLocal);
    }
    }

    if (events_notification->succ_resourc_alloc_reports) {
    cJSON *succ_resourc_alloc_reportsList = cJSON_AddArrayToObject(item, "succResourcAllocReports");
    if (succ_resourc_alloc_reportsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [succ_resourc_alloc_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->succ_resourc_alloc_reports, node) {
        cJSON *itemLocal = OpenAPI_resources_allocation_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [succ_resourc_alloc_reports]");
            goto end;
        }
        cJSON_AddItemToArray(succ_resourc_alloc_reportsList, itemLocal);
    }
    }

    if (events_notification->no_net_loc_supp != OpenAPI_net_loc_access_support_NULL) {
    if (cJSON_AddStringToObject(item, "noNetLocSupp", OpenAPI_net_loc_access_support_ToString(events_notification->no_net_loc_supp)) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [no_net_loc_supp]");
        goto end;
    }
    }

    if (events_notification->out_of_cred_reports) {
    cJSON *out_of_cred_reportsList = cJSON_AddArrayToObject(item, "outOfCredReports");
    if (out_of_cred_reportsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [out_of_cred_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->out_of_cred_reports, node) {
        cJSON *itemLocal = OpenAPI_out_of_credit_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [out_of_cred_reports]");
            goto end;
        }
        cJSON_AddItemToArray(out_of_cred_reportsList, itemLocal);
    }
    }

    if (events_notification->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(events_notification->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (events_notification->qnc_reports) {
    cJSON *qnc_reportsList = cJSON_AddArrayToObject(item, "qncReports");
    if (qnc_reportsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [qnc_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->qnc_reports, node) {
        cJSON *itemLocal = OpenAPI_qos_notification_control_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [qnc_reports]");
            goto end;
        }
        cJSON_AddItemToArray(qnc_reportsList, itemLocal);
    }
    }

    if (events_notification->qos_mon_reports) {
    cJSON *qos_mon_reportsList = cJSON_AddArrayToObject(item, "qosMonReports");
    if (qos_mon_reportsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [qos_mon_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->qos_mon_reports, node) {
        cJSON *itemLocal = OpenAPI_qos_monitoring_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [qos_mon_reports]");
            goto end;
        }
        cJSON_AddItemToArray(qos_mon_reportsList, itemLocal);
    }
    }

    if (events_notification->qos_mon_dat_rate_reps) {
    cJSON *qos_mon_dat_rate_repsList = cJSON_AddArrayToObject(item, "qosMonDatRateReps");
    if (qos_mon_dat_rate_repsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [qos_mon_dat_rate_reps]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->qos_mon_dat_rate_reps, node) {
        cJSON *itemLocal = OpenAPI_qos_monitoring_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [qos_mon_dat_rate_reps]");
            goto end;
        }
        cJSON_AddItemToArray(qos_mon_dat_rate_repsList, itemLocal);
    }
    }

    if (events_notification->pdv_mon_reports) {
    cJSON *pdv_mon_reportsList = cJSON_AddArrayToObject(item, "pdvMonReports");
    if (pdv_mon_reportsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [pdv_mon_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->pdv_mon_reports, node) {
        cJSON *itemLocal = OpenAPI_pdv_monitoring_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [pdv_mon_reports]");
            goto end;
        }
        cJSON_AddItemToArray(pdv_mon_reportsList, itemLocal);
    }
    }

    if (events_notification->congest_reports) {
    cJSON *congest_reportsList = cJSON_AddArrayToObject(item, "congestReports");
    if (congest_reportsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [congest_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->congest_reports, node) {
        cJSON *itemLocal = OpenAPI_qos_monitoring_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [congest_reports]");
            goto end;
        }
        cJSON_AddItemToArray(congest_reportsList, itemLocal);
    }
    }

    if (events_notification->rtt_mon_reports) {
    cJSON *rtt_mon_reportsList = cJSON_AddArrayToObject(item, "rttMonReports");
    if (rtt_mon_reportsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [rtt_mon_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->rtt_mon_reports, node) {
        cJSON *itemLocal = OpenAPI_qos_monitoring_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [rtt_mon_reports]");
            goto end;
        }
        cJSON_AddItemToArray(rtt_mon_reportsList, itemLocal);
    }
    }

    if (events_notification->qos_mon_cap_repos) {
    cJSON *qos_mon_cap_repos = cJSON_AddObjectToObject(item, "qosMonCapRepos");
    if (qos_mon_cap_repos == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [qos_mon_cap_repos]");
        goto end;
    }
    cJSON *localMapObject = qos_mon_cap_repos;
    if (events_notification->qos_mon_cap_repos) {
        OpenAPI_list_for_each(events_notification->qos_mon_cap_repos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_events_notification_convertToJSON() failed [qos_mon_cap_repos]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_events_notification_convertToJSON() failed [qos_mon_cap_repos]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_capability_report_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_events_notification_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (events_notification->ran_nas_rel_causes) {
    cJSON *ran_nas_rel_causesList = cJSON_AddArrayToObject(item, "ranNasRelCauses");
    if (ran_nas_rel_causesList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ran_nas_rel_causes]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->ran_nas_rel_causes, node) {
        cJSON *itemLocal = OpenAPI_ran_nas_rel_cause_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [ran_nas_rel_causes]");
            goto end;
        }
        cJSON_AddItemToArray(ran_nas_rel_causesList, itemLocal);
    }
    }

    if (events_notification->rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(events_notification->rat_type)) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (events_notification->sat_backhaul_category != OpenAPI_satellite_backhaul_category_NULL) {
    if (cJSON_AddStringToObject(item, "satBackhaulCategory", OpenAPI_satellite_backhaul_category_ToString(events_notification->sat_backhaul_category)) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [sat_backhaul_category]");
        goto end;
    }
    }

    if (events_notification->ue_loc) {
    cJSON *ue_loc_local_JSON = OpenAPI_user_location_convertToJSON(events_notification->ue_loc);
    if (ue_loc_local_JSON == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ue_loc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLoc", ue_loc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ue_loc]");
        goto end;
    }
    }

    if (events_notification->ue_loc_time) {
    if (cJSON_AddStringToObject(item, "ueLocTime", events_notification->ue_loc_time) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ue_loc_time]");
        goto end;
    }
    }

    if (events_notification->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", events_notification->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (events_notification->usg_rep) {
    cJSON *usg_rep_local_JSON = OpenAPI_accumulated_usage_convertToJSON(events_notification->usg_rep);
    if (usg_rep_local_JSON == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [usg_rep]");
        goto end;
    }
    cJSON_AddItemToObject(item, "usgRep", usg_rep_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [usg_rep]");
        goto end;
    }
    }

    if (events_notification->ursp_enf_rep) {
    if (cJSON_AddStringToObject(item, "urspEnfRep", events_notification->ursp_enf_rep) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ursp_enf_rep]");
        goto end;
    }
    }

    if (events_notification->ssc_mode != OpenAPI_ssc_mode_NULL) {
    if (cJSON_AddStringToObject(item, "sscMode", OpenAPI_ssc_mode_ToString(events_notification->ssc_mode)) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ssc_mode]");
        goto end;
    }
    }

    if (events_notification->ue_req_dnn) {
    if (cJSON_AddStringToObject(item, "ueReqDnn", events_notification->ue_req_dnn) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ue_req_dnn]");
        goto end;
    }
    }

    if (events_notification->ue_req_pdu_session_type != OpenAPI_pdu_session_type_NULL) {
    if (cJSON_AddStringToObject(item, "ueReqPduSessionType", OpenAPI_pdu_session_type_ToString(events_notification->ue_req_pdu_session_type)) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ue_req_pdu_session_type]");
        goto end;
    }
    }

    if (events_notification->tsn_bridge_man_cont) {
    cJSON *tsn_bridge_man_cont_local_JSON = OpenAPI_bridge_management_container_convertToJSON(events_notification->tsn_bridge_man_cont);
    if (tsn_bridge_man_cont_local_JSON == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnBridgeManCont", tsn_bridge_man_cont_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    }

    if (events_notification->tsn_port_man_cont_dstt) {
    cJSON *tsn_port_man_cont_dstt_local_JSON = OpenAPI_port_management_container_convertToJSON(events_notification->tsn_port_man_cont_dstt);
    if (tsn_port_man_cont_dstt_local_JSON == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnPortManContDstt", tsn_port_man_cont_dstt_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    }

    if (events_notification->tsn_port_man_cont_nwtts) {
    cJSON *tsn_port_man_cont_nwttsList = cJSON_AddArrayToObject(item, "tsnPortManContNwtts");
    if (tsn_port_man_cont_nwttsList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [tsn_port_man_cont_nwtts]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->tsn_port_man_cont_nwtts, node) {
        cJSON *itemLocal = OpenAPI_port_management_container_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }
        cJSON_AddItemToArray(tsn_port_man_cont_nwttsList, itemLocal);
    }
    }

    if (events_notification->ipv4_addr_list) {
    cJSON *ipv4_addr_listList = cJSON_AddArrayToObject(item, "ipv4AddrList");
    if (ipv4_addr_listList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ipv4_addr_list]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->ipv4_addr_list, node) {
        if (cJSON_AddStringToObject(ipv4_addr_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [ipv4_addr_list]");
            goto end;
        }
    }
    }

    if (events_notification->ipv6_prefix_list) {
    cJSON *ipv6_prefix_listList = cJSON_AddArrayToObject(item, "ipv6PrefixList");
    if (ipv6_prefix_listList == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ipv6_prefix_list]");
        goto end;
    }
    OpenAPI_list_for_each(events_notification->ipv6_prefix_list, node) {
        if (cJSON_AddStringToObject(ipv6_prefix_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_events_notification_convertToJSON() failed [ipv6_prefix_list]");
            goto end;
        }
    }
    }

    if (events_notification->bat_offset_info) {
    cJSON *bat_offset_info_local_JSON = OpenAPI_bat_offset_info_convertToJSON(events_notification->bat_offset_info);
    if (bat_offset_info_local_JSON == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [bat_offset_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "batOffsetInfo", bat_offset_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [bat_offset_info]");
        goto end;
    }
    }

    if (events_notification->ue_reach_status != OpenAPI_ue_reachability_status_NULL) {
    if (cJSON_AddStringToObject(item, "ueReachStatus", OpenAPI_ue_reachability_status_ToString(events_notification->ue_reach_status)) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [ue_reach_status]");
        goto end;
    }
    }

    if (events_notification->is_retry_after) {
    if (cJSON_AddNumberToObject(item, "retryAfter", events_notification->retry_after) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [retry_after]");
        goto end;
    }
    }

    if (events_notification->serv_sat_id) {
    if (cJSON_AddStringToObject(item, "servSatId", events_notification->serv_sat_id) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [serv_sat_id]");
        goto end;
    }
    }

    if (events_notification->rate_limit_repo) {
    cJSON *rate_limit_repo_local_JSON = OpenAPI_rate_limit_repo_convertToJSON(events_notification->rate_limit_repo);
    if (rate_limit_repo_local_JSON == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [rate_limit_repo]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rateLimitRepo", rate_limit_repo_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [rate_limit_repo]");
        goto end;
    }
    }

    if (events_notification->dnai_chg_type != OpenAPI_dnai_change_type_NULL) {
    if (cJSON_AddStringToObject(item, "dnaiChgType", OpenAPI_dnai_change_type_ToString(events_notification->dnai_chg_type)) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [dnai_chg_type]");
        goto end;
    }
    }

    if (events_notification->source_dnai) {
    if (cJSON_AddStringToObject(item, "sourceDnai", events_notification->source_dnai) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [source_dnai]");
        goto end;
    }
    }

    if (events_notification->target_dnai) {
    if (cJSON_AddStringToObject(item, "targetDnai", events_notification->target_dnai) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [target_dnai]");
        goto end;
    }
    }

    if (events_notification->session_recov_status != OpenAPI_pc_session_recovery_status_NULL) {
    if (cJSON_AddStringToObject(item, "sessionRecovStatus", OpenAPI_pc_session_recovery_status_ToString(events_notification->session_recov_status)) == NULL) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed [session_recov_status]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_events_notification_t *OpenAPI_events_notification_parseFromJSON(cJSON *events_notificationJSON)
{
    OpenAPI_events_notification_t *events_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ad_reports = NULL;
    OpenAPI_list_t *ad_reportsList = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    cJSON *add_access_info = NULL;
    OpenAPI_additional_access_info_t *add_access_info_local_nonprim = NULL;
    cJSON *rel_access_info = NULL;
    OpenAPI_additional_access_info_t *rel_access_info_local_nonprim = NULL;
    cJSON *an_charg_addr = NULL;
    OpenAPI_acc_net_charging_address_t *an_charg_addr_local_nonprim = NULL;
    cJSON *an_charg_ids = NULL;
    OpenAPI_list_t *an_charg_idsList = NULL;
    cJSON *an_gw_addr = NULL;
    OpenAPI_an_gw_address_t *an_gw_addr_local_nonprim = NULL;
    cJSON *l4s_reports = NULL;
    OpenAPI_list_t *l4s_reportsList = NULL;
    cJSON *ev_subs_uri = NULL;
    cJSON *ev_notifs = NULL;
    OpenAPI_list_t *ev_notifsList = NULL;
    cJSON *failed_resourc_alloc_reports = NULL;
    OpenAPI_list_t *failed_resourc_alloc_reportsList = NULL;
    cJSON *succ_resourc_alloc_reports = NULL;
    OpenAPI_list_t *succ_resourc_alloc_reportsList = NULL;
    cJSON *no_net_loc_supp = NULL;
    OpenAPI_net_loc_access_support_e no_net_loc_suppVariable = 0;
    cJSON *out_of_cred_reports = NULL;
    OpenAPI_list_t *out_of_cred_reportsList = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_nid_t *plmn_id_local_nonprim = NULL;
    cJSON *qnc_reports = NULL;
    OpenAPI_list_t *qnc_reportsList = NULL;
    cJSON *qos_mon_reports = NULL;
    OpenAPI_list_t *qos_mon_reportsList = NULL;
    cJSON *qos_mon_dat_rate_reps = NULL;
    OpenAPI_list_t *qos_mon_dat_rate_repsList = NULL;
    cJSON *pdv_mon_reports = NULL;
    OpenAPI_list_t *pdv_mon_reportsList = NULL;
    cJSON *congest_reports = NULL;
    OpenAPI_list_t *congest_reportsList = NULL;
    cJSON *rtt_mon_reports = NULL;
    OpenAPI_list_t *rtt_mon_reportsList = NULL;
    cJSON *qos_mon_cap_repos = NULL;
    OpenAPI_list_t *qos_mon_cap_reposList = NULL;
    cJSON *ran_nas_rel_causes = NULL;
    OpenAPI_list_t *ran_nas_rel_causesList = NULL;
    cJSON *rat_type = NULL;
    OpenAPI_rat_type_e rat_typeVariable = 0;
    cJSON *sat_backhaul_category = NULL;
    OpenAPI_satellite_backhaul_category_e sat_backhaul_categoryVariable = 0;
    cJSON *ue_loc = NULL;
    OpenAPI_user_location_t *ue_loc_local_nonprim = NULL;
    cJSON *ue_loc_time = NULL;
    cJSON *ue_time_zone = NULL;
    cJSON *usg_rep = NULL;
    OpenAPI_accumulated_usage_t *usg_rep_local_nonprim = NULL;
    cJSON *ursp_enf_rep = NULL;
    cJSON *ssc_mode = NULL;
    OpenAPI_ssc_mode_e ssc_modeVariable = 0;
    cJSON *ue_req_dnn = NULL;
    cJSON *ue_req_pdu_session_type = NULL;
    OpenAPI_pdu_session_type_e ue_req_pdu_session_typeVariable = 0;
    cJSON *tsn_bridge_man_cont = NULL;
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont_local_nonprim = NULL;
    cJSON *tsn_port_man_cont_dstt = NULL;
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt_local_nonprim = NULL;
    cJSON *tsn_port_man_cont_nwtts = NULL;
    OpenAPI_list_t *tsn_port_man_cont_nwttsList = NULL;
    cJSON *ipv4_addr_list = NULL;
    OpenAPI_list_t *ipv4_addr_listList = NULL;
    cJSON *ipv6_prefix_list = NULL;
    OpenAPI_list_t *ipv6_prefix_listList = NULL;
    cJSON *bat_offset_info = NULL;
    OpenAPI_bat_offset_info_t *bat_offset_info_local_nonprim = NULL;
    cJSON *ue_reach_status = NULL;
    OpenAPI_ue_reachability_status_e ue_reach_statusVariable = 0;
    cJSON *retry_after = NULL;
    cJSON *serv_sat_id = NULL;
    cJSON *rate_limit_repo = NULL;
    OpenAPI_rate_limit_repo_t *rate_limit_repo_local_nonprim = NULL;
    cJSON *dnai_chg_type = NULL;
    OpenAPI_dnai_change_type_e dnai_chg_typeVariable = 0;
    cJSON *source_dnai = NULL;
    cJSON *target_dnai = NULL;
    cJSON *session_recov_status = NULL;
    OpenAPI_pc_session_recovery_status_e session_recov_statusVariable = 0;
    ad_reports = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "adReports");
    if (ad_reports) {
        cJSON *ad_reports_local = NULL;
        if (!cJSON_IsArray(ad_reports)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ad_reports]");
            goto end;
        }

        ad_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ad_reports_local, ad_reports) {
            if (!cJSON_IsObject(ad_reports_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ad_reports]");
                goto end;
            }
            OpenAPI_app_detection_report_t *ad_reportsItem = OpenAPI_app_detection_report_parseFromJSON(ad_reports_local);
            if (!ad_reportsItem) {
                ogs_error("No ad_reportsItem");
                goto end;
            }
            OpenAPI_list_add(ad_reportsList, ad_reportsItem);
        }
    }

    access_type = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "accessType");
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    add_access_info = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "addAccessInfo");
    if (add_access_info) {
    add_access_info_local_nonprim = OpenAPI_additional_access_info_parseFromJSON(add_access_info);
    if (!add_access_info_local_nonprim) {
        ogs_error("OpenAPI_additional_access_info_parseFromJSON failed [add_access_info]");
        goto end;
    }
    }

    rel_access_info = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "relAccessInfo");
    if (rel_access_info) {
    rel_access_info_local_nonprim = OpenAPI_additional_access_info_parseFromJSON(rel_access_info);
    if (!rel_access_info_local_nonprim) {
        ogs_error("OpenAPI_additional_access_info_parseFromJSON failed [rel_access_info]");
        goto end;
    }
    }

    an_charg_addr = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "anChargAddr");
    if (an_charg_addr) {
    if (!cJSON_IsNull(an_charg_addr)) {
    an_charg_addr_local_nonprim = OpenAPI_acc_net_charging_address_parseFromJSON(an_charg_addr);
    if (!an_charg_addr_local_nonprim) {
        ogs_error("OpenAPI_acc_net_charging_address_parseFromJSON failed [an_charg_addr]");
        goto end;
    }
    }
    }

    an_charg_ids = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "anChargIds");
    if (an_charg_ids) {
        cJSON *an_charg_ids_local = NULL;
        if (!cJSON_IsArray(an_charg_ids)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [an_charg_ids]");
            goto end;
        }

        an_charg_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(an_charg_ids_local, an_charg_ids) {
            if (!cJSON_IsObject(an_charg_ids_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [an_charg_ids]");
                goto end;
            }
            OpenAPI_access_net_charging_identifier_t *an_charg_idsItem = OpenAPI_access_net_charging_identifier_parseFromJSON(an_charg_ids_local);
            if (!an_charg_idsItem) {
                ogs_error("No an_charg_idsItem");
                goto end;
            }
            OpenAPI_list_add(an_charg_idsList, an_charg_idsItem);
        }
    }

    an_gw_addr = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "anGwAddr");
    if (an_gw_addr) {
    if (!cJSON_IsNull(an_gw_addr)) {
    an_gw_addr_local_nonprim = OpenAPI_an_gw_address_parseFromJSON(an_gw_addr);
    if (!an_gw_addr_local_nonprim) {
        ogs_error("OpenAPI_an_gw_address_parseFromJSON failed [an_gw_addr]");
        goto end;
    }
    }
    }

    l4s_reports = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "l4sReports");
    if (l4s_reports) {
        cJSON *l4s_reports_local = NULL;
        if (!cJSON_IsArray(l4s_reports)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [l4s_reports]");
            goto end;
        }

        l4s_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(l4s_reports_local, l4s_reports) {
            if (!cJSON_IsObject(l4s_reports_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [l4s_reports]");
                goto end;
            }
            OpenAPI_l4s_support_t *l4s_reportsItem = OpenAPI_l4s_support_parseFromJSON(l4s_reports_local);
            if (!l4s_reportsItem) {
                ogs_error("No l4s_reportsItem");
                goto end;
            }
            OpenAPI_list_add(l4s_reportsList, l4s_reportsItem);
        }
    }

    ev_subs_uri = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "evSubsUri");
    if (!ev_subs_uri) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ev_subs_uri]");
        goto end;
    }
    if (!cJSON_IsString(ev_subs_uri)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ev_subs_uri]");
        goto end;
    }

    ev_notifs = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "evNotifs");
    if (!ev_notifs) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ev_notifs]");
        goto end;
    }
        cJSON *ev_notifs_local = NULL;
        if (!cJSON_IsArray(ev_notifs)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ev_notifs]");
            goto end;
        }

        ev_notifsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ev_notifs_local, ev_notifs) {
            if (!cJSON_IsObject(ev_notifs_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ev_notifs]");
                goto end;
            }
            OpenAPI_af_event_notification_t *ev_notifsItem = OpenAPI_af_event_notification_parseFromJSON(ev_notifs_local);
            if (!ev_notifsItem) {
                ogs_error("No ev_notifsItem");
                goto end;
            }
            OpenAPI_list_add(ev_notifsList, ev_notifsItem);
        }

    failed_resourc_alloc_reports = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "failedResourcAllocReports");
    if (failed_resourc_alloc_reports) {
        cJSON *failed_resourc_alloc_reports_local = NULL;
        if (!cJSON_IsArray(failed_resourc_alloc_reports)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [failed_resourc_alloc_reports]");
            goto end;
        }

        failed_resourc_alloc_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(failed_resourc_alloc_reports_local, failed_resourc_alloc_reports) {
            if (!cJSON_IsObject(failed_resourc_alloc_reports_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [failed_resourc_alloc_reports]");
                goto end;
            }
            OpenAPI_resources_allocation_info_t *failed_resourc_alloc_reportsItem = OpenAPI_resources_allocation_info_parseFromJSON(failed_resourc_alloc_reports_local);
            if (!failed_resourc_alloc_reportsItem) {
                ogs_error("No failed_resourc_alloc_reportsItem");
                goto end;
            }
            OpenAPI_list_add(failed_resourc_alloc_reportsList, failed_resourc_alloc_reportsItem);
        }
    }

    succ_resourc_alloc_reports = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "succResourcAllocReports");
    if (succ_resourc_alloc_reports) {
        cJSON *succ_resourc_alloc_reports_local = NULL;
        if (!cJSON_IsArray(succ_resourc_alloc_reports)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [succ_resourc_alloc_reports]");
            goto end;
        }

        succ_resourc_alloc_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(succ_resourc_alloc_reports_local, succ_resourc_alloc_reports) {
            if (!cJSON_IsObject(succ_resourc_alloc_reports_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [succ_resourc_alloc_reports]");
                goto end;
            }
            OpenAPI_resources_allocation_info_t *succ_resourc_alloc_reportsItem = OpenAPI_resources_allocation_info_parseFromJSON(succ_resourc_alloc_reports_local);
            if (!succ_resourc_alloc_reportsItem) {
                ogs_error("No succ_resourc_alloc_reportsItem");
                goto end;
            }
            OpenAPI_list_add(succ_resourc_alloc_reportsList, succ_resourc_alloc_reportsItem);
        }
    }

    no_net_loc_supp = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "noNetLocSupp");
    if (no_net_loc_supp) {
    if (!cJSON_IsString(no_net_loc_supp)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [no_net_loc_supp]");
        goto end;
    }
    no_net_loc_suppVariable = OpenAPI_net_loc_access_support_FromString(no_net_loc_supp->valuestring);
    }

    out_of_cred_reports = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "outOfCredReports");
    if (out_of_cred_reports) {
        cJSON *out_of_cred_reports_local = NULL;
        if (!cJSON_IsArray(out_of_cred_reports)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [out_of_cred_reports]");
            goto end;
        }

        out_of_cred_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(out_of_cred_reports_local, out_of_cred_reports) {
            if (!cJSON_IsObject(out_of_cred_reports_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [out_of_cred_reports]");
                goto end;
            }
            OpenAPI_out_of_credit_information_t *out_of_cred_reportsItem = OpenAPI_out_of_credit_information_parseFromJSON(out_of_cred_reports_local);
            if (!out_of_cred_reportsItem) {
                ogs_error("No out_of_cred_reportsItem");
                goto end;
            }
            OpenAPI_list_add(out_of_cred_reportsList, out_of_cred_reportsItem);
        }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    qnc_reports = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "qncReports");
    if (qnc_reports) {
        cJSON *qnc_reports_local = NULL;
        if (!cJSON_IsArray(qnc_reports)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [qnc_reports]");
            goto end;
        }

        qnc_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qnc_reports_local, qnc_reports) {
            if (!cJSON_IsObject(qnc_reports_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [qnc_reports]");
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

    qos_mon_reports = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "qosMonReports");
    if (qos_mon_reports) {
        cJSON *qos_mon_reports_local = NULL;
        if (!cJSON_IsArray(qos_mon_reports)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [qos_mon_reports]");
            goto end;
        }

        qos_mon_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_mon_reports_local, qos_mon_reports) {
            if (!cJSON_IsObject(qos_mon_reports_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [qos_mon_reports]");
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

    qos_mon_dat_rate_reps = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "qosMonDatRateReps");
    if (qos_mon_dat_rate_reps) {
        cJSON *qos_mon_dat_rate_reps_local = NULL;
        if (!cJSON_IsArray(qos_mon_dat_rate_reps)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [qos_mon_dat_rate_reps]");
            goto end;
        }

        qos_mon_dat_rate_repsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_mon_dat_rate_reps_local, qos_mon_dat_rate_reps) {
            if (!cJSON_IsObject(qos_mon_dat_rate_reps_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [qos_mon_dat_rate_reps]");
                goto end;
            }
            OpenAPI_qos_monitoring_report_t *qos_mon_dat_rate_repsItem = OpenAPI_qos_monitoring_report_parseFromJSON(qos_mon_dat_rate_reps_local);
            if (!qos_mon_dat_rate_repsItem) {
                ogs_error("No qos_mon_dat_rate_repsItem");
                goto end;
            }
            OpenAPI_list_add(qos_mon_dat_rate_repsList, qos_mon_dat_rate_repsItem);
        }
    }

    pdv_mon_reports = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "pdvMonReports");
    if (pdv_mon_reports) {
        cJSON *pdv_mon_reports_local = NULL;
        if (!cJSON_IsArray(pdv_mon_reports)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [pdv_mon_reports]");
            goto end;
        }

        pdv_mon_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdv_mon_reports_local, pdv_mon_reports) {
            if (!cJSON_IsObject(pdv_mon_reports_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [pdv_mon_reports]");
                goto end;
            }
            OpenAPI_pdv_monitoring_report_t *pdv_mon_reportsItem = OpenAPI_pdv_monitoring_report_parseFromJSON(pdv_mon_reports_local);
            if (!pdv_mon_reportsItem) {
                ogs_error("No pdv_mon_reportsItem");
                goto end;
            }
            OpenAPI_list_add(pdv_mon_reportsList, pdv_mon_reportsItem);
        }
    }

    congest_reports = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "congestReports");
    if (congest_reports) {
        cJSON *congest_reports_local = NULL;
        if (!cJSON_IsArray(congest_reports)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [congest_reports]");
            goto end;
        }

        congest_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(congest_reports_local, congest_reports) {
            if (!cJSON_IsObject(congest_reports_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [congest_reports]");
                goto end;
            }
            OpenAPI_qos_monitoring_report_t *congest_reportsItem = OpenAPI_qos_monitoring_report_parseFromJSON(congest_reports_local);
            if (!congest_reportsItem) {
                ogs_error("No congest_reportsItem");
                goto end;
            }
            OpenAPI_list_add(congest_reportsList, congest_reportsItem);
        }
    }

    rtt_mon_reports = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "rttMonReports");
    if (rtt_mon_reports) {
        cJSON *rtt_mon_reports_local = NULL;
        if (!cJSON_IsArray(rtt_mon_reports)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [rtt_mon_reports]");
            goto end;
        }

        rtt_mon_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(rtt_mon_reports_local, rtt_mon_reports) {
            if (!cJSON_IsObject(rtt_mon_reports_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [rtt_mon_reports]");
                goto end;
            }
            OpenAPI_qos_monitoring_report_t *rtt_mon_reportsItem = OpenAPI_qos_monitoring_report_parseFromJSON(rtt_mon_reports_local);
            if (!rtt_mon_reportsItem) {
                ogs_error("No rtt_mon_reportsItem");
                goto end;
            }
            OpenAPI_list_add(rtt_mon_reportsList, rtt_mon_reportsItem);
        }
    }

    qos_mon_cap_repos = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "qosMonCapRepos");
    if (qos_mon_cap_repos) {
        cJSON *qos_mon_cap_repos_local_map = NULL;
        if (!cJSON_IsObject(qos_mon_cap_repos) && !cJSON_IsNull(qos_mon_cap_repos)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [qos_mon_cap_repos]");
            goto end;
        }
        if (cJSON_IsObject(qos_mon_cap_repos)) {
            qos_mon_cap_reposList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(qos_mon_cap_repos_local_map, qos_mon_cap_repos) {
                cJSON *localMapObject = qos_mon_cap_repos_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_capability_report_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_events_notification_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(qos_mon_cap_reposList, localMapKeyPair);
            }
        }
    }

    ran_nas_rel_causes = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "ranNasRelCauses");
    if (ran_nas_rel_causes) {
        cJSON *ran_nas_rel_causes_local = NULL;
        if (!cJSON_IsArray(ran_nas_rel_causes)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ran_nas_rel_causes]");
            goto end;
        }

        ran_nas_rel_causesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ran_nas_rel_causes_local, ran_nas_rel_causes) {
            if (!cJSON_IsObject(ran_nas_rel_causes_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ran_nas_rel_causes]");
                goto end;
            }
            OpenAPI_ran_nas_rel_cause_t *ran_nas_rel_causesItem = OpenAPI_ran_nas_rel_cause_parseFromJSON(ran_nas_rel_causes_local);
            if (!ran_nas_rel_causesItem) {
                ogs_error("No ran_nas_rel_causesItem");
                goto end;
            }
            OpenAPI_list_add(ran_nas_rel_causesList, ran_nas_rel_causesItem);
        }
    }

    rat_type = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "ratType");
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    sat_backhaul_category = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "satBackhaulCategory");
    if (sat_backhaul_category) {
    if (!cJSON_IsString(sat_backhaul_category)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [sat_backhaul_category]");
        goto end;
    }
    sat_backhaul_categoryVariable = OpenAPI_satellite_backhaul_category_FromString(sat_backhaul_category->valuestring);
    }

    ue_loc = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "ueLoc");
    if (ue_loc) {
    ue_loc_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_loc);
    if (!ue_loc_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_loc]");
        goto end;
    }
    }

    ue_loc_time = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "ueLocTime");
    if (ue_loc_time) {
    if (!cJSON_IsString(ue_loc_time) && !cJSON_IsNull(ue_loc_time)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ue_loc_time]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    usg_rep = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "usgRep");
    if (usg_rep) {
    usg_rep_local_nonprim = OpenAPI_accumulated_usage_parseFromJSON(usg_rep);
    if (!usg_rep_local_nonprim) {
        ogs_error("OpenAPI_accumulated_usage_parseFromJSON failed [usg_rep]");
        goto end;
    }
    }

    ursp_enf_rep = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "urspEnfRep");
    if (ursp_enf_rep) {
    if (!cJSON_IsString(ursp_enf_rep) && !cJSON_IsNull(ursp_enf_rep)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ursp_enf_rep]");
        goto end;
    }
    }

    ssc_mode = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "sscMode");
    if (ssc_mode) {
    if (!cJSON_IsString(ssc_mode)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ssc_mode]");
        goto end;
    }
    ssc_modeVariable = OpenAPI_ssc_mode_FromString(ssc_mode->valuestring);
    }

    ue_req_dnn = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "ueReqDnn");
    if (ue_req_dnn) {
    if (!cJSON_IsString(ue_req_dnn) && !cJSON_IsNull(ue_req_dnn)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ue_req_dnn]");
        goto end;
    }
    }

    ue_req_pdu_session_type = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "ueReqPduSessionType");
    if (ue_req_pdu_session_type) {
    if (!cJSON_IsString(ue_req_pdu_session_type)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ue_req_pdu_session_type]");
        goto end;
    }
    ue_req_pdu_session_typeVariable = OpenAPI_pdu_session_type_FromString(ue_req_pdu_session_type->valuestring);
    }

    tsn_bridge_man_cont = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "tsnBridgeManCont");
    if (tsn_bridge_man_cont) {
    tsn_bridge_man_cont_local_nonprim = OpenAPI_bridge_management_container_parseFromJSON(tsn_bridge_man_cont);
    if (!tsn_bridge_man_cont_local_nonprim) {
        ogs_error("OpenAPI_bridge_management_container_parseFromJSON failed [tsn_bridge_man_cont]");
        goto end;
    }
    }

    tsn_port_man_cont_dstt = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "tsnPortManContDstt");
    if (tsn_port_man_cont_dstt) {
    tsn_port_man_cont_dstt_local_nonprim = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_dstt);
    if (!tsn_port_man_cont_dstt_local_nonprim) {
        ogs_error("OpenAPI_port_management_container_parseFromJSON failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    }

    tsn_port_man_cont_nwtts = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "tsnPortManContNwtts");
    if (tsn_port_man_cont_nwtts) {
        cJSON *tsn_port_man_cont_nwtts_local = NULL;
        if (!cJSON_IsArray(tsn_port_man_cont_nwtts)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }

        tsn_port_man_cont_nwttsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tsn_port_man_cont_nwtts_local, tsn_port_man_cont_nwtts) {
            if (!cJSON_IsObject(tsn_port_man_cont_nwtts_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
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

    ipv4_addr_list = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "ipv4AddrList");
    if (ipv4_addr_list) {
        cJSON *ipv4_addr_list_local = NULL;
        if (!cJSON_IsArray(ipv4_addr_list)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ipv4_addr_list]");
            goto end;
        }

        ipv4_addr_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_addr_list_local, ipv4_addr_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv4_addr_list_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ipv4_addr_list]");
                goto end;
            }
            OpenAPI_list_add(ipv4_addr_listList, ogs_strdup(ipv4_addr_list_local->valuestring));
        }
    }

    ipv6_prefix_list = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "ipv6PrefixList");
    if (ipv6_prefix_list) {
        cJSON *ipv6_prefix_list_local = NULL;
        if (!cJSON_IsArray(ipv6_prefix_list)) {
            ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ipv6_prefix_list]");
            goto end;
        }

        ipv6_prefix_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_prefix_list_local, ipv6_prefix_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv6_prefix_list_local)) {
                ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ipv6_prefix_list]");
                goto end;
            }
            OpenAPI_list_add(ipv6_prefix_listList, ogs_strdup(ipv6_prefix_list_local->valuestring));
        }
    }

    bat_offset_info = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "batOffsetInfo");
    if (bat_offset_info) {
    bat_offset_info_local_nonprim = OpenAPI_bat_offset_info_parseFromJSON(bat_offset_info);
    if (!bat_offset_info_local_nonprim) {
        ogs_error("OpenAPI_bat_offset_info_parseFromJSON failed [bat_offset_info]");
        goto end;
    }
    }

    ue_reach_status = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "ueReachStatus");
    if (ue_reach_status) {
    if (!cJSON_IsString(ue_reach_status)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [ue_reach_status]");
        goto end;
    }
    ue_reach_statusVariable = OpenAPI_ue_reachability_status_FromString(ue_reach_status->valuestring);
    }

    retry_after = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "retryAfter");
    if (retry_after) {
    if (!cJSON_IsNumber(retry_after)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [retry_after]");
        goto end;
    }
    }

    serv_sat_id = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "servSatId");
    if (serv_sat_id) {
    if (!cJSON_IsString(serv_sat_id) && !cJSON_IsNull(serv_sat_id)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [serv_sat_id]");
        goto end;
    }
    }

    rate_limit_repo = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "rateLimitRepo");
    if (rate_limit_repo) {
    rate_limit_repo_local_nonprim = OpenAPI_rate_limit_repo_parseFromJSON(rate_limit_repo);
    if (!rate_limit_repo_local_nonprim) {
        ogs_error("OpenAPI_rate_limit_repo_parseFromJSON failed [rate_limit_repo]");
        goto end;
    }
    }

    dnai_chg_type = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "dnaiChgType");
    if (dnai_chg_type) {
    if (!cJSON_IsString(dnai_chg_type)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [dnai_chg_type]");
        goto end;
    }
    dnai_chg_typeVariable = OpenAPI_dnai_change_type_FromString(dnai_chg_type->valuestring);
    }

    source_dnai = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "sourceDnai");
    if (source_dnai) {
    if (!cJSON_IsString(source_dnai) && !cJSON_IsNull(source_dnai)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [source_dnai]");
        goto end;
    }
    }

    target_dnai = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "targetDnai");
    if (target_dnai) {
    if (!cJSON_IsString(target_dnai) && !cJSON_IsNull(target_dnai)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [target_dnai]");
        goto end;
    }
    }

    session_recov_status = cJSON_GetObjectItemCaseSensitive(events_notificationJSON, "sessionRecovStatus");
    if (session_recov_status) {
    if (!cJSON_IsString(session_recov_status)) {
        ogs_error("OpenAPI_events_notification_parseFromJSON() failed [session_recov_status]");
        goto end;
    }
    session_recov_statusVariable = OpenAPI_pc_session_recovery_status_FromString(session_recov_status->valuestring);
    }

    events_notification_local_var = OpenAPI_events_notification_create (
        ad_reports ? ad_reportsList : NULL,
        access_type ? access_typeVariable : 0,
        add_access_info ? add_access_info_local_nonprim : NULL,
        rel_access_info ? rel_access_info_local_nonprim : NULL,
        an_charg_addr && cJSON_IsNull(an_charg_addr) ? true : false,
        an_charg_addr ? an_charg_addr_local_nonprim : NULL,
        an_charg_ids ? an_charg_idsList : NULL,
        an_gw_addr && cJSON_IsNull(an_gw_addr) ? true : false,
        an_gw_addr ? an_gw_addr_local_nonprim : NULL,
        l4s_reports ? l4s_reportsList : NULL,
        ogs_strdup(ev_subs_uri->valuestring),
        ev_notifsList,
        failed_resourc_alloc_reports ? failed_resourc_alloc_reportsList : NULL,
        succ_resourc_alloc_reports ? succ_resourc_alloc_reportsList : NULL,
        no_net_loc_supp ? no_net_loc_suppVariable : 0,
        out_of_cred_reports ? out_of_cred_reportsList : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        qnc_reports ? qnc_reportsList : NULL,
        qos_mon_reports ? qos_mon_reportsList : NULL,
        qos_mon_dat_rate_reps ? qos_mon_dat_rate_repsList : NULL,
        pdv_mon_reports ? pdv_mon_reportsList : NULL,
        congest_reports ? congest_reportsList : NULL,
        rtt_mon_reports ? rtt_mon_reportsList : NULL,
        qos_mon_cap_repos ? qos_mon_cap_reposList : NULL,
        ran_nas_rel_causes ? ran_nas_rel_causesList : NULL,
        rat_type ? rat_typeVariable : 0,
        sat_backhaul_category ? sat_backhaul_categoryVariable : 0,
        ue_loc ? ue_loc_local_nonprim : NULL,
        ue_loc_time && !cJSON_IsNull(ue_loc_time) ? ogs_strdup(ue_loc_time->valuestring) : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        usg_rep ? usg_rep_local_nonprim : NULL,
        ursp_enf_rep && !cJSON_IsNull(ursp_enf_rep) ? ogs_strdup(ursp_enf_rep->valuestring) : NULL,
        ssc_mode ? ssc_modeVariable : 0,
        ue_req_dnn && !cJSON_IsNull(ue_req_dnn) ? ogs_strdup(ue_req_dnn->valuestring) : NULL,
        ue_req_pdu_session_type ? ue_req_pdu_session_typeVariable : 0,
        tsn_bridge_man_cont ? tsn_bridge_man_cont_local_nonprim : NULL,
        tsn_port_man_cont_dstt ? tsn_port_man_cont_dstt_local_nonprim : NULL,
        tsn_port_man_cont_nwtts ? tsn_port_man_cont_nwttsList : NULL,
        ipv4_addr_list ? ipv4_addr_listList : NULL,
        ipv6_prefix_list ? ipv6_prefix_listList : NULL,
        bat_offset_info ? bat_offset_info_local_nonprim : NULL,
        ue_reach_status ? ue_reach_statusVariable : 0,
        retry_after ? true : false,
        retry_after ? retry_after->valuedouble : 0,
        serv_sat_id && !cJSON_IsNull(serv_sat_id) ? ogs_strdup(serv_sat_id->valuestring) : NULL,
        rate_limit_repo ? rate_limit_repo_local_nonprim : NULL,
        dnai_chg_type ? dnai_chg_typeVariable : 0,
        source_dnai && !cJSON_IsNull(source_dnai) ? ogs_strdup(source_dnai->valuestring) : NULL,
        target_dnai && !cJSON_IsNull(target_dnai) ? ogs_strdup(target_dnai->valuestring) : NULL,
        session_recov_status ? session_recov_statusVariable : 0
    );

    return events_notification_local_var;
end:
    if (ad_reportsList) {
        OpenAPI_list_for_each(ad_reportsList, node) {
            OpenAPI_app_detection_report_free(node->data);
        }
        OpenAPI_list_free(ad_reportsList);
        ad_reportsList = NULL;
    }
    if (add_access_info_local_nonprim) {
        OpenAPI_additional_access_info_free(add_access_info_local_nonprim);
        add_access_info_local_nonprim = NULL;
    }
    if (rel_access_info_local_nonprim) {
        OpenAPI_additional_access_info_free(rel_access_info_local_nonprim);
        rel_access_info_local_nonprim = NULL;
    }
    if (an_charg_addr_local_nonprim) {
        OpenAPI_acc_net_charging_address_free(an_charg_addr_local_nonprim);
        an_charg_addr_local_nonprim = NULL;
    }
    if (an_charg_idsList) {
        OpenAPI_list_for_each(an_charg_idsList, node) {
            OpenAPI_access_net_charging_identifier_free(node->data);
        }
        OpenAPI_list_free(an_charg_idsList);
        an_charg_idsList = NULL;
    }
    if (an_gw_addr_local_nonprim) {
        OpenAPI_an_gw_address_free(an_gw_addr_local_nonprim);
        an_gw_addr_local_nonprim = NULL;
    }
    if (l4s_reportsList) {
        OpenAPI_list_for_each(l4s_reportsList, node) {
            OpenAPI_l4s_support_free(node->data);
        }
        OpenAPI_list_free(l4s_reportsList);
        l4s_reportsList = NULL;
    }
    if (ev_notifsList) {
        OpenAPI_list_for_each(ev_notifsList, node) {
            OpenAPI_af_event_notification_free(node->data);
        }
        OpenAPI_list_free(ev_notifsList);
        ev_notifsList = NULL;
    }
    if (failed_resourc_alloc_reportsList) {
        OpenAPI_list_for_each(failed_resourc_alloc_reportsList, node) {
            OpenAPI_resources_allocation_info_free(node->data);
        }
        OpenAPI_list_free(failed_resourc_alloc_reportsList);
        failed_resourc_alloc_reportsList = NULL;
    }
    if (succ_resourc_alloc_reportsList) {
        OpenAPI_list_for_each(succ_resourc_alloc_reportsList, node) {
            OpenAPI_resources_allocation_info_free(node->data);
        }
        OpenAPI_list_free(succ_resourc_alloc_reportsList);
        succ_resourc_alloc_reportsList = NULL;
    }
    if (out_of_cred_reportsList) {
        OpenAPI_list_for_each(out_of_cred_reportsList, node) {
            OpenAPI_out_of_credit_information_free(node->data);
        }
        OpenAPI_list_free(out_of_cred_reportsList);
        out_of_cred_reportsList = NULL;
    }
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_nid_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
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
    if (qos_mon_dat_rate_repsList) {
        OpenAPI_list_for_each(qos_mon_dat_rate_repsList, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(qos_mon_dat_rate_repsList);
        qos_mon_dat_rate_repsList = NULL;
    }
    if (pdv_mon_reportsList) {
        OpenAPI_list_for_each(pdv_mon_reportsList, node) {
            OpenAPI_pdv_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(pdv_mon_reportsList);
        pdv_mon_reportsList = NULL;
    }
    if (congest_reportsList) {
        OpenAPI_list_for_each(congest_reportsList, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(congest_reportsList);
        congest_reportsList = NULL;
    }
    if (rtt_mon_reportsList) {
        OpenAPI_list_for_each(rtt_mon_reportsList, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(rtt_mon_reportsList);
        rtt_mon_reportsList = NULL;
    }
    if (qos_mon_cap_reposList) {
        OpenAPI_list_for_each(qos_mon_cap_reposList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_capability_report_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(qos_mon_cap_reposList);
        qos_mon_cap_reposList = NULL;
    }
    if (ran_nas_rel_causesList) {
        OpenAPI_list_for_each(ran_nas_rel_causesList, node) {
            OpenAPI_ran_nas_rel_cause_free(node->data);
        }
        OpenAPI_list_free(ran_nas_rel_causesList);
        ran_nas_rel_causesList = NULL;
    }
    if (ue_loc_local_nonprim) {
        OpenAPI_user_location_free(ue_loc_local_nonprim);
        ue_loc_local_nonprim = NULL;
    }
    if (usg_rep_local_nonprim) {
        OpenAPI_accumulated_usage_free(usg_rep_local_nonprim);
        usg_rep_local_nonprim = NULL;
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
    if (ipv4_addr_listList) {
        OpenAPI_list_for_each(ipv4_addr_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv4_addr_listList);
        ipv4_addr_listList = NULL;
    }
    if (ipv6_prefix_listList) {
        OpenAPI_list_for_each(ipv6_prefix_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv6_prefix_listList);
        ipv6_prefix_listList = NULL;
    }
    if (bat_offset_info_local_nonprim) {
        OpenAPI_bat_offset_info_free(bat_offset_info_local_nonprim);
        bat_offset_info_local_nonprim = NULL;
    }
    if (rate_limit_repo_local_nonprim) {
        OpenAPI_rate_limit_repo_free(rate_limit_repo_local_nonprim);
        rate_limit_repo_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_events_notification_t *OpenAPI_events_notification_copy(OpenAPI_events_notification_t *dst, OpenAPI_events_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_events_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_events_notification_convertToJSON() failed");
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

    OpenAPI_events_notification_free(dst);
    dst = OpenAPI_events_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

