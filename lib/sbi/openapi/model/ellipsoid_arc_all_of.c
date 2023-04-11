
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ellipsoid_arc_all_of.h"

OpenAPI_ellipsoid_arc_all_of_t *OpenAPI_ellipsoid_arc_all_of_create(
    OpenAPI_geographical_coordinates_t *point,
    int inner_radius,
    float uncertainty_radius,
    int offset_angle,
    int included_angle,
    int confidence
)
{
    OpenAPI_ellipsoid_arc_all_of_t *ellipsoid_arc_all_of_local_var = ogs_malloc(sizeof(OpenAPI_ellipsoid_arc_all_of_t));
    ogs_assert(ellipsoid_arc_all_of_local_var);

    ellipsoid_arc_all_of_local_var->point = point;
    ellipsoid_arc_all_of_local_var->inner_radius = inner_radius;
    ellipsoid_arc_all_of_local_var->uncertainty_radius = uncertainty_radius;
    ellipsoid_arc_all_of_local_var->offset_angle = offset_angle;
    ellipsoid_arc_all_of_local_var->included_angle = included_angle;
    ellipsoid_arc_all_of_local_var->confidence = confidence;

    return ellipsoid_arc_all_of_local_var;
}

void OpenAPI_ellipsoid_arc_all_of_free(OpenAPI_ellipsoid_arc_all_of_t *ellipsoid_arc_all_of)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ellipsoid_arc_all_of) {
        return;
    }
    if (ellipsoid_arc_all_of->point) {
        OpenAPI_geographical_coordinates_free(ellipsoid_arc_all_of->point);
        ellipsoid_arc_all_of->point = NULL;
    }
    ogs_free(ellipsoid_arc_all_of);
}

cJSON *OpenAPI_ellipsoid_arc_all_of_convertToJSON(OpenAPI_ellipsoid_arc_all_of_t *ellipsoid_arc_all_of)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ellipsoid_arc_all_of == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_convertToJSON() failed [EllipsoidArc_allOf]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ellipsoid_arc_all_of->point) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_convertToJSON() failed [point]");
        return NULL;
    }
    cJSON *point_local_JSON = OpenAPI_geographical_coordinates_convertToJSON(ellipsoid_arc_all_of->point);
    if (point_local_JSON == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_convertToJSON() failed [point]");
        goto end;
    }
    cJSON_AddItemToObject(item, "point", point_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_convertToJSON() failed [point]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "innerRadius", ellipsoid_arc_all_of->inner_radius) == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_convertToJSON() failed [inner_radius]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "uncertaintyRadius", ellipsoid_arc_all_of->uncertainty_radius) == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_convertToJSON() failed [uncertainty_radius]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "offsetAngle", ellipsoid_arc_all_of->offset_angle) == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_convertToJSON() failed [offset_angle]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "includedAngle", ellipsoid_arc_all_of->included_angle) == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_convertToJSON() failed [included_angle]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "confidence", ellipsoid_arc_all_of->confidence) == NULL) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_convertToJSON() failed [confidence]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ellipsoid_arc_all_of_t *OpenAPI_ellipsoid_arc_all_of_parseFromJSON(cJSON *ellipsoid_arc_all_ofJSON)
{
    OpenAPI_ellipsoid_arc_all_of_t *ellipsoid_arc_all_of_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *point = NULL;
    OpenAPI_geographical_coordinates_t *point_local_nonprim = NULL;
    cJSON *inner_radius = NULL;
    cJSON *uncertainty_radius = NULL;
    cJSON *offset_angle = NULL;
    cJSON *included_angle = NULL;
    cJSON *confidence = NULL;
    point = cJSON_GetObjectItemCaseSensitive(ellipsoid_arc_all_ofJSON, "point");
    if (!point) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_parseFromJSON() failed [point]");
        goto end;
    }
    point_local_nonprim = OpenAPI_geographical_coordinates_parseFromJSON(point);
    if (!point_local_nonprim) {
        ogs_error("OpenAPI_geographical_coordinates_parseFromJSON failed [point]");
        goto end;
    }

    inner_radius = cJSON_GetObjectItemCaseSensitive(ellipsoid_arc_all_ofJSON, "innerRadius");
    if (!inner_radius) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_parseFromJSON() failed [inner_radius]");
        goto end;
    }
    if (!cJSON_IsNumber(inner_radius)) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_parseFromJSON() failed [inner_radius]");
        goto end;
    }

    uncertainty_radius = cJSON_GetObjectItemCaseSensitive(ellipsoid_arc_all_ofJSON, "uncertaintyRadius");
    if (!uncertainty_radius) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_parseFromJSON() failed [uncertainty_radius]");
        goto end;
    }
    if (!cJSON_IsNumber(uncertainty_radius)) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_parseFromJSON() failed [uncertainty_radius]");
        goto end;
    }

    offset_angle = cJSON_GetObjectItemCaseSensitive(ellipsoid_arc_all_ofJSON, "offsetAngle");
    if (!offset_angle) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_parseFromJSON() failed [offset_angle]");
        goto end;
    }
    if (!cJSON_IsNumber(offset_angle)) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_parseFromJSON() failed [offset_angle]");
        goto end;
    }

    included_angle = cJSON_GetObjectItemCaseSensitive(ellipsoid_arc_all_ofJSON, "includedAngle");
    if (!included_angle) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_parseFromJSON() failed [included_angle]");
        goto end;
    }
    if (!cJSON_IsNumber(included_angle)) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_parseFromJSON() failed [included_angle]");
        goto end;
    }

    confidence = cJSON_GetObjectItemCaseSensitive(ellipsoid_arc_all_ofJSON, "confidence");
    if (!confidence) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_parseFromJSON() failed [confidence]");
        goto end;
    }
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_parseFromJSON() failed [confidence]");
        goto end;
    }

    ellipsoid_arc_all_of_local_var = OpenAPI_ellipsoid_arc_all_of_create (
        point_local_nonprim,
        
        inner_radius->valuedouble,
        
        uncertainty_radius->valuedouble,
        
        offset_angle->valuedouble,
        
        included_angle->valuedouble,
        
        confidence->valuedouble
    );

    return ellipsoid_arc_all_of_local_var;
end:
    if (point_local_nonprim) {
        OpenAPI_geographical_coordinates_free(point_local_nonprim);
        point_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ellipsoid_arc_all_of_t *OpenAPI_ellipsoid_arc_all_of_copy(OpenAPI_ellipsoid_arc_all_of_t *dst, OpenAPI_ellipsoid_arc_all_of_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ellipsoid_arc_all_of_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ellipsoid_arc_all_of_convertToJSON() failed");
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

    OpenAPI_ellipsoid_arc_all_of_free(dst);
    dst = OpenAPI_ellipsoid_arc_all_of_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

