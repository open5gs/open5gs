
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscribed_event.h"

OpenAPI_subscribed_event_t *OpenAPI_subscribed_event_create(
)
{
    OpenAPI_subscribed_event_t *subscribed_event_local_var = ogs_malloc(sizeof(OpenAPI_subscribed_event_t));
    ogs_assert(subscribed_event_local_var);


    return subscribed_event_local_var;
}

void OpenAPI_subscribed_event_free(OpenAPI_subscribed_event_t *subscribed_event)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == subscribed_event) {
        return;
    }
    ogs_free(subscribed_event);
}

cJSON *OpenAPI_subscribed_event_convertToJSON(OpenAPI_subscribed_event_t *subscribed_event)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (subscribed_event == NULL) {
        ogs_error("OpenAPI_subscribed_event_convertToJSON() failed [SubscribedEvent]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_subscribed_event_t *OpenAPI_subscribed_event_parseFromJSON(cJSON *subscribed_eventJSON)
{
    OpenAPI_subscribed_event_t *subscribed_event_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    subscribed_event_local_var = OpenAPI_subscribed_event_create (
    );

    return subscribed_event_local_var;
end:
    return NULL;
}

OpenAPI_subscribed_event_t *OpenAPI_subscribed_event_copy(OpenAPI_subscribed_event_t *dst, OpenAPI_subscribed_event_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_subscribed_event_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_subscribed_event_convertToJSON() failed");
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

    OpenAPI_subscribed_event_free(dst);
    dst = OpenAPI_subscribed_event_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

