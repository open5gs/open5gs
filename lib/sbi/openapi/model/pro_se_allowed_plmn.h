/*
 * pro_se_allowed_plmn.h
 *
 * Contains the PLMN identities where the Prose services are authorised to use and the authorised Prose services on this given PLMNs.
 */

#ifndef _OpenAPI_pro_se_allowed_plmn_H_
#define _OpenAPI_pro_se_allowed_plmn_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"
#include "prose_direct_allowed.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pro_se_allowed_plmn_s OpenAPI_pro_se_allowed_plmn_t;
typedef struct OpenAPI_pro_se_allowed_plmn_s {
    struct OpenAPI_plmn_id_s *visited_plmn;
    OpenAPI_list_t *prose_direct_allowed;
} OpenAPI_pro_se_allowed_plmn_t;

OpenAPI_pro_se_allowed_plmn_t *OpenAPI_pro_se_allowed_plmn_create(
    OpenAPI_plmn_id_t *visited_plmn,
    OpenAPI_list_t *prose_direct_allowed
);
void OpenAPI_pro_se_allowed_plmn_free(OpenAPI_pro_se_allowed_plmn_t *pro_se_allowed_plmn);
OpenAPI_pro_se_allowed_plmn_t *OpenAPI_pro_se_allowed_plmn_parseFromJSON(cJSON *pro_se_allowed_plmnJSON);
cJSON *OpenAPI_pro_se_allowed_plmn_convertToJSON(OpenAPI_pro_se_allowed_plmn_t *pro_se_allowed_plmn);
OpenAPI_pro_se_allowed_plmn_t *OpenAPI_pro_se_allowed_plmn_copy(OpenAPI_pro_se_allowed_plmn_t *dst, OpenAPI_pro_se_allowed_plmn_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pro_se_allowed_plmn_H_ */

