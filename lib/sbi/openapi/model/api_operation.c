
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "api_operation.h"

OpenAPI_api_operation_t *OpenAPI_api_operation_create(
    char *name,
    char *description
)
{
    OpenAPI_api_operation_t *api_operation_local_var = ogs_malloc(sizeof(OpenAPI_api_operation_t));
    ogs_assert(api_operation_local_var);

    api_operation_local_var->name = name;
    api_operation_local_var->description = description;

    return api_operation_local_var;
}

void OpenAPI_api_operation_free(OpenAPI_api_operation_t *api_operation)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == api_operation) {
        return;
    }
    if (api_operation->name) {
        ogs_free(api_operation->name);
        api_operation->name = NULL;
    }
    if (api_operation->description) {
        ogs_free(api_operation->description);
        api_operation->description = NULL;
    }
    ogs_free(api_operation);
}

cJSON *OpenAPI_api_operation_convertToJSON(OpenAPI_api_operation_t *api_operation)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (api_operation == NULL) {
        ogs_error("OpenAPI_api_operation_convertToJSON() failed [APIOperation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (api_operation->name) {
    if (cJSON_AddStringToObject(item, "name", api_operation->name) == NULL) {
        ogs_error("OpenAPI_api_operation_convertToJSON() failed [name]");
        goto end;
    }
    }

    if (api_operation->description) {
    if (cJSON_AddStringToObject(item, "description", api_operation->description) == NULL) {
        ogs_error("OpenAPI_api_operation_convertToJSON() failed [description]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_api_operation_t *OpenAPI_api_operation_parseFromJSON(cJSON *api_operationJSON)
{
    OpenAPI_api_operation_t *api_operation_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *name = NULL;
    cJSON *description = NULL;
    name = cJSON_GetObjectItemCaseSensitive(api_operationJSON, "name");
    if (name) {
    if (!cJSON_IsString(name) && !cJSON_IsNull(name)) {
        ogs_error("OpenAPI_api_operation_parseFromJSON() failed [name]");
        goto end;
    }
    }

    description = cJSON_GetObjectItemCaseSensitive(api_operationJSON, "description");
    if (description) {
    if (!cJSON_IsString(description) && !cJSON_IsNull(description)) {
        ogs_error("OpenAPI_api_operation_parseFromJSON() failed [description]");
        goto end;
    }
    }

    api_operation_local_var = OpenAPI_api_operation_create (
        name && !cJSON_IsNull(name) ? ogs_strdup(name->valuestring) : NULL,
        description && !cJSON_IsNull(description) ? ogs_strdup(description->valuestring) : NULL
    );

    return api_operation_local_var;
end:
    return NULL;
}

OpenAPI_api_operation_t *OpenAPI_api_operation_copy(OpenAPI_api_operation_t *dst, OpenAPI_api_operation_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_api_operation_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_api_operation_convertToJSON() failed");
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

    OpenAPI_api_operation_free(dst);
    dst = OpenAPI_api_operation_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

