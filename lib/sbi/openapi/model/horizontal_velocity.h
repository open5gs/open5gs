/*
 * horizontal_velocity.h
 *
 * Horizontal velocity.
 */

#ifndef _OpenAPI_horizontal_velocity_H_
#define _OpenAPI_horizontal_velocity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_horizontal_velocity_s OpenAPI_horizontal_velocity_t;
typedef struct OpenAPI_horizontal_velocity_s {
    float h_speed;
    int bearing;
} OpenAPI_horizontal_velocity_t;

OpenAPI_horizontal_velocity_t *OpenAPI_horizontal_velocity_create(
    float h_speed,
    int bearing
);
void OpenAPI_horizontal_velocity_free(OpenAPI_horizontal_velocity_t *horizontal_velocity);
OpenAPI_horizontal_velocity_t *OpenAPI_horizontal_velocity_parseFromJSON(cJSON *horizontal_velocityJSON);
cJSON *OpenAPI_horizontal_velocity_convertToJSON(OpenAPI_horizontal_velocity_t *horizontal_velocity);
OpenAPI_horizontal_velocity_t *OpenAPI_horizontal_velocity_copy(OpenAPI_horizontal_velocity_t *dst, OpenAPI_horizontal_velocity_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_horizontal_velocity_H_ */

