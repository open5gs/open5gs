
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "point.h"

OpenAPI_point_t *OpenAPI_point_create(
    OpenAPI_supported_gad_shapes_e shape,
    OpenAPI_geographical_coordinates_t *point
)
{
    OpenAPI_point_t *point_local_var = ogs_malloc(sizeof(OpenAPI_point_t));
    ogs_assert(point_local_var);

    point_local_var->shape = shape;
    point_local_var->point = point;

    return point_local_var;
}

void OpenAPI_point_free(OpenAPI_point_t *point)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == point) {
        return;
    }
    if (point->point) {
        OpenAPI_geographical_coordinates_free(point->point);
        point->point = NULL;
    }
    ogs_free(point);
}

cJSON *OpenAPI_point_convertToJSON(OpenAPI_point_t *point)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (point == NULL) {
        ogs_error("OpenAPI_point_convertToJSON() failed [Point]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (point->shape == OpenAPI_supported_gad_shapes_NULL) {
        ogs_error("OpenAPI_point_convertToJSON() failed [shape]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "shape", OpenAPI_supported_gad_shapes_ToString(point->shape)) == NULL) {
        ogs_error("OpenAPI_point_convertToJSON() failed [shape]");
        goto end;
    }

    if (!point->point) {
        ogs_error("OpenAPI_point_convertToJSON() failed [point]");
        return NULL;
    }
    cJSON *point_local_JSON = OpenAPI_geographical_coordinates_convertToJSON(point->point);
    if (point_local_JSON == NULL) {
        ogs_error("OpenAPI_point_convertToJSON() failed [point]");
        goto end;
    }
    cJSON_AddItemToObject(item, "point", point_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_point_convertToJSON() failed [point]");
        goto end;
    }

end:
    return item;
}

OpenAPI_point_t *OpenAPI_point_parseFromJSON(cJSON *pointJSON)
{
    OpenAPI_point_t *point_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *shape = NULL;
    OpenAPI_supported_gad_shapes_e shapeVariable = 0;
    cJSON *point = NULL;
    OpenAPI_geographical_coordinates_t *point_local_nonprim = NULL;
    shape = cJSON_GetObjectItemCaseSensitive(pointJSON, "shape");
    if (!shape) {
        ogs_error("OpenAPI_point_parseFromJSON() failed [shape]");
        goto end;
    }
    if (!cJSON_IsString(shape)) {
        ogs_error("OpenAPI_point_parseFromJSON() failed [shape]");
        goto end;
    }
    shapeVariable = OpenAPI_supported_gad_shapes_FromString(shape->valuestring);

    point = cJSON_GetObjectItemCaseSensitive(pointJSON, "point");
    if (!point) {
        ogs_error("OpenAPI_point_parseFromJSON() failed [point]");
        goto end;
    }
    point_local_nonprim = OpenAPI_geographical_coordinates_parseFromJSON(point);
    if (!point_local_nonprim) {
        ogs_error("OpenAPI_geographical_coordinates_parseFromJSON failed [point]");
        goto end;
    }

    point_local_var = OpenAPI_point_create (
        shapeVariable,
        point_local_nonprim
    );

    return point_local_var;
end:
    if (point_local_nonprim) {
        OpenAPI_geographical_coordinates_free(point_local_nonprim);
        point_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_point_t *OpenAPI_point_copy(OpenAPI_point_t *dst, OpenAPI_point_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_point_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_point_convertToJSON() failed");
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

    OpenAPI_point_free(dst);
    dst = OpenAPI_point_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

