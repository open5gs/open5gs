
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eap_session.h"

OpenAPI_eap_session_t *OpenAPI_eap_session_create(
    bool is_eap_payload_null,
    char *eap_payload,
    char *k_seaf,
    OpenAPI_list_t* _links,
    OpenAPI_auth_result_e auth_result,
    char *supi,
    char *supported_features,
    OpenAPI_list_t *pvs_info,
    char *msk
)
{
    OpenAPI_eap_session_t *eap_session_local_var = ogs_malloc(sizeof(OpenAPI_eap_session_t));
    ogs_assert(eap_session_local_var);

    eap_session_local_var->is_eap_payload_null = is_eap_payload_null;
    eap_session_local_var->eap_payload = eap_payload;
    eap_session_local_var->k_seaf = k_seaf;
    eap_session_local_var->_links = _links;
    eap_session_local_var->auth_result = auth_result;
    eap_session_local_var->supi = supi;
    eap_session_local_var->supported_features = supported_features;
    eap_session_local_var->pvs_info = pvs_info;
    eap_session_local_var->msk = msk;

    return eap_session_local_var;
}

void OpenAPI_eap_session_free(OpenAPI_eap_session_t *eap_session)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == eap_session) {
        return;
    }
    if (eap_session->eap_payload) {
        ogs_free(eap_session->eap_payload);
        eap_session->eap_payload = NULL;
    }
    if (eap_session->k_seaf) {
        ogs_free(eap_session->k_seaf);
        eap_session->k_seaf = NULL;
    }
    if (eap_session->_links) {
        OpenAPI_list_for_each(eap_session->_links, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_links_value_schema_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(eap_session->_links);
        eap_session->_links = NULL;
    }
    if (eap_session->supi) {
        ogs_free(eap_session->supi);
        eap_session->supi = NULL;
    }
    if (eap_session->supported_features) {
        ogs_free(eap_session->supported_features);
        eap_session->supported_features = NULL;
    }
    if (eap_session->pvs_info) {
        OpenAPI_list_for_each(eap_session->pvs_info, node) {
            OpenAPI_server_addressing_info_free(node->data);
        }
        OpenAPI_list_free(eap_session->pvs_info);
        eap_session->pvs_info = NULL;
    }
    if (eap_session->msk) {
        ogs_free(eap_session->msk);
        eap_session->msk = NULL;
    }
    ogs_free(eap_session);
}

cJSON *OpenAPI_eap_session_convertToJSON(OpenAPI_eap_session_t *eap_session)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (eap_session == NULL) {
        ogs_error("OpenAPI_eap_session_convertToJSON() failed [EapSession]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!eap_session->eap_payload) {
        ogs_error("OpenAPI_eap_session_convertToJSON() failed [eap_payload]");
        return NULL;
    }
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
    if (eap_session->_links) {
        OpenAPI_list_for_each(eap_session->_links, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_eap_session_convertToJSON() failed [_links]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_eap_session_convertToJSON() failed [_links]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_links_value_schema_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_eap_session_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (eap_session->auth_result != OpenAPI_auth_result_NULL) {
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

    if (eap_session->pvs_info) {
    cJSON *pvs_infoList = cJSON_AddArrayToObject(item, "pvsInfo");
    if (pvs_infoList == NULL) {
        ogs_error("OpenAPI_eap_session_convertToJSON() failed [pvs_info]");
        goto end;
    }
    OpenAPI_list_for_each(eap_session->pvs_info, node) {
        cJSON *itemLocal = OpenAPI_server_addressing_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_eap_session_convertToJSON() failed [pvs_info]");
            goto end;
        }
        cJSON_AddItemToArray(pvs_infoList, itemLocal);
    }
    }

    if (eap_session->msk) {
    if (cJSON_AddStringToObject(item, "msk", eap_session->msk) == NULL) {
        ogs_error("OpenAPI_eap_session_convertToJSON() failed [msk]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_eap_session_t *OpenAPI_eap_session_parseFromJSON(cJSON *eap_sessionJSON)
{
    OpenAPI_eap_session_t *eap_session_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *eap_payload = NULL;
    cJSON *k_seaf = NULL;
    cJSON *_links = NULL;
    OpenAPI_list_t *_linksList = NULL;
    cJSON *auth_result = NULL;
    OpenAPI_auth_result_e auth_resultVariable = 0;
    cJSON *supi = NULL;
    cJSON *supported_features = NULL;
    cJSON *pvs_info = NULL;
    OpenAPI_list_t *pvs_infoList = NULL;
    cJSON *msk = NULL;
    eap_payload = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "eapPayload");
    if (!eap_payload) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [eap_payload]");
        goto end;
    }
    if (!cJSON_IsString(eap_payload)) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [eap_payload]");
        goto end;
    }

    k_seaf = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "kSeaf");
    if (k_seaf) {
    if (!cJSON_IsString(k_seaf) && !cJSON_IsNull(k_seaf)) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [k_seaf]");
        goto end;
    }
    }

    _links = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "_links");
    if (_links) {
        cJSON *_links_local_map = NULL;
        if (!cJSON_IsObject(_links) && !cJSON_IsNull(_links)) {
            ogs_error("OpenAPI_eap_session_parseFromJSON() failed [_links]");
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
                    ogs_error("OpenAPI_eap_session_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(_linksList, localMapKeyPair);
            }
        }
    }

    auth_result = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "authResult");
    if (auth_result) {
    if (!cJSON_IsString(auth_result)) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [auth_result]");
        goto end;
    }
    auth_resultVariable = OpenAPI_auth_result_FromString(auth_result->valuestring);
    }

    supi = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    pvs_info = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "pvsInfo");
    if (pvs_info) {
        cJSON *pvs_info_local = NULL;
        if (!cJSON_IsArray(pvs_info)) {
            ogs_error("OpenAPI_eap_session_parseFromJSON() failed [pvs_info]");
            goto end;
        }

        pvs_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(pvs_info_local, pvs_info) {
            if (!cJSON_IsObject(pvs_info_local)) {
                ogs_error("OpenAPI_eap_session_parseFromJSON() failed [pvs_info]");
                goto end;
            }
            OpenAPI_server_addressing_info_t *pvs_infoItem = OpenAPI_server_addressing_info_parseFromJSON(pvs_info_local);
            if (!pvs_infoItem) {
                ogs_error("No pvs_infoItem");
                goto end;
            }
            OpenAPI_list_add(pvs_infoList, pvs_infoItem);
        }
    }

    msk = cJSON_GetObjectItemCaseSensitive(eap_sessionJSON, "msk");
    if (msk) {
    if (!cJSON_IsString(msk) && !cJSON_IsNull(msk)) {
        ogs_error("OpenAPI_eap_session_parseFromJSON() failed [msk]");
        goto end;
    }
    }

    eap_session_local_var = OpenAPI_eap_session_create (
        eap_payload && cJSON_IsNull(eap_payload) ? true : false,
        ogs_strdup(eap_payload->valuestring),
        k_seaf && !cJSON_IsNull(k_seaf) ? ogs_strdup(k_seaf->valuestring) : NULL,
        _links ? _linksList : NULL,
        auth_result ? auth_resultVariable : 0,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        pvs_info ? pvs_infoList : NULL,
        msk && !cJSON_IsNull(msk) ? ogs_strdup(msk->valuestring) : NULL
    );

    return eap_session_local_var;
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
    if (pvs_infoList) {
        OpenAPI_list_for_each(pvs_infoList, node) {
            OpenAPI_server_addressing_info_free(node->data);
        }
        OpenAPI_list_free(pvs_infoList);
        pvs_infoList = NULL;
    }
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

