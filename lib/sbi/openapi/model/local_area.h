/*
 * local_area.h
 *
 * Local area specified by different shape
 */

#ifndef _OpenAPI_local_area_H_
#define _OpenAPI_local_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "local2d_point_uncertainty_ellipse.h"
#include "local3d_point_uncertainty_ellipsoid.h"
#include "local_origin.h"
#include "relative_cartesian_location.h"
#include "supported_gad_shapes.h"
#include "uncertainty_ellipse.h"
#include "uncertainty_ellipsoid.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_local_area_s OpenAPI_local_area_t;
typedef struct OpenAPI_local_area_s {
    struct OpenAPI_supported_gad_shapes_s *shape;
    struct OpenAPI_local_origin_s *local_origin;
    struct OpenAPI_relative_cartesian_location_s *point;
    struct OpenAPI_uncertainty_ellipse_s *uncertainty_ellipse;
    int confidence;
    struct OpenAPI_uncertainty_ellipsoid_s *uncertainty_ellipsoid;
} OpenAPI_local_area_t;

OpenAPI_local_area_t *OpenAPI_local_area_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_local_origin_t *local_origin,
    OpenAPI_relative_cartesian_location_t *point,
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse,
    int confidence,
    OpenAPI_uncertainty_ellipsoid_t *uncertainty_ellipsoid
);
void OpenAPI_local_area_free(OpenAPI_local_area_t *local_area);
OpenAPI_local_area_t *OpenAPI_local_area_parseFromJSON(cJSON *local_areaJSON);
cJSON *OpenAPI_local_area_convertToJSON(OpenAPI_local_area_t *local_area);
OpenAPI_local_area_t *OpenAPI_local_area_copy(OpenAPI_local_area_t *dst, OpenAPI_local_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_local_area_H_ */

