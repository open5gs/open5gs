/*
 * supported_gad_shapes_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_supported_gad_shapes_any_of_H_
#define _OpenAPI_supported_gad_shapes_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_supported_gad_shapes_any_of_NULL = 0, OpenAPI_supported_gad_shapes_any_of_POINT, OpenAPI_supported_gad_shapes_any_of_POINT_UNCERTAINTY_CIRCLE, OpenAPI_supported_gad_shapes_any_of_POINT_UNCERTAINTY_ELLIPSE, OpenAPI_supported_gad_shapes_any_of_POLYGON, OpenAPI_supported_gad_shapes_any_of_POINT_ALTITUDE, OpenAPI_supported_gad_shapes_any_of_POINT_ALTITUDE_UNCERTAINTY, OpenAPI_supported_gad_shapes_any_of_ELLIPSOID_ARC } OpenAPI_supported_gad_shapes_any_of_e;

char* OpenAPI_supported_gad_shapes_any_of_ToString(OpenAPI_supported_gad_shapes_any_of_e supported_gad_shapes_any_of);

OpenAPI_supported_gad_shapes_any_of_e OpenAPI_supported_gad_shapes_any_of_FromString(char* supported_gad_shapes_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_supported_gad_shapes_any_of_H_ */

