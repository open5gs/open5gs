/*
 * expected_ue_behavior.h
 *
 * Represents the expected UE behavior (e.g. UE moving trajectory) and its validity period
 */

#ifndef _OpenAPI_expected_ue_behavior_H_
#define _OpenAPI_expected_ue_behavior_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_expected_ue_behavior_s OpenAPI_expected_ue_behavior_t;
typedef struct OpenAPI_expected_ue_behavior_s {
    OpenAPI_list_t *exp_move_trajectory;
    char *validity_time;
} OpenAPI_expected_ue_behavior_t;

OpenAPI_expected_ue_behavior_t *OpenAPI_expected_ue_behavior_create(
    OpenAPI_list_t *exp_move_trajectory,
    char *validity_time
);
void OpenAPI_expected_ue_behavior_free(OpenAPI_expected_ue_behavior_t *expected_ue_behavior);
OpenAPI_expected_ue_behavior_t *OpenAPI_expected_ue_behavior_parseFromJSON(cJSON *expected_ue_behaviorJSON);
cJSON *OpenAPI_expected_ue_behavior_convertToJSON(OpenAPI_expected_ue_behavior_t *expected_ue_behavior);
OpenAPI_expected_ue_behavior_t *OpenAPI_expected_ue_behavior_copy(OpenAPI_expected_ue_behavior_t *dst, OpenAPI_expected_ue_behavior_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_expected_ue_behavior_H_ */

