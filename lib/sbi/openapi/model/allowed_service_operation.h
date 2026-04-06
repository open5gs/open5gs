/*
 * allowed_service_operation.h
 *
 * The Allowed Service Operation.
 */

#ifndef _OpenAPI_allowed_service_operation_H_
#define _OpenAPI_allowed_service_operation_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_allowed_service_operation_NULL = 0, OpenAPI_allowed_service_operation_INVENTORY, OpenAPI_allowed_service_operation_READ, OpenAPI_allowed_service_operation_WRITE, OpenAPI_allowed_service_operation_PERMANENT_DISABLE } OpenAPI_allowed_service_operation_e;

char* OpenAPI_allowed_service_operation_ToString(OpenAPI_allowed_service_operation_e allowed_service_operation);

OpenAPI_allowed_service_operation_e OpenAPI_allowed_service_operation_FromString(char* allowed_service_operation);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_allowed_service_operation_H_ */

