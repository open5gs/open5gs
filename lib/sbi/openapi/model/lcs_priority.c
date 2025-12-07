
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_priority.h"

OpenAPI_lcs_priority_t *OpenAPI_lcs_priority_create(
)
{
    OpenAPI_lcs_priority_t *lcs_priority_local_var = ogs_malloc(sizeof(OpenAPI_lcs_priority_t));
    ogs_assert(lcs_priority_local_var);


    return lcs_priority_local_var;
}

void OpenAPI_lcs_priority_free(OpenAPI_lcs_priority_t *lcs_priority)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == lcs_priority) {
        return;
    }
    ogs_free(lcs_priority);
}

cJSON *OpenAPI_lcs_priority_convertToJSON(OpenAPI_lcs_priority_t *lcs_priority)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (lcs_priority == NULL) {
        ogs_error("OpenAPI_lcs_priority_convertToJSON() failed [LcsPriority]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_lcs_priority_t *OpenAPI_lcs_priority_parseFromJSON(cJSON *lcs_priorityJSON)
{
    OpenAPI_lcs_priority_t *lcs_priority_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    lcs_priority_local_var = OpenAPI_lcs_priority_create (
    );

    return lcs_priority_local_var;
end:
    return NULL;
}

OpenAPI_lcs_priority_t *OpenAPI_lcs_priority_copy(OpenAPI_lcs_priority_t *dst, OpenAPI_lcs_priority_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lcs_priority_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lcs_priority_convertToJSON() failed");
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

    OpenAPI_lcs_priority_free(dst);
    dst = OpenAPI_lcs_priority_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

