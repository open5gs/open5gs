/*
 * ldr_type_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_ldr_type_any_of_H_
#define _OpenAPI_ldr_type_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ldr_type_any_of_NULL = 0, OpenAPI_ldr_type_any_of_UE_AVAILABLE, OpenAPI_ldr_type_any_of_PERIODIC, OpenAPI_ldr_type_any_of_ENTERING_INTO_AREA, OpenAPI_ldr_type_any_of_LEAVING_FROM_AREA, OpenAPI_ldr_type_any_of_BEING_INSIDE_AREA, OpenAPI_ldr_type_any_of_MOTION } OpenAPI_ldr_type_any_of_e;

char* OpenAPI_ldr_type_any_of_ToString(OpenAPI_ldr_type_any_of_e ldr_type_any_of);

OpenAPI_ldr_type_any_of_e OpenAPI_ldr_type_any_of_FromString(char* ldr_type_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ldr_type_any_of_H_ */

