/*
 * mm_context.h
 *
 * 
 */

#ifndef _OpenAPI_mm_context_H_
#define _OpenAPI_mm_context_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "eps_nas_security_mode.h"
#include "expected_ue_behavior.h"
#include "global_ran_node_id.h"
#include "nas_security_mode.h"
#include "nssaa_status.h"
#include "nssai_mapping.h"
#include "snssai.h"
#include "ue_differentiation_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mm_context_s OpenAPI_mm_context_t;
typedef struct OpenAPI_mm_context_s {
    OpenAPI_access_type_e access_type;
    struct OpenAPI_nas_security_mode_s *nas_security_mode;
    struct OpenAPI_eps_nas_security_mode_s *eps_nas_security_mode;
    bool is_nas_downlink_count;
    int nas_downlink_count;
    bool is_nas_uplink_count;
    int nas_uplink_count;
    char ue_security_capability;
    char s1_ue_network_capability;
    OpenAPI_list_t *allowed_nssai;
    OpenAPI_list_t *nssai_mapping_list;
    OpenAPI_list_t *allowed_home_nssai;
    OpenAPI_list_t *ns_instance_list;
    struct OpenAPI_expected_ue_behavior_s *expected_u_ebehavior;
    struct OpenAPI_ue_differentiation_info_s *ue_differentiation_info;
    char plmn_assi_ue_radio_cap_id;
    char man_assi_ue_radio_cap_id;
    char *ucmf_dic_entry_id;
    struct OpenAPI_global_ran_node_id_s *n3_iwf_id;
    struct OpenAPI_global_ran_node_id_s *wagf_id;
    struct OpenAPI_global_ran_node_id_s *tngf_id;
    bool is_an_n2_ap_id;
    int an_n2_ap_id;
    OpenAPI_list_t *nssaa_status_list;
    OpenAPI_list_t *pending_nssai_mapping_list;
} OpenAPI_mm_context_t;

OpenAPI_mm_context_t *OpenAPI_mm_context_create(
    OpenAPI_access_type_e access_type,
    OpenAPI_nas_security_mode_t *nas_security_mode,
    OpenAPI_eps_nas_security_mode_t *eps_nas_security_mode,
    bool is_nas_downlink_count,
    int nas_downlink_count,
    bool is_nas_uplink_count,
    int nas_uplink_count,
    char ue_security_capability,
    char s1_ue_network_capability,
    OpenAPI_list_t *allowed_nssai,
    OpenAPI_list_t *nssai_mapping_list,
    OpenAPI_list_t *allowed_home_nssai,
    OpenAPI_list_t *ns_instance_list,
    OpenAPI_expected_ue_behavior_t *expected_u_ebehavior,
    OpenAPI_ue_differentiation_info_t *ue_differentiation_info,
    char plmn_assi_ue_radio_cap_id,
    char man_assi_ue_radio_cap_id,
    char *ucmf_dic_entry_id,
    OpenAPI_global_ran_node_id_t *n3_iwf_id,
    OpenAPI_global_ran_node_id_t *wagf_id,
    OpenAPI_global_ran_node_id_t *tngf_id,
    bool is_an_n2_ap_id,
    int an_n2_ap_id,
    OpenAPI_list_t *nssaa_status_list,
    OpenAPI_list_t *pending_nssai_mapping_list
);
void OpenAPI_mm_context_free(OpenAPI_mm_context_t *mm_context);
OpenAPI_mm_context_t *OpenAPI_mm_context_parseFromJSON(cJSON *mm_contextJSON);
cJSON *OpenAPI_mm_context_convertToJSON(OpenAPI_mm_context_t *mm_context);
OpenAPI_mm_context_t *OpenAPI_mm_context_copy(OpenAPI_mm_context_t *dst, OpenAPI_mm_context_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mm_context_H_ */

