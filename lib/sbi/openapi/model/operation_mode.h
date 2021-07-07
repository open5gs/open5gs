/*
 * operation_mode.h
 *
 * 
 */

#ifndef _OpenAPI_operation_mode_H_
#define _OpenAPI_operation_mode_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_operation_mode_NULL = 0, OpenAPI_operation_mode_WB_S1, OpenAPI_operation_mode_NB_S1, OpenAPI_operation_mode_WB_N1, OpenAPI_operation_mode_NB_N1 } OpenAPI_operation_mode_e;

char* OpenAPI_operation_mode_ToString(OpenAPI_operation_mode_e operation_mode);

OpenAPI_operation_mode_e OpenAPI_operation_mode_FromString(char* operation_mode);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_operation_mode_H_ */

