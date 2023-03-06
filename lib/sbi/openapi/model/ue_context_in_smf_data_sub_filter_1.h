/*
 * ue_context_in_smf_data_sub_filter_1.h
 *
 * UE Context In Smf Data Subscription Filter.
 */

#ifndef _OpenAPI_ue_context_in_smf_data_sub_filter_1_H_
#define _OpenAPI_ue_context_in_smf_data_sub_filter_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_in_smf_data_sub_filter_1_s OpenAPI_ue_context_in_smf_data_sub_filter_1_t;
typedef struct OpenAPI_ue_context_in_smf_data_sub_filter_1_s {
    OpenAPI_list_t *dnn_list;
    OpenAPI_list_t *snssai_list;
    bool is_emergency_ind;
    int emergency_ind;
} OpenAPI_ue_context_in_smf_data_sub_filter_1_t;

OpenAPI_ue_context_in_smf_data_sub_filter_1_t *OpenAPI_ue_context_in_smf_data_sub_filter_1_create(
    OpenAPI_list_t *dnn_list,
    OpenAPI_list_t *snssai_list,
    bool is_emergency_ind,
    int emergency_ind
);
void OpenAPI_ue_context_in_smf_data_sub_filter_1_free(OpenAPI_ue_context_in_smf_data_sub_filter_1_t *ue_context_in_smf_data_sub_filter_1);
OpenAPI_ue_context_in_smf_data_sub_filter_1_t *OpenAPI_ue_context_in_smf_data_sub_filter_1_parseFromJSON(cJSON *ue_context_in_smf_data_sub_filter_1JSON);
cJSON *OpenAPI_ue_context_in_smf_data_sub_filter_1_convertToJSON(OpenAPI_ue_context_in_smf_data_sub_filter_1_t *ue_context_in_smf_data_sub_filter_1);
OpenAPI_ue_context_in_smf_data_sub_filter_1_t *OpenAPI_ue_context_in_smf_data_sub_filter_1_copy(OpenAPI_ue_context_in_smf_data_sub_filter_1_t *dst, OpenAPI_ue_context_in_smf_data_sub_filter_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_in_smf_data_sub_filter_1_H_ */

