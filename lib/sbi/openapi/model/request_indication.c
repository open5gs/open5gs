
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "request_indication.h"

OpenAPI_request_indication_t *OpenAPI_request_indication_create(
    )
{
    OpenAPI_request_indication_t *request_indication_local_var = OpenAPI_malloc(sizeof(OpenAPI_request_indication_t));
    if (!request_indication_local_var) {
        return NULL;
    }

    return request_indication_local_var;
}

void OpenAPI_request_indication_free(OpenAPI_request_indication_t *request_indication)
{
    if (NULL == request_indication) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(request_indication);
}

cJSON *OpenAPI_request_indication_convertToJSON(OpenAPI_request_indication_t *request_indication)
{
    cJSON *item = NULL;

    if (request_indication == NULL) {
        ogs_error("OpenAPI_request_indication_convertToJSON() failed [RequestIndication]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_request_indication_t *OpenAPI_request_indication_parseFromJSON(cJSON *request_indicationJSON)
{
    OpenAPI_request_indication_t *request_indication_local_var = NULL;
    request_indication_local_var = OpenAPI_request_indication_create (
        );

    return request_indication_local_var;
end:
    return NULL;
}

OpenAPI_request_indication_t *OpenAPI_request_indication_copy(OpenAPI_request_indication_t *dst, OpenAPI_request_indication_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_request_indication_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_request_indication_convertToJSON() failed");
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

    OpenAPI_request_indication_free(dst);
    dst = OpenAPI_request_indication_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

