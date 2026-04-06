/*
 * pdu_session_created_data.h
 *
 * Data within Create Response
 */

#ifndef _OpenAPI_pdu_session_created_data_H_
#define _OpenAPI_pdu_session_created_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pdu_session_created_data_s OpenAPI_pdu_session_created_data_t;
#include "ambr.h"
#include "eps_bearer_info.h"
#include "eps_pdn_cnx_info.h"
#include "hrsbo_info_from_hplmn.h"
#include "ip_address.h"
#include "local_offloading_mgt_info_to_ismf.h"
#include "max_integrity_protected_data_rate.h"
#include "pdu_session_type.h"
#include "pending_update_info.h"
#include "qos_flow_setup_item.h"
#include "redundant_pdu_session_information.h"
#include "ref_to_binary_data.h"
#include "roaming_charging_profile.h"
#include "snssai.h"
#include "tunnel_info.h"
#include "uli_change_granularity.h"
#include "up_security.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pdu_session_created_data_s {
    OpenAPI_pdu_session_type_e pdu_session_type;
    char *ssc_mode;
    struct OpenAPI_tunnel_info_s *hcn_tunnel_info;
    struct OpenAPI_tunnel_info_s *cn_tunnel_info;
    struct OpenAPI_tunnel_info_s *additional_cn_tunnel_info;
    struct OpenAPI_ambr_s *session_ambr;
    OpenAPI_list_t *qos_flows_setup_list;
    char *h_smf_instance_id;
    char *smf_instance_id;
    bool is_pdu_session_id;
    int pdu_session_id;
    struct OpenAPI_snssai_s *s_nssai;
    struct OpenAPI_snssai_s *additional_snssai;
    bool is_enable_pause_charging;
    int enable_pause_charging;
    char *ue_ipv4_address;
    char *ue_ipv6_prefix;
    struct OpenAPI_ref_to_binary_data_s *n1_sm_info_to_ue;
    struct OpenAPI_eps_pdn_cnx_info_s *eps_pdn_cnx_info;
    OpenAPI_list_t *eps_bearer_info;
    char *supported_features;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl;
    bool is_always_on_granted;
    int always_on_granted;
    char *gpsi;
    struct OpenAPI_up_security_s *up_security;
    struct OpenAPI_roaming_charging_profile_s *roaming_charging_profile;
    char *h_smf_service_instance_id;
    char *smf_service_instance_id;
    char *recovery_time;
    OpenAPI_list_t *dnai_list;
    bool is_ipv6_multi_homing_ind;
    int ipv6_multi_homing_ind;
    bool is_ma_accepted_ind;
    int ma_accepted_ind;
    char *home_provided_charging_id;
    char *home_provided_smf_charging_id;
    bool is_nef_ext_buf_support_ind;
    int nef_ext_buf_support_ind;
    bool is_small_data_rate_control_enabled;
    int small_data_rate_control_enabled;
    char *ue_ipv6_interface_id;
    bool is_ipv6_index;
    int ipv6_index;
    struct OpenAPI_ip_address_s *dn_aaa_address;
    struct OpenAPI_redundant_pdu_session_information_s *redundant_pdu_session_info;
    bool is_nspu_support_ind;
    int nspu_support_ind;
    char *inter_plmn_api_root;
    char *intra_plmn_api_root;
    char *udm_group_id;
    char *pcf_group_id;
    struct OpenAPI_hrsbo_info_from_hplmn_s *hrsbo_info;
    struct OpenAPI_local_offloading_mgt_info_to_ismf_s *local_offload_mgt_info;
    OpenAPI_list_t *pending_update_info_list;
    OpenAPI_uli_change_granularity_e uli_change_granularity;
};

OpenAPI_pdu_session_created_data_t *OpenAPI_pdu_session_created_data_create(
    OpenAPI_pdu_session_type_e pdu_session_type,
    char *ssc_mode,
    OpenAPI_tunnel_info_t *hcn_tunnel_info,
    OpenAPI_tunnel_info_t *cn_tunnel_info,
    OpenAPI_tunnel_info_t *additional_cn_tunnel_info,
    OpenAPI_ambr_t *session_ambr,
    OpenAPI_list_t *qos_flows_setup_list,
    char *h_smf_instance_id,
    char *smf_instance_id,
    bool is_pdu_session_id,
    int pdu_session_id,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_snssai_t *additional_snssai,
    bool is_enable_pause_charging,
    int enable_pause_charging,
    char *ue_ipv4_address,
    char *ue_ipv6_prefix,
    OpenAPI_ref_to_binary_data_t *n1_sm_info_to_ue,
    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info,
    OpenAPI_list_t *eps_bearer_info,
    char *supported_features,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl,
    bool is_always_on_granted,
    int always_on_granted,
    char *gpsi,
    OpenAPI_up_security_t *up_security,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile,
    char *h_smf_service_instance_id,
    char *smf_service_instance_id,
    char *recovery_time,
    OpenAPI_list_t *dnai_list,
    bool is_ipv6_multi_homing_ind,
    int ipv6_multi_homing_ind,
    bool is_ma_accepted_ind,
    int ma_accepted_ind,
    char *home_provided_charging_id,
    char *home_provided_smf_charging_id,
    bool is_nef_ext_buf_support_ind,
    int nef_ext_buf_support_ind,
    bool is_small_data_rate_control_enabled,
    int small_data_rate_control_enabled,
    char *ue_ipv6_interface_id,
    bool is_ipv6_index,
    int ipv6_index,
    OpenAPI_ip_address_t *dn_aaa_address,
    OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_info,
    bool is_nspu_support_ind,
    int nspu_support_ind,
    char *inter_plmn_api_root,
    char *intra_plmn_api_root,
    char *udm_group_id,
    char *pcf_group_id,
    OpenAPI_hrsbo_info_from_hplmn_t *hrsbo_info,
    OpenAPI_local_offloading_mgt_info_to_ismf_t *local_offload_mgt_info,
    OpenAPI_list_t *pending_update_info_list,
    OpenAPI_uli_change_granularity_e uli_change_granularity
);
void OpenAPI_pdu_session_created_data_free(OpenAPI_pdu_session_created_data_t *pdu_session_created_data);
OpenAPI_pdu_session_created_data_t *OpenAPI_pdu_session_created_data_parseFromJSON(cJSON *pdu_session_created_dataJSON);
cJSON *OpenAPI_pdu_session_created_data_convertToJSON(OpenAPI_pdu_session_created_data_t *pdu_session_created_data);
OpenAPI_pdu_session_created_data_t *OpenAPI_pdu_session_created_data_copy(OpenAPI_pdu_session_created_data_t *dst, OpenAPI_pdu_session_created_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_created_data_H_ */

