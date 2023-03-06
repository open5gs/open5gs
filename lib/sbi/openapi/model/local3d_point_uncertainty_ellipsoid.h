/*
 * local3d_point_uncertainty_ellipsoid.h
 *
 * Local 3D point with uncertainty ellipsoid
 */

#ifndef _OpenAPI_local3d_point_uncertainty_ellipsoid_H_
#define _OpenAPI_local3d_point_uncertainty_ellipsoid_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "gad_shape.h"
#include "local_origin.h"
#include "relative_cartesian_location.h"
#include "supported_gad_shapes.h"
#include "uncertainty_ellipsoid.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_local3d_point_uncertainty_ellipsoid_s OpenAPI_local3d_point_uncertainty_ellipsoid_t;
typedef struct OpenAPI_local3d_point_uncertainty_ellipsoid_s {
    struct OpenAPI_supported_gad_shapes_s *shape;
    struct OpenAPI_local_origin_s *local_origin;
    struct OpenAPI_relative_cartesian_location_s *point;
    struct OpenAPI_uncertainty_ellipsoid_s *uncertainty_ellipsoid;
    int confidence;
} OpenAPI_local3d_point_uncertainty_ellipsoid_t;

OpenAPI_local3d_point_uncertainty_ellipsoid_t *OpenAPI_local3d_point_uncertainty_ellipsoid_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_local_origin_t *local_origin,
    OpenAPI_relative_cartesian_location_t *point,
    OpenAPI_uncertainty_ellipsoid_t *uncertainty_ellipsoid,
    int confidence
);
void OpenAPI_local3d_point_uncertainty_ellipsoid_free(OpenAPI_local3d_point_uncertainty_ellipsoid_t *local3d_point_uncertainty_ellipsoid);
OpenAPI_local3d_point_uncertainty_ellipsoid_t *OpenAPI_local3d_point_uncertainty_ellipsoid_parseFromJSON(cJSON *local3d_point_uncertainty_ellipsoidJSON);
cJSON *OpenAPI_local3d_point_uncertainty_ellipsoid_convertToJSON(OpenAPI_local3d_point_uncertainty_ellipsoid_t *local3d_point_uncertainty_ellipsoid);
OpenAPI_local3d_point_uncertainty_ellipsoid_t *OpenAPI_local3d_point_uncertainty_ellipsoid_copy(OpenAPI_local3d_point_uncertainty_ellipsoid_t *dst, OpenAPI_local3d_point_uncertainty_ellipsoid_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_local3d_point_uncertainty_ellipsoid_H_ */

