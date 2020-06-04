
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "presence_state.h"

OpenAPI_presence_state_t *OpenAPI_presence_state_create(
    )
{
    OpenAPI_presence_state_t *presence_state_local_var = OpenAPI_malloc(sizeof(OpenAPI_presence_state_t));
    if (!presence_state_local_var) {
        return NULL;
    }

    return presence_state_local_var;
}

void OpenAPI_presence_state_free(OpenAPI_presence_state_t *presence_state)
{
    if (NULL == presence_state) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(presence_state);
}

cJSON *OpenAPI_presence_state_convertToJSON(OpenAPI_presence_state_t *presence_state)
{
    cJSON *item = NULL;

    if (presence_state == NULL) {
        ogs_error("OpenAPI_presence_state_convertToJSON() failed [PresenceState]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_presence_state_t *OpenAPI_presence_state_parseFromJSON(cJSON *presence_stateJSON)
{
    OpenAPI_presence_state_t *presence_state_local_var = NULL;
    presence_state_local_var = OpenAPI_presence_state_create (
        );

    return presence_state_local_var;
end:
    return NULL;
}

