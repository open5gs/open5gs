/*
 * on_path_n6_method.h
 *
 * 
 */

#ifndef _OpenAPI_on_path_n6_method_H_
#define _OpenAPI_on_path_n6_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_on_path_n6_method_NULL = 0, OpenAPI_on_path_n6_method_CONNECT_UDP } OpenAPI_on_path_n6_method_e;

char* OpenAPI_on_path_n6_method_ToString(OpenAPI_on_path_n6_method_e on_path_n6_method);

OpenAPI_on_path_n6_method_e OpenAPI_on_path_n6_method_FromString(char* on_path_n6_method);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_on_path_n6_method_H_ */

