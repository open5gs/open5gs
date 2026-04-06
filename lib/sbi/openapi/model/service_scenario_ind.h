/*
 * service_scenario_ind.h
 *
 * The service scenario indication. 
 */

#ifndef _OpenAPI_service_scenario_ind_H_
#define _OpenAPI_service_scenario_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_service_scenario_ind_NULL = 0, OpenAPI_service_scenario_ind_VOICE_SERVICE, OpenAPI_service_scenario_ind_DATA_SERVICE, OpenAPI_service_scenario_ind_VOICE_AND_DATA_SERVICE, OpenAPI_service_scenario_ind_IOT, OpenAPI_service_scenario_ind_NPN } OpenAPI_service_scenario_ind_e;

char* OpenAPI_service_scenario_ind_ToString(OpenAPI_service_scenario_ind_e service_scenario_ind);

OpenAPI_service_scenario_ind_e OpenAPI_service_scenario_ind_FromString(char* service_scenario_ind);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_scenario_ind_H_ */

