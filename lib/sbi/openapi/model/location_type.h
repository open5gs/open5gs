/*
 * location_type.h
 *
 * 
 */

#ifndef _OpenAPI_location_type_H_
#define _OpenAPI_location_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_location_type_NULL = 0, OpenAPI_location_type_CURRENT_LOCATION, OpenAPI_location_type_LAST_KNOWN_LOCATION, OpenAPI_location_type_INITIAL_LOCATION, OpenAPI_location_type_DEFERRED_LOCATION, OpenAPI_location_type_NOTIFICATION_VERIFICATION } OpenAPI_location_type_e;

char* OpenAPI_location_type_ToString(OpenAPI_location_type_e location_type);

OpenAPI_location_type_e OpenAPI_location_type_FromString(char* location_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_type_H_ */

