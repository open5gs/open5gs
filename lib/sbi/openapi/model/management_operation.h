/*
 * management_operation.h
 *
 * 
 */

#ifndef _OpenAPI_management_operation_H_
#define _OpenAPI_management_operation_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_management_operation_NULL = 0, OpenAPI_management_operation_CreateMOI, OpenAPI_management_operation_ModifyMOIAttributes, OpenAPI_management_operation_DeleteMOI, OpenAPI_management_operation_CREATE_MOI, OpenAPI_management_operation_MODIFY_MOI_ATTR, OpenAPI_management_operation_DELETE_MOI, OpenAPI_management_operation_NOTIFY_MOI_CREATION, OpenAPI_management_operation_NOTIFY_MOI_ATTR_CHANGE, OpenAPI_management_operation_NOTIFY_MOI_DELETION } OpenAPI_management_operation_e;

char* OpenAPI_management_operation_ToString(OpenAPI_management_operation_e management_operation);

OpenAPI_management_operation_e OpenAPI_management_operation_FromString(char* management_operation);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_management_operation_H_ */

