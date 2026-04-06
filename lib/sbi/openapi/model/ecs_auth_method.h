/*
 * ecs_auth_method.h
 *
 * Contains the ECS Authentication Methods.
 */

#ifndef _OpenAPI_ecs_auth_method_H_
#define _OpenAPI_ecs_auth_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ecs_auth_method_NULL = 0, OpenAPI_ecs_auth_method_TLS_CLIENT_SERVER_CERTIFICATE, OpenAPI_ecs_auth_method_TLS_AKMA, OpenAPI_ecs_auth_method_TLS_GBA } OpenAPI_ecs_auth_method_e;

char* OpenAPI_ecs_auth_method_ToString(OpenAPI_ecs_auth_method_e ecs_auth_method);

OpenAPI_ecs_auth_method_e OpenAPI_ecs_auth_method_FromString(char* ecs_auth_method);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ecs_auth_method_H_ */

