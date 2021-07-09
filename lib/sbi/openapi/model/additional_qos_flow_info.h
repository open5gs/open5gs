/*
 * additional_qos_flow_info.h
 *
 * 
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

