/*
 * location_reporting_configuration_1.h
 *
 * 
 */

#ifndef _OpenAPI_location_reporting_configuration_1_H_
#define _OpenAPI_location_reporting_configuration_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "location_accuracy.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_location_reporting_configuration_1_s OpenAPI_location_reporting_configuration_1_t;
typedef struct OpenAPI_location_reporting_configuration_1_s {
    int current_location;
    bool is_one_time;
    int one_time;
    struct OpenAPI_location_accuracy_s *accuracy;
    struct OpenAPI_location_accuracy_s *n3gpp_accuracy;
} OpenAPI_location_reporting_configuration_1_t;

OpenAPI_location_reporting_configuration_1_t *OpenAPI_location_reporting_configuration_1_create(
    int current_location,
    bool is_one_time,
    int one_time,
    OpenAPI_location_accuracy_t *accuracy,
    OpenAPI_location_accuracy_t *n3gpp_accuracy
);
void OpenAPI_location_reporting_configuration_1_free(OpenAPI_location_reporting_configuration_1_t *location_reporting_configuration_1);
OpenAPI_location_reporting_configuration_1_t *OpenAPI_location_reporting_configuration_1_parseFromJSON(cJSON *location_reporting_configuration_1JSON);
cJSON *OpenAPI_location_reporting_configuration_1_convertToJSON(OpenAPI_location_reporting_configuration_1_t *location_reporting_configuration_1);
OpenAPI_location_reporting_configuration_1_t *OpenAPI_location_reporting_configuration_1_copy(OpenAPI_location_reporting_configuration_1_t *dst, OpenAPI_location_reporting_configuration_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_reporting_configuration_1_H_ */

