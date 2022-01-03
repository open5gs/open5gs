
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ladn_info.h"

OpenAPI_ladn_info_t *OpenAPI_ladn_info_create(
    char *ladn,
    OpenAPI_presence_state_e presence
)
{
    OpenAPI_ladn_info_t *ladn_info_local_var = ogs_malloc(sizeof(OpenAPI_ladn_info_t));
    ogs_assert(ladn_info_local_var);

    ladn_info_local_var->ladn = ladn;
    ladn_info_local_var->presence = presence;

    return ladn_info_local_var;
}

void OpenAPI_ladn_info_free(OpenAPI_ladn_info_t *ladn_info)
{
    if (NULL == ladn_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ladn_info->ladn);
    ogs_free(ladn_info);
}

cJSON *OpenAPI_ladn_info_convertToJSON(OpenAPI_ladn_info_t *ladn_info)
{
    cJSON *item = NULL;

    if (ladn_info == NULL) {
        ogs_error("OpenAPI_ladn_info_convertToJSON() failed [LadnInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "ladn", ladn_info->ladn) == NULL) {
        ogs_error("OpenAPI_ladn_info_convertToJSON() failed [ladn]");
        goto end;
    }

    if (ladn_info->presence) {
    if (cJSON_AddStringToObject(item, "presence", OpenAPI_presence_state_ToString(ladn_info->presence)) == NULL) {
        ogs_error("OpenAPI_ladn_info_convertToJSON() failed [presence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ladn_info_t *OpenAPI_ladn_info_parseFromJSON(cJSON *ladn_infoJSON)
{
    OpenAPI_ladn_info_t *ladn_info_local_var = NULL;
    cJSON *ladn = cJSON_GetObjectItemCaseSensitive(ladn_infoJSON, "ladn");
    if (!ladn) {
        ogs_error("OpenAPI_ladn_info_parseFromJSON() failed [ladn]");
        goto end;
    }

    if (!cJSON_IsString(ladn)) {
        ogs_error("OpenAPI_ladn_info_parseFromJSON() failed [ladn]");
        goto end;
    }

    cJSON *presence = cJSON_GetObjectItemCaseSensitive(ladn_infoJSON, "presence");

    OpenAPI_presence_state_e presenceVariable;
    if (presence) {
    if (!cJSON_IsString(presence)) {
        ogs_error("OpenAPI_ladn_info_parseFromJSON() failed [presence]");
        goto end;
    }
    presenceVariable = OpenAPI_presence_state_FromString(presence->valuestring);
    }

    ladn_info_local_var = OpenAPI_ladn_info_create (
        ogs_strdup(ladn->valuestring),
        presence ? presenceVariable : 0
    );

    return ladn_info_local_var;
end:
    return NULL;
}

OpenAPI_ladn_info_t *OpenAPI_ladn_info_copy(OpenAPI_ladn_info_t *dst, OpenAPI_ladn_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ladn_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ladn_info_convertToJSON() failed");
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

    OpenAPI_ladn_info_free(dst);
    dst = OpenAPI_ladn_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

