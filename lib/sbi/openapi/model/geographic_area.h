/*
 * geographic_area.h
 *
 * 
 */

#ifndef _OpenAPI_geographic_area_H_
#define _OpenAPI_geographic_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ellipsoid_arc.h"
#include "geographical_coordinates.h"
#include "point.h"
#include "point_altitude.h"
#include "point_altitude_uncertainty.h"
#include "point_uncertainty_circle.h"
#include "point_uncertainty_ellipse.h"
#include "polygon.h"
#include "supported_gad_shapes.h"
#include "uncertainty_ellipse.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_geographic_area_s OpenAPI_geographic_area_t;
typedef struct OpenAPI_geographic_area_s {
    struct OpenAPI_supported_gad_shapes_s *shape;
    struct OpenAPI_geographical_coordinates_s *point;
    float uncertainty;
    struct OpenAPI_uncertainty_ellipse_s *uncertainty_ellipse;
    int confidence;
    OpenAPI_list_t *point_list;
    double altitude;
    float uncertainty_altitude;
    int inner_radius;
    float uncertainty_radius;
    int offset_angle;
    int included_angle;
} OpenAPI_geographic_area_t;

OpenAPI_geographic_area_t *OpenAPI_geographic_area_create(
    OpenAPI_supported_gad_shapes_t *shape,
    OpenAPI_geographical_coordinates_t *point,
    float uncertainty,
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse,
    int confidence,
    OpenAPI_list_t *point_list,
    double altitude,
    float uncertainty_altitude,
    int inner_radius,
    float uncertainty_radius,
    int offset_angle,
    int included_angle
);
void OpenAPI_geographic_area_free(OpenAPI_geographic_area_t *geographic_area);
OpenAPI_geographic_area_t *OpenAPI_geographic_area_parseFromJSON(cJSON *geographic_areaJSON);
cJSON *OpenAPI_geographic_area_convertToJSON(OpenAPI_geographic_area_t *geographic_area);
OpenAPI_geographic_area_t *OpenAPI_geographic_area_copy(OpenAPI_geographic_area_t *dst, OpenAPI_geographic_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_geographic_area_H_ */

