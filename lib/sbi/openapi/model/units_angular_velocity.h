/*
 * units_angular_velocity.h
 *
 * The units of angular velocity.
 */

#ifndef _OpenAPI_units_angular_velocity_H_
#define _OpenAPI_units_angular_velocity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_units_angular_velocity_NULL = 0, OpenAPI_units_angular_velocity_DEGPERSEC1, OpenAPI_units_angular_velocity_DEGPERSEC01 } OpenAPI_units_angular_velocity_e;

char* OpenAPI_units_angular_velocity_ToString(OpenAPI_units_angular_velocity_e units_angular_velocity);

OpenAPI_units_angular_velocity_e OpenAPI_units_angular_velocity_FromString(char* units_angular_velocity);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_units_angular_velocity_H_ */

