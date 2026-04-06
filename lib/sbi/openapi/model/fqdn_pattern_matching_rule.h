/*
 * fqdn_pattern_matching_rule.h
 *
 * a matching rule for a FQDN pattern
 */

#ifndef _OpenAPI_fqdn_pattern_matching_rule_H_
#define _OpenAPI_fqdn_pattern_matching_rule_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_fqdn_pattern_matching_rule_s OpenAPI_fqdn_pattern_matching_rule_t;
#include "string_matching_rule.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_fqdn_pattern_matching_rule_s {
    char *regex;
    struct OpenAPI_string_matching_rule_s *string_matching_rule;
};

OpenAPI_fqdn_pattern_matching_rule_t *OpenAPI_fqdn_pattern_matching_rule_create(
    char *regex,
    OpenAPI_string_matching_rule_t *string_matching_rule
);
void OpenAPI_fqdn_pattern_matching_rule_free(OpenAPI_fqdn_pattern_matching_rule_t *fqdn_pattern_matching_rule);
OpenAPI_fqdn_pattern_matching_rule_t *OpenAPI_fqdn_pattern_matching_rule_parseFromJSON(cJSON *fqdn_pattern_matching_ruleJSON);
cJSON *OpenAPI_fqdn_pattern_matching_rule_convertToJSON(OpenAPI_fqdn_pattern_matching_rule_t *fqdn_pattern_matching_rule);
OpenAPI_fqdn_pattern_matching_rule_t *OpenAPI_fqdn_pattern_matching_rule_copy(OpenAPI_fqdn_pattern_matching_rule_t *dst, OpenAPI_fqdn_pattern_matching_rule_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_fqdn_pattern_matching_rule_H_ */

