/*
 * hsmf_update_data.h
 *
 * Data within Update Request towards H-SMF, or from I-SMF to SMF
 */

#ifndef _OpenAPI_hsmf_update_data_H_
#define _OpenAPI_hsmf_update_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "cause.h"
#include "eps_interworking_indication.h"
#include "guami.h"
#include "ma_release_indication.h"
#include "max_integrity_protected_data_rate.h"
#include "mo_exp_data_counter.h"
#include "n4_information.h"
#include "ng_ap_cause.h"
#include "pcf_ue_callback_info.h"
#include "pdu_session_notify_item.h"
#include "plmn_id_nid.h"
#include "presence_state.h"
#include "psa_information.h"
#include "qos_flow_item.h"
#include "qos_flow_notify_item.h"
#include "rat_type.h"
#include "ref_to_binary_data.h"
#include "request_indication.h"
#include "roaming_charging_profile.h"
#include "satellite_backhaul_category.h"
#include "secondary_rat_usage_info.h"
#include "secondary_rat_usage_report.h"
#include "security_result.h"
#include "tunnel_info.h"
#include "ulcl_bp_information.h"
#include "unavailable_access_indication.h"
#include "up_cnx_state.h"
#include "up_security_info.h"
#include "user_location.h"
#include "vplmn_qos.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_hsmf_update_data_s OpenAPI_hsmf_update_data_t;
typedef enum { OpenAPI_hsmf_update_data_SMPOLICYNOTIFYIND_NULL = 0, OpenAPI_hsmf_update_data_SMPOLICYNOTIFYIND__true } OpenAPI_hsmf_update_data_sm_policy_notify_ind_e;

char* OpenAPI_hsmf_update_data_sm_policy_notify_ind_ToString(OpenAPI_hsmf_update_data_sm_policy_notify_ind_e sm_policy_notify_ind);

OpenAPI_hsmf_update_data_sm_policy_notify_ind_e OpenAPI_hsmf_update_data_sm_policy_notify_ind_FromString(char* sm_policy_notify_ind);
typedef struct OpenAPI_hsmf_update_data_s {
    OpenAPI_request_indication_e request_indication;
    char *pei;
    struct OpenAPI_tunnel_info_s *vcn_tunnel_info;
    struct OpenAPI_tunnel_info_s *icn_tunnel_info;
    struct OpenAPI_tunnel_info_s *additional_cn_tunnel_info;
    struct OpenAPI_plmn_id_nid_s *serving_network;
    OpenAPI_access_type_e an_type;
    OpenAPI_access_type_e additional_an_type;
    OpenAPI_rat_type_e rat_type;
    struct OpenAPI_user_location_s *ue_location;
    char *ue_time_zone;
    struct OpenAPI_user_location_s *add_ue_location;
    bool is_pause_charging;
    int pause_charging;
    bool is_pti;
    int pti;
    struct OpenAPI_ref_to_binary_data_s *n1_sm_info_from_ue;
    struct OpenAPI_ref_to_binary_data_s *unknown_n1_sm_info;
    OpenAPI_list_t *qos_flows_rel_notify_list;
    OpenAPI_list_t *qos_flows_notify_list;
    OpenAPI_list_t *notify_list;
    OpenAPI_list_t *eps_bearer_id;
    bool is_ho_preparation_indication;
    int ho_preparation_indication;
    OpenAPI_list_t *revoke_ebi_list;
    OpenAPI_cause_e cause;
    struct OpenAPI_ng_ap_cause_s *ng_ap_cause;
    bool is__5g_mm_cause_value;
    int _5g_mm_cause_value;
    bool is_always_on_requested;
    int always_on_requested;
    OpenAPI_eps_interworking_indication_e eps_interworking_ind;
    OpenAPI_list_t *secondary_rat_usage_report;
    OpenAPI_list_t *secondary_rat_usage_info;
    bool is_an_type_can_be_changed;
    int an_type_can_be_changed;
    OpenAPI_ma_release_indication_e ma_release_ind;
    bool is_ma_nw_upgrade_ind;
    int ma_nw_upgrade_ind;
    bool is_ma_request_ind;
    int ma_request_ind;
    OpenAPI_unavailable_access_indication_e unavailable_access_ind;
    OpenAPI_list_t *psa_info;
    struct OpenAPI_ulcl_bp_information_s *ulcl_bp_info;
    struct OpenAPI_n4_information_s *n4_info;
    struct OpenAPI_n4_information_s *n4_info_ext1;
    struct OpenAPI_n4_information_s *n4_info_ext2;
    OpenAPI_presence_state_e presence_in_ladn;
    char *vsmf_pdu_session_uri;
    char *vsmf_id;
    char *v_smf_service_instance_id;
    char *ismf_pdu_session_uri;
    char *ismf_id;
    char *i_smf_service_instance_id;
    bool is_dl_serving_plmn_rate_ctl_null;
    bool is_dl_serving_plmn_rate_ctl;
    int dl_serving_plmn_rate_ctl;
    OpenAPI_list_t *dnai_list;
    char *supported_features;
    struct OpenAPI_roaming_charging_profile_s *roaming_charging_profile;
    struct OpenAPI_mo_exp_data_counter_s *mo_exp_data_counter;
    struct OpenAPI_vplmn_qos_s *vplmn_qos;
    struct OpenAPI_security_result_s *security_result;
    struct OpenAPI_up_security_info_s *up_security_info;
    char *amf_nf_id;
    struct OpenAPI_guami_s *guami;
    OpenAPI_list_t *secondary_rat_usage_data_report_container;
    OpenAPI_hsmf_update_data_sm_policy_notify_ind_e sm_policy_notify_ind;
    bool is_pcf_ue_callback_info_null;
    struct OpenAPI_pcf_ue_callback_info_s *pcf_ue_callback_info;
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_cat;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ul;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl;
    OpenAPI_up_cnx_state_e up_cnx_state;
} OpenAPI_hsmf_update_data_t;

