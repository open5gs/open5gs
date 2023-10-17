/*
 * gba_authentication_info_request.h
 *
 * 
 */

#ifndef _OpenAPI_gba_authentication_info_request_H_
#define _OpenAPI_gba_authentication_info_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "gba_auth_type.h"
#include "resynchronization_info_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_gba_authentication_info_request_s OpenAPI_gba_authentication_info_request_t;
typedef struct OpenAPI_gba_authentication_info_request_s {
    OpenAPI_gba_auth_type_e auth_type;
    struct OpenAPI_resynchronization_info_1_s *resynchronization_info;
    char *supported_features;
} OpenAPI_gba_authentication_info_request_t;

OpenAPI_gba_authentication_info_request_t *OpenAPI_gba_authentication_info_request_create(
    OpenAPI_gba_auth_type_e auth_type,
    OpenAPI_resynchronization_info_1_t *resynchronization_info,
    char *supported_features
);
void OpenAPI_gba_authentication_info_request_free(OpenAPI_gba_authentication_info_request_t *gba_authentication_info_request);
OpenAPI_gba_authentication_info_request_t *OpenAPI_gba_authentication_info_request_parseFromJSON(cJSON *gba_authentication_info_requestJSON);
cJSON *OpenAPI_gba_authentication_info_request_convertToJSON(OpenAPI_gba_authentication_info_request_t *gba_authentication_info_request);
OpenAPI_gba_authentication_info_request_t *OpenAPI_gba_authentication_info_request_copy(OpenAPI_gba_authentication_info_request_t *dst, OpenAPI_gba_authentication_info_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gba_authentication_info_request_H_ */

