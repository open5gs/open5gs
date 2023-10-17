
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_mbr.h"

OpenAPI_slice_mbr_t *OpenAPI_slice_mbr_create(
    char *uplink,
    char *downlink
)
{
    OpenAPI_slice_mbr_t *slice_mbr_local_var = ogs_malloc(sizeof(OpenAPI_slice_mbr_t));
    ogs_assert(slice_mbr_local_var);

    slice_mbr_local_var->uplink = uplink;
    slice_mbr_local_var->downlink = downlink;

    return slice_mbr_local_var;
}

void OpenAPI_slice_mbr_free(OpenAPI_slice_mbr_t *slice_mbr)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == slice_mbr) {
        return;
    }
    if (slice_mbr->uplink) {
        ogs_free(slice_mbr->uplink);
        slice_mbr->uplink = NULL;
    }
    if (slice_mbr->downlink) {
        ogs_free(slice_mbr->downlink);
        slice_mbr->downlink = NULL;
    }
    ogs_free(slice_mbr);
}

cJSON *OpenAPI_slice_mbr_convertToJSON(OpenAPI_slice_mbr_t *slice_mbr)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (slice_mbr == NULL) {
        ogs_error("OpenAPI_slice_mbr_convertToJSON() failed [SliceMbr]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!slice_mbr->uplink) {
        ogs_error("OpenAPI_slice_mbr_convertToJSON() failed [uplink]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "uplink", slice_mbr->uplink) == NULL) {
        ogs_error("OpenAPI_slice_mbr_convertToJSON() failed [uplink]");
        goto end;
    }

    if (!slice_mbr->downlink) {
        ogs_error("OpenAPI_slice_mbr_convertToJSON() failed [downlink]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "downlink", slice_mbr->downlink) == NULL) {
        ogs_error("OpenAPI_slice_mbr_convertToJSON() failed [downlink]");
        goto end;
    }

end:
    return item;
}

OpenAPI_slice_mbr_t *OpenAPI_slice_mbr_parseFromJSON(cJSON *slice_mbrJSON)
{
    OpenAPI_slice_mbr_t *slice_mbr_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *uplink = NULL;
    cJSON *downlink = NULL;
    uplink = cJSON_GetObjectItemCaseSensitive(slice_mbrJSON, "uplink");
    if (!uplink) {
        ogs_error("OpenAPI_slice_mbr_parseFromJSON() failed [uplink]");
        goto end;
    }
    if (!cJSON_IsString(uplink)) {
        ogs_error("OpenAPI_slice_mbr_parseFromJSON() failed [uplink]");
        goto end;
    }

    downlink = cJSON_GetObjectItemCaseSensitive(slice_mbrJSON, "downlink");
    if (!downlink) {
        ogs_error("OpenAPI_slice_mbr_parseFromJSON() failed [downlink]");
        goto end;
    }
    if (!cJSON_IsString(downlink)) {
        ogs_error("OpenAPI_slice_mbr_parseFromJSON() failed [downlink]");
        goto end;
    }

    slice_mbr_local_var = OpenAPI_slice_mbr_create (
        ogs_strdup(uplink->valuestring),
        ogs_strdup(downlink->valuestring)
    );

    return slice_mbr_local_var;
end:
    return NULL;
}

OpenAPI_slice_mbr_t *OpenAPI_slice_mbr_copy(OpenAPI_slice_mbr_t *dst, OpenAPI_slice_mbr_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_mbr_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_mbr_convertToJSON() failed");
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

    OpenAPI_slice_mbr_free(dst);
    dst = OpenAPI_slice_mbr_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

