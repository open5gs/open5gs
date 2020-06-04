
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "default_notification_subscription.h"

OpenAPI_default_notification_subscription_t *OpenAPI_default_notification_subscription_create(
    OpenAPI_notification_type_e notification_type,
    char *callback_uri,
    OpenAPI_n1_message_class_e n1_message_class,
    OpenAPI_n2_information_class_t *n2_information_class,
    OpenAPI_list_t *versions
    )
{
    OpenAPI_default_notification_subscription_t *default_notification_subscription_local_var = OpenAPI_malloc(sizeof(OpenAPI_default_notification_subscription_t));
    if (!default_notification_subscription_local_var) {
        return NULL;
    }
    default_notification_subscription_local_var->notification_type = notification_type;
    default_notification_subscription_local_var->callback_uri = callback_uri;
    default_notification_subscription_local_var->n1_message_class = n1_message_class;
    default_notification_subscription_local_var->n2_information_class = n2_information_class;
    default_notification_subscription_local_var->versions = versions;

    return default_notification_subscription_local_var;
}

void OpenAPI_default_notification_subscription_free(OpenAPI_default_notification_subscription_t *default_notification_subscription)
{
    if (NULL == default_notification_subscription) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(default_notification_subscription->callback_uri);
    OpenAPI_n2_information_class_free(default_notification_subscription->n2_information_class);
    OpenAPI_list_for_each(default_notification_subscription->versions, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(default_notification_subscription->versions);
    ogs_free(default_notification_subscription);
}

cJSON *OpenAPI_default_notification_subscription_convertToJSON(OpenAPI_default_notification_subscription_t *default_notification_subscription)
{
    cJSON *item = NULL;

    if (default_notification_subscription == NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [DefaultNotificationSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!default_notification_subscription->notification_type) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [notification_type]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "notificationType", OpenAPI_notification_type_ToString(default_notification_subscription->notification_type)) == NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [notification_type]");
        goto end;
    }

    if (!default_notification_subscription->callback_uri) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [callback_uri]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "callbackUri", default_notification_subscription->callback_uri) == NULL) {
        ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [callback_uri]");
        goto end;
    }

    if (default_notification_subscription->n1_message_class) {
        if (cJSON_AddStringToObject(item, "n1MessageClass", OpenAPI_n1_message_class_ToString(default_notification_subscription->n1_message_class)) == NULL) {
            ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [n1_message_class]");
            goto end;
        }
    }

    if (default_notification_subscription->n2_information_class) {
        cJSON *n2_information_class_local_JSON = OpenAPI_n2_information_class_convertToJSON(default_notification_subscription->n2_information_class);
        if (n2_information_class_local_JSON == NULL) {
            ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [n2_information_class]");
            goto end;
        }
        cJSON_AddItemToObject(item, "n2InformationClass", n2_information_class_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [n2_information_class]");
            goto end;
        }
    }

    if (default_notification_subscription->versions) {
        cJSON *versions = cJSON_AddArrayToObject(item, "versions");
        if (versions == NULL) {
            ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [versions]");
            goto end;
        }

        OpenAPI_lnode_t *versions_node;
        OpenAPI_list_for_each(default_notification_subscription->versions, versions_node)  {
            if (cJSON_AddStringToObject(versions, "", (char*)versions_node->data) == NULL) {
                ogs_error("OpenAPI_default_notification_subscription_convertToJSON() failed [versions]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_default_notification_subscription_t *OpenAPI_default_notification_subscription_parseFromJSON(cJSON *default_notification_subscriptionJSON)
{
    OpenAPI_default_notification_subscription_t *default_notification_subscription_local_var = NULL;
    cJSON *notification_type = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "notificationType");
    if (!notification_type) {
        ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [notification_type]");
        goto end;
    }

    OpenAPI_notification_type_e notification_typeVariable;

    if (!cJSON_IsString(notification_type)) {
        ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [notification_type]");
        goto end;
    }
    notification_typeVariable = OpenAPI_notification_type_FromString(notification_type->valuestring);

    cJSON *callback_uri = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "callbackUri");
    if (!callback_uri) {
        ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [callback_uri]");
        goto end;
    }


    if (!cJSON_IsString(callback_uri)) {
        ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [callback_uri]");
        goto end;
    }

    cJSON *n1_message_class = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "n1MessageClass");

    OpenAPI_n1_message_class_e n1_message_classVariable;
    if (n1_message_class) {
        if (!cJSON_IsString(n1_message_class)) {
            ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [n1_message_class]");
            goto end;
        }
        n1_message_classVariable = OpenAPI_n1_message_class_FromString(n1_message_class->valuestring);
    }

    cJSON *n2_information_class = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "n2InformationClass");

    OpenAPI_n2_information_class_t *n2_information_class_local_nonprim = NULL;
    if (n2_information_class) {
        n2_information_class_local_nonprim = OpenAPI_n2_information_class_parseFromJSON(n2_information_class);
    }

    cJSON *versions = cJSON_GetObjectItemCaseSensitive(default_notification_subscriptionJSON, "versions");

    OpenAPI_list_t *versionsList;
    if (versions) {
        cJSON *versions_local;
        if (!cJSON_IsArray(versions)) {
            ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [versions]");
            goto end;
        }
        versionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(versions_local, versions) {
            if (!cJSON_IsString(versions_local)) {
                ogs_error("OpenAPI_default_notification_subscription_parseFromJSON() failed [versions]");
                goto end;
            }
            OpenAPI_list_add(versionsList, ogs_strdup(versions_local->valuestring));
        }
    }

    default_notification_subscription_local_var = OpenAPI_default_notification_subscription_create (
        notification_typeVariable,
        ogs_strdup(callback_uri->valuestring),
        n1_message_class ? n1_message_classVariable : 0,
        n2_information_class ? n2_information_class_local_nonprim : NULL,
        versions ? versionsList : NULL
        );

    return default_notification_subscription_local_var;
end:
    return NULL;
}

