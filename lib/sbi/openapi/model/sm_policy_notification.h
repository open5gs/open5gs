/*
 * sm_policy_notification.h
 *
 *
 */

#ifndef _OpenAPI_sm_policy_notification_H_
#define _OpenAPI_sm_policy_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sm_policy_decision.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_policy_notification_s OpenAPI_sm_policy_notification_t;
typedef struct OpenAPI_sm_policy_notification_s {
    char *resource_uri;
    struct OpenAPI_sm_policy_decision_s *sm_policy_decision;
} OpenAPI_sm_policy_notification_t;

OpenAPI_sm_policy_notification_t *OpenAPI_sm_policy_notification_create(
    char *resource_uri,
    OpenAPI_sm_policy_decision_t *sm_policy_decision
    );
void OpenAPI_sm_policy_notification_free(OpenAPI_sm_policy_notification_t *sm_policy_notification);
OpenAPI_sm_policy_notification_t *OpenAPI_sm_policy_notification_parseFromJSON(cJSON *sm_policy_notificationJSON);
cJSON *OpenAPI_sm_policy_notification_convertToJSON(OpenAPI_sm_policy_notification_t *sm_policy_notification);
OpenAPI_sm_policy_notification_t *OpenAPI_sm_policy_notification_copy(OpenAPI_sm_policy_notification_t *dst, OpenAPI_sm_policy_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_policy_notification_H_ */

