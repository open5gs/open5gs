
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_notify_item.h"

OpenAPI_pdu_session_notify_item_t *OpenAPI_pdu_session_notify_item_create(
    OpenAPI_notification_cause_e notification_cause
)
{
    OpenAPI_pdu_session_notify_item_t *pdu_session_notify_item_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_notify_item_t));
    ogs_assert(pdu_session_notify_item_local_var);

    pdu_session_notify_item_local_var->notification_cause = notification_cause;

    return pdu_session_notify_item_local_var;
}

void OpenAPI_pdu_session_notify_item_free(OpenAPI_pdu_session_notify_item_t *pdu_session_notify_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_notify_item) {
        return;
    }
    ogs_free(pdu_session_notify_item);
}

cJSON *OpenAPI_pdu_session_notify_item_convertToJSON(OpenAPI_pdu_session_notify_item_t *pdu_session_notify_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_notify_item == NULL) {
        ogs_error("OpenAPI_pdu_session_notify_item_convertToJSON() failed [PduSessionNotifyItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_session_notify_item->notification_cause == OpenAPI_notification_cause_NULL) {
        ogs_error("OpenAPI_pdu_session_notify_item_convertToJSON() failed [notification_cause]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notificationCause", OpenAPI_notification_cause_ToString(pdu_session_notify_item->notification_cause)) == NULL) {
        ogs_error("OpenAPI_pdu_session_notify_item_convertToJSON() failed [notification_cause]");
        goto end;
    }

end:
    return item;
}

OpenAPI_pdu_session_notify_item_t *OpenAPI_pdu_session_notify_item_parseFromJSON(cJSON *pdu_session_notify_itemJSON)
{
    OpenAPI_pdu_session_notify_item_t *pdu_session_notify_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notification_cause = NULL;
    OpenAPI_notification_cause_e notification_causeVariable = 0;
    notification_cause = cJSON_GetObjectItemCaseSensitive(pdu_session_notify_itemJSON, "notificationCause");
    if (!notification_cause) {
        ogs_error("OpenAPI_pdu_session_notify_item_parseFromJSON() failed [notification_cause]");
        goto end;
    }
    if (!cJSON_IsString(notification_cause)) {
        ogs_error("OpenAPI_pdu_session_notify_item_parseFromJSON() failed [notification_cause]");
        goto end;
    }
    notification_causeVariable = OpenAPI_notification_cause_FromString(notification_cause->valuestring);

    pdu_session_notify_item_local_var = OpenAPI_pdu_session_notify_item_create (
        notification_causeVariable
    );

    return pdu_session_notify_item_local_var;
end:
    return NULL;
}

OpenAPI_pdu_session_notify_item_t *OpenAPI_pdu_session_notify_item_copy(OpenAPI_pdu_session_notify_item_t *dst, OpenAPI_pdu_session_notify_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_notify_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_notify_item_convertToJSON() failed");
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

    OpenAPI_pdu_session_notify_item_free(dst);
    dst = OpenAPI_pdu_session_notify_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

