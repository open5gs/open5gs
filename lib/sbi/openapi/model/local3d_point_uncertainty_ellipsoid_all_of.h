/*
 * local3d_point_uncertainty_ellipsoid_all_of.h
 *
 * 
 */

#ifndef _OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_H_
#define _OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "local_origin.h"
#include "relative_cartesian_location.h"
#include "uncertainty_ellipsoid.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_s OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_t;
typedef struct OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_s {
    struct OpenAPI_local_origin_s *local_origin;
    struct OpenAPI_relative_cartesian_location_s *point;
    struct OpenAPI_uncertainty_ellipsoid_s *uncertainty_ellipsoid;
    int confidence;
} OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_t;

OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_t *OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_create(
    OpenAPI_local_origin_t *local_origin,
    OpenAPI_relative_cartesian_location_t *point,
    OpenAPI_uncertainty_ellipsoid_t *uncertainty_ellipsoid,
    int confidence
);
void OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_free(OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_t *local3d_point_uncertainty_ellipsoid_all_of);
OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_t *OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_parseFromJSON(cJSON *local3d_point_uncertainty_ellipsoid_all_ofJSON);
cJSON *OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_convertToJSON(OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_t *local3d_point_uncertainty_ellipsoid_all_of);
OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_t *OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_copy(OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_t *dst, OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_local3d_point_uncertainty_ellipsoid_all_of_H_ */

