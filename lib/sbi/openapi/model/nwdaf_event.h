/*
 * nwdaf_event.h
 *
 * 
 */

#ifndef _OpenAPI_nwdaf_event_H_
#define _OpenAPI_nwdaf_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nwdaf_event_NULL = 0, OpenAPI_nwdaf_event_SLICE_LOAD_LEVEL, OpenAPI_nwdaf_event_NETWORK_PERFORMANCE, OpenAPI_nwdaf_event_NF_LOAD, OpenAPI_nwdaf_event_SERVICE_EXPERIENCE, OpenAPI_nwdaf_event_UE_MOBILITY, OpenAPI_nwdaf_event_UE_COMMUNICATION, OpenAPI_nwdaf_event_QOS_SUSTAINABILITY, OpenAPI_nwdaf_event_ABNORMAL_BEHAVIOUR, OpenAPI_nwdaf_event_USER_DATA_CONGESTION, OpenAPI_nwdaf_event_NSI_LOAD_LEVEL, OpenAPI_nwdaf_event_DN_PERFORMANCE, OpenAPI_nwdaf_event_DISPERSION, OpenAPI_nwdaf_event_RED_TRANS_EXP, OpenAPI_nwdaf_event_WLAN_PERFORMANCE, OpenAPI_nwdaf_event_SM_CONGESTION, OpenAPI_nwdaf_event_PFD_DETERMINATION, OpenAPI_nwdaf_event_PDU_SESSION_TRAFFIC, OpenAPI_nwdaf_event_E2E_DATA_VOL_TRANS_TIME, OpenAPI_nwdaf_event_MOVEMENT_BEHAVIOUR, OpenAPI_nwdaf_event_LOC_ACCURACY, OpenAPI_nwdaf_event_RELATIVE_PROXIMITY, OpenAPI_nwdaf_event_SIGNALLING_STORM, OpenAPI_nwdaf_event_QOS_POLICY_ASSIST } OpenAPI_nwdaf_event_e;

char* OpenAPI_nwdaf_event_ToString(OpenAPI_nwdaf_event_e nwdaf_event);

OpenAPI_nwdaf_event_e OpenAPI_nwdaf_event_FromString(char* nwdaf_event);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_event_H_ */

