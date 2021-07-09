/*
 * registration_reason.h
 *
 * 
 */

#ifndef _OpenAPI_registration_reason_H_
#define _OpenAPI_registration_reason_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_registration_reason_NULL = 0, OpenAPI_registration_reason_SMF_CONTEXT_TRANSFERRED } OpenAPI_registration_reason_e;

char* OpenAPI_registration_reason_ToString(OpenAPI_registration_reason_e registration_reason);

OpenAPI_registration_reason_e OpenAPI_registration_reason_FromString(char* registration_reason);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_registration_reason_H_ */

