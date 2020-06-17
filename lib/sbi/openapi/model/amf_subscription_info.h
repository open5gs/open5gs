/*
 * amf_subscription_info.h
 *
 *
 */

#ifndef _OpenAPI_amf_subscription_info_H_
#define _OpenAPI_amf_subscription_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_amf_subscription_info_s OpenAPI_amf_subscription_info_t;
typedef struct OpenAPI_amf_subscription_info_s {
    char *amf_instance_id;
    char *subscription_id;
    char *subs_change_notify_correlation_id;
} OpenAPI_amf_subscription_info_t;

OpenAPI_amf_subscription_info_t *OpenAPI_amf_subscription_info_create(
    char *amf_instance_id,
    char *subscription_id,
    char *subs_change_notify_correlation_id
    );
void OpenAPI_amf_subscription_info_free(OpenAPI_amf_subscription_info_t *amf_subscription_info);
OpenAPI_amf_subscription_info_t *OpenAPI_amf_subscription_info_parseFromJSON(cJSON *amf_subscription_infoJSON);
cJSON *OpenAPI_amf_subscription_info_convertToJSON(OpenAPI_amf_subscription_info_t *amf_subscription_info);
OpenAPI_amf_subscription_info_t *OpenAPI_amf_subscription_info_copy(OpenAPI_amf_subscription_info_t *dst, OpenAPI_amf_subscription_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_subscription_info_H_ */

