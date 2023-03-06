/*
 * routing_info_sm_response.h
 *
 * Addressing information of available nodes for SMS delivery
 */

#ifndef _OpenAPI_routing_info_sm_response_H_
#define _OpenAPI_routing_info_sm_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ip_sm_gw_info.h"
#include "sms_router_info.h"
#include "smsf_registration.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_routing_info_sm_response_s OpenAPI_routing_info_sm_response_t;
typedef struct OpenAPI_routing_info_sm_response_s {
    char *supi;
    struct OpenAPI_smsf_registration_s *smsf3_gpp;
    struct OpenAPI_smsf_registration_s *smsf_non3_gpp;
    struct OpenAPI_ip_sm_gw_info_s *ip_sm_gw;
    struct OpenAPI_sms_router_info_s *sms_router;
} OpenAPI_routing_info_sm_response_t;

OpenAPI_routing_info_sm_response_t *OpenAPI_routing_info_sm_response_create(
    char *supi,
    OpenAPI_smsf_registration_t *smsf3_gpp,
    OpenAPI_smsf_registration_t *smsf_non3_gpp,
    OpenAPI_ip_sm_gw_info_t *ip_sm_gw,
    OpenAPI_sms_router_info_t *sms_router
);
void OpenAPI_routing_info_sm_response_free(OpenAPI_routing_info_sm_response_t *routing_info_sm_response);
OpenAPI_routing_info_sm_response_t *OpenAPI_routing_info_sm_response_parseFromJSON(cJSON *routing_info_sm_responseJSON);
cJSON *OpenAPI_routing_info_sm_response_convertToJSON(OpenAPI_routing_info_sm_response_t *routing_info_sm_response);
OpenAPI_routing_info_sm_response_t *OpenAPI_routing_info_sm_response_copy(OpenAPI_routing_info_sm_response_t *dst, OpenAPI_routing_info_sm_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_routing_info_sm_response_H_ */

