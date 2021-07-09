/*
 * point_altitude.h
 *
 * 
 */

#ifndef _OpenAPI_point_altitude_H_
#define _OpenAPI_point_altitude_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "gad_shape.h"
#include "geographical_coordinates.h"
#include "point_altitude_all_of.h"
#include "supported_gad_shapes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_point_altitude_s OpenAPI_point_altitude_t;
typedef struct OpenAPI_point_altitude_s {
    struct OpenAPI_supported_gad_shapes_s *shape;
    struct OpenAPI_geographical_coordinates_s *point;
    double altitude;
} OpenAPI_point_altitude_t;

OpenAPI_point_altitude_t *OpenAPI_point_altitude_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_geographical_coordinates_t *point,
    double altitude
);
void OpenAPI_point_altitude_free(OpenAPI_point_altitude_t *point_altitude);
OpenAPI_point_altitude_t *OpenAPI_point_altitude_parseFromJSON(cJSON *point_altitudeJSON);
cJSON *OpenAPI_point_altitude_convertToJSON(OpenAPI_point_altitude_t *point_altitude);
OpenAPI_point_altitude_t *OpenAPI_point_altitude_copy(OpenAPI_point_altitude_t *dst, OpenAPI_point_altitude_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_point_altitude_H_ */

