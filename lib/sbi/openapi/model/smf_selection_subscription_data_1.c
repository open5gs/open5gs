
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_selection_subscription_data_1.h"

OpenAPI_smf_selection_subscription_data_1_t *OpenAPI_smf_selection_subscription_data_1_create(
    char *supported_features,
    OpenAPI_list_t* subscribed_snssai_infos,
    char *shared_snssai_infos_id
)
{
    OpenAPI_smf_selection_subscription_data_1_t *smf_selection_subscription_data_1_local_var = ogs_malloc(sizeof(OpenAPI_smf_selection_subscription_data_1_t));
    ogs_assert(smf_selection_subscription_data_1_local_var);

    smf_selection_subscription_data_1_local_var->supported_features = supported_features;
    smf_selection_subscription_data_1_local_var->subscribed_snssai_infos = subscribed_snssai_infos;
    smf_selection_subscription_data_1_local_var->shared_snssai_infos_id = shared_snssai_infos_id;

    return smf_selection_subscription_data_1_local_var;
}

void OpenAPI_smf_selection_subscription_data_1_free(OpenAPI_smf_selection_subscription_data_1_t *smf_selection_subscription_data_1)
{
    if (NULL == smf_selection_subscription_data_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(smf_selection_subscription_data_1->supported_features);
    OpenAPI_list_for_each(smf_selection_subscription_data_1->subscribed_snssai_infos, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_snssai_info_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(smf_selection_subscription_data_1->subscribed_snssai_infos);
    ogs_free(smf_selection_subscription_data_1->shared_snssai_infos_id);
    ogs_free(smf_selection_subscription_data_1);
}

cJSON *OpenAPI_smf_selection_subscription_data_1_convertToJSON(OpenAPI_smf_selection_subscription_data_1_t *smf_selection_subscription_data_1)
{
    cJSON *item = NULL;

    if (smf_selection_subscription_data_1 == NULL) {
        ogs_error("OpenAPI_smf_selection_subscription_data_1_convertToJSON() failed [SmfSelectionSubscriptionData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (smf_selection_subscription_data_1->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", smf_selection_subscription_data_1->supported_features) == NULL) {
        ogs_error("OpenAPI_smf_selection_subscription_data_1_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (smf_selection_subscription_data_1->subscribed_snssai_infos) {
    cJSON *subscribed_snssai_infos = cJSON_AddObjectToObject(item, "subscribedSnssaiInfos");
    if (subscribed_snssai_infos == NULL) {
        ogs_error("OpenAPI_smf_selection_subscription_data_1_convertToJSON() failed [subscribed_snssai_infos]");
        goto end;
    }
    cJSON *localMapObject = subscribed_snssai_infos;
    OpenAPI_lnode_t *subscribed_snssai_infos_node;
    if (smf_selection_subscription_data_1->subscribed_snssai_infos) {
        OpenAPI_list_for_each(smf_selection_subscription_data_1->subscribed_snssai_infos, subscribed_snssai_infos_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)subscribed_snssai_infos_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_snssai_info_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_smf_selection_subscription_data_1_convertToJSON() failed [subscribed_snssai_infos]");
            goto end;
        }
        cJSON_AddItemToObject(subscribed_snssai_infos, localKeyValue->key, itemLocal);
            }
        }
    }

    if (smf_selection_subscription_data_1->shared_snssai_infos_id) {
    if (cJSON_AddStringToObject(item, "sharedSnssaiInfosId", smf_selection_subscription_data_1->shared_snssai_infos_id) == NULL) {
        ogs_error("OpenAPI_smf_selection_subscription_data_1_convertToJSON() failed [shared_snssai_infos_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_smf_selection_subscription_data_1_t *OpenAPI_smf_selection_subscription_data_1_parseFromJSON(cJSON *smf_selection_subscription_data_1JSON)
{
    OpenAPI_smf_selection_subscription_data_1_t *smf_selection_subscription_data_1_local_var = NULL;
    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(smf_selection_subscription_data_1JSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_smf_selection_subscription_data_1_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *subscribed_snssai_infos = cJSON_GetObjectItemCaseSensitive(smf_selection_subscription_data_1JSON, "subscribedSnssaiInfos");

    OpenAPI_list_t *subscribed_snssai_infosList;
    if (subscribed_snssai_infos) {
    cJSON *subscribed_snssai_infos_local_map;
    if (!cJSON_IsObject(subscribed_snssai_infos)) {
        ogs_error("OpenAPI_smf_selection_subscription_data_1_parseFromJSON() failed [subscribed_snssai_infos]");
        goto end;
    }
    subscribed_snssai_infosList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(subscribed_snssai_infos_local_map, subscribed_snssai_infos) {
        cJSON *localMapObject = subscribed_snssai_infos_local_map;
        if (cJSON_IsObject(subscribed_snssai_infos_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_snssai_info_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(subscribed_snssai_infos_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_smf_selection_subscription_data_1_parseFromJSON() failed [subscribed_snssai_infos]");
            goto end;
        }
        OpenAPI_list_add(subscribed_snssai_infosList , localMapKeyPair);
    }
    }

    cJSON *shared_snssai_infos_id = cJSON_GetObjectItemCaseSensitive(smf_selection_subscription_data_1JSON, "sharedSnssaiInfosId");

    if (shared_snssai_infos_id) {
    if (!cJSON_IsString(shared_snssai_infos_id)) {
        ogs_error("OpenAPI_smf_selection_subscription_data_1_parseFromJSON() failed [shared_snssai_infos_id]");
        goto end;
    }
    }

    smf_selection_subscription_data_1_local_var = OpenAPI_smf_selection_subscription_data_1_create (
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        subscribed_snssai_infos ? subscribed_snssai_infosList : NULL,
        shared_snssai_infos_id ? ogs_strdup(shared_snssai_infos_id->valuestring) : NULL
    );

    return smf_selection_subscription_data_1_local_var;
end:
    return NULL;
}

OpenAPI_smf_selection_subscription_data_1_t *OpenAPI_smf_selection_subscription_data_1_copy(OpenAPI_smf_selection_subscription_data_1_t *dst, OpenAPI_smf_selection_subscription_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smf_selection_subscription_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smf_selection_subscription_data_1_convertToJSON() failed");
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

    OpenAPI_smf_selection_subscription_data_1_free(dst);
    dst = OpenAPI_smf_selection_subscription_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

