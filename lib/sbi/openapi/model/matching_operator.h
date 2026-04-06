/*
 * matching_operator.h
 *
 * 
 */

#ifndef _OpenAPI_matching_operator_H_
#define _OpenAPI_matching_operator_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_matching_operator_NULL = 0, OpenAPI_matching_operator_FULL_MATCH, OpenAPI_matching_operator_MATCH_ALL, OpenAPI_matching_operator_STARTS_WITH, OpenAPI_matching_operator_NOT_START_WITH, OpenAPI_matching_operator_ENDS_WITH, OpenAPI_matching_operator_NOT_END_WITH, OpenAPI_matching_operator_CONTAINS, OpenAPI_matching_operator_NOT_CONTAIN } OpenAPI_matching_operator_e;

char* OpenAPI_matching_operator_ToString(OpenAPI_matching_operator_e matching_operator);

OpenAPI_matching_operator_e OpenAPI_matching_operator_FromString(char* matching_operator);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_matching_operator_H_ */

