
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_data_set_rm.h"

OpenAPI_traffic_data_set_rm_t *OpenAPI_traffic_data_set_rm_create(
    char *set_id,
    bool is_traffic_filters_null,
    OpenAPI_list_t *traffic_filters,
    bool is_eth_traffic_filters_null,
    OpenAPI_list_t *eth_traffic_filters,
    bool is_traffic_routes_null,
    OpenAPI_list_t *traffic_routes
)
{
    OpenAPI_traffic_data_set_rm_t *traffic_data_set_rm_local_var = ogs_malloc(sizeof(OpenAPI_traffic_data_set_rm_t));
    ogs_assert(traffic_data_set_rm_local_var);

    traffic_data_set_rm_local_var->set_id = set_id;
    traffic_data_set_rm_local_var->is_traffic_filters_null = is_traffic_filters_null;
    traffic_data_set_rm_local_var->traffic_filters = traffic_filters;
    traffic_data_set_rm_local_var->is_eth_traffic_filters_null = is_eth_traffic_filters_null;
    traffic_data_set_rm_local_var->eth_traffic_filters = eth_traffic_filters;
    traffic_data_set_rm_local_var->is_traffic_routes_null = is_traffic_routes_null;
    traffic_data_set_rm_local_var->traffic_routes = traffic_routes;

    return traffic_data_set_rm_local_var;
}

void OpenAPI_traffic_data_set_rm_free(OpenAPI_traffic_data_set_rm_t *traffic_data_set_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_data_set_rm) {
        return;
    }
    if (traffic_data_set_rm->set_id) {
        ogs_free(traffic_data_set_rm->set_id);
        traffic_data_set_rm->set_id = NULL;
    }
    if (traffic_data_set_rm->traffic_filters) {
        OpenAPI_list_for_each(traffic_data_set_rm->traffic_filters, node) {
            OpenAPI_flow_info_1_free(node->data);
        }
        OpenAPI_list_free(traffic_data_set_rm->traffic_filters);
        traffic_data_set_rm->traffic_filters = NULL;
    }
    if (traffic_data_set_rm->eth_traffic_filters) {
        OpenAPI_list_for_each(traffic_data_set_rm->eth_traffic_filters, node) {
            OpenAPI_eth_flow_description_1_free(node->data);
        }
        OpenAPI_list_free(traffic_data_set_rm->eth_traffic_filters);
        traffic_data_set_rm->eth_traffic_filters = NULL;
    }
    if (traffic_data_set_rm->traffic_routes) {
        OpenAPI_list_for_each(traffic_data_set_rm->traffic_routes, node) {
            OpenAPI_route_to_location_1_free(node->data);
        }
        OpenAPI_list_free(traffic_data_set_rm->traffic_routes);
        traffic_data_set_rm->traffic_routes = NULL;
    }
    ogs_free(traffic_data_set_rm);
}

