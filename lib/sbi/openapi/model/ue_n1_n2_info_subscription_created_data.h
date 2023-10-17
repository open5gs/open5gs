/*
 * ue_n1_n2_info_subscription_created_data.h
 *
 * Data for the created subscription for UE specific N1 and/or N2 information notification
 */

#ifndef _OpenAPI_ue_n1_n2_info_subscription_created_data_H_
#define _OpenAPI_ue_n1_n2_info_subscription_created_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_n1_n2_info_subscription_created_data_s OpenAPI_ue_n1_n2_info_subscription_created_data_t;
typedef struct OpenAPI_ue_n1_n2_info_subscription_created_data_s {
    char *n1n2_notify_subscription_id;
    char *supported_features;
} OpenAPI_ue_n1_n2_info_subscription_created_data_t;

OpenAPI_ue_n1_n2_info_subscription_created_data_t *OpenAPI_ue_n1_n2_info_subscription_created_data_create(
    char *n1n2_notify_subscription_id,
    char *supported_features
);
void OpenAPI_ue_n1_n2_info_subscription_created_data_free(OpenAPI_ue_n1_n2_info_subscription_created_data_t *ue_n1_n2_info_subscription_created_data);
OpenAPI_ue_n1_n2_info_subscription_created_data_t *OpenAPI_ue_n1_n2_info_subscription_created_data_parseFromJSON(cJSON *ue_n1_n2_info_subscription_created_dataJSON);
cJSON *OpenAPI_ue_n1_n2_info_subscription_created_data_convertToJSON(OpenAPI_ue_n1_n2_info_subscription_created_data_t *ue_n1_n2_info_subscription_created_data);
OpenAPI_ue_n1_n2_info_subscription_created_data_t *OpenAPI_ue_n1_n2_info_subscription_created_data_copy(OpenAPI_ue_n1_n2_info_subscription_created_data_t *dst, OpenAPI_ue_n1_n2_info_subscription_created_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_n1_n2_info_subscription_created_data_H_ */

