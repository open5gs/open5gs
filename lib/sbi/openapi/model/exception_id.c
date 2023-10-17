
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "exception_id.h"

OpenAPI_exception_id_t *OpenAPI_exception_id_create(
)
{
    OpenAPI_exception_id_t *exception_id_local_var = ogs_malloc(sizeof(OpenAPI_exception_id_t));
    ogs_assert(exception_id_local_var);


    return exception_id_local_var;
}

void OpenAPI_exception_id_free(OpenAPI_exception_id_t *exception_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == exception_id) {
        return;
    }
    ogs_free(exception_id);
}

cJSON *OpenAPI_exception_id_convertToJSON(OpenAPI_exception_id_t *exception_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (exception_id == NULL) {
        ogs_error("OpenAPI_exception_id_convertToJSON() failed [ExceptionId]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_exception_id_t *OpenAPI_exception_id_parseFromJSON(cJSON *exception_idJSON)
{
    OpenAPI_exception_id_t *exception_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    exception_id_local_var = OpenAPI_exception_id_create (
    );

    return exception_id_local_var;
end:
    return NULL;
}

OpenAPI_exception_id_t *OpenAPI_exception_id_copy(OpenAPI_exception_id_t *dst, OpenAPI_exception_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_exception_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_exception_id_convertToJSON() failed");
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

    OpenAPI_exception_id_free(dst);
    dst = OpenAPI_exception_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

