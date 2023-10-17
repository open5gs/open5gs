/*
 * reachability_for_data_report_config.h
 *
 * 
 */

#ifndef _OpenAPI_reachability_for_data_report_config_H_
#define _OpenAPI_reachability_for_data_report_config_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "reachability_for_data_report_config_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_reachability_for_data_report_config_s OpenAPI_reachability_for_data_report_config_t;
typedef struct OpenAPI_reachability_for_data_report_config_s {
} OpenAPI_reachability_for_data_report_config_t;

OpenAPI_reachability_for_data_report_config_t *OpenAPI_reachability_for_data_report_config_create(
);
void OpenAPI_reachability_for_data_report_config_free(OpenAPI_reachability_for_data_report_config_t *reachability_for_data_report_config);
OpenAPI_reachability_for_data_report_config_t *OpenAPI_reachability_for_data_report_config_parseFromJSON(cJSON *reachability_for_data_report_configJSON);
cJSON *OpenAPI_reachability_for_data_report_config_convertToJSON(OpenAPI_reachability_for_data_report_config_t *reachability_for_data_report_config);
OpenAPI_reachability_for_data_report_config_t *OpenAPI_reachability_for_data_report_config_copy(OpenAPI_reachability_for_data_report_config_t *dst, OpenAPI_reachability_for_data_report_config_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reachability_for_data_report_config_H_ */

