
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_control_data.h"

OpenAPI_traffic_control_data_t *OpenAPI_traffic_control_data_create(
    char *tc_id,
    OpenAPI_flow_status_t *flow_status,
    OpenAPI_redirect_information_t *redirect_info,
    OpenAPI_list_t *add_redirect_info,
    int mute_notif,
    char *traffic_steering_pol_id_dl,
    char *traffic_steering_pol_id_ul,
    OpenAPI_list_t *route_to_locs,
    int traff_corre_ind,
    OpenAPI_up_path_chg_event_t *up_path_chg_event,
    OpenAPI_steering_functionality_t *steer_fun,
    OpenAPI_steering_mode_t *steer_mode_dl,
    OpenAPI_steering_mode_t *steer_mode_ul,
    OpenAPI_multicast_access_control_t *mul_acc_ctrl
    )
{
    OpenAPI_traffic_control_data_t *traffic_control_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_traffic_control_data_t));
    if (!traffic_control_data_local_var) {
        return NULL;
    }
    traffic_control_data_local_var->tc_id = tc_id;
    traffic_control_data_local_var->flow_status = flow_status;
    traffic_control_data_local_var->redirect_info = redirect_info;
    traffic_control_data_local_var->add_redirect_info = add_redirect_info;
    traffic_control_data_local_var->mute_notif = mute_notif;
    traffic_control_data_local_var->traffic_steering_pol_id_dl = traffic_steering_pol_id_dl;
    traffic_control_data_local_var->traffic_steering_pol_id_ul = traffic_steering_pol_id_ul;
    traffic_control_data_local_var->route_to_locs = route_to_locs;
    traffic_control_data_local_var->traff_corre_ind = traff_corre_ind;
    traffic_control_data_local_var->up_path_chg_event = up_path_chg_event;
    traffic_control_data_local_var->steer_fun = steer_fun;
    traffic_control_data_local_var->steer_mode_dl = steer_mode_dl;
    traffic_control_data_local_var->steer_mode_ul = steer_mode_ul;
    traffic_control_data_local_var->mul_acc_ctrl = mul_acc_ctrl;

    return traffic_control_data_local_var;
}

