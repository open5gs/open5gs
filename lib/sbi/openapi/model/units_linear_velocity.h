/*
 * units_linear_velocity.h
 *
 * The the units of linear velocity.
 */

#ifndef _OpenAPI_units_linear_velocity_H_
#define _OpenAPI_units_linear_velocity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_units_linear_velocity_NULL = 0, OpenAPI_units_linear_velocity_MPERS, OpenAPI_units_linear_velocity_CMPERS } OpenAPI_units_linear_velocity_e;

char* OpenAPI_units_linear_velocity_ToString(OpenAPI_units_linear_velocity_e units_linear_velocity);

OpenAPI_units_linear_velocity_e OpenAPI_units_linear_velocity_FromString(char* units_linear_velocity);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_units_linear_velocity_H_ */

