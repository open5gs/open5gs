
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ee_profile_data.h"

OpenAPI_ee_profile_data_t *OpenAPI_ee_profile_data_create(
    OpenAPI_list_t *restricted_event_types,
    char *supported_features,
    OpenAPI_list_t* allowed_mtc_provider,
    bool is_iwk_epc_restricted,
    int iwk_epc_restricted,
    char *imsi,
    char *hss_group_id
)
{
    OpenAPI_ee_profile_data_t *ee_profile_data_local_var = ogs_malloc(sizeof(OpenAPI_ee_profile_data_t));
    ogs_assert(ee_profile_data_local_var);

    ee_profile_data_local_var->restricted_event_types = restricted_event_types;
    ee_profile_data_local_var->supported_features = supported_features;
    ee_profile_data_local_var->allowed_mtc_provider = allowed_mtc_provider;
    ee_profile_data_local_var->is_iwk_epc_restricted = is_iwk_epc_restricted;
    ee_profile_data_local_var->iwk_epc_restricted = iwk_epc_restricted;
    ee_profile_data_local_var->imsi = imsi;
    ee_profile_data_local_var->hss_group_id = hss_group_id;

    return ee_profile_data_local_var;
}

void OpenAPI_ee_profile_data_free(OpenAPI_ee_profile_data_t *ee_profile_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ee_profile_data) {
        return;
    }
    if (ee_profile_data->restricted_event_types) {
        OpenAPI_list_for_each(ee_profile_data->restricted_event_types, node) {
            OpenAPI_event_type_free(node->data);
        }
        OpenAPI_list_free(ee_profile_data->restricted_event_types);
        ee_profile_data->restricted_event_types = NULL;
    }
    if (ee_profile_data->supported_features) {
        ogs_free(ee_profile_data->supported_features);
        ee_profile_data->supported_features = NULL;
    }
    if (ee_profile_data->allowed_mtc_provider) {
        OpenAPI_list_for_each(ee_profile_data->allowed_mtc_provider, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ee_profile_data->allowed_mtc_provider);
        ee_profile_data->allowed_mtc_provider = NULL;
    }
    if (ee_profile_data->imsi) {
        ogs_free(ee_profile_data->imsi);
        ee_profile_data->imsi = NULL;
    }
    if (ee_profile_data->hss_group_id) {
        ogs_free(ee_profile_data->hss_group_id);
        ee_profile_data->hss_group_id = NULL;
    }
    ogs_free(ee_profile_data);
}

