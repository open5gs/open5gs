/*
 * nnwdaf_events_subscription.h
 *
 * Represents an Individual NWDAF Event Subscription resource.
 */

#ifndef _OpenAPI_nnwdaf_events_subscription_H_
#define _OpenAPI_nnwdaf_events_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "consumer_nf_information.h"
#include "event_notification.h"
#include "event_subscription.h"
#include "failure_event_info.h"
#include "prev_sub_info.h"
#include "reporting_information.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nnwdaf_events_subscription_s OpenAPI_nnwdaf_events_subscription_t;
typedef struct OpenAPI_nnwdaf_events_subscription_s {
    OpenAPI_list_t *event_subscriptions;
    struct OpenAPI_reporting_information_s *evt_req;
    char *notification_uri;
    char *notif_corr_id;
    char *supported_features;
    OpenAPI_list_t *event_notifications;
    OpenAPI_list_t *fail_event_reports;
    struct OpenAPI_prev_sub_info_s *prev_sub;
    struct OpenAPI_consumer_nf_information_s *cons_nf_info;
} OpenAPI_nnwdaf_events_subscription_t;

OpenAPI_nnwdaf_events_subscription_t *OpenAPI_nnwdaf_events_subscription_create(
    OpenAPI_list_t *event_subscriptions,
    OpenAPI_reporting_information_t *evt_req,
    char *notification_uri,
    char *notif_corr_id,
    char *supported_features,
    OpenAPI_list_t *event_notifications,
    OpenAPI_list_t *fail_event_reports,
    OpenAPI_prev_sub_info_t *prev_sub,
    OpenAPI_consumer_nf_information_t *cons_nf_info
);
void OpenAPI_nnwdaf_events_subscription_free(OpenAPI_nnwdaf_events_subscription_t *nnwdaf_events_subscription);
OpenAPI_nnwdaf_events_subscription_t *OpenAPI_nnwdaf_events_subscription_parseFromJSON(cJSON *nnwdaf_events_subscriptionJSON);
cJSON *OpenAPI_nnwdaf_events_subscription_convertToJSON(OpenAPI_nnwdaf_events_subscription_t *nnwdaf_events_subscription);
OpenAPI_nnwdaf_events_subscription_t *OpenAPI_nnwdaf_events_subscription_copy(OpenAPI_nnwdaf_events_subscription_t *dst, OpenAPI_nnwdaf_events_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nnwdaf_events_subscription_H_ */

