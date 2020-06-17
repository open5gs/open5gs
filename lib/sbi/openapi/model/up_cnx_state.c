
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_cnx_state.h"

OpenAPI_up_cnx_state_t *OpenAPI_up_cnx_state_create(
    )
{
    OpenAPI_up_cnx_state_t *up_cnx_state_local_var = OpenAPI_malloc(sizeof(OpenAPI_up_cnx_state_t));
    if (!up_cnx_state_local_var) {
        return NULL;
    }

    return up_cnx_state_local_var;
}

void OpenAPI_up_cnx_state_free(OpenAPI_up_cnx_state_t *up_cnx_state)
{
    if (NULL == up_cnx_state) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(up_cnx_state);
}

cJSON *OpenAPI_up_cnx_state_convertToJSON(OpenAPI_up_cnx_state_t *up_cnx_state)
{
    cJSON *item = NULL;

    if (up_cnx_state == NULL) {
        ogs_error("OpenAPI_up_cnx_state_convertToJSON() failed [UpCnxState]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_up_cnx_state_t *OpenAPI_up_cnx_state_parseFromJSON(cJSON *up_cnx_stateJSON)
{
    OpenAPI_up_cnx_state_t *up_cnx_state_local_var = NULL;
    up_cnx_state_local_var = OpenAPI_up_cnx_state_create (
        );

    return up_cnx_state_local_var;
end:
    return NULL;
}

OpenAPI_up_cnx_state_t *OpenAPI_up_cnx_state_copy(OpenAPI_up_cnx_state_t *dst, OpenAPI_up_cnx_state_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_up_cnx_state_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_up_cnx_state_convertToJSON() failed");
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

    OpenAPI_up_cnx_state_free(dst);
    dst = OpenAPI_up_cnx_state_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

