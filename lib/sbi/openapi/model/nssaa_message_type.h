/*
 * nssaa_message_type.h
 *
 * 
 */

#ifndef _OpenAPI_nssaa_message_type_H_
#define _OpenAPI_nssaa_message_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nssaa_message_type_NULL = 0, OpenAPI_nssaa_message_type_Authenticate, OpenAPI_nssaa_message_type_Re_Authentication_Notification, OpenAPI_nssaa_message_type_Revocation_Notification } OpenAPI_nssaa_message_type_e;

char* OpenAPI_nssaa_message_type_ToString(OpenAPI_nssaa_message_type_e nssaa_message_type);

OpenAPI_nssaa_message_type_e OpenAPI_nssaa_message_type_FromString(char* nssaa_message_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nssaa_message_type_H_ */

