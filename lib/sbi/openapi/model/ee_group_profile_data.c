
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ee_group_profile_data.h"

OpenAPI_ee_group_profile_data_t *OpenAPI_ee_group_profile_data_create(
    OpenAPI_list_t *restricted_event_types,
    OpenAPI_list_t* allowed_mtc_provider,
    char *supported_features
)
{
    OpenAPI_ee_group_profile_data_t *ee_group_profile_data_local_var = ogs_malloc(sizeof(OpenAPI_ee_group_profile_data_t));
    ogs_assert(ee_group_profile_data_local_var);

    ee_group_profile_data_local_var->restricted_event_types = restricted_event_types;
    ee_group_profile_data_local_var->allowed_mtc_provider = allowed_mtc_provider;
    ee_group_profile_data_local_var->supported_features = supported_features;

    return ee_group_profile_data_local_var;
}

void OpenAPI_ee_group_profile_data_free(OpenAPI_ee_group_profile_data_t *ee_group_profile_data)
{
    if (NULL == ee_group_profile_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(ee_group_profile_data->restricted_event_types, node) {
        OpenAPI_event_type_free(node->data);
    }
    OpenAPI_list_free(ee_group_profile_data->restricted_event_types);
    OpenAPI_list_for_each(ee_group_profile_data->allowed_mtc_provider, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        ogs_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(ee_group_profile_data->allowed_mtc_provider);
    ogs_free(ee_group_profile_data->supported_features);
    ogs_free(ee_group_profile_data);
}

cJSON *OpenAPI_ee_group_profile_data_convertToJSON(OpenAPI_ee_group_profile_data_t *ee_group_profile_data)
{
    cJSON *item = NULL;

    if (ee_group_profile_data == NULL) {
        ogs_error("OpenAPI_ee_group_profile_data_convertToJSON() failed [EeGroupProfileData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ee_group_profile_data->restricted_event_types) {
    cJSON *restricted_event_typesList = cJSON_AddArrayToObject(item, "restrictedEventTypes");
    if (restricted_event_typesList == NULL) {
        ogs_error("OpenAPI_ee_group_profile_data_convertToJSON() failed [restricted_event_types]");
        goto end;
    }

    OpenAPI_lnode_t *restricted_event_types_node;
    if (ee_group_profile_data->restricted_event_types) {
        OpenAPI_list_for_each(ee_group_profile_data->restricted_event_types, restricted_event_types_node) {
            cJSON *itemLocal = OpenAPI_event_type_convertToJSON(restricted_event_types_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ee_group_profile_data_convertToJSON() failed [restricted_event_types]");
                goto end;
            }
            cJSON_AddItemToArray(restricted_event_typesList, itemLocal);
        }
    }
    }

    if (ee_group_profile_data->allowed_mtc_provider) {
    cJSON *allowed_mtc_provider = cJSON_AddObjectToObject(item, "allowedMtcProvider");
    if (allowed_mtc_provider == NULL) {
        ogs_error("OpenAPI_ee_group_profile_data_convertToJSON() failed [allowed_mtc_provider]");
        goto end;
    }
    cJSON *localMapObject = allowed_mtc_provider;
    OpenAPI_lnode_t *allowed_mtc_provider_node;
    if (ee_group_profile_data->allowed_mtc_provider) {
        OpenAPI_list_for_each(ee_group_profile_data->allowed_mtc_provider, allowed_mtc_provider_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)allowed_mtc_provider_node->data;
            }
        }
    }

    if (ee_group_profile_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", ee_group_profile_data->supported_features) == NULL) {
        ogs_error("OpenAPI_ee_group_profile_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ee_group_profile_data_t *OpenAPI_ee_group_profile_data_parseFromJSON(cJSON *ee_group_profile_dataJSON)
{
    OpenAPI_ee_group_profile_data_t *ee_group_profile_data_local_var = NULL;
    cJSON *restricted_event_types = cJSON_GetObjectItemCaseSensitive(ee_group_profile_dataJSON, "restrictedEventTypes");

    OpenAPI_list_t *restricted_event_typesList;
    if (restricted_event_types) {
    cJSON *restricted_event_types_local_nonprimitive;
    if (!cJSON_IsArray(restricted_event_types)){
        ogs_error("OpenAPI_ee_group_profile_data_parseFromJSON() failed [restricted_event_types]");
        goto end;
    }

    restricted_event_typesList = OpenAPI_list_create();

    cJSON_ArrayForEach(restricted_event_types_local_nonprimitive, restricted_event_types ) {
        if (!cJSON_IsObject(restricted_event_types_local_nonprimitive)) {
            ogs_error("OpenAPI_ee_group_profile_data_parseFromJSON() failed [restricted_event_types]");
            goto end;
        }
        OpenAPI_event_type_t *restricted_event_typesItem = OpenAPI_event_type_parseFromJSON(restricted_event_types_local_nonprimitive);

        if (!restricted_event_typesItem) {
            ogs_error("No restricted_event_typesItem");
            OpenAPI_list_free(restricted_event_typesList);
            goto end;
        }

        OpenAPI_list_add(restricted_event_typesList, restricted_event_typesItem);
    }
    }

    cJSON *allowed_mtc_provider = cJSON_GetObjectItemCaseSensitive(ee_group_profile_dataJSON, "allowedMtcProvider");

    OpenAPI_list_t *allowed_mtc_providerList;
    if (allowed_mtc_provider) {
    cJSON *allowed_mtc_provider_local_map;
    if (!cJSON_IsObject(allowed_mtc_provider)) {
        ogs_error("OpenAPI_ee_group_profile_data_parseFromJSON() failed [allowed_mtc_provider]");
        goto end;
    }
    allowed_mtc_providerList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(allowed_mtc_provider_local_map, allowed_mtc_provider) {
        cJSON *localMapObject = allowed_mtc_provider_local_map;
        OpenAPI_list_add(allowed_mtc_providerList , localMapKeyPair);
    }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(ee_group_profile_dataJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_ee_group_profile_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    ee_group_profile_data_local_var = OpenAPI_ee_group_profile_data_create (
        restricted_event_types ? restricted_event_typesList : NULL,
        allowed_mtc_provider ? allowed_mtc_providerList : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return ee_group_profile_data_local_var;
end:
    return NULL;
}

OpenAPI_ee_group_profile_data_t *OpenAPI_ee_group_profile_data_copy(OpenAPI_ee_group_profile_data_t *dst, OpenAPI_ee_group_profile_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ee_group_profile_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ee_group_profile_data_convertToJSON() failed");
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

    OpenAPI_ee_group_profile_data_free(dst);
    dst = OpenAPI_ee_group_profile_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

