/*
 * hss_subscription_item.h
 *
 * Contains info about a single HSS event subscription
 */

#ifndef _OpenAPI_hss_subscription_item_H_
#define _OpenAPI_hss_subscription_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "context_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_hss_subscription_item_s OpenAPI_hss_subscription_item_t;
typedef struct OpenAPI_hss_subscription_item_s {
    char *hss_instance_id;
    char *subscription_id;
    struct OpenAPI_context_info_s *context_info;
} OpenAPI_hss_subscription_item_t;

OpenAPI_hss_subscription_item_t *OpenAPI_hss_subscription_item_create(
    char *hss_instance_id,
    char *subscription_id,
    OpenAPI_context_info_t *context_info
);
void OpenAPI_hss_subscription_item_free(OpenAPI_hss_subscription_item_t *hss_subscription_item);
OpenAPI_hss_subscription_item_t *OpenAPI_hss_subscription_item_parseFromJSON(cJSON *hss_subscription_itemJSON);
cJSON *OpenAPI_hss_subscription_item_convertToJSON(OpenAPI_hss_subscription_item_t *hss_subscription_item);
OpenAPI_hss_subscription_item_t *OpenAPI_hss_subscription_item_copy(OpenAPI_hss_subscription_item_t *dst, OpenAPI_hss_subscription_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hss_subscription_item_H_ */

