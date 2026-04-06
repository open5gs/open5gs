/*
 * ue_policy_set.h
 *
 * Contains the UE policy data for a given subscriber.
 */

#ifndef _OpenAPI_ue_policy_set_H_
#define _OpenAPI_ue_policy_set_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ue_policy_set_s OpenAPI_ue_policy_set_t;
#include "charging_information.h"
#include "plmn_route_selection_descriptor.h"
#include "policy_counter_info.h"
#include "presence_info.h"
#include "restricted_status.h"
#include "trace_data.h"
#include "ue_policy_section.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ue_policy_set_s {
    OpenAPI_list_t* pra_infos;
    OpenAPI_list_t *subsc_cats;
    OpenAPI_list_t* ue_policy_sections;
    OpenAPI_list_t *upsis;
    OpenAPI_list_t* allowed_route_sel_descs;
    bool is_andsp_ind;
    int andsp_ind;
    bool is_eps_ursp_ind;
    int eps_ursp_ind;
    bool is_vps_ursp_ind;
    int vps_ursp_ind;
    bool is_ursp_enf_ind;
    int ursp_enf_ind;
    char *pei;
    OpenAPI_list_t *os_ids;
    struct OpenAPI_charging_information_s *chf_info;
    char *chf_group_id;
    bool is_subsc_spending_limits;
    int subsc_spending_limits;
    OpenAPI_list_t* spend_lim_info;
    bool is_tracing_req_null;
    struct OpenAPI_trace_data_s *tracing_req;
    OpenAPI_list_t *reset_ids;
    OpenAPI_list_t *restri_status;
    char *supp_feat;
};

OpenAPI_ue_policy_set_t *OpenAPI_ue_policy_set_create(
    OpenAPI_list_t* pra_infos,
    OpenAPI_list_t *subsc_cats,
    OpenAPI_list_t* ue_policy_sections,
    OpenAPI_list_t *upsis,
    OpenAPI_list_t* allowed_route_sel_descs,
    bool is_andsp_ind,
    int andsp_ind,
    bool is_eps_ursp_ind,
    int eps_ursp_ind,
    bool is_vps_ursp_ind,
    int vps_ursp_ind,
    bool is_ursp_enf_ind,
    int ursp_enf_ind,
    char *pei,
    OpenAPI_list_t *os_ids,
    OpenAPI_charging_information_t *chf_info,
    char *chf_group_id,
    bool is_subsc_spending_limits,
    int subsc_spending_limits,
    OpenAPI_list_t* spend_lim_info,
    bool is_tracing_req_null,
    OpenAPI_trace_data_t *tracing_req,
    OpenAPI_list_t *reset_ids,
    OpenAPI_list_t *restri_status,
    char *supp_feat
);
void OpenAPI_ue_policy_set_free(OpenAPI_ue_policy_set_t *ue_policy_set);
OpenAPI_ue_policy_set_t *OpenAPI_ue_policy_set_parseFromJSON(cJSON *ue_policy_setJSON);
cJSON *OpenAPI_ue_policy_set_convertToJSON(OpenAPI_ue_policy_set_t *ue_policy_set);
OpenAPI_ue_policy_set_t *OpenAPI_ue_policy_set_copy(OpenAPI_ue_policy_set_t *dst, OpenAPI_ue_policy_set_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_policy_set_H_ */

