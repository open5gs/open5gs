/*
 * policy_counter_info.h
 *
 * Represents the data structure presenting the policy counter status.
 */

#ifndef _OpenAPI_policy_counter_info_H_
#define _OpenAPI_policy_counter_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_policy_counter_info_s OpenAPI_policy_counter_info_t;
#include "pending_policy_counter_status.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_policy_counter_info_s {
    char *policy_counter_id;
    char *current_status;
    OpenAPI_list_t *pen_pol_counter_statuses;
};

OpenAPI_policy_counter_info_t *OpenAPI_policy_counter_info_create(
    char *policy_counter_id,
    char *current_status,
    OpenAPI_list_t *pen_pol_counter_statuses
);
void OpenAPI_policy_counter_info_free(OpenAPI_policy_counter_info_t *policy_counter_info);
OpenAPI_policy_counter_info_t *OpenAPI_policy_counter_info_parseFromJSON(cJSON *policy_counter_infoJSON);
cJSON *OpenAPI_policy_counter_info_convertToJSON(OpenAPI_policy_counter_info_t *policy_counter_info);
OpenAPI_policy_counter_info_t *OpenAPI_policy_counter_info_copy(OpenAPI_policy_counter_info_t *dst, OpenAPI_policy_counter_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_counter_info_H_ */

