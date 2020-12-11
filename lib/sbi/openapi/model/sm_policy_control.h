/*
 * sm_policy_control.h
 *
 *
 */

#ifndef _OpenAPI_sm_policy_control_H_
#define _OpenAPI_sm_policy_control_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sm_policy_context_data.h"
#include "sm_policy_decision.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_policy_control_s OpenAPI_sm_policy_control_t;
typedef struct OpenAPI_sm_policy_control_s {
    struct OpenAPI_sm_policy_context_data_s *context;
    struct OpenAPI_sm_policy_decision_s *policy;
} OpenAPI_sm_policy_control_t;

OpenAPI_sm_policy_control_t *OpenAPI_sm_policy_control_create(
    OpenAPI_sm_policy_context_data_t *context,
    OpenAPI_sm_policy_decision_t *policy
    );
void OpenAPI_sm_policy_control_free(OpenAPI_sm_policy_control_t *sm_policy_control);
OpenAPI_sm_policy_control_t *OpenAPI_sm_policy_control_parseFromJSON(cJSON *sm_policy_controlJSON);
cJSON *OpenAPI_sm_policy_control_convertToJSON(OpenAPI_sm_policy_control_t *sm_policy_control);
OpenAPI_sm_policy_control_t *OpenAPI_sm_policy_control_copy(OpenAPI_sm_policy_control_t *dst, OpenAPI_sm_policy_control_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_policy_control_H_ */

