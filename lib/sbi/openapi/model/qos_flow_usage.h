/*
 * qos_flow_usage.h
 *
 * 
 */

#ifndef _OpenAPI_qos_flow_usage_H_
#define _OpenAPI_qos_flow_usage_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_qos_flow_usage_NULL = 0, OpenAPI_qos_flow_usage_GENERAL, OpenAPI_qos_flow_usage_IMS_SIG } OpenAPI_qos_flow_usage_e;

char* OpenAPI_qos_flow_usage_ToString(OpenAPI_qos_flow_usage_e qos_flow_usage);

OpenAPI_qos_flow_usage_e OpenAPI_qos_flow_usage_FromString(char* qos_flow_usage);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_flow_usage_H_ */

