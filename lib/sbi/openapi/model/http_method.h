/*
 * http_method.h
 *
 * 
 */

#ifndef _OpenAPI_http_method_H_
#define _OpenAPI_http_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_http_method_NULL = 0, OpenAPI_http_method_GET, OpenAPI_http_method_PUT, OpenAPI_http_method_POST, OpenAPI_http_method__DELETE, OpenAPI_http_method_PATCH, OpenAPI_http_method_HEAD, OpenAPI_http_method_OPTIONS, OpenAPI_http_method_CONNECT, OpenAPI_http_method_TRACE } OpenAPI_http_method_e;

char* OpenAPI_http_method_ToString(OpenAPI_http_method_e http_method);

OpenAPI_http_method_e OpenAPI_http_method_FromString(char* http_method);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_http_method_H_ */

