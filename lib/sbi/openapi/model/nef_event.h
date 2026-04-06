/*
 * nef_event.h
 *
 * 
 */

#ifndef _OpenAPI_nef_event_H_
#define _OpenAPI_nef_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nef_event_NULL = 0, OpenAPI_nef_event_SVC_EXPERIENCE, OpenAPI_nef_event_UE_MOBILITY, OpenAPI_nef_event_UE_COMM, OpenAPI_nef_event_EXCEPTIONS, OpenAPI_nef_event_USER_DATA_CONGESTION, OpenAPI_nef_event_PERF_DATA, OpenAPI_nef_event_DISPERSION, OpenAPI_nef_event_COLLECTIVE_BEHAVIOUR, OpenAPI_nef_event_MS_QOE_METRICS, OpenAPI_nef_event_MS_CONSUMPTION, OpenAPI_nef_event_MS_NET_ASSIST_INVOCATION, OpenAPI_nef_event_MS_DYN_POLICY_INVOCATION, OpenAPI_nef_event_MS_ACCESS_ACTIVITY, OpenAPI_nef_event_GNSS_ASSISTANCE_DATA, OpenAPI_nef_event_DATA_VOLUME_TRANSFER_TIME, OpenAPI_nef_event_APP_ACTIVE_TIME, OpenAPI_nef_event_SIGNALLING_INFO, OpenAPI_nef_event_UE_ALTITUDE } OpenAPI_nef_event_e;

char* OpenAPI_nef_event_ToString(OpenAPI_nef_event_e nef_event);

OpenAPI_nef_event_e OpenAPI_nef_event_FromString(char* nef_event);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nef_event_H_ */

