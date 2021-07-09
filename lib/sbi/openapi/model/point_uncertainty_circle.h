/*
 * point_uncertainty_circle.h
 *
 * 
 */

#ifndef _OpenAPI_point_uncertainty_circle_H_
#define _OpenAPI_point_uncertainty_circle_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "gad_shape.h"
#include "geographical_coordinates.h"
#include "point_uncertainty_circle_all_of.h"
#include "supported_gad_shapes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_point_uncertainty_circle_s OpenAPI_point_uncertainty_circle_t;
typedef struct OpenAPI_point_uncertainty_circle_s {
    struct OpenAPI_supported_gad_shapes_s *shape;
    struct OpenAPI_geographical_coordinates_s *point;
    float uncertainty;
} OpenAPI_point_uncertainty_circle_t;

OpenAPI_point_uncertainty_circle_t *OpenAPI_point_uncertainty_circle_create(
    OpenAPI_supported_gad_shapes_t *shape,
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

