
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "events_subsc_put_data.h"

OpenAPI_events_subsc_put_data_t *OpenAPI_events_subsc_put_data_create(
    OpenAPI_list_t *events,
    char *notif_uri,
    OpenAPI_list_t *req_qos_mon_params,
    OpenAPI_qos_monitoring_information_t *qos_mon,
    OpenAPI_qos_monitoring_information_t *qos_mon_dat_rate,
    OpenAPI_list_t *pdv_req_mon_params,
    OpenAPI_qos_monitoring_information_t *pdv_mon,
    OpenAPI_qos_monitoring_information_t *congest_mon,
    OpenAPI_qos_monitoring_information_t *rtt_mon,
    OpenAPI_qos_monitoring_information_t *avl_bit_rate_mon,
    OpenAPI_rtt_flow_reference_t *rtt_flow_ref,
    OpenAPI_list_t *req_anis,
    OpenAPI_usage_threshold_t *usg_thres,
    char *notif_corre_id,
    OpenAPI_list_t *af_app_ids,
    bool is_direct_notif_ind,
    int direct_notif_ind,
    bool is_avrg_wndw,
    int avrg_wndw,
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
    OpenAPI_events_subsc_put_data_t *events_subsc_put_data_local_var = ogs_malloc(sizeof(OpenAPI_events_subsc_put_data_t));
    ogs_assert(events_subsc_put_data_local_var);

    events_subsc_put_data_local_var->events = events;
    events_subsc_put_data_local_var->notif_uri = notif_uri;
    events_subsc_put_data_local_var->req_qos_mon_params = req_qos_mon_params;
    events_subsc_put_data_local_var->qos_mon = qos_mon;
    events_subsc_put_data_local_var->qos_mon_dat_rate = qos_mon_dat_rate;
    events_subsc_put_data_local_var->pdv_req_mon_params = pdv_req_mon_params;
    events_subsc_put_data_local_var->pdv_mon = pdv_mon;
    events_subsc_put_data_local_var->congest_mon = congest_mon;
    events_subsc_put_data_local_var->rtt_mon = rtt_mon;
    events_subsc_put_data_local_var->avl_bit_rate_mon = avl_bit_rate_mon;
    events_subsc_put_data_local_var->rtt_flow_ref = rtt_flow_ref;
    events_subsc_put_data_local_var->req_anis = req_anis;
    events_subsc_put_data_local_var->usg_thres = usg_thres;
    events_subsc_put_data_local_var->notif_corre_id = notif_corre_id;
    events_subsc_put_data_local_var->af_app_ids = af_app_ids;
    events_subsc_put_data_local_var->is_direct_notif_ind = is_direct_notif_ind;
    events_subsc_put_data_local_var->direct_notif_ind = direct_notif_ind;
    events_subsc_put_data_local_var->is_avrg_wndw = is_avrg_wndw;
    events_subsc_put_data_local_var->avrg_wndw = avrg_wndw;
    events_subsc_put_data_local_var->ad_reports = ad_reports;
    events_subsc_put_data_local_var->access_type = access_type;
    events_subsc_put_data_local_var->add_access_info = add_access_info;
    events_subsc_put_data_local_var->rel_access_info = rel_access_info;
    events_subsc_put_data_local_var->is_an_charg_addr_null = is_an_charg_addr_null;
    events_subsc_put_data_local_var->an_charg_addr = an_charg_addr;
    events_subsc_put_data_local_var->an_charg_ids = an_charg_ids;
    events_subsc_put_data_local_var->is_an_gw_addr_null = is_an_gw_addr_null;
    events_subsc_put_data_local_var->an_gw_addr = an_gw_addr;
    events_subsc_put_data_local_var->l4s_reports = l4s_reports;
    events_subsc_put_data_local_var->ev_subs_uri = ev_subs_uri;
    events_subsc_put_data_local_var->ev_notifs = ev_notifs;
    events_subsc_put_data_local_var->failed_resourc_alloc_reports = failed_resourc_alloc_reports;
    events_subsc_put_data_local_var->succ_resourc_alloc_reports = succ_resourc_alloc_reports;
    events_subsc_put_data_local_var->no_net_loc_supp = no_net_loc_supp;
    events_subsc_put_data_local_var->out_of_cred_reports = out_of_cred_reports;
    events_subsc_put_data_local_var->plmn_id = plmn_id;
    events_subsc_put_data_local_var->qnc_reports = qnc_reports;
    events_subsc_put_data_local_var->qos_mon_reports = qos_mon_reports;
    events_subsc_put_data_local_var->qos_mon_dat_rate_reps = qos_mon_dat_rate_reps;
    events_subsc_put_data_local_var->pdv_mon_reports = pdv_mon_reports;
    events_subsc_put_data_local_var->congest_reports = congest_reports;
    events_subsc_put_data_local_var->rtt_mon_reports = rtt_mon_reports;
    events_subsc_put_data_local_var->qos_mon_cap_repos = qos_mon_cap_repos;
    events_subsc_put_data_local_var->ran_nas_rel_causes = ran_nas_rel_causes;
    events_subsc_put_data_local_var->rat_type = rat_type;
    events_subsc_put_data_local_var->sat_backhaul_category = sat_backhaul_category;
    events_subsc_put_data_local_var->ue_loc = ue_loc;
    events_subsc_put_data_local_var->ue_loc_time = ue_loc_time;
    events_subsc_put_data_local_var->ue_time_zone = ue_time_zone;
    events_subsc_put_data_local_var->usg_rep = usg_rep;
    events_subsc_put_data_local_var->ursp_enf_rep = ursp_enf_rep;
    events_subsc_put_data_local_var->ssc_mode = ssc_mode;
    events_subsc_put_data_local_var->ue_req_dnn = ue_req_dnn;
    events_subsc_put_data_local_var->ue_req_pdu_session_type = ue_req_pdu_session_type;
    events_subsc_put_data_local_var->tsn_bridge_man_cont = tsn_bridge_man_cont;
    events_subsc_put_data_local_var->tsn_port_man_cont_dstt = tsn_port_man_cont_dstt;
    events_subsc_put_data_local_var->tsn_port_man_cont_nwtts = tsn_port_man_cont_nwtts;
    events_subsc_put_data_local_var->ipv4_addr_list = ipv4_addr_list;
    events_subsc_put_data_local_var->ipv6_prefix_list = ipv6_prefix_list;
    events_subsc_put_data_local_var->bat_offset_info = bat_offset_info;
    events_subsc_put_data_local_var->ue_reach_status = ue_reach_status;
    events_subsc_put_data_local_var->is_retry_after = is_retry_after;
    events_subsc_put_data_local_var->retry_after = retry_after;
    events_subsc_put_data_local_var->serv_sat_id = serv_sat_id;
    events_subsc_put_data_local_var->rate_limit_repo = rate_limit_repo;
    events_subsc_put_data_local_var->dnai_chg_type = dnai_chg_type;
    events_subsc_put_data_local_var->source_dnai = source_dnai;
    events_subsc_put_data_local_var->target_dnai = target_dnai;
    events_subsc_put_data_local_var->session_recov_status = session_recov_status;

    return events_subsc_put_data_local_var;
}

