
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cm_state.h"

OpenAPI_cm_state_t *OpenAPI_cm_state_create(
)
{
    OpenAPI_cm_state_t *cm_state_local_var = ogs_malloc(sizeof(OpenAPI_cm_state_t));
    ogs_assert(cm_state_local_var);


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

OpenAPI_cm_state_t *OpenAPI_cm_state_copy(OpenAPI_cm_state_t *dst, OpenAPI_cm_state_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cm_state_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cm_state_convertToJSON() failed");
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

    OpenAPI_cm_state_free(dst);
    dst = OpenAPI_cm_state_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

