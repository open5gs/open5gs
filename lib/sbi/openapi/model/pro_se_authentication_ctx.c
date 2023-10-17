
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pro_se_authentication_ctx.h"

OpenAPI_pro_se_authentication_ctx_t *OpenAPI_pro_se_authentication_ctx_create(
    OpenAPI_auth_type_e auth_type,
    OpenAPI_list_t* _links,
    OpenAPI_pro_se_auth_data_t *pro_se_auth_data,
    char *supported_features
)
{
    OpenAPI_pro_se_authentication_ctx_t *pro_se_authentication_ctx_local_var = ogs_malloc(sizeof(OpenAPI_pro_se_authentication_ctx_t));
    ogs_assert(pro_se_authentication_ctx_local_var);

    pro_se_authentication_ctx_local_var->auth_type = auth_type;
    pro_se_authentication_ctx_local_var->_links = _links;
    pro_se_authentication_ctx_local_var->pro_se_auth_data = pro_se_auth_data;
    pro_se_authentication_ctx_local_var->supported_features = supported_features;

    return pro_se_authentication_ctx_local_var;
}

void OpenAPI_pro_se_authentication_ctx_free(OpenAPI_pro_se_authentication_ctx_t *pro_se_authentication_ctx)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pro_se_authentication_ctx) {
        return;
    }
    if (pro_se_authentication_ctx->_links) {
        OpenAPI_list_for_each(pro_se_authentication_ctx->_links, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_links_value_schema_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pro_se_authentication_ctx->_links);
        pro_se_authentication_ctx->_links = NULL;
    }
    if (pro_se_authentication_ctx->pro_se_auth_data) {
        OpenAPI_pro_se_auth_data_free(pro_se_authentication_ctx->pro_se_auth_data);
        pro_se_authentication_ctx->pro_se_auth_data = NULL;
    }
    if (pro_se_authentication_ctx->supported_features) {
        ogs_free(pro_se_authentication_ctx->supported_features);
        pro_se_authentication_ctx->supported_features = NULL;
    }
    ogs_free(pro_se_authentication_ctx);
}

cJSON *OpenAPI_pro_se_authentication_ctx_convertToJSON(OpenAPI_pro_se_authentication_ctx_t *pro_se_authentication_ctx)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pro_se_authentication_ctx == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_convertToJSON() failed [ProSeAuthenticationCtx]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pro_se_authentication_ctx->auth_type == OpenAPI_auth_type_NULL) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_convertToJSON() failed [auth_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "authType", OpenAPI_auth_type_ToString(pro_se_authentication_ctx->auth_type)) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_convertToJSON() failed [auth_type]");
        goto end;
    }

    if (!pro_se_authentication_ctx->_links) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_convertToJSON() failed [_links]");
        return NULL;
    }
    cJSON *_links = cJSON_AddObjectToObject(item, "_links");
    if (_links == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_convertToJSON() failed [_links]");
        goto end;
    }
    cJSON *localMapObject = _links;
    if (pro_se_authentication_ctx->_links) {
        OpenAPI_list_for_each(pro_se_authentication_ctx->_links, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_pro_se_authentication_ctx_convertToJSON() failed [_links]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_pro_se_authentication_ctx_convertToJSON() failed [_links]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_links_value_schema_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pro_se_authentication_ctx_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }

    if (!pro_se_authentication_ctx->pro_se_auth_data) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_convertToJSON() failed [pro_se_auth_data]");
        return NULL;
    }
    cJSON *pro_se_auth_data_local_JSON = OpenAPI_pro_se_auth_data_convertToJSON(pro_se_authentication_ctx->pro_se_auth_data);
    if (pro_se_auth_data_local_JSON == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_convertToJSON() failed [pro_se_auth_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "proSeAuthData", pro_se_auth_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_convertToJSON() failed [pro_se_auth_data]");
        goto end;
    }

    if (pro_se_authentication_ctx->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", pro_se_authentication_ctx->supported_features) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pro_se_authentication_ctx_t *OpenAPI_pro_se_authentication_ctx_parseFromJSON(cJSON *pro_se_authentication_ctxJSON)
{
    OpenAPI_pro_se_authentication_ctx_t *pro_se_authentication_ctx_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *auth_type = NULL;
    OpenAPI_auth_type_e auth_typeVariable = 0;
    cJSON *_links = NULL;
    OpenAPI_list_t *_linksList = NULL;
    cJSON *pro_se_auth_data = NULL;
    OpenAPI_pro_se_auth_data_t *pro_se_auth_data_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    auth_type = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_ctxJSON, "authType");
    if (!auth_type) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_parseFromJSON() failed [auth_type]");
        goto end;
    }
    if (!cJSON_IsString(auth_type)) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_parseFromJSON() failed [auth_type]");
        goto end;
    }
    auth_typeVariable = OpenAPI_auth_type_FromString(auth_type->valuestring);

    _links = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_ctxJSON, "_links");
    if (!_links) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_parseFromJSON() failed [_links]");
        goto end;
    }
        cJSON *_links_local_map = NULL;
        if (!cJSON_IsObject(_links) && !cJSON_IsNull(_links)) {
            ogs_error("OpenAPI_pro_se_authentication_ctx_parseFromJSON() failed [_links]");
            goto end;
        }
        if (cJSON_IsObject(_links)) {
            _linksList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(_links_local_map, _links) {
                cJSON *localMapObject = _links_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_links_value_schema_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_pro_se_authentication_ctx_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(_linksList, localMapKeyPair);
            }
        }

    pro_se_auth_data = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_ctxJSON, "proSeAuthData");
    if (!pro_se_auth_data) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_parseFromJSON() failed [pro_se_auth_data]");
        goto end;
    }
    pro_se_auth_data_local_nonprim = OpenAPI_pro_se_auth_data_parseFromJSON(pro_se_auth_data);
    if (!pro_se_auth_data_local_nonprim) {
        ogs_error("OpenAPI_pro_se_auth_data_parseFromJSON failed [pro_se_auth_data]");
        goto end;
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_ctxJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    pro_se_authentication_ctx_local_var = OpenAPI_pro_se_authentication_ctx_create (
        auth_typeVariable,
        _linksList,
        pro_se_auth_data_local_nonprim,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return pro_se_authentication_ctx_local_var;
end:
    if (_linksList) {
        OpenAPI_list_for_each(_linksList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_links_value_schema_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(_linksList);
        _linksList = NULL;
    }
    if (pro_se_auth_data_local_nonprim) {
        OpenAPI_pro_se_auth_data_free(pro_se_auth_data_local_nonprim);
        pro_se_auth_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pro_se_authentication_ctx_t *OpenAPI_pro_se_authentication_ctx_copy(OpenAPI_pro_se_authentication_ctx_t *dst, OpenAPI_pro_se_authentication_ctx_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pro_se_authentication_ctx_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pro_se_authentication_ctx_convertToJSON() failed");
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

    OpenAPI_pro_se_authentication_ctx_free(dst);
    dst = OpenAPI_pro_se_authentication_ctx_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