void OpenAPI_traffic_control_data_free(OpenAPI_traffic_control_data_t *traffic_control_data)
{
    if (NULL == traffic_control_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(traffic_control_data->tc_id);
    OpenAPI_flow_status_free(traffic_control_data->flow_status);
    OpenAPI_redirect_information_free(traffic_control_data->redirect_info);
    OpenAPI_list_for_each(traffic_control_data->add_redirect_info, node) {
        OpenAPI_redirect_information_free(node->data);
    }
    OpenAPI_list_free(traffic_control_data->add_redirect_info);
    ogs_free(traffic_control_data->traffic_steering_pol_id_dl);
    ogs_free(traffic_control_data->traffic_steering_pol_id_ul);
    OpenAPI_list_for_each(traffic_control_data->route_to_locs, node) {
        OpenAPI_route_to_location_free(node->data);
    }
    OpenAPI_list_free(traffic_control_data->route_to_locs);
    OpenAPI_up_path_chg_event_free(traffic_control_data->up_path_chg_event);
    OpenAPI_steering_functionality_free(traffic_control_data->steer_fun);
    OpenAPI_steering_mode_free(traffic_control_data->steer_mode_dl);
    OpenAPI_steering_mode_free(traffic_control_data->steer_mode_ul);
    OpenAPI_multicast_access_control_free(traffic_control_data->mul_acc_ctrl);
    ogs_free(traffic_control_data);
}

cJSON *OpenAPI_traffic_control_data_convertToJSON(OpenAPI_traffic_control_data_t *traffic_control_data)
{
    cJSON *item = NULL;

    if (traffic_control_data == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [TrafficControlData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!traffic_control_data->tc_id) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [tc_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "tcId", traffic_control_data->tc_id) == NULL) {
        ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [tc_id]");
        goto end;
    }

    if (traffic_control_data->flow_status) {
        cJSON *flow_status_local_JSON = OpenAPI_flow_status_convertToJSON(traffic_control_data->flow_status);
        if (flow_status_local_JSON == NULL) {
            ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [flow_status]");
            goto end;
        }
        cJSON_AddItemToObject(item, "flowStatus", flow_status_local_JSON);
        if (item->child == NULL) {
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

        OpenAPI_lnode_t *add_redirect_info_node;
        if (traffic_control_data->add_redirect_info) {
            OpenAPI_list_for_each(traffic_control_data->add_redirect_info, add_redirect_info_node) {
                cJSON *itemLocal = OpenAPI_redirect_information_convertToJSON(add_redirect_info_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [add_redirect_info]");
                    goto end;
                }
                cJSON_AddItemToArray(add_redirect_infoList, itemLocal);
            }
        }
    }

    if (traffic_control_data->mute_notif) {
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
    }

    if (traffic_control_data->traffic_steering_pol_id_ul) {
        if (cJSON_AddStringToObject(item, "trafficSteeringPolIdUl", traffic_control_data->traffic_steering_pol_id_ul) == NULL) {
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

        OpenAPI_lnode_t *route_to_locs_node;
        if (traffic_control_data->route_to_locs) {
            OpenAPI_list_for_each(traffic_control_data->route_to_locs, route_to_locs_node) {
                cJSON *itemLocal = OpenAPI_route_to_location_convertToJSON(route_to_locs_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [route_to_locs]");
                    goto end;
                }
                cJSON_AddItemToArray(route_to_locsList, itemLocal);
            }
        }
    }

    if (traffic_control_data->traff_corre_ind) {
        if (cJSON_AddBoolToObject(item, "traffCorreInd", traffic_control_data->traff_corre_ind) == NULL) {
            ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [traff_corre_ind]");
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
    }

    if (traffic_control_data->steer_fun) {
        cJSON *steer_fun_local_JSON = OpenAPI_steering_functionality_convertToJSON(traffic_control_data->steer_fun);
        if (steer_fun_local_JSON == NULL) {
            ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [steer_fun]");
            goto end;
        }
        cJSON_AddItemToObject(item, "steerFun", steer_fun_local_JSON);
        if (item->child == NULL) {
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

    if (traffic_control_data->mul_acc_ctrl) {
        cJSON *mul_acc_ctrl_local_JSON = OpenAPI_multicast_access_control_convertToJSON(traffic_control_data->mul_acc_ctrl);
        if (mul_acc_ctrl_local_JSON == NULL) {
            ogs_error("OpenAPI_traffic_control_data_convertToJSON() failed [mul_acc_ctrl]");
            goto end;
        }
        cJSON_AddItemToObject(item, "mulAccCtrl", mul_acc_ctrl_local_JSON);
        if (item->child == NULL) {
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
    cJSON *tc_id = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "tcId");
    if (!tc_id) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [tc_id]");
        goto end;
    }


    if (!cJSON_IsString(tc_id)) {
        ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [tc_id]");
        goto end;
    }

    cJSON *flow_status = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "flowStatus");

    OpenAPI_flow_status_t *flow_status_local_nonprim = NULL;
    if (flow_status) {
        flow_status_local_nonprim = OpenAPI_flow_status_parseFromJSON(flow_status);
    }

    cJSON *redirect_info = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "redirectInfo");

    OpenAPI_redirect_information_t *redirect_info_local_nonprim = NULL;
    if (redirect_info) {
        redirect_info_local_nonprim = OpenAPI_redirect_information_parseFromJSON(redirect_info);
    }

    cJSON *add_redirect_info = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "addRedirectInfo");

    OpenAPI_list_t *add_redirect_infoList;
    if (add_redirect_info) {
        cJSON *add_redirect_info_local_nonprimitive;
        if (!cJSON_IsArray(add_redirect_info)) {
            ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [add_redirect_info]");
            goto end;
        }

        add_redirect_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_redirect_info_local_nonprimitive, add_redirect_info ) {
            if (!cJSON_IsObject(add_redirect_info_local_nonprimitive)) {
                ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [add_redirect_info]");
                goto end;
            }
            OpenAPI_redirect_information_t *add_redirect_infoItem = OpenAPI_redirect_information_parseFromJSON(add_redirect_info_local_nonprimitive);

            OpenAPI_list_add(add_redirect_infoList, add_redirect_infoItem);
        }
    }

    cJSON *mute_notif = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "muteNotif");

    if (mute_notif) {
        if (!cJSON_IsBool(mute_notif)) {
            ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [mute_notif]");
            goto end;
        }
    }

    cJSON *traffic_steering_pol_id_dl = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "trafficSteeringPolIdDl");

    if (traffic_steering_pol_id_dl) {
        if (!cJSON_IsString(traffic_steering_pol_id_dl)) {
            ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [traffic_steering_pol_id_dl]");
            goto end;
        }
    }

    cJSON *traffic_steering_pol_id_ul = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "trafficSteeringPolIdUl");

    if (traffic_steering_pol_id_ul) {
        if (!cJSON_IsString(traffic_steering_pol_id_ul)) {
            ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [traffic_steering_pol_id_ul]");
            goto end;
        }
    }

    cJSON *route_to_locs = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "routeToLocs");

    OpenAPI_list_t *route_to_locsList;
    if (route_to_locs) {
        cJSON *route_to_locs_local_nonprimitive;
        if (!cJSON_IsArray(route_to_locs)) {
            ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [route_to_locs]");
            goto end;
        }

        route_to_locsList = OpenAPI_list_create();

        cJSON_ArrayForEach(route_to_locs_local_nonprimitive, route_to_locs ) {
            if (!cJSON_IsObject(route_to_locs_local_nonprimitive)) {
                ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [route_to_locs]");
                goto end;
            }
            OpenAPI_route_to_location_t *route_to_locsItem = OpenAPI_route_to_location_parseFromJSON(route_to_locs_local_nonprimitive);

            OpenAPI_list_add(route_to_locsList, route_to_locsItem);
        }
    }

    cJSON *traff_corre_ind = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "traffCorreInd");

    if (traff_corre_ind) {
        if (!cJSON_IsBool(traff_corre_ind)) {
            ogs_error("OpenAPI_traffic_control_data_parseFromJSON() failed [traff_corre_ind]");
            goto end;
        }
    }

    cJSON *up_path_chg_event = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "upPathChgEvent");

    OpenAPI_up_path_chg_event_t *up_path_chg_event_local_nonprim = NULL;
    if (up_path_chg_event) {
        up_path_chg_event_local_nonprim = OpenAPI_up_path_chg_event_parseFromJSON(up_path_chg_event);
    }

    cJSON *steer_fun = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "steerFun");

    OpenAPI_steering_functionality_t *steer_fun_local_nonprim = NULL;
    if (steer_fun) {
        steer_fun_local_nonprim = OpenAPI_steering_functionality_parseFromJSON(steer_fun);
    }

    cJSON *steer_mode_dl = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "steerModeDl");

    OpenAPI_steering_mode_t *steer_mode_dl_local_nonprim = NULL;
    if (steer_mode_dl) {
        steer_mode_dl_local_nonprim = OpenAPI_steering_mode_parseFromJSON(steer_mode_dl);
    }

    cJSON *steer_mode_ul = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "steerModeUl");

    OpenAPI_steering_mode_t *steer_mode_ul_local_nonprim = NULL;
    if (steer_mode_ul) {
        steer_mode_ul_local_nonprim = OpenAPI_steering_mode_parseFromJSON(steer_mode_ul);
    }

    cJSON *mul_acc_ctrl = cJSON_GetObjectItemCaseSensitive(traffic_control_dataJSON, "mulAccCtrl");

    OpenAPI_multicast_access_control_t *mul_acc_ctrl_local_nonprim = NULL;
    if (mul_acc_ctrl) {
        mul_acc_ctrl_local_nonprim = OpenAPI_multicast_access_control_parseFromJSON(mul_acc_ctrl);
    }

    traffic_control_data_local_var = OpenAPI_traffic_control_data_create (
        ogs_strdup(tc_id->valuestring),
        flow_status ? flow_status_local_nonprim : NULL,
        redirect_info ? redirect_info_local_nonprim : NULL,
        add_redirect_info ? add_redirect_infoList : NULL,
        mute_notif ? mute_notif->valueint : 0,
        traffic_steering_pol_id_dl ? ogs_strdup(traffic_steering_pol_id_dl->valuestring) : NULL,
        traffic_steering_pol_id_ul ? ogs_strdup(traffic_steering_pol_id_ul->valuestring) : NULL,
        route_to_locs ? route_to_locsList : NULL,
        traff_corre_ind ? traff_corre_ind->valueint : 0,
        up_path_chg_event ? up_path_chg_event_local_nonprim : NULL,
        steer_fun ? steer_fun_local_nonprim : NULL,
        steer_mode_dl ? steer_mode_dl_local_nonprim : NULL,
        steer_mode_ul ? steer_mode_ul_local_nonprim : NULL,
        mul_acc_ctrl ? mul_acc_ctrl_local_nonprim : NULL
        );

    return traffic_control_data_local_var;
end:
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

