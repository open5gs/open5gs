/*
 * serving_location.h
 *
 * 
 */

#ifndef _OpenAPI_serving_location_H_
#define _OpenAPI_serving_location_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_serving_location_s OpenAPI_serving_location_t;
#include "geo_loc.h"
#include "topological_service_area.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_serving_location_s {
    struct OpenAPI_geo_loc_s *geographical_location;
    struct OpenAPI_topological_service_area_s *topological_location;
};

OpenAPI_serving_location_t *OpenAPI_serving_location_create(
    OpenAPI_geo_loc_t *geographical_location,
    OpenAPI_topological_service_area_t *topological_location
);
void OpenAPI_serving_location_free(OpenAPI_serving_location_t *serving_location);
OpenAPI_serving_location_t *OpenAPI_serving_location_parseFromJSON(cJSON *serving_locationJSON);
cJSON *OpenAPI_serving_location_convertToJSON(OpenAPI_serving_location_t *serving_location);
OpenAPI_serving_location_t *OpenAPI_serving_location_copy(OpenAPI_serving_location_t *dst, OpenAPI_serving_location_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_serving_location_H_ */

