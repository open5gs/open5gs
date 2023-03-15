
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_influ_data.h"

OpenAPI_traffic_influ_data_t *OpenAPI_traffic_influ_data_create(
    char *up_path_chg_notif_corre_id,
    bool is_app_relo_ind,
    int app_relo_ind,
    char *af_app_id,
    char *dnn,
    OpenAPI_list_t *eth_traffic_filters,
    OpenAPI_snssai_t *snssai,
    char *inter_group_id,
    char *supi,
    OpenAPI_list_t *traffic_filters,
    OpenAPI_list_t *traffic_routes,
    bool is_traff_corre_ind,
    int traff_corre_ind,
    char *valid_start_time,
    char *valid_end_time,
    OpenAPI_list_t *temp_validities,
    OpenAPI_network_area_info_2_t *nw_area_info,
    char *up_path_chg_notif_uri,
    OpenAPI_list_t *headers,
    OpenAPI_list_t *subscribed_events,
    OpenAPI_dnai_change_type_e dnai_chg_type,
    bool is_af_ack_ind,
    int af_ack_ind,
    bool is_addr_preser_ind,
    int addr_preser_ind,
    bool is_max_allowed_up_lat,
    int max_allowed_up_lat,
    bool is_sim_conn_ind,
    int sim_conn_ind,
    bool is_sim_conn_term,
    int sim_conn_term,
    char *supported_features,
    char *res_uri,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_traffic_influ_data_t *traffic_influ_data_local_var = ogs_malloc(sizeof(OpenAPI_traffic_influ_data_t));
    ogs_assert(traffic_influ_data_local_var);

    traffic_influ_data_local_var->up_path_chg_notif_corre_id = up_path_chg_notif_corre_id;
    traffic_influ_data_local_var->is_app_relo_ind = is_app_relo_ind;
    traffic_influ_data_local_var->app_relo_ind = app_relo_ind;
    traffic_influ_data_local_var->af_app_id = af_app_id;
    traffic_influ_data_local_var->dnn = dnn;
    traffic_influ_data_local_var->eth_traffic_filters = eth_traffic_filters;
    traffic_influ_data_local_var->snssai = snssai;
    traffic_influ_data_local_var->inter_group_id = inter_group_id;
    traffic_influ_data_local_var->supi = supi;
    traffic_influ_data_local_var->traffic_filters = traffic_filters;
    traffic_influ_data_local_var->traffic_routes = traffic_routes;
    traffic_influ_data_local_var->is_traff_corre_ind = is_traff_corre_ind;
    traffic_influ_data_local_var->traff_corre_ind = traff_corre_ind;
    traffic_influ_data_local_var->valid_start_time = valid_start_time;
    traffic_influ_data_local_var->valid_end_time = valid_end_time;
    traffic_influ_data_local_var->temp_validities = temp_validities;
    traffic_influ_data_local_var->nw_area_info = nw_area_info;
    traffic_influ_data_local_var->up_path_chg_notif_uri = up_path_chg_notif_uri;
    traffic_influ_data_local_var->headers = headers;
    traffic_influ_data_local_var->subscribed_events = subscribed_events;
    traffic_influ_data_local_var->dnai_chg_type = dnai_chg_type;
    traffic_influ_data_local_var->is_af_ack_ind = is_af_ack_ind;
    traffic_influ_data_local_var->af_ack_ind = af_ack_ind;
    traffic_influ_data_local_var->is_addr_preser_ind = is_addr_preser_ind;
    traffic_influ_data_local_var->addr_preser_ind = addr_preser_ind;
    traffic_influ_data_local_var->is_max_allowed_up_lat = is_max_allowed_up_lat;
    traffic_influ_data_local_var->max_allowed_up_lat = max_allowed_up_lat;
    traffic_influ_data_local_var->is_sim_conn_ind = is_sim_conn_ind;
    traffic_influ_data_local_var->sim_conn_ind = sim_conn_ind;
    traffic_influ_data_local_var->is_sim_conn_term = is_sim_conn_term;
    traffic_influ_data_local_var->sim_conn_term = sim_conn_term;
    traffic_influ_data_local_var->supported_features = supported_features;
    traffic_influ_data_local_var->res_uri = res_uri;
    traffic_influ_data_local_var->reset_ids = reset_ids;

    return traffic_influ_data_local_var;
}