OpenAPI_hsmf_update_data_t *OpenAPI_hsmf_update_data_create(
    OpenAPI_request_indication_e request_indication,
    char *pei,
    OpenAPI_tunnel_info_t *vcn_tunnel_info,
    OpenAPI_tunnel_info_t *icn_tunnel_info,
    OpenAPI_tunnel_info_t *additional_cn_tunnel_info,
    OpenAPI_plmn_id_nid_t *serving_network,
    OpenAPI_access_type_e an_type,
    OpenAPI_access_type_e additional_an_type,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone,
    OpenAPI_user_location_t *add_ue_location,
    bool is_pause_charging,
    int pause_charging,
    bool is_pti,
    int pti,
    OpenAPI_ref_to_binary_data_t *n1_sm_info_from_ue,
    OpenAPI_ref_to_binary_data_t *unknown_n1_sm_info,
    OpenAPI_list_t *qos_flows_rel_notify_list,
    OpenAPI_list_t *qos_flows_notify_list,
    OpenAPI_list_t *notify_list,
    OpenAPI_list_t *eps_bearer_id,
    bool is_ho_preparation_indication,
    int ho_preparation_indication,
    OpenAPI_list_t *revoke_ebi_list,
    OpenAPI_cause_e cause,
    OpenAPI_ng_ap_cause_t *ng_ap_cause,
    bool is__5g_mm_cause_value,
    int _5g_mm_cause_value,
    bool is_always_on_requested,
    int always_on_requested,
    OpenAPI_eps_interworking_indication_e eps_interworking_ind,
    OpenAPI_list_t *secondary_rat_usage_report,
    OpenAPI_list_t *secondary_rat_usage_info,
    bool is_an_type_can_be_changed,
    int an_type_can_be_changed,
    OpenAPI_ma_release_indication_e ma_release_ind,
    bool is_ma_nw_upgrade_ind,
    int ma_nw_upgrade_ind,
    bool is_ma_request_ind,
    int ma_request_ind,
    OpenAPI_unavailable_access_indication_e unavailable_access_ind,
    OpenAPI_list_t *psa_info,
    OpenAPI_ulcl_bp_information_t *ulcl_bp_info,
    OpenAPI_n4_information_t *n4_info,
    OpenAPI_n4_information_t *n4_info_ext1,
    OpenAPI_n4_information_t *n4_info_ext2,
    OpenAPI_presence_state_e presence_in_ladn,
    char *vsmf_pdu_session_uri,
    char *vsmf_id,
    char *v_smf_service_instance_id,
    char *ismf_pdu_session_uri,
    char *ismf_id,
    char *i_smf_service_instance_id,
    bool is_dl_serving_plmn_rate_ctl_null,
    bool is_dl_serving_plmn_rate_ctl,
    int dl_serving_plmn_rate_ctl,
    OpenAPI_list_t *dnai_list,
    char *supported_features,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile,
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter,
    OpenAPI_vplmn_qos_t *vplmn_qos,
    OpenAPI_security_result_t *security_result,
    OpenAPI_up_security_info_t *up_security_info,
    char *amf_nf_id,
    OpenAPI_guami_t *guami,
    OpenAPI_list_t *secondary_rat_usage_data_report_container,
    OpenAPI_hsmf_update_data_sm_policy_notify_ind_e sm_policy_notify_ind,
    bool is_pcf_ue_callback_info_null,
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info,
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_cat,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ul,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl,
    OpenAPI_up_cnx_state_e up_cnx_state
);
void OpenAPI_hsmf_update_data_free(OpenAPI_hsmf_update_data_t *hsmf_update_data);
OpenAPI_hsmf_update_data_t *OpenAPI_hsmf_update_data_parseFromJSON(cJSON *hsmf_update_dataJSON);
cJSON *OpenAPI_hsmf_update_data_convertToJSON(OpenAPI_hsmf_update_data_t *hsmf_update_data);
OpenAPI_hsmf_update_data_t *OpenAPI_hsmf_update_data_copy(OpenAPI_hsmf_update_data_t *dst, OpenAPI_hsmf_update_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hsmf_update_data_H_ */

