/*
 * qos_monitoring_report.h
 *
 * QoS Monitoring reporting information.
 */

#ifndef _OpenAPI_qos_monitoring_report_H_
#define _OpenAPI_qos_monitoring_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "flows.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_monitoring_report_s OpenAPI_qos_monitoring_report_t;
typedef struct OpenAPI_qos_monitoring_report_s {
    OpenAPI_list_t *flows;
    OpenAPI_list_t *ul_delays;
    OpenAPI_list_t *dl_delays;
    OpenAPI_list_t *rt_delays;
} OpenAPI_qos_monitoring_report_t;

OpenAPI_qos_monitoring_report_t *OpenAPI_qos_monitoring_report_create(
    OpenAPI_list_t *flows,
    OpenAPI_list_t *ul_delays,
    OpenAPI_list_t *dl_delays,
    OpenAPI_list_t *rt_delays
);
void OpenAPI_qos_monitoring_report_free(OpenAPI_qos_monitoring_report_t *qos_monitoring_report);
OpenAPI_qos_monitoring_report_t *OpenAPI_qos_monitoring_report_parseFromJSON(cJSON *qos_monitoring_reportJSON);
cJSON *OpenAPI_qos_monitoring_report_convertToJSON(OpenAPI_qos_monitoring_report_t *qos_monitoring_report);
OpenAPI_qos_monitoring_report_t *OpenAPI_qos_monitoring_report_copy(OpenAPI_qos_monitoring_report_t *dst, OpenAPI_qos_monitoring_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_monitoring_report_H_ */

