/*
 * exposure_data_subscription.h
 *
 *
 */

#ifndef _OpenAPI_exposure_data_subscription_H_
#define _OpenAPI_exposure_data_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_exposure_data_subscription_s OpenAPI_exposure_data_subscription_t;
typedef struct OpenAPI_exposure_data_subscription_s {
    char *notification_uri;
    OpenAPI_list_t *monitored_resource_uris;
    char *expiry;
    char *supported_features;
} OpenAPI_exposure_data_subscription_t;

OpenAPI_exposure_data_subscription_t *OpenAPI_exposure_data_subscription_create(
    char *notification_uri,
    OpenAPI_list_t *monitored_resource_uris,
    char *expiry,
    char *supported_features
    );
void OpenAPI_exposure_data_subscription_free(OpenAPI_exposure_data_subscription_t *exposure_data_subscription);
OpenAPI_exposure_data_subscription_t *OpenAPI_exposure_data_subscription_parseFromJSON(cJSON *exposure_data_subscriptionJSON);
cJSON *OpenAPI_exposure_data_subscription_convertToJSON(OpenAPI_exposure_data_subscription_t *exposure_data_subscription);
OpenAPI_exposure_data_subscription_t *OpenAPI_exposure_data_subscription_copy(OpenAPI_exposure_data_subscription_t *dst, OpenAPI_exposure_data_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_exposure_data_subscription_H_ */

