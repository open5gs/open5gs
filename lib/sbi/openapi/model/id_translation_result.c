
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "id_translation_result.h"

OpenAPI_id_translation_result_t *OpenAPI_id_translation_result_create(
    char *supported_features,
    char *supi,
    char *gpsi
)
{
    OpenAPI_id_translation_result_t *id_translation_result_local_var = ogs_malloc(sizeof(OpenAPI_id_translation_result_t));
    ogs_assert(id_translation_result_local_var);

    id_translation_result_local_var->supported_features = supported_features;
    id_translation_result_local_var->supi = supi;
    id_translation_result_local_var->gpsi = gpsi;

    return id_translation_result_local_var;
}

void OpenAPI_id_translation_result_free(OpenAPI_id_translation_result_t *id_translation_result)
{
    if (NULL == id_translation_result) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(id_translation_result->supported_features);
    ogs_free(id_translation_result->supi);
    ogs_free(id_translation_result->gpsi);
    ogs_free(id_translation_result);
}

cJSON *OpenAPI_id_translation_result_convertToJSON(OpenAPI_id_translation_result_t *id_translation_result)
{
    cJSON *item = NULL;

    if (id_translation_result == NULL) {
        ogs_error("OpenAPI_id_translation_result_convertToJSON() failed [IdTranslationResult]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (id_translation_result->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", id_translation_result->supported_features) == NULL) {
        ogs_error("OpenAPI_id_translation_result_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (cJSON_AddStringToObject(item, "supi", id_translation_result->supi) == NULL) {
        ogs_error("OpenAPI_id_translation_result_convertToJSON() failed [supi]");
        goto end;
    }

    if (id_translation_result->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", id_translation_result->gpsi) == NULL) {
        ogs_error("OpenAPI_id_translation_result_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_id_translation_result_t *OpenAPI_id_translation_result_parseFromJSON(cJSON *id_translation_resultJSON)
{
    OpenAPI_id_translation_result_t *id_translation_result_local_var = NULL;
    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(id_translation_resultJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_id_translation_result_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *supi = cJSON_GetObjectItemCaseSensitive(id_translation_resultJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_id_translation_result_parseFromJSON() failed [supi]");
        goto end;
    }

    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_id_translation_result_parseFromJSON() failed [supi]");
        goto end;
    }

    cJSON *gpsi = cJSON_GetObjectItemCaseSensitive(id_translation_resultJSON, "gpsi");

    if (gpsi) {
    if (!cJSON_IsString(gpsi)) {
        ogs_error("OpenAPI_id_translation_result_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    id_translation_result_local_var = OpenAPI_id_translation_result_create (
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        ogs_strdup(supi->valuestring),
        gpsi ? ogs_strdup(gpsi->valuestring) : NULL
    );

    return id_translation_result_local_var;
end:
    return NULL;
}

OpenAPI_id_translation_result_t *OpenAPI_id_translation_result_copy(OpenAPI_id_translation_result_t *dst, OpenAPI_id_translation_result_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_id_translation_result_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_id_translation_result_convertToJSON() failed");
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

    OpenAPI_id_translation_result_free(dst);
    dst = OpenAPI_id_translation_result_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

