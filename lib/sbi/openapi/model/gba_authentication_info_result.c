
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gba_authentication_info_result.h"

OpenAPI_gba_authentication_info_result_t *OpenAPI_gba_authentication_info_result_create(
    OpenAPI_model_3_g_aka_av_t *_3g_aka_av,
    char *supported_features
)
{
    OpenAPI_gba_authentication_info_result_t *gba_authentication_info_result_local_var = ogs_malloc(sizeof(OpenAPI_gba_authentication_info_result_t));
    ogs_assert(gba_authentication_info_result_local_var);

    gba_authentication_info_result_local_var->_3g_aka_av = _3g_aka_av;
    gba_authentication_info_result_local_var->supported_features = supported_features;

    return gba_authentication_info_result_local_var;
}

void OpenAPI_gba_authentication_info_result_free(OpenAPI_gba_authentication_info_result_t *gba_authentication_info_result)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == gba_authentication_info_result) {
        return;
    }
    if (gba_authentication_info_result->_3g_aka_av) {
        OpenAPI_model_3_g_aka_av_free(gba_authentication_info_result->_3g_aka_av);
        gba_authentication_info_result->_3g_aka_av = NULL;
    }
    if (gba_authentication_info_result->supported_features) {
        ogs_free(gba_authentication_info_result->supported_features);
        gba_authentication_info_result->supported_features = NULL;
    }
    ogs_free(gba_authentication_info_result);
}

cJSON *OpenAPI_gba_authentication_info_result_convertToJSON(OpenAPI_gba_authentication_info_result_t *gba_authentication_info_result)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (gba_authentication_info_result == NULL) {
        ogs_error("OpenAPI_gba_authentication_info_result_convertToJSON() failed [GbaAuthenticationInfoResult]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (gba_authentication_info_result->_3g_aka_av) {
    cJSON *_3g_aka_av_local_JSON = OpenAPI_model_3_g_aka_av_convertToJSON(gba_authentication_info_result->_3g_aka_av);
    if (_3g_aka_av_local_JSON == NULL) {
        ogs_error("OpenAPI_gba_authentication_info_result_convertToJSON() failed [_3g_aka_av]");
        goto end;
    }
    cJSON_AddItemToObject(item, "3gAkaAv", _3g_aka_av_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_gba_authentication_info_result_convertToJSON() failed [_3g_aka_av]");
        goto end;
    }
    }

    if (gba_authentication_info_result->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", gba_authentication_info_result->supported_features) == NULL) {
        ogs_error("OpenAPI_gba_authentication_info_result_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_gba_authentication_info_result_t *OpenAPI_gba_authentication_info_result_parseFromJSON(cJSON *gba_authentication_info_resultJSON)
{
    OpenAPI_gba_authentication_info_result_t *gba_authentication_info_result_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *_3g_aka_av = NULL;
    OpenAPI_model_3_g_aka_av_t *_3g_aka_av_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    _3g_aka_av = cJSON_GetObjectItemCaseSensitive(gba_authentication_info_resultJSON, "3gAkaAv");
    if (_3g_aka_av) {
    _3g_aka_av_local_nonprim = OpenAPI_model_3_g_aka_av_parseFromJSON(_3g_aka_av);
    if (!_3g_aka_av_local_nonprim) {
        ogs_error("OpenAPI_model_3_g_aka_av_parseFromJSON failed [_3g_aka_av]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(gba_authentication_info_resultJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_gba_authentication_info_result_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    gba_authentication_info_result_local_var = OpenAPI_gba_authentication_info_result_create (
        _3g_aka_av ? _3g_aka_av_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return gba_authentication_info_result_local_var;
end:
    if (_3g_aka_av_local_nonprim) {
        OpenAPI_model_3_g_aka_av_free(_3g_aka_av_local_nonprim);
        _3g_aka_av_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_gba_authentication_info_result_t *OpenAPI_gba_authentication_info_result_copy(OpenAPI_gba_authentication_info_result_t *dst, OpenAPI_gba_authentication_info_result_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_gba_authentication_info_result_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_gba_authentication_info_result_convertToJSON() failed");
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

    OpenAPI_gba_authentication_info_result_free(dst);
    dst = OpenAPI_gba_authentication_info_result_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

