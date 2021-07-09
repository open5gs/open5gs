/*
 * amf_event_subscription_add_info.h
 *
 * 
 */

#ifndef _OpenAPI_amf_event_subscription_add_info_H_
#define _OpenAPI_amf_event_subscription_add_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nf_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_amf_event_subscription_add_info_s OpenAPI_amf_event_subscription_add_info_t;
typedef struct OpenAPI_amf_event_subscription_add_info_s {
    OpenAPI_list_t *binding_info;
    OpenAPI_nf_type_e subscribing_nf_type;
} OpenAPI_amf_event_subscription_add_info_t;

OpenAPI_amf_event_subscription_add_info_t *OpenAPI_amf_event_subscription_add_info_create(
    OpenAPI_list_t *binding_info,
    OpenAPI_nf_type_e subscribing_nf_type
);
void OpenAPI_amf_event_subscription_add_info_free(OpenAPI_amf_event_subscription_add_info_t *amf_event_subscription_add_info);
OpenAPI_amf_event_subscription_add_info_t *OpenAPI_amf_event_subscription_add_info_parseFromJSON(cJSON *amf_event_subscription_add_infoJSON);
cJSON *OpenAPI_amf_event_subscription_add_info_convertToJSON(OpenAPI_amf_event_subscription_add_info_t *amf_event_subscription_add_info);
OpenAPI_amf_event_subscription_add_info_t *OpenAPI_amf_event_subscription_add_info_copy(OpenAPI_amf_event_subscription_add_info_t *dst, OpenAPI_amf_event_subscription_add_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf_event_subscription_add_info_H_ */

