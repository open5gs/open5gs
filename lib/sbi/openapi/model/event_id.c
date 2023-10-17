
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_id.h"

OpenAPI_event_id_t *OpenAPI_event_id_create(
)
{
    OpenAPI_event_id_t *event_id_local_var = ogs_malloc(sizeof(OpenAPI_event_id_t));
    ogs_assert(event_id_local_var);


    return event_id_local_var;
}

void OpenAPI_event_id_free(OpenAPI_event_id_t *event_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == event_id) {
        return;
    }
    ogs_free(event_id);
}

cJSON *OpenAPI_event_id_convertToJSON(OpenAPI_event_id_t *event_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_lnode_t *node = NULL;
    event_id_local_var = OpenAPI_event_id_create (
    );

    return event_id_local_var;
end:
    return NULL;
}

OpenAPI_event_id_t *OpenAPI_event_id_copy(OpenAPI_event_id_t *dst, OpenAPI_event_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_event_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_event_id_convertToJSON() failed");
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

    OpenAPI_event_id_free(dst);
    dst = OpenAPI_event_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

