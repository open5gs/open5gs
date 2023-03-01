
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "time_window.h"

OpenAPI_time_window_t *OpenAPI_time_window_create(
    char *start_time,
    char *stop_time
)
{
    OpenAPI_time_window_t *time_window_local_var = ogs_malloc(sizeof(OpenAPI_time_window_t));
    ogs_assert(time_window_local_var);

    time_window_local_var->start_time = start_time;
    time_window_local_var->stop_time = stop_time;

    return time_window_local_var;
}

void OpenAPI_time_window_free(OpenAPI_time_window_t *time_window)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == time_window) {
        return;
    }
    if (time_window->start_time) {
        ogs_free(time_window->start_time);
        time_window->start_time = NULL;
    }
    if (time_window->stop_time) {
        ogs_free(time_window->stop_time);
        time_window->stop_time = NULL;
    }
    ogs_free(time_window);
}

cJSON *OpenAPI_time_window_convertToJSON(OpenAPI_time_window_t *time_window)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (time_window == NULL) {
        ogs_error("OpenAPI_time_window_convertToJSON() failed [TimeWindow]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!time_window->start_time) {
        ogs_error("OpenAPI_time_window_convertToJSON() failed [start_time]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "startTime", time_window->start_time) == NULL) {
        ogs_error("OpenAPI_time_window_convertToJSON() failed [start_time]");
        goto end;
    }

    if (!time_window->stop_time) {
        ogs_error("OpenAPI_time_window_convertToJSON() failed [stop_time]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "stopTime", time_window->stop_time) == NULL) {
        ogs_error("OpenAPI_time_window_convertToJSON() failed [stop_time]");
        goto end;
    }

end:
    return item;
}

OpenAPI_time_window_t *OpenAPI_time_window_parseFromJSON(cJSON *time_windowJSON)
{
    OpenAPI_time_window_t *time_window_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *start_time = NULL;
    cJSON *stop_time = NULL;
    start_time = cJSON_GetObjectItemCaseSensitive(time_windowJSON, "startTime");
    if (!start_time) {
        ogs_error("OpenAPI_time_window_parseFromJSON() failed [start_time]");
        goto end;
    }
    if (!cJSON_IsString(start_time) && !cJSON_IsNull(start_time)) {
        ogs_error("OpenAPI_time_window_parseFromJSON() failed [start_time]");
        goto end;
    }

    stop_time = cJSON_GetObjectItemCaseSensitive(time_windowJSON, "stopTime");
    if (!stop_time) {
        ogs_error("OpenAPI_time_window_parseFromJSON() failed [stop_time]");
        goto end;
    }
    if (!cJSON_IsString(stop_time) && !cJSON_IsNull(stop_time)) {
        ogs_error("OpenAPI_time_window_parseFromJSON() failed [stop_time]");
        goto end;
    }

    time_window_local_var = OpenAPI_time_window_create (
        ogs_strdup(start_time->valuestring),
        ogs_strdup(stop_time->valuestring)
    );

    return time_window_local_var;
end:
    return NULL;
}

OpenAPI_time_window_t *OpenAPI_time_window_copy(OpenAPI_time_window_t *dst, OpenAPI_time_window_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_time_window_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_time_window_convertToJSON() failed");
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

    OpenAPI_time_window_free(dst);
    dst = OpenAPI_time_window_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

