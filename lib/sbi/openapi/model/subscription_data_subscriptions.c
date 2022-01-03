
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscription_data_subscriptions.h"

OpenAPI_subscription_data_subscriptions_t *OpenAPI_subscription_data_subscriptions_create(
    char *ue_id,
    char *callback_reference,
    char *original_callback_reference,
    OpenAPI_list_t *monitored_resource_uris,
    char *expiry,
    OpenAPI_sdm_subscription_1_t *sdm_subscription,
    char *subscription_id,
    char *supported_features
)
{
    OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptions_local_var = ogs_malloc(sizeof(OpenAPI_subscription_data_subscriptions_t));
    ogs_assert(subscription_data_subscriptions_local_var);

    subscription_data_subscriptions_local_var->ue_id = ue_id;
    subscription_data_subscriptions_local_var->callback_reference = callback_reference;
    subscription_data_subscriptions_local_var->original_callback_reference = original_callback_reference;
    subscription_data_subscriptions_local_var->monitored_resource_uris = monitored_resource_uris;
    subscription_data_subscriptions_local_var->expiry = expiry;
    subscription_data_subscriptions_local_var->sdm_subscription = sdm_subscription;
    subscription_data_subscriptions_local_var->subscription_id = subscription_id;
    subscription_data_subscriptions_local_var->supported_features = supported_features;

    return subscription_data_subscriptions_local_var;
}

void OpenAPI_subscription_data_subscriptions_free(OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptions)
{
    if (NULL == subscription_data_subscriptions) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(subscription_data_subscriptions->ue_id);
    ogs_free(subscription_data_subscriptions->callback_reference);
    ogs_free(subscription_data_subscriptions->original_callback_reference);
    OpenAPI_list_for_each(subscription_data_subscriptions->monitored_resource_uris, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(subscription_data_subscriptions->monitored_resource_uris);
    ogs_free(subscription_data_subscriptions->expiry);
    OpenAPI_sdm_subscription_1_free(subscription_data_subscriptions->sdm_subscription);
    ogs_free(subscription_data_subscriptions->subscription_id);
    ogs_free(subscription_data_subscriptions->supported_features);
    ogs_free(subscription_data_subscriptions);
}

cJSON *OpenAPI_subscription_data_subscriptions_convertToJSON(OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptions)
{
    cJSON *item = NULL;

    if (subscription_data_subscriptions == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [SubscriptionDataSubscriptions]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (subscription_data_subscriptions->ue_id) {
    if (cJSON_AddStringToObject(item, "ueId", subscription_data_subscriptions->ue_id) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [ue_id]");
        goto end;
    }
    }

    if (cJSON_AddStringToObject(item, "callbackReference", subscription_data_subscriptions->callback_reference) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [callback_reference]");
        goto end;
    }

    if (subscription_data_subscriptions->original_callback_reference) {
    if (cJSON_AddStringToObject(item, "originalCallbackReference", subscription_data_subscriptions->original_callback_reference) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [original_callback_reference]");
        goto end;
    }
    }

    cJSON *monitored_resource_uris = cJSON_AddArrayToObject(item, "monitoredResourceUris");
    if (monitored_resource_uris == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }

    OpenAPI_lnode_t *monitored_resource_uris_node;
    OpenAPI_list_for_each(subscription_data_subscriptions->monitored_resource_uris, monitored_resource_uris_node)  {
    if (cJSON_AddStringToObject(monitored_resource_uris, "", (char*)monitored_resource_uris_node->data) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }
                    }

    if (subscription_data_subscriptions->expiry) {
    if (cJSON_AddStringToObject(item, "expiry", subscription_data_subscriptions->expiry) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [expiry]");
        goto end;
    }
    }

    if (subscription_data_subscriptions->sdm_subscription) {
    cJSON *sdm_subscription_local_JSON = OpenAPI_sdm_subscription_1_convertToJSON(subscription_data_subscriptions->sdm_subscription);
    if (sdm_subscription_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [sdm_subscription]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sdmSubscription", sdm_subscription_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [sdm_subscription]");
        goto end;
    }
    }

    if (subscription_data_subscriptions->subscription_id) {
    if (cJSON_AddStringToObject(item, "subscriptionId", subscription_data_subscriptions->subscription_id) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [subscription_id]");
        goto end;
    }
    }

    if (subscription_data_subscriptions->supported_features) {
    if (cJSON_AddStringToObject(item, "supported-features", subscription_data_subscriptions->supported_features) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_subscription_data_subscriptions_t *OpenAPI_subscription_data_subscriptions_parseFromJSON(cJSON *subscription_data_subscriptionsJSON)
{
    OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptions_local_var = NULL;
    cJSON *ue_id = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "ueId");

    if (ue_id) {
    if (!cJSON_IsString(ue_id)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [ue_id]");
        goto end;
    }
    }

    cJSON *callback_reference = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "callbackReference");
    if (!callback_reference) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [callback_reference]");
        goto end;
    }

    if (!cJSON_IsString(callback_reference)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [callback_reference]");
        goto end;
    }

    cJSON *original_callback_reference = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "originalCallbackReference");

    if (original_callback_reference) {
    if (!cJSON_IsString(original_callback_reference)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [original_callback_reference]");
        goto end;
    }
    }

    cJSON *monitored_resource_uris = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "monitoredResourceUris");
    if (!monitored_resource_uris) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }

    OpenAPI_list_t *monitored_resource_urisList;
    cJSON *monitored_resource_uris_local;
    if (!cJSON_IsArray(monitored_resource_uris)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
    monitored_resource_urisList = OpenAPI_list_create();

    cJSON_ArrayForEach(monitored_resource_uris_local, monitored_resource_uris) {
    if (!cJSON_IsString(monitored_resource_uris_local)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
    OpenAPI_list_add(monitored_resource_urisList , ogs_strdup(monitored_resource_uris_local->valuestring));
    }

    cJSON *expiry = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "expiry");

    if (expiry) {
    if (!cJSON_IsString(expiry)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    cJSON *sdm_subscription = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "sdmSubscription");

    OpenAPI_sdm_subscription_1_t *sdm_subscription_local_nonprim = NULL;
    if (sdm_subscription) {
    sdm_subscription_local_nonprim = OpenAPI_sdm_subscription_1_parseFromJSON(sdm_subscription);
    }

    cJSON *subscription_id = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "subscriptionId");

    if (subscription_id) {
    if (!cJSON_IsString(subscription_id)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [subscription_id]");
        goto end;
    }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "supported-features");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    subscription_data_subscriptions_local_var = OpenAPI_subscription_data_subscriptions_create (
        ue_id ? ogs_strdup(ue_id->valuestring) : NULL,
        ogs_strdup(callback_reference->valuestring),
        original_callback_reference ? ogs_strdup(original_callback_reference->valuestring) : NULL,
        monitored_resource_urisList,
        expiry ? ogs_strdup(expiry->valuestring) : NULL,
        sdm_subscription ? sdm_subscription_local_nonprim : NULL,
        subscription_id ? ogs_strdup(subscription_id->valuestring) : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return subscription_data_subscriptions_local_var;
end:
    return NULL;
}

OpenAPI_subscription_data_subscriptions_t *OpenAPI_subscription_data_subscriptions_copy(OpenAPI_subscription_data_subscriptions_t *dst, OpenAPI_subscription_data_subscriptions_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_subscription_data_subscriptions_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed");
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

    OpenAPI_subscription_data_subscriptions_free(dst);
    dst = OpenAPI_subscription_data_subscriptions_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

