
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_event_subscription_add_info.h"

OpenAPI_amf_event_subscription_add_info_t *OpenAPI_amf_event_subscription_add_info_create(
    OpenAPI_list_t *binding_info,
    OpenAPI_nf_type_e subscribing_nf_type,
    bool is_event_sync_ind,
    int event_sync_ind,
    OpenAPI_list_t *nf_consumer_info,
    OpenAPI_list_t* aoi_state_list
)
{
    OpenAPI_amf_event_subscription_add_info_t *amf_event_subscription_add_info_local_var = ogs_malloc(sizeof(OpenAPI_amf_event_subscription_add_info_t));
    ogs_assert(amf_event_subscription_add_info_local_var);

    amf_event_subscription_add_info_local_var->binding_info = binding_info;
    amf_event_subscription_add_info_local_var->subscribing_nf_type = subscribing_nf_type;
    amf_event_subscription_add_info_local_var->is_event_sync_ind = is_event_sync_ind;
    amf_event_subscription_add_info_local_var->event_sync_ind = event_sync_ind;
    amf_event_subscription_add_info_local_var->nf_consumer_info = nf_consumer_info;
    amf_event_subscription_add_info_local_var->aoi_state_list = aoi_state_list;

    return amf_event_subscription_add_info_local_var;
}

