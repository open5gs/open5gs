/*
 * location_reporting_configuration.h
 *
 * 
 */

#ifndef _OpenAPI_location_reporting_configuration_H_
#define _OpenAPI_location_reporting_configuration_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_location_reporting_configuration_s OpenAPI_location_reporting_configuration_t;
#include "location_accuracy.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_location_reporting_configuration_s {
    int current_location;
    bool is_one_time;
    int one_time;
    OpenAPI_location_accuracy_e accuracy;
    OpenAPI_location_accuracy_e n3gpp_accuracy;
};

OpenAPI_location_reporting_configuration_t *OpenAPI_location_reporting_configuration_create(
    int current_location,
    bool is_one_time,
    int one_time,
    OpenAPI_location_accuracy_e accuracy,
    OpenAPI_location_accuracy_e n3gpp_accuracy
);
void OpenAPI_location_reporting_configuration_free(OpenAPI_location_reporting_configuration_t *location_reporting_configuration);
OpenAPI_location_reporting_configuration_t *OpenAPI_location_reporting_configuration_parseFromJSON(cJSON *location_reporting_configurationJSON);
cJSON *OpenAPI_location_reporting_configuration_convertToJSON(OpenAPI_location_reporting_configuration_t *location_reporting_configuration);
OpenAPI_location_reporting_configuration_t *OpenAPI_location_reporting_configuration_copy(OpenAPI_location_reporting_configuration_t *dst, OpenAPI_location_reporting_configuration_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_reporting_configuration_H_ */

