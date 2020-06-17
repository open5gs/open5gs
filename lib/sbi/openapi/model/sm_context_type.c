
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_type.h"

OpenAPI_sm_context_type_t *OpenAPI_sm_context_type_create(
    )
{
    OpenAPI_sm_context_type_t *sm_context_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_sm_context_type_t));
    if (!sm_context_type_local_var) {
        return NULL;
    }

    return sm_context_type_local_var;
}

void OpenAPI_sm_context_type_free(OpenAPI_sm_context_type_t *sm_context_type)
{
    if (NULL == sm_context_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sm_context_type);
}

cJSON *OpenAPI_sm_context_type_convertToJSON(OpenAPI_sm_context_type_t *sm_context_type)
{
    cJSON *item = NULL;

    if (sm_context_type == NULL) {
        ogs_error("OpenAPI_sm_context_type_convertToJSON() failed [SmContextType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_sm_context_type_t *OpenAPI_sm_context_type_parseFromJSON(cJSON *sm_context_typeJSON)
{
    OpenAPI_sm_context_type_t *sm_context_type_local_var = NULL;
    sm_context_type_local_var = OpenAPI_sm_context_type_create (
        );

    return sm_context_type_local_var;
end:
    return NULL;
}

OpenAPI_sm_context_type_t *OpenAPI_sm_context_type_copy(OpenAPI_sm_context_type_t *dst, OpenAPI_sm_context_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_type_convertToJSON() failed");
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

    OpenAPI_sm_context_type_free(dst);
    dst = OpenAPI_sm_context_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

