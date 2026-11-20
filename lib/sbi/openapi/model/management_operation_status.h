/*
 * management_operation_status.h
 *
 * 
 */

#ifndef _OpenAPI_management_operation_status_H_
#define _OpenAPI_management_operation_status_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_management_operation_status_NULL = 0, OpenAPI_management_operation_status_OPERATION_SUCCEEDED, OpenAPI_management_operation_status_OPERATION_FAILED } OpenAPI_management_operation_status_e;

char* OpenAPI_management_operation_status_ToString(OpenAPI_management_operation_status_e management_operation_status);

OpenAPI_management_operation_status_e OpenAPI_management_operation_status_FromString(char* management_operation_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_management_operation_status_H_ */

