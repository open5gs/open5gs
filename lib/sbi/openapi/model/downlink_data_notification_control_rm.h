/*
 * downlink_data_notification_control_rm.h
 *
 * this data type is defined in the same way as the DownlinkDataNotificationControl data type, but with the OpenAPI nullable property set to true.
 */

#ifndef _OpenAPI_downlink_data_notification_control_rm_H_
#define _OpenAPI_downlink_data_notification_control_rm_H_

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

typedef struct OpenAPI_downlink_data_notification_control_rm_s OpenAPI_downlink_data_notification_control_rm_t;
typedef struct OpenAPI_downlink_data_notification_control_rm_s {
    OpenAPI_list_t *notif_ctrl_inds;
    OpenAPI_list_t *types_of_notif;
} OpenAPI_downlink_data_notification_control_rm_t;

OpenAPI_downlink_data_notification_control_rm_t *OpenAPI_downlink_data_notification_control_rm_create(
    OpenAPI_list_t *notif_ctrl_inds,
    OpenAPI_list_t *types_of_notif
);
void OpenAPI_downlink_data_notification_control_rm_free(OpenAPI_downlink_data_notification_control_rm_t *downlink_data_notification_control_rm);
OpenAPI_downlink_data_notification_control_rm_t *OpenAPI_downlink_data_notification_control_rm_parseFromJSON(cJSON *downlink_data_notification_control_rmJSON);
cJSON *OpenAPI_downlink_data_notification_control_rm_convertToJSON(OpenAPI_downlink_data_notification_control_rm_t *downlink_data_notification_control_rm);
OpenAPI_downlink_data_notification_control_rm_t *OpenAPI_downlink_data_notification_control_rm_copy(OpenAPI_downlink_data_notification_control_rm_t *dst, OpenAPI_downlink_data_notification_control_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_downlink_data_notification_control_rm_H_ */

