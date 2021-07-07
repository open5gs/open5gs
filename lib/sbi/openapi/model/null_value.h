/*
 * null_value.h
 *
 * 
 */

#ifndef _OpenAPI_null_value_H_
#define _OpenAPI_null_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_null_value_NULL = 0, OpenAPI_null_value_null } OpenAPI_null_value_e;

char* OpenAPI_null_value_ToString(OpenAPI_null_value_e null_value);

OpenAPI_null_value_e OpenAPI_null_value_FromString(char* null_value);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_null_value_H_ */

