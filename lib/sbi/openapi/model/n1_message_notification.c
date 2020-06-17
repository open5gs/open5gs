
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n1_message_notification.h"

OpenAPI_n1_message_notification_t *OpenAPI_n1_message_notification_create(
    char *n1_notify_subscription_id,
    OpenAPI_n1_message_container_t *n1_message_container,
    char *lcs_correlation_id,
    OpenAPI_registration_context_container_t *registration_ctxt_container,
    char *new_lmf_identification
    )
{
    OpenAPI_n1_message_notification_t *n1_message_notification_local_var = OpenAPI_malloc(sizeof(OpenAPI_n1_message_notification_t));
    if (!n1_message_notification_local_var) {
        return NULL;
    }
    n1_message_notification_local_var->n1_notify_subscription_id = n1_notify_subscription_id;
    n1_message_notification_local_var->n1_message_container = n1_message_container;
    n1_message_notification_local_var->lcs_correlation_id = lcs_correlation_id;
    n1_message_notification_local_var->registration_ctxt_container = registration_ctxt_container;
    n1_message_notification_local_var->new_lmf_identification = new_lmf_identification;

    return n1_message_notification_local_var;
}

void OpenAPI_n1_message_notification_free(OpenAPI_n1_message_notification_t *n1_message_notification)
{
    if (NULL == n1_message_notification) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(n1_message_notification->n1_notify_subscription_id);
    OpenAPI_n1_message_container_free(n1_message_notification->n1_message_container);
    ogs_free(n1_message_notification->lcs_correlation_id);
    OpenAPI_registration_context_container_free(n1_message_notification->registration_ctxt_container);
    ogs_free(n1_message_notification->new_lmf_identification);
    ogs_free(n1_message_notification);
}

cJSON *OpenAPI_n1_message_notification_convertToJSON(OpenAPI_n1_message_notification_t *n1_message_notification)
{
    cJSON *item = NULL;

    if (n1_message_notification == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [N1MessageNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n1_message_notification->n1_notify_subscription_id) {
        if (cJSON_AddStringToObject(item, "n1NotifySubscriptionId", n1_message_notification->n1_notify_subscription_id) == NULL) {
            ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [n1_notify_subscription_id]");
            goto end;
        }
    }

    if (!n1_message_notification->n1_message_container) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [n1_message_container]");
        goto end;
    }
    cJSON *n1_message_container_local_JSON = OpenAPI_n1_message_container_convertToJSON(n1_message_notification->n1_message_container);
    if (n1_message_container_local_JSON == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [n1_message_container]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n1MessageContainer", n1_message_container_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [n1_message_container]");
        goto end;
    }

    if (n1_message_notification->lcs_correlation_id) {
        if (cJSON_AddStringToObject(item, "lcsCorrelationId", n1_message_notification->lcs_correlation_id) == NULL) {
            ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [lcs_correlation_id]");
            goto end;
        }
    }

    if (n1_message_notification->registration_ctxt_container) {
        cJSON *registration_ctxt_container_local_JSON = OpenAPI_registration_context_container_convertToJSON(n1_message_notification->registration_ctxt_container);
        if (registration_ctxt_container_local_JSON == NULL) {
            ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [registration_ctxt_container]");
            goto end;
        }
        cJSON_AddItemToObject(item, "registrationCtxtContainer", registration_ctxt_container_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [registration_ctxt_container]");
            goto end;
        }
    }

    if (n1_message_notification->new_lmf_identification) {
        if (cJSON_AddStringToObject(item, "newLmfIdentification", n1_message_notification->new_lmf_identification) == NULL) {
            ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed [new_lmf_identification]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_n1_message_notification_t *OpenAPI_n1_message_notification_parseFromJSON(cJSON *n1_message_notificationJSON)
{
    OpenAPI_n1_message_notification_t *n1_message_notification_local_var = NULL;
    cJSON *n1_notify_subscription_id = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "n1NotifySubscriptionId");

    if (n1_notify_subscription_id) {
        if (!cJSON_IsString(n1_notify_subscription_id)) {
            ogs_error("OpenAPI_n1_message_notification_parseFromJSON() failed [n1_notify_subscription_id]");
            goto end;
        }
    }

    cJSON *n1_message_container = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "n1MessageContainer");
    if (!n1_message_container) {
        ogs_error("OpenAPI_n1_message_notification_parseFromJSON() failed [n1_message_container]");
        goto end;
    }

    OpenAPI_n1_message_container_t *n1_message_container_local_nonprim = NULL;

    n1_message_container_local_nonprim = OpenAPI_n1_message_container_parseFromJSON(n1_message_container);

    cJSON *lcs_correlation_id = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "lcsCorrelationId");

    if (lcs_correlation_id) {
        if (!cJSON_IsString(lcs_correlation_id)) {
            ogs_error("OpenAPI_n1_message_notification_parseFromJSON() failed [lcs_correlation_id]");
            goto end;
        }
    }

    cJSON *registration_ctxt_container = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "registrationCtxtContainer");

    OpenAPI_registration_context_container_t *registration_ctxt_container_local_nonprim = NULL;
    if (registration_ctxt_container) {
        registration_ctxt_container_local_nonprim = OpenAPI_registration_context_container_parseFromJSON(registration_ctxt_container);
    }

    cJSON *new_lmf_identification = cJSON_GetObjectItemCaseSensitive(n1_message_notificationJSON, "newLmfIdentification");

    if (new_lmf_identification) {
        if (!cJSON_IsString(new_lmf_identification)) {
            ogs_error("OpenAPI_n1_message_notification_parseFromJSON() failed [new_lmf_identification]");
            goto end;
        }
    }

    n1_message_notification_local_var = OpenAPI_n1_message_notification_create (
        n1_notify_subscription_id ? ogs_strdup(n1_notify_subscription_id->valuestring) : NULL,
        n1_message_container_local_nonprim,
        lcs_correlation_id ? ogs_strdup(lcs_correlation_id->valuestring) : NULL,
        registration_ctxt_container ? registration_ctxt_container_local_nonprim : NULL,
        new_lmf_identification ? ogs_strdup(new_lmf_identification->valuestring) : NULL
        );

    return n1_message_notification_local_var;
end:
    return NULL;
}

OpenAPI_n1_message_notification_t *OpenAPI_n1_message_notification_copy(OpenAPI_n1_message_notification_t *dst, OpenAPI_n1_message_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n1_message_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n1_message_notification_convertToJSON() failed");
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

    OpenAPI_n1_message_notification_free(dst);
    dst = OpenAPI_n1_message_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

