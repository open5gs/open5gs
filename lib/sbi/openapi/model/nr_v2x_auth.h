/*
 * nr_v2x_auth.h
 *
 * Contains NR V2X services authorized information.
 */

#ifndef _OpenAPI_nr_v2x_auth_H_
#define _OpenAPI_nr_v2x_auth_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_auth.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nr_v2x_auth_s OpenAPI_nr_v2x_auth_t;
typedef struct OpenAPI_nr_v2x_auth_s {
    OpenAPI_ue_auth_e vehicle_ue_auth;
    OpenAPI_ue_auth_e pedestrian_ue_auth;
} OpenAPI_nr_v2x_auth_t;

OpenAPI_nr_v2x_auth_t *OpenAPI_nr_v2x_auth_create(
    OpenAPI_ue_auth_e vehicle_ue_auth,
    OpenAPI_ue_auth_e pedestrian_ue_auth
);
void OpenAPI_nr_v2x_auth_free(OpenAPI_nr_v2x_auth_t *nr_v2x_auth);
OpenAPI_nr_v2x_auth_t *OpenAPI_nr_v2x_auth_parseFromJSON(cJSON *nr_v2x_authJSON);
cJSON *OpenAPI_nr_v2x_auth_convertToJSON(OpenAPI_nr_v2x_auth_t *nr_v2x_auth);
OpenAPI_nr_v2x_auth_t *OpenAPI_nr_v2x_auth_copy(OpenAPI_nr_v2x_auth_t *dst, OpenAPI_nr_v2x_auth_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nr_v2x_auth_H_ */

