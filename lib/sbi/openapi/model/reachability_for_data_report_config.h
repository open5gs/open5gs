/*
 * reachability_for_data_report_config.h
 *
 * Indicates the configuration for report of reachability status for data. 
 */

#ifndef _OpenAPI_reachability_for_data_report_config_H_
#define _OpenAPI_reachability_for_data_report_config_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_reachability_for_data_report_config_NULL = 0, OpenAPI_reachability_for_data_report_config_DIRECT_REPORT, OpenAPI_reachability_for_data_report_config_INDIRECT_REPORT } OpenAPI_reachability_for_data_report_config_e;

char* OpenAPI_reachability_for_data_report_config_ToString(OpenAPI_reachability_for_data_report_config_e reachability_for_data_report_config);

OpenAPI_reachability_for_data_report_config_e OpenAPI_reachability_for_data_report_config_FromString(char* reachability_for_data_report_config);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reachability_for_data_report_config_H_ */

