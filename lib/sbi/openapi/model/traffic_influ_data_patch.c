
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_influ_data_patch.h"

OpenAPI_traffic_influ_data_patch_t *OpenAPI_traffic_influ_data_patch_create(
    char *up_path_chg_notif_corre_id,
    bool is_app_relo_ind,
    int app_relo_ind,
    OpenAPI_list_t *eth_traffic_filters,
    OpenAPI_list_t *traffic_filters,
    OpenAPI_list_t *traffic_routes,
    bool is_traff_corre_ind,
    int traff_corre_ind,
    char *valid_start_time,
    char *valid_end_time,
    bool is_temp_validities_null,
    OpenAPI_list_t *temp_validities,
    OpenAPI_network_area_info_2_t *nw_area_info,
    char *up_path_chg_notif_uri,
    OpenAPI_list_t *headers,
    bool is_af_ack_ind,
    int af_ack_ind,
    bool is_addr_preser_ind,
    int addr_preser_ind,
    bool is_max_allowed_up_lat_null,
    bool is_max_allowed_up_lat,
    int max_allowed_up_lat,
    bool is_sim_conn_ind,
    int sim_conn_ind,
    bool is_sim_conn_term_null,
    bool is_sim_conn_term,
    int sim_conn_term
)
{
    OpenAPI_traffic_influ_data_patch_t *traffic_influ_data_patch_local_var = ogs_malloc(sizeof(OpenAPI_traffic_influ_data_patch_t));
    ogs_assert(traffic_influ_data_patch_local_var);

    traffic_influ_data_patch_local_var->up_path_chg_notif_corre_id = up_path_chg_notif_corre_id;
    traffic_influ_data_patch_local_var->is_app_relo_ind = is_app_relo_ind;
    traffic_influ_data_patch_local_var->app_relo_ind = app_relo_ind;
    traffic_influ_data_patch_local_var->eth_traffic_filters = eth_traffic_filters;
    traffic_influ_data_patch_local_var->traffic_filters = traffic_filters;
    traffic_influ_data_patch_local_var->traffic_routes = traffic_routes;
    traffic_influ_data_patch_local_var->is_traff_corre_ind = is_traff_corre_ind;
    traffic_influ_data_patch_local_var->traff_corre_ind = traff_corre_ind;
    traffic_influ_data_patch_local_var->valid_start_time = valid_start_time;
    traffic_influ_data_patch_local_var->valid_end_time = valid_end_time;
    traffic_influ_data_patch_local_var->is_temp_validities_null = is_temp_validities_null;
    traffic_influ_data_patch_local_var->temp_validities = temp_validities;
    traffic_influ_data_patch_local_var->nw_area_info = nw_area_info;
    traffic_influ_data_patch_local_var->up_path_chg_notif_uri = up_path_chg_notif_uri;
    traffic_influ_data_patch_local_var->headers = headers;
    traffic_influ_data_patch_local_var->is_af_ack_ind = is_af_ack_ind;
    traffic_influ_data_patch_local_var->af_ack_ind = af_ack_ind;
    traffic_influ_data_patch_local_var->is_addr_preser_ind = is_addr_preser_ind;
    traffic_influ_data_patch_local_var->addr_preser_ind = addr_preser_ind;
    traffic_influ_data_patch_local_var->is_max_allowed_up_lat_null = is_max_allowed_up_lat_null;
    traffic_influ_data_patch_local_var->is_max_allowed_up_lat = is_max_allowed_up_lat;
    traffic_influ_data_patch_local_var->max_allowed_up_lat = max_allowed_up_lat;
    traffic_influ_data_patch_local_var->is_sim_conn_ind = is_sim_conn_ind;
    traffic_influ_data_patch_local_var->sim_conn_ind = sim_conn_ind;
    traffic_influ_data_patch_local_var->is_sim_conn_term_null = is_sim_conn_term_null;
    traffic_influ_data_patch_local_var->is_sim_conn_term = is_sim_conn_term;
    traffic_influ_data_patch_local_var->sim_conn_term = sim_conn_term;

    return traffic_influ_data_patch_local_var;
}

