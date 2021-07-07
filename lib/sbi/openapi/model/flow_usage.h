/*
 * flow_usage.h
 *
 * 
 */

#ifndef _OpenAPI_flow_usage_H_
#define _OpenAPI_flow_usage_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_flow_usage_NULL = 0, OpenAPI_flow_usage_NO_INFO, OpenAPI_flow_usage_RTCP, OpenAPI_flow_usage_AF_SIGNALLING } OpenAPI_flow_usage_e;

char* OpenAPI_flow_usage_ToString(OpenAPI_flow_usage_e flow_usage);

OpenAPI_flow_usage_e OpenAPI_flow_usage_FromString(char* flow_usage);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_flow_usage_H_ */

