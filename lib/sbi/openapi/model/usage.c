
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage.h"

OpenAPI_usage_t *OpenAPI_usage_create(
)
{
    OpenAPI_usage_t *usage_local_var = ogs_malloc(sizeof(OpenAPI_usage_t));
    ogs_assert(usage_local_var);


    return usage_local_var;
}

void OpenAPI_usage_free(OpenAPI_usage_t *usage)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == usage) {
        return;
    }
    ogs_free(usage);
}

cJSON *OpenAPI_usage_convertToJSON(OpenAPI_usage_t *usage)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (usage == NULL) {
        ogs_error("OpenAPI_usage_convertToJSON() failed [Usage]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_usage_t *OpenAPI_usage_parseFromJSON(cJSON *usageJSON)
{
    OpenAPI_usage_t *usage_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    usage_local_var = OpenAPI_usage_create (
    );

    return usage_local_var;
end:
    return NULL;
}

OpenAPI_usage_t *OpenAPI_usage_copy(OpenAPI_usage_t *dst, OpenAPI_usage_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_usage_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_usage_convertToJSON() failed");
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

    OpenAPI_usage_free(dst);
    dst = OpenAPI_usage_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