void OpenAPI_traffic_influ_data_free(OpenAPI_traffic_influ_data_t *traffic_influ_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_influ_data) {
        return;
    }
    if (traffic_influ_data->up_path_chg_notif_corre_id) {
        ogs_free(traffic_influ_data->up_path_chg_notif_corre_id);
        traffic_influ_data->up_path_chg_notif_corre_id = NULL;
    }
    if (traffic_influ_data->af_app_id) {
        ogs_free(traffic_influ_data->af_app_id);
        traffic_influ_data->af_app_id = NULL;
    }
    if (traffic_influ_data->dnn) {
        ogs_free(traffic_influ_data->dnn);
        traffic_influ_data->dnn = NULL;
    }
    if (traffic_influ_data->eth_traffic_filters) {
        OpenAPI_list_for_each(traffic_influ_data->eth_traffic_filters, node) {
            OpenAPI_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_data->eth_traffic_filters);
        traffic_influ_data->eth_traffic_filters = NULL;
    }
    if (traffic_influ_data->snssai) {
        OpenAPI_snssai_free(traffic_influ_data->snssai);
        traffic_influ_data->snssai = NULL;
    }
    if (traffic_influ_data->inter_group_id) {
        ogs_free(traffic_influ_data->inter_group_id);
        traffic_influ_data->inter_group_id = NULL;
    }
    if (traffic_influ_data->supi) {
        ogs_free(traffic_influ_data->supi);
        traffic_influ_data->supi = NULL;
    }
    if (traffic_influ_data->traffic_filters) {
        OpenAPI_list_for_each(traffic_influ_data->traffic_filters, node) {
            OpenAPI_flow_info_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_data->traffic_filters);
        traffic_influ_data->traffic_filters = NULL;
    }
    if (traffic_influ_data->traffic_routes) {
        OpenAPI_list_for_each(traffic_influ_data->traffic_routes, node) {
            OpenAPI_route_to_location_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_data->traffic_routes);
        traffic_influ_data->traffic_routes = NULL;
    }
    if (traffic_influ_data->valid_start_time) {
        ogs_free(traffic_influ_data->valid_start_time);
        traffic_influ_data->valid_start_time = NULL;
    }
    if (traffic_influ_data->valid_end_time) {
        ogs_free(traffic_influ_data->valid_end_time);
        traffic_influ_data->valid_end_time = NULL;
    }
    if (traffic_influ_data->temp_validities) {
        OpenAPI_list_for_each(traffic_influ_data->temp_validities, node) {
            OpenAPI_temporal_validity_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_data->temp_validities);
        traffic_influ_data->temp_validities = NULL;
    }
    if (traffic_influ_data->nw_area_info) {
        OpenAPI_network_area_info_2_free(traffic_influ_data->nw_area_info);
        traffic_influ_data->nw_area_info = NULL;
    }
    if (traffic_influ_data->up_path_chg_notif_uri) {
        ogs_free(traffic_influ_data->up_path_chg_notif_uri);
        traffic_influ_data->up_path_chg_notif_uri = NULL;
    }
    if (traffic_influ_data->headers) {
        OpenAPI_list_for_each(traffic_influ_data->headers, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_data->headers);
        traffic_influ_data->headers = NULL;
    }
    if (traffic_influ_data->subscribed_events) {
        OpenAPI_list_for_each(traffic_influ_data->subscribed_events, node) {
            OpenAPI_subscribed_event_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_data->subscribed_events);
        traffic_influ_data->subscribed_events = NULL;
    }
    if (traffic_influ_data->supported_features) {
        ogs_free(traffic_influ_data->supported_features);
        traffic_influ_data->supported_features = NULL;
    }
    if (traffic_influ_data->res_uri) {
        ogs_free(traffic_influ_data->res_uri);
        traffic_influ_data->res_uri = NULL;
    }
    if (traffic_influ_data->reset_ids) {
        OpenAPI_list_for_each(traffic_influ_data->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_data->reset_ids);
        traffic_influ_data->reset_ids = NULL;
    }
    ogs_free(traffic_influ_data);
}

