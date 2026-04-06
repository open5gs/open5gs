/*
 * qos_monitoring_param_type.h
 *
 * 
 */

#ifndef _OpenAPI_qos_monitoring_param_type_H_
#define _OpenAPI_qos_monitoring_param_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_qos_monitoring_param_type_NULL = 0, OpenAPI_qos_monitoring_param_type_PACKET_DELAY, OpenAPI_qos_monitoring_param_type_CONGESTION, OpenAPI_qos_monitoring_param_type_DATA_RATE, OpenAPI_qos_monitoring_param_type_AVAILABLE_BITRATE } OpenAPI_qos_monitoring_param_type_e;

char* OpenAPI_qos_monitoring_param_type_ToString(OpenAPI_qos_monitoring_param_type_e qos_monitoring_param_type);

OpenAPI_qos_monitoring_param_type_e OpenAPI_qos_monitoring_param_type_FromString(char* qos_monitoring_param_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_monitoring_param_type_H_ */

