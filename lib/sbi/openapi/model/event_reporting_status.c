
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_reporting_status.h"

OpenAPI_event_reporting_status_t *OpenAPI_event_reporting_status_create(
    bool is_event_report_counter,
    int event_report_counter,
    bool is_event_report_duration,
    int event_report_duration
)
{
    OpenAPI_event_reporting_status_t *event_reporting_status_local_var = ogs_malloc(sizeof(OpenAPI_event_reporting_status_t));
    ogs_assert(event_reporting_status_local_var);

    event_reporting_status_local_var->is_event_report_counter = is_event_report_counter;
    event_reporting_status_local_var->event_report_counter = event_report_counter;
    event_reporting_status_local_var->is_event_report_duration = is_event_report_duration;
    event_reporting_status_local_var->event_report_duration = event_report_duration;

    return event_reporting_status_local_var;
}

void OpenAPI_event_reporting_status_free(OpenAPI_event_reporting_status_t *event_reporting_status)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == event_reporting_status) {
        return;
    }
    ogs_free(event_reporting_status);
}

cJSON *OpenAPI_event_reporting_status_convertToJSON(OpenAPI_event_reporting_status_t *event_reporting_status)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (event_reporting_status == NULL) {
        ogs_error("OpenAPI_event_reporting_status_convertToJSON() failed [EventReportingStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (event_reporting_status->is_event_report_counter) {
    if (cJSON_AddNumberToObject(item, "eventReportCounter", event_reporting_status->event_report_counter) == NULL) {
        ogs_error("OpenAPI_event_reporting_status_convertToJSON() failed [event_report_counter]");
        goto end;
    }
    }

    if (event_reporting_status->is_event_report_duration) {
    if (cJSON_AddNumberToObject(item, "eventReportDuration", event_reporting_status->event_report_duration) == NULL) {
        ogs_error("OpenAPI_event_reporting_status_convertToJSON() failed [event_report_duration]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_event_reporting_status_t *OpenAPI_event_reporting_status_parseFromJSON(cJSON *event_reporting_statusJSON)
{
    OpenAPI_event_reporting_status_t *event_reporting_status_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event_report_counter = NULL;
    cJSON *event_report_duration = NULL;
    event_report_counter = cJSON_GetObjectItemCaseSensitive(event_reporting_statusJSON, "eventReportCounter");
    if (event_report_counter) {
    if (!cJSON_IsNumber(event_report_counter)) {
        ogs_error("OpenAPI_event_reporting_status_parseFromJSON() failed [event_report_counter]");
        goto end;
    }
    }

    event_report_duration = cJSON_GetObjectItemCaseSensitive(event_reporting_statusJSON, "eventReportDuration");
    if (event_report_duration) {
    if (!cJSON_IsNumber(event_report_duration)) {
        ogs_error("OpenAPI_event_reporting_status_parseFromJSON() failed [event_report_duration]");
        goto end;
    }
    }

    event_reporting_status_local_var = OpenAPI_event_reporting_status_create (
        event_report_counter ? true : false,
        event_report_counter ? event_report_counter->valuedouble : 0,
        event_report_duration ? true : false,
        event_report_duration ? event_report_duration->valuedouble : 0
    );

    return event_reporting_status_local_var;
end:
    return NULL;
}

OpenAPI_event_reporting_status_t *OpenAPI_event_reporting_status_copy(OpenAPI_event_reporting_status_t *dst, OpenAPI_event_reporting_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_event_reporting_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_event_reporting_status_convertToJSON() failed");
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

    OpenAPI_event_reporting_status_free(dst);
    dst = OpenAPI_event_reporting_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

