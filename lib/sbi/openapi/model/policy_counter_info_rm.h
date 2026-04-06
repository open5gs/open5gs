/*
 * policy_counter_info_rm.h
 *
 * Represents the data structure presenting the policy counter status.
 */

#ifndef _OpenAPI_policy_counter_info_rm_H_
#define _OpenAPI_policy_counter_info_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_policy_counter_info_rm_s OpenAPI_policy_counter_info_rm_t;
#include "pending_policy_counter_status.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_policy_counter_info_rm_s {
    bool is_current_status_null;
    char *current_status;
    bool is_pen_pol_counter_statuses_null;
    OpenAPI_list_t *pen_pol_counter_statuses;
};

OpenAPI_policy_counter_info_rm_t *OpenAPI_policy_counter_info_rm_create(
    bool is_current_status_null,
    char *current_status,
    bool is_pen_pol_counter_statuses_null,
    OpenAPI_list_t *pen_pol_counter_statuses
);
void OpenAPI_policy_counter_info_rm_free(OpenAPI_policy_counter_info_rm_t *policy_counter_info_rm);
OpenAPI_policy_counter_info_rm_t *OpenAPI_policy_counter_info_rm_parseFromJSON(cJSON *policy_counter_info_rmJSON);
cJSON *OpenAPI_policy_counter_info_rm_convertToJSON(OpenAPI_policy_counter_info_rm_t *policy_counter_info_rm);
OpenAPI_policy_counter_info_rm_t *OpenAPI_policy_counter_info_rm_copy(OpenAPI_policy_counter_info_rm_t *dst, OpenAPI_policy_counter_info_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_counter_info_rm_H_ */

