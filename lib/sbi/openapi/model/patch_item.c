
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "patch_item.h"

OpenAPI_patch_item_t *OpenAPI_patch_item_create(
    OpenAPI_patch_operation_e op,
    char *path,
    char *from,
    bool is_value_null,
    OpenAPI_any_type_t *value
)
{
    OpenAPI_patch_item_t *patch_item_local_var = ogs_malloc(sizeof(OpenAPI_patch_item_t));
    ogs_assert(patch_item_local_var);

    patch_item_local_var->op = op;
    patch_item_local_var->path = path;
    patch_item_local_var->from = from;
    patch_item_local_var->is_value_null = is_value_null;
    patch_item_local_var->value = value;

    return patch_item_local_var;
}

void OpenAPI_patch_item_free(OpenAPI_patch_item_t *patch_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == patch_item) {
        return;
    }
    if (patch_item->path) {
        ogs_free(patch_item->path);
        patch_item->path = NULL;
    }
    if (patch_item->from) {
        ogs_free(patch_item->from);
        patch_item->from = NULL;
    }
    if (patch_item->value) {
        OpenAPI_any_type_free(patch_item->value);
        patch_item->value = NULL;
    }
    ogs_free(patch_item);
}

cJSON *OpenAPI_patch_item_convertToJSON(OpenAPI_patch_item_t *patch_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (patch_item == NULL) {
        ogs_error("OpenAPI_patch_item_convertToJSON() failed [PatchItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (patch_item->op == OpenAPI_patch_operation_NULL) {
        ogs_error("OpenAPI_patch_item_convertToJSON() failed [op]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "op", OpenAPI_patch_operation_ToString(patch_item->op)) == NULL) {
        ogs_error("OpenAPI_patch_item_convertToJSON() failed [op]");
        goto end;
    }

    if (!patch_item->path) {
        ogs_error("OpenAPI_patch_item_convertToJSON() failed [path]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "path", patch_item->path) == NULL) {
        ogs_error("OpenAPI_patch_item_convertToJSON() failed [path]");
        goto end;
    }

    if (patch_item->from) {
    if (cJSON_AddStringToObject(item, "from", patch_item->from) == NULL) {
        ogs_error("OpenAPI_patch_item_convertToJSON() failed [from]");
        goto end;
    }
    }

    if (patch_item->value) {
    cJSON *value_object = OpenAPI_any_type_convertToJSON(patch_item->value);
    if (value_object == NULL) {
        ogs_error("OpenAPI_patch_item_convertToJSON() failed [value]");
        goto end;
    }
    cJSON_AddItemToObject(item, "value", value_object);
    if (item->child == NULL) {
        ogs_error("OpenAPI_patch_item_convertToJSON() failed [value]");
        goto end;
    }
    } else if (patch_item->is_value_null) {
        if (cJSON_AddNullToObject(item, "value") == NULL) {
            ogs_error("OpenAPI_patch_item_convertToJSON() failed [value]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_patch_item_t *OpenAPI_patch_item_parseFromJSON(cJSON *patch_itemJSON)
{
    OpenAPI_patch_item_t *patch_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *op = NULL;
    OpenAPI_patch_operation_e opVariable = 0;
    cJSON *path = NULL;
    cJSON *from = NULL;
    cJSON *value = NULL;
    OpenAPI_any_type_t *value_local_object = NULL;
    op = cJSON_GetObjectItemCaseSensitive(patch_itemJSON, "op");
    if (!op) {
        ogs_error("OpenAPI_patch_item_parseFromJSON() failed [op]");
        goto end;
    }
    if (!cJSON_IsString(op)) {
        ogs_error("OpenAPI_patch_item_parseFromJSON() failed [op]");
        goto end;
    }
    opVariable = OpenAPI_patch_operation_FromString(op->valuestring);

    path = cJSON_GetObjectItemCaseSensitive(patch_itemJSON, "path");
    if (!path) {
        ogs_error("OpenAPI_patch_item_parseFromJSON() failed [path]");
        goto end;
    }
    if (!cJSON_IsString(path)) {
        ogs_error("OpenAPI_patch_item_parseFromJSON() failed [path]");
        goto end;
    }

    from = cJSON_GetObjectItemCaseSensitive(patch_itemJSON, "from");
    if (from) {
    if (!cJSON_IsString(from) && !cJSON_IsNull(from)) {
        ogs_error("OpenAPI_patch_item_parseFromJSON() failed [from]");
        goto end;
    }
    }

    value = cJSON_GetObjectItemCaseSensitive(patch_itemJSON, "value");
    if (value) {
    if (!cJSON_IsNull(value)) {
    value_local_object = OpenAPI_any_type_parseFromJSON(value);
    }
    }

    patch_item_local_var = OpenAPI_patch_item_create (
        opVariable,
        ogs_strdup(path->valuestring),
        from && !cJSON_IsNull(from) ? ogs_strdup(from->valuestring) : NULL,
        value && cJSON_IsNull(value) ? true : false,
        value ? value_local_object : NULL
    );

    return patch_item_local_var;
end:
    if (value_local_object) {
        OpenAPI_any_type_free(value_local_object);
        value_local_object = NULL;
    }
    return NULL;
}

OpenAPI_patch_item_t *OpenAPI_patch_item_copy(OpenAPI_patch_item_t *dst, OpenAPI_patch_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_patch_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_patch_item_convertToJSON() failed");
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

    OpenAPI_patch_item_free(dst);
    dst = OpenAPI_patch_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

