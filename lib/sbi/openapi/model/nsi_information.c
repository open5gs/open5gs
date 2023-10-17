
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nsi_information.h"

OpenAPI_nsi_information_t *OpenAPI_nsi_information_create(
    char *nrf_id,
    char *nsi_id,
    char *nrf_nf_mgt_uri,
    char *nrf_access_token_uri,
    OpenAPI_list_t* nrf_oauth2_required
)
{
    OpenAPI_nsi_information_t *nsi_information_local_var = ogs_malloc(sizeof(OpenAPI_nsi_information_t));
    ogs_assert(nsi_information_local_var);

    nsi_information_local_var->nrf_id = nrf_id;
    nsi_information_local_var->nsi_id = nsi_id;
    nsi_information_local_var->nrf_nf_mgt_uri = nrf_nf_mgt_uri;
    nsi_information_local_var->nrf_access_token_uri = nrf_access_token_uri;
    nsi_information_local_var->nrf_oauth2_required = nrf_oauth2_required;

    return nsi_information_local_var;
}

void OpenAPI_nsi_information_free(OpenAPI_nsi_information_t *nsi_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nsi_information) {
        return;
    }
    if (nsi_information->nrf_id) {
        ogs_free(nsi_information->nrf_id);
        nsi_information->nrf_id = NULL;
    }
    if (nsi_information->nsi_id) {
        ogs_free(nsi_information->nsi_id);
        nsi_information->nsi_id = NULL;
    }
    if (nsi_information->nrf_nf_mgt_uri) {
        ogs_free(nsi_information->nrf_nf_mgt_uri);
        nsi_information->nrf_nf_mgt_uri = NULL;
    }
    if (nsi_information->nrf_access_token_uri) {
        ogs_free(nsi_information->nrf_access_token_uri);
        nsi_information->nrf_access_token_uri = NULL;
    }
    if (nsi_information->nrf_oauth2_required) {
        OpenAPI_list_for_each(nsi_information->nrf_oauth2_required, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nsi_information->nrf_oauth2_required);
        nsi_information->nrf_oauth2_required = NULL;
    }
    ogs_free(nsi_information);
}

