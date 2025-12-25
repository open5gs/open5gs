/*
 * horizontal_with_vertical_velocity_and_uncertainty.h
 *
 * Horizontal and vertical velocity with speed uncertainty.
 */

#ifndef _OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_H_
#define _OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "vertical_direction.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_s OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t;
typedef struct OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_s {
    float h_speed;
    int bearing;
    float v_speed;
    OpenAPI_vertical_direction_e v_direction;
    float h_uncertainty;
    float v_uncertainty;
} OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t;

OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_create(
    float h_speed,
    int bearing,
    float v_speed,
    OpenAPI_vertical_direction_e v_direction,
    float h_uncertainty,
    float v_uncertainty
);
void OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_free(OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *horizontal_with_vertical_velocity_and_uncertainty);
OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON(cJSON *horizontal_with_vertical_velocity_and_uncertaintyJSON);
cJSON *OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_convertToJSON(OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *horizontal_with_vertical_velocity_and_uncertainty);
OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_copy(OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *dst, OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_H_ */

