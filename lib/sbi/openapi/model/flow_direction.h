/*
 * flow_direction.h
 *
 * 
 */

#ifndef _OpenAPI_flow_direction_H_
#define _OpenAPI_flow_direction_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_flow_direction_NULL = 0, OpenAPI_flow_direction_DOWNLINK, OpenAPI_flow_direction_UPLINK, OpenAPI_flow_direction_BIDIRECTIONAL, OpenAPI_flow_direction_UNSPECIFIED } OpenAPI_flow_direction_e;

char* OpenAPI_flow_direction_ToString(OpenAPI_flow_direction_e flow_direction);

OpenAPI_flow_direction_e OpenAPI_flow_direction_FromString(char* flow_direction);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_flow_direction_H_ */

