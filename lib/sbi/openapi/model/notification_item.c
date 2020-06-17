
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_item.h"

OpenAPI_notification_item_t *OpenAPI_notification_item_create(
    char *resource_id,
    OpenAPI_list_t *notif_items
    )
{
    OpenAPI_notification_item_t *notification_item_local_var = OpenAPI_malloc(sizeof(OpenAPI_notification_item_t));
    if (!notification_item_local_var) {
        return NULL;
    }
    notification_item_local_var->resource_id = resource_id;
    notification_item_local_var->notif_items = notif_items;

    return notification_item_local_var;
}

void OpenAPI_notification_item_free(OpenAPI_notification_item_t *notification_item)
{
    if (NULL == notification_item) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(notification_item->resource_id);
    OpenAPI_list_for_each(notification_item->notif_items, node) {
        OpenAPI_updated_item_free(node->data);
    }
    OpenAPI_list_free(notification_item->notif_items);
    ogs_free(notification_item);
}

cJSON *OpenAPI_notification_item_convertToJSON(OpenAPI_notification_item_t *notification_item)
{
    cJSON *item = NULL;

    if (notification_item == NULL) {
        ogs_error("OpenAPI_notification_item_convertToJSON() failed [NotificationItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!notification_item->resource_id) {
        ogs_error("OpenAPI_notification_item_convertToJSON() failed [resource_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "resourceId", notification_item->resource_id) == NULL) {
        ogs_error("OpenAPI_notification_item_convertToJSON() failed [resource_id]");
        goto end;
    }

    if (!notification_item->notif_items) {
        ogs_error("OpenAPI_notification_item_convertToJSON() failed [notif_items]");
        goto end;
    }
    cJSON *notif_itemsList = cJSON_AddArrayToObject(item, "notifItems");
    if (notif_itemsList == NULL) {
        ogs_error("OpenAPI_notification_item_convertToJSON() failed [notif_items]");
        goto end;
    }

    OpenAPI_lnode_t *notif_items_node;
    if (notification_item->notif_items) {
        OpenAPI_list_for_each(notification_item->notif_items, notif_items_node) {
            cJSON *itemLocal = OpenAPI_updated_item_convertToJSON(notif_items_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_notification_item_convertToJSON() failed [notif_items]");
                goto end;
            }
            cJSON_AddItemToArray(notif_itemsList, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_notification_item_t *OpenAPI_notification_item_parseFromJSON(cJSON *notification_itemJSON)
{
    OpenAPI_notification_item_t *notification_item_local_var = NULL;
    cJSON *resource_id = cJSON_GetObjectItemCaseSensitive(notification_itemJSON, "resourceId");
    if (!resource_id) {
        ogs_error("OpenAPI_notification_item_parseFromJSON() failed [resource_id]");
        goto end;
    }


    if (!cJSON_IsString(resource_id)) {
        ogs_error("OpenAPI_notification_item_parseFromJSON() failed [resource_id]");
        goto end;
    }

    cJSON *notif_items = cJSON_GetObjectItemCaseSensitive(notification_itemJSON, "notifItems");
    if (!notif_items) {
        ogs_error("OpenAPI_notification_item_parseFromJSON() failed [notif_items]");
        goto end;
    }

    OpenAPI_list_t *notif_itemsList;

    cJSON *notif_items_local_nonprimitive;
    if (!cJSON_IsArray(notif_items)) {
        ogs_error("OpenAPI_notification_item_parseFromJSON() failed [notif_items]");
        goto end;
    }

    notif_itemsList = OpenAPI_list_create();

    cJSON_ArrayForEach(notif_items_local_nonprimitive, notif_items ) {
        if (!cJSON_IsObject(notif_items_local_nonprimitive)) {
            ogs_error("OpenAPI_notification_item_parseFromJSON() failed [notif_items]");
            goto end;
        }
        OpenAPI_updated_item_t *notif_itemsItem = OpenAPI_updated_item_parseFromJSON(notif_items_local_nonprimitive);

        OpenAPI_list_add(notif_itemsList, notif_itemsItem);
    }

    notification_item_local_var = OpenAPI_notification_item_create (
        ogs_strdup(resource_id->valuestring),
        notif_itemsList
        );

    return notification_item_local_var;
end:
    return NULL;
}

OpenAPI_notification_item_t *OpenAPI_notification_item_copy(OpenAPI_notification_item_t *dst, OpenAPI_notification_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_notification_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_notification_item_convertToJSON() failed");
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

    OpenAPI_notification_item_free(dst);
    dst = OpenAPI_notification_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

