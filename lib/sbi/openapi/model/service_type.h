/*
 * service_type.h
 *
 * 
 */

#ifndef _OpenAPI_service_type_H_
#define _OpenAPI_service_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_service_type_NULL = 0, OpenAPI_service_type_AF_GUIDANCE_FOR_URSP, OpenAPI_service_type_AF_REQUESTED_QOS, OpenAPI_service_type_AF_PROVISION_N3GPP_DEV_ID_INFO } OpenAPI_service_type_e;

char* OpenAPI_service_type_ToString(OpenAPI_service_type_e service_type);

OpenAPI_service_type_e OpenAPI_service_type_FromString(char* service_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_type_H_ */

