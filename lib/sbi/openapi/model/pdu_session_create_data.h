/*
 * pdu_session_create_data.h
 *
 * Data within Create Request
 */

#ifndef _OpenAPI_pdu_session_create_data_H_
#define _OpenAPI_pdu_session_create_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "apn_rate_status.h"
#include "dnn_selection_mode.h"
#include "eps_interworking_indication.h"
#include "guami.h"
#include "max_integrity_protected_data_rate.h"
#include "pcf_ue_callback_info.h"
#include "plmn_id_nid.h"
#include "presence_state.h"
#include "rat_type.h"
#include "redundant_pdu_session_information.h"
#include "ref_to_binary_data.h"
#include "request_type.h"
#include "roaming_charging_profile.h"
#include "satellite_backhaul_category.h"
#include "secondary_rat_usage_info.h"
#include "small_data_rate_status.h"
#include "snssai.h"
#include "tunnel_info.h"
#include "up_cnx_state.h"
#include "up_security_info.h"
#include "user_location.h"
#include "vplmn_qos.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_create_data_s OpenAPI_pdu_session_create_data_t;
typedef struct OpenAPI_pdu_session_create_data_s {
    char *supi;
    bool is_unauthenticated_supi;
    int unauthenticated_supi;
    char *pei;
    bool is_pdu_session_id;
    int pdu_session_id;
    char *dnn;
    char *selected_dnn;
    struct OpenAPI_snssai_s *s_nssai;
    struct OpenAPI_snssai_s *hplmn_snssai;
    char *vsmf_id;
    char *ismf_id;
    struct OpenAPI_plmn_id_nid_s *serving_network;
    OpenAPI_request_type_e request_type;
    OpenAPI_list_t *eps_bearer_id;
    char *pgw_s8c_fteid;
    char *vsmf_pdu_session_uri;
    char *ismf_pdu_session_uri;
    struct OpenAPI_tunnel_info_s *vcn_tunnel_info;
    struct OpenAPI_tunnel_info_s *icn_tunnel_info;
    struct OpenAPI_tunnel_info_s *n9_forwarding_tunnel_info;
    struct OpenAPI_tunnel_info_s *additional_cn_tunnel_info;
    OpenAPI_access_type_e an_type;
    OpenAPI_access_type_e additional_an_type;
    OpenAPI_rat_type_e rat_type;
    struct OpenAPI_user_location_s *ue_location;
    char *ue_time_zone;
    struct OpenAPI_user_location_s *add_ue_location;
    char *gpsi;
    struct OpenAPI_ref_to_binary_data_s *n1_sm_info_from_ue;
    struct OpenAPI_ref_to_binary_data_s *unknown_n1_sm_info;
    char *supported_features;
    char *h_pcf_id;
    char *pcf_id;
    char *pcf_group_id;
    char *pcf_set_id;
    bool is_ho_preparation_indication;
    int ho_preparation_indication;
    OpenAPI_dnn_selection_mode_e sel_mode;
    bool is_always_on_requested;
    int always_on_requested;
    char *udm_group_id;
    char *routing_indicator;
    bool is_h_nw_pub_key_id;
    int h_nw_pub_key_id;
    OpenAPI_eps_interworking_indication_e eps_interworking_ind;
    char *v_smf_service_instance_id;
    char *i_smf_service_instance_id;
    char *recovery_time;
    struct OpenAPI_roaming_charging_profile_s *roaming_charging_profile;
    char *charging_id;
    bool is_old_pdu_session_id;
    int old_pdu_session_id;
    char *eps_bearer_ctx_status;
    char *amf_nf_id;
    struct OpenAPI_guami_s *guami;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ul;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl;
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
    OpenAPI_list_t *dnai_list;
    OpenAPI_presence_state_e presence_in_ladn;
    OpenAPI_list_t *secondary_rat_usage_info;
    struct OpenAPI_small_data_rate_status_s *small_data_rate_status;
    struct OpenAPI_apn_rate_status_s *apn_rate_status;
    bool is_dl_serving_plmn_rate_ctl;
    int dl_serving_plmn_rate_ctl;
    struct OpenAPI_up_security_info_s *up_security_info;
    struct OpenAPI_vplmn_qos_s *vplmn_qos;
    char *old_sm_context_ref;
    struct OpenAPI_redundant_pdu_session_information_s *redundant_pdu_session_info;
    char *old_pdu_session_ref;
    bool is_sm_policy_notify_ind;
    int sm_policy_notify_ind;
    bool is_pcf_ue_callback_info_null;
    struct OpenAPI_pcf_ue_callback_info_s *pcf_ue_callback_info;
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_cat;
    bool is_upip_supported;
    int upip_supported;
    OpenAPI_up_cnx_state_e up_cnx_state;
    bool is_disaster_roaming_ind;
    int disaster_roaming_ind;
} OpenAPI_pdu_session_create_data_t;

