/*
 * pending_policy_counter_status.h
 *
 * Represents the data structure presenting the pending policy counter status. 
 */

#ifndef _OpenAPI_pending_policy_counter_status_H_
#define _OpenAPI_pending_policy_counter_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pending_policy_counter_status_s OpenAPI_pending_policy_counter_status_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pending_policy_counter_status_s {
    char *policy_counter_status;
    char *activation_time;
};

OpenAPI_pending_policy_counter_status_t *OpenAPI_pending_policy_counter_status_create(
    char *policy_counter_status,
    char *activation_time
);
void OpenAPI_pending_policy_counter_status_free(OpenAPI_pending_policy_counter_status_t *pending_policy_counter_status);
OpenAPI_pending_policy_counter_status_t *OpenAPI_pending_policy_counter_status_parseFromJSON(cJSON *pending_policy_counter_statusJSON);
cJSON *OpenAPI_pending_policy_counter_status_convertToJSON(OpenAPI_pending_policy_counter_status_t *pending_policy_counter_status);
OpenAPI_pending_policy_counter_status_t *OpenAPI_pending_policy_counter_status_copy(OpenAPI_pending_policy_counter_status_t *dst, OpenAPI_pending_policy_counter_status_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pending_policy_counter_status_H_ */

