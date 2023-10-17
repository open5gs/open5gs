
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "local2d_point_uncertainty_ellipse.h"

OpenAPI_local2d_point_uncertainty_ellipse_t *OpenAPI_local2d_point_uncertainty_ellipse_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_local_origin_t *local_origin,
    OpenAPI_relative_cartesian_location_t *point,
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse,
    int confidence
)
{
    OpenAPI_local2d_point_uncertainty_ellipse_t *local2d_point_uncertainty_ellipse_local_var = ogs_malloc(sizeof(OpenAPI_local2d_point_uncertainty_ellipse_t));
    ogs_assert(local2d_point_uncertainty_ellipse_local_var);

    local2d_point_uncertainty_ellipse_local_var->shape = shape;
    local2d_point_uncertainty_ellipse_local_var->local_origin = local_origin;
    local2d_point_uncertainty_ellipse_local_var->point = point;
    local2d_point_uncertainty_ellipse_local_var->uncertainty_ellipse = uncertainty_ellipse;
    local2d_point_uncertainty_ellipse_local_var->confidence = confidence;

    return local2d_point_uncertainty_ellipse_local_var;
}

void OpenAPI_local2d_point_uncertainty_ellipse_free(OpenAPI_local2d_point_uncertainty_ellipse_t *local2d_point_uncertainty_ellipse)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == local2d_point_uncertainty_ellipse) {
        return;
    }
    if (local2d_point_uncertainty_ellipse->shape) {
        OpenAPI_supported_gad_shapes_free(local2d_point_uncertainty_ellipse->shape);
        local2d_point_uncertainty_ellipse->shape = NULL;
    }
    if (local2d_point_uncertainty_ellipse->local_origin) {
        OpenAPI_local_origin_free(local2d_point_uncertainty_ellipse->local_origin);
        local2d_point_uncertainty_ellipse->local_origin = NULL;
    }
    if (local2d_point_uncertainty_ellipse->point) {
        OpenAPI_relative_cartesian_location_free(local2d_point_uncertainty_ellipse->point);
        local2d_point_uncertainty_ellipse->point = NULL;
    }
    if (local2d_point_uncertainty_ellipse->uncertainty_ellipse) {
        OpenAPI_uncertainty_ellipse_free(local2d_point_uncertainty_ellipse->uncertainty_ellipse);
        local2d_point_uncertainty_ellipse->uncertainty_ellipse = NULL;
    }
    ogs_free(local2d_point_uncertainty_ellipse);
}

cJSON *OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON(OpenAPI_local2d_point_uncertainty_ellipse_t *local2d_point_uncertainty_ellipse)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (local2d_point_uncertainty_ellipse == NULL) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [Local2dPointUncertaintyEllipse]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!local2d_point_uncertainty_ellipse->shape) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [shape]");
        return NULL;
    }
    cJSON *shape_local_JSON = OpenAPI_supported_gad_shapes_convertToJSON(local2d_point_uncertainty_ellipse->shape);
    if (shape_local_JSON == NULL) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [shape]");
        goto end;
    }
    cJSON_AddItemToObject(item, "shape", shape_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [shape]");
        goto end;
    }

    if (!local2d_point_uncertainty_ellipse->local_origin) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [local_origin]");
        return NULL;
    }
    cJSON *local_origin_local_JSON = OpenAPI_local_origin_convertToJSON(local2d_point_uncertainty_ellipse->local_origin);
    if (local_origin_local_JSON == NULL) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [local_origin]");
        goto end;
    }
    cJSON_AddItemToObject(item, "localOrigin", local_origin_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [local_origin]");
        goto end;
    }

    if (!local2d_point_uncertainty_ellipse->point) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [point]");
        return NULL;
    }
    cJSON *point_local_JSON = OpenAPI_relative_cartesian_location_convertToJSON(local2d_point_uncertainty_ellipse->point);
    if (point_local_JSON == NULL) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [point]");
        goto end;
    }
    cJSON_AddItemToObject(item, "point", point_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [point]");
        goto end;
    }

    if (!local2d_point_uncertainty_ellipse->uncertainty_ellipse) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [uncertainty_ellipse]");
        return NULL;
    }
    cJSON *uncertainty_ellipse_local_JSON = OpenAPI_uncertainty_ellipse_convertToJSON(local2d_point_uncertainty_ellipse->uncertainty_ellipse);
    if (uncertainty_ellipse_local_JSON == NULL) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [uncertainty_ellipse]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uncertaintyEllipse", uncertainty_ellipse_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [uncertainty_ellipse]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "confidence", local2d_point_uncertainty_ellipse->confidence) == NULL) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed [confidence]");
        goto end;
    }

