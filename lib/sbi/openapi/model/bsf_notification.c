
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsf_notification.h"

OpenAPI_bsf_notification_t *OpenAPI_bsf_notification_create(
    char *notif_corre_id,
    char *pcf_id,
    char *pcf_set_id,
    OpenAPI_binding_level_e bind_level,
    OpenAPI_list_t *event_notifs
)
{
    OpenAPI_bsf_notification_t *bsf_notification_local_var = ogs_malloc(sizeof(OpenAPI_bsf_notification_t));
    ogs_assert(bsf_notification_local_var);

    bsf_notification_local_var->notif_corre_id = notif_corre_id;
    bsf_notification_local_var->pcf_id = pcf_id;
    bsf_notification_local_var->pcf_set_id = pcf_set_id;
    bsf_notification_local_var->bind_level = bind_level;
    bsf_notification_local_var->event_notifs = event_notifs;

    return bsf_notification_local_var;
}

void OpenAPI_bsf_notification_free(OpenAPI_bsf_notification_t *bsf_notification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == bsf_notification) {
        return;
    }
    if (bsf_notification->notif_corre_id) {
        ogs_free(bsf_notification->notif_corre_id);
        bsf_notification->notif_corre_id = NULL;
    }
    if (bsf_notification->pcf_id) {
        ogs_free(bsf_notification->pcf_id);
        bsf_notification->pcf_id = NULL;
    }
    if (bsf_notification->pcf_set_id) {
        ogs_free(bsf_notification->pcf_set_id);
        bsf_notification->pcf_set_id = NULL;
    }
    if (bsf_notification->event_notifs) {
        OpenAPI_list_for_each(bsf_notification->event_notifs, node) {
            OpenAPI_bsf_event_notification_free(node->data);
        }
        OpenAPI_list_free(bsf_notification->event_notifs);
        bsf_notification->event_notifs = NULL;
    }
    ogs_free(bsf_notification);
}

cJSON *OpenAPI_bsf_notification_convertToJSON(OpenAPI_bsf_notification_t *bsf_notification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (bsf_notification == NULL) {
        ogs_error("OpenAPI_bsf_notification_convertToJSON() failed [BsfNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!bsf_notification->notif_corre_id) {
        ogs_error("OpenAPI_bsf_notification_convertToJSON() failed [notif_corre_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notifCorreId", bsf_notification->notif_corre_id) == NULL) {
        ogs_error("OpenAPI_bsf_notification_convertToJSON() failed [notif_corre_id]");
        goto end;
    }

    if (bsf_notification->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", bsf_notification->pcf_id) == NULL) {
        ogs_error("OpenAPI_bsf_notification_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (bsf_notification->pcf_set_id) {
    if (cJSON_AddStringToObject(item, "pcfSetId", bsf_notification->pcf_set_id) == NULL) {
        ogs_error("OpenAPI_bsf_notification_convertToJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    if (bsf_notification->bind_level != OpenAPI_binding_level_NULL) {
    if (cJSON_AddStringToObject(item, "bindLevel", OpenAPI_binding_level_ToString(bsf_notification->bind_level)) == NULL) {
        ogs_error("OpenAPI_bsf_notification_convertToJSON() failed [bind_level]");
        goto end;
    }
    }

    if (!bsf_notification->event_notifs) {
        ogs_error("OpenAPI_bsf_notification_convertToJSON() failed [event_notifs]");
        return NULL;
    }
    cJSON *event_notifsList = cJSON_AddArrayToObject(item, "eventNotifs");
    if (event_notifsList == NULL) {
        ogs_error("OpenAPI_bsf_notification_convertToJSON() failed [event_notifs]");
        goto end;
    }
    OpenAPI_list_for_each(bsf_notification->event_notifs, node) {
        cJSON *itemLocal = OpenAPI_bsf_event_notification_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_bsf_notification_convertToJSON() failed [event_notifs]");
            goto end;
        }
        cJSON_AddItemToArray(event_notifsList, itemLocal);
    }

end:
    return item;
}

OpenAPI_bsf_notification_t *OpenAPI_bsf_notification_parseFromJSON(cJSON *bsf_notificationJSON)
{
    OpenAPI_bsf_notification_t *bsf_notification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notif_corre_id = NULL;
    cJSON *pcf_id = NULL;
    cJSON *pcf_set_id = NULL;
    cJSON *bind_level = NULL;
    OpenAPI_binding_level_e bind_levelVariable = 0;
    cJSON *event_notifs = NULL;
    OpenAPI_list_t *event_notifsList = NULL;
    notif_corre_id = cJSON_GetObjectItemCaseSensitive(bsf_notificationJSON, "notifCorreId");
    if (!notif_corre_id) {
        ogs_error("OpenAPI_bsf_notification_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }
    if (!cJSON_IsString(notif_corre_id)) {
        ogs_error("OpenAPI_bsf_notification_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }

    pcf_id = cJSON_GetObjectItemCaseSensitive(bsf_notificationJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_bsf_notification_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_set_id = cJSON_GetObjectItemCaseSensitive(bsf_notificationJSON, "pcfSetId");
    if (pcf_set_id) {
    if (!cJSON_IsString(pcf_set_id) && !cJSON_IsNull(pcf_set_id)) {
        ogs_error("OpenAPI_bsf_notification_parseFromJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    bind_level = cJSON_GetObjectItemCaseSensitive(bsf_notificationJSON, "bindLevel");
    if (bind_level) {
    if (!cJSON_IsString(bind_level)) {
        ogs_error("OpenAPI_bsf_notification_parseFromJSON() failed [bind_level]");
        goto end;
    }
    bind_levelVariable = OpenAPI_binding_level_FromString(bind_level->valuestring);
    }

    event_notifs = cJSON_GetObjectItemCaseSensitive(bsf_notificationJSON, "eventNotifs");
    if (!event_notifs) {
        ogs_error("OpenAPI_bsf_notification_parseFromJSON() failed [event_notifs]");
        goto end;
    }
        cJSON *event_notifs_local = NULL;
        if (!cJSON_IsArray(event_notifs)) {
            ogs_error("OpenAPI_bsf_notification_parseFromJSON() failed [event_notifs]");
            goto end;
        }

        event_notifsList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_notifs_local, event_notifs) {
            if (!cJSON_IsObject(event_notifs_local)) {
                ogs_error("OpenAPI_bsf_notification_parseFromJSON() failed [event_notifs]");
                goto end;
            }
            OpenAPI_bsf_event_notification_t *event_notifsItem = OpenAPI_bsf_event_notification_parseFromJSON(event_notifs_local);
            if (!event_notifsItem) {
                ogs_error("No event_notifsItem");
                goto end;
            }
            OpenAPI_list_add(event_notifsList, event_notifsItem);
        }

    bsf_notification_local_var = OpenAPI_bsf_notification_create (
        ogs_strdup(notif_corre_id->valuestring),
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_set_id && !cJSON_IsNull(pcf_set_id) ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        bind_level ? bind_levelVariable : 0,
        event_notifsList
    );

    return bsf_notification_local_var;
end:
    if (event_notifsList) {
        OpenAPI_list_for_each(event_notifsList, node) {
            OpenAPI_bsf_event_notification_free(node->data);
        }
        OpenAPI_list_free(event_notifsList);
        event_notifsList = NULL;
    }
    return NULL;
}

OpenAPI_bsf_notification_t *OpenAPI_bsf_notification_copy(OpenAPI_bsf_notification_t *dst, OpenAPI_bsf_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bsf_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bsf_notification_convertToJSON() failed");
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

    OpenAPI_bsf_notification_free(dst);
    dst = OpenAPI_bsf_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

