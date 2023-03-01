
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "allowed_mtc_provider_info.h"

OpenAPI_allowed_mtc_provider_info_t *OpenAPI_allowed_mtc_provider_info_create(
    char *mtc_provider_information,
    char *af_id
)
{
    OpenAPI_allowed_mtc_provider_info_t *allowed_mtc_provider_info_local_var = ogs_malloc(sizeof(OpenAPI_allowed_mtc_provider_info_t));
    ogs_assert(allowed_mtc_provider_info_local_var);

    allowed_mtc_provider_info_local_var->mtc_provider_information = mtc_provider_information;
    allowed_mtc_provider_info_local_var->af_id = af_id;

    return allowed_mtc_provider_info_local_var;
}

void OpenAPI_allowed_mtc_provider_info_free(OpenAPI_allowed_mtc_provider_info_t *allowed_mtc_provider_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == allowed_mtc_provider_info) {
        return;
    }
    if (allowed_mtc_provider_info->mtc_provider_information) {
        ogs_free(allowed_mtc_provider_info->mtc_provider_information);
        allowed_mtc_provider_info->mtc_provider_information = NULL;
    }
    if (allowed_mtc_provider_info->af_id) {
        ogs_free(allowed_mtc_provider_info->af_id);
        allowed_mtc_provider_info->af_id = NULL;
    }
    ogs_free(allowed_mtc_provider_info);
}

cJSON *OpenAPI_allowed_mtc_provider_info_convertToJSON(OpenAPI_allowed_mtc_provider_info_t *allowed_mtc_provider_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (allowed_mtc_provider_info == NULL) {
        ogs_error("OpenAPI_allowed_mtc_provider_info_convertToJSON() failed [AllowedMtcProviderInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (allowed_mtc_provider_info->mtc_provider_information) {
    if (cJSON_AddStringToObject(item, "mtcProviderInformation", allowed_mtc_provider_info->mtc_provider_information) == NULL) {
        ogs_error("OpenAPI_allowed_mtc_provider_info_convertToJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    if (allowed_mtc_provider_info->af_id) {
    if (cJSON_AddStringToObject(item, "afId", allowed_mtc_provider_info->af_id) == NULL) {
        ogs_error("OpenAPI_allowed_mtc_provider_info_convertToJSON() failed [af_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_allowed_mtc_provider_info_t *OpenAPI_allowed_mtc_provider_info_parseFromJSON(cJSON *allowed_mtc_provider_infoJSON)
{
    OpenAPI_allowed_mtc_provider_info_t *allowed_mtc_provider_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mtc_provider_information = NULL;
    cJSON *af_id = NULL;
    mtc_provider_information = cJSON_GetObjectItemCaseSensitive(allowed_mtc_provider_infoJSON, "mtcProviderInformation");
    if (mtc_provider_information) {
    if (!cJSON_IsString(mtc_provider_information) && !cJSON_IsNull(mtc_provider_information)) {
        ogs_error("OpenAPI_allowed_mtc_provider_info_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    af_id = cJSON_GetObjectItemCaseSensitive(allowed_mtc_provider_infoJSON, "afId");
    if (af_id) {
    if (!cJSON_IsString(af_id) && !cJSON_IsNull(af_id)) {
        ogs_error("OpenAPI_allowed_mtc_provider_info_parseFromJSON() failed [af_id]");
        goto end;
    }
    }

    allowed_mtc_provider_info_local_var = OpenAPI_allowed_mtc_provider_info_create (
        mtc_provider_information && !cJSON_IsNull(mtc_provider_information) ? ogs_strdup(mtc_provider_information->valuestring) : NULL,
        af_id && !cJSON_IsNull(af_id) ? ogs_strdup(af_id->valuestring) : NULL
    );

    return allowed_mtc_provider_info_local_var;
end:
    return NULL;
}

OpenAPI_allowed_mtc_provider_info_t *OpenAPI_allowed_mtc_provider_info_copy(OpenAPI_allowed_mtc_provider_info_t *dst, OpenAPI_allowed_mtc_provider_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_allowed_mtc_provider_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_allowed_mtc_provider_info_convertToJSON() failed");
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

    OpenAPI_allowed_mtc_provider_info_free(dst);
    dst = OpenAPI_allowed_mtc_provider_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

