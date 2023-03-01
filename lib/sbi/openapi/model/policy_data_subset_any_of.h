/*
 * policy_data_subset_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_policy_data_subset_any_of_H_
#define _OpenAPI_policy_data_subset_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_policy_data_subset_any_of_NULL = 0, OpenAPI_policy_data_subset_any_of_AM_POLICY_DATA, OpenAPI_policy_data_subset_any_of_SM_POLICY_DATA, OpenAPI_policy_data_subset_any_of_UE_POLICY_DATA, OpenAPI_policy_data_subset_any_of_UM_DATA, OpenAPI_policy_data_subset_any_of_OPERATOR_SPECIFIC_DATA } OpenAPI_policy_data_subset_any_of_e;

char* OpenAPI_policy_data_subset_any_of_ToString(OpenAPI_policy_data_subset_any_of_e policy_data_subset_any_of);

OpenAPI_policy_data_subset_any_of_e OpenAPI_policy_data_subset_any_of_FromString(char* policy_data_subset_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_data_subset_any_of_H_ */

