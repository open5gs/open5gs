/*
 * registration_context_container.h
 *
 * 
 */

#ifndef _OpenAPI_registration_context_container_H_
#define _OpenAPI_registration_context_container_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "allowed_nssai.h"
#include "ce_mode_b_ind.h"
#include "configured_snssai.h"
#include "global_ran_node_id.h"
#include "lte_m_ind.h"
#include "npn_access_info.h"
#include "plmn_id.h"
#include "snssai.h"
#include "ue_context.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_registration_context_container_s OpenAPI_registration_context_container_t;
typedef struct OpenAPI_registration_context_container_s {
    struct OpenAPI_ue_context_s *ue_context;
    char *local_time_zone;
    OpenAPI_access_type_e an_type;
    int an_n2_ap_id;
    struct OpenAPI_global_ran_node_id_s *ran_node_id;
    char *initial_amf_name;
    struct OpenAPI_user_location_s *user_location;
    char *rrc_est_cause;
    bool is_ue_context_request;
    int ue_context_request;
    bool is_initial_amf_n2_ap_id;
    int initial_amf_n2_ap_id;
    char *an_n2_ipv4_addr;
    char *an_n2_ipv6_addr;
    struct OpenAPI_allowed_nssai_s *allowed_nssai;
    OpenAPI_list_t *configured_nssai;
    OpenAPI_list_t *rejected_nssai_in_plmn;
    OpenAPI_list_t *rejected_nssai_in_ta;
    struct OpenAPI_plmn_id_s *selected_plmn_id;
    bool is_iab_node_ind;
    int iab_node_ind;
    struct OpenAPI_ce_mode_b_ind_s *ce_mode_b_ind;
    struct OpenAPI_lte_m_ind_s *lte_m_ind;
    bool is_authenticated_ind;
    int authenticated_ind;
    struct OpenAPI_npn_access_info_s *npn_access_info;
} OpenAPI_registration_context_container_t;

OpenAPI_registration_context_container_t *OpenAPI_registration_context_container_create(
    OpenAPI_ue_context_t *ue_context,
    char *local_time_zone,
    OpenAPI_access_type_e an_type,
    int an_n2_ap_id,
    OpenAPI_global_ran_node_id_t *ran_node_id,
    char *initial_amf_name,
    OpenAPI_user_location_t *user_location,
    char *rrc_est_cause,
    bool is_ue_context_request,
    int ue_context_request,
    bool is_initial_amf_n2_ap_id,
    int initial_amf_n2_ap_id,
    char *an_n2_ipv4_addr,
    char *an_n2_ipv6_addr,
    OpenAPI_allowed_nssai_t *allowed_nssai,
    OpenAPI_list_t *configured_nssai,
    OpenAPI_list_t *rejected_nssai_in_plmn,
    OpenAPI_list_t *rejected_nssai_in_ta,
    OpenAPI_plmn_id_t *selected_plmn_id,
    bool is_iab_node_ind,
    int iab_node_ind,
    OpenAPI_ce_mode_b_ind_t *ce_mode_b_ind,
    OpenAPI_lte_m_ind_t *lte_m_ind,
    bool is_authenticated_ind,
    int authenticated_ind,
    OpenAPI_npn_access_info_t *npn_access_info
);
void OpenAPI_registration_context_container_free(OpenAPI_registration_context_container_t *registration_context_container);
OpenAPI_registration_context_container_t *OpenAPI_registration_context_container_parseFromJSON(cJSON *registration_context_containerJSON);
cJSON *OpenAPI_registration_context_container_convertToJSON(OpenAPI_registration_context_container_t *registration_context_container);
OpenAPI_registration_context_container_t *OpenAPI_registration_context_container_copy(OpenAPI_registration_context_container_t *dst, OpenAPI_registration_context_container_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_registration_context_container_H_ */

