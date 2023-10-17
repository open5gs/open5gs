/*
 * ue_context_create_data.h
 *
 * Data within a request to create an individual ueContext resource
 */

#ifndef _OpenAPI_ue_context_create_data_H_
#define _OpenAPI_ue_context_create_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_info_content.h"
#include "n2_sm_information.h"
#include "ng_ap_cause.h"
#include "ng_ran_target_id.h"
#include "plmn_id_nid.h"
#include "ue_context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_create_data_s OpenAPI_ue_context_create_data_t;
typedef struct OpenAPI_ue_context_create_data_s {
    struct OpenAPI_ue_context_s *ue_context;
    struct OpenAPI_ng_ran_target_id_s *target_id;
    struct OpenAPI_n2_info_content_s *source_to_target_data;
    OpenAPI_list_t *pdu_session_list;
    char *n2_notify_uri;
    struct OpenAPI_n2_info_content_s *ue_radio_capability;
    struct OpenAPI_n2_info_content_s *ue_radio_capability_for_paging;
    struct OpenAPI_ng_ap_cause_s *ngap_cause;
    char *supported_features;
    struct OpenAPI_plmn_id_nid_s *serving_network;
} OpenAPI_ue_context_create_data_t;

OpenAPI_ue_context_create_data_t *OpenAPI_ue_context_create_data_create(
    OpenAPI_ue_context_t *ue_context,
    OpenAPI_ng_ran_target_id_t *target_id,
    OpenAPI_n2_info_content_t *source_to_target_data,
    OpenAPI_list_t *pdu_session_list,
    char *n2_notify_uri,
    OpenAPI_n2_info_content_t *ue_radio_capability,
    OpenAPI_n2_info_content_t *ue_radio_capability_for_paging,
    OpenAPI_ng_ap_cause_t *ngap_cause,
    char *supported_features,
    OpenAPI_plmn_id_nid_t *serving_network
);
void OpenAPI_ue_context_create_data_free(OpenAPI_ue_context_create_data_t *ue_context_create_data);
OpenAPI_ue_context_create_data_t *OpenAPI_ue_context_create_data_parseFromJSON(cJSON *ue_context_create_dataJSON);
cJSON *OpenAPI_ue_context_create_data_convertToJSON(OpenAPI_ue_context_create_data_t *ue_context_create_data);
OpenAPI_ue_context_create_data_t *OpenAPI_ue_context_create_data_copy(OpenAPI_ue_context_create_data_t *dst, OpenAPI_ue_context_create_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_create_data_H_ */

