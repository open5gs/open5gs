/*
 * ie_type_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_ie_type_any_of_H_
#define _OpenAPI_ie_type_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ie_type_any_of_NULL = 0, OpenAPI_ie_type_any_of_UEID, OpenAPI_ie_type_any_of_LOCATION, OpenAPI_ie_type_any_of_KEY_MATERIAL, OpenAPI_ie_type_any_of_AUTHENTICATION_MATERIAL, OpenAPI_ie_type_any_of_AUTHORIZATION_TOKEN, OpenAPI_ie_type_any_of_OTHER, OpenAPI_ie_type_any_of_NONSENSITIVE } OpenAPI_ie_type_any_of_e;

char* OpenAPI_ie_type_any_of_ToString(OpenAPI_ie_type_any_of_e ie_type_any_of);

OpenAPI_ie_type_any_of_e OpenAPI_ie_type_any_of_FromString(char* ie_type_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ie_type_any_of_H_ */

