
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cause.h"

OpenAPI_cause_t *OpenAPI_cause_create(
    )
{
    OpenAPI_cause_t *cause_local_var = OpenAPI_malloc(sizeof(OpenAPI_cause_t));
    if (!cause_local_var) {
        return NULL;
    }

    return cause_local_var;
}

void OpenAPI_cause_free(OpenAPI_cause_t *cause)
{
    if (NULL == cause) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(cause);
}

cJSON *OpenAPI_cause_convertToJSON(OpenAPI_cause_t *cause)
{
    cJSON *item = NULL;

    if (cause == NULL) {
        ogs_error("OpenAPI_cause_convertToJSON() failed [Cause]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_cause_t *OpenAPI_cause_parseFromJSON(cJSON *causeJSON)
{
    OpenAPI_cause_t *cause_local_var = NULL;
    cause_local_var = OpenAPI_cause_create (
        );

    return cause_local_var;
end:
    return NULL;
}

OpenAPI_cause_t *OpenAPI_cause_copy(OpenAPI_cause_t *dst, OpenAPI_cause_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cause_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cause_convertToJSON() failed");
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

    OpenAPI_cause_free(dst);
    dst = OpenAPI_cause_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

