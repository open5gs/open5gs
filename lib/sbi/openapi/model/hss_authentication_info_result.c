
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hss_authentication_info_result.h"

OpenAPI_hss_authentication_info_result_t *OpenAPI_hss_authentication_info_result_create(
    char *supported_features,
    OpenAPI_hss_authentication_vectors_t *hss_authentication_vectors
)
{
    OpenAPI_hss_authentication_info_result_t *hss_authentication_info_result_local_var = ogs_malloc(sizeof(OpenAPI_hss_authentication_info_result_t));
    ogs_assert(hss_authentication_info_result_local_var);

    hss_authentication_info_result_local_var->supported_features = supported_features;
    hss_authentication_info_result_local_var->hss_authentication_vectors = hss_authentication_vectors;

    return hss_authentication_info_result_local_var;
}

void OpenAPI_hss_authentication_info_result_free(OpenAPI_hss_authentication_info_result_t *hss_authentication_info_result)
{
    if (NULL == hss_authentication_info_result) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(hss_authentication_info_result->supported_features);
    OpenAPI_hss_authentication_vectors_free(hss_authentication_info_result->hss_authentication_vectors);
    ogs_free(hss_authentication_info_result);
}

cJSON *OpenAPI_hss_authentication_info_result_convertToJSON(OpenAPI_hss_authentication_info_result_t *hss_authentication_info_result)
{
    cJSON *item = NULL;

    if (hss_authentication_info_result == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_result_convertToJSON() failed [HssAuthenticationInfoResult]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (hss_authentication_info_result->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", hss_authentication_info_result->supported_features) == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_result_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *hss_authentication_vectors_local_JSON = OpenAPI_hss_authentication_vectors_convertToJSON(hss_authentication_info_result->hss_authentication_vectors);
    if (hss_authentication_vectors_local_JSON == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_result_convertToJSON() failed [hss_authentication_vectors]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hssAuthenticationVectors", hss_authentication_vectors_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hss_authentication_info_result_convertToJSON() failed [hss_authentication_vectors]");
        goto end;
    }

end:
    return item;
}

OpenAPI_hss_authentication_info_result_t *OpenAPI_hss_authentication_info_result_parseFromJSON(cJSON *hss_authentication_info_resultJSON)
{
    OpenAPI_hss_authentication_info_result_t *hss_authentication_info_result_local_var = NULL;
    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(hss_authentication_info_resultJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_hss_authentication_info_result_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *hss_authentication_vectors = cJSON_GetObjectItemCaseSensitive(hss_authentication_info_resultJSON, "hssAuthenticationVectors");
    if (!hss_authentication_vectors) {
        ogs_error("OpenAPI_hss_authentication_info_result_parseFromJSON() failed [hss_authentication_vectors]");
        goto end;
    }

    OpenAPI_hss_authentication_vectors_t *hss_authentication_vectors_local_nonprim = NULL;
    hss_authentication_vectors_local_nonprim = OpenAPI_hss_authentication_vectors_parseFromJSON(hss_authentication_vectors);

    hss_authentication_info_result_local_var = OpenAPI_hss_authentication_info_result_create (
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        hss_authentication_vectors_local_nonprim
    );

    return hss_authentication_info_result_local_var;
end:
    return NULL;
}

OpenAPI_hss_authentication_info_result_t *OpenAPI_hss_authentication_info_result_copy(OpenAPI_hss_authentication_info_result_t *dst, OpenAPI_hss_authentication_info_result_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_hss_authentication_info_result_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_hss_authentication_info_result_convertToJSON() failed");
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

    OpenAPI_hss_authentication_info_result_free(dst);
    dst = OpenAPI_hss_authentication_info_result_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

