/*
 * flow_status.h
 *
 * 
 */

#ifndef _OpenAPI_flow_status_H_
#define _OpenAPI_flow_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_flow_status_NULL = 0, OpenAPI_flow_status_ENABLED_UPLINK, OpenAPI_flow_status_ENABLED_DOWNLINK, OpenAPI_flow_status_ENABLED, OpenAPI_flow_status_DISABLED, OpenAPI_flow_status_REMOVED } OpenAPI_flow_status_e;

char* OpenAPI_flow_status_ToString(OpenAPI_flow_status_e flow_status);

OpenAPI_flow_status_e OpenAPI_flow_status_FromString(char* flow_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_flow_status_H_ */

