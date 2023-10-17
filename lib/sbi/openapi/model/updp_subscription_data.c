
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "updp_subscription_data.h"

OpenAPI_updp_subscription_data_t *OpenAPI_updp_subscription_data_create(
    char *updp_notify_subscription_id,
    char *updp_notify_callback_uri,
    char *supported_features,
    char *updp_callback_binding
)
{
    OpenAPI_updp_subscription_data_t *updp_subscription_data_local_var = ogs_malloc(sizeof(OpenAPI_updp_subscription_data_t));
    ogs_assert(updp_subscription_data_local_var);

    updp_subscription_data_local_var->updp_notify_subscription_id = updp_notify_subscription_id;
    updp_subscription_data_local_var->updp_notify_callback_uri = updp_notify_callback_uri;
    updp_subscription_data_local_var->supported_features = supported_features;
    updp_subscription_data_local_var->updp_callback_binding = updp_callback_binding;

    return updp_subscription_data_local_var;
}

void OpenAPI_updp_subscription_data_free(OpenAPI_updp_subscription_data_t *updp_subscription_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == updp_subscription_data) {
        return;
    }
    if (updp_subscription_data->updp_notify_subscription_id) {
        ogs_free(updp_subscription_data->updp_notify_subscription_id);
        updp_subscription_data->updp_notify_subscription_id = NULL;
    }
    if (updp_subscription_data->updp_notify_callback_uri) {
        ogs_free(updp_subscription_data->updp_notify_callback_uri);
        updp_subscription_data->updp_notify_callback_uri = NULL;
    }
    if (updp_subscription_data->supported_features) {
        ogs_free(updp_subscription_data->supported_features);
        updp_subscription_data->supported_features = NULL;
    }
    if (updp_subscription_data->updp_callback_binding) {
        ogs_free(updp_subscription_data->updp_callback_binding);
        updp_subscription_data->updp_callback_binding = NULL;
    }
    ogs_free(updp_subscription_data);
}

cJSON *OpenAPI_updp_subscription_data_convertToJSON(OpenAPI_updp_subscription_data_t *updp_subscription_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (updp_subscription_data == NULL) {
        ogs_error("OpenAPI_updp_subscription_data_convertToJSON() failed [UpdpSubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!updp_subscription_data->updp_notify_subscription_id) {
        ogs_error("OpenAPI_updp_subscription_data_convertToJSON() failed [updp_notify_subscription_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "updpNotifySubscriptionId", updp_subscription_data->updp_notify_subscription_id) == NULL) {
        ogs_error("OpenAPI_updp_subscription_data_convertToJSON() failed [updp_notify_subscription_id]");
        goto end;
    }

    if (!updp_subscription_data->updp_notify_callback_uri) {
        ogs_error("OpenAPI_updp_subscription_data_convertToJSON() failed [updp_notify_callback_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "updpNotifyCallbackUri", updp_subscription_data->updp_notify_callback_uri) == NULL) {
        ogs_error("OpenAPI_updp_subscription_data_convertToJSON() failed [updp_notify_callback_uri]");
        goto end;
    }

    if (updp_subscription_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", updp_subscription_data->supported_features) == NULL) {
        ogs_error("OpenAPI_updp_subscription_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (updp_subscription_data->updp_callback_binding) {
    if (cJSON_AddStringToObject(item, "updpCallbackBinding", updp_subscription_data->updp_callback_binding) == NULL) {
        ogs_error("OpenAPI_updp_subscription_data_convertToJSON() failed [updp_callback_binding]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_updp_subscription_data_t *OpenAPI_updp_subscription_data_parseFromJSON(cJSON *updp_subscription_dataJSON)
{
    OpenAPI_updp_subscription_data_t *updp_subscription_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *updp_notify_subscription_id = NULL;
    cJSON *updp_notify_callback_uri = NULL;
    cJSON *supported_features = NULL;
    cJSON *updp_callback_binding = NULL;
    updp_notify_subscription_id = cJSON_GetObjectItemCaseSensitive(updp_subscription_dataJSON, "updpNotifySubscriptionId");
    if (!updp_notify_subscription_id) {
        ogs_error("OpenAPI_updp_subscription_data_parseFromJSON() failed [updp_notify_subscription_id]");
        goto end;
    }
    if (!cJSON_IsString(updp_notify_subscription_id)) {
        ogs_error("OpenAPI_updp_subscription_data_parseFromJSON() failed [updp_notify_subscription_id]");
        goto end;
    }

    updp_notify_callback_uri = cJSON_GetObjectItemCaseSensitive(updp_subscription_dataJSON, "updpNotifyCallbackUri");
    if (!updp_notify_callback_uri) {
        ogs_error("OpenAPI_updp_subscription_data_parseFromJSON() failed [updp_notify_callback_uri]");
        goto end;
    }
    if (!cJSON_IsString(updp_notify_callback_uri)) {
        ogs_error("OpenAPI_updp_subscription_data_parseFromJSON() failed [updp_notify_callback_uri]");
        goto end;
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(updp_subscription_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_updp_subscription_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    updp_callback_binding = cJSON_GetObjectItemCaseSensitive(updp_subscription_dataJSON, "updpCallbackBinding");
    if (updp_callback_binding) {
    if (!cJSON_IsString(updp_callback_binding) && !cJSON_IsNull(updp_callback_binding)) {
        ogs_error("OpenAPI_updp_subscription_data_parseFromJSON() failed [updp_callback_binding]");
        goto end;
    }
    }

    updp_subscription_data_local_var = OpenAPI_updp_subscription_data_create (
        ogs_strdup(updp_notify_subscription_id->valuestring),
        ogs_strdup(updp_notify_callback_uri->valuestring),
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        updp_callback_binding && !cJSON_IsNull(updp_callback_binding) ? ogs_strdup(updp_callback_binding->valuestring) : NULL
    );

    return updp_subscription_data_local_var;
end:
    return NULL;
}

OpenAPI_updp_subscription_data_t *OpenAPI_updp_subscription_data_copy(OpenAPI_updp_subscription_data_t *dst, OpenAPI_updp_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_updp_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_updp_subscription_data_convertToJSON() failed");
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

    OpenAPI_updp_subscription_data_free(dst);
    dst = OpenAPI_updp_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

