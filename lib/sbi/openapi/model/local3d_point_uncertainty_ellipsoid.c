
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "local3d_point_uncertainty_ellipsoid.h"

OpenAPI_local3d_point_uncertainty_ellipsoid_t *OpenAPI_local3d_point_uncertainty_ellipsoid_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_local_origin_t *local_origin,
    OpenAPI_relative_cartesian_location_t *point,
    OpenAPI_uncertainty_ellipsoid_t *uncertainty_ellipsoid,
    int confidence
)
{
    OpenAPI_local3d_point_uncertainty_ellipsoid_t *local3d_point_uncertainty_ellipsoid_local_var = ogs_malloc(sizeof(OpenAPI_local3d_point_uncertainty_ellipsoid_t));
    ogs_assert(local3d_point_uncertainty_ellipsoid_local_var);

    local3d_point_uncertainty_ellipsoid_local_var->shape = shape;
    local3d_point_uncertainty_ellipsoid_local_var->local_origin = local_origin;
    local3d_point_uncertainty_ellipsoid_local_var->point = point;
    local3d_point_uncertainty_ellipsoid_local_var->uncertainty_ellipsoid = uncertainty_ellipsoid;
    local3d_point_uncertainty_ellipsoid_local_var->confidence = confidence;

    return local3d_point_uncertainty_ellipsoid_local_var;
}

void OpenAPI_local3d_point_uncertainty_ellipsoid_free(OpenAPI_local3d_point_uncertainty_ellipsoid_t *local3d_point_uncertainty_ellipsoid)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == local3d_point_uncertainty_ellipsoid) {
        return;
    }
    if (local3d_point_uncertainty_ellipsoid->shape) {
        OpenAPI_supported_gad_shapes_free(local3d_point_uncertainty_ellipsoid->shape);
        local3d_point_uncertainty_ellipsoid->shape = NULL;
    }
    if (local3d_point_uncertainty_ellipsoid->local_origin) {
        OpenAPI_local_origin_free(local3d_point_uncertainty_ellipsoid->local_origin);
        local3d_point_uncertainty_ellipsoid->local_origin = NULL;
    }
    if (local3d_point_uncertainty_ellipsoid->point) {
        OpenAPI_relative_cartesian_location_free(local3d_point_uncertainty_ellipsoid->point);
        local3d_point_uncertainty_ellipsoid->point = NULL;
    }
    if (local3d_point_uncertainty_ellipsoid->uncertainty_ellipsoid) {
        OpenAPI_uncertainty_ellipsoid_free(local3d_point_uncertainty_ellipsoid->uncertainty_ellipsoid);
        local3d_point_uncertainty_ellipsoid->uncertainty_ellipsoid = NULL;
    }
    ogs_free(local3d_point_uncertainty_ellipsoid);
}

cJSON *OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON(OpenAPI_local3d_point_uncertainty_ellipsoid_t *local3d_point_uncertainty_ellipsoid)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (local3d_point_uncertainty_ellipsoid == NULL) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [Local3dPointUncertaintyEllipsoid]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!local3d_point_uncertainty_ellipsoid->shape) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [shape]");
        return NULL;
    }
    cJSON *shape_local_JSON = OpenAPI_supported_gad_shapes_convertToJSON(local3d_point_uncertainty_ellipsoid->shape);
    if (shape_local_JSON == NULL) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [shape]");
        goto end;
    }
    cJSON_AddItemToObject(item, "shape", shape_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [shape]");
        goto end;
    }

    if (!local3d_point_uncertainty_ellipsoid->local_origin) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [local_origin]");
        return NULL;
    }
    cJSON *local_origin_local_JSON = OpenAPI_local_origin_convertToJSON(local3d_point_uncertainty_ellipsoid->local_origin);
    if (local_origin_local_JSON == NULL) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [local_origin]");
        goto end;
    }
    cJSON_AddItemToObject(item, "localOrigin", local_origin_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [local_origin]");
        goto end;
    }

    if (!local3d_point_uncertainty_ellipsoid->point) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [point]");
        return NULL;
    }
    cJSON *point_local_JSON = OpenAPI_relative_cartesian_location_convertToJSON(local3d_point_uncertainty_ellipsoid->point);
    if (point_local_JSON == NULL) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [point]");
        goto end;
    }
    cJSON_AddItemToObject(item, "point", point_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [point]");
        goto end;
    }

    if (!local3d_point_uncertainty_ellipsoid->uncertainty_ellipsoid) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [uncertainty_ellipsoid]");
        return NULL;
    }
    cJSON *uncertainty_ellipsoid_local_JSON = OpenAPI_uncertainty_ellipsoid_convertToJSON(local3d_point_uncertainty_ellipsoid->uncertainty_ellipsoid);
    if (uncertainty_ellipsoid_local_JSON == NULL) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [uncertainty_ellipsoid]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uncertaintyEllipsoid", uncertainty_ellipsoid_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [uncertainty_ellipsoid]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "confidence", local3d_point_uncertainty_ellipsoid->confidence) == NULL) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed [confidence]");
        goto end;
    }

