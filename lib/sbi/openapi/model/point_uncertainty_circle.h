/*
 * point_uncertainty_circle.h
 *
 * Ellipsoid point with uncertainty circle.
 */

#ifndef _OpenAPI_point_uncertainty_circle_H_
#define _OpenAPI_point_uncertainty_circle_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_point_uncertainty_circle_s OpenAPI_point_uncertainty_circle_t;
#include "gad_shape.h"
#include "geographical_coordinates.h"
#include "supported_gad_shapes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_point_uncertainty_circle_s {
    OpenAPI_supported_gad_shapes_e shape;
    struct OpenAPI_geographical_coordinates_s *point;
    float uncertainty;
};

OpenAPI_point_uncertainty_circle_t *OpenAPI_point_uncertainty_circle_create(
    OpenAPI_supported_gad_shapes_e shape,
    OpenAPI_geographical_coordinates_t *point,
    float uncertainty
);
void OpenAPI_point_uncertainty_circle_free(OpenAPI_point_uncertainty_circle_t *point_uncertainty_circle);
OpenAPI_point_uncertainty_circle_t *OpenAPI_point_uncertainty_circle_parseFromJSON(cJSON *point_uncertainty_circleJSON);
cJSON *OpenAPI_point_uncertainty_circle_convertToJSON(OpenAPI_point_uncertainty_circle_t *point_uncertainty_circle);
OpenAPI_point_uncertainty_circle_t *OpenAPI_point_uncertainty_circle_copy(OpenAPI_point_uncertainty_circle_t *dst, OpenAPI_point_uncertainty_circle_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_point_uncertainty_circle_H_ */

