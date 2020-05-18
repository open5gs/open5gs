
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_id.h"

OpenAPI_event_id_t *OpenAPI_event_id_create(
    )
{
    OpenAPI_event_id_t *event_id_local_var = OpenAPI_malloc(sizeof(OpenAPI_event_id_t));
    if (!event_id_local_var) {
        return NULL;
    }

    return event_id_local_var;
}

void OpenAPI_event_id_free(OpenAPI_event_id_t *event_id)
{
    if (NULL == event_id) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(event_id);
}

cJSON *OpenAPI_event_id_convertToJSON(OpenAPI_event_id_t *event_id)
{
    cJSON *item = NULL;

    if (event_id == NULL) {
        ogs_error("OpenAPI_event_id_convertToJSON() failed [EventId]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_event_id_t *OpenAPI_event_id_parseFromJSON(cJSON *event_idJSON)
{
    OpenAPI_event_id_t *event_id_local_var = NULL;
    event_id_local_var = OpenAPI_event_id_create (
        );

    return event_id_local_var;
end:
    return NULL;
}

