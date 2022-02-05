
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "events_subsc_put_data.h"

OpenAPI_events_subsc_put_data_t *OpenAPI_events_subsc_put_data_create(
    OpenAPI_list_t *events,
    char *notif_uri,
    OpenAPI_list_t *req_qos_mon_params,
    OpenAPI_qos_monitoring_information_t *qos_mon,
    OpenAPI_list_t *req_anis,
    OpenAPI_usage_threshold_t *usg_thres,
    char *notif_corre_id,
    OpenAPI_access_type_e access_type,
    OpenAPI_additional_access_info_t *add_access_info,
    OpenAPI_additional_access_info_t *rel_access_info,
    OpenAPI_acc_net_charging_address_t *an_charg_addr,
    OpenAPI_list_t *an_charg_ids,
    OpenAPI_an_gw_address_t *an_gw_addr,
    char *ev_subs_uri,
    OpenAPI_list_t *ev_notifs,
    OpenAPI_list_t *failed_resourc_alloc_reports,
    OpenAPI_list_t *succ_resourc_alloc_reports,
    OpenAPI_net_loc_access_support_e no_net_loc_supp,
    OpenAPI_list_t *out_of_cred_reports,
    OpenAPI_plmn_id_nid_t *plmn_id,
    OpenAPI_list_t *qnc_reports,
    OpenAPI_list_t *qos_mon_reports,
    OpenAPI_list_t *ran_nas_rel_causes,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_user_location_t *ue_loc,
    char *ue_time_zone,
    OpenAPI_accumulated_usage_t *usg_rep,
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts
)
{
    OpenAPI_events_subsc_put_data_t *events_subsc_put_data_local_var = ogs_malloc(sizeof(OpenAPI_events_subsc_put_data_t));
    ogs_assert(events_subsc_put_data_local_var);

    events_subsc_put_data_local_var->events = events;
    events_subsc_put_data_local_var->notif_uri = notif_uri;
    events_subsc_put_data_local_var->req_qos_mon_params = req_qos_mon_params;
    events_subsc_put_data_local_var->qos_mon = qos_mon;
    events_subsc_put_data_local_var->req_anis = req_anis;
    events_subsc_put_data_local_var->usg_thres = usg_thres;
    events_subsc_put_data_local_var->notif_corre_id = notif_corre_id;
    events_subsc_put_data_local_var->access_type = access_type;
    events_subsc_put_data_local_var->add_access_info = add_access_info;
    events_subsc_put_data_local_var->rel_access_info = rel_access_info;
    events_subsc_put_data_local_var->an_charg_addr = an_charg_addr;
    events_subsc_put_data_local_var->an_charg_ids = an_charg_ids;
    events_subsc_put_data_local_var->an_gw_addr = an_gw_addr;
    events_subsc_put_data_local_var->ev_subs_uri = ev_subs_uri;
    events_subsc_put_data_local_var->ev_notifs = ev_notifs;
    events_subsc_put_data_local_var->failed_resourc_alloc_reports = failed_resourc_alloc_reports;
    events_subsc_put_data_local_var->succ_resourc_alloc_reports = succ_resourc_alloc_reports;
    events_subsc_put_data_local_var->no_net_loc_supp = no_net_loc_supp;
    events_subsc_put_data_local_var->out_of_cred_reports = out_of_cred_reports;
    events_subsc_put_data_local_var->plmn_id = plmn_id;
    events_subsc_put_data_local_var->qnc_reports = qnc_reports;
    events_subsc_put_data_local_var->qos_mon_reports = qos_mon_reports;
    events_subsc_put_data_local_var->ran_nas_rel_causes = ran_nas_rel_causes;
    events_subsc_put_data_local_var->rat_type = rat_type;
    events_subsc_put_data_local_var->ue_loc = ue_loc;
    events_subsc_put_data_local_var->ue_time_zone = ue_time_zone;
    events_subsc_put_data_local_var->usg_rep = usg_rep;
    events_subsc_put_data_local_var->tsn_bridge_man_cont = tsn_bridge_man_cont;
    events_subsc_put_data_local_var->tsn_port_man_cont_dstt = tsn_port_man_cont_dstt;
    events_subsc_put_data_local_var->tsn_port_man_cont_nwtts = tsn_port_man_cont_nwtts;

    return events_subsc_put_data_local_var;
}

