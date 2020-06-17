/*
 * non_ue_n2_info_subscription_create_data.h
 *
 *
 */

#ifndef _OpenAPI_non_ue_n2_info_subscription_create_data_H_
#define _OpenAPI_non_ue_n2_info_subscription_create_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "global_ran_node_id.h"
#include "n2_information_class.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_non_ue_n2_info_subscription_create_data_s OpenAPI_non_ue_n2_info_subscription_create_data_t;
typedef struct OpenAPI_non_ue_n2_info_subscription_create_data_s {
    OpenAPI_list_t *global_ran_node_list;
    OpenAPI_list_t *an_type_list;
    OpenAPI_n2_information_class_e n2_information_class;
    char *n2_notify_callback_uri;
    char *nf_id;
    char *supported_features;
} OpenAPI_non_ue_n2_info_subscription_create_data_t;

OpenAPI_non_ue_n2_info_subscription_create_data_t *OpenAPI_non_ue_n2_info_subscription_create_data_create(
    OpenAPI_list_t *global_ran_node_list,
    OpenAPI_list_t *an_type_list,
    OpenAPI_n2_information_class_e n2_information_class,
    char *n2_notify_callback_uri,
    char *nf_id,
    char *supported_features
    );
void OpenAPI_non_ue_n2_info_subscription_create_data_free(OpenAPI_non_ue_n2_info_subscription_create_data_t *non_ue_n2_info_subscription_create_data);
OpenAPI_non_ue_n2_info_subscription_create_data_t *OpenAPI_non_ue_n2_info_subscription_create_data_parseFromJSON(cJSON *non_ue_n2_info_subscription_create_dataJSON);
cJSON *OpenAPI_non_ue_n2_info_subscription_create_data_convertToJSON(OpenAPI_non_ue_n2_info_subscription_create_data_t *non_ue_n2_info_subscription_create_data);
OpenAPI_non_ue_n2_info_subscription_create_data_t *OpenAPI_non_ue_n2_info_subscription_create_data_copy(OpenAPI_non_ue_n2_info_subscription_create_data_t *dst, OpenAPI_non_ue_n2_info_subscription_create_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_non_ue_n2_info_subscription_create_data_H_ */

