/*
 * policy_association_release_cause.h
 *
 * 
 */

#ifndef _OpenAPI_policy_association_release_cause_H_
#define _OpenAPI_policy_association_release_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_policy_association_release_cause_NULL = 0, OpenAPI_policy_association_release_cause_UNSPECIFIED, OpenAPI_policy_association_release_cause_UE_SUBSCRIPTION, OpenAPI_policy_association_release_cause_INSUFFICIENT_RES } OpenAPI_policy_association_release_cause_e;

char* OpenAPI_policy_association_release_cause_ToString(OpenAPI_policy_association_release_cause_e policy_association_release_cause);

OpenAPI_policy_association_release_cause_e OpenAPI_policy_association_release_cause_FromString(char* policy_association_release_cause);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_association_release_cause_H_ */

