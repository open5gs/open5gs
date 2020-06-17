
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

OpenAPI_presence_state_t *OpenAPI_presence_state_copy(OpenAPI_presence_state_t *dst, OpenAPI_presence_state_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_presence_state_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_presence_state_convertToJSON() failed");
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

    OpenAPI_presence_state_free(dst);
    dst = OpenAPI_presence_state_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

