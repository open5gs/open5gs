
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_data_subscription.h"

OpenAPI_policy_data_subscription_t *OpenAPI_policy_data_subscription_create(
    char *notification_uri,
    char *notif_id,
    OpenAPI_list_t *monitored_resource_uris,
    OpenAPI_list_t *mon_res_items,
    char *expiry,
    char *supported_features
    )
{
    OpenAPI_policy_data_subscription_t *policy_data_subscription_local_var = OpenAPI_malloc(sizeof(OpenAPI_policy_data_subscription_t));
    if (!policy_data_subscription_local_var) {
        return NULL;
    }
    policy_data_subscription_local_var->notification_uri = notification_uri;
    policy_data_subscription_local_var->notif_id = notif_id;
    policy_data_subscription_local_var->monitored_resource_uris = monitored_resource_uris;
    policy_data_subscription_local_var->mon_res_items = mon_res_items;
    policy_data_subscription_local_var->expiry = expiry;
    policy_data_subscription_local_var->supported_features = supported_features;

    return policy_data_subscription_local_var;
}

void OpenAPI_policy_data_subscription_free(OpenAPI_policy_data_subscription_t *policy_data_subscription)
{
    if (NULL == policy_data_subscription) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(policy_data_subscription->notification_uri);
    ogs_free(policy_data_subscription->notif_id);
    OpenAPI_list_for_each(policy_data_subscription->monitored_resource_uris, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(policy_data_subscription->monitored_resource_uris);
    OpenAPI_list_for_each(policy_data_subscription->mon_res_items, node) {
        OpenAPI_resource_item_free(node->data);
    }
    OpenAPI_list_free(policy_data_subscription->mon_res_items);
    ogs_free(policy_data_subscription->expiry);
    ogs_free(policy_data_subscription->supported_features);
    ogs_free(policy_data_subscription);
}

cJSON *OpenAPI_policy_data_subscription_convertToJSON(OpenAPI_policy_data_subscription_t *policy_data_subscription)
{
    cJSON *item = NULL;

    if (policy_data_subscription == NULL) {
        ogs_error("OpenAPI_policy_data_subscription_convertToJSON() failed [PolicyDataSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!policy_data_subscription->notification_uri) {
        ogs_error("OpenAPI_policy_data_subscription_convertToJSON() failed [notification_uri]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "notificationUri", policy_data_subscription->notification_uri) == NULL) {
        ogs_error("OpenAPI_policy_data_subscription_convertToJSON() failed [notification_uri]");
        goto end;
    }

    if (policy_data_subscription->notif_id) {
        if (cJSON_AddStringToObject(item, "notifId", policy_data_subscription->notif_id) == NULL) {
            ogs_error("OpenAPI_policy_data_subscription_convertToJSON() failed [notif_id]");
            goto end;
        }
    }

    if (!policy_data_subscription->monitored_resource_uris) {
        ogs_error("OpenAPI_policy_data_subscription_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }
    cJSON *monitored_resource_uris = cJSON_AddArrayToObject(item, "monitoredResourceUris");
    if (monitored_resource_uris == NULL) {
        ogs_error("OpenAPI_policy_data_subscription_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }

    OpenAPI_lnode_t *monitored_resource_uris_node;
    OpenAPI_list_for_each(policy_data_subscription->monitored_resource_uris, monitored_resource_uris_node)  {
        if (cJSON_AddStringToObject(monitored_resource_uris, "", (char*)monitored_resource_uris_node->data) == NULL) {
            ogs_error("OpenAPI_policy_data_subscription_convertToJSON() failed [monitored_resource_uris]");
            goto end;
        }
    }

    if (policy_data_subscription->mon_res_items) {
        cJSON *mon_res_itemsList = cJSON_AddArrayToObject(item, "monResItems");
        if (mon_res_itemsList == NULL) {
            ogs_error("OpenAPI_policy_data_subscription_convertToJSON() failed [mon_res_items]");
            goto end;
        }

        OpenAPI_lnode_t *mon_res_items_node;
        if (policy_data_subscription->mon_res_items) {
            OpenAPI_list_for_each(policy_data_subscription->mon_res_items, mon_res_items_node) {
                cJSON *itemLocal = OpenAPI_resource_item_convertToJSON(mon_res_items_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_policy_data_subscription_convertToJSON() failed [mon_res_items]");
                    goto end;
                }
                cJSON_AddItemToArray(mon_res_itemsList, itemLocal);
            }
        }
    }

    if (policy_data_subscription->expiry) {
        if (cJSON_AddStringToObject(item, "expiry", policy_data_subscription->expiry) == NULL) {
            ogs_error("OpenAPI_policy_data_subscription_convertToJSON() failed [expiry]");
            goto end;
        }
    }

    if (policy_data_subscription->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", policy_data_subscription->supported_features) == NULL) {
            ogs_error("OpenAPI_policy_data_subscription_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_policy_data_subscription_t *OpenAPI_policy_data_subscription_parseFromJSON(cJSON *policy_data_subscriptionJSON)
{
    OpenAPI_policy_data_subscription_t *policy_data_subscription_local_var = NULL;
    cJSON *notification_uri = cJSON_GetObjectItemCaseSensitive(policy_data_subscriptionJSON, "notificationUri");
    if (!notification_uri) {
        ogs_error("OpenAPI_policy_data_subscription_parseFromJSON() failed [notification_uri]");
        goto end;
    }


    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_policy_data_subscription_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    cJSON *notif_id = cJSON_GetObjectItemCaseSensitive(policy_data_subscriptionJSON, "notifId");

    if (notif_id) {
        if (!cJSON_IsString(notif_id)) {
            ogs_error("OpenAPI_policy_data_subscription_parseFromJSON() failed [notif_id]");
            goto end;
        }
    }

    cJSON *monitored_resource_uris = cJSON_GetObjectItemCaseSensitive(policy_data_subscriptionJSON, "monitoredResourceUris");
    if (!monitored_resource_uris) {
        ogs_error("OpenAPI_policy_data_subscription_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }

    OpenAPI_list_t *monitored_resource_urisList;

    cJSON *monitored_resource_uris_local;
    if (!cJSON_IsArray(monitored_resource_uris)) {
        ogs_error("OpenAPI_policy_data_subscription_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
    monitored_resource_urisList = OpenAPI_list_create();

    cJSON_ArrayForEach(monitored_resource_uris_local, monitored_resource_uris) {
        if (!cJSON_IsString(monitored_resource_uris_local)) {
            ogs_error("OpenAPI_policy_data_subscription_parseFromJSON() failed [monitored_resource_uris]");
            goto end;
        }
        OpenAPI_list_add(monitored_resource_urisList, ogs_strdup(monitored_resource_uris_local->valuestring));
    }

    cJSON *mon_res_items = cJSON_GetObjectItemCaseSensitive(policy_data_subscriptionJSON, "monResItems");

    OpenAPI_list_t *mon_res_itemsList;
    if (mon_res_items) {
        cJSON *mon_res_items_local_nonprimitive;
        if (!cJSON_IsArray(mon_res_items)) {
            ogs_error("OpenAPI_policy_data_subscription_parseFromJSON() failed [mon_res_items]");
            goto end;
        }

        mon_res_itemsList = OpenAPI_list_create();

        cJSON_ArrayForEach(mon_res_items_local_nonprimitive, mon_res_items ) {
            if (!cJSON_IsObject(mon_res_items_local_nonprimitive)) {
                ogs_error("OpenAPI_policy_data_subscription_parseFromJSON() failed [mon_res_items]");
                goto end;
            }
            OpenAPI_resource_item_t *mon_res_itemsItem = OpenAPI_resource_item_parseFromJSON(mon_res_items_local_nonprimitive);

            OpenAPI_list_add(mon_res_itemsList, mon_res_itemsItem);
        }
    }

    cJSON *expiry = cJSON_GetObjectItemCaseSensitive(policy_data_subscriptionJSON, "expiry");

    if (expiry) {
        if (!cJSON_IsString(expiry)) {
            ogs_error("OpenAPI_policy_data_subscription_parseFromJSON() failed [expiry]");
            goto end;
        }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(policy_data_subscriptionJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_policy_data_subscription_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    policy_data_subscription_local_var = OpenAPI_policy_data_subscription_create (
        ogs_strdup(notification_uri->valuestring),
        notif_id ? ogs_strdup(notif_id->valuestring) : NULL,
        monitored_resource_urisList,
        mon_res_items ? mon_res_itemsList : NULL,
        expiry ? ogs_strdup(expiry->valuestring) : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
        );

    return policy_data_subscription_local_var;
end:
    return NULL;
}

OpenAPI_policy_data_subscription_t *OpenAPI_policy_data_subscription_copy(OpenAPI_policy_data_subscription_t *dst, OpenAPI_policy_data_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_policy_data_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_policy_data_subscription_convertToJSON() failed");
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

    OpenAPI_policy_data_subscription_free(dst);
    dst = OpenAPI_policy_data_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

