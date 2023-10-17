
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ext_amf_event_subscription.h"

OpenAPI_ext_amf_event_subscription_t *OpenAPI_ext_amf_event_subscription_create(
    OpenAPI_list_t *event_list,
    char *event_notify_uri,
    char *notify_correlation_id,
    char *nf_id,
    char *subs_change_notify_uri,
    char *subs_change_notify_correlation_id,
    char *supi,
    char *group_id,
    OpenAPI_list_t *exclude_supi_list,
    OpenAPI_list_t *exclude_gpsi_list,
    OpenAPI_list_t *include_supi_list,
    OpenAPI_list_t *include_gpsi_list,
    char *gpsi,
    char *pei,
    bool is_any_ue,
    int any_ue,
    OpenAPI_amf_event_mode_t *options,
    OpenAPI_nf_type_e source_nf_type,
    OpenAPI_list_t *binding_info,
    OpenAPI_nf_type_e subscribing_nf_type,
    bool is_event_sync_ind,
    int event_sync_ind,
    OpenAPI_list_t *nf_consumer_info,
    OpenAPI_list_t* aoi_state_list
)
{
    OpenAPI_ext_amf_event_subscription_t *ext_amf_event_subscription_local_var = ogs_malloc(sizeof(OpenAPI_ext_amf_event_subscription_t));
    ogs_assert(ext_amf_event_subscription_local_var);

    ext_amf_event_subscription_local_var->event_list = event_list;
    ext_amf_event_subscription_local_var->event_notify_uri = event_notify_uri;
    ext_amf_event_subscription_local_var->notify_correlation_id = notify_correlation_id;
    ext_amf_event_subscription_local_var->nf_id = nf_id;
    ext_amf_event_subscription_local_var->subs_change_notify_uri = subs_change_notify_uri;
    ext_amf_event_subscription_local_var->subs_change_notify_correlation_id = subs_change_notify_correlation_id;
    ext_amf_event_subscription_local_var->supi = supi;
    ext_amf_event_subscription_local_var->group_id = group_id;
    ext_amf_event_subscription_local_var->exclude_supi_list = exclude_supi_list;
    ext_amf_event_subscription_local_var->exclude_gpsi_list = exclude_gpsi_list;
    ext_amf_event_subscription_local_var->include_supi_list = include_supi_list;
    ext_amf_event_subscription_local_var->include_gpsi_list = include_gpsi_list;
    ext_amf_event_subscription_local_var->gpsi = gpsi;
    ext_amf_event_subscription_local_var->pei = pei;
    ext_amf_event_subscription_local_var->is_any_ue = is_any_ue;
    ext_amf_event_subscription_local_var->any_ue = any_ue;
    ext_amf_event_subscription_local_var->options = options;
    ext_amf_event_subscription_local_var->source_nf_type = source_nf_type;
    ext_amf_event_subscription_local_var->binding_info = binding_info;
    ext_amf_event_subscription_local_var->subscribing_nf_type = subscribing_nf_type;
    ext_amf_event_subscription_local_var->is_event_sync_ind = is_event_sync_ind;
    ext_amf_event_subscription_local_var->event_sync_ind = event_sync_ind;
    ext_amf_event_subscription_local_var->nf_consumer_info = nf_consumer_info;
    ext_amf_event_subscription_local_var->aoi_state_list = aoi_state_list;

    return ext_amf_event_subscription_local_var;
}

