/*
 * line_type.h
 *
 * 
 */

#ifndef _OpenAPI_line_type_H_
#define _OpenAPI_line_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_line_type_NULL = 0, OpenAPI_line_type_DSL, OpenAPI_line_type_PON } OpenAPI_line_type_e;

char* OpenAPI_line_type_ToString(OpenAPI_line_type_e line_type);

OpenAPI_line_type_e OpenAPI_line_type_FromString(char* line_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_line_type_H_ */

