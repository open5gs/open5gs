
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pro_se_authentication_result.h"

OpenAPI_pro_se_authentication_result_t *OpenAPI_pro_se_authentication_result_create(
    char *knr_pro_se,
    bool is_nonce2_null,
    char *nonce2,
    char *supported_features
)
{
    OpenAPI_pro_se_authentication_result_t *pro_se_authentication_result_local_var = ogs_malloc(sizeof(OpenAPI_pro_se_authentication_result_t));
    ogs_assert(pro_se_authentication_result_local_var);

    pro_se_authentication_result_local_var->knr_pro_se = knr_pro_se;
    pro_se_authentication_result_local_var->is_nonce2_null = is_nonce2_null;
    pro_se_authentication_result_local_var->nonce2 = nonce2;
    pro_se_authentication_result_local_var->supported_features = supported_features;

    return pro_se_authentication_result_local_var;
}

void OpenAPI_pro_se_authentication_result_free(OpenAPI_pro_se_authentication_result_t *pro_se_authentication_result)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pro_se_authentication_result) {
        return;
    }
    if (pro_se_authentication_result->knr_pro_se) {
        ogs_free(pro_se_authentication_result->knr_pro_se);
        pro_se_authentication_result->knr_pro_se = NULL;
    }
    if (pro_se_authentication_result->nonce2) {
        ogs_free(pro_se_authentication_result->nonce2);
        pro_se_authentication_result->nonce2 = NULL;
    }
    if (pro_se_authentication_result->supported_features) {
        ogs_free(pro_se_authentication_result->supported_features);
        pro_se_authentication_result->supported_features = NULL;
    }
    ogs_free(pro_se_authentication_result);
}

cJSON *OpenAPI_pro_se_authentication_result_convertToJSON(OpenAPI_pro_se_authentication_result_t *pro_se_authentication_result)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pro_se_authentication_result == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_result_convertToJSON() failed [ProSeAuthenticationResult]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pro_se_authentication_result->knr_pro_se) {
    if (cJSON_AddStringToObject(item, "knrProSe", pro_se_authentication_result->knr_pro_se) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_result_convertToJSON() failed [knr_pro_se]");
        goto end;
    }
    }

    if (pro_se_authentication_result->nonce2) {
    if (cJSON_AddStringToObject(item, "nonce2", pro_se_authentication_result->nonce2) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_result_convertToJSON() failed [nonce2]");
        goto end;
    }
    } else if (pro_se_authentication_result->is_nonce2_null) {
        if (cJSON_AddNullToObject(item, "nonce2") == NULL) {
            ogs_error("OpenAPI_pro_se_authentication_result_convertToJSON() failed [nonce2]");
            goto end;
        }
    }

    if (pro_se_authentication_result->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", pro_se_authentication_result->supported_features) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_result_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pro_se_authentication_result_t *OpenAPI_pro_se_authentication_result_parseFromJSON(cJSON *pro_se_authentication_resultJSON)
{
    OpenAPI_pro_se_authentication_result_t *pro_se_authentication_result_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *knr_pro_se = NULL;
    cJSON *nonce2 = NULL;
    cJSON *supported_features = NULL;
    knr_pro_se = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_resultJSON, "knrProSe");
    if (knr_pro_se) {
    if (!cJSON_IsString(knr_pro_se) && !cJSON_IsNull(knr_pro_se)) {
        ogs_error("OpenAPI_pro_se_authentication_result_parseFromJSON() failed [knr_pro_se]");
        goto end;
    }
    }

    nonce2 = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_resultJSON, "nonce2");
    if (nonce2) {
    if (!cJSON_IsNull(nonce2)) {
    if (!cJSON_IsString(nonce2) && !cJSON_IsNull(nonce2)) {
        ogs_error("OpenAPI_pro_se_authentication_result_parseFromJSON() failed [nonce2]");
        goto end;
    }
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_resultJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_pro_se_authentication_result_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    pro_se_authentication_result_local_var = OpenAPI_pro_se_authentication_result_create (
        knr_pro_se && !cJSON_IsNull(knr_pro_se) ? ogs_strdup(knr_pro_se->valuestring) : NULL,
        nonce2 && cJSON_IsNull(nonce2) ? true : false,
        nonce2 && !cJSON_IsNull(nonce2) ? ogs_strdup(nonce2->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return pro_se_authentication_result_local_var;
end:
    return NULL;
}

OpenAPI_pro_se_authentication_result_t *OpenAPI_pro_se_authentication_result_copy(OpenAPI_pro_se_authentication_result_t *dst, OpenAPI_pro_se_authentication_result_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pro_se_authentication_result_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pro_se_authentication_result_convertToJSON() failed");
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

    OpenAPI_pro_se_authentication_result_free(dst);
    dst = OpenAPI_pro_se_authentication_result_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

