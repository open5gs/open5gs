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

typedef struct OpenAPI_sm_policy_association_release_cause_s OpenAPI_sm_policy_association_release_cause_t;
typedef struct OpenAPI_sm_policy_association_release_cause_s {
} OpenAPI_sm_policy_association_release_cause_t;

OpenAPI_sm_policy_association_release_cause_t *OpenAPI_sm_policy_association_release_cause_create(
    );
void OpenAPI_sm_policy_association_release_cause_free(OpenAPI_sm_policy_association_release_cause_t *sm_policy_association_release_cause);
OpenAPI_sm_policy_association_release_cause_t *OpenAPI_sm_policy_association_release_cause_parseFromJSON(cJSON *sm_policy_association_release_causeJSON);
cJSON *OpenAPI_sm_policy_association_release_cause_convertToJSON(OpenAPI_sm_policy_association_release_cause_t *sm_policy_association_release_cause);
OpenAPI_sm_policy_association_release_cause_t *OpenAPI_sm_policy_association_release_cause_copy(OpenAPI_sm_policy_association_release_cause_t *dst, OpenAPI_sm_policy_association_release_cause_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_policy_association_release_cause_H_ */

