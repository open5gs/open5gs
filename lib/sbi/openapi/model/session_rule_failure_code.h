/*
 * session_rule_failure_code.h
 *
 * 
 */

#ifndef _OpenAPI_session_rule_failure_code_H_
#define _OpenAPI_session_rule_failure_code_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_session_rule_failure_code_NULL = 0, OpenAPI_session_rule_failure_code_NF_MAL, OpenAPI_session_rule_failure_code_RES_LIM, OpenAPI_session_rule_failure_code_SESSION_RESOURCE_ALLOCATION_FAILURE, OpenAPI_session_rule_failure_code_UNSUCC_QOS_VAL, OpenAPI_session_rule_failure_code_INCORRECT_UM, OpenAPI_session_rule_failure_code_UE_STA_SUSP, OpenAPI_session_rule_failure_code_UNKNOWN_REF_ID, OpenAPI_session_rule_failure_code_INCORRECT_COND_DATA, OpenAPI_session_rule_failure_code_REF_ID_COLLISION, OpenAPI_session_rule_failure_code_AN_GW_FAILED } OpenAPI_session_rule_failure_code_e;

char* OpenAPI_session_rule_failure_code_ToString(OpenAPI_session_rule_failure_code_e session_rule_failure_code);

OpenAPI_session_rule_failure_code_e OpenAPI_session_rule_failure_code_FromString(char* session_rule_failure_code);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_session_rule_failure_code_H_ */

