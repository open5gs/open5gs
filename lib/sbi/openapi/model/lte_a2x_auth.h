/*
 * lte_a2x_auth.h
 *
 * Contains LTE A2X services authorized information.
 */

#ifndef _OpenAPI_lte_a2x_auth_H_
#define _OpenAPI_lte_a2x_auth_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_lte_a2x_auth_s OpenAPI_lte_a2x_auth_t;
#include "ue_auth.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_lte_a2x_auth_s {
    OpenAPI_ue_auth_e uav_ue_auth;
};

OpenAPI_lte_a2x_auth_t *OpenAPI_lte_a2x_auth_create(
    OpenAPI_ue_auth_e uav_ue_auth
);
void OpenAPI_lte_a2x_auth_free(OpenAPI_lte_a2x_auth_t *lte_a2x_auth);
OpenAPI_lte_a2x_auth_t *OpenAPI_lte_a2x_auth_parseFromJSON(cJSON *lte_a2x_authJSON);
cJSON *OpenAPI_lte_a2x_auth_convertToJSON(OpenAPI_lte_a2x_auth_t *lte_a2x_auth);
OpenAPI_lte_a2x_auth_t *OpenAPI_lte_a2x_auth_copy(OpenAPI_lte_a2x_auth_t *dst, OpenAPI_lte_a2x_auth_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lte_a2x_auth_H_ */

