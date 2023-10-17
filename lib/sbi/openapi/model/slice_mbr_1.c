
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_mbr_1.h"

OpenAPI_slice_mbr_1_t *OpenAPI_slice_mbr_1_create(
    char *uplink,
    char *downlink
)
{
    OpenAPI_slice_mbr_1_t *slice_mbr_1_local_var = ogs_malloc(sizeof(OpenAPI_slice_mbr_1_t));
    ogs_assert(slice_mbr_1_local_var);

    slice_mbr_1_local_var->uplink = uplink;
    slice_mbr_1_local_var->downlink = downlink;

    return slice_mbr_1_local_var;
}

void OpenAPI_slice_mbr_1_free(OpenAPI_slice_mbr_1_t *slice_mbr_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == slice_mbr_1) {
        return;
    }
    if (slice_mbr_1->uplink) {
        ogs_free(slice_mbr_1->uplink);
        slice_mbr_1->uplink = NULL;
    }
    if (slice_mbr_1->downlink) {
        ogs_free(slice_mbr_1->downlink);
        slice_mbr_1->downlink = NULL;
    }
    ogs_free(slice_mbr_1);
}

cJSON *OpenAPI_slice_mbr_1_convertToJSON(OpenAPI_slice_mbr_1_t *slice_mbr_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (slice_mbr_1 == NULL) {
        ogs_error("OpenAPI_slice_mbr_1_convertToJSON() failed [SliceMbr_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!slice_mbr_1->uplink) {
        ogs_error("OpenAPI_slice_mbr_1_convertToJSON() failed [uplink]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "uplink", slice_mbr_1->uplink) == NULL) {
        ogs_error("OpenAPI_slice_mbr_1_convertToJSON() failed [uplink]");
        goto end;
    }

    if (!slice_mbr_1->downlink) {
        ogs_error("OpenAPI_slice_mbr_1_convertToJSON() failed [downlink]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "downlink", slice_mbr_1->downlink) == NULL) {
        ogs_error("OpenAPI_slice_mbr_1_convertToJSON() failed [downlink]");
        goto end;
    }

end:
    return item;
}

OpenAPI_slice_mbr_1_t *OpenAPI_slice_mbr_1_parseFromJSON(cJSON *slice_mbr_1JSON)
{
    OpenAPI_slice_mbr_1_t *slice_mbr_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *uplink = NULL;
    cJSON *downlink = NULL;
    uplink = cJSON_GetObjectItemCaseSensitive(slice_mbr_1JSON, "uplink");
    if (!uplink) {
        ogs_error("OpenAPI_slice_mbr_1_parseFromJSON() failed [uplink]");
        goto end;
    }
    if (!cJSON_IsString(uplink)) {
        ogs_error("OpenAPI_slice_mbr_1_parseFromJSON() failed [uplink]");
        goto end;
    }

    downlink = cJSON_GetObjectItemCaseSensitive(slice_mbr_1JSON, "downlink");
    if (!downlink) {
        ogs_error("OpenAPI_slice_mbr_1_parseFromJSON() failed [downlink]");
        goto end;
    }
    if (!cJSON_IsString(downlink)) {
        ogs_error("OpenAPI_slice_mbr_1_parseFromJSON() failed [downlink]");
        goto end;
    }

    slice_mbr_1_local_var = OpenAPI_slice_mbr_1_create (
        ogs_strdup(uplink->valuestring),
        ogs_strdup(downlink->valuestring)
    );

    return slice_mbr_1_local_var;
end:
    return NULL;
}

OpenAPI_slice_mbr_1_t *OpenAPI_slice_mbr_1_copy(OpenAPI_slice_mbr_1_t *dst, OpenAPI_slice_mbr_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_mbr_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_mbr_1_convertToJSON() failed");
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

    OpenAPI_slice_mbr_1_free(dst);
    dst = OpenAPI_slice_mbr_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

