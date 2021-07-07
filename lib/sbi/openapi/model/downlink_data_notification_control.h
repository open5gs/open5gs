/*
 * downlink_data_notification_control.h
 *
 * Contains the downlink data notification control information.
 */

#ifndef _OpenAPI_downlink_data_notification_control_H_
#define _OpenAPI_downlink_data_notification_control_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dl_data_delivery_status.h"
#include "notification_control_indication.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_downlink_data_notification_control_s OpenAPI_downlink_data_notification_control_t;
typedef struct OpenAPI_downlink_data_notification_control_s {
    OpenAPI_list_t *notif_ctrl_inds;
    OpenAPI_list_t *types_of_notif;
} OpenAPI_downlink_data_notification_control_t;

OpenAPI_downlink_data_notification_control_t *OpenAPI_downlink_data_notification_control_create(
    OpenAPI_list_t *notif_ctrl_inds,
    OpenAPI_list_t *types_of_notif
);
void OpenAPI_downlink_data_notification_control_free(OpenAPI_downlink_data_notification_control_t *downlink_data_notification_control);
OpenAPI_downlink_data_notification_control_t *OpenAPI_downlink_data_notification_control_parseFromJSON(cJSON *downlink_data_notification_controlJSON);
cJSON *OpenAPI_downlink_data_notification_control_convertToJSON(OpenAPI_downlink_data_notification_control_t *downlink_data_notification_control);
OpenAPI_downlink_data_notification_control_t *OpenAPI_downlink_data_notification_control_copy(OpenAPI_downlink_data_notification_control_t *dst, OpenAPI_downlink_data_notification_control_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_downlink_data_notification_control_H_ */

