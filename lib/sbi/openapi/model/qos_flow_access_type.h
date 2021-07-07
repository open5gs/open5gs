/*
 * qos_flow_access_type.h
 *
 * 
 */

#ifndef _OpenAPI_qos_flow_access_type_H_
#define _OpenAPI_qos_flow_access_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_qos_flow_access_type_NULL = 0, OpenAPI_qos_flow_access_type__3GPP, OpenAPI_qos_flow_access_type_NON_3GPP, OpenAPI_qos_flow_access_type__3GPP_AND_NON_3GPP } OpenAPI_qos_flow_access_type_e;

char* OpenAPI_qos_flow_access_type_ToString(OpenAPI_qos_flow_access_type_e qos_flow_access_type);

OpenAPI_qos_flow_access_type_e OpenAPI_qos_flow_access_type_FromString(char* qos_flow_access_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_flow_access_type_H_ */

