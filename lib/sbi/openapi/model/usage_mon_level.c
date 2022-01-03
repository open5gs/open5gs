
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage_mon_level.h"

OpenAPI_usage_mon_level_t *OpenAPI_usage_mon_level_create(
)
{
    OpenAPI_usage_mon_level_t *usage_mon_level_local_var = ogs_malloc(sizeof(OpenAPI_usage_mon_level_t));
    ogs_assert(usage_mon_level_local_var);


    return usage_mon_level_local_var;
}

void OpenAPI_usage_mon_level_free(OpenAPI_usage_mon_level_t *usage_mon_level)
{
    if (NULL == usage_mon_level) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(usage_mon_level);
}

cJSON *OpenAPI_usage_mon_level_convertToJSON(OpenAPI_usage_mon_level_t *usage_mon_level)
{
    cJSON *item = NULL;

    if (usage_mon_level == NULL) {
        ogs_error("OpenAPI_usage_mon_level_convertToJSON() failed [UsageMonLevel]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_usage_mon_level_t *OpenAPI_usage_mon_level_parseFromJSON(cJSON *usage_mon_levelJSON)
{
    OpenAPI_usage_mon_level_t *usage_mon_level_local_var = NULL;
    usage_mon_level_local_var = OpenAPI_usage_mon_level_create (
    );

    return usage_mon_level_local_var;
end:
    return NULL;
}

OpenAPI_usage_mon_level_t *OpenAPI_usage_mon_level_copy(OpenAPI_usage_mon_level_t *dst, OpenAPI_usage_mon_level_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_usage_mon_level_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_usage_mon_level_convertToJSON() failed");
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

    OpenAPI_usage_mon_level_free(dst);
    dst = OpenAPI_usage_mon_level_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

