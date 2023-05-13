
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pp_profile_data.h"

OpenAPI_pp_profile_data_t *OpenAPI_pp_profile_data_create(
    OpenAPI_list_t* allowed_mtc_providers,
    char *supported_features
)
{
    OpenAPI_pp_profile_data_t *pp_profile_data_local_var = ogs_malloc(sizeof(OpenAPI_pp_profile_data_t));
    ogs_assert(pp_profile_data_local_var);

    pp_profile_data_local_var->allowed_mtc_providers = allowed_mtc_providers;
    pp_profile_data_local_var->supported_features = supported_features;

    return pp_profile_data_local_var;
}

void OpenAPI_pp_profile_data_free(OpenAPI_pp_profile_data_t *pp_profile_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pp_profile_data) {
        return;
    }
    if (pp_profile_data->allowed_mtc_providers) {
        OpenAPI_list_for_each(pp_profile_data->allowed_mtc_providers, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pp_profile_data->allowed_mtc_providers);
        pp_profile_data->allowed_mtc_providers = NULL;
    }
    if (pp_profile_data->supported_features) {
        ogs_free(pp_profile_data->supported_features);
        pp_profile_data->supported_features = NULL;
    }
    ogs_free(pp_profile_data);
}

cJSON *OpenAPI_pp_profile_data_convertToJSON(OpenAPI_pp_profile_data_t *pp_profile_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pp_profile_data == NULL) {
        ogs_error("OpenAPI_pp_profile_data_convertToJSON() failed [PpProfileData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pp_profile_data->allowed_mtc_providers) {
    cJSON *allowed_mtc_providers = cJSON_AddObjectToObject(item, "allowedMtcProviders");
    if (allowed_mtc_providers == NULL) {
        ogs_error("OpenAPI_pp_profile_data_convertToJSON() failed [allowed_mtc_providers]");
        goto end;
    }
    cJSON *localMapObject = allowed_mtc_providers;
    if (pp_profile_data->allowed_mtc_providers) {
        OpenAPI_list_for_each(pp_profile_data->allowed_mtc_providers, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_pp_profile_data_convertToJSON() failed [allowed_mtc_providers]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_pp_profile_data_convertToJSON() failed [allowed_mtc_providers]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_allowed_mtc_provider_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pp_profile_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (pp_profile_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", pp_profile_data->supported_features) == NULL) {
        ogs_error("OpenAPI_pp_profile_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pp_profile_data_t *OpenAPI_pp_profile_data_parseFromJSON(cJSON *pp_profile_dataJSON)
{
    OpenAPI_pp_profile_data_t *pp_profile_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *allowed_mtc_providers = NULL;
    OpenAPI_list_t *allowed_mtc_providersList = NULL;
    cJSON *supported_features = NULL;
    allowed_mtc_providers = cJSON_GetObjectItemCaseSensitive(pp_profile_dataJSON, "allowedMtcProviders");
    if (allowed_mtc_providers) {
        cJSON *allowed_mtc_providers_local_map = NULL;
        if (!cJSON_IsObject(allowed_mtc_providers) && !cJSON_IsNull(allowed_mtc_providers)) {
            ogs_error("OpenAPI_pp_profile_data_parseFromJSON() failed [allowed_mtc_providers]");
            goto end;
        }
        if (cJSON_IsObject(allowed_mtc_providers)) {
            allowed_mtc_providersList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(allowed_mtc_providers_local_map, allowed_mtc_providers) {
                cJSON *localMapObject = allowed_mtc_providers_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_allowed_mtc_provider_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_pp_profile_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(allowed_mtc_providersList, localMapKeyPair);
            }
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(pp_profile_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_pp_profile_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    pp_profile_data_local_var = OpenAPI_pp_profile_data_create (
        allowed_mtc_providers ? allowed_mtc_providersList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return pp_profile_data_local_var;
end:
    if (allowed_mtc_providersList) {
        OpenAPI_list_for_each(allowed_mtc_providersList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(allowed_mtc_providersList);
        allowed_mtc_providersList = NULL;
    }
    return NULL;
}

OpenAPI_pp_profile_data_t *OpenAPI_pp_profile_data_copy(OpenAPI_pp_profile_data_t *dst, OpenAPI_pp_profile_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pp_profile_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pp_profile_data_convertToJSON() failed");
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

    OpenAPI_pp_profile_data_free(dst);
    dst = OpenAPI_pp_profile_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

