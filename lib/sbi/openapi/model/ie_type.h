/*
 * ie_type.h
 *
 * 
 */

#ifndef _OpenAPI_ie_type_H_
#define _OpenAPI_ie_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ie_type_NULL = 0, OpenAPI_ie_type_UEID, OpenAPI_ie_type_LOCATION, OpenAPI_ie_type_KEY_MATERIAL, OpenAPI_ie_type_AUTHENTICATION_MATERIAL, OpenAPI_ie_type_AUTHORIZATION_TOKEN, OpenAPI_ie_type_OTHER, OpenAPI_ie_type_NONSENSITIVE } OpenAPI_ie_type_e;

char* OpenAPI_ie_type_ToString(OpenAPI_ie_type_e ie_type);

OpenAPI_ie_type_e OpenAPI_ie_type_FromString(char* ie_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ie_type_H_ */

