
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_event.h"

OpenAPI_af_event_t *OpenAPI_af_event_create(
    )
{
    OpenAPI_af_event_t *af_event_local_var = OpenAPI_malloc(sizeof(OpenAPI_af_event_t));
    if (!af_event_local_var) {
        return NULL;
    }

    return af_event_local_var;
}

void OpenAPI_af_event_free(OpenAPI_af_event_t *af_event)
{
    if (NULL == af_event) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(af_event);
}

cJSON *OpenAPI_af_event_convertToJSON(OpenAPI_af_event_t *af_event)
{
    cJSON *item = NULL;

    if (af_event == NULL) {
        ogs_error("OpenAPI_af_event_convertToJSON() failed [AfEvent]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_af_event_t *OpenAPI_af_event_parseFromJSON(cJSON *af_eventJSON)
{
    OpenAPI_af_event_t *af_event_local_var = NULL;
    af_event_local_var = OpenAPI_af_event_create (
        );

    return af_event_local_var;
end:
    return NULL;
}

OpenAPI_af_event_t *OpenAPI_af_event_copy(OpenAPI_af_event_t *dst, OpenAPI_af_event_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_event_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_event_convertToJSON() failed");
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

    OpenAPI_af_event_free(dst);
    dst = OpenAPI_af_event_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

