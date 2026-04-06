/*
 * ue_comm_req.h
 *
 * UE communication analytics requirement.
 */

#ifndef _OpenAPI_ue_comm_req_H_
#define _OpenAPI_ue_comm_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ue_comm_req_s OpenAPI_ue_comm_req_t;
#include "matching_direction.h"
#include "ue_comm_order_criterion.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ue_comm_req_s {
    OpenAPI_ue_comm_order_criterion_e order_criterion;
    OpenAPI_matching_direction_e order_direction;
};

OpenAPI_ue_comm_req_t *OpenAPI_ue_comm_req_create(
    OpenAPI_ue_comm_order_criterion_e order_criterion,
    OpenAPI_matching_direction_e order_direction
);
void OpenAPI_ue_comm_req_free(OpenAPI_ue_comm_req_t *ue_comm_req);
OpenAPI_ue_comm_req_t *OpenAPI_ue_comm_req_parseFromJSON(cJSON *ue_comm_reqJSON);
cJSON *OpenAPI_ue_comm_req_convertToJSON(OpenAPI_ue_comm_req_t *ue_comm_req);
OpenAPI_ue_comm_req_t *OpenAPI_ue_comm_req_copy(OpenAPI_ue_comm_req_t *dst, OpenAPI_ue_comm_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_comm_req_H_ */

