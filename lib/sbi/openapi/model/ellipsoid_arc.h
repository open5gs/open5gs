/*
 * ellipsoid_arc.h
 *
 * Ellipsoid Arc.
 */

#ifndef _OpenAPI_ellipsoid_arc_H_
#define _OpenAPI_ellipsoid_arc_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "gad_shape.h"
#include "geographical_coordinates.h"
#include "supported_gad_shapes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ellipsoid_arc_s OpenAPI_ellipsoid_arc_t;
typedef struct OpenAPI_ellipsoid_arc_s {
    struct OpenAPI_supported_gad_shapes_s *shape;
    struct OpenAPI_geographical_coordinates_s *point;
    int inner_radius;
    float uncertainty_radius;
    int offset_angle;
    int included_angle;
    int confidence;
} OpenAPI_ellipsoid_arc_t;

OpenAPI_ellipsoid_arc_t *OpenAPI_ellipsoid_arc_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_geographical_coordinates_t *point,
    int inner_radius,
    float uncertainty_radius,
    int offset_angle,
    int included_angle,
    int confidence
);
void OpenAPI_ellipsoid_arc_free(OpenAPI_ellipsoid_arc_t *ellipsoid_arc);
OpenAPI_ellipsoid_arc_t *OpenAPI_ellipsoid_arc_parseFromJSON(cJSON *ellipsoid_arcJSON);
cJSON *OpenAPI_ellipsoid_arc_convertToJSON(OpenAPI_ellipsoid_arc_t *ellipsoid_arc);
OpenAPI_ellipsoid_arc_t *OpenAPI_ellipsoid_arc_copy(OpenAPI_ellipsoid_arc_t *dst, OpenAPI_ellipsoid_arc_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ellipsoid_arc_H_ */

