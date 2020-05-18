/*
 * patch_operation.h
 *
 *
 */

#ifndef _OpenAPI_patch_operation_H_
#define _OpenAPI_patch_operation_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_patch_operation_NULL = 0, OpenAPI_patch_operation_add, OpenAPI_patch_operation_copy, OpenAPI_patch_operation_move, OpenAPI_patch_operation__remove, OpenAPI_patch_operation_replace, OpenAPI_patch_operation_test } OpenAPI_patch_operation_e;

char* OpenAPI_patch_operation_ToString(OpenAPI_patch_operation_e patch_operation);

OpenAPI_patch_operation_e OpenAPI_patch_operation_FromString(char* patch_operation);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_patch_operation_H_ */

