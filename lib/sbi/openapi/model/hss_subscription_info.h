/*
 * hss_subscription_info.h
 *
 * Information related to active subscriptions at the HSS(s)
 */

#ifndef _OpenAPI_hss_subscription_info_H_
#define _OpenAPI_hss_subscription_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "hss_subscription_item.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_hss_subscription_info_s OpenAPI_hss_subscription_info_t;
typedef struct OpenAPI_hss_subscription_info_s {
    OpenAPI_list_t *hss_subscription_list;
} OpenAPI_hss_subscription_info_t;

OpenAPI_hss_subscription_info_t *OpenAPI_hss_subscription_info_create(
    OpenAPI_list_t *hss_subscription_list
);
void OpenAPI_hss_subscription_info_free(OpenAPI_hss_subscription_info_t *hss_subscription_info);
OpenAPI_hss_subscription_info_t *OpenAPI_hss_subscription_info_parseFromJSON(cJSON *hss_subscription_infoJSON);
cJSON *OpenAPI_hss_subscription_info_convertToJSON(OpenAPI_hss_subscription_info_t *hss_subscription_info);
OpenAPI_hss_subscription_info_t *OpenAPI_hss_subscription_info_copy(OpenAPI_hss_subscription_info_t *dst, OpenAPI_hss_subscription_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hss_subscription_info_H_ */

