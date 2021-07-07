/*
 * ue_auth.h
 *
 * 
 */

#ifndef _OpenAPI_ue_auth_H_
#define _OpenAPI_ue_auth_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ue_auth_NULL = 0, OpenAPI_ue_auth_AUTHORIZED, OpenAPI_ue_auth_NOT_AUTHORIZED } OpenAPI_ue_auth_e;

char* OpenAPI_ue_auth_ToString(OpenAPI_ue_auth_e ue_auth);

OpenAPI_ue_auth_e OpenAPI_ue_auth_FromString(char* ue_auth);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_auth_H_ */

