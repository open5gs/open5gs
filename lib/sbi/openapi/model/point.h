/*
 * point.h
 *
 * Ellipsoid Point.
 */

#ifndef _OpenAPI_point_H_
#define _OpenAPI_point_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_point_s OpenAPI_point_t;
#include "gad_shape.h"
#include "geographical_coordinates.h"
#include "supported_gad_shapes.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_point_s {
    OpenAPI_supported_gad_shapes_e shape;
    struct OpenAPI_geographical_coordinates_s *point;
};

OpenAPI_point_t *OpenAPI_point_create(
    OpenAPI_supported_gad_shapes_e shape,
    OpenAPI_geographical_coordinates_t *point
);
void OpenAPI_point_free(OpenAPI_point_t *point);
OpenAPI_point_t *OpenAPI_point_parseFromJSON(cJSON *pointJSON);
cJSON *OpenAPI_point_convertToJSON(OpenAPI_point_t *point);
OpenAPI_point_t *OpenAPI_point_copy(OpenAPI_point_t *dst, OpenAPI_point_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_point_H_ */

