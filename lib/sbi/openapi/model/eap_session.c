
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eap_session.h"

OpenAPI_eap_session_t *OpenAPI_eap_session_create(
    char *eap_payload,
    char *k_seaf,
    OpenAPI_list_t* _links,
    OpenAPI_auth_result_e auth_result,
    char *supi,
    char *supported_features
)
{
    OpenAPI_eap_session_t *eap_session_local_var = ogs_malloc(sizeof(OpenAPI_eap_session_t));
    ogs_assert(eap_session_local_var);

    eap_session_local_var->eap_payload = eap_payload;
    eap_session_local_var->k_seaf = k_seaf;
    eap_session_local_var->_links = _links;
    eap_session_local_var->auth_result = auth_result;
    eap_session_local_var->supi = supi;
    eap_session_local_var->supported_features = supported_features;

    return eap_session_local_var;
}

void OpenAPI_eap_session_free(OpenAPI_eap_session_t *eap_session)
{
    if (NULL == eap_session) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(eap_session->eap_payload);
    ogs_free(eap_session->k_seaf);
    OpenAPI_list_for_each(eap_session->_links, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_links_value_schema_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(eap_session->_links);
    ogs_free(eap_session->supi);
    ogs_free(eap_session->supported_features);
    ogs_free(eap_session);
}

cJSON *OpenAPI_eap_session_convertToJSON(OpenAPI_eap_session_t *eap_session)
{
    cJSON *item = NULL;

    if (eap_session == NULL) {
        ogs_error("OpenAPI_eap_session_convertToJSON() failed [EapSession]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "eapPayload", eap_session->eap_payload) == NULL) {
        ogs_error("OpenAPI_eap_session_convertToJSON() failed [eap_payload]");
        goto end;
    }

    if (eap_session->k_seaf) {
    if (cJSON_AddStringToObject(item, "kSeaf", eap_session->k_seaf) == NULL) {
        ogs_error("OpenAPI_eap_session_convertToJSON() failed [k_seaf]");
        goto end;
    }
    }

    if (eap_session->_links) {
    cJSON *_links = cJSON_AddObjectToObject(item, "_links");
    if (_links == NULL) {
        ogs_error("OpenAPI_eap_session_convertToJSON() failed [_links]");
        goto end;
    }
    cJSON *localMapObject = _links;
    OpenAPI_lnode_t *_links_node;
    if (eap_session->_links) {
        OpenAPI_list_for_each(eap_session->_links, _links_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)_links_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_links_value_schema_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_eap_session_convertToJSON() failed [_links]");
            goto end;
        }
        cJSON_AddItemToObject(_links, localKeyValue->key, itemLocal);
            }
        }
    }

    if (eap_session->auth_result) {
    if (cJSON_AddStringToObject(item, "authResult", OpenAPI_auth_result_ToString(eap_session->auth_result)) == NULL) {
        ogs_error("OpenAPI_eap_session_convertToJSON() failed [auth_result]");
        goto end;
    }
    }

    if (eap_session->supi) {
    if (cJSON_AddStringToObject(item, "supi", eap_session->supi) == NULL) {
        ogs_error("OpenAPI_eap_session_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (eap_session->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", eap_session->supported_features) == NULL) {
        ogs_error("OpenAPI_eap_session_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_eap_session_t *OpenAPI_eap_session_parseFromJSON(cJSON *eap_sessionJSON)
{
    OpenAPI_eap_session_t *eap_session_local_var = NULL;
    cJSON *eap_payload = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "eapPayload");
    if (!eap_payload) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [eap_payload]");
        goto end;
    }

    if (!cJSON_IsString(eap_payload)) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [eap_payload]");
        goto end;
    }

    cJSON *k_seaf = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "kSeaf");

    if (k_seaf) {
    if (!cJSON_IsString(k_seaf)) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [k_seaf]");
        goto end;
    }
    }

    cJSON *_links = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "_links");

    OpenAPI_list_t *_linksList;
    if (_links) {
    cJSON *_links_local_map;
    if (!cJSON_IsObject(_links)) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [_links]");
        goto end;
    }
    _linksList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(_links_local_map, _links) {
        cJSON *localMapObject = _links_local_map;
        if (cJSON_IsObject(_links_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_links_value_schema_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(_links_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_eap_session_parseFromJSON() failed [_links]");
            goto end;
        }
        OpenAPI_list_add(_linksList , localMapKeyPair);
    }
    }

    cJSON *auth_result = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "authResult");

    OpenAPI_auth_result_e auth_resultVariable;
    if (auth_result) {
    if (!cJSON_IsString(auth_result)) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [auth_result]");
        goto end;
    }
    auth_resultVariable = OpenAPI_auth_result_FromString(auth_result->valuestring);
    }

    cJSON *supi = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "supi");

    if (supi) {
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    eap_session_local_var = OpenAPI_eap_session_create (
        ogs_strdup(eap_payload->valuestring),
        k_seaf ? ogs_strdup(k_seaf->valuestring) : NULL,
        _links ? _linksList : NULL,
        auth_result ? auth_resultVariable : 0,
        supi ? ogs_strdup(supi->valuestring) : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return eap_session_local_var;
end:
    return NULL;
}

OpenAPI_eap_session_t *OpenAPI_eap_session_copy(OpenAPI_eap_session_t *dst, OpenAPI_eap_session_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eap_session_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eap_session_convertToJSON() failed");
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

    OpenAPI_eap_session_free(dst);
    dst = OpenAPI_eap_session_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

