
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ambr.h"

OpenAPI_ambr_t *OpenAPI_ambr_create(
    char *uplink,
    char *downlink
    )
{
    OpenAPI_ambr_t *ambr_local_var = OpenAPI_malloc(sizeof(OpenAPI_ambr_t));
    if (!ambr_local_var) {
        return NULL;
    }
    ambr_local_var->uplink = uplink;
    ambr_local_var->downlink = downlink;

    return ambr_local_var;
}

void OpenAPI_ambr_free(OpenAPI_ambr_t *ambr)
{
    if (NULL == ambr) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ambr->uplink);
    ogs_free(ambr->downlink);
    ogs_free(ambr);
}

cJSON *OpenAPI_ambr_convertToJSON(OpenAPI_ambr_t *ambr)
{
    cJSON *item = NULL;

    if (ambr == NULL) {
        ogs_error("OpenAPI_ambr_convertToJSON() failed [Ambr]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ambr->uplink) {
        ogs_error("OpenAPI_ambr_convertToJSON() failed [uplink]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "uplink", ambr->uplink) == NULL) {
        ogs_error("OpenAPI_ambr_convertToJSON() failed [uplink]");
        goto end;
    }

    if (!ambr->downlink) {
        ogs_error("OpenAPI_ambr_convertToJSON() failed [downlink]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "downlink", ambr->downlink) == NULL) {
        ogs_error("OpenAPI_ambr_convertToJSON() failed [downlink]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ambr_t *OpenAPI_ambr_parseFromJSON(cJSON *ambrJSON)
{
    OpenAPI_ambr_t *ambr_local_var = NULL;
    cJSON *uplink = cJSON_GetObjectItemCaseSensitive(ambrJSON, "uplink");
    if (!uplink) {
        ogs_error("OpenAPI_ambr_parseFromJSON() failed [uplink]");
        goto end;
    }


    if (!cJSON_IsString(uplink)) {
        ogs_error("OpenAPI_ambr_parseFromJSON() failed [uplink]");
        goto end;
    }

    cJSON *downlink = cJSON_GetObjectItemCaseSensitive(ambrJSON, "downlink");
    if (!downlink) {
        ogs_error("OpenAPI_ambr_parseFromJSON() failed [downlink]");
        goto end;
    }


    if (!cJSON_IsString(downlink)) {
        ogs_error("OpenAPI_ambr_parseFromJSON() failed [downlink]");
        goto end;
    }

    ambr_local_var = OpenAPI_ambr_create (
        ogs_strdup(uplink->valuestring),
        ogs_strdup(downlink->valuestring)
        );

    return ambr_local_var;
end:
    return NULL;
}

