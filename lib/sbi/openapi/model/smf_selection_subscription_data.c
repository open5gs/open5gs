
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_selection_subscription_data.h"

OpenAPI_smf_selection_subscription_data_t *OpenAPI_smf_selection_subscription_data_create(
    char *supported_features,
    OpenAPI_list_t* subscribed_snssai_infos,
    char *shared_snssai_infos_id,
    char *hss_group_id
)
{
    OpenAPI_smf_selection_subscription_data_t *smf_selection_subscription_data_local_var = ogs_malloc(sizeof(OpenAPI_smf_selection_subscription_data_t));
    ogs_assert(smf_selection_subscription_data_local_var);

    smf_selection_subscription_data_local_var->supported_features = supported_features;
    smf_selection_subscription_data_local_var->subscribed_snssai_infos = subscribed_snssai_infos;
    smf_selection_subscription_data_local_var->shared_snssai_infos_id = shared_snssai_infos_id;
    smf_selection_subscription_data_local_var->hss_group_id = hss_group_id;

    return smf_selection_subscription_data_local_var;
}

void OpenAPI_smf_selection_subscription_data_free(OpenAPI_smf_selection_subscription_data_t *smf_selection_subscription_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == smf_selection_subscription_data) {
        return;
    }
    if (smf_selection_subscription_data->supported_features) {
        ogs_free(smf_selection_subscription_data->supported_features);
        smf_selection_subscription_data->supported_features = NULL;
    }
    if (smf_selection_subscription_data->subscribed_snssai_infos) {
        OpenAPI_list_for_each(smf_selection_subscription_data->subscribed_snssai_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_snssai_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(smf_selection_subscription_data->subscribed_snssai_infos);
        smf_selection_subscription_data->subscribed_snssai_infos = NULL;
    }
    if (smf_selection_subscription_data->shared_snssai_infos_id) {
        ogs_free(smf_selection_subscription_data->shared_snssai_infos_id);
        smf_selection_subscription_data->shared_snssai_infos_id = NULL;
    }
    if (smf_selection_subscription_data->hss_group_id) {
        ogs_free(smf_selection_subscription_data->hss_group_id);
        smf_selection_subscription_data->hss_group_id = NULL;
    }
    ogs_free(smf_selection_subscription_data);
}

