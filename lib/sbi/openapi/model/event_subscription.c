
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_subscription.h"

OpenAPI_event_subscription_t *OpenAPI_event_subscription_create(
    bool is_any_slice,
    int any_slice,
    OpenAPI_list_t *app_ids,
    OpenAPI_list_t *dnns,
    OpenAPI_list_t *dnais,
    OpenAPI_nwdaf_event_t *event,
    OpenAPI_event_reporting_requirement_t *extra_report_req,
    OpenAPI_list_t *ladn_dnns,
    bool is_load_level_threshold,
    int load_level_threshold,
    OpenAPI_notification_method_t *notification_method,
    OpenAPI_matching_direction_t *matching_dir,
    OpenAPI_list_t *nf_load_lvl_thds,
    OpenAPI_list_t *nf_instance_ids,
    OpenAPI_list_t *nf_set_ids,
    OpenAPI_list_t *nf_types,
    OpenAPI_network_area_info_t *network_area,
    OpenAPI_list_t *visited_areas,
    bool is_max_top_app_ul_nbr,
    int max_top_app_ul_nbr,
    bool is_max_top_app_dl_nbr,
    int max_top_app_dl_nbr,
    OpenAPI_list_t *nsi_id_infos,
    OpenAPI_list_t *nsi_level_thrds,
    OpenAPI_qos_requirement_t *qos_requ,
    OpenAPI_list_t *qos_flow_ret_thds,
    OpenAPI_list_t *ran_ue_throu_thds,
    bool is_repetition_period,
    int repetition_period,
    OpenAPI_list_t *snssaia,
    OpenAPI_target_ue_information_t *tgt_ue,
    OpenAPI_list_t *cong_thresholds,
    OpenAPI_list_t *nw_perf_requs,
    OpenAPI_list_t *bw_requs,
    OpenAPI_list_t *excep_requs,
    OpenAPI_expected_analytics_type_t *expt_ana_type,
    OpenAPI_expected_ue_behaviour_data_t *expt_ue_behav,
    OpenAPI_list_t *rat_freqs,
    OpenAPI_list_t *list_of_ana_subsets,
    OpenAPI_list_t *disper_reqs,
    OpenAPI_list_t *red_trans_reqs,
    OpenAPI_list_t *wlan_reqs,
    OpenAPI_upf_information_t *upf_info,
    OpenAPI_list_t *app_server_addrs,
    OpenAPI_list_t *dn_perf_reqs
)
{
    OpenAPI_event_subscription_t *event_subscription_local_var = ogs_malloc(sizeof(OpenAPI_event_subscription_t));
    ogs_assert(event_subscription_local_var);

    event_subscription_local_var->is_any_slice = is_any_slice;
    event_subscription_local_var->any_slice = any_slice;
    event_subscription_local_var->app_ids = app_ids;
    event_subscription_local_var->dnns = dnns;
    event_subscription_local_var->dnais = dnais;
    event_subscription_local_var->event = event;
    event_subscription_local_var->extra_report_req = extra_report_req;
    event_subscription_local_var->ladn_dnns = ladn_dnns;
    event_subscription_local_var->is_load_level_threshold = is_load_level_threshold;
    event_subscription_local_var->load_level_threshold = load_level_threshold;
    event_subscription_local_var->notification_method = notification_method;
    event_subscription_local_var->matching_dir = matching_dir;
    event_subscription_local_var->nf_load_lvl_thds = nf_load_lvl_thds;
    event_subscription_local_var->nf_instance_ids = nf_instance_ids;
    event_subscription_local_var->nf_set_ids = nf_set_ids;
    event_subscription_local_var->nf_types = nf_types;
    event_subscription_local_var->network_area = network_area;
    event_subscription_local_var->visited_areas = visited_areas;
    event_subscription_local_var->is_max_top_app_ul_nbr = is_max_top_app_ul_nbr;
    event_subscription_local_var->max_top_app_ul_nbr = max_top_app_ul_nbr;
    event_subscription_local_var->is_max_top_app_dl_nbr = is_max_top_app_dl_nbr;
    event_subscription_local_var->max_top_app_dl_nbr = max_top_app_dl_nbr;
    event_subscription_local_var->nsi_id_infos = nsi_id_infos;
    event_subscription_local_var->nsi_level_thrds = nsi_level_thrds;
    event_subscription_local_var->qos_requ = qos_requ;
    event_subscription_local_var->qos_flow_ret_thds = qos_flow_ret_thds;
    event_subscription_local_var->ran_ue_throu_thds = ran_ue_throu_thds;
    event_subscription_local_var->is_repetition_period = is_repetition_period;
    event_subscription_local_var->repetition_period = repetition_period;
    event_subscription_local_var->snssaia = snssaia;
    event_subscription_local_var->tgt_ue = tgt_ue;
    event_subscription_local_var->cong_thresholds = cong_thresholds;
    event_subscription_local_var->nw_perf_requs = nw_perf_requs;
    event_subscription_local_var->bw_requs = bw_requs;
    event_subscription_local_var->excep_requs = excep_requs;
    event_subscription_local_var->expt_ana_type = expt_ana_type;
    event_subscription_local_var->expt_ue_behav = expt_ue_behav;
    event_subscription_local_var->rat_freqs = rat_freqs;
    event_subscription_local_var->list_of_ana_subsets = list_of_ana_subsets;
    event_subscription_local_var->disper_reqs = disper_reqs;
    event_subscription_local_var->red_trans_reqs = red_trans_reqs;
    event_subscription_local_var->wlan_reqs = wlan_reqs;
    event_subscription_local_var->upf_info = upf_info;
    event_subscription_local_var->app_server_addrs = app_server_addrs;
    event_subscription_local_var->dn_perf_reqs = dn_perf_reqs;

    return event_subscription_local_var;
}

