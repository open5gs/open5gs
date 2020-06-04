
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rm_state.h"

OpenAPI_rm_state_t *OpenAPI_rm_state_create(
    )
{
    OpenAPI_rm_state_t *rm_state_local_var = OpenAPI_malloc(sizeof(OpenAPI_rm_state_t));
    if (!rm_state_local_var) {
        return NULL;
    }

    return rm_state_local_var;
}

void OpenAPI_rm_state_free(OpenAPI_rm_state_t *rm_state)
{
    if (NULL == rm_state) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(rm_state);
}

cJSON *OpenAPI_rm_state_convertToJSON(OpenAPI_rm_state_t *rm_state)
{
    cJSON *item = NULL;

    if (rm_state == NULL) {
        ogs_error("OpenAPI_rm_state_convertToJSON() failed [RmState]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_rm_state_t *OpenAPI_rm_state_parseFromJSON(cJSON *rm_stateJSON)
{
    OpenAPI_rm_state_t *rm_state_local_var = NULL;
    rm_state_local_var = OpenAPI_rm_state_create (
        );

    return rm_state_local_var;
end:
    return NULL;
}

