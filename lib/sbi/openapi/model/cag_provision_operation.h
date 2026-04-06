/*
 * cag_provision_operation.h
 *
 * CAG provision operation
 */

#ifndef _OpenAPI_cag_provision_operation_H_
#define _OpenAPI_cag_provision_operation_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_cag_provision_operation_NULL = 0, OpenAPI_cag_provision_operation_ADD, OpenAPI_cag_provision_operation_REPLACE, OpenAPI_cag_provision_operation__REMOVE } OpenAPI_cag_provision_operation_e;

char* OpenAPI_cag_provision_operation_ToString(OpenAPI_cag_provision_operation_e cag_provision_operation);

OpenAPI_cag_provision_operation_e OpenAPI_cag_provision_operation_FromString(char* cag_provision_operation);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cag_provision_operation_H_ */