void OpenAPI_events_subsc_put_data_free(OpenAPI_events_subsc_put_data_t *events_subsc_put_data)
{
    if (NULL == events_subsc_put_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(events_subsc_put_data->events, node) {
        OpenAPI_af_event_subscription_free(node->data);
    }
    OpenAPI_list_free(events_subsc_put_data->events);
    ogs_free(events_subsc_put_data->notif_uri);
    OpenAPI_list_free(events_subsc_put_data->req_qos_mon_params);
    OpenAPI_qos_monitoring_information_free(events_subsc_put_data->qos_mon);
    OpenAPI_list_free(events_subsc_put_data->req_anis);
    OpenAPI_usage_threshold_free(events_subsc_put_data->usg_thres);
    ogs_free(events_subsc_put_data->notif_corre_id);
    OpenAPI_additional_access_info_free(events_subsc_put_data->add_access_info);
    OpenAPI_additional_access_info_free(events_subsc_put_data->rel_access_info);
    OpenAPI_acc_net_charging_address_free(events_subsc_put_data->an_charg_addr);
    OpenAPI_list_for_each(events_subsc_put_data->an_charg_ids, node) {
        OpenAPI_access_net_charging_identifier_free(node->data);
    }
    OpenAPI_list_free(events_subsc_put_data->an_charg_ids);
    OpenAPI_an_gw_address_free(events_subsc_put_data->an_gw_addr);
    ogs_free(events_subsc_put_data->ev_subs_uri);
    OpenAPI_list_for_each(events_subsc_put_data->ev_notifs, node) {
        OpenAPI_af_event_notification_free(node->data);
    }
    OpenAPI_list_free(events_subsc_put_data->ev_notifs);
    OpenAPI_list_for_each(events_subsc_put_data->failed_resourc_alloc_reports, node) {
        OpenAPI_resources_allocation_info_free(node->data);
    }
    OpenAPI_list_free(events_subsc_put_data->failed_resourc_alloc_reports);
    OpenAPI_list_for_each(events_subsc_put_data->succ_resourc_alloc_reports, node) {
        OpenAPI_resources_allocation_info_free(node->data);
    }
    OpenAPI_list_free(events_subsc_put_data->succ_resourc_alloc_reports);
    OpenAPI_list_for_each(events_subsc_put_data->out_of_cred_reports, node) {
        OpenAPI_out_of_credit_information_free(node->data);
    }
    OpenAPI_list_free(events_subsc_put_data->out_of_cred_reports);
    OpenAPI_plmn_id_nid_free(events_subsc_put_data->plmn_id);
    OpenAPI_list_for_each(events_subsc_put_data->qnc_reports, node) {
        OpenAPI_qos_notification_control_info_free(node->data);
    }
    OpenAPI_list_free(events_subsc_put_data->qnc_reports);
    OpenAPI_list_for_each(events_subsc_put_data->qos_mon_reports, node) {
        OpenAPI_qos_monitoring_report_free(node->data);
    }
    OpenAPI_list_free(events_subsc_put_data->qos_mon_reports);
    OpenAPI_list_for_each(events_subsc_put_data->ran_nas_rel_causes, node) {
        OpenAPI_ran_nas_rel_cause_free(node->data);
    }
    OpenAPI_list_free(events_subsc_put_data->ran_nas_rel_causes);
    OpenAPI_user_location_free(events_subsc_put_data->ue_loc);
    ogs_free(events_subsc_put_data->ue_time_zone);
    OpenAPI_accumulated_usage_free(events_subsc_put_data->usg_rep);
    OpenAPI_bridge_management_container_free(events_subsc_put_data->tsn_bridge_man_cont);
    OpenAPI_port_management_container_free(events_subsc_put_data->tsn_port_man_cont_dstt);
    OpenAPI_list_for_each(events_subsc_put_data->tsn_port_man_cont_nwtts, node) {
        OpenAPI_port_management_container_free(node->data);
    }
    OpenAPI_list_free(events_subsc_put_data->tsn_port_man_cont_nwtts);
    ogs_free(events_subsc_put_data);
}

cJSON *OpenAPI_events_subsc_put_data_convertToJSON(OpenAPI_events_subsc_put_data_t *events_subsc_put_data)
{
    cJSON *item = NULL;

    if (events_subsc_put_data == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [EventsSubscPutData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *eventsList = cJSON_AddArrayToObject(item, "events");
    if (eventsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [events]");
        goto end;
    }

    OpenAPI_lnode_t *events_node;
    if (events_subsc_put_data->events) {
        OpenAPI_list_for_each(events_subsc_put_data->events, events_node) {
            cJSON *itemLocal = OpenAPI_af_event_subscription_convertToJSON(events_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [events]");
                goto end;
            }
            cJSON_AddItemToArray(eventsList, itemLocal);
        }
    }

    if (events_subsc_put_data->notif_uri) {
    if (cJSON_AddStringToObject(item, "notifUri", events_subsc_put_data->notif_uri) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [notif_uri]");
        goto end;
    }
    }

    if (events_subsc_put_data->req_qos_mon_params) {
    cJSON *req_qos_mon_params = cJSON_AddArrayToObject(item, "reqQosMonParams");
    if (req_qos_mon_params == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [req_qos_mon_params]");
        goto end;
    }
    OpenAPI_lnode_t *req_qos_mon_params_node;
    OpenAPI_list_for_each(events_subsc_put_data->req_qos_mon_params, req_qos_mon_params_node) {
        if (cJSON_AddStringToObject(req_qos_mon_params, "", OpenAPI_requested_qos_monitoring_parameter_ToString((intptr_t)req_qos_mon_params_node->data)) == NULL) {
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

    if (events_subsc_put_data->req_anis) {
    cJSON *req_anis = cJSON_AddArrayToObject(item, "reqAnis");
    if (req_anis == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [req_anis]");
        goto end;
    }
    OpenAPI_lnode_t *req_anis_node;
    OpenAPI_list_for_each(events_subsc_put_data->req_anis, req_anis_node) {
        if (cJSON_AddStringToObject(req_anis, "", OpenAPI_required_access_info_ToString((intptr_t)req_anis_node->data)) == NULL) {
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

    if (events_subsc_put_data->access_type) {
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
    }

    if (events_subsc_put_data->an_charg_ids) {
    cJSON *an_charg_idsList = cJSON_AddArrayToObject(item, "anChargIds");
    if (an_charg_idsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [an_charg_ids]");
        goto end;
    }

    OpenAPI_lnode_t *an_charg_ids_node;
    if (events_subsc_put_data->an_charg_ids) {
        OpenAPI_list_for_each(events_subsc_put_data->an_charg_ids, an_charg_ids_node) {
            cJSON *itemLocal = OpenAPI_access_net_charging_identifier_convertToJSON(an_charg_ids_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [an_charg_ids]");
                goto end;
            }
            cJSON_AddItemToArray(an_charg_idsList, itemLocal);
        }
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
    }

    if (cJSON_AddStringToObject(item, "evSubsUri", events_subsc_put_data->ev_subs_uri) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ev_subs_uri]");
        goto end;
    }

    cJSON *ev_notifsList = cJSON_AddArrayToObject(item, "evNotifs");
    if (ev_notifsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ev_notifs]");
        goto end;
    }

    OpenAPI_lnode_t *ev_notifs_node;
    if (events_subsc_put_data->ev_notifs) {
        OpenAPI_list_for_each(events_subsc_put_data->ev_notifs, ev_notifs_node) {
            cJSON *itemLocal = OpenAPI_af_event_notification_convertToJSON(ev_notifs_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ev_notifs]");
                goto end;
            }
            cJSON_AddItemToArray(ev_notifsList, itemLocal);
        }
    }

    if (events_subsc_put_data->failed_resourc_alloc_reports) {
    cJSON *failed_resourc_alloc_reportsList = cJSON_AddArrayToObject(item, "failedResourcAllocReports");
    if (failed_resourc_alloc_reportsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [failed_resourc_alloc_reports]");
        goto end;
    }

    OpenAPI_lnode_t *failed_resourc_alloc_reports_node;
    if (events_subsc_put_data->failed_resourc_alloc_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->failed_resourc_alloc_reports, failed_resourc_alloc_reports_node) {
            cJSON *itemLocal = OpenAPI_resources_allocation_info_convertToJSON(failed_resourc_alloc_reports_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [failed_resourc_alloc_reports]");
                goto end;
            }
            cJSON_AddItemToArray(failed_resourc_alloc_reportsList, itemLocal);
        }
    }
    }

    if (events_subsc_put_data->succ_resourc_alloc_reports) {
    cJSON *succ_resourc_alloc_reportsList = cJSON_AddArrayToObject(item, "succResourcAllocReports");
    if (succ_resourc_alloc_reportsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [succ_resourc_alloc_reports]");
        goto end;
    }

    OpenAPI_lnode_t *succ_resourc_alloc_reports_node;
    if (events_subsc_put_data->succ_resourc_alloc_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->succ_resourc_alloc_reports, succ_resourc_alloc_reports_node) {
            cJSON *itemLocal = OpenAPI_resources_allocation_info_convertToJSON(succ_resourc_alloc_reports_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [succ_resourc_alloc_reports]");
                goto end;
            }
            cJSON_AddItemToArray(succ_resourc_alloc_reportsList, itemLocal);
        }
    }
    }

    if (events_subsc_put_data->no_net_loc_supp) {
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

    OpenAPI_lnode_t *out_of_cred_reports_node;
    if (events_subsc_put_data->out_of_cred_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->out_of_cred_reports, out_of_cred_reports_node) {
            cJSON *itemLocal = OpenAPI_out_of_credit_information_convertToJSON(out_of_cred_reports_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [out_of_cred_reports]");
                goto end;
            }
            cJSON_AddItemToArray(out_of_cred_reportsList, itemLocal);
        }
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

    OpenAPI_lnode_t *qnc_reports_node;
    if (events_subsc_put_data->qnc_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->qnc_reports, qnc_reports_node) {
            cJSON *itemLocal = OpenAPI_qos_notification_control_info_convertToJSON(qnc_reports_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qnc_reports]");
                goto end;
            }
            cJSON_AddItemToArray(qnc_reportsList, itemLocal);
        }
    }
    }

    if (events_subsc_put_data->qos_mon_reports) {
    cJSON *qos_mon_reportsList = cJSON_AddArrayToObject(item, "qosMonReports");
    if (qos_mon_reportsList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qos_mon_reports]");
        goto end;
    }

    OpenAPI_lnode_t *qos_mon_reports_node;
    if (events_subsc_put_data->qos_mon_reports) {
        OpenAPI_list_for_each(events_subsc_put_data->qos_mon_reports, qos_mon_reports_node) {
            cJSON *itemLocal = OpenAPI_qos_monitoring_report_convertToJSON(qos_mon_reports_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [qos_mon_reports]");
                goto end;
            }
            cJSON_AddItemToArray(qos_mon_reportsList, itemLocal);
        }
    }
    }

    if (events_subsc_put_data->ran_nas_rel_causes) {
    cJSON *ran_nas_rel_causesList = cJSON_AddArrayToObject(item, "ranNasRelCauses");
    if (ran_nas_rel_causesList == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ran_nas_rel_causes]");
        goto end;
    }

    OpenAPI_lnode_t *ran_nas_rel_causes_node;
    if (events_subsc_put_data->ran_nas_rel_causes) {
        OpenAPI_list_for_each(events_subsc_put_data->ran_nas_rel_causes, ran_nas_rel_causes_node) {
            cJSON *itemLocal = OpenAPI_ran_nas_rel_cause_convertToJSON(ran_nas_rel_causes_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [ran_nas_rel_causes]");
                goto end;
            }
            cJSON_AddItemToArray(ran_nas_rel_causesList, itemLocal);
        }
    }
    }

    if (events_subsc_put_data->rat_type) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(events_subsc_put_data->rat_type)) == NULL) {
        ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [rat_type]");
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

    OpenAPI_lnode_t *tsn_port_man_cont_nwtts_node;
    if (events_subsc_put_data->tsn_port_man_cont_nwtts) {
        OpenAPI_list_for_each(events_subsc_put_data->tsn_port_man_cont_nwtts, tsn_port_man_cont_nwtts_node) {
            cJSON *itemLocal = OpenAPI_port_management_container_convertToJSON(tsn_port_man_cont_nwtts_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_events_subsc_put_data_convertToJSON() failed [tsn_port_man_cont_nwtts]");
                goto end;
            }
            cJSON_AddItemToArray(tsn_port_man_cont_nwttsList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_events_subsc_put_data_t *OpenAPI_events_subsc_put_data_parseFromJSON(cJSON *events_subsc_put_dataJSON)
{
    OpenAPI_events_subsc_put_data_t *events_subsc_put_data_local_var = NULL;
    cJSON *events = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "events");
    if (!events) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [events]");
        goto end;
    }

    OpenAPI_list_t *eventsList;
    cJSON *events_local_nonprimitive;
    if (!cJSON_IsArray(events)){
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [events]");
        goto end;
    }

    eventsList = OpenAPI_list_create();

    cJSON_ArrayForEach(events_local_nonprimitive, events ) {
        if (!cJSON_IsObject(events_local_nonprimitive)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [events]");
            goto end;
        }
        OpenAPI_af_event_subscription_t *eventsItem = OpenAPI_af_event_subscription_parseFromJSON(events_local_nonprimitive);

        if (!eventsItem) {
            ogs_error("No eventsItem");
            OpenAPI_list_free(eventsList);
            goto end;
        }

        OpenAPI_list_add(eventsList, eventsItem);
    }

    cJSON *notif_uri = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "notifUri");

    if (notif_uri) {
    if (!cJSON_IsString(notif_uri)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    }

    cJSON *req_qos_mon_params = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "reqQosMonParams");

    OpenAPI_list_t *req_qos_mon_paramsList;
    if (req_qos_mon_params) {
    cJSON *req_qos_mon_params_local_nonprimitive;
    if (!cJSON_IsArray(req_qos_mon_params)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [req_qos_mon_params]");
        goto end;
    }

    req_qos_mon_paramsList = OpenAPI_list_create();

    cJSON_ArrayForEach(req_qos_mon_params_local_nonprimitive, req_qos_mon_params ) {
        if (!cJSON_IsString(req_qos_mon_params_local_nonprimitive)){
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [req_qos_mon_params]");
            goto end;
        }

        OpenAPI_list_add(req_qos_mon_paramsList, (void *)OpenAPI_requested_qos_monitoring_parameter_FromString(req_qos_mon_params_local_nonprimitive->valuestring));
    }
    }

    cJSON *qos_mon = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "qosMon");

    OpenAPI_qos_monitoring_information_t *qos_mon_local_nonprim = NULL;
    if (qos_mon) {
    qos_mon_local_nonprim = OpenAPI_qos_monitoring_information_parseFromJSON(qos_mon);
    }

    cJSON *req_anis = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "reqAnis");

    OpenAPI_list_t *req_anisList;
    if (req_anis) {
    cJSON *req_anis_local_nonprimitive;
    if (!cJSON_IsArray(req_anis)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [req_anis]");
        goto end;
    }

    req_anisList = OpenAPI_list_create();

    cJSON_ArrayForEach(req_anis_local_nonprimitive, req_anis ) {
        if (!cJSON_IsString(req_anis_local_nonprimitive)){
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [req_anis]");
            goto end;
        }

        OpenAPI_list_add(req_anisList, (void *)OpenAPI_required_access_info_FromString(req_anis_local_nonprimitive->valuestring));
    }
    }

    cJSON *usg_thres = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "usgThres");

    OpenAPI_usage_threshold_t *usg_thres_local_nonprim = NULL;
    if (usg_thres) {
    usg_thres_local_nonprim = OpenAPI_usage_threshold_parseFromJSON(usg_thres);
    }

    cJSON *notif_corre_id = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "notifCorreId");

    if (notif_corre_id) {
    if (!cJSON_IsString(notif_corre_id)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }
    }

    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "accessType");

    OpenAPI_access_type_e access_typeVariable;
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    cJSON *add_access_info = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "addAccessInfo");

    OpenAPI_additional_access_info_t *add_access_info_local_nonprim = NULL;
    if (add_access_info) {
    add_access_info_local_nonprim = OpenAPI_additional_access_info_parseFromJSON(add_access_info);
    }

    cJSON *rel_access_info = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "relAccessInfo");

    OpenAPI_additional_access_info_t *rel_access_info_local_nonprim = NULL;
    if (rel_access_info) {
    rel_access_info_local_nonprim = OpenAPI_additional_access_info_parseFromJSON(rel_access_info);
    }

    cJSON *an_charg_addr = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "anChargAddr");

    OpenAPI_acc_net_charging_address_t *an_charg_addr_local_nonprim = NULL;
    if (an_charg_addr) {
    an_charg_addr_local_nonprim = OpenAPI_acc_net_charging_address_parseFromJSON(an_charg_addr);
    }

    cJSON *an_charg_ids = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "anChargIds");

    OpenAPI_list_t *an_charg_idsList;
    if (an_charg_ids) {
    cJSON *an_charg_ids_local_nonprimitive;
    if (!cJSON_IsArray(an_charg_ids)){
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [an_charg_ids]");
        goto end;
    }

    an_charg_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(an_charg_ids_local_nonprimitive, an_charg_ids ) {
        if (!cJSON_IsObject(an_charg_ids_local_nonprimitive)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [an_charg_ids]");
            goto end;
        }
        OpenAPI_access_net_charging_identifier_t *an_charg_idsItem = OpenAPI_access_net_charging_identifier_parseFromJSON(an_charg_ids_local_nonprimitive);

        if (!an_charg_idsItem) {
            ogs_error("No an_charg_idsItem");
            OpenAPI_list_free(an_charg_idsList);
            goto end;
        }

        OpenAPI_list_add(an_charg_idsList, an_charg_idsItem);
    }
    }

    cJSON *an_gw_addr = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "anGwAddr");

    OpenAPI_an_gw_address_t *an_gw_addr_local_nonprim = NULL;
    if (an_gw_addr) {
    an_gw_addr_local_nonprim = OpenAPI_an_gw_address_parseFromJSON(an_gw_addr);
    }

    cJSON *ev_subs_uri = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "evSubsUri");
    if (!ev_subs_uri) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ev_subs_uri]");
        goto end;
    }

    if (!cJSON_IsString(ev_subs_uri)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ev_subs_uri]");
        goto end;
    }

    cJSON *ev_notifs = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "evNotifs");
    if (!ev_notifs) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ev_notifs]");
        goto end;
    }

    OpenAPI_list_t *ev_notifsList;
    cJSON *ev_notifs_local_nonprimitive;
    if (!cJSON_IsArray(ev_notifs)){
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ev_notifs]");
        goto end;
    }

    ev_notifsList = OpenAPI_list_create();

    cJSON_ArrayForEach(ev_notifs_local_nonprimitive, ev_notifs ) {
        if (!cJSON_IsObject(ev_notifs_local_nonprimitive)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ev_notifs]");
            goto end;
        }
        OpenAPI_af_event_notification_t *ev_notifsItem = OpenAPI_af_event_notification_parseFromJSON(ev_notifs_local_nonprimitive);

        if (!ev_notifsItem) {
            ogs_error("No ev_notifsItem");
            OpenAPI_list_free(ev_notifsList);
            goto end;
        }

        OpenAPI_list_add(ev_notifsList, ev_notifsItem);
    }

    cJSON *failed_resourc_alloc_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "failedResourcAllocReports");

    OpenAPI_list_t *failed_resourc_alloc_reportsList;
    if (failed_resourc_alloc_reports) {
    cJSON *failed_resourc_alloc_reports_local_nonprimitive;
    if (!cJSON_IsArray(failed_resourc_alloc_reports)){
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [failed_resourc_alloc_reports]");
        goto end;
    }

    failed_resourc_alloc_reportsList = OpenAPI_list_create();

    cJSON_ArrayForEach(failed_resourc_alloc_reports_local_nonprimitive, failed_resourc_alloc_reports ) {
        if (!cJSON_IsObject(failed_resourc_alloc_reports_local_nonprimitive)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [failed_resourc_alloc_reports]");
            goto end;
        }
        OpenAPI_resources_allocation_info_t *failed_resourc_alloc_reportsItem = OpenAPI_resources_allocation_info_parseFromJSON(failed_resourc_alloc_reports_local_nonprimitive);

        if (!failed_resourc_alloc_reportsItem) {
            ogs_error("No failed_resourc_alloc_reportsItem");
            OpenAPI_list_free(failed_resourc_alloc_reportsList);
            goto end;
        }

        OpenAPI_list_add(failed_resourc_alloc_reportsList, failed_resourc_alloc_reportsItem);
    }
    }

    cJSON *succ_resourc_alloc_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "succResourcAllocReports");

    OpenAPI_list_t *succ_resourc_alloc_reportsList;
    if (succ_resourc_alloc_reports) {
    cJSON *succ_resourc_alloc_reports_local_nonprimitive;
    if (!cJSON_IsArray(succ_resourc_alloc_reports)){
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [succ_resourc_alloc_reports]");
        goto end;
    }

    succ_resourc_alloc_reportsList = OpenAPI_list_create();

    cJSON_ArrayForEach(succ_resourc_alloc_reports_local_nonprimitive, succ_resourc_alloc_reports ) {
        if (!cJSON_IsObject(succ_resourc_alloc_reports_local_nonprimitive)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [succ_resourc_alloc_reports]");
            goto end;
        }
        OpenAPI_resources_allocation_info_t *succ_resourc_alloc_reportsItem = OpenAPI_resources_allocation_info_parseFromJSON(succ_resourc_alloc_reports_local_nonprimitive);

        if (!succ_resourc_alloc_reportsItem) {
            ogs_error("No succ_resourc_alloc_reportsItem");
            OpenAPI_list_free(succ_resourc_alloc_reportsList);
            goto end;
        }

        OpenAPI_list_add(succ_resourc_alloc_reportsList, succ_resourc_alloc_reportsItem);
    }
    }

    cJSON *no_net_loc_supp = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "noNetLocSupp");

    OpenAPI_net_loc_access_support_e no_net_loc_suppVariable;
    if (no_net_loc_supp) {
    if (!cJSON_IsString(no_net_loc_supp)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [no_net_loc_supp]");
        goto end;
    }
    no_net_loc_suppVariable = OpenAPI_net_loc_access_support_FromString(no_net_loc_supp->valuestring);
    }

    cJSON *out_of_cred_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "outOfCredReports");

    OpenAPI_list_t *out_of_cred_reportsList;
    if (out_of_cred_reports) {
    cJSON *out_of_cred_reports_local_nonprimitive;
    if (!cJSON_IsArray(out_of_cred_reports)){
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [out_of_cred_reports]");
        goto end;
    }

    out_of_cred_reportsList = OpenAPI_list_create();

    cJSON_ArrayForEach(out_of_cred_reports_local_nonprimitive, out_of_cred_reports ) {
        if (!cJSON_IsObject(out_of_cred_reports_local_nonprimitive)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [out_of_cred_reports]");
            goto end;
        }
        OpenAPI_out_of_credit_information_t *out_of_cred_reportsItem = OpenAPI_out_of_credit_information_parseFromJSON(out_of_cred_reports_local_nonprimitive);

        if (!out_of_cred_reportsItem) {
            ogs_error("No out_of_cred_reportsItem");
            OpenAPI_list_free(out_of_cred_reportsList);
            goto end;
        }

        OpenAPI_list_add(out_of_cred_reportsList, out_of_cred_reportsItem);
    }
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "plmnId");

    OpenAPI_plmn_id_nid_t *plmn_id_local_nonprim = NULL;
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(plmn_id);
    }

    cJSON *qnc_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "qncReports");

    OpenAPI_list_t *qnc_reportsList;
    if (qnc_reports) {
    cJSON *qnc_reports_local_nonprimitive;
    if (!cJSON_IsArray(qnc_reports)){
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [qnc_reports]");
        goto end;
    }

    qnc_reportsList = OpenAPI_list_create();

    cJSON_ArrayForEach(qnc_reports_local_nonprimitive, qnc_reports ) {
        if (!cJSON_IsObject(qnc_reports_local_nonprimitive)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [qnc_reports]");
            goto end;
        }
        OpenAPI_qos_notification_control_info_t *qnc_reportsItem = OpenAPI_qos_notification_control_info_parseFromJSON(qnc_reports_local_nonprimitive);

        if (!qnc_reportsItem) {
            ogs_error("No qnc_reportsItem");
            OpenAPI_list_free(qnc_reportsList);
            goto end;
        }

        OpenAPI_list_add(qnc_reportsList, qnc_reportsItem);
    }
    }

    cJSON *qos_mon_reports = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "qosMonReports");

    OpenAPI_list_t *qos_mon_reportsList;
    if (qos_mon_reports) {
    cJSON *qos_mon_reports_local_nonprimitive;
    if (!cJSON_IsArray(qos_mon_reports)){
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [qos_mon_reports]");
        goto end;
    }

    qos_mon_reportsList = OpenAPI_list_create();

    cJSON_ArrayForEach(qos_mon_reports_local_nonprimitive, qos_mon_reports ) {
        if (!cJSON_IsObject(qos_mon_reports_local_nonprimitive)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [qos_mon_reports]");
            goto end;
        }
        OpenAPI_qos_monitoring_report_t *qos_mon_reportsItem = OpenAPI_qos_monitoring_report_parseFromJSON(qos_mon_reports_local_nonprimitive);

        if (!qos_mon_reportsItem) {
            ogs_error("No qos_mon_reportsItem");
            OpenAPI_list_free(qos_mon_reportsList);
            goto end;
        }

        OpenAPI_list_add(qos_mon_reportsList, qos_mon_reportsItem);
    }
    }

    cJSON *ran_nas_rel_causes = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ranNasRelCauses");

    OpenAPI_list_t *ran_nas_rel_causesList;
    if (ran_nas_rel_causes) {
    cJSON *ran_nas_rel_causes_local_nonprimitive;
    if (!cJSON_IsArray(ran_nas_rel_causes)){
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ran_nas_rel_causes]");
        goto end;
    }

    ran_nas_rel_causesList = OpenAPI_list_create();

    cJSON_ArrayForEach(ran_nas_rel_causes_local_nonprimitive, ran_nas_rel_causes ) {
        if (!cJSON_IsObject(ran_nas_rel_causes_local_nonprimitive)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ran_nas_rel_causes]");
            goto end;
        }
        OpenAPI_ran_nas_rel_cause_t *ran_nas_rel_causesItem = OpenAPI_ran_nas_rel_cause_parseFromJSON(ran_nas_rel_causes_local_nonprimitive);

        if (!ran_nas_rel_causesItem) {
            ogs_error("No ran_nas_rel_causesItem");
            OpenAPI_list_free(ran_nas_rel_causesList);
            goto end;
        }

        OpenAPI_list_add(ran_nas_rel_causesList, ran_nas_rel_causesItem);
    }
    }

    cJSON *rat_type = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ratType");

    OpenAPI_rat_type_e rat_typeVariable;
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    cJSON *ue_loc = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ueLoc");

    OpenAPI_user_location_t *ue_loc_local_nonprim = NULL;
    if (ue_loc) {
    ue_loc_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_loc);
    }

    cJSON *ue_time_zone = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "ueTimeZone");

    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone)) {
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    cJSON *usg_rep = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "usgRep");

    OpenAPI_accumulated_usage_t *usg_rep_local_nonprim = NULL;
    if (usg_rep) {
    usg_rep_local_nonprim = OpenAPI_accumulated_usage_parseFromJSON(usg_rep);
    }

    cJSON *tsn_bridge_man_cont = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "tsnBridgeManCont");

    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont_local_nonprim = NULL;
    if (tsn_bridge_man_cont) {
    tsn_bridge_man_cont_local_nonprim = OpenAPI_bridge_management_container_parseFromJSON(tsn_bridge_man_cont);
    }

    cJSON *tsn_port_man_cont_dstt = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "tsnPortManContDstt");

    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt_local_nonprim = NULL;
    if (tsn_port_man_cont_dstt) {
    tsn_port_man_cont_dstt_local_nonprim = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_dstt);
    }

    cJSON *tsn_port_man_cont_nwtts = cJSON_GetObjectItemCaseSensitive(events_subsc_put_dataJSON, "tsnPortManContNwtts");

    OpenAPI_list_t *tsn_port_man_cont_nwttsList;
    if (tsn_port_man_cont_nwtts) {
    cJSON *tsn_port_man_cont_nwtts_local_nonprimitive;
    if (!cJSON_IsArray(tsn_port_man_cont_nwtts)){
        ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
        goto end;
    }

    tsn_port_man_cont_nwttsList = OpenAPI_list_create();

    cJSON_ArrayForEach(tsn_port_man_cont_nwtts_local_nonprimitive, tsn_port_man_cont_nwtts ) {
        if (!cJSON_IsObject(tsn_port_man_cont_nwtts_local_nonprimitive)) {
            ogs_error("OpenAPI_events_subsc_put_data_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }
        OpenAPI_port_management_container_t *tsn_port_man_cont_nwttsItem = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_nwtts_local_nonprimitive);

        if (!tsn_port_man_cont_nwttsItem) {
            ogs_error("No tsn_port_man_cont_nwttsItem");
            OpenAPI_list_free(tsn_port_man_cont_nwttsList);
            goto end;
        }

        OpenAPI_list_add(tsn_port_man_cont_nwttsList, tsn_port_man_cont_nwttsItem);
    }
    }

    events_subsc_put_data_local_var = OpenAPI_events_subsc_put_data_create (
        eventsList,
        notif_uri ? ogs_strdup(notif_uri->valuestring) : NULL,
        req_qos_mon_params ? req_qos_mon_paramsList : NULL,
        qos_mon ? qos_mon_local_nonprim : NULL,
        req_anis ? req_anisList : NULL,
        usg_thres ? usg_thres_local_nonprim : NULL,
        notif_corre_id ? ogs_strdup(notif_corre_id->valuestring) : NULL,
        access_type ? access_typeVariable : 0,
        add_access_info ? add_access_info_local_nonprim : NULL,
        rel_access_info ? rel_access_info_local_nonprim : NULL,
        an_charg_addr ? an_charg_addr_local_nonprim : NULL,
        an_charg_ids ? an_charg_idsList : NULL,
        an_gw_addr ? an_gw_addr_local_nonprim : NULL,
        ogs_strdup(ev_subs_uri->valuestring),
        ev_notifsList,
        failed_resourc_alloc_reports ? failed_resourc_alloc_reportsList : NULL,
        succ_resourc_alloc_reports ? succ_resourc_alloc_reportsList : NULL,
        no_net_loc_supp ? no_net_loc_suppVariable : 0,
        out_of_cred_reports ? out_of_cred_reportsList : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        qnc_reports ? qnc_reportsList : NULL,
        qos_mon_reports ? qos_mon_reportsList : NULL,
        ran_nas_rel_causes ? ran_nas_rel_causesList : NULL,
        rat_type ? rat_typeVariable : 0,
        ue_loc ? ue_loc_local_nonprim : NULL,
        ue_time_zone ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        usg_rep ? usg_rep_local_nonprim : NULL,
        tsn_bridge_man_cont ? tsn_bridge_man_cont_local_nonprim : NULL,
        tsn_port_man_cont_dstt ? tsn_port_man_cont_dstt_local_nonprim : NULL,
        tsn_port_man_cont_nwtts ? tsn_port_man_cont_nwttsList : NULL
    );

    return events_subsc_put_data_local_var;
end:
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

