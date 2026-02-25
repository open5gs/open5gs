
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "response_time.h"

OpenAPI_response_time_t *OpenAPI_response_time_create(
)
{
    OpenAPI_response_time_t *response_time_local_var = ogs_malloc(sizeof(OpenAPI_response_time_t));
    ogs_assert(response_time_local_var);


    return response_time_local_var;
}

void OpenAPI_response_time_free(OpenAPI_response_time_t *response_time)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == response_time) {
        return;
    }
    ogs_free(response_time);
}

cJSON *OpenAPI_response_time_convertToJSON(OpenAPI_response_time_t *response_time)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (response_time == NULL) {
        ogs_error("OpenAPI_response_time_convertToJSON() failed [ResponseTime]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_response_time_t *OpenAPI_response_time_parseFromJSON(cJSON *response_timeJSON)
{
    OpenAPI_response_time_t *response_time_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    response_time_local_var = OpenAPI_response_time_create (
    );

    return response_time_local_var;
end:
    return NULL;
}

OpenAPI_response_time_t *OpenAPI_response_time_copy(OpenAPI_response_time_t *dst, OpenAPI_response_time_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_response_time_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_response_time_convertToJSON() failed");
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

    OpenAPI_response_time_free(dst);
    dst = OpenAPI_response_time_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

