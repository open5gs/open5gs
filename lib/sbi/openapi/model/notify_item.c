
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notify_item.h"

OpenAPI_notify_item_t *OpenAPI_notify_item_create(
    char *resource_id,
    OpenAPI_list_t *changes
)
{
    OpenAPI_notify_item_t *notify_item_local_var = ogs_malloc(sizeof(OpenAPI_notify_item_t));
    ogs_assert(notify_item_local_var);

    notify_item_local_var->resource_id = resource_id;
    notify_item_local_var->changes = changes;

    return notify_item_local_var;
}

void OpenAPI_notify_item_free(OpenAPI_notify_item_t *notify_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == notify_item) {
        return;
    }
    if (notify_item->resource_id) {
        ogs_free(notify_item->resource_id);
        notify_item->resource_id = NULL;
    }
    if (notify_item->changes) {
        OpenAPI_list_for_each(notify_item->changes, node) {
            OpenAPI_change_item_free(node->data);
        }
        OpenAPI_list_free(notify_item->changes);
        notify_item->changes = NULL;
    }
    ogs_free(notify_item);
}

cJSON *OpenAPI_notify_item_convertToJSON(OpenAPI_notify_item_t *notify_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (notify_item == NULL) {
        ogs_error("OpenAPI_notify_item_convertToJSON() failed [NotifyItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!notify_item->resource_id) {
        ogs_error("OpenAPI_notify_item_convertToJSON() failed [resource_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "resourceId", notify_item->resource_id) == NULL) {
        ogs_error("OpenAPI_notify_item_convertToJSON() failed [resource_id]");
        goto end;
    }

    if (!notify_item->changes) {
        ogs_error("OpenAPI_notify_item_convertToJSON() failed [changes]");
        return NULL;
    }
    cJSON *changesList = cJSON_AddArrayToObject(item, "changes");
    if (changesList == NULL) {
        ogs_error("OpenAPI_notify_item_convertToJSON() failed [changes]");
        goto end;
    }
    OpenAPI_list_for_each(notify_item->changes, node) {
        cJSON *itemLocal = OpenAPI_change_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_notify_item_convertToJSON() failed [changes]");
            goto end;
        }
        cJSON_AddItemToArray(changesList, itemLocal);
    }

end:
    return item;
}

OpenAPI_notify_item_t *OpenAPI_notify_item_parseFromJSON(cJSON *notify_itemJSON)
{
    OpenAPI_notify_item_t *notify_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *resource_id = NULL;
    cJSON *changes = NULL;
    OpenAPI_list_t *changesList = NULL;
    resource_id = cJSON_GetObjectItemCaseSensitive(notify_itemJSON, "resourceId");
    if (!resource_id) {
        ogs_error("OpenAPI_notify_item_parseFromJSON() failed [resource_id]");
        goto end;
    }
    if (!cJSON_IsString(resource_id)) {
        ogs_error("OpenAPI_notify_item_parseFromJSON() failed [resource_id]");
        goto end;
    }

    changes = cJSON_GetObjectItemCaseSensitive(notify_itemJSON, "changes");
    if (!changes) {
        ogs_error("OpenAPI_notify_item_parseFromJSON() failed [changes]");
        goto end;
    }
        cJSON *changes_local = NULL;
        if (!cJSON_IsArray(changes)) {
            ogs_error("OpenAPI_notify_item_parseFromJSON() failed [changes]");
            goto end;
        }

        changesList = OpenAPI_list_create();

        cJSON_ArrayForEach(changes_local, changes) {
            if (!cJSON_IsObject(changes_local)) {
                ogs_error("OpenAPI_notify_item_parseFromJSON() failed [changes]");
                goto end;
            }
            OpenAPI_change_item_t *changesItem = OpenAPI_change_item_parseFromJSON(changes_local);
            if (!changesItem) {
                ogs_error("No changesItem");
                goto end;
            }
            OpenAPI_list_add(changesList, changesItem);
        }

    notify_item_local_var = OpenAPI_notify_item_create (
        ogs_strdup(resource_id->valuestring),
        changesList
    );

    return notify_item_local_var;
end:
    if (changesList) {
        OpenAPI_list_for_each(changesList, node) {
            OpenAPI_change_item_free(node->data);
        }
        OpenAPI_list_free(changesList);
        changesList = NULL;
    }
    return NULL;
}

OpenAPI_notify_item_t *OpenAPI_notify_item_copy(OpenAPI_notify_item_t *dst, OpenAPI_notify_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_notify_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_notify_item_convertToJSON() failed");
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

    OpenAPI_notify_item_free(dst);
    dst = OpenAPI_notify_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

