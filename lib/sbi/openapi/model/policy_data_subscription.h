/*
 * policy_data_subscription.h
 *
 * Identifies a subscription to policy data change notification.
 */

#ifndef _OpenAPI_policy_data_subscription_H_
#define _OpenAPI_policy_data_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "resource_item.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_policy_data_subscription_s OpenAPI_policy_data_subscription_t;
typedef struct OpenAPI_policy_data_subscription_s {
    char *notification_uri;
    char *notif_id;
    OpenAPI_list_t *monitored_resource_uris;
    OpenAPI_list_t *mon_res_items;
    char *expiry;
    char *supported_features;
} OpenAPI_policy_data_subscription_t;

OpenAPI_policy_data_subscription_t *OpenAPI_policy_data_subscription_create(
    char *notification_uri,
    char *notif_id,
    OpenAPI_list_t *monitored_resource_uris,
    OpenAPI_list_t *mon_res_items,
    char *expiry,
    char *supported_features
);
void OpenAPI_policy_data_subscription_free(OpenAPI_policy_data_subscription_t *policy_data_subscription);
OpenAPI_policy_data_subscription_t *OpenAPI_policy_data_subscription_parseFromJSON(cJSON *policy_data_subscriptionJSON);
cJSON *OpenAPI_policy_data_subscription_convertToJSON(OpenAPI_policy_data_subscription_t *policy_data_subscription);
OpenAPI_policy_data_subscription_t *OpenAPI_policy_data_subscription_copy(OpenAPI_policy_data_subscription_t *dst, OpenAPI_policy_data_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_data_subscription_H_ */

