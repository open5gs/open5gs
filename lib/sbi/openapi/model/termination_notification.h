/*
 * termination_notification.h
 *
 * Represents a Termination Notification.
 */

#ifndef _OpenAPI_termination_notification_H_
#define _OpenAPI_termination_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sm_policy_association_release_cause.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_termination_notification_s OpenAPI_termination_notification_t;
typedef struct OpenAPI_termination_notification_s {
    char *resource_uri;
    OpenAPI_sm_policy_association_release_cause_e cause;
} OpenAPI_termination_notification_t;

OpenAPI_termination_notification_t *OpenAPI_termination_notification_create(
    char *resource_uri,
    OpenAPI_sm_policy_association_release_cause_e cause
);
void OpenAPI_termination_notification_free(OpenAPI_termination_notification_t *termination_notification);
OpenAPI_termination_notification_t *OpenAPI_termination_notification_parseFromJSON(cJSON *termination_notificationJSON);
cJSON *OpenAPI_termination_notification_convertToJSON(OpenAPI_termination_notification_t *termination_notification);
OpenAPI_termination_notification_t *OpenAPI_termination_notification_copy(OpenAPI_termination_notification_t *dst, OpenAPI_termination_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_termination_notification_H_ */

