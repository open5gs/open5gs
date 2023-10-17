
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rm_state.h"

OpenAPI_rm_state_t *OpenAPI_rm_state_create(
)
{
    OpenAPI_rm_state_t *rm_state_local_var = ogs_malloc(sizeof(OpenAPI_rm_state_t));
    ogs_assert(rm_state_local_var);


    return rm_state_local_var;
}

void OpenAPI_rm_state_free(OpenAPI_rm_state_t *rm_state)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == rm_state) {
        return;
    }
    ogs_free(rm_state);
}

cJSON *OpenAPI_rm_state_convertToJSON(OpenAPI_rm_state_t *rm_state)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_lnode_t *node = NULL;
    rm_state_local_var = OpenAPI_rm_state_create (
    );

    return rm_state_local_var;
end:
    return NULL;
}

OpenAPI_rm_state_t *OpenAPI_rm_state_copy(OpenAPI_rm_state_t *dst, OpenAPI_rm_state_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rm_state_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rm_state_convertToJSON() failed");
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

    OpenAPI_rm_state_free(dst);
    dst = OpenAPI_rm_state_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

