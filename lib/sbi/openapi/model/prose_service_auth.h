/*
 * prose_service_auth.h
 *
 * Indicates whether the UE is authorized to use ProSe Direct Discovery, ProSe Direct Communication, or both. 
 */

#ifndef _OpenAPI_prose_service_auth_H_
#define _OpenAPI_prose_service_auth_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_auth.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_prose_service_auth_s OpenAPI_prose_service_auth_t;
typedef struct OpenAPI_prose_service_auth_s {
    OpenAPI_ue_auth_e prose_direct_discovery_auth;
    OpenAPI_ue_auth_e prose_direct_communication_auth;
} OpenAPI_prose_service_auth_t;

OpenAPI_prose_service_auth_t *OpenAPI_prose_service_auth_create(
    OpenAPI_ue_auth_e prose_direct_discovery_auth,
    OpenAPI_ue_auth_e prose_direct_communication_auth
);
void OpenAPI_prose_service_auth_free(OpenAPI_prose_service_auth_t *prose_service_auth);
OpenAPI_prose_service_auth_t *OpenAPI_prose_service_auth_parseFromJSON(cJSON *prose_service_authJSON);
cJSON *OpenAPI_prose_service_auth_convertToJSON(OpenAPI_prose_service_auth_t *prose_service_auth);
OpenAPI_prose_service_auth_t *OpenAPI_prose_service_auth_copy(OpenAPI_prose_service_auth_t *dst, OpenAPI_prose_service_auth_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_prose_service_auth_H_ */

