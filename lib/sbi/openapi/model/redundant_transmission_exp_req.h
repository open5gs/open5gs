/*
 * redundant_transmission_exp_req.h
 *
 * Represents other redundant transmission experience analytics requirements.
 */

#ifndef _OpenAPI_redundant_transmission_exp_req_H_
#define _OpenAPI_redundant_transmission_exp_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_redundant_transmission_exp_req_s OpenAPI_redundant_transmission_exp_req_t;
#include "matching_direction.h"
#include "red_trans_exp_ordering_criterion.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_redundant_transmission_exp_req_s {
    OpenAPI_red_trans_exp_ordering_criterion_e red_t_order_criter;
    OpenAPI_matching_direction_e order;
};

OpenAPI_redundant_transmission_exp_req_t *OpenAPI_redundant_transmission_exp_req_create(
    OpenAPI_red_trans_exp_ordering_criterion_e red_t_order_criter,
    OpenAPI_matching_direction_e order
);
void OpenAPI_redundant_transmission_exp_req_free(OpenAPI_redundant_transmission_exp_req_t *redundant_transmission_exp_req);
OpenAPI_redundant_transmission_exp_req_t *OpenAPI_redundant_transmission_exp_req_parseFromJSON(cJSON *redundant_transmission_exp_reqJSON);
cJSON *OpenAPI_redundant_transmission_exp_req_convertToJSON(OpenAPI_redundant_transmission_exp_req_t *redundant_transmission_exp_req);
OpenAPI_redundant_transmission_exp_req_t *OpenAPI_redundant_transmission_exp_req_copy(OpenAPI_redundant_transmission_exp_req_t *dst, OpenAPI_redundant_transmission_exp_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_redundant_transmission_exp_req_H_ */

