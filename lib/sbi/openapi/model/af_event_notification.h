/*
 * af_event_notification.h
 *
 * Describes the event information delivered in the notification.
 */

#ifndef _OpenAPI_af_event_notification_H_
#define _OpenAPI_af_event_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_af_event_notification_s OpenAPI_af_event_notification_t;
#include "flows.h"
#include "npcf_af_event.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_af_event_notification_s {
    OpenAPI_npcf_af_event_e event;
    OpenAPI_list_t *flows;
    bool is_retry_after;
    int retry_after;
};

OpenAPI_af_event_notification_t *OpenAPI_af_event_notification_create(
    OpenAPI_npcf_af_event_e event,
    OpenAPI_list_t *flows,
    bool is_retry_after,
    int retry_after
);
void OpenAPI_af_event_notification_free(OpenAPI_af_event_notification_t *af_event_notification);
OpenAPI_af_event_notification_t *OpenAPI_af_event_notification_parseFromJSON(cJSON *af_event_notificationJSON);
cJSON *OpenAPI_af_event_notification_convertToJSON(OpenAPI_af_event_notification_t *af_event_notification);
OpenAPI_af_event_notification_t *OpenAPI_af_event_notification_copy(OpenAPI_af_event_notification_t *dst, OpenAPI_af_event_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_event_notification_H_ */

