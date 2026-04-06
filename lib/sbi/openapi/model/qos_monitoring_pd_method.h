/*
 * qos_monitoring_pd_method.h
 *
 * 
 */

#ifndef _OpenAPI_qos_monitoring_pd_method_H_
#define _OpenAPI_qos_monitoring_pd_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_qos_monitoring_pd_method_NULL = 0, OpenAPI_qos_monitoring_pd_method_USING_TIMESTAMP, OpenAPI_qos_monitoring_pd_method_USING_GTP_U_PATH } OpenAPI_qos_monitoring_pd_method_e;

char* OpenAPI_qos_monitoring_pd_method_ToString(OpenAPI_qos_monitoring_pd_method_e qos_monitoring_pd_method);

OpenAPI_qos_monitoring_pd_method_e OpenAPI_qos_monitoring_pd_method_FromString(char* qos_monitoring_pd_method);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_monitoring_pd_method_H_ */

