
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n1_n2_msg_txfr_failure_notification.h"

OpenAPI_n1_n2_msg_txfr_failure_notification_t *OpenAPI_n1_n2_msg_txfr_failure_notification_create(
    OpenAPI_n1_n2_message_transfer_cause_e cause,
    char *n1n2_msg_data_uri
)
{
    OpenAPI_n1_n2_msg_txfr_failure_notification_t *n1_n2_msg_txfr_failure_notification_local_var = ogs_malloc(sizeof(OpenAPI_n1_n2_msg_txfr_failure_notification_t));
    ogs_assert(n1_n2_msg_txfr_failure_notification_local_var);

    n1_n2_msg_txfr_failure_notification_local_var->cause = cause;
    n1_n2_msg_txfr_failure_notification_local_var->n1n2_msg_data_uri = n1n2_msg_data_uri;

    return n1_n2_msg_txfr_failure_notification_local_var;
}

void OpenAPI_n1_n2_msg_txfr_failure_notification_free(OpenAPI_n1_n2_msg_txfr_failure_notification_t *n1_n2_msg_txfr_failure_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n1_n2_msg_txfr_failure_notification) {
        return;
    }
    if (n1_n2_msg_txfr_failure_notification->n1n2_msg_data_uri) {
        ogs_free(n1_n2_msg_txfr_failure_notification->n1n2_msg_data_uri);
        n1_n2_msg_txfr_failure_notification->n1n2_msg_data_uri = NULL;
    }
    ogs_free(n1_n2_msg_txfr_failure_notification);
}

cJSON *OpenAPI_n1_n2_msg_txfr_failure_notification_convertToJSON(OpenAPI_n1_n2_msg_txfr_failure_notification_t *n1_n2_msg_txfr_failure_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n1_n2_msg_txfr_failure_notification == NULL) {
        ogs_error("OpenAPI_n1_n2_msg_txfr_failure_notification_convertToJSON() failed [N1N2MsgTxfrFailureNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n1_n2_msg_txfr_failure_notification->cause == OpenAPI_n1_n2_message_transfer_cause_NULL) {
        ogs_error("OpenAPI_n1_n2_msg_txfr_failure_notification_convertToJSON() failed [cause]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "cause", OpenAPI_n1_n2_message_transfer_cause_ToString(n1_n2_msg_txfr_failure_notification->cause)) == NULL) {
        ogs_error("OpenAPI_n1_n2_msg_txfr_failure_notification_convertToJSON() failed [cause]");
        goto end;
    }

    if (!n1_n2_msg_txfr_failure_notification->n1n2_msg_data_uri) {
        ogs_error("OpenAPI_n1_n2_msg_txfr_failure_notification_convertToJSON() failed [n1n2_msg_data_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "n1n2MsgDataUri", n1_n2_msg_txfr_failure_notification->n1n2_msg_data_uri) == NULL) {
        ogs_error("OpenAPI_n1_n2_msg_txfr_failure_notification_convertToJSON() failed [n1n2_msg_data_uri]");
        goto end;
    }

end:
    return item;
}

OpenAPI_n1_n2_msg_txfr_failure_notification_t *OpenAPI_n1_n2_msg_txfr_failure_notification_parseFromJSON(cJSON *n1_n2_msg_txfr_failure_notificationJSON)
{
    OpenAPI_n1_n2_msg_txfr_failure_notification_t *n1_n2_msg_txfr_failure_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cause = NULL;
    OpenAPI_n1_n2_message_transfer_cause_e causeVariable = 0;
    cJSON *n1n2_msg_data_uri = NULL;
    cause = cJSON_GetObjectItemCaseSensitive(n1_n2_msg_txfr_failure_notificationJSON, "cause");
    if (!cause) {
        ogs_error("OpenAPI_n1_n2_msg_txfr_failure_notification_parseFromJSON() failed [cause]");
        goto end;
    }
    if (!cJSON_IsString(cause)) {
        ogs_error("OpenAPI_n1_n2_msg_txfr_failure_notification_parseFromJSON() failed [cause]");
        goto end;
    }
    causeVariable = OpenAPI_n1_n2_message_transfer_cause_FromString(cause->valuestring);

    n1n2_msg_data_uri = cJSON_GetObjectItemCaseSensitive(n1_n2_msg_txfr_failure_notificationJSON, "n1n2MsgDataUri");
    if (!n1n2_msg_data_uri) {
        ogs_error("OpenAPI_n1_n2_msg_txfr_failure_notification_parseFromJSON() failed [n1n2_msg_data_uri]");
        goto end;
    }
    if (!cJSON_IsString(n1n2_msg_data_uri)) {
        ogs_error("OpenAPI_n1_n2_msg_txfr_failure_notification_parseFromJSON() failed [n1n2_msg_data_uri]");
        goto end;
    }

    n1_n2_msg_txfr_failure_notification_local_var = OpenAPI_n1_n2_msg_txfr_failure_notification_create (
        causeVariable,
        ogs_strdup(n1n2_msg_data_uri->valuestring)
    );

    return n1_n2_msg_txfr_failure_notification_local_var;
end:
    return NULL;
}

OpenAPI_n1_n2_msg_txfr_failure_notification_t *OpenAPI_n1_n2_msg_txfr_failure_notification_copy(OpenAPI_n1_n2_msg_txfr_failure_notification_t *dst, OpenAPI_n1_n2_msg_txfr_failure_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n1_n2_msg_txfr_failure_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n1_n2_msg_txfr_failure_notification_convertToJSON() failed");
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

    OpenAPI_n1_n2_msg_txfr_failure_notification_free(dst);
    dst = OpenAPI_n1_n2_msg_txfr_failure_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

