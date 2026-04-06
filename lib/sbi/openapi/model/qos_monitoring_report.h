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
typedef struct OpenAPI_qos_monitoring_report_s OpenAPI_qos_monitoring_report_t;
#include "flows.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qos_monitoring_report_s {
    OpenAPI_list_t *flows;
    OpenAPI_list_t *ul_delays;
    OpenAPI_list_t *dl_delays;
    OpenAPI_list_t *rt_delays;
    bool is_pdmf;
    int pdmf;
    OpenAPI_list_t *ul_con_info;
    OpenAPI_list_t *dl_con_info;
    char *ul_data_rate;
    char *dl_data_rate;
};

OpenAPI_qos_monitoring_report_t *OpenAPI_qos_monitoring_report_create(
    OpenAPI_list_t *flows,
    OpenAPI_list_t *ul_delays,
    OpenAPI_list_t *dl_delays,
    OpenAPI_list_t *rt_delays,
    bool is_pdmf,
    int pdmf,
    OpenAPI_list_t *ul_con_info,
    OpenAPI_list_t *dl_con_info,
    char *ul_data_rate,
    char *dl_data_rate
);
void OpenAPI_qos_monitoring_report_free(OpenAPI_qos_monitoring_report_t *qos_monitoring_report);
OpenAPI_qos_monitoring_report_t *OpenAPI_qos_monitoring_report_parseFromJSON(cJSON *qos_monitoring_reportJSON);
cJSON *OpenAPI_qos_monitoring_report_convertToJSON(OpenAPI_qos_monitoring_report_t *qos_monitoring_report);
OpenAPI_qos_monitoring_report_t *OpenAPI_qos_monitoring_report_copy(OpenAPI_qos_monitoring_report_t *dst, OpenAPI_qos_monitoring_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_monitoring_report_H_ */

