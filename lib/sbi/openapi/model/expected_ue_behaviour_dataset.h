/*
 * expected_ue_behaviour_dataset.h
 *
 * Indicates the name of the Expected UE Behaviour Dataset 
 */

#ifndef _OpenAPI_expected_ue_behaviour_dataset_H_
#define _OpenAPI_expected_ue_behaviour_dataset_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_expected_ue_behaviour_dataset_NULL = 0, OpenAPI_expected_ue_behaviour_dataset_STATIONARY_INDICATION, OpenAPI_expected_ue_behaviour_dataset_COMMUNICATION_DURATION_TIME, OpenAPI_expected_ue_behaviour_dataset_PERIODIC_TIME, OpenAPI_expected_ue_behaviour_dataset_SCHEDULED_COMMUNICATION_TIME, OpenAPI_expected_ue_behaviour_dataset_SCHEDULED_COMMUNICATION_TYPE, OpenAPI_expected_ue_behaviour_dataset_EXPECTED_UMTS, OpenAPI_expected_ue_behaviour_dataset_TRAFFIC_PROFILE, OpenAPI_expected_ue_behaviour_dataset_BATTERY_INDICATION, OpenAPI_expected_ue_behaviour_dataset_EXPECTED_INACTIVITY_TIME } OpenAPI_expected_ue_behaviour_dataset_e;

char* OpenAPI_expected_ue_behaviour_dataset_ToString(OpenAPI_expected_ue_behaviour_dataset_e expected_ue_behaviour_dataset);

OpenAPI_expected_ue_behaviour_dataset_e OpenAPI_expected_ue_behaviour_dataset_FromString(char* expected_ue_behaviour_dataset);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_expected_ue_behaviour_dataset_H_ */

