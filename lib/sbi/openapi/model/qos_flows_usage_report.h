/*
 * qos_flows_usage_report.h
 *
 * 
 */

#ifndef _OpenAPI_qos_flows_usage_report_H_
#define _OpenAPI_qos_flows_usage_report_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qos_flows_usage_report_s OpenAPI_qos_flows_usage_report_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qos_flows_usage_report_s {
    bool is_q_fi;
    int q_fi;
    char *start_timestamp;
    char *end_timestamp;
    bool is_uplink_volume;
    long uplink_volume;
    bool is_downlink_volume;
    long downlink_volume;
};

OpenAPI_qos_flows_usage_report_t *OpenAPI_qos_flows_usage_report_create(
    bool is_q_fi,
    int q_fi,
    char *start_timestamp,
    char *end_timestamp,
    bool is_uplink_volume,
    long uplink_volume,
    bool is_downlink_volume,
    long downlink_volume
);
void OpenAPI_qos_flows_usage_report_free(OpenAPI_qos_flows_usage_report_t *qos_flows_usage_report);
OpenAPI_qos_flows_usage_report_t *OpenAPI_qos_flows_usage_report_parseFromJSON(cJSON *qos_flows_usage_reportJSON);
cJSON *OpenAPI_qos_flows_usage_report_convertToJSON(OpenAPI_qos_flows_usage_report_t *qos_flows_usage_report);
OpenAPI_qos_flows_usage_report_t *OpenAPI_qos_flows_usage_report_copy(OpenAPI_qos_flows_usage_report_t *dst, OpenAPI_qos_flows_usage_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_flows_usage_report_H_ */

