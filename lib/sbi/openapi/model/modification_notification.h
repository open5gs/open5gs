/*
 * modification_notification.h
 *
 * 
 */

#ifndef _OpenAPI_modification_notification_H_
#define _OpenAPI_modification_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "notify_item.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_modification_notification_s OpenAPI_modification_notification_t;
typedef struct OpenAPI_modification_notification_s {
    OpenAPI_list_t *notify_items;
} OpenAPI_modification_notification_t;

OpenAPI_modification_notification_t *OpenAPI_modification_notification_create(
    OpenAPI_list_t *notify_items
);
void OpenAPI_modification_notification_free(OpenAPI_modification_notification_t *modification_notification);
OpenAPI_modification_notification_t *OpenAPI_modification_notification_parseFromJSON(cJSON *modification_notificationJSON);
cJSON *OpenAPI_modification_notification_convertToJSON(OpenAPI_modification_notification_t *modification_notification);
OpenAPI_modification_notification_t *OpenAPI_modification_notification_copy(OpenAPI_modification_notification_t *dst, OpenAPI_modification_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_modification_notification_H_ */

