
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "roaming_restrictions.h"

OpenAPI_roaming_restrictions_t *OpenAPI_roaming_restrictions_create(
    bool is_access_allowed,
    int access_allowed
)
{
    OpenAPI_roaming_restrictions_t *roaming_restrictions_local_var = ogs_malloc(sizeof(OpenAPI_roaming_restrictions_t));
    ogs_assert(roaming_restrictions_local_var);

    roaming_restrictions_local_var->is_access_allowed = is_access_allowed;
    roaming_restrictions_local_var->access_allowed = access_allowed;

    return roaming_restrictions_local_var;
}

void OpenAPI_roaming_restrictions_free(OpenAPI_roaming_restrictions_t *roaming_restrictions)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == roaming_restrictions) {
        return;
    }
    ogs_free(roaming_restrictions);
}

cJSON *OpenAPI_roaming_restrictions_convertToJSON(OpenAPI_roaming_restrictions_t *roaming_restrictions)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (roaming_restrictions == NULL) {
        ogs_error("OpenAPI_roaming_restrictions_convertToJSON() failed [RoamingRestrictions]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (roaming_restrictions->is_access_allowed) {
    if (cJSON_AddBoolToObject(item, "accessAllowed", roaming_restrictions->access_allowed) == NULL) {
        ogs_error("OpenAPI_roaming_restrictions_convertToJSON() failed [access_allowed]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_roaming_restrictions_t *OpenAPI_roaming_restrictions_parseFromJSON(cJSON *roaming_restrictionsJSON)
{
    OpenAPI_roaming_restrictions_t *roaming_restrictions_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *access_allowed = NULL;
    access_allowed = cJSON_GetObjectItemCaseSensitive(roaming_restrictionsJSON, "accessAllowed");
    if (access_allowed) {
    if (!cJSON_IsBool(access_allowed)) {
        ogs_error("OpenAPI_roaming_restrictions_parseFromJSON() failed [access_allowed]");
        goto end;
    }
    }

    roaming_restrictions_local_var = OpenAPI_roaming_restrictions_create (
        access_allowed ? true : false,
        access_allowed ? access_allowed->valueint : 0
    );

    return roaming_restrictions_local_var;
end:
    return NULL;
}

OpenAPI_roaming_restrictions_t *OpenAPI_roaming_restrictions_copy(OpenAPI_roaming_restrictions_t *dst, OpenAPI_roaming_restrictions_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_roaming_restrictions_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_roaming_restrictions_convertToJSON() failed");
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

    OpenAPI_roaming_restrictions_free(dst);
    dst = OpenAPI_roaming_restrictions_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

