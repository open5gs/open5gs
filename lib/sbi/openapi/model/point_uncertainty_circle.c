
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "point_uncertainty_circle.h"

OpenAPI_point_uncertainty_circle_t *OpenAPI_point_uncertainty_circle_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_geographical_coordinates_t *point,
    float uncertainty
)
{
    OpenAPI_point_uncertainty_circle_t *point_uncertainty_circle_local_var = ogs_malloc(sizeof(OpenAPI_point_uncertainty_circle_t));
    ogs_assert(point_uncertainty_circle_local_var);

    point_uncertainty_circle_local_var->shape = shape;
    point_uncertainty_circle_local_var->point = point;
    point_uncertainty_circle_local_var->uncertainty = uncertainty;

    return point_uncertainty_circle_local_var;
}

void OpenAPI_point_uncertainty_circle_free(OpenAPI_point_uncertainty_circle_t *point_uncertainty_circle)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == point_uncertainty_circle) {
        return;
    }
    if (point_uncertainty_circle->shape) {
        OpenAPI_supported_gad_shapes_free(point_uncertainty_circle->shape);
        point_uncertainty_circle->shape = NULL;
    }
    if (point_uncertainty_circle->point) {
        OpenAPI_geographical_coordinates_free(point_uncertainty_circle->point);
        point_uncertainty_circle->point = NULL;
    }
    ogs_free(point_uncertainty_circle);
}

cJSON *OpenAPI_point_uncertainty_circle_convertToJSON(OpenAPI_point_uncertainty_circle_t *point_uncertainty_circle)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (point_uncertainty_circle == NULL) {
        ogs_error("OpenAPI_point_uncertainty_circle_convertToJSON() failed [PointUncertaintyCircle]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!point_uncertainty_circle->shape) {
        ogs_error("OpenAPI_point_uncertainty_circle_convertToJSON() failed [shape]");
        return NULL;
    }
    cJSON *shape_local_JSON = OpenAPI_supported_gad_shapes_convertToJSON(point_uncertainty_circle->shape);
    if (shape_local_JSON == NULL) {
        ogs_error("OpenAPI_point_uncertainty_circle_convertToJSON() failed [shape]");
        goto end;
    }
    cJSON_AddItemToObject(item, "shape", shape_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_point_uncertainty_circle_convertToJSON() failed [shape]");
        goto end;
    }

    if (!point_uncertainty_circle->point) {
        ogs_error("OpenAPI_point_uncertainty_circle_convertToJSON() failed [point]");
        return NULL;
    }
    cJSON *point_local_JSON = OpenAPI_geographical_coordinates_convertToJSON(point_uncertainty_circle->point);
    if (point_local_JSON == NULL) {
        ogs_error("OpenAPI_point_uncertainty_circle_convertToJSON() failed [point]");
        goto end;
    }
    cJSON_AddItemToObject(item, "point", point_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_point_uncertainty_circle_convertToJSON() failed [point]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "uncertainty", point_uncertainty_circle->uncertainty) == NULL) {
        ogs_error("OpenAPI_point_uncertainty_circle_convertToJSON() failed [uncertainty]");
        goto end;
    }

end:
    return item;
}

OpenAPI_point_uncertainty_circle_t *OpenAPI_point_uncertainty_circle_parseFromJSON(cJSON *point_uncertainty_circleJSON)
{
    OpenAPI_point_uncertainty_circle_t *point_uncertainty_circle_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *shape = NULL;
    OpenAPI_supported_gad_shapes_t *shape_local_nonprim = NULL;
    cJSON *point = NULL;
    OpenAPI_geographical_coordinates_t *point_local_nonprim = NULL;
    cJSON *uncertainty = NULL;
    shape = cJSON_GetObjectItemCaseSensitive(point_uncertainty_circleJSON, "shape");
    if (!shape) {
        ogs_error("OpenAPI_point_uncertainty_circle_parseFromJSON() failed [shape]");
        goto end;
    }
    shape_local_nonprim = OpenAPI_supported_gad_shapes_parseFromJSON(shape);
    if (!shape_local_nonprim) {
        ogs_error("OpenAPI_supported_gad_shapes_parseFromJSON failed [shape]");
        goto end;
    }

    point = cJSON_GetObjectItemCaseSensitive(point_uncertainty_circleJSON, "point");
    if (!point) {
        ogs_error("OpenAPI_point_uncertainty_circle_parseFromJSON() failed [point]");
        goto end;
    }
    point_local_nonprim = OpenAPI_geographical_coordinates_parseFromJSON(point);
    if (!point_local_nonprim) {
        ogs_error("OpenAPI_geographical_coordinates_parseFromJSON failed [point]");
        goto end;
    }

    uncertainty = cJSON_GetObjectItemCaseSensitive(point_uncertainty_circleJSON, "uncertainty");
    if (!uncertainty) {
        ogs_error("OpenAPI_point_uncertainty_circle_parseFromJSON() failed [uncertainty]");
        goto end;
    }
    if (!cJSON_IsNumber(uncertainty)) {
        ogs_error("OpenAPI_point_uncertainty_circle_parseFromJSON() failed [uncertainty]");
        goto end;
    }

    point_uncertainty_circle_local_var = OpenAPI_point_uncertainty_circle_create (
        shape_local_nonprim,
        point_local_nonprim,
        
        uncertainty->valuedouble
    );

    return point_uncertainty_circle_local_var;
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

OpenAPI_point_uncertainty_circle_t *OpenAPI_point_uncertainty_circle_copy(OpenAPI_point_uncertainty_circle_t *dst, OpenAPI_point_uncertainty_circle_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_point_uncertainty_circle_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_point_uncertainty_circle_convertToJSON() failed");
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

    OpenAPI_point_uncertainty_circle_free(dst);
    dst = OpenAPI_point_uncertainty_circle_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

