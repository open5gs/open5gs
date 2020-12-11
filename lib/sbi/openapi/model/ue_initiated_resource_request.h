/*
 * ue_initiated_resource_request.h
 *
 *
 */

#ifndef _OpenAPI_ue_initiated_resource_request_H_
#define _OpenAPI_ue_initiated_resource_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "packet_filter_info.h"
#include "requested_qos.h"
#include "rule_operation.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_initiated_resource_request_s OpenAPI_ue_initiated_resource_request_t;
typedef struct OpenAPI_ue_initiated_resource_request_s {
    char *pcc_rule_id;
    struct OpenAPI_rule_operation_s *rule_op;
    int precedence;
    OpenAPI_list_t *pack_filt_info;
    struct OpenAPI_requested_qos_s *req_qos;
} OpenAPI_ue_initiated_resource_request_t;

OpenAPI_ue_initiated_resource_request_t *OpenAPI_ue_initiated_resource_request_create(
    char *pcc_rule_id,
    OpenAPI_rule_operation_t *rule_op,
    int precedence,
    OpenAPI_list_t *pack_filt_info,
    OpenAPI_requested_qos_t *req_qos
    );
void OpenAPI_ue_initiated_resource_request_free(OpenAPI_ue_initiated_resource_request_t *ue_initiated_resource_request);
OpenAPI_ue_initiated_resource_request_t *OpenAPI_ue_initiated_resource_request_parseFromJSON(cJSON *ue_initiated_resource_requestJSON);
cJSON *OpenAPI_ue_initiated_resource_request_convertToJSON(OpenAPI_ue_initiated_resource_request_t *ue_initiated_resource_request);
OpenAPI_ue_initiated_resource_request_t *OpenAPI_ue_initiated_resource_request_copy(OpenAPI_ue_initiated_resource_request_t *dst, OpenAPI_ue_initiated_resource_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_initiated_resource_request_H_ */

