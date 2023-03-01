
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "exposure_data_subscription.h"

OpenAPI_exposure_data_subscription_t *OpenAPI_exposure_data_subscription_create(
    char *notification_uri,
    OpenAPI_list_t *monitored_resource_uris,
    char *expiry,
    char *supported_features,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_exposure_data_subscription_t *exposure_data_subscription_local_var = ogs_malloc(sizeof(OpenAPI_exposure_data_subscription_t));
    ogs_assert(exposure_data_subscription_local_var);

    exposure_data_subscription_local_var->notification_uri = notification_uri;
    exposure_data_subscription_local_var->monitored_resource_uris = monitored_resource_uris;
    exposure_data_subscription_local_var->expiry = expiry;
    exposure_data_subscription_local_var->supported_features = supported_features;
    exposure_data_subscription_local_var->reset_ids = reset_ids;

    return exposure_data_subscription_local_var;
}

void OpenAPI_exposure_data_subscription_free(OpenAPI_exposure_data_subscription_t *exposure_data_subscription)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == exposure_data_subscription) {
        return;
    }
    if (exposure_data_subscription->notification_uri) {
        ogs_free(exposure_data_subscription->notification_uri);
        exposure_data_subscription->notification_uri = NULL;
    }
    if (exposure_data_subscription->monitored_resource_uris) {
        OpenAPI_list_for_each(exposure_data_subscription->monitored_resource_uris, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(exposure_data_subscription->monitored_resource_uris);
        exposure_data_subscription->monitored_resource_uris = NULL;
    }
    if (exposure_data_subscription->expiry) {
        ogs_free(exposure_data_subscription->expiry);
        exposure_data_subscription->expiry = NULL;
    }
    if (exposure_data_subscription->supported_features) {
        ogs_free(exposure_data_subscription->supported_features);
        exposure_data_subscription->supported_features = NULL;
    }
    if (exposure_data_subscription->reset_ids) {
        OpenAPI_list_for_each(exposure_data_subscription->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(exposure_data_subscription->reset_ids);
        exposure_data_subscription->reset_ids = NULL;
    }
    ogs_free(exposure_data_subscription);
}

cJSON *OpenAPI_exposure_data_subscription_convertToJSON(OpenAPI_exposure_data_subscription_t *exposure_data_subscription)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (exposure_data_subscription == NULL) {
        ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed [ExposureDataSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!exposure_data_subscription->notification_uri) {
        ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed [notification_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notificationUri", exposure_data_subscription->notification_uri) == NULL) {
        ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed [notification_uri]");
        goto end;
    }

    if (!exposure_data_subscription->monitored_resource_uris) {
        ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed [monitored_resource_uris]");
        return NULL;
    }
    cJSON *monitored_resource_urisList = cJSON_AddArrayToObject(item, "monitoredResourceUris");
    if (monitored_resource_urisList == NULL) {
        ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }
    OpenAPI_list_for_each(exposure_data_subscription->monitored_resource_uris, node) {
        if (cJSON_AddStringToObject(monitored_resource_urisList, "", (char*)node->data) == NULL) {
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

    if (exposure_data_subscription->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(exposure_data_subscription->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_exposure_data_subscription_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_exposure_data_subscription_t *OpenAPI_exposure_data_subscription_parseFromJSON(cJSON *exposure_data_subscriptionJSON)
{
    OpenAPI_exposure_data_subscription_t *exposure_data_subscription_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notification_uri = NULL;
    cJSON *monitored_resource_uris = NULL;
    OpenAPI_list_t *monitored_resource_urisList = NULL;
    cJSON *expiry = NULL;
    cJSON *supported_features = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    notification_uri = cJSON_GetObjectItemCaseSensitive(exposure_data_subscriptionJSON, "notificationUri");
    if (!notification_uri) {
        ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [notification_uri]");
        goto end;
    }
    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    monitored_resource_uris = cJSON_GetObjectItemCaseSensitive(exposure_data_subscriptionJSON, "monitoredResourceUris");
    if (!monitored_resource_uris) {
        ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
        cJSON *monitored_resource_uris_local = NULL;
        if (!cJSON_IsArray(monitored_resource_uris)) {
            ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [monitored_resource_uris]");
            goto end;
        }

        monitored_resource_urisList = OpenAPI_list_create();

        cJSON_ArrayForEach(monitored_resource_uris_local, monitored_resource_uris) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(monitored_resource_uris_local)) {
                ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [monitored_resource_uris]");
                goto end;
            }
            OpenAPI_list_add(monitored_resource_urisList, ogs_strdup(monitored_resource_uris_local->valuestring));
        }

    expiry = cJSON_GetObjectItemCaseSensitive(exposure_data_subscriptionJSON, "expiry");
    if (expiry) {
    if (!cJSON_IsString(expiry) && !cJSON_IsNull(expiry)) {
        ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(exposure_data_subscriptionJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(exposure_data_subscriptionJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_exposure_data_subscription_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    exposure_data_subscription_local_var = OpenAPI_exposure_data_subscription_create (
        ogs_strdup(notification_uri->valuestring),
        monitored_resource_urisList,
        expiry && !cJSON_IsNull(expiry) ? ogs_strdup(expiry->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return exposure_data_subscription_local_var;
end:
    if (monitored_resource_urisList) {
        OpenAPI_list_for_each(monitored_resource_urisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(monitored_resource_urisList);
        monitored_resource_urisList = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
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

