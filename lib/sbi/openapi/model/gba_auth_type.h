/*
 * gba_auth_type.h
 *
 * 
 */

#ifndef _OpenAPI_gba_auth_type_H_
#define _OpenAPI_gba_auth_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_gba_auth_type_NULL = 0, OpenAPI_gba_auth_type_DIGEST_AKAV1_MD5 } OpenAPI_gba_auth_type_e;

char* OpenAPI_gba_auth_type_ToString(OpenAPI_gba_auth_type_e gba_auth_type);

OpenAPI_gba_auth_type_e OpenAPI_gba_auth_type_FromString(char* gba_auth_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gba_auth_type_H_ */

