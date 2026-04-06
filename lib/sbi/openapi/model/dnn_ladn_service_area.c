
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_ladn_service_area.h"

OpenAPI_dnn_ladn_service_area_t *OpenAPI_dnn_ladn_service_area_create(
    char *dnn,
    OpenAPI_list_t *ladn_service_area
)
{
    OpenAPI_dnn_ladn_service_area_t *dnn_ladn_service_area_local_var = ogs_malloc(sizeof(OpenAPI_dnn_ladn_service_area_t));
    ogs_assert(dnn_ladn_service_area_local_var);

    dnn_ladn_service_area_local_var->dnn = dnn;
    dnn_ladn_service_area_local_var->ladn_service_area = ladn_service_area;

    return dnn_ladn_service_area_local_var;
}

void OpenAPI_dnn_ladn_service_area_free(OpenAPI_dnn_ladn_service_area_t *dnn_ladn_service_area)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnn_ladn_service_area) {
        return;
    }
    if (dnn_ladn_service_area->dnn) {
        ogs_free(dnn_ladn_service_area->dnn);
        dnn_ladn_service_area->dnn = NULL;
    }
    if (dnn_ladn_service_area->ladn_service_area) {
        OpenAPI_list_for_each(dnn_ladn_service_area->ladn_service_area, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(dnn_ladn_service_area->ladn_service_area);
        dnn_ladn_service_area->ladn_service_area = NULL;
    }
    ogs_free(dnn_ladn_service_area);
}

cJSON *OpenAPI_dnn_ladn_service_area_convertToJSON(OpenAPI_dnn_ladn_service_area_t *dnn_ladn_service_area)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnn_ladn_service_area == NULL) {
        ogs_error("OpenAPI_dnn_ladn_service_area_convertToJSON() failed [DnnLadnServiceArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnn_ladn_service_area->dnn) {
        ogs_error("OpenAPI_dnn_ladn_service_area_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", dnn_ladn_service_area->dnn) == NULL) {
        ogs_error("OpenAPI_dnn_ladn_service_area_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!dnn_ladn_service_area->ladn_service_area) {
        ogs_error("OpenAPI_dnn_ladn_service_area_convertToJSON() failed [ladn_service_area]");
        return NULL;
    }
    cJSON *ladn_service_areaList = cJSON_AddArrayToObject(item, "ladnServiceArea");
    if (ladn_service_areaList == NULL) {
        ogs_error("OpenAPI_dnn_ladn_service_area_convertToJSON() failed [ladn_service_area]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_ladn_service_area->ladn_service_area, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_ladn_service_area_convertToJSON() failed [ladn_service_area]");
            goto end;
        }
        cJSON_AddItemToArray(ladn_service_areaList, itemLocal);
    }

end:
    return item;
}

OpenAPI_dnn_ladn_service_area_t *OpenAPI_dnn_ladn_service_area_parseFromJSON(cJSON *dnn_ladn_service_areaJSON)
{
    OpenAPI_dnn_ladn_service_area_t *dnn_ladn_service_area_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *ladn_service_area = NULL;
    OpenAPI_list_t *ladn_service_areaList = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(dnn_ladn_service_areaJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_dnn_ladn_service_area_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_dnn_ladn_service_area_parseFromJSON() failed [dnn]");
        goto end;
    }

    ladn_service_area = cJSON_GetObjectItemCaseSensitive(dnn_ladn_service_areaJSON, "ladnServiceArea");
    if (!ladn_service_area) {
        ogs_error("OpenAPI_dnn_ladn_service_area_parseFromJSON() failed [ladn_service_area]");
        goto end;
    }
        cJSON *ladn_service_area_local = NULL;
        if (!cJSON_IsArray(ladn_service_area)) {
            ogs_error("OpenAPI_dnn_ladn_service_area_parseFromJSON() failed [ladn_service_area]");
            goto end;
        }

        ladn_service_areaList = OpenAPI_list_create();

        cJSON_ArrayForEach(ladn_service_area_local, ladn_service_area) {
            if (!cJSON_IsObject(ladn_service_area_local)) {
                ogs_error("OpenAPI_dnn_ladn_service_area_parseFromJSON() failed [ladn_service_area]");
                goto end;
            }
            OpenAPI_tai_t *ladn_service_areaItem = OpenAPI_tai_parseFromJSON(ladn_service_area_local);
            if (!ladn_service_areaItem) {
                ogs_error("No ladn_service_areaItem");
                goto end;
            }
            OpenAPI_list_add(ladn_service_areaList, ladn_service_areaItem);
        }

    dnn_ladn_service_area_local_var = OpenAPI_dnn_ladn_service_area_create (
        ogs_strdup(dnn->valuestring),
        ladn_service_areaList
    );

    return dnn_ladn_service_area_local_var;
end:
    if (ladn_service_areaList) {
        OpenAPI_list_for_each(ladn_service_areaList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(ladn_service_areaList);
        ladn_service_areaList = NULL;
    }
    return NULL;
}

OpenAPI_dnn_ladn_service_area_t *OpenAPI_dnn_ladn_service_area_copy(OpenAPI_dnn_ladn_service_area_t *dst, OpenAPI_dnn_ladn_service_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_ladn_service_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_ladn_service_area_convertToJSON() failed");
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

    OpenAPI_dnn_ladn_service_area_free(dst);
    dst = OpenAPI_dnn_ladn_service_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