cJSON *OpenAPI_nsi_information_convertToJSON(OpenAPI_nsi_information_t *nsi_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nsi_information == NULL) {
        ogs_error("OpenAPI_nsi_information_convertToJSON() failed [NsiInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nsi_information->nrf_id) {
        ogs_error("OpenAPI_nsi_information_convertToJSON() failed [nrf_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nrfId", nsi_information->nrf_id) == NULL) {
        ogs_error("OpenAPI_nsi_information_convertToJSON() failed [nrf_id]");
        goto end;
    }

    if (nsi_information->nsi_id) {
    if (cJSON_AddStringToObject(item, "nsiId", nsi_information->nsi_id) == NULL) {
        ogs_error("OpenAPI_nsi_information_convertToJSON() failed [nsi_id]");
        goto end;
    }
    }

    if (nsi_information->nrf_nf_mgt_uri) {
    if (cJSON_AddStringToObject(item, "nrfNfMgtUri", nsi_information->nrf_nf_mgt_uri) == NULL) {
        ogs_error("OpenAPI_nsi_information_convertToJSON() failed [nrf_nf_mgt_uri]");
        goto end;
    }
    }

    if (nsi_information->nrf_access_token_uri) {
    if (cJSON_AddStringToObject(item, "nrfAccessTokenUri", nsi_information->nrf_access_token_uri) == NULL) {
        ogs_error("OpenAPI_nsi_information_convertToJSON() failed [nrf_access_token_uri]");
        goto end;
    }
    }

    if (nsi_information->nrf_oauth2_required) {
    cJSON *nrf_oauth2_required = cJSON_AddObjectToObject(item, "nrfOauth2Required");
    if (nrf_oauth2_required == NULL) {
        ogs_error("OpenAPI_nsi_information_convertToJSON() failed [nrf_oauth2_required]");
        goto end;
    }
    cJSON *localMapObject = nrf_oauth2_required;
    if (nsi_information->nrf_oauth2_required) {
        OpenAPI_list_for_each(nsi_information->nrf_oauth2_required, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nsi_information_convertToJSON() failed [nrf_oauth2_required]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nsi_information_convertToJSON() failed [nrf_oauth2_required]");
                goto end;
            }
            if (cJSON_AddBoolToObject(localMapObject, localKeyValue->key, (uintptr_t)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_nsi_information_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

end:
    return item;
}

OpenAPI_nsi_information_t *OpenAPI_nsi_information_parseFromJSON(cJSON *nsi_informationJSON)
{
    OpenAPI_nsi_information_t *nsi_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nrf_id = NULL;
    cJSON *nsi_id = NULL;
    cJSON *nrf_nf_mgt_uri = NULL;
    cJSON *nrf_access_token_uri = NULL;
    cJSON *nrf_oauth2_required = NULL;
    OpenAPI_list_t *nrf_oauth2_requiredList = NULL;
    nrf_id = cJSON_GetObjectItemCaseSensitive(nsi_informationJSON, "nrfId");
    if (!nrf_id) {
        ogs_error("OpenAPI_nsi_information_parseFromJSON() failed [nrf_id]");
        goto end;
    }
    if (!cJSON_IsString(nrf_id)) {
        ogs_error("OpenAPI_nsi_information_parseFromJSON() failed [nrf_id]");
        goto end;
    }

    nsi_id = cJSON_GetObjectItemCaseSensitive(nsi_informationJSON, "nsiId");
    if (nsi_id) {
    if (!cJSON_IsString(nsi_id) && !cJSON_IsNull(nsi_id)) {
        ogs_error("OpenAPI_nsi_information_parseFromJSON() failed [nsi_id]");
        goto end;
    }
    }

    nrf_nf_mgt_uri = cJSON_GetObjectItemCaseSensitive(nsi_informationJSON, "nrfNfMgtUri");
    if (nrf_nf_mgt_uri) {
    if (!cJSON_IsString(nrf_nf_mgt_uri) && !cJSON_IsNull(nrf_nf_mgt_uri)) {
        ogs_error("OpenAPI_nsi_information_parseFromJSON() failed [nrf_nf_mgt_uri]");
        goto end;
    }
    }

    nrf_access_token_uri = cJSON_GetObjectItemCaseSensitive(nsi_informationJSON, "nrfAccessTokenUri");
    if (nrf_access_token_uri) {
    if (!cJSON_IsString(nrf_access_token_uri) && !cJSON_IsNull(nrf_access_token_uri)) {
        ogs_error("OpenAPI_nsi_information_parseFromJSON() failed [nrf_access_token_uri]");
        goto end;
    }
    }

    nrf_oauth2_required = cJSON_GetObjectItemCaseSensitive(nsi_informationJSON, "nrfOauth2Required");
    if (nrf_oauth2_required) {
        cJSON *nrf_oauth2_required_local_map = NULL;
        if (!cJSON_IsObject(nrf_oauth2_required) && !cJSON_IsNull(nrf_oauth2_required)) {
            ogs_error("OpenAPI_nsi_information_parseFromJSON() failed [nrf_oauth2_required]");
            goto end;
        }
        if (cJSON_IsObject(nrf_oauth2_required)) {
            nrf_oauth2_requiredList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(nrf_oauth2_required_local_map, nrf_oauth2_required) {
                cJSON *localMapObject = nrf_oauth2_required_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsBool(localMapObject)) {
                    ogs_error("OpenAPI_nsi_information_parseFromJSON() failed [inner]");
                    goto end;
                }
                localInt = (int *)ogs_calloc(1, sizeof(int));
                if (!localInt) {
                    ogs_error("OpenAPI_nsi_information_parseFromJSON() failed [inner]");
                    goto end;
                }
                *localInt = localMapObject->valueint;
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), localInt);
                OpenAPI_list_add(nrf_oauth2_requiredList, localMapKeyPair);
            }
        }
    }

    nsi_information_local_var = OpenAPI_nsi_information_create (
        ogs_strdup(nrf_id->valuestring),
        nsi_id && !cJSON_IsNull(nsi_id) ? ogs_strdup(nsi_id->valuestring) : NULL,
        nrf_nf_mgt_uri && !cJSON_IsNull(nrf_nf_mgt_uri) ? ogs_strdup(nrf_nf_mgt_uri->valuestring) : NULL,
        nrf_access_token_uri && !cJSON_IsNull(nrf_access_token_uri) ? ogs_strdup(nrf_access_token_uri->valuestring) : NULL,
        nrf_oauth2_required ? nrf_oauth2_requiredList : NULL
    );

    return nsi_information_local_var;
end:
    if (nrf_oauth2_requiredList) {
        OpenAPI_list_for_each(nrf_oauth2_requiredList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_oauth2_requiredList);
        nrf_oauth2_requiredList = NULL;
    }
    return NULL;
}

OpenAPI_nsi_information_t *OpenAPI_nsi_information_copy(OpenAPI_nsi_information_t *dst, OpenAPI_nsi_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nsi_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nsi_information_convertToJSON() failed");
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

    OpenAPI_nsi_information_free(dst);
    dst = OpenAPI_nsi_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

