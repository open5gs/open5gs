
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "serving_location.h"

OpenAPI_serving_location_t *OpenAPI_serving_location_create(
    OpenAPI_geo_loc_t *geographical_location,
    OpenAPI_topological_service_area_t *topological_location
)
{
    OpenAPI_serving_location_t *serving_location_local_var = ogs_malloc(sizeof(OpenAPI_serving_location_t));
    ogs_assert(serving_location_local_var);

    serving_location_local_var->geographical_location = geographical_location;
    serving_location_local_var->topological_location = topological_location;

    return serving_location_local_var;
}

void OpenAPI_serving_location_free(OpenAPI_serving_location_t *serving_location)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == serving_location) {
        return;
    }
    if (serving_location->geographical_location) {
        OpenAPI_geo_loc_free(serving_location->geographical_location);
        serving_location->geographical_location = NULL;
    }
    if (serving_location->topological_location) {
        OpenAPI_topological_service_area_free(serving_location->topological_location);
        serving_location->topological_location = NULL;
    }
    ogs_free(serving_location);
}

cJSON *OpenAPI_serving_location_convertToJSON(OpenAPI_serving_location_t *serving_location)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (serving_location == NULL) {
        ogs_error("OpenAPI_serving_location_convertToJSON() failed [ServingLocation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (serving_location->geographical_location) {
    cJSON *geographical_location_local_JSON = OpenAPI_geo_loc_convertToJSON(serving_location->geographical_location);
    if (geographical_location_local_JSON == NULL) {
        ogs_error("OpenAPI_serving_location_convertToJSON() failed [geographical_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "geographicalLocation", geographical_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_serving_location_convertToJSON() failed [geographical_location]");
        goto end;
    }
    }

    if (serving_location->topological_location) {
    cJSON *topological_location_local_JSON = OpenAPI_topological_service_area_convertToJSON(serving_location->topological_location);
    if (topological_location_local_JSON == NULL) {
        ogs_error("OpenAPI_serving_location_convertToJSON() failed [topological_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "topologicalLocation", topological_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_serving_location_convertToJSON() failed [topological_location]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_serving_location_t *OpenAPI_serving_location_parseFromJSON(cJSON *serving_locationJSON)
{
    OpenAPI_serving_location_t *serving_location_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *geographical_location = NULL;
    OpenAPI_geo_loc_t *geographical_location_local_nonprim = NULL;
    cJSON *topological_location = NULL;
    OpenAPI_topological_service_area_t *topological_location_local_nonprim = NULL;
    geographical_location = cJSON_GetObjectItemCaseSensitive(serving_locationJSON, "geographicalLocation");
    if (geographical_location) {
    geographical_location_local_nonprim = OpenAPI_geo_loc_parseFromJSON(geographical_location);
    if (!geographical_location_local_nonprim) {
        ogs_error("OpenAPI_geo_loc_parseFromJSON failed [geographical_location]");
        goto end;
    }
    }

    topological_location = cJSON_GetObjectItemCaseSensitive(serving_locationJSON, "topologicalLocation");
    if (topological_location) {
    topological_location_local_nonprim = OpenAPI_topological_service_area_parseFromJSON(topological_location);
    if (!topological_location_local_nonprim) {
        ogs_error("OpenAPI_topological_service_area_parseFromJSON failed [topological_location]");
        goto end;
    }
    }

    serving_location_local_var = OpenAPI_serving_location_create (
        geographical_location ? geographical_location_local_nonprim : NULL,
        topological_location ? topological_location_local_nonprim : NULL
    );

    return serving_location_local_var;
end:
    if (geographical_location_local_nonprim) {
        OpenAPI_geo_loc_free(geographical_location_local_nonprim);
        geographical_location_local_nonprim = NULL;
    }
    if (topological_location_local_nonprim) {
        OpenAPI_topological_service_area_free(topological_location_local_nonprim);
        topological_location_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_serving_location_t *OpenAPI_serving_location_copy(OpenAPI_serving_location_t *dst, OpenAPI_serving_location_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_serving_location_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_serving_location_convertToJSON() failed");
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

    OpenAPI_serving_location_free(dst);
    dst = OpenAPI_serving_location_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

