
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "geographical_coordinates.h"

OpenAPI_geographical_coordinates_t *OpenAPI_geographical_coordinates_create(
    double lon,
    double lat
)
{
    OpenAPI_geographical_coordinates_t *geographical_coordinates_local_var = ogs_malloc(sizeof(OpenAPI_geographical_coordinates_t));
    ogs_assert(geographical_coordinates_local_var);

    geographical_coordinates_local_var->lon = lon;
    geographical_coordinates_local_var->lat = lat;

    return geographical_coordinates_local_var;
}

void OpenAPI_geographical_coordinates_free(OpenAPI_geographical_coordinates_t *geographical_coordinates)
{
    if (NULL == geographical_coordinates) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(geographical_coordinates);
}

cJSON *OpenAPI_geographical_coordinates_convertToJSON(OpenAPI_geographical_coordinates_t *geographical_coordinates)
{
    cJSON *item = NULL;

    if (geographical_coordinates == NULL) {
        ogs_error("OpenAPI_geographical_coordinates_convertToJSON() failed [GeographicalCoordinates]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "lon", geographical_coordinates->lon) == NULL) {
        ogs_error("OpenAPI_geographical_coordinates_convertToJSON() failed [lon]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "lat", geographical_coordinates->lat) == NULL) {
        ogs_error("OpenAPI_geographical_coordinates_convertToJSON() failed [lat]");
        goto end;
    }

end:
    return item;
}

OpenAPI_geographical_coordinates_t *OpenAPI_geographical_coordinates_parseFromJSON(cJSON *geographical_coordinatesJSON)
{
    OpenAPI_geographical_coordinates_t *geographical_coordinates_local_var = NULL;
    cJSON *lon = cJSON_GetObjectItemCaseSensitive(geographical_coordinatesJSON, "lon");
    if (!lon) {
        ogs_error("OpenAPI_geographical_coordinates_parseFromJSON() failed [lon]");
        goto end;
    }

    if (!cJSON_IsNumber(lon)) {
        ogs_error("OpenAPI_geographical_coordinates_parseFromJSON() failed [lon]");
        goto end;
    }

    cJSON *lat = cJSON_GetObjectItemCaseSensitive(geographical_coordinatesJSON, "lat");
    if (!lat) {
        ogs_error("OpenAPI_geographical_coordinates_parseFromJSON() failed [lat]");
        goto end;
    }

    if (!cJSON_IsNumber(lat)) {
        ogs_error("OpenAPI_geographical_coordinates_parseFromJSON() failed [lat]");
        goto end;
    }

    geographical_coordinates_local_var = OpenAPI_geographical_coordinates_create (
        
        lon->valuedouble,
        
        lat->valuedouble
    );

    return geographical_coordinates_local_var;
end:
    return NULL;
}

OpenAPI_geographical_coordinates_t *OpenAPI_geographical_coordinates_copy(OpenAPI_geographical_coordinates_t *dst, OpenAPI_geographical_coordinates_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_geographical_coordinates_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_geographical_coordinates_convertToJSON() failed");
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

    OpenAPI_geographical_coordinates_free(dst);
    dst = OpenAPI_geographical_coordinates_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

