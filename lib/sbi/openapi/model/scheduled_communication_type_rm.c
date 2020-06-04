
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scheduled_communication_type_rm.h"

OpenAPI_scheduled_communication_type_rm_t *OpenAPI_scheduled_communication_type_rm_create(
    )
{
    OpenAPI_scheduled_communication_type_rm_t *scheduled_communication_type_rm_local_var = OpenAPI_malloc(sizeof(OpenAPI_scheduled_communication_type_rm_t));
    if (!scheduled_communication_type_rm_local_var) {
        return NULL;
    }

    return scheduled_communication_type_rm_local_var;
}

void OpenAPI_scheduled_communication_type_rm_free(OpenAPI_scheduled_communication_type_rm_t *scheduled_communication_type_rm)
{
    if (NULL == scheduled_communication_type_rm) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(scheduled_communication_type_rm);
}

cJSON *OpenAPI_scheduled_communication_type_rm_convertToJSON(OpenAPI_scheduled_communication_type_rm_t *scheduled_communication_type_rm)
{
    cJSON *item = NULL;

    if (scheduled_communication_type_rm == NULL) {
        ogs_error("OpenAPI_scheduled_communication_type_rm_convertToJSON() failed [ScheduledCommunicationTypeRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_scheduled_communication_type_rm_t *OpenAPI_scheduled_communication_type_rm_parseFromJSON(cJSON *scheduled_communication_type_rmJSON)
{
    OpenAPI_scheduled_communication_type_rm_t *scheduled_communication_type_rm_local_var = NULL;
    scheduled_communication_type_rm_local_var = OpenAPI_scheduled_communication_type_rm_create (
        );

    return scheduled_communication_type_rm_local_var;
end:
    return NULL;
}

