/*
 * steer_mode_value.h
 *
 * 
 */

#ifndef _OpenAPI_steer_mode_value_H_
#define _OpenAPI_steer_mode_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_steer_mode_value_NULL = 0, OpenAPI_steer_mode_value_ACTIVE_STANDBY, OpenAPI_steer_mode_value_LOAD_BALANCING, OpenAPI_steer_mode_value_SMALLEST_DELAY, OpenAPI_steer_mode_value_PRIORITY_BASED } OpenAPI_steer_mode_value_e;

char* OpenAPI_steer_mode_value_ToString(OpenAPI_steer_mode_value_e steer_mode_value);

OpenAPI_steer_mode_value_e OpenAPI_steer_mode_value_FromString(char* steer_mode_value);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_steer_mode_value_H_ */

