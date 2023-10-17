
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "callback_name.h"

OpenAPI_callback_name_t *OpenAPI_callback_name_create(
    char *callback_type
)
{
    OpenAPI_callback_name_t *callback_name_local_var = ogs_malloc(sizeof(OpenAPI_callback_name_t));
    ogs_assert(callback_name_local_var);

    callback_name_local_var->callback_type = callback_type;

    return callback_name_local_var;
}

void OpenAPI_callback_name_free(OpenAPI_callback_name_t *callback_name)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == callback_name) {
        return;
    }
    if (callback_name->callback_type) {
        ogs_free(callback_name->callback_type);
        callback_name->callback_type = NULL;
    }
    ogs_free(callback_name);
}

cJSON *OpenAPI_callback_name_convertToJSON(OpenAPI_callback_name_t *callback_name)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (callback_name == NULL) {
        ogs_error("OpenAPI_callback_name_convertToJSON() failed [CallbackName]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!callback_name->callback_type) {
        ogs_error("OpenAPI_callback_name_convertToJSON() failed [callback_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "callbackType", callback_name->callback_type) == NULL) {
        ogs_error("OpenAPI_callback_name_convertToJSON() failed [callback_type]");
        goto end;
    }

end:
    return item;
}

OpenAPI_callback_name_t *OpenAPI_callback_name_parseFromJSON(cJSON *callback_nameJSON)
{
    OpenAPI_callback_name_t *callback_name_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *callback_type = NULL;
    callback_type = cJSON_GetObjectItemCaseSensitive(callback_nameJSON, "callbackType");
    if (!callback_type) {
        ogs_error("OpenAPI_callback_name_parseFromJSON() failed [callback_type]");
        goto end;
    }
    if (!cJSON_IsString(callback_type)) {
        ogs_error("OpenAPI_callback_name_parseFromJSON() failed [callback_type]");
        goto end;
    }

    callback_name_local_var = OpenAPI_callback_name_create (
        ogs_strdup(callback_type->valuestring)
    );

    return callback_name_local_var;
end:
    return NULL;
}

OpenAPI_callback_name_t *OpenAPI_callback_name_copy(OpenAPI_callback_name_t *dst, OpenAPI_callback_name_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_callback_name_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_callback_name_convertToJSON() failed");
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

    OpenAPI_callback_name_free(dst);
    dst = OpenAPI_callback_name_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

