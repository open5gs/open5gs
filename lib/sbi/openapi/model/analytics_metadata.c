
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "analytics_metadata.h"

OpenAPI_analytics_metadata_t *OpenAPI_analytics_metadata_create(
)
{
    OpenAPI_analytics_metadata_t *analytics_metadata_local_var = ogs_malloc(sizeof(OpenAPI_analytics_metadata_t));
    ogs_assert(analytics_metadata_local_var);


    return analytics_metadata_local_var;
}

void OpenAPI_analytics_metadata_free(OpenAPI_analytics_metadata_t *analytics_metadata)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == analytics_metadata) {
        return;
    }
    ogs_free(analytics_metadata);
}

cJSON *OpenAPI_analytics_metadata_convertToJSON(OpenAPI_analytics_metadata_t *analytics_metadata)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (analytics_metadata == NULL) {
        ogs_error("OpenAPI_analytics_metadata_convertToJSON() failed [AnalyticsMetadata]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_analytics_metadata_t *OpenAPI_analytics_metadata_parseFromJSON(cJSON *analytics_metadataJSON)
{
    OpenAPI_analytics_metadata_t *analytics_metadata_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    analytics_metadata_local_var = OpenAPI_analytics_metadata_create (
    );

    return analytics_metadata_local_var;
end:
    return NULL;
}

OpenAPI_analytics_metadata_t *OpenAPI_analytics_metadata_copy(OpenAPI_analytics_metadata_t *dst, OpenAPI_analytics_metadata_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_analytics_metadata_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_analytics_metadata_convertToJSON() failed");
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

    OpenAPI_analytics_metadata_free(dst);
    dst = OpenAPI_analytics_metadata_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

