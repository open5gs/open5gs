/*
 * ue_mobility_req.h
 *
 * UE mobility analytics requirement.
 */

#ifndef _OpenAPI_ue_mobility_req_H_
#define _OpenAPI_ue_mobility_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ue_mobility_req_s OpenAPI_ue_mobility_req_t;
#include "matching_direction.h"
#include "ue_mobility_order_criterion.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ue_mobility_req_s {
    OpenAPI_ue_mobility_order_criterion_e order_criterion;
    OpenAPI_matching_direction_e order_direction;
    bool is_ue_loc_order_ind;
    int ue_loc_order_ind;
    OpenAPI_list_t *dist_thresholds;
};

OpenAPI_ue_mobility_req_t *OpenAPI_ue_mobility_req_create(
    OpenAPI_ue_mobility_order_criterion_e order_criterion,
    OpenAPI_matching_direction_e order_direction,
    bool is_ue_loc_order_ind,
    int ue_loc_order_ind,
    OpenAPI_list_t *dist_thresholds
);
void OpenAPI_ue_mobility_req_free(OpenAPI_ue_mobility_req_t *ue_mobility_req);
OpenAPI_ue_mobility_req_t *OpenAPI_ue_mobility_req_parseFromJSON(cJSON *ue_mobility_reqJSON);
cJSON *OpenAPI_ue_mobility_req_convertToJSON(OpenAPI_ue_mobility_req_t *ue_mobility_req);
OpenAPI_ue_mobility_req_t *OpenAPI_ue_mobility_req_copy(OpenAPI_ue_mobility_req_t *dst, OpenAPI_ue_mobility_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_mobility_req_H_ */

