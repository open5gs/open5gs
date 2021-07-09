/*
 * default_notification_subscription.h
 *
 * Data structure for specifying the notifications the NF service subscribes by default along with callback URI
 */

#ifndef _OpenAPI_default_notification_subscription_H_
#define _OpenAPI_default_notification_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n1_message_class.h"
#include "n2_information_class.h"
#include "notification_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_default_notification_subscription_s OpenAPI_default_notification_subscription_t;
typedef struct OpenAPI_default_notification_subscription_s {
    OpenAPI_notification_type_e notification_type;
    char *callback_uri;
    OpenAPI_n1_message_class_e n1_message_class;
    OpenAPI_n2_information_class_e n2_information_class;
    OpenAPI_list_t *versions;
    char *binding;
} OpenAPI_default_notification_subscription_t;

OpenAPI_default_notification_subscription_t *OpenAPI_default_notification_subscription_create(
    OpenAPI_notification_type_e notification_type,
    char *callback_uri,
    OpenAPI_n1_message_class_e n1_message_class,
    OpenAPI_n2_information_class_e n2_information_class,
    OpenAPI_list_t *versions,
    char *binding
);
void OpenAPI_default_notification_subscription_free(OpenAPI_default_notification_subscription_t *default_notification_subscription);
OpenAPI_default_notification_subscription_t *OpenAPI_default_notification_subscription_parseFromJSON(cJSON *default_notification_subscriptionJSON);
cJSON *OpenAPI_default_notification_subscription_convertToJSON(OpenAPI_default_notification_subscription_t *default_notification_subscription);
OpenAPI_default_notification_subscription_t *OpenAPI_default_notification_subscription_copy(OpenAPI_default_notification_subscription_t *dst, OpenAPI_default_notification_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_default_notification_subscription_H_ */

