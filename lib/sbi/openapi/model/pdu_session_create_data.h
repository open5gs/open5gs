/*
 * pdu_session_create_data.h
 *
 *
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
#include "plmn_id_nid.h"
#include "presence_state.h"
#include "rat_type.h"
#include "ref_to_binary_data.h"
#include "request_type.h"
#include "roaming_charging_profile.h"
#include "secondary_rat_usage_info.h"
#include "small_data_rate_status.h"
#include "snssai.h"
#include "tunnel_info.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_create_data_s OpenAPI_pdu_session_create_data_t;
typedef struct OpenAPI_pdu_session_create_data_s {
    char *supi;
    int unauthenticated_supi;
    char *pei;
    int pdu_session_id;
    char *dnn;
    struct OpenAPI_snssai_s *s_nssai;
    char *vsmf_id;
    char *ismf_id;
    struct OpenAPI_plmn_id_nid_s *serving_network;
    OpenAPI_request_type_e request_type;
    OpenAPI_list_t *eps_bearer_id;
    char pgw_s8c_fteid;
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
    int ho_preparation_indication;
    OpenAPI_dnn_selection_mode_e sel_mode;
    int always_on_requested;
    char *udm_group_id;
    char *routing_indicator;
    OpenAPI_eps_interworking_indication_e eps_interworking_ind;
    char *v_smf_service_instance_id;
    char *i_smf_service_instance_id;
    char *recovery_time;
    struct OpenAPI_roaming_charging_profile_s *roaming_charging_profile;
    char *charging_id;
    int old_pdu_session_id;
    char *eps_bearer_ctx_status;
    char *amf_nf_id;
    struct OpenAPI_guami_s *guami;
    int cp_ciot_enabled;
    int cp_only_ind;
    int invoke_nef;
    int ma_request_ind;
    int ma_nw_upgrade_ind;
    OpenAPI_list_t *dnai_list;
    OpenAPI_presence_state_e presence_in_ladn;
    OpenAPI_list_t *secondary_rat_usage_info;
    struct OpenAPI_small_data_rate_status_s *small_data_rate_status;
    struct OpenAPI_apn_rate_status_s *apn_rate_status;
    int dl_serving_plmn_rate_ctl;
} OpenAPI_pdu_session_create_data_t;

OpenAPI_pdu_session_create_data_t *OpenAPI_pdu_session_create_data_create(
    char *supi,
    int unauthenticated_supi,
    char *pei,
    int pdu_session_id,
    char *dnn,
    OpenAPI_snssai_t *s_nssai,
    char *vsmf_id,
    char *ismf_id,
    OpenAPI_plmn_id_nid_t *serving_network,
    OpenAPI_request_type_e request_type,
    OpenAPI_list_t *eps_bearer_id,
    char pgw_s8c_fteid,
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
    int ho_preparation_indication,
    OpenAPI_dnn_selection_mode_e sel_mode,
    int always_on_requested,
    char *udm_group_id,
    char *routing_indicator,
    OpenAPI_eps_interworking_indication_e eps_interworking_ind,
    char *v_smf_service_instance_id,
    char *i_smf_service_instance_id,
    char *recovery_time,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile,
    char *charging_id,
    int old_pdu_session_id,
    char *eps_bearer_ctx_status,
    char *amf_nf_id,
    OpenAPI_guami_t *guami,
    int cp_ciot_enabled,
    int cp_only_ind,
    int invoke_nef,
    int ma_request_ind,
    int ma_nw_upgrade_ind,
    OpenAPI_list_t *dnai_list,
    OpenAPI_presence_state_e presence_in_ladn,
    OpenAPI_list_t *secondary_rat_usage_info,
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status,
    int dl_serving_plmn_rate_ctl
    );
void OpenAPI_pdu_session_create_data_free(OpenAPI_pdu_session_create_data_t *pdu_session_create_data);
OpenAPI_pdu_session_create_data_t *OpenAPI_pdu_session_create_data_parseFromJSON(cJSON *pdu_session_create_dataJSON);
cJSON *OpenAPI_pdu_session_create_data_convertToJSON(OpenAPI_pdu_session_create_data_t *pdu_session_create_data);
OpenAPI_pdu_session_create_data_t *OpenAPI_pdu_session_create_data_copy(OpenAPI_pdu_session_create_data_t *dst, OpenAPI_pdu_session_create_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_create_data_H_ */

