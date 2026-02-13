/*
 * horizontal_velocity_with_uncertainty.h
 *
 * Horizontal velocity with speed uncertainty.
 */

#ifndef _OpenAPI_horizontal_velocity_with_uncertainty_H_
#define _OpenAPI_horizontal_velocity_with_uncertainty_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_horizontal_velocity_with_uncertainty_s OpenAPI_horizontal_velocity_with_uncertainty_t;
typedef struct OpenAPI_horizontal_velocity_with_uncertainty_s {
    float h_speed;
    int bearing;
    float h_uncertainty;
} OpenAPI_horizontal_velocity_with_uncertainty_t;

OpenAPI_horizontal_velocity_with_uncertainty_t *OpenAPI_horizontal_velocity_with_uncertainty_create(
    float h_speed,
    int bearing,
    float h_uncertainty
);
void OpenAPI_horizontal_velocity_with_uncertainty_free(OpenAPI_horizontal_velocity_with_uncertainty_t *horizontal_velocity_with_uncertainty);
OpenAPI_horizontal_velocity_with_uncertainty_t *OpenAPI_horizontal_velocity_with_uncertainty_parseFromJSON(cJSON *horizontal_velocity_with_uncertaintyJSON);
cJSON *OpenAPI_horizontal_velocity_with_uncertainty_convertToJSON(OpenAPI_horizontal_velocity_with_uncertainty_t *horizontal_velocity_with_uncertainty);
OpenAPI_horizontal_velocity_with_uncertainty_t *OpenAPI_horizontal_velocity_with_uncertainty_copy(OpenAPI_horizontal_velocity_with_uncertainty_t *dst, OpenAPI_horizontal_velocity_with_uncertainty_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_horizontal_velocity_with_uncertainty_H_ */

