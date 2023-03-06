/*
 * modifysubscription_data_subscription_200_response.h
 *
 * 
 */

#ifndef _OpenAPI_modifysubscription_data_subscription_200_response_H_
#define _OpenAPI_modifysubscription_data_subscription_200_response_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "hss_subscription_info.h"
#include "patch_result.h"
#include "report_item.h"
#include "sdm_subscription_1.h"
#include "subscription_data_subscriptions.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_modifysubscription_data_subscription_200_response_s OpenAPI_modifysubscription_data_subscription_200_response_t;
typedef struct OpenAPI_modifysubscription_data_subscription_200_response_s {
    OpenAPI_list_t *report;
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
} OpenAPI_modifysubscription_data_subscription_200_response_t;

OpenAPI_modifysubscription_data_subscription_200_response_t *OpenAPI_modifysubscription_data_subscription_200_response_create(
    OpenAPI_list_t *report,
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
    char *supported_features
);
void OpenAPI_modifysubscription_data_subscription_200_response_free(OpenAPI_modifysubscription_data_subscription_200_response_t *modifysubscription_data_subscription_200_response);
OpenAPI_modifysubscription_data_subscription_200_response_t *OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON(cJSON *modifysubscription_data_subscription_200_responseJSON);
cJSON *OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON(OpenAPI_modifysubscription_data_subscription_200_response_t *modifysubscription_data_subscription_200_response);
OpenAPI_modifysubscription_data_subscription_200_response_t *OpenAPI_modifysubscription_data_subscription_200_response_copy(OpenAPI_modifysubscription_data_subscription_200_response_t *dst, OpenAPI_modifysubscription_data_subscription_200_response_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_modifysubscription_data_subscription_200_response_H_ */