OpenAPI_pdu_session_create_data_t *OpenAPI_pdu_session_create_data_create(
    char *supi,
    bool is_unauthenticated_supi,
    int unauthenticated_supi,
    char *pei,
    bool is_pdu_session_id,
    int pdu_session_id,
    char *dnn,
    char *selected_dnn,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_snssai_t *hplmn_snssai,
    char *vsmf_id,
    char *ismf_id,
    OpenAPI_plmn_id_nid_t *serving_network,
    OpenAPI_request_type_e request_type,
    OpenAPI_list_t *eps_bearer_id,
    char *pgw_s8c_fteid,
    char *vsmf_pdu_session_uri,
    char *ismf_pdu_session_uri,
    OpenAPI_tunnel_info_t *vcn_tunnel_info,
    OpenAPI_tunnel_info_t *icn_tunnel_info,
    OpenAPI_tunnel_info_t *n9_forwarding_tunnel_info,
    OpenAPI_tunnel_info_t *additional_cn_tunnel_info,
    OpenAPI_access_type_e an_type,
    OpenAPI_access_type_e additional_an_type,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone,
    OpenAPI_user_location_t *add_ue_location,
    char *gpsi,
    OpenAPI_ref_to_binary_data_t *n1_sm_info_from_ue,
    OpenAPI_ref_to_binary_data_t *unknown_n1_sm_info,
    char *supported_features,
    char *h_pcf_id,
    char *pcf_id,
    char *pcf_group_id,
    char *pcf_set_id,
    bool is_ho_preparation_indication,
    int ho_preparation_indication,
    OpenAPI_dnn_selection_mode_e sel_mode,
    bool is_always_on_requested,
    int always_on_requested,
    char *udm_group_id,
    char *routing_indicator,
    bool is_h_nw_pub_key_id,
    int h_nw_pub_key_id,
    OpenAPI_eps_interworking_indication_e eps_interworking_ind,
    char *v_smf_service_instance_id,
    char *i_smf_service_instance_id,
    char *recovery_time,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile,
    char *charging_id,
    bool is_old_pdu_session_id,
    int old_pdu_session_id,
    char *eps_bearer_ctx_status,
    char *amf_nf_id,
    OpenAPI_guami_t *guami,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ul,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl,
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
    OpenAPI_list_t *dnai_list,
    OpenAPI_presence_state_e presence_in_ladn,
    OpenAPI_list_t *secondary_rat_usage_info,
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status,
    bool is_dl_serving_plmn_rate_ctl,
    int dl_serving_plmn_rate_ctl,
    OpenAPI_up_security_info_t *up_security_info,
    OpenAPI_vplmn_qos_t *vplmn_qos,
    char *old_sm_context_ref,
    OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_info,
    char *old_pdu_session_ref,
    bool is_sm_policy_notify_ind,
    int sm_policy_notify_ind,
    bool is_pcf_ue_callback_info_null,
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info,
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_cat,
    bool is_upip_supported,
    int upip_supported,
    OpenAPI_up_cnx_state_e up_cnx_state,
    bool is_disaster_roaming_ind,
    int disaster_roaming_ind
);
void OpenAPI_pdu_session_create_data_free(OpenAPI_pdu_session_create_data_t *pdu_session_create_data);
OpenAPI_pdu_session_create_data_t *OpenAPI_pdu_session_create_data_parseFromJSON(cJSON *pdu_session_create_dataJSON);
cJSON *OpenAPI_pdu_session_create_data_convertToJSON(OpenAPI_pdu_session_create_data_t *pdu_session_create_data);
OpenAPI_pdu_session_create_data_t *OpenAPI_pdu_session_create_data_copy(OpenAPI_pdu_session_create_data_t *dst, OpenAPI_pdu_session_create_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_create_data_H_ */

