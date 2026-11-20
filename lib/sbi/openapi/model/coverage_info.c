
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "coverage_info.h"

OpenAPI_coverage_info_t *OpenAPI_coverage_info_create(
    bool is_coverage_status,
    int coverage_status,
    char *change_time,
    OpenAPI_list_t *location_info
)
{
    OpenAPI_coverage_info_t *coverage_info_local_var = ogs_malloc(sizeof(OpenAPI_coverage_info_t));
    ogs_assert(coverage_info_local_var);

    coverage_info_local_var->is_coverage_status = is_coverage_status;
    coverage_info_local_var->coverage_status = coverage_status;
    coverage_info_local_var->change_time = change_time;
    coverage_info_local_var->location_info = location_info;

    return coverage_info_local_var;
}

void OpenAPI_coverage_info_free(OpenAPI_coverage_info_t *coverage_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == coverage_info) {
        return;
    }
    if (coverage_info->change_time) {
        ogs_free(coverage_info->change_time);
        coverage_info->change_time = NULL;
    }
    if (coverage_info->location_info) {
        OpenAPI_list_for_each(coverage_info->location_info, node) {
            OpenAPI_charging_user_location_free(node->data);
        }
        OpenAPI_list_free(coverage_info->location_info);
        coverage_info->location_info = NULL;
    }
    ogs_free(coverage_info);
}

cJSON *OpenAPI_coverage_info_convertToJSON(OpenAPI_coverage_info_t *coverage_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (coverage_info == NULL) {
        ogs_error("OpenAPI_coverage_info_convertToJSON() failed [CoverageInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (coverage_info->is_coverage_status) {
    if (cJSON_AddBoolToObject(item, "coverageStatus", coverage_info->coverage_status) == NULL) {
        ogs_error("OpenAPI_coverage_info_convertToJSON() failed [coverage_status]");
        goto end;
    }
    }

    if (coverage_info->change_time) {
    if (cJSON_AddStringToObject(item, "changeTime", coverage_info->change_time) == NULL) {
        ogs_error("OpenAPI_coverage_info_convertToJSON() failed [change_time]");
        goto end;
    }
    }

    if (coverage_info->location_info) {
    cJSON *location_infoList = cJSON_AddArrayToObject(item, "locationInfo");
    if (location_infoList == NULL) {
        ogs_error("OpenAPI_coverage_info_convertToJSON() failed [location_info]");
        goto end;
    }
    OpenAPI_list_for_each(coverage_info->location_info, node) {
        cJSON *itemLocal = OpenAPI_charging_user_location_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_coverage_info_convertToJSON() failed [location_info]");
            goto end;
        }
        cJSON_AddItemToArray(location_infoList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_coverage_info_t *OpenAPI_coverage_info_parseFromJSON(cJSON *coverage_infoJSON)
{
    OpenAPI_coverage_info_t *coverage_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *coverage_status = NULL;
    cJSON *change_time = NULL;
    cJSON *location_info = NULL;
    OpenAPI_list_t *location_infoList = NULL;
    coverage_status = cJSON_GetObjectItemCaseSensitive(coverage_infoJSON, "coverageStatus");
    if (coverage_status) {
    if (!cJSON_IsBool(coverage_status)) {
        ogs_error("OpenAPI_coverage_info_parseFromJSON() failed [coverage_status]");
        goto end;
    }
    }

    change_time = cJSON_GetObjectItemCaseSensitive(coverage_infoJSON, "changeTime");
    if (change_time) {
    if (!cJSON_IsString(change_time) && !cJSON_IsNull(change_time)) {
        ogs_error("OpenAPI_coverage_info_parseFromJSON() failed [change_time]");
        goto end;
    }
    }

    location_info = cJSON_GetObjectItemCaseSensitive(coverage_infoJSON, "locationInfo");
    if (location_info) {
        cJSON *location_info_local = NULL;
        if (!cJSON_IsArray(location_info)) {
            ogs_error("OpenAPI_coverage_info_parseFromJSON() failed [location_info]");
            goto end;
        }

        location_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(location_info_local, location_info) {
            if (!cJSON_IsObject(location_info_local)) {
                ogs_error("OpenAPI_coverage_info_parseFromJSON() failed [location_info]");
                goto end;
            }
            OpenAPI_charging_user_location_t *location_infoItem = OpenAPI_charging_user_location_parseFromJSON(location_info_local);
            if (!location_infoItem) {
                ogs_error("No location_infoItem");
                goto end;
            }
            OpenAPI_list_add(location_infoList, location_infoItem);
        }
    }

    coverage_info_local_var = OpenAPI_coverage_info_create (
        coverage_status ? true : false,
        coverage_status ? coverage_status->valueint : 0,
        change_time && !cJSON_IsNull(change_time) ? ogs_strdup(change_time->valuestring) : NULL,
        location_info ? location_infoList : NULL
    );

    return coverage_info_local_var;
end:
    if (location_infoList) {
        OpenAPI_list_for_each(location_infoList, node) {
            OpenAPI_charging_user_location_free(node->data);
        }
        OpenAPI_list_free(location_infoList);
        location_infoList = NULL;
    }
    return NULL;
}

OpenAPI_coverage_info_t *OpenAPI_coverage_info_copy(OpenAPI_coverage_info_t *dst, OpenAPI_coverage_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_coverage_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_coverage_info_convertToJSON() failed");
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

    OpenAPI_coverage_info_free(dst);
    dst = OpenAPI_coverage_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

