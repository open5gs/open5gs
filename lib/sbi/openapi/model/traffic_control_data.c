
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_control_data.h"

OpenAPI_traffic_control_data_t *OpenAPI_traffic_control_data_create(
    char *tc_id,
    OpenAPI_flow_status_e flow_status,
    OpenAPI_redirect_information_t *redirect_info,
    OpenAPI_list_t *add_redirect_info,
    bool is_mute_notif,
    int mute_notif,
    bool is_traffic_steering_pol_id_dl_null,
    char *traffic_steering_pol_id_dl,
    bool is_traffic_steering_pol_id_ul_null,
    char *traffic_steering_pol_id_ul,
    bool is_route_to_locs_null,
    OpenAPI_list_t *route_to_locs,
    bool is_max_allowed_up_lat_null,
    bool is_max_allowed_up_lat,
    int max_allowed_up_lat,
    bool is_eas_ip_replace_infos_null,
    OpenAPI_list_t *eas_ip_replace_infos,
    bool is_traff_corre_ind,
    int traff_corre_ind,
    bool is_sim_conn_ind,
    int sim_conn_ind,
    bool is_sim_conn_term,
    int sim_conn_term,
    bool is_up_path_chg_event_null,
    OpenAPI_up_path_chg_event_t *up_path_chg_event,
    OpenAPI_steering_functionality_e steer_fun,
    OpenAPI_steering_mode_t *steer_mode_dl,
    OpenAPI_steering_mode_t *steer_mode_ul,
    OpenAPI_npcf_multicast_access_control_e mul_acc_ctrl
)
{
    OpenAPI_traffic_control_data_t *traffic_control_data_local_var = ogs_malloc(sizeof(OpenAPI_traffic_control_data_t));
    ogs_assert(traffic_control_data_local_var);

    traffic_control_data_local_var->tc_id = tc_id;
    traffic_control_data_local_var->flow_status = flow_status;
    traffic_control_data_local_var->redirect_info = redirect_info;
    traffic_control_data_local_var->add_redirect_info = add_redirect_info;
    traffic_control_data_local_var->is_mute_notif = is_mute_notif;
    traffic_control_data_local_var->mute_notif = mute_notif;
    traffic_control_data_local_var->is_traffic_steering_pol_id_dl_null = is_traffic_steering_pol_id_dl_null;
    traffic_control_data_local_var->traffic_steering_pol_id_dl = traffic_steering_pol_id_dl;
    traffic_control_data_local_var->is_traffic_steering_pol_id_ul_null = is_traffic_steering_pol_id_ul_null;
    traffic_control_data_local_var->traffic_steering_pol_id_ul = traffic_steering_pol_id_ul;
    traffic_control_data_local_var->is_route_to_locs_null = is_route_to_locs_null;
    traffic_control_data_local_var->route_to_locs = route_to_locs;
    traffic_control_data_local_var->is_max_allowed_up_lat_null = is_max_allowed_up_lat_null;
    traffic_control_data_local_var->is_max_allowed_up_lat = is_max_allowed_up_lat;
    traffic_control_data_local_var->max_allowed_up_lat = max_allowed_up_lat;
    traffic_control_data_local_var->is_eas_ip_replace_infos_null = is_eas_ip_replace_infos_null;
    traffic_control_data_local_var->eas_ip_replace_infos = eas_ip_replace_infos;
    traffic_control_data_local_var->is_traff_corre_ind = is_traff_corre_ind;
    traffic_control_data_local_var->traff_corre_ind = traff_corre_ind;
    traffic_control_data_local_var->is_sim_conn_ind = is_sim_conn_ind;
    traffic_control_data_local_var->sim_conn_ind = sim_conn_ind;
    traffic_control_data_local_var->is_sim_conn_term = is_sim_conn_term;
    traffic_control_data_local_var->sim_conn_term = sim_conn_term;
    traffic_control_data_local_var->is_up_path_chg_event_null = is_up_path_chg_event_null;
    traffic_control_data_local_var->up_path_chg_event = up_path_chg_event;
    traffic_control_data_local_var->steer_fun = steer_fun;
    traffic_control_data_local_var->steer_mode_dl = steer_mode_dl;
    traffic_control_data_local_var->steer_mode_ul = steer_mode_ul;
    traffic_control_data_local_var->mul_acc_ctrl = mul_acc_ctrl;

    return traffic_control_data_local_var;
}

