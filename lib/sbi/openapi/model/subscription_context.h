/*
 * subscription_context.h
 *
 * Context data related to a created subscription, to be included in notifications sent by NRF 
 */

#ifndef _OpenAPI_subscription_context_H_
#define _OpenAPI_subscription_context_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "subscr_cond.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_subscription_context_s OpenAPI_subscription_context_t;
typedef struct OpenAPI_subscription_context_s {
    char *subscription_id;
    struct OpenAPI_subscr_cond_s *subscr_cond;
} OpenAPI_subscription_context_t;

OpenAPI_subscription_context_t *OpenAPI_subscription_context_create(
    char *subscription_id,
    OpenAPI_subscr_cond_t *subscr_cond
);
void OpenAPI_subscription_context_free(OpenAPI_subscription_context_t *subscription_context);
OpenAPI_subscription_context_t *OpenAPI_subscription_context_parseFromJSON(cJSON *subscription_contextJSON);
cJSON *OpenAPI_subscription_context_convertToJSON(OpenAPI_subscription_context_t *subscription_context);
OpenAPI_subscription_context_t *OpenAPI_subscription_context_copy(OpenAPI_subscription_context_t *dst, OpenAPI_subscription_context_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_subscription_context_H_ */

