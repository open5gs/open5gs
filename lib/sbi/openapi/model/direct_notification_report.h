/*
 * direct_notification_report.h
 *
 * Represents the QoS monitoring parameters that cannot be directly notified for the indicated flows. 
 */

#ifndef _OpenAPI_direct_notification_report_H_
#define _OpenAPI_direct_notification_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_direct_notification_report_s OpenAPI_direct_notification_report_t;
#include "flows.h"
#include "qos_monitoring_param_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_direct_notification_report_s {
    OpenAPI_qos_monitoring_param_type_e qos_mon_param_type;
    OpenAPI_list_t *flows;
};

OpenAPI_direct_notification_report_t *OpenAPI_direct_notification_report_create(
    OpenAPI_qos_monitoring_param_type_e qos_mon_param_type,
    OpenAPI_list_t *flows
);
void OpenAPI_direct_notification_report_free(OpenAPI_direct_notification_report_t *direct_notification_report);
OpenAPI_direct_notification_report_t *OpenAPI_direct_notification_report_parseFromJSON(cJSON *direct_notification_reportJSON);
cJSON *OpenAPI_direct_notification_report_convertToJSON(OpenAPI_direct_notification_report_t *direct_notification_report);
OpenAPI_direct_notification_report_t *OpenAPI_direct_notification_report_copy(OpenAPI_direct_notification_report_t *dst, OpenAPI_direct_notification_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_direct_notification_report_H_ */

