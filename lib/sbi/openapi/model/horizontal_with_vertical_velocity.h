/*
 * horizontal_with_vertical_velocity.h
 *
 * Horizontal and vertical velocity.
 */

#ifndef _OpenAPI_horizontal_with_vertical_velocity_H_
#define _OpenAPI_horizontal_with_vertical_velocity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "vertical_direction.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_horizontal_with_vertical_velocity_s OpenAPI_horizontal_with_vertical_velocity_t;
typedef struct OpenAPI_horizontal_with_vertical_velocity_s {
    float h_speed;
    int bearing;
    float v_speed;
    OpenAPI_vertical_direction_e v_direction;
} OpenAPI_horizontal_with_vertical_velocity_t;

OpenAPI_horizontal_with_vertical_velocity_t *OpenAPI_horizontal_with_vertical_velocity_create(
    float h_speed,
    int bearing,
    float v_speed,
    OpenAPI_vertical_direction_e v_direction
);
void OpenAPI_horizontal_with_vertical_velocity_free(OpenAPI_horizontal_with_vertical_velocity_t *horizontal_with_vertical_velocity);
OpenAPI_horizontal_with_vertical_velocity_t *OpenAPI_horizontal_with_vertical_velocity_parseFromJSON(cJSON *horizontal_with_vertical_velocityJSON);
cJSON *OpenAPI_horizontal_with_vertical_velocity_convertToJSON(OpenAPI_horizontal_with_vertical_velocity_t *horizontal_with_vertical_velocity);
OpenAPI_horizontal_with_vertical_velocity_t *OpenAPI_horizontal_with_vertical_velocity_copy(OpenAPI_horizontal_with_vertical_velocity_t *dst, OpenAPI_horizontal_with_vertical_velocity_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_horizontal_with_vertical_velocity_H_ */

