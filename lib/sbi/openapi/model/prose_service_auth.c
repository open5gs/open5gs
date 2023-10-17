
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "prose_service_auth.h"

OpenAPI_prose_service_auth_t *OpenAPI_prose_service_auth_create(
    OpenAPI_ue_auth_e prose_direct_discovery_auth,
    OpenAPI_ue_auth_e prose_direct_communication_auth
)
{
    OpenAPI_prose_service_auth_t *prose_service_auth_local_var = ogs_malloc(sizeof(OpenAPI_prose_service_auth_t));
    ogs_assert(prose_service_auth_local_var);

    prose_service_auth_local_var->prose_direct_discovery_auth = prose_direct_discovery_auth;
    prose_service_auth_local_var->prose_direct_communication_auth = prose_direct_communication_auth;

    return prose_service_auth_local_var;
}

void OpenAPI_prose_service_auth_free(OpenAPI_prose_service_auth_t *prose_service_auth)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == prose_service_auth) {
        return;
    }
    ogs_free(prose_service_auth);
}

cJSON *OpenAPI_prose_service_auth_convertToJSON(OpenAPI_prose_service_auth_t *prose_service_auth)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (prose_service_auth == NULL) {
        ogs_error("OpenAPI_prose_service_auth_convertToJSON() failed [ProseServiceAuth]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (prose_service_auth->prose_direct_discovery_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseDirectDiscoveryAuth", OpenAPI_ue_auth_ToString(prose_service_auth->prose_direct_discovery_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_convertToJSON() failed [prose_direct_discovery_auth]");
        goto end;
    }
    }

    if (prose_service_auth->prose_direct_communication_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseDirectCommunicationAuth", OpenAPI_ue_auth_ToString(prose_service_auth->prose_direct_communication_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_convertToJSON() failed [prose_direct_communication_auth]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_prose_service_auth_t *OpenAPI_prose_service_auth_parseFromJSON(cJSON *prose_service_authJSON)
{
    OpenAPI_prose_service_auth_t *prose_service_auth_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *prose_direct_discovery_auth = NULL;
    OpenAPI_ue_auth_e prose_direct_discovery_authVariable = 0;
    cJSON *prose_direct_communication_auth = NULL;
    OpenAPI_ue_auth_e prose_direct_communication_authVariable = 0;
    prose_direct_discovery_auth = cJSON_GetObjectItemCaseSensitive(prose_service_authJSON, "proseDirectDiscoveryAuth");
    if (prose_direct_discovery_auth) {
    if (!cJSON_IsString(prose_direct_discovery_auth)) {
        ogs_error("OpenAPI_prose_service_auth_parseFromJSON() failed [prose_direct_discovery_auth]");
        goto end;
    }
    prose_direct_discovery_authVariable = OpenAPI_ue_auth_FromString(prose_direct_discovery_auth->valuestring);
    }

    prose_direct_communication_auth = cJSON_GetObjectItemCaseSensitive(prose_service_authJSON, "proseDirectCommunicationAuth");
    if (prose_direct_communication_auth) {
    if (!cJSON_IsString(prose_direct_communication_auth)) {
        ogs_error("OpenAPI_prose_service_auth_parseFromJSON() failed [prose_direct_communication_auth]");
        goto end;
    }
    prose_direct_communication_authVariable = OpenAPI_ue_auth_FromString(prose_direct_communication_auth->valuestring);
    }

    prose_service_auth_local_var = OpenAPI_prose_service_auth_create (
        prose_direct_discovery_auth ? prose_direct_discovery_authVariable : 0,
        prose_direct_communication_auth ? prose_direct_communication_authVariable : 0
    );

    return prose_service_auth_local_var;
end:
    return NULL;
}

OpenAPI_prose_service_auth_t *OpenAPI_prose_service_auth_copy(OpenAPI_prose_service_auth_t *dst, OpenAPI_prose_service_auth_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_prose_service_auth_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_prose_service_auth_convertToJSON() failed");
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

    OpenAPI_prose_service_auth_free(dst);
    dst = OpenAPI_prose_service_auth_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

