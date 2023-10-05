
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pro_se_eap_session.h"

OpenAPI_pro_se_eap_session_t *OpenAPI_pro_se_eap_session_create(
    bool is_eap_payload_null,
    char *eap_payload,
    char *knr_pro_se,
    OpenAPI_list_t* _links,
    OpenAPI_auth_result_e auth_result,
    char *supported_features,
    bool is_nonce2_null,
    char *nonce2,
    char *_5g_pruk_id
)
{
    OpenAPI_pro_se_eap_session_t *pro_se_eap_session_local_var = ogs_malloc(sizeof(OpenAPI_pro_se_eap_session_t));
    ogs_assert(pro_se_eap_session_local_var);

    pro_se_eap_session_local_var->is_eap_payload_null = is_eap_payload_null;
    pro_se_eap_session_local_var->eap_payload = eap_payload;
    pro_se_eap_session_local_var->knr_pro_se = knr_pro_se;
    pro_se_eap_session_local_var->_links = _links;
    pro_se_eap_session_local_var->auth_result = auth_result;
    pro_se_eap_session_local_var->supported_features = supported_features;
    pro_se_eap_session_local_var->is_nonce2_null = is_nonce2_null;
    pro_se_eap_session_local_var->nonce2 = nonce2;
    pro_se_eap_session_local_var->_5g_pruk_id = _5g_pruk_id;

    return pro_se_eap_session_local_var;
}

void OpenAPI_pro_se_eap_session_free(OpenAPI_pro_se_eap_session_t *pro_se_eap_session)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pro_se_eap_session) {
        return;
    }
    if (pro_se_eap_session->eap_payload) {
        ogs_free(pro_se_eap_session->eap_payload);
        pro_se_eap_session->eap_payload = NULL;
    }
    if (pro_se_eap_session->knr_pro_se) {
        ogs_free(pro_se_eap_session->knr_pro_se);
        pro_se_eap_session->knr_pro_se = NULL;
    }
    if (pro_se_eap_session->_links) {
        OpenAPI_list_for_each(pro_se_eap_session->_links, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_links_value_schema_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pro_se_eap_session->_links);
        pro_se_eap_session->_links = NULL;
    }
    if (pro_se_eap_session->supported_features) {
        ogs_free(pro_se_eap_session->supported_features);
        pro_se_eap_session->supported_features = NULL;
    }
    if (pro_se_eap_session->nonce2) {
        ogs_free(pro_se_eap_session->nonce2);
        pro_se_eap_session->nonce2 = NULL;
    }
    if (pro_se_eap_session->_5g_pruk_id) {
        ogs_free(pro_se_eap_session->_5g_pruk_id);
        pro_se_eap_session->_5g_pruk_id = NULL;
    }
    ogs_free(pro_se_eap_session);
}

