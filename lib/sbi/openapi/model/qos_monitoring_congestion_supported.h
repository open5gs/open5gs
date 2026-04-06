/*
 * qos_monitoring_congestion_supported.h
 *
 * 
 */

#ifndef _OpenAPI_qos_monitoring_congestion_supported_H_
#define _OpenAPI_qos_monitoring_congestion_supported_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_qos_monitoring_congestion_supported_NULL = 0, OpenAPI_qos_monitoring_congestion_supported_SUPPORTED, OpenAPI_qos_monitoring_congestion_supported_NOT_SUPPORTED, OpenAPI_qos_monitoring_congestion_supported_UNKNOWN } OpenAPI_qos_monitoring_congestion_supported_e;

char* OpenAPI_qos_monitoring_congestion_supported_ToString(OpenAPI_qos_monitoring_congestion_supported_e qos_monitoring_congestion_supported);

OpenAPI_qos_monitoring_congestion_supported_e OpenAPI_qos_monitoring_congestion_supported_FromString(char* qos_monitoring_congestion_supported);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_monitoring_congestion_supported_H_ */

