/*
 * nwdaf_subscription.h
 *
 * Individual NWDAF subscription identified by the subscription Id.
 */

#ifndef _OpenAPI_nwdaf_subscription_H_
#define _OpenAPI_nwdaf_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nnwdaf_events_subscription.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nwdaf_subscription_s OpenAPI_nwdaf_subscription_t;
typedef struct OpenAPI_nwdaf_subscription_s {
    char *nwdaf_evt_subs_service_uri;
    struct OpenAPI_nnwdaf_events_subscription_s *nwdaf_events_subscription;
} OpenAPI_nwdaf_subscription_t;

OpenAPI_nwdaf_subscription_t *OpenAPI_nwdaf_subscription_create(
    char *nwdaf_evt_subs_service_uri,
    OpenAPI_nnwdaf_events_subscription_t *nwdaf_events_subscription
);
void OpenAPI_nwdaf_subscription_free(OpenAPI_nwdaf_subscription_t *nwdaf_subscription);
OpenAPI_nwdaf_subscription_t *OpenAPI_nwdaf_subscription_parseFromJSON(cJSON *nwdaf_subscriptionJSON);
cJSON *OpenAPI_nwdaf_subscription_convertToJSON(OpenAPI_nwdaf_subscription_t *nwdaf_subscription);
OpenAPI_nwdaf_subscription_t *OpenAPI_nwdaf_subscription_copy(OpenAPI_nwdaf_subscription_t *dst, OpenAPI_nwdaf_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_subscription_H_ */

