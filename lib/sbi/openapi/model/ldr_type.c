
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ldr_type.h"

OpenAPI_ldr_type_t *OpenAPI_ldr_type_create(
)
{
    OpenAPI_ldr_type_t *ldr_type_local_var = ogs_malloc(sizeof(OpenAPI_ldr_type_t));
    ogs_assert(ldr_type_local_var);


    return ldr_type_local_var;
}

void OpenAPI_ldr_type_free(OpenAPI_ldr_type_t *ldr_type)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ldr_type) {
        return;
    }
    ogs_free(ldr_type);
}

cJSON *OpenAPI_ldr_type_convertToJSON(OpenAPI_ldr_type_t *ldr_type)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ldr_type == NULL) {
        ogs_error("OpenAPI_ldr_type_convertToJSON() failed [LdrType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_ldr_type_t *OpenAPI_ldr_type_parseFromJSON(cJSON *ldr_typeJSON)
{
    OpenAPI_ldr_type_t *ldr_type_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    ldr_type_local_var = OpenAPI_ldr_type_create (
    );

    return ldr_type_local_var;
end:
    return NULL;
}

OpenAPI_ldr_type_t *OpenAPI_ldr_type_copy(OpenAPI_ldr_type_t *dst, OpenAPI_ldr_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ldr_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ldr_type_convertToJSON() failed");
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

    OpenAPI_ldr_type_free(dst);
    dst = OpenAPI_ldr_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

