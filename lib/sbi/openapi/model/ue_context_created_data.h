/*
 * ue_context_created_data.h
 *
 * Data within a successful response for creating an individual ueContext resource
 */

#ifndef _OpenAPI_ue_context_created_data_H_
#define _OpenAPI_ue_context_created_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_info_content.h"
#include "n2_sm_information.h"
#include "ue_context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_created_data_s OpenAPI_ue_context_created_data_t;
typedef struct OpenAPI_ue_context_created_data_s {
    struct OpenAPI_ue_context_s *ue_context;
    struct OpenAPI_n2_info_content_s *target_to_source_data;
    OpenAPI_list_t *pdu_session_list;
    OpenAPI_list_t *failed_session_list;
    char *supported_features;
    bool is_pcf_reselected_ind;
    int pcf_reselected_ind;
    OpenAPI_list_t *analytics_not_used_list;
} OpenAPI_ue_context_created_data_t;

OpenAPI_ue_context_created_data_t *OpenAPI_ue_context_created_data_create(
    OpenAPI_ue_context_t *ue_context,
    OpenAPI_n2_info_content_t *target_to_source_data,
    OpenAPI_list_t *pdu_session_list,
    OpenAPI_list_t *failed_session_list,
    char *supported_features,
    bool is_pcf_reselected_ind,
    int pcf_reselected_ind,
    OpenAPI_list_t *analytics_not_used_list
);
void OpenAPI_ue_context_created_data_free(OpenAPI_ue_context_created_data_t *ue_context_created_data);
OpenAPI_ue_context_created_data_t *OpenAPI_ue_context_created_data_parseFromJSON(cJSON *ue_context_created_dataJSON);
cJSON *OpenAPI_ue_context_created_data_convertToJSON(OpenAPI_ue_context_created_data_t *ue_context_created_data);
OpenAPI_ue_context_created_data_t *OpenAPI_ue_context_created_data_copy(OpenAPI_ue_context_created_data_t *dst, OpenAPI_ue_context_created_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_created_data_H_ */

