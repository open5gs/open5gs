
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "analytics_subset.h"

OpenAPI_analytics_subset_t *OpenAPI_analytics_subset_create(
)
{
    OpenAPI_analytics_subset_t *analytics_subset_local_var = ogs_malloc(sizeof(OpenAPI_analytics_subset_t));
    ogs_assert(analytics_subset_local_var);


    return analytics_subset_local_var;
}

void OpenAPI_analytics_subset_free(OpenAPI_analytics_subset_t *analytics_subset)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == analytics_subset) {
        return;
    }
    ogs_free(analytics_subset);
}

cJSON *OpenAPI_analytics_subset_convertToJSON(OpenAPI_analytics_subset_t *analytics_subset)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (analytics_subset == NULL) {
        ogs_error("OpenAPI_analytics_subset_convertToJSON() failed [AnalyticsSubset]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_analytics_subset_t *OpenAPI_analytics_subset_parseFromJSON(cJSON *analytics_subsetJSON)
{
    OpenAPI_analytics_subset_t *analytics_subset_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    analytics_subset_local_var = OpenAPI_analytics_subset_create (
    );

    return analytics_subset_local_var;
end:
    return NULL;
}

OpenAPI_analytics_subset_t *OpenAPI_analytics_subset_copy(OpenAPI_analytics_subset_t *dst, OpenAPI_analytics_subset_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_analytics_subset_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_analytics_subset_convertToJSON() failed");
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

    OpenAPI_analytics_subset_free(dst);
    dst = OpenAPI_analytics_subset_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

