/*
 * updp_subscription_data.h
 *
 * UE policy delivery related N1 message notification subscription data.
 */

#ifndef _OpenAPI_updp_subscription_data_H_
#define _OpenAPI_updp_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_updp_subscription_data_s OpenAPI_updp_subscription_data_t;
typedef struct OpenAPI_updp_subscription_data_s {
    char *updp_notify_subscription_id;
    char *updp_notify_callback_uri;
    char *supported_features;
    char *updp_callback_binding;
} OpenAPI_updp_subscription_data_t;

OpenAPI_updp_subscription_data_t *OpenAPI_updp_subscription_data_create(
    char *updp_notify_subscription_id,
    char *updp_notify_callback_uri,
    char *supported_features,
    char *updp_callback_binding
);
void OpenAPI_updp_subscription_data_free(OpenAPI_updp_subscription_data_t *updp_subscription_data);
OpenAPI_updp_subscription_data_t *OpenAPI_updp_subscription_data_parseFromJSON(cJSON *updp_subscription_dataJSON);
cJSON *OpenAPI_updp_subscription_data_convertToJSON(OpenAPI_updp_subscription_data_t *updp_subscription_data);
OpenAPI_updp_subscription_data_t *OpenAPI_updp_subscription_data_copy(OpenAPI_updp_subscription_data_t *dst, OpenAPI_updp_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_updp_subscription_data_H_ */

