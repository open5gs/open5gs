
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
    if (NULL == resource_item) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(resource_item->mon_resource_uri);
    OpenAPI_list_for_each(resource_item->items, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(resource_item->items);
    ogs_free(resource_item);
}

cJSON *OpenAPI_resource_item_convertToJSON(OpenAPI_resource_item_t *resource_item)
{
    cJSON *item = NULL;

    if (resource_item == NULL) {
        ogs_error("OpenAPI_resource_item_convertToJSON() failed [ResourceItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "monResourceUri", resource_item->mon_resource_uri) == NULL) {
        ogs_error("OpenAPI_resource_item_convertToJSON() failed [mon_resource_uri]");
        goto end;
    }

    cJSON *items = cJSON_AddArrayToObject(item, "items");
    if (items == NULL) {
        ogs_error("OpenAPI_resource_item_convertToJSON() failed [items]");
        goto end;
    }

    OpenAPI_lnode_t *items_node;
    OpenAPI_list_for_each(resource_item->items, items_node)  {
    if (cJSON_AddStringToObject(items, "", (char*)items_node->data) == NULL) {
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
    cJSON *mon_resource_uri = cJSON_GetObjectItemCaseSensitive(resource_itemJSON, "monResourceUri");
    if (!mon_resource_uri) {
        ogs_error("OpenAPI_resource_item_parseFromJSON() failed [mon_resource_uri]");
        goto end;
    }

    if (!cJSON_IsString(mon_resource_uri)) {
        ogs_error("OpenAPI_resource_item_parseFromJSON() failed [mon_resource_uri]");
        goto end;
    }

    cJSON *items = cJSON_GetObjectItemCaseSensitive(resource_itemJSON, "items");
    if (!items) {
        ogs_error("OpenAPI_resource_item_parseFromJSON() failed [items]");
        goto end;
    }

    OpenAPI_list_t *itemsList;
    cJSON *items_local;
    if (!cJSON_IsArray(items)) {
        ogs_error("OpenAPI_resource_item_parseFromJSON() failed [items]");
        goto end;
    }
    itemsList = OpenAPI_list_create();

    cJSON_ArrayForEach(items_local, items) {
    if (!cJSON_IsString(items_local)) {
        ogs_error("OpenAPI_resource_item_parseFromJSON() failed [items]");
        goto end;
    }
    OpenAPI_list_add(itemsList , ogs_strdup(items_local->valuestring));
    }

    resource_item_local_var = OpenAPI_resource_item_create (
        ogs_strdup(mon_resource_uri->valuestring),
        itemsList
    );

    return resource_item_local_var;
end:
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

