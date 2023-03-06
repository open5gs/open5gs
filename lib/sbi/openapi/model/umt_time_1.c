
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "umt_time_1.h"

OpenAPI_umt_time_1_t *OpenAPI_umt_time_1_create(
    char *time_of_day,
    int day_of_week
)
{
    OpenAPI_umt_time_1_t *umt_time_1_local_var = ogs_malloc(sizeof(OpenAPI_umt_time_1_t));
    ogs_assert(umt_time_1_local_var);

    umt_time_1_local_var->time_of_day = time_of_day;
    umt_time_1_local_var->day_of_week = day_of_week;

    return umt_time_1_local_var;
}

void OpenAPI_umt_time_1_free(OpenAPI_umt_time_1_t *umt_time_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == umt_time_1) {
        return;
    }
    if (umt_time_1->time_of_day) {
        ogs_free(umt_time_1->time_of_day);
        umt_time_1->time_of_day = NULL;
    }
    ogs_free(umt_time_1);
}

cJSON *OpenAPI_umt_time_1_convertToJSON(OpenAPI_umt_time_1_t *umt_time_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (umt_time_1 == NULL) {
        ogs_error("OpenAPI_umt_time_1_convertToJSON() failed [UmtTime_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!umt_time_1->time_of_day) {
        ogs_error("OpenAPI_umt_time_1_convertToJSON() failed [time_of_day]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "timeOfDay", umt_time_1->time_of_day) == NULL) {
        ogs_error("OpenAPI_umt_time_1_convertToJSON() failed [time_of_day]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "dayOfWeek", umt_time_1->day_of_week) == NULL) {
        ogs_error("OpenAPI_umt_time_1_convertToJSON() failed [day_of_week]");
        goto end;
    }

end:
    return item;
}

OpenAPI_umt_time_1_t *OpenAPI_umt_time_1_parseFromJSON(cJSON *umt_time_1JSON)
{
    OpenAPI_umt_time_1_t *umt_time_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *time_of_day = NULL;
    cJSON *day_of_week = NULL;
    time_of_day = cJSON_GetObjectItemCaseSensitive(umt_time_1JSON, "timeOfDay");
    if (!time_of_day) {
        ogs_error("OpenAPI_umt_time_1_parseFromJSON() failed [time_of_day]");
        goto end;
    }
    if (!cJSON_IsString(time_of_day)) {
        ogs_error("OpenAPI_umt_time_1_parseFromJSON() failed [time_of_day]");
        goto end;
    }

    day_of_week = cJSON_GetObjectItemCaseSensitive(umt_time_1JSON, "dayOfWeek");
    if (!day_of_week) {
        ogs_error("OpenAPI_umt_time_1_parseFromJSON() failed [day_of_week]");
        goto end;
    }
    if (!cJSON_IsNumber(day_of_week)) {
        ogs_error("OpenAPI_umt_time_1_parseFromJSON() failed [day_of_week]");
        goto end;
    }

    umt_time_1_local_var = OpenAPI_umt_time_1_create (
        ogs_strdup(time_of_day->valuestring),
        
        day_of_week->valuedouble
    );

    return umt_time_1_local_var;
end:
    return NULL;
}

OpenAPI_umt_time_1_t *OpenAPI_umt_time_1_copy(OpenAPI_umt_time_1_t *dst, OpenAPI_umt_time_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_umt_time_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_umt_time_1_convertToJSON() failed");
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

    OpenAPI_umt_time_1_free(dst);
    dst = OpenAPI_umt_time_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

