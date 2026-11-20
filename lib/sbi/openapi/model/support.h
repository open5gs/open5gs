/*
 * support.h
 *
 * 
 */

#ifndef _OpenAPI_support_H_
#define _OpenAPI_support_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_support_NULL = 0, OpenAPI_support_NOT_SUPPORTED, OpenAPI_support_SUPPORTED } OpenAPI_support_e;

char* OpenAPI_support_ToString(OpenAPI_support_e support);

OpenAPI_support_e OpenAPI_support_FromString(char* support);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_support_H_ */

