
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "auth_trigger_info.h"

OpenAPI_auth_trigger_info_t *OpenAPI_auth_trigger_info_create(
    char *supi
)
{
    OpenAPI_auth_trigger_info_t *auth_trigger_info_local_var = ogs_malloc(sizeof(OpenAPI_auth_trigger_info_t));
    ogs_assert(auth_trigger_info_local_var);

    auth_trigger_info_local_var->supi = supi;

    return auth_trigger_info_local_var;
}

void OpenAPI_auth_trigger_info_free(OpenAPI_auth_trigger_info_t *auth_trigger_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == auth_trigger_info) {
        return;
    }
    if (auth_trigger_info->supi) {
        ogs_free(auth_trigger_info->supi);
        auth_trigger_info->supi = NULL;
    }
    ogs_free(auth_trigger_info);
}

cJSON *OpenAPI_auth_trigger_info_convertToJSON(OpenAPI_auth_trigger_info_t *auth_trigger_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (auth_trigger_info == NULL) {
        ogs_error("OpenAPI_auth_trigger_info_convertToJSON() failed [AuthTriggerInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (auth_trigger_info->supi) {
    if (cJSON_AddStringToObject(item, "supi", auth_trigger_info->supi) == NULL) {
        ogs_error("OpenAPI_auth_trigger_info_convertToJSON() failed [supi]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_auth_trigger_info_t *OpenAPI_auth_trigger_info_parseFromJSON(cJSON *auth_trigger_infoJSON)
{
    OpenAPI_auth_trigger_info_t *auth_trigger_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(auth_trigger_infoJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_auth_trigger_info_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    auth_trigger_info_local_var = OpenAPI_auth_trigger_info_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL
    );

    return auth_trigger_info_local_var;
end:
    return NULL;
}

OpenAPI_auth_trigger_info_t *OpenAPI_auth_trigger_info_copy(OpenAPI_auth_trigger_info_t *dst, OpenAPI_auth_trigger_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_auth_trigger_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_auth_trigger_info_convertToJSON() failed");
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

    OpenAPI_auth_trigger_info_free(dst);
    dst = OpenAPI_auth_trigger_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