void OpenAPI_event_subscription_free(OpenAPI_event_subscription_t *event_subscription)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == event_subscription) {
        return;
    }
    if (event_subscription->app_ids) {
        OpenAPI_list_for_each(event_subscription->app_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(event_subscription->app_ids);
        event_subscription->app_ids = NULL;
    }
    if (event_subscription->dnns) {
        OpenAPI_list_for_each(event_subscription->dnns, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(event_subscription->dnns);
        event_subscription->dnns = NULL;
    }
    if (event_subscription->dnais) {
        OpenAPI_list_for_each(event_subscription->dnais, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(event_subscription->dnais);
        event_subscription->dnais = NULL;
    }
    if (event_subscription->event) {
        OpenAPI_nwdaf_event_free(event_subscription->event);
        event_subscription->event = NULL;
    }
    if (event_subscription->extra_report_req) {
        OpenAPI_event_reporting_requirement_free(event_subscription->extra_report_req);
        event_subscription->extra_report_req = NULL;
    }
    if (event_subscription->ladn_dnns) {
        OpenAPI_list_for_each(event_subscription->ladn_dnns, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(event_subscription->ladn_dnns);
        event_subscription->ladn_dnns = NULL;
    }
    if (event_subscription->notification_method) {
        OpenAPI_notification_method_free(event_subscription->notification_method);
        event_subscription->notification_method = NULL;
    }
    if (event_subscription->matching_dir) {
        OpenAPI_matching_direction_free(event_subscription->matching_dir);
        event_subscription->matching_dir = NULL;
    }
    if (event_subscription->nf_load_lvl_thds) {
        OpenAPI_list_for_each(event_subscription->nf_load_lvl_thds, node) {
            OpenAPI_threshold_level_free(node->data);
        }
        OpenAPI_list_free(event_subscription->nf_load_lvl_thds);
        event_subscription->nf_load_lvl_thds = NULL;
    }
    if (event_subscription->nf_instance_ids) {
        OpenAPI_list_for_each(event_subscription->nf_instance_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(event_subscription->nf_instance_ids);
        event_subscription->nf_instance_ids = NULL;
    }
    if (event_subscription->nf_set_ids) {
        OpenAPI_list_for_each(event_subscription->nf_set_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(event_subscription->nf_set_ids);
        event_subscription->nf_set_ids = NULL;
    }
    if (event_subscription->nf_types) {
        OpenAPI_list_free(event_subscription->nf_types);
        event_subscription->nf_types = NULL;
    }
    if (event_subscription->network_area) {
        OpenAPI_network_area_info_free(event_subscription->network_area);
        event_subscription->network_area = NULL;
    }
    if (event_subscription->visited_areas) {
        OpenAPI_list_for_each(event_subscription->visited_areas, node) {
            OpenAPI_network_area_info_free(node->data);
        }
        OpenAPI_list_free(event_subscription->visited_areas);
        event_subscription->visited_areas = NULL;
    }
    if (event_subscription->nsi_id_infos) {
        OpenAPI_list_for_each(event_subscription->nsi_id_infos, node) {
            OpenAPI_nsi_id_info_free(node->data);
        }
        OpenAPI_list_free(event_subscription->nsi_id_infos);
        event_subscription->nsi_id_infos = NULL;
    }
    if (event_subscription->nsi_level_thrds) {
        OpenAPI_list_for_each(event_subscription->nsi_level_thrds, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(event_subscription->nsi_level_thrds);
        event_subscription->nsi_level_thrds = NULL;
    }
    if (event_subscription->qos_requ) {
        OpenAPI_qos_requirement_free(event_subscription->qos_requ);
        event_subscription->qos_requ = NULL;
    }
    if (event_subscription->qos_flow_ret_thds) {
        OpenAPI_list_for_each(event_subscription->qos_flow_ret_thds, node) {
            OpenAPI_retainability_threshold_free(node->data);
        }
        OpenAPI_list_free(event_subscription->qos_flow_ret_thds);
        event_subscription->qos_flow_ret_thds = NULL;
    }
    if (event_subscription->ran_ue_throu_thds) {
        OpenAPI_list_for_each(event_subscription->ran_ue_throu_thds, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(event_subscription->ran_ue_throu_thds);
        event_subscription->ran_ue_throu_thds = NULL;
    }
    if (event_subscription->snssaia) {
        OpenAPI_list_for_each(event_subscription->snssaia, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(event_subscription->snssaia);
        event_subscription->snssaia = NULL;
    }
    if (event_subscription->tgt_ue) {
        OpenAPI_target_ue_information_free(event_subscription->tgt_ue);
        event_subscription->tgt_ue = NULL;
    }
    if (event_subscription->cong_thresholds) {
        OpenAPI_list_for_each(event_subscription->cong_thresholds, node) {
            OpenAPI_threshold_level_free(node->data);
        }
        OpenAPI_list_free(event_subscription->cong_thresholds);
        event_subscription->cong_thresholds = NULL;
    }
    if (event_subscription->nw_perf_requs) {
        OpenAPI_list_for_each(event_subscription->nw_perf_requs, node) {
            OpenAPI_network_perf_requirement_free(node->data);
        }
        OpenAPI_list_free(event_subscription->nw_perf_requs);
        event_subscription->nw_perf_requs = NULL;
    }
    if (event_subscription->bw_requs) {
        OpenAPI_list_for_each(event_subscription->bw_requs, node) {
            OpenAPI_bw_requirement_free(node->data);
        }
        OpenAPI_list_free(event_subscription->bw_requs);
        event_subscription->bw_requs = NULL;
    }
    if (event_subscription->excep_requs) {
        OpenAPI_list_for_each(event_subscription->excep_requs, node) {
            OpenAPI_exception_free(node->data);
        }
        OpenAPI_list_free(event_subscription->excep_requs);
        event_subscription->excep_requs = NULL;
    }
    if (event_subscription->expt_ana_type) {
        OpenAPI_expected_analytics_type_free(event_subscription->expt_ana_type);
        event_subscription->expt_ana_type = NULL;
    }
    if (event_subscription->expt_ue_behav) {
        OpenAPI_expected_ue_behaviour_data_free(event_subscription->expt_ue_behav);
        event_subscription->expt_ue_behav = NULL;
    }
    if (event_subscription->rat_freqs) {
        OpenAPI_list_for_each(event_subscription->rat_freqs, node) {
            OpenAPI_rat_freq_information_free(node->data);
        }
        OpenAPI_list_free(event_subscription->rat_freqs);
        event_subscription->rat_freqs = NULL;
    }
    if (event_subscription->list_of_ana_subsets) {
        OpenAPI_list_for_each(event_subscription->list_of_ana_subsets, node) {
            OpenAPI_analytics_subset_free(node->data);
        }
        OpenAPI_list_free(event_subscription->list_of_ana_subsets);
        event_subscription->list_of_ana_subsets = NULL;
    }
    if (event_subscription->disper_reqs) {
        OpenAPI_list_for_each(event_subscription->disper_reqs, node) {
            OpenAPI_dispersion_requirement_free(node->data);
        }
        OpenAPI_list_free(event_subscription->disper_reqs);
        event_subscription->disper_reqs = NULL;
    }
    if (event_subscription->red_trans_reqs) {
        OpenAPI_list_for_each(event_subscription->red_trans_reqs, node) {
            OpenAPI_redundant_transmission_exp_req_free(node->data);
        }
        OpenAPI_list_free(event_subscription->red_trans_reqs);
        event_subscription->red_trans_reqs = NULL;
    }
    if (event_subscription->wlan_reqs) {
        OpenAPI_list_for_each(event_subscription->wlan_reqs, node) {
            OpenAPI_wlan_performance_req_free(node->data);
        }
        OpenAPI_list_free(event_subscription->wlan_reqs);
        event_subscription->wlan_reqs = NULL;
    }
    if (event_subscription->upf_info) {
        OpenAPI_upf_information_free(event_subscription->upf_info);
        event_subscription->upf_info = NULL;
    }
    if (event_subscription->app_server_addrs) {
        OpenAPI_list_for_each(event_subscription->app_server_addrs, node) {
            OpenAPI_addr_fqdn_free(node->data);
        }
        OpenAPI_list_free(event_subscription->app_server_addrs);
        event_subscription->app_server_addrs = NULL;
    }
    if (event_subscription->dn_perf_reqs) {
        OpenAPI_list_for_each(event_subscription->dn_perf_reqs, node) {
            OpenAPI_dn_performance_req_free(node->data);
        }
        OpenAPI_list_free(event_subscription->dn_perf_reqs);
        event_subscription->dn_perf_reqs = NULL;
    }
    ogs_free(event_subscription);
}

cJSON *OpenAPI_event_subscription_convertToJSON(OpenAPI_event_subscription_t *event_subscription)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (event_subscription == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [EventSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (event_subscription->is_any_slice) {
    if (cJSON_AddBoolToObject(item, "anySlice", event_subscription->any_slice) == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [any_slice]");
        goto end;
    }
    }

    if (event_subscription->app_ids) {
    cJSON *app_idsList = cJSON_AddArrayToObject(item, "appIds");
    if (app_idsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [app_ids]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->app_ids, node) {
        if (cJSON_AddStringToObject(app_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [app_ids]");
            goto end;
        }
    }
    }

    if (event_subscription->dnns) {
    cJSON *dnnsList = cJSON_AddArrayToObject(item, "dnns");
    if (dnnsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [dnns]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->dnns, node) {
        if (cJSON_AddStringToObject(dnnsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [dnns]");
            goto end;
        }
    }
    }

    if (event_subscription->dnais) {
    cJSON *dnaisList = cJSON_AddArrayToObject(item, "dnais");
    if (dnaisList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [dnais]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->dnais, node) {
        if (cJSON_AddStringToObject(dnaisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [dnais]");
            goto end;
        }
    }
    }

    if (!event_subscription->event) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [event]");
        return NULL;
    }
    cJSON *event_local_JSON = OpenAPI_nwdaf_event_convertToJSON(event_subscription->event);
    if (event_local_JSON == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [event]");
        goto end;
    }
    cJSON_AddItemToObject(item, "event", event_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [event]");
        goto end;
    }

    if (event_subscription->extra_report_req) {
    cJSON *extra_report_req_local_JSON = OpenAPI_event_reporting_requirement_convertToJSON(event_subscription->extra_report_req);
    if (extra_report_req_local_JSON == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [extra_report_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "extraReportReq", extra_report_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [extra_report_req]");
        goto end;
    }
    }

    if (event_subscription->ladn_dnns) {
    cJSON *ladn_dnnsList = cJSON_AddArrayToObject(item, "ladnDnns");
    if (ladn_dnnsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [ladn_dnns]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->ladn_dnns, node) {
        if (cJSON_AddStringToObject(ladn_dnnsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [ladn_dnns]");
            goto end;
        }
    }
    }

    if (event_subscription->is_load_level_threshold) {
    if (cJSON_AddNumberToObject(item, "loadLevelThreshold", event_subscription->load_level_threshold) == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [load_level_threshold]");
        goto end;
    }
    }

    if (event_subscription->notification_method) {
    cJSON *notification_method_local_JSON = OpenAPI_notification_method_convertToJSON(event_subscription->notification_method);
    if (notification_method_local_JSON == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [notification_method]");
        goto end;
    }
    cJSON_AddItemToObject(item, "notificationMethod", notification_method_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [notification_method]");
        goto end;
    }
    }

    if (event_subscription->matching_dir) {
    cJSON *matching_dir_local_JSON = OpenAPI_matching_direction_convertToJSON(event_subscription->matching_dir);
    if (matching_dir_local_JSON == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [matching_dir]");
        goto end;
    }
    cJSON_AddItemToObject(item, "matchingDir", matching_dir_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [matching_dir]");
        goto end;
    }
    }

    if (event_subscription->nf_load_lvl_thds) {
    cJSON *nf_load_lvl_thdsList = cJSON_AddArrayToObject(item, "nfLoadLvlThds");
    if (nf_load_lvl_thdsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nf_load_lvl_thds]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->nf_load_lvl_thds, node) {
        cJSON *itemLocal = OpenAPI_threshold_level_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nf_load_lvl_thds]");
            goto end;
        }
        cJSON_AddItemToArray(nf_load_lvl_thdsList, itemLocal);
    }
    }

    if (event_subscription->nf_instance_ids) {
    cJSON *nf_instance_idsList = cJSON_AddArrayToObject(item, "nfInstanceIds");
    if (nf_instance_idsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nf_instance_ids]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->nf_instance_ids, node) {
        if (cJSON_AddStringToObject(nf_instance_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nf_instance_ids]");
            goto end;
        }
    }
    }

    if (event_subscription->nf_set_ids) {
    cJSON *nf_set_idsList = cJSON_AddArrayToObject(item, "nfSetIds");
    if (nf_set_idsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nf_set_ids]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->nf_set_ids, node) {
        if (cJSON_AddStringToObject(nf_set_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nf_set_ids]");
            goto end;
        }
    }
    }

    if (event_subscription->nf_types != OpenAPI_nf_type_NULL) {
    cJSON *nf_typesList = cJSON_AddArrayToObject(item, "nfTypes");
    if (nf_typesList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nf_types]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->nf_types, node) {
        if (cJSON_AddStringToObject(nf_typesList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nf_types]");
            goto end;
        }
    }
    }

    if (event_subscription->network_area) {
    cJSON *network_area_local_JSON = OpenAPI_network_area_info_convertToJSON(event_subscription->network_area);
    if (network_area_local_JSON == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [network_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "networkArea", network_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [network_area]");
        goto end;
    }
    }

    if (event_subscription->visited_areas) {
    cJSON *visited_areasList = cJSON_AddArrayToObject(item, "visitedAreas");
    if (visited_areasList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [visited_areas]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->visited_areas, node) {
        cJSON *itemLocal = OpenAPI_network_area_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [visited_areas]");
            goto end;
        }
        cJSON_AddItemToArray(visited_areasList, itemLocal);
    }
    }

    if (event_subscription->is_max_top_app_ul_nbr) {
    if (cJSON_AddNumberToObject(item, "maxTopAppUlNbr", event_subscription->max_top_app_ul_nbr) == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [max_top_app_ul_nbr]");
        goto end;
    }
    }

    if (event_subscription->is_max_top_app_dl_nbr) {
    if (cJSON_AddNumberToObject(item, "maxTopAppDlNbr", event_subscription->max_top_app_dl_nbr) == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [max_top_app_dl_nbr]");
        goto end;
    }
    }

    if (event_subscription->nsi_id_infos) {
    cJSON *nsi_id_infosList = cJSON_AddArrayToObject(item, "nsiIdInfos");
    if (nsi_id_infosList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nsi_id_infos]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->nsi_id_infos, node) {
        cJSON *itemLocal = OpenAPI_nsi_id_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nsi_id_infos]");
            goto end;
        }
        cJSON_AddItemToArray(nsi_id_infosList, itemLocal);
    }
    }

    if (event_subscription->nsi_level_thrds) {
    cJSON *nsi_level_thrdsList = cJSON_AddArrayToObject(item, "nsiLevelThrds");
    if (nsi_level_thrdsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nsi_level_thrds]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->nsi_level_thrds, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nsi_level_thrds]");
            goto end;
        }
        if (cJSON_AddNumberToObject(nsi_level_thrdsList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nsi_level_thrds]");
            goto end;
        }
    }
    }

    if (event_subscription->qos_requ) {
    cJSON *qos_requ_local_JSON = OpenAPI_qos_requirement_convertToJSON(event_subscription->qos_requ);
    if (qos_requ_local_JSON == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [qos_requ]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qosRequ", qos_requ_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [qos_requ]");
        goto end;
    }
    }

    if (event_subscription->qos_flow_ret_thds) {
    cJSON *qos_flow_ret_thdsList = cJSON_AddArrayToObject(item, "qosFlowRetThds");
    if (qos_flow_ret_thdsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [qos_flow_ret_thds]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->qos_flow_ret_thds, node) {
        cJSON *itemLocal = OpenAPI_retainability_threshold_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [qos_flow_ret_thds]");
            goto end;
        }
        cJSON_AddItemToArray(qos_flow_ret_thdsList, itemLocal);
    }
    }

    if (event_subscription->ran_ue_throu_thds) {
    cJSON *ran_ue_throu_thdsList = cJSON_AddArrayToObject(item, "ranUeThrouThds");
    if (ran_ue_throu_thdsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [ran_ue_throu_thds]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->ran_ue_throu_thds, node) {
        if (cJSON_AddStringToObject(ran_ue_throu_thdsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [ran_ue_throu_thds]");
            goto end;
        }
    }
    }

    if (event_subscription->is_repetition_period) {
    if (cJSON_AddNumberToObject(item, "repetitionPeriod", event_subscription->repetition_period) == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [repetition_period]");
        goto end;
    }
    }

    if (event_subscription->snssaia) {
    cJSON *snssaiaList = cJSON_AddArrayToObject(item, "snssaia");
    if (snssaiaList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [snssaia]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->snssaia, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [snssaia]");
            goto end;
        }
        cJSON_AddItemToArray(snssaiaList, itemLocal);
    }
    }

    if (event_subscription->tgt_ue) {
    cJSON *tgt_ue_local_JSON = OpenAPI_target_ue_information_convertToJSON(event_subscription->tgt_ue);
    if (tgt_ue_local_JSON == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [tgt_ue]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tgtUe", tgt_ue_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [tgt_ue]");
        goto end;
    }
    }

    if (event_subscription->cong_thresholds) {
    cJSON *cong_thresholdsList = cJSON_AddArrayToObject(item, "congThresholds");
    if (cong_thresholdsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [cong_thresholds]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->cong_thresholds, node) {
        cJSON *itemLocal = OpenAPI_threshold_level_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [cong_thresholds]");
            goto end;
        }
        cJSON_AddItemToArray(cong_thresholdsList, itemLocal);
    }
    }

    if (event_subscription->nw_perf_requs) {
    cJSON *nw_perf_requsList = cJSON_AddArrayToObject(item, "nwPerfRequs");
    if (nw_perf_requsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nw_perf_requs]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->nw_perf_requs, node) {
        cJSON *itemLocal = OpenAPI_network_perf_requirement_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [nw_perf_requs]");
            goto end;
        }
        cJSON_AddItemToArray(nw_perf_requsList, itemLocal);
    }
    }

    if (event_subscription->bw_requs) {
    cJSON *bw_requsList = cJSON_AddArrayToObject(item, "bwRequs");
    if (bw_requsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [bw_requs]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->bw_requs, node) {
        cJSON *itemLocal = OpenAPI_bw_requirement_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [bw_requs]");
            goto end;
        }
        cJSON_AddItemToArray(bw_requsList, itemLocal);
    }
    }

    if (event_subscription->excep_requs) {
    cJSON *excep_requsList = cJSON_AddArrayToObject(item, "excepRequs");
    if (excep_requsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [excep_requs]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->excep_requs, node) {
        cJSON *itemLocal = OpenAPI_exception_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [excep_requs]");
            goto end;
        }
        cJSON_AddItemToArray(excep_requsList, itemLocal);
    }
    }

    if (event_subscription->expt_ana_type) {
    cJSON *expt_ana_type_local_JSON = OpenAPI_expected_analytics_type_convertToJSON(event_subscription->expt_ana_type);
    if (expt_ana_type_local_JSON == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [expt_ana_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "exptAnaType", expt_ana_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [expt_ana_type]");
        goto end;
    }
    }

    if (event_subscription->expt_ue_behav) {
    cJSON *expt_ue_behav_local_JSON = OpenAPI_expected_ue_behaviour_data_convertToJSON(event_subscription->expt_ue_behav);
    if (expt_ue_behav_local_JSON == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [expt_ue_behav]");
        goto end;
    }
    cJSON_AddItemToObject(item, "exptUeBehav", expt_ue_behav_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [expt_ue_behav]");
        goto end;
    }
    }

    if (event_subscription->rat_freqs) {
    cJSON *rat_freqsList = cJSON_AddArrayToObject(item, "ratFreqs");
    if (rat_freqsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [rat_freqs]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->rat_freqs, node) {
        cJSON *itemLocal = OpenAPI_rat_freq_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [rat_freqs]");
            goto end;
        }
        cJSON_AddItemToArray(rat_freqsList, itemLocal);
    }
    }

    if (event_subscription->list_of_ana_subsets) {
    cJSON *list_of_ana_subsetsList = cJSON_AddArrayToObject(item, "listOfAnaSubsets");
    if (list_of_ana_subsetsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [list_of_ana_subsets]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->list_of_ana_subsets, node) {
        cJSON *itemLocal = OpenAPI_analytics_subset_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [list_of_ana_subsets]");
            goto end;
        }
        cJSON_AddItemToArray(list_of_ana_subsetsList, itemLocal);
    }
    }

    if (event_subscription->disper_reqs) {
    cJSON *disper_reqsList = cJSON_AddArrayToObject(item, "disperReqs");
    if (disper_reqsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [disper_reqs]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->disper_reqs, node) {
        cJSON *itemLocal = OpenAPI_dispersion_requirement_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [disper_reqs]");
            goto end;
        }
        cJSON_AddItemToArray(disper_reqsList, itemLocal);
    }
    }

    if (event_subscription->red_trans_reqs) {
    cJSON *red_trans_reqsList = cJSON_AddArrayToObject(item, "redTransReqs");
    if (red_trans_reqsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [red_trans_reqs]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->red_trans_reqs, node) {
        cJSON *itemLocal = OpenAPI_redundant_transmission_exp_req_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [red_trans_reqs]");
            goto end;
        }
        cJSON_AddItemToArray(red_trans_reqsList, itemLocal);
    }
    }

    if (event_subscription->wlan_reqs) {
    cJSON *wlan_reqsList = cJSON_AddArrayToObject(item, "wlanReqs");
    if (wlan_reqsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [wlan_reqs]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->wlan_reqs, node) {
        cJSON *itemLocal = OpenAPI_wlan_performance_req_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [wlan_reqs]");
            goto end;
        }
        cJSON_AddItemToArray(wlan_reqsList, itemLocal);
    }
    }

    if (event_subscription->upf_info) {
    cJSON *upf_info_local_JSON = OpenAPI_upf_information_convertToJSON(event_subscription->upf_info);
    if (upf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [upf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upfInfo", upf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [upf_info]");
        goto end;
    }
    }

    if (event_subscription->app_server_addrs) {
    cJSON *app_server_addrsList = cJSON_AddArrayToObject(item, "appServerAddrs");
    if (app_server_addrsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [app_server_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->app_server_addrs, node) {
        cJSON *itemLocal = OpenAPI_addr_fqdn_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [app_server_addrs]");
            goto end;
        }
        cJSON_AddItemToArray(app_server_addrsList, itemLocal);
    }
    }

    if (event_subscription->dn_perf_reqs) {
    cJSON *dn_perf_reqsList = cJSON_AddArrayToObject(item, "dnPerfReqs");
    if (dn_perf_reqsList == NULL) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed [dn_perf_reqs]");
        goto end;
    }
    OpenAPI_list_for_each(event_subscription->dn_perf_reqs, node) {
        cJSON *itemLocal = OpenAPI_dn_performance_req_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_subscription_convertToJSON() failed [dn_perf_reqs]");
            goto end;
        }
        cJSON_AddItemToArray(dn_perf_reqsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_event_subscription_t *OpenAPI_event_subscription_parseFromJSON(cJSON *event_subscriptionJSON)
{
    OpenAPI_event_subscription_t *event_subscription_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *any_slice = NULL;
    cJSON *app_ids = NULL;
    OpenAPI_list_t *app_idsList = NULL;
    cJSON *dnns = NULL;
    OpenAPI_list_t *dnnsList = NULL;
    cJSON *dnais = NULL;
    OpenAPI_list_t *dnaisList = NULL;
    cJSON *event = NULL;
    OpenAPI_nwdaf_event_t *event_local_nonprim = NULL;
    cJSON *extra_report_req = NULL;
    OpenAPI_event_reporting_requirement_t *extra_report_req_local_nonprim = NULL;
    cJSON *ladn_dnns = NULL;
    OpenAPI_list_t *ladn_dnnsList = NULL;
    cJSON *load_level_threshold = NULL;
    cJSON *notification_method = NULL;
    OpenAPI_notification_method_t *notification_method_local_nonprim = NULL;
    cJSON *matching_dir = NULL;
    OpenAPI_matching_direction_t *matching_dir_local_nonprim = NULL;
    cJSON *nf_load_lvl_thds = NULL;
    OpenAPI_list_t *nf_load_lvl_thdsList = NULL;
    cJSON *nf_instance_ids = NULL;
    OpenAPI_list_t *nf_instance_idsList = NULL;
    cJSON *nf_set_ids = NULL;
    OpenAPI_list_t *nf_set_idsList = NULL;
    cJSON *nf_types = NULL;
    OpenAPI_list_t *nf_typesList = NULL;
    cJSON *network_area = NULL;
    OpenAPI_network_area_info_t *network_area_local_nonprim = NULL;
    cJSON *visited_areas = NULL;
    OpenAPI_list_t *visited_areasList = NULL;
    cJSON *max_top_app_ul_nbr = NULL;
    cJSON *max_top_app_dl_nbr = NULL;
    cJSON *nsi_id_infos = NULL;
    OpenAPI_list_t *nsi_id_infosList = NULL;
    cJSON *nsi_level_thrds = NULL;
    OpenAPI_list_t *nsi_level_thrdsList = NULL;
    cJSON *qos_requ = NULL;
    OpenAPI_qos_requirement_t *qos_requ_local_nonprim = NULL;
    cJSON *qos_flow_ret_thds = NULL;
    OpenAPI_list_t *qos_flow_ret_thdsList = NULL;
    cJSON *ran_ue_throu_thds = NULL;
    OpenAPI_list_t *ran_ue_throu_thdsList = NULL;
    cJSON *repetition_period = NULL;
    cJSON *snssaia = NULL;
    OpenAPI_list_t *snssaiaList = NULL;
    cJSON *tgt_ue = NULL;
    OpenAPI_target_ue_information_t *tgt_ue_local_nonprim = NULL;
    cJSON *cong_thresholds = NULL;
    OpenAPI_list_t *cong_thresholdsList = NULL;
    cJSON *nw_perf_requs = NULL;
    OpenAPI_list_t *nw_perf_requsList = NULL;
    cJSON *bw_requs = NULL;
    OpenAPI_list_t *bw_requsList = NULL;
    cJSON *excep_requs = NULL;
    OpenAPI_list_t *excep_requsList = NULL;
    cJSON *expt_ana_type = NULL;
    OpenAPI_expected_analytics_type_t *expt_ana_type_local_nonprim = NULL;
    cJSON *expt_ue_behav = NULL;
    OpenAPI_expected_ue_behaviour_data_t *expt_ue_behav_local_nonprim = NULL;
    cJSON *rat_freqs = NULL;
    OpenAPI_list_t *rat_freqsList = NULL;
    cJSON *list_of_ana_subsets = NULL;
    OpenAPI_list_t *list_of_ana_subsetsList = NULL;
    cJSON *disper_reqs = NULL;
    OpenAPI_list_t *disper_reqsList = NULL;
    cJSON *red_trans_reqs = NULL;
    OpenAPI_list_t *red_trans_reqsList = NULL;
    cJSON *wlan_reqs = NULL;
    OpenAPI_list_t *wlan_reqsList = NULL;
    cJSON *upf_info = NULL;
    OpenAPI_upf_information_t *upf_info_local_nonprim = NULL;
    cJSON *app_server_addrs = NULL;
    OpenAPI_list_t *app_server_addrsList = NULL;
    cJSON *dn_perf_reqs = NULL;
    OpenAPI_list_t *dn_perf_reqsList = NULL;
    any_slice = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "anySlice");
    if (any_slice) {
    if (!cJSON_IsBool(any_slice)) {
        ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [any_slice]");
        goto end;
    }
    }

    app_ids = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "appIds");
    if (app_ids) {
        cJSON *app_ids_local = NULL;
        if (!cJSON_IsArray(app_ids)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [app_ids]");
            goto end;
        }

        app_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_ids_local, app_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(app_ids_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [app_ids]");
                goto end;
            }
            OpenAPI_list_add(app_idsList, ogs_strdup(app_ids_local->valuestring));
        }
    }

    dnns = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "dnns");
    if (dnns) {
        cJSON *dnns_local = NULL;
        if (!cJSON_IsArray(dnns)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [dnns]");
            goto end;
        }

        dnnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnns_local, dnns) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnns_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [dnns]");
                goto end;
            }
            OpenAPI_list_add(dnnsList, ogs_strdup(dnns_local->valuestring));
        }
    }

    dnais = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "dnais");
    if (dnais) {
        cJSON *dnais_local = NULL;
        if (!cJSON_IsArray(dnais)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [dnais]");
            goto end;
        }

        dnaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnais_local, dnais) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnais_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [dnais]");
                goto end;
            }
            OpenAPI_list_add(dnaisList, ogs_strdup(dnais_local->valuestring));
        }
    }

    event = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "event");
    if (!event) {
        ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [event]");
        goto end;
    }
    event_local_nonprim = OpenAPI_nwdaf_event_parseFromJSON(event);
    if (!event_local_nonprim) {
        ogs_error("OpenAPI_nwdaf_event_parseFromJSON failed [event]");
        goto end;
    }

    extra_report_req = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "extraReportReq");
    if (extra_report_req) {
    extra_report_req_local_nonprim = OpenAPI_event_reporting_requirement_parseFromJSON(extra_report_req);
    if (!extra_report_req_local_nonprim) {
        ogs_error("OpenAPI_event_reporting_requirement_parseFromJSON failed [extra_report_req]");
        goto end;
    }
    }

    ladn_dnns = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "ladnDnns");
    if (ladn_dnns) {
        cJSON *ladn_dnns_local = NULL;
        if (!cJSON_IsArray(ladn_dnns)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [ladn_dnns]");
            goto end;
        }

        ladn_dnnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ladn_dnns_local, ladn_dnns) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ladn_dnns_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [ladn_dnns]");
                goto end;
            }
            OpenAPI_list_add(ladn_dnnsList, ogs_strdup(ladn_dnns_local->valuestring));
        }
    }

    load_level_threshold = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "loadLevelThreshold");
    if (load_level_threshold) {
    if (!cJSON_IsNumber(load_level_threshold)) {
        ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [load_level_threshold]");
        goto end;
    }
    }

    notification_method = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "notificationMethod");
    if (notification_method) {
    notification_method_local_nonprim = OpenAPI_notification_method_parseFromJSON(notification_method);
    if (!notification_method_local_nonprim) {
        ogs_error("OpenAPI_notification_method_parseFromJSON failed [notification_method]");
        goto end;
    }
    }

    matching_dir = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "matchingDir");
    if (matching_dir) {
    matching_dir_local_nonprim = OpenAPI_matching_direction_parseFromJSON(matching_dir);
    if (!matching_dir_local_nonprim) {
        ogs_error("OpenAPI_matching_direction_parseFromJSON failed [matching_dir]");
        goto end;
    }
    }

    nf_load_lvl_thds = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "nfLoadLvlThds");
    if (nf_load_lvl_thds) {
        cJSON *nf_load_lvl_thds_local = NULL;
        if (!cJSON_IsArray(nf_load_lvl_thds)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nf_load_lvl_thds]");
            goto end;
        }

        nf_load_lvl_thdsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_load_lvl_thds_local, nf_load_lvl_thds) {
            if (!cJSON_IsObject(nf_load_lvl_thds_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nf_load_lvl_thds]");
                goto end;
            }
            OpenAPI_threshold_level_t *nf_load_lvl_thdsItem = OpenAPI_threshold_level_parseFromJSON(nf_load_lvl_thds_local);
            if (!nf_load_lvl_thdsItem) {
                ogs_error("No nf_load_lvl_thdsItem");
                goto end;
            }
            OpenAPI_list_add(nf_load_lvl_thdsList, nf_load_lvl_thdsItem);
        }
    }

    nf_instance_ids = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "nfInstanceIds");
    if (nf_instance_ids) {
        cJSON *nf_instance_ids_local = NULL;
        if (!cJSON_IsArray(nf_instance_ids)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nf_instance_ids]");
            goto end;
        }

        nf_instance_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_instance_ids_local, nf_instance_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_instance_ids_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nf_instance_ids]");
                goto end;
            }
            OpenAPI_list_add(nf_instance_idsList, ogs_strdup(nf_instance_ids_local->valuestring));
        }
    }

    nf_set_ids = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "nfSetIds");
    if (nf_set_ids) {
        cJSON *nf_set_ids_local = NULL;
        if (!cJSON_IsArray(nf_set_ids)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nf_set_ids]");
            goto end;
        }

        nf_set_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_set_ids_local, nf_set_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_set_ids_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nf_set_ids]");
                goto end;
            }
            OpenAPI_list_add(nf_set_idsList, ogs_strdup(nf_set_ids_local->valuestring));
        }
    }

    nf_types = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "nfTypes");
    if (nf_types) {
        cJSON *nf_types_local = NULL;
        if (!cJSON_IsArray(nf_types)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nf_types]");
            goto end;
        }

        nf_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_types_local, nf_types) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(nf_types_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nf_types]");
                goto end;
            }
            localEnum = OpenAPI_nf_type_FromString(nf_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"nf_types\" is not supported. Ignoring it ...",
                         nf_types_local->valuestring);
            } else {
                OpenAPI_list_add(nf_typesList, (void *)localEnum);
            }
        }
        if (nf_typesList->count == 0) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed: Expected nf_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    network_area = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "networkArea");
    if (network_area) {
    network_area_local_nonprim = OpenAPI_network_area_info_parseFromJSON(network_area);
    if (!network_area_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_parseFromJSON failed [network_area]");
        goto end;
    }
    }

    visited_areas = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "visitedAreas");
    if (visited_areas) {
        cJSON *visited_areas_local = NULL;
        if (!cJSON_IsArray(visited_areas)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [visited_areas]");
            goto end;
        }

        visited_areasList = OpenAPI_list_create();

        cJSON_ArrayForEach(visited_areas_local, visited_areas) {
            if (!cJSON_IsObject(visited_areas_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [visited_areas]");
                goto end;
            }
            OpenAPI_network_area_info_t *visited_areasItem = OpenAPI_network_area_info_parseFromJSON(visited_areas_local);
            if (!visited_areasItem) {
                ogs_error("No visited_areasItem");
                goto end;
            }
            OpenAPI_list_add(visited_areasList, visited_areasItem);
        }
    }

    max_top_app_ul_nbr = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "maxTopAppUlNbr");
    if (max_top_app_ul_nbr) {
    if (!cJSON_IsNumber(max_top_app_ul_nbr)) {
        ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [max_top_app_ul_nbr]");
        goto end;
    }
    }

    max_top_app_dl_nbr = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "maxTopAppDlNbr");
    if (max_top_app_dl_nbr) {
    if (!cJSON_IsNumber(max_top_app_dl_nbr)) {
        ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [max_top_app_dl_nbr]");
        goto end;
    }
    }

    nsi_id_infos = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "nsiIdInfos");
    if (nsi_id_infos) {
        cJSON *nsi_id_infos_local = NULL;
        if (!cJSON_IsArray(nsi_id_infos)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nsi_id_infos]");
            goto end;
        }

        nsi_id_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(nsi_id_infos_local, nsi_id_infos) {
            if (!cJSON_IsObject(nsi_id_infos_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nsi_id_infos]");
                goto end;
            }
            OpenAPI_nsi_id_info_t *nsi_id_infosItem = OpenAPI_nsi_id_info_parseFromJSON(nsi_id_infos_local);
            if (!nsi_id_infosItem) {
                ogs_error("No nsi_id_infosItem");
                goto end;
            }
            OpenAPI_list_add(nsi_id_infosList, nsi_id_infosItem);
        }
    }

    nsi_level_thrds = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "nsiLevelThrds");
    if (nsi_level_thrds) {
        cJSON *nsi_level_thrds_local = NULL;
        if (!cJSON_IsArray(nsi_level_thrds)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nsi_level_thrds]");
            goto end;
        }

        nsi_level_thrdsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nsi_level_thrds_local, nsi_level_thrds) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(nsi_level_thrds_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nsi_level_thrds]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nsi_level_thrds]");
                goto end;
            }
            *localDouble = nsi_level_thrds_local->valuedouble;
            OpenAPI_list_add(nsi_level_thrdsList, localDouble);
        }
    }

    qos_requ = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "qosRequ");
    if (qos_requ) {
    qos_requ_local_nonprim = OpenAPI_qos_requirement_parseFromJSON(qos_requ);
    if (!qos_requ_local_nonprim) {
        ogs_error("OpenAPI_qos_requirement_parseFromJSON failed [qos_requ]");
        goto end;
    }
    }

    qos_flow_ret_thds = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "qosFlowRetThds");
    if (qos_flow_ret_thds) {
        cJSON *qos_flow_ret_thds_local = NULL;
        if (!cJSON_IsArray(qos_flow_ret_thds)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [qos_flow_ret_thds]");
            goto end;
        }

        qos_flow_ret_thdsList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_flow_ret_thds_local, qos_flow_ret_thds) {
            if (!cJSON_IsObject(qos_flow_ret_thds_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [qos_flow_ret_thds]");
                goto end;
            }
            OpenAPI_retainability_threshold_t *qos_flow_ret_thdsItem = OpenAPI_retainability_threshold_parseFromJSON(qos_flow_ret_thds_local);
            if (!qos_flow_ret_thdsItem) {
                ogs_error("No qos_flow_ret_thdsItem");
                goto end;
            }
            OpenAPI_list_add(qos_flow_ret_thdsList, qos_flow_ret_thdsItem);
        }
    }

    ran_ue_throu_thds = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "ranUeThrouThds");
    if (ran_ue_throu_thds) {
        cJSON *ran_ue_throu_thds_local = NULL;
        if (!cJSON_IsArray(ran_ue_throu_thds)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [ran_ue_throu_thds]");
            goto end;
        }

        ran_ue_throu_thdsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ran_ue_throu_thds_local, ran_ue_throu_thds) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ran_ue_throu_thds_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [ran_ue_throu_thds]");
                goto end;
            }
            OpenAPI_list_add(ran_ue_throu_thdsList, ogs_strdup(ran_ue_throu_thds_local->valuestring));
        }
    }

    repetition_period = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "repetitionPeriod");
    if (repetition_period) {
    if (!cJSON_IsNumber(repetition_period)) {
        ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [repetition_period]");
        goto end;
    }
    }

    snssaia = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "snssaia");
    if (snssaia) {
        cJSON *snssaia_local = NULL;
        if (!cJSON_IsArray(snssaia)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [snssaia]");
            goto end;
        }

        snssaiaList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssaia_local, snssaia) {
            if (!cJSON_IsObject(snssaia_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [snssaia]");
                goto end;
            }
            OpenAPI_snssai_t *snssaiaItem = OpenAPI_snssai_parseFromJSON(snssaia_local);
            if (!snssaiaItem) {
                ogs_error("No snssaiaItem");
                goto end;
            }
            OpenAPI_list_add(snssaiaList, snssaiaItem);
        }
    }

    tgt_ue = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "tgtUe");
    if (tgt_ue) {
    tgt_ue_local_nonprim = OpenAPI_target_ue_information_parseFromJSON(tgt_ue);
    if (!tgt_ue_local_nonprim) {
        ogs_error("OpenAPI_target_ue_information_parseFromJSON failed [tgt_ue]");
        goto end;
    }
    }

    cong_thresholds = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "congThresholds");
    if (cong_thresholds) {
        cJSON *cong_thresholds_local = NULL;
        if (!cJSON_IsArray(cong_thresholds)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [cong_thresholds]");
            goto end;
        }

        cong_thresholdsList = OpenAPI_list_create();

        cJSON_ArrayForEach(cong_thresholds_local, cong_thresholds) {
            if (!cJSON_IsObject(cong_thresholds_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [cong_thresholds]");
                goto end;
            }
            OpenAPI_threshold_level_t *cong_thresholdsItem = OpenAPI_threshold_level_parseFromJSON(cong_thresholds_local);
            if (!cong_thresholdsItem) {
                ogs_error("No cong_thresholdsItem");
                goto end;
            }
            OpenAPI_list_add(cong_thresholdsList, cong_thresholdsItem);
        }
    }

    nw_perf_requs = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "nwPerfRequs");
    if (nw_perf_requs) {
        cJSON *nw_perf_requs_local = NULL;
        if (!cJSON_IsArray(nw_perf_requs)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nw_perf_requs]");
            goto end;
        }

        nw_perf_requsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nw_perf_requs_local, nw_perf_requs) {
            if (!cJSON_IsObject(nw_perf_requs_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [nw_perf_requs]");
                goto end;
            }
            OpenAPI_network_perf_requirement_t *nw_perf_requsItem = OpenAPI_network_perf_requirement_parseFromJSON(nw_perf_requs_local);
            if (!nw_perf_requsItem) {
                ogs_error("No nw_perf_requsItem");
                goto end;
            }
            OpenAPI_list_add(nw_perf_requsList, nw_perf_requsItem);
        }
    }

    bw_requs = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "bwRequs");
    if (bw_requs) {
        cJSON *bw_requs_local = NULL;
        if (!cJSON_IsArray(bw_requs)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [bw_requs]");
            goto end;
        }

        bw_requsList = OpenAPI_list_create();

        cJSON_ArrayForEach(bw_requs_local, bw_requs) {
            if (!cJSON_IsObject(bw_requs_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [bw_requs]");
                goto end;
            }
            OpenAPI_bw_requirement_t *bw_requsItem = OpenAPI_bw_requirement_parseFromJSON(bw_requs_local);
            if (!bw_requsItem) {
                ogs_error("No bw_requsItem");
                goto end;
            }
            OpenAPI_list_add(bw_requsList, bw_requsItem);
        }
    }

    excep_requs = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "excepRequs");
    if (excep_requs) {
        cJSON *excep_requs_local = NULL;
        if (!cJSON_IsArray(excep_requs)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [excep_requs]");
            goto end;
        }

        excep_requsList = OpenAPI_list_create();

        cJSON_ArrayForEach(excep_requs_local, excep_requs) {
            if (!cJSON_IsObject(excep_requs_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [excep_requs]");
                goto end;
            }
            OpenAPI_exception_t *excep_requsItem = OpenAPI_exception_parseFromJSON(excep_requs_local);
            if (!excep_requsItem) {
                ogs_error("No excep_requsItem");
                goto end;
            }
            OpenAPI_list_add(excep_requsList, excep_requsItem);
        }
    }

    expt_ana_type = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "exptAnaType");
    if (expt_ana_type) {
    expt_ana_type_local_nonprim = OpenAPI_expected_analytics_type_parseFromJSON(expt_ana_type);
    if (!expt_ana_type_local_nonprim) {
        ogs_error("OpenAPI_expected_analytics_type_parseFromJSON failed [expt_ana_type]");
        goto end;
    }
    }

    expt_ue_behav = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "exptUeBehav");
    if (expt_ue_behav) {
    expt_ue_behav_local_nonprim = OpenAPI_expected_ue_behaviour_data_parseFromJSON(expt_ue_behav);
    if (!expt_ue_behav_local_nonprim) {
        ogs_error("OpenAPI_expected_ue_behaviour_data_parseFromJSON failed [expt_ue_behav]");
        goto end;
    }
    }

    rat_freqs = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "ratFreqs");
    if (rat_freqs) {
        cJSON *rat_freqs_local = NULL;
        if (!cJSON_IsArray(rat_freqs)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [rat_freqs]");
            goto end;
        }

        rat_freqsList = OpenAPI_list_create();

        cJSON_ArrayForEach(rat_freqs_local, rat_freqs) {
            if (!cJSON_IsObject(rat_freqs_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [rat_freqs]");
                goto end;
            }
            OpenAPI_rat_freq_information_t *rat_freqsItem = OpenAPI_rat_freq_information_parseFromJSON(rat_freqs_local);
            if (!rat_freqsItem) {
                ogs_error("No rat_freqsItem");
                goto end;
            }
            OpenAPI_list_add(rat_freqsList, rat_freqsItem);
        }
    }

    list_of_ana_subsets = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "listOfAnaSubsets");
    if (list_of_ana_subsets) {
        cJSON *list_of_ana_subsets_local = NULL;
        if (!cJSON_IsArray(list_of_ana_subsets)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [list_of_ana_subsets]");
            goto end;
        }

        list_of_ana_subsetsList = OpenAPI_list_create();

        cJSON_ArrayForEach(list_of_ana_subsets_local, list_of_ana_subsets) {
            if (!cJSON_IsObject(list_of_ana_subsets_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [list_of_ana_subsets]");
                goto end;
            }
            OpenAPI_analytics_subset_t *list_of_ana_subsetsItem = OpenAPI_analytics_subset_parseFromJSON(list_of_ana_subsets_local);
            if (!list_of_ana_subsetsItem) {
                ogs_error("No list_of_ana_subsetsItem");
                goto end;
            }
            OpenAPI_list_add(list_of_ana_subsetsList, list_of_ana_subsetsItem);
        }
    }

    disper_reqs = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "disperReqs");
    if (disper_reqs) {
        cJSON *disper_reqs_local = NULL;
        if (!cJSON_IsArray(disper_reqs)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [disper_reqs]");
            goto end;
        }

        disper_reqsList = OpenAPI_list_create();

        cJSON_ArrayForEach(disper_reqs_local, disper_reqs) {
            if (!cJSON_IsObject(disper_reqs_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [disper_reqs]");
                goto end;
            }
            OpenAPI_dispersion_requirement_t *disper_reqsItem = OpenAPI_dispersion_requirement_parseFromJSON(disper_reqs_local);
            if (!disper_reqsItem) {
                ogs_error("No disper_reqsItem");
                goto end;
            }
            OpenAPI_list_add(disper_reqsList, disper_reqsItem);
        }
    }

    red_trans_reqs = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "redTransReqs");
    if (red_trans_reqs) {
        cJSON *red_trans_reqs_local = NULL;
        if (!cJSON_IsArray(red_trans_reqs)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [red_trans_reqs]");
            goto end;
        }

        red_trans_reqsList = OpenAPI_list_create();

        cJSON_ArrayForEach(red_trans_reqs_local, red_trans_reqs) {
            if (!cJSON_IsObject(red_trans_reqs_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [red_trans_reqs]");
                goto end;
            }
            OpenAPI_redundant_transmission_exp_req_t *red_trans_reqsItem = OpenAPI_redundant_transmission_exp_req_parseFromJSON(red_trans_reqs_local);
            if (!red_trans_reqsItem) {
                ogs_error("No red_trans_reqsItem");
                goto end;
            }
            OpenAPI_list_add(red_trans_reqsList, red_trans_reqsItem);
        }
    }

    wlan_reqs = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "wlanReqs");
    if (wlan_reqs) {
        cJSON *wlan_reqs_local = NULL;
        if (!cJSON_IsArray(wlan_reqs)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [wlan_reqs]");
            goto end;
        }

        wlan_reqsList = OpenAPI_list_create();

        cJSON_ArrayForEach(wlan_reqs_local, wlan_reqs) {
            if (!cJSON_IsObject(wlan_reqs_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [wlan_reqs]");
                goto end;
            }
            OpenAPI_wlan_performance_req_t *wlan_reqsItem = OpenAPI_wlan_performance_req_parseFromJSON(wlan_reqs_local);
            if (!wlan_reqsItem) {
                ogs_error("No wlan_reqsItem");
                goto end;
            }
            OpenAPI_list_add(wlan_reqsList, wlan_reqsItem);
        }
    }

    upf_info = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "upfInfo");
    if (upf_info) {
    upf_info_local_nonprim = OpenAPI_upf_information_parseFromJSON(upf_info);
    if (!upf_info_local_nonprim) {
        ogs_error("OpenAPI_upf_information_parseFromJSON failed [upf_info]");
        goto end;
    }
    }

    app_server_addrs = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "appServerAddrs");
    if (app_server_addrs) {
        cJSON *app_server_addrs_local = NULL;
        if (!cJSON_IsArray(app_server_addrs)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [app_server_addrs]");
            goto end;
        }

        app_server_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_server_addrs_local, app_server_addrs) {
            if (!cJSON_IsObject(app_server_addrs_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [app_server_addrs]");
                goto end;
            }
            OpenAPI_addr_fqdn_t *app_server_addrsItem = OpenAPI_addr_fqdn_parseFromJSON(app_server_addrs_local);
            if (!app_server_addrsItem) {
                ogs_error("No app_server_addrsItem");
                goto end;
            }
            OpenAPI_list_add(app_server_addrsList, app_server_addrsItem);
        }
    }

    dn_perf_reqs = cJSON_GetObjectItemCaseSensitive(event_subscriptionJSON, "dnPerfReqs");
    if (dn_perf_reqs) {
        cJSON *dn_perf_reqs_local = NULL;
        if (!cJSON_IsArray(dn_perf_reqs)) {
            ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [dn_perf_reqs]");
            goto end;
        }

        dn_perf_reqsList = OpenAPI_list_create();

        cJSON_ArrayForEach(dn_perf_reqs_local, dn_perf_reqs) {
            if (!cJSON_IsObject(dn_perf_reqs_local)) {
                ogs_error("OpenAPI_event_subscription_parseFromJSON() failed [dn_perf_reqs]");
                goto end;
            }
            OpenAPI_dn_performance_req_t *dn_perf_reqsItem = OpenAPI_dn_performance_req_parseFromJSON(dn_perf_reqs_local);
            if (!dn_perf_reqsItem) {
                ogs_error("No dn_perf_reqsItem");
                goto end;
            }
            OpenAPI_list_add(dn_perf_reqsList, dn_perf_reqsItem);
        }
    }

    event_subscription_local_var = OpenAPI_event_subscription_create (
        any_slice ? true : false,
        any_slice ? any_slice->valueint : 0,
        app_ids ? app_idsList : NULL,
        dnns ? dnnsList : NULL,
        dnais ? dnaisList : NULL,
        event_local_nonprim,
        extra_report_req ? extra_report_req_local_nonprim : NULL,
        ladn_dnns ? ladn_dnnsList : NULL,
        load_level_threshold ? true : false,
        load_level_threshold ? load_level_threshold->valuedouble : 0,
        notification_method ? notification_method_local_nonprim : NULL,
        matching_dir ? matching_dir_local_nonprim : NULL,
        nf_load_lvl_thds ? nf_load_lvl_thdsList : NULL,
        nf_instance_ids ? nf_instance_idsList : NULL,
        nf_set_ids ? nf_set_idsList : NULL,
        nf_types ? nf_typesList : NULL,
        network_area ? network_area_local_nonprim : NULL,
        visited_areas ? visited_areasList : NULL,
        max_top_app_ul_nbr ? true : false,
        max_top_app_ul_nbr ? max_top_app_ul_nbr->valuedouble : 0,
        max_top_app_dl_nbr ? true : false,
        max_top_app_dl_nbr ? max_top_app_dl_nbr->valuedouble : 0,
        nsi_id_infos ? nsi_id_infosList : NULL,
        nsi_level_thrds ? nsi_level_thrdsList : NULL,
        qos_requ ? qos_requ_local_nonprim : NULL,
        qos_flow_ret_thds ? qos_flow_ret_thdsList : NULL,
        ran_ue_throu_thds ? ran_ue_throu_thdsList : NULL,
        repetition_period ? true : false,
        repetition_period ? repetition_period->valuedouble : 0,
        snssaia ? snssaiaList : NULL,
        tgt_ue ? tgt_ue_local_nonprim : NULL,
        cong_thresholds ? cong_thresholdsList : NULL,
        nw_perf_requs ? nw_perf_requsList : NULL,
        bw_requs ? bw_requsList : NULL,
        excep_requs ? excep_requsList : NULL,
        expt_ana_type ? expt_ana_type_local_nonprim : NULL,
        expt_ue_behav ? expt_ue_behav_local_nonprim : NULL,
        rat_freqs ? rat_freqsList : NULL,
        list_of_ana_subsets ? list_of_ana_subsetsList : NULL,
        disper_reqs ? disper_reqsList : NULL,
        red_trans_reqs ? red_trans_reqsList : NULL,
        wlan_reqs ? wlan_reqsList : NULL,
        upf_info ? upf_info_local_nonprim : NULL,
        app_server_addrs ? app_server_addrsList : NULL,
        dn_perf_reqs ? dn_perf_reqsList : NULL
    );

    return event_subscription_local_var;
