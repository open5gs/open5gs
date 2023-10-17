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
#include "policy_data_subset_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_policy_data_subset_s OpenAPI_policy_data_subset_t;
typedef struct OpenAPI_policy_data_subset_s {
} OpenAPI_policy_data_subset_t;

OpenAPI_policy_data_subset_t *OpenAPI_policy_data_subset_create(
);
void OpenAPI_policy_data_subset_free(OpenAPI_policy_data_subset_t *policy_data_subset);
OpenAPI_policy_data_subset_t *OpenAPI_policy_data_subset_parseFromJSON(cJSON *policy_data_subsetJSON);
cJSON *OpenAPI_policy_data_subset_convertToJSON(OpenAPI_policy_data_subset_t *policy_data_subset);
OpenAPI_policy_data_subset_t *OpenAPI_policy_data_subset_copy(OpenAPI_policy_data_subset_t *dst, OpenAPI_policy_data_subset_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_data_subset_H_ */

