/*
 * supported_gad_shapes.h
 *
 * Indicates supported GAD shapes.
 */

#ifndef _OpenAPI_supported_gad_shapes_H_
#define _OpenAPI_supported_gad_shapes_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_supported_gad_shapes_NULL = 0, OpenAPI_supported_gad_shapes_POINT, OpenAPI_supported_gad_shapes_POINT_UNCERTAINTY_CIRCLE, OpenAPI_supported_gad_shapes_POINT_UNCERTAINTY_ELLIPSE, OpenAPI_supported_gad_shapes_POLYGON, OpenAPI_supported_gad_shapes_POINT_ALTITUDE, OpenAPI_supported_gad_shapes_POINT_ALTITUDE_UNCERTAINTY, OpenAPI_supported_gad_shapes_ELLIPSOID_ARC, OpenAPI_supported_gad_shapes_LOCAL_2D_POINT_UNCERTAINTY_ELLIPSE, OpenAPI_supported_gad_shapes_LOCAL_3D_POINT_UNCERTAINTY_ELLIPSOID, OpenAPI_supported_gad_shapes_DISTANCE_DIRECTION, OpenAPI_supported_gad_shapes_RELATIVE_2D_LOCATION_UNCERTAINTY_ELLIPSE, OpenAPI_supported_gad_shapes_RELATIVE_3D_LOCATION_UNCERTAINTY_ELLIPSOID } OpenAPI_supported_gad_shapes_e;

char* OpenAPI_supported_gad_shapes_ToString(OpenAPI_supported_gad_shapes_e supported_gad_shapes);

OpenAPI_supported_gad_shapes_e OpenAPI_supported_gad_shapes_FromString(char* supported_gad_shapes);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_supported_gad_shapes_H_ */