void OpenAPI_ext_amf_event_subscription_free(OpenAPI_ext_amf_event_subscription_t *ext_amf_event_subscription)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ext_amf_event_subscription) {
        return;
    }
    if (ext_amf_event_subscription->event_list) {
        OpenAPI_list_for_each(ext_amf_event_subscription->event_list, node) {
            OpenAPI_amf_event_free(node->data);
        }
        OpenAPI_list_free(ext_amf_event_subscription->event_list);
        ext_amf_event_subscription->event_list = NULL;
    }
    if (ext_amf_event_subscription->event_notify_uri) {
        ogs_free(ext_amf_event_subscription->event_notify_uri);
        ext_amf_event_subscription->event_notify_uri = NULL;
    }
    if (ext_amf_event_subscription->notify_correlation_id) {
        ogs_free(ext_amf_event_subscription->notify_correlation_id);
        ext_amf_event_subscription->notify_correlation_id = NULL;
    }
    if (ext_amf_event_subscription->nf_id) {
        ogs_free(ext_amf_event_subscription->nf_id);
        ext_amf_event_subscription->nf_id = NULL;
    }
    if (ext_amf_event_subscription->subs_change_notify_uri) {
        ogs_free(ext_amf_event_subscription->subs_change_notify_uri);
        ext_amf_event_subscription->subs_change_notify_uri = NULL;
    }
    if (ext_amf_event_subscription->subs_change_notify_correlation_id) {
        ogs_free(ext_amf_event_subscription->subs_change_notify_correlation_id);
        ext_amf_event_subscription->subs_change_notify_correlation_id = NULL;
    }
    if (ext_amf_event_subscription->supi) {
        ogs_free(ext_amf_event_subscription->supi);
        ext_amf_event_subscription->supi = NULL;
    }
    if (ext_amf_event_subscription->group_id) {
        ogs_free(ext_amf_event_subscription->group_id);
        ext_amf_event_subscription->group_id = NULL;
    }
    if (ext_amf_event_subscription->exclude_supi_list) {
        OpenAPI_list_for_each(ext_amf_event_subscription->exclude_supi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ext_amf_event_subscription->exclude_supi_list);
        ext_amf_event_subscription->exclude_supi_list = NULL;
    }
    if (ext_amf_event_subscription->exclude_gpsi_list) {
        OpenAPI_list_for_each(ext_amf_event_subscription->exclude_gpsi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ext_amf_event_subscription->exclude_gpsi_list);
        ext_amf_event_subscription->exclude_gpsi_list = NULL;
    }
    if (ext_amf_event_subscription->include_supi_list) {
        OpenAPI_list_for_each(ext_amf_event_subscription->include_supi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ext_amf_event_subscription->include_supi_list);
        ext_amf_event_subscription->include_supi_list = NULL;
    }
    if (ext_amf_event_subscription->include_gpsi_list) {
        OpenAPI_list_for_each(ext_amf_event_subscription->include_gpsi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ext_amf_event_subscription->include_gpsi_list);
        ext_amf_event_subscription->include_gpsi_list = NULL;
    }
    if (ext_amf_event_subscription->gpsi) {
        ogs_free(ext_amf_event_subscription->gpsi);
        ext_amf_event_subscription->gpsi = NULL;
    }
    if (ext_amf_event_subscription->pei) {
        ogs_free(ext_amf_event_subscription->pei);
        ext_amf_event_subscription->pei = NULL;
    }
    if (ext_amf_event_subscription->options) {
        OpenAPI_amf_event_mode_free(ext_amf_event_subscription->options);
        ext_amf_event_subscription->options = NULL;
    }
    if (ext_amf_event_subscription->binding_info) {
        OpenAPI_list_for_each(ext_amf_event_subscription->binding_info, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ext_amf_event_subscription->binding_info);
        ext_amf_event_subscription->binding_info = NULL;
    }
    if (ext_amf_event_subscription->nf_consumer_info) {
        OpenAPI_list_for_each(ext_amf_event_subscription->nf_consumer_info, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ext_amf_event_subscription->nf_consumer_info);
        ext_amf_event_subscription->nf_consumer_info = NULL;
    }
    if (ext_amf_event_subscription->aoi_state_list) {
        OpenAPI_list_for_each(ext_amf_event_subscription->aoi_state_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_area_of_interest_event_state_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ext_amf_event_subscription->aoi_state_list);
        ext_amf_event_subscription->aoi_state_list = NULL;
    }
    ogs_free(ext_amf_event_subscription);
}

