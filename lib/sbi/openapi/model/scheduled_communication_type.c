
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scheduled_communication_type.h"

OpenAPI_scheduled_communication_type_t *OpenAPI_scheduled_communication_type_create(
    )
{
    OpenAPI_scheduled_communication_type_t *scheduled_communication_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_scheduled_communication_type_t));
    if (!scheduled_communication_type_local_var) {
        return NULL;
    }

    return scheduled_communication_type_local_var;
}

void OpenAPI_scheduled_communication_type_free(OpenAPI_scheduled_communication_type_t *scheduled_communication_type)
{
    if (NULL == scheduled_communication_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(scheduled_communication_type);
}

cJSON *OpenAPI_scheduled_communication_type_convertToJSON(OpenAPI_scheduled_communication_type_t *scheduled_communication_type)
{
    cJSON *item = NULL;

    if (scheduled_communication_type == NULL) {
        ogs_error("OpenAPI_scheduled_communication_type_convertToJSON() failed [ScheduledCommunicationType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_scheduled_communication_type_t *OpenAPI_scheduled_communication_type_parseFromJSON(cJSON *scheduled_communication_typeJSON)
{
    OpenAPI_scheduled_communication_type_t *scheduled_communication_type_local_var = NULL;
    scheduled_communication_type_local_var = OpenAPI_scheduled_communication_type_create (
        );

    return scheduled_communication_type_local_var;
end:
    return NULL;
}

OpenAPI_scheduled_communication_type_t *OpenAPI_scheduled_communication_type_copy(OpenAPI_scheduled_communication_type_t *dst, OpenAPI_scheduled_communication_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_scheduled_communication_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_scheduled_communication_type_convertToJSON() failed");
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

    OpenAPI_scheduled_communication_type_free(dst);
    dst = OpenAPI_scheduled_communication_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

