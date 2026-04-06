/*
 * string_matching_rule.h
 *
 * A list of conditions for string matching
 */

#ifndef _OpenAPI_string_matching_rule_H_
#define _OpenAPI_string_matching_rule_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_string_matching_rule_s OpenAPI_string_matching_rule_t;
#include "string_matching_condition.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_string_matching_rule_s {
    OpenAPI_list_t *string_matching_conditions;
};

OpenAPI_string_matching_rule_t *OpenAPI_string_matching_rule_create(
    OpenAPI_list_t *string_matching_conditions
);
void OpenAPI_string_matching_rule_free(OpenAPI_string_matching_rule_t *string_matching_rule);
OpenAPI_string_matching_rule_t *OpenAPI_string_matching_rule_parseFromJSON(cJSON *string_matching_ruleJSON);
cJSON *OpenAPI_string_matching_rule_convertToJSON(OpenAPI_string_matching_rule_t *string_matching_rule);
OpenAPI_string_matching_rule_t *OpenAPI_string_matching_rule_copy(OpenAPI_string_matching_rule_t *dst, OpenAPI_string_matching_rule_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_string_matching_rule_H_ */

