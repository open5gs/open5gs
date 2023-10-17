/*
 * sm_policy_context_data.h
 *
 * Contains the parameters used to create an Individual SM policy resource.
 */

#ifndef _OpenAPI_sm_policy_context_data_H_
#define _OpenAPI_sm_policy_context_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "acc_net_ch_id.h"
#include "acc_net_charging_address.h"
#include "access_type.h"
#include "additional_access_info.h"
#include "ambr.h"
#include "dnn_selection_mode.h"
#include "ma_pdu_indication.h"
#include "npcf_atsss_capability.h"
#include "nwdaf_data.h"
#include "pcf_ue_callback_info.h"
#include "pdu_session_type.h"
#include "plmn_id_nid.h"
#include "qos_flow_usage.h"
#include "rat_type.h"
#include "satellite_backhaul_category.h"
#include "server_addressing_info.h"
#include "serving_nf_identity.h"
#include "snssai.h"
#include "subscribed_default_qos.h"
#include "trace_data.h"
#include "user_location.h"
#include "vplmn_qos.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_policy_context_data_s OpenAPI_sm_policy_context_data_t;
typedef struct OpenAPI_sm_policy_context_data_s {
    struct OpenAPI_acc_net_ch_id_s *acc_net_ch_id;
    struct OpenAPI_acc_net_charging_address_s *charg_entity_addr;
    char *gpsi;
    char *supi;
    bool is_invalid_supi;
    int invalid_supi;
    OpenAPI_list_t *inter_grp_ids;
    int pdu_session_id;
    OpenAPI_pdu_session_type_e pdu_session_type;
    char *chargingcharacteristics;
    char *dnn;
    OpenAPI_dnn_selection_mode_e dnn_sel_mode;
    char *notification_uri;
    OpenAPI_access_type_e access_type;
    OpenAPI_rat_type_e rat_type;
    struct OpenAPI_additional_access_info_s *add_access_info;
    struct OpenAPI_plmn_id_nid_s *serving_network;
    struct OpenAPI_user_location_s *user_location_info;
    char *ue_time_zone;
    char *pei;
    char *ipv4_address;
    char *ipv6_address_prefix;
    char *ip_domain;
    struct OpenAPI_ambr_s *subs_sess_ambr;
    char *auth_prof_index;
    struct OpenAPI_subscribed_default_qos_s *subs_def_qos;
    struct OpenAPI_vplmn_qos_s *vplmn_qos;
    bool is_num_of_pack_filter;
    int num_of_pack_filter;
    bool is_online;
    int online;
    bool is_offline;
    int offline;
    bool is__3gpp_ps_data_off_status;
    int _3gpp_ps_data_off_status;
    bool is_ref_qos_indication;
    int ref_qos_indication;
    bool is_trace_req_null;
    struct OpenAPI_trace_data_s *trace_req;
    struct OpenAPI_snssai_s *slice_info;
    OpenAPI_qos_flow_usage_e qos_flow_usage;
    struct OpenAPI_serving_nf_identity_s *serv_nf_id;
    char *supp_feat;
    char *smf_id;
    char *recovery_time;
    OpenAPI_ma_pdu_indication_e ma_pdu_ind;
    OpenAPI_npcf_atsss_capability_e atsss_capab;
    OpenAPI_list_t *ipv4_frame_route_list;
    OpenAPI_list_t *ipv6_frame_route_list;
    OpenAPI_satellite_backhaul_category_e sat_backhaul_category;
    bool is_pcf_ue_info_null;
    struct OpenAPI_pcf_ue_callback_info_s *pcf_ue_info;
    OpenAPI_list_t *pvs_info;
    bool is_onboard_ind;
    int onboard_ind;
    OpenAPI_list_t *nwdaf_datas;
} OpenAPI_sm_policy_context_data_t;

OpenAPI_sm_policy_context_data_t *OpenAPI_sm_policy_context_data_create(
    OpenAPI_acc_net_ch_id_t *acc_net_ch_id,
    OpenAPI_acc_net_charging_address_t *charg_entity_addr,
    char *gpsi,
    char *supi,
    bool is_invalid_supi,
    int invalid_supi,
    OpenAPI_list_t *inter_grp_ids,
    int pdu_session_id,
    OpenAPI_pdu_session_type_e pdu_session_type,
    char *chargingcharacteristics,
    char *dnn,
    OpenAPI_dnn_selection_mode_e dnn_sel_mode,
    char *notification_uri,
    OpenAPI_access_type_e access_type,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_additional_access_info_t *add_access_info,
    OpenAPI_plmn_id_nid_t *serving_network,
    OpenAPI_user_location_t *user_location_info,
    char *ue_time_zone,
    char *pei,
    char *ipv4_address,
    char *ipv6_address_prefix,
    char *ip_domain,
    OpenAPI_ambr_t *subs_sess_ambr,
    char *auth_prof_index,
    OpenAPI_subscribed_default_qos_t *subs_def_qos,
    OpenAPI_vplmn_qos_t *vplmn_qos,
    bool is_num_of_pack_filter,
    int num_of_pack_filter,
    bool is_online,
    int online,
    bool is_offline,
    int offline,
    bool is__3gpp_ps_data_off_status,
    int _3gpp_ps_data_off_status,
    bool is_ref_qos_indication,
    int ref_qos_indication,
    bool is_trace_req_null,
    OpenAPI_trace_data_t *trace_req,
    OpenAPI_snssai_t *slice_info,
    OpenAPI_qos_flow_usage_e qos_flow_usage,
    OpenAPI_serving_nf_identity_t *serv_nf_id,
    char *supp_feat,
    char *smf_id,
    char *recovery_time,
    OpenAPI_ma_pdu_indication_e ma_pdu_ind,
    OpenAPI_npcf_atsss_capability_e atsss_capab,
    OpenAPI_list_t *ipv4_frame_route_list,
    OpenAPI_list_t *ipv6_frame_route_list,
    OpenAPI_satellite_backhaul_category_e sat_backhaul_category,
    bool is_pcf_ue_info_null,
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_info,
    OpenAPI_list_t *pvs_info,
    bool is_onboard_ind,
    int onboard_ind,
    OpenAPI_list_t *nwdaf_datas
);
void OpenAPI_sm_policy_context_data_free(OpenAPI_sm_policy_context_data_t *sm_policy_context_data);
OpenAPI_sm_policy_context_data_t *OpenAPI_sm_policy_context_data_parseFromJSON(cJSON *sm_policy_context_dataJSON);
cJSON *OpenAPI_sm_policy_context_data_convertToJSON(OpenAPI_sm_policy_context_data_t *sm_policy_context_data);
OpenAPI_sm_policy_context_data_t *OpenAPI_sm_policy_context_data_copy(OpenAPI_sm_policy_context_data_t *dst, OpenAPI_sm_policy_context_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_policy_context_data_H_ */