cJSON *OpenAPI_ext_amf_event_subscription_convertToJSON(OpenAPI_ext_amf_event_subscription_t *ext_amf_event_subscription)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ext_amf_event_subscription == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [ExtAmfEventSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ext_amf_event_subscription->event_list) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [event_list]");
        return NULL;
    }
    cJSON *event_listList = cJSON_AddArrayToObject(item, "eventList");
    if (event_listList == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [event_list]");
        goto end;
    }
    OpenAPI_list_for_each(ext_amf_event_subscription->event_list, node) {
        cJSON *itemLocal = OpenAPI_amf_event_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [event_list]");
            goto end;
        }
        cJSON_AddItemToArray(event_listList, itemLocal);
    }

    if (!ext_amf_event_subscription->event_notify_uri) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [event_notify_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "eventNotifyUri", ext_amf_event_subscription->event_notify_uri) == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [event_notify_uri]");
        goto end;
    }

    if (!ext_amf_event_subscription->notify_correlation_id) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [notify_correlation_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notifyCorrelationId", ext_amf_event_subscription->notify_correlation_id) == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [notify_correlation_id]");
        goto end;
    }

    if (!ext_amf_event_subscription->nf_id) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [nf_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nfId", ext_amf_event_subscription->nf_id) == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [nf_id]");
        goto end;
    }

    if (ext_amf_event_subscription->subs_change_notify_uri) {
    if (cJSON_AddStringToObject(item, "subsChangeNotifyUri", ext_amf_event_subscription->subs_change_notify_uri) == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [subs_change_notify_uri]");
        goto end;
    }
    }

    if (ext_amf_event_subscription->subs_change_notify_correlation_id) {
    if (cJSON_AddStringToObject(item, "subsChangeNotifyCorrelationId", ext_amf_event_subscription->subs_change_notify_correlation_id) == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [subs_change_notify_correlation_id]");
        goto end;
    }
    }

    if (ext_amf_event_subscription->supi) {
    if (cJSON_AddStringToObject(item, "supi", ext_amf_event_subscription->supi) == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (ext_amf_event_subscription->group_id) {
    if (cJSON_AddStringToObject(item, "groupId", ext_amf_event_subscription->group_id) == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [group_id]");
        goto end;
    }
    }

    if (ext_amf_event_subscription->exclude_supi_list) {
    cJSON *exclude_supi_listList = cJSON_AddArrayToObject(item, "excludeSupiList");
    if (exclude_supi_listList == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [exclude_supi_list]");
        goto end;
    }
    OpenAPI_list_for_each(ext_amf_event_subscription->exclude_supi_list, node) {
        if (cJSON_AddStringToObject(exclude_supi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [exclude_supi_list]");
            goto end;
        }
    }
    }

    if (ext_amf_event_subscription->exclude_gpsi_list) {
    cJSON *exclude_gpsi_listList = cJSON_AddArrayToObject(item, "excludeGpsiList");
    if (exclude_gpsi_listList == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [exclude_gpsi_list]");
        goto end;
    }
    OpenAPI_list_for_each(ext_amf_event_subscription->exclude_gpsi_list, node) {
        if (cJSON_AddStringToObject(exclude_gpsi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [exclude_gpsi_list]");
            goto end;
        }
    }
    }

    if (ext_amf_event_subscription->include_supi_list) {
    cJSON *include_supi_listList = cJSON_AddArrayToObject(item, "includeSupiList");
    if (include_supi_listList == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [include_supi_list]");
        goto end;
    }
    OpenAPI_list_for_each(ext_amf_event_subscription->include_supi_list, node) {
        if (cJSON_AddStringToObject(include_supi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [include_supi_list]");
            goto end;
        }
    }
    }

    if (ext_amf_event_subscription->include_gpsi_list) {
    cJSON *include_gpsi_listList = cJSON_AddArrayToObject(item, "includeGpsiList");
    if (include_gpsi_listList == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [include_gpsi_list]");
        goto end;
    }
    OpenAPI_list_for_each(ext_amf_event_subscription->include_gpsi_list, node) {
        if (cJSON_AddStringToObject(include_gpsi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [include_gpsi_list]");
            goto end;
        }
    }
    }

    if (ext_amf_event_subscription->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", ext_amf_event_subscription->gpsi) == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (ext_amf_event_subscription->pei) {
    if (cJSON_AddStringToObject(item, "pei", ext_amf_event_subscription->pei) == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (ext_amf_event_subscription->is_any_ue) {
    if (cJSON_AddBoolToObject(item, "anyUE", ext_amf_event_subscription->any_ue) == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [any_ue]");
        goto end;
    }
    }

    if (ext_amf_event_subscription->options) {
    cJSON *options_local_JSON = OpenAPI_amf_event_mode_convertToJSON(ext_amf_event_subscription->options);
    if (options_local_JSON == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [options]");
        goto end;
    }
    cJSON_AddItemToObject(item, "options", options_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [options]");
        goto end;
    }
    }

    if (ext_amf_event_subscription->source_nf_type != OpenAPI_nf_type_NULL) {
    if (cJSON_AddStringToObject(item, "sourceNfType", OpenAPI_nf_type_ToString(ext_amf_event_subscription->source_nf_type)) == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [source_nf_type]");
        goto end;
    }
    }

    if (ext_amf_event_subscription->binding_info) {
    cJSON *binding_infoList = cJSON_AddArrayToObject(item, "bindingInfo");
    if (binding_infoList == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [binding_info]");
        goto end;
    }
    OpenAPI_list_for_each(ext_amf_event_subscription->binding_info, node) {
        if (cJSON_AddStringToObject(binding_infoList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [binding_info]");
            goto end;
        }
    }
    }

    if (ext_amf_event_subscription->subscribing_nf_type != OpenAPI_nf_type_NULL) {
    if (cJSON_AddStringToObject(item, "subscribingNfType", OpenAPI_nf_type_ToString(ext_amf_event_subscription->subscribing_nf_type)) == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [subscribing_nf_type]");
        goto end;
    }
    }

    if (ext_amf_event_subscription->is_event_sync_ind) {
    if (cJSON_AddBoolToObject(item, "eventSyncInd", ext_amf_event_subscription->event_sync_ind) == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [event_sync_ind]");
        goto end;
    }
    }

    if (ext_amf_event_subscription->nf_consumer_info) {
    cJSON *nf_consumer_infoList = cJSON_AddArrayToObject(item, "nfConsumerInfo");
    if (nf_consumer_infoList == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [nf_consumer_info]");
        goto end;
    }
    OpenAPI_list_for_each(ext_amf_event_subscription->nf_consumer_info, node) {
        if (cJSON_AddStringToObject(nf_consumer_infoList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [nf_consumer_info]");
            goto end;
        }
    }
    }

    if (ext_amf_event_subscription->aoi_state_list) {
    cJSON *aoi_state_list = cJSON_AddObjectToObject(item, "aoiStateList");
    if (aoi_state_list == NULL) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [aoi_state_list]");
        goto end;
    }
    cJSON *localMapObject = aoi_state_list;
    if (ext_amf_event_subscription->aoi_state_list) {
        OpenAPI_list_for_each(ext_amf_event_subscription->aoi_state_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [aoi_state_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [aoi_state_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_area_of_interest_event_state_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_ext_amf_event_subscription_t *OpenAPI_ext_amf_event_subscription_parseFromJSON(cJSON *ext_amf_event_subscriptionJSON)
{
    OpenAPI_ext_amf_event_subscription_t *ext_amf_event_subscription_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event_list = NULL;
    OpenAPI_list_t *event_listList = NULL;
    cJSON *event_notify_uri = NULL;
    cJSON *notify_correlation_id = NULL;
    cJSON *nf_id = NULL;
    cJSON *subs_change_notify_uri = NULL;
    cJSON *subs_change_notify_correlation_id = NULL;
    cJSON *supi = NULL;
    cJSON *group_id = NULL;
    cJSON *exclude_supi_list = NULL;
    OpenAPI_list_t *exclude_supi_listList = NULL;
    cJSON *exclude_gpsi_list = NULL;
    OpenAPI_list_t *exclude_gpsi_listList = NULL;
    cJSON *include_supi_list = NULL;
    OpenAPI_list_t *include_supi_listList = NULL;
    cJSON *include_gpsi_list = NULL;
    OpenAPI_list_t *include_gpsi_listList = NULL;
    cJSON *gpsi = NULL;
    cJSON *pei = NULL;
    cJSON *any_ue = NULL;
    cJSON *options = NULL;
    OpenAPI_amf_event_mode_t *options_local_nonprim = NULL;
    cJSON *source_nf_type = NULL;
    OpenAPI_nf_type_e source_nf_typeVariable = 0;
    cJSON *binding_info = NULL;
    OpenAPI_list_t *binding_infoList = NULL;
    cJSON *subscribing_nf_type = NULL;
    OpenAPI_nf_type_e subscribing_nf_typeVariable = 0;
    cJSON *event_sync_ind = NULL;
    cJSON *nf_consumer_info = NULL;
    OpenAPI_list_t *nf_consumer_infoList = NULL;
    cJSON *aoi_state_list = NULL;
    OpenAPI_list_t *aoi_state_listList = NULL;
    event_list = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "eventList");
    if (!event_list) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [event_list]");
        goto end;
    }
        cJSON *event_list_local = NULL;
        if (!cJSON_IsArray(event_list)) {
            ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [event_list]");
            goto end;
        }

        event_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_list_local, event_list) {
            if (!cJSON_IsObject(event_list_local)) {
                ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [event_list]");
                goto end;
            }
            OpenAPI_amf_event_t *event_listItem = OpenAPI_amf_event_parseFromJSON(event_list_local);
            if (!event_listItem) {
                ogs_error("No event_listItem");
                goto end;
            }
            OpenAPI_list_add(event_listList, event_listItem);
        }

    event_notify_uri = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "eventNotifyUri");
    if (!event_notify_uri) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [event_notify_uri]");
        goto end;
    }
    if (!cJSON_IsString(event_notify_uri)) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [event_notify_uri]");
        goto end;
    }

    notify_correlation_id = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "notifyCorrelationId");
    if (!notify_correlation_id) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [notify_correlation_id]");
        goto end;
    }
    if (!cJSON_IsString(notify_correlation_id)) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [notify_correlation_id]");
        goto end;
    }

    nf_id = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "nfId");
    if (!nf_id) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [nf_id]");
        goto end;
    }
    if (!cJSON_IsString(nf_id)) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [nf_id]");
        goto end;
    }

    subs_change_notify_uri = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "subsChangeNotifyUri");
    if (subs_change_notify_uri) {
    if (!cJSON_IsString(subs_change_notify_uri) && !cJSON_IsNull(subs_change_notify_uri)) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [subs_change_notify_uri]");
        goto end;
    }
    }

    subs_change_notify_correlation_id = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "subsChangeNotifyCorrelationId");
    if (subs_change_notify_correlation_id) {
    if (!cJSON_IsString(subs_change_notify_correlation_id) && !cJSON_IsNull(subs_change_notify_correlation_id)) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [subs_change_notify_correlation_id]");
        goto end;
    }
    }

    supi = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    group_id = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "groupId");
    if (group_id) {
    if (!cJSON_IsString(group_id) && !cJSON_IsNull(group_id)) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [group_id]");
        goto end;
    }
    }

    exclude_supi_list = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "excludeSupiList");
    if (exclude_supi_list) {
        cJSON *exclude_supi_list_local = NULL;
        if (!cJSON_IsArray(exclude_supi_list)) {
            ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [exclude_supi_list]");
            goto end;
        }

        exclude_supi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(exclude_supi_list_local, exclude_supi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(exclude_supi_list_local)) {
                ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [exclude_supi_list]");
                goto end;
            }
            OpenAPI_list_add(exclude_supi_listList, ogs_strdup(exclude_supi_list_local->valuestring));
        }
    }

    exclude_gpsi_list = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "excludeGpsiList");
    if (exclude_gpsi_list) {
        cJSON *exclude_gpsi_list_local = NULL;
        if (!cJSON_IsArray(exclude_gpsi_list)) {
            ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [exclude_gpsi_list]");
            goto end;
        }

        exclude_gpsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(exclude_gpsi_list_local, exclude_gpsi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(exclude_gpsi_list_local)) {
                ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [exclude_gpsi_list]");
                goto end;
            }
            OpenAPI_list_add(exclude_gpsi_listList, ogs_strdup(exclude_gpsi_list_local->valuestring));
        }
    }

    include_supi_list = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "includeSupiList");
    if (include_supi_list) {
        cJSON *include_supi_list_local = NULL;
        if (!cJSON_IsArray(include_supi_list)) {
            ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [include_supi_list]");
            goto end;
        }

        include_supi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(include_supi_list_local, include_supi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(include_supi_list_local)) {
                ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [include_supi_list]");
                goto end;
            }
            OpenAPI_list_add(include_supi_listList, ogs_strdup(include_supi_list_local->valuestring));
        }
    }

    include_gpsi_list = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "includeGpsiList");
    if (include_gpsi_list) {
        cJSON *include_gpsi_list_local = NULL;
        if (!cJSON_IsArray(include_gpsi_list)) {
            ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [include_gpsi_list]");
            goto end;
        }

        include_gpsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(include_gpsi_list_local, include_gpsi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(include_gpsi_list_local)) {
                ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [include_gpsi_list]");
                goto end;
            }
            OpenAPI_list_add(include_gpsi_listList, ogs_strdup(include_gpsi_list_local->valuestring));
        }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    pei = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    any_ue = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "anyUE");
    if (any_ue) {
    if (!cJSON_IsBool(any_ue)) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [any_ue]");
        goto end;
    }
    }

    options = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "options");
    if (options) {
    options_local_nonprim = OpenAPI_amf_event_mode_parseFromJSON(options);
    if (!options_local_nonprim) {
        ogs_error("OpenAPI_amf_event_mode_parseFromJSON failed [options]");
        goto end;
    }
    }

    source_nf_type = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "sourceNfType");
    if (source_nf_type) {
    if (!cJSON_IsString(source_nf_type)) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [source_nf_type]");
        goto end;
    }
    source_nf_typeVariable = OpenAPI_nf_type_FromString(source_nf_type->valuestring);
    }

    binding_info = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "bindingInfo");
    if (binding_info) {
        cJSON *binding_info_local = NULL;
        if (!cJSON_IsArray(binding_info)) {
            ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [binding_info]");
            goto end;
        }

        binding_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(binding_info_local, binding_info) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(binding_info_local)) {
                ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [binding_info]");
                goto end;
            }
            OpenAPI_list_add(binding_infoList, ogs_strdup(binding_info_local->valuestring));
        }
    }

    subscribing_nf_type = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "subscribingNfType");
    if (subscribing_nf_type) {
    if (!cJSON_IsString(subscribing_nf_type)) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [subscribing_nf_type]");
        goto end;
    }
    subscribing_nf_typeVariable = OpenAPI_nf_type_FromString(subscribing_nf_type->valuestring);
    }

    event_sync_ind = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "eventSyncInd");
    if (event_sync_ind) {
    if (!cJSON_IsBool(event_sync_ind)) {
        ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [event_sync_ind]");
        goto end;
    }
    }

    nf_consumer_info = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "nfConsumerInfo");
    if (nf_consumer_info) {
        cJSON *nf_consumer_info_local = NULL;
        if (!cJSON_IsArray(nf_consumer_info)) {
            ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [nf_consumer_info]");
            goto end;
        }

        nf_consumer_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_consumer_info_local, nf_consumer_info) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_consumer_info_local)) {
                ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [nf_consumer_info]");
                goto end;
            }
            OpenAPI_list_add(nf_consumer_infoList, ogs_strdup(nf_consumer_info_local->valuestring));
        }
    }

    aoi_state_list = cJSON_GetObjectItemCaseSensitive(ext_amf_event_subscriptionJSON, "aoiStateList");
    if (aoi_state_list) {
        cJSON *aoi_state_list_local_map = NULL;
        if (!cJSON_IsObject(aoi_state_list) && !cJSON_IsNull(aoi_state_list)) {
            ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [aoi_state_list]");
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
                    ogs_error("OpenAPI_ext_amf_event_subscription_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(aoi_state_listList, localMapKeyPair);
            }
        }
    }

    ext_amf_event_subscription_local_var = OpenAPI_ext_amf_event_subscription_create (
        event_listList,
        ogs_strdup(event_notify_uri->valuestring),
        ogs_strdup(notify_correlation_id->valuestring),
        ogs_strdup(nf_id->valuestring),
        subs_change_notify_uri && !cJSON_IsNull(subs_change_notify_uri) ? ogs_strdup(subs_change_notify_uri->valuestring) : NULL,
        subs_change_notify_correlation_id && !cJSON_IsNull(subs_change_notify_correlation_id) ? ogs_strdup(subs_change_notify_correlation_id->valuestring) : NULL,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        group_id && !cJSON_IsNull(group_id) ? ogs_strdup(group_id->valuestring) : NULL,
        exclude_supi_list ? exclude_supi_listList : NULL,
        exclude_gpsi_list ? exclude_gpsi_listList : NULL,
        include_supi_list ? include_supi_listList : NULL,
        include_gpsi_list ? include_gpsi_listList : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        any_ue ? true : false,
        any_ue ? any_ue->valueint : 0,
        options ? options_local_nonprim : NULL,
        source_nf_type ? source_nf_typeVariable : 0,
        binding_info ? binding_infoList : NULL,
        subscribing_nf_type ? subscribing_nf_typeVariable : 0,
        event_sync_ind ? true : false,
        event_sync_ind ? event_sync_ind->valueint : 0,
        nf_consumer_info ? nf_consumer_infoList : NULL,
        aoi_state_list ? aoi_state_listList : NULL
    );

    return ext_amf_event_subscription_local_var;
