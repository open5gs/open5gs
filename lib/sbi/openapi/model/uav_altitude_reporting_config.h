/*
 * uav_altitude_reporting_config.h
 *
 * altitude reporting
 */

#ifndef _OpenAPI_uav_altitude_reporting_config_H_
#define _OpenAPI_uav_altitude_reporting_config_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_uav_altitude_reporting_config_s OpenAPI_uav_altitude_reporting_config_t;
#include "alt_report_event_trigger.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_uav_altitude_reporting_config_s {
    double rpt_threshold_min;
    double rpt_threshold_max;
    bool is_alt_report_period;
    int alt_report_period;
    OpenAPI_list_t *alt_report_event_trigger;
};

OpenAPI_uav_altitude_reporting_config_t *OpenAPI_uav_altitude_reporting_config_create(
    double rpt_threshold_min,
    double rpt_threshold_max,
    bool is_alt_report_period,
    int alt_report_period,
    OpenAPI_list_t *alt_report_event_trigger
);
void OpenAPI_uav_altitude_reporting_config_free(OpenAPI_uav_altitude_reporting_config_t *uav_altitude_reporting_config);
OpenAPI_uav_altitude_reporting_config_t *OpenAPI_uav_altitude_reporting_config_parseFromJSON(cJSON *uav_altitude_reporting_configJSON);
cJSON *OpenAPI_uav_altitude_reporting_config_convertToJSON(OpenAPI_uav_altitude_reporting_config_t *uav_altitude_reporting_config);
OpenAPI_uav_altitude_reporting_config_t *OpenAPI_uav_altitude_reporting_config_copy(OpenAPI_uav_altitude_reporting_config_t *dst, OpenAPI_uav_altitude_reporting_config_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_uav_altitude_reporting_config_H_ */

