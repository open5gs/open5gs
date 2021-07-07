/*
 * smf_subscription_item.h
 *
 * Contains info about a single SMF event subscription
 */

#ifndef _OpenAPI_smf_subscription_item_H_
#define _OpenAPI_smf_subscription_item_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_smf_subscription_item_s OpenAPI_smf_subscription_item_t;
typedef struct OpenAPI_smf_subscription_item_s {
    char *smf_instance_id;
    char *subscription_id;
} OpenAPI_smf_subscription_item_t;

OpenAPI_smf_subscription_item_t *OpenAPI_smf_subscription_item_create(
    char *smf_instance_id,
    char *subscription_id
);
void OpenAPI_smf_subscription_item_free(OpenAPI_smf_subscription_item_t *smf_subscription_item);
OpenAPI_smf_subscription_item_t *OpenAPI_smf_subscription_item_parseFromJSON(cJSON *smf_subscription_itemJSON);
cJSON *OpenAPI_smf_subscription_item_convertToJSON(OpenAPI_smf_subscription_item_t *smf_subscription_item);
OpenAPI_smf_subscription_item_t *OpenAPI_smf_subscription_item_copy(OpenAPI_smf_subscription_item_t *dst, OpenAPI_smf_subscription_item_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smf_subscription_item_H_ */

