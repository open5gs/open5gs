/*
 * qos_policy_assist_req.h
 *
 * The QoS and policy assistance analytics requirement information.
 */

#ifndef _OpenAPI_qos_policy_assist_req_H_
#define _OpenAPI_qos_policy_assist_req_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qos_policy_assist_req_s OpenAPI_qos_policy_assist_req_t;
#include "matching_direction.h"
#include "qos_para.h"
#include "qos_pol_order_criterion.h"
#include "rat_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qos_policy_assist_req_s {
    OpenAPI_qos_pol_order_criterion_e order_criterion;
    OpenAPI_matching_direction_e order_direction;
    OpenAPI_list_t *freqs;
    OpenAPI_list_t *rat_types;
    OpenAPI_list_t *qos_param_sets;
    bool is_requested_qoe;
    float requested_qoe;
};

OpenAPI_qos_policy_assist_req_t *OpenAPI_qos_policy_assist_req_create(
    OpenAPI_qos_pol_order_criterion_e order_criterion,
    OpenAPI_matching_direction_e order_direction,
    OpenAPI_list_t *freqs,
    OpenAPI_list_t *rat_types,
    OpenAPI_list_t *qos_param_sets,
    bool is_requested_qoe,
    float requested_qoe
);
void OpenAPI_qos_policy_assist_req_free(OpenAPI_qos_policy_assist_req_t *qos_policy_assist_req);
OpenAPI_qos_policy_assist_req_t *OpenAPI_qos_policy_assist_req_parseFromJSON(cJSON *qos_policy_assist_reqJSON);
cJSON *OpenAPI_qos_policy_assist_req_convertToJSON(OpenAPI_qos_policy_assist_req_t *qos_policy_assist_req);
OpenAPI_qos_policy_assist_req_t *OpenAPI_qos_policy_assist_req_copy(OpenAPI_qos_policy_assist_req_t *dst, OpenAPI_qos_policy_assist_req_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_policy_assist_req_H_ */

