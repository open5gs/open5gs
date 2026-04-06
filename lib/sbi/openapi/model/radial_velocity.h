/*
 * radial_velocity.h
 *
 * Rate of change of a range.
 */

#ifndef _OpenAPI_radial_velocity_H_
#define _OpenAPI_radial_velocity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_radial_velocity_s OpenAPI_radial_velocity_t;
#include "units_linear_velocity.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_radial_velocity_s {
    OpenAPI_units_linear_velocity_e units_radial_velocity;
    int radial_velocity;
    int r_velocity_uncertainty;
};

OpenAPI_radial_velocity_t *OpenAPI_radial_velocity_create(
    OpenAPI_units_linear_velocity_e units_radial_velocity,
    int radial_velocity,
    int r_velocity_uncertainty
);
void OpenAPI_radial_velocity_free(OpenAPI_radial_velocity_t *radial_velocity);
OpenAPI_radial_velocity_t *OpenAPI_radial_velocity_parseFromJSON(cJSON *radial_velocityJSON);
cJSON *OpenAPI_radial_velocity_convertToJSON(OpenAPI_radial_velocity_t *radial_velocity);
OpenAPI_radial_velocity_t *OpenAPI_radial_velocity_copy(OpenAPI_radial_velocity_t *dst, OpenAPI_radial_velocity_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_radial_velocity_H_ */