cJSON *OpenAPI_traffic_influ_data_convertToJSON(OpenAPI_traffic_influ_data_t *traffic_influ_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_influ_data == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [TrafficInfluData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (traffic_influ_data->up_path_chg_notif_corre_id) {
    if (cJSON_AddStringToObject(item, "upPathChgNotifCorreId", traffic_influ_data->up_path_chg_notif_corre_id) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [up_path_chg_notif_corre_id]");
        goto end;
    }
    }

    if (traffic_influ_data->is_app_relo_ind) {
    if (cJSON_AddBoolToObject(item, "appReloInd", traffic_influ_data->app_relo_ind) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [app_relo_ind]");
        goto end;
    }
    }

    if (traffic_influ_data->af_app_id) {
    if (cJSON_AddStringToObject(item, "afAppId", traffic_influ_data->af_app_id) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [af_app_id]");
        goto end;
    }
    }

    if (traffic_influ_data->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", traffic_influ_data->dnn) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (traffic_influ_data->eth_traffic_filters) {
    cJSON *eth_traffic_filtersList = cJSON_AddArrayToObject(item, "ethTrafficFilters");
    if (eth_traffic_filtersList == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [eth_traffic_filters]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_data->eth_traffic_filters, node) {
        cJSON *itemLocal = OpenAPI_eth_flow_description_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [eth_traffic_filters]");
            goto end;
        }
        cJSON_AddItemToArray(eth_traffic_filtersList, itemLocal);
    }
    }

    if (traffic_influ_data->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(traffic_influ_data->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (traffic_influ_data->inter_group_id) {
    if (cJSON_AddStringToObject(item, "interGroupId", traffic_influ_data->inter_group_id) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [inter_group_id]");
        goto end;
    }
    }

    if (traffic_influ_data->supi) {
    if (cJSON_AddStringToObject(item, "supi", traffic_influ_data->supi) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (traffic_influ_data->traffic_filters) {
    cJSON *traffic_filtersList = cJSON_AddArrayToObject(item, "trafficFilters");
    if (traffic_filtersList == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [traffic_filters]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_data->traffic_filters, node) {
        cJSON *itemLocal = OpenAPI_flow_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [traffic_filters]");
            goto end;
        }
        cJSON_AddItemToArray(traffic_filtersList, itemLocal);
    }
    }

    if (traffic_influ_data->traffic_routes) {
    cJSON *traffic_routesList = cJSON_AddArrayToObject(item, "trafficRoutes");
    if (traffic_routesList == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [traffic_routes]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_data->traffic_routes, node) {
        cJSON *itemLocal = OpenAPI_route_to_location_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [traffic_routes]");
            goto end;
        }
        cJSON_AddItemToArray(traffic_routesList, itemLocal);
    }
    }

    if (traffic_influ_data->is_traff_corre_ind) {
    if (cJSON_AddBoolToObject(item, "traffCorreInd", traffic_influ_data->traff_corre_ind) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [traff_corre_ind]");
        goto end;
    }
    }

    if (traffic_influ_data->valid_start_time) {
    if (cJSON_AddStringToObject(item, "validStartTime", traffic_influ_data->valid_start_time) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [valid_start_time]");
        goto end;
    }
    }

    if (traffic_influ_data->valid_end_time) {
    if (cJSON_AddStringToObject(item, "validEndTime", traffic_influ_data->valid_end_time) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [valid_end_time]");
        goto end;
    }
    }

    if (traffic_influ_data->temp_validities) {
    cJSON *temp_validitiesList = cJSON_AddArrayToObject(item, "tempValidities");
    if (temp_validitiesList == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [temp_validities]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_data->temp_validities, node) {
        cJSON *itemLocal = OpenAPI_temporal_validity_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [temp_validities]");
            goto end;
        }
        cJSON_AddItemToArray(temp_validitiesList, itemLocal);
    }
    }

    if (traffic_influ_data->nw_area_info) {
    cJSON *nw_area_info_local_JSON = OpenAPI_network_area_info_2_convertToJSON(traffic_influ_data->nw_area_info);
    if (nw_area_info_local_JSON == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [nw_area_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nwAreaInfo", nw_area_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [nw_area_info]");
        goto end;
    }
    }

    if (traffic_influ_data->up_path_chg_notif_uri) {
    if (cJSON_AddStringToObject(item, "upPathChgNotifUri", traffic_influ_data->up_path_chg_notif_uri) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [up_path_chg_notif_uri]");
        goto end;
    }
    }

    if (traffic_influ_data->headers) {
    cJSON *headersList = cJSON_AddArrayToObject(item, "headers");
    if (headersList == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [headers]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_data->headers, node) {
        if (cJSON_AddStringToObject(headersList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [headers]");
            goto end;
        }
    }
    }

    if (traffic_influ_data->subscribed_events) {
    cJSON *subscribed_eventsList = cJSON_AddArrayToObject(item, "subscribedEvents");
    if (subscribed_eventsList == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [subscribed_events]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_data->subscribed_events, node) {
        cJSON *itemLocal = OpenAPI_subscribed_event_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [subscribed_events]");
            goto end;
        }
        cJSON_AddItemToArray(subscribed_eventsList, itemLocal);
    }
    }

    if (traffic_influ_data->dnai_chg_type != OpenAPI_dnai_change_type_NULL) {
    if (cJSON_AddStringToObject(item, "dnaiChgType", OpenAPI_dnai_change_type_ToString(traffic_influ_data->dnai_chg_type)) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [dnai_chg_type]");
        goto end;
    }
    }

    if (traffic_influ_data->is_af_ack_ind) {
    if (cJSON_AddBoolToObject(item, "afAckInd", traffic_influ_data->af_ack_ind) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [af_ack_ind]");
        goto end;
    }
    }

    if (traffic_influ_data->is_addr_preser_ind) {
    if (cJSON_AddBoolToObject(item, "addrPreserInd", traffic_influ_data->addr_preser_ind) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [addr_preser_ind]");
        goto end;
    }
    }

    if (traffic_influ_data->is_max_allowed_up_lat) {
    if (cJSON_AddNumberToObject(item, "maxAllowedUpLat", traffic_influ_data->max_allowed_up_lat) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [max_allowed_up_lat]");
        goto end;
    }
    }

    if (traffic_influ_data->is_sim_conn_ind) {
    if (cJSON_AddBoolToObject(item, "simConnInd", traffic_influ_data->sim_conn_ind) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [sim_conn_ind]");
        goto end;
    }
    }

    if (traffic_influ_data->is_sim_conn_term) {
    if (cJSON_AddNumberToObject(item, "simConnTerm", traffic_influ_data->sim_conn_term) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [sim_conn_term]");
        goto end;
    }
    }

    if (traffic_influ_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", traffic_influ_data->supported_features) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (traffic_influ_data->res_uri) {
    if (cJSON_AddStringToObject(item, "resUri", traffic_influ_data->res_uri) == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [res_uri]");
        goto end;
    }
    }

    if (traffic_influ_data->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_data->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_traffic_influ_data_t *OpenAPI_traffic_influ_data_parseFromJSON(cJSON *traffic_influ_dataJSON)
{
    OpenAPI_traffic_influ_data_t *traffic_influ_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *up_path_chg_notif_corre_id = NULL;
    cJSON *app_relo_ind = NULL;
    cJSON *af_app_id = NULL;
    cJSON *dnn = NULL;
    cJSON *eth_traffic_filters = NULL;
    OpenAPI_list_t *eth_traffic_filtersList = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *inter_group_id = NULL;
    cJSON *supi = NULL;
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
    cJSON *subscribed_events = NULL;
    OpenAPI_list_t *subscribed_eventsList = NULL;
    cJSON *dnai_chg_type = NULL;
    OpenAPI_dnai_change_type_e dnai_chg_typeVariable = 0;
    cJSON *af_ack_ind = NULL;
    cJSON *addr_preser_ind = NULL;
    cJSON *max_allowed_up_lat = NULL;
    cJSON *sim_conn_ind = NULL;
    cJSON *sim_conn_term = NULL;
    cJSON *supported_features = NULL;
    cJSON *res_uri = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    up_path_chg_notif_corre_id = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "upPathChgNotifCorreId");
    if (up_path_chg_notif_corre_id) {
    if (!cJSON_IsString(up_path_chg_notif_corre_id) && !cJSON_IsNull(up_path_chg_notif_corre_id)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [up_path_chg_notif_corre_id]");
        goto end;
    }
    }

    app_relo_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "appReloInd");
    if (app_relo_ind) {
    if (!cJSON_IsBool(app_relo_ind)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [app_relo_ind]");
        goto end;
    }
    }

    af_app_id = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "afAppId");
    if (af_app_id) {
    if (!cJSON_IsString(af_app_id) && !cJSON_IsNull(af_app_id)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [af_app_id]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    eth_traffic_filters = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "ethTrafficFilters");
    if (eth_traffic_filters) {
        cJSON *eth_traffic_filters_local = NULL;
        if (!cJSON_IsArray(eth_traffic_filters)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [eth_traffic_filters]");
            goto end;
        }

        eth_traffic_filtersList = OpenAPI_list_create();

        cJSON_ArrayForEach(eth_traffic_filters_local, eth_traffic_filters) {
            if (!cJSON_IsObject(eth_traffic_filters_local)) {
                ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [eth_traffic_filters]");
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

    snssai = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    inter_group_id = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "interGroupId");
    if (inter_group_id) {
    if (!cJSON_IsString(inter_group_id) && !cJSON_IsNull(inter_group_id)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [inter_group_id]");
        goto end;
    }
    }

    supi = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    traffic_filters = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "trafficFilters");
    if (traffic_filters) {
        cJSON *traffic_filters_local = NULL;
        if (!cJSON_IsArray(traffic_filters)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [traffic_filters]");
            goto end;
        }

        traffic_filtersList = OpenAPI_list_create();

        cJSON_ArrayForEach(traffic_filters_local, traffic_filters) {
            if (!cJSON_IsObject(traffic_filters_local)) {
                ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [traffic_filters]");
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

    traffic_routes = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "trafficRoutes");
    if (traffic_routes) {
        cJSON *traffic_routes_local = NULL;
        if (!cJSON_IsArray(traffic_routes)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [traffic_routes]");
            goto end;
        }

        traffic_routesList = OpenAPI_list_create();

        cJSON_ArrayForEach(traffic_routes_local, traffic_routes) {
            if (!cJSON_IsObject(traffic_routes_local)) {
                ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [traffic_routes]");
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

    traff_corre_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "traffCorreInd");
    if (traff_corre_ind) {
    if (!cJSON_IsBool(traff_corre_ind)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [traff_corre_ind]");
        goto end;
    }
    }

    valid_start_time = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "validStartTime");
    if (valid_start_time) {
    if (!cJSON_IsString(valid_start_time) && !cJSON_IsNull(valid_start_time)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [valid_start_time]");
        goto end;
    }
    }

    valid_end_time = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "validEndTime");
    if (valid_end_time) {
    if (!cJSON_IsString(valid_end_time) && !cJSON_IsNull(valid_end_time)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [valid_end_time]");
        goto end;
    }
    }

    temp_validities = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "tempValidities");
    if (temp_validities) {
        cJSON *temp_validities_local = NULL;
        if (!cJSON_IsArray(temp_validities)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [temp_validities]");
            goto end;
        }

        temp_validitiesList = OpenAPI_list_create();

        cJSON_ArrayForEach(temp_validities_local, temp_validities) {
            if (!cJSON_IsObject(temp_validities_local)) {
                ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [temp_validities]");
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

    nw_area_info = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "nwAreaInfo");
    if (nw_area_info) {
    nw_area_info_local_nonprim = OpenAPI_network_area_info_2_parseFromJSON(nw_area_info);
    if (!nw_area_info_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_2_parseFromJSON failed [nw_area_info]");
        goto end;
    }
    }

    up_path_chg_notif_uri = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "upPathChgNotifUri");
    if (up_path_chg_notif_uri) {
    if (!cJSON_IsString(up_path_chg_notif_uri) && !cJSON_IsNull(up_path_chg_notif_uri)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [up_path_chg_notif_uri]");
        goto end;
    }
    }

    headers = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "headers");
    if (headers) {
        cJSON *headers_local = NULL;
        if (!cJSON_IsArray(headers)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [headers]");
            goto end;
        }

        headersList = OpenAPI_list_create();

        cJSON_ArrayForEach(headers_local, headers) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(headers_local)) {
                ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [headers]");
                goto end;
            }
            OpenAPI_list_add(headersList, ogs_strdup(headers_local->valuestring));
        }
    }

    subscribed_events = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "subscribedEvents");
    if (subscribed_events) {
        cJSON *subscribed_events_local = NULL;
        if (!cJSON_IsArray(subscribed_events)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [subscribed_events]");
            goto end;
        }

        subscribed_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(subscribed_events_local, subscribed_events) {
            if (!cJSON_IsObject(subscribed_events_local)) {
                ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [subscribed_events]");
                goto end;
            }
            OpenAPI_subscribed_event_t *subscribed_eventsItem = OpenAPI_subscribed_event_parseFromJSON(subscribed_events_local);
            if (!subscribed_eventsItem) {
                ogs_error("No subscribed_eventsItem");
                goto end;
            }
            OpenAPI_list_add(subscribed_eventsList, subscribed_eventsItem);
        }
    }

    dnai_chg_type = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "dnaiChgType");
    if (dnai_chg_type) {
    if (!cJSON_IsString(dnai_chg_type)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [dnai_chg_type]");
        goto end;
    }
    dnai_chg_typeVariable = OpenAPI_dnai_change_type_FromString(dnai_chg_type->valuestring);
    }

    af_ack_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "afAckInd");
    if (af_ack_ind) {
    if (!cJSON_IsBool(af_ack_ind)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [af_ack_ind]");
        goto end;
    }
    }

    addr_preser_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "addrPreserInd");
    if (addr_preser_ind) {
    if (!cJSON_IsBool(addr_preser_ind)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [addr_preser_ind]");
        goto end;
    }
    }

    max_allowed_up_lat = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "maxAllowedUpLat");
    if (max_allowed_up_lat) {
    if (!cJSON_IsNumber(max_allowed_up_lat)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [max_allowed_up_lat]");
        goto end;
    }
    }

    sim_conn_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "simConnInd");
    if (sim_conn_ind) {
    if (!cJSON_IsBool(sim_conn_ind)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [sim_conn_ind]");
        goto end;
    }
    }

    sim_conn_term = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "simConnTerm");
    if (sim_conn_term) {
    if (!cJSON_IsNumber(sim_conn_term)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [sim_conn_term]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    res_uri = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "resUri");
    if (res_uri) {
    if (!cJSON_IsString(res_uri) && !cJSON_IsNull(res_uri)) {
        ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [res_uri]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    traffic_influ_data_local_var = OpenAPI_traffic_influ_data_create (
        up_path_chg_notif_corre_id && !cJSON_IsNull(up_path_chg_notif_corre_id) ? ogs_strdup(up_path_chg_notif_corre_id->valuestring) : NULL,
        app_relo_ind ? true : false,
        app_relo_ind ? app_relo_ind->valueint : 0,
        af_app_id && !cJSON_IsNull(af_app_id) ? ogs_strdup(af_app_id->valuestring) : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        eth_traffic_filters ? eth_traffic_filtersList : NULL,
        snssai ? snssai_local_nonprim : NULL,
        inter_group_id && !cJSON_IsNull(inter_group_id) ? ogs_strdup(inter_group_id->valuestring) : NULL,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        traffic_filters ? traffic_filtersList : NULL,
        traffic_routes ? traffic_routesList : NULL,
        traff_corre_ind ? true : false,
        traff_corre_ind ? traff_corre_ind->valueint : 0,
        valid_start_time && !cJSON_IsNull(valid_start_time) ? ogs_strdup(valid_start_time->valuestring) : NULL,
        valid_end_time && !cJSON_IsNull(valid_end_time) ? ogs_strdup(valid_end_time->valuestring) : NULL,
        temp_validities ? temp_validitiesList : NULL,
        nw_area_info ? nw_area_info_local_nonprim : NULL,
        up_path_chg_notif_uri && !cJSON_IsNull(up_path_chg_notif_uri) ? ogs_strdup(up_path_chg_notif_uri->valuestring) : NULL,
        headers ? headersList : NULL,
        subscribed_events ? subscribed_eventsList : NULL,
        dnai_chg_type ? dnai_chg_typeVariable : 0,
        af_ack_ind ? true : false,
        af_ack_ind ? af_ack_ind->valueint : 0,
        addr_preser_ind ? true : false,
        addr_preser_ind ? addr_preser_ind->valueint : 0,
        max_allowed_up_lat ? true : false,
        max_allowed_up_lat ? max_allowed_up_lat->valuedouble : 0,
        sim_conn_ind ? true : false,
        sim_conn_ind ? sim_conn_ind->valueint : 0,
        sim_conn_term ? true : false,
        sim_conn_term ? sim_conn_term->valuedouble : 0,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        res_uri && !cJSON_IsNull(res_uri) ? ogs_strdup(res_uri->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return traffic_influ_data_local_var;
end:
    if (eth_traffic_filtersList) {
        OpenAPI_list_for_each(eth_traffic_filtersList, node) {
            OpenAPI_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(eth_traffic_filtersList);
        eth_traffic_filtersList = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
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
    if (subscribed_eventsList) {
        OpenAPI_list_for_each(subscribed_eventsList, node) {
            OpenAPI_subscribed_event_free(node->data);
        }
        OpenAPI_list_free(subscribed_eventsList);
        subscribed_eventsList = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
    return NULL;
}

OpenAPI_traffic_influ_data_t *OpenAPI_traffic_influ_data_copy(OpenAPI_traffic_influ_data_t *dst, OpenAPI_traffic_influ_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_influ_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed");
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

    OpenAPI_traffic_influ_data_free(dst);
    dst = OpenAPI_traffic_influ_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

