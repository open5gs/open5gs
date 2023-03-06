/*
 * bsf_notification.h
 *
 * Contains the event notifications.
 */

#ifndef _OpenAPI_bsf_notification_H_
#define _OpenAPI_bsf_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "binding_level.h"
#include "bsf_event_notification.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_bsf_notification_s OpenAPI_bsf_notification_t;
typedef struct OpenAPI_bsf_notification_s {
    char *notif_corre_id;
    char *pcf_id;
    char *pcf_set_id;
    OpenAPI_binding_level_e bind_level;
    OpenAPI_list_t *event_notifs;
} OpenAPI_bsf_notification_t;

OpenAPI_bsf_notification_t *OpenAPI_bsf_notification_create(
    char *notif_corre_id,
    char *pcf_id,
    char *pcf_set_id,
    OpenAPI_binding_level_e bind_level,
    OpenAPI_list_t *event_notifs
);
void OpenAPI_bsf_notification_free(OpenAPI_bsf_notification_t *bsf_notification);
OpenAPI_bsf_notification_t *OpenAPI_bsf_notification_parseFromJSON(cJSON *bsf_notificationJSON);
cJSON *OpenAPI_bsf_notification_convertToJSON(OpenAPI_bsf_notification_t *bsf_notification);
OpenAPI_bsf_notification_t *OpenAPI_bsf_notification_copy(OpenAPI_bsf_notification_t *dst, OpenAPI_bsf_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bsf_notification_H_ */

