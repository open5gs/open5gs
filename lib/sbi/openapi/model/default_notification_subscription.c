
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "default_notification_subscription.h"

OpenAPI_default_notification_subscription_t *OpenAPI_default_notification_subscription_create(
    OpenAPI_notification_type_e notification_type,
    char *callback_uri,
    OpenAPI_n1_message_class_e n1_message_class,
    OpenAPI_n2_information_class_e n2_information_class,
    OpenAPI_list_t *versions,
    char *binding,
    char *accepted_encoding,
    char *supported_features,
    OpenAPI_list_t* service_info_list
)
{
    OpenAPI_default_notification_subscription_t *default_notification_subscription_local_var = ogs_malloc(sizeof(OpenAPI_default_notification_subscription_t));
    ogs_assert(default_notification_subscription_local_var);

    default_notification_subscription_local_var->notification_type = notification_type;
    default_notification_subscription_local_var->callback_uri = callback_uri;
    default_notification_subscription_local_var->n1_message_class = n1_message_class;
    default_notification_subscription_local_var->n2_information_class = n2_information_class;
    default_notification_subscription_local_var->versions = versions;
    default_notification_subscription_local_var->binding = binding;
    default_notification_subscription_local_var->accepted_encoding = accepted_encoding;
    default_notification_subscription_local_var->supported_features = supported_features;
    default_notification_subscription_local_var->service_info_list = service_info_list;

    return default_notification_subscription_local_var;
}

