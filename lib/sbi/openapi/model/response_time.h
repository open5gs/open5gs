/*
 * response_time.h
 *
 * Indicates acceptable delay of location request.
 */

#ifndef _OpenAPI_response_time_H_
#define _OpenAPI_response_time_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_response_time_NULL = 0, OpenAPI_response_time_LOW_DELAY, OpenAPI_response_time_DELAY_TOLERANT, OpenAPI_response_time_NO_DELAY } OpenAPI_response_time_e;

char* OpenAPI_response_time_ToString(OpenAPI_response_time_e response_time);

OpenAPI_response_time_e OpenAPI_response_time_FromString(char* response_time);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_response_time_H_ */

