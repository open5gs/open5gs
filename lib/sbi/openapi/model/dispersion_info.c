
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dispersion_info.h"

OpenAPI_dispersion_info_t *OpenAPI_dispersion_info_create(
    char *ts_start,
    int ts_duration,
    OpenAPI_list_t *disper_collects,
    OpenAPI_dispersion_type_t *disper_type
)
{
    OpenAPI_dispersion_info_t *dispersion_info_local_var = ogs_malloc(sizeof(OpenAPI_dispersion_info_t));
    ogs_assert(dispersion_info_local_var);

    dispersion_info_local_var->ts_start = ts_start;
    dispersion_info_local_var->ts_duration = ts_duration;
    dispersion_info_local_var->disper_collects = disper_collects;
    dispersion_info_local_var->disper_type = disper_type;

    return dispersion_info_local_var;
}

void OpenAPI_dispersion_info_free(OpenAPI_dispersion_info_t *dispersion_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dispersion_info) {
        return;
    }
    if (dispersion_info->ts_start) {
        ogs_free(dispersion_info->ts_start);
        dispersion_info->ts_start = NULL;
    }
    if (dispersion_info->disper_collects) {
        OpenAPI_list_for_each(dispersion_info->disper_collects, node) {
            OpenAPI_dispersion_collection_free(node->data);
        }
        OpenAPI_list_free(dispersion_info->disper_collects);
        dispersion_info->disper_collects = NULL;
    }
    if (dispersion_info->disper_type) {
        OpenAPI_dispersion_type_free(dispersion_info->disper_type);
        dispersion_info->disper_type = NULL;
    }
    ogs_free(dispersion_info);
}

