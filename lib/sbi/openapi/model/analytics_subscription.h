/*
 * analytics_subscription.h
 *
 * Analytics subscriptions created in the NWDAF.
 */

#ifndef _OpenAPI_analytics_subscription_H_
#define _OpenAPI_analytics_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nwdaf_subscription.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_analytics_subscription_s OpenAPI_analytics_subscription_t;
typedef struct OpenAPI_analytics_subscription_s {
    char *nwdaf_id;
    char *nwdaf_set_id;
    OpenAPI_list_t *nwdaf_subscription_list;
} OpenAPI_analytics_subscription_t;

OpenAPI_analytics_subscription_t *OpenAPI_analytics_subscription_create(
    char *nwdaf_id,
    char *nwdaf_set_id,
    OpenAPI_list_t *nwdaf_subscription_list
);
void OpenAPI_analytics_subscription_free(OpenAPI_analytics_subscription_t *analytics_subscription);
OpenAPI_analytics_subscription_t *OpenAPI_analytics_subscription_parseFromJSON(cJSON *analytics_subscriptionJSON);
cJSON *OpenAPI_analytics_subscription_convertToJSON(OpenAPI_analytics_subscription_t *analytics_subscription);
OpenAPI_analytics_subscription_t *OpenAPI_analytics_subscription_copy(OpenAPI_analytics_subscription_t *dst, OpenAPI_analytics_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_analytics_subscription_H_ */