end:
    return item;
}

OpenAPI_local3d_point_uncertainty_ellipsoid_t *OpenAPI_local3d_point_uncertainty_ellipsoid_parseFromJSON(cJSON *local3d_point_uncertainty_ellipsoidJSON)
{
    OpenAPI_local3d_point_uncertainty_ellipsoid_t *local3d_point_uncertainty_ellipsoid_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *shape = NULL;
    OpenAPI_supported_gad_shapes_t *shape_local_nonprim = NULL;
    cJSON *local_origin = NULL;
    OpenAPI_local_origin_t *local_origin_local_nonprim = NULL;
    cJSON *point = NULL;
    OpenAPI_relative_cartesian_location_t *point_local_nonprim = NULL;
    cJSON *uncertainty_ellipsoid = NULL;
    OpenAPI_uncertainty_ellipsoid_t *uncertainty_ellipsoid_local_nonprim = NULL;
    cJSON *confidence = NULL;
    shape = cJSON_GetObjectItemCaseSensitive(local3d_point_uncertainty_ellipsoidJSON, "shape");
    if (!shape) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_parseFromJSON() failed [shape]");
        goto end;
    }
    shape_local_nonprim = OpenAPI_supported_gad_shapes_parseFromJSON(shape);
    if (!shape_local_nonprim) {
        ogs_error("OpenAPI_supported_gad_shapes_parseFromJSON failed [shape]");
        goto end;
    }

    local_origin = cJSON_GetObjectItemCaseSensitive(local3d_point_uncertainty_ellipsoidJSON, "localOrigin");
    if (!local_origin) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_parseFromJSON() failed [local_origin]");
        goto end;
    }
    local_origin_local_nonprim = OpenAPI_local_origin_parseFromJSON(local_origin);
    if (!local_origin_local_nonprim) {
        ogs_error("OpenAPI_local_origin_parseFromJSON failed [local_origin]");
        goto end;
    }

    point = cJSON_GetObjectItemCaseSensitive(local3d_point_uncertainty_ellipsoidJSON, "point");
    if (!point) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_parseFromJSON() failed [point]");
        goto end;
    }
    point_local_nonprim = OpenAPI_relative_cartesian_location_parseFromJSON(point);
    if (!point_local_nonprim) {
        ogs_error("OpenAPI_relative_cartesian_location_parseFromJSON failed [point]");
        goto end;
    }

    uncertainty_ellipsoid = cJSON_GetObjectItemCaseSensitive(local3d_point_uncertainty_ellipsoidJSON, "uncertaintyEllipsoid");
    if (!uncertainty_ellipsoid) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_parseFromJSON() failed [uncertainty_ellipsoid]");
        goto end;
    }
    uncertainty_ellipsoid_local_nonprim = OpenAPI_uncertainty_ellipsoid_parseFromJSON(uncertainty_ellipsoid);
    if (!uncertainty_ellipsoid_local_nonprim) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_parseFromJSON failed [uncertainty_ellipsoid]");
        goto end;
    }

    confidence = cJSON_GetObjectItemCaseSensitive(local3d_point_uncertainty_ellipsoidJSON, "confidence");
    if (!confidence) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_parseFromJSON() failed [confidence]");
        goto end;
    }
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_parseFromJSON() failed [confidence]");
        goto end;
    }

    local3d_point_uncertainty_ellipsoid_local_var = OpenAPI_local3d_point_uncertainty_ellipsoid_create (
        shape_local_nonprim,
        local_origin_local_nonprim,
        point_local_nonprim,
        uncertainty_ellipsoid_local_nonprim,
        
        confidence->valuedouble
    );

    return local3d_point_uncertainty_ellipsoid_local_var;
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
    if (uncertainty_ellipsoid_local_nonprim) {
        OpenAPI_uncertainty_ellipsoid_free(uncertainty_ellipsoid_local_nonprim);
        uncertainty_ellipsoid_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_local3d_point_uncertainty_ellipsoid_t *OpenAPI_local3d_point_uncertainty_ellipsoid_copy(OpenAPI_local3d_point_uncertainty_ellipsoid_t *dst, OpenAPI_local3d_point_uncertainty_ellipsoid_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON() failed");
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

    OpenAPI_local3d_point_uncertainty_ellipsoid_free(dst);
    dst = OpenAPI_local3d_point_uncertainty_ellipsoid_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

