
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_n1_n2_info_subscription_create_data.h"

OpenAPI_ue_n1_n2_info_subscription_create_data_t *OpenAPI_ue_n1_n2_info_subscription_create_data_create(
    OpenAPI_n2_information_class_e n2_information_class,
    char *n2_notify_callback_uri,
    OpenAPI_n1_message_class_e n1_message_class,
    char *n1_notify_callback_uri,
    char *nf_id,
    char *supported_features,
    OpenAPI_guami_t *old_guami
    )
{
    OpenAPI_ue_n1_n2_info_subscription_create_data_t *ue_n1_n2_info_subscription_create_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_ue_n1_n2_info_subscription_create_data_t));
    if (!ue_n1_n2_info_subscription_create_data_local_var) {
        return NULL;
    }
    ue_n1_n2_info_subscription_create_data_local_var->n2_information_class = n2_information_class;
    ue_n1_n2_info_subscription_create_data_local_var->n2_notify_callback_uri = n2_notify_callback_uri;
    ue_n1_n2_info_subscription_create_data_local_var->n1_message_class = n1_message_class;
    ue_n1_n2_info_subscription_create_data_local_var->n1_notify_callback_uri = n1_notify_callback_uri;
    ue_n1_n2_info_subscription_create_data_local_var->nf_id = nf_id;
    ue_n1_n2_info_subscription_create_data_local_var->supported_features = supported_features;
    ue_n1_n2_info_subscription_create_data_local_var->old_guami = old_guami;

    return ue_n1_n2_info_subscription_create_data_local_var;
}

void OpenAPI_ue_n1_n2_info_subscription_create_data_free(OpenAPI_ue_n1_n2_info_subscription_create_data_t *ue_n1_n2_info_subscription_create_data)
{
    if (NULL == ue_n1_n2_info_subscription_create_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ue_n1_n2_info_subscription_create_data->n2_notify_callback_uri);
    ogs_free(ue_n1_n2_info_subscription_create_data->n1_notify_callback_uri);
    ogs_free(ue_n1_n2_info_subscription_create_data->nf_id);
    ogs_free(ue_n1_n2_info_subscription_create_data->supported_features);
    OpenAPI_guami_free(ue_n1_n2_info_subscription_create_data->old_guami);
    ogs_free(ue_n1_n2_info_subscription_create_data);
}

