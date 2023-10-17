/*
 * routing_info_sm_request.h
 *
 * Request body of the send-routing-info-sm custom operation
 */

#ifndef _OpenAPI_routing_info_sm_request_H_
#define _OpenAPI_routing_info_sm_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_routing_info_sm_request_s OpenAPI_routing_info_sm_request_t;
typedef struct OpenAPI_routing_info_sm_request_s {
    bool is_ip_sm_gw_ind;
    int ip_sm_gw_ind;
    char *supported_features;
} OpenAPI_routing_info_sm_request_t;

OpenAPI_routing_info_sm_request_t *OpenAPI_routing_info_sm_request_create(
    bool is_ip_sm_gw_ind,
    int ip_sm_gw_ind,
    char *supported_features
);
void OpenAPI_routing_info_sm_request_free(OpenAPI_routing_info_sm_request_t *routing_info_sm_request);
OpenAPI_routing_info_sm_request_t *OpenAPI_routing_info_sm_request_parseFromJSON(cJSON *routing_info_sm_requestJSON);
cJSON *OpenAPI_routing_info_sm_request_convertToJSON(OpenAPI_routing_info_sm_request_t *routing_info_sm_request);
OpenAPI_routing_info_sm_request_t *OpenAPI_routing_info_sm_request_copy(OpenAPI_routing_info_sm_request_t *dst, OpenAPI_routing_info_sm_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_routing_info_sm_request_H_ */

