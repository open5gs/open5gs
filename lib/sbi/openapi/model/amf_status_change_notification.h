/*
 * amf_status_change_notification.h
 *
 * Data within an AMF Status Change Notification request
 */

#ifndef _OpenAPI_amf_status_change_notification_H_
#define _OpenAPI_amf_status_change_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "amf_status_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_amf_status_change_notification_s OpenAPI_amf_status_change_notification_t;
typedef struct OpenAPI_amf_status_change_notification_s {
    OpenAPI_list_t *amf_status_info_list;
} OpenAPI_amf_status_change_notification_t;

OpenAPI_amf_status_change_notification_t *OpenAPI_amf_status_change_notification_create(
    OpenAPI_list_t *amf_status_info_list
);
void OpenAPI_amf_status_change_notification_free(OpenAPI_amf_status_change_notification_t *amf_status_change_notification);
OpenAPI_amf_status_change_notification_t *OpenAPI_amf_status_change_notification_parseFromJSON(cJSON *amf_status_change_notificationJSON);
cJSON *OpenAPI_amf_status_change_notification_convertToJSON(OpenAPI_amf_status_change_notification_t *amf_status_change_notification);
OpenAPI_amf_status_change_notification_t *OpenAPI_amf_status_change_notification_copy(OpenAPI_amf_status_change_notification_t *dst, OpenAPI_amf_status_change_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_status_change_notification_H_ */

