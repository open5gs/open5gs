/*
 * mdt_user_consent.h
 *
 * 
 */

#ifndef _OpenAPI_mdt_user_consent_H_
#define _OpenAPI_mdt_user_consent_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_mdt_user_consent_NULL = 0, OpenAPI_mdt_user_consent_CONSENT_NOT_GIVEN, OpenAPI_mdt_user_consent_CONSENT_GIVEN } OpenAPI_mdt_user_consent_e;

char* OpenAPI_mdt_user_consent_ToString(OpenAPI_mdt_user_consent_e mdt_user_consent);

OpenAPI_mdt_user_consent_e OpenAPI_mdt_user_consent_FromString(char* mdt_user_consent);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mdt_user_consent_H_ */

