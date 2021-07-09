/*
 * failure_cause.h
 *
 * 
 */

#ifndef _OpenAPI_failure_cause_H_
#define _OpenAPI_failure_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_failure_cause_NULL = 0, OpenAPI_failure_cause_PCC_RULE_EVENT, OpenAPI_failure_cause_PCC_QOS_FLOW_EVENT, OpenAPI_failure_cause_RULE_PERMANENT_ERROR, OpenAPI_failure_cause_RULE_TEMPORARY_ERROR, OpenAPI_failure_cause_POL_DEC_ERROR } OpenAPI_failure_cause_e;

char* OpenAPI_failure_cause_ToString(OpenAPI_failure_cause_e failure_cause);

OpenAPI_failure_cause_e OpenAPI_failure_cause_FromString(char* failure_cause);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_failure_cause_H_ */

