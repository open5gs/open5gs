/*
 * qos_monitoring_req.h
 *
 * 
 */

#ifndef _OpenAPI_qos_monitoring_req_H_
#define _OpenAPI_qos_monitoring_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_qos_monitoring_req_NULL = 0, OpenAPI_qos_monitoring_req_UL, OpenAPI_qos_monitoring_req_DL, OpenAPI_qos_monitoring_req_BOTH, OpenAPI_qos_monitoring_req_NONE } OpenAPI_qos_monitoring_req_e;

char* OpenAPI_qos_monitoring_req_ToString(OpenAPI_qos_monitoring_req_e qos_monitoring_req);

OpenAPI_qos_monitoring_req_e OpenAPI_qos_monitoring_req_FromString(char* qos_monitoring_req);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_monitoring_req_H_ */

