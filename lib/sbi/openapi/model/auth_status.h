/*
 * auth_status.h
 *
 * 
 */

#ifndef _OpenAPI_auth_status_H_
#define _OpenAPI_auth_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_auth_status_NULL = 0, OpenAPI_auth_status_EAP_SUCCESS, OpenAPI_auth_status_EAP_FAILURE, OpenAPI_auth_status_PENDING } OpenAPI_auth_status_e;

char* OpenAPI_auth_status_ToString(OpenAPI_auth_status_e auth_status);

OpenAPI_auth_status_e OpenAPI_auth_status_FromString(char* auth_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_auth_status_H_ */

