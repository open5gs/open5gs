
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_method.h"

OpenAPI_notification_method_t *OpenAPI_notification_method_create(
)
{
    OpenAPI_notification_method_t *notification_method_local_var = ogs_malloc(sizeof(OpenAPI_notification_method_t));
    ogs_assert(notification_method_local_var);


    return notification_method_local_var;
}

void OpenAPI_notification_method_free(OpenAPI_notification_method_t *notification_method)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == notification_method) {
        return;
    }
    ogs_free(notification_method);
}

cJSON *OpenAPI_notification_method_convertToJSON(OpenAPI_notification_method_t *notification_method)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (notification_method == NULL) {
        ogs_error("OpenAPI_notification_method_convertToJSON() failed [NotificationMethod]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_notification_method_t *OpenAPI_notification_method_parseFromJSON(cJSON *notification_methodJSON)
{
    OpenAPI_notification_method_t *notification_method_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    notification_method_local_var = OpenAPI_notification_method_create (
    );

    return notification_method_local_var;
end:
    return NULL;
}

OpenAPI_notification_method_t *OpenAPI_notification_method_copy(OpenAPI_notification_method_t *dst, OpenAPI_notification_method_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_notification_method_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_notification_method_convertToJSON() failed");
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

    OpenAPI_notification_method_free(dst);
    dst = OpenAPI_notification_method_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

