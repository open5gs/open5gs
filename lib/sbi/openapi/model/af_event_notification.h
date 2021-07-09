/*
 * af_event_notification.h
 *
 * describes the event information delivered in the notification
 */

#ifndef _OpenAPI_af_event_notification_H_
#define _OpenAPI_af_event_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_event.h"
#include "flows.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_af_event_notification_s OpenAPI_af_event_notification_t;
typedef struct OpenAPI_af_event_notification_s {
    OpenAPI_af_event_e event;
    OpenAPI_list_t *flows;
} OpenAPI_af_event_notification_t;

OpenAPI_af_event_notification_t *OpenAPI_af_event_notification_create(
    OpenAPI_af_event_e event,
    OpenAPI_list_t *flows
);
void OpenAPI_af_event_notification_free(OpenAPI_af_event_notification_t *af_event_notification);
OpenAPI_af_event_notification_t *OpenAPI_af_event_notification_parseFromJSON(cJSON *af_event_notificationJSON);
cJSON *OpenAPI_af_event_notification_convertToJSON(OpenAPI_af_event_notification_t *af_event_notification);
OpenAPI_af_event_notification_t *OpenAPI_af_event_notification_copy(OpenAPI_af_event_notification_t *dst, OpenAPI_af_event_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_event_notification_H_ */

