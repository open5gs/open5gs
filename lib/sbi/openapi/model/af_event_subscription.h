/*
 * af_event_subscription.h
 *
 * describes the event information delivered in the subscription
 */

#ifndef _OpenAPI_af_event_subscription_H_
#define _OpenAPI_af_event_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_event.h"
#include "af_notif_method.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_af_event_subscription_s OpenAPI_af_event_subscription_t;
typedef struct OpenAPI_af_event_subscription_s {
    OpenAPI_af_event_e event;
    OpenAPI_af_notif_method_e notif_method;
    bool is_rep_period;
    int rep_period;
    bool is_wait_time;
    int wait_time;
} OpenAPI_af_event_subscription_t;

OpenAPI_af_event_subscription_t *OpenAPI_af_event_subscription_create(
    OpenAPI_af_event_e event,
    OpenAPI_af_notif_method_e notif_method,
    bool is_rep_period,
    int rep_period,
    bool is_wait_time,
    int wait_time
);
void OpenAPI_af_event_subscription_free(OpenAPI_af_event_subscription_t *af_event_subscription);
OpenAPI_af_event_subscription_t *OpenAPI_af_event_subscription_parseFromJSON(cJSON *af_event_subscriptionJSON);
cJSON *OpenAPI_af_event_subscription_convertToJSON(OpenAPI_af_event_subscription_t *af_event_subscription);
OpenAPI_af_event_subscription_t *OpenAPI_af_event_subscription_copy(OpenAPI_af_event_subscription_t *dst, OpenAPI_af_event_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_event_subscription_H_ */

