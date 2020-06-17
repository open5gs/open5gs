
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_influ_data.h"

OpenAPI_traffic_influ_data_t *OpenAPI_traffic_influ_data_create(
    char *up_path_chg_notif_corre_id,
    int app_relo_ind,
    char *af_app_id,
    char *dnn,
    OpenAPI_list_t *eth_traffic_filters,
    OpenAPI_snssai_t *snssai,
    char *inter_group_id,
    char *supi,
    OpenAPI_list_t *traffic_filters,
    OpenAPI_list_t *traffic_routes,
    int traff_corre_ind,
    char *valid_start_time,
    char *valid_end_time,
    OpenAPI_list_t *temp_validities,
    OpenAPI_network_area_info_2_t *nw_area_info,
    char *up_path_chg_notif_uri,
    OpenAPI_list_t *subscribed_events,
    OpenAPI_dnai_change_type_t *dnai_chg_type,
    int af_ack_ind,
    int addr_preser_ind,
    char *supported_features
    )
{
    OpenAPI_traffic_influ_data_t *traffic_influ_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_traffic_influ_data_t));
    if (!traffic_influ_data_local_var) {
        return NULL;
    }
    traffic_influ_data_local_var->up_path_chg_notif_corre_id = up_path_chg_notif_corre_id;
    traffic_influ_data_local_var->app_relo_ind = app_relo_ind;
    traffic_influ_data_local_var->af_app_id = af_app_id;
    traffic_influ_data_local_var->dnn = dnn;
    traffic_influ_data_local_var->eth_traffic_filters = eth_traffic_filters;
    traffic_influ_data_local_var->snssai = snssai;
    traffic_influ_data_local_var->inter_group_id = inter_group_id;
    traffic_influ_data_local_var->supi = supi;
    traffic_influ_data_local_var->traffic_filters = traffic_filters;
    traffic_influ_data_local_var->traffic_routes = traffic_routes;
    traffic_influ_data_local_var->traff_corre_ind = traff_corre_ind;
    traffic_influ_data_local_var->valid_start_time = valid_start_time;
    traffic_influ_data_local_var->valid_end_time = valid_end_time;
    traffic_influ_data_local_var->temp_validities = temp_validities;
    traffic_influ_data_local_var->nw_area_info = nw_area_info;
    traffic_influ_data_local_var->up_path_chg_notif_uri = up_path_chg_notif_uri;
    traffic_influ_data_local_var->subscribed_events = subscribed_events;
    traffic_influ_data_local_var->dnai_chg_type = dnai_chg_type;
    traffic_influ_data_local_var->af_ack_ind = af_ack_ind;
    traffic_influ_data_local_var->addr_preser_ind = addr_preser_ind;
    traffic_influ_data_local_var->supported_features = supported_features;

    return traffic_influ_data_local_var;
}

