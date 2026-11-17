/*
 * failure_handling.h
 *
 * 
 */

#ifndef _OpenAPI_failure_handling_H_
#define _OpenAPI_failure_handling_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_failure_handling_NULL = 0, OpenAPI_failure_handling_TERMINATE, OpenAPI_failure_handling__CONTINUE, OpenAPI_failure_handling_RETRY_AND_TERMINATE } OpenAPI_failure_handling_e;

char* OpenAPI_failure_handling_ToString(OpenAPI_failure_handling_e failure_handling);

OpenAPI_failure_handling_e OpenAPI_failure_handling_FromString(char* failure_handling);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_failure_handling_H_ */

