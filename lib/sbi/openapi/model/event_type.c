
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_type.h"

OpenAPI_event_type_t *OpenAPI_event_type_create(
    )
{
    OpenAPI_event_type_t *event_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_event_type_t));
    if (!event_type_local_var) {
        return NULL;
    }

    return event_type_local_var;
}

void OpenAPI_event_type_free(OpenAPI_event_type_t *event_type)
{
    if (NULL == event_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(event_type);
}

cJSON *OpenAPI_event_type_convertToJSON(OpenAPI_event_type_t *event_type)
{
    cJSON *item = NULL;

    if (event_type == NULL) {
        ogs_error("OpenAPI_event_type_convertToJSON() failed [EventType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_event_type_t *OpenAPI_event_type_parseFromJSON(cJSON *event_typeJSON)
{
    OpenAPI_event_type_t *event_type_local_var = NULL;
    event_type_local_var = OpenAPI_event_type_create (
        );

    return event_type_local_var;
end:
    return NULL;
}

