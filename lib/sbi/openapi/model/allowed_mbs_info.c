
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "allowed_mbs_info.h"

OpenAPI_allowed_mbs_info_t *OpenAPI_allowed_mbs_info_create(
    char *af_id
)
{
    OpenAPI_allowed_mbs_info_t *allowed_mbs_info_local_var = ogs_malloc(sizeof(OpenAPI_allowed_mbs_info_t));
    ogs_assert(allowed_mbs_info_local_var);

    allowed_mbs_info_local_var->af_id = af_id;

    return allowed_mbs_info_local_var;
}

void OpenAPI_allowed_mbs_info_free(OpenAPI_allowed_mbs_info_t *allowed_mbs_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == allowed_mbs_info) {
        return;
    }
    if (allowed_mbs_info->af_id) {
        ogs_free(allowed_mbs_info->af_id);
        allowed_mbs_info->af_id = NULL;
    }
    ogs_free(allowed_mbs_info);
}

cJSON *OpenAPI_allowed_mbs_info_convertToJSON(OpenAPI_allowed_mbs_info_t *allowed_mbs_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (allowed_mbs_info == NULL) {
        ogs_error("OpenAPI_allowed_mbs_info_convertToJSON() failed [AllowedMbsInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (allowed_mbs_info->af_id) {
    if (cJSON_AddStringToObject(item, "afId", allowed_mbs_info->af_id) == NULL) {
        ogs_error("OpenAPI_allowed_mbs_info_convertToJSON() failed [af_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_allowed_mbs_info_t *OpenAPI_allowed_mbs_info_parseFromJSON(cJSON *allowed_mbs_infoJSON)
{
    OpenAPI_allowed_mbs_info_t *allowed_mbs_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_id = NULL;
    af_id = cJSON_GetObjectItemCaseSensitive(allowed_mbs_infoJSON, "afId");
    if (af_id) {
    if (!cJSON_IsString(af_id) && !cJSON_IsNull(af_id)) {
        ogs_error("OpenAPI_allowed_mbs_info_parseFromJSON() failed [af_id]");
        goto end;
    }
    }

    allowed_mbs_info_local_var = OpenAPI_allowed_mbs_info_create (
        af_id && !cJSON_IsNull(af_id) ? ogs_strdup(af_id->valuestring) : NULL
    );

    return allowed_mbs_info_local_var;
end:
    return NULL;
}

OpenAPI_allowed_mbs_info_t *OpenAPI_allowed_mbs_info_copy(OpenAPI_allowed_mbs_info_t *dst, OpenAPI_allowed_mbs_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_allowed_mbs_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_allowed_mbs_info_convertToJSON() failed");
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

    OpenAPI_allowed_mbs_info_free(dst);
    dst = OpenAPI_allowed_mbs_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

