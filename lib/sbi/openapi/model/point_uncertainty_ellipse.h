/*
 * point_uncertainty_ellipse.h
 *
 * Ellipsoid point with uncertainty ellipse.
 */

#ifndef _OpenAPI_point_uncertainty_ellipse_H_
#define _OpenAPI_point_uncertainty_ellipse_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "gad_shape.h"
#include "geographical_coordinates.h"
#include "supported_gad_shapes.h"
#include "uncertainty_ellipse.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_point_uncertainty_ellipse_s OpenAPI_point_uncertainty_ellipse_t;
typedef struct OpenAPI_point_uncertainty_ellipse_s {
    struct OpenAPI_supported_gad_shapes_s *shape;
    struct OpenAPI_geographical_coordinates_s *point;
    struct OpenAPI_uncertainty_ellipse_s *uncertainty_ellipse;
    int confidence;
} OpenAPI_point_uncertainty_ellipse_t;

OpenAPI_point_uncertainty_ellipse_t *OpenAPI_point_uncertainty_ellipse_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_geographical_coordinates_t *point,
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse,
    int confidence
);
void OpenAPI_point_uncertainty_ellipse_free(OpenAPI_point_uncertainty_ellipse_t *point_uncertainty_ellipse);
OpenAPI_point_uncertainty_ellipse_t *OpenAPI_point_uncertainty_ellipse_parseFromJSON(cJSON *point_uncertainty_ellipseJSON);
cJSON *OpenAPI_point_uncertainty_ellipse_convertToJSON(OpenAPI_point_uncertainty_ellipse_t *point_uncertainty_ellipse);
OpenAPI_point_uncertainty_ellipse_t *OpenAPI_point_uncertainty_ellipse_copy(OpenAPI_point_uncertainty_ellipse_t *dst, OpenAPI_point_uncertainty_ellipse_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_point_uncertainty_ellipse_H_ */

