/*
 * service_experience_type.h
 *
 * Possible values are: - VOICE: Indicates that the service experience analytics is for voice service. - VIDEO: Indicates that the service experience analytics is for video service. - OTHER: Indicates that the service experience analytics is for other service. 
 */

#ifndef _OpenAPI_service_experience_type_H_
#define _OpenAPI_service_experience_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "service_experience_type_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_experience_type_s OpenAPI_service_experience_type_t;
typedef struct OpenAPI_service_experience_type_s {
} OpenAPI_service_experience_type_t;

OpenAPI_service_experience_type_t *OpenAPI_service_experience_type_create(
);
void OpenAPI_service_experience_type_free(OpenAPI_service_experience_type_t *service_experience_type);
OpenAPI_service_experience_type_t *OpenAPI_service_experience_type_parseFromJSON(cJSON *service_experience_typeJSON);
cJSON *OpenAPI_service_experience_type_convertToJSON(OpenAPI_service_experience_type_t *service_experience_type);
OpenAPI_service_experience_type_t *OpenAPI_service_experience_type_copy(OpenAPI_service_experience_type_t *dst, OpenAPI_service_experience_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_experience_type_H_ */

