/*
 * string_matching_condition.h
 *
 * A String with Matching Operator
 */

#ifndef _OpenAPI_string_matching_condition_H_
#define _OpenAPI_string_matching_condition_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_string_matching_condition_s OpenAPI_string_matching_condition_t;
#include "matching_operator.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_string_matching_condition_s {
    char *matching_string;
    OpenAPI_matching_operator_e matching_operator;
};

OpenAPI_string_matching_condition_t *OpenAPI_string_matching_condition_create(
    char *matching_string,
    OpenAPI_matching_operator_e matching_operator
);
void OpenAPI_string_matching_condition_free(OpenAPI_string_matching_condition_t *string_matching_condition);
OpenAPI_string_matching_condition_t *OpenAPI_string_matching_condition_parseFromJSON(cJSON *string_matching_conditionJSON);
cJSON *OpenAPI_string_matching_condition_convertToJSON(OpenAPI_string_matching_condition_t *string_matching_condition);
OpenAPI_string_matching_condition_t *OpenAPI_string_matching_condition_copy(OpenAPI_string_matching_condition_t *dst, OpenAPI_string_matching_condition_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_string_matching_condition_H_ */

