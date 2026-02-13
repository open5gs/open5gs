
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "local_area.h"

OpenAPI_local_area_t *OpenAPI_local_area_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_local_origin_t *local_origin,
    OpenAPI_relative_cartesian_location_t *point,
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse,
    int confidence,
    OpenAPI_uncertainty_ellipsoid_t *uncertainty_ellipsoid
)
{
    OpenAPI_local_area_t *local_area_local_var = ogs_malloc(sizeof(OpenAPI_local_area_t));
    ogs_assert(local_area_local_var);

    local_area_local_var->shape = shape;
    local_area_local_var->local_origin = local_origin;
    local_area_local_var->point = point;
    local_area_local_var->uncertainty_ellipse = uncertainty_ellipse;
    local_area_local_var->confidence = confidence;
    local_area_local_var->uncertainty_ellipsoid = uncertainty_ellipsoid;

    return local_area_local_var;
}

void OpenAPI_local_area_free(OpenAPI_local_area_t *local_area)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == local_area) {
        return;
    }
    if (local_area->shape) {
        OpenAPI_supported_gad_shapes_free(local_area->shape);
        local_area->shape = NULL;
    }
    if (local_area->local_origin) {
        OpenAPI_local_origin_free(local_area->local_origin);
        local_area->local_origin = NULL;
    }
    if (local_area->point) {
        OpenAPI_relative_cartesian_location_free(local_area->point);
        local_area->point = NULL;
    }
    if (local_area->uncertainty_ellipse) {
        OpenAPI_uncertainty_ellipse_free(local_area->uncertainty_ellipse);
        local_area->uncertainty_ellipse = NULL;
    }
    if (local_area->uncertainty_ellipsoid) {
        OpenAPI_uncertainty_ellipsoid_free(local_area->uncertainty_ellipsoid);
        local_area->uncertainty_ellipsoid = NULL;
    }
    ogs_free(local_area);
}

cJSON *OpenAPI_local_area_convertToJSON(OpenAPI_local_area_t *local_area)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (local_area == NULL) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [LocalArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!local_area->shape) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [shape]");
        return NULL;
    }
    cJSON *shape_local_JSON = OpenAPI_supported_gad_shapes_convertToJSON(local_area->shape);
    if (shape_local_JSON == NULL) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [shape]");
        goto end;
    }
    cJSON_AddItemToObject(item, "shape", shape_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [shape]");
        goto end;
    }

    if (!local_area->local_origin) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [local_origin]");
        return NULL;
    }
    cJSON *local_origin_local_JSON = OpenAPI_local_origin_convertToJSON(local_area->local_origin);
    if (local_origin_local_JSON == NULL) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [local_origin]");
        goto end;
    }
    cJSON_AddItemToObject(item, "localOrigin", local_origin_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [local_origin]");
        goto end;
    }

    if (!local_area->point) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [point]");
        return NULL;
    }
    cJSON *point_local_JSON = OpenAPI_relative_cartesian_location_convertToJSON(local_area->point);
    if (point_local_JSON == NULL) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [point]");
        goto end;
    }
    cJSON_AddItemToObject(item, "point", point_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [point]");
        goto end;
    }

    if (!local_area->uncertainty_ellipse) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [uncertainty_ellipse]");
        return NULL;
    }
    cJSON *uncertainty_ellipse_local_JSON = OpenAPI_uncertainty_ellipse_convertToJSON(local_area->uncertainty_ellipse);
    if (uncertainty_ellipse_local_JSON == NULL) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [uncertainty_ellipse]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uncertaintyEllipse", uncertainty_ellipse_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [uncertainty_ellipse]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "confidence", local_area->confidence) == NULL) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [confidence]");
        goto end;
    }

    if (!local_area->uncertainty_ellipsoid) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [uncertainty_ellipsoid]");
        return NULL;
    }
    cJSON *uncertainty_ellipsoid_local_JSON = OpenAPI_uncertainty_ellipsoid_convertToJSON(local_area->uncertainty_ellipsoid);
    if (uncertainty_ellipsoid_local_JSON == NULL) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [uncertainty_ellipsoid]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uncertaintyEllipsoid", uncertainty_ellipsoid_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed [uncertainty_ellipsoid]");
        goto end;
    }

end:
    return item;
}

