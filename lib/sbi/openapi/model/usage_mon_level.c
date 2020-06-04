
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage_mon_level.h"

OpenAPI_usage_mon_level_t *OpenAPI_usage_mon_level_create(
    )
{
    OpenAPI_usage_mon_level_t *usage_mon_level_local_var = OpenAPI_malloc(sizeof(OpenAPI_usage_mon_level_t));
    if (!usage_mon_level_local_var) {
        return NULL;
    }

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

