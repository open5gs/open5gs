
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "geo_service_area.h"

OpenAPI_geo_service_area_t *OpenAPI_geo_service_area_create(
    OpenAPI_list_t *geographic_area_list,
    OpenAPI_list_t *civic_address_list
)
{
    OpenAPI_geo_service_area_t *geo_service_area_local_var = ogs_malloc(sizeof(OpenAPI_geo_service_area_t));
    ogs_assert(geo_service_area_local_var);

    geo_service_area_local_var->geographic_area_list = geographic_area_list;
    geo_service_area_local_var->civic_address_list = civic_address_list;

    return geo_service_area_local_var;
}

void OpenAPI_geo_service_area_free(OpenAPI_geo_service_area_t *geo_service_area)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == geo_service_area) {
        return;
    }
    if (geo_service_area->geographic_area_list) {
        OpenAPI_list_for_each(geo_service_area->geographic_area_list, node) {
            OpenAPI_geographic_area_free(node->data);
        }
        OpenAPI_list_free(geo_service_area->geographic_area_list);
        geo_service_area->geographic_area_list = NULL;
    }
    if (geo_service_area->civic_address_list) {
        OpenAPI_list_for_each(geo_service_area->civic_address_list, node) {
            OpenAPI_civic_address_free(node->data);
        }
        OpenAPI_list_free(geo_service_area->civic_address_list);
        geo_service_area->civic_address_list = NULL;
    }
    ogs_free(geo_service_area);
}

cJSON *OpenAPI_geo_service_area_convertToJSON(OpenAPI_geo_service_area_t *geo_service_area)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (geo_service_area == NULL) {
        ogs_error("OpenAPI_geo_service_area_convertToJSON() failed [GeoServiceArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (geo_service_area->geographic_area_list) {
    cJSON *geographic_area_listList = cJSON_AddArrayToObject(item, "geographicAreaList");
    if (geographic_area_listList == NULL) {
        ogs_error("OpenAPI_geo_service_area_convertToJSON() failed [geographic_area_list]");
        goto end;
    }
    OpenAPI_list_for_each(geo_service_area->geographic_area_list, node) {
        cJSON *itemLocal = OpenAPI_geographic_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_geo_service_area_convertToJSON() failed [geographic_area_list]");
            goto end;
        }
        cJSON_AddItemToArray(geographic_area_listList, itemLocal);
    }
    }

    if (geo_service_area->civic_address_list) {
    cJSON *civic_address_listList = cJSON_AddArrayToObject(item, "civicAddressList");
    if (civic_address_listList == NULL) {
        ogs_error("OpenAPI_geo_service_area_convertToJSON() failed [civic_address_list]");
        goto end;
    }
    OpenAPI_list_for_each(geo_service_area->civic_address_list, node) {
        cJSON *itemLocal = OpenAPI_civic_address_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_geo_service_area_convertToJSON() failed [civic_address_list]");
            goto end;
        }
        cJSON_AddItemToArray(civic_address_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_geo_service_area_t *OpenAPI_geo_service_area_parseFromJSON(cJSON *geo_service_areaJSON)
{
    OpenAPI_geo_service_area_t *geo_service_area_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *geographic_area_list = NULL;
    OpenAPI_list_t *geographic_area_listList = NULL;
    cJSON *civic_address_list = NULL;
    OpenAPI_list_t *civic_address_listList = NULL;
    geographic_area_list = cJSON_GetObjectItemCaseSensitive(geo_service_areaJSON, "geographicAreaList");
    if (geographic_area_list) {
        cJSON *geographic_area_list_local = NULL;
        if (!cJSON_IsArray(geographic_area_list)) {
            ogs_error("OpenAPI_geo_service_area_parseFromJSON() failed [geographic_area_list]");
            goto end;
        }

        geographic_area_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(geographic_area_list_local, geographic_area_list) {
            if (!cJSON_IsObject(geographic_area_list_local)) {
                ogs_error("OpenAPI_geo_service_area_parseFromJSON() failed [geographic_area_list]");
                goto end;
            }
            OpenAPI_geographic_area_t *geographic_area_listItem = OpenAPI_geographic_area_parseFromJSON(geographic_area_list_local);
            if (!geographic_area_listItem) {
                ogs_error("No geographic_area_listItem");
                goto end;
            }
            OpenAPI_list_add(geographic_area_listList, geographic_area_listItem);
        }
    }

    civic_address_list = cJSON_GetObjectItemCaseSensitive(geo_service_areaJSON, "civicAddressList");
    if (civic_address_list) {
        cJSON *civic_address_list_local = NULL;
        if (!cJSON_IsArray(civic_address_list)) {
            ogs_error("OpenAPI_geo_service_area_parseFromJSON() failed [civic_address_list]");
            goto end;
        }

        civic_address_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(civic_address_list_local, civic_address_list) {
            if (!cJSON_IsObject(civic_address_list_local)) {
                ogs_error("OpenAPI_geo_service_area_parseFromJSON() failed [civic_address_list]");
                goto end;
            }
            OpenAPI_civic_address_t *civic_address_listItem = OpenAPI_civic_address_parseFromJSON(civic_address_list_local);
            if (!civic_address_listItem) {
                ogs_error("No civic_address_listItem");
                goto end;
            }
            OpenAPI_list_add(civic_address_listList, civic_address_listItem);
        }
    }

    geo_service_area_local_var = OpenAPI_geo_service_area_create (
        geographic_area_list ? geographic_area_listList : NULL,
        civic_address_list ? civic_address_listList : NULL
    );

    return geo_service_area_local_var;
end:
    if (geographic_area_listList) {
        OpenAPI_list_for_each(geographic_area_listList, node) {
            OpenAPI_geographic_area_free(node->data);
        }
        OpenAPI_list_free(geographic_area_listList);
        geographic_area_listList = NULL;
    }
    if (civic_address_listList) {
        OpenAPI_list_for_each(civic_address_listList, node) {
            OpenAPI_civic_address_free(node->data);
        }
        OpenAPI_list_free(civic_address_listList);
        civic_address_listList = NULL;
    }
    return NULL;
}

OpenAPI_geo_service_area_t *OpenAPI_geo_service_area_copy(OpenAPI_geo_service_area_t *dst, OpenAPI_geo_service_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_geo_service_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_geo_service_area_convertToJSON() failed");
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

    OpenAPI_geo_service_area_free(dst);
    dst = OpenAPI_geo_service_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

