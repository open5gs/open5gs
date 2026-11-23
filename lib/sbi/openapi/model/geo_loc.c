
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "geo_loc.h"

OpenAPI_geo_loc_t *OpenAPI_geo_loc_create(
    OpenAPI_charging_geographical_coordinates_t *geographical_coordinates,
    char *civic_location
)
{
    OpenAPI_geo_loc_t *geo_loc_local_var = ogs_malloc(sizeof(OpenAPI_geo_loc_t));
    ogs_assert(geo_loc_local_var);

    geo_loc_local_var->geographical_coordinates = geographical_coordinates;
    geo_loc_local_var->civic_location = civic_location;

    return geo_loc_local_var;
}

void OpenAPI_geo_loc_free(OpenAPI_geo_loc_t *geo_loc)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == geo_loc) {
        return;
    }
    if (geo_loc->geographical_coordinates) {
        OpenAPI_charging_geographical_coordinates_free(geo_loc->geographical_coordinates);
        geo_loc->geographical_coordinates = NULL;
    }
    if (geo_loc->civic_location) {
        ogs_free(geo_loc->civic_location);
        geo_loc->civic_location = NULL;
    }
    ogs_free(geo_loc);
}

cJSON *OpenAPI_geo_loc_convertToJSON(OpenAPI_geo_loc_t *geo_loc)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (geo_loc == NULL) {
        ogs_error("OpenAPI_geo_loc_convertToJSON() failed [GeoLoc]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (geo_loc->geographical_coordinates) {
    cJSON *geographical_coordinates_local_JSON = OpenAPI_charging_geographical_coordinates_convertToJSON(geo_loc->geographical_coordinates);
    if (geographical_coordinates_local_JSON == NULL) {
        ogs_error("OpenAPI_geo_loc_convertToJSON() failed [geographical_coordinates]");
        goto end;
    }
    cJSON_AddItemToObject(item, "geographicalCoordinates", geographical_coordinates_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_geo_loc_convertToJSON() failed [geographical_coordinates]");
        goto end;
    }
    }

    if (geo_loc->civic_location) {
    if (cJSON_AddStringToObject(item, "civicLocation", geo_loc->civic_location) == NULL) {
        ogs_error("OpenAPI_geo_loc_convertToJSON() failed [civic_location]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_geo_loc_t *OpenAPI_geo_loc_parseFromJSON(cJSON *geo_locJSON)
{
    OpenAPI_geo_loc_t *geo_loc_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *geographical_coordinates = NULL;
    OpenAPI_charging_geographical_coordinates_t *geographical_coordinates_local_nonprim = NULL;
    cJSON *civic_location = NULL;
    geographical_coordinates = cJSON_GetObjectItemCaseSensitive(geo_locJSON, "geographicalCoordinates");
    if (geographical_coordinates) {
    geographical_coordinates_local_nonprim = OpenAPI_charging_geographical_coordinates_parseFromJSON(geographical_coordinates);
    if (!geographical_coordinates_local_nonprim) {
        ogs_error("OpenAPI_charging_geographical_coordinates_parseFromJSON failed [geographical_coordinates]");
        goto end;
    }
    }

    civic_location = cJSON_GetObjectItemCaseSensitive(geo_locJSON, "civicLocation");
    if (civic_location) {
    if (!cJSON_IsString(civic_location) && !cJSON_IsNull(civic_location)) {
        ogs_error("OpenAPI_geo_loc_parseFromJSON() failed [civic_location]");
        goto end;
    }
    }

    geo_loc_local_var = OpenAPI_geo_loc_create (
        geographical_coordinates ? geographical_coordinates_local_nonprim : NULL,
        civic_location && !cJSON_IsNull(civic_location) ? ogs_strdup(civic_location->valuestring) : NULL
    );

    return geo_loc_local_var;
end:
    if (geographical_coordinates_local_nonprim) {
        OpenAPI_charging_geographical_coordinates_free(geographical_coordinates_local_nonprim);
        geographical_coordinates_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_geo_loc_t *OpenAPI_geo_loc_copy(OpenAPI_geo_loc_t *dst, OpenAPI_geo_loc_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_geo_loc_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_geo_loc_convertToJSON() failed");
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

    OpenAPI_geo_loc_free(dst);
    dst = OpenAPI_geo_loc_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

