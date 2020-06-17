
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tmbr.h"

OpenAPI_tmbr_t *OpenAPI_tmbr_create(
    char *uplink,
    char *downlink
    )
{
    OpenAPI_tmbr_t *tmbr_local_var = OpenAPI_malloc(sizeof(OpenAPI_tmbr_t));
    if (!tmbr_local_var) {
        return NULL;
    }
    tmbr_local_var->uplink = uplink;
    tmbr_local_var->downlink = downlink;

    return tmbr_local_var;
}

void OpenAPI_tmbr_free(OpenAPI_tmbr_t *tmbr)
{
    if (NULL == tmbr) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(tmbr->uplink);
    ogs_free(tmbr->downlink);
    ogs_free(tmbr);
}

cJSON *OpenAPI_tmbr_convertToJSON(OpenAPI_tmbr_t *tmbr)
{
    cJSON *item = NULL;

    if (tmbr == NULL) {
        ogs_error("OpenAPI_tmbr_convertToJSON() failed [Tmbr]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!tmbr->uplink) {
        ogs_error("OpenAPI_tmbr_convertToJSON() failed [uplink]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "uplink", tmbr->uplink) == NULL) {
        ogs_error("OpenAPI_tmbr_convertToJSON() failed [uplink]");
        goto end;
    }

    if (!tmbr->downlink) {
        ogs_error("OpenAPI_tmbr_convertToJSON() failed [downlink]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "downlink", tmbr->downlink) == NULL) {
        ogs_error("OpenAPI_tmbr_convertToJSON() failed [downlink]");
        goto end;
    }

end:
    return item;
}

OpenAPI_tmbr_t *OpenAPI_tmbr_parseFromJSON(cJSON *tmbrJSON)
{
    OpenAPI_tmbr_t *tmbr_local_var = NULL;
    cJSON *uplink = cJSON_GetObjectItemCaseSensitive(tmbrJSON, "uplink");
    if (!uplink) {
        ogs_error("OpenAPI_tmbr_parseFromJSON() failed [uplink]");
        goto end;
    }


    if (!cJSON_IsString(uplink)) {
        ogs_error("OpenAPI_tmbr_parseFromJSON() failed [uplink]");
        goto end;
    }

    cJSON *downlink = cJSON_GetObjectItemCaseSensitive(tmbrJSON, "downlink");
    if (!downlink) {
        ogs_error("OpenAPI_tmbr_parseFromJSON() failed [downlink]");
        goto end;
    }


    if (!cJSON_IsString(downlink)) {
        ogs_error("OpenAPI_tmbr_parseFromJSON() failed [downlink]");
        goto end;
    }

    tmbr_local_var = OpenAPI_tmbr_create (
        ogs_strdup(uplink->valuestring),
        ogs_strdup(downlink->valuestring)
        );

    return tmbr_local_var;
end:
    return NULL;
}

OpenAPI_tmbr_t *OpenAPI_tmbr_copy(OpenAPI_tmbr_t *dst, OpenAPI_tmbr_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tmbr_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tmbr_convertToJSON() failed");
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

    OpenAPI_tmbr_free(dst);
    dst = OpenAPI_tmbr_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

