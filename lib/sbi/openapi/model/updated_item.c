
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "updated_item.h"

OpenAPI_updated_item_t *OpenAPI_updated_item_create(
    char *item,
    bool is_value_null,
    OpenAPI_any_type_t *value
)
{
    OpenAPI_updated_item_t *updated_item_local_var = ogs_malloc(sizeof(OpenAPI_updated_item_t));
    ogs_assert(updated_item_local_var);

    updated_item_local_var->item = item;
    updated_item_local_var->is_value_null = is_value_null;
    updated_item_local_var->value = value;

    return updated_item_local_var;
}

void OpenAPI_updated_item_free(OpenAPI_updated_item_t *updated_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == updated_item) {
        return;
    }
    if (updated_item->item) {
        ogs_free(updated_item->item);
        updated_item->item = NULL;
    }
    if (updated_item->value) {
        OpenAPI_any_type_free(updated_item->value);
        updated_item->value = NULL;
    }
    ogs_free(updated_item);
}

cJSON *OpenAPI_updated_item_convertToJSON(OpenAPI_updated_item_t *updated_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (updated_item == NULL) {
        ogs_error("OpenAPI_updated_item_convertToJSON() failed [UpdatedItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!updated_item->item) {
        ogs_error("OpenAPI_updated_item_convertToJSON() failed [item]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "item", updated_item->item) == NULL) {
        ogs_error("OpenAPI_updated_item_convertToJSON() failed [item]");
        goto end;
    }

    if (!updated_item->value) {
        ogs_error("OpenAPI_updated_item_convertToJSON() failed [value]");
        return NULL;
    }
    cJSON *value_object = OpenAPI_any_type_convertToJSON(updated_item->value);
    if (value_object == NULL) {
        ogs_error("OpenAPI_updated_item_convertToJSON() failed [value]");
        goto end;
    }
    cJSON_AddItemToObject(item, "value", value_object);
    if (item->child == NULL) {
        ogs_error("OpenAPI_updated_item_convertToJSON() failed [value]");
        goto end;
    }

end:
    return item;
}

OpenAPI_updated_item_t *OpenAPI_updated_item_parseFromJSON(cJSON *updated_itemJSON)
{
    OpenAPI_updated_item_t *updated_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *item = NULL;
    cJSON *value = NULL;
    OpenAPI_any_type_t *value_local_object = NULL;
    item = cJSON_GetObjectItemCaseSensitive(updated_itemJSON, "item");
    if (!item) {
        ogs_error("OpenAPI_updated_item_parseFromJSON() failed [item]");
        goto end;
    }
    if (!cJSON_IsString(item)) {
        ogs_error("OpenAPI_updated_item_parseFromJSON() failed [item]");
        goto end;
    }

    value = cJSON_GetObjectItemCaseSensitive(updated_itemJSON, "value");
    if (!value) {
        ogs_error("OpenAPI_updated_item_parseFromJSON() failed [value]");
        goto end;
    }
    value_local_object = OpenAPI_any_type_parseFromJSON(value);

    updated_item_local_var = OpenAPI_updated_item_create (
        ogs_strdup(item->valuestring),
        value && cJSON_IsNull(value) ? true : false,
        value_local_object
    );

    return updated_item_local_var;
end:
    if (value_local_object) {
        OpenAPI_any_type_free(value_local_object);
        value_local_object = NULL;
    }
    return NULL;
}

OpenAPI_updated_item_t *OpenAPI_updated_item_copy(OpenAPI_updated_item_t *dst, OpenAPI_updated_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_updated_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_updated_item_convertToJSON() failed");
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

    OpenAPI_updated_item_free(dst);
    dst = OpenAPI_updated_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

