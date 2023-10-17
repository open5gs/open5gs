/*
 * reachability_for_data_configuration.h
 *
 * 
 */

#ifndef _OpenAPI_reachability_for_data_configuration_H_
#define _OpenAPI_reachability_for_data_configuration_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "reachability_for_data_report_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_reachability_for_data_configuration_s OpenAPI_reachability_for_data_configuration_t;
typedef struct OpenAPI_reachability_for_data_configuration_s {
    struct OpenAPI_reachability_for_data_report_config_s *report_cfg;
    bool is_min_interval;
    int min_interval;
} OpenAPI_reachability_for_data_configuration_t;

OpenAPI_reachability_for_data_configuration_t *OpenAPI_reachability_for_data_configuration_create(
    OpenAPI_reachability_for_data_report_config_t *report_cfg,
    bool is_min_interval,
    int min_interval
);
void OpenAPI_reachability_for_data_configuration_free(OpenAPI_reachability_for_data_configuration_t *reachability_for_data_configuration);
OpenAPI_reachability_for_data_configuration_t *OpenAPI_reachability_for_data_configuration_parseFromJSON(cJSON *reachability_for_data_configurationJSON);
cJSON *OpenAPI_reachability_for_data_configuration_convertToJSON(OpenAPI_reachability_for_data_configuration_t *reachability_for_data_configuration);
OpenAPI_reachability_for_data_configuration_t *OpenAPI_reachability_for_data_configuration_copy(OpenAPI_reachability_for_data_configuration_t *dst, OpenAPI_reachability_for_data_configuration_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reachability_for_data_configuration_H_ */

