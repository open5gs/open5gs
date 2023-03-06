/*
 * auth_result.h
 *
 * Indicates the result of the authentication.
 */

#ifndef _OpenAPI_auth_result_H_
#define _OpenAPI_auth_result_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_auth_result_NULL = 0, OpenAPI_auth_result_AUTHENTICATION_SUCCESS, OpenAPI_auth_result_AUTHENTICATION_FAILURE, OpenAPI_auth_result_AUTHENTICATION_ONGOING } OpenAPI_auth_result_e;

char* OpenAPI_auth_result_ToString(OpenAPI_auth_result_e auth_result);

OpenAPI_auth_result_e OpenAPI_auth_result_FromString(char* auth_result);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_auth_result_H_ */

