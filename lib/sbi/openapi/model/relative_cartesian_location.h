/*
 * relative_cartesian_location.h
 *
 * Relative Cartesian Location
 */

#ifndef _OpenAPI_relative_cartesian_location_H_
#define _OpenAPI_relative_cartesian_location_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_relative_cartesian_location_s OpenAPI_relative_cartesian_location_t;
typedef struct OpenAPI_relative_cartesian_location_s {
    float x;
    float y;
    bool is_z;
    float z;
} OpenAPI_relative_cartesian_location_t;

OpenAPI_relative_cartesian_location_t *OpenAPI_relative_cartesian_location_create(
    float x,
    float y,
    bool is_z,
    float z
);
void OpenAPI_relative_cartesian_location_free(OpenAPI_relative_cartesian_location_t *relative_cartesian_location);
OpenAPI_relative_cartesian_location_t *OpenAPI_relative_cartesian_location_parseFromJSON(cJSON *relative_cartesian_locationJSON);
cJSON *OpenAPI_relative_cartesian_location_convertToJSON(OpenAPI_relative_cartesian_location_t *relative_cartesian_location);
OpenAPI_relative_cartesian_location_t *OpenAPI_relative_cartesian_location_copy(OpenAPI_relative_cartesian_location_t *dst, OpenAPI_relative_cartesian_location_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_relative_cartesian_location_H_ */

