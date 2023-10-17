
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_n1_n2_info_subscription_created_data.h"

OpenAPI_ue_n1_n2_info_subscription_created_data_t *OpenAPI_ue_n1_n2_info_subscription_created_data_create(
    char *n1n2_notify_subscription_id,
    char *supported_features
)
{
    OpenAPI_ue_n1_n2_info_subscription_created_data_t *ue_n1_n2_info_subscription_created_data_local_var = ogs_malloc(sizeof(OpenAPI_ue_n1_n2_info_subscription_created_data_t));
    ogs_assert(ue_n1_n2_info_subscription_created_data_local_var);

    ue_n1_n2_info_subscription_created_data_local_var->n1n2_notify_subscription_id = n1n2_notify_subscription_id;
    ue_n1_n2_info_subscription_created_data_local_var->supported_features = supported_features;

    return ue_n1_n2_info_subscription_created_data_local_var;
}

void OpenAPI_ue_n1_n2_info_subscription_created_data_free(OpenAPI_ue_n1_n2_info_subscription_created_data_t *ue_n1_n2_info_subscription_created_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_n1_n2_info_subscription_created_data) {
        return;
    }
    if (ue_n1_n2_info_subscription_created_data->n1n2_notify_subscription_id) {
        ogs_free(ue_n1_n2_info_subscription_created_data->n1n2_notify_subscription_id);
        ue_n1_n2_info_subscription_created_data->n1n2_notify_subscription_id = NULL;
    }
    if (ue_n1_n2_info_subscription_created_data->supported_features) {
        ogs_free(ue_n1_n2_info_subscription_created_data->supported_features);
        ue_n1_n2_info_subscription_created_data->supported_features = NULL;
    }
    ogs_free(ue_n1_n2_info_subscription_created_data);
}

cJSON *OpenAPI_ue_n1_n2_info_subscription_created_data_convertToJSON(OpenAPI_ue_n1_n2_info_subscription_created_data_t *ue_n1_n2_info_subscription_created_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_n1_n2_info_subscription_created_data == NULL) {
        ogs_error("OpenAPI_ue_n1_n2_info_subscription_created_data_convertToJSON() failed [UeN1N2InfoSubscriptionCreatedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ue_n1_n2_info_subscription_created_data->n1n2_notify_subscription_id) {
        ogs_error("OpenAPI_ue_n1_n2_info_subscription_created_data_convertToJSON() failed [n1n2_notify_subscription_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "n1n2NotifySubscriptionId", ue_n1_n2_info_subscription_created_data->n1n2_notify_subscription_id) == NULL) {
        ogs_error("OpenAPI_ue_n1_n2_info_subscription_created_data_convertToJSON() failed [n1n2_notify_subscription_id]");
        goto end;
    }

    if (ue_n1_n2_info_subscription_created_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", ue_n1_n2_info_subscription_created_data->supported_features) == NULL) {
        ogs_error("OpenAPI_ue_n1_n2_info_subscription_created_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_n1_n2_info_subscription_created_data_t *OpenAPI_ue_n1_n2_info_subscription_created_data_parseFromJSON(cJSON *ue_n1_n2_info_subscription_created_dataJSON)
{
    OpenAPI_ue_n1_n2_info_subscription_created_data_t *ue_n1_n2_info_subscription_created_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n1n2_notify_subscription_id = NULL;
    cJSON *supported_features = NULL;
    n1n2_notify_subscription_id = cJSON_GetObjectItemCaseSensitive(ue_n1_n2_info_subscription_created_dataJSON, "n1n2NotifySubscriptionId");
    if (!n1n2_notify_subscription_id) {
        ogs_error("OpenAPI_ue_n1_n2_info_subscription_created_data_parseFromJSON() failed [n1n2_notify_subscription_id]");
        goto end;
    }
    if (!cJSON_IsString(n1n2_notify_subscription_id)) {
        ogs_error("OpenAPI_ue_n1_n2_info_subscription_created_data_parseFromJSON() failed [n1n2_notify_subscription_id]");
        goto end;
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(ue_n1_n2_info_subscription_created_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_ue_n1_n2_info_subscription_created_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    ue_n1_n2_info_subscription_created_data_local_var = OpenAPI_ue_n1_n2_info_subscription_created_data_create (
        ogs_strdup(n1n2_notify_subscription_id->valuestring),
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return ue_n1_n2_info_subscription_created_data_local_var;
end:
    return NULL;
}

OpenAPI_ue_n1_n2_info_subscription_created_data_t *OpenAPI_ue_n1_n2_info_subscription_created_data_copy(OpenAPI_ue_n1_n2_info_subscription_created_data_t *dst, OpenAPI_ue_n1_n2_info_subscription_created_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_n1_n2_info_subscription_created_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_n1_n2_info_subscription_created_data_convertToJSON() failed");
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

    OpenAPI_ue_n1_n2_info_subscription_created_data_free(dst);
    dst = OpenAPI_ue_n1_n2_info_subscription_created_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

