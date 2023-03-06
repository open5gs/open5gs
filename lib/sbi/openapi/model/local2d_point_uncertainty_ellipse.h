/*
 * local2d_point_uncertainty_ellipse.h
 *
 * Local 2D point with uncertainty ellipse
 */

#ifndef _OpenAPI_local2d_point_uncertainty_ellipse_H_
#define _OpenAPI_local2d_point_uncertainty_ellipse_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "gad_shape.h"
#include "local_origin.h"
#include "relative_cartesian_location.h"
#include "supported_gad_shapes.h"
#include "uncertainty_ellipse.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_local2d_point_uncertainty_ellipse_s OpenAPI_local2d_point_uncertainty_ellipse_t;
typedef struct OpenAPI_local2d_point_uncertainty_ellipse_s {
    struct OpenAPI_supported_gad_shapes_s *shape;
    struct OpenAPI_local_origin_s *local_origin;
    struct OpenAPI_relative_cartesian_location_s *point;
    struct OpenAPI_uncertainty_ellipse_s *uncertainty_ellipse;
    int confidence;
} OpenAPI_local2d_point_uncertainty_ellipse_t;

OpenAPI_local2d_point_uncertainty_ellipse_t *OpenAPI_local2d_point_uncertainty_ellipse_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_local_origin_t *local_origin,
    OpenAPI_relative_cartesian_location_t *point,
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse,
    int confidence
);
void OpenAPI_local2d_point_uncertainty_ellipse_free(OpenAPI_local2d_point_uncertainty_ellipse_t *local2d_point_uncertainty_ellipse);
OpenAPI_local2d_point_uncertainty_ellipse_t *OpenAPI_local2d_point_uncertainty_ellipse_parseFromJSON(cJSON *local2d_point_uncertainty_ellipseJSON);
cJSON *OpenAPI_local2d_point_uncertainty_ellipse_convertToJSON(OpenAPI_local2d_point_uncertainty_ellipse_t *local2d_point_uncertainty_ellipse);
OpenAPI_local2d_point_uncertainty_ellipse_t *OpenAPI_local2d_point_uncertainty_ellipse_copy(OpenAPI_local2d_point_uncertainty_ellipse_t *dst, OpenAPI_local2d_point_uncertainty_ellipse_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_local2d_point_uncertainty_ellipse_H_ */

