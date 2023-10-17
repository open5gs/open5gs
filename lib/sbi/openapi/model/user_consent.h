/*
 * user_consent.h
 *
 * 
 */

#ifndef _OpenAPI_user_consent_H_
#define _OpenAPI_user_consent_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_user_consent_NULL = 0, OpenAPI_user_consent_CONSENT_NOT_GIVEN, OpenAPI_user_consent_CONSENT_GIVEN } OpenAPI_user_consent_e;

char* OpenAPI_user_consent_ToString(OpenAPI_user_consent_e user_consent);

OpenAPI_user_consent_e OpenAPI_user_consent_FromString(char* user_consent);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_user_consent_H_ */

