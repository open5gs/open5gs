
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uri_list.h"

OpenAPI_uri_list_t *OpenAPI_uri_list_create(
    OpenAPI_list_t* _links,
    bool is_total_item_count,
    int total_item_count
)
{
    OpenAPI_uri_list_t *uri_list_local_var = ogs_malloc(sizeof(OpenAPI_uri_list_t));
    ogs_assert(uri_list_local_var);

    uri_list_local_var->_links = _links;
    uri_list_local_var->is_total_item_count = is_total_item_count;
    uri_list_local_var->total_item_count = total_item_count;

    return uri_list_local_var;
}

void OpenAPI_uri_list_free(OpenAPI_uri_list_t *uri_list)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == uri_list) {
        return;
    }
    if (uri_list->_links) {
        OpenAPI_list_for_each(uri_list->_links, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_links_value_schema_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(uri_list->_links);
        uri_list->_links = NULL;
    }
    ogs_free(uri_list);
}

cJSON *OpenAPI_uri_list_convertToJSON(OpenAPI_uri_list_t *uri_list)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (uri_list == NULL) {
        ogs_error("OpenAPI_uri_list_convertToJSON() failed [UriList]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (uri_list->_links) {
    cJSON *_links = cJSON_AddObjectToObject(item, "_links");
    if (_links == NULL) {
        ogs_error("OpenAPI_uri_list_convertToJSON() failed [_links]");
        goto end;
    }
    cJSON *localMapObject = _links;
    if (uri_list->_links) {
        OpenAPI_list_for_each(uri_list->_links, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_uri_list_convertToJSON() failed [_links]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_uri_list_convertToJSON() failed [_links]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_links_value_schema_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_uri_list_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (uri_list->is_total_item_count) {
    if (cJSON_AddNumberToObject(item, "totalItemCount", uri_list->total_item_count) == NULL) {
        ogs_error("OpenAPI_uri_list_convertToJSON() failed [total_item_count]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_uri_list_t *OpenAPI_uri_list_parseFromJSON(cJSON *uri_listJSON)
{
    OpenAPI_uri_list_t *uri_list_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *_links = NULL;
    OpenAPI_list_t *_linksList = NULL;
    cJSON *total_item_count = NULL;
    _links = cJSON_GetObjectItemCaseSensitive(uri_listJSON, "_links");
    if (_links) {
        cJSON *_links_local_map = NULL;
        if (!cJSON_IsObject(_links) && !cJSON_IsNull(_links)) {
            ogs_error("OpenAPI_uri_list_parseFromJSON() failed [_links]");
            goto end;
        }
        if (cJSON_IsObject(_links)) {
            _linksList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(_links_local_map, _links) {
                cJSON *localMapObject = _links_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_links_value_schema_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_uri_list_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(_linksList, localMapKeyPair);
            }
        }
    }

    total_item_count = cJSON_GetObjectItemCaseSensitive(uri_listJSON, "totalItemCount");
    if (total_item_count) {
    if (!cJSON_IsNumber(total_item_count)) {
        ogs_error("OpenAPI_uri_list_parseFromJSON() failed [total_item_count]");
        goto end;
    }
    }

    uri_list_local_var = OpenAPI_uri_list_create (
        _links ? _linksList : NULL,
        total_item_count ? true : false,
        total_item_count ? total_item_count->valuedouble : 0
    );

    return uri_list_local_var;
end:
    if (_linksList) {
        OpenAPI_list_for_each(_linksList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_links_value_schema_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(_linksList);
        _linksList = NULL;
    }
    return NULL;
}

OpenAPI_uri_list_t *OpenAPI_uri_list_copy(OpenAPI_uri_list_t *dst, OpenAPI_uri_list_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_uri_list_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_uri_list_convertToJSON() failed");
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

    OpenAPI_uri_list_free(dst);
    dst = OpenAPI_uri_list_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