void OpenAPI_events_subsc_put_data_free(OpenAPI_events_subsc_put_data_t *events_subsc_put_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == events_subsc_put_data) {
        return;
    }
    if (events_subsc_put_data->events) {
        OpenAPI_list_for_each(events_subsc_put_data->events, node) {
            OpenAPI_af_event_subscription_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->events);
        events_subsc_put_data->events = NULL;
    }
    if (events_subsc_put_data->notif_uri) {
        ogs_free(events_subsc_put_data->notif_uri);
        events_subsc_put_data->notif_uri = NULL;
    }
    if (events_subsc_put_data->req_qos_mon_params) {
        OpenAPI_list_free(events_subsc_put_data->req_qos_mon_params);
        events_subsc_put_data->req_qos_mon_params = NULL;
    }
    if (events_subsc_put_data->qos_mon) {
        OpenAPI_qos_monitoring_information_free(events_subsc_put_data->qos_mon);
        events_subsc_put_data->qos_mon = NULL;
    }
    if (events_subsc_put_data->qos_mon_dat_rate) {
        OpenAPI_qos_monitoring_information_free(events_subsc_put_data->qos_mon_dat_rate);
        events_subsc_put_data->qos_mon_dat_rate = NULL;
    }
    if (events_subsc_put_data->pdv_req_mon_params) {
        OpenAPI_list_free(events_subsc_put_data->pdv_req_mon_params);
        events_subsc_put_data->pdv_req_mon_params = NULL;
    }
    if (events_subsc_put_data->pdv_mon) {
        OpenAPI_qos_monitoring_information_free(events_subsc_put_data->pdv_mon);
        events_subsc_put_data->pdv_mon = NULL;
    }
    if (events_subsc_put_data->congest_mon) {
        OpenAPI_qos_monitoring_information_free(events_subsc_put_data->congest_mon);
        events_subsc_put_data->congest_mon = NULL;
    }
    if (events_subsc_put_data->rtt_mon) {
        OpenAPI_qos_monitoring_information_free(events_subsc_put_data->rtt_mon);
        events_subsc_put_data->rtt_mon = NULL;
    }
    if (events_subsc_put_data->avl_bit_rate_mon) {
        OpenAPI_qos_monitoring_information_free(events_subsc_put_data->avl_bit_rate_mon);
        events_subsc_put_data->avl_bit_rate_mon = NULL;
    }
    if (events_subsc_put_data->rtt_flow_ref) {
        OpenAPI_rtt_flow_reference_free(events_subsc_put_data->rtt_flow_ref);
        events_subsc_put_data->rtt_flow_ref = NULL;
    }
    if (events_subsc_put_data->req_anis) {
        OpenAPI_list_free(events_subsc_put_data->req_anis);
        events_subsc_put_data->req_anis = NULL;
    }
    if (events_subsc_put_data->usg_thres) {
        OpenAPI_usage_threshold_free(events_subsc_put_data->usg_thres);
        events_subsc_put_data->usg_thres = NULL;
    }
    if (events_subsc_put_data->notif_corre_id) {
        ogs_free(events_subsc_put_data->notif_corre_id);
        events_subsc_put_data->notif_corre_id = NULL;
    }
    if (events_subsc_put_data->af_app_ids) {
        OpenAPI_list_for_each(events_subsc_put_data->af_app_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->af_app_ids);
        events_subsc_put_data->af_app_ids = NULL;
    }
    if (events_subsc_put_data->ad_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->ad_reports, node) {
            OpenAPI_app_detection_report_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->ad_reports);
        events_subsc_put_data->ad_reports = NULL;
    }
    if (events_subsc_put_data->add_access_info) {
        OpenAPI_additional_access_info_free(events_subsc_put_data->add_access_info);
        events_subsc_put_data->add_access_info = NULL;
    }
    if (events_subsc_put_data->rel_access_info) {
        OpenAPI_additional_access_info_free(events_subsc_put_data->rel_access_info);
        events_subsc_put_data->rel_access_info = NULL;
    }
    if (events_subsc_put_data->an_charg_addr) {
        OpenAPI_acc_net_charging_address_free(events_subsc_put_data->an_charg_addr);
        events_subsc_put_data->an_charg_addr = NULL;
    }
    if (events_subsc_put_data->an_charg_ids) {
        OpenAPI_list_for_each(events_subsc_put_data->an_charg_ids, node) {
            OpenAPI_access_net_charging_identifier_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->an_charg_ids);
        events_subsc_put_data->an_charg_ids = NULL;
    }
    if (events_subsc_put_data->an_gw_addr) {
        OpenAPI_an_gw_address_free(events_subsc_put_data->an_gw_addr);
        events_subsc_put_data->an_gw_addr = NULL;
    }
    if (events_subsc_put_data->l4s_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->l4s_reports, node) {
            OpenAPI_l4s_support_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->l4s_reports);
        events_subsc_put_data->l4s_reports = NULL;
    }
    if (events_subsc_put_data->ev_subs_uri) {
        ogs_free(events_subsc_put_data->ev_subs_uri);
        events_subsc_put_data->ev_subs_uri = NULL;
    }
    if (events_subsc_put_data->ev_notifs) {
        OpenAPI_list_for_each(events_subsc_put_data->ev_notifs, node) {
            OpenAPI_af_event_notification_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->ev_notifs);
        events_subsc_put_data->ev_notifs = NULL;
    }
    if (events_subsc_put_data->failed_resourc_alloc_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->failed_resourc_alloc_reports, node) {
            OpenAPI_resources_allocation_info_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->failed_resourc_alloc_reports);
        events_subsc_put_data->failed_resourc_alloc_reports = NULL;
    }
    if (events_subsc_put_data->succ_resourc_alloc_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->succ_resourc_alloc_reports, node) {
            OpenAPI_resources_allocation_info_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->succ_resourc_alloc_reports);
        events_subsc_put_data->succ_resourc_alloc_reports = NULL;
    }
    if (events_subsc_put_data->out_of_cred_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->out_of_cred_reports, node) {
            OpenAPI_out_of_credit_information_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->out_of_cred_reports);
        events_subsc_put_data->out_of_cred_reports = NULL;
    }
    if (events_subsc_put_data->plmn_id) {
        OpenAPI_plmn_id_nid_free(events_subsc_put_data->plmn_id);
        events_subsc_put_data->plmn_id = NULL;
    }
    if (events_subsc_put_data->qnc_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->qnc_reports, node) {
            OpenAPI_qos_notification_control_info_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->qnc_reports);
        events_subsc_put_data->qnc_reports = NULL;
    }
    if (events_subsc_put_data->qos_mon_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->qos_mon_reports, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->qos_mon_reports);
        events_subsc_put_data->qos_mon_reports = NULL;
    }
    if (events_subsc_put_data->qos_mon_dat_rate_reps) {
        OpenAPI_list_for_each(events_subsc_put_data->qos_mon_dat_rate_reps, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->qos_mon_dat_rate_reps);
        events_subsc_put_data->qos_mon_dat_rate_reps = NULL;
    }
    if (events_subsc_put_data->pdv_mon_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->pdv_mon_reports, node) {
            OpenAPI_pdv_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->pdv_mon_reports);
        events_subsc_put_data->pdv_mon_reports = NULL;
    }
    if (events_subsc_put_data->congest_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->congest_reports, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->congest_reports);
        events_subsc_put_data->congest_reports = NULL;
    }
    if (events_subsc_put_data->rtt_mon_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->rtt_mon_reports, node) {
            OpenAPI_qos_monitoring_report_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->rtt_mon_reports);
        events_subsc_put_data->rtt_mon_reports = NULL;
    }
    if (events_subsc_put_data->qos_mon_cap_repos) {
        OpenAPI_list_for_each(events_subsc_put_data->qos_mon_cap_repos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_capability_report_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(events_subsc_put_data->qos_mon_cap_repos);
        events_subsc_put_data->qos_mon_cap_repos = NULL;
    }
    if (events_subsc_put_data->ran_nas_rel_causes) {
        OpenAPI_list_for_each(events_subsc_put_data->ran_nas_rel_causes, node) {
            OpenAPI_ran_nas_rel_cause_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->ran_nas_rel_causes);
        events_subsc_put_data->ran_nas_rel_causes = NULL;
    }
    if (events_subsc_put_data->ue_loc) {
        OpenAPI_user_location_free(events_subsc_put_data->ue_loc);
        events_subsc_put_data->ue_loc = NULL;
    }
    if (events_subsc_put_data->ue_loc_time) {
        ogs_free(events_subsc_put_data->ue_loc_time);
        events_subsc_put_data->ue_loc_time = NULL;
    }
    if (events_subsc_put_data->ue_time_zone) {
        ogs_free(events_subsc_put_data->ue_time_zone);
        events_subsc_put_data->ue_time_zone = NULL;
    }
    if (events_subsc_put_data->usg_rep) {
        OpenAPI_accumulated_usage_free(events_subsc_put_data->usg_rep);
        events_subsc_put_data->usg_rep = NULL;
    }
    if (events_subsc_put_data->ursp_enf_rep) {
        ogs_free(events_subsc_put_data->ursp_enf_rep);
        events_subsc_put_data->ursp_enf_rep = NULL;
    }
    if (events_subsc_put_data->ue_req_dnn) {
        ogs_free(events_subsc_put_data->ue_req_dnn);
        events_subsc_put_data->ue_req_dnn = NULL;
    }
    if (events_subsc_put_data->tsn_bridge_man_cont) {
        OpenAPI_bridge_management_container_free(events_subsc_put_data->tsn_bridge_man_cont);
        events_subsc_put_data->tsn_bridge_man_cont = NULL;
    }
    if (events_subsc_put_data->tsn_port_man_cont_dstt) {
        OpenAPI_port_management_container_free(events_subsc_put_data->tsn_port_man_cont_dstt);
        events_subsc_put_data->tsn_port_man_cont_dstt = NULL;
    }
    if (events_subsc_put_data->tsn_port_man_cont_nwtts) {
        OpenAPI_list_for_each(events_subsc_put_data->tsn_port_man_cont_nwtts, node) {
            OpenAPI_port_management_container_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->tsn_port_man_cont_nwtts);
        events_subsc_put_data->tsn_port_man_cont_nwtts = NULL;
    }
    if (events_subsc_put_data->ipv4_addr_list) {
        OpenAPI_list_for_each(events_subsc_put_data->ipv4_addr_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->ipv4_addr_list);
        events_subsc_put_data->ipv4_addr_list = NULL;
    }
    if (events_subsc_put_data->ipv6_prefix_list) {
        OpenAPI_list_for_each(events_subsc_put_data->ipv6_prefix_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(events_subsc_put_data->ipv6_prefix_list);
        events_subsc_put_data->ipv6_prefix_list = NULL;
    }
    if (events_subsc_put_data->bat_offset_info) {
        OpenAPI_bat_offset_info_free(events_subsc_put_data->bat_offset_info);
        events_subsc_put_data->bat_offset_info = NULL;
    }
    if (events_subsc_put_data->serv_sat_id) {
        ogs_free(events_subsc_put_data->serv_sat_id);
        events_subsc_put_data->serv_sat_id = NULL;
    }
    if (events_subsc_put_data->rate_limit_repo) {
        OpenAPI_rate_limit_repo_free(events_subsc_put_data->rate_limit_repo);
        events_subsc_put_data->rate_limit_repo = NULL;
    }
    if (events_subsc_put_data->source_dnai) {
        ogs_free(events_subsc_put_data->source_dnai);
        events_subsc_put_data->source_dnai = NULL;
    }
    if (events_subsc_put_data->target_dnai) {
        ogs_free(events_subsc_put_data->target_dnai);
        events_subsc_put_data->target_dnai = NULL;
    }
    ogs_free(events_subsc_put_data);
}

cJSON *OpenAPI_events_subsc_put_data_convertToJSON(OpenAPI_events_subsc_put_data_t *events_subsc_put_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (events_subsc_put_data == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [EventsSubscPutData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!events_subsc_put_data->events) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [events]");
        return NULL;
    }
    cJSON *eventsList = cJSON_AddArrayToObject(item, "events");
    if (eventsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [events]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->events, node) {
        cJSON *itemLocal = OpenAPI_af_event_subscription_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [events]");
            goto end;
        }
        cJSON_AddItemToArray(eventsList, itemLocal);
    }

    if (events_subsc_put_data->notif_uri) {
    if (cJSON_AddStringToObject(item, "notifUri", events_subsc_put_data->notif_uri) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [notif_uri]");
        goto end;
    }
    }

    if (events_subsc_put_data->req_qos_mon_params != OpenAPI_requested_qos_monitoring_parameter_NULL) {
    cJSON *req_qos_mon_paramsList = cJSON_AddArrayToObject(item, "reqQosMonParams");
    if (req_qos_mon_paramsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [req_qos_mon_params]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->req_qos_mon_params, node) {
        if (cJSON_AddStringToObject(req_qos_mon_paramsList, "", OpenAPI_requested_qos_monitoring_parameter_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [req_qos_mon_params]");
            goto end;
        }
    }
    }

    if (events_subsc_put_data->qos_mon) {
    cJSON *qos_mon_local_JSON = OpenAPI_qos_monitoring_information_convertToJSON(events_subsc_put_data->qos_mon);
    if (qos_mon_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qos_mon]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qosMon", qos_mon_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qos_mon]");
        goto end;
    }
    }

    if (events_subsc_put_data->qos_mon_dat_rate) {
    cJSON *qos_mon_dat_rate_local_JSON = OpenAPI_qos_monitoring_information_convertToJSON(events_subsc_put_data->qos_mon_dat_rate);
    if (qos_mon_dat_rate_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qos_mon_dat_rate]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qosMonDatRate", qos_mon_dat_rate_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qos_mon_dat_rate]");
        goto end;
    }
    }

    if (events_subsc_put_data->pdv_req_mon_params != OpenAPI_requested_qos_monitoring_parameter_NULL) {
    cJSON *pdv_req_mon_paramsList = cJSON_AddArrayToObject(item, "pdvReqMonParams");
    if (pdv_req_mon_paramsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [pdv_req_mon_params]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->pdv_req_mon_params, node) {
        if (cJSON_AddStringToObject(pdv_req_mon_paramsList, "", OpenAPI_requested_qos_monitoring_parameter_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [pdv_req_mon_params]");
            goto end;
        }
    }
    }

    if (events_subsc_put_data->pdv_mon) {
    cJSON *pdv_mon_local_JSON = OpenAPI_qos_monitoring_information_convertToJSON(events_subsc_put_data->pdv_mon);
    if (pdv_mon_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [pdv_mon]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pdvMon", pdv_mon_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [pdv_mon]");
        goto end;
    }
    }

    if (events_subsc_put_data->congest_mon) {
    cJSON *congest_mon_local_JSON = OpenAPI_qos_monitoring_information_convertToJSON(events_subsc_put_data->congest_mon);
    if (congest_mon_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [congest_mon]");
        goto end;
    }
    cJSON_AddItemToObject(item, "congestMon", congest_mon_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [congest_mon]");
        goto end;
    }
    }

    if (events_subsc_put_data->rtt_mon) {
    cJSON *rtt_mon_local_JSON = OpenAPI_qos_monitoring_information_convertToJSON(events_subsc_put_data->rtt_mon);
    if (rtt_mon_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [rtt_mon]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rttMon", rtt_mon_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [rtt_mon]");
        goto end;
    }
    }

    if (events_subsc_put_data->avl_bit_rate_mon) {
    cJSON *avl_bit_rate_mon_local_JSON = OpenAPI_qos_monitoring_information_convertToJSON(events_subsc_put_data->avl_bit_rate_mon);
    if (avl_bit_rate_mon_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [avl_bit_rate_mon]");
        goto end;
    }
    cJSON_AddItemToObject(item, "avlBitRateMon", avl_bit_rate_mon_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [avl_bit_rate_mon]");
        goto end;
    }
    }

    if (events_subsc_put_data->rtt_flow_ref) {
    cJSON *rtt_flow_ref_local_JSON = OpenAPI_rtt_flow_reference_convertToJSON(events_subsc_put_data->rtt_flow_ref);
    if (rtt_flow_ref_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [rtt_flow_ref]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rttFlowRef", rtt_flow_ref_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [rtt_flow_ref]");
        goto end;
    }
    }

    if (events_subsc_put_data->req_anis != OpenAPI_required_access_info_NULL) {
    cJSON *req_anisList = cJSON_AddArrayToObject(item, "reqAnis");
    if (req_anisList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [req_anis]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->req_anis, node) {
        if (cJSON_AddStringToObject(req_anisList, "", OpenAPI_required_access_info_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [req_anis]");
            goto end;
        }
    }
    }

    if (events_subsc_put_data->usg_thres) {
    cJSON *usg_thres_local_JSON = OpenAPI_usage_threshold_convertToJSON(events_subsc_put_data->usg_thres);
    if (usg_thres_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [usg_thres]");
        goto end;
    }
    cJSON_AddItemToObject(item, "usgThres", usg_thres_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [usg_thres]");
        goto end;
    }
    }

    if (events_subsc_put_data->notif_corre_id) {
    if (cJSON_AddStringToObject(item, "notifCorreId", events_subsc_put_data->notif_corre_id) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [notif_corre_id]");
        goto end;
    }
    }

    if (events_subsc_put_data->af_app_ids) {
    cJSON *af_app_idsList = cJSON_AddArrayToObject(item, "afAppIds");
    if (af_app_idsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [af_app_ids]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->af_app_ids, node) {
        if (cJSON_AddStringToObject(af_app_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [af_app_ids]");
            goto end;
        }
    }
    }

    if (events_subsc_put_data->is_direct_notif_ind) {
    if (cJSON_AddBoolToObject(item, "directNotifInd", events_subsc_put_data->direct_notif_ind) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [direct_notif_ind]");
        goto end;
    }
    }

    if (events_subsc_put_data->is_avrg_wndw) {
    if (cJSON_AddNumberToObject(item, "avrgWndw", events_subsc_put_data->avrg_wndw) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [avrg_wndw]");
        goto end;
    }
    }

    if (events_subsc_put_data->ad_reports) {
    cJSON *ad_reportsList = cJSON_AddArrayToObject(item, "adReports");
    if (ad_reportsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ad_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->ad_reports, node) {
        cJSON *itemLocal = OpenAPI_app_detection_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ad_reports]");
            goto end;
        }
        cJSON_AddItemToArray(ad_reportsList, itemLocal);
    }
    }

    if (events_subsc_put_data->access_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(events_subsc_put_data->access_type)) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [access_type]");
        goto end;
    }
    }

    if (events_subsc_put_data->add_access_info) {
    cJSON *add_access_info_local_JSON = OpenAPI_additional_access_info_convertToJSON(events_subsc_put_data->add_access_info);
    if (add_access_info_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [add_access_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "addAccessInfo", add_access_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [add_access_info]");
        goto end;
    }
    }

    if (events_subsc_put_data->rel_access_info) {
    cJSON *rel_access_info_local_JSON = OpenAPI_additional_access_info_convertToJSON(events_subsc_put_data->rel_access_info);
    if (rel_access_info_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [rel_access_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "relAccessInfo", rel_access_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [rel_access_info]");
        goto end;
    }
    }

    if (events_subsc_put_data->an_charg_addr) {
    cJSON *an_charg_addr_local_JSON = OpenAPI_acc_net_charging_address_convertToJSON(events_subsc_put_data->an_charg_addr);
    if (an_charg_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [an_charg_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "anChargAddr", an_charg_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [an_charg_addr]");
        goto end;
    }
    } else if (events_subsc_put_data->is_an_charg_addr_null) {
        if (cJSON_AddNullToObject(item, "anChargAddr") == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [an_charg_addr]");
            goto end;
        }
    }

    if (events_subsc_put_data->an_charg_ids) {
    cJSON *an_charg_idsList = cJSON_AddArrayToObject(item, "anChargIds");
    if (an_charg_idsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [an_charg_ids]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->an_charg_ids, node) {
        cJSON *itemLocal = OpenAPI_access_net_charging_identifier_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [an_charg_ids]");
            goto end;
        }
        cJSON_AddItemToArray(an_charg_idsList, itemLocal);
    }
    }

    if (events_subsc_put_data->an_gw_addr) {
    cJSON *an_gw_addr_local_JSON = OpenAPI_an_gw_address_convertToJSON(events_subsc_put_data->an_gw_addr);
    if (an_gw_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [an_gw_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "anGwAddr", an_gw_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [an_gw_addr]");
        goto end;
    }
    } else if (events_subsc_put_data->is_an_gw_addr_null) {
        if (cJSON_AddNullToObject(item, "anGwAddr") == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [an_gw_addr]");
            goto end;
        }
    }

    if (events_subsc_put_data->l4s_reports) {
    cJSON *l4s_reportsList = cJSON_AddArrayToObject(item, "l4sReports");
    if (l4s_reportsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [l4s_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->l4s_reports, node) {
        cJSON *itemLocal = OpenAPI_l4s_support_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [l4s_reports]");
            goto end;
        }
        cJSON_AddItemToArray(l4s_reportsList, itemLocal);
    }
    }

    if (!events_subsc_put_data->ev_subs_uri) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ev_subs_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "evSubsUri", events_subsc_put_data->ev_subs_uri) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ev_subs_uri]");
        goto end;
    }

    if (!events_subsc_put_data->ev_notifs) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ev_notifs]");
        return NULL;
    }
    cJSON *ev_notifsList = cJSON_AddArrayToObject(item, "evNotifs");
    if (ev_notifsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ev_notifs]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->ev_notifs, node) {
        cJSON *itemLocal = OpenAPI_af_event_notification_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ev_notifs]");
            goto end;
        }
        cJSON_AddItemToArray(ev_notifsList, itemLocal);
    }

    if (events_subsc_put_data->failed_resourc_alloc_reports) {
    cJSON *failed_resourc_alloc_reportsList = cJSON_AddArrayToObject(item, "failedResourcAllocReports");
    if (failed_resourc_alloc_reportsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [failed_resourc_alloc_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->failed_resourc_alloc_reports, node) {
        cJSON *itemLocal = OpenAPI_resources_allocation_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [failed_resourc_alloc_reports]");
            goto end;
        }
        cJSON_AddItemToArray(failed_resourc_alloc_reportsList, itemLocal);
    }
    }

    if (events_subsc_put_data->succ_resourc_alloc_reports) {
    cJSON *succ_resourc_alloc_reportsList = cJSON_AddArrayToObject(item, "succResourcAllocReports");
    if (succ_resourc_alloc_reportsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [succ_resourc_alloc_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->succ_resourc_alloc_reports, node) {
        cJSON *itemLocal = OpenAPI_resources_allocation_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [succ_resourc_alloc_reports]");
            goto end;
        }
        cJSON_AddItemToArray(succ_resourc_alloc_reportsList, itemLocal);
    }
    }

    if (events_subsc_put_data->no_net_loc_supp != OpenAPI_net_loc_access_support_NULL) {
    if (cJSON_AddStringToObject(item, "noNetLocSupp", OpenAPI_net_loc_access_support_ToString(events_subsc_put_data->no_net_loc_supp)) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [no_net_loc_supp]");
        goto end;
    }
    }

    if (events_subsc_put_data->out_of_cred_reports) {
    cJSON *out_of_cred_reportsList = cJSON_AddArrayToObject(item, "outOfCredReports");
    if (out_of_cred_reportsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [out_of_cred_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->out_of_cred_reports, node) {
        cJSON *itemLocal = OpenAPI_out_of_credit_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [out_of_cred_reports]");
            goto end;
        }
        cJSON_AddItemToArray(out_of_cred_reportsList, itemLocal);
    }
    }

    if (events_subsc_put_data->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(events_subsc_put_data->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (events_subsc_put_data->qnc_reports) {
    cJSON *qnc_reportsList = cJSON_AddArrayToObject(item, "qncReports");
    if (qnc_reportsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qnc_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->qnc_reports, node) {
        cJSON *itemLocal = OpenAPI_qos_notification_control_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qnc_reports]");
            goto end;
        }
        cJSON_AddItemToArray(qnc_reportsList, itemLocal);
    }
    }

    if (events_subsc_put_data->qos_mon_reports) {
    cJSON *qos_mon_reportsList = cJSON_AddArrayToObject(item, "qosMonReports");
    if (qos_mon_reportsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qos_mon_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->qos_mon_reports, node) {
        cJSON *itemLocal = OpenAPI_qos_monitoring_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qos_mon_reports]");
            goto end;
        }
        cJSON_AddItemToArray(qos_mon_reportsList, itemLocal);
    }
    }

    if (events_subsc_put_data->qos_mon_dat_rate_reps) {
    cJSON *qos_mon_dat_rate_repsList = cJSON_AddArrayToObject(item, "qosMonDatRateReps");
    if (qos_mon_dat_rate_repsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qos_mon_dat_rate_reps]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->qos_mon_dat_rate_reps, node) {
        cJSON *itemLocal = OpenAPI_qos_monitoring_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qos_mon_dat_rate_reps]");
            goto end;
        }
        cJSON_AddItemToArray(qos_mon_dat_rate_repsList, itemLocal);
    }
    }

    if (events_subsc_put_data->pdv_mon_reports) {
    cJSON *pdv_mon_reportsList = cJSON_AddArrayToObject(item, "pdvMonReports");
    if (pdv_mon_reportsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [pdv_mon_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->pdv_mon_reports, node) {
        cJSON *itemLocal = OpenAPI_pdv_monitoring_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [pdv_mon_reports]");
            goto end;
        }
        cJSON_AddItemToArray(pdv_mon_reportsList, itemLocal);
    }
    }

    if (events_subsc_put_data->congest_reports) {
    cJSON *congest_reportsList = cJSON_AddArrayToObject(item, "congestReports");
    if (congest_reportsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [congest_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->congest_reports, node) {
        cJSON *itemLocal = OpenAPI_qos_monitoring_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [congest_reports]");
            goto end;
        }
        cJSON_AddItemToArray(congest_reportsList, itemLocal);
    }
    }

    if (events_subsc_put_data->rtt_mon_reports) {
    cJSON *rtt_mon_reportsList = cJSON_AddArrayToObject(item, "rttMonReports");
    if (rtt_mon_reportsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [rtt_mon_reports]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->rtt_mon_reports, node) {
        cJSON *itemLocal = OpenAPI_qos_monitoring_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [rtt_mon_reports]");
            goto end;
        }
        cJSON_AddItemToArray(rtt_mon_reportsList, itemLocal);
    }
    }

    if (events_subsc_put_data->qos_mon_cap_repos) {
    cJSON *qos_mon_cap_repos = cJSON_AddObjectToObject(item, "qosMonCapRepos");
    if (qos_mon_cap_repos == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qos_mon_cap_repos]");
        goto end;
    }
    cJSON *localMapObject = qos_mon_cap_repos;
    if (events_subsc_put_data->qos_mon_cap_repos) {
        OpenAPI_list_for_each(events_subsc_put_data->qos_mon_cap_repos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qos_mon_cap_repos]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qos_mon_cap_repos]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_capability_report_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (events_subsc_put_data->ran_nas_rel_causes) {
    cJSON *ran_nas_rel_causesList = cJSON_AddArrayToObject(item, "ranNasRelCauses");
    if (ran_nas_rel_causesList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ran_nas_rel_causes]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->ran_nas_rel_causes, node) {
        cJSON *itemLocal = OpenAPI_ran_nas_rel_cause_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ran_nas_rel_causes]");
            goto end;
        }
        cJSON_AddItemToArray(ran_nas_rel_causesList, itemLocal);
    }
    }

    if (events_subsc_put_data->rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(events_subsc_put_data->rat_type)) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (events_subsc_put_data->sat_backhaul_category != OpenAPI_satellite_backhaul_category_NULL) {
    if (cJSON_AddStringToObject(item, "satBackhaulCategory", OpenAPI_satellite_backhaul_category_ToString(events_subsc_put_data->sat_backhaul_category)) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [sat_backhaul_category]");
        goto end;
    }
    }

    if (events_subsc_put_data->ue_loc) {
    cJSON *ue_loc_local_JSON = OpenAPI_user_location_convertToJSON(events_subsc_put_data->ue_loc);
    if (ue_loc_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ue_loc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLoc", ue_loc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ue_loc]");
        goto end;
    }
    }

    if (events_subsc_put_data->ue_loc_time) {
    if (cJSON_AddStringToObject(item, "ueLocTime", events_subsc_put_data->ue_loc_time) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ue_loc_time]");
        goto end;
    }
    }

    if (events_subsc_put_data->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", events_subsc_put_data->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (events_subsc_put_data->usg_rep) {
    cJSON *usg_rep_local_JSON = OpenAPI_accumulated_usage_convertToJSON(events_subsc_put_data->usg_rep);
    if (usg_rep_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [usg_rep]");
        goto end;
    }
    cJSON_AddItemToObject(item, "usgRep", usg_rep_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [usg_rep]");
        goto end;
    }
    }

    if (events_subsc_put_data->ursp_enf_rep) {
    if (cJSON_AddStringToObject(item, "urspEnfRep", events_subsc_put_data->ursp_enf_rep) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ursp_enf_rep]");
        goto end;
    }
    }

    if (events_subsc_put_data->ssc_mode != OpenAPI_ssc_mode_NULL) {
    if (cJSON_AddStringToObject(item, "sscMode", OpenAPI_ssc_mode_ToString(events_subsc_put_data->ssc_mode)) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ssc_mode]");
        goto end;
    }
    }

    if (events_subsc_put_data->ue_req_dnn) {
    if (cJSON_AddStringToObject(item, "ueReqDnn", events_subsc_put_data->ue_req_dnn) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ue_req_dnn]");
        goto end;
    }
    }

    if (events_subsc_put_data->ue_req_pdu_session_type != OpenAPI_pdu_session_type_NULL) {
    if (cJSON_AddStringToObject(item, "ueReqPduSessionType", OpenAPI_pdu_session_type_ToString(events_subsc_put_data->ue_req_pdu_session_type)) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ue_req_pdu_session_type]");
        goto end;
    }
    }

    if (events_subsc_put_data->tsn_bridge_man_cont) {
    cJSON *tsn_bridge_man_cont_local_JSON = OpenAPI_bridge_management_container_convertToJSON(events_subsc_put_data->tsn_bridge_man_cont);
    if (tsn_bridge_man_cont_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnBridgeManCont", tsn_bridge_man_cont_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    }

    if (events_subsc_put_data->tsn_port_man_cont_dstt) {
    cJSON *tsn_port_man_cont_dstt_local_JSON = OpenAPI_port_management_container_convertToJSON(events_subsc_put_data->tsn_port_man_cont_dstt);
    if (tsn_port_man_cont_dstt_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnPortManContDstt", tsn_port_man_cont_dstt_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    }

    if (events_subsc_put_data->tsn_port_man_cont_nwtts) {
    cJSON *tsn_port_man_cont_nwttsList = cJSON_AddArrayToObject(item, "tsnPortManContNwtts");
    if (tsn_port_man_cont_nwttsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [tsn_port_man_cont_nwtts]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->tsn_port_man_cont_nwtts, node) {
        cJSON *itemLocal = OpenAPI_port_management_container_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }
        cJSON_AddItemToArray(tsn_port_man_cont_nwttsList, itemLocal);
    }
    }

    if (events_subsc_put_data->ipv4_addr_list) {
    cJSON *ipv4_addr_listList = cJSON_AddArrayToObject(item, "ipv4AddrList");
    if (ipv4_addr_listList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ipv4_addr_list]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->ipv4_addr_list, node) {
        if (cJSON_AddStringToObject(ipv4_addr_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ipv4_addr_list]");
            goto end;
        }
    }
    }

    if (events_subsc_put_data->ipv6_prefix_list) {
    cJSON *ipv6_prefix_listList = cJSON_AddArrayToObject(item, "ipv6PrefixList");
    if (ipv6_prefix_listList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ipv6_prefix_list]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_put_data->ipv6_prefix_list, node) {
        if (cJSON_AddStringToObject(ipv6_prefix_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ipv6_prefix_list]");
            goto end;
        }
    }
    }

    if (events_subsc_put_data->bat_offset_info) {
    cJSON *bat_offset_info_local_JSON = OpenAPI_bat_offset_info_convertToJSON(events_subsc_put_data->bat_offset_info);
    if (bat_offset_info_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [bat_offset_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "batOffsetInfo", bat_offset_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [bat_offset_info]");
        goto end;
    }
    }

    if (events_subsc_put_data->ue_reach_status != OpenAPI_ue_reachability_status_NULL) {
    if (cJSON_AddStringToObject(item, "ueReachStatus", OpenAPI_ue_reachability_status_ToString(events_subsc_put_data->ue_reach_status)) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ue_reach_status]");
        goto end;
    }
    }

    if (events_subsc_put_data->is_retry_after) {
    if (cJSON_AddNumberToObject(item, "retryAfter", events_subsc_put_data->retry_after) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [retry_after]");
        goto end;
    }
    }

    if (events_subsc_put_data->serv_sat_id) {
    if (cJSON_AddStringToObject(item, "servSatId", events_subsc_put_data->serv_sat_id) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [serv_sat_id]");
        goto end;
    }
    }

    if (events_subsc_put_data->rate_limit_repo) {
    cJSON *rate_limit_repo_local_JSON = OpenAPI_rate_limit_repo_convertToJSON(events_subsc_put_data->rate_limit_repo);
    if (rate_limit_repo_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [rate_limit_repo]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rateLimitRepo", rate_limit_repo_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [rate_limit_repo]");
        goto end;
    }
    }

    if (events_subsc_put_data->dnai_chg_type != OpenAPI_dnai_change_type_NULL) {
    if (cJSON_AddStringToObject(item, "dnaiChgType", OpenAPI_dnai_change_type_ToString(events_subsc_put_data->dnai_chg_type)) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [dnai_chg_type]");
        goto end;
    }
    }

    if (events_subsc_put_data->source_dnai) {
    if (cJSON_AddStringToObject(item, "sourceDnai", events_subsc_put_data->source_dnai) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [source_dnai]");
        goto end;
    }
    }

    if (events_subsc_put_data->target_dnai) {
    if (cJSON_AddStringToObject(item, "targetDnai", events_subsc_put_data->target_dnai) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [target_dnai]");
        goto end;
    }
    }

    if (events_subsc_put_data->session_recov_status != OpenAPI_pc_session_recovery_status_NULL) {
    if (cJSON_AddStringToObject(item, "sessionRecovStatus", OpenAPI_pc_session_recovery_status_ToString(events_subsc_put_data->session_recov_status)) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [session_recov_status]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_events_subsc_put_data_t *OpenAPI_events_subsc_put_data_parseFromJSON(cJSON *events_subsc_put_dataJSON)
{
    OpenAPI_events_subsc_put_data_t *events_subsc_put_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *events = NULL;
    OpenAPI_list_t *eventsList = NULL;
    cJSON *notif_uri = NULL;
    cJSON *req_qos_mon_params = NULL;
    OpenAPI_list_t *req_qos_mon_paramsList = NULL;
    cJSON *qos_mon = NULL;
    OpenAPI_qos_monitoring_information_t *qos_mon_local_nonprim = NULL;
    cJSON *qos_mon_dat_rate = NULL;
    OpenAPI_qos_monitoring_information_t *qos_mon_dat_rate_local_nonprim = NULL;
    cJSON *pdv_req_mon_params = NULL;
    OpenAPI_list_t *pdv_req_mon_paramsList = NULL;
    cJSON *pdv_mon = NULL;
    OpenAPI_qos_monitoring_information_t *pdv_mon_local_nonprim = NULL;
    cJSON *congest_mon = NULL;
    OpenAPI_qos_monitoring_information_t *congest_mon_local_nonprim = NULL;
    cJSON *rtt_mon = NULL;
    OpenAPI_qos_monitoring_information_t *rtt_mon_local_nonprim = NULL;
    cJSON *avl_bit_rate_mon = NULL;
    OpenAPI_qos_monitoring_information_t *avl_bit_rate_mon_local_nonprim = NULL;
    cJSON *rtt_flow_ref = NULL;
    OpenAPI_rtt_flow_reference_t *rtt_flow_ref_local_nonprim = NULL;
    cJSON *req_anis = NULL;
    OpenAPI_list_t *req_anisList = NULL;
    cJSON *usg_thres = NULL;
    OpenAPI_usage_threshold_t *usg_thres_local_nonprim = NULL;
    cJSON *notif_corre_id = NULL;
    cJSON *af_app_ids = NULL;
    OpenAPI_list_t *af_app_idsList = NULL;
    cJSON *direct_notif_ind = NULL;
    cJSON *avrg_wndw = NULL;
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
    events = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "events");
    if (!events) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [events]");
        goto end;
    }
        cJSON *events_local = NULL;
        if (!cJSON_IsArray(events)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [events]");
            goto end;
        }

        eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(events_local, events) {
            if (!cJSON_IsObject(events_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [events]");
                goto end;
            }
            OpenAPI_af_event_subscription_t *eventsItem = OpenAPI_af_event_subscription_parseFromJSON(events_local);
            if (!eventsItem) {
                ogs_error("No eventsItem");
                goto end;
            }
            OpenAPI_list_add(eventsList, eventsItem);
        }

    notif_uri = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "notifUri");
    if (notif_uri) {
    if (!cJSON_IsString(notif_uri) && !cJSON_IsNull(notif_uri)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    }

    req_qos_mon_params = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "reqQosMonParams");
    if (req_qos_mon_params) {
        cJSON *req_qos_mon_params_local = NULL;
        if (!cJSON_IsArray(req_qos_mon_params)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [req_qos_mon_params]");
            goto end;
        }

        req_qos_mon_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_qos_mon_params_local, req_qos_mon_params) {
            OpenAPI_requested_qos_monitoring_parameter_e localEnum = OpenAPI_requested_qos_monitoring_parameter_NULL;
            if (!cJSON_IsString(req_qos_mon_params_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [req_qos_mon_params]");
                goto end;
            }
            localEnum = OpenAPI_requested_qos_monitoring_parameter_FromString(req_qos_mon_params_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"req_qos_mon_params\" is not supported. Ignoring it ...",
                         req_qos_mon_params_local->valuestring);
            } else {
                OpenAPI_list_add(req_qos_mon_paramsList, (void *)localEnum);
            }
        }
        if (req_qos_mon_paramsList->count == 0) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed: Expected req_qos_mon_paramsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    qos_mon = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "qosMon");
    if (qos_mon) {
    qos_mon_local_nonprim = OpenAPI_qos_monitoring_information_parseFromJSON(qos_mon);
    if (!qos_mon_local_nonprim) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON failed [qos_mon]");
        goto end;
    }
    }

    qos_mon_dat_rate = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "qosMonDatRate");
    if (qos_mon_dat_rate) {
    qos_mon_dat_rate_local_nonprim = OpenAPI_qos_monitoring_information_parseFromJSON(qos_mon_dat_rate);
    if (!qos_mon_dat_rate_local_nonprim) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON failed [qos_mon_dat_rate]");
        goto end;
    }
    }

    pdv_req_mon_params = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "pdvReqMonParams");
    if (pdv_req_mon_params) {
        cJSON *pdv_req_mon_params_local = NULL;
        if (!cJSON_IsArray(pdv_req_mon_params)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [pdv_req_mon_params]");
            goto end;
        }

        pdv_req_mon_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdv_req_mon_params_local, pdv_req_mon_params) {
            OpenAPI_requested_qos_monitoring_parameter_e localEnum = OpenAPI_requested_qos_monitoring_parameter_NULL;
            if (!cJSON_IsString(pdv_req_mon_params_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [pdv_req_mon_params]");
                goto end;
            }
            localEnum = OpenAPI_requested_qos_monitoring_parameter_FromString(pdv_req_mon_params_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"pdv_req_mon_params\" is not supported. Ignoring it ...",
                         pdv_req_mon_params_local->valuestring);
            } else {
                OpenAPI_list_add(pdv_req_mon_paramsList, (void *)localEnum);
            }
        }
        if (pdv_req_mon_paramsList->count == 0) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed: Expected pdv_req_mon_paramsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    pdv_mon = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "pdvMon");
    if (pdv_mon) {
    pdv_mon_local_nonprim = OpenAPI_qos_monitoring_information_parseFromJSON(pdv_mon);
    if (!pdv_mon_local_nonprim) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON failed [pdv_mon]");
        goto end;
    }
    }

    congest_mon = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "congestMon");
    if (congest_mon) {
    congest_mon_local_nonprim = OpenAPI_qos_monitoring_information_parseFromJSON(congest_mon);
    if (!congest_mon_local_nonprim) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON failed [congest_mon]");
        goto end;
    }
    }

    rtt_mon = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "rttMon");
    if (rtt_mon) {
    rtt_mon_local_nonprim = OpenAPI_qos_monitoring_information_parseFromJSON(rtt_mon);
    if (!rtt_mon_local_nonprim) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON failed [rtt_mon]");
        goto end;
    }
    }

    avl_bit_rate_mon = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "avlBitRateMon");
    if (avl_bit_rate_mon) {
    avl_bit_rate_mon_local_nonprim = OpenAPI_qos_monitoring_information_parseFromJSON(avl_bit_rate_mon);
    if (!avl_bit_rate_mon_local_nonprim) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON failed [avl_bit_rate_mon]");
        goto end;
    }
    }

    rtt_flow_ref = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "rttFlowRef");
    if (rtt_flow_ref) {
    rtt_flow_ref_local_nonprim = OpenAPI_rtt_flow_reference_parseFromJSON(rtt_flow_ref);
    if (!rtt_flow_ref_local_nonprim) {
        ogs_error("OpenAPI_rtt_flow_reference_parseFromJSON failed [rtt_flow_ref]");
        goto end;
    }
    }

    req_anis = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "reqAnis");
    if (req_anis) {
        cJSON *req_anis_local = NULL;
        if (!cJSON_IsArray(req_anis)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [req_anis]");
            goto end;
        }

        req_anisList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_anis_local, req_anis) {
            OpenAPI_required_access_info_e localEnum = OpenAPI_required_access_info_NULL;
            if (!cJSON_IsString(req_anis_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [req_anis]");
                goto end;
            }
            localEnum = OpenAPI_required_access_info_FromString(req_anis_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"req_anis\" is not supported. Ignoring it ...",
                         req_anis_local->valuestring);
            } else {
                OpenAPI_list_add(req_anisList, (void *)localEnum);
            }
        }
        if (req_anisList->count == 0) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed: Expected req_anisList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    usg_thres = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "usgThres");
    if (usg_thres) {
    usg_thres_local_nonprim = OpenAPI_usage_threshold_parseFromJSON(usg_thres);
    if (!usg_thres_local_nonprim) {
        ogs_error("OpenAPI_usage_threshold_parseFromJSON failed [usg_thres]");
        goto end;
    }
    }

    notif_corre_id = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "notifCorreId");
    if (notif_corre_id) {
    if (!cJSON_IsString(notif_corre_id) && !cJSON_IsNull(notif_corre_id)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }
    }

    af_app_ids = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "afAppIds");
    if (af_app_ids) {
        cJSON *af_app_ids_local = NULL;
        if (!cJSON_IsArray(af_app_ids)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [af_app_ids]");
            goto end;
        }

        af_app_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(af_app_ids_local, af_app_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(af_app_ids_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [af_app_ids]");
                goto end;
            }
            OpenAPI_list_add(af_app_idsList, ogs_strdup(af_app_ids_local->valuestring));
        }
    }

    direct_notif_ind = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "directNotifInd");
    if (direct_notif_ind) {
    if (!cJSON_IsBool(direct_notif_ind)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [direct_notif_ind]");
        goto end;
    }
    }

    avrg_wndw = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "avrgWndw");
    if (avrg_wndw) {
    if (!cJSON_IsNumber(avrg_wndw)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [avrg_wndw]");
        goto end;
    }
    }

    ad_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "adReports");
    if (ad_reports) {
        cJSON *ad_reports_local = NULL;
        if (!cJSON_IsArray(ad_reports)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ad_reports]");
            goto end;
        }

        ad_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ad_reports_local, ad_reports) {
            if (!cJSON_IsObject(ad_reports_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ad_reports]");
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

    access_type = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "accessType");
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    add_access_info = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "addAccessInfo");
    if (add_access_info) {
    add_access_info_local_nonprim = OpenAPI_additional_access_info_parseFromJSON(add_access_info);
    if (!add_access_info_local_nonprim) {
        ogs_error("OpenAPI_additional_access_info_parseFromJSON failed [add_access_info]");
        goto end;
    }
    }

    rel_access_info = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "relAccessInfo");
    if (rel_access_info) {
    rel_access_info_local_nonprim = OpenAPI_additional_access_info_parseFromJSON(rel_access_info);
    if (!rel_access_info_local_nonprim) {
        ogs_error("OpenAPI_additional_access_info_parseFromJSON failed [rel_access_info]");
        goto end;
    }
    }

    an_charg_addr = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "anChargAddr");
    if (an_charg_addr) {
    if (!cJSON_IsNull(an_charg_addr)) {
    an_charg_addr_local_nonprim = OpenAPI_acc_net_charging_address_parseFromJSON(an_charg_addr);
    if (!an_charg_addr_local_nonprim) {
        ogs_error("OpenAPI_acc_net_charging_address_parseFromJSON failed [an_charg_addr]");
        goto end;
    }
    }
    }

    an_charg_ids = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "anChargIds");
    if (an_charg_ids) {
        cJSON *an_charg_ids_local = NULL;
        if (!cJSON_IsArray(an_charg_ids)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [an_charg_ids]");
            goto end;
        }

        an_charg_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(an_charg_ids_local, an_charg_ids) {
            if (!cJSON_IsObject(an_charg_ids_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [an_charg_ids]");
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

    an_gw_addr = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "anGwAddr");
    if (an_gw_addr) {
    if (!cJSON_IsNull(an_gw_addr)) {
    an_gw_addr_local_nonprim = OpenAPI_an_gw_address_parseFromJSON(an_gw_addr);
    if (!an_gw_addr_local_nonprim) {
        ogs_error("OpenAPI_an_gw_address_parseFromJSON failed [an_gw_addr]");
        goto end;
    }
    }
    }

    l4s_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "l4sReports");
    if (l4s_reports) {
        cJSON *l4s_reports_local = NULL;
        if (!cJSON_IsArray(l4s_reports)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [l4s_reports]");
            goto end;
        }

        l4s_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(l4s_reports_local, l4s_reports) {
            if (!cJSON_IsObject(l4s_reports_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [l4s_reports]");
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

    ev_subs_uri = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "evSubsUri");
    if (!ev_subs_uri) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ev_subs_uri]");
        goto end;
    }
    if (!cJSON_IsString(ev_subs_uri)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ev_subs_uri]");
        goto end;
    }

    ev_notifs = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "evNotifs");
    if (!ev_notifs) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ev_notifs]");
        goto end;
    }
        cJSON *ev_notifs_local = NULL;
        if (!cJSON_IsArray(ev_notifs)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ev_notifs]");
            goto end;
        }

        ev_notifsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ev_notifs_local, ev_notifs) {
            if (!cJSON_IsObject(ev_notifs_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ev_notifs]");
                goto end;
            }
            OpenAPI_af_event_notification_t *ev_notifsItem = OpenAPI_af_event_notification_parseFromJSON(ev_notifs_local);
            if (!ev_notifsItem) {
                ogs_error("No ev_notifsItem");
                goto end;
            }
            OpenAPI_list_add(ev_notifsList, ev_notifsItem);
        }

    failed_resourc_alloc_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "failedResourcAllocReports");
    if (failed_resourc_alloc_reports) {
        cJSON *failed_resourc_alloc_reports_local = NULL;
        if (!cJSON_IsArray(failed_resourc_alloc_reports)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [failed_resourc_alloc_reports]");
            goto end;
        }

        failed_resourc_alloc_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(failed_resourc_alloc_reports_local, failed_resourc_alloc_reports) {
            if (!cJSON_IsObject(failed_resourc_alloc_reports_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [failed_resourc_alloc_reports]");
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

    succ_resourc_alloc_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "succResourcAllocReports");
    if (succ_resourc_alloc_reports) {
        cJSON *succ_resourc_alloc_reports_local = NULL;
        if (!cJSON_IsArray(succ_resourc_alloc_reports)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [succ_resourc_alloc_reports]");
            goto end;
        }

        succ_resourc_alloc_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(succ_resourc_alloc_reports_local, succ_resourc_alloc_reports) {
            if (!cJSON_IsObject(succ_resourc_alloc_reports_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [succ_resourc_alloc_reports]");
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

    no_net_loc_supp = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "noNetLocSupp");
    if (no_net_loc_supp) {
    if (!cJSON_IsString(no_net_loc_supp)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [no_net_loc_supp]");
        goto end;
    }
    no_net_loc_suppVariable = OpenAPI_net_loc_access_support_FromString(no_net_loc_supp->valuestring);
    }

    out_of_cred_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "outOfCredReports");
    if (out_of_cred_reports) {
        cJSON *out_of_cred_reports_local = NULL;
        if (!cJSON_IsArray(out_of_cred_reports)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [out_of_cred_reports]");
            goto end;
        }

        out_of_cred_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(out_of_cred_reports_local, out_of_cred_reports) {
            if (!cJSON_IsObject(out_of_cred_reports_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [out_of_cred_reports]");
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

    plmn_id = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    qnc_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "qncReports");
    if (qnc_reports) {
        cJSON *qnc_reports_local = NULL;
        if (!cJSON_IsArray(qnc_reports)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [qnc_reports]");
            goto end;
        }

        qnc_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qnc_reports_local, qnc_reports) {
            if (!cJSON_IsObject(qnc_reports_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [qnc_reports]");
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

    qos_mon_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "qosMonReports");
    if (qos_mon_reports) {
        cJSON *qos_mon_reports_local = NULL;
        if (!cJSON_IsArray(qos_mon_reports)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [qos_mon_reports]");
            goto end;
        }

        qos_mon_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_mon_reports_local, qos_mon_reports) {
            if (!cJSON_IsObject(qos_mon_reports_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [qos_mon_reports]");
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

    qos_mon_dat_rate_reps = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "qosMonDatRateReps");
    if (qos_mon_dat_rate_reps) {
        cJSON *qos_mon_dat_rate_reps_local = NULL;
        if (!cJSON_IsArray(qos_mon_dat_rate_reps)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [qos_mon_dat_rate_reps]");
            goto end;
        }

        qos_mon_dat_rate_repsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_mon_dat_rate_reps_local, qos_mon_dat_rate_reps) {
            if (!cJSON_IsObject(qos_mon_dat_rate_reps_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [qos_mon_dat_rate_reps]");
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

    pdv_mon_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "pdvMonReports");
    if (pdv_mon_reports) {
        cJSON *pdv_mon_reports_local = NULL;
        if (!cJSON_IsArray(pdv_mon_reports)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [pdv_mon_reports]");
            goto end;
        }

        pdv_mon_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdv_mon_reports_local, pdv_mon_reports) {
            if (!cJSON_IsObject(pdv_mon_reports_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [pdv_mon_reports]");
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

    congest_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "congestReports");
    if (congest_reports) {
        cJSON *congest_reports_local = NULL;
        if (!cJSON_IsArray(congest_reports)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [congest_reports]");
            goto end;
        }

        congest_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(congest_reports_local, congest_reports) {
            if (!cJSON_IsObject(congest_reports_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [congest_reports]");
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

    rtt_mon_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "rttMonReports");
    if (rtt_mon_reports) {
        cJSON *rtt_mon_reports_local = NULL;
        if (!cJSON_IsArray(rtt_mon_reports)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [rtt_mon_reports]");
            goto end;
        }

        rtt_mon_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(rtt_mon_reports_local, rtt_mon_reports) {
            if (!cJSON_IsObject(rtt_mon_reports_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [rtt_mon_reports]");
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

    qos_mon_cap_repos = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "qosMonCapRepos");
    if (qos_mon_cap_repos) {
        cJSON *qos_mon_cap_repos_local_map = NULL;
        if (!cJSON_IsObject(qos_mon_cap_repos) && !cJSON_IsNull(qos_mon_cap_repos)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [qos_mon_cap_repos]");
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
                    ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(qos_mon_cap_reposList, localMapKeyPair);
            }
        }
    }

    ran_nas_rel_causes = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ranNasRelCauses");
    if (ran_nas_rel_causes) {
        cJSON *ran_nas_rel_causes_local = NULL;
        if (!cJSON_IsArray(ran_nas_rel_causes)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ran_nas_rel_causes]");
            goto end;
        }

        ran_nas_rel_causesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ran_nas_rel_causes_local, ran_nas_rel_causes) {
            if (!cJSON_IsObject(ran_nas_rel_causes_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ran_nas_rel_causes]");
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

    rat_type = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ratType");
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    sat_backhaul_category = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "satBackhaulCategory");
    if (sat_backhaul_category) {
    if (!cJSON_IsString(sat_backhaul_category)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [sat_backhaul_category]");
        goto end;
    }
    sat_backhaul_categoryVariable = OpenAPI_satellite_backhaul_category_FromString(sat_backhaul_category->valuestring);
    }

    ue_loc = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ueLoc");
    if (ue_loc) {
    ue_loc_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_loc);
    if (!ue_loc_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_loc]");
        goto end;
    }
    }

    ue_loc_time = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ueLocTime");
    if (ue_loc_time) {
    if (!cJSON_IsString(ue_loc_time) && !cJSON_IsNull(ue_loc_time)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ue_loc_time]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    usg_rep = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "usgRep");
    if (usg_rep) {
    usg_rep_local_nonprim = OpenAPI_accumulated_usage_parseFromJSON(usg_rep);
    if (!usg_rep_local_nonprim) {
        ogs_error("OpenAPI_accumulated_usage_parseFromJSON failed [usg_rep]");
        goto end;
    }
    }

    ursp_enf_rep = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "urspEnfRep");
    if (ursp_enf_rep) {
    if (!cJSON_IsString(ursp_enf_rep) && !cJSON_IsNull(ursp_enf_rep)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ursp_enf_rep]");
        goto end;
    }
    }

    ssc_mode = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "sscMode");
    if (ssc_mode) {
    if (!cJSON_IsString(ssc_mode)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ssc_mode]");
        goto end;
    }
    ssc_modeVariable = OpenAPI_ssc_mode_FromString(ssc_mode->valuestring);
    }

    ue_req_dnn = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ueReqDnn");
    if (ue_req_dnn) {
    if (!cJSON_IsString(ue_req_dnn) && !cJSON_IsNull(ue_req_dnn)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ue_req_dnn]");
        goto end;
    }
    }

    ue_req_pdu_session_type = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ueReqPduSessionType");
    if (ue_req_pdu_session_type) {
    if (!cJSON_IsString(ue_req_pdu_session_type)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ue_req_pdu_session_type]");
        goto end;
    }
    ue_req_pdu_session_typeVariable = OpenAPI_pdu_session_type_FromString(ue_req_pdu_session_type->valuestring);
    }

    tsn_bridge_man_cont = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "tsnBridgeManCont");
    if (tsn_bridge_man_cont) {
    tsn_bridge_man_cont_local_nonprim = OpenAPI_bridge_management_container_parseFromJSON(tsn_bridge_man_cont);
    if (!tsn_bridge_man_cont_local_nonprim) {
        ogs_error("OpenAPI_bridge_management_container_parseFromJSON failed [tsn_bridge_man_cont]");
        goto end;
    }
    }

    tsn_port_man_cont_dstt = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "tsnPortManContDstt");
    if (tsn_port_man_cont_dstt) {
    tsn_port_man_cont_dstt_local_nonprim = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_dstt);
    if (!tsn_port_man_cont_dstt_local_nonprim) {
        ogs_error("OpenAPI_port_management_container_parseFromJSON failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    }

    tsn_port_man_cont_nwtts = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "tsnPortManContNwtts");
    if (tsn_port_man_cont_nwtts) {
        cJSON *tsn_port_man_cont_nwtts_local = NULL;
        if (!cJSON_IsArray(tsn_port_man_cont_nwtts)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }

        tsn_port_man_cont_nwttsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tsn_port_man_cont_nwtts_local, tsn_port_man_cont_nwtts) {
            if (!cJSON_IsObject(tsn_port_man_cont_nwtts_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
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

    ipv4_addr_list = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ipv4AddrList");
    if (ipv4_addr_list) {
        cJSON *ipv4_addr_list_local = NULL;
        if (!cJSON_IsArray(ipv4_addr_list)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ipv4_addr_list]");
            goto end;
        }

        ipv4_addr_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_addr_list_local, ipv4_addr_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv4_addr_list_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ipv4_addr_list]");
                goto end;
            }
            OpenAPI_list_add(ipv4_addr_listList, ogs_strdup(ipv4_addr_list_local->valuestring));
        }
    }

    ipv6_prefix_list = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ipv6PrefixList");
    if (ipv6_prefix_list) {
        cJSON *ipv6_prefix_list_local = NULL;
        if (!cJSON_IsArray(ipv6_prefix_list)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ipv6_prefix_list]");
            goto end;
        }

        ipv6_prefix_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_prefix_list_local, ipv6_prefix_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv6_prefix_list_local)) {
                ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ipv6_prefix_list]");
                goto end;
            }
            OpenAPI_list_add(ipv6_prefix_listList, ogs_strdup(ipv6_prefix_list_local->valuestring));
        }
    }

    bat_offset_info = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "batOffsetInfo");
    if (bat_offset_info) {
    bat_offset_info_local_nonprim = OpenAPI_bat_offset_info_parseFromJSON(bat_offset_info);
    if (!bat_offset_info_local_nonprim) {
        ogs_error("OpenAPI_bat_offset_info_parseFromJSON failed [bat_offset_info]");
        goto end;
    }
    }

    ue_reach_status = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ueReachStatus");
    if (ue_reach_status) {
    if (!cJSON_IsString(ue_reach_status)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ue_reach_status]");
        goto end;
    }
    ue_reach_statusVariable = OpenAPI_ue_reachability_status_FromString(ue_reach_status->valuestring);
    }

    retry_after = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "retryAfter");
    if (retry_after) {
    if (!cJSON_IsNumber(retry_after)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [retry_after]");
        goto end;
    }
    }

    serv_sat_id = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "servSatId");
    if (serv_sat_id) {
    if (!cJSON_IsString(serv_sat_id) && !cJSON_IsNull(serv_sat_id)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [serv_sat_id]");
        goto end;
    }
    }

    rate_limit_repo = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "rateLimitRepo");
    if (rate_limit_repo) {
    rate_limit_repo_local_nonprim = OpenAPI_rate_limit_repo_parseFromJSON(rate_limit_repo);
    if (!rate_limit_repo_local_nonprim) {
        ogs_error("OpenAPI_rate_limit_repo_parseFromJSON failed [rate_limit_repo]");
        goto end;
    }
    }

    dnai_chg_type = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "dnaiChgType");
    if (dnai_chg_type) {
    if (!cJSON_IsString(dnai_chg_type)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [dnai_chg_type]");
        goto end;
    }
    dnai_chg_typeVariable = OpenAPI_dnai_change_type_FromString(dnai_chg_type->valuestring);
    }

    source_dnai = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "sourceDnai");
    if (source_dnai) {
    if (!cJSON_IsString(source_dnai) && !cJSON_IsNull(source_dnai)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [source_dnai]");
        goto end;
    }
    }

    target_dnai = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "targetDnai");
    if (target_dnai) {
    if (!cJSON_IsString(target_dnai) && !cJSON_IsNull(target_dnai)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [target_dnai]");
        goto end;
    }
    }

    session_recov_status = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "sessionRecovStatus");
    if (session_recov_status) {
    if (!cJSON_IsString(session_recov_status)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [session_recov_status]");
        goto end;
    }
    session_recov_statusVariable = OpenAPI_pc_session_recovery_status_FromString(session_recov_status->valuestring);
    }

    events_subsc_put_data_local_var = OpenAPI_events_subsc_put_data_create (
        eventsList,
        notif_uri && !cJSON_IsNull(notif_uri) ? ogs_strdup(notif_uri->valuestring) : NULL,
        req_qos_mon_params ? req_qos_mon_paramsList : NULL,
        qos_mon ? qos_mon_local_nonprim : NULL,
        qos_mon_dat_rate ? qos_mon_dat_rate_local_nonprim : NULL,
        pdv_req_mon_params ? pdv_req_mon_paramsList : NULL,
        pdv_mon ? pdv_mon_local_nonprim : NULL,
        congest_mon ? congest_mon_local_nonprim : NULL,
        rtt_mon ? rtt_mon_local_nonprim : NULL,
        avl_bit_rate_mon ? avl_bit_rate_mon_local_nonprim : NULL,
        rtt_flow_ref ? rtt_flow_ref_local_nonprim : NULL,
        req_anis ? req_anisList : NULL,
        usg_thres ? usg_thres_local_nonprim : NULL,
        notif_corre_id && !cJSON_IsNull(notif_corre_id) ? ogs_strdup(notif_corre_id->valuestring) : NULL,
        af_app_ids ? af_app_idsList : NULL,
        direct_notif_ind ? true : false,
        direct_notif_ind ? direct_notif_ind->valueint : 0,
        avrg_wndw ? true : false,
        avrg_wndw ? avrg_wndw->valuedouble : 0,
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

    return events_subsc_put_data_local_var;
end:
    if (eventsList) {
        OpenAPI_list_for_each(eventsList, node) {
            OpenAPI_af_event_subscription_free(node->data);
        }
        OpenAPI_list_free(eventsList);
        eventsList = NULL;
    }
    if (req_qos_mon_paramsList) {
        OpenAPI_list_free(req_qos_mon_paramsList);
        req_qos_mon_paramsList = NULL;
    }
    if (qos_mon_local_nonprim) {
        OpenAPI_qos_monitoring_information_free(qos_mon_local_nonprim);
        qos_mon_local_nonprim = NULL;
    }
    if (qos_mon_dat_rate_local_nonprim) {
        OpenAPI_qos_monitoring_information_free(qos_mon_dat_rate_local_nonprim);
        qos_mon_dat_rate_local_nonprim = NULL;
    }
    if (pdv_req_mon_paramsList) {
        OpenAPI_list_free(pdv_req_mon_paramsList);
        pdv_req_mon_paramsList = NULL;
    }
    if (pdv_mon_local_nonprim) {
        OpenAPI_qos_monitoring_information_free(pdv_mon_local_nonprim);
        pdv_mon_local_nonprim = NULL;
    }
    if (congest_mon_local_nonprim) {
        OpenAPI_qos_monitoring_information_free(congest_mon_local_nonprim);
        congest_mon_local_nonprim = NULL;
    }
    if (rtt_mon_local_nonprim) {
        OpenAPI_qos_monitoring_information_free(rtt_mon_local_nonprim);
        rtt_mon_local_nonprim = NULL;
    }
    if (avl_bit_rate_mon_local_nonprim) {
        OpenAPI_qos_monitoring_information_free(avl_bit_rate_mon_local_nonprim);
        avl_bit_rate_mon_local_nonprim = NULL;
    }
    if (rtt_flow_ref_local_nonprim) {
        OpenAPI_rtt_flow_reference_free(rtt_flow_ref_local_nonprim);
        rtt_flow_ref_local_nonprim = NULL;
    }
    if (req_anisList) {
        OpenAPI_list_free(req_anisList);
        req_anisList = NULL;
    }
    if (usg_thres_local_nonprim) {
        OpenAPI_usage_threshold_free(usg_thres_local_nonprim);
        usg_thres_local_nonprim = NULL;
    }
    if (af_app_idsList) {
        OpenAPI_list_for_each(af_app_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(af_app_idsList);
        af_app_idsList = NULL;
    }
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

OpenAPI_events_subsc_put_data_t *OpenAPI_events_subsc_put_data_copy(OpenAPI_events_subsc_put_data_t *dst, OpenAPI_events_subsc_put_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_events_subsc_put_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed");
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

    OpenAPI_events_subsc_put_data_free(dst);
    dst = OpenAPI_events_subsc_put_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

