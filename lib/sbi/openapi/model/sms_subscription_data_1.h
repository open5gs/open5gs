/*
 * sms_subscription_data_1.h
 *
 * 
 */

#ifndef _OpenAPI_sms_subscription_data_1_H_
#define _OpenAPI_sms_subscription_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sms_subscription_data_1_s OpenAPI_sms_subscription_data_1_t;
typedef struct OpenAPI_sms_subscription_data_1_s {
    bool is_sms_subscribed;
    int sms_subscribed;
    char *shared_sms_subs_data_id;
} OpenAPI_sms_subscription_data_1_t;

OpenAPI_sms_subscription_data_1_t *OpenAPI_sms_subscription_data_1_create(
    bool is_sms_subscribed,
    int sms_subscribed,
    char *shared_sms_subs_data_id
);
void OpenAPI_sms_subscription_data_1_free(OpenAPI_sms_subscription_data_1_t *sms_subscription_data_1);
OpenAPI_sms_subscription_data_1_t *OpenAPI_sms_subscription_data_1_parseFromJSON(cJSON *sms_subscription_data_1JSON);
cJSON *OpenAPI_sms_subscription_data_1_convertToJSON(OpenAPI_sms_subscription_data_1_t *sms_subscription_data_1);
OpenAPI_sms_subscription_data_1_t *OpenAPI_sms_subscription_data_1_copy(OpenAPI_sms_subscription_data_1_t *dst, OpenAPI_sms_subscription_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_subscription_data_1_H_ */

