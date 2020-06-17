/*
 * point.h
 *
 *
 */

#ifndef _OpenAPI_point_H_
#define _OpenAPI_point_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "gad_shape.h"
#include "geographical_coordinates.h"
#include "point_all_of.h"
#include "supported_gad_shapes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_point_s OpenAPI_point_t;
typedef struct OpenAPI_point_s {
    struct OpenAPI_supported_gad_shapes_s *shape;
    struct OpenAPI_geographical_coordinates_s *point;
} OpenAPI_point_t;

OpenAPI_point_t *OpenAPI_point_create(
    OpenAPI_supported_gad_shapes_t *shape,
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

