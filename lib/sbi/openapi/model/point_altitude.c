
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "point_altitude.h"

OpenAPI_point_altitude_t *OpenAPI_point_altitude_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_geographical_coordinates_t *point,
    double altitude
)
{
    OpenAPI_point_altitude_t *point_altitude_local_var = ogs_malloc(sizeof(OpenAPI_point_altitude_t));
    ogs_assert(point_altitude_local_var);

    point_altitude_local_var->shape = shape;
    point_altitude_local_var->point = point;
    point_altitude_local_var->altitude = altitude;

    return point_altitude_local_var;
}

void OpenAPI_point_altitude_free(OpenAPI_point_altitude_t *point_altitude)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == point_altitude) {
        return;
    }
    if (point_altitude->shape) {
        OpenAPI_supported_gad_shapes_free(point_altitude->shape);
        point_altitude->shape = NULL;
    }
    if (point_altitude->point) {
        OpenAPI_geographical_coordinates_free(point_altitude->point);
        point_altitude->point = NULL;
    }
    ogs_free(point_altitude);
}

cJSON *OpenAPI_point_altitude_convertToJSON(OpenAPI_point_altitude_t *point_altitude)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (point_altitude == NULL) {
        ogs_error("OpenAPI_point_altitude_convertToJSON() failed [PointAltitude]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!point_altitude->shape) {
        ogs_error("OpenAPI_point_altitude_convertToJSON() failed [shape]");
        return NULL;
    }
    cJSON *shape_local_JSON = OpenAPI_supported_gad_shapes_convertToJSON(point_altitude->shape);
    if (shape_local_JSON == NULL) {
        ogs_error("OpenAPI_point_altitude_convertToJSON() failed [shape]");
        goto end;
    }
    cJSON_AddItemToObject(item, "shape", shape_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_point_altitude_convertToJSON() failed [shape]");
        goto end;
    }

    if (!point_altitude->point) {
        ogs_error("OpenAPI_point_altitude_convertToJSON() failed [point]");
        return NULL;
    }
    cJSON *point_local_JSON = OpenAPI_geographical_coordinates_convertToJSON(point_altitude->point);
    if (point_local_JSON == NULL) {
        ogs_error("OpenAPI_point_altitude_convertToJSON() failed [point]");
        goto end;
    }
    cJSON_AddItemToObject(item, "point", point_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_point_altitude_convertToJSON() failed [point]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "altitude", point_altitude->altitude) == NULL) {
        ogs_error("OpenAPI_point_altitude_convertToJSON() failed [altitude]");
        goto end;
    }

end:
    return item;
}

OpenAPI_point_altitude_t *OpenAPI_point_altitude_parseFromJSON(cJSON *point_altitudeJSON)
{
    OpenAPI_point_altitude_t *point_altitude_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *shape = NULL;
    OpenAPI_supported_gad_shapes_t *shape_local_nonprim = NULL;
    cJSON *point = NULL;
    OpenAPI_geographical_coordinates_t *point_local_nonprim = NULL;
    cJSON *altitude = NULL;
    shape = cJSON_GetObjectItemCaseSensitive(point_altitudeJSON, "shape");
    if (!shape) {
        ogs_error("OpenAPI_point_altitude_parseFromJSON() failed [shape]");
        goto end;
    }
    shape_local_nonprim = OpenAPI_supported_gad_shapes_parseFromJSON(shape);
    if (!shape_local_nonprim) {
        ogs_error("OpenAPI_supported_gad_shapes_parseFromJSON failed [shape]");
        goto end;
    }

    point = cJSON_GetObjectItemCaseSensitive(point_altitudeJSON, "point");
    if (!point) {
        ogs_error("OpenAPI_point_altitude_parseFromJSON() failed [point]");
        goto end;
    }
    point_local_nonprim = OpenAPI_geographical_coordinates_parseFromJSON(point);
    if (!point_local_nonprim) {
        ogs_error("OpenAPI_geographical_coordinates_parseFromJSON failed [point]");
        goto end;
    }

    altitude = cJSON_GetObjectItemCaseSensitive(point_altitudeJSON, "altitude");
    if (!altitude) {
        ogs_error("OpenAPI_point_altitude_parseFromJSON() failed [altitude]");
        goto end;
    }
    if (!cJSON_IsNumber(altitude)) {
        ogs_error("OpenAPI_point_altitude_parseFromJSON() failed [altitude]");
        goto end;
    }

    point_altitude_local_var = OpenAPI_point_altitude_create (
        shape_local_nonprim,
        point_local_nonprim,
        
        altitude->valuedouble
    );

    return point_altitude_local_var;
end:
    if (shape_local_nonprim) {
        OpenAPI_supported_gad_shapes_free(shape_local_nonprim);
        shape_local_nonprim = NULL;
    }
    if (point_local_nonprim) {
        OpenAPI_geographical_coordinates_free(point_local_nonprim);
        point_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_point_altitude_t *OpenAPI_point_altitude_copy(OpenAPI_point_altitude_t *dst, OpenAPI_point_altitude_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_point_altitude_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_point_altitude_convertToJSON() failed");
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

    OpenAPI_point_altitude_free(dst);
    dst = OpenAPI_point_altitude_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

