
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "exposure_data_subscription.h"

OpenAPI_exposure_data_subscription_t *OpenAPI_exposure_data_subscription_create(
    char *notification_uri,
    OpenAPI_list_t *monitored_resource_uris,
    char *expiry,
    char *supported_features
)
{
    OpenAPI_exposure_data_subscription_t *exposure_data_subscription_local_var = ogs_malloc(sizeof(OpenAPI_exposure_data_subscription_t));
    ogs_assert(exposure_data_subscription_local_var);

    exposure_data_subscription_local_var->notification_uri = notification_uri;
    exposure_data_subscription_local_var->monitored_resource_uris = monitored_resource_uris;
    exposure_data_subscription_local_var->expiry = expiry;
    exposure_data_subscription_local_var->supported_features = supported_features;

    return exposure_data_subscription_local_var;
}

void OpenAPI_exposure_data_subscription_free(OpenAPI_exposure_data_subscription_t *exposure_data_subscription)
{
    if (NULL == exposure_data_subscription) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(exposure_data_subscription->notification_uri);
    OpenAPI_list_for_each(exposure_data_subscription->monitored_resource_uris, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(exposure_data_subscription->monitored_resource_uris);
    ogs_free(exposure_data_subscription->expiry);
    ogs_free(exposure_data_subscription->supported_features);
    ogs_free(exposure_data_subscription);
}

cJSON *OpenAPI_exposure_data_subscription_convertToJSON(OpenAPI_exposure_data_subscription_t *exposure_data_subscription)
{
    cJSON *item = NULL;

    if (exposure_data_subscription == NULL) {
        ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed [ExposureDataSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "notificationUri", exposure_data_subscription->notification_uri) == NULL) {
        ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed [notification_uri]");
        goto end;
    }

    cJSON *monitored_resource_uris = cJSON_AddArrayToObject(item, "monitoredResourceUris");
    if (monitored_resource_uris == NULL) {
        ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }

    OpenAPI_lnode_t *monitored_resource_uris_node;
    OpenAPI_list_for_each(exposure_data_subscription->monitored_resource_uris, monitored_resource_uris_node)  {
    if (cJSON_AddStringToObject(monitored_resource_uris, "", (char*)monitored_resource_uris_node->data) == NULL) {
        ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }
                    }

    if (exposure_data_subscription->expiry) {
    if (cJSON_AddStringToObject(item, "expiry", exposure_data_subscription->expiry) == NULL) {
        ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed [expiry]");
        goto end;
    }
    }

    if (exposure_data_subscription->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", exposure_data_subscription->supported_features) == NULL) {
        ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_exposure_data_subscription_t *OpenAPI_exposure_data_subscription_parseFromJSON(cJSON *exposure_data_subscriptionJSON)
{
    OpenAPI_exposure_data_subscription_t *exposure_data_subscription_local_var = NULL;
    cJSON *notification_uri = cJSON_GetObjectItemCaseSensitive(exposure_data_subscriptionJSON, "notificationUri");
    if (!notification_uri) {
        ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    cJSON *monitored_resource_uris = cJSON_GetObjectItemCaseSensitive(exposure_data_subscriptionJSON, "monitoredResourceUris");
    if (!monitored_resource_uris) {
        ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }

    OpenAPI_list_t *monitored_resource_urisList;
    cJSON *monitored_resource_uris_local;
    if (!cJSON_IsArray(monitored_resource_uris)) {
        ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
    monitored_resource_urisList = OpenAPI_list_create();

    cJSON_ArrayForEach(monitored_resource_uris_local, monitored_resource_uris) {
    if (!cJSON_IsString(monitored_resource_uris_local)) {
        ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
    OpenAPI_list_add(monitored_resource_urisList , ogs_strdup(monitored_resource_uris_local->valuestring));
    }

    cJSON *expiry = cJSON_GetObjectItemCaseSensitive(exposure_data_subscriptionJSON, "expiry");

    if (expiry) {
    if (!cJSON_IsString(expiry)) {
        ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(exposure_data_subscriptionJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    exposure_data_subscription_local_var = OpenAPI_exposure_data_subscription_create (
        ogs_strdup(notification_uri->valuestring),
        monitored_resource_urisList,
        expiry ? ogs_strdup(expiry->valuestring) : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return exposure_data_subscription_local_var;
end:
    return NULL;
}

OpenAPI_exposure_data_subscription_t *OpenAPI_exposure_data_subscription_copy(OpenAPI_exposure_data_subscription_t *dst, OpenAPI_exposure_data_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_exposure_data_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed");
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

    OpenAPI_exposure_data_subscription_free(dst);
    dst = OpenAPI_exposure_data_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

