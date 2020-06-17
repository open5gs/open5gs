
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "request_type.h"

OpenAPI_request_type_t *OpenAPI_request_type_create(
    )
{
    OpenAPI_request_type_t *request_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_request_type_t));
    if (!request_type_local_var) {
        return NULL;
    }

    return request_type_local_var;
}

void OpenAPI_request_type_free(OpenAPI_request_type_t *request_type)
{
    if (NULL == request_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(request_type);
}

cJSON *OpenAPI_request_type_convertToJSON(OpenAPI_request_type_t *request_type)
{
    cJSON *item = NULL;

    if (request_type == NULL) {
        ogs_error("OpenAPI_request_type_convertToJSON() failed [RequestType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_request_type_t *OpenAPI_request_type_parseFromJSON(cJSON *request_typeJSON)
{
    OpenAPI_request_type_t *request_type_local_var = NULL;
    request_type_local_var = OpenAPI_request_type_create (
        );

    return request_type_local_var;
end:
    return NULL;
}

OpenAPI_request_type_t *OpenAPI_request_type_copy(OpenAPI_request_type_t *dst, OpenAPI_request_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_request_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_request_type_convertToJSON() failed");
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

    OpenAPI_request_type_free(dst);
    dst = OpenAPI_request_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

