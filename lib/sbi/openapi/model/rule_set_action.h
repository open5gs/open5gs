/*
 * rule_set_action.h
 *
 * Specifies whether access/scope is allowed or denied for a specific NF-Consumer. 
 */

#ifndef _OpenAPI_rule_set_action_H_
#define _OpenAPI_rule_set_action_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_rule_set_action_NULL = 0, OpenAPI_rule_set_action_ALLOW, OpenAPI_rule_set_action_DENY } OpenAPI_rule_set_action_e;

char* OpenAPI_rule_set_action_ToString(OpenAPI_rule_set_action_e rule_set_action);

OpenAPI_rule_set_action_e OpenAPI_rule_set_action_FromString(char* rule_set_action);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rule_set_action_H_ */

