
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eap_auth_method_200_response.h"

OpenAPI_eap_auth_method_200_response_t *OpenAPI_eap_auth_method_200_response_create(
    bool is_eap_payload_null,
    char *eap_payload,
    OpenAPI_list_t* _links
)
{
    OpenAPI_eap_auth_method_200_response_t *eap_auth_method_200_response_local_var = ogs_malloc(sizeof(OpenAPI_eap_auth_method_200_response_t));
    ogs_assert(eap_auth_method_200_response_local_var);

    eap_auth_method_200_response_local_var->is_eap_payload_null = is_eap_payload_null;
    eap_auth_method_200_response_local_var->eap_payload = eap_payload;
    eap_auth_method_200_response_local_var->_links = _links;

    return eap_auth_method_200_response_local_var;
}

void OpenAPI_eap_auth_method_200_response_free(OpenAPI_eap_auth_method_200_response_t *eap_auth_method_200_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == eap_auth_method_200_response) {
        return;
    }
    if (eap_auth_method_200_response->eap_payload) {
        ogs_free(eap_auth_method_200_response->eap_payload);
        eap_auth_method_200_response->eap_payload = NULL;
    }
    if (eap_auth_method_200_response->_links) {
        OpenAPI_list_for_each(eap_auth_method_200_response->_links, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_links_value_schema_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(eap_auth_method_200_response->_links);
        eap_auth_method_200_response->_links = NULL;
    }
    ogs_free(eap_auth_method_200_response);
}

cJSON *OpenAPI_eap_auth_method_200_response_convertToJSON(OpenAPI_eap_auth_method_200_response_t *eap_auth_method_200_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (eap_auth_method_200_response == NULL) {
        ogs_error("OpenAPI_eap_auth_method_200_response_convertToJSON() failed [EapAuthMethod_200_response]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!eap_auth_method_200_response->eap_payload) {
        ogs_error("OpenAPI_eap_auth_method_200_response_convertToJSON() failed [eap_payload]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "eapPayload", eap_auth_method_200_response->eap_payload) == NULL) {
        ogs_error("OpenAPI_eap_auth_method_200_response_convertToJSON() failed [eap_payload]");
        goto end;
    }

    if (!eap_auth_method_200_response->_links) {
        ogs_error("OpenAPI_eap_auth_method_200_response_convertToJSON() failed [_links]");
        return NULL;
    }
    cJSON *_links = cJSON_AddObjectToObject(item, "_links");
    if (_links == NULL) {
        ogs_error("OpenAPI_eap_auth_method_200_response_convertToJSON() failed [_links]");
        goto end;
    }
    cJSON *localMapObject = _links;
    if (eap_auth_method_200_response->_links) {
        OpenAPI_list_for_each(eap_auth_method_200_response->_links, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_eap_auth_method_200_response_convertToJSON() failed [_links]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_eap_auth_method_200_response_convertToJSON() failed [_links]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_links_value_schema_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_eap_auth_method_200_response_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_eap_auth_method_200_response_t *OpenAPI_eap_auth_method_200_response_parseFromJSON(cJSON *eap_auth_method_200_responseJSON)
{
    OpenAPI_eap_auth_method_200_response_t *eap_auth_method_200_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *eap_payload = NULL;
    cJSON *_links = NULL;
    OpenAPI_list_t *_linksList = NULL;
    eap_payload = cJSON_GetObjectItemCaseSensitive(eap_auth_method_200_responseJSON, "eapPayload");
    if (!eap_payload) {
        ogs_error("OpenAPI_eap_auth_method_200_response_parseFromJSON() failed [eap_payload]");
        goto end;
    }
    if (!cJSON_IsString(eap_payload)) {
        ogs_error("OpenAPI_eap_auth_method_200_response_parseFromJSON() failed [eap_payload]");
        goto end;
    }

    _links = cJSON_GetObjectItemCaseSensitive(eap_auth_method_200_responseJSON, "_links");
    if (!_links) {
        ogs_error("OpenAPI_eap_auth_method_200_response_parseFromJSON() failed [_links]");
        goto end;
    }
        cJSON *_links_local_map = NULL;
        if (!cJSON_IsObject(_links) && !cJSON_IsNull(_links)) {
            ogs_error("OpenAPI_eap_auth_method_200_response_parseFromJSON() failed [_links]");
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
                    ogs_error("OpenAPI_eap_auth_method_200_response_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(_linksList, localMapKeyPair);
            }
        }

    eap_auth_method_200_response_local_var = OpenAPI_eap_auth_method_200_response_create (
        eap_payload && cJSON_IsNull(eap_payload) ? true : false,
        ogs_strdup(eap_payload->valuestring),
        _linksList
    );

    return eap_auth_method_200_response_local_var;
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

OpenAPI_eap_auth_method_200_response_t *OpenAPI_eap_auth_method_200_response_copy(OpenAPI_eap_auth_method_200_response_t *dst, OpenAPI_eap_auth_method_200_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eap_auth_method_200_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eap_auth_method_200_response_convertToJSON() failed");
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

    OpenAPI_eap_auth_method_200_response_free(dst);
    dst = OpenAPI_eap_auth_method_200_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

