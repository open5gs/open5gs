
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "deregistration_info.h"

OpenAPI_deregistration_info_t *OpenAPI_deregistration_info_create(
    char *supi,
    char *supported_features
)
{
    OpenAPI_deregistration_info_t *deregistration_info_local_var = ogs_malloc(sizeof(OpenAPI_deregistration_info_t));
    ogs_assert(deregistration_info_local_var);

    deregistration_info_local_var->supi = supi;
    deregistration_info_local_var->supported_features = supported_features;

    return deregistration_info_local_var;
}

void OpenAPI_deregistration_info_free(OpenAPI_deregistration_info_t *deregistration_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == deregistration_info) {
        return;
    }
    if (deregistration_info->supi) {
        ogs_free(deregistration_info->supi);
        deregistration_info->supi = NULL;
    }
    if (deregistration_info->supported_features) {
        ogs_free(deregistration_info->supported_features);
        deregistration_info->supported_features = NULL;
    }
    ogs_free(deregistration_info);
}

cJSON *OpenAPI_deregistration_info_convertToJSON(OpenAPI_deregistration_info_t *deregistration_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (deregistration_info == NULL) {
        ogs_error("OpenAPI_deregistration_info_convertToJSON() failed [DeregistrationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!deregistration_info->supi) {
        ogs_error("OpenAPI_deregistration_info_convertToJSON() failed [supi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supi", deregistration_info->supi) == NULL) {
        ogs_error("OpenAPI_deregistration_info_convertToJSON() failed [supi]");
        goto end;
    }

    if (deregistration_info->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", deregistration_info->supported_features) == NULL) {
        ogs_error("OpenAPI_deregistration_info_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_deregistration_info_t *OpenAPI_deregistration_info_parseFromJSON(cJSON *deregistration_infoJSON)
{
    OpenAPI_deregistration_info_t *deregistration_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *supported_features = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(deregistration_infoJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_deregistration_info_parseFromJSON() failed [supi]");
        goto end;
    }
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_deregistration_info_parseFromJSON() failed [supi]");
        goto end;
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(deregistration_infoJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_deregistration_info_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    deregistration_info_local_var = OpenAPI_deregistration_info_create (
        ogs_strdup(supi->valuestring),
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return deregistration_info_local_var;
end:
    return NULL;
}

OpenAPI_deregistration_info_t *OpenAPI_deregistration_info_copy(OpenAPI_deregistration_info_t *dst, OpenAPI_deregistration_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_deregistration_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_deregistration_info_convertToJSON() failed");
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

    OpenAPI_deregistration_info_free(dst);
    dst = OpenAPI_deregistration_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

