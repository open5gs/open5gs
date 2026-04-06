/*
 * point_altitude_uncertainty.h
 *
 * Ellipsoid point with altitude and uncertainty ellipsoid.
 */

#ifndef _OpenAPI_point_altitude_uncertainty_H_
#define _OpenAPI_point_altitude_uncertainty_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_point_altitude_uncertainty_s OpenAPI_point_altitude_uncertainty_t;
#include "gad_shape.h"
#include "geographical_coordinates.h"
#include "supported_gad_shapes.h"
#include "uncertainty_ellipse.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_point_altitude_uncertainty_s {
    OpenAPI_supported_gad_shapes_e shape;
    struct OpenAPI_geographical_coordinates_s *point;
    double altitude;
    struct OpenAPI_uncertainty_ellipse_s *uncertainty_ellipse;
    float uncertainty_altitude;
    int confidence;
    bool is_v_confidence;
    int v_confidence;
};

OpenAPI_point_altitude_uncertainty_t *OpenAPI_point_altitude_uncertainty_create(
    OpenAPI_supported_gad_shapes_e shape,
    OpenAPI_geographical_coordinates_t *point,
    double altitude,
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse,
    float uncertainty_altitude,
    int confidence,
    bool is_v_confidence,
    int v_confidence
);
void OpenAPI_point_altitude_uncertainty_free(OpenAPI_point_altitude_uncertainty_t *point_altitude_uncertainty);
OpenAPI_point_altitude_uncertainty_t *OpenAPI_point_altitude_uncertainty_parseFromJSON(cJSON *point_altitude_uncertaintyJSON);
cJSON *OpenAPI_point_altitude_uncertainty_convertToJSON(OpenAPI_point_altitude_uncertainty_t *point_altitude_uncertainty);
OpenAPI_point_altitude_uncertainty_t *OpenAPI_point_altitude_uncertainty_copy(OpenAPI_point_altitude_uncertainty_t *dst, OpenAPI_point_altitude_uncertainty_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_point_altitude_uncertainty_H_ */

