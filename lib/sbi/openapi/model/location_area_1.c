
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_area_1.h"

OpenAPI_location_area_1_t *OpenAPI_location_area_1_create(
    OpenAPI_list_t *geographic_areas,
    OpenAPI_list_t *civic_addresses,
    OpenAPI_network_area_info_1_t *nw_area_info,
    OpenAPI_umt_time_1_t *umt_time
)
{
    OpenAPI_location_area_1_t *location_area_1_local_var = ogs_malloc(sizeof(OpenAPI_location_area_1_t));
    ogs_assert(location_area_1_local_var);

    location_area_1_local_var->geographic_areas = geographic_areas;
    location_area_1_local_var->civic_addresses = civic_addresses;
    location_area_1_local_var->nw_area_info = nw_area_info;
    location_area_1_local_var->umt_time = umt_time;

    return location_area_1_local_var;
}

void OpenAPI_location_area_1_free(OpenAPI_location_area_1_t *location_area_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == location_area_1) {
        return;
    }
    if (location_area_1->geographic_areas) {
        OpenAPI_list_for_each(location_area_1->geographic_areas, node) {
            OpenAPI_geographic_area_free(node->data);
        }
        OpenAPI_list_free(location_area_1->geographic_areas);
        location_area_1->geographic_areas = NULL;
    }
    if (location_area_1->civic_addresses) {
        OpenAPI_list_for_each(location_area_1->civic_addresses, node) {
            OpenAPI_civic_address_free(node->data);
        }
        OpenAPI_list_free(location_area_1->civic_addresses);
        location_area_1->civic_addresses = NULL;
    }
    if (location_area_1->nw_area_info) {
        OpenAPI_network_area_info_1_free(location_area_1->nw_area_info);
        location_area_1->nw_area_info = NULL;
    }
    if (location_area_1->umt_time) {
        OpenAPI_umt_time_1_free(location_area_1->umt_time);
        location_area_1->umt_time = NULL;
    }
    ogs_free(location_area_1);
}

