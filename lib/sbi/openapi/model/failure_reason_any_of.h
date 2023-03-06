/*
 * failure_reason_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_failure_reason_any_of_H_
#define _OpenAPI_failure_reason_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_failure_reason_any_of_NULL = 0, OpenAPI_failure_reason_any_of_INVALID_JSON_POINTER, OpenAPI_failure_reason_any_of_INVALID_INDEX_TO_ENCRYPTED_BLOCK, OpenAPI_failure_reason_any_of_INVALID_HTTP_HEADER } OpenAPI_failure_reason_any_of_e;

char* OpenAPI_failure_reason_any_of_ToString(OpenAPI_failure_reason_any_of_e failure_reason_any_of);

OpenAPI_failure_reason_any_of_e OpenAPI_failure_reason_any_of_FromString(char* failure_reason_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_failure_reason_any_of_H_ */

