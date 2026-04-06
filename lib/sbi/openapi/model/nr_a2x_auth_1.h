/*
 * nr_a2x_auth_1.h
 *
 * Contains NR A2X services authorized information.
 */

#ifndef _OpenAPI_nr_a2x_auth_1_H_
#define _OpenAPI_nr_a2x_auth_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nr_a2x_auth_1_s OpenAPI_nr_a2x_auth_1_t;
#include "ue_auth.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nr_a2x_auth_1_s {
    OpenAPI_ue_auth_e uav_ue_auth;
};

OpenAPI_nr_a2x_auth_1_t *OpenAPI_nr_a2x_auth_1_create(
    OpenAPI_ue_auth_e uav_ue_auth
);
void OpenAPI_nr_a2x_auth_1_free(OpenAPI_nr_a2x_auth_1_t *nr_a2x_auth_1);
OpenAPI_nr_a2x_auth_1_t *OpenAPI_nr_a2x_auth_1_parseFromJSON(cJSON *nr_a2x_auth_1JSON);
cJSON *OpenAPI_nr_a2x_auth_1_convertToJSON(OpenAPI_nr_a2x_auth_1_t *nr_a2x_auth_1);
OpenAPI_nr_a2x_auth_1_t *OpenAPI_nr_a2x_auth_1_copy(OpenAPI_nr_a2x_auth_1_t *dst, OpenAPI_nr_a2x_auth_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nr_a2x_auth_1_H_ */