cJSON *OpenAPI_location_area_1_convertToJSON(OpenAPI_location_area_1_t *location_area_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_list_for_each(location_area_1->geographic_areas, node) {
        cJSON *itemLocal = OpenAPI_geographic_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_location_area_1_convertToJSON() failed [geographic_areas]");
            goto end;
        }
        cJSON_AddItemToArray(geographic_areasList, itemLocal);
    }
    }

    if (location_area_1->civic_addresses) {
    cJSON *civic_addressesList = cJSON_AddArrayToObject(item, "civicAddresses");
    if (civic_addressesList == NULL) {
        ogs_error("OpenAPI_location_area_1_convertToJSON() failed [civic_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(location_area_1->civic_addresses, node) {
        cJSON *itemLocal = OpenAPI_civic_address_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_location_area_1_convertToJSON() failed [civic_addresses]");
            goto end;
        }
        cJSON_AddItemToArray(civic_addressesList, itemLocal);
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

    if (location_area_1->umt_time) {
    cJSON *umt_time_local_JSON = OpenAPI_umt_time_1_convertToJSON(location_area_1->umt_time);
    if (umt_time_local_JSON == NULL) {
        ogs_error("OpenAPI_location_area_1_convertToJSON() failed [umt_time]");
        goto end;
    }
    cJSON_AddItemToObject(item, "umtTime", umt_time_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_area_1_convertToJSON() failed [umt_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_location_area_1_t *OpenAPI_location_area_1_parseFromJSON(cJSON *location_area_1JSON)
{
    OpenAPI_location_area_1_t *location_area_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *geographic_areas = NULL;
    OpenAPI_list_t *geographic_areasList = NULL;
    cJSON *civic_addresses = NULL;
    OpenAPI_list_t *civic_addressesList = NULL;
    cJSON *nw_area_info = NULL;
    OpenAPI_network_area_info_1_t *nw_area_info_local_nonprim = NULL;
    cJSON *umt_time = NULL;
    OpenAPI_umt_time_1_t *umt_time_local_nonprim = NULL;
    geographic_areas = cJSON_GetObjectItemCaseSensitive(location_area_1JSON, "geographicAreas");
    if (geographic_areas) {
        cJSON *geographic_areas_local = NULL;
        if (!cJSON_IsArray(geographic_areas)) {
            ogs_error("OpenAPI_location_area_1_parseFromJSON() failed [geographic_areas]");
            goto end;
        }

        geographic_areasList = OpenAPI_list_create();

        cJSON_ArrayForEach(geographic_areas_local, geographic_areas) {
            if (!cJSON_IsObject(geographic_areas_local)) {
                ogs_error("OpenAPI_location_area_1_parseFromJSON() failed [geographic_areas]");
                goto end;
            }
            OpenAPI_geographic_area_t *geographic_areasItem = OpenAPI_geographic_area_parseFromJSON(geographic_areas_local);
            if (!geographic_areasItem) {
                ogs_error("No geographic_areasItem");
                goto end;
            }
            OpenAPI_list_add(geographic_areasList, geographic_areasItem);
        }
    }

    civic_addresses = cJSON_GetObjectItemCaseSensitive(location_area_1JSON, "civicAddresses");
    if (civic_addresses) {
        cJSON *civic_addresses_local = NULL;
        if (!cJSON_IsArray(civic_addresses)) {
            ogs_error("OpenAPI_location_area_1_parseFromJSON() failed [civic_addresses]");
            goto end;
        }

        civic_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(civic_addresses_local, civic_addresses) {
            if (!cJSON_IsObject(civic_addresses_local)) {
                ogs_error("OpenAPI_location_area_1_parseFromJSON() failed [civic_addresses]");
                goto end;
            }
            OpenAPI_civic_address_t *civic_addressesItem = OpenAPI_civic_address_parseFromJSON(civic_addresses_local);
            if (!civic_addressesItem) {
                ogs_error("No civic_addressesItem");
                goto end;
            }
            OpenAPI_list_add(civic_addressesList, civic_addressesItem);
        }
    }

    nw_area_info = cJSON_GetObjectItemCaseSensitive(location_area_1JSON, "nwAreaInfo");
    if (nw_area_info) {
    nw_area_info_local_nonprim = OpenAPI_network_area_info_1_parseFromJSON(nw_area_info);
    if (!nw_area_info_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_1_parseFromJSON failed [nw_area_info]");
        goto end;
    }
    }

    umt_time = cJSON_GetObjectItemCaseSensitive(location_area_1JSON, "umtTime");
    if (umt_time) {
    umt_time_local_nonprim = OpenAPI_umt_time_1_parseFromJSON(umt_time);
    if (!umt_time_local_nonprim) {
        ogs_error("OpenAPI_umt_time_1_parseFromJSON failed [umt_time]");
        goto end;
    }
    }

    location_area_1_local_var = OpenAPI_location_area_1_create (
        geographic_areas ? geographic_areasList : NULL,
        civic_addresses ? civic_addressesList : NULL,
        nw_area_info ? nw_area_info_local_nonprim : NULL,
        umt_time ? umt_time_local_nonprim : NULL
    );

    return location_area_1_local_var;
end:
    if (geographic_areasList) {
        OpenAPI_list_for_each(geographic_areasList, node) {
            OpenAPI_geographic_area_free(node->data);
        }
        OpenAPI_list_free(geographic_areasList);
        geographic_areasList = NULL;
    }
    if (civic_addressesList) {
        OpenAPI_list_for_each(civic_addressesList, node) {
            OpenAPI_civic_address_free(node->data);
        }
        OpenAPI_list_free(civic_addressesList);
        civic_addressesList = NULL;
    }
    if (nw_area_info_local_nonprim) {
        OpenAPI_network_area_info_1_free(nw_area_info_local_nonprim);
        nw_area_info_local_nonprim = NULL;
    }
    if (umt_time_local_nonprim) {
        OpenAPI_umt_time_1_free(umt_time_local_nonprim);
        umt_time_local_nonprim = NULL;
    }
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

