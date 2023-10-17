/*
 * bsf_subscription.h
 *
 * Contains the event subscription data.
 */

#ifndef _OpenAPI_bsf_subscription_H_
#define _OpenAPI_bsf_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "bsf_event.h"
#include "snssai_dnn_pair.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_bsf_subscription_s OpenAPI_bsf_subscription_t;
typedef struct OpenAPI_bsf_subscription_s {
    OpenAPI_list_t *events;
    char *notif_uri;
    char *notif_corre_id;
    char *supi;
    char *gpsi;
    struct OpenAPI_snssai_dnn_pair_s *snssai_dnn_pairs;
    OpenAPI_list_t *add_snssai_dnn_pairs;
    char *supp_feat;
} OpenAPI_bsf_subscription_t;

OpenAPI_bsf_subscription_t *OpenAPI_bsf_subscription_create(
    OpenAPI_list_t *events,
    char *notif_uri,
    char *notif_corre_id,
    char *supi,
    char *gpsi,
    OpenAPI_snssai_dnn_pair_t *snssai_dnn_pairs,
    OpenAPI_list_t *add_snssai_dnn_pairs,
    char *supp_feat
);
void OpenAPI_bsf_subscription_free(OpenAPI_bsf_subscription_t *bsf_subscription);
OpenAPI_bsf_subscription_t *OpenAPI_bsf_subscription_parseFromJSON(cJSON *bsf_subscriptionJSON);
cJSON *OpenAPI_bsf_subscription_convertToJSON(OpenAPI_bsf_subscription_t *bsf_subscription);
OpenAPI_bsf_subscription_t *OpenAPI_bsf_subscription_copy(OpenAPI_bsf_subscription_t *dst, OpenAPI_bsf_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bsf_subscription_H_ */

