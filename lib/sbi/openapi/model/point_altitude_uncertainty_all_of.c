
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "point_altitude_uncertainty_all_of.h"

OpenAPI_point_altitude_uncertainty_all_of_t *OpenAPI_point_altitude_uncertainty_all_of_create(
    OpenAPI_geographical_coordinates_t *point,
    double altitude,
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse,
    float uncertainty_altitude,
    int confidence
)
{
    OpenAPI_point_altitude_uncertainty_all_of_t *point_altitude_uncertainty_all_of_local_var = ogs_malloc(sizeof(OpenAPI_point_altitude_uncertainty_all_of_t));
    ogs_assert(point_altitude_uncertainty_all_of_local_var);

    point_altitude_uncertainty_all_of_local_var->point = point;
    point_altitude_uncertainty_all_of_local_var->altitude = altitude;
    point_altitude_uncertainty_all_of_local_var->uncertainty_ellipse = uncertainty_ellipse;
    point_altitude_uncertainty_all_of_local_var->uncertainty_altitude = uncertainty_altitude;
    point_altitude_uncertainty_all_of_local_var->confidence = confidence;

    return point_altitude_uncertainty_all_of_local_var;
}

void OpenAPI_point_altitude_uncertainty_all_of_free(OpenAPI_point_altitude_uncertainty_all_of_t *point_altitude_uncertainty_all_of)
{
    if (NULL == point_altitude_uncertainty_all_of) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_geographical_coordinates_free(point_altitude_uncertainty_all_of->point);
    OpenAPI_uncertainty_ellipse_free(point_altitude_uncertainty_all_of->uncertainty_ellipse);
    ogs_free(point_altitude_uncertainty_all_of);
}

cJSON *OpenAPI_point_altitude_uncertainty_all_of_convertToJSON(OpenAPI_point_altitude_uncertainty_all_of_t *point_altitude_uncertainty_all_of)
{
    cJSON *item = NULL;

    if (point_altitude_uncertainty_all_of == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_convertToJSON() failed [PointAltitudeUncertainty_allOf]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *point_local_JSON = OpenAPI_geographical_coordinates_convertToJSON(point_altitude_uncertainty_all_of->point);
    if (point_local_JSON == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_convertToJSON() failed [point]");
        goto end;
    }
    cJSON_AddItemToObject(item, "point", point_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_convertToJSON() failed [point]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "altitude", point_altitude_uncertainty_all_of->altitude) == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_convertToJSON() failed [altitude]");
        goto end;
    }

    cJSON *uncertainty_ellipse_local_JSON = OpenAPI_uncertainty_ellipse_convertToJSON(point_altitude_uncertainty_all_of->uncertainty_ellipse);
    if (uncertainty_ellipse_local_JSON == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_convertToJSON() failed [uncertainty_ellipse]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uncertaintyEllipse", uncertainty_ellipse_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_convertToJSON() failed [uncertainty_ellipse]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "uncertaintyAltitude", point_altitude_uncertainty_all_of->uncertainty_altitude) == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_convertToJSON() failed [uncertainty_altitude]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "confidence", point_altitude_uncertainty_all_of->confidence) == NULL) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_convertToJSON() failed [confidence]");
        goto end;
    }

end:
    return item;
}

OpenAPI_point_altitude_uncertainty_all_of_t *OpenAPI_point_altitude_uncertainty_all_of_parseFromJSON(cJSON *point_altitude_uncertainty_all_ofJSON)
{
    OpenAPI_point_altitude_uncertainty_all_of_t *point_altitude_uncertainty_all_of_local_var = NULL;
    cJSON *point = cJSON_GetObjectItemCaseSensitive(point_altitude_uncertainty_all_ofJSON, "point");
    if (!point) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_parseFromJSON() failed [point]");
        goto end;
    }

    OpenAPI_geographical_coordinates_t *point_local_nonprim = NULL;
    point_local_nonprim = OpenAPI_geographical_coordinates_parseFromJSON(point);

    cJSON *altitude = cJSON_GetObjectItemCaseSensitive(point_altitude_uncertainty_all_ofJSON, "altitude");
    if (!altitude) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_parseFromJSON() failed [altitude]");
        goto end;
    }

    if (!cJSON_IsNumber(altitude)) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_parseFromJSON() failed [altitude]");
        goto end;
    }

    cJSON *uncertainty_ellipse = cJSON_GetObjectItemCaseSensitive(point_altitude_uncertainty_all_ofJSON, "uncertaintyEllipse");
    if (!uncertainty_ellipse) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_parseFromJSON() failed [uncertainty_ellipse]");
        goto end;
    }

    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse_local_nonprim = NULL;
    uncertainty_ellipse_local_nonprim = OpenAPI_uncertainty_ellipse_parseFromJSON(uncertainty_ellipse);

    cJSON *uncertainty_altitude = cJSON_GetObjectItemCaseSensitive(point_altitude_uncertainty_all_ofJSON, "uncertaintyAltitude");
    if (!uncertainty_altitude) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_parseFromJSON() failed [uncertainty_altitude]");
        goto end;
    }

    if (!cJSON_IsNumber(uncertainty_altitude)) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_parseFromJSON() failed [uncertainty_altitude]");
        goto end;
    }

    cJSON *confidence = cJSON_GetObjectItemCaseSensitive(point_altitude_uncertainty_all_ofJSON, "confidence");
    if (!confidence) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_parseFromJSON() failed [confidence]");
        goto end;
    }

    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_parseFromJSON() failed [confidence]");
        goto end;
    }

    point_altitude_uncertainty_all_of_local_var = OpenAPI_point_altitude_uncertainty_all_of_create (
        point_local_nonprim,
        
        altitude->valuedouble,
        uncertainty_ellipse_local_nonprim,
        
        uncertainty_altitude->valuedouble,
        
        confidence->valuedouble
    );

    return point_altitude_uncertainty_all_of_local_var;
end:
    return NULL;
}

OpenAPI_point_altitude_uncertainty_all_of_t *OpenAPI_point_altitude_uncertainty_all_of_copy(OpenAPI_point_altitude_uncertainty_all_of_t *dst, OpenAPI_point_altitude_uncertainty_all_of_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_point_altitude_uncertainty_all_of_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_point_altitude_uncertainty_all_of_convertToJSON() failed");
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

    OpenAPI_point_altitude_uncertainty_all_of_free(dst);
    dst = OpenAPI_point_altitude_uncertainty_all_of_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

