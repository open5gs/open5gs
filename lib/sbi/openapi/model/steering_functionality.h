/*
 * steering_functionality.h
 *
 * 
 */

#ifndef _OpenAPI_steering_functionality_H_
#define _OpenAPI_steering_functionality_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_steering_functionality_NULL = 0, OpenAPI_steering_functionality_MPTCP, OpenAPI_steering_functionality_ATSSS_LL } OpenAPI_steering_functionality_e;

char* OpenAPI_steering_functionality_ToString(OpenAPI_steering_functionality_e steering_functionality);

OpenAPI_steering_functionality_e OpenAPI_steering_functionality_FromString(char* steering_functionality);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_steering_functionality_H_ */

