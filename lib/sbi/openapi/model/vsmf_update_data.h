/*
 * vsmf_update_data.h
 *
 * 
 */

#ifndef _OpenAPI_vsmf_update_data_H_
#define _OpenAPI_vsmf_update_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ambr.h"
#include "arp.h"
#include "cause.h"
#include "ebi_arp_mapping.h"
#include "eps_bearer_info.h"
#include "ma_release_indication.h"
#include "n4_information.h"
#include "qos_flow_add_modify_request_item.h"
#include "qos_flow_release_request_item.h"
#include "qos_monitoring_info.h"
#include "ref_to_binary_data.h"
#include "request_indication.h"
#include "tunnel_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_vsmf_update_data_s OpenAPI_vsmf_update_data_t;
typedef struct OpenAPI_vsmf_update_data_s {
    OpenAPI_request_indication_e request_indication;
    struct OpenAPI_ambr_s *session_ambr;
    OpenAPI_list_t *qos_flows_add_mod_request_list;
    OpenAPI_list_t *qos_flows_rel_request_list;
    OpenAPI_list_t *eps_bearer_info;
    OpenAPI_list_t *assign_ebi_list;
    OpenAPI_list_t *revoke_ebi_list;
    OpenAPI_list_t *modified_ebi_list;
    bool is_pti;
    int pti;
    struct OpenAPI_ref_to_binary_data_s *n1_sm_info_to_ue;
    bool is_always_on_granted;
    int always_on_granted;
    char *hsmf_pdu_session_uri;
    char *supported_features;
    OpenAPI_cause_e cause;
    char *n1sm_cause;
    bool is_back_off_timer;
    int back_off_timer;
    OpenAPI_ma_release_indication_e ma_release_ind;
    bool is_ma_accepted_ind;
    int ma_accepted_ind;
    struct OpenAPI_tunnel_info_s *additional_cn_tunnel_info;
    OpenAPI_list_t *dnai_list;
    struct OpenAPI_n4_information_s *n4_info;
    struct OpenAPI_n4_information_s *n4_info_ext1;
    struct OpenAPI_n4_information_s *n4_info_ext2;
    bool is_small_data_rate_control_enabled;
    int small_data_rate_control_enabled;
    struct OpenAPI_qos_monitoring_info_s *qos_monitoring_info;
} OpenAPI_vsmf_update_data_t;

OpenAPI_vsmf_update_data_t *OpenAPI_vsmf_update_data_create(
    OpenAPI_request_indication_e request_indication,
    OpenAPI_ambr_t *session_ambr,
    OpenAPI_list_t *qos_flows_add_mod_request_list,
    OpenAPI_list_t *qos_flows_rel_request_list,
    OpenAPI_list_t *eps_bearer_info,
    OpenAPI_list_t *assign_ebi_list,
    OpenAPI_list_t *revoke_ebi_list,
    OpenAPI_list_t *modified_ebi_list,
    bool is_pti,
    int pti,
    OpenAPI_ref_to_binary_data_t *n1_sm_info_to_ue,
    bool is_always_on_granted,
    int always_on_granted,
    char *hsmf_pdu_session_uri,
    char *supported_features,
    OpenAPI_cause_e cause,
    char *n1sm_cause,
    bool is_back_off_timer,
    int back_off_timer,
    OpenAPI_ma_release_indication_e ma_release_ind,
    bool is_ma_accepted_ind,
    int ma_accepted_ind,
    OpenAPI_tunnel_info_t *additional_cn_tunnel_info,
    OpenAPI_list_t *dnai_list,
    OpenAPI_n4_information_t *n4_info,
    OpenAPI_n4_information_t *n4_info_ext1,
    OpenAPI_n4_information_t *n4_info_ext2,
    bool is_small_data_rate_control_enabled,
    int small_data_rate_control_enabled,
    OpenAPI_qos_monitoring_info_t *qos_monitoring_info
);
void OpenAPI_vsmf_update_data_free(OpenAPI_vsmf_update_data_t *vsmf_update_data);
OpenAPI_vsmf_update_data_t *OpenAPI_vsmf_update_data_parseFromJSON(cJSON *vsmf_update_dataJSON);
cJSON *OpenAPI_vsmf_update_data_convertToJSON(OpenAPI_vsmf_update_data_t *vsmf_update_data);
OpenAPI_vsmf_update_data_t *OpenAPI_vsmf_update_data_copy(OpenAPI_vsmf_update_data_t *dst, OpenAPI_vsmf_update_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vsmf_update_data_H_ */