cJSON *OpenAPI_ue_n1_n2_info_subscription_create_data_convertToJSON(OpenAPI_ue_n1_n2_info_subscription_create_data_t *ue_n1_n2_info_subscription_create_data)
{
    cJSON *item = NULL;

    if (ue_n1_n2_info_subscription_create_data == NULL) {
        ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_convertToJSON() failed [UeN1N2InfoSubscriptionCreateData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_n1_n2_info_subscription_create_data->n2_information_class) {
        if (cJSON_AddStringToObject(item, "n2InformationClass", OpenAPI_n2_information_class_ToString(ue_n1_n2_info_subscription_create_data->n2_information_class)) == NULL) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_convertToJSON() failed [n2_information_class]");
            goto end;
        }
    }

    if (ue_n1_n2_info_subscription_create_data->n2_notify_callback_uri) {
        if (cJSON_AddStringToObject(item, "n2NotifyCallbackUri", ue_n1_n2_info_subscription_create_data->n2_notify_callback_uri) == NULL) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_convertToJSON() failed [n2_notify_callback_uri]");
            goto end;
        }
    }

    if (ue_n1_n2_info_subscription_create_data->n1_message_class) {
        if (cJSON_AddStringToObject(item, "n1MessageClass", OpenAPI_n1_message_class_ToString(ue_n1_n2_info_subscription_create_data->n1_message_class)) == NULL) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_convertToJSON() failed [n1_message_class]");
            goto end;
        }
    }

    if (ue_n1_n2_info_subscription_create_data->n1_notify_callback_uri) {
        if (cJSON_AddStringToObject(item, "n1NotifyCallbackUri", ue_n1_n2_info_subscription_create_data->n1_notify_callback_uri) == NULL) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_convertToJSON() failed [n1_notify_callback_uri]");
            goto end;
        }
    }

    if (ue_n1_n2_info_subscription_create_data->nf_id) {
        if (cJSON_AddStringToObject(item, "nfId", ue_n1_n2_info_subscription_create_data->nf_id) == NULL) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_convertToJSON() failed [nf_id]");
            goto end;
        }
    }

    if (ue_n1_n2_info_subscription_create_data->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", ue_n1_n2_info_subscription_create_data->supported_features) == NULL) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

    if (ue_n1_n2_info_subscription_create_data->old_guami) {
        cJSON *old_guami_local_JSON = OpenAPI_guami_convertToJSON(ue_n1_n2_info_subscription_create_data->old_guami);
        if (old_guami_local_JSON == NULL) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_convertToJSON() failed [old_guami]");
            goto end;
        }
        cJSON_AddItemToObject(item, "oldGuami", old_guami_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_convertToJSON() failed [old_guami]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_ue_n1_n2_info_subscription_create_data_t *OpenAPI_ue_n1_n2_info_subscription_create_data_parseFromJSON(cJSON *ue_n1_n2_info_subscription_create_dataJSON)
{
    OpenAPI_ue_n1_n2_info_subscription_create_data_t *ue_n1_n2_info_subscription_create_data_local_var = NULL;
    cJSON *n2_information_class = cJSON_GetObjectItemCaseSensitive(ue_n1_n2_info_subscription_create_dataJSON, "n2InformationClass");

    OpenAPI_n2_information_class_e n2_information_classVariable;
    if (n2_information_class) {
        if (!cJSON_IsString(n2_information_class)) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_parseFromJSON() failed [n2_information_class]");
            goto end;
        }
        n2_information_classVariable = OpenAPI_n2_information_class_FromString(n2_information_class->valuestring);
    }

    cJSON *n2_notify_callback_uri = cJSON_GetObjectItemCaseSensitive(ue_n1_n2_info_subscription_create_dataJSON, "n2NotifyCallbackUri");

    if (n2_notify_callback_uri) {
        if (!cJSON_IsString(n2_notify_callback_uri)) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_parseFromJSON() failed [n2_notify_callback_uri]");
            goto end;
        }
    }

    cJSON *n1_message_class = cJSON_GetObjectItemCaseSensitive(ue_n1_n2_info_subscription_create_dataJSON, "n1MessageClass");

    OpenAPI_n1_message_class_e n1_message_classVariable;
    if (n1_message_class) {
        if (!cJSON_IsString(n1_message_class)) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_parseFromJSON() failed [n1_message_class]");
            goto end;
        }
        n1_message_classVariable = OpenAPI_n1_message_class_FromString(n1_message_class->valuestring);
    }

    cJSON *n1_notify_callback_uri = cJSON_GetObjectItemCaseSensitive(ue_n1_n2_info_subscription_create_dataJSON, "n1NotifyCallbackUri");

    if (n1_notify_callback_uri) {
        if (!cJSON_IsString(n1_notify_callback_uri)) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_parseFromJSON() failed [n1_notify_callback_uri]");
            goto end;
        }
    }

    cJSON *nf_id = cJSON_GetObjectItemCaseSensitive(ue_n1_n2_info_subscription_create_dataJSON, "nfId");

    if (nf_id) {
        if (!cJSON_IsString(nf_id)) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_parseFromJSON() failed [nf_id]");
            goto end;
        }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(ue_n1_n2_info_subscription_create_dataJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    cJSON *old_guami = cJSON_GetObjectItemCaseSensitive(ue_n1_n2_info_subscription_create_dataJSON, "oldGuami");

    OpenAPI_guami_t *old_guami_local_nonprim = NULL;
    if (old_guami) {
        old_guami_local_nonprim = OpenAPI_guami_parseFromJSON(old_guami);
    }

    ue_n1_n2_info_subscription_create_data_local_var = OpenAPI_ue_n1_n2_info_subscription_create_data_create (
        n2_information_class ? n2_information_classVariable : 0,
        n2_notify_callback_uri ? ogs_strdup(n2_notify_callback_uri->valuestring) : NULL,
        n1_message_class ? n1_message_classVariable : 0,
        n1_notify_callback_uri ? ogs_strdup(n1_notify_callback_uri->valuestring) : NULL,
        nf_id ? ogs_strdup(nf_id->valuestring) : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        old_guami ? old_guami_local_nonprim : NULL
        );

    return ue_n1_n2_info_subscription_create_data_local_var;
end:
    return NULL;
}

OpenAPI_ue_n1_n2_info_subscription_create_data_t *OpenAPI_ue_n1_n2_info_subscription_create_data_copy(OpenAPI_ue_n1_n2_info_subscription_create_data_t *dst, OpenAPI_ue_n1_n2_info_subscription_create_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_n1_n2_info_subscription_create_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_n1_n2_info_subscription_create_data_convertToJSON() failed");
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

    OpenAPI_ue_n1_n2_info_subscription_create_data_free(dst);
    dst = OpenAPI_ue_n1_n2_info_subscription_create_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

