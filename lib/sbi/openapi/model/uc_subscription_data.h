/*
 * uc_subscription_data.h
 *
 * Contains the User Consent Subscription Data.
 */

#ifndef _OpenAPI_uc_subscription_data_H_
#define _OpenAPI_uc_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "user_consent.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_uc_subscription_data_s OpenAPI_uc_subscription_data_t;
typedef struct OpenAPI_uc_subscription_data_s {
    OpenAPI_list_t* user_consent_per_purpose_list;
} OpenAPI_uc_subscription_data_t;

OpenAPI_uc_subscription_data_t *OpenAPI_uc_subscription_data_create(
    OpenAPI_list_t* user_consent_per_purpose_list
);
void OpenAPI_uc_subscription_data_free(OpenAPI_uc_subscription_data_t *uc_subscription_data);
OpenAPI_uc_subscription_data_t *OpenAPI_uc_subscription_data_parseFromJSON(cJSON *uc_subscription_dataJSON);
cJSON *OpenAPI_uc_subscription_data_convertToJSON(OpenAPI_uc_subscription_data_t *uc_subscription_data);
OpenAPI_uc_subscription_data_t *OpenAPI_uc_subscription_data_copy(OpenAPI_uc_subscription_data_t *dst, OpenAPI_uc_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_uc_subscription_data_H_ */

