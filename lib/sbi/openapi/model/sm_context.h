/*
 * sm_context.h
 *
 * Complete SM Context
 */

#ifndef _OpenAPI_sm_context_H_
#define _OpenAPI_sm_context_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sm_context_s OpenAPI_sm_context_t;
#include "ambr.h"
#include "charging_information.h"
#include "dnn_selection_mode.h"
#include "eas_info_to_refresh.h"
#include "eps_bearer_info.h"
#include "eps_pdn_cnx_info.h"
#include "ip_address.h"
#include "local_offloading_management_info.h"
#include "max_integrity_protected_data_rate.h"
#include "pdu_session_type.h"
#include "pending_update_info.h"
#include "qos_flow_setup_item.h"
#include "qos_flow_tunnel.h"
#include "qos_monitoring_info.h"
#include "redundant_pdu_session_information.h"
#include "roaming_charging_profile.h"
#include "satellite_backhaul_category.h"
#include "sbi_binding_level.h"
#include "snssai.h"
#include "traffic_influence_info.h"
#include "tunnel_info.h"
#include "up_security.h"
#include "vplmn_dl_ambr.h"
#include "vplmn_offloading_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sm_context_s {
    int pdu_session_id;
    char *dnn;
    char *selected_dnn;
    struct OpenAPI_snssai_s *s_nssai;
    struct OpenAPI_snssai_s *alt_snssai;
    struct OpenAPI_snssai_s *hplmn_snssai;
    struct OpenAPI_snssai_s *alt_hplmn_snssai;
    OpenAPI_pdu_session_type_e pdu_session_type;
    char *gpsi;
    char *h_smf_uri;
    char *smf_uri;
    char *pdu_session_ref;
    char *inter_plmn_api_root;
    char *intra_plmn_api_root;
    char *pcf_id;
    char *pcf_group_id;
    char *pcf_set_id;
    OpenAPI_dnn_selection_mode_e sel_mode;
    char *udm_group_id;
    char *routing_indicator;
    bool is_h_nw_pub_key_id;
    int h_nw_pub_key_id;
    struct OpenAPI_ambr_s *session_ambr;
    OpenAPI_list_t *qos_flows_list;
    char *h_smf_instance_id;
    char *smf_instance_id;
    char *pdu_session_smf_set_id;
    char *pdu_session_smf_service_set_id;
    OpenAPI_sbi_binding_level_e pdu_session_smf_binding;
    bool is_enable_pause_charging;
    int enable_pause_charging;
    char *ue_ipv4_address;
    char *ue_ipv6_prefix;
    struct OpenAPI_eps_pdn_cnx_info_s *eps_pdn_cnx_info;
    OpenAPI_list_t *eps_bearer_info;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl;
    bool is_always_on_granted;
    int always_on_granted;
    struct OpenAPI_up_security_s *up_security;
    char *h_smf_service_instance_id;
    char *smf_service_instance_id;
    char *recovery_time;
    bool is_forwarding_ind;
    int forwarding_ind;
    struct OpenAPI_tunnel_info_s *psa_tunnel_info;
    char *charging_id;
    char *smf_charging_id;
    struct OpenAPI_charging_information_s *charging_info;
    struct OpenAPI_roaming_charging_profile_s *roaming_charging_profile;
    bool is_nef_ext_buf_support_ind;
    int nef_ext_buf_support_ind;
    bool is_ipv6_index;
    int ipv6_index;
    struct OpenAPI_ip_address_s *dn_aaa_address;
    struct OpenAPI_redundant_pdu_session_information_s *redundant_pdu_session_info;
    struct OpenAPI_qos_flow_tunnel_s *ran_tunnel_info;
    OpenAPI_list_t *add_ran_tunnel_info;
    struct OpenAPI_qos_flow_tunnel_s *red_ran_tunnel_info;
    OpenAPI_list_t *add_red_ran_tunnel_info;
    bool is_nspu_support_ind;
    int nspu_support_ind;
    char *smf_binding_info;
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_cat;
    char *ssc_mode;
    bool is_dlset_support_ind;
    int dlset_support_ind;
    bool is_n9fsc_support_ind;
    int n9fsc_support_ind;
    bool is_anchor_smf_oauth2_required;
    int anchor_smf_oauth2_required;
    OpenAPI_list_t *full_dnai_list;
    bool is_hrsbo_auth_req_ind;
    int hrsbo_auth_req_ind;
    struct OpenAPI_ip_address_s *h_dns_addr;
    struct OpenAPI_ip_address_s *h_plmn_addr;
    OpenAPI_list_t *vplmn_offloading_info_list;
    bool is_vplmn_dl_ambr_null;
    struct OpenAPI_vplmn_dl_ambr_s *vplmn_dl_ambr;
    OpenAPI_list_t *offload_ids;
    char *target_dnai;
    struct OpenAPI_traffic_influence_info_s *traffic_influ_info;
    OpenAPI_list_t *pending_update_info_list;
    struct OpenAPI_eas_info_to_refresh_s *eas_info_to_refresh;
    bool is_local_offloading_mgt_allowed_ind;
    int local_offloading_mgt_allowed_ind;
    struct OpenAPI_ip_address_s *dns_addr;
    struct OpenAPI_ip_address_s *psa_upf_addr;
    OpenAPI_list_t *local_offloading_info_list;
    bool is_priority_user_ind;
    int priority_user_ind;
    struct OpenAPI_qos_monitoring_info_s *qos_monitoring_info;
};

