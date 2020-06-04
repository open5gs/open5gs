
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