void OpenAPI_traffic_control_data_free(OpenAPI_traffic_control_data_t *traffic_control_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_control_data) {
        return;
    }
    if (traffic_control_data->tc_id) {
        ogs_free(traffic_control_data->tc_id);
        traffic_control_data->tc_id = NULL;
    }
    if (traffic_control_data->redirect_info) {
        OpenAPI_redirect_information_free(traffic_control_data->redirect_info);
        traffic_control_data->redirect_info = NULL;
    }
    if (traffic_control_data->add_redirect_info) {
        OpenAPI_list_for_each(traffic_control_data->add_redirect_info, node) {
            OpenAPI_redirect_information_free(node->data);
        }
        OpenAPI_list_free(traffic_control_data->add_redirect_info);
        traffic_control_data->add_redirect_info = NULL;
    }
    if (traffic_control_data->traffic_steering_pol_id_dl) {
        ogs_free(traffic_control_data->traffic_steering_pol_id_dl);
        traffic_control_data->traffic_steering_pol_id_dl = NULL;
    }
    if (traffic_control_data->traffic_steering_pol_id_ul) {
        ogs_free(traffic_control_data->traffic_steering_pol_id_ul);
        traffic_control_data->traffic_steering_pol_id_ul = NULL;
    }
    if (traffic_control_data->route_to_locs) {
        OpenAPI_list_for_each(traffic_control_data->route_to_locs, node) {
            OpenAPI_route_to_location_free(node->data);
        }
        OpenAPI_list_free(traffic_control_data->route_to_locs);
        traffic_control_data->route_to_locs = NULL;
    }
    if (traffic_control_data->eas_ip_replace_infos) {
        OpenAPI_list_for_each(traffic_control_data->eas_ip_replace_infos, node) {
            OpenAPI_eas_ip_replacement_info_free(node->data);
        }
        OpenAPI_list_free(traffic_control_data->eas_ip_replace_infos);
        traffic_control_data->eas_ip_replace_infos = NULL;
    }
    if (traffic_control_data->up_path_chg_event) {
        OpenAPI_up_path_chg_event_free(traffic_control_data->up_path_chg_event);
        traffic_control_data->up_path_chg_event = NULL;
    }
    if (traffic_control_data->steer_mode_dl) {
        OpenAPI_steering_mode_free(traffic_control_data->steer_mode_dl);
        traffic_control_data->steer_mode_dl = NULL;
    }
    if (traffic_control_data->steer_mode_ul) {
        OpenAPI_steering_mode_free(traffic_control_data->steer_mode_ul);
        traffic_control_data->steer_mode_ul = NULL;
    }
    ogs_free(traffic_control_data);
}

