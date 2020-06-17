/*
 * monitoring_configuration.h
 *
 *
 */

#ifndef _OpenAPI_monitoring_configuration_H_
#define _OpenAPI_monitoring_configuration_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "association_type.h"
#include "datalink_reporting_configuration.h"
#include "event_type.h"
#include "location_reporting_configuration.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_monitoring_configuration_s OpenAPI_monitoring_configuration_t;
typedef struct OpenAPI_monitoring_configuration_s {
    struct OpenAPI_event_type_s *event_type;
    int immediate_flag;
    struct OpenAPI_location_reporting_configuration_s *location_reporting_configuration;
    struct OpenAPI_association_type_s *association_type;
    struct OpenAPI_datalink_reporting_configuration_s *datalink_report_cfg;
} OpenAPI_monitoring_configuration_t;

OpenAPI_monitoring_configuration_t *OpenAPI_monitoring_configuration_create(
    OpenAPI_event_type_t *event_type,
    int immediate_flag,
    OpenAPI_location_reporting_configuration_t *location_reporting_configuration,
    OpenAPI_association_type_t *association_type,
    OpenAPI_datalink_reporting_configuration_t *datalink_report_cfg
    );
void OpenAPI_monitoring_configuration_free(OpenAPI_monitoring_configuration_t *monitoring_configuration);
OpenAPI_monitoring_configuration_t *OpenAPI_monitoring_configuration_parseFromJSON(cJSON *monitoring_configurationJSON);
cJSON *OpenAPI_monitoring_configuration_convertToJSON(OpenAPI_monitoring_configuration_t *monitoring_configuration);
OpenAPI_monitoring_configuration_t *OpenAPI_monitoring_configuration_copy(OpenAPI_monitoring_configuration_t *dst, OpenAPI_monitoring_configuration_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_monitoring_configuration_H_ */