cJSON *OpenAPI_smf_selection_subscription_data_convertToJSON(OpenAPI_smf_selection_subscription_data_t *smf_selection_subscription_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (smf_selection_subscription_data == NULL) {
        ogs_error("OpenAPI_smf_selection_subscription_data_convertToJSON() failed [SmfSelectionSubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (smf_selection_subscription_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", smf_selection_subscription_data->supported_features) == NULL) {
        ogs_error("OpenAPI_smf_selection_subscription_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (smf_selection_subscription_data->subscribed_snssai_infos) {
    cJSON *subscribed_snssai_infos = cJSON_AddObjectToObject(item, "subscribedSnssaiInfos");
    if (subscribed_snssai_infos == NULL) {
        ogs_error("OpenAPI_smf_selection_subscription_data_convertToJSON() failed [subscribed_snssai_infos]");
        goto end;
    }
    cJSON *localMapObject = subscribed_snssai_infos;
    if (smf_selection_subscription_data->subscribed_snssai_infos) {
        OpenAPI_list_for_each(smf_selection_subscription_data->subscribed_snssai_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_smf_selection_subscription_data_convertToJSON() failed [subscribed_snssai_infos]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_smf_selection_subscription_data_convertToJSON() failed [subscribed_snssai_infos]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_snssai_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_smf_selection_subscription_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (smf_selection_subscription_data->shared_snssai_infos_id) {
    if (cJSON_AddStringToObject(item, "sharedSnssaiInfosId", smf_selection_subscription_data->shared_snssai_infos_id) == NULL) {
        ogs_error("OpenAPI_smf_selection_subscription_data_convertToJSON() failed [shared_snssai_infos_id]");
        goto end;
    }
    }

    if (smf_selection_subscription_data->hss_group_id) {
    if (cJSON_AddStringToObject(item, "hssGroupId", smf_selection_subscription_data->hss_group_id) == NULL) {
        ogs_error("OpenAPI_smf_selection_subscription_data_convertToJSON() failed [hss_group_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_smf_selection_subscription_data_t *OpenAPI_smf_selection_subscription_data_parseFromJSON(cJSON *smf_selection_subscription_dataJSON)
{
    OpenAPI_smf_selection_subscription_data_t *smf_selection_subscription_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supported_features = NULL;
    cJSON *subscribed_snssai_infos = NULL;
    OpenAPI_list_t *subscribed_snssai_infosList = NULL;
    cJSON *shared_snssai_infos_id = NULL;
    cJSON *hss_group_id = NULL;
    supported_features = cJSON_GetObjectItemCaseSensitive(smf_selection_subscription_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_smf_selection_subscription_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    subscribed_snssai_infos = cJSON_GetObjectItemCaseSensitive(smf_selection_subscription_dataJSON, "subscribedSnssaiInfos");
    if (subscribed_snssai_infos) {
        cJSON *subscribed_snssai_infos_local_map = NULL;
        if (!cJSON_IsObject(subscribed_snssai_infos) && !cJSON_IsNull(subscribed_snssai_infos)) {
            ogs_error("OpenAPI_smf_selection_subscription_data_parseFromJSON() failed [subscribed_snssai_infos]");
            goto end;
        }
        if (cJSON_IsObject(subscribed_snssai_infos)) {
            subscribed_snssai_infosList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(subscribed_snssai_infos_local_map, subscribed_snssai_infos) {
                cJSON *localMapObject = subscribed_snssai_infos_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_snssai_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_smf_selection_subscription_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(subscribed_snssai_infosList, localMapKeyPair);
            }
        }
    }

    shared_snssai_infos_id = cJSON_GetObjectItemCaseSensitive(smf_selection_subscription_dataJSON, "sharedSnssaiInfosId");
    if (shared_snssai_infos_id) {
    if (!cJSON_IsString(shared_snssai_infos_id) && !cJSON_IsNull(shared_snssai_infos_id)) {
        ogs_error("OpenAPI_smf_selection_subscription_data_parseFromJSON() failed [shared_snssai_infos_id]");
        goto end;
    }
    }

    hss_group_id = cJSON_GetObjectItemCaseSensitive(smf_selection_subscription_dataJSON, "hssGroupId");
    if (hss_group_id) {
    if (!cJSON_IsString(hss_group_id) && !cJSON_IsNull(hss_group_id)) {
        ogs_error("OpenAPI_smf_selection_subscription_data_parseFromJSON() failed [hss_group_id]");
        goto end;
    }
    }

    smf_selection_subscription_data_local_var = OpenAPI_smf_selection_subscription_data_create (
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        subscribed_snssai_infos ? subscribed_snssai_infosList : NULL,
        shared_snssai_infos_id && !cJSON_IsNull(shared_snssai_infos_id) ? ogs_strdup(shared_snssai_infos_id->valuestring) : NULL,
        hss_group_id && !cJSON_IsNull(hss_group_id) ? ogs_strdup(hss_group_id->valuestring) : NULL
    );

    return smf_selection_subscription_data_local_var;
end:
    if (subscribed_snssai_infosList) {
        OpenAPI_list_for_each(subscribed_snssai_infosList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_snssai_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(subscribed_snssai_infosList);
        subscribed_snssai_infosList = NULL;
    }
    return NULL;
}

OpenAPI_smf_selection_subscription_data_t *OpenAPI_smf_selection_subscription_data_copy(OpenAPI_smf_selection_subscription_data_t *dst, OpenAPI_smf_selection_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smf_selection_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smf_selection_subscription_data_convertToJSON() failed");
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

    OpenAPI_smf_selection_subscription_data_free(dst);
    dst = OpenAPI_smf_selection_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