void OpenAPI_traffic_influ_data_patch_free(OpenAPI_traffic_influ_data_patch_t *traffic_influ_data_patch)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_influ_data_patch) {
        return;
    }
    if (traffic_influ_data_patch->up_path_chg_notif_corre_id) {
        ogs_free(traffic_influ_data_patch->up_path_chg_notif_corre_id);
        traffic_influ_data_patch->up_path_chg_notif_corre_id = NULL;
    }
    if (traffic_influ_data_patch->eth_traffic_filters) {
        OpenAPI_list_for_each(traffic_influ_data_patch->eth_traffic_filters, node) {
            OpenAPI_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_data_patch->eth_traffic_filters);
        traffic_influ_data_patch->eth_traffic_filters = NULL;
    }
    if (traffic_influ_data_patch->traffic_filters) {
        OpenAPI_list_for_each(traffic_influ_data_patch->traffic_filters, node) {
            OpenAPI_flow_info_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_data_patch->traffic_filters);
        traffic_influ_data_patch->traffic_filters = NULL;
    }
    if (traffic_influ_data_patch->traffic_routes) {
        OpenAPI_list_for_each(traffic_influ_data_patch->traffic_routes, node) {
            OpenAPI_route_to_location_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_data_patch->traffic_routes);
        traffic_influ_data_patch->traffic_routes = NULL;
    }
    if (traffic_influ_data_patch->valid_start_time) {
        ogs_free(traffic_influ_data_patch->valid_start_time);
        traffic_influ_data_patch->valid_start_time = NULL;
    }
    if (traffic_influ_data_patch->valid_end_time) {
        ogs_free(traffic_influ_data_patch->valid_end_time);
        traffic_influ_data_patch->valid_end_time = NULL;
    }
    if (traffic_influ_data_patch->temp_validities) {
        OpenAPI_list_for_each(traffic_influ_data_patch->temp_validities, node) {
            OpenAPI_temporal_validity_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_data_patch->temp_validities);
        traffic_influ_data_patch->temp_validities = NULL;
    }
    if (traffic_influ_data_patch->nw_area_info) {
        OpenAPI_network_area_info_2_free(traffic_influ_data_patch->nw_area_info);
        traffic_influ_data_patch->nw_area_info = NULL;
    }
    if (traffic_influ_data_patch->up_path_chg_notif_uri) {
        ogs_free(traffic_influ_data_patch->up_path_chg_notif_uri);
        traffic_influ_data_patch->up_path_chg_notif_uri = NULL;
    }
    if (traffic_influ_data_patch->headers) {
        OpenAPI_list_for_each(traffic_influ_data_patch->headers, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_data_patch->headers);
        traffic_influ_data_patch->headers = NULL;
    }
    ogs_free(traffic_influ_data_patch);
}

