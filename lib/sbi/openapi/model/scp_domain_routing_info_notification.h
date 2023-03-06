/*
 * scp_domain_routing_info_notification.h
 *
 * SCP Domain Routing Information Notification
 */

#ifndef _OpenAPI_scp_domain_routing_info_notification_H_
#define _OpenAPI_scp_domain_routing_info_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "scp_domain_routing_information.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_scp_domain_routing_info_notification_s OpenAPI_scp_domain_routing_info_notification_t;
typedef struct OpenAPI_scp_domain_routing_info_notification_s {
    struct OpenAPI_scp_domain_routing_information_s *routing_info;
    bool is_local_ind;
    int local_ind;
} OpenAPI_scp_domain_routing_info_notification_t;

OpenAPI_scp_domain_routing_info_notification_t *OpenAPI_scp_domain_routing_info_notification_create(
    OpenAPI_scp_domain_routing_information_t *routing_info,
    bool is_local_ind,
    int local_ind
);
void OpenAPI_scp_domain_routing_info_notification_free(OpenAPI_scp_domain_routing_info_notification_t *scp_domain_routing_info_notification);
OpenAPI_scp_domain_routing_info_notification_t *OpenAPI_scp_domain_routing_info_notification_parseFromJSON(cJSON *scp_domain_routing_info_notificationJSON);
cJSON *OpenAPI_scp_domain_routing_info_notification_convertToJSON(OpenAPI_scp_domain_routing_info_notification_t *scp_domain_routing_info_notification);
OpenAPI_scp_domain_routing_info_notification_t *OpenAPI_scp_domain_routing_info_notification_copy(OpenAPI_scp_domain_routing_info_notification_t *dst, OpenAPI_scp_domain_routing_info_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scp_domain_routing_info_notification_H_ */

