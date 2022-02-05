
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_area_1.h"

OpenAPI_location_area_1_t *OpenAPI_location_area_1_create(
    OpenAPI_list_t *geographic_areas,
    OpenAPI_list_t *civic_addresses,
    OpenAPI_network_area_info_1_t *nw_area_info
)
{
    OpenAPI_location_area_1_t *location_area_1_local_var = ogs_malloc(sizeof(OpenAPI_location_area_1_t));
    ogs_assert(location_area_1_local_var);

    location_area_1_local_var->geographic_areas = geographic_areas;
    location_area_1_local_var->civic_addresses = civic_addresses;
    location_area_1_local_var->nw_area_info = nw_area_info;

    return location_area_1_local_var;
}

void OpenAPI_location_area_1_free(OpenAPI_location_area_1_t *location_area_1)
{
    if (NULL == location_area_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(location_area_1->geographic_areas, node) {
        OpenAPI_geographic_area_free(node->data);
    }
    OpenAPI_list_free(location_area_1->geographic_areas);
    OpenAPI_list_for_each(location_area_1->civic_addresses, node) {
        OpenAPI_civic_address_free(node->data);
    }
    OpenAPI_list_free(location_area_1->civic_addresses);
    OpenAPI_network_area_info_1_free(location_area_1->nw_area_info);
    ogs_free(location_area_1);
}

cJSON *OpenAPI_location_area_1_convertToJSON(OpenAPI_location_area_1_t *location_area_1)
{
    cJSON *item = NULL;

    if (location_area_1 == NULL) {
        ogs_error("OpenAPI_location_area_1_convertToJSON() failed [LocationArea_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (location_area_1->geographic_areas) {
    cJSON *geographic_areasList = cJSON_AddArrayToObject(item, "geographicAreas");
    if (geographic_areasList == NULL) {
        ogs_error("OpenAPI_location_area_1_convertToJSON() failed [geographic_areas]");
        goto end;
    }

    OpenAPI_lnode_t *geographic_areas_node;
    if (location_area_1->geographic_areas) {
        OpenAPI_list_for_each(location_area_1->geographic_areas, geographic_areas_node) {
            cJSON *itemLocal = OpenAPI_geographic_area_convertToJSON(geographic_areas_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_location_area_1_convertToJSON() failed [geographic_areas]");
                goto end;
            }
            cJSON_AddItemToArray(geographic_areasList, itemLocal);
        }
    }
    }

    if (location_area_1->civic_addresses) {
    cJSON *civic_addressesList = cJSON_AddArrayToObject(item, "civicAddresses");
    if (civic_addressesList == NULL) {
        ogs_error("OpenAPI_location_area_1_convertToJSON() failed [civic_addresses]");
        goto end;
    }

    OpenAPI_lnode_t *civic_addresses_node;
    if (location_area_1->civic_addresses) {
        OpenAPI_list_for_each(location_area_1->civic_addresses, civic_addresses_node) {
            cJSON *itemLocal = OpenAPI_civic_address_convertToJSON(civic_addresses_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_location_area_1_convertToJSON() failed [civic_addresses]");
                goto end;
            }
            cJSON_AddItemToArray(civic_addressesList, itemLocal);
        }
    }
    }

    if (location_area_1->nw_area_info) {
    cJSON *nw_area_info_local_JSON = OpenAPI_network_area_info_1_convertToJSON(location_area_1->nw_area_info);
    if (nw_area_info_local_JSON == NULL) {
        ogs_error("OpenAPI_location_area_1_convertToJSON() failed [nw_area_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nwAreaInfo", nw_area_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_area_1_convertToJSON() failed [nw_area_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_location_area_1_t *OpenAPI_location_area_1_parseFromJSON(cJSON *location_area_1JSON)
{
    OpenAPI_location_area_1_t *location_area_1_local_var = NULL;
    cJSON *geographic_areas = cJSON_GetObjectItemCaseSensitive(location_area_1JSON, "geographicAreas");

    OpenAPI_list_t *geographic_areasList;
    if (geographic_areas) {
    cJSON *geographic_areas_local_nonprimitive;
    if (!cJSON_IsArray(geographic_areas)){
        ogs_error("OpenAPI_location_area_1_parseFromJSON() failed [geographic_areas]");
        goto end;
    }

    geographic_areasList = OpenAPI_list_create();

    cJSON_ArrayForEach(geographic_areas_local_nonprimitive, geographic_areas ) {
        if (!cJSON_IsObject(geographic_areas_local_nonprimitive)) {
            ogs_error("OpenAPI_location_area_1_parseFromJSON() failed [geographic_areas]");
            goto end;
        }
        OpenAPI_geographic_area_t *geographic_areasItem = OpenAPI_geographic_area_parseFromJSON(geographic_areas_local_nonprimitive);

        if (!geographic_areasItem) {
            ogs_error("No geographic_areasItem");
            OpenAPI_list_free(geographic_areasList);
            goto end;
        }

        OpenAPI_list_add(geographic_areasList, geographic_areasItem);
    }
    }

    cJSON *civic_addresses = cJSON_GetObjectItemCaseSensitive(location_area_1JSON, "civicAddresses");

    OpenAPI_list_t *civic_addressesList;
    if (civic_addresses) {
    cJSON *civic_addresses_local_nonprimitive;
    if (!cJSON_IsArray(civic_addresses)){
        ogs_error("OpenAPI_location_area_1_parseFromJSON() failed [civic_addresses]");
        goto end;
    }

    civic_addressesList = OpenAPI_list_create();

    cJSON_ArrayForEach(civic_addresses_local_nonprimitive, civic_addresses ) {
        if (!cJSON_IsObject(civic_addresses_local_nonprimitive)) {
            ogs_error("OpenAPI_location_area_1_parseFromJSON() failed [civic_addresses]");
            goto end;
        }
        OpenAPI_civic_address_t *civic_addressesItem = OpenAPI_civic_address_parseFromJSON(civic_addresses_local_nonprimitive);

        if (!civic_addressesItem) {
            ogs_error("No civic_addressesItem");
            OpenAPI_list_free(civic_addressesList);
            goto end;
        }

        OpenAPI_list_add(civic_addressesList, civic_addressesItem);
    }
    }

    cJSON *nw_area_info = cJSON_GetObjectItemCaseSensitive(location_area_1JSON, "nwAreaInfo");

    OpenAPI_network_area_info_1_t *nw_area_info_local_nonprim = NULL;
    if (nw_area_info) {
    nw_area_info_local_nonprim = OpenAPI_network_area_info_1_parseFromJSON(nw_area_info);
    }

    location_area_1_local_var = OpenAPI_location_area_1_create (
        geographic_areas ? geographic_areasList : NULL,
        civic_addresses ? civic_addressesList : NULL,
        nw_area_info ? nw_area_info_local_nonprim : NULL
    );

    return location_area_1_local_var;
end:
    return NULL;
}

OpenAPI_location_area_1_t *OpenAPI_location_area_1_copy(OpenAPI_location_area_1_t *dst, OpenAPI_location_area_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_location_area_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_location_area_1_convertToJSON() failed");
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

    OpenAPI_location_area_1_free(dst);
    dst = OpenAPI_location_area_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

