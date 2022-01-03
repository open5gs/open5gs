
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ambr_rm.h"

OpenAPI_ambr_rm_t *OpenAPI_ambr_rm_create(
    char *uplink,
    char *downlink
)
{
    OpenAPI_ambr_rm_t *ambr_rm_local_var = ogs_malloc(sizeof(OpenAPI_ambr_rm_t));
    ogs_assert(ambr_rm_local_var);

    ambr_rm_local_var->uplink = uplink;
    ambr_rm_local_var->downlink = downlink;

    return ambr_rm_local_var;
}

void OpenAPI_ambr_rm_free(OpenAPI_ambr_rm_t *ambr_rm)
{
    if (NULL == ambr_rm) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ambr_rm->uplink);
    ogs_free(ambr_rm->downlink);
    ogs_free(ambr_rm);
}

cJSON *OpenAPI_ambr_rm_convertToJSON(OpenAPI_ambr_rm_t *ambr_rm)
{
    cJSON *item = NULL;

    if (ambr_rm == NULL) {
        ogs_error("OpenAPI_ambr_rm_convertToJSON() failed [AmbrRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "uplink", ambr_rm->uplink) == NULL) {
        ogs_error("OpenAPI_ambr_rm_convertToJSON() failed [uplink]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "downlink", ambr_rm->downlink) == NULL) {
        ogs_error("OpenAPI_ambr_rm_convertToJSON() failed [downlink]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ambr_rm_t *OpenAPI_ambr_rm_parseFromJSON(cJSON *ambr_rmJSON)
{
    OpenAPI_ambr_rm_t *ambr_rm_local_var = NULL;
    cJSON *uplink = cJSON_GetObjectItemCaseSensitive(ambr_rmJSON, "uplink");
    if (!uplink) {
        ogs_error("OpenAPI_ambr_rm_parseFromJSON() failed [uplink]");
        goto end;
    }

    if (!cJSON_IsString(uplink)) {
        ogs_error("OpenAPI_ambr_rm_parseFromJSON() failed [uplink]");
        goto end;
    }

    cJSON *downlink = cJSON_GetObjectItemCaseSensitive(ambr_rmJSON, "downlink");
    if (!downlink) {
        ogs_error("OpenAPI_ambr_rm_parseFromJSON() failed [downlink]");
        goto end;
    }

    if (!cJSON_IsString(downlink)) {
        ogs_error("OpenAPI_ambr_rm_parseFromJSON() failed [downlink]");
        goto end;
    }

    ambr_rm_local_var = OpenAPI_ambr_rm_create (
        ogs_strdup(uplink->valuestring),
        ogs_strdup(downlink->valuestring)
    );

    return ambr_rm_local_var;
end:
    return NULL;
}

OpenAPI_ambr_rm_t *OpenAPI_ambr_rm_copy(OpenAPI_ambr_rm_t *dst, OpenAPI_ambr_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ambr_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ambr_rm_convertToJSON() failed");
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

    OpenAPI_ambr_rm_free(dst);
    dst = OpenAPI_ambr_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

