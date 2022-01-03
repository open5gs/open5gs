
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scheduled_communication_time_1.h"

OpenAPI_scheduled_communication_time_1_t *OpenAPI_scheduled_communication_time_1_create(
    OpenAPI_list_t *days_of_week,
    char *time_of_day_start,
    char *time_of_day_end
)
{
    OpenAPI_scheduled_communication_time_1_t *scheduled_communication_time_1_local_var = ogs_malloc(sizeof(OpenAPI_scheduled_communication_time_1_t));
    ogs_assert(scheduled_communication_time_1_local_var);

    scheduled_communication_time_1_local_var->days_of_week = days_of_week;
    scheduled_communication_time_1_local_var->time_of_day_start = time_of_day_start;
    scheduled_communication_time_1_local_var->time_of_day_end = time_of_day_end;

    return scheduled_communication_time_1_local_var;
}

void OpenAPI_scheduled_communication_time_1_free(OpenAPI_scheduled_communication_time_1_t *scheduled_communication_time_1)
{
    if (NULL == scheduled_communication_time_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(scheduled_communication_time_1->days_of_week, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(scheduled_communication_time_1->days_of_week);
    ogs_free(scheduled_communication_time_1->time_of_day_start);
    ogs_free(scheduled_communication_time_1->time_of_day_end);
    ogs_free(scheduled_communication_time_1);
}

cJSON *OpenAPI_scheduled_communication_time_1_convertToJSON(OpenAPI_scheduled_communication_time_1_t *scheduled_communication_time_1)
{
    cJSON *item = NULL;

    if (scheduled_communication_time_1 == NULL) {
        ogs_error("OpenAPI_scheduled_communication_time_1_convertToJSON() failed [ScheduledCommunicationTime_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (scheduled_communication_time_1->days_of_week) {
    cJSON *days_of_week = cJSON_AddArrayToObject(item, "daysOfWeek");
    if (days_of_week == NULL) {
        ogs_error("OpenAPI_scheduled_communication_time_1_convertToJSON() failed [days_of_week]");
        goto end;
    }

    OpenAPI_lnode_t *days_of_week_node;
    OpenAPI_list_for_each(scheduled_communication_time_1->days_of_week, days_of_week_node)  {
    if (cJSON_AddNumberToObject(days_of_week, "", *(double *)days_of_week_node->data) == NULL) {
        ogs_error("OpenAPI_scheduled_communication_time_1_convertToJSON() failed [days_of_week]");
        goto end;
    }
                    }
    }

    if (scheduled_communication_time_1->time_of_day_start) {
    if (cJSON_AddStringToObject(item, "timeOfDayStart", scheduled_communication_time_1->time_of_day_start) == NULL) {
        ogs_error("OpenAPI_scheduled_communication_time_1_convertToJSON() failed [time_of_day_start]");
        goto end;
    }
    }

    if (scheduled_communication_time_1->time_of_day_end) {
    if (cJSON_AddStringToObject(item, "timeOfDayEnd", scheduled_communication_time_1->time_of_day_end) == NULL) {
        ogs_error("OpenAPI_scheduled_communication_time_1_convertToJSON() failed [time_of_day_end]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_scheduled_communication_time_1_t *OpenAPI_scheduled_communication_time_1_parseFromJSON(cJSON *scheduled_communication_time_1JSON)
{
    OpenAPI_scheduled_communication_time_1_t *scheduled_communication_time_1_local_var = NULL;
    cJSON *days_of_week = cJSON_GetObjectItemCaseSensitive(scheduled_communication_time_1JSON, "daysOfWeek");

    OpenAPI_list_t *days_of_weekList;
    if (days_of_week) {
    cJSON *days_of_week_local;
    if (!cJSON_IsArray(days_of_week)) {
        ogs_error("OpenAPI_scheduled_communication_time_1_parseFromJSON() failed [days_of_week]");
        goto end;
    }
    days_of_weekList = OpenAPI_list_create();

    cJSON_ArrayForEach(days_of_week_local, days_of_week) {
    if (!cJSON_IsNumber(days_of_week_local)) {
        ogs_error("OpenAPI_scheduled_communication_time_1_parseFromJSON() failed [days_of_week]");
        goto end;
    }
    OpenAPI_list_add(days_of_weekList , &days_of_week_local->valuedouble);
    }
    }

    cJSON *time_of_day_start = cJSON_GetObjectItemCaseSensitive(scheduled_communication_time_1JSON, "timeOfDayStart");

    if (time_of_day_start) {
    if (!cJSON_IsString(time_of_day_start)) {
        ogs_error("OpenAPI_scheduled_communication_time_1_parseFromJSON() failed [time_of_day_start]");
        goto end;
    }
    }

    cJSON *time_of_day_end = cJSON_GetObjectItemCaseSensitive(scheduled_communication_time_1JSON, "timeOfDayEnd");

    if (time_of_day_end) {
    if (!cJSON_IsString(time_of_day_end)) {
        ogs_error("OpenAPI_scheduled_communication_time_1_parseFromJSON() failed [time_of_day_end]");
        goto end;
    }
    }

    scheduled_communication_time_1_local_var = OpenAPI_scheduled_communication_time_1_create (
        days_of_week ? days_of_weekList : NULL,
        time_of_day_start ? ogs_strdup(time_of_day_start->valuestring) : NULL,
        time_of_day_end ? ogs_strdup(time_of_day_end->valuestring) : NULL
    );

    return scheduled_communication_time_1_local_var;
end:
    return NULL;
}

OpenAPI_scheduled_communication_time_1_t *OpenAPI_scheduled_communication_time_1_copy(OpenAPI_scheduled_communication_time_1_t *dst, OpenAPI_scheduled_communication_time_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_scheduled_communication_time_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_scheduled_communication_time_1_convertToJSON() failed");
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

    OpenAPI_scheduled_communication_time_1_free(dst);
    dst = OpenAPI_scheduled_communication_time_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

