
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_control.h"

OpenAPI_notification_control_t *OpenAPI_notification_control_create(
    )
{
    OpenAPI_notification_control_t *notification_control_local_var = OpenAPI_malloc(sizeof(OpenAPI_notification_control_t));
    if (!notification_control_local_var) {
        return NULL;
    }

    return notification_control_local_var;
}

void OpenAPI_notification_control_free(OpenAPI_notification_control_t *notification_control)
{
    if (NULL == notification_control) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(notification_control);
}

cJSON *OpenAPI_notification_control_convertToJSON(OpenAPI_notification_control_t *notification_control)
{
    cJSON *item = NULL;

    if (notification_control == NULL) {
        ogs_error("OpenAPI_notification_control_convertToJSON() failed [NotificationControl]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_notification_control_t *OpenAPI_notification_control_parseFromJSON(cJSON *notification_controlJSON)
{
    OpenAPI_notification_control_t *notification_control_local_var = NULL;
    notification_control_local_var = OpenAPI_notification_control_create (
        );

    return notification_control_local_var;
end:
    return NULL;
}

OpenAPI_notification_control_t *OpenAPI_notification_control_copy(OpenAPI_notification_control_t *dst, OpenAPI_notification_control_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_notification_control_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_notification_control_convertToJSON() failed");
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

    OpenAPI_notification_control_free(dst);
    dst = OpenAPI_notification_control_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

