
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ho_state.h"

OpenAPI_ho_state_t *OpenAPI_ho_state_create(
    )
{
    OpenAPI_ho_state_t *ho_state_local_var = OpenAPI_malloc(sizeof(OpenAPI_ho_state_t));
    if (!ho_state_local_var) {
        return NULL;
    }

    return ho_state_local_var;
}

void OpenAPI_ho_state_free(OpenAPI_ho_state_t *ho_state)
{
    if (NULL == ho_state) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ho_state);
}

cJSON *OpenAPI_ho_state_convertToJSON(OpenAPI_ho_state_t *ho_state)
{
    cJSON *item = NULL;

    if (ho_state == NULL) {
        ogs_error("OpenAPI_ho_state_convertToJSON() failed [HoState]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_ho_state_t *OpenAPI_ho_state_parseFromJSON(cJSON *ho_stateJSON)
{
    OpenAPI_ho_state_t *ho_state_local_var = NULL;
    ho_state_local_var = OpenAPI_ho_state_create (
        );

    return ho_state_local_var;
end:
    return NULL;
}

OpenAPI_ho_state_t *OpenAPI_ho_state_copy(OpenAPI_ho_state_t *dst, OpenAPI_ho_state_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ho_state_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ho_state_convertToJSON() failed");
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

    OpenAPI_ho_state_free(dst);
    dst = OpenAPI_ho_state_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

