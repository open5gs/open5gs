/*
 * ue_context_relocate_data.h
 *
 * Data within a Relocate UE Context request
 */

#ifndef _OpenAPI_ue_context_relocate_data_H_
#define _OpenAPI_ue_context_relocate_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_info_content.h"
#include "n2_sm_information.h"
#include "ng_ap_cause.h"
#include "ng_ran_target_id.h"
#include "ref_to_binary_data.h"
#include "ue_context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_relocate_data_s OpenAPI_ue_context_relocate_data_t;
typedef struct OpenAPI_ue_context_relocate_data_s {
    struct OpenAPI_ue_context_s *ue_context;
    struct OpenAPI_ng_ran_target_id_s *target_id;
    struct OpenAPI_n2_info_content_s *source_to_target_data;
    struct OpenAPI_ref_to_binary_data_s *forward_relocation_request;
    OpenAPI_list_t *pdu_session_list;
    struct OpenAPI_n2_info_content_s *ue_radio_capability;
    struct OpenAPI_ng_ap_cause_s *ngap_cause;
    char *supported_features;
} OpenAPI_ue_context_relocate_data_t;

OpenAPI_ue_context_relocate_data_t *OpenAPI_ue_context_relocate_data_create(
    OpenAPI_ue_context_t *ue_context,
    OpenAPI_ng_ran_target_id_t *target_id,
    OpenAPI_n2_info_content_t *source_to_target_data,
    OpenAPI_ref_to_binary_data_t *forward_relocation_request,
    OpenAPI_list_t *pdu_session_list,
    OpenAPI_n2_info_content_t *ue_radio_capability,
    OpenAPI_ng_ap_cause_t *ngap_cause,
    char *supported_features
);
void OpenAPI_ue_context_relocate_data_free(OpenAPI_ue_context_relocate_data_t *ue_context_relocate_data);
OpenAPI_ue_context_relocate_data_t *OpenAPI_ue_context_relocate_data_parseFromJSON(cJSON *ue_context_relocate_dataJSON);
cJSON *OpenAPI_ue_context_relocate_data_convertToJSON(OpenAPI_ue_context_relocate_data_t *ue_context_relocate_data);
OpenAPI_ue_context_relocate_data_t *OpenAPI_ue_context_relocate_data_copy(OpenAPI_ue_context_relocate_data_t *dst, OpenAPI_ue_context_relocate_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_relocate_data_H_ */

