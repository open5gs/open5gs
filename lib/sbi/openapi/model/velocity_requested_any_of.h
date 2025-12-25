/*
 * velocity_requested_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_velocity_requested_any_of_H_
#define _OpenAPI_velocity_requested_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_velocity_requested_any_of_NULL = 0, OpenAPI_velocity_requested_any_of_VELOCITY_IS_NOT_REQUESTED, OpenAPI_velocity_requested_any_of_VELOCITY_IS_REQUESTED } OpenAPI_velocity_requested_any_of_e;

char* OpenAPI_velocity_requested_any_of_ToString(OpenAPI_velocity_requested_any_of_e velocity_requested_any_of);

OpenAPI_velocity_requested_any_of_e OpenAPI_velocity_requested_any_of_FromString(char* velocity_requested_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_velocity_requested_any_of_H_ */