cJSON *OpenAPI_traffic_influ_data_patch_convertToJSON(OpenAPI_traffic_influ_data_patch_t *traffic_influ_data_patch)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_influ_data_patch == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [TrafficInfluDataPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (traffic_influ_data_patch->up_path_chg_notif_corre_id) {
    if (cJSON_AddStringToObject(item, "upPathChgNotifCorreId", traffic_influ_data_patch->up_path_chg_notif_corre_id) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [up_path_chg_notif_corre_id]");
        goto end;
    }
    }

    if (traffic_influ_data_patch->is_app_relo_ind) {
    if (cJSON_AddBoolToObject(item, "appReloInd", traffic_influ_data_patch->app_relo_ind) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [app_relo_ind]");
        goto end;
    }
    }

    if (traffic_influ_data_patch->eth_traffic_filters) {
    cJSON *eth_traffic_filtersList = cJSON_AddArrayToObject(item, "ethTrafficFilters");
    if (eth_traffic_filtersList == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [eth_traffic_filters]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_data_patch->eth_traffic_filters, node) {
        cJSON *itemLocal = OpenAPI_eth_flow_description_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [eth_traffic_filters]");
            goto end;
        }
        cJSON_AddItemToArray(eth_traffic_filtersList, itemLocal);
    }
    }

    if (traffic_influ_data_patch->traffic_filters) {
    cJSON *traffic_filtersList = cJSON_AddArrayToObject(item, "trafficFilters");
    if (traffic_filtersList == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [traffic_filters]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_data_patch->traffic_filters, node) {
        cJSON *itemLocal = OpenAPI_flow_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [traffic_filters]");
            goto end;
        }
        cJSON_AddItemToArray(traffic_filtersList, itemLocal);
    }
    }

    if (traffic_influ_data_patch->traffic_routes) {
    cJSON *traffic_routesList = cJSON_AddArrayToObject(item, "trafficRoutes");
    if (traffic_routesList == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [traffic_routes]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_data_patch->traffic_routes, node) {
        cJSON *itemLocal = OpenAPI_route_to_location_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [traffic_routes]");
            goto end;
        }
        cJSON_AddItemToArray(traffic_routesList, itemLocal);
    }
    }

    if (traffic_influ_data_patch->is_traff_corre_ind) {
    if (cJSON_AddBoolToObject(item, "traffCorreInd", traffic_influ_data_patch->traff_corre_ind) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [traff_corre_ind]");
        goto end;
    }
    }

    if (traffic_influ_data_patch->valid_start_time) {
    if (cJSON_AddStringToObject(item, "validStartTime", traffic_influ_data_patch->valid_start_time) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [valid_start_time]");
        goto end;
    }
    }

    if (traffic_influ_data_patch->valid_end_time) {
    if (cJSON_AddStringToObject(item, "validEndTime", traffic_influ_data_patch->valid_end_time) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [valid_end_time]");
        goto end;
    }
    }

    if (traffic_influ_data_patch->temp_validities) {
    cJSON *temp_validitiesList = cJSON_AddArrayToObject(item, "tempValidities");
    if (temp_validitiesList == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [temp_validities]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_data_patch->temp_validities, node) {
        cJSON *itemLocal = OpenAPI_temporal_validity_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [temp_validities]");
            goto end;
        }
        cJSON_AddItemToArray(temp_validitiesList, itemLocal);
    }
    } else if (traffic_influ_data_patch->is_temp_validities_null) {
        if (cJSON_AddNullToObject(item, "tempValidities") == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [temp_validities]");
            goto end;
        }
    }

    if (traffic_influ_data_patch->nw_area_info) {
    cJSON *nw_area_info_local_JSON = OpenAPI_network_area_info_2_convertToJSON(traffic_influ_data_patch->nw_area_info);
    if (nw_area_info_local_JSON == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [nw_area_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nwAreaInfo", nw_area_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [nw_area_info]");
        goto end;
    }
    }

    if (traffic_influ_data_patch->up_path_chg_notif_uri) {
    if (cJSON_AddStringToObject(item, "upPathChgNotifUri", traffic_influ_data_patch->up_path_chg_notif_uri) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [up_path_chg_notif_uri]");
        goto end;
    }
    }

    if (traffic_influ_data_patch->headers) {
    cJSON *headersList = cJSON_AddArrayToObject(item, "headers");
    if (headersList == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [headers]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_data_patch->headers, node) {
        if (cJSON_AddStringToObject(headersList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [headers]");
            goto end;
        }
    }
    }

    if (traffic_influ_data_patch->is_af_ack_ind) {
    if (cJSON_AddBoolToObject(item, "afAckInd", traffic_influ_data_patch->af_ack_ind) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [af_ack_ind]");
        goto end;
    }
    }

    if (traffic_influ_data_patch->is_addr_preser_ind) {
    if (cJSON_AddBoolToObject(item, "addrPreserInd", traffic_influ_data_patch->addr_preser_ind) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [addr_preser_ind]");
        goto end;
    }
    }

    if (traffic_influ_data_patch->is_max_allowed_up_lat) {
    if (cJSON_AddNumberToObject(item, "maxAllowedUpLat", traffic_influ_data_patch->max_allowed_up_lat) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [max_allowed_up_lat]");
        goto end;
    }
    } else if (traffic_influ_data_patch->is_max_allowed_up_lat_null) {
        if (cJSON_AddNullToObject(item, "maxAllowedUpLat") == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [max_allowed_up_lat]");
            goto end;
        }
    }

    if (traffic_influ_data_patch->is_sim_conn_ind) {
    if (cJSON_AddBoolToObject(item, "simConnInd", traffic_influ_data_patch->sim_conn_ind) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [sim_conn_ind]");
        goto end;
    }
    }

    if (traffic_influ_data_patch->is_sim_conn_term) {
    if (cJSON_AddNumberToObject(item, "simConnTerm", traffic_influ_data_patch->sim_conn_term) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [sim_conn_term]");
        goto end;
    }
    } else if (traffic_influ_data_patch->is_sim_conn_term_null) {
        if (cJSON_AddNullToObject(item, "simConnTerm") == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed [sim_conn_term]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_traffic_influ_data_patch_t *OpenAPI_traffic_influ_data_patch_parseFromJSON(cJSON *traffic_influ_data_patchJSON)
{
    OpenAPI_traffic_influ_data_patch_t *traffic_influ_data_patch_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *up_path_chg_notif_corre_id = NULL;
    cJSON *app_relo_ind = NULL;
    cJSON *eth_traffic_filters = NULL;
    OpenAPI_list_t *eth_traffic_filtersList = NULL;
    cJSON *traffic_filters = NULL;
    OpenAPI_list_t *traffic_filtersList = NULL;
    cJSON *traffic_routes = NULL;
    OpenAPI_list_t *traffic_routesList = NULL;
    cJSON *traff_corre_ind = NULL;
    cJSON *valid_start_time = NULL;
    cJSON *valid_end_time = NULL;
    cJSON *temp_validities = NULL;
    OpenAPI_list_t *temp_validitiesList = NULL;
    cJSON *nw_area_info = NULL;
    OpenAPI_network_area_info_2_t *nw_area_info_local_nonprim = NULL;
    cJSON *up_path_chg_notif_uri = NULL;
    cJSON *headers = NULL;
    OpenAPI_list_t *headersList = NULL;
    cJSON *af_ack_ind = NULL;
    cJSON *addr_preser_ind = NULL;
    cJSON *max_allowed_up_lat = NULL;
    cJSON *sim_conn_ind = NULL;
    cJSON *sim_conn_term = NULL;
    up_path_chg_notif_corre_id = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "upPathChgNotifCorreId");
    if (up_path_chg_notif_corre_id) {
    if (!cJSON_IsString(up_path_chg_notif_corre_id) && !cJSON_IsNull(up_path_chg_notif_corre_id)) {
        ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [up_path_chg_notif_corre_id]");
        goto end;
    }
    }

    app_relo_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "appReloInd");
    if (app_relo_ind) {
    if (!cJSON_IsBool(app_relo_ind)) {
        ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [app_relo_ind]");
        goto end;
    }
    }

    eth_traffic_filters = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "ethTrafficFilters");
    if (eth_traffic_filters) {
        cJSON *eth_traffic_filters_local = NULL;
        if (!cJSON_IsArray(eth_traffic_filters)) {
            ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [eth_traffic_filters]");
            goto end;
        }

        eth_traffic_filtersList = OpenAPI_list_create();

        cJSON_ArrayForEach(eth_traffic_filters_local, eth_traffic_filters) {
            if (!cJSON_IsObject(eth_traffic_filters_local)) {
                ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [eth_traffic_filters]");
                goto end;
            }
            OpenAPI_eth_flow_description_t *eth_traffic_filtersItem = OpenAPI_eth_flow_description_parseFromJSON(eth_traffic_filters_local);
            if (!eth_traffic_filtersItem) {
                ogs_error("No eth_traffic_filtersItem");
                goto end;
            }
            OpenAPI_list_add(eth_traffic_filtersList, eth_traffic_filtersItem);
        }
    }

    traffic_filters = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "trafficFilters");
    if (traffic_filters) {
        cJSON *traffic_filters_local = NULL;
        if (!cJSON_IsArray(traffic_filters)) {
            ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [traffic_filters]");
            goto end;
        }

        traffic_filtersList = OpenAPI_list_create();

        cJSON_ArrayForEach(traffic_filters_local, traffic_filters) {
            if (!cJSON_IsObject(traffic_filters_local)) {
                ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [traffic_filters]");
                goto end;
            }
            OpenAPI_flow_info_t *traffic_filtersItem = OpenAPI_flow_info_parseFromJSON(traffic_filters_local);
            if (!traffic_filtersItem) {
                ogs_error("No traffic_filtersItem");
                goto end;
            }
            OpenAPI_list_add(traffic_filtersList, traffic_filtersItem);
        }
    }

    traffic_routes = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "trafficRoutes");
    if (traffic_routes) {
        cJSON *traffic_routes_local = NULL;
        if (!cJSON_IsArray(traffic_routes)) {
            ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [traffic_routes]");
            goto end;
        }

        traffic_routesList = OpenAPI_list_create();

        cJSON_ArrayForEach(traffic_routes_local, traffic_routes) {
            if (!cJSON_IsObject(traffic_routes_local)) {
                ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [traffic_routes]");
                goto end;
            }
            OpenAPI_route_to_location_t *traffic_routesItem = OpenAPI_route_to_location_parseFromJSON(traffic_routes_local);
            if (!traffic_routesItem) {
                ogs_error("No traffic_routesItem");
                goto end;
            }
            OpenAPI_list_add(traffic_routesList, traffic_routesItem);
        }
    }

    traff_corre_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "traffCorreInd");
    if (traff_corre_ind) {
    if (!cJSON_IsBool(traff_corre_ind)) {
        ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [traff_corre_ind]");
        goto end;
    }
    }

    valid_start_time = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "validStartTime");
    if (valid_start_time) {
    if (!cJSON_IsString(valid_start_time) && !cJSON_IsNull(valid_start_time)) {
        ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [valid_start_time]");
        goto end;
    }
    }

    valid_end_time = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "validEndTime");
    if (valid_end_time) {
    if (!cJSON_IsString(valid_end_time) && !cJSON_IsNull(valid_end_time)) {
        ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [valid_end_time]");
        goto end;
    }
    }

    temp_validities = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "tempValidities");
    if (temp_validities) {
    if (!cJSON_IsNull(temp_validities)) {
        cJSON *temp_validities_local = NULL;
        if (!cJSON_IsArray(temp_validities)) {
            ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [temp_validities]");
            goto end;
        }

        temp_validitiesList = OpenAPI_list_create();

        cJSON_ArrayForEach(temp_validities_local, temp_validities) {
            if (!cJSON_IsObject(temp_validities_local)) {
                ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [temp_validities]");
                goto end;
            }
            OpenAPI_temporal_validity_t *temp_validitiesItem = OpenAPI_temporal_validity_parseFromJSON(temp_validities_local);
            if (!temp_validitiesItem) {
                ogs_error("No temp_validitiesItem");
                goto end;
            }
            OpenAPI_list_add(temp_validitiesList, temp_validitiesItem);
        }
    }
    }

    nw_area_info = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "nwAreaInfo");
    if (nw_area_info) {
    nw_area_info_local_nonprim = OpenAPI_network_area_info_2_parseFromJSON(nw_area_info);
    if (!nw_area_info_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_2_parseFromJSON failed [nw_area_info]");
        goto end;
    }
    }

    up_path_chg_notif_uri = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "upPathChgNotifUri");
    if (up_path_chg_notif_uri) {
    if (!cJSON_IsString(up_path_chg_notif_uri) && !cJSON_IsNull(up_path_chg_notif_uri)) {
        ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [up_path_chg_notif_uri]");
        goto end;
    }
    }

    headers = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "headers");
    if (headers) {
        cJSON *headers_local = NULL;
        if (!cJSON_IsArray(headers)) {
            ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [headers]");
            goto end;
        }

        headersList = OpenAPI_list_create();

        cJSON_ArrayForEach(headers_local, headers) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(headers_local)) {
                ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [headers]");
                goto end;
            }
            OpenAPI_list_add(headersList, ogs_strdup(headers_local->valuestring));
        }
    }

    af_ack_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "afAckInd");
    if (af_ack_ind) {
    if (!cJSON_IsBool(af_ack_ind)) {
        ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [af_ack_ind]");
        goto end;
    }
    }

    addr_preser_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "addrPreserInd");
    if (addr_preser_ind) {
    if (!cJSON_IsBool(addr_preser_ind)) {
        ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [addr_preser_ind]");
        goto end;
    }
    }

    max_allowed_up_lat = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "maxAllowedUpLat");
    if (max_allowed_up_lat) {
    if (!cJSON_IsNull(max_allowed_up_lat)) {
    if (!cJSON_IsNumber(max_allowed_up_lat)) {
        ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [max_allowed_up_lat]");
        goto end;
    }
    }
    }

    sim_conn_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "simConnInd");
    if (sim_conn_ind) {
    if (!cJSON_IsBool(sim_conn_ind)) {
        ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [sim_conn_ind]");
        goto end;
    }
    }

    sim_conn_term = cJSON_GetObjectItemCaseSensitive(traffic_influ_data_patchJSON, "simConnTerm");
    if (sim_conn_term) {
    if (!cJSON_IsNull(sim_conn_term)) {
    if (!cJSON_IsNumber(sim_conn_term)) {
        ogs_error("OpenAPI_traffic_influ_data_patch_parseFromJSON() failed [sim_conn_term]");
        goto end;
    }
    }
    }

    traffic_influ_data_patch_local_var = OpenAPI_traffic_influ_data_patch_create (
        up_path_chg_notif_corre_id && !cJSON_IsNull(up_path_chg_notif_corre_id) ? ogs_strdup(up_path_chg_notif_corre_id->valuestring) : NULL,
        app_relo_ind ? true : false,
        app_relo_ind ? app_relo_ind->valueint : 0,
        eth_traffic_filters ? eth_traffic_filtersList : NULL,
        traffic_filters ? traffic_filtersList : NULL,
        traffic_routes ? traffic_routesList : NULL,
        traff_corre_ind ? true : false,
        traff_corre_ind ? traff_corre_ind->valueint : 0,
        valid_start_time && !cJSON_IsNull(valid_start_time) ? ogs_strdup(valid_start_time->valuestring) : NULL,
        valid_end_time && !cJSON_IsNull(valid_end_time) ? ogs_strdup(valid_end_time->valuestring) : NULL,
        temp_validities && cJSON_IsNull(temp_validities) ? true : false,
        temp_validities ? temp_validitiesList : NULL,
        nw_area_info ? nw_area_info_local_nonprim : NULL,
        up_path_chg_notif_uri && !cJSON_IsNull(up_path_chg_notif_uri) ? ogs_strdup(up_path_chg_notif_uri->valuestring) : NULL,
        headers ? headersList : NULL,
        af_ack_ind ? true : false,
        af_ack_ind ? af_ack_ind->valueint : 0,
        addr_preser_ind ? true : false,
        addr_preser_ind ? addr_preser_ind->valueint : 0,
        max_allowed_up_lat && cJSON_IsNull(max_allowed_up_lat) ? true : false,
        max_allowed_up_lat ? true : false,
        max_allowed_up_lat ? max_allowed_up_lat->valuedouble : 0,
        sim_conn_ind ? true : false,
        sim_conn_ind ? sim_conn_ind->valueint : 0,
        sim_conn_term && cJSON_IsNull(sim_conn_term) ? true : false,
        sim_conn_term ? true : false,
        sim_conn_term ? sim_conn_term->valuedouble : 0
    );

    return traffic_influ_data_patch_local_var;