end:
    if (event_listList) {
        OpenAPI_list_for_each(event_listList, node) {
            OpenAPI_amf_event_free(node->data);
        }
        OpenAPI_list_free(event_listList);
        event_listList = NULL;
    }
    if (exclude_supi_listList) {
        OpenAPI_list_for_each(exclude_supi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(exclude_supi_listList);
        exclude_supi_listList = NULL;
    }
    if (exclude_gpsi_listList) {
        OpenAPI_list_for_each(exclude_gpsi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(exclude_gpsi_listList);
        exclude_gpsi_listList = NULL;
    }
    if (include_supi_listList) {
        OpenAPI_list_for_each(include_supi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(include_supi_listList);
        include_supi_listList = NULL;
    }
    if (include_gpsi_listList) {
        OpenAPI_list_for_each(include_gpsi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(include_gpsi_listList);
        include_gpsi_listList = NULL;
    }
    if (options_local_nonprim) {
        OpenAPI_amf_event_mode_free(options_local_nonprim);
        options_local_nonprim = NULL;
    }
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

OpenAPI_ext_amf_event_subscription_t *OpenAPI_ext_amf_event_subscription_copy(OpenAPI_ext_amf_event_subscription_t *dst, OpenAPI_ext_amf_event_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ext_amf_event_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ext_amf_event_subscription_convertToJSON() failed");
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

    OpenAPI_ext_amf_event_subscription_free(dst);
    dst = OpenAPI_ext_amf_event_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

