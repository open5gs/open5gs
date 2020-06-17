/*
 * data_change_notify.h
 *
 *
 */

#ifndef _OpenAPI_data_change_notify_H_
#define _OpenAPI_data_change_notify_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "notify_item.h"
#include "sdm_subscription.h"
#include "subscription_data_subscriptions.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_data_change_notify_s OpenAPI_data_change_notify_t;
typedef struct OpenAPI_data_change_notify_s {
    OpenAPI_list_t *original_callback_reference;
    char *ue_id;
    OpenAPI_list_t *notify_items;
    struct OpenAPI_sdm_subscription_s *sdm_subscription;
    OpenAPI_list_t *additional_sdm_subscriptions;
    OpenAPI_list_t *subscription_data_subscriptions;
} OpenAPI_data_change_notify_t;

OpenAPI_data_change_notify_t *OpenAPI_data_change_notify_create(
    OpenAPI_list_t *original_callback_reference,
    char *ue_id,
    OpenAPI_list_t *notify_items,
    OpenAPI_sdm_subscription_t *sdm_subscription,
    OpenAPI_list_t *additional_sdm_subscriptions,
    OpenAPI_list_t *subscription_data_subscriptions
    );
void OpenAPI_data_change_notify_free(OpenAPI_data_change_notify_t *data_change_notify);
OpenAPI_data_change_notify_t *OpenAPI_data_change_notify_parseFromJSON(cJSON *data_change_notifyJSON);
cJSON *OpenAPI_data_change_notify_convertToJSON(OpenAPI_data_change_notify_t *data_change_notify);
OpenAPI_data_change_notify_t *OpenAPI_data_change_notify_copy(OpenAPI_data_change_notify_t *dst, OpenAPI_data_change_notify_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_data_change_notify_H_ */

