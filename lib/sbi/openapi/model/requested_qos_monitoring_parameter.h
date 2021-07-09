/*
 * requested_qos_monitoring_parameter.h
 *
 * 
 */

#ifndef _OpenAPI_requested_qos_monitoring_parameter_H_
#define _OpenAPI_requested_qos_monitoring_parameter_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_requested_qos_monitoring_parameter_NULL = 0, OpenAPI_requested_qos_monitoring_parameter_DOWNLINK, OpenAPI_requested_qos_monitoring_parameter_UPLINK, OpenAPI_requested_qos_monitoring_parameter_ROUND_TRIP } OpenAPI_requested_qos_monitoring_parameter_e;

char* OpenAPI_requested_qos_monitoring_parameter_ToString(OpenAPI_requested_qos_monitoring_parameter_e requested_qos_monitoring_parameter);

OpenAPI_requested_qos_monitoring_parameter_e OpenAPI_requested_qos_monitoring_parameter_FromString(char* requested_qos_monitoring_parameter);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_requested_qos_monitoring_parameter_H_ */

