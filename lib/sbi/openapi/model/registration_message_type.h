/*
 * registration_message_type.h
 *
 * 
 */

#ifndef _OpenAPI_registration_message_type_H_
#define _OpenAPI_registration_message_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_registration_message_type_NULL = 0, OpenAPI_registration_message_type_INITIAL, OpenAPI_registration_message_type_MOBILITY, OpenAPI_registration_message_type_PERIODIC, OpenAPI_registration_message_type_EMERGENCY, OpenAPI_registration_message_type_DEREGISTRATION } OpenAPI_registration_message_type_e;

char* OpenAPI_registration_message_type_ToString(OpenAPI_registration_message_type_e registration_message_type);

OpenAPI_registration_message_type_e OpenAPI_registration_message_type_FromString(char* registration_message_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_registration_message_type_H_ */