void OpenAPI_default_notification_subscription_free(OpenAPI_default_notification_subscription_t *default_notification_subscription)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == default_notification_subscription) {
        return;
    }
    if (default_notification_subscription->callback_uri) {
        ogs_free(default_notification_subscription->callback_uri);
        default_notification_subscription->callback_uri = NULL;
    }
    if (default_notification_subscription->versions) {
        OpenAPI_list_for_each(default_notification_subscription->versions, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(default_notification_subscription->versions);
        default_notification_subscription->versions = NULL;
    }
    if (default_notification_subscription->binding) {
        ogs_free(default_notification_subscription->binding);
        default_notification_subscription->binding = NULL;
    }
    if (default_notification_subscription->accepted_encoding) {
        ogs_free(default_notification_subscription->accepted_encoding);
        default_notification_subscription->accepted_encoding = NULL;
    }
    if (default_notification_subscription->supported_features) {
        ogs_free(default_notification_subscription->supported_features);
        default_notification_subscription->supported_features = NULL;
    }
    if (default_notification_subscription->service_info_list) {
        OpenAPI_list_for_each(default_notification_subscription->service_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_def_sub_service_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(default_notification_subscription->service_info_list);
        default_notification_subscription->service_info_list = NULL;
    }
    ogs_free(default_notification_subscription);
}

cJSON *OpenAPI_default_notification_subscription_convertToJSON(OpenAPI_default_notification_subscription_t *default_notification_subscription)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (default_notification_subscription == NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [DefaultNotificationSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (default_notification_subscription->notification_type == OpenAPI_notification_type_NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [notification_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notificationType", OpenAPI_notification_type_ToString(default_notification_subscription->notification_type)) == NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [notification_type]");
        goto end;
    }

    if (!default_notification_subscription->callback_uri) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [callback_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "callbackUri", default_notification_subscription->callback_uri) == NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [callback_uri]");
        goto end;
    }

    if (default_notification_subscription->n1_message_class != OpenAPI_n1_message_class_NULL) {
    if (cJSON_AddStringToObject(item, "n1MessageClass", OpenAPI_n1_message_class_ToString(default_notification_subscription->n1_message_class)) == NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [n1_message_class]");
        goto end;
    }
    }

    if (default_notification_subscription->n2_information_class != OpenAPI_n2_information_class_NULL) {
    if (cJSON_AddStringToObject(item, "n2InformationClass", OpenAPI_n2_information_class_ToString(default_notification_subscription->n2_information_class)) == NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [n2_information_class]");
        goto end;
    }
    }

    if (default_notification_subscription->versions) {
    cJSON *versionsList = cJSON_AddArrayToObject(item, "versions");
    if (versionsList == NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [versions]");
        goto end;
    }
    OpenAPI_list_for_each(default_notification_subscription->versions, node) {
        if (cJSON_AddStringToObject(versionsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [versions]");
            goto end;
        }
    }
    }

    if (default_notification_subscription->binding) {
    if (cJSON_AddStringToObject(item, "binding", default_notification_subscription->binding) == NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [binding]");
        goto end;
    }
    }

    if (default_notification_subscription->accepted_encoding) {
    if (cJSON_AddStringToObject(item, "acceptedEncoding", default_notification_subscription->accepted_encoding) == NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [accepted_encoding]");
        goto end;
    }
    }

    if (default_notification_subscription->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", default_notification_subscription->supported_features) == NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (default_notification_subscription->service_info_list) {
    cJSON *service_info_list = cJSON_AddObjectToObject(item, "serviceInfoList");
    if (service_info_list == NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [service_info_list]");
        goto end;
    }
    cJSON *localMapObject = service_info_list;
    if (default_notification_subscription->service_info_list) {
        OpenAPI_list_for_each(default_notification_subscription->service_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [service_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [service_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_def_sub_service_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_default_notification_subscription_t *OpenAPI_default_notification_subscription_parseFromJSON(cJSON *default_notification_subscriptionJSON)
{
    OpenAPI_default_notification_subscription_t *default_notification_subscription_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notification_type = NULL;
    OpenAPI_notification_type_e notification_typeVariable = 0;
    cJSON *callback_uri = NULL;
    cJSON *n1_message_class = NULL;
    OpenAPI_n1_message_class_e n1_message_classVariable = 0;
    cJSON *n2_information_class = NULL;
    OpenAPI_n2_information_class_e n2_information_classVariable = 0;
    cJSON *versions = NULL;
    OpenAPI_list_t *versionsList = NULL;
    cJSON *binding = NULL;
    cJSON *accepted_encoding = NULL;
    cJSON *supported_features = NULL;
    cJSON *service_info_list = NULL;
    OpenAPI_list_t *service_info_listList = NULL;
    notification_type = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "notificationType");
    if (!notification_type) {
        ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [notification_type]");
        goto end;
    }
    if (!cJSON_IsString(notification_type)) {
        ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [notification_type]");
        goto end;
    }
    notification_typeVariable = OpenAPI_notification_type_FromString(notification_type->valuestring);

    callback_uri = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "callbackUri");
    if (!callback_uri) {
        ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [callback_uri]");
        goto end;
    }
    if (!cJSON_IsString(callback_uri)) {
        ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [callback_uri]");
        goto end;
    }

    n1_message_class = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "n1MessageClass");
    if (n1_message_class) {
    if (!cJSON_IsString(n1_message_class)) {
        ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [n1_message_class]");
        goto end;
    }
    n1_message_classVariable = OpenAPI_n1_message_class_FromString(n1_message_class->valuestring);
    }

    n2_information_class = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "n2InformationClass");
    if (n2_information_class) {
    if (!cJSON_IsString(n2_information_class)) {
        ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [n2_information_class]");
        goto end;
    }
    n2_information_classVariable = OpenAPI_n2_information_class_FromString(n2_information_class->valuestring);
    }

    versions = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "versions");
    if (versions) {
        cJSON *versions_local = NULL;
        if (!cJSON_IsArray(versions)) {
            ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [versions]");
            goto end;
        }

        versionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(versions_local, versions) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(versions_local)) {
                ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [versions]");
                goto end;
            }
            OpenAPI_list_add(versionsList, ogs_strdup(versions_local->valuestring));
        }
    }

    binding = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "binding");
    if (binding) {
    if (!cJSON_IsString(binding) && !cJSON_IsNull(binding)) {
        ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [binding]");
        goto end;
    }
    }

    accepted_encoding = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "acceptedEncoding");
    if (accepted_encoding) {
    if (!cJSON_IsString(accepted_encoding) && !cJSON_IsNull(accepted_encoding)) {
        ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [accepted_encoding]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    service_info_list = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "serviceInfoList");
    if (service_info_list) {
        cJSON *service_info_list_local_map = NULL;
        if (!cJSON_IsObject(service_info_list) && !cJSON_IsNull(service_info_list)) {
            ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [service_info_list]");
            goto end;
        }
        if (cJSON_IsObject(service_info_list)) {
            service_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(service_info_list_local_map, service_info_list) {
                cJSON *localMapObject = service_info_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_def_sub_service_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(service_info_listList, localMapKeyPair);
            }
        }
    }

    default_notification_subscription_local_var = OpenAPI_default_notification_subscription_create (
        notification_typeVariable,
        ogs_strdup(callback_uri->valuestring),
        n1_message_class ? n1_message_classVariable : 0,
        n2_information_class ? n2_information_classVariable : 0,
        versions ? versionsList : NULL,
        binding && !cJSON_IsNull(binding) ? ogs_strdup(binding->valuestring) : NULL,
        accepted_encoding && !cJSON_IsNull(accepted_encoding) ? ogs_strdup(accepted_encoding->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        service_info_list ? service_info_listList : NULL
    );

    return default_notification_subscription_local_var;
end:
    if (versionsList) {
        OpenAPI_list_for_each(versionsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(versionsList);
        versionsList = NULL;
    }
    if (service_info_listList) {
        OpenAPI_list_for_each(service_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_def_sub_service_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(service_info_listList);
        service_info_listList = NULL;
    }
    return NULL;
}

OpenAPI_default_notification_subscription_t *OpenAPI_default_notification_subscription_copy(OpenAPI_default_notification_subscription_t *dst, OpenAPI_default_notification_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_default_notification_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed");
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

    OpenAPI_default_notification_subscription_free(dst);
    dst = OpenAPI_default_notification_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

