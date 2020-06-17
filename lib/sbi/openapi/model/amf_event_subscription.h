/*
 * amf_event_subscription.h
 *
 *
 */

#ifndef _OpenAPI_amf_event_subscription_H_
#define _OpenAPI_amf_event_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "amf_event.h"
#include "amf_event_mode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_amf_event_subscription_s OpenAPI_amf_event_subscription_t;
typedef struct OpenAPI_amf_event_subscription_s {
    OpenAPI_list_t *event_list;
    char *event_notify_uri;
    char *notify_correlation_id;
    char *nf_id;
    char *subs_change_notify_uri;
    char *subs_change_notify_correlation_id;
    char *supi;
    char *group_id;
    char *gpsi;
    char *pei;
    int any_ue;
    struct OpenAPI_amf_event_mode_s *options;
} OpenAPI_amf_event_subscription_t;

OpenAPI_amf_event_subscription_t *OpenAPI_amf_event_subscription_create(
    OpenAPI_list_t *event_list,
    char *event_notify_uri,
    char *notify_correlation_id,
    char *nf_id,
    char *subs_change_notify_uri,
    char *subs_change_notify_correlation_id,
    char *supi,
    char *group_id,
    char *gpsi,
    char *pei,
    int any_ue,
    OpenAPI_amf_event_mode_t *options
    );
void OpenAPI_amf_event_subscription_free(OpenAPI_amf_event_subscription_t *amf_event_subscription);
OpenAPI_amf_event_subscription_t *OpenAPI_amf_event_subscription_parseFromJSON(cJSON *amf_event_subscriptionJSON);
cJSON *OpenAPI_amf_event_subscription_convertToJSON(OpenAPI_amf_event_subscription_t *amf_event_subscription);
OpenAPI_amf_event_subscription_t *OpenAPI_amf_event_subscription_copy(OpenAPI_amf_event_subscription_t *dst, OpenAPI_amf_event_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_event_subscription_H_ */

