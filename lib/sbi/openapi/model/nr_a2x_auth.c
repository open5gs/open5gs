
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nr_a2x_auth.h"

OpenAPI_nr_a2x_auth_t *OpenAPI_nr_a2x_auth_create(
    OpenAPI_ue_auth_e uav_ue_auth
)
{
    OpenAPI_nr_a2x_auth_t *nr_a2x_auth_local_var = ogs_malloc(sizeof(OpenAPI_nr_a2x_auth_t));
    ogs_assert(nr_a2x_auth_local_var);

    nr_a2x_auth_local_var->uav_ue_auth = uav_ue_auth;

    return nr_a2x_auth_local_var;
}

void OpenAPI_nr_a2x_auth_free(OpenAPI_nr_a2x_auth_t *nr_a2x_auth)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nr_a2x_auth) {
        return;
    }
    ogs_free(nr_a2x_auth);
}

cJSON *OpenAPI_nr_a2x_auth_convertToJSON(OpenAPI_nr_a2x_auth_t *nr_a2x_auth)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nr_a2x_auth == NULL) {
        ogs_error("OpenAPI_nr_a2x_auth_convertToJSON() failed [NrA2xAuth]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nr_a2x_auth->uav_ue_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "uavUeAuth", OpenAPI_ue_auth_ToString(nr_a2x_auth->uav_ue_auth)) == NULL) {
        ogs_error("OpenAPI_nr_a2x_auth_convertToJSON() failed [uav_ue_auth]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nr_a2x_auth_t *OpenAPI_nr_a2x_auth_parseFromJSON(cJSON *nr_a2x_authJSON)
{
    OpenAPI_nr_a2x_auth_t *nr_a2x_auth_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *uav_ue_auth = NULL;
    OpenAPI_ue_auth_e uav_ue_authVariable = 0;
    uav_ue_auth = cJSON_GetObjectItemCaseSensitive(nr_a2x_authJSON, "uavUeAuth");
    if (uav_ue_auth) {
    if (!cJSON_IsString(uav_ue_auth)) {
        ogs_error("OpenAPI_nr_a2x_auth_parseFromJSON() failed [uav_ue_auth]");
        goto end;
    }
    uav_ue_authVariable = OpenAPI_ue_auth_FromString(uav_ue_auth->valuestring);
    }

    nr_a2x_auth_local_var = OpenAPI_nr_a2x_auth_create (
        uav_ue_auth ? uav_ue_authVariable : 0
    );

    return nr_a2x_auth_local_var;
end:
    return NULL;
}

OpenAPI_nr_a2x_auth_t *OpenAPI_nr_a2x_auth_copy(OpenAPI_nr_a2x_auth_t *dst, OpenAPI_nr_a2x_auth_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nr_a2x_auth_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nr_a2x_auth_convertToJSON() failed");
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

    OpenAPI_nr_a2x_auth_free(dst);
    dst = OpenAPI_nr_a2x_auth_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

