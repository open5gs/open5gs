/*
 * lte_v2x_auth_1.h
 *
 * 
 */

#ifndef _OpenAPI_lte_v2x_auth_1_H_
#define _OpenAPI_lte_v2x_auth_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_auth.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_lte_v2x_auth_1_s OpenAPI_lte_v2x_auth_1_t;
typedef struct OpenAPI_lte_v2x_auth_1_s {
    OpenAPI_ue_auth_e vehicle_ue_auth;
    OpenAPI_ue_auth_e pedestrian_ue_auth;
} OpenAPI_lte_v2x_auth_1_t;

OpenAPI_lte_v2x_auth_1_t *OpenAPI_lte_v2x_auth_1_create(
    OpenAPI_ue_auth_e vehicle_ue_auth,
    OpenAPI_ue_auth_e pedestrian_ue_auth
);
void OpenAPI_lte_v2x_auth_1_free(OpenAPI_lte_v2x_auth_1_t *lte_v2x_auth_1);
OpenAPI_lte_v2x_auth_1_t *OpenAPI_lte_v2x_auth_1_parseFromJSON(cJSON *lte_v2x_auth_1JSON);
cJSON *OpenAPI_lte_v2x_auth_1_convertToJSON(OpenAPI_lte_v2x_auth_1_t *lte_v2x_auth_1);
OpenAPI_lte_v2x_auth_1_t *OpenAPI_lte_v2x_auth_1_copy(OpenAPI_lte_v2x_auth_1_t *dst, OpenAPI_lte_v2x_auth_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lte_v2x_auth_1_H_ */

