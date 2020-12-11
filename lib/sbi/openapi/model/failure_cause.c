
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "failure_cause.h"

OpenAPI_failure_cause_t *OpenAPI_failure_cause_create(
    )
{
    OpenAPI_failure_cause_t *failure_cause_local_var = OpenAPI_malloc(sizeof(OpenAPI_failure_cause_t));
    if (!failure_cause_local_var) {
        return NULL;
    }

    return failure_cause_local_var;
}

void OpenAPI_failure_cause_free(OpenAPI_failure_cause_t *failure_cause)
{
    if (NULL == failure_cause) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(failure_cause);
}

cJSON *OpenAPI_failure_cause_convertToJSON(OpenAPI_failure_cause_t *failure_cause)
{
    cJSON *item = NULL;

    if (failure_cause == NULL) {
        ogs_error("OpenAPI_failure_cause_convertToJSON() failed [FailureCause]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_failure_cause_t *OpenAPI_failure_cause_parseFromJSON(cJSON *failure_causeJSON)
{
    OpenAPI_failure_cause_t *failure_cause_local_var = NULL;
    failure_cause_local_var = OpenAPI_failure_cause_create (
        );

    return failure_cause_local_var;
end:
    return NULL;
}

OpenAPI_failure_cause_t *OpenAPI_failure_cause_copy(OpenAPI_failure_cause_t *dst, OpenAPI_failure_cause_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_failure_cause_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_failure_cause_convertToJSON() failed");
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

    OpenAPI_failure_cause_free(dst);
    dst = OpenAPI_failure_cause_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