end:
    if (eth_traffic_filtersList) {
        OpenAPI_list_for_each(eth_traffic_filtersList, node) {
            OpenAPI_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(eth_traffic_filtersList);
        eth_traffic_filtersList = NULL;
    }
    if (traffic_filtersList) {
        OpenAPI_list_for_each(traffic_filtersList, node) {
            OpenAPI_flow_info_free(node->data);
        }
        OpenAPI_list_free(traffic_filtersList);
        traffic_filtersList = NULL;
    }
    if (traffic_routesList) {
        OpenAPI_list_for_each(traffic_routesList, node) {
            OpenAPI_route_to_location_free(node->data);
        }
        OpenAPI_list_free(traffic_routesList);
        traffic_routesList = NULL;
    }
    if (temp_validitiesList) {
        OpenAPI_list_for_each(temp_validitiesList, node) {
            OpenAPI_temporal_validity_free(node->data);
        }
        OpenAPI_list_free(temp_validitiesList);
        temp_validitiesList = NULL;
    }
    if (nw_area_info_local_nonprim) {
        OpenAPI_network_area_info_2_free(nw_area_info_local_nonprim);
        nw_area_info_local_nonprim = NULL;
    }
    if (headersList) {
        OpenAPI_list_for_each(headersList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(headersList);
        headersList = NULL;
    }
    return NULL;
}

OpenAPI_traffic_influ_data_patch_t *OpenAPI_traffic_influ_data_patch_copy(OpenAPI_traffic_influ_data_patch_t *dst, OpenAPI_traffic_influ_data_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_influ_data_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_influ_data_patch_convertToJSON() failed");
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

    OpenAPI_traffic_influ_data_patch_free(dst);
    dst = OpenAPI_traffic_influ_data_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

