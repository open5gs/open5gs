/*
 * qos_flow_notify_item.h
 *
 * 
 */

#ifndef _OpenAPI_qos_flow_notify_item_H_
#define _OpenAPI_qos_flow_notify_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "notification_cause.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_flow_notify_item_s OpenAPI_qos_flow_notify_item_t;
typedef struct OpenAPI_qos_flow_notify_item_s {
    int qfi;
    OpenAPI_notification_cause_e notification_cause;
    bool is_current_qos_profile_index;
    int current_qos_profile_index;
    bool is_null_qo_s_profile_index;
    int null_qo_s_profile_index;
} OpenAPI_qos_flow_notify_item_t;

OpenAPI_qos_flow_notify_item_t *OpenAPI_qos_flow_notify_item_create(
    int qfi,
    OpenAPI_notification_cause_e notification_cause,
    bool is_current_qos_profile_index,
    int current_qos_profile_index,
    bool is_null_qo_s_profile_index,
    int null_qo_s_profile_index
);
void OpenAPI_qos_flow_notify_item_free(OpenAPI_qos_flow_notify_item_t *qos_flow_notify_item);
OpenAPI_qos_flow_notify_item_t *OpenAPI_qos_flow_notify_item_parseFromJSON(cJSON *qos_flow_notify_itemJSON);
cJSON *OpenAPI_qos_flow_notify_item_convertToJSON(OpenAPI_qos_flow_notify_item_t *qos_flow_notify_item);
OpenAPI_qos_flow_notify_item_t *OpenAPI_qos_flow_notify_item_copy(OpenAPI_qos_flow_notify_item_t *dst, OpenAPI_qos_flow_notify_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_flow_notify_item_H_ */

