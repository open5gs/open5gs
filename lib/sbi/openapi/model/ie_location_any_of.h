/*
 * ie_location_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_ie_location_any_of_H_
#define _OpenAPI_ie_location_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ie_location_any_of_NULL = 0, OpenAPI_ie_location_any_of_URI_PARAM, OpenAPI_ie_location_any_of_HEADER, OpenAPI_ie_location_any_of_BODY, OpenAPI_ie_location_any_of_MULTIPART_BINARY } OpenAPI_ie_location_any_of_e;

char* OpenAPI_ie_location_any_of_ToString(OpenAPI_ie_location_any_of_e ie_location_any_of);

OpenAPI_ie_location_any_of_e OpenAPI_ie_location_any_of_FromString(char* ie_location_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ie_location_any_of_H_ */

