/*
 * range_direction.h
 *
 * Represents a distance and direction from a point A to a point B.
 */

#ifndef _OpenAPI_range_direction_H_
#define _OpenAPI_range_direction_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_range_direction_s OpenAPI_range_direction_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_range_direction_s {
    bool is_distance;
    double distance;
    bool is_azimuth_direction;
    int azimuth_direction;
    bool is_elevation_direction;
    int elevation_direction;
};

OpenAPI_range_direction_t *OpenAPI_range_direction_create(
    bool is_distance,
    double distance,
    bool is_azimuth_direction,
    int azimuth_direction,
    bool is_elevation_direction,
    int elevation_direction
);
void OpenAPI_range_direction_free(OpenAPI_range_direction_t *range_direction);
OpenAPI_range_direction_t *OpenAPI_range_direction_parseFromJSON(cJSON *range_directionJSON);
cJSON *OpenAPI_range_direction_convertToJSON(OpenAPI_range_direction_t *range_direction);
OpenAPI_range_direction_t *OpenAPI_range_direction_copy(OpenAPI_range_direction_t *dst, OpenAPI_range_direction_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_range_direction_H_ */

