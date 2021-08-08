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

typedef enum { OpenAPI_af_event_NULL = 0, OpenAPI_af_event_ACCESS_TYPE_CHANGE, OpenAPI_af_event_ANI_REPORT, OpenAPI_af_event_CHARGING_CORRELATION, OpenAPI_af_event_EPS_FALLBACK, OpenAPI_af_event_FAILED_RESOURCES_ALLOCATION, OpenAPI_af_event_OUT_OF_CREDIT, OpenAPI_af_event_PLMN_CHG, OpenAPI_af_event_QOS_MONITORING, OpenAPI_af_event_QOS_NOTIF, OpenAPI_af_event_RAN_NAS_CAUSE, OpenAPI_af_event_REALLOCATION_OF_CREDIT, OpenAPI_af_event_SUCCESSFUL_RESOURCES_ALLOCATION, OpenAPI_af_event_TSN_BRIDGE_INFO, OpenAPI_af_event_USAGE_REPORT } OpenAPI_af_event_e;

char* OpenAPI_af_event_ToString(OpenAPI_af_event_e af_event);

OpenAPI_af_event_e OpenAPI_af_event_FromString(char* af_event);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_event_H_ */