end:
    return item;
}

OpenAPI_local2d_point_uncertainty_ellipse_t *OpenAPI_local2d_point_uncertainty_ellipse_parseFromJSON(cJSON *local2d_point_uncertainty_ellipseJSON)
{
    OpenAPI_local2d_point_uncertainty_ellipse_t *local2d_point_uncertainty_ellipse_local_var = NULL;
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
    shape = cJSON_GetObjectItemCaseSensitive(local2d_point_uncertainty_ellipseJSON, "shape");
    if (!shape) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_parseFromJSON() failed [shape]");
        goto end;
    }
    shape_local_nonprim = OpenAPI_supported_gad_shapes_parseFromJSON(shape);
    if (!shape_local_nonprim) {
        ogs_error("OpenAPI_supported_gad_shapes_parseFromJSON failed [shape]");
        goto end;
    }

    local_origin = cJSON_GetObjectItemCaseSensitive(local2d_point_uncertainty_ellipseJSON, "localOrigin");
    if (!local_origin) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_parseFromJSON() failed [local_origin]");
        goto end;
    }
    local_origin_local_nonprim = OpenAPI_local_origin_parseFromJSON(local_origin);
    if (!local_origin_local_nonprim) {
        ogs_error("OpenAPI_local_origin_parseFromJSON failed [local_origin]");
        goto end;
    }

    point = cJSON_GetObjectItemCaseSensitive(local2d_point_uncertainty_ellipseJSON, "point");
    if (!point) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_parseFromJSON() failed [point]");
        goto end;
    }
    point_local_nonprim = OpenAPI_relative_cartesian_location_parseFromJSON(point);
    if (!point_local_nonprim) {
        ogs_error("OpenAPI_relative_cartesian_location_parseFromJSON failed [point]");
        goto end;
    }

    uncertainty_ellipse = cJSON_GetObjectItemCaseSensitive(local2d_point_uncertainty_ellipseJSON, "uncertaintyEllipse");
    if (!uncertainty_ellipse) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_parseFromJSON() failed [uncertainty_ellipse]");
        goto end;
    }
    uncertainty_ellipse_local_nonprim = OpenAPI_uncertainty_ellipse_parseFromJSON(uncertainty_ellipse);
    if (!uncertainty_ellipse_local_nonprim) {
        ogs_error("OpenAPI_uncertainty_ellipse_parseFromJSON failed [uncertainty_ellipse]");
        goto end;
    }

    confidence = cJSON_GetObjectItemCaseSensitive(local2d_point_uncertainty_ellipseJSON, "confidence");
    if (!confidence) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_parseFromJSON() failed [confidence]");
        goto end;
    }
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_parseFromJSON() failed [confidence]");
        goto end;
    }

    local2d_point_uncertainty_ellipse_local_var = OpenAPI_local2d_point_uncertainty_ellipse_create (
        shape_local_nonprim,
        local_origin_local_nonprim,
        point_local_nonprim,
        uncertainty_ellipse_local_nonprim,
        
        confidence->valuedouble
    );

    return local2d_point_uncertainty_ellipse_local_var;
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
    return NULL;
}

OpenAPI_local2d_point_uncertainty_ellipse_t *OpenAPI_local2d_point_uncertainty_ellipse_copy(OpenAPI_local2d_point_uncertainty_ellipse_t *dst, OpenAPI_local2d_point_uncertainty_ellipse_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON() failed");
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

    OpenAPI_local2d_point_uncertainty_ellipse_free(dst);
    dst = OpenAPI_local2d_point_uncertainty_ellipse_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

