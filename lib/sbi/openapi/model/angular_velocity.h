/*
 * angular_velocity.h
 *
 * Rate of change of an angle.
 */

#ifndef _OpenAPI_angular_velocity_H_
#define _OpenAPI_angular_velocity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_angular_velocity_s OpenAPI_angular_velocity_t;
#include "units_angular_velocity.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_angular_velocity_s {
    OpenAPI_units_angular_velocity_e units_angular_velocity;
    int angular_velocity;
    int a_velocity_uncertainty;
};

OpenAPI_angular_velocity_t *OpenAPI_angular_velocity_create(
    OpenAPI_units_angular_velocity_e units_angular_velocity,
    int angular_velocity,
    int a_velocity_uncertainty
);
void OpenAPI_angular_velocity_free(OpenAPI_angular_velocity_t *angular_velocity);
OpenAPI_angular_velocity_t *OpenAPI_angular_velocity_parseFromJSON(cJSON *angular_velocityJSON);
cJSON *OpenAPI_angular_velocity_convertToJSON(OpenAPI_angular_velocity_t *angular_velocity);
OpenAPI_angular_velocity_t *OpenAPI_angular_velocity_copy(OpenAPI_angular_velocity_t *dst, OpenAPI_angular_velocity_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_angular_velocity_H_ */

