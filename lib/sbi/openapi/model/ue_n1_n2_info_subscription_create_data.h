/*
 * ue_n1_n2_info_subscription_create_data.h
 *
 * Data within a create subscription request for UE specific N1 and/or N2 information notification
 */

#ifndef _OpenAPI_ue_n1_n2_info_subscription_create_data_H_
#define _OpenAPI_ue_n1_n2_info_subscription_create_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "guami.h"
#include "n1_message_class.h"
#include "n2_information_class.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_n1_n2_info_subscription_create_data_s OpenAPI_ue_n1_n2_info_subscription_create_data_t;
typedef struct OpenAPI_ue_n1_n2_info_subscription_create_data_s {
    OpenAPI_n2_information_class_e n2_information_class;
    char *n2_notify_callback_uri;
    OpenAPI_n1_message_class_e n1_message_class;
    char *n1_notify_callback_uri;
    char *nf_id;
    char *supported_features;
    struct OpenAPI_guami_s *old_guami;
} OpenAPI_ue_n1_n2_info_subscription_create_data_t;

OpenAPI_ue_n1_n2_info_subscription_create_data_t *OpenAPI_ue_n1_n2_info_subscription_create_data_create(
    OpenAPI_n2_information_class_e n2_information_class,
    char *n2_notify_callback_uri,
    OpenAPI_n1_message_class_e n1_message_class,
    char *n1_notify_callback_uri,
    char *nf_id,
    char *supported_features,
    OpenAPI_guami_t *old_guami
);
void OpenAPI_ue_n1_n2_info_subscription_create_data_free(OpenAPI_ue_n1_n2_info_subscription_create_data_t *ue_n1_n2_info_subscription_create_data);
OpenAPI_ue_n1_n2_info_subscription_create_data_t *OpenAPI_ue_n1_n2_info_subscription_create_data_parseFromJSON(cJSON *ue_n1_n2_info_subscription_create_dataJSON);
cJSON *OpenAPI_ue_n1_n2_info_subscription_create_data_convertToJSON(OpenAPI_ue_n1_n2_info_subscription_create_data_t *ue_n1_n2_info_subscription_create_data);
OpenAPI_ue_n1_n2_info_subscription_create_data_t *OpenAPI_ue_n1_n2_info_subscription_create_data_copy(OpenAPI_ue_n1_n2_info_subscription_create_data_t *dst, OpenAPI_ue_n1_n2_info_subscription_create_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_n1_n2_info_subscription_create_data_H_ */

