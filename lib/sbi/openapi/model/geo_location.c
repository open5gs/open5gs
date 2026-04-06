
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "geo_location.h"

OpenAPI_geo_location_t *OpenAPI_geo_location_create(
char *value
)
{
    OpenAPI_geo_location_t *geo_location_local_var = ogs_malloc(sizeof(OpenAPI_geo_location_t));
    ogs_assert(geo_location_local_var);

    geo_location_local_var->value = value;

    return geo_location_local_var;
}

void OpenAPI_geo_location_free(OpenAPI_geo_location_t *geo_location)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == geo_location) {
        return;
    }
    if (geo_location->value) {
        ogs_free(geo_location->value);
        geo_location->value = NULL;
    }
    ogs_free(geo_location);
}

cJSON *OpenAPI_geo_location_convertToJSON(OpenAPI_geo_location_t *geo_location)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (geo_location == NULL) {
        ogs_error("OpenAPI_geo_location_convertToJSON() failed [GeoLocation]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_geo_location_t *OpenAPI_geo_location_parseFromJSON(cJSON *geo_locationJSON)
{
    OpenAPI_geo_location_t *geo_location_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    geo_location_local_var = OpenAPI_geo_location_create (
        NULL
    );

    return geo_location_local_var;
end:
    return NULL;
}

OpenAPI_geo_location_t *OpenAPI_geo_location_copy(OpenAPI_geo_location_t *dst, OpenAPI_geo_location_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_geo_location_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_geo_location_convertToJSON() failed");
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

    OpenAPI_geo_location_free(dst);
    dst = OpenAPI_geo_location_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

