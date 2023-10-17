/*
 * security_capability.h
 *
 * 
 */

#ifndef _OpenAPI_security_capability_H_
#define _OpenAPI_security_capability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_security_capability_NULL = 0, OpenAPI_security_capability_TLS, OpenAPI_security_capability_PRINS, OpenAPI_security_capability_NONE } OpenAPI_security_capability_e;

char* OpenAPI_security_capability_ToString(OpenAPI_security_capability_e security_capability);

OpenAPI_security_capability_e OpenAPI_security_capability_FromString(char* security_capability);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_security_capability_H_ */

