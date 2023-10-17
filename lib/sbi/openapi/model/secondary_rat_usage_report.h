/*
 * secondary_rat_usage_report.h
 *
 * Secondary RAT Usage Report to report usage data for a secondary RAT for QoS flows.
 */

#ifndef _OpenAPI_secondary_rat_usage_report_H_
#define _OpenAPI_secondary_rat_usage_report_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "qos_flow_usage_report.h"
#include "rat_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_secondary_rat_usage_report_s OpenAPI_secondary_rat_usage_report_t;
typedef struct OpenAPI_secondary_rat_usage_report_s {
    OpenAPI_rat_type_e secondary_rat_type;
    OpenAPI_list_t *qos_flows_usage_data;
} OpenAPI_secondary_rat_usage_report_t;

OpenAPI_secondary_rat_usage_report_t *OpenAPI_secondary_rat_usage_report_create(
    OpenAPI_rat_type_e secondary_rat_type,
    OpenAPI_list_t *qos_flows_usage_data
);
void OpenAPI_secondary_rat_usage_report_free(OpenAPI_secondary_rat_usage_report_t *secondary_rat_usage_report);
OpenAPI_secondary_rat_usage_report_t *OpenAPI_secondary_rat_usage_report_parseFromJSON(cJSON *secondary_rat_usage_reportJSON);
cJSON *OpenAPI_secondary_rat_usage_report_convertToJSON(OpenAPI_secondary_rat_usage_report_t *secondary_rat_usage_report);
OpenAPI_secondary_rat_usage_report_t *OpenAPI_secondary_rat_usage_report_copy(OpenAPI_secondary_rat_usage_report_t *dst, OpenAPI_secondary_rat_usage_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_secondary_rat_usage_report_H_ */

