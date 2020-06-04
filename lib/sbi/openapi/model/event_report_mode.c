
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_report_mode.h"

OpenAPI_event_report_mode_t *OpenAPI_event_report_mode_create(
    )
{
    OpenAPI_event_report_mode_t *event_report_mode_local_var = OpenAPI_malloc(sizeof(OpenAPI_event_report_mode_t));
    if (!event_report_mode_local_var) {
        return NULL;
    }

    return event_report_mode_local_var;
}

void OpenAPI_event_report_mode_free(OpenAPI_event_report_mode_t *event_report_mode)
{
    if (NULL == event_report_mode) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(event_report_mode);
}

cJSON *OpenAPI_event_report_mode_convertToJSON(OpenAPI_event_report_mode_t *event_report_mode)
{
    cJSON *item = NULL;

    if (event_report_mode == NULL) {
        ogs_error("OpenAPI_event_report_mode_convertToJSON() failed [EventReportMode]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_event_report_mode_t *OpenAPI_event_report_mode_parseFromJSON(cJSON *event_report_modeJSON)
{
    OpenAPI_event_report_mode_t *event_report_mode_local_var = NULL;
    event_report_mode_local_var = OpenAPI_event_report_mode_create (
        );

    return event_report_mode_local_var;
end:
    return NULL;
}

