/*
 * amf_event_type.h
 *
 * Describes the supported event types of Namf_EventExposure Service
 */

#ifndef _OpenAPI_amf_event_type_H_
#define _OpenAPI_amf_event_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_amf_event_type_NULL = 0, OpenAPI_amf_event_type_LOCATION_REPORT, OpenAPI_amf_event_type_PRESENCE_IN_AOI_REPORT, OpenAPI_amf_event_type_TIMEZONE_REPORT, OpenAPI_amf_event_type_ACCESS_TYPE_REPORT, OpenAPI_amf_event_type_REGISTRATION_STATE_REPORT, OpenAPI_amf_event_type_CONNECTIVITY_STATE_REPORT, OpenAPI_amf_event_type_REACHABILITY_REPORT, OpenAPI_amf_event_type_COMMUNICATION_FAILURE_REPORT, OpenAPI_amf_event_type_UES_IN_AREA_REPORT, OpenAPI_amf_event_type_SUBSCRIPTION_ID_CHANGE, OpenAPI_amf_event_type_SUBSCRIPTION_ID_ADDITION, OpenAPI_amf_event_type_SUBSCRIPTION_TERMINATION, OpenAPI_amf_event_type_LOSS_OF_CONNECTIVITY, OpenAPI_amf_event_type__5GS_USER_STATE_REPORT, OpenAPI_amf_event_type_AVAILABILITY_AFTER_DDN_FAILURE, OpenAPI_amf_event_type_TYPE_ALLOCATION_CODE_REPORT, OpenAPI_amf_event_type_FREQUENT_MOBILITY_REGISTRATION_REPORT, OpenAPI_amf_event_type_SNSSAI_TA_MAPPING_REPORT, OpenAPI_amf_event_type_UE_LOCATION_TRENDS, OpenAPI_amf_event_type_UE_ACCESS_BEHAVIOR_TRENDS, OpenAPI_amf_event_type_UE_MM_TRANSACTION_REPORT, OpenAPI_amf_event_type_TRAJECTORY_TRACKING_REPORT, OpenAPI_amf_event_type_SIGNALLING_MEASUREMENT_REPORT, OpenAPI_amf_event_type_SIGNALLING_MEASUREMENT_UE_RELATED_REPORT, OpenAPI_amf_event_type_ALTITUDE_REPORT } OpenAPI_amf_event_type_e;

char* OpenAPI_amf_event_type_ToString(OpenAPI_amf_event_type_e amf_event_type);

OpenAPI_amf_event_type_e OpenAPI_amf_event_type_FromString(char* amf_event_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_event_type_H_ */

