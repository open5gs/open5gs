
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "links.h"

cJSON *ogs_sbi_links_convertToJSON(ogs_sbi_links_t *links)
{
    cJSON *root = NULL;
    cJSON *object = NULL;

    cJSON *linksJSON = NULL;
    cJSON *itemsJSON = NULL;
    cJSON *selfJSON = NULL;

    OpenAPI_lnode_t *node;

    ogs_assert(links);

    /* _links.items */
    itemsJSON = cJSON_CreateArray();
    ogs_assert(itemsJSON);

    OpenAPI_list_for_each(links->items, node) {
        if (!node->data) continue;

        object = cJSON_CreateObject();
        ogs_assert(object);

        cJSON_AddItemToObject(object, "href", cJSON_CreateString(node->data));
        cJSON_AddItemToArray(itemsJSON, object);
    }

    /* _links.self */
    selfJSON = cJSON_CreateObject();
    ogs_assert(selfJSON);

    ogs_assert(links->self);
    object = cJSON_CreateString(links->self);
    ogs_assert(object);

    cJSON_AddItemToObject(selfJSON, "href", object);

    /* _links */
    linksJSON = cJSON_CreateObject();
    ogs_assert(linksJSON);

    cJSON_AddItemToObject(linksJSON, "items", itemsJSON);
    cJSON_AddItemToObject(linksJSON, "self", selfJSON); 

    /* root */
    root = cJSON_CreateObject();
    ogs_assert(root);

    cJSON_AddItemToObject(root, "_links", linksJSON);

    return root;
}
