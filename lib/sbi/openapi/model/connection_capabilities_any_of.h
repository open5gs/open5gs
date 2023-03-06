/*
 * connection_capabilities_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_connection_capabilities_any_of_H_
#define _OpenAPI_connection_capabilities_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_connection_capabilities_any_of_NULL = 0, OpenAPI_connection_capabilities_any_of_IMS, OpenAPI_connection_capabilities_any_of_MMS, OpenAPI_connection_capabilities_any_of_SUPL, OpenAPI_connection_capabilities_any_of_INTERNET } OpenAPI_connection_capabilities_any_of_e;

char* OpenAPI_connection_capabilities_any_of_ToString(OpenAPI_connection_capabilities_any_of_e connection_capabilities_any_of);

OpenAPI_connection_capabilities_any_of_e OpenAPI_connection_capabilities_any_of_FromString(char* connection_capabilities_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_connection_capabilities_any_of_H_ */

