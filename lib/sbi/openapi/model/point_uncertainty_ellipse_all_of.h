/*
 * point_uncertainty_ellipse_all_of.h
 *
 *
 */

#ifndef _OpenAPI_point_uncertainty_ellipse_all_of_H_
#define _OpenAPI_point_uncertainty_ellipse_all_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "geographical_coordinates.h"
#include "uncertainty_ellipse.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_point_uncertainty_ellipse_all_of_s OpenAPI_point_uncertainty_ellipse_all_of_t;
typedef struct OpenAPI_point_uncertainty_ellipse_all_of_s {
    struct OpenAPI_geographical_coordinates_s *point;
    struct OpenAPI_uncertainty_ellipse_s *uncertainty_ellipse;
    int confidence;
} OpenAPI_point_uncertainty_ellipse_all_of_t;

OpenAPI_point_uncertainty_ellipse_all_of_t *OpenAPI_point_uncertainty_ellipse_all_of_create(
    OpenAPI_geographical_coordinates_t *point,
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse,
    int confidence
    );
void OpenAPI_point_uncertainty_ellipse_all_of_free(OpenAPI_point_uncertainty_ellipse_all_of_t *point_uncertainty_ellipse_all_of);
OpenAPI_point_uncertainty_ellipse_all_of_t *OpenAPI_point_uncertainty_ellipse_all_of_parseFromJSON(cJSON *point_uncertainty_ellipse_all_ofJSON);
cJSON *OpenAPI_point_uncertainty_ellipse_all_of_convertToJSON(OpenAPI_point_uncertainty_ellipse_all_of_t *point_uncertainty_ellipse_all_of);
OpenAPI_point_uncertainty_ellipse_all_of_t *OpenAPI_point_uncertainty_ellipse_all_of_copy(OpenAPI_point_uncertainty_ellipse_all_of_t *dst, OpenAPI_point_uncertainty_ellipse_all_of_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_point_uncertainty_ellipse_all_of_H_ */