end:
    if (app_idsList) {
        OpenAPI_list_for_each(app_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(app_idsList);
        app_idsList = NULL;
    }
    if (dnnsList) {
        OpenAPI_list_for_each(dnnsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnnsList);
        dnnsList = NULL;
    }
    if (dnaisList) {
        OpenAPI_list_for_each(dnaisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnaisList);
        dnaisList = NULL;
    }
    if (event_local_nonprim) {
        OpenAPI_nwdaf_event_free(event_local_nonprim);
        event_local_nonprim = NULL;
    }
    if (extra_report_req_local_nonprim) {
        OpenAPI_event_reporting_requirement_free(extra_report_req_local_nonprim);
        extra_report_req_local_nonprim = NULL;
    }
    if (ladn_dnnsList) {
        OpenAPI_list_for_each(ladn_dnnsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ladn_dnnsList);
        ladn_dnnsList = NULL;
    }
    if (notification_method_local_nonprim) {
        OpenAPI_notification_method_free(notification_method_local_nonprim);
        notification_method_local_nonprim = NULL;
    }
    if (matching_dir_local_nonprim) {
        OpenAPI_matching_direction_free(matching_dir_local_nonprim);
        matching_dir_local_nonprim = NULL;
    }
    if (nf_load_lvl_thdsList) {
        OpenAPI_list_for_each(nf_load_lvl_thdsList, node) {
            OpenAPI_threshold_level_free(node->data);
        }
        OpenAPI_list_free(nf_load_lvl_thdsList);
        nf_load_lvl_thdsList = NULL;
    }
    if (nf_instance_idsList) {
        OpenAPI_list_for_each(nf_instance_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_instance_idsList);
        nf_instance_idsList = NULL;
    }
    if (nf_set_idsList) {
        OpenAPI_list_for_each(nf_set_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_set_idsList);
        nf_set_idsList = NULL;
    }
    if (nf_typesList) {
        OpenAPI_list_free(nf_typesList);
        nf_typesList = NULL;
    }
    if (network_area_local_nonprim) {
        OpenAPI_network_area_info_free(network_area_local_nonprim);
        network_area_local_nonprim = NULL;
    }
    if (visited_areasList) {
        OpenAPI_list_for_each(visited_areasList, node) {
            OpenAPI_network_area_info_free(node->data);
        }
        OpenAPI_list_free(visited_areasList);
        visited_areasList = NULL;
    }
    if (nsi_id_infosList) {
        OpenAPI_list_for_each(nsi_id_infosList, node) {
            OpenAPI_nsi_id_info_free(node->data);
        }
        OpenAPI_list_free(nsi_id_infosList);
        nsi_id_infosList = NULL;
    }
    if (nsi_level_thrdsList) {
        OpenAPI_list_for_each(nsi_level_thrdsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nsi_level_thrdsList);
        nsi_level_thrdsList = NULL;
    }
    if (qos_requ_local_nonprim) {
        OpenAPI_qos_requirement_free(qos_requ_local_nonprim);
        qos_requ_local_nonprim = NULL;
    }
    if (qos_flow_ret_thdsList) {
        OpenAPI_list_for_each(qos_flow_ret_thdsList, node) {
            OpenAPI_retainability_threshold_free(node->data);
        }
        OpenAPI_list_free(qos_flow_ret_thdsList);
        qos_flow_ret_thdsList = NULL;
    }
    if (ran_ue_throu_thdsList) {
        OpenAPI_list_for_each(ran_ue_throu_thdsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ran_ue_throu_thdsList);
        ran_ue_throu_thdsList = NULL;
    }
    if (snssaiaList) {
        OpenAPI_list_for_each(snssaiaList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(snssaiaList);
        snssaiaList = NULL;
    }
    if (tgt_ue_local_nonprim) {
        OpenAPI_target_ue_information_free(tgt_ue_local_nonprim);
        tgt_ue_local_nonprim = NULL;
    }
    if (cong_thresholdsList) {
        OpenAPI_list_for_each(cong_thresholdsList, node) {
            OpenAPI_threshold_level_free(node->data);
        }
        OpenAPI_list_free(cong_thresholdsList);
        cong_thresholdsList = NULL;
    }
    if (nw_perf_requsList) {
        OpenAPI_list_for_each(nw_perf_requsList, node) {
            OpenAPI_network_perf_requirement_free(node->data);
        }
        OpenAPI_list_free(nw_perf_requsList);
        nw_perf_requsList = NULL;
    }
    if (bw_requsList) {
        OpenAPI_list_for_each(bw_requsList, node) {
            OpenAPI_bw_requirement_free(node->data);
        }
        OpenAPI_list_free(bw_requsList);
        bw_requsList = NULL;
    }
    if (excep_requsList) {
        OpenAPI_list_for_each(excep_requsList, node) {
            OpenAPI_exception_free(node->data);
        }
        OpenAPI_list_free(excep_requsList);
        excep_requsList = NULL;
    }
    if (expt_ana_type_local_nonprim) {
        OpenAPI_expected_analytics_type_free(expt_ana_type_local_nonprim);
        expt_ana_type_local_nonprim = NULL;
    }
    if (expt_ue_behav_local_nonprim) {
        OpenAPI_expected_ue_behaviour_data_free(expt_ue_behav_local_nonprim);
        expt_ue_behav_local_nonprim = NULL;
    }
    if (rat_freqsList) {
        OpenAPI_list_for_each(rat_freqsList, node) {
            OpenAPI_rat_freq_information_free(node->data);
        }
        OpenAPI_list_free(rat_freqsList);
        rat_freqsList = NULL;
    }
    if (list_of_ana_subsetsList) {
        OpenAPI_list_for_each(list_of_ana_subsetsList, node) {
            OpenAPI_analytics_subset_free(node->data);
        }
        OpenAPI_list_free(list_of_ana_subsetsList);
        list_of_ana_subsetsList = NULL;
    }
    if (disper_reqsList) {
        OpenAPI_list_for_each(disper_reqsList, node) {
            OpenAPI_dispersion_requirement_free(node->data);
        }
        OpenAPI_list_free(disper_reqsList);
        disper_reqsList = NULL;
    }
    if (red_trans_reqsList) {
        OpenAPI_list_for_each(red_trans_reqsList, node) {
            OpenAPI_redundant_transmission_exp_req_free(node->data);
        }
        OpenAPI_list_free(red_trans_reqsList);
        red_trans_reqsList = NULL;
    }
    if (wlan_reqsList) {
        OpenAPI_list_for_each(wlan_reqsList, node) {
            OpenAPI_wlan_performance_req_free(node->data);
        }
        OpenAPI_list_free(wlan_reqsList);
        wlan_reqsList = NULL;
    }
    if (upf_info_local_nonprim) {
        OpenAPI_upf_information_free(upf_info_local_nonprim);
        upf_info_local_nonprim = NULL;
    }
    if (app_server_addrsList) {
        OpenAPI_list_for_each(app_server_addrsList, node) {
            OpenAPI_addr_fqdn_free(node->data);
        }
        OpenAPI_list_free(app_server_addrsList);
        app_server_addrsList = NULL;
    }
    if (dn_perf_reqsList) {
        OpenAPI_list_for_each(dn_perf_reqsList, node) {
            OpenAPI_dn_performance_req_free(node->data);
        }
        OpenAPI_list_free(dn_perf_reqsList);
        dn_perf_reqsList = NULL;
    }
    return NULL;
}

OpenAPI_event_subscription_t *OpenAPI_event_subscription_copy(OpenAPI_event_subscription_t *dst, OpenAPI_event_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_event_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_event_subscription_convertToJSON() failed");
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

    OpenAPI_event_subscription_free(dst);
    dst = OpenAPI_event_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

