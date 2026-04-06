
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbsr_time_info.h"

OpenAPI_mbsr_time_info_t *OpenAPI_mbsr_time_info_create(
    OpenAPI_list_t *mbsr_time_window,
    OpenAPI_list_t *mbsr_recur_time
)
{
    OpenAPI_mbsr_time_info_t *mbsr_time_info_local_var = ogs_malloc(sizeof(OpenAPI_mbsr_time_info_t));
    ogs_assert(mbsr_time_info_local_var);

    mbsr_time_info_local_var->mbsr_time_window = mbsr_time_window;
    mbsr_time_info_local_var->mbsr_recur_time = mbsr_recur_time;

    return mbsr_time_info_local_var;
}

void OpenAPI_mbsr_time_info_free(OpenAPI_mbsr_time_info_t *mbsr_time_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mbsr_time_info) {
        return;
    }
    if (mbsr_time_info->mbsr_time_window) {
        OpenAPI_list_for_each(mbsr_time_info->mbsr_time_window, node) {
            OpenAPI_valid_time_period_free(node->data);
        }
        OpenAPI_list_free(mbsr_time_info->mbsr_time_window);
        mbsr_time_info->mbsr_time_window = NULL;
    }
    if (mbsr_time_info->mbsr_recur_time) {
        OpenAPI_list_for_each(mbsr_time_info->mbsr_recur_time, node) {
            OpenAPI_recur_time_free(node->data);
        }
        OpenAPI_list_free(mbsr_time_info->mbsr_recur_time);
        mbsr_time_info->mbsr_recur_time = NULL;
    }
    ogs_free(mbsr_time_info);
}

cJSON *OpenAPI_mbsr_time_info_convertToJSON(OpenAPI_mbsr_time_info_t *mbsr_time_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mbsr_time_info == NULL) {
        ogs_error("OpenAPI_mbsr_time_info_convertToJSON() failed [MbsrTimeInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mbsr_time_info->mbsr_time_window) {
    cJSON *mbsr_time_windowList = cJSON_AddArrayToObject(item, "mbsrTimeWindow");
    if (mbsr_time_windowList == NULL) {
        ogs_error("OpenAPI_mbsr_time_info_convertToJSON() failed [mbsr_time_window]");
        goto end;
    }
    OpenAPI_list_for_each(mbsr_time_info->mbsr_time_window, node) {
        cJSON *itemLocal = OpenAPI_valid_time_period_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mbsr_time_info_convertToJSON() failed [mbsr_time_window]");
            goto end;
        }
        cJSON_AddItemToArray(mbsr_time_windowList, itemLocal);
    }
    }

    if (mbsr_time_info->mbsr_recur_time) {
    cJSON *mbsr_recur_timeList = cJSON_AddArrayToObject(item, "mbsrRecurTime");
    if (mbsr_recur_timeList == NULL) {
        ogs_error("OpenAPI_mbsr_time_info_convertToJSON() failed [mbsr_recur_time]");
        goto end;
    }
    OpenAPI_list_for_each(mbsr_time_info->mbsr_recur_time, node) {
        cJSON *itemLocal = OpenAPI_recur_time_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mbsr_time_info_convertToJSON() failed [mbsr_recur_time]");
            goto end;
        }
        cJSON_AddItemToArray(mbsr_recur_timeList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_mbsr_time_info_t *OpenAPI_mbsr_time_info_parseFromJSON(cJSON *mbsr_time_infoJSON)
{
    OpenAPI_mbsr_time_info_t *mbsr_time_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mbsr_time_window = NULL;
    OpenAPI_list_t *mbsr_time_windowList = NULL;
    cJSON *mbsr_recur_time = NULL;
    OpenAPI_list_t *mbsr_recur_timeList = NULL;
    mbsr_time_window = cJSON_GetObjectItemCaseSensitive(mbsr_time_infoJSON, "mbsrTimeWindow");
    if (mbsr_time_window) {
        cJSON *mbsr_time_window_local = NULL;
        if (!cJSON_IsArray(mbsr_time_window)) {
            ogs_error("OpenAPI_mbsr_time_info_parseFromJSON() failed [mbsr_time_window]");
            goto end;
        }

        mbsr_time_windowList = OpenAPI_list_create();

        cJSON_ArrayForEach(mbsr_time_window_local, mbsr_time_window) {
            if (!cJSON_IsObject(mbsr_time_window_local)) {
                ogs_error("OpenAPI_mbsr_time_info_parseFromJSON() failed [mbsr_time_window]");
                goto end;
            }
            OpenAPI_valid_time_period_t *mbsr_time_windowItem = OpenAPI_valid_time_period_parseFromJSON(mbsr_time_window_local);
            if (!mbsr_time_windowItem) {
                ogs_error("No mbsr_time_windowItem");
                goto end;
            }
            OpenAPI_list_add(mbsr_time_windowList, mbsr_time_windowItem);
        }
    }

    mbsr_recur_time = cJSON_GetObjectItemCaseSensitive(mbsr_time_infoJSON, "mbsrRecurTime");
    if (mbsr_recur_time) {
        cJSON *mbsr_recur_time_local = NULL;
        if (!cJSON_IsArray(mbsr_recur_time)) {
            ogs_error("OpenAPI_mbsr_time_info_parseFromJSON() failed [mbsr_recur_time]");
            goto end;
        }

        mbsr_recur_timeList = OpenAPI_list_create();

        cJSON_ArrayForEach(mbsr_recur_time_local, mbsr_recur_time) {
            if (!cJSON_IsObject(mbsr_recur_time_local)) {
                ogs_error("OpenAPI_mbsr_time_info_parseFromJSON() failed [mbsr_recur_time]");
                goto end;
            }
            OpenAPI_recur_time_t *mbsr_recur_timeItem = OpenAPI_recur_time_parseFromJSON(mbsr_recur_time_local);
            if (!mbsr_recur_timeItem) {
                ogs_error("No mbsr_recur_timeItem");
                goto end;
            }
            OpenAPI_list_add(mbsr_recur_timeList, mbsr_recur_timeItem);
        }
    }

    mbsr_time_info_local_var = OpenAPI_mbsr_time_info_create (
        mbsr_time_window ? mbsr_time_windowList : NULL,
        mbsr_recur_time ? mbsr_recur_timeList : NULL
    );

    return mbsr_time_info_local_var;
end:
    if (mbsr_time_windowList) {
        OpenAPI_list_for_each(mbsr_time_windowList, node) {
            OpenAPI_valid_time_period_free(node->data);
        }
        OpenAPI_list_free(mbsr_time_windowList);
        mbsr_time_windowList = NULL;
    }
    if (mbsr_recur_timeList) {
        OpenAPI_list_for_each(mbsr_recur_timeList, node) {
            OpenAPI_recur_time_free(node->data);
        }
        OpenAPI_list_free(mbsr_recur_timeList);
        mbsr_recur_timeList = NULL;
    }
    return NULL;
}

OpenAPI_mbsr_time_info_t *OpenAPI_mbsr_time_info_copy(OpenAPI_mbsr_time_info_t *dst, OpenAPI_mbsr_time_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbsr_time_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbsr_time_info_convertToJSON() failed");
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

    OpenAPI_mbsr_time_info_free(dst);
    dst = OpenAPI_mbsr_time_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

