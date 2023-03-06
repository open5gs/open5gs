/*
 * event_type_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_event_type_any_of_H_
#define _OpenAPI_event_type_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_event_type_any_of_NULL = 0, OpenAPI_event_type_any_of_LOSS_OF_CONNECTIVITY, OpenAPI_event_type_any_of_UE_REACHABILITY_FOR_DATA, OpenAPI_event_type_any_of_UE_REACHABILITY_FOR_SMS, OpenAPI_event_type_any_of_LOCATION_REPORTING, OpenAPI_event_type_any_of_CHANGE_OF_SUPI_PEI_ASSOCIATION, OpenAPI_event_type_any_of_ROAMING_STATUS, OpenAPI_event_type_any_of_COMMUNICATION_FAILURE, OpenAPI_event_type_any_of_AVAILABILITY_AFTER_DDN_FAILURE, OpenAPI_event_type_any_of_CN_TYPE_CHANGE, OpenAPI_event_type_any_of_DL_DATA_DELIVERY_STATUS, OpenAPI_event_type_any_of_PDN_CONNECTIVITY_STATUS, OpenAPI_event_type_any_of_UE_CONNECTION_MANAGEMENT_STATE, OpenAPI_event_type_any_of_ACCESS_TYPE_REPORT, OpenAPI_event_type_any_of_REGISTRATION_STATE_REPORT, OpenAPI_event_type_any_of_CONNECTIVITY_STATE_REPORT, OpenAPI_event_type_any_of_TYPE_ALLOCATION_CODE_REPORT, OpenAPI_event_type_any_of_FREQUENT_MOBILITY_REGISTRATION_REPORT, OpenAPI_event_type_any_of_PDU_SES_REL, OpenAPI_event_type_any_of_PDU_SES_EST, OpenAPI_event_type_any_of_UE_MEMORY_AVAILABLE_FOR_SMS } OpenAPI_event_type_any_of_e;

char* OpenAPI_event_type_any_of_ToString(OpenAPI_event_type_any_of_e event_type_any_of);

OpenAPI_event_type_any_of_e OpenAPI_event_type_any_of_FromString(char* event_type_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_type_any_of_H_ */

