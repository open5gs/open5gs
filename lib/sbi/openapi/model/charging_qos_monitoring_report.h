/*
 * charging_qos_monitoring_report.h
 *
 * Contains reporting information on QoS monitoring.
 */

#ifndef _OpenAPI_charging_qos_monitoring_report_H_
#define _OpenAPI_charging_qos_monitoring_report_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_charging_qos_monitoring_report_s OpenAPI_charging_qos_monitoring_report_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_charging_qos_monitoring_report_s {
    OpenAPI_list_t *ul_delays;
    OpenAPI_list_t *dl_delays;
    OpenAPI_list_t *rt_delays;
};

OpenAPI_charging_qos_monitoring_report_t *OpenAPI_charging_qos_monitoring_report_create(
    OpenAPI_list_t *ul_delays,
    OpenAPI_list_t *dl_delays,
    OpenAPI_list_t *rt_delays
);
void OpenAPI_charging_qos_monitoring_report_free(OpenAPI_charging_qos_monitoring_report_t *charging_qos_monitoring_report);
OpenAPI_charging_qos_monitoring_report_t *OpenAPI_charging_qos_monitoring_report_parseFromJSON(cJSON *charging_qos_monitoring_reportJSON);
cJSON *OpenAPI_charging_qos_monitoring_report_convertToJSON(OpenAPI_charging_qos_monitoring_report_t *charging_qos_monitoring_report);
OpenAPI_charging_qos_monitoring_report_t *OpenAPI_charging_qos_monitoring_report_copy(OpenAPI_charging_qos_monitoring_report_t *dst, OpenAPI_charging_qos_monitoring_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_qos_monitoring_report_H_ */