void OpenAPI_traffic_influ_data_free(OpenAPI_traffic_influ_data_t *traffic_influ_data)
{
    if (NULL == traffic_influ_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(traffic_influ_data->up_path_chg_notif_corre_id);
    ogs_free(traffic_influ_data->af_app_id);
    ogs_free(traffic_influ_data->dnn);
    OpenAPI_list_for_each(traffic_influ_data->eth_traffic_filters, node) {
        OpenAPI_eth_flow_description_free(node->data);
    }
    OpenAPI_list_free(traffic_influ_data->eth_traffic_filters);
    OpenAPI_snssai_free(traffic_influ_data->snssai);
    ogs_free(traffic_influ_data->inter_group_id);
    ogs_free(traffic_influ_data->supi);
    OpenAPI_list_for_each(traffic_influ_data->traffic_filters, node) {
        OpenAPI_flow_info_free(node->data);
    }
    OpenAPI_list_free(traffic_influ_data->traffic_filters);
    OpenAPI_list_for_each(traffic_influ_data->traffic_routes, node) {
        OpenAPI_route_to_location_free(node->data);
    }
    OpenAPI_list_free(traffic_influ_data->traffic_routes);
    ogs_free(traffic_influ_data->valid_start_time);
    ogs_free(traffic_influ_data->valid_end_time);
    OpenAPI_list_for_each(traffic_influ_data->temp_validities, node) {
        OpenAPI_temporal_validity_free(node->data);
    }
    OpenAPI_list_free(traffic_influ_data->temp_validities);
    OpenAPI_network_area_info_2_free(traffic_influ_data->nw_area_info);
    ogs_free(traffic_influ_data->up_path_chg_notif_uri);
    OpenAPI_list_for_each(traffic_influ_data->subscribed_events, node) {
        OpenAPI_subscribed_event_free(node->data);
    }
    OpenAPI_list_free(traffic_influ_data->subscribed_events);
    OpenAPI_dnai_change_type_free(traffic_influ_data->dnai_chg_type);
    ogs_free(traffic_influ_data->supported_features);
    ogs_free(traffic_influ_data);
}

cJSON *OpenAPI_traffic_influ_data_convertToJSON(OpenAPI_traffic_influ_data_t *traffic_influ_data)
{
    cJSON *item = NULL;

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

    if (traffic_influ_data->app_relo_ind) {
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

        OpenAPI_lnode_t *eth_traffic_filters_node;
        if (traffic_influ_data->eth_traffic_filters) {
            OpenAPI_list_for_each(traffic_influ_data->eth_traffic_filters, eth_traffic_filters_node) {
                cJSON *itemLocal = OpenAPI_eth_flow_description_convertToJSON(eth_traffic_filters_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [eth_traffic_filters]");
                    goto end;
                }
                cJSON_AddItemToArray(eth_traffic_filtersList, itemLocal);
            }
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

        OpenAPI_lnode_t *traffic_filters_node;
        if (traffic_influ_data->traffic_filters) {
            OpenAPI_list_for_each(traffic_influ_data->traffic_filters, traffic_filters_node) {
                cJSON *itemLocal = OpenAPI_flow_info_convertToJSON(traffic_filters_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [traffic_filters]");
                    goto end;
                }
                cJSON_AddItemToArray(traffic_filtersList, itemLocal);
            }
        }
    }

    if (traffic_influ_data->traffic_routes) {
        cJSON *traffic_routesList = cJSON_AddArrayToObject(item, "trafficRoutes");
        if (traffic_routesList == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [traffic_routes]");
            goto end;
        }

        OpenAPI_lnode_t *traffic_routes_node;
        if (traffic_influ_data->traffic_routes) {
            OpenAPI_list_for_each(traffic_influ_data->traffic_routes, traffic_routes_node) {
                cJSON *itemLocal = OpenAPI_route_to_location_convertToJSON(traffic_routes_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [traffic_routes]");
                    goto end;
                }
                cJSON_AddItemToArray(traffic_routesList, itemLocal);
            }
        }
    }

    if (traffic_influ_data->traff_corre_ind) {
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

        OpenAPI_lnode_t *temp_validities_node;
        if (traffic_influ_data->temp_validities) {
            OpenAPI_list_for_each(traffic_influ_data->temp_validities, temp_validities_node) {
                cJSON *itemLocal = OpenAPI_temporal_validity_convertToJSON(temp_validities_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [temp_validities]");
                    goto end;
                }
                cJSON_AddItemToArray(temp_validitiesList, itemLocal);
            }
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

    if (traffic_influ_data->subscribed_events) {
        cJSON *subscribed_eventsList = cJSON_AddArrayToObject(item, "subscribedEvents");
        if (subscribed_eventsList == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [subscribed_events]");
            goto end;
        }

        OpenAPI_lnode_t *subscribed_events_node;
        if (traffic_influ_data->subscribed_events) {
            OpenAPI_list_for_each(traffic_influ_data->subscribed_events, subscribed_events_node) {
                cJSON *itemLocal = OpenAPI_subscribed_event_convertToJSON(subscribed_events_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [subscribed_events]");
                    goto end;
                }
                cJSON_AddItemToArray(subscribed_eventsList, itemLocal);
            }
        }
    }

    if (traffic_influ_data->dnai_chg_type) {
        cJSON *dnai_chg_type_local_JSON = OpenAPI_dnai_change_type_convertToJSON(traffic_influ_data->dnai_chg_type);
        if (dnai_chg_type_local_JSON == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [dnai_chg_type]");
            goto end;
        }
        cJSON_AddItemToObject(item, "dnaiChgType", dnai_chg_type_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [dnai_chg_type]");
            goto end;
        }
    }

    if (traffic_influ_data->af_ack_ind) {
        if (cJSON_AddBoolToObject(item, "afAckInd", traffic_influ_data->af_ack_ind) == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [af_ack_ind]");
            goto end;
        }
    }

    if (traffic_influ_data->addr_preser_ind) {
        if (cJSON_AddBoolToObject(item, "addrPreserInd", traffic_influ_data->addr_preser_ind) == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [addr_preser_ind]");
            goto end;
        }
    }

    if (traffic_influ_data->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", traffic_influ_data->supported_features) == NULL) {
            ogs_error("OpenAPI_traffic_influ_data_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_traffic_influ_data_t *OpenAPI_traffic_influ_data_parseFromJSON(cJSON *traffic_influ_dataJSON)
{
    OpenAPI_traffic_influ_data_t *traffic_influ_data_local_var = NULL;
    cJSON *up_path_chg_notif_corre_id = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "upPathChgNotifCorreId");

    if (up_path_chg_notif_corre_id) {
        if (!cJSON_IsString(up_path_chg_notif_corre_id)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [up_path_chg_notif_corre_id]");
            goto end;
        }
    }

    cJSON *app_relo_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "appReloInd");

    if (app_relo_ind) {
        if (!cJSON_IsBool(app_relo_ind)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [app_relo_ind]");
            goto end;
        }
    }

    cJSON *af_app_id = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "afAppId");

    if (af_app_id) {
        if (!cJSON_IsString(af_app_id)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [af_app_id]");
            goto end;
        }
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "dnn");

    if (dnn) {
        if (!cJSON_IsString(dnn)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [dnn]");
            goto end;
        }
    }

    cJSON *eth_traffic_filters = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "ethTrafficFilters");

    OpenAPI_list_t *eth_traffic_filtersList;
    if (eth_traffic_filters) {
        cJSON *eth_traffic_filters_local_nonprimitive;
        if (!cJSON_IsArray(eth_traffic_filters)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [eth_traffic_filters]");
            goto end;
        }

        eth_traffic_filtersList = OpenAPI_list_create();

        cJSON_ArrayForEach(eth_traffic_filters_local_nonprimitive, eth_traffic_filters ) {
            if (!cJSON_IsObject(eth_traffic_filters_local_nonprimitive)) {
                ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [eth_traffic_filters]");
                goto end;
            }
            OpenAPI_eth_flow_description_t *eth_traffic_filtersItem = OpenAPI_eth_flow_description_parseFromJSON(eth_traffic_filters_local_nonprimitive);

            OpenAPI_list_add(eth_traffic_filtersList, eth_traffic_filtersItem);
        }
    }

    cJSON *snssai = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "snssai");

    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    if (snssai) {
        snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    }

    cJSON *inter_group_id = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "interGroupId");

    if (inter_group_id) {
        if (!cJSON_IsString(inter_group_id)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [inter_group_id]");
            goto end;
        }
    }

    cJSON *supi = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "supi");

    if (supi) {
        if (!cJSON_IsString(supi)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [supi]");
            goto end;
        }
    }

    cJSON *traffic_filters = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "trafficFilters");

    OpenAPI_list_t *traffic_filtersList;
    if (traffic_filters) {
        cJSON *traffic_filters_local_nonprimitive;
        if (!cJSON_IsArray(traffic_filters)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [traffic_filters]");
            goto end;
        }

        traffic_filtersList = OpenAPI_list_create();

        cJSON_ArrayForEach(traffic_filters_local_nonprimitive, traffic_filters ) {
            if (!cJSON_IsObject(traffic_filters_local_nonprimitive)) {
                ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [traffic_filters]");
                goto end;
            }
            OpenAPI_flow_info_t *traffic_filtersItem = OpenAPI_flow_info_parseFromJSON(traffic_filters_local_nonprimitive);

            OpenAPI_list_add(traffic_filtersList, traffic_filtersItem);
        }
    }

    cJSON *traffic_routes = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "trafficRoutes");

    OpenAPI_list_t *traffic_routesList;
    if (traffic_routes) {
        cJSON *traffic_routes_local_nonprimitive;
        if (!cJSON_IsArray(traffic_routes)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [traffic_routes]");
            goto end;
        }

        traffic_routesList = OpenAPI_list_create();

        cJSON_ArrayForEach(traffic_routes_local_nonprimitive, traffic_routes ) {
            if (!cJSON_IsObject(traffic_routes_local_nonprimitive)) {
                ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [traffic_routes]");
                goto end;
            }
            OpenAPI_route_to_location_t *traffic_routesItem = OpenAPI_route_to_location_parseFromJSON(traffic_routes_local_nonprimitive);

            OpenAPI_list_add(traffic_routesList, traffic_routesItem);
        }
    }

    cJSON *traff_corre_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "traffCorreInd");

    if (traff_corre_ind) {
        if (!cJSON_IsBool(traff_corre_ind)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [traff_corre_ind]");
            goto end;
        }
    }

    cJSON *valid_start_time = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "validStartTime");

    if (valid_start_time) {
        if (!cJSON_IsString(valid_start_time)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [valid_start_time]");
            goto end;
        }
    }

    cJSON *valid_end_time = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "validEndTime");

    if (valid_end_time) {
        if (!cJSON_IsString(valid_end_time)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [valid_end_time]");
            goto end;
        }
    }

    cJSON *temp_validities = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "tempValidities");

    OpenAPI_list_t *temp_validitiesList;
    if (temp_validities) {
        cJSON *temp_validities_local_nonprimitive;
        if (!cJSON_IsArray(temp_validities)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [temp_validities]");
            goto end;
        }

        temp_validitiesList = OpenAPI_list_create();

        cJSON_ArrayForEach(temp_validities_local_nonprimitive, temp_validities ) {
            if (!cJSON_IsObject(temp_validities_local_nonprimitive)) {
                ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [temp_validities]");
                goto end;
            }
            OpenAPI_temporal_validity_t *temp_validitiesItem = OpenAPI_temporal_validity_parseFromJSON(temp_validities_local_nonprimitive);

            OpenAPI_list_add(temp_validitiesList, temp_validitiesItem);
        }
    }

    cJSON *nw_area_info = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "nwAreaInfo");

    OpenAPI_network_area_info_2_t *nw_area_info_local_nonprim = NULL;
    if (nw_area_info) {
        nw_area_info_local_nonprim = OpenAPI_network_area_info_2_parseFromJSON(nw_area_info);
    }

    cJSON *up_path_chg_notif_uri = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "upPathChgNotifUri");

    if (up_path_chg_notif_uri) {
        if (!cJSON_IsString(up_path_chg_notif_uri)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [up_path_chg_notif_uri]");
            goto end;
        }
    }

    cJSON *subscribed_events = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "subscribedEvents");

    OpenAPI_list_t *subscribed_eventsList;
    if (subscribed_events) {
        cJSON *subscribed_events_local_nonprimitive;
        if (!cJSON_IsArray(subscribed_events)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [subscribed_events]");
            goto end;
        }

        subscribed_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(subscribed_events_local_nonprimitive, subscribed_events ) {
            if (!cJSON_IsObject(subscribed_events_local_nonprimitive)) {
                ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [subscribed_events]");
                goto end;
            }
            OpenAPI_subscribed_event_t *subscribed_eventsItem = OpenAPI_subscribed_event_parseFromJSON(subscribed_events_local_nonprimitive);

            OpenAPI_list_add(subscribed_eventsList, subscribed_eventsItem);
        }
    }

    cJSON *dnai_chg_type = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "dnaiChgType");

    OpenAPI_dnai_change_type_t *dnai_chg_type_local_nonprim = NULL;
    if (dnai_chg_type) {
        dnai_chg_type_local_nonprim = OpenAPI_dnai_change_type_parseFromJSON(dnai_chg_type);
    }

    cJSON *af_ack_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "afAckInd");

    if (af_ack_ind) {
        if (!cJSON_IsBool(af_ack_ind)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [af_ack_ind]");
            goto end;
        }
    }

    cJSON *addr_preser_ind = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "addrPreserInd");

    if (addr_preser_ind) {
        if (!cJSON_IsBool(addr_preser_ind)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [addr_preser_ind]");
            goto end;
        }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(traffic_influ_dataJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_traffic_influ_data_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    traffic_influ_data_local_var = OpenAPI_traffic_influ_data_create (
        up_path_chg_notif_corre_id ? ogs_strdup(up_path_chg_notif_corre_id->valuestring) : NULL,
        app_relo_ind ? app_relo_ind->valueint : 0,
        af_app_id ? ogs_strdup(af_app_id->valuestring) : NULL,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        eth_traffic_filters ? eth_traffic_filtersList : NULL,
        snssai ? snssai_local_nonprim : NULL,
        inter_group_id ? ogs_strdup(inter_group_id->valuestring) : NULL,
        supi ? ogs_strdup(supi->valuestring) : NULL,
        traffic_filters ? traffic_filtersList : NULL,
        traffic_routes ? traffic_routesList : NULL,
        traff_corre_ind ? traff_corre_ind->valueint : 0,
        valid_start_time ? ogs_strdup(valid_start_time->valuestring) : NULL,
        valid_end_time ? ogs_strdup(valid_end_time->valuestring) : NULL,
        temp_validities ? temp_validitiesList : NULL,
        nw_area_info ? nw_area_info_local_nonprim : NULL,
        up_path_chg_notif_uri ? ogs_strdup(up_path_chg_notif_uri->valuestring) : NULL,
        subscribed_events ? subscribed_eventsList : NULL,
        dnai_chg_type ? dnai_chg_type_local_nonprim : NULL,
        af_ack_ind ? af_ack_ind->valueint : 0,
        addr_preser_ind ? addr_preser_ind->valueint : 0,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
        );

    return traffic_influ_data_local_var;
end:
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

