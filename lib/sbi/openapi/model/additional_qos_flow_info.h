/*
 * additional_qos_flow_info.h
 *
 * The enumeration AdditionalQosFlowInfo provides additional QoS flow information (see clause  9.3.1.12 3GPP TS 38.413 [11]). It shall comply with the provisions defined in table 5.5.3.12-1. 
 */

#ifndef _OpenAPI_additional_qos_flow_info_H_
#define _OpenAPI_additional_qos_flow_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_additional_qos_flow_info_NULL = 0, OpenAPI_additional_qos_flow_info_MORE_LIKELY } OpenAPI_additional_qos_flow_info_e;

char* OpenAPI_additional_qos_flow_info_ToString(OpenAPI_additional_qos_flow_info_e additional_qos_flow_info);

OpenAPI_additional_qos_flow_info_e OpenAPI_additional_qos_flow_info_FromString(char* additional_qos_flow_info);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_additional_qos_flow_info_H_ */

