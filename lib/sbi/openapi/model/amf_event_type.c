
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_event_type.h"

OpenAPI_amf_event_type_t *OpenAPI_amf_event_type_create(
    )
{
    OpenAPI_amf_event_type_t *amf_event_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_amf_event_type_t));
    if (!amf_event_type_local_var) {
        return NULL;
    }

    return amf_event_type_local_var;
}

void OpenAPI_amf_event_type_free(OpenAPI_amf_event_type_t *amf_event_type)
{
    if (NULL == amf_event_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(amf_event_type);
}

cJSON *OpenAPI_amf_event_type_convertToJSON(OpenAPI_amf_event_type_t *amf_event_type)
{
    cJSON *item = NULL;

    if (amf_event_type == NULL) {
        ogs_error("OpenAPI_amf_event_type_convertToJSON() failed [AmfEventType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_amf_event_type_t *OpenAPI_amf_event_type_parseFromJSON(cJSON *amf_event_typeJSON)
{
    OpenAPI_amf_event_type_t *amf_event_type_local_var = NULL;
    amf_event_type_local_var = OpenAPI_amf_event_type_create (
        );

    return amf_event_type_local_var;
end:
    return NULL;
}

OpenAPI_amf_event_type_t *OpenAPI_amf_event_type_copy(OpenAPI_amf_event_type_t *dst, OpenAPI_amf_event_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_event_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_event_type_convertToJSON() failed");
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

    OpenAPI_amf_event_type_free(dst);
    dst = OpenAPI_amf_event_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