OpenAPI_sm_context_t *OpenAPI_sm_context_create(
    int pdu_session_id,
    char *dnn,
    char *selected_dnn,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_snssai_t *alt_snssai,
    OpenAPI_snssai_t *hplmn_snssai,
    OpenAPI_snssai_t *alt_hplmn_snssai,
    OpenAPI_pdu_session_type_e pdu_session_type,
    char *gpsi,
    char *h_smf_uri,
    char *smf_uri,
    char *pdu_session_ref,
    char *inter_plmn_api_root,
    char *intra_plmn_api_root,
    char *pcf_id,
    char *pcf_group_id,
    char *pcf_set_id,
    OpenAPI_dnn_selection_mode_e sel_mode,
    char *udm_group_id,
    char *routing_indicator,
    bool is_h_nw_pub_key_id,
    int h_nw_pub_key_id,
    OpenAPI_ambr_t *session_ambr,
    OpenAPI_list_t *qos_flows_list,
    char *h_smf_instance_id,
    char *smf_instance_id,
    char *pdu_session_smf_set_id,
    char *pdu_session_smf_service_set_id,
    OpenAPI_sbi_binding_level_e pdu_session_smf_binding,
    bool is_enable_pause_charging,
    int enable_pause_charging,
    char *ue_ipv4_address,
    char *ue_ipv6_prefix,
    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info,
    OpenAPI_list_t *eps_bearer_info,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl,
    bool is_always_on_granted,
    int always_on_granted,
    OpenAPI_up_security_t *up_security,
    char *h_smf_service_instance_id,
    char *smf_service_instance_id,
    char *recovery_time,
    bool is_forwarding_ind,
    int forwarding_ind,
    OpenAPI_tunnel_info_t *psa_tunnel_info,
    char *charging_id,
    char *smf_charging_id,
    OpenAPI_charging_information_t *charging_info,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile,
    bool is_nef_ext_buf_support_ind,
    int nef_ext_buf_support_ind,
    bool is_ipv6_index,
    int ipv6_index,
    OpenAPI_ip_address_t *dn_aaa_address,
    OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_info,
    OpenAPI_qos_flow_tunnel_t *ran_tunnel_info,
    OpenAPI_list_t *add_ran_tunnel_info,
    OpenAPI_qos_flow_tunnel_t *red_ran_tunnel_info,
    OpenAPI_list_t *add_red_ran_tunnel_info,
    bool is_nspu_support_ind,
    int nspu_support_ind,
    char *smf_binding_info,
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_cat,
    char *ssc_mode,
    bool is_dlset_support_ind,
    int dlset_support_ind,
    bool is_n9fsc_support_ind,
    int n9fsc_support_ind,
    bool is_anchor_smf_oauth2_required,
    int anchor_smf_oauth2_required,
    OpenAPI_list_t *full_dnai_list,
    bool is_hrsbo_auth_req_ind,
    int hrsbo_auth_req_ind,
    OpenAPI_ip_address_t *h_dns_addr,
    OpenAPI_ip_address_t *h_plmn_addr,
    OpenAPI_list_t *vplmn_offloading_info_list,
    bool is_vplmn_dl_ambr_null,
    OpenAPI_vplmn_dl_ambr_t *vplmn_dl_ambr,
    OpenAPI_list_t *offload_ids,
    char *target_dnai,
    OpenAPI_traffic_influence_info_t *traffic_influ_info,
    OpenAPI_list_t *pending_update_info_list,
    OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh,
    bool is_local_offloading_mgt_allowed_ind,
    int local_offloading_mgt_allowed_ind,
    OpenAPI_ip_address_t *dns_addr,
    OpenAPI_ip_address_t *psa_upf_addr,
    OpenAPI_list_t *local_offloading_info_list,
    bool is_priority_user_ind,
    int priority_user_ind,
    OpenAPI_qos_monitoring_info_t *qos_monitoring_info
);
void OpenAPI_sm_context_free(OpenAPI_sm_context_t *sm_context);
OpenAPI_sm_context_t *OpenAPI_sm_context_parseFromJSON(cJSON *sm_contextJSON);
cJSON *OpenAPI_sm_context_convertToJSON(OpenAPI_sm_context_t *sm_context);
OpenAPI_sm_context_t *OpenAPI_sm_context_copy(OpenAPI_sm_context_t *dst, OpenAPI_sm_context_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_context_H_ */

