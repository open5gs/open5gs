/*
 * auth_type.h
 *
 * 
 */

#ifndef _OpenAPI_auth_type_H_
#define _OpenAPI_auth_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_auth_type_NULL = 0, OpenAPI_auth_type_5G_AKA, OpenAPI_auth_type_EAP_AKA_PRIME, OpenAPI_auth_type_EAP_TLS, OpenAPI_auth_type_EAP_TTLS } OpenAPI_auth_type_e;

char* OpenAPI_auth_type_ToString(OpenAPI_auth_type_e auth_type);

OpenAPI_auth_type_e OpenAPI_auth_type_FromString(char* auth_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_auth_type_H_ */

