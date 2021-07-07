/*
 * smf_selection_subscription_data.h
 *
 * 
 */

#ifndef _OpenAPI_smf_selection_subscription_data_H_
#define _OpenAPI_smf_selection_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_smf_selection_subscription_data_s OpenAPI_smf_selection_subscription_data_t;
typedef struct OpenAPI_smf_selection_subscription_data_s {
    char *supported_features;
    OpenAPI_list_t* subscribed_snssai_infos;
    char *shared_snssai_infos_id;
} OpenAPI_smf_selection_subscription_data_t;

OpenAPI_smf_selection_subscription_data_t *OpenAPI_smf_selection_subscription_data_create(
    char *supported_features,
    OpenAPI_list_t* subscribed_snssai_infos,
    char *shared_snssai_infos_id
);
void OpenAPI_smf_selection_subscription_data_free(OpenAPI_smf_selection_subscription_data_t *smf_selection_subscription_data);
OpenAPI_smf_selection_subscription_data_t *OpenAPI_smf_selection_subscription_data_parseFromJSON(cJSON *smf_selection_subscription_dataJSON);
cJSON *OpenAPI_smf_selection_subscription_data_convertToJSON(OpenAPI_smf_selection_subscription_data_t *smf_selection_subscription_data);
OpenAPI_smf_selection_subscription_data_t *OpenAPI_smf_selection_subscription_data_copy(OpenAPI_smf_selection_subscription_data_t *dst, OpenAPI_smf_selection_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smf_selection_subscription_data_H_ */

