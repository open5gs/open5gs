
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_failure_code.h"

OpenAPI_nwdaf_failure_code_t *OpenAPI_nwdaf_failure_code_create(
)
{
    OpenAPI_nwdaf_failure_code_t *nwdaf_failure_code_local_var = ogs_malloc(sizeof(OpenAPI_nwdaf_failure_code_t));
    ogs_assert(nwdaf_failure_code_local_var);


    return nwdaf_failure_code_local_var;
}

void OpenAPI_nwdaf_failure_code_free(OpenAPI_nwdaf_failure_code_t *nwdaf_failure_code)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nwdaf_failure_code) {
        return;
    }
    ogs_free(nwdaf_failure_code);
}

cJSON *OpenAPI_nwdaf_failure_code_convertToJSON(OpenAPI_nwdaf_failure_code_t *nwdaf_failure_code)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nwdaf_failure_code == NULL) {
        ogs_error("OpenAPI_nwdaf_failure_code_convertToJSON() failed [NwdafFailureCode]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_nwdaf_failure_code_t *OpenAPI_nwdaf_failure_code_parseFromJSON(cJSON *nwdaf_failure_codeJSON)
{
    OpenAPI_nwdaf_failure_code_t *nwdaf_failure_code_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    nwdaf_failure_code_local_var = OpenAPI_nwdaf_failure_code_create (
    );

    return nwdaf_failure_code_local_var;
end:
    return NULL;
}

OpenAPI_nwdaf_failure_code_t *OpenAPI_nwdaf_failure_code_copy(OpenAPI_nwdaf_failure_code_t *dst, OpenAPI_nwdaf_failure_code_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nwdaf_failure_code_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nwdaf_failure_code_convertToJSON() failed");
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

    OpenAPI_nwdaf_failure_code_free(dst);
    dst = OpenAPI_nwdaf_failure_code_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

