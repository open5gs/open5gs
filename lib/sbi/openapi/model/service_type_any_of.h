/*
 * service_type_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_service_type_any_of_H_
#define _OpenAPI_service_type_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_service_type_any_of_NULL = 0, OpenAPI_service_type_any_of_AF_GUIDANCE_FOR_URSP } OpenAPI_service_type_any_of_e;

char* OpenAPI_service_type_any_of_ToString(OpenAPI_service_type_any_of_e service_type_any_of);

OpenAPI_service_type_any_of_e OpenAPI_service_type_any_of_FromString(char* service_type_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_type_any_of_H_ */

