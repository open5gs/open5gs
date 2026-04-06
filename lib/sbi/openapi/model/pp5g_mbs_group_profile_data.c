
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pp5g_mbs_group_profile_data.h"

OpenAPI_pp5g_mbs_group_profile_data_t *OpenAPI_pp5g_mbs_group_profile_data_create(
    OpenAPI_list_t* allowed_mbs_infos,
    char *supported_features
)
{
    OpenAPI_pp5g_mbs_group_profile_data_t *pp5g_mbs_group_profile_data_local_var = ogs_malloc(sizeof(OpenAPI_pp5g_mbs_group_profile_data_t));
    ogs_assert(pp5g_mbs_group_profile_data_local_var);

    pp5g_mbs_group_profile_data_local_var->allowed_mbs_infos = allowed_mbs_infos;
    pp5g_mbs_group_profile_data_local_var->supported_features = supported_features;

    return pp5g_mbs_group_profile_data_local_var;
}

void OpenAPI_pp5g_mbs_group_profile_data_free(OpenAPI_pp5g_mbs_group_profile_data_t *pp5g_mbs_group_profile_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pp5g_mbs_group_profile_data) {
        return;
    }
    if (pp5g_mbs_group_profile_data->allowed_mbs_infos) {
        OpenAPI_list_for_each(pp5g_mbs_group_profile_data->allowed_mbs_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_allowed_mbs_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pp5g_mbs_group_profile_data->allowed_mbs_infos);
        pp5g_mbs_group_profile_data->allowed_mbs_infos = NULL;
    }
    if (pp5g_mbs_group_profile_data->supported_features) {
        ogs_free(pp5g_mbs_group_profile_data->supported_features);
        pp5g_mbs_group_profile_data->supported_features = NULL;
    }
    ogs_free(pp5g_mbs_group_profile_data);
}

cJSON *OpenAPI_pp5g_mbs_group_profile_data_convertToJSON(OpenAPI_pp5g_mbs_group_profile_data_t *pp5g_mbs_group_profile_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pp5g_mbs_group_profile_data == NULL) {
        ogs_error("OpenAPI_pp5g_mbs_group_profile_data_convertToJSON() failed [Pp5gMbsGroupProfileData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pp5g_mbs_group_profile_data->allowed_mbs_infos) {
    cJSON *allowed_mbs_infos = cJSON_AddObjectToObject(item, "allowedMbsInfos");
    if (allowed_mbs_infos == NULL) {
        ogs_error("OpenAPI_pp5g_mbs_group_profile_data_convertToJSON() failed [allowed_mbs_infos]");
        goto end;
    }
    cJSON *localMapObject = allowed_mbs_infos;
    if (pp5g_mbs_group_profile_data->allowed_mbs_infos) {
        OpenAPI_list_for_each(pp5g_mbs_group_profile_data->allowed_mbs_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_pp5g_mbs_group_profile_data_convertToJSON() failed [allowed_mbs_infos]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_pp5g_mbs_group_profile_data_convertToJSON() failed [allowed_mbs_infos]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_allowed_mbs_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pp5g_mbs_group_profile_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (pp5g_mbs_group_profile_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", pp5g_mbs_group_profile_data->supported_features) == NULL) {
        ogs_error("OpenAPI_pp5g_mbs_group_profile_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pp5g_mbs_group_profile_data_t *OpenAPI_pp5g_mbs_group_profile_data_parseFromJSON(cJSON *pp5g_mbs_group_profile_dataJSON)
{
    OpenAPI_pp5g_mbs_group_profile_data_t *pp5g_mbs_group_profile_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *allowed_mbs_infos = NULL;
    OpenAPI_list_t *allowed_mbs_infosList = NULL;
    cJSON *supported_features = NULL;
    allowed_mbs_infos = cJSON_GetObjectItemCaseSensitive(pp5g_mbs_group_profile_dataJSON, "allowedMbsInfos");
    if (allowed_mbs_infos) {
        cJSON *allowed_mbs_infos_local_map = NULL;
        if (!cJSON_IsObject(allowed_mbs_infos) && !cJSON_IsNull(allowed_mbs_infos)) {
            ogs_error("OpenAPI_pp5g_mbs_group_profile_data_parseFromJSON() failed [allowed_mbs_infos]");
            goto end;
        }
        if (cJSON_IsObject(allowed_mbs_infos)) {
            allowed_mbs_infosList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(allowed_mbs_infos_local_map, allowed_mbs_infos) {
                cJSON *localMapObject = allowed_mbs_infos_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_allowed_mbs_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_pp5g_mbs_group_profile_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(allowed_mbs_infosList, localMapKeyPair);
            }
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(pp5g_mbs_group_profile_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_pp5g_mbs_group_profile_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    pp5g_mbs_group_profile_data_local_var = OpenAPI_pp5g_mbs_group_profile_data_create (
        allowed_mbs_infos ? allowed_mbs_infosList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return pp5g_mbs_group_profile_data_local_var;
end:
    if (allowed_mbs_infosList) {
        OpenAPI_list_for_each(allowed_mbs_infosList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_allowed_mbs_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(allowed_mbs_infosList);
        allowed_mbs_infosList = NULL;
    }
    return NULL;
}

OpenAPI_pp5g_mbs_group_profile_data_t *OpenAPI_pp5g_mbs_group_profile_data_copy(OpenAPI_pp5g_mbs_group_profile_data_t *dst, OpenAPI_pp5g_mbs_group_profile_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pp5g_mbs_group_profile_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pp5g_mbs_group_profile_data_convertToJSON() failed");
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

    OpenAPI_pp5g_mbs_group_profile_data_free(dst);
    dst = OpenAPI_pp5g_mbs_group_profile_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

