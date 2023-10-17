/*
 * http_method_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_http_method_any_of_H_
#define _OpenAPI_http_method_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_http_method_any_of_NULL = 0, OpenAPI_http_method_any_of_GET, OpenAPI_http_method_any_of_PUT, OpenAPI_http_method_any_of_POST, OpenAPI_http_method_any_of__DELETE, OpenAPI_http_method_any_of_PATCH, OpenAPI_http_method_any_of_HEAD, OpenAPI_http_method_any_of_OPTIONS, OpenAPI_http_method_any_of_CONNECT, OpenAPI_http_method_any_of_TRACE } OpenAPI_http_method_any_of_e;

char* OpenAPI_http_method_any_of_ToString(OpenAPI_http_method_any_of_e http_method_any_of);

OpenAPI_http_method_any_of_e OpenAPI_http_method_any_of_FromString(char* http_method_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_http_method_any_of_H_ */

