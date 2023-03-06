/*
 * af_event.h
 *
 * 
 */

#ifndef _OpenAPI_af_event_H_
#define _OpenAPI_af_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_af_event_NULL = 0, OpenAPI_af_event_SVC_EXPERIENCE, OpenAPI_af_event_UE_MOBILITY, OpenAPI_af_event_UE_COMM, OpenAPI_af_event_EXCEPTIONS, OpenAPI_af_event_USER_DATA_CONGESTION, OpenAPI_af_event_PERF_DATA, OpenAPI_af_event_DISPERSION, OpenAPI_af_event_COLLECTIVE_BEHAVIOUR, OpenAPI_af_event_MS_QOE_METRICS, OpenAPI_af_event_MS_CONSUMPTION, OpenAPI_af_event_MS_NET_ASSIST_INVOCATION, OpenAPI_af_event_MS_DYN_POLICY_INVOCATION, OpenAPI_af_event_MS_ACCESS_ACTIVITY } OpenAPI_af_event_e;

char* OpenAPI_af_event_ToString(OpenAPI_af_event_e af_event);

OpenAPI_af_event_e OpenAPI_af_event_FromString(char* af_event);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_event_H_ */

