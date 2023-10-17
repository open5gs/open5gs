/*
 * non_ue_n2_info_subscription_created_data.h
 *
 * Data for the created subscription for non-UE specific N2 information notification
 */

#ifndef _OpenAPI_non_ue_n2_info_subscription_created_data_H_
#define _OpenAPI_non_ue_n2_info_subscription_created_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_information_class.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_non_ue_n2_info_subscription_created_data_s OpenAPI_non_ue_n2_info_subscription_created_data_t;
typedef struct OpenAPI_non_ue_n2_info_subscription_created_data_s {
    char *n2_notify_subscription_id;
    char *supported_features;
    OpenAPI_n2_information_class_e n2_information_class;
} OpenAPI_non_ue_n2_info_subscription_created_data_t;

OpenAPI_non_ue_n2_info_subscription_created_data_t *OpenAPI_non_ue_n2_info_subscription_created_data_create(
    char *n2_notify_subscription_id,
    char *supported_features,
    OpenAPI_n2_information_class_e n2_information_class
);
void OpenAPI_non_ue_n2_info_subscription_created_data_free(OpenAPI_non_ue_n2_info_subscription_created_data_t *non_ue_n2_info_subscription_created_data);
OpenAPI_non_ue_n2_info_subscription_created_data_t *OpenAPI_non_ue_n2_info_subscription_created_data_parseFromJSON(cJSON *non_ue_n2_info_subscription_created_dataJSON);
cJSON *OpenAPI_non_ue_n2_info_subscription_created_data_convertToJSON(OpenAPI_non_ue_n2_info_subscription_created_data_t *non_ue_n2_info_subscription_created_data);
OpenAPI_non_ue_n2_info_subscription_created_data_t *OpenAPI_non_ue_n2_info_subscription_created_data_copy(OpenAPI_non_ue_n2_info_subscription_created_data_t *dst, OpenAPI_non_ue_n2_info_subscription_created_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_non_ue_n2_info_subscription_created_data_H_ */

