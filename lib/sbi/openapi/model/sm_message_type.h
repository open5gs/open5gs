/*
 * sm_message_type.h
 *
 * 
 */

#ifndef _OpenAPI_sm_message_type_H_
#define _OpenAPI_sm_message_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sm_message_type_NULL = 0, OpenAPI_sm_message_type_SUBMISSION, OpenAPI_sm_message_type_DELIVERY_REPORT, OpenAPI_sm_message_type_SM_SERVICE_REQUEST, OpenAPI_sm_message_type_DELIVERY } OpenAPI_sm_message_type_e;

char* OpenAPI_sm_message_type_ToString(OpenAPI_sm_message_type_e sm_message_type);

OpenAPI_sm_message_type_e OpenAPI_sm_message_type_FromString(char* sm_message_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_message_type_H_ */

