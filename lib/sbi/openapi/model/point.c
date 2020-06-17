
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "point.h"

OpenAPI_point_t *OpenAPI_point_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_geographical_coordinates_t *point
    )
{
    OpenAPI_point_t *point_local_var = OpenAPI_malloc(sizeof(OpenAPI_point_t));
    if (!point_local_var) {
        return NULL;
    }
    point_local_var->shape = shape;
    point_local_var->point = point;

    return point_local_var;
}

void OpenAPI_point_free(OpenAPI_point_t *point)
{
    if (NULL == point) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_supported_gad_shapes_free(point->shape);
    OpenAPI_geographical_coordinates_free(point->point);
    ogs_free(point);
}

cJSON *OpenAPI_point_convertToJSON(OpenAPI_point_t *point)
{
    cJSON *item = NULL;

    if (point == NULL) {
        ogs_error("OpenAPI_point_convertToJSON() failed [Point]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!point->shape) {
        ogs_error("OpenAPI_point_convertToJSON() failed [shape]");
        goto end;
    }
    cJSON *shape_local_JSON = OpenAPI_supported_gad_shapes_convertToJSON(point->shape);
    if (shape_local_JSON == NULL) {
        ogs_error("OpenAPI_point_convertToJSON() failed [shape]");
        goto end;
    }
    cJSON_AddItemToObject(item, "shape", shape_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_point_convertToJSON() failed [shape]");
        goto end;
    }

    if (!point->point) {
        ogs_error("OpenAPI_point_convertToJSON() failed [point]");
        goto end;
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
    cJSON *shape = cJSON_GetObjectItemCaseSensitive(pointJSON, "shape");
    if (!shape) {
        ogs_error("OpenAPI_point_parseFromJSON() failed [shape]");
        goto end;
    }

    OpenAPI_supported_gad_shapes_t *shape_local_nonprim = NULL;

    shape_local_nonprim = OpenAPI_supported_gad_shapes_parseFromJSON(shape);

    cJSON *point = cJSON_GetObjectItemCaseSensitive(pointJSON, "point");
    if (!point) {
        ogs_error("OpenAPI_point_parseFromJSON() failed [point]");
        goto end;
    }

    OpenAPI_geographical_coordinates_t *point_local_nonprim = NULL;

    point_local_nonprim = OpenAPI_geographical_coordinates_parseFromJSON(point);

    point_local_var = OpenAPI_point_create (
        shape_local_nonprim,
        point_local_nonprim
        );

    return point_local_var;
end:
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