OpenAPI_local_area_t *OpenAPI_local_area_parseFromJSON(cJSON *local_areaJSON)
{
    OpenAPI_local_area_t *local_area_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *shape = NULL;
    OpenAPI_supported_gad_shapes_t *shape_local_nonprim = NULL;
    cJSON *local_origin = NULL;
    OpenAPI_local_origin_t *local_origin_local_nonprim = NULL;
    cJSON *point = NULL;
    OpenAPI_relative_cartesian_location_t *point_local_nonprim = NULL;
    cJSON *uncertainty_ellipse = NULL;
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse_local_nonprim = NULL;
    cJSON *confidence = NULL;
    cJSON *uncertainty_ellipsoid = NULL;
    OpenAPI_uncertainty_ellipsoid_t *uncertainty_ellipsoid_local_nonprim = NULL;
    shape = cJSON_GetObjectItemCaseSensitive(local_areaJSON, "shape");
    if (!shape) {
        ogs_error("OpenAPI_local_area_parseFromJSON() failed [shape]");
        goto end;
    }
    shape_local_nonprim = OpenAPI_supported_gad_shapes_parseFromJSON(shape);
    if (!shape_local_nonprim) {
        ogs_error("OpenAPI_supported_gad_shapes_parseFromJSON failed [shape]");
        goto end;
    }

    local_origin = cJSON_GetObjectItemCaseSensitive(local_areaJSON, "localOrigin");
    if (!local_origin) {
        ogs_error("OpenAPI_local_area_parseFromJSON() failed [local_origin]");
        goto end;
    }
    local_origin_local_nonprim = OpenAPI_local_origin_parseFromJSON(local_origin);
    if (!local_origin_local_nonprim) {
        ogs_error("OpenAPI_local_origin_parseFromJSON failed [local_origin]");
        goto end;
    }

    point = cJSON_GetObjectItemCaseSensitive(local_areaJSON, "point");
    if (!point) {
        ogs_error("OpenAPI_local_area_parseFromJSON() failed [point]");
        goto end;
    }
    point_local_nonprim = OpenAPI_relative_cartesian_location_parseFromJSON(point);
    if (!point_local_nonprim) {
        ogs_error("OpenAPI_relative_cartesian_location_parseFromJSON failed [point]");
        goto end;
    }

    uncertainty_ellipse = cJSON_GetObjectItemCaseSensitive(local_areaJSON, "uncertaintyEllipse");
    if (!uncertainty_ellipse) {
        ogs_error("OpenAPI_local_area_parseFromJSON() failed [uncertainty_ellipse]");
        goto end;
    }
    uncertainty_ellipse_local_nonprim = OpenAPI_uncertainty_ellipse_parseFromJSON(uncertainty_ellipse);
    if (!uncertainty_ellipse_local_nonprim) {
        ogs_error("OpenAPI_uncertainty_ellipse_parseFromJSON failed [uncertainty_ellipse]");
        goto end;
    }

    confidence = cJSON_GetObjectItemCaseSensitive(local_areaJSON, "confidence");
    if (!confidence) {
        ogs_error("OpenAPI_local_area_parseFromJSON() failed [confidence]");
        goto end;
    }
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_local_area_parseFromJSON() failed [confidence]");
        goto end;
    }

    uncertainty_ellipsoid = cJSON_GetObjectItemCaseSensitive(local_areaJSON, "uncertaintyEllipsoid");
    if (!uncertainty_ellipsoid) {
        ogs_error("OpenAPI_local_area_parseFromJSON() failed [uncertainty_ellipsoid]");
        goto end;
    }
    uncertainty_ellipsoid_local_nonprim = OpenAPI_uncertainty_ellipsoid_parseFromJSON(uncertainty_ellipsoid);
    if (!uncertainty_ellipsoid_local_nonprim) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_parseFromJSON failed [uncertainty_ellipsoid]");
        goto end;
    }

    local_area_local_var = OpenAPI_local_area_create (
        shape_local_nonprim,
        local_origin_local_nonprim,
        point_local_nonprim,
        uncertainty_ellipse_local_nonprim,
        
        confidence->valuedouble,
        uncertainty_ellipsoid_local_nonprim
    );

    return local_area_local_var;
end:
    if (shape_local_nonprim) {
        OpenAPI_supported_gad_shapes_free(shape_local_nonprim);
        shape_local_nonprim = NULL;
    }
    if (local_origin_local_nonprim) {
        OpenAPI_local_origin_free(local_origin_local_nonprim);
        local_origin_local_nonprim = NULL;
    }
    if (point_local_nonprim) {
        OpenAPI_relative_cartesian_location_free(point_local_nonprim);
        point_local_nonprim = NULL;
    }
    if (uncertainty_ellipse_local_nonprim) {
        OpenAPI_uncertainty_ellipse_free(uncertainty_ellipse_local_nonprim);
        uncertainty_ellipse_local_nonprim = NULL;
    }
    if (uncertainty_ellipsoid_local_nonprim) {
        OpenAPI_uncertainty_ellipsoid_free(uncertainty_ellipsoid_local_nonprim);
        uncertainty_ellipsoid_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_local_area_t *OpenAPI_local_area_copy(OpenAPI_local_area_t *dst, OpenAPI_local_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_local_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_local_area_convertToJSON() failed");
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

    OpenAPI_local_area_free(dst);
    dst = OpenAPI_local_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