cJSON *OpenAPI_pro_se_eap_session_convertToJSON(OpenAPI_pro_se_eap_session_t *pro_se_eap_session)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pro_se_eap_session == NULL) {
        ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed [ProSeEapSession]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pro_se_eap_session->eap_payload) {
        ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed [eap_payload]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "eapPayload", pro_se_eap_session->eap_payload) == NULL) {
        ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed [eap_payload]");
        goto end;
    }

    if (pro_se_eap_session->knr_pro_se) {
    if (cJSON_AddStringToObject(item, "knrProSe", pro_se_eap_session->knr_pro_se) == NULL) {
        ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed [knr_pro_se]");
        goto end;
    }
    }

    if (pro_se_eap_session->_links) {
    cJSON *_links = cJSON_AddObjectToObject(item, "_links");
    if (_links == NULL) {
        ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed [_links]");
        goto end;
    }
    cJSON *localMapObject = _links;
    if (pro_se_eap_session->_links) {
        OpenAPI_list_for_each(pro_se_eap_session->_links, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed [_links]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed [_links]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_links_value_schema_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (pro_se_eap_session->auth_result != OpenAPI_auth_result_NULL) {
    if (cJSON_AddStringToObject(item, "authResult", OpenAPI_auth_result_ToString(pro_se_eap_session->auth_result)) == NULL) {
        ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed [auth_result]");
        goto end;
    }
    }

    if (pro_se_eap_session->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", pro_se_eap_session->supported_features) == NULL) {
        ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (pro_se_eap_session->nonce2) {
    if (cJSON_AddStringToObject(item, "nonce2", pro_se_eap_session->nonce2) == NULL) {
        ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed [nonce2]");
        goto end;
    }
    } else if (pro_se_eap_session->is_nonce2_null) {
        if (cJSON_AddNullToObject(item, "nonce2") == NULL) {
            ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed [nonce2]");
            goto end;
        }
    }

    if (pro_se_eap_session->_5g_pruk_id) {
    if (cJSON_AddStringToObject(item, "5gPrukId", pro_se_eap_session->_5g_pruk_id) == NULL) {
        ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed [_5g_pruk_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pro_se_eap_session_t *OpenAPI_pro_se_eap_session_parseFromJSON(cJSON *pro_se_eap_sessionJSON)
{
    OpenAPI_pro_se_eap_session_t *pro_se_eap_session_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *eap_payload = NULL;
    cJSON *knr_pro_se = NULL;
    cJSON *_links = NULL;
    OpenAPI_list_t *_linksList = NULL;
    cJSON *auth_result = NULL;
    OpenAPI_auth_result_e auth_resultVariable = 0;
    cJSON *supported_features = NULL;
    cJSON *nonce2 = NULL;
    cJSON *_5g_pruk_id = NULL;
    eap_payload = cJSON_GetObjectItemCaseSensitive(pro_se_eap_sessionJSON, "eapPayload");
    if (!eap_payload) {
        ogs_error("OpenAPI_pro_se_eap_session_parseFromJSON() failed [eap_payload]");
        goto end;
    }
    if (!cJSON_IsString(eap_payload)) {
        ogs_error("OpenAPI_pro_se_eap_session_parseFromJSON() failed [eap_payload]");
        goto end;
    }

    knr_pro_se = cJSON_GetObjectItemCaseSensitive(pro_se_eap_sessionJSON, "knrProSe");
    if (knr_pro_se) {
    if (!cJSON_IsString(knr_pro_se) && !cJSON_IsNull(knr_pro_se)) {
        ogs_error("OpenAPI_pro_se_eap_session_parseFromJSON() failed [knr_pro_se]");
        goto end;
    }
    }

    _links = cJSON_GetObjectItemCaseSensitive(pro_se_eap_sessionJSON, "_links");
    if (_links) {
        cJSON *_links_local_map = NULL;
        if (!cJSON_IsObject(_links) && !cJSON_IsNull(_links)) {
            ogs_error("OpenAPI_pro_se_eap_session_parseFromJSON() failed [_links]");
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
                    ogs_error("OpenAPI_pro_se_eap_session_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(_linksList, localMapKeyPair);
            }
        }
    }

    auth_result = cJSON_GetObjectItemCaseSensitive(pro_se_eap_sessionJSON, "authResult");
    if (auth_result) {
    if (!cJSON_IsString(auth_result)) {
        ogs_error("OpenAPI_pro_se_eap_session_parseFromJSON() failed [auth_result]");
        goto end;
    }
    auth_resultVariable = OpenAPI_auth_result_FromString(auth_result->valuestring);
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(pro_se_eap_sessionJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_pro_se_eap_session_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    nonce2 = cJSON_GetObjectItemCaseSensitive(pro_se_eap_sessionJSON, "nonce2");
    if (nonce2) {
    if (!cJSON_IsNull(nonce2)) {
    if (!cJSON_IsString(nonce2) && !cJSON_IsNull(nonce2)) {
        ogs_error("OpenAPI_pro_se_eap_session_parseFromJSON() failed [nonce2]");
        goto end;
    }
    }
    }

    _5g_pruk_id = cJSON_GetObjectItemCaseSensitive(pro_se_eap_sessionJSON, "5gPrukId");
    if (_5g_pruk_id) {
    if (!cJSON_IsString(_5g_pruk_id) && !cJSON_IsNull(_5g_pruk_id)) {
        ogs_error("OpenAPI_pro_se_eap_session_parseFromJSON() failed [_5g_pruk_id]");
        goto end;
    }
    }

    pro_se_eap_session_local_var = OpenAPI_pro_se_eap_session_create (
        eap_payload && cJSON_IsNull(eap_payload) ? true : false,
        ogs_strdup(eap_payload->valuestring),
        knr_pro_se && !cJSON_IsNull(knr_pro_se) ? ogs_strdup(knr_pro_se->valuestring) : NULL,
        _links ? _linksList : NULL,
        auth_result ? auth_resultVariable : 0,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        nonce2 && cJSON_IsNull(nonce2) ? true : false,
        nonce2 && !cJSON_IsNull(nonce2) ? ogs_strdup(nonce2->valuestring) : NULL,
        _5g_pruk_id && !cJSON_IsNull(_5g_pruk_id) ? ogs_strdup(_5g_pruk_id->valuestring) : NULL
    );

    return pro_se_eap_session_local_var;
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
    return NULL;
}

OpenAPI_pro_se_eap_session_t *OpenAPI_pro_se_eap_session_copy(OpenAPI_pro_se_eap_session_t *dst, OpenAPI_pro_se_eap_session_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pro_se_eap_session_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pro_se_eap_session_convertToJSON() failed");
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

    OpenAPI_pro_se_eap_session_free(dst);
    dst = OpenAPI_pro_se_eap_session_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