void OpenAPI_amf_event_subscription_add_info_free(OpenAPI_amf_event_subscription_add_info_t *amf_event_subscription_add_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == amf_event_subscription_add_info) {
        return;
    }
    if (amf_event_subscription_add_info->binding_info) {
        OpenAPI_list_for_each(amf_event_subscription_add_info->binding_info, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(amf_event_subscription_add_info->binding_info);
        amf_event_subscription_add_info->binding_info = NULL;
    }
    if (amf_event_subscription_add_info->nf_consumer_info) {
        OpenAPI_list_for_each(amf_event_subscription_add_info->nf_consumer_info, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(amf_event_subscription_add_info->nf_consumer_info);
        amf_event_subscription_add_info->nf_consumer_info = NULL;
    }
    if (amf_event_subscription_add_info->aoi_state_list) {
        OpenAPI_list_for_each(amf_event_subscription_add_info->aoi_state_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_area_of_interest_event_state_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(amf_event_subscription_add_info->aoi_state_list);
        amf_event_subscription_add_info->aoi_state_list = NULL;
    }
    ogs_free(amf_event_subscription_add_info);
}

cJSON *OpenAPI_amf_event_subscription_add_info_convertToJSON(OpenAPI_amf_event_subscription_add_info_t *amf_event_subscription_add_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (amf_event_subscription_add_info == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [AmfEventSubscriptionAddInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (amf_event_subscription_add_info->binding_info) {
    cJSON *binding_infoList = cJSON_AddArrayToObject(item, "bindingInfo");
    if (binding_infoList == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [binding_info]");
        goto end;
    }
    OpenAPI_list_for_each(amf_event_subscription_add_info->binding_info, node) {
        if (cJSON_AddStringToObject(binding_infoList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [binding_info]");
            goto end;
        }
    }
    }

    if (amf_event_subscription_add_info->subscribing_nf_type != OpenAPI_nf_type_NULL) {
    if (cJSON_AddStringToObject(item, "subscribingNfType", OpenAPI_nf_type_ToString(amf_event_subscription_add_info->subscribing_nf_type)) == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [subscribing_nf_type]");
        goto end;
    }
    }

    if (amf_event_subscription_add_info->is_event_sync_ind) {
    if (cJSON_AddBoolToObject(item, "eventSyncInd", amf_event_subscription_add_info->event_sync_ind) == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [event_sync_ind]");
        goto end;
    }
    }

    if (amf_event_subscription_add_info->nf_consumer_info) {
    cJSON *nf_consumer_infoList = cJSON_AddArrayToObject(item, "nfConsumerInfo");
    if (nf_consumer_infoList == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [nf_consumer_info]");
        goto end;
    }
    OpenAPI_list_for_each(amf_event_subscription_add_info->nf_consumer_info, node) {
        if (cJSON_AddStringToObject(nf_consumer_infoList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [nf_consumer_info]");
            goto end;
        }
    }
    }

    if (amf_event_subscription_add_info->aoi_state_list) {
    cJSON *aoi_state_list = cJSON_AddObjectToObject(item, "aoiStateList");
    if (aoi_state_list == NULL) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [aoi_state_list]");
        goto end;
    }
    cJSON *localMapObject = aoi_state_list;
    if (amf_event_subscription_add_info->aoi_state_list) {
        OpenAPI_list_for_each(amf_event_subscription_add_info->aoi_state_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [aoi_state_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [aoi_state_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_area_of_interest_event_state_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_amf_event_subscription_add_info_t *OpenAPI_amf_event_subscription_add_info_parseFromJSON(cJSON *amf_event_subscription_add_infoJSON)
{
    OpenAPI_amf_event_subscription_add_info_t *amf_event_subscription_add_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *binding_info = NULL;
    OpenAPI_list_t *binding_infoList = NULL;
    cJSON *subscribing_nf_type = NULL;
    OpenAPI_nf_type_e subscribing_nf_typeVariable = 0;
    cJSON *event_sync_ind = NULL;
    cJSON *nf_consumer_info = NULL;
    OpenAPI_list_t *nf_consumer_infoList = NULL;
    cJSON *aoi_state_list = NULL;
    OpenAPI_list_t *aoi_state_listList = NULL;
    binding_info = cJSON_GetObjectItemCaseSensitive(amf_event_subscription_add_infoJSON, "bindingInfo");
    if (binding_info) {
        cJSON *binding_info_local = NULL;
        if (!cJSON_IsArray(binding_info)) {
            ogs_error("OpenAPI_amf_event_subscription_add_info_parseFromJSON() failed [binding_info]");
            goto end;
        }

        binding_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(binding_info_local, binding_info) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(binding_info_local)) {
                ogs_error("OpenAPI_amf_event_subscription_add_info_parseFromJSON() failed [binding_info]");
                goto end;
            }
            OpenAPI_list_add(binding_infoList, ogs_strdup(binding_info_local->valuestring));
        }
    }

    subscribing_nf_type = cJSON_GetObjectItemCaseSensitive(amf_event_subscription_add_infoJSON, "subscribingNfType");
    if (subscribing_nf_type) {
    if (!cJSON_IsString(subscribing_nf_type)) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_parseFromJSON() failed [subscribing_nf_type]");
        goto end;
    }
    subscribing_nf_typeVariable = OpenAPI_nf_type_FromString(subscribing_nf_type->valuestring);
    }

    event_sync_ind = cJSON_GetObjectItemCaseSensitive(amf_event_subscription_add_infoJSON, "eventSyncInd");
    if (event_sync_ind) {
    if (!cJSON_IsBool(event_sync_ind)) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_parseFromJSON() failed [event_sync_ind]");
        goto end;
    }
    }

    nf_consumer_info = cJSON_GetObjectItemCaseSensitive(amf_event_subscription_add_infoJSON, "nfConsumerInfo");
    if (nf_consumer_info) {
        cJSON *nf_consumer_info_local = NULL;
        if (!cJSON_IsArray(nf_consumer_info)) {
            ogs_error("OpenAPI_amf_event_subscription_add_info_parseFromJSON() failed [nf_consumer_info]");
            goto end;
        }

        nf_consumer_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_consumer_info_local, nf_consumer_info) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_consumer_info_local)) {
                ogs_error("OpenAPI_amf_event_subscription_add_info_parseFromJSON() failed [nf_consumer_info]");
                goto end;
            }
            OpenAPI_list_add(nf_consumer_infoList, ogs_strdup(nf_consumer_info_local->valuestring));
        }
    }

    aoi_state_list = cJSON_GetObjectItemCaseSensitive(amf_event_subscription_add_infoJSON, "aoiStateList");
    if (aoi_state_list) {
        cJSON *aoi_state_list_local_map = NULL;
        if (!cJSON_IsObject(aoi_state_list) && !cJSON_IsNull(aoi_state_list)) {
            ogs_error("OpenAPI_amf_event_subscription_add_info_parseFromJSON() failed [aoi_state_list]");
            goto end;
        }
        if (cJSON_IsObject(aoi_state_list)) {
            aoi_state_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(aoi_state_list_local_map, aoi_state_list) {
                cJSON *localMapObject = aoi_state_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_area_of_interest_event_state_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_amf_event_subscription_add_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(aoi_state_listList, localMapKeyPair);
            }
        }
    }

    amf_event_subscription_add_info_local_var = OpenAPI_amf_event_subscription_add_info_create (
        binding_info ? binding_infoList : NULL,
        subscribing_nf_type ? subscribing_nf_typeVariable : 0,
        event_sync_ind ? true : false,
        event_sync_ind ? event_sync_ind->valueint : 0,
        nf_consumer_info ? nf_consumer_infoList : NULL,
        aoi_state_list ? aoi_state_listList : NULL
    );

    return amf_event_subscription_add_info_local_var;
end:
    if (binding_infoList) {
        OpenAPI_list_for_each(binding_infoList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(binding_infoList);
        binding_infoList = NULL;
    }
    if (nf_consumer_infoList) {
        OpenAPI_list_for_each(nf_consumer_infoList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_consumer_infoList);
        nf_consumer_infoList = NULL;
    }
    if (aoi_state_listList) {
        OpenAPI_list_for_each(aoi_state_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_area_of_interest_event_state_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(aoi_state_listList);
        aoi_state_listList = NULL;
    }
    return NULL;
}

OpenAPI_amf_event_subscription_add_info_t *OpenAPI_amf_event_subscription_add_info_copy(OpenAPI_amf_event_subscription_add_info_t *dst, OpenAPI_amf_event_subscription_add_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_event_subscription_add_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_event_subscription_add_info_convertToJSON() failed");
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

    OpenAPI_amf_event_subscription_add_info_free(dst);
    dst = OpenAPI_amf_event_subscription_add_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

