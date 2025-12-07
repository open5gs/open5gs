/*
 * usage_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_usage_any_of_H_
#define _OpenAPI_usage_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_usage_any_of_NULL = 0, OpenAPI_usage_any_of_UNSUCCESS, OpenAPI_usage_any_of_SUCCESS_RESULTS_NOT_USED, OpenAPI_usage_any_of_SUCCESS_RESULTS_USED_TO_VERIFY_LOCATION, OpenAPI_usage_any_of_SUCCESS_RESULTS_USED_TO_GENERATE_LOCATION, OpenAPI_usage_any_of_SUCCESS_METHOD_NOT_DETERMINED } OpenAPI_usage_any_of_e;

char* OpenAPI_usage_any_of_ToString(OpenAPI_usage_any_of_e usage_any_of);

OpenAPI_usage_any_of_e OpenAPI_usage_any_of_FromString(char* usage_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_usage_any_of_H_ */

