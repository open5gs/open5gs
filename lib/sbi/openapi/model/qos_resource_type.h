/*
 * qos_resource_type.h
 *
 * 
 */

#ifndef _OpenAPI_qos_resource_type_H_
#define _OpenAPI_qos_resource_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_qos_resource_type_NULL = 0, OpenAPI_qos_resource_type_NON_GBR, OpenAPI_qos_resource_type_NON_CRITICAL_GBR, OpenAPI_qos_resource_type_CRITICAL_GBR } OpenAPI_qos_resource_type_e;

char* OpenAPI_qos_resource_type_ToString(OpenAPI_qos_resource_type_e qos_resource_type);

OpenAPI_qos_resource_type_e OpenAPI_qos_resource_type_FromString(char* qos_resource_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_resource_type_H_ */

