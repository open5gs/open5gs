
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ellipsoid_arc.h"

OpenAPI_ellipsoid_arc_t *OpenAPI_ellipsoid_arc_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_geographical_coordinates_t *point,
    int inner_radius,
    float uncertainty_radius,
    int offset_angle,
    int included_angle,
    int confidence
)
{
    OpenAPI_ellipsoid_arc_t *ellipsoid_arc_local_var = ogs_malloc(sizeof(OpenAPI_ellipsoid_arc_t));
    ogs_assert(ellipsoid_arc_local_var);

    ellipsoid_arc_local_var->shape = shape;
    ellipsoid_arc_local_var->point = point;
    ellipsoid_arc_local_var->inner_radius = inner_radius;
    ellipsoid_arc_local_var->uncertainty_radius = uncertainty_radius;
    ellipsoid_arc_local_var->offset_angle = offset_angle;
    ellipsoid_arc_local_var->included_angle = included_angle;
    ellipsoid_arc_local_var->confidence = confidence;

    return ellipsoid_arc_local_var;
}

void OpenAPI_ellipsoid_arc_free(OpenAPI_ellipsoid_arc_t *ellipsoid_arc)
{
    if (NULL == ellipsoid_arc) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_supported_gad_shapes_free(ellipsoid_arc->shape);
    OpenAPI_geographical_coordinates_free(ellipsoid_arc->point);
    ogs_free(ellipsoid_arc);
}

cJSON *OpenAPI_ellipsoid_arc_convertToJSON(OpenAPI_ellipsoid_arc_t *ellipsoid_arc)
{
    cJSON *item = NULL;

    if (ellipsoid_arc == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_convertToJSON() failed [EllipsoidArc]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *shape_local_JSON = OpenAPI_supported_gad_shapes_convertToJSON(ellipsoid_arc->shape);
    if (shape_local_JSON == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_convertToJSON() failed [shape]");
        goto end;
    }
    cJSON_AddItemToObject(item, "shape", shape_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_convertToJSON() failed [shape]");
        goto end;
    }

    cJSON *point_local_JSON = OpenAPI_geographical_coordinates_convertToJSON(ellipsoid_arc->point);
    if (point_local_JSON == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_convertToJSON() failed [point]");
        goto end;
    }
    cJSON_AddItemToObject(item, "point", point_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_convertToJSON() failed [point]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "innerRadius", ellipsoid_arc->inner_radius) == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_convertToJSON() failed [inner_radius]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "uncertaintyRadius", ellipsoid_arc->uncertainty_radius) == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_convertToJSON() failed [uncertainty_radius]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "offsetAngle", ellipsoid_arc->offset_angle) == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_convertToJSON() failed [offset_angle]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "includedAngle", ellipsoid_arc->included_angle) == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_convertToJSON() failed [included_angle]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "confidence", ellipsoid_arc->confidence) == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_convertToJSON() failed [confidence]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ellipsoid_arc_t *OpenAPI_ellipsoid_arc_parseFromJSON(cJSON *ellipsoid_arcJSON)
{
    OpenAPI_ellipsoid_arc_t *ellipsoid_arc_local_var = NULL;
    cJSON *shape = cJSON_GetObjectItemCaseSensitive(ellipsoid_arcJSON, "shape");
    if (!shape) {
        ogs_error("OpenAPI_ellipsoid_arc_parseFromJSON() failed [shape]");
        goto end;
    }

    OpenAPI_supported_gad_shapes_t *shape_local_nonprim = NULL;
    shape_local_nonprim = OpenAPI_supported_gad_shapes_parseFromJSON(shape);

    cJSON *point = cJSON_GetObjectItemCaseSensitive(ellipsoid_arcJSON, "point");
    if (!point) {
        ogs_error("OpenAPI_ellipsoid_arc_parseFromJSON() failed [point]");
        goto end;
    }

    OpenAPI_geographical_coordinates_t *point_local_nonprim = NULL;
    point_local_nonprim = OpenAPI_geographical_coordinates_parseFromJSON(point);

    cJSON *inner_radius = cJSON_GetObjectItemCaseSensitive(ellipsoid_arcJSON, "innerRadius");
    if (!inner_radius) {
        ogs_error("OpenAPI_ellipsoid_arc_parseFromJSON() failed [inner_radius]");
        goto end;
    }

    if (!cJSON_IsNumber(inner_radius)) {
        ogs_error("OpenAPI_ellipsoid_arc_parseFromJSON() failed [inner_radius]");
        goto end;
    }

    cJSON *uncertainty_radius = cJSON_GetObjectItemCaseSensitive(ellipsoid_arcJSON, "uncertaintyRadius");
    if (!uncertainty_radius) {
        ogs_error("OpenAPI_ellipsoid_arc_parseFromJSON() failed [uncertainty_radius]");
        goto end;
    }

    if (!cJSON_IsNumber(uncertainty_radius)) {
        ogs_error("OpenAPI_ellipsoid_arc_parseFromJSON() failed [uncertainty_radius]");
        goto end;
    }

    cJSON *offset_angle = cJSON_GetObjectItemCaseSensitive(ellipsoid_arcJSON, "offsetAngle");
    if (!offset_angle) {
        ogs_error("OpenAPI_ellipsoid_arc_parseFromJSON() failed [offset_angle]");
        goto end;
    }

    if (!cJSON_IsNumber(offset_angle)) {
        ogs_error("OpenAPI_ellipsoid_arc_parseFromJSON() failed [offset_angle]");
        goto end;
    }

    cJSON *included_angle = cJSON_GetObjectItemCaseSensitive(ellipsoid_arcJSON, "includedAngle");
    if (!included_angle) {
        ogs_error("OpenAPI_ellipsoid_arc_parseFromJSON() failed [included_angle]");
        goto end;
    }

    if (!cJSON_IsNumber(included_angle)) {
        ogs_error("OpenAPI_ellipsoid_arc_parseFromJSON() failed [included_angle]");
        goto end;
    }

    cJSON *confidence = cJSON_GetObjectItemCaseSensitive(ellipsoid_arcJSON, "confidence");
    if (!confidence) {
        ogs_error("OpenAPI_ellipsoid_arc_parseFromJSON() failed [confidence]");
        goto end;
    }

    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_ellipsoid_arc_parseFromJSON() failed [confidence]");
        goto end;
    }

    ellipsoid_arc_local_var = OpenAPI_ellipsoid_arc_create (
        shape_local_nonprim,
        point_local_nonprim,
        
        inner_radius->valuedouble,
        
        uncertainty_radius->valuedouble,
        
        offset_angle->valuedouble,
        
        included_angle->valuedouble,
        
        confidence->valuedouble
    );

    return ellipsoid_arc_local_var;
end:
    return NULL;
}

OpenAPI_ellipsoid_arc_t *OpenAPI_ellipsoid_arc_copy(OpenAPI_ellipsoid_arc_t *dst, OpenAPI_ellipsoid_arc_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ellipsoid_arc_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ellipsoid_arc_convertToJSON() failed");
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

    OpenAPI_ellipsoid_arc_free(dst);
    dst = OpenAPI_ellipsoid_arc_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

