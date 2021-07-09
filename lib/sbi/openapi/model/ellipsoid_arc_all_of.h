/*
 * ellipsoid_arc_all_of.h
 *
 * 
 */

#ifndef _OpenAPI_ellipsoid_arc_all_of_H_
#define _OpenAPI_ellipsoid_arc_all_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "geographical_coordinates.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ellipsoid_arc_all_of_s OpenAPI_ellipsoid_arc_all_of_t;
typedef struct OpenAPI_ellipsoid_arc_all_of_s {
    struct OpenAPI_geographical_coordinates_s *point;
    int inner_radius;
    float uncertainty_radius;
    int offset_angle;
    int included_angle;
    int confidence;
} OpenAPI_ellipsoid_arc_all_of_t;

OpenAPI_ellipsoid_arc_all_of_t *OpenAPI_ellipsoid_arc_all_of_create(
    OpenAPI_geographical_coordinates_t *point,
    int inner_radius,
    float uncertainty_radius,
    int offset_angle,
    int included_angle,
    int confidence
);
void OpenAPI_ellipsoid_arc_all_of_free(OpenAPI_ellipsoid_arc_all_of_t *ellipsoid_arc_all_of);
OpenAPI_ellipsoid_arc_all_of_t *OpenAPI_ellipsoid_arc_all_of_parseFromJSON(cJSON *ellipsoid_arc_all_ofJSON);
cJSON *OpenAPI_ellipsoid_arc_all_of_convertToJSON(OpenAPI_ellipsoid_arc_all_of_t *ellipsoid_arc_all_of);
OpenAPI_ellipsoid_arc_all_of_t *OpenAPI_ellipsoid_arc_all_of_copy(OpenAPI_ellipsoid_arc_all_of_t *dst, OpenAPI_ellipsoid_arc_all_of_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ellipsoid_arc_all_of_H_ */

