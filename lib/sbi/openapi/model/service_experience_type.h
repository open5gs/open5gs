/*
 * service_experience_type.h
 *
 * 
 */

#ifndef _OpenAPI_service_experience_type_H_
#define _OpenAPI_service_experience_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_service_experience_type_NULL = 0, OpenAPI_service_experience_type_VOICE, OpenAPI_service_experience_type_VIDEO, OpenAPI_service_experience_type_OTHER } OpenAPI_service_experience_type_e;

char* OpenAPI_service_experience_type_ToString(OpenAPI_service_experience_type_e service_experience_type);

OpenAPI_service_experience_type_e OpenAPI_service_experience_type_FromString(char* service_experience_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_experience_type_H_ */

