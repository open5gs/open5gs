/*
 * ee_subscription.h
 *
 *
 */

#ifndef _OpenAPI_ee_subscription_H_
#define _OpenAPI_ee_subscription_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "monitoring_configuration.h"
#include "reporting_options.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ee_subscription_s OpenAPI_ee_subscription_t;
typedef struct OpenAPI_ee_subscription_s {
    char *callback_reference;
    OpenAPI_list_t* monitoring_configurations;
    struct OpenAPI_reporting_options_s *reporting_options;
    char *supported_features;
    char *subscription_id;
} OpenAPI_ee_subscription_t;

OpenAPI_ee_subscription_t *OpenAPI_ee_subscription_create(
    char *callback_reference,
    OpenAPI_list_t* monitoring_configurations,
    OpenAPI_reporting_options_t *reporting_options,
    char *supported_features,
    char *subscription_id
    );
void OpenAPI_ee_subscription_free(OpenAPI_ee_subscription_t *ee_subscription);
OpenAPI_ee_subscription_t *OpenAPI_ee_subscription_parseFromJSON(cJSON *ee_subscriptionJSON);
cJSON *OpenAPI_ee_subscription_convertToJSON(OpenAPI_ee_subscription_t *ee_subscription);
OpenAPI_ee_subscription_t *OpenAPI_ee_subscription_copy(OpenAPI_ee_subscription_t *dst, OpenAPI_ee_subscription_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ee_subscription_H_ */