cJSON *OpenAPI_traffic_control_data_convertToJSON(OpenAPI_traffic_control_data_t *traffic_control_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_control_data == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [TrafficControlData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!traffic_control_data->tc_id) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [tc_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "tcId", traffic_control_data->tc_id) == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [tc_id]");
        goto end;
    }

    if (traffic_control_data->flow_status != OpenAPI_flow_status_NULL) {
    if (cJSON_AddStringToObject(item, "flowStatus", OpenAPI_flow_status_ToString(traffic_control_data->flow_status)) == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [flow_status]");
        goto end;
    }
    }

    if (traffic_control_data->redirect_info) {
    cJSON *redirect_info_local_JSON = OpenAPI_redirect_information_convertToJSON(traffic_control_data->redirect_info);
    if (redirect_info_local_JSON == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [redirect_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "redirectInfo", redirect_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [redirect_info]");
        goto end;
    }
    }

    if (traffic_control_data->add_redirect_info) {
    cJSON *add_redirect_infoList = cJSON_AddArrayToObject(item, "addRedirectInfo");
    if (add_redirect_infoList == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [add_redirect_info]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_control_data->add_redirect_info, node) {
        cJSON *itemLocal = OpenAPI_redirect_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [add_redirect_info]");
            goto end;
        }
        cJSON_AddItemToArray(add_redirect_infoList, itemLocal);
    }
    }

    if (traffic_control_data->is_mute_notif) {
    if (cJSON_AddBoolToObject(item, "muteNotif", traffic_control_data->mute_notif) == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [mute_notif]");
        goto end;
    }
    }

    if (traffic_control_data->traffic_steering_pol_id_dl) {
    if (cJSON_AddStringToObject(item, "trafficSteeringPolIdDl", traffic_control_data->traffic_steering_pol_id_dl) == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [traffic_steering_pol_id_dl]");
        goto end;
    }
    } else if (traffic_control_data->is_traffic_steering_pol_id_dl_null) {
        if (cJSON_AddNullToObject(item, "trafficSteeringPolIdDl") == NULL) {
            ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [traffic_steering_pol_id_dl]");
            goto end;
        }
    }

    if (traffic_control_data->traffic_steering_pol_id_ul) {
    if (cJSON_AddStringToObject(item, "trafficSteeringPolIdUl", traffic_control_data->traffic_steering_pol_id_ul) == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [traffic_steering_pol_id_ul]");
        goto end;
    }
    } else if (traffic_control_data->is_traffic_steering_pol_id_ul_null) {
        if (cJSON_AddNullToObject(item, "trafficSteeringPolIdUl") == NULL) {
            ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [traffic_steering_pol_id_ul]");
            goto end;
        }
    }

    if (traffic_control_data->route_to_locs) {
    cJSON *route_to_locsList = cJSON_AddArrayToObject(item, "routeToLocs");
    if (route_to_locsList == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [route_to_locs]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_control_data->route_to_locs, node) {
        cJSON *itemLocal = OpenAPI_route_to_location_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [route_to_locs]");
            goto end;
        }
        cJSON_AddItemToArray(route_to_locsList, itemLocal);
    }
    } else if (traffic_control_data->is_route_to_locs_null) {
        if (cJSON_AddNullToObject(item, "routeToLocs") == NULL) {
            ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [route_to_locs]");
            goto end;
        }
    }

    if (traffic_control_data->is_max_allowed_up_lat) {
    if (cJSON_AddNumberToObject(item, "maxAllowedUpLat", traffic_control_data->max_allowed_up_lat) == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [max_allowed_up_lat]");
        goto end;
    }
    } else if (traffic_control_data->is_max_allowed_up_lat_null) {
        if (cJSON_AddNullToObject(item, "maxAllowedUpLat") == NULL) {
            ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [max_allowed_up_lat]");
            goto end;
        }
    }

    if (traffic_control_data->eas_ip_replace_infos) {
    cJSON *eas_ip_replace_infosList = cJSON_AddArrayToObject(item, "easIpReplaceInfos");
    if (eas_ip_replace_infosList == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [eas_ip_replace_infos]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_control_data->eas_ip_replace_infos, node) {
        cJSON *itemLocal = OpenAPI_eas_ip_replacement_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [eas_ip_replace_infos]");
            goto end;
        }
        cJSON_AddItemToArray(eas_ip_replace_infosList, itemLocal);
    }
    } else if (traffic_control_data->is_eas_ip_replace_infos_null) {
        if (cJSON_AddNullToObject(item, "easIpReplaceInfos") == NULL) {
            ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [eas_ip_replace_infos]");
            goto end;
        }
    }

    if (traffic_control_data->is_traff_corre_ind) {
    if (cJSON_AddBoolToObject(item, "traffCorreInd", traffic_control_data->traff_corre_ind) == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [traff_corre_ind]");
        goto end;
    }
    }

    if (traffic_control_data->is_sim_conn_ind) {
    if (cJSON_AddBoolToObject(item, "simConnInd", traffic_control_data->sim_conn_ind) == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [sim_conn_ind]");
        goto end;
    }
    }

    if (traffic_control_data->is_sim_conn_term) {
    if (cJSON_AddNumberToObject(item, "simConnTerm", traffic_control_data->sim_conn_term) == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [sim_conn_term]");
        goto end;
    }
    }

    if (traffic_control_data->up_path_chg_event) {
    cJSON *up_path_chg_event_local_JSON = OpenAPI_up_path_chg_event_convertToJSON(traffic_control_data->up_path_chg_event);
    if (up_path_chg_event_local_JSON == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [up_path_chg_event]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upPathChgEvent", up_path_chg_event_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [up_path_chg_event]");
        goto end;
    }
    } else if (traffic_control_data->is_up_path_chg_event_null) {
        if (cJSON_AddNullToObject(item, "upPathChgEvent") == NULL) {
            ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [up_path_chg_event]");
            goto end;
        }
    }

    if (traffic_control_data->steer_fun != OpenAPI_steering_functionality_NULL) {
    if (cJSON_AddStringToObject(item, "steerFun", OpenAPI_steering_functionality_ToString(traffic_control_data->steer_fun)) == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [steer_fun]");
        goto end;
    }
    }

    if (traffic_control_data->steer_mode_dl) {
    cJSON *steer_mode_dl_local_JSON = OpenAPI_steering_mode_convertToJSON(traffic_control_data->steer_mode_dl);
    if (steer_mode_dl_local_JSON == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [steer_mode_dl]");
        goto end;
    }
    cJSON_AddItemToObject(item, "steerModeDl", steer_mode_dl_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [steer_mode_dl]");
        goto end;
    }
    }

    if (traffic_control_data->steer_mode_ul) {
    cJSON *steer_mode_ul_local_JSON = OpenAPI_steering_mode_convertToJSON(traffic_control_data->steer_mode_ul);
    if (steer_mode_ul_local_JSON == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [steer_mode_ul]");
        goto end;
    }
    cJSON_AddItemToObject(item, "steerModeUl", steer_mode_ul_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [steer_mode_ul]");
        goto end;
    }
    }

    if (traffic_control_data->mul_acc_ctrl != OpenAPI_npcf_multicast_access_control_NULL) {
    if (cJSON_AddStringToObject(item, "mulAccCtrl", OpenAPI_npcf_multicast_access_control_ToString(traffic_control_data->mul_acc_ctrl)) == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [mul_acc_ctrl]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_traffic_control_data_t *OpenAPI_traffic_control_data_parseFromJSON(cJSON *traffic_control_dataJSON)
{
    OpenAPI_traffic_control_data_t *traffic_control_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tc_id = NULL;
    cJSON *flow_status = NULL;
    OpenAPI_flow_status_e flow_statusVariable = 0;
    cJSON *redirect_info = NULL;
    OpenAPI_redirect_information_t *redirect_info_local_nonprim = NULL;
    cJSON *add_redirect_info = NULL;
    OpenAPI_list_t *add_redirect_infoList = NULL;
    cJSON *mute_notif = NULL;
    cJSON *traffic_steering_pol_id_dl = NULL;
    cJSON *traffic_steering_pol_id_ul = NULL;
    cJSON *route_to_locs = NULL;
    OpenAPI_list_t *route_to_locsList = NULL;
    cJSON *max_allowed_up_lat = NULL;
    cJSON *eas_ip_replace_infos = NULL;
    OpenAPI_list_t *eas_ip_replace_infosList = NULL;
    cJSON *traff_corre_ind = NULL;
    cJSON *sim_conn_ind = NULL;
    cJSON *sim_conn_term = NULL;
    cJSON *up_path_chg_event = NULL;
    OpenAPI_up_path_chg_event_t *up_path_chg_event_local_nonprim = NULL;
    cJSON *steer_fun = NULL;
    OpenAPI_steering_functionality_e steer_funVariable = 0;
    cJSON *steer_mode_dl = NULL;
    OpenAPI_steering_mode_t *steer_mode_dl_local_nonprim = NULL;
    cJSON *steer_mode_ul = NULL;
    OpenAPI_steering_mode_t *steer_mode_ul_local_nonprim = NULL;
    cJSON *mul_acc_ctrl = NULL;
    OpenAPI_npcf_multicast_access_control_e mul_acc_ctrlVariable = 0;
    tc_id = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "tcId");
    if (!tc_id) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [tc_id]");
        goto end;
    }
    if (!cJSON_IsString(tc_id)) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [tc_id]");
        goto end;
    }

    flow_status = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "flowStatus");
    if (flow_status) {
    if (!cJSON_IsString(flow_status)) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [flow_status]");
        goto end;
    }
    flow_statusVariable = OpenAPI_flow_status_FromString(flow_status->valuestring);
    }

    redirect_info = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "redirectInfo");
    if (redirect_info) {
    redirect_info_local_nonprim = OpenAPI_redirect_information_parseFromJSON(redirect_info);
    if (!redirect_info_local_nonprim) {
        ogs_error("OpenAPI_redirect_information_parseFromJSON failed [redirect_info]");
        goto end;
    }
    }

    add_redirect_info = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "addRedirectInfo");
    if (add_redirect_info) {
        cJSON *add_redirect_info_local = NULL;
        if (!cJSON_IsArray(add_redirect_info)) {
            ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [add_redirect_info]");
            goto end;
        }

        add_redirect_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_redirect_info_local, add_redirect_info) {
            if (!cJSON_IsObject(add_redirect_info_local)) {
                ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [add_redirect_info]");
                goto end;
            }
            OpenAPI_redirect_information_t *add_redirect_infoItem = OpenAPI_redirect_information_parseFromJSON(add_redirect_info_local);
            if (!add_redirect_infoItem) {
                ogs_error("No add_redirect_infoItem");
                goto end;
            }
            OpenAPI_list_add(add_redirect_infoList, add_redirect_infoItem);
        }
    }

    mute_notif = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "muteNotif");
    if (mute_notif) {
    if (!cJSON_IsBool(mute_notif)) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [mute_notif]");
        goto end;
    }
    }

    traffic_steering_pol_id_dl = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "trafficSteeringPolIdDl");
    if (traffic_steering_pol_id_dl) {
    if (!cJSON_IsNull(traffic_steering_pol_id_dl)) {
    if (!cJSON_IsString(traffic_steering_pol_id_dl) && !cJSON_IsNull(traffic_steering_pol_id_dl)) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [traffic_steering_pol_id_dl]");
        goto end;
    }
    }
    }

    traffic_steering_pol_id_ul = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "trafficSteeringPolIdUl");
    if (traffic_steering_pol_id_ul) {
    if (!cJSON_IsNull(traffic_steering_pol_id_ul)) {
    if (!cJSON_IsString(traffic_steering_pol_id_ul) && !cJSON_IsNull(traffic_steering_pol_id_ul)) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [traffic_steering_pol_id_ul]");
        goto end;
    }
    }
    }

    route_to_locs = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "routeToLocs");
    if (route_to_locs) {
    if (!cJSON_IsNull(route_to_locs)) {
        cJSON *route_to_locs_local = NULL;
        if (!cJSON_IsArray(route_to_locs)) {
            ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [route_to_locs]");
            goto end;
        }

        route_to_locsList = OpenAPI_list_create();

        cJSON_ArrayForEach(route_to_locs_local, route_to_locs) {
            if (!cJSON_IsObject(route_to_locs_local)) {
                ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [route_to_locs]");
                goto end;
            }
            OpenAPI_route_to_location_t *route_to_locsItem = OpenAPI_route_to_location_parseFromJSON(route_to_locs_local);
            if (!route_to_locsItem) {
                ogs_error("No route_to_locsItem");
                goto end;
            }
            OpenAPI_list_add(route_to_locsList, route_to_locsItem);
        }
    }
    }

    max_allowed_up_lat = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "maxAllowedUpLat");
    if (max_allowed_up_lat) {
    if (!cJSON_IsNull(max_allowed_up_lat)) {
    if (!cJSON_IsNumber(max_allowed_up_lat)) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [max_allowed_up_lat]");
        goto end;
    }
    }
    }

    eas_ip_replace_infos = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "easIpReplaceInfos");
    if (eas_ip_replace_infos) {
    if (!cJSON_IsNull(eas_ip_replace_infos)) {
        cJSON *eas_ip_replace_infos_local = NULL;
        if (!cJSON_IsArray(eas_ip_replace_infos)) {
            ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [eas_ip_replace_infos]");
            goto end;
        }

        eas_ip_replace_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(eas_ip_replace_infos_local, eas_ip_replace_infos) {
            if (!cJSON_IsObject(eas_ip_replace_infos_local)) {
                ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [eas_ip_replace_infos]");
                goto end;
            }
            OpenAPI_eas_ip_replacement_info_t *eas_ip_replace_infosItem = OpenAPI_eas_ip_replacement_info_parseFromJSON(eas_ip_replace_infos_local);
            if (!eas_ip_replace_infosItem) {
                ogs_error("No eas_ip_replace_infosItem");
                goto end;
            }
            OpenAPI_list_add(eas_ip_replace_infosList, eas_ip_replace_infosItem);
        }
    }
    }

    traff_corre_ind = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "traffCorreInd");
    if (traff_corre_ind) {
    if (!cJSON_IsBool(traff_corre_ind)) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [traff_corre_ind]");
        goto end;
    }
    }

    sim_conn_ind = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "simConnInd");
    if (sim_conn_ind) {
    if (!cJSON_IsBool(sim_conn_ind)) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [sim_conn_ind]");
        goto end;
    }
    }

    sim_conn_term = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "simConnTerm");
    if (sim_conn_term) {
    if (!cJSON_IsNumber(sim_conn_term)) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [sim_conn_term]");
        goto end;
    }
    }

    up_path_chg_event = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "upPathChgEvent");
    if (up_path_chg_event) {
    if (!cJSON_IsNull(up_path_chg_event)) {
    up_path_chg_event_local_nonprim = OpenAPI_up_path_chg_event_parseFromJSON(up_path_chg_event);
    if (!up_path_chg_event_local_nonprim) {
        ogs_error("OpenAPI_up_path_chg_event_parseFromJSON failed [up_path_chg_event]");
        goto end;
    }
    }
    }

    steer_fun = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "steerFun");
    if (steer_fun) {
    if (!cJSON_IsString(steer_fun)) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [steer_fun]");
        goto end;
    }
    steer_funVariable = OpenAPI_steering_functionality_FromString(steer_fun->valuestring);
    }

    steer_mode_dl = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "steerModeDl");
    if (steer_mode_dl) {
    steer_mode_dl_local_nonprim = OpenAPI_steering_mode_parseFromJSON(steer_mode_dl);
    if (!steer_mode_dl_local_nonprim) {
        ogs_error("OpenAPI_steering_mode_parseFromJSON failed [steer_mode_dl]");
        goto end;
    }
    }

    steer_mode_ul = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "steerModeUl");
    if (steer_mode_ul) {
    steer_mode_ul_local_nonprim = OpenAPI_steering_mode_parseFromJSON(steer_mode_ul);
    if (!steer_mode_ul_local_nonprim) {
        ogs_error("OpenAPI_steering_mode_parseFromJSON failed [steer_mode_ul]");
        goto end;
    }
    }

    mul_acc_ctrl = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "mulAccCtrl");
    if (mul_acc_ctrl) {
    if (!cJSON_IsString(mul_acc_ctrl)) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [mul_acc_ctrl]");
        goto end;
    }
    mul_acc_ctrlVariable = OpenAPI_npcf_multicast_access_control_FromString(mul_acc_ctrl->valuestring);
    }

    traffic_control_data_local_var = OpenAPI_traffic_control_data_create (
        ogs_strdup(tc_id->valuestring),
        flow_status ? flow_statusVariable : 0,
        redirect_info ? redirect_info_local_nonprim : NULL,
        add_redirect_info ? add_redirect_infoList : NULL,
        mute_notif ? true : false,
        mute_notif ? mute_notif->valueint : 0,
        traffic_steering_pol_id_dl && cJSON_IsNull(traffic_steering_pol_id_dl) ? true : false,
        traffic_steering_pol_id_dl && !cJSON_IsNull(traffic_steering_pol_id_dl) ? ogs_strdup(traffic_steering_pol_id_dl->valuestring) : NULL,
        traffic_steering_pol_id_ul && cJSON_IsNull(traffic_steering_pol_id_ul) ? true : false,
        traffic_steering_pol_id_ul && !cJSON_IsNull(traffic_steering_pol_id_ul) ? ogs_strdup(traffic_steering_pol_id_ul->valuestring) : NULL,
        route_to_locs && cJSON_IsNull(route_to_locs) ? true : false,
        route_to_locs ? route_to_locsList : NULL,
        max_allowed_up_lat && cJSON_IsNull(max_allowed_up_lat) ? true : false,
        max_allowed_up_lat ? true : false,
        max_allowed_up_lat ? max_allowed_up_lat->valuedouble : 0,
        eas_ip_replace_infos && cJSON_IsNull(eas_ip_replace_infos) ? true : false,
        eas_ip_replace_infos ? eas_ip_replace_infosList : NULL,
        traff_corre_ind ? true : false,
        traff_corre_ind ? traff_corre_ind->valueint : 0,
        sim_conn_ind ? true : false,
        sim_conn_ind ? sim_conn_ind->valueint : 0,
        sim_conn_term ? true : false,
        sim_conn_term ? sim_conn_term->valuedouble : 0,
        up_path_chg_event && cJSON_IsNull(up_path_chg_event) ? true : false,
        up_path_chg_event ? up_path_chg_event_local_nonprim : NULL,
        steer_fun ? steer_funVariable : 0,
        steer_mode_dl ? steer_mode_dl_local_nonprim : NULL,
        steer_mode_ul ? steer_mode_ul_local_nonprim : NULL,
        mul_acc_ctrl ? mul_acc_ctrlVariable : 0
    );

    return traffic_control_data_local_var;
