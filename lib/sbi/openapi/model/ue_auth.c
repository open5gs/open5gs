
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_auth.h"

OpenAPI_ue_auth_t *OpenAPI_ue_auth_create(
    )
{
    OpenAPI_ue_auth_t *ue_auth_local_var = OpenAPI_malloc(sizeof(OpenAPI_ue_auth_t));
    if (!ue_auth_local_var) {
        return NULL;
    }

    return ue_auth_local_var;
}

void OpenAPI_ue_auth_free(OpenAPI_ue_auth_t *ue_auth)
{
    if (NULL == ue_auth) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ue_auth);
}

cJSON *OpenAPI_ue_auth_convertToJSON(OpenAPI_ue_auth_t *ue_auth)
{
    cJSON *item = NULL;

    if (ue_auth == NULL) {
        ogs_error("OpenAPI_ue_auth_convertToJSON() failed [UeAuth]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_ue_auth_t *OpenAPI_ue_auth_parseFromJSON(cJSON *ue_authJSON)
{
    OpenAPI_ue_auth_t *ue_auth_local_var = NULL;
    ue_auth_local_var = OpenAPI_ue_auth_create (
        );

    return ue_auth_local_var;
end:
    return NULL;
}

OpenAPI_ue_auth_t *OpenAPI_ue_auth_copy(OpenAPI_ue_auth_t *dst, OpenAPI_ue_auth_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_auth_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_auth_convertToJSON() failed");
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

    OpenAPI_ue_auth_free(dst);
    dst = OpenAPI_ue_auth_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

