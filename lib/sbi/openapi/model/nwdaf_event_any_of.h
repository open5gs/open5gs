/*
 * nwdaf_event_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_nwdaf_event_any_of_H_
#define _OpenAPI_nwdaf_event_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nwdaf_event_any_of_NULL = 0, OpenAPI_nwdaf_event_any_of_SLICE_LOAD_LEVEL, OpenAPI_nwdaf_event_any_of_NETWORK_PERFORMANCE, OpenAPI_nwdaf_event_any_of_NF_LOAD, OpenAPI_nwdaf_event_any_of_SERVICE_EXPERIENCE, OpenAPI_nwdaf_event_any_of_UE_MOBILITY, OpenAPI_nwdaf_event_any_of_UE_COMMUNICATION, OpenAPI_nwdaf_event_any_of_QOS_SUSTAINABILITY, OpenAPI_nwdaf_event_any_of_ABNORMAL_BEHAVIOUR, OpenAPI_nwdaf_event_any_of_USER_DATA_CONGESTION, OpenAPI_nwdaf_event_any_of_NSI_LOAD_LEVEL, OpenAPI_nwdaf_event_any_of_DN_PERFORMANCE, OpenAPI_nwdaf_event_any_of_DISPERSION, OpenAPI_nwdaf_event_any_of_RED_TRANS_EXP, OpenAPI_nwdaf_event_any_of_WLAN_PERFORMANCE, OpenAPI_nwdaf_event_any_of_SM_CONGESTION } OpenAPI_nwdaf_event_any_of_e;

char* OpenAPI_nwdaf_event_any_of_ToString(OpenAPI_nwdaf_event_any_of_e nwdaf_event_any_of);

OpenAPI_nwdaf_event_any_of_e OpenAPI_nwdaf_event_any_of_FromString(char* nwdaf_event_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_event_any_of_H_ */

