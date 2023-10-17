/*
 * service_type.h
 *
 * Possible values are - AF_GUIDANCE_FOR_URSP 
 */

#ifndef _OpenAPI_service_type_H_
#define _OpenAPI_service_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "service_type_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_type_s OpenAPI_service_type_t;
typedef struct OpenAPI_service_type_s {
} OpenAPI_service_type_t;

OpenAPI_service_type_t *OpenAPI_service_type_create(
);
void OpenAPI_service_type_free(OpenAPI_service_type_t *service_type);
OpenAPI_service_type_t *OpenAPI_service_type_parseFromJSON(cJSON *service_typeJSON);
cJSON *OpenAPI_service_type_convertToJSON(OpenAPI_service_type_t *service_type);
OpenAPI_service_type_t *OpenAPI_service_type_copy(OpenAPI_service_type_t *dst, OpenAPI_service_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_type_H_ */