cJSON *OpenAPI_ee_profile_data_convertToJSON(OpenAPI_ee_profile_data_t *ee_profile_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ee_profile_data == NULL) {
        ogs_error("OpenAPI_ee_profile_data_convertToJSON() failed [EeProfileData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ee_profile_data->restricted_event_types) {
    cJSON *restricted_event_typesList = cJSON_AddArrayToObject(item, "restrictedEventTypes");
    if (restricted_event_typesList == NULL) {
        ogs_error("OpenAPI_ee_profile_data_convertToJSON() failed [restricted_event_types]");
        goto end;
    }
    OpenAPI_list_for_each(ee_profile_data->restricted_event_types, node) {
        cJSON *itemLocal = OpenAPI_event_type_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ee_profile_data_convertToJSON() failed [restricted_event_types]");
            goto end;
        }
        cJSON_AddItemToArray(restricted_event_typesList, itemLocal);
    }
    }

    if (ee_profile_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", ee_profile_data->supported_features) == NULL) {
        ogs_error("OpenAPI_ee_profile_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (ee_profile_data->allowed_mtc_provider) {
    cJSON *allowed_mtc_provider = cJSON_AddObjectToObject(item, "allowedMtcProvider");
    if (allowed_mtc_provider == NULL) {
        ogs_error("OpenAPI_ee_profile_data_convertToJSON() failed [allowed_mtc_provider]");
        goto end;
    }
    cJSON *localMapObject = allowed_mtc_provider;
    if (ee_profile_data->allowed_mtc_provider) {
        OpenAPI_list_for_each(ee_profile_data->allowed_mtc_provider, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ee_profile_data_convertToJSON() failed [allowed_mtc_provider]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ee_profile_data_convertToJSON() failed [allowed_mtc_provider]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_mtc_provider_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ee_profile_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (ee_profile_data->is_iwk_epc_restricted) {
    if (cJSON_AddBoolToObject(item, "iwkEpcRestricted", ee_profile_data->iwk_epc_restricted) == NULL) {
        ogs_error("OpenAPI_ee_profile_data_convertToJSON() failed [iwk_epc_restricted]");
        goto end;
    }
    }

    if (ee_profile_data->imsi) {
    if (cJSON_AddStringToObject(item, "imsi", ee_profile_data->imsi) == NULL) {
        ogs_error("OpenAPI_ee_profile_data_convertToJSON() failed [imsi]");
        goto end;
    }
    }

    if (ee_profile_data->hss_group_id) {
    if (cJSON_AddStringToObject(item, "hssGroupId", ee_profile_data->hss_group_id) == NULL) {
        ogs_error("OpenAPI_ee_profile_data_convertToJSON() failed [hss_group_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ee_profile_data_t *OpenAPI_ee_profile_data_parseFromJSON(cJSON *ee_profile_dataJSON)
{
    OpenAPI_ee_profile_data_t *ee_profile_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *restricted_event_types = NULL;
    OpenAPI_list_t *restricted_event_typesList = NULL;
    cJSON *supported_features = NULL;
    cJSON *allowed_mtc_provider = NULL;
    OpenAPI_list_t *allowed_mtc_providerList = NULL;
    cJSON *iwk_epc_restricted = NULL;
    cJSON *imsi = NULL;
    cJSON *hss_group_id = NULL;
    restricted_event_types = cJSON_GetObjectItemCaseSensitive(ee_profile_dataJSON, "restrictedEventTypes");
    if (restricted_event_types) {
        cJSON *restricted_event_types_local = NULL;
        if (!cJSON_IsArray(restricted_event_types)) {
            ogs_error("OpenAPI_ee_profile_data_parseFromJSON() failed [restricted_event_types]");
            goto end;
        }

        restricted_event_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(restricted_event_types_local, restricted_event_types) {
            if (!cJSON_IsObject(restricted_event_types_local)) {
                ogs_error("OpenAPI_ee_profile_data_parseFromJSON() failed [restricted_event_types]");
                goto end;
            }
            OpenAPI_event_type_t *restricted_event_typesItem = OpenAPI_event_type_parseFromJSON(restricted_event_types_local);
            if (!restricted_event_typesItem) {
                ogs_error("No restricted_event_typesItem");
                goto end;
            }
            OpenAPI_list_add(restricted_event_typesList, restricted_event_typesItem);
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(ee_profile_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_ee_profile_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    allowed_mtc_provider = cJSON_GetObjectItemCaseSensitive(ee_profile_dataJSON, "allowedMtcProvider");
    if (allowed_mtc_provider) {
        cJSON *allowed_mtc_provider_local_map = NULL;
        if (!cJSON_IsObject(allowed_mtc_provider) && !cJSON_IsNull(allowed_mtc_provider)) {
            ogs_error("OpenAPI_ee_profile_data_parseFromJSON() failed [allowed_mtc_provider]");
            goto end;
        }
        if (cJSON_IsObject(allowed_mtc_provider)) {
            allowed_mtc_providerList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(allowed_mtc_provider_local_map, allowed_mtc_provider) {
                cJSON *localMapObject = allowed_mtc_provider_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_mtc_provider_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_ee_profile_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(allowed_mtc_providerList, localMapKeyPair);
            }
        }
    }

    iwk_epc_restricted = cJSON_GetObjectItemCaseSensitive(ee_profile_dataJSON, "iwkEpcRestricted");
    if (iwk_epc_restricted) {
    if (!cJSON_IsBool(iwk_epc_restricted)) {
        ogs_error("OpenAPI_ee_profile_data_parseFromJSON() failed [iwk_epc_restricted]");
        goto end;
    }
    }

    imsi = cJSON_GetObjectItemCaseSensitive(ee_profile_dataJSON, "imsi");
    if (imsi) {
    if (!cJSON_IsString(imsi) && !cJSON_IsNull(imsi)) {
        ogs_error("OpenAPI_ee_profile_data_parseFromJSON() failed [imsi]");
        goto end;
    }
    }

    hss_group_id = cJSON_GetObjectItemCaseSensitive(ee_profile_dataJSON, "hssGroupId");
    if (hss_group_id) {
    if (!cJSON_IsString(hss_group_id) && !cJSON_IsNull(hss_group_id)) {
        ogs_error("OpenAPI_ee_profile_data_parseFromJSON() failed [hss_group_id]");
        goto end;
    }
    }

    ee_profile_data_local_var = OpenAPI_ee_profile_data_create (
        restricted_event_types ? restricted_event_typesList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        allowed_mtc_provider ? allowed_mtc_providerList : NULL,
        iwk_epc_restricted ? true : false,
        iwk_epc_restricted ? iwk_epc_restricted->valueint : 0,
        imsi && !cJSON_IsNull(imsi) ? ogs_strdup(imsi->valuestring) : NULL,
        hss_group_id && !cJSON_IsNull(hss_group_id) ? ogs_strdup(hss_group_id->valuestring) : NULL
    );

    return ee_profile_data_local_var;
end:
    if (restricted_event_typesList) {
        OpenAPI_list_for_each(restricted_event_typesList, node) {
            OpenAPI_event_type_free(node->data);
        }
        OpenAPI_list_free(restricted_event_typesList);
        restricted_event_typesList = NULL;
    }
    if (allowed_mtc_providerList) {
        OpenAPI_list_for_each(allowed_mtc_providerList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(allowed_mtc_providerList);
        allowed_mtc_providerList = NULL;
    }
    return NULL;
}

OpenAPI_ee_profile_data_t *OpenAPI_ee_profile_data_copy(OpenAPI_ee_profile_data_t *dst, OpenAPI_ee_profile_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ee_profile_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ee_profile_data_convertToJSON() failed");
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

    OpenAPI_ee_profile_data_free(dst);
    dst = OpenAPI_ee_profile_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

