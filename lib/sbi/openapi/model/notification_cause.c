
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_cause.h"

OpenAPI_notification_cause_t *OpenAPI_notification_cause_create(
    )
{
    OpenAPI_notification_cause_t *notification_cause_local_var = OpenAPI_malloc(sizeof(OpenAPI_notification_cause_t));
    if (!notification_cause_local_var) {
        return NULL;
    }

    return notification_cause_local_var;
}

void OpenAPI_notification_cause_free(OpenAPI_notification_cause_t *notification_cause)
{
    if (NULL == notification_cause) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(notification_cause);
}

cJSON *OpenAPI_notification_cause_convertToJSON(OpenAPI_notification_cause_t *notification_cause)
{
    cJSON *item = NULL;

    if (notification_cause == NULL) {
        ogs_error("OpenAPI_notification_cause_convertToJSON() failed [NotificationCause]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_notification_cause_t *OpenAPI_notification_cause_parseFromJSON(cJSON *notification_causeJSON)
{
    OpenAPI_notification_cause_t *notification_cause_local_var = NULL;
    notification_cause_local_var = OpenAPI_notification_cause_create (
        );

    return notification_cause_local_var;
end:
    return NULL;
}

OpenAPI_notification_cause_t *OpenAPI_notification_cause_copy(OpenAPI_notification_cause_t *dst, OpenAPI_notification_cause_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_notification_cause_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_notification_cause_convertToJSON() failed");
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

    OpenAPI_notification_cause_free(dst);
    dst = OpenAPI_notification_cause_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

