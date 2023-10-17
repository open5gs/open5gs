/*
 * scp_domain_routing_info_subscription.h
 *
 * SCP Domain Routing Information Subscription
 */

#ifndef _OpenAPI_scp_domain_routing_info_subscription_H_
#define _OpenAPI_scp_domain_routing_info_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_scp_domain_routing_info_subscription_s OpenAPI_scp_domain_routing_info_subscription_t;
typedef struct OpenAPI_scp_domain_routing_info_subscription_s {
    char *callback_uri;
    char *validity_time;
    char *req_instance_id;
    bool is_local_ind;
    int local_ind;
} OpenAPI_scp_domain_routing_info_subscription_t;

OpenAPI_scp_domain_routing_info_subscription_t *OpenAPI_scp_domain_routing_info_subscription_create(
    char *callback_uri,
    char *validity_time,
    char *req_instance_id,
    bool is_local_ind,
    int local_ind
);
void OpenAPI_scp_domain_routing_info_subscription_free(OpenAPI_scp_domain_routing_info_subscription_t *scp_domain_routing_info_subscription);
OpenAPI_scp_domain_routing_info_subscription_t *OpenAPI_scp_domain_routing_info_subscription_parseFromJSON(cJSON *scp_domain_routing_info_subscriptionJSON);
cJSON *OpenAPI_scp_domain_routing_info_subscription_convertToJSON(OpenAPI_scp_domain_routing_info_subscription_t *scp_domain_routing_info_subscription);
OpenAPI_scp_domain_routing_info_subscription_t *OpenAPI_scp_domain_routing_info_subscription_copy(OpenAPI_scp_domain_routing_info_subscription_t *dst, OpenAPI_scp_domain_routing_info_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scp_domain_routing_info_subscription_H_ */

