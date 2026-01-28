/*
 * response_time_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_response_time_any_of_H_
#define _OpenAPI_response_time_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_response_time_any_of_NULL = 0, OpenAPI_response_time_any_of_LOW_DELAY, OpenAPI_response_time_any_of_DELAY_TOLERANT, OpenAPI_response_time_any_of_NO_DELAY } OpenAPI_response_time_any_of_e;

char* OpenAPI_response_time_any_of_ToString(OpenAPI_response_time_any_of_e response_time_any_of);

OpenAPI_response_time_any_of_e OpenAPI_response_time_any_of_FromString(char* response_time_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_response_time_any_of_H_ */

