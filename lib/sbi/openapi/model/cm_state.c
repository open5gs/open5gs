
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cm_state.h"

OpenAPI_cm_state_t *OpenAPI_cm_state_create(
    )
{
    OpenAPI_cm_state_t *cm_state_local_var = OpenAPI_malloc(sizeof(OpenAPI_cm_state_t));
    if (!cm_state_local_var) {
        return NULL;
    }

    return cm_state_local_var;
}

void OpenAPI_cm_state_free(OpenAPI_cm_state_t *cm_state)
{
    if (NULL == cm_state) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(cm_state);
}

cJSON *OpenAPI_cm_state_convertToJSON(OpenAPI_cm_state_t *cm_state)
{
    cJSON *item = NULL;

    if (cm_state == NULL) {
        ogs_error("OpenAPI_cm_state_convertToJSON() failed [CmState]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_cm_state_t *OpenAPI_cm_state_parseFromJSON(cJSON *cm_stateJSON)
{
    OpenAPI_cm_state_t *cm_state_local_var = NULL;
    cm_state_local_var = OpenAPI_cm_state_create (
        );

    return cm_state_local_var;
end:
    return NULL;
}

