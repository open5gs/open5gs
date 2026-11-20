/*
 * result_code.h
 *
 * 
 */

#ifndef _OpenAPI_result_code_H_
#define _OpenAPI_result_code_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_result_code_NULL = 0, OpenAPI_result_code_SUCCESS, OpenAPI_result_code_END_USER_SERVICE_DENIED, OpenAPI_result_code_QUOTA_MANAGEMENT_NOT_APPLICABLE, OpenAPI_result_code_QUOTA_LIMIT_REACHED, OpenAPI_result_code_END_USER_SERVICE_REJECTED, OpenAPI_result_code_USER_UNKNOWN, OpenAPI_result_code_RATING_FAILED, OpenAPI_result_code_QUOTA_MANAGEMENT } OpenAPI_result_code_e;

char* OpenAPI_result_code_ToString(OpenAPI_result_code_e result_code);

OpenAPI_result_code_e OpenAPI_result_code_FromString(char* result_code);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_result_code_H_ */

