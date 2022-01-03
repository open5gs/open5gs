
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_type_rm.h"

OpenAPI_access_type_rm_t *OpenAPI_access_type_rm_create(
)
{
    OpenAPI_access_type_rm_t *access_type_rm_local_var = ogs_malloc(sizeof(OpenAPI_access_type_rm_t));
    ogs_assert(access_type_rm_local_var);


    return access_type_rm_local_var;
}

void OpenAPI_access_type_rm_free(OpenAPI_access_type_rm_t *access_type_rm)
{
    if (NULL == access_type_rm) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(access_type_rm);
}

cJSON *OpenAPI_access_type_rm_convertToJSON(OpenAPI_access_type_rm_t *access_type_rm)
{
    cJSON *item = NULL;

    if (access_type_rm == NULL) {
        ogs_error("OpenAPI_access_type_rm_convertToJSON() failed [AccessTypeRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_access_type_rm_t *OpenAPI_access_type_rm_parseFromJSON(cJSON *access_type_rmJSON)
{
    OpenAPI_access_type_rm_t *access_type_rm_local_var = NULL;
    access_type_rm_local_var = OpenAPI_access_type_rm_create (
    );

    return access_type_rm_local_var;
end:
    return NULL;
}

OpenAPI_access_type_rm_t *OpenAPI_access_type_rm_copy(OpenAPI_access_type_rm_t *dst, OpenAPI_access_type_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_access_type_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_access_type_rm_convertToJSON() failed");
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

    OpenAPI_access_type_rm_free(dst);
    dst = OpenAPI_access_type_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

