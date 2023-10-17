/*
 * qos_notification_control_info.h
 *
 * Indicates whether the QoS targets for a GRB flow are not guaranteed or guaranteed again.
 */

#ifndef _OpenAPI_qos_notification_control_info_H_
#define _OpenAPI_qos_notification_control_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "flows.h"
#include "qos_notif_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_notification_control_info_s OpenAPI_qos_notification_control_info_t;
typedef struct OpenAPI_qos_notification_control_info_s {
    OpenAPI_qos_notif_type_e notif_type;
    OpenAPI_list_t *flows;
    char *alt_ser_req;
} OpenAPI_qos_notification_control_info_t;

OpenAPI_qos_notification_control_info_t *OpenAPI_qos_notification_control_info_create(
    OpenAPI_qos_notif_type_e notif_type,
    OpenAPI_list_t *flows,
    char *alt_ser_req
);
void OpenAPI_qos_notification_control_info_free(OpenAPI_qos_notification_control_info_t *qos_notification_control_info);
OpenAPI_qos_notification_control_info_t *OpenAPI_qos_notification_control_info_parseFromJSON(cJSON *qos_notification_control_infoJSON);
cJSON *OpenAPI_qos_notification_control_info_convertToJSON(OpenAPI_qos_notification_control_info_t *qos_notification_control_info);
OpenAPI_qos_notification_control_info_t *OpenAPI_qos_notification_control_info_copy(OpenAPI_qos_notification_control_info_t *dst, OpenAPI_qos_notification_control_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_notification_control_info_H_ */

