/*
 * eas_deployment_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_eas_deployment_charging_information_H_
#define _OpenAPI_eas_deployment_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_eas_deployment_charging_information_s OpenAPI_eas_deployment_charging_information_t;
#include "eas_requirements.h"
#include "management_operation.h"
#include "satellite_backhaul_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_eas_deployment_charging_information_s {
    struct OpenAPI_eas_requirements_s *e_eas_deployment_requirements;
    OpenAPI_management_operation_e l_cm_event_type;
    char *l_cm_start_time;
    char *l_cm_end_time;
    struct OpenAPI_satellite_backhaul_information_s *satellite_backhaul_information;
};

OpenAPI_eas_deployment_charging_information_t *OpenAPI_eas_deployment_charging_information_create(
    OpenAPI_eas_requirements_t *e_eas_deployment_requirements,
    OpenAPI_management_operation_e l_cm_event_type,
    char *l_cm_start_time,
    char *l_cm_end_time,
    OpenAPI_satellite_backhaul_information_t *satellite_backhaul_information
);
void OpenAPI_eas_deployment_charging_information_free(OpenAPI_eas_deployment_charging_information_t *eas_deployment_charging_information);
OpenAPI_eas_deployment_charging_information_t *OpenAPI_eas_deployment_charging_information_parseFromJSON(cJSON *eas_deployment_charging_informationJSON);
cJSON *OpenAPI_eas_deployment_charging_information_convertToJSON(OpenAPI_eas_deployment_charging_information_t *eas_deployment_charging_information);
OpenAPI_eas_deployment_charging_information_t *OpenAPI_eas_deployment_charging_information_copy(OpenAPI_eas_deployment_charging_information_t *dst, OpenAPI_eas_deployment_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eas_deployment_charging_information_H_ */

