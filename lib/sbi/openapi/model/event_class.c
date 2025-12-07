
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_class.h"

OpenAPI_event_class_t *OpenAPI_event_class_create(
)
{
    OpenAPI_event_class_t *event_class_local_var = ogs_malloc(sizeof(OpenAPI_event_class_t));
    ogs_assert(event_class_local_var);


    return event_class_local_var;
}

void OpenAPI_event_class_free(OpenAPI_event_class_t *event_class)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == event_class) {
        return;
    }
    ogs_free(event_class);
}

cJSON *OpenAPI_event_class_convertToJSON(OpenAPI_event_class_t *event_class)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (event_class == NULL) {
        ogs_error("OpenAPI_event_class_convertToJSON() failed [EventClass]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_event_class_t *OpenAPI_event_class_parseFromJSON(cJSON *event_classJSON)
{
    OpenAPI_event_class_t *event_class_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    event_class_local_var = OpenAPI_event_class_create (
    );

    return event_class_local_var;
end:
    return NULL;
}

OpenAPI_event_class_t *OpenAPI_event_class_copy(OpenAPI_event_class_t *dst, OpenAPI_event_class_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_event_class_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_event_class_convertToJSON() failed");
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

    OpenAPI_event_class_free(dst);
    dst = OpenAPI_event_class_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

