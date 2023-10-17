/*
 * qos_flow_usage_report.h
 *
 * Contains QoS flows usage data information.
 */

#ifndef _OpenAPI_qos_flow_usage_report_H_
#define _OpenAPI_qos_flow_usage_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_flow_usage_report_s OpenAPI_qos_flow_usage_report_t;
typedef struct OpenAPI_qos_flow_usage_report_s {
    int qfi;
    char *start_time_stamp;
    char *end_time_stamp;
    long downlink_volume;
    long uplink_volume;
} OpenAPI_qos_flow_usage_report_t;

OpenAPI_qos_flow_usage_report_t *OpenAPI_qos_flow_usage_report_create(
    int qfi,
    char *start_time_stamp,
    char *end_time_stamp,
    long downlink_volume,
    long uplink_volume
);
void OpenAPI_qos_flow_usage_report_free(OpenAPI_qos_flow_usage_report_t *qos_flow_usage_report);
OpenAPI_qos_flow_usage_report_t *OpenAPI_qos_flow_usage_report_parseFromJSON(cJSON *qos_flow_usage_reportJSON);
cJSON *OpenAPI_qos_flow_usage_report_convertToJSON(OpenAPI_qos_flow_usage_report_t *qos_flow_usage_report);
OpenAPI_qos_flow_usage_report_t *OpenAPI_qos_flow_usage_report_copy(OpenAPI_qos_flow_usage_report_t *dst, OpenAPI_qos_flow_usage_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_flow_usage_report_H_ */

