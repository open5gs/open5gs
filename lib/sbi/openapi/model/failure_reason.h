/*
 * failure_reason.h
 *
 * 
 */

#ifndef _OpenAPI_failure_reason_H_
#define _OpenAPI_failure_reason_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_failure_reason_NULL = 0, OpenAPI_failure_reason_INVALID_JSON_POINTER, OpenAPI_failure_reason_INVALID_INDEX_TO_ENCRYPTED_BLOCK, OpenAPI_failure_reason_INVALID_HTTP_HEADER } OpenAPI_failure_reason_e;

char* OpenAPI_failure_reason_ToString(OpenAPI_failure_reason_e failure_reason);

OpenAPI_failure_reason_e OpenAPI_failure_reason_FromString(char* failure_reason);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_failure_reason_H_ */

