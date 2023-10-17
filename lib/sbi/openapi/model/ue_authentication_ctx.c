
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_authentication_ctx.h"

OpenAPI_ue_authentication_ctx_t *OpenAPI_ue_authentication_ctx_create(
    OpenAPI_auth_type_e auth_type,
    OpenAPI_ue_authentication_ctx_5g_auth_data_t *_5g_auth_data,
    OpenAPI_list_t* _links,
    char *serving_network_name
)
{
    OpenAPI_ue_authentication_ctx_t *ue_authentication_ctx_local_var = ogs_malloc(sizeof(OpenAPI_ue_authentication_ctx_t));
    ogs_assert(ue_authentication_ctx_local_var);

    ue_authentication_ctx_local_var->auth_type = auth_type;
    ue_authentication_ctx_local_var->_5g_auth_data = _5g_auth_data;
    ue_authentication_ctx_local_var->_links = _links;
    ue_authentication_ctx_local_var->serving_network_name = serving_network_name;

    return ue_authentication_ctx_local_var;
}

void OpenAPI_ue_authentication_ctx_free(OpenAPI_ue_authentication_ctx_t *ue_authentication_ctx)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_authentication_ctx) {
        return;
    }
    if (ue_authentication_ctx->_5g_auth_data) {
        OpenAPI_ue_authentication_ctx_5g_auth_data_free(ue_authentication_ctx->_5g_auth_data);
        ue_authentication_ctx->_5g_auth_data = NULL;
    }
    if (ue_authentication_ctx->_links) {
        OpenAPI_list_for_each(ue_authentication_ctx->_links, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_links_value_schema_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_authentication_ctx->_links);
        ue_authentication_ctx->_links = NULL;
    }
    if (ue_authentication_ctx->serving_network_name) {
        ogs_free(ue_authentication_ctx->serving_network_name);
        ue_authentication_ctx->serving_network_name = NULL;
    }
    ogs_free(ue_authentication_ctx);
}

cJSON *OpenAPI_ue_authentication_ctx_convertToJSON(OpenAPI_ue_authentication_ctx_t *ue_authentication_ctx)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_authentication_ctx == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [UEAuthenticationCtx]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_authentication_ctx->auth_type == OpenAPI_auth_type_NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [auth_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "authType", OpenAPI_auth_type_ToString(ue_authentication_ctx->auth_type)) == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [auth_type]");
        goto end;
    }

    if (!ue_authentication_ctx->_5g_auth_data) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [_5g_auth_data]");
        return NULL;
    }
    cJSON *_5g_auth_data_local_JSON = OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON(ue_authentication_ctx->_5g_auth_data);
    if (_5g_auth_data_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [_5g_auth_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "5gAuthData", _5g_auth_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [_5g_auth_data]");
        goto end;
    }

    if (!ue_authentication_ctx->_links) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [_links]");
        return NULL;
    }
    cJSON *_links = cJSON_AddObjectToObject(item, "_links");
    if (_links == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [_links]");
        goto end;
    }
    cJSON *localMapObject = _links;
    if (ue_authentication_ctx->_links) {
        OpenAPI_list_for_each(ue_authentication_ctx->_links, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [_links]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [_links]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_links_value_schema_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }

    if (ue_authentication_ctx->serving_network_name) {
    if (cJSON_AddStringToObject(item, "servingNetworkName", ue_authentication_ctx->serving_network_name) == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [serving_network_name]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_authentication_ctx_t *OpenAPI_ue_authentication_ctx_parseFromJSON(cJSON *ue_authentication_ctxJSON)
{
    OpenAPI_ue_authentication_ctx_t *ue_authentication_ctx_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *auth_type = NULL;
    OpenAPI_auth_type_e auth_typeVariable = 0;
    cJSON *_5g_auth_data = NULL;
    OpenAPI_ue_authentication_ctx_5g_auth_data_t *_5g_auth_data_local_nonprim = NULL;
    cJSON *_links = NULL;
    OpenAPI_list_t *_linksList = NULL;
    cJSON *serving_network_name = NULL;
    auth_type = cJSON_GetObjectItemCaseSensitive(ue_authentication_ctxJSON, "authType");
    if (!auth_type) {
        ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [auth_type]");
        goto end;
    }
    if (!cJSON_IsString(auth_type)) {
        ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [auth_type]");
        goto end;
    }
    auth_typeVariable = OpenAPI_auth_type_FromString(auth_type->valuestring);

    _5g_auth_data = cJSON_GetObjectItemCaseSensitive(ue_authentication_ctxJSON, "5gAuthData");
    if (!_5g_auth_data) {
        ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [_5g_auth_data]");
        goto end;
    }
    _5g_auth_data_local_nonprim = OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON(_5g_auth_data);
    if (!_5g_auth_data_local_nonprim) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON failed [_5g_auth_data]");
        goto end;
    }

    _links = cJSON_GetObjectItemCaseSensitive(ue_authentication_ctxJSON, "_links");
    if (!_links) {
        ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [_links]");
        goto end;
    }
        cJSON *_links_local_map = NULL;
        if (!cJSON_IsObject(_links) && !cJSON_IsNull(_links)) {
            ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [_links]");
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
                    ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(_linksList, localMapKeyPair);
            }
        }

    serving_network_name = cJSON_GetObjectItemCaseSensitive(ue_authentication_ctxJSON, "servingNetworkName");
    if (serving_network_name) {
    if (!cJSON_IsString(serving_network_name) && !cJSON_IsNull(serving_network_name)) {
        ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [serving_network_name]");
        goto end;
    }
    }

    ue_authentication_ctx_local_var = OpenAPI_ue_authentication_ctx_create (
        auth_typeVariable,
        _5g_auth_data_local_nonprim,
        _linksList,
        serving_network_name && !cJSON_IsNull(serving_network_name) ? ogs_strdup(serving_network_name->valuestring) : NULL
    );

    return ue_authentication_ctx_local_var;
end:
    if (_5g_auth_data_local_nonprim) {
        OpenAPI_ue_authentication_ctx_5g_auth_data_free(_5g_auth_data_local_nonprim);
        _5g_auth_data_local_nonprim = NULL;
    }
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
    return NULL;
}

OpenAPI_ue_authentication_ctx_t *OpenAPI_ue_authentication_ctx_copy(OpenAPI_ue_authentication_ctx_t *dst, OpenAPI_ue_authentication_ctx_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_authentication_ctx_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed");
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

    OpenAPI_ue_authentication_ctx_free(dst);
    dst = OpenAPI_ue_authentication_ctx_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