end:
    if (redirect_info_local_nonprim) {
        OpenAPI_redirect_information_free(redirect_info_local_nonprim);
        redirect_info_local_nonprim = NULL;
    }
    if (add_redirect_infoList) {
        OpenAPI_list_for_each(add_redirect_infoList, node) {
            OpenAPI_redirect_information_free(node->data);
        }
        OpenAPI_list_free(add_redirect_infoList);
        add_redirect_infoList = NULL;
    }
    if (route_to_locsList) {
        OpenAPI_list_for_each(route_to_locsList, node) {
            OpenAPI_route_to_location_free(node->data);
        }
        OpenAPI_list_free(route_to_locsList);
        route_to_locsList = NULL;
    }
    if (eas_ip_replace_infosList) {
        OpenAPI_list_for_each(eas_ip_replace_infosList, node) {
            OpenAPI_eas_ip_replacement_info_free(node->data);
        }
        OpenAPI_list_free(eas_ip_replace_infosList);
        eas_ip_replace_infosList = NULL;
    }
    if (up_path_chg_event_local_nonprim) {
        OpenAPI_up_path_chg_event_free(up_path_chg_event_local_nonprim);
        up_path_chg_event_local_nonprim = NULL;
    }
    if (steer_mode_dl_local_nonprim) {
        OpenAPI_steering_mode_free(steer_mode_dl_local_nonprim);
        steer_mode_dl_local_nonprim = NULL;
    }
    if (steer_mode_ul_local_nonprim) {
        OpenAPI_steering_mode_free(steer_mode_ul_local_nonprim);
        steer_mode_ul_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_traffic_control_data_t *OpenAPI_traffic_control_data_copy(OpenAPI_traffic_control_data_t *dst, OpenAPI_traffic_control_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_control_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed");
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

    OpenAPI_traffic_control_data_free(dst);
    dst = OpenAPI_traffic_control_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

