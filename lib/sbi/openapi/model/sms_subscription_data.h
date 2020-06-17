/*
 * sms_subscription_data.h
 *
 *
 */

#ifndef _OpenAPI_sms_subscription_data_H_
#define _OpenAPI_sms_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sms_subscription_data_s OpenAPI_sms_subscription_data_t;
typedef struct OpenAPI_sms_subscription_data_s {
    int sms_subscribed;
    char *shared_sms_subs_data_id;
} OpenAPI_sms_subscription_data_t;

OpenAPI_sms_subscription_data_t *OpenAPI_sms_subscription_data_create(
    int sms_subscribed,
    char *shared_sms_subs_data_id
    );
void OpenAPI_sms_subscription_data_free(OpenAPI_sms_subscription_data_t *sms_subscription_data);
OpenAPI_sms_subscription_data_t *OpenAPI_sms_subscription_data_parseFromJSON(cJSON *sms_subscription_dataJSON);
cJSON *OpenAPI_sms_subscription_data_convertToJSON(OpenAPI_sms_subscription_data_t *sms_subscription_data);
OpenAPI_sms_subscription_data_t *OpenAPI_sms_subscription_data_copy(OpenAPI_sms_subscription_data_t *dst, OpenAPI_sms_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sms_subscription_data_H_ */

