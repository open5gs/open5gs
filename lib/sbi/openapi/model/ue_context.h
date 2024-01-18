/*
 * ue_context.h
 *
 * Represents an individual ueContext resource
 */

#ifndef _OpenAPI_ue_context_H_
#define _OpenAPI_ue_context_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ambr.h"
#include "analytics_subscription.h"
#include "area.h"
#include "cag_data.h"
#include "core_network_type.h"
#include "ec_restriction_data_wb.h"
#include "eps_interworking_info.h"
#include "ext_amf_event_subscription.h"
#include "immediate_mdt_conf.h"
#include "mm_context.h"
#include "mo_exp_data_counter.h"
#include "pcf_ue_callback_info.h"
#include "pdu_session_context.h"
#include "pdu_session_info.h"
#include "plmn_id.h"
#include "policy_req_trigger.h"
#include "presence_info.h"
#include "prose_context.h"
#include "rat_type.h"
#include "sbi_binding_level.h"
#include "seaf_data.h"
#include "service_area_restriction.h"
#include "slice_mbr.h"
#include "small_data_rate_status_info.h"
#include "smf_selection_data.h"
#include "trace_data.h"
#include "updp_subscription_data.h"
#include "v2x_context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_context_s OpenAPI_ue_context_t;
typedef struct OpenAPI_ue_context_s {
    char *supi;
    bool is_supi_unauth_ind;
    int supi_unauth_ind;
    OpenAPI_list_t *gpsi_list;
    char *pei;
    char *udm_group_id;
    char *ausf_group_id;
    char *pcf_group_id;
    char *routing_indicator;
    bool is_h_nw_pub_key_id;
    int h_nw_pub_key_id;
    OpenAPI_list_t *group_list;
    char *drx_parameter;
    bool is_sub_rfsp;
    int sub_rfsp;
    bool is_used_rfsp;
    int used_rfsp;
    struct OpenAPI_ambr_s *sub_ue_ambr;
    OpenAPI_list_t* sub_ue_slice_mbr_list;
    char *smsf_id;
    struct OpenAPI_seaf_data_s *seaf_data;
    char *_5g_mm_capability;
    char *pcf_id;
    char *pcf_set_id;
    char *pcf_amp_service_set_id;
    char *pcf_uep_service_set_id;
    OpenAPI_sbi_binding_level_e pcf_binding;
    char *pcf_am_policy_uri;
    OpenAPI_list_t *am_policy_req_trigger_list;
    char *pcf_ue_policy_uri;
    OpenAPI_list_t *ue_policy_req_trigger_list;
    char *hpcf_id;
    char *hpcf_set_id;
    OpenAPI_list_t *restricted_rat_list;
    OpenAPI_list_t *forbidden_area_list;
    struct OpenAPI_service_area_restriction_s *service_area_restriction;
    OpenAPI_list_t *restricted_core_nw_type_list;
    OpenAPI_list_t *event_subscription_list;
    OpenAPI_list_t *mm_context_list;
    OpenAPI_list_t *session_context_list;
    struct OpenAPI_eps_interworking_info_s *eps_interworking_info;
    bool is_trace_data_null;
    struct OpenAPI_trace_data_s *trace_data;
    char *service_gap_expiry_time;
    char *stn_sr;
    char *c_msisdn;
    char *ms_classmark2;
    OpenAPI_list_t *supported_codec_list;
    OpenAPI_list_t *small_data_rate_status_infos;
    OpenAPI_list_t *restricted_primary_rat_list;
    OpenAPI_list_t *restricted_secondary_rat_list;
    struct OpenAPI_v2x_context_s *v2x_context;
    bool is_lte_cat_m_ind;
    int lte_cat_m_ind;
    bool is_red_cap_ind;
    int red_cap_ind;
    struct OpenAPI_mo_exp_data_counter_s *mo_exp_data_counter;
    struct OpenAPI_cag_data_s *cag_data;
    bool is_management_mdt_ind;
    int management_mdt_ind;
    struct OpenAPI_immediate_mdt_conf_s *immediate_mdt_conf;
    struct OpenAPI_ec_restriction_data_wb_s *ec_restriction_data_wb;
    bool is_ec_restriction_data_nb;
    int ec_restriction_data_nb;
    bool is_iab_operation_allowed;
    int iab_operation_allowed;
    struct OpenAPI_prose_context_s *prose_context;
    OpenAPI_list_t *analytics_subscription_list;
    char *pcf_amp_binding_info;
    char *pcf_uep_binding_info;
    struct OpenAPI_service_area_restriction_s *used_service_area_restriction;
    OpenAPI_list_t* pra_in_am_policy;
    OpenAPI_list_t* pra_in_ue_policy;
    struct OpenAPI_updp_subscription_data_s *updp_subscription_data;
    OpenAPI_list_t *sm_policy_notify_pdu_list;
    bool is_pcf_ue_callback_info_null;
    struct OpenAPI_pcf_ue_callback_info_s *pcf_ue_callback_info;
    char *ue_positioning_cap;
    bool is_asti_distribution_indication;
    int asti_distribution_indication;
    bool is_ts_error_budget;
    int ts_error_budget;
    bool is_snpn_onboard_ind;
    int snpn_onboard_ind;
    bool is_smf_sel_info_null;
    struct OpenAPI_smf_selection_data_s *smf_sel_info;
    OpenAPI_list_t* pcf_ue_slice_mbr_list;
    char *smsf_set_id;
    char *smsf_service_set_id;
    char *smsf_binding_info;
    bool is_disaster_roaming_ind;
    int disaster_roaming_ind;
    struct OpenAPI_plmn_id_s *disaster_plmn;
} OpenAPI_ue_context_t;

