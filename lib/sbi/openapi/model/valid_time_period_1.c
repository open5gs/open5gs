
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "valid_time_period_1.h"

OpenAPI_valid_time_period_1_t *OpenAPI_valid_time_period_1_create(
    char *start_time,
    char *end_time
)
{
    OpenAPI_valid_time_period_1_t *valid_time_period_1_local_var = ogs_malloc(sizeof(OpenAPI_valid_time_period_1_t));
    ogs_assert(valid_time_period_1_local_var);

    valid_time_period_1_local_var->start_time = start_time;
    valid_time_period_1_local_var->end_time = end_time;

    return valid_time_period_1_local_var;
}

void OpenAPI_valid_time_period_1_free(OpenAPI_valid_time_period_1_t *valid_time_period_1)
{
    if (NULL == valid_time_period_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(valid_time_period_1->start_time);
    ogs_free(valid_time_period_1->end_time);
    ogs_free(valid_time_period_1);
}

cJSON *OpenAPI_valid_time_period_1_convertToJSON(OpenAPI_valid_time_period_1_t *valid_time_period_1)
{
    cJSON *item = NULL;

    if (valid_time_period_1 == NULL) {
        ogs_error("OpenAPI_valid_time_period_1_convertToJSON() failed [ValidTimePeriod_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (valid_time_period_1->start_time) {
    if (cJSON_AddStringToObject(item, "startTime", valid_time_period_1->start_time) == NULL) {
        ogs_error("OpenAPI_valid_time_period_1_convertToJSON() failed [start_time]");
        goto end;
    }
    }

    if (valid_time_period_1->end_time) {
    if (cJSON_AddStringToObject(item, "endTime", valid_time_period_1->end_time) == NULL) {
        ogs_error("OpenAPI_valid_time_period_1_convertToJSON() failed [end_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_valid_time_period_1_t *OpenAPI_valid_time_period_1_parseFromJSON(cJSON *valid_time_period_1JSON)
{
    OpenAPI_valid_time_period_1_t *valid_time_period_1_local_var = NULL;
    cJSON *start_time = cJSON_GetObjectItemCaseSensitive(valid_time_period_1JSON, "startTime");

    if (start_time) {
    if (!cJSON_IsString(start_time)) {
        ogs_error("OpenAPI_valid_time_period_1_parseFromJSON() failed [start_time]");
        goto end;
    }
    }

    cJSON *end_time = cJSON_GetObjectItemCaseSensitive(valid_time_period_1JSON, "endTime");

    if (end_time) {
    if (!cJSON_IsString(end_time)) {
        ogs_error("OpenAPI_valid_time_period_1_parseFromJSON() failed [end_time]");
        goto end;
    }
    }

    valid_time_period_1_local_var = OpenAPI_valid_time_period_1_create (
        start_time ? ogs_strdup(start_time->valuestring) : NULL,
        end_time ? ogs_strdup(end_time->valuestring) : NULL
    );

    return valid_time_period_1_local_var;
end:
    return NULL;
}

OpenAPI_valid_time_period_1_t *OpenAPI_valid_time_period_1_copy(OpenAPI_valid_time_period_1_t *dst, OpenAPI_valid_time_period_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_valid_time_period_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_valid_time_period_1_convertToJSON() failed");
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

    OpenAPI_valid_time_period_1_free(dst);
    dst = OpenAPI_valid_time_period_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

