/*
 * interface_type.h
 *
 * 
 */

#ifndef _OpenAPI_interface_type_H_
#define _OpenAPI_interface_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_interface_type_NULL = 0, OpenAPI_interface_type_UNKNOWN, OpenAPI_interface_type_MOBILE_ORIGINATING, OpenAPI_interface_type_MOBILE_TERMINATING, OpenAPI_interface_type_APPLICATION_ORIGINATING, OpenAPI_interface_type_APPLICATION_TERMINATING } OpenAPI_interface_type_e;

char* OpenAPI_interface_type_ToString(OpenAPI_interface_type_e interface_type);

OpenAPI_interface_type_e OpenAPI_interface_type_FromString(char* interface_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_interface_type_H_ */

