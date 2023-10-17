/*
 * aerial_ue_subscription_info.h
 *
 * Contains the Aerial UE Subscription Information, it at least contains the Aerial UE Indication.
 */

#ifndef _OpenAPI_aerial_ue_subscription_info_H_
#define _OpenAPI_aerial_ue_subscription_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "aerial_ue_indication.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_aerial_ue_subscription_info_s OpenAPI_aerial_ue_subscription_info_t;
typedef struct OpenAPI_aerial_ue_subscription_info_s {
    OpenAPI_aerial_ue_indication_e aerial_ue_ind;
    char *_3gpp_uav_id;
} OpenAPI_aerial_ue_subscription_info_t;

OpenAPI_aerial_ue_subscription_info_t *OpenAPI_aerial_ue_subscription_info_create(
    OpenAPI_aerial_ue_indication_e aerial_ue_ind,
    char *_3gpp_uav_id
);
void OpenAPI_aerial_ue_subscription_info_free(OpenAPI_aerial_ue_subscription_info_t *aerial_ue_subscription_info);
OpenAPI_aerial_ue_subscription_info_t *OpenAPI_aerial_ue_subscription_info_parseFromJSON(cJSON *aerial_ue_subscription_infoJSON);
cJSON *OpenAPI_aerial_ue_subscription_info_convertToJSON(OpenAPI_aerial_ue_subscription_info_t *aerial_ue_subscription_info);
OpenAPI_aerial_ue_subscription_info_t *OpenAPI_aerial_ue_subscription_info_copy(OpenAPI_aerial_ue_subscription_info_t *dst, OpenAPI_aerial_ue_subscription_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_aerial_ue_subscription_info_H_ */

