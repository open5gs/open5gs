/*
 * ran_secondary_rat_usage_report.h
 *
 * 
 */

#ifndef _OpenAPI_ran_secondary_rat_usage_report_H_
#define _OpenAPI_ran_secondary_rat_usage_report_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ran_secondary_rat_usage_report_s OpenAPI_ran_secondary_rat_usage_report_t;
#include "qos_flows_usage_report.h"
#include "rat_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ran_secondary_rat_usage_report_s {
    OpenAPI_rat_type_e r_an_secondary_rat_type;
    OpenAPI_list_t *qos_flows_usage_reports;
};

OpenAPI_ran_secondary_rat_usage_report_t *OpenAPI_ran_secondary_rat_usage_report_create(
    OpenAPI_rat_type_e r_an_secondary_rat_type,
    OpenAPI_list_t *qos_flows_usage_reports
);
void OpenAPI_ran_secondary_rat_usage_report_free(OpenAPI_ran_secondary_rat_usage_report_t *ran_secondary_rat_usage_report);
OpenAPI_ran_secondary_rat_usage_report_t *OpenAPI_ran_secondary_rat_usage_report_parseFromJSON(cJSON *ran_secondary_rat_usage_reportJSON);
cJSON *OpenAPI_ran_secondary_rat_usage_report_convertToJSON(OpenAPI_ran_secondary_rat_usage_report_t *ran_secondary_rat_usage_report);
OpenAPI_ran_secondary_rat_usage_report_t *OpenAPI_ran_secondary_rat_usage_report_copy(OpenAPI_ran_secondary_rat_usage_report_t *dst, OpenAPI_ran_secondary_rat_usage_report_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ran_secondary_rat_usage_report_H_ */

