
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_ladn_service_areas.h"

OpenAPI_dnn_ladn_service_areas_t *OpenAPI_dnn_ladn_service_areas_create(
    OpenAPI_list_t *dnn_ladn_service_areas
)
{
    OpenAPI_dnn_ladn_service_areas_t *dnn_ladn_service_areas_local_var = ogs_malloc(sizeof(OpenAPI_dnn_ladn_service_areas_t));
    ogs_assert(dnn_ladn_service_areas_local_var);

    dnn_ladn_service_areas_local_var->dnn_ladn_service_areas = dnn_ladn_service_areas;

    return dnn_ladn_service_areas_local_var;
}

void OpenAPI_dnn_ladn_service_areas_free(OpenAPI_dnn_ladn_service_areas_t *dnn_ladn_service_areas)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnn_ladn_service_areas) {
        return;
    }
    if (dnn_ladn_service_areas->dnn_ladn_service_areas) {
        OpenAPI_list_for_each(dnn_ladn_service_areas->dnn_ladn_service_areas, node) {
            OpenAPI_dnn_ladn_service_area_free(node->data);
        }
        OpenAPI_list_free(dnn_ladn_service_areas->dnn_ladn_service_areas);
        dnn_ladn_service_areas->dnn_ladn_service_areas = NULL;
    }
    ogs_free(dnn_ladn_service_areas);
}

cJSON *OpenAPI_dnn_ladn_service_areas_convertToJSON(OpenAPI_dnn_ladn_service_areas_t *dnn_ladn_service_areas)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnn_ladn_service_areas == NULL) {
        ogs_error("OpenAPI_dnn_ladn_service_areas_convertToJSON() failed [DnnLadnServiceAreas]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnn_ladn_service_areas->dnn_ladn_service_areas) {
        ogs_error("OpenAPI_dnn_ladn_service_areas_convertToJSON() failed [dnn_ladn_service_areas]");
        return NULL;
    }
    cJSON *dnn_ladn_service_areasList = cJSON_AddArrayToObject(item, "dnnLadnServiceAreas");
    if (dnn_ladn_service_areasList == NULL) {
        ogs_error("OpenAPI_dnn_ladn_service_areas_convertToJSON() failed [dnn_ladn_service_areas]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_ladn_service_areas->dnn_ladn_service_areas, node) {
        cJSON *itemLocal = OpenAPI_dnn_ladn_service_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dnn_ladn_service_areas_convertToJSON() failed [dnn_ladn_service_areas]");
            goto end;
        }
        cJSON_AddItemToArray(dnn_ladn_service_areasList, itemLocal);
    }

end:
    return item;
}

OpenAPI_dnn_ladn_service_areas_t *OpenAPI_dnn_ladn_service_areas_parseFromJSON(cJSON *dnn_ladn_service_areasJSON)
{
    OpenAPI_dnn_ladn_service_areas_t *dnn_ladn_service_areas_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn_ladn_service_areas = NULL;
    OpenAPI_list_t *dnn_ladn_service_areasList = NULL;
    dnn_ladn_service_areas = cJSON_GetObjectItemCaseSensitive(dnn_ladn_service_areasJSON, "dnnLadnServiceAreas");
    if (!dnn_ladn_service_areas) {
        ogs_error("OpenAPI_dnn_ladn_service_areas_parseFromJSON() failed [dnn_ladn_service_areas]");
        goto end;
    }
        cJSON *dnn_ladn_service_areas_local = NULL;
        if (!cJSON_IsArray(dnn_ladn_service_areas)) {
            ogs_error("OpenAPI_dnn_ladn_service_areas_parseFromJSON() failed [dnn_ladn_service_areas]");
            goto end;
        }

        dnn_ladn_service_areasList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_ladn_service_areas_local, dnn_ladn_service_areas) {
            if (!cJSON_IsObject(dnn_ladn_service_areas_local)) {
                ogs_error("OpenAPI_dnn_ladn_service_areas_parseFromJSON() failed [dnn_ladn_service_areas]");
                goto end;
            }
            OpenAPI_dnn_ladn_service_area_t *dnn_ladn_service_areasItem = OpenAPI_dnn_ladn_service_area_parseFromJSON(dnn_ladn_service_areas_local);
            if (!dnn_ladn_service_areasItem) {
                ogs_error("No dnn_ladn_service_areasItem");
                goto end;
            }
            OpenAPI_list_add(dnn_ladn_service_areasList, dnn_ladn_service_areasItem);
        }

    dnn_ladn_service_areas_local_var = OpenAPI_dnn_ladn_service_areas_create (
        dnn_ladn_service_areasList
    );

    return dnn_ladn_service_areas_local_var;
end:
    if (dnn_ladn_service_areasList) {
        OpenAPI_list_for_each(dnn_ladn_service_areasList, node) {
            OpenAPI_dnn_ladn_service_area_free(node->data);
        }
        OpenAPI_list_free(dnn_ladn_service_areasList);
        dnn_ladn_service_areasList = NULL;
    }
    return NULL;
}

OpenAPI_dnn_ladn_service_areas_t *OpenAPI_dnn_ladn_service_areas_copy(OpenAPI_dnn_ladn_service_areas_t *dst, OpenAPI_dnn_ladn_service_areas_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_ladn_service_areas_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_ladn_service_areas_convertToJSON() failed");
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

    OpenAPI_dnn_ladn_service_areas_free(dst);
    dst = OpenAPI_dnn_ladn_service_areas_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

