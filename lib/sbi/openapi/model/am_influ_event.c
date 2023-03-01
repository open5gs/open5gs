
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "am_influ_event.h"

OpenAPI_am_influ_event_t *OpenAPI_am_influ_event_create(
)
{
    OpenAPI_am_influ_event_t *am_influ_event_local_var = ogs_malloc(sizeof(OpenAPI_am_influ_event_t));
    ogs_assert(am_influ_event_local_var);


    return am_influ_event_local_var;
}

void OpenAPI_am_influ_event_free(OpenAPI_am_influ_event_t *am_influ_event)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == am_influ_event) {
        return;
    }
    ogs_free(am_influ_event);
}

cJSON *OpenAPI_am_influ_event_convertToJSON(OpenAPI_am_influ_event_t *am_influ_event)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (am_influ_event == NULL) {
        ogs_error("OpenAPI_am_influ_event_convertToJSON() failed [AmInfluEvent]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_am_influ_event_t *OpenAPI_am_influ_event_parseFromJSON(cJSON *am_influ_eventJSON)
{
    OpenAPI_am_influ_event_t *am_influ_event_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    am_influ_event_local_var = OpenAPI_am_influ_event_create (
    );

    return am_influ_event_local_var;
end:
    return NULL;
}

OpenAPI_am_influ_event_t *OpenAPI_am_influ_event_copy(OpenAPI_am_influ_event_t *dst, OpenAPI_am_influ_event_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_am_influ_event_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_am_influ_event_convertToJSON() failed");
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

    OpenAPI_am_influ_event_free(dst);
    dst = OpenAPI_am_influ_event_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

