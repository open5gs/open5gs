/*
 * bsf_subscription_resp.h
 *
 * It represents a response to a modification or creation request of an Individual Binding  Subscription resource. It may contain the notification of the already met events. 
 */

#ifndef _OpenAPI_bsf_subscription_resp_H_
#define _OpenAPI_bsf_subscription_resp_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "binding_level.h"
#include "bsf_event.h"
#include "bsf_event_notification.h"
#include "bsf_notification.h"
#include "bsf_subscription.h"
#include "snssai_dnn_pair.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_bsf_subscription_resp_s OpenAPI_bsf_subscription_resp_t;
typedef struct OpenAPI_bsf_subscription_resp_s {
    OpenAPI_list_t *events;
    char *notif_uri;
    char *notif_corre_id;
    char *supi;
    char *gpsi;
    struct OpenAPI_snssai_dnn_pair_s *snssai_dnn_pairs;
    OpenAPI_list_t *add_snssai_dnn_pairs;
    char *supp_feat;
    char *pcf_id;
    char *pcf_set_id;
    OpenAPI_binding_level_e bind_level;
    OpenAPI_list_t *event_notifs;
} OpenAPI_bsf_subscription_resp_t;

OpenAPI_bsf_subscription_resp_t *OpenAPI_bsf_subscription_resp_create(
    OpenAPI_list_t *events,
    char *notif_uri,
    char *notif_corre_id,
    char *supi,
    char *gpsi,
    OpenAPI_snssai_dnn_pair_t *snssai_dnn_pairs,
    OpenAPI_list_t *add_snssai_dnn_pairs,
    char *supp_feat,
    char *pcf_id,
    char *pcf_set_id,
    OpenAPI_binding_level_e bind_level,
    OpenAPI_list_t *event_notifs
);
void OpenAPI_bsf_subscription_resp_free(OpenAPI_bsf_subscription_resp_t *bsf_subscription_resp);
OpenAPI_bsf_subscription_resp_t *OpenAPI_bsf_subscription_resp_parseFromJSON(cJSON *bsf_subscription_respJSON);
cJSON *OpenAPI_bsf_subscription_resp_convertToJSON(OpenAPI_bsf_subscription_resp_t *bsf_subscription_resp);
OpenAPI_bsf_subscription_resp_t *OpenAPI_bsf_subscription_resp_copy(OpenAPI_bsf_subscription_resp_t *dst, OpenAPI_bsf_subscription_resp_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bsf_subscription_resp_H_ */

