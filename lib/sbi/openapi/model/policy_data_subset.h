/*
 * policy_data_subset.h
 *
 * Indicates a policy data subset.
 */

#ifndef _OpenAPI_policy_data_subset_H_
#define _OpenAPI_policy_data_subset_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_policy_data_subset_NULL = 0, OpenAPI_policy_data_subset_AM_POLICY_DATA, OpenAPI_policy_data_subset_SM_POLICY_DATA, OpenAPI_policy_data_subset_UE_POLICY_DATA, OpenAPI_policy_data_subset_UM_DATA, OpenAPI_policy_data_subset_OPERATOR_SPECIFIC_DATA } OpenAPI_policy_data_subset_e;

char* OpenAPI_policy_data_subset_ToString(OpenAPI_policy_data_subset_e policy_data_subset);

OpenAPI_policy_data_subset_e OpenAPI_policy_data_subset_FromString(char* policy_data_subset);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_data_subset_H_ */