cJSON *OpenAPI_traffic_data_set_rm_convertToJSON(OpenAPI_traffic_data_set_rm_t *traffic_data_set_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_data_set_rm == NULL) {
        ogs_error("OpenAPI_traffic_data_set_rm_convertToJSON() failed [TrafficDataSetRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!traffic_data_set_rm->set_id) {
        ogs_error("OpenAPI_traffic_data_set_rm_convertToJSON() failed [set_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "setId", traffic_data_set_rm->set_id) == NULL) {
        ogs_error("OpenAPI_traffic_data_set_rm_convertToJSON() failed [set_id]");
        goto end;
    }

    if (traffic_data_set_rm->traffic_filters) {
    cJSON *traffic_filtersList = cJSON_AddArrayToObject(item, "trafficFilters");
    if (traffic_filtersList == NULL) {
        ogs_error("OpenAPI_traffic_data_set_rm_convertToJSON() failed [traffic_filters]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_data_set_rm->traffic_filters, node) {
        cJSON *itemLocal = OpenAPI_flow_info_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_data_set_rm_convertToJSON() failed [traffic_filters]");
            goto end;
        }
        cJSON_AddItemToArray(traffic_filtersList, itemLocal);
    }
    } else if (traffic_data_set_rm->is_traffic_filters_null) {
        if (cJSON_AddNullToObject(item, "trafficFilters") == NULL) {
            ogs_error("OpenAPI_traffic_data_set_rm_convertToJSON() failed [traffic_filters]");
            goto end;
        }
    }

    if (traffic_data_set_rm->eth_traffic_filters) {
    cJSON *eth_traffic_filtersList = cJSON_AddArrayToObject(item, "ethTrafficFilters");
    if (eth_traffic_filtersList == NULL) {
        ogs_error("OpenAPI_traffic_data_set_rm_convertToJSON() failed [eth_traffic_filters]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_data_set_rm->eth_traffic_filters, node) {
        cJSON *itemLocal = OpenAPI_eth_flow_description_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_data_set_rm_convertToJSON() failed [eth_traffic_filters]");
            goto end;
        }
        cJSON_AddItemToArray(eth_traffic_filtersList, itemLocal);
    }
    } else if (traffic_data_set_rm->is_eth_traffic_filters_null) {
        if (cJSON_AddNullToObject(item, "ethTrafficFilters") == NULL) {
            ogs_error("OpenAPI_traffic_data_set_rm_convertToJSON() failed [eth_traffic_filters]");
            goto end;
        }
    }

    if (traffic_data_set_rm->traffic_routes) {
    cJSON *traffic_routesList = cJSON_AddArrayToObject(item, "trafficRoutes");
    if (traffic_routesList == NULL) {
        ogs_error("OpenAPI_traffic_data_set_rm_convertToJSON() failed [traffic_routes]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_data_set_rm->traffic_routes, node) {
        cJSON *itemLocal = OpenAPI_route_to_location_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_data_set_rm_convertToJSON() failed [traffic_routes]");
            goto end;
        }
        cJSON_AddItemToArray(traffic_routesList, itemLocal);
    }
    } else if (traffic_data_set_rm->is_traffic_routes_null) {
        if (cJSON_AddNullToObject(item, "trafficRoutes") == NULL) {
            ogs_error("OpenAPI_traffic_data_set_rm_convertToJSON() failed [traffic_routes]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_traffic_data_set_rm_t *OpenAPI_traffic_data_set_rm_parseFromJSON(cJSON *traffic_data_set_rmJSON)
{
    OpenAPI_traffic_data_set_rm_t *traffic_data_set_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *set_id = NULL;
    cJSON *traffic_filters = NULL;
    OpenAPI_list_t *traffic_filtersList = NULL;
    cJSON *eth_traffic_filters = NULL;
    OpenAPI_list_t *eth_traffic_filtersList = NULL;
    cJSON *traffic_routes = NULL;
    OpenAPI_list_t *traffic_routesList = NULL;
    set_id = cJSON_GetObjectItemCaseSensitive(traffic_data_set_rmJSON, "setId");
    if (!set_id) {
        ogs_error("OpenAPI_traffic_data_set_rm_parseFromJSON() failed [set_id]");
        goto end;
    }
    if (!cJSON_IsString(set_id)) {
        ogs_error("OpenAPI_traffic_data_set_rm_parseFromJSON() failed [set_id]");
        goto end;
    }

    traffic_filters = cJSON_GetObjectItemCaseSensitive(traffic_data_set_rmJSON, "trafficFilters");
    if (traffic_filters) {
    if (!cJSON_IsNull(traffic_filters)) {
        cJSON *traffic_filters_local = NULL;
        if (!cJSON_IsArray(traffic_filters)) {
            ogs_error("OpenAPI_traffic_data_set_rm_parseFromJSON() failed [traffic_filters]");
            goto end;
        }

        traffic_filtersList = OpenAPI_list_create();

        cJSON_ArrayForEach(traffic_filters_local, traffic_filters) {
            if (!cJSON_IsObject(traffic_filters_local)) {
                ogs_error("OpenAPI_traffic_data_set_rm_parseFromJSON() failed [traffic_filters]");
                goto end;
            }
            OpenAPI_flow_info_1_t *traffic_filtersItem = OpenAPI_flow_info_1_parseFromJSON(traffic_filters_local);
            if (!traffic_filtersItem) {
                ogs_error("No traffic_filtersItem");
                goto end;
            }
            OpenAPI_list_add(traffic_filtersList, traffic_filtersItem);
        }
    }
    }

    eth_traffic_filters = cJSON_GetObjectItemCaseSensitive(traffic_data_set_rmJSON, "ethTrafficFilters");
    if (eth_traffic_filters) {
    if (!cJSON_IsNull(eth_traffic_filters)) {
        cJSON *eth_traffic_filters_local = NULL;
        if (!cJSON_IsArray(eth_traffic_filters)) {
            ogs_error("OpenAPI_traffic_data_set_rm_parseFromJSON() failed [eth_traffic_filters]");
            goto end;
        }

        eth_traffic_filtersList = OpenAPI_list_create();

        cJSON_ArrayForEach(eth_traffic_filters_local, eth_traffic_filters) {
            if (!cJSON_IsObject(eth_traffic_filters_local)) {
                ogs_error("OpenAPI_traffic_data_set_rm_parseFromJSON() failed [eth_traffic_filters]");
                goto end;
            }
            OpenAPI_eth_flow_description_1_t *eth_traffic_filtersItem = OpenAPI_eth_flow_description_1_parseFromJSON(eth_traffic_filters_local);
            if (!eth_traffic_filtersItem) {
                ogs_error("No eth_traffic_filtersItem");
                goto end;
            }
            OpenAPI_list_add(eth_traffic_filtersList, eth_traffic_filtersItem);
        }
    }
    }

    traffic_routes = cJSON_GetObjectItemCaseSensitive(traffic_data_set_rmJSON, "trafficRoutes");
    if (traffic_routes) {
    if (!cJSON_IsNull(traffic_routes)) {
        cJSON *traffic_routes_local = NULL;
        if (!cJSON_IsArray(traffic_routes)) {
            ogs_error("OpenAPI_traffic_data_set_rm_parseFromJSON() failed [traffic_routes]");
            goto end;
        }

        traffic_routesList = OpenAPI_list_create();

        cJSON_ArrayForEach(traffic_routes_local, traffic_routes) {
            if (!cJSON_IsObject(traffic_routes_local)) {
                ogs_error("OpenAPI_traffic_data_set_rm_parseFromJSON() failed [traffic_routes]");
                goto end;
            }
            OpenAPI_route_to_location_1_t *traffic_routesItem = OpenAPI_route_to_location_1_parseFromJSON(traffic_routes_local);
            if (!traffic_routesItem) {
                ogs_error("No traffic_routesItem");
                goto end;
            }
            OpenAPI_list_add(traffic_routesList, traffic_routesItem);
        }
    }
    }

    traffic_data_set_rm_local_var = OpenAPI_traffic_data_set_rm_create (
        ogs_strdup(set_id->valuestring),
        traffic_filters && cJSON_IsNull(traffic_filters) ? true : false,
        traffic_filters ? traffic_filtersList : NULL,
        eth_traffic_filters && cJSON_IsNull(eth_traffic_filters) ? true : false,
        eth_traffic_filters ? eth_traffic_filtersList : NULL,
        traffic_routes && cJSON_IsNull(traffic_routes) ? true : false,
        traffic_routes ? traffic_routesList : NULL
    );

    return traffic_data_set_rm_local_var;
end:
    if (traffic_filtersList) {
        OpenAPI_list_for_each(traffic_filtersList, node) {
            OpenAPI_flow_info_1_free(node->data);
        }
        OpenAPI_list_free(traffic_filtersList);
        traffic_filtersList = NULL;
    }
    if (eth_traffic_filtersList) {
        OpenAPI_list_for_each(eth_traffic_filtersList, node) {
            OpenAPI_eth_flow_description_1_free(node->data);
        }
        OpenAPI_list_free(eth_traffic_filtersList);
        eth_traffic_filtersList = NULL;
    }
    if (traffic_routesList) {
        OpenAPI_list_for_each(traffic_routesList, node) {
            OpenAPI_route_to_location_1_free(node->data);
        }
        OpenAPI_list_free(traffic_routesList);
        traffic_routesList = NULL;
    }
    return NULL;
}

OpenAPI_traffic_data_set_rm_t *OpenAPI_traffic_data_set_rm_copy(OpenAPI_traffic_data_set_rm_t *dst, OpenAPI_traffic_data_set_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_data_set_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_data_set_rm_convertToJSON() failed");
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

    OpenAPI_traffic_data_set_rm_free(dst);
    dst = OpenAPI_traffic_data_set_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

