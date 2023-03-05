/*
 * ie_location.h
 *
 * 
 */

#ifndef _OpenAPI_ie_location_H_
#define _OpenAPI_ie_location_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ie_location_NULL = 0, OpenAPI_ie_location_URI_PARAM, OpenAPI_ie_location_HEADER, OpenAPI_ie_location_BODY, OpenAPI_ie_location_MULTIPART_BINARY } OpenAPI_ie_location_e;

char* OpenAPI_ie_location_ToString(OpenAPI_ie_location_e ie_location);

OpenAPI_ie_location_e OpenAPI_ie_location_FromString(char* ie_location);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ie_location_H_ */

