/*
 * sm_context_create_data.h
 *
 * Data within Create SM Context Request
 */

#ifndef _OpenAPI_sm_context_create_data_H_
#define _OpenAPI_sm_context_create_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "apn_rate_status.h"
#include "backup_amf_info.h"
#include "ddn_failure_subs.h"
#include "dnn_selection_mode.h"
#include "eps_interworking_indication.h"
#include "guami.h"
#include "ho_state.h"
#include "n2_sm_info_type.h"
#include "ng_ran_target_id.h"
#include "pcf_ue_callback_info.h"
#include "plmn_id_nid.h"
#include "presence_state.h"
#include "rat_type.h"
#include "ref_to_binary_data.h"
#include "request_type.h"
#include "satellite_backhaul_category.h"
#include "sbi_binding_level.h"
#include "server_addressing_info.h"
#include "small_data_rate_status.h"
#include "snssai.h"
#include "tngf_info.h"
#include "trace_data.h"
#include "twif_info.h"
#include "up_cnx_state.h"
#include "user_location.h"
#include "w_agf_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_context_create_data_s OpenAPI_sm_context_create_data_t;
typedef struct OpenAPI_sm_context_create_data_s {
    char *supi;
    bool is_unauthenticated_supi;
    int unauthenticated_supi;
    char *pei;
    char *gpsi;
    bool is_pdu_session_id;
    int pdu_session_id;
    char *dnn;
    char *selected_dnn;
    struct OpenAPI_snssai_s *s_nssai;
    struct OpenAPI_snssai_s *hplmn_snssai;
    char *serving_nf_id;
    struct OpenAPI_guami_s *guami;
    char *service_name;
    struct OpenAPI_plmn_id_nid_s *serving_network;
    OpenAPI_request_type_e request_type;
    struct OpenAPI_ref_to_binary_data_s *n1_sm_msg;
    OpenAPI_access_type_e an_type;
    OpenAPI_access_type_e additional_an_type;
    OpenAPI_rat_type_e rat_type;
    OpenAPI_presence_state_e presence_in_ladn;
    struct OpenAPI_user_location_s *ue_location;
    char *ue_time_zone;
    struct OpenAPI_user_location_s *add_ue_location;
    char *sm_context_status_uri;
    char *h_smf_uri;
    char *h_smf_id;
    char *smf_uri;
    char *smf_id;
    OpenAPI_list_t *additional_hsmf_uri;
    OpenAPI_list_t *additional_hsmf_id;
    OpenAPI_list_t *additional_smf_uri;
    OpenAPI_list_t *additional_smf_id;
    bool is_old_pdu_session_id;
    int old_pdu_session_id;
    OpenAPI_list_t *pdu_sessions_activate_list;
    char *ue_eps_pdn_connection;
    OpenAPI_ho_state_e ho_state;
    char *pcf_id;
    char *pcf_group_id;
    char *pcf_set_id;
    char *nrf_uri;
    char *supported_features;
    OpenAPI_dnn_selection_mode_e sel_mode;
    OpenAPI_list_t *backup_amf_info;
    bool is_trace_data_null;
    struct OpenAPI_trace_data_s *trace_data;
    char *udm_group_id;
    char *routing_indicator;
    bool is_h_nw_pub_key_id;
    int h_nw_pub_key_id;
    OpenAPI_eps_interworking_indication_e eps_interworking_ind;
    bool is_indirect_forwarding_flag;
    int indirect_forwarding_flag;
    bool is_direct_forwarding_flag;
    int direct_forwarding_flag;
    struct OpenAPI_ng_ran_target_id_s *target_id;
    char *eps_bearer_ctx_status;
    bool is_cp_ciot_enabled;
    int cp_ciot_enabled;
    bool is_cp_only_ind;
    int cp_only_ind;
    bool is_invoke_nef;
    int invoke_nef;
    bool is_ma_request_ind;
    int ma_request_ind;
    bool is_ma_nw_upgrade_ind;
    int ma_nw_upgrade_ind;
    struct OpenAPI_ref_to_binary_data_s *n2_sm_info;
    OpenAPI_n2_sm_info_type_e n2_sm_info_type;
    struct OpenAPI_ref_to_binary_data_s *n2_sm_info_ext1;
    OpenAPI_n2_sm_info_type_e n2_sm_info_type_ext1;
    char *sm_context_ref;
    struct OpenAPI_plmn_id_nid_s *sm_context_smf_plmn_id;
    char *sm_context_smf_id;
    char *sm_context_smf_set_id;
    char *sm_context_smf_service_set_id;
    OpenAPI_sbi_binding_level_e sm_context_smf_binding;
    OpenAPI_up_cnx_state_e up_cnx_state;
    struct OpenAPI_small_data_rate_status_s *small_data_rate_status;
    struct OpenAPI_apn_rate_status_s *apn_rate_status;
    bool is_extended_nas_sm_timer_ind;
    int extended_nas_sm_timer_ind;
    bool is_dl_data_waiting_ind;
    int dl_data_waiting_ind;
    struct OpenAPI_ddn_failure_subs_s *ddn_failure_subs;
    bool is_smf_transfer_ind;
    int smf_transfer_ind;
    char *old_smf_id;
    char *old_sm_context_ref;
    struct OpenAPI_w_agf_info_s *w_agf_info;
    struct OpenAPI_tngf_info_s *tngf_info;
    struct OpenAPI_twif_info_s *twif_info;
    bool is_ran_unchanged_ind;
    int ran_unchanged_ind;
    bool is_same_pcf_selection_ind;
    int same_pcf_selection_ind;
    char *target_dnai;
    char *nrf_management_uri;
    char *nrf_discovery_uri;
    char *nrf_access_token_uri;
    OpenAPI_list_t* nrf_oauth2_required;
    char *smf_binding_info;
    OpenAPI_list_t *pvs_info;
    bool is_onboarding_ind;
    int onboarding_ind;
    char *old_pdu_session_ref;
    bool is_sm_policy_notify_ind;
    int sm_policy_notify_ind;
    bool is_pcf_ue_callback_info_null;
    struct OpenAPI_pcf_ue_callback_info_s *pcf_ue_callback_info;
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_cat;
    bool is_upip_supported;
    int upip_supported;
    bool is_uav_authenticated;
    int uav_authenticated;
    bool is_disaster_roaming_ind;
    int disaster_roaming_ind;
    bool is_anchor_smf_oauth2_required;
    int anchor_smf_oauth2_required;
    bool is_sm_context_smf_oauth2_required;
    int sm_context_smf_oauth2_required;
} OpenAPI_sm_context_create_data_t;