cJSON *OpenAPI_dispersion_info_convertToJSON(OpenAPI_dispersion_info_t *dispersion_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dispersion_info == NULL) {
        ogs_error("OpenAPI_dispersion_info_convertToJSON() failed [DispersionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dispersion_info->ts_start) {
        ogs_error("OpenAPI_dispersion_info_convertToJSON() failed [ts_start]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "tsStart", dispersion_info->ts_start) == NULL) {
        ogs_error("OpenAPI_dispersion_info_convertToJSON() failed [ts_start]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "tsDuration", dispersion_info->ts_duration) == NULL) {
        ogs_error("OpenAPI_dispersion_info_convertToJSON() failed [ts_duration]");
        goto end;
    }

    if (!dispersion_info->disper_collects) {
        ogs_error("OpenAPI_dispersion_info_convertToJSON() failed [disper_collects]");
        return NULL;
    }
    cJSON *disper_collectsList = cJSON_AddArrayToObject(item, "disperCollects");
    if (disper_collectsList == NULL) {
        ogs_error("OpenAPI_dispersion_info_convertToJSON() failed [disper_collects]");
        goto end;
    }
    OpenAPI_list_for_each(dispersion_info->disper_collects, node) {
        cJSON *itemLocal = OpenAPI_dispersion_collection_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dispersion_info_convertToJSON() failed [disper_collects]");
            goto end;
        }
        cJSON_AddItemToArray(disper_collectsList, itemLocal);
    }

    if (!dispersion_info->disper_type) {
        ogs_error("OpenAPI_dispersion_info_convertToJSON() failed [disper_type]");
        return NULL;
    }
    cJSON *disper_type_local_JSON = OpenAPI_dispersion_type_convertToJSON(dispersion_info->disper_type);
    if (disper_type_local_JSON == NULL) {
        ogs_error("OpenAPI_dispersion_info_convertToJSON() failed [disper_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "disperType", disper_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_dispersion_info_convertToJSON() failed [disper_type]");
        goto end;
    }

end:
    return item;
}

OpenAPI_dispersion_info_t *OpenAPI_dispersion_info_parseFromJSON(cJSON *dispersion_infoJSON)
{
    OpenAPI_dispersion_info_t *dispersion_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ts_start = NULL;
    cJSON *ts_duration = NULL;
    cJSON *disper_collects = NULL;
    OpenAPI_list_t *disper_collectsList = NULL;
    cJSON *disper_type = NULL;
    OpenAPI_dispersion_type_t *disper_type_local_nonprim = NULL;
    ts_start = cJSON_GetObjectItemCaseSensitive(dispersion_infoJSON, "tsStart");
    if (!ts_start) {
        ogs_error("OpenAPI_dispersion_info_parseFromJSON() failed [ts_start]");
        goto end;
    }
    if (!cJSON_IsString(ts_start) && !cJSON_IsNull(ts_start)) {
        ogs_error("OpenAPI_dispersion_info_parseFromJSON() failed [ts_start]");
        goto end;
    }

    ts_duration = cJSON_GetObjectItemCaseSensitive(dispersion_infoJSON, "tsDuration");
    if (!ts_duration) {
        ogs_error("OpenAPI_dispersion_info_parseFromJSON() failed [ts_duration]");
        goto end;
    }
    if (!cJSON_IsNumber(ts_duration)) {
        ogs_error("OpenAPI_dispersion_info_parseFromJSON() failed [ts_duration]");
        goto end;
    }

    disper_collects = cJSON_GetObjectItemCaseSensitive(dispersion_infoJSON, "disperCollects");
    if (!disper_collects) {
        ogs_error("OpenAPI_dispersion_info_parseFromJSON() failed [disper_collects]");
        goto end;
    }
        cJSON *disper_collects_local = NULL;
        if (!cJSON_IsArray(disper_collects)) {
            ogs_error("OpenAPI_dispersion_info_parseFromJSON() failed [disper_collects]");
            goto end;
        }

        disper_collectsList = OpenAPI_list_create();

        cJSON_ArrayForEach(disper_collects_local, disper_collects) {
            if (!cJSON_IsObject(disper_collects_local)) {
                ogs_error("OpenAPI_dispersion_info_parseFromJSON() failed [disper_collects]");
                goto end;
            }
            OpenAPI_dispersion_collection_t *disper_collectsItem = OpenAPI_dispersion_collection_parseFromJSON(disper_collects_local);
            if (!disper_collectsItem) {
                ogs_error("No disper_collectsItem");
                goto end;
            }
            OpenAPI_list_add(disper_collectsList, disper_collectsItem);
        }

    disper_type = cJSON_GetObjectItemCaseSensitive(dispersion_infoJSON, "disperType");
    if (!disper_type) {
        ogs_error("OpenAPI_dispersion_info_parseFromJSON() failed [disper_type]");
        goto end;
    }
    disper_type_local_nonprim = OpenAPI_dispersion_type_parseFromJSON(disper_type);
    if (!disper_type_local_nonprim) {
        ogs_error("OpenAPI_dispersion_type_parseFromJSON failed [disper_type]");
        goto end;
    }

    dispersion_info_local_var = OpenAPI_dispersion_info_create (
        ogs_strdup(ts_start->valuestring),
        
        ts_duration->valuedouble,
        disper_collectsList,
        disper_type_local_nonprim
    );

    return dispersion_info_local_var;
end:
    if (disper_collectsList) {
        OpenAPI_list_for_each(disper_collectsList, node) {
            OpenAPI_dispersion_collection_free(node->data);
        }
        OpenAPI_list_free(disper_collectsList);
        disper_collectsList = NULL;
    }
    if (disper_type_local_nonprim) {
        OpenAPI_dispersion_type_free(disper_type_local_nonprim);
        disper_type_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_dispersion_info_t *OpenAPI_dispersion_info_copy(OpenAPI_dispersion_info_t *dst, OpenAPI_dispersion_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dispersion_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dispersion_info_convertToJSON() failed");
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

    OpenAPI_dispersion_info_free(dst);
    dst = OpenAPI_dispersion_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

