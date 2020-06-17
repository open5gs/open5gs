
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
    OpenAPI_point_altitude_t *point_altitude_local_var = OpenAPI_malloc(sizeof(OpenAPI_point_altitude_t));
    if (!point_altitude_local_var) {
        return NULL;
    }
    point_altitude_local_var->shape = shape;
    point_altitude_local_var->point = point;
    point_altitude_local_var->altitude = altitude;

    return point_altitude_local_var;
}

void OpenAPI_point_altitude_free(OpenAPI_point_altitude_t *point_altitude)
{
    if (NULL == point_altitude) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_supported_gad_shapes_free(point_altitude->shape);
    OpenAPI_geographical_coordinates_free(point_altitude->point);
    ogs_free(point_altitude);
}

cJSON *OpenAPI_point_altitude_convertToJSON(OpenAPI_point_altitude_t *point_altitude)
{
    cJSON *item = NULL;

    if (point_altitude == NULL) {
        ogs_error("OpenAPI_point_altitude_convertToJSON() failed [PointAltitude]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!point_altitude->shape) {
        ogs_error("OpenAPI_point_altitude_convertToJSON() failed [shape]");
        goto end;
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
        goto end;
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

    if (!point_altitude->altitude) {
        ogs_error("OpenAPI_point_altitude_convertToJSON() failed [altitude]");
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
    cJSON *shape = cJSON_GetObjectItemCaseSensitive(point_altitudeJSON, "shape");
    if (!shape) {
        ogs_error("OpenAPI_point_altitude_parseFromJSON() failed [shape]");
        goto end;
    }

    OpenAPI_supported_gad_shapes_t *shape_local_nonprim = NULL;

    shape_local_nonprim = OpenAPI_supported_gad_shapes_parseFromJSON(shape);

    cJSON *point = cJSON_GetObjectItemCaseSensitive(point_altitudeJSON, "point");
    if (!point) {
        ogs_error("OpenAPI_point_altitude_parseFromJSON() failed [point]");
        goto end;
    }

    OpenAPI_geographical_coordinates_t *point_local_nonprim = NULL;

    point_local_nonprim = OpenAPI_geographical_coordinates_parseFromJSON(point);

    cJSON *altitude = cJSON_GetObjectItemCaseSensitive(point_altitudeJSON, "altitude");
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

