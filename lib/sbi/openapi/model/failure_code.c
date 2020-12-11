
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "failure_code.h"

OpenAPI_failure_code_t *OpenAPI_failure_code_create(
    )
{
    OpenAPI_failure_code_t *failure_code_local_var = OpenAPI_malloc(sizeof(OpenAPI_failure_code_t));
    if (!failure_code_local_var) {
        return NULL;
    }

    return failure_code_local_var;
}

void OpenAPI_failure_code_free(OpenAPI_failure_code_t *failure_code)
{
    if (NULL == failure_code) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(failure_code);
}

cJSON *OpenAPI_failure_code_convertToJSON(OpenAPI_failure_code_t *failure_code)
{
    cJSON *item = NULL;

    if (failure_code == NULL) {
        ogs_error("OpenAPI_failure_code_convertToJSON() failed [FailureCode]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_failure_code_t *OpenAPI_failure_code_parseFromJSON(cJSON *failure_codeJSON)
{
    OpenAPI_failure_code_t *failure_code_local_var = NULL;
    failure_code_local_var = OpenAPI_failure_code_create (
        );

    return failure_code_local_var;
end:
    return NULL;
}

OpenAPI_failure_code_t *OpenAPI_failure_code_copy(OpenAPI_failure_code_t *dst, OpenAPI_failure_code_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_failure_code_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_failure_code_convertToJSON() failed");
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

    OpenAPI_failure_code_free(dst);
    dst = OpenAPI_failure_code_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

