/*
 * event_id.h
 *
 * 
 */

#ifndef _OpenAPI_event_id_H_
#define _OpenAPI_event_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_event_id_NULL = 0, OpenAPI_event_id_LOAD_LEVEL_INFORMATION, OpenAPI_event_id_NETWORK_PERFORMANCE, OpenAPI_event_id_NF_LOAD, OpenAPI_event_id_SERVICE_EXPERIENCE, OpenAPI_event_id_UE_MOBILITY, OpenAPI_event_id_UE_COMMUNICATION, OpenAPI_event_id_QOS_SUSTAINABILITY, OpenAPI_event_id_ABNORMAL_BEHAVIOUR, OpenAPI_event_id_USER_DATA_CONGESTION, OpenAPI_event_id_NSI_LOAD_LEVEL, OpenAPI_event_id_SM_CONGESTION, OpenAPI_event_id_DISPERSION, OpenAPI_event_id_RED_TRANS_EXP, OpenAPI_event_id_WLAN_PERFORMANCE, OpenAPI_event_id_DN_PERFORMANCE, OpenAPI_event_id_PDU_SESSION_TRAFFIC, OpenAPI_event_id_E2E_DATA_VOL_TRANS_TIME, OpenAPI_event_id_MOVEMENT_BEHAVIOUR, OpenAPI_event_id_LOC_ACCURACY, OpenAPI_event_id_RELATIVE_PROXIMITY, OpenAPI_event_id_SIGNALLING_STORM, OpenAPI_event_id_QOS_POLICY_ASSIST } OpenAPI_event_id_e;

char* OpenAPI_event_id_ToString(OpenAPI_event_id_e event_id);

OpenAPI_event_id_e OpenAPI_event_id_FromString(char* event_id);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_id_H_ */

