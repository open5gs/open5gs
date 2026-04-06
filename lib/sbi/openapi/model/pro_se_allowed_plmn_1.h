/*
 * pro_se_allowed_plmn_1.h
 *
 * Contains the PLMN identities where the Prose services are authorised to use and the authorised Prose services on this given PLMNs. 
 */

#ifndef _OpenAPI_pro_se_allowed_plmn_1_H_
#define _OpenAPI_pro_se_allowed_plmn_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pro_se_allowed_plmn_1_s OpenAPI_pro_se_allowed_plmn_1_t;
#include "plmn_id.h"
#include "prose_direct_allowed.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pro_se_allowed_plmn_1_s {
    struct OpenAPI_plmn_id_s *visited_plmn;
    OpenAPI_list_t *prose_direct_allowed;
};

OpenAPI_pro_se_allowed_plmn_1_t *OpenAPI_pro_se_allowed_plmn_1_create(
    OpenAPI_plmn_id_t *visited_plmn,
    OpenAPI_list_t *prose_direct_allowed
);
void OpenAPI_pro_se_allowed_plmn_1_free(OpenAPI_pro_se_allowed_plmn_1_t *pro_se_allowed_plmn_1);
OpenAPI_pro_se_allowed_plmn_1_t *OpenAPI_pro_se_allowed_plmn_1_parseFromJSON(cJSON *pro_se_allowed_plmn_1JSON);
cJSON *OpenAPI_pro_se_allowed_plmn_1_convertToJSON(OpenAPI_pro_se_allowed_plmn_1_t *pro_se_allowed_plmn_1);
OpenAPI_pro_se_allowed_plmn_1_t *OpenAPI_pro_se_allowed_plmn_1_copy(OpenAPI_pro_se_allowed_plmn_1_t *dst, OpenAPI_pro_se_allowed_plmn_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pro_se_allowed_plmn_1_H_ */

