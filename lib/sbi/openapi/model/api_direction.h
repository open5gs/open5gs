/*
 * api_direction.h
 *
 * 
 */

#ifndef _OpenAPI_api_direction_H_
#define _OpenAPI_api_direction_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_api_direction_NULL = 0, OpenAPI_api_direction_INVOCATION, OpenAPI_api_direction_NOTIFICATION } OpenAPI_api_direction_e;

char* OpenAPI_api_direction_ToString(OpenAPI_api_direction_e api_direction);

OpenAPI_api_direction_e OpenAPI_api_direction_FromString(char* api_direction);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_api_direction_H_ */

