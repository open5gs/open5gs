
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_filter.h"

OpenAPI_location_filter_t *OpenAPI_location_filter_create(
    )
{
    OpenAPI_location_filter_t *location_filter_local_var = OpenAPI_malloc(sizeof(OpenAPI_location_filter_t));
    if (!location_filter_local_var) {
        return NULL;
    }

    return location_filter_local_var;
}

void OpenAPI_location_filter_free(OpenAPI_location_filter_t *location_filter)
{
    if (NULL == location_filter) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(location_filter);
}

cJSON *OpenAPI_location_filter_convertToJSON(OpenAPI_location_filter_t *location_filter)
{
    cJSON *item = NULL;

    if (location_filter == NULL) {
        ogs_error("OpenAPI_location_filter_convertToJSON() failed [LocationFilter]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_location_filter_t *OpenAPI_location_filter_parseFromJSON(cJSON *location_filterJSON)
{
    OpenAPI_location_filter_t *location_filter_local_var = NULL;
    location_filter_local_var = OpenAPI_location_filter_create (
        );

    return location_filter_local_var;
end:
    return NULL;
}

OpenAPI_location_filter_t *OpenAPI_location_filter_copy(OpenAPI_location_filter_t *dst, OpenAPI_location_filter_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_location_filter_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_location_filter_convertToJSON() failed");
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

    OpenAPI_location_filter_free(dst);
    dst = OpenAPI_location_filter_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

