
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pro_se_authentication_info.h"

OpenAPI_pro_se_authentication_info_t *OpenAPI_pro_se_authentication_info_create(
    char *supi_or_suci,
    char *_5g_pruk_id,
    int relay_service_code,
    bool is_nonce1_null,
    char *nonce1,
    char *supported_features
)
{
    OpenAPI_pro_se_authentication_info_t *pro_se_authentication_info_local_var = ogs_malloc(sizeof(OpenAPI_pro_se_authentication_info_t));
    ogs_assert(pro_se_authentication_info_local_var);

    pro_se_authentication_info_local_var->supi_or_suci = supi_or_suci;
    pro_se_authentication_info_local_var->_5g_pruk_id = _5g_pruk_id;
    pro_se_authentication_info_local_var->relay_service_code = relay_service_code;
    pro_se_authentication_info_local_var->is_nonce1_null = is_nonce1_null;
    pro_se_authentication_info_local_var->nonce1 = nonce1;
    pro_se_authentication_info_local_var->supported_features = supported_features;

    return pro_se_authentication_info_local_var;
}

void OpenAPI_pro_se_authentication_info_free(OpenAPI_pro_se_authentication_info_t *pro_se_authentication_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pro_se_authentication_info) {
        return;
    }
    if (pro_se_authentication_info->supi_or_suci) {
        ogs_free(pro_se_authentication_info->supi_or_suci);
        pro_se_authentication_info->supi_or_suci = NULL;
    }
    if (pro_se_authentication_info->_5g_pruk_id) {
        ogs_free(pro_se_authentication_info->_5g_pruk_id);
        pro_se_authentication_info->_5g_pruk_id = NULL;
    }
    if (pro_se_authentication_info->nonce1) {
        ogs_free(pro_se_authentication_info->nonce1);
        pro_se_authentication_info->nonce1 = NULL;
    }
    if (pro_se_authentication_info->supported_features) {
        ogs_free(pro_se_authentication_info->supported_features);
        pro_se_authentication_info->supported_features = NULL;
    }
    ogs_free(pro_se_authentication_info);
}

cJSON *OpenAPI_pro_se_authentication_info_convertToJSON(OpenAPI_pro_se_authentication_info_t *pro_se_authentication_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pro_se_authentication_info == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_convertToJSON() failed [ProSeAuthenticationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pro_se_authentication_info->supi_or_suci) {
    if (cJSON_AddStringToObject(item, "supiOrSuci", pro_se_authentication_info->supi_or_suci) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_convertToJSON() failed [supi_or_suci]");
        goto end;
    }
    }

    if (pro_se_authentication_info->_5g_pruk_id) {
    if (cJSON_AddStringToObject(item, "5gPrukId", pro_se_authentication_info->_5g_pruk_id) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_convertToJSON() failed [_5g_pruk_id]");
        goto end;
    }
    }

    if (cJSON_AddNumberToObject(item, "relayServiceCode", pro_se_authentication_info->relay_service_code) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_convertToJSON() failed [relay_service_code]");
        goto end;
    }

    if (!pro_se_authentication_info->nonce1) {
        ogs_error("OpenAPI_pro_se_authentication_info_convertToJSON() failed [nonce1]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nonce1", pro_se_authentication_info->nonce1) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_convertToJSON() failed [nonce1]");
        goto end;
    }

    if (pro_se_authentication_info->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", pro_se_authentication_info->supported_features) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pro_se_authentication_info_t *OpenAPI_pro_se_authentication_info_parseFromJSON(cJSON *pro_se_authentication_infoJSON)
{
    OpenAPI_pro_se_authentication_info_t *pro_se_authentication_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi_or_suci = NULL;
    cJSON *_5g_pruk_id = NULL;
    cJSON *relay_service_code = NULL;
    cJSON *nonce1 = NULL;
    cJSON *supported_features = NULL;
    supi_or_suci = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_infoJSON, "supiOrSuci");
    if (supi_or_suci) {
    if (!cJSON_IsString(supi_or_suci) && !cJSON_IsNull(supi_or_suci)) {
        ogs_error("OpenAPI_pro_se_authentication_info_parseFromJSON() failed [supi_or_suci]");
        goto end;
    }
    }

    _5g_pruk_id = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_infoJSON, "5gPrukId");
    if (_5g_pruk_id) {
    if (!cJSON_IsString(_5g_pruk_id) && !cJSON_IsNull(_5g_pruk_id)) {
        ogs_error("OpenAPI_pro_se_authentication_info_parseFromJSON() failed [_5g_pruk_id]");
        goto end;
    }
    }

    relay_service_code = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_infoJSON, "relayServiceCode");
    if (!relay_service_code) {
        ogs_error("OpenAPI_pro_se_authentication_info_parseFromJSON() failed [relay_service_code]");
        goto end;
    }
    if (!cJSON_IsNumber(relay_service_code)) {
        ogs_error("OpenAPI_pro_se_authentication_info_parseFromJSON() failed [relay_service_code]");
        goto end;
    }

    nonce1 = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_infoJSON, "nonce1");
    if (!nonce1) {
        ogs_error("OpenAPI_pro_se_authentication_info_parseFromJSON() failed [nonce1]");
        goto end;
    }
    if (!cJSON_IsString(nonce1)) {
        ogs_error("OpenAPI_pro_se_authentication_info_parseFromJSON() failed [nonce1]");
        goto end;
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_infoJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_pro_se_authentication_info_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    pro_se_authentication_info_local_var = OpenAPI_pro_se_authentication_info_create (
        supi_or_suci && !cJSON_IsNull(supi_or_suci) ? ogs_strdup(supi_or_suci->valuestring) : NULL,
        _5g_pruk_id && !cJSON_IsNull(_5g_pruk_id) ? ogs_strdup(_5g_pruk_id->valuestring) : NULL,
        
        relay_service_code->valuedouble,
        nonce1 && cJSON_IsNull(nonce1) ? true : false,
        ogs_strdup(nonce1->valuestring),
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return pro_se_authentication_info_local_var;
end:
    return NULL;
}

OpenAPI_pro_se_authentication_info_t *OpenAPI_pro_se_authentication_info_copy(OpenAPI_pro_se_authentication_info_t *dst, OpenAPI_pro_se_authentication_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pro_se_authentication_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pro_se_authentication_info_convertToJSON() failed");
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

    OpenAPI_pro_se_authentication_info_free(dst);
    dst = OpenAPI_pro_se_authentication_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

