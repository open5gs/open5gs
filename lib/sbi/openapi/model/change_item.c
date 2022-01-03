
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "change_item.h"

OpenAPI_change_item_t *OpenAPI_change_item_create(
    OpenAPI_change_type_e op,
    char *path,
    char *from,
    char *orig_value,
    char *new_value
)
{
    OpenAPI_change_item_t *change_item_local_var = ogs_malloc(sizeof(OpenAPI_change_item_t));
    ogs_assert(change_item_local_var);

    change_item_local_var->op = op;
    change_item_local_var->path = path;
    change_item_local_var->from = from;
    change_item_local_var->orig_value = orig_value;
    change_item_local_var->new_value = new_value;

    return change_item_local_var;
}

void OpenAPI_change_item_free(OpenAPI_change_item_t *change_item)
{
    if (NULL == change_item) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(change_item->path);
    ogs_free(change_item->from);
    ogs_free(change_item->orig_value);
    ogs_free(change_item->new_value);
    ogs_free(change_item);
}

cJSON *OpenAPI_change_item_convertToJSON(OpenAPI_change_item_t *change_item)
{
    cJSON *item = NULL;

    if (change_item == NULL) {
        ogs_error("OpenAPI_change_item_convertToJSON() failed [ChangeItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "op", OpenAPI_change_type_ToString(change_item->op)) == NULL) {
        ogs_error("OpenAPI_change_item_convertToJSON() failed [op]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "path", change_item->path) == NULL) {
        ogs_error("OpenAPI_change_item_convertToJSON() failed [path]");
        goto end;
    }

    if (change_item->from) {
    if (cJSON_AddStringToObject(item, "from", change_item->from) == NULL) {
        ogs_error("OpenAPI_change_item_convertToJSON() failed [from]");
        goto end;
    }
    }

    if (change_item->orig_value) {
    if (cJSON_AddStringToObject(item, "origValue", change_item->orig_value) == NULL) {
        ogs_error("OpenAPI_change_item_convertToJSON() failed [orig_value]");
        goto end;
    }
    }

    if (change_item->new_value) {
    if (cJSON_AddStringToObject(item, "newValue", change_item->new_value) == NULL) {
        ogs_error("OpenAPI_change_item_convertToJSON() failed [new_value]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_change_item_t *OpenAPI_change_item_parseFromJSON(cJSON *change_itemJSON)
{
    OpenAPI_change_item_t *change_item_local_var = NULL;
    cJSON *op = cJSON_GetObjectItemCaseSensitive(change_itemJSON, "op");
    if (!op) {
        ogs_error("OpenAPI_change_item_parseFromJSON() failed [op]");
        goto end;
    }

    OpenAPI_change_type_e opVariable;
    if (!cJSON_IsString(op)) {
        ogs_error("OpenAPI_change_item_parseFromJSON() failed [op]");
        goto end;
    }
    opVariable = OpenAPI_change_type_FromString(op->valuestring);

    cJSON *path = cJSON_GetObjectItemCaseSensitive(change_itemJSON, "path");
    if (!path) {
        ogs_error("OpenAPI_change_item_parseFromJSON() failed [path]");
        goto end;
    }

    if (!cJSON_IsString(path)) {
        ogs_error("OpenAPI_change_item_parseFromJSON() failed [path]");
        goto end;
    }

    cJSON *from = cJSON_GetObjectItemCaseSensitive(change_itemJSON, "from");

    if (from) {
    if (!cJSON_IsString(from)) {
        ogs_error("OpenAPI_change_item_parseFromJSON() failed [from]");
        goto end;
    }
    }

    cJSON *orig_value = cJSON_GetObjectItemCaseSensitive(change_itemJSON, "origValue");

    if (orig_value) {
    if (!cJSON_IsString(orig_value)) {
        ogs_error("OpenAPI_change_item_parseFromJSON() failed [orig_value]");
        goto end;
    }
    }

    cJSON *new_value = cJSON_GetObjectItemCaseSensitive(change_itemJSON, "newValue");

    if (new_value) {
    if (!cJSON_IsString(new_value)) {
        ogs_error("OpenAPI_change_item_parseFromJSON() failed [new_value]");
        goto end;
    }
    }

    change_item_local_var = OpenAPI_change_item_create (
        opVariable,
        ogs_strdup(path->valuestring),
        from ? ogs_strdup(from->valuestring) : NULL,
        orig_value ? ogs_strdup(orig_value->valuestring) : NULL,
        new_value ? ogs_strdup(new_value->valuestring) : NULL
    );

    return change_item_local_var;
end:
    return NULL;
}

OpenAPI_change_item_t *OpenAPI_change_item_copy(OpenAPI_change_item_t *dst, OpenAPI_change_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_change_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_change_item_convertToJSON() failed");
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

    OpenAPI_change_item_free(dst);
    dst = OpenAPI_change_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

