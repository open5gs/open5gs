/*
 * ue_auth.h
 *
 *
 */

#ifndef _OpenAPI_ue_auth_H_
#define _OpenAPI_ue_auth_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_auth_s OpenAPI_ue_auth_t;
typedef struct OpenAPI_ue_auth_s {
} OpenAPI_ue_auth_t;

OpenAPI_ue_auth_t *OpenAPI_ue_auth_create(
    );
void OpenAPI_ue_auth_free(OpenAPI_ue_auth_t *ue_auth);
OpenAPI_ue_auth_t *OpenAPI_ue_auth_parseFromJSON(cJSON *ue_authJSON);
cJSON *OpenAPI_ue_auth_convertToJSON(OpenAPI_ue_auth_t *ue_auth);
OpenAPI_ue_auth_t *OpenAPI_ue_auth_copy(OpenAPI_ue_auth_t *dst, OpenAPI_ue_auth_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_auth_H_ */

