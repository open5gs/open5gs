/*
 * security_capability_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_security_capability_any_of_H_
#define _OpenAPI_security_capability_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_security_capability_any_of_NULL = 0, OpenAPI_security_capability_any_of_TLS, OpenAPI_security_capability_any_of_PRINS, OpenAPI_security_capability_any_of_NONE } OpenAPI_security_capability_any_of_e;

char* OpenAPI_security_capability_any_of_ToString(OpenAPI_security_capability_any_of_e security_capability_any_of);

OpenAPI_security_capability_any_of_e OpenAPI_security_capability_any_of_FromString(char* security_capability_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_security_capability_any_of_H_ */

