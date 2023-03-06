
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event.h"

OpenAPI_event_t *OpenAPI_event_create(
)
{
    OpenAPI_event_t *event_local_var = ogs_malloc(sizeof(OpenAPI_event_t));
    ogs_assert(event_local_var);


    return event_local_var;
}

void OpenAPI_event_free(OpenAPI_event_t *event)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == event) {
        return;
    }
    ogs_free(event);
}

cJSON *OpenAPI_event_convertToJSON(OpenAPI_event_t *event)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (event == NULL) {
        ogs_error("OpenAPI_event_convertToJSON() failed [Event]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_event_t *OpenAPI_event_parseFromJSON(cJSON *eventJSON)
{
    OpenAPI_event_t *event_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    event_local_var = OpenAPI_event_create (
    );

    return event_local_var;
end:
    return NULL;
}

OpenAPI_event_t *OpenAPI_event_copy(OpenAPI_event_t *dst, OpenAPI_event_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_event_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_event_convertToJSON() failed");
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

    OpenAPI_event_free(dst);
    dst = OpenAPI_event_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

