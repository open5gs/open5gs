/*
 * subscription_data_subscriptions.h
 *
 * A subscription to notifications.
 */

#ifndef _OpenAPI_subscription_data_subscriptions_H_
#define _OpenAPI_subscription_data_subscriptions_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_subscription_data_subscriptions_s OpenAPI_subscription_data_subscriptions_t;
#include "additional_data_ref.h"
#include "hss_subscription_info.h"
#include "immediate_report_1.h"
#include "sdm_subscription_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_subscription_data_subscriptions_s {
    char *ue_id;
    char *callback_reference;
    char *original_callback_reference;
    OpenAPI_list_t *monitored_resource_uris;
    char *expiry;
    struct OpenAPI_sdm_subscription_1_s *sdm_subscription;
    struct OpenAPI_hss_subscription_info_s *hss_subscription_info;
    char *subscription_id;
    bool is_unique_subscription;
    int unique_subscription;
    char *supported_features;
    bool is_immediate_report;
    int immediate_report;
    struct OpenAPI_immediate_report_1_s *report;
    OpenAPI_list_t *additional_data_refs;
    OpenAPI_list_t *reset_ids;
};

OpenAPI_subscription_data_subscriptions_t *OpenAPI_subscription_data_subscriptions_create(
    char *ue_id,
    char *callback_reference,
    char *original_callback_reference,
    OpenAPI_list_t *monitored_resource_uris,
    char *expiry,
    OpenAPI_sdm_subscription_1_t *sdm_subscription,
    OpenAPI_hss_subscription_info_t *hss_subscription_info,
    char *subscription_id,
    bool is_unique_subscription,
    int unique_subscription,
    char *supported_features,
    bool is_immediate_report,
    int immediate_report,
    OpenAPI_immediate_report_1_t *report,
    OpenAPI_list_t *additional_data_refs,
    OpenAPI_list_t *reset_ids
);
void OpenAPI_subscription_data_subscriptions_free(OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptions);
OpenAPI_subscription_data_subscriptions_t *OpenAPI_subscription_data_subscriptions_parseFromJSON(cJSON *subscription_data_subscriptionsJSON);
cJSON *OpenAPI_subscription_data_subscriptions_convertToJSON(OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptions);
OpenAPI_subscription_data_subscriptions_t *OpenAPI_subscription_data_subscriptions_copy(OpenAPI_subscription_data_subscriptions_t *dst, OpenAPI_subscription_data_subscriptions_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscription_data_subscriptions_H_ */

