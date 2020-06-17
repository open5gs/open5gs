/*
 * lte_v2x_auth.h
 *
 *
 */

#ifndef _OpenAPI_lte_v2x_auth_H_
#define _OpenAPI_lte_v2x_auth_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_auth.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_lte_v2x_auth_s OpenAPI_lte_v2x_auth_t;
typedef struct OpenAPI_lte_v2x_auth_s {
    struct OpenAPI_ue_auth_s *vehicle_ue_auth;
    struct OpenAPI_ue_auth_s *pedestrian_ue_auth;
} OpenAPI_lte_v2x_auth_t;

OpenAPI_lte_v2x_auth_t *OpenAPI_lte_v2x_auth_create(
    OpenAPI_ue_auth_t *vehicle_ue_auth,
    OpenAPI_ue_auth_t *pedestrian_ue_auth
    );
void OpenAPI_lte_v2x_auth_free(OpenAPI_lte_v2x_auth_t *lte_v2x_auth);
OpenAPI_lte_v2x_auth_t *OpenAPI_lte_v2x_auth_parseFromJSON(cJSON *lte_v2x_authJSON);
cJSON *OpenAPI_lte_v2x_auth_convertToJSON(OpenAPI_lte_v2x_auth_t *lte_v2x_auth);
OpenAPI_lte_v2x_auth_t *OpenAPI_lte_v2x_auth_copy(OpenAPI_lte_v2x_auth_t *dst, OpenAPI_lte_v2x_auth_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lte_v2x_auth_H_ */

