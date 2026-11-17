/*
 * variable_part_type.h
 *
 * 
 */

#ifndef _OpenAPI_variable_part_type_H_
#define _OpenAPI_variable_part_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_variable_part_type_NULL = 0, OpenAPI_variable_part_type_INTEGER, OpenAPI_variable_part_type_NUMBER, OpenAPI_variable_part_type_TIME, OpenAPI_variable_part_type_DATE, OpenAPI_variable_part_type_CURRENCY } OpenAPI_variable_part_type_e;

char* OpenAPI_variable_part_type_ToString(OpenAPI_variable_part_type_e variable_part_type);

OpenAPI_variable_part_type_e OpenAPI_variable_part_type_FromString(char* variable_part_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_variable_part_type_H_ */

