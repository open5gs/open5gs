/*
 * smf_selection_subscription_data_1.h
 *
 * Contains SMF selection subscription data
 */

#ifndef _OpenAPI_smf_selection_subscription_data_1_H_
#define _OpenAPI_smf_selection_subscription_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_smf_selection_subscription_data_1_s OpenAPI_smf_selection_subscription_data_1_t;
#include "snssai_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_smf_selection_subscription_data_1_s {
    char *supported_features;
    OpenAPI_list_t* subscribed_snssai_infos;
    char *shared_snssai_infos_id;
    char *hss_group_id;
    OpenAPI_list_t *additional_shared_snssai_infos_ids;
};

OpenAPI_smf_selection_subscription_data_1_t *OpenAPI_smf_selection_subscription_data_1_create(
    char *supported_features,
    OpenAPI_list_t* subscribed_snssai_infos,
    char *shared_snssai_infos_id,
    char *hss_group_id,
    OpenAPI_list_t *additional_shared_snssai_infos_ids
);
void OpenAPI_smf_selection_subscription_data_1_free(OpenAPI_smf_selection_subscription_data_1_t *smf_selection_subscription_data_1);
OpenAPI_smf_selection_subscription_data_1_t *OpenAPI_smf_selection_subscription_data_1_parseFromJSON(cJSON *smf_selection_subscription_data_1JSON);
cJSON *OpenAPI_smf_selection_subscription_data_1_convertToJSON(OpenAPI_smf_selection_subscription_data_1_t *smf_selection_subscription_data_1);
OpenAPI_smf_selection_subscription_data_1_t *OpenAPI_smf_selection_subscription_data_1_copy(OpenAPI_smf_selection_subscription_data_1_t *dst, OpenAPI_smf_selection_subscription_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smf_selection_subscription_data_1_H_ */

