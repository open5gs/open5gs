
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "point_altitude_uncertainty.h"

OpenAPI_point_altitude_uncertainty_t *OpenAPI_point_altitude_uncertainty_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_geographical_coordinates_t *point,
    double altitude,
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse,
    float uncertainty_altitude,
    int confidence
)
{
    OpenAPI_point_altitude_uncertainty_t *point_altitude_uncertainty_local_var = ogs_malloc(sizeof(OpenAPI_point_altitude_uncertainty_t));
    ogs_assert(point_altitude_uncertainty_local_var);

    point_altitude_uncertainty_local_var->shape = shape;
    point_altitude_uncertainty_local_var->point = point;
    point_altitude_uncertainty_local_var->altitude = altitude;
    point_altitude_uncertainty_local_var->uncertainty_ellipse = uncertainty_ellipse;
    point_altitude_uncertainty_local_var->uncertainty_altitude = uncertainty_altitude;
    point_altitude_uncertainty_local_var->confidence = confidence;

    return point_altitude_uncertainty_local_var;
}

void OpenAPI_point_altitude_uncertainty_free(OpenAPI_point_altitude_uncertainty_t *point_altitude_uncertainty)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == point_altitude_uncertainty) {
        return;
    }
    if (point_altitude_uncertainty->shape) {
        OpenAPI_supported_gad_shapes_free(point_altitude_uncertainty->shape);
        point_altitude_uncertainty->shape = NULL;
    }
    if (point_altitude_uncertainty->point) {
        OpenAPI_geographical_coordinates_free(point_altitude_uncertainty->point);
        point_altitude_uncertainty->point = NULL;
    }
    if (point_altitude_uncertainty->uncertainty_ellipse) {
        OpenAPI_uncertainty_ellipse_free(point_altitude_uncertainty->uncertainty_ellipse);
        point_altitude_uncertainty->uncertainty_ellipse = NULL;
    }
    ogs_free(point_altitude_uncertainty);
}

cJSON *OpenAPI_point_altitude_uncertainty_convertToJSON(OpenAPI_point_altitude_uncertainty_t *point_altitude_uncertainty)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (point_altitude_uncertainty == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed [PointAltitudeUncertainty]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!point_altitude_uncertainty->shape) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed [shape]");
        return NULL;
    }
    cJSON *shape_local_JSON = OpenAPI_supported_gad_shapes_convertToJSON(point_altitude_uncertainty->shape);
    if (shape_local_JSON == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed [shape]");
        goto end;
    }
    cJSON_AddItemToObject(item, "shape", shape_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed [shape]");
        goto end;
    }

    if (!point_altitude_uncertainty->point) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed [point]");
        return NULL;
    }
    cJSON *point_local_JSON = OpenAPI_geographical_coordinates_convertToJSON(point_altitude_uncertainty->point);
    if (point_local_JSON == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed [point]");
        goto end;
    }
    cJSON_AddItemToObject(item, "point", point_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed [point]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "altitude", point_altitude_uncertainty->altitude) == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed [altitude]");
        goto end;
    }

    if (!point_altitude_uncertainty->uncertainty_ellipse) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed [uncertainty_ellipse]");
        return NULL;
    }
    cJSON *uncertainty_ellipse_local_JSON = OpenAPI_uncertainty_ellipse_convertToJSON(point_altitude_uncertainty->uncertainty_ellipse);
    if (uncertainty_ellipse_local_JSON == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed [uncertainty_ellipse]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uncertaintyEllipse", uncertainty_ellipse_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed [uncertainty_ellipse]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "uncertaintyAltitude", point_altitude_uncertainty->uncertainty_altitude) == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed [uncertainty_altitude]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "confidence", point_altitude_uncertainty->confidence) == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed [confidence]");
        goto end;
    }

end:
    return item;
}

