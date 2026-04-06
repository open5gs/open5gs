/*
 * value_expression.h
 *
 * 
 */

#ifndef _OpenAPI_value_expression_H_
#define _OpenAPI_value_expression_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_value_expression_NULL = 0, OpenAPI_value_expression_AVERAGE, OpenAPI_value_expression_PEAK } OpenAPI_value_expression_e;

char* OpenAPI_value_expression_ToString(OpenAPI_value_expression_e value_expression);

OpenAPI_value_expression_e OpenAPI_value_expression_FromString(char* value_expression);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_value_expression_H_ */

