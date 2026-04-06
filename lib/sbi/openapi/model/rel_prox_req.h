/*
 * rel_prox_req.h
 *
 * Represents the Relative Proximity analytics requirements.
 */

#ifndef _OpenAPI_rel_prox_req_H_
#define _OpenAPI_rel_prox_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_rel_prox_req_s OpenAPI_rel_prox_req_t;
#include "direction.h"
#include "proximity_criterion.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_rel_prox_req_s {
    OpenAPI_list_t *direction;
    bool is_num_of_ue;
    int num_of_ue;
    OpenAPI_list_t *proximity_crits;
};

OpenAPI_rel_prox_req_t *OpenAPI_rel_prox_req_create(
    OpenAPI_list_t *direction,
    bool is_num_of_ue,
    int num_of_ue,
    OpenAPI_list_t *proximity_crits
);
void OpenAPI_rel_prox_req_free(OpenAPI_rel_prox_req_t *rel_prox_req);
OpenAPI_rel_prox_req_t *OpenAPI_rel_prox_req_parseFromJSON(cJSON *rel_prox_reqJSON);
cJSON *OpenAPI_rel_prox_req_convertToJSON(OpenAPI_rel_prox_req_t *rel_prox_req);
OpenAPI_rel_prox_req_t *OpenAPI_rel_prox_req_copy(OpenAPI_rel_prox_req_t *dst, OpenAPI_rel_prox_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rel_prox_req_H_ */

