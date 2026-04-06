
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "aiot_area.h"

OpenAPI_aiot_area_t *OpenAPI_aiot_area_create(
    OpenAPI_list_t *area_ids
)
{
    OpenAPI_aiot_area_t *aiot_area_local_var = ogs_malloc(sizeof(OpenAPI_aiot_area_t));
    ogs_assert(aiot_area_local_var);

    aiot_area_local_var->area_ids = area_ids;

    return aiot_area_local_var;
}

void OpenAPI_aiot_area_free(OpenAPI_aiot_area_t *aiot_area)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == aiot_area) {
        return;
    }
    if (aiot_area->area_ids) {
        OpenAPI_list_for_each(aiot_area->area_ids, node) {
            OpenAPI_aiot_area_id_free(node->data);
        }
        OpenAPI_list_free(aiot_area->area_ids);
        aiot_area->area_ids = NULL;
    }
    ogs_free(aiot_area);
}

cJSON *OpenAPI_aiot_area_convertToJSON(OpenAPI_aiot_area_t *aiot_area)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (aiot_area == NULL) {
        ogs_error("OpenAPI_aiot_area_convertToJSON() failed [AiotArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!aiot_area->area_ids) {
        ogs_error("OpenAPI_aiot_area_convertToJSON() failed [area_ids]");
        return NULL;
    }
    cJSON *area_idsList = cJSON_AddArrayToObject(item, "areaIds");
    if (area_idsList == NULL) {
        ogs_error("OpenAPI_aiot_area_convertToJSON() failed [area_ids]");
        goto end;
    }
    OpenAPI_list_for_each(aiot_area->area_ids, node) {
        cJSON *itemLocal = OpenAPI_aiot_area_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_aiot_area_convertToJSON() failed [area_ids]");
            goto end;
        }
        cJSON_AddItemToArray(area_idsList, itemLocal);
    }

end:
    return item;
}

OpenAPI_aiot_area_t *OpenAPI_aiot_area_parseFromJSON(cJSON *aiot_areaJSON)
{
    OpenAPI_aiot_area_t *aiot_area_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *area_ids = NULL;
    OpenAPI_list_t *area_idsList = NULL;
    area_ids = cJSON_GetObjectItemCaseSensitive(aiot_areaJSON, "areaIds");
    if (!area_ids) {
        ogs_error("OpenAPI_aiot_area_parseFromJSON() failed [area_ids]");
        goto end;
    }
        cJSON *area_ids_local = NULL;
        if (!cJSON_IsArray(area_ids)) {
            ogs_error("OpenAPI_aiot_area_parseFromJSON() failed [area_ids]");
            goto end;
        }

        area_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(area_ids_local, area_ids) {
            if (!cJSON_IsObject(area_ids_local)) {
                ogs_error("OpenAPI_aiot_area_parseFromJSON() failed [area_ids]");
                goto end;
            }
            OpenAPI_aiot_area_id_t *area_idsItem = OpenAPI_aiot_area_id_parseFromJSON(area_ids_local);
            if (!area_idsItem) {
                ogs_error("No area_idsItem");
                goto end;
            }
            OpenAPI_list_add(area_idsList, area_idsItem);
        }

    aiot_area_local_var = OpenAPI_aiot_area_create (
        area_idsList
    );

    return aiot_area_local_var;
end:
    if (area_idsList) {
        OpenAPI_list_for_each(area_idsList, node) {
            OpenAPI_aiot_area_id_free(node->data);
        }
        OpenAPI_list_free(area_idsList);
        area_idsList = NULL;
    }
    return NULL;
}

OpenAPI_aiot_area_t *OpenAPI_aiot_area_copy(OpenAPI_aiot_area_t *dst, OpenAPI_aiot_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_aiot_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_aiot_area_convertToJSON() failed");
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

    OpenAPI_aiot_area_free(dst);
    dst = OpenAPI_aiot_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

