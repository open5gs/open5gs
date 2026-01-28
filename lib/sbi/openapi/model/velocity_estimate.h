/*
 * velocity_estimate.h
 *
 * Velocity estimate.
 */

#ifndef _OpenAPI_velocity_estimate_H_
#define _OpenAPI_velocity_estimate_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "horizontal_velocity.h"
#include "horizontal_velocity_with_uncertainty.h"
#include "horizontal_with_vertical_velocity.h"
#include "horizontal_with_vertical_velocity_and_uncertainty.h"
#include "vertical_direction.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_velocity_estimate_s OpenAPI_velocity_estimate_t;
typedef struct OpenAPI_velocity_estimate_s {
    float h_speed;
    int bearing;
    float v_speed;
    OpenAPI_vertical_direction_e v_direction;
    float h_uncertainty;
    float v_uncertainty;
} OpenAPI_velocity_estimate_t;

OpenAPI_velocity_estimate_t *OpenAPI_velocity_estimate_create(
    float h_speed,
    int bearing,
    float v_speed,
    OpenAPI_vertical_direction_e v_direction,
    float h_uncertainty,
    float v_uncertainty
);
void OpenAPI_velocity_estimate_free(OpenAPI_velocity_estimate_t *velocity_estimate);
OpenAPI_velocity_estimate_t *OpenAPI_velocity_estimate_parseFromJSON(cJSON *velocity_estimateJSON);
cJSON *OpenAPI_velocity_estimate_convertToJSON(OpenAPI_velocity_estimate_t *velocity_estimate);
OpenAPI_velocity_estimate_t *OpenAPI_velocity_estimate_copy(OpenAPI_velocity_estimate_t *dst, OpenAPI_velocity_estimate_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_velocity_estimate_H_ */

