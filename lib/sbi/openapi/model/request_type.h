/*
 * request_type.h
 *
 * 
 */

#ifndef _OpenAPI_request_type_H_
#define _OpenAPI_request_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_request_type_NULL = 0, OpenAPI_request_type_INITIAL_REQUEST, OpenAPI_request_type_EXISTING_PDU_SESSION, OpenAPI_request_type_INITIAL_EMERGENCY_REQUEST, OpenAPI_request_type_EXISTING_EMERGENCY_PDU_SESSION } OpenAPI_request_type_e;

char* OpenAPI_request_type_ToString(OpenAPI_request_type_e request_type);

OpenAPI_request_type_e OpenAPI_request_type_FromString(char* request_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_request_type_H_ */

