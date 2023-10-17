
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_instance_info.h"

OpenAPI_nf_instance_info_t *OpenAPI_nf_instance_info_create(
    char *nrf_disc_api_uri,
    OpenAPI_preferred_search_t *preferred_search,
    OpenAPI_list_t* nrf_altered_priorities
)
{
    OpenAPI_nf_instance_info_t *nf_instance_info_local_var = ogs_malloc(sizeof(OpenAPI_nf_instance_info_t));
    ogs_assert(nf_instance_info_local_var);

    nf_instance_info_local_var->nrf_disc_api_uri = nrf_disc_api_uri;
    nf_instance_info_local_var->preferred_search = preferred_search;
    nf_instance_info_local_var->nrf_altered_priorities = nrf_altered_priorities;

    return nf_instance_info_local_var;
}

void OpenAPI_nf_instance_info_free(OpenAPI_nf_instance_info_t *nf_instance_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nf_instance_info) {
        return;
    }
    if (nf_instance_info->nrf_disc_api_uri) {
        ogs_free(nf_instance_info->nrf_disc_api_uri);
        nf_instance_info->nrf_disc_api_uri = NULL;
    }
    if (nf_instance_info->preferred_search) {
        OpenAPI_preferred_search_free(nf_instance_info->preferred_search);
        nf_instance_info->preferred_search = NULL;
    }
    if (nf_instance_info->nrf_altered_priorities) {
        OpenAPI_list_for_each(nf_instance_info->nrf_altered_priorities, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nf_instance_info->nrf_altered_priorities);
        nf_instance_info->nrf_altered_priorities = NULL;
    }
    ogs_free(nf_instance_info);
}

cJSON *OpenAPI_nf_instance_info_convertToJSON(OpenAPI_nf_instance_info_t *nf_instance_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nf_instance_info == NULL) {
        ogs_error("OpenAPI_nf_instance_info_convertToJSON() failed [NfInstanceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nf_instance_info->nrf_disc_api_uri) {
    if (cJSON_AddStringToObject(item, "nrfDiscApiUri", nf_instance_info->nrf_disc_api_uri) == NULL) {
        ogs_error("OpenAPI_nf_instance_info_convertToJSON() failed [nrf_disc_api_uri]");
        goto end;
    }
    }

    if (nf_instance_info->preferred_search) {
    cJSON *preferred_search_local_JSON = OpenAPI_preferred_search_convertToJSON(nf_instance_info->preferred_search);
    if (preferred_search_local_JSON == NULL) {
        ogs_error("OpenAPI_nf_instance_info_convertToJSON() failed [preferred_search]");
        goto end;
    }
    cJSON_AddItemToObject(item, "preferredSearch", preferred_search_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nf_instance_info_convertToJSON() failed [preferred_search]");
        goto end;
    }
    }

    if (nf_instance_info->nrf_altered_priorities) {
    cJSON *nrf_altered_priorities = cJSON_AddObjectToObject(item, "nrfAlteredPriorities");
    if (nrf_altered_priorities == NULL) {
        ogs_error("OpenAPI_nf_instance_info_convertToJSON() failed [nrf_altered_priorities]");
        goto end;
    }
    cJSON *localMapObject = nrf_altered_priorities;
    if (nf_instance_info->nrf_altered_priorities) {
        OpenAPI_list_for_each(nf_instance_info->nrf_altered_priorities, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nf_instance_info_convertToJSON() failed [nrf_altered_priorities]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nf_instance_info_convertToJSON() failed [nrf_altered_priorities]");
                goto end;
            }
            if (localKeyValue->value == NULL) {
                ogs_error("OpenAPI_nf_instance_info_convertToJSON() failed [inner]");
                goto end;
            }
            if (cJSON_AddNumberToObject(localMapObject, localKeyValue->key, *(double *)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_nf_instance_info_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

end:
    return item;
}

OpenAPI_nf_instance_info_t *OpenAPI_nf_instance_info_parseFromJSON(cJSON *nf_instance_infoJSON)
{
    OpenAPI_nf_instance_info_t *nf_instance_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nrf_disc_api_uri = NULL;
    cJSON *preferred_search = NULL;
    OpenAPI_preferred_search_t *preferred_search_local_nonprim = NULL;
    cJSON *nrf_altered_priorities = NULL;
    OpenAPI_list_t *nrf_altered_prioritiesList = NULL;
    nrf_disc_api_uri = cJSON_GetObjectItemCaseSensitive(nf_instance_infoJSON, "nrfDiscApiUri");
    if (nrf_disc_api_uri) {
    if (!cJSON_IsString(nrf_disc_api_uri) && !cJSON_IsNull(nrf_disc_api_uri)) {
        ogs_error("OpenAPI_nf_instance_info_parseFromJSON() failed [nrf_disc_api_uri]");
        goto end;
    }
    }

    preferred_search = cJSON_GetObjectItemCaseSensitive(nf_instance_infoJSON, "preferredSearch");
    if (preferred_search) {
    preferred_search_local_nonprim = OpenAPI_preferred_search_parseFromJSON(preferred_search);
    if (!preferred_search_local_nonprim) {
        ogs_error("OpenAPI_preferred_search_parseFromJSON failed [preferred_search]");
        goto end;
    }
    }

    nrf_altered_priorities = cJSON_GetObjectItemCaseSensitive(nf_instance_infoJSON, "nrfAlteredPriorities");
    if (nrf_altered_priorities) {
        cJSON *nrf_altered_priorities_local_map = NULL;
        if (!cJSON_IsObject(nrf_altered_priorities) && !cJSON_IsNull(nrf_altered_priorities)) {
            ogs_error("OpenAPI_nf_instance_info_parseFromJSON() failed [nrf_altered_priorities]");
            goto end;
        }
        if (cJSON_IsObject(nrf_altered_priorities)) {
            nrf_altered_prioritiesList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(nrf_altered_priorities_local_map, nrf_altered_priorities) {
                cJSON *localMapObject = nrf_altered_priorities_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsNumber(localMapObject)) {
                    ogs_error("OpenAPI_nf_instance_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                localDouble = (double *)ogs_calloc(1, sizeof(double));
                if (!localDouble) {
                    ogs_error("OpenAPI_nf_instance_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                *localDouble = localMapObject->valuedouble;
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), localDouble);
                OpenAPI_list_add(nrf_altered_prioritiesList, localMapKeyPair);
            }
        }
    }

    nf_instance_info_local_var = OpenAPI_nf_instance_info_create (
        nrf_disc_api_uri && !cJSON_IsNull(nrf_disc_api_uri) ? ogs_strdup(nrf_disc_api_uri->valuestring) : NULL,
        preferred_search ? preferred_search_local_nonprim : NULL,
        nrf_altered_priorities ? nrf_altered_prioritiesList : NULL
    );

    return nf_instance_info_local_var;
end:
    if (preferred_search_local_nonprim) {
        OpenAPI_preferred_search_free(preferred_search_local_nonprim);
        preferred_search_local_nonprim = NULL;
    }
    if (nrf_altered_prioritiesList) {
        OpenAPI_list_for_each(nrf_altered_prioritiesList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_altered_prioritiesList);
        nrf_altered_prioritiesList = NULL;
    }
    return NULL;
}

OpenAPI_nf_instance_info_t *OpenAPI_nf_instance_info_copy(OpenAPI_nf_instance_info_t *dst, OpenAPI_nf_instance_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nf_instance_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nf_instance_info_convertToJSON() failed");
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

    OpenAPI_nf_instance_info_free(dst);
    dst = OpenAPI_nf_instance_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

