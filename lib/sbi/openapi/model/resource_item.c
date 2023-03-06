
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "resource_item.h"

OpenAPI_resource_item_t *OpenAPI_resource_item_create(
    char *mon_resource_uri,
    OpenAPI_list_t *items
)
{
    OpenAPI_resource_item_t *resource_item_local_var = ogs_malloc(sizeof(OpenAPI_resource_item_t));
    ogs_assert(resource_item_local_var);

    resource_item_local_var->mon_resource_uri = mon_resource_uri;
    resource_item_local_var->items = items;

    return resource_item_local_var;
}

void OpenAPI_resource_item_free(OpenAPI_resource_item_t *resource_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == resource_item) {
        return;
    }
    if (resource_item->mon_resource_uri) {
        ogs_free(resource_item->mon_resource_uri);
        resource_item->mon_resource_uri = NULL;
    }
    if (resource_item->items) {
        OpenAPI_list_for_each(resource_item->items, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(resource_item->items);
        resource_item->items = NULL;
    }
    ogs_free(resource_item);
}

cJSON *OpenAPI_resource_item_convertToJSON(OpenAPI_resource_item_t *resource_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (resource_item == NULL) {
        ogs_error("OpenAPI_resource_item_convertToJSON() failed [ResourceItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!resource_item->mon_resource_uri) {
        ogs_error("OpenAPI_resource_item_convertToJSON() failed [mon_resource_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "monResourceUri", resource_item->mon_resource_uri) == NULL) {
        ogs_error("OpenAPI_resource_item_convertToJSON() failed [mon_resource_uri]");
        goto end;
    }

    if (!resource_item->items) {
        ogs_error("OpenAPI_resource_item_convertToJSON() failed [items]");
        return NULL;
    }
    cJSON *itemsList = cJSON_AddArrayToObject(item, "items");
    if (itemsList == NULL) {
        ogs_error("OpenAPI_resource_item_convertToJSON() failed [items]");
        goto end;
    }
    OpenAPI_list_for_each(resource_item->items, node) {
        if (cJSON_AddStringToObject(itemsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_resource_item_convertToJSON() failed [items]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_resource_item_t *OpenAPI_resource_item_parseFromJSON(cJSON *resource_itemJSON)
{
    OpenAPI_resource_item_t *resource_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mon_resource_uri = NULL;
    cJSON *items = NULL;
    OpenAPI_list_t *itemsList = NULL;
    mon_resource_uri = cJSON_GetObjectItemCaseSensitive(resource_itemJSON, "monResourceUri");
    if (!mon_resource_uri) {
        ogs_error("OpenAPI_resource_item_parseFromJSON() failed [mon_resource_uri]");
        goto end;
    }
    if (!cJSON_IsString(mon_resource_uri)) {
        ogs_error("OpenAPI_resource_item_parseFromJSON() failed [mon_resource_uri]");
        goto end;
    }

    items = cJSON_GetObjectItemCaseSensitive(resource_itemJSON, "items");
    if (!items) {
        ogs_error("OpenAPI_resource_item_parseFromJSON() failed [items]");
        goto end;
    }
        cJSON *items_local = NULL;
        if (!cJSON_IsArray(items)) {
            ogs_error("OpenAPI_resource_item_parseFromJSON() failed [items]");
            goto end;
        }

        itemsList = OpenAPI_list_create();

        cJSON_ArrayForEach(items_local, items) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(items_local)) {
                ogs_error("OpenAPI_resource_item_parseFromJSON() failed [items]");
                goto end;
            }
            OpenAPI_list_add(itemsList, ogs_strdup(items_local->valuestring));
        }

    resource_item_local_var = OpenAPI_resource_item_create (
        ogs_strdup(mon_resource_uri->valuestring),
        itemsList
    );

    return resource_item_local_var;
end:
    if (itemsList) {
        OpenAPI_list_for_each(itemsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(itemsList);
        itemsList = NULL;
    }
    return NULL;
}

OpenAPI_resource_item_t *OpenAPI_resource_item_copy(OpenAPI_resource_item_t *dst, OpenAPI_resource_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_resource_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_resource_item_convertToJSON() failed");
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

    OpenAPI_resource_item_free(dst);
    dst = OpenAPI_resource_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

