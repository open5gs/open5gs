
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

