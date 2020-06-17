
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_change_notify.h"

OpenAPI_data_change_notify_t *OpenAPI_data_change_notify_create(
    OpenAPI_list_t *original_callback_reference,
    char *ue_id,
    OpenAPI_list_t *notify_items,
    OpenAPI_sdm_subscription_t *sdm_subscription,
    OpenAPI_list_t *additional_sdm_subscriptions,
    OpenAPI_list_t *subscription_data_subscriptions
    )
{
    OpenAPI_data_change_notify_t *data_change_notify_local_var = OpenAPI_malloc(sizeof(OpenAPI_data_change_notify_t));
    if (!data_change_notify_local_var) {
        return NULL;
    }
    data_change_notify_local_var->original_callback_reference = original_callback_reference;
    data_change_notify_local_var->ue_id = ue_id;
    data_change_notify_local_var->notify_items = notify_items;
    data_change_notify_local_var->sdm_subscription = sdm_subscription;
    data_change_notify_local_var->additional_sdm_subscriptions = additional_sdm_subscriptions;
    data_change_notify_local_var->subscription_data_subscriptions = subscription_data_subscriptions;

    return data_change_notify_local_var;
}

void OpenAPI_data_change_notify_free(OpenAPI_data_change_notify_t *data_change_notify)
{
    if (NULL == data_change_notify) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(data_change_notify->original_callback_reference, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(data_change_notify->original_callback_reference);
    ogs_free(data_change_notify->ue_id);
    OpenAPI_list_for_each(data_change_notify->notify_items, node) {
        OpenAPI_notify_item_free(node->data);
    }
    OpenAPI_list_free(data_change_notify->notify_items);
    OpenAPI_sdm_subscription_free(data_change_notify->sdm_subscription);
    OpenAPI_list_for_each(data_change_notify->additional_sdm_subscriptions, node) {
        OpenAPI_sdm_subscription_free(node->data);
    }
    OpenAPI_list_free(data_change_notify->additional_sdm_subscriptions);
    OpenAPI_list_for_each(data_change_notify->subscription_data_subscriptions, node) {
        OpenAPI_subscription_data_subscriptions_free(node->data);
    }
    OpenAPI_list_free(data_change_notify->subscription_data_subscriptions);
    ogs_free(data_change_notify);
}

cJSON *OpenAPI_data_change_notify_convertToJSON(OpenAPI_data_change_notify_t *data_change_notify)
{
    cJSON *item = NULL;

    if (data_change_notify == NULL) {
        ogs_error("OpenAPI_data_change_notify_convertToJSON() failed [DataChangeNotify]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (data_change_notify->original_callback_reference) {
        cJSON *original_callback_reference = cJSON_AddArrayToObject(item, "originalCallbackReference");
        if (original_callback_reference == NULL) {
            ogs_error("OpenAPI_data_change_notify_convertToJSON() failed [original_callback_reference]");
            goto end;
        }

        OpenAPI_lnode_t *original_callback_reference_node;
        OpenAPI_list_for_each(data_change_notify->original_callback_reference, original_callback_reference_node)  {
            if (cJSON_AddStringToObject(original_callback_reference, "", (char*)original_callback_reference_node->data) == NULL) {
                ogs_error("OpenAPI_data_change_notify_convertToJSON() failed [original_callback_reference]");
                goto end;
            }
        }
    }

    if (data_change_notify->ue_id) {
        if (cJSON_AddStringToObject(item, "ueId", data_change_notify->ue_id) == NULL) {
            ogs_error("OpenAPI_data_change_notify_convertToJSON() failed [ue_id]");
            goto end;
        }
    }

    if (data_change_notify->notify_items) {
        cJSON *notify_itemsList = cJSON_AddArrayToObject(item, "notifyItems");
        if (notify_itemsList == NULL) {
            ogs_error("OpenAPI_data_change_notify_convertToJSON() failed [notify_items]");
            goto end;
        }

        OpenAPI_lnode_t *notify_items_node;
        if (data_change_notify->notify_items) {
            OpenAPI_list_for_each(data_change_notify->notify_items, notify_items_node) {
                cJSON *itemLocal = OpenAPI_notify_item_convertToJSON(notify_items_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_data_change_notify_convertToJSON() failed [notify_items]");
                    goto end;
                }
                cJSON_AddItemToArray(notify_itemsList, itemLocal);
            }
        }
    }

    if (data_change_notify->sdm_subscription) {
        cJSON *sdm_subscription_local_JSON = OpenAPI_sdm_subscription_convertToJSON(data_change_notify->sdm_subscription);
        if (sdm_subscription_local_JSON == NULL) {
            ogs_error("OpenAPI_data_change_notify_convertToJSON() failed [sdm_subscription]");
            goto end;
        }
        cJSON_AddItemToObject(item, "sdmSubscription", sdm_subscription_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_data_change_notify_convertToJSON() failed [sdm_subscription]");
            goto end;
        }
    }

    if (data_change_notify->additional_sdm_subscriptions) {
        cJSON *additional_sdm_subscriptionsList = cJSON_AddArrayToObject(item, "additionalSdmSubscriptions");
        if (additional_sdm_subscriptionsList == NULL) {
            ogs_error("OpenAPI_data_change_notify_convertToJSON() failed [additional_sdm_subscriptions]");
            goto end;
        }

        OpenAPI_lnode_t *additional_sdm_subscriptions_node;
        if (data_change_notify->additional_sdm_subscriptions) {
            OpenAPI_list_for_each(data_change_notify->additional_sdm_subscriptions, additional_sdm_subscriptions_node) {
                cJSON *itemLocal = OpenAPI_sdm_subscription_convertToJSON(additional_sdm_subscriptions_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_data_change_notify_convertToJSON() failed [additional_sdm_subscriptions]");
                    goto end;
                }
                cJSON_AddItemToArray(additional_sdm_subscriptionsList, itemLocal);
            }
        }
    }

    if (data_change_notify->subscription_data_subscriptions) {
        cJSON *subscription_data_subscriptionsList = cJSON_AddArrayToObject(item, "subscriptionDataSubscriptions");
        if (subscription_data_subscriptionsList == NULL) {
            ogs_error("OpenAPI_data_change_notify_convertToJSON() failed [subscription_data_subscriptions]");
            goto end;
        }

        OpenAPI_lnode_t *subscription_data_subscriptions_node;
        if (data_change_notify->subscription_data_subscriptions) {
            OpenAPI_list_for_each(data_change_notify->subscription_data_subscriptions, subscription_data_subscriptions_node) {
                cJSON *itemLocal = OpenAPI_subscription_data_subscriptions_convertToJSON(subscription_data_subscriptions_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_data_change_notify_convertToJSON() failed [subscription_data_subscriptions]");
                    goto end;
                }
                cJSON_AddItemToArray(subscription_data_subscriptionsList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_data_change_notify_t *OpenAPI_data_change_notify_parseFromJSON(cJSON *data_change_notifyJSON)
{
    OpenAPI_data_change_notify_t *data_change_notify_local_var = NULL;
    cJSON *original_callback_reference = cJSON_GetObjectItemCaseSensitive(data_change_notifyJSON, "originalCallbackReference");

    OpenAPI_list_t *original_callback_referenceList;
    if (original_callback_reference) {
        cJSON *original_callback_reference_local;
        if (!cJSON_IsArray(original_callback_reference)) {
            ogs_error("OpenAPI_data_change_notify_parseFromJSON() failed [original_callback_reference]");
            goto end;
        }
        original_callback_referenceList = OpenAPI_list_create();

        cJSON_ArrayForEach(original_callback_reference_local, original_callback_reference) {
            if (!cJSON_IsString(original_callback_reference_local)) {
                ogs_error("OpenAPI_data_change_notify_parseFromJSON() failed [original_callback_reference]");
                goto end;
            }
            OpenAPI_list_add(original_callback_referenceList, ogs_strdup(original_callback_reference_local->valuestring));
        }
    }

    cJSON *ue_id = cJSON_GetObjectItemCaseSensitive(data_change_notifyJSON, "ueId");

    if (ue_id) {
        if (!cJSON_IsString(ue_id)) {
            ogs_error("OpenAPI_data_change_notify_parseFromJSON() failed [ue_id]");
            goto end;
        }
    }

    cJSON *notify_items = cJSON_GetObjectItemCaseSensitive(data_change_notifyJSON, "notifyItems");

    OpenAPI_list_t *notify_itemsList;
    if (notify_items) {
        cJSON *notify_items_local_nonprimitive;
        if (!cJSON_IsArray(notify_items)) {
            ogs_error("OpenAPI_data_change_notify_parseFromJSON() failed [notify_items]");
            goto end;
        }

        notify_itemsList = OpenAPI_list_create();

        cJSON_ArrayForEach(notify_items_local_nonprimitive, notify_items ) {
            if (!cJSON_IsObject(notify_items_local_nonprimitive)) {
                ogs_error("OpenAPI_data_change_notify_parseFromJSON() failed [notify_items]");
                goto end;
            }
            OpenAPI_notify_item_t *notify_itemsItem = OpenAPI_notify_item_parseFromJSON(notify_items_local_nonprimitive);

            OpenAPI_list_add(notify_itemsList, notify_itemsItem);
        }
    }

    cJSON *sdm_subscription = cJSON_GetObjectItemCaseSensitive(data_change_notifyJSON, "sdmSubscription");

    OpenAPI_sdm_subscription_t *sdm_subscription_local_nonprim = NULL;
    if (sdm_subscription) {
        sdm_subscription_local_nonprim = OpenAPI_sdm_subscription_parseFromJSON(sdm_subscription);
    }

    cJSON *additional_sdm_subscriptions = cJSON_GetObjectItemCaseSensitive(data_change_notifyJSON, "additionalSdmSubscriptions");

    OpenAPI_list_t *additional_sdm_subscriptionsList;
    if (additional_sdm_subscriptions) {
        cJSON *additional_sdm_subscriptions_local_nonprimitive;
        if (!cJSON_IsArray(additional_sdm_subscriptions)) {
            ogs_error("OpenAPI_data_change_notify_parseFromJSON() failed [additional_sdm_subscriptions]");
            goto end;
        }

        additional_sdm_subscriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_sdm_subscriptions_local_nonprimitive, additional_sdm_subscriptions ) {
            if (!cJSON_IsObject(additional_sdm_subscriptions_local_nonprimitive)) {
                ogs_error("OpenAPI_data_change_notify_parseFromJSON() failed [additional_sdm_subscriptions]");
                goto end;
            }
            OpenAPI_sdm_subscription_t *additional_sdm_subscriptionsItem = OpenAPI_sdm_subscription_parseFromJSON(additional_sdm_subscriptions_local_nonprimitive);

            OpenAPI_list_add(additional_sdm_subscriptionsList, additional_sdm_subscriptionsItem);
        }
    }

    cJSON *subscription_data_subscriptions = cJSON_GetObjectItemCaseSensitive(data_change_notifyJSON, "subscriptionDataSubscriptions");

    OpenAPI_list_t *subscription_data_subscriptionsList;
    if (subscription_data_subscriptions) {
        cJSON *subscription_data_subscriptions_local_nonprimitive;
        if (!cJSON_IsArray(subscription_data_subscriptions)) {
            ogs_error("OpenAPI_data_change_notify_parseFromJSON() failed [subscription_data_subscriptions]");
            goto end;
        }

        subscription_data_subscriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(subscription_data_subscriptions_local_nonprimitive, subscription_data_subscriptions ) {
            if (!cJSON_IsObject(subscription_data_subscriptions_local_nonprimitive)) {
                ogs_error("OpenAPI_data_change_notify_parseFromJSON() failed [subscription_data_subscriptions]");
                goto end;
            }
            OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptionsItem = OpenAPI_subscription_data_subscriptions_parseFromJSON(subscription_data_subscriptions_local_nonprimitive);

            OpenAPI_list_add(subscription_data_subscriptionsList, subscription_data_subscriptionsItem);
        }
    }

    data_change_notify_local_var = OpenAPI_data_change_notify_create (
        original_callback_reference ? original_callback_referenceList : NULL,
        ue_id ? ogs_strdup(ue_id->valuestring) : NULL,
        notify_items ? notify_itemsList : NULL,
        sdm_subscription ? sdm_subscription_local_nonprim : NULL,
        additional_sdm_subscriptions ? additional_sdm_subscriptionsList : NULL,
        subscription_data_subscriptions ? subscription_data_subscriptionsList : NULL
        );

    return data_change_notify_local_var;
end:
    return NULL;
}

OpenAPI_data_change_notify_t *OpenAPI_data_change_notify_copy(OpenAPI_data_change_notify_t *dst, OpenAPI_data_change_notify_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_data_change_notify_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_data_change_notify_convertToJSON() failed");
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

    OpenAPI_data_change_notify_free(dst);
    dst = OpenAPI_data_change_notify_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

