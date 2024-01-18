/*
 * policy_update.h
 *
 * Represents updated policies that the PCF provides in a notification or in a reply to an Update Request. 
 */

#ifndef _OpenAPI_policy_update_H_
#define _OpenAPI_policy_update_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ambr.h"
#include "as_time_distribution_param.h"
#include "pcf_ue_callback_info.h"
#include "pdu_session_info.h"
#include "presence_info_rm.h"
#include "request_trigger.h"
#include "service_area_restriction.h"
#include "smf_selection_data.h"
#include "ue_slice_mbr.h"
#include "wireline_service_area_restriction.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_policy_update_s OpenAPI_policy_update_t;
typedef struct OpenAPI_policy_update_s {
    char *resource_uri;
    bool is_triggers_null;
    OpenAPI_list_t *triggers;
    struct OpenAPI_service_area_restriction_s *serv_area_res;
    struct OpenAPI_wireline_service_area_restriction_s *wl_serv_area_res;
    bool is_rfsp;
    int rfsp;
    bool is_target_rfsp;
    int target_rfsp;
    bool is_smf_sel_info_null;
    struct OpenAPI_smf_selection_data_s *smf_sel_info;
    struct OpenAPI_ambr_s *ue_ambr;
    OpenAPI_list_t *ue_slice_mbrs;
    bool is_pras_null;
    OpenAPI_list_t* pras;
    bool is_pcf_ue_info_null;
    struct OpenAPI_pcf_ue_callback_info_s *pcf_ue_info;
    bool is_match_pdus_null;
    OpenAPI_list_t *match_pdus;
    bool is_as_time_dis_param_null;
    struct OpenAPI_as_time_distribution_param_s *as_time_dis_param;
} OpenAPI_policy_update_t;

OpenAPI_policy_update_t *OpenAPI_policy_update_create(
    char *resource_uri,
    bool is_triggers_null,
    OpenAPI_list_t *triggers,
    OpenAPI_service_area_restriction_t *serv_area_res,
    OpenAPI_wireline_service_area_restriction_t *wl_serv_area_res,
    bool is_rfsp,
    int rfsp,
    bool is_target_rfsp,
    int target_rfsp,
    bool is_smf_sel_info_null,
    OpenAPI_smf_selection_data_t *smf_sel_info,
    OpenAPI_ambr_t *ue_ambr,
    OpenAPI_list_t *ue_slice_mbrs,
    bool is_pras_null,
    OpenAPI_list_t* pras,
    bool is_pcf_ue_info_null,
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_info,
    bool is_match_pdus_null,
    OpenAPI_list_t *match_pdus,
    bool is_as_time_dis_param_null,
    OpenAPI_as_time_distribution_param_t *as_time_dis_param
);
void OpenAPI_policy_update_free(OpenAPI_policy_update_t *policy_update);
OpenAPI_policy_update_t *OpenAPI_policy_update_parseFromJSON(cJSON *policy_updateJSON);
cJSON *OpenAPI_policy_update_convertToJSON(OpenAPI_policy_update_t *policy_update);
OpenAPI_policy_update_t *OpenAPI_policy_update_copy(OpenAPI_policy_update_t *dst, OpenAPI_policy_update_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_update_H_ */

