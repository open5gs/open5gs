
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "callback_uri_prefix_item.h"

OpenAPI_callback_uri_prefix_item_t *OpenAPI_callback_uri_prefix_item_create(
    char *callback_uri_prefix,
    OpenAPI_list_t *notification_types
)
{
    OpenAPI_callback_uri_prefix_item_t *callback_uri_prefix_item_local_var = ogs_malloc(sizeof(OpenAPI_callback_uri_prefix_item_t));
    ogs_assert(callback_uri_prefix_item_local_var);

    callback_uri_prefix_item_local_var->callback_uri_prefix = callback_uri_prefix;
    callback_uri_prefix_item_local_var->notification_types = notification_types;

    return callback_uri_prefix_item_local_var;
}

void OpenAPI_callback_uri_prefix_item_free(OpenAPI_callback_uri_prefix_item_t *callback_uri_prefix_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == callback_uri_prefix_item) {
        return;
    }
    if (callback_uri_prefix_item->callback_uri_prefix) {
        ogs_free(callback_uri_prefix_item->callback_uri_prefix);
        callback_uri_prefix_item->callback_uri_prefix = NULL;
    }
    if (callback_uri_prefix_item->notification_types) {
        OpenAPI_list_for_each(callback_uri_prefix_item->notification_types, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(callback_uri_prefix_item->notification_types);
        callback_uri_prefix_item->notification_types = NULL;
    }
    ogs_free(callback_uri_prefix_item);
}

cJSON *OpenAPI_callback_uri_prefix_item_convertToJSON(OpenAPI_callback_uri_prefix_item_t *callback_uri_prefix_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (callback_uri_prefix_item == NULL) {
        ogs_error("OpenAPI_callback_uri_prefix_item_convertToJSON() failed [CallbackUriPrefixItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!callback_uri_prefix_item->callback_uri_prefix) {
        ogs_error("OpenAPI_callback_uri_prefix_item_convertToJSON() failed [callback_uri_prefix]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "callbackUriPrefix", callback_uri_prefix_item->callback_uri_prefix) == NULL) {
        ogs_error("OpenAPI_callback_uri_prefix_item_convertToJSON() failed [callback_uri_prefix]");
        goto end;
    }

    if (!callback_uri_prefix_item->notification_types) {
        ogs_error("OpenAPI_callback_uri_prefix_item_convertToJSON() failed [notification_types]");
        return NULL;
    }
    cJSON *notification_typesList = cJSON_AddArrayToObject(item, "notificationTypes");
    if (notification_typesList == NULL) {
        ogs_error("OpenAPI_callback_uri_prefix_item_convertToJSON() failed [notification_types]");
        goto end;
    }
    OpenAPI_list_for_each(callback_uri_prefix_item->notification_types, node) {
        if (cJSON_AddStringToObject(notification_typesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_callback_uri_prefix_item_convertToJSON() failed [notification_types]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_callback_uri_prefix_item_t *OpenAPI_callback_uri_prefix_item_parseFromJSON(cJSON *callback_uri_prefix_itemJSON)
{
    OpenAPI_callback_uri_prefix_item_t *callback_uri_prefix_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *callback_uri_prefix = NULL;
    cJSON *notification_types = NULL;
    OpenAPI_list_t *notification_typesList = NULL;
    callback_uri_prefix = cJSON_GetObjectItemCaseSensitive(callback_uri_prefix_itemJSON, "callbackUriPrefix");
    if (!callback_uri_prefix) {
        ogs_error("OpenAPI_callback_uri_prefix_item_parseFromJSON() failed [callback_uri_prefix]");
        goto end;
    }
    if (!cJSON_IsString(callback_uri_prefix)) {
        ogs_error("OpenAPI_callback_uri_prefix_item_parseFromJSON() failed [callback_uri_prefix]");
        goto end;
    }

    notification_types = cJSON_GetObjectItemCaseSensitive(callback_uri_prefix_itemJSON, "notificationTypes");
    if (!notification_types) {
        ogs_error("OpenAPI_callback_uri_prefix_item_parseFromJSON() failed [notification_types]");
        goto end;
    }
        cJSON *notification_types_local = NULL;
        if (!cJSON_IsArray(notification_types)) {
            ogs_error("OpenAPI_callback_uri_prefix_item_parseFromJSON() failed [notification_types]");
            goto end;
        }

        notification_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(notification_types_local, notification_types) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(notification_types_local)) {
                ogs_error("OpenAPI_callback_uri_prefix_item_parseFromJSON() failed [notification_types]");
                goto end;
            }
            OpenAPI_list_add(notification_typesList, ogs_strdup(notification_types_local->valuestring));
        }

    callback_uri_prefix_item_local_var = OpenAPI_callback_uri_prefix_item_create (
        ogs_strdup(callback_uri_prefix->valuestring),
        notification_typesList
    );

    return callback_uri_prefix_item_local_var;
end:
    if (notification_typesList) {
        OpenAPI_list_for_each(notification_typesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(notification_typesList);
        notification_typesList = NULL;
    }
    return NULL;
}

OpenAPI_callback_uri_prefix_item_t *OpenAPI_callback_uri_prefix_item_copy(OpenAPI_callback_uri_prefix_item_t *dst, OpenAPI_callback_uri_prefix_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_callback_uri_prefix_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_callback_uri_prefix_item_convertToJSON() failed");
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

    OpenAPI_callback_uri_prefix_item_free(dst);
    dst = OpenAPI_callback_uri_prefix_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

