
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_event_trigger.h"

OpenAPI_amf_event_trigger_t *OpenAPI_amf_event_trigger_create(
)
{
    OpenAPI_amf_event_trigger_t *amf_event_trigger_local_var = ogs_malloc(sizeof(OpenAPI_amf_event_trigger_t));
    ogs_assert(amf_event_trigger_local_var);


    return amf_event_trigger_local_var;
}

void OpenAPI_amf_event_trigger_free(OpenAPI_amf_event_trigger_t *amf_event_trigger)
{
    if (NULL == amf_event_trigger) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(amf_event_trigger);
}

cJSON *OpenAPI_amf_event_trigger_convertToJSON(OpenAPI_amf_event_trigger_t *amf_event_trigger)
{
    cJSON *item = NULL;

    if (amf_event_trigger == NULL) {
        ogs_error("OpenAPI_amf_event_trigger_convertToJSON() failed [AmfEventTrigger]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_amf_event_trigger_t *OpenAPI_amf_event_trigger_parseFromJSON(cJSON *amf_event_triggerJSON)
{
    OpenAPI_amf_event_trigger_t *amf_event_trigger_local_var = NULL;
    amf_event_trigger_local_var = OpenAPI_amf_event_trigger_create (
    );

    return amf_event_trigger_local_var;
end:
    return NULL;
}

OpenAPI_amf_event_trigger_t *OpenAPI_amf_event_trigger_copy(OpenAPI_amf_event_trigger_t *dst, OpenAPI_amf_event_trigger_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_event_trigger_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_event_trigger_convertToJSON() failed");
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

    OpenAPI_amf_event_trigger_free(dst);
    dst = OpenAPI_amf_event_trigger_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