OpenAPI_point_altitude_uncertainty_t *OpenAPI_point_altitude_uncertainty_parseFromJSON(cJSON *point_altitude_uncertaintyJSON)
{
    OpenAPI_point_altitude_uncertainty_t *point_altitude_uncertainty_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *shape = NULL;
    OpenAPI_supported_gad_shapes_t *shape_local_nonprim = NULL;
    cJSON *point = NULL;
    OpenAPI_geographical_coordinates_t *point_local_nonprim = NULL;
    cJSON *altitude = NULL;
    cJSON *uncertainty_ellipse = NULL;
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse_local_nonprim = NULL;
    cJSON *uncertainty_altitude = NULL;
    cJSON *confidence = NULL;
    shape = cJSON_GetObjectItemCaseSensitive(point_altitude_uncertaintyJSON, "shape");
    if (!shape) {
        ogs_error("OpenAPI_point_altitude_uncertainty_parseFromJSON() failed [shape]");
        goto end;
    }
    shape_local_nonprim = OpenAPI_supported_gad_shapes_parseFromJSON(shape);
    if (!shape_local_nonprim) {
        ogs_error("OpenAPI_supported_gad_shapes_parseFromJSON failed [shape]");
        goto end;
    }

    point = cJSON_GetObjectItemCaseSensitive(point_altitude_uncertaintyJSON, "point");
    if (!point) {
        ogs_error("OpenAPI_point_altitude_uncertainty_parseFromJSON() failed [point]");
        goto end;
    }
    point_local_nonprim = OpenAPI_geographical_coordinates_parseFromJSON(point);
    if (!point_local_nonprim) {
        ogs_error("OpenAPI_geographical_coordinates_parseFromJSON failed [point]");
        goto end;
    }

    altitude = cJSON_GetObjectItemCaseSensitive(point_altitude_uncertaintyJSON, "altitude");
    if (!altitude) {
        ogs_error("OpenAPI_point_altitude_uncertainty_parseFromJSON() failed [altitude]");
        goto end;
    }
    if (!cJSON_IsNumber(altitude)) {
        ogs_error("OpenAPI_point_altitude_uncertainty_parseFromJSON() failed [altitude]");
        goto end;
    }

    uncertainty_ellipse = cJSON_GetObjectItemCaseSensitive(point_altitude_uncertaintyJSON, "uncertaintyEllipse");
    if (!uncertainty_ellipse) {
        ogs_error("OpenAPI_point_altitude_uncertainty_parseFromJSON() failed [uncertainty_ellipse]");
        goto end;
    }
    uncertainty_ellipse_local_nonprim = OpenAPI_uncertainty_ellipse_parseFromJSON(uncertainty_ellipse);
    if (!uncertainty_ellipse_local_nonprim) {
        ogs_error("OpenAPI_uncertainty_ellipse_parseFromJSON failed [uncertainty_ellipse]");
        goto end;
    }

    uncertainty_altitude = cJSON_GetObjectItemCaseSensitive(point_altitude_uncertaintyJSON, "uncertaintyAltitude");
    if (!uncertainty_altitude) {
        ogs_error("OpenAPI_point_altitude_uncertainty_parseFromJSON() failed [uncertainty_altitude]");
        goto end;
    }
    if (!cJSON_IsNumber(uncertainty_altitude)) {
        ogs_error("OpenAPI_point_altitude_uncertainty_parseFromJSON() failed [uncertainty_altitude]");
        goto end;
    }

    confidence = cJSON_GetObjectItemCaseSensitive(point_altitude_uncertaintyJSON, "confidence");
    if (!confidence) {
        ogs_error("OpenAPI_point_altitude_uncertainty_parseFromJSON() failed [confidence]");
        goto end;
    }
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_point_altitude_uncertainty_parseFromJSON() failed [confidence]");
        goto end;
    }

    point_altitude_uncertainty_local_var = OpenAPI_point_altitude_uncertainty_create (
        shape_local_nonprim,
        point_local_nonprim,
        
        altitude->valuedouble,
        uncertainty_ellipse_local_nonprim,
        
        uncertainty_altitude->valuedouble,
        
        confidence->valuedouble
    );

    return point_altitude_uncertainty_local_var;
end:
    if (shape_local_nonprim) {
        OpenAPI_supported_gad_shapes_free(shape_local_nonprim);
        shape_local_nonprim = NULL;
    }
    if (point_local_nonprim) {
        OpenAPI_geographical_coordinates_free(point_local_nonprim);
        point_local_nonprim = NULL;
    }
    if (uncertainty_ellipse_local_nonprim) {
        OpenAPI_uncertainty_ellipse_free(uncertainty_ellipse_local_nonprim);
        uncertainty_ellipse_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_point_altitude_uncertainty_t *OpenAPI_point_altitude_uncertainty_copy(OpenAPI_point_altitude_uncertainty_t *dst, OpenAPI_point_altitude_uncertainty_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_point_altitude_uncertainty_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_point_altitude_uncertainty_convertToJSON() failed");
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

    OpenAPI_point_altitude_uncertainty_free(dst);
    dst = OpenAPI_point_altitude_uncertainty_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

