/*
 * subscription_data_subscriptions.h
 *
 * 
 */

#ifndef _OpenAPI_subscription_data_subscriptions_H_
#define _OpenAPI_subscription_data_subscriptions_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sdm_subscription_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_subscription_data_subscriptions_s OpenAPI_subscription_data_subscriptions_t;
typedef struct OpenAPI_subscription_data_subscriptions_s {
    char *ue_id;
    char *callback_reference;
    char *original_callback_reference;
    OpenAPI_list_t *monitored_resource_uris;
    char *expiry;
    struct OpenAPI_sdm_subscription_1_s *sdm_subscription;
    char *subscription_id;
    char *supported_features;
} OpenAPI_subscription_data_subscriptions_t;

OpenAPI_subscription_data_subscriptions_t *OpenAPI_subscription_data_subscriptions_create(
    char *ue_id,
    char *callback_reference,
    char *original_callback_reference,
    OpenAPI_list_t *monitored_resource_uris,
    char *expiry,
    OpenAPI_sdm_subscription_1_t *sdm_subscription,
    char *subscription_id,
    char *supported_features
);
void OpenAPI_subscription_data_subscriptions_free(OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptions);
OpenAPI_subscription_data_subscriptions_t *OpenAPI_subscription_data_subscriptions_parseFromJSON(cJSON *subscription_data_subscriptionsJSON);
cJSON *OpenAPI_subscription_data_subscriptions_convertToJSON(OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptions);
OpenAPI_subscription_data_subscriptions_t *OpenAPI_subscription_data_subscriptions_copy(OpenAPI_subscription_data_subscriptions_t *dst, OpenAPI_subscription_data_subscriptions_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscription_data_subscriptions_H_ */

