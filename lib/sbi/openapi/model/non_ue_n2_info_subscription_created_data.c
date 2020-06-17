
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "non_ue_n2_info_subscription_created_data.h"

OpenAPI_non_ue_n2_info_subscription_created_data_t *OpenAPI_non_ue_n2_info_subscription_created_data_create(
    char *n2_notify_subscription_id,
    char *supported_features,
    OpenAPI_n2_information_class_e n2_information_class
    )
{
    OpenAPI_non_ue_n2_info_subscription_created_data_t *non_ue_n2_info_subscription_created_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_non_ue_n2_info_subscription_created_data_t));
    if (!non_ue_n2_info_subscription_created_data_local_var) {
        return NULL;
    }
    non_ue_n2_info_subscription_created_data_local_var->n2_notify_subscription_id = n2_notify_subscription_id;
    non_ue_n2_info_subscription_created_data_local_var->supported_features = supported_features;
    non_ue_n2_info_subscription_created_data_local_var->n2_information_class = n2_information_class;

    return non_ue_n2_info_subscription_created_data_local_var;
}

void OpenAPI_non_ue_n2_info_subscription_created_data_free(OpenAPI_non_ue_n2_info_subscription_created_data_t *non_ue_n2_info_subscription_created_data)
{
    if (NULL == non_ue_n2_info_subscription_created_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(non_ue_n2_info_subscription_created_data->n2_notify_subscription_id);
    ogs_free(non_ue_n2_info_subscription_created_data->supported_features);
    ogs_free(non_ue_n2_info_subscription_created_data);
}

cJSON *OpenAPI_non_ue_n2_info_subscription_created_data_convertToJSON(OpenAPI_non_ue_n2_info_subscription_created_data_t *non_ue_n2_info_subscription_created_data)
{
    cJSON *item = NULL;

    if (non_ue_n2_info_subscription_created_data == NULL) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_created_data_convertToJSON() failed [NonUeN2InfoSubscriptionCreatedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!non_ue_n2_info_subscription_created_data->n2_notify_subscription_id) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_created_data_convertToJSON() failed [n2_notify_subscription_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "n2NotifySubscriptionId", non_ue_n2_info_subscription_created_data->n2_notify_subscription_id) == NULL) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_created_data_convertToJSON() failed [n2_notify_subscription_id]");
        goto end;
    }

    if (non_ue_n2_info_subscription_created_data->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", non_ue_n2_info_subscription_created_data->supported_features) == NULL) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_created_data_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

    if (non_ue_n2_info_subscription_created_data->n2_information_class) {
        if (cJSON_AddStringToObject(item, "n2InformationClass", OpenAPI_n2_information_class_ToString(non_ue_n2_info_subscription_created_data->n2_information_class)) == NULL) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_created_data_convertToJSON() failed [n2_information_class]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_non_ue_n2_info_subscription_created_data_t *OpenAPI_non_ue_n2_info_subscription_created_data_parseFromJSON(cJSON *non_ue_n2_info_subscription_created_dataJSON)
{
    OpenAPI_non_ue_n2_info_subscription_created_data_t *non_ue_n2_info_subscription_created_data_local_var = NULL;
    cJSON *n2_notify_subscription_id = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_created_dataJSON, "n2NotifySubscriptionId");
    if (!n2_notify_subscription_id) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_created_data_parseFromJSON() failed [n2_notify_subscription_id]");
        goto end;
    }


    if (!cJSON_IsString(n2_notify_subscription_id)) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_created_data_parseFromJSON() failed [n2_notify_subscription_id]");
        goto end;
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_created_dataJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_created_data_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    cJSON *n2_information_class = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_subscription_created_dataJSON, "n2InformationClass");

    OpenAPI_n2_information_class_e n2_information_classVariable;
    if (n2_information_class) {
        if (!cJSON_IsString(n2_information_class)) {
            ogs_error("OpenAPI_non_ue_n2_info_subscription_created_data_parseFromJSON() failed [n2_information_class]");
            goto end;
        }
        n2_information_classVariable = OpenAPI_n2_information_class_FromString(n2_information_class->valuestring);
    }

    non_ue_n2_info_subscription_created_data_local_var = OpenAPI_non_ue_n2_info_subscription_created_data_create (
        ogs_strdup(n2_notify_subscription_id->valuestring),
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        n2_information_class ? n2_information_classVariable : 0
        );

    return non_ue_n2_info_subscription_created_data_local_var;
end:
    return NULL;
}

OpenAPI_non_ue_n2_info_subscription_created_data_t *OpenAPI_non_ue_n2_info_subscription_created_data_copy(OpenAPI_non_ue_n2_info_subscription_created_data_t *dst, OpenAPI_non_ue_n2_info_subscription_created_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_non_ue_n2_info_subscription_created_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_non_ue_n2_info_subscription_created_data_convertToJSON() failed");
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

    OpenAPI_non_ue_n2_info_subscription_created_data_free(dst);
    dst = OpenAPI_non_ue_n2_info_subscription_created_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

