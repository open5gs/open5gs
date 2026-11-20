/*
 * nsm_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_nsm_charging_information_H_
#define _OpenAPI_nsm_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nsm_charging_information_s OpenAPI_nsm_charging_information_t;
#include "administrative_state.h"
#include "management_operation.h"
#include "management_operation_status.h"
#include "operational_state.h"
#include "service_profile_charging_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nsm_charging_information_s {
    OpenAPI_management_operation_e management_operation;
    char *id_network_slice_instance;
    OpenAPI_list_t *list_ofservice_profile_charging_information;
    OpenAPI_management_operation_status_e management_operation_status;
    OpenAPI_operational_state_e management_operational_state;
    OpenAPI_administrative_state_e management_administrative_state;
};

OpenAPI_nsm_charging_information_t *OpenAPI_nsm_charging_information_create(
    OpenAPI_management_operation_e management_operation,
    char *id_network_slice_instance,
    OpenAPI_list_t *list_ofservice_profile_charging_information,
    OpenAPI_management_operation_status_e management_operation_status,
    OpenAPI_operational_state_e management_operational_state,
    OpenAPI_administrative_state_e management_administrative_state
);
void OpenAPI_nsm_charging_information_free(OpenAPI_nsm_charging_information_t *nsm_charging_information);
OpenAPI_nsm_charging_information_t *OpenAPI_nsm_charging_information_parseFromJSON(cJSON *nsm_charging_informationJSON);
cJSON *OpenAPI_nsm_charging_information_convertToJSON(OpenAPI_nsm_charging_information_t *nsm_charging_information);
OpenAPI_nsm_charging_information_t *OpenAPI_nsm_charging_information_copy(OpenAPI_nsm_charging_information_t *dst, OpenAPI_nsm_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nsm_charging_information_H_ */

