/*
 * qoe_service_type.h
 *
 * The enumeration QoeServiceType indicates the kind of service that shall be recorded for QMC. It shall comply with the provisions defined in TS 29.571, table 5.6.3.21-1. 
 */

#ifndef _OpenAPI_qoe_service_type_H_
#define _OpenAPI_qoe_service_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_qoe_service_type_NULL = 0, OpenAPI_qoe_service_type_DASH, OpenAPI_qoe_service_type_MTSI, OpenAPI_qoe_service_type_VR } OpenAPI_qoe_service_type_e;

char* OpenAPI_qoe_service_type_ToString(OpenAPI_qoe_service_type_e qoe_service_type);

OpenAPI_qoe_service_type_e OpenAPI_qoe_service_type_FromString(char* qoe_service_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qoe_service_type_H_ */

