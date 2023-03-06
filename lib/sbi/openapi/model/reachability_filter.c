
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reachability_filter.h"

OpenAPI_reachability_filter_t *OpenAPI_reachability_filter_create(
)
{
    OpenAPI_reachability_filter_t *reachability_filter_local_var = ogs_malloc(sizeof(OpenAPI_reachability_filter_t));
    ogs_assert(reachability_filter_local_var);


    return reachability_filter_local_var;
}

void OpenAPI_reachability_filter_free(OpenAPI_reachability_filter_t *reachability_filter)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == reachability_filter) {
        return;
    }
    ogs_free(reachability_filter);
}

cJSON *OpenAPI_reachability_filter_convertToJSON(OpenAPI_reachability_filter_t *reachability_filter)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (reachability_filter == NULL) {
        ogs_error("OpenAPI_reachability_filter_convertToJSON() failed [ReachabilityFilter]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_reachability_filter_t *OpenAPI_reachability_filter_parseFromJSON(cJSON *reachability_filterJSON)
{
    OpenAPI_reachability_filter_t *reachability_filter_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    reachability_filter_local_var = OpenAPI_reachability_filter_create (
    );

    return reachability_filter_local_var;
end:
    return NULL;
}

OpenAPI_reachability_filter_t *OpenAPI_reachability_filter_copy(OpenAPI_reachability_filter_t *dst, OpenAPI_reachability_filter_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reachability_filter_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reachability_filter_convertToJSON() failed");
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

    OpenAPI_reachability_filter_free(dst);
    dst = OpenAPI_reachability_filter_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

