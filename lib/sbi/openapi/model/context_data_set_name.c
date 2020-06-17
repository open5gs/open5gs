
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "context_data_set_name.h"

OpenAPI_context_data_set_name_t *OpenAPI_context_data_set_name_create(
    )
{
    OpenAPI_context_data_set_name_t *context_data_set_name_local_var = OpenAPI_malloc(sizeof(OpenAPI_context_data_set_name_t));
    if (!context_data_set_name_local_var) {
        return NULL;
    }

    return context_data_set_name_local_var;
}

void OpenAPI_context_data_set_name_free(OpenAPI_context_data_set_name_t *context_data_set_name)
{
    if (NULL == context_data_set_name) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(context_data_set_name);
}

cJSON *OpenAPI_context_data_set_name_convertToJSON(OpenAPI_context_data_set_name_t *context_data_set_name)
{
    cJSON *item = NULL;

    if (context_data_set_name == NULL) {
        ogs_error("OpenAPI_context_data_set_name_convertToJSON() failed [ContextDataSetName]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_context_data_set_name_t *OpenAPI_context_data_set_name_parseFromJSON(cJSON *context_data_set_nameJSON)
{
    OpenAPI_context_data_set_name_t *context_data_set_name_local_var = NULL;
    context_data_set_name_local_var = OpenAPI_context_data_set_name_create (
        );

    return context_data_set_name_local_var;
end:
    return NULL;
}

OpenAPI_context_data_set_name_t *OpenAPI_context_data_set_name_copy(OpenAPI_context_data_set_name_t *dst, OpenAPI_context_data_set_name_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_context_data_set_name_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_context_data_set_name_convertToJSON() failed");
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

    OpenAPI_context_data_set_name_free(dst);
    dst = OpenAPI_context_data_set_name_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

