
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pp5g_vn_group_profile_data.h"

OpenAPI_pp5g_vn_group_profile_data_t *OpenAPI_pp5g_vn_group_profile_data_create(
    OpenAPI_list_t* allowed_mtc_providers,
    char *supported_features
)
{
    OpenAPI_pp5g_vn_group_profile_data_t *pp5g_vn_group_profile_data_local_var = ogs_malloc(sizeof(OpenAPI_pp5g_vn_group_profile_data_t));
    ogs_assert(pp5g_vn_group_profile_data_local_var);

    pp5g_vn_group_profile_data_local_var->allowed_mtc_providers = allowed_mtc_providers;
    pp5g_vn_group_profile_data_local_var->supported_features = supported_features;

    return pp5g_vn_group_profile_data_local_var;
}

void OpenAPI_pp5g_vn_group_profile_data_free(OpenAPI_pp5g_vn_group_profile_data_t *pp5g_vn_group_profile_data)
{
    if (NULL == pp5g_vn_group_profile_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(pp5g_vn_group_profile_data->allowed_mtc_providers, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        ogs_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(pp5g_vn_group_profile_data->allowed_mtc_providers);
    ogs_free(pp5g_vn_group_profile_data->supported_features);
    ogs_free(pp5g_vn_group_profile_data);
}

cJSON *OpenAPI_pp5g_vn_group_profile_data_convertToJSON(OpenAPI_pp5g_vn_group_profile_data_t *pp5g_vn_group_profile_data)
{
    cJSON *item = NULL;

    if (pp5g_vn_group_profile_data == NULL) {
        ogs_error("OpenAPI_pp5g_vn_group_profile_data_convertToJSON() failed [Pp5gVnGroupProfileData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pp5g_vn_group_profile_data->allowed_mtc_providers) {
    cJSON *allowed_mtc_providers = cJSON_AddObjectToObject(item, "allowedMtcProviders");
    if (allowed_mtc_providers == NULL) {
        ogs_error("OpenAPI_pp5g_vn_group_profile_data_convertToJSON() failed [allowed_mtc_providers]");
        goto end;
    }
    cJSON *localMapObject = allowed_mtc_providers;
    OpenAPI_lnode_t *allowed_mtc_providers_node;
    if (pp5g_vn_group_profile_data->allowed_mtc_providers) {
        OpenAPI_list_for_each(pp5g_vn_group_profile_data->allowed_mtc_providers, allowed_mtc_providers_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)allowed_mtc_providers_node->data;
            }
        }
    }

    if (pp5g_vn_group_profile_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", pp5g_vn_group_profile_data->supported_features) == NULL) {
        ogs_error("OpenAPI_pp5g_vn_group_profile_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pp5g_vn_group_profile_data_t *OpenAPI_pp5g_vn_group_profile_data_parseFromJSON(cJSON *pp5g_vn_group_profile_dataJSON)
{
    OpenAPI_pp5g_vn_group_profile_data_t *pp5g_vn_group_profile_data_local_var = NULL;
    cJSON *allowed_mtc_providers = cJSON_GetObjectItemCaseSensitive(pp5g_vn_group_profile_dataJSON, "allowedMtcProviders");

    OpenAPI_list_t *allowed_mtc_providersList;
    if (allowed_mtc_providers) {
    cJSON *allowed_mtc_providers_local_map;
    if (!cJSON_IsObject(allowed_mtc_providers)) {
        ogs_error("OpenAPI_pp5g_vn_group_profile_data_parseFromJSON() failed [allowed_mtc_providers]");
        goto end;
    }
    allowed_mtc_providersList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(allowed_mtc_providers_local_map, allowed_mtc_providers) {
        cJSON *localMapObject = allowed_mtc_providers_local_map;
        OpenAPI_list_add(allowed_mtc_providersList , localMapKeyPair);
    }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(pp5g_vn_group_profile_dataJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_pp5g_vn_group_profile_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    pp5g_vn_group_profile_data_local_var = OpenAPI_pp5g_vn_group_profile_data_create (
        allowed_mtc_providers ? allowed_mtc_providersList : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return pp5g_vn_group_profile_data_local_var;
end:
    return NULL;
}

OpenAPI_pp5g_vn_group_profile_data_t *OpenAPI_pp5g_vn_group_profile_data_copy(OpenAPI_pp5g_vn_group_profile_data_t *dst, OpenAPI_pp5g_vn_group_profile_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pp5g_vn_group_profile_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pp5g_vn_group_profile_data_convertToJSON() failed");
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

    OpenAPI_pp5g_vn_group_profile_data_free(dst);
    dst = OpenAPI_pp5g_vn_group_profile_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