OpenAPI_ue_context_t *OpenAPI_ue_context_create(
    char *supi,
    bool is_supi_unauth_ind,
    int supi_unauth_ind,
    OpenAPI_list_t *gpsi_list,
    char *pei,
    char *udm_group_id,
    char *ausf_group_id,
    char *pcf_group_id,
    char *routing_indicator,
    bool is_h_nw_pub_key_id,
    int h_nw_pub_key_id,
    OpenAPI_list_t *group_list,
    char *drx_parameter,
    bool is_sub_rfsp,
    int sub_rfsp,
    bool is_used_rfsp,
    int used_rfsp,
    OpenAPI_ambr_t *sub_ue_ambr,
    OpenAPI_list_t* sub_ue_slice_mbr_list,
    char *smsf_id,
    OpenAPI_seaf_data_t *seaf_data,
    char *_5g_mm_capability,
    char *pcf_id,
    char *pcf_set_id,
    char *pcf_amp_service_set_id,
    char *pcf_uep_service_set_id,
    OpenAPI_sbi_binding_level_e pcf_binding,
    char *pcf_am_policy_uri,
    OpenAPI_list_t *am_policy_req_trigger_list,
    char *pcf_ue_policy_uri,
    OpenAPI_list_t *ue_policy_req_trigger_list,
    char *hpcf_id,
    char *hpcf_set_id,
    OpenAPI_list_t *restricted_rat_list,
    OpenAPI_list_t *forbidden_area_list,
    OpenAPI_service_area_restriction_t *service_area_restriction,
    OpenAPI_list_t *restricted_core_nw_type_list,
    OpenAPI_list_t *event_subscription_list,
    OpenAPI_list_t *mm_context_list,
    OpenAPI_list_t *session_context_list,
    OpenAPI_eps_interworking_info_t *eps_interworking_info,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    char *service_gap_expiry_time,
    char *stn_sr,
    char *c_msisdn,
    char *ms_classmark2,
    OpenAPI_list_t *supported_codec_list,
    OpenAPI_list_t *small_data_rate_status_infos,
    OpenAPI_list_t *restricted_primary_rat_list,
    OpenAPI_list_t *restricted_secondary_rat_list,
    OpenAPI_v2x_context_t *v2x_context,
    bool is_lte_cat_m_ind,
    int lte_cat_m_ind,
    bool is_red_cap_ind,
    int red_cap_ind,
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter,
    OpenAPI_cag_data_t *cag_data,
    bool is_management_mdt_ind,
    int management_mdt_ind,
    OpenAPI_immediate_mdt_conf_t *immediate_mdt_conf,
    OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb,
    bool is_ec_restriction_data_nb,
    int ec_restriction_data_nb,
    bool is_iab_operation_allowed,
    int iab_operation_allowed,
    OpenAPI_prose_context_t *prose_context,
    OpenAPI_list_t *analytics_subscription_list,
    char *pcf_amp_binding_info,
    char *pcf_uep_binding_info,
    OpenAPI_service_area_restriction_t *used_service_area_restriction,
    OpenAPI_list_t* pra_in_am_policy,
    OpenAPI_list_t* pra_in_ue_policy,
    OpenAPI_updp_subscription_data_t *updp_subscription_data,
    OpenAPI_list_t *sm_policy_notify_pdu_list,
    bool is_pcf_ue_callback_info_null,
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info,
    char *ue_positioning_cap,
    bool is_asti_distribution_indication,
    int asti_distribution_indication,
    bool is_ts_error_budget,
    int ts_error_budget,
    bool is_snpn_onboard_ind,
    int snpn_onboard_ind,
    bool is_smf_sel_info_null,
    OpenAPI_smf_selection_data_t *smf_sel_info,
    OpenAPI_list_t* pcf_ue_slice_mbr_list,
    char *smsf_set_id,
    char *smsf_service_set_id,
    char *smsf_binding_info,
    bool is_disaster_roaming_ind,
    int disaster_roaming_ind,
    OpenAPI_plmn_id_t *disaster_plmn
);
void OpenAPI_ue_context_free(OpenAPI_ue_context_t *ue_context);
OpenAPI_ue_context_t *OpenAPI_ue_context_parseFromJSON(cJSON *ue_contextJSON);
cJSON *OpenAPI_ue_context_convertToJSON(OpenAPI_ue_context_t *ue_context);
OpenAPI_ue_context_t *OpenAPI_ue_context_copy(OpenAPI_ue_context_t *dst, OpenAPI_ue_context_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_H_ */

