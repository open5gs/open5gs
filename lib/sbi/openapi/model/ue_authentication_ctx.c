
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_authentication_ctx.h"

OpenAPI_ue_authentication_ctx_t *OpenAPI_ue_authentication_ctx_create(
    OpenAPI_auth_type_e auth_type,
    OpenAPI_av5g_aka_t *_5g_auth_data,
    OpenAPI_list_t* _links,
    char *serving_network_name
    )
{
    OpenAPI_ue_authentication_ctx_t *ue_authentication_ctx_local_var = OpenAPI_malloc(sizeof(OpenAPI_ue_authentication_ctx_t));
    if (!ue_authentication_ctx_local_var) {
        return NULL;
    }
    ue_authentication_ctx_local_var->auth_type = auth_type;
    ue_authentication_ctx_local_var->_5g_auth_data = _5g_auth_data;
    ue_authentication_ctx_local_var->_links = _links;
    ue_authentication_ctx_local_var->serving_network_name = serving_network_name;

    return ue_authentication_ctx_local_var;
}

void OpenAPI_ue_authentication_ctx_free(OpenAPI_ue_authentication_ctx_t *ue_authentication_ctx)
{
    if (NULL == ue_authentication_ctx) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_av5g_aka_free(ue_authentication_ctx->_5g_auth_data);
    OpenAPI_list_for_each(ue_authentication_ctx->_links, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_links_value_schema_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(ue_authentication_ctx->_links);
    ogs_free(ue_authentication_ctx->serving_network_name);
    ogs_free(ue_authentication_ctx);
}

cJSON *OpenAPI_ue_authentication_ctx_convertToJSON(OpenAPI_ue_authentication_ctx_t *ue_authentication_ctx)
{
    cJSON *item = NULL;

    if (ue_authentication_ctx == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [UEAuthenticationCtx]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ue_authentication_ctx->auth_type) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [auth_type]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "authType", OpenAPI_auth_type_ToString(ue_authentication_ctx->auth_type)) == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [auth_type]");
        goto end;
    }

    if (!ue_authentication_ctx->_5g_auth_data) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [_5g_auth_data]");
        goto end;
    }
    cJSON *_5g_auth_data_local_JSON = OpenAPI_av5g_aka_convertToJSON(ue_authentication_ctx->_5g_auth_data);
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
        goto end;
    }
    cJSON *_links = cJSON_AddObjectToObject(item, "_links");
    if (_links == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [_links]");
        goto end;
    }
    cJSON *localMapObject = _links;
    OpenAPI_lnode_t *_links_node;
    if (ue_authentication_ctx->_links) {
        OpenAPI_list_for_each(ue_authentication_ctx->_links, _links_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)_links_node->data;
            cJSON *itemLocal = OpenAPI_links_value_schema_convertToJSON(localKeyValue->value);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_authentication_ctx_convertToJSON() failed [_links]");
                goto end;
            }
            cJSON_AddItemToObject(_links, localKeyValue->key, itemLocal);
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
    cJSON *auth_type = cJSON_GetObjectItemCaseSensitive(ue_authentication_ctxJSON, "authType");
    if (!auth_type) {
        ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [auth_type]");
        goto end;
    }

    OpenAPI_auth_type_e auth_typeVariable;

    if (!cJSON_IsString(auth_type)) {
        ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [auth_type]");
        goto end;
    }
    auth_typeVariable = OpenAPI_auth_type_FromString(auth_type->valuestring);

    cJSON *_5g_auth_data = cJSON_GetObjectItemCaseSensitive(ue_authentication_ctxJSON, "5gAuthData");
    if (!_5g_auth_data) {
        ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [_5g_auth_data]");
        goto end;
    }

    OpenAPI_av5g_aka_t *_5g_auth_data_local_nonprim = NULL;

    _5g_auth_data_local_nonprim = OpenAPI_av5g_aka_parseFromJSON(_5g_auth_data);

    cJSON *_links = cJSON_GetObjectItemCaseSensitive(ue_authentication_ctxJSON, "_links");
    if (!_links) {
        ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [_links]");
        goto end;
    }

    OpenAPI_list_t *_linksList;

    cJSON *_links_local_map;
    if (!cJSON_IsObject(_links)) {
        ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [_links]");
        goto end;
    }
    _linksList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(_links_local_map, _links) {
        cJSON *localMapObject = _links_local_map;
        if (!cJSON_IsObject(_links_local_map)) {
            ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [_links]");
            goto end;
        }
        localMapKeyPair = OpenAPI_map_create(
            localMapObject->string, OpenAPI_links_value_schema_parseFromJSON(localMapObject));
        OpenAPI_list_add(_linksList, localMapKeyPair);
    }

    cJSON *serving_network_name = cJSON_GetObjectItemCaseSensitive(ue_authentication_ctxJSON, "servingNetworkName");

    if (serving_network_name) {
        if (!cJSON_IsString(serving_network_name)) {
            ogs_error("OpenAPI_ue_authentication_ctx_parseFromJSON() failed [serving_network_name]");
            goto end;
        }
    }

    ue_authentication_ctx_local_var = OpenAPI_ue_authentication_ctx_create (
        auth_typeVariable,
        _5g_auth_data_local_nonprim,
        _linksList,
        serving_network_name ? ogs_strdup(serving_network_name->valuestring) : NULL
        );

    return ue_authentication_ctx_local_var;
end:
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

