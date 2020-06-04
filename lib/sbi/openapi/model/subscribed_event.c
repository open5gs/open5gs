
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscribed_event.h"

OpenAPI_subscribed_event_t *OpenAPI_subscribed_event_create(
    )
{
    OpenAPI_subscribed_event_t *subscribed_event_local_var = OpenAPI_malloc(sizeof(OpenAPI_subscribed_event_t));
    if (!subscribed_event_local_var) {
        return NULL;
    }

    return subscribed_event_local_var;
}

void OpenAPI_subscribed_event_free(OpenAPI_subscribed_event_t *subscribed_event)
{
    if (NULL == subscribed_event) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(subscribed_event);
}

cJSON *OpenAPI_subscribed_event_convertToJSON(OpenAPI_subscribed_event_t *subscribed_event)
{
    cJSON *item = NULL;

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
    subscribed_event_local_var = OpenAPI_subscribed_event_create (
        );

    return subscribed_event_local_var;
end:
    return NULL;
}

