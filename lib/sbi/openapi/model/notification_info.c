
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_info.h"

OpenAPI_notification_info_t *OpenAPI_notification_info_create(
    char *notif_id,
    char *notif_uri,
    bool is_up_buffer_ind,
    int up_buffer_ind
)
{
    OpenAPI_notification_info_t *notification_info_local_var = ogs_malloc(sizeof(OpenAPI_notification_info_t));
    ogs_assert(notification_info_local_var);

    notification_info_local_var->notif_id = notif_id;
    notification_info_local_var->notif_uri = notif_uri;
    notification_info_local_var->is_up_buffer_ind = is_up_buffer_ind;
    notification_info_local_var->up_buffer_ind = up_buffer_ind;

    return notification_info_local_var;
}

void OpenAPI_notification_info_free(OpenAPI_notification_info_t *notification_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == notification_info) {
        return;
    }
    if (notification_info->notif_id) {
        ogs_free(notification_info->notif_id);
        notification_info->notif_id = NULL;
    }
    if (notification_info->notif_uri) {
        ogs_free(notification_info->notif_uri);
        notification_info->notif_uri = NULL;
    }
    ogs_free(notification_info);
}

cJSON *OpenAPI_notification_info_convertToJSON(OpenAPI_notification_info_t *notification_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (notification_info == NULL) {
        ogs_error("OpenAPI_notification_info_convertToJSON() failed [NotificationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!notification_info->notif_id) {
        ogs_error("OpenAPI_notification_info_convertToJSON() failed [notif_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notifId", notification_info->notif_id) == NULL) {
        ogs_error("OpenAPI_notification_info_convertToJSON() failed [notif_id]");
        goto end;
    }

    if (!notification_info->notif_uri) {
        ogs_error("OpenAPI_notification_info_convertToJSON() failed [notif_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notifUri", notification_info->notif_uri) == NULL) {
        ogs_error("OpenAPI_notification_info_convertToJSON() failed [notif_uri]");
        goto end;
    }

    if (notification_info->is_up_buffer_ind) {
    if (cJSON_AddBoolToObject(item, "upBufferInd", notification_info->up_buffer_ind) == NULL) {
        ogs_error("OpenAPI_notification_info_convertToJSON() failed [up_buffer_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_notification_info_t *OpenAPI_notification_info_parseFromJSON(cJSON *notification_infoJSON)
{
    OpenAPI_notification_info_t *notification_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notif_id = NULL;
    cJSON *notif_uri = NULL;
    cJSON *up_buffer_ind = NULL;
    notif_id = cJSON_GetObjectItemCaseSensitive(notification_infoJSON, "notifId");
    if (!notif_id) {
        ogs_error("OpenAPI_notification_info_parseFromJSON() failed [notif_id]");
        goto end;
    }
    if (!cJSON_IsString(notif_id)) {
        ogs_error("OpenAPI_notification_info_parseFromJSON() failed [notif_id]");
        goto end;
    }

    notif_uri = cJSON_GetObjectItemCaseSensitive(notification_infoJSON, "notifUri");
    if (!notif_uri) {
        ogs_error("OpenAPI_notification_info_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    if (!cJSON_IsString(notif_uri)) {
        ogs_error("OpenAPI_notification_info_parseFromJSON() failed [notif_uri]");
        goto end;
    }

    up_buffer_ind = cJSON_GetObjectItemCaseSensitive(notification_infoJSON, "upBufferInd");
    if (up_buffer_ind) {
    if (!cJSON_IsBool(up_buffer_ind)) {
        ogs_error("OpenAPI_notification_info_parseFromJSON() failed [up_buffer_ind]");
        goto end;
    }
    }

    notification_info_local_var = OpenAPI_notification_info_create (
        ogs_strdup(notif_id->valuestring),
        ogs_strdup(notif_uri->valuestring),
        up_buffer_ind ? true : false,
        up_buffer_ind ? up_buffer_ind->valueint : 0
    );

    return notification_info_local_var;
end:
    return NULL;
}

OpenAPI_notification_info_t *OpenAPI_notification_info_copy(OpenAPI_notification_info_t *dst, OpenAPI_notification_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_notification_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_notification_info_convertToJSON() failed");
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

    OpenAPI_notification_info_free(dst);
    dst = OpenAPI_notification_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

