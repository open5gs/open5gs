/*
 * sm_policy_association_release_cause.h
 *
 * 
 */

#ifndef _OpenAPI_sm_policy_association_release_cause_H_
#define _OpenAPI_sm_policy_association_release_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sm_policy_association_release_cause_NULL = 0, OpenAPI_sm_policy_association_release_cause_UNSPECIFIED, OpenAPI_sm_policy_association_release_cause_UE_SUBSCRIPTION, OpenAPI_sm_policy_association_release_cause_INSUFFICIENT_RES, OpenAPI_sm_policy_association_release_cause_VALIDATION_CONDITION_NOT_MET } OpenAPI_sm_policy_association_release_cause_e;

char* OpenAPI_sm_policy_association_release_cause_ToString(OpenAPI_sm_policy_association_release_cause_e sm_policy_association_release_cause);

OpenAPI_sm_policy_association_release_cause_e OpenAPI_sm_policy_association_release_cause_FromString(char* sm_policy_association_release_cause);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_policy_association_release_cause_H_ */

