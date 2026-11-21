/*
 * geo_loc.h
 *
 * 
 */

#ifndef _OpenAPI_geo_loc_H_
#define _OpenAPI_geo_loc_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_geo_loc_s OpenAPI_geo_loc_t;
#include "charging_geographical_coordinates.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_geo_loc_s {
    struct OpenAPI_charging_geographical_coordinates_s *geographical_coordinates;
    char *civic_location;
};

OpenAPI_geo_loc_t *OpenAPI_geo_loc_create(
    OpenAPI_charging_geographical_coordinates_t *geographical_coordinates,
    char *civic_location
);
void OpenAPI_geo_loc_free(OpenAPI_geo_loc_t *geo_loc);
OpenAPI_geo_loc_t *OpenAPI_geo_loc_parseFromJSON(cJSON *geo_locJSON);
cJSON *OpenAPI_geo_loc_convertToJSON(OpenAPI_geo_loc_t *geo_loc);
OpenAPI_geo_loc_t *OpenAPI_geo_loc_copy(OpenAPI_geo_loc_t *dst, OpenAPI_geo_loc_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_geo_loc_H_ */