OpenAPI_sm_context_create_data_t *OpenAPI_sm_context_create_data_create(
    char *supi,
    bool is_unauthenticated_supi,
    int unauthenticated_supi,
    char *pei,
    char *gpsi,
    bool is_pdu_session_id,
    int pdu_session_id,
    char *dnn,
    char *selected_dnn,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_snssai_t *hplmn_snssai,
    char *serving_nf_id,
    OpenAPI_guami_t *guami,
    char *service_name,
    OpenAPI_plmn_id_nid_t *serving_network,
    OpenAPI_request_type_e request_type,
    OpenAPI_ref_to_binary_data_t *n1_sm_msg,
    OpenAPI_access_type_e an_type,
    OpenAPI_access_type_e additional_an_type,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_presence_state_e presence_in_ladn,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone,
    OpenAPI_user_location_t *add_ue_location,
    char *sm_context_status_uri,
    char *h_smf_uri,
    char *h_smf_id,
    char *smf_uri,
    char *smf_id,
    OpenAPI_list_t *additional_hsmf_uri,
    OpenAPI_list_t *additional_hsmf_id,
    OpenAPI_list_t *additional_smf_uri,
    OpenAPI_list_t *additional_smf_id,
    bool is_old_pdu_session_id,
    int old_pdu_session_id,
    OpenAPI_list_t *pdu_sessions_activate_list,
    char *ue_eps_pdn_connection,
    OpenAPI_ho_state_e ho_state,
    char *pcf_id,
    char *pcf_group_id,
    char *pcf_set_id,
    char *nrf_uri,
    char *supported_features,
    OpenAPI_dnn_selection_mode_e sel_mode,
    OpenAPI_list_t *backup_amf_info,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    char *udm_group_id,
    char *routing_indicator,
    bool is_h_nw_pub_key_id,
    int h_nw_pub_key_id,
    OpenAPI_eps_interworking_indication_e eps_interworking_ind,
    bool is_indirect_forwarding_flag,
    int indirect_forwarding_flag,
    bool is_direct_forwarding_flag,
    int direct_forwarding_flag,
    OpenAPI_ng_ran_target_id_t *target_id,
    char *eps_bearer_ctx_status,
    bool is_cp_ciot_enabled,
    int cp_ciot_enabled,
    bool is_cp_only_ind,
    int cp_only_ind,
    bool is_invoke_nef,
    int invoke_nef,
    bool is_ma_request_ind,
    int ma_request_ind,
    bool is_ma_nw_upgrade_ind,
    int ma_nw_upgrade_ind,
    OpenAPI_ref_to_binary_data_t *n2_sm_info,
    OpenAPI_n2_sm_info_type_e n2_sm_info_type,
    OpenAPI_ref_to_binary_data_t *n2_sm_info_ext1,
    OpenAPI_n2_sm_info_type_e n2_sm_info_type_ext1,
    char *sm_context_ref,
    OpenAPI_plmn_id_nid_t *sm_context_smf_plmn_id,
    char *sm_context_smf_id,
    char *sm_context_smf_set_id,
    char *sm_context_smf_service_set_id,
    OpenAPI_sbi_binding_level_e sm_context_smf_binding,
    OpenAPI_up_cnx_state_e up_cnx_state,
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status,
    bool is_extended_nas_sm_timer_ind,
    int extended_nas_sm_timer_ind,
    bool is_dl_data_waiting_ind,
    int dl_data_waiting_ind,
    OpenAPI_ddn_failure_subs_t *ddn_failure_subs,
    bool is_smf_transfer_ind,
    int smf_transfer_ind,
    char *old_smf_id,
    char *old_sm_context_ref,
    OpenAPI_w_agf_info_t *w_agf_info,
    OpenAPI_tngf_info_t *tngf_info,
    OpenAPI_twif_info_t *twif_info,
    bool is_ran_unchanged_ind,
    int ran_unchanged_ind,
    bool is_same_pcf_selection_ind,
    int same_pcf_selection_ind,
    char *target_dnai,
    char *nrf_management_uri,
    char *nrf_discovery_uri,
    char *nrf_access_token_uri,
    OpenAPI_list_t* nrf_oauth2_required,
    char *smf_binding_info,
    OpenAPI_list_t *pvs_info,
    bool is_onboarding_ind,
    int onboarding_ind,
    char *old_pdu_session_ref,
    bool is_sm_policy_notify_ind,
    int sm_policy_notify_ind,
    bool is_pcf_ue_callback_info_null,
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info,
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_cat,
    bool is_upip_supported,
    int upip_supported,
    bool is_uav_authenticated,
    int uav_authenticated,
    bool is_disaster_roaming_ind,
    int disaster_roaming_ind,
    bool is_anchor_smf_oauth2_required,
    int anchor_smf_oauth2_required,
    bool is_sm_context_smf_oauth2_required,
    int sm_context_smf_oauth2_required
);
void OpenAPI_sm_context_create_data_free(OpenAPI_sm_context_create_data_t *sm_context_create_data);
OpenAPI_sm_context_create_data_t *OpenAPI_sm_context_create_data_parseFromJSON(cJSON *sm_context_create_dataJSON);
cJSON *OpenAPI_sm_context_create_data_convertToJSON(OpenAPI_sm_context_create_data_t *sm_context_create_data);
OpenAPI_sm_context_create_data_t *OpenAPI_sm_context_create_data_copy(OpenAPI_sm_context_create_data_t *dst, OpenAPI_sm_context_create_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_context_create_data_H_ */

