
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "links.h"

cJSON *ogs_sbi_links_convertToJSON(ogs_sbi_links_t *links)
{
    cJSON *rootJSON = NULL;
    cJSON *linksJSON = NULL;
    cJSON *itemsJSON = NULL;
    cJSON *selfJSON = NULL;
    OpenAPI_lnode_t *node;

    ogs_assert(links);
    ogs_assert(links->self);

    rootJSON = cJSON_CreateObject();
    ogs_assert(rootJSON);

    linksJSON = cJSON_AddObjectToObject(rootJSON, "_links");
    ogs_assert(linksJSON);

    itemsJSON = cJSON_AddObjectToObject(linksJSON, "items");
    ogs_assert(itemsJSON);

    OpenAPI_list_for_each(links->items, node) {
        if (!node->data) continue;
        cJSON_AddStringToObject(itemsJSON, "href", node->data);
    }

    selfJSON = cJSON_AddObjectToObject(linksJSON, "self");
    ogs_assert(selfJSON);
    cJSON_AddStringToObject(selfJSON, "href", links->self);

    return rootJSON;
}
