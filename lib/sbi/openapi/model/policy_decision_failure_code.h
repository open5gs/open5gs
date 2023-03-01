/*
 * policy_decision_failure_code.h
 *
 * 
 */

#ifndef _OpenAPI_policy_decision_failure_code_H_
#define _OpenAPI_policy_decision_failure_code_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_policy_decision_failure_code_NULL = 0, OpenAPI_policy_decision_failure_code_TRA_CTRL_DECS_ERR, OpenAPI_policy_decision_failure_code_QOS_DECS_ERR, OpenAPI_policy_decision_failure_code_CHG_DECS_ERR, OpenAPI_policy_decision_failure_code_USA_MON_DECS_ERR, OpenAPI_policy_decision_failure_code_QOS_MON_DECS_ERR, OpenAPI_policy_decision_failure_code_CON_DATA_ERR, OpenAPI_policy_decision_failure_code_POLICY_PARAM_ERR } OpenAPI_policy_decision_failure_code_e;

char* OpenAPI_policy_decision_failure_code_ToString(OpenAPI_policy_decision_failure_code_e policy_decision_failure_code);

OpenAPI_policy_decision_failure_code_e OpenAPI_policy_decision_failure_code_FromString(char* policy_decision_failure_code);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_decision_failure_code_H_ */

