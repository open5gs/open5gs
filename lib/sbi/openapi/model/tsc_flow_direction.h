/*
 * tsc_flow_direction.h
 *
 * 
 */

#ifndef _OpenAPI_tsc_flow_direction_H_
#define _OpenAPI_tsc_flow_direction_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_tsc_flow_direction_NULL = 0, OpenAPI_tsc_flow_direction_UPLINK, OpenAPI_tsc_flow_direction_DOWNLINK } OpenAPI_tsc_flow_direction_e;

char* OpenAPI_tsc_flow_direction_ToString(OpenAPI_tsc_flow_direction_e tsc_flow_direction);

OpenAPI_tsc_flow_direction_e OpenAPI_tsc_flow_direction_FromString(char* tsc_flow_direction);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tsc_flow_direction_H_ */

