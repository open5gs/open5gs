
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "inline_response_200.h"

OpenAPI_inline_response_200_t *OpenAPI_inline_response_200_create(
    OpenAPI_list_t* _links
)
{
    OpenAPI_inline_response_200_t *inline_response_200_local_var = ogs_malloc(sizeof(OpenAPI_inline_response_200_t));
    ogs_assert(inline_response_200_local_var);

    inline_response_200_local_var->_links = _links;

    return inline_response_200_local_var;
}

void OpenAPI_inline_response_200_free(OpenAPI_inline_response_200_t *inline_response_200)
{
    if (NULL == inline_response_200) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(inline_response_200->_links, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_links_value_schema_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(inline_response_200->_links);
    ogs_free(inline_response_200);
}

cJSON *OpenAPI_inline_response_200_convertToJSON(OpenAPI_inline_response_200_t *inline_response_200)
{
    cJSON *item = NULL;

    if (inline_response_200 == NULL) {
        ogs_error("OpenAPI_inline_response_200_convertToJSON() failed [inline_response_200]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (inline_response_200->_links) {
    cJSON *_links = cJSON_AddObjectToObject(item, "_links");
    if (_links == NULL) {
        ogs_error("OpenAPI_inline_response_200_convertToJSON() failed [_links]");
        goto end;
    }
    cJSON *localMapObject = _links;
    OpenAPI_lnode_t *_links_node;
    if (inline_response_200->_links) {
        OpenAPI_list_for_each(inline_response_200->_links, _links_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)_links_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_links_value_schema_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_inline_response_200_convertToJSON() failed [_links]");
            goto end;
        }
        cJSON_AddItemToObject(_links, localKeyValue->key, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_inline_response_200_t *OpenAPI_inline_response_200_parseFromJSON(cJSON *inline_response_200JSON)
{
    OpenAPI_inline_response_200_t *inline_response_200_local_var = NULL;
    cJSON *_links = cJSON_GetObjectItemCaseSensitive(inline_response_200JSON, "_links");

    OpenAPI_list_t *_linksList;
    if (_links) {
    cJSON *_links_local_map;
    if (!cJSON_IsObject(_links)) {
        ogs_error("OpenAPI_inline_response_200_parseFromJSON() failed [_links]");
        goto end;
    }
    _linksList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(_links_local_map, _links) {
        cJSON *localMapObject = _links_local_map;
        if (cJSON_IsObject(_links_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_links_value_schema_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(_links_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_inline_response_200_parseFromJSON() failed [_links]");
            goto end;
        }
        OpenAPI_list_add(_linksList , localMapKeyPair);
    }
    }

    inline_response_200_local_var = OpenAPI_inline_response_200_create (
        _links ? _linksList : NULL
    );

    return inline_response_200_local_var;
end:
    return NULL;
}

OpenAPI_inline_response_200_t *OpenAPI_inline_response_200_copy(OpenAPI_inline_response_200_t *dst, OpenAPI_inline_response_200_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_inline_response_200_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_inline_response_200_convertToJSON() failed");
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

    OpenAPI_inline_response_200_free(dst);
    dst = OpenAPI_inline_response_200_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

