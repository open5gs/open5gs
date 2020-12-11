/*
 * qos_notification_control_info.h
 *
 *
 */

#ifndef _OpenAPI_qos_notification_control_info_H_
#define _OpenAPI_qos_notification_control_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "qos_notif_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_notification_control_info_s OpenAPI_qos_notification_control_info_t;
typedef struct OpenAPI_qos_notification_control_info_s {
    OpenAPI_list_t *ref_pcc_rule_ids;
    struct OpenAPI_qos_notif_type_s *notif_type;
    int cont_ver;
    char *alt_qos_param_id;
} OpenAPI_qos_notification_control_info_t;

OpenAPI_qos_notification_control_info_t *OpenAPI_qos_notification_control_info_create(
    OpenAPI_list_t *ref_pcc_rule_ids,
    OpenAPI_qos_notif_type_t *notif_type,
    int cont_ver,
    char *alt_qos_param_id
    );
void OpenAPI_qos_notification_control_info_free(OpenAPI_qos_notification_control_info_t *qos_notification_control_info);
OpenAPI_qos_notification_control_info_t *OpenAPI_qos_notification_control_info_parseFromJSON(cJSON *qos_notification_control_infoJSON);
cJSON *OpenAPI_qos_notification_control_info_convertToJSON(OpenAPI_qos_notification_control_info_t *qos_notification_control_info);
OpenAPI_qos_notification_control_info_t *OpenAPI_qos_notification_control_info_copy(OpenAPI_qos_notification_control_info_t *dst, OpenAPI_qos_notification_control_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_notification_control_info_H_ */

