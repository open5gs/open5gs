/*
 * auth_method.h
 *
 *
 */

#ifndef _OpenAPI_auth_method_H_
#define _OpenAPI_auth_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_auth_method_NULL = 0, OpenAPI_auth_method_5G_AKA, OpenAPI_auth_method_EAP_AKA_PRIME, OpenAPI_auth_method_EAP_TLS } OpenAPI_auth_method_e;

char* OpenAPI_auth_method_ToString(OpenAPI_auth_method_e auth_method);

OpenAPI_auth_method_e OpenAPI_auth_method_FromString(char* auth_method);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_auth_method_H_ */

