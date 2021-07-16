/*
 * ue_context.h
 *
 * 
 */

#ifndef _OpenAPI_ue_context_H_
#define _OpenAPI_ue_context_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ambr.h"
#include "area.h"
#include "cag_data.h"
#include "core_network_type.h"
#include "ec_restriction_data_wb.h"
#include "ext_amf_event_subscription.h"
#include "immediate_mdt_conf.h"
#include "mm_context.h"
#include "mo_exp_data_counter.h"
#include "pdu_session_context.h"
#include "policy_req_trigger.h"
#include "rat_type.h"
#include "sbi_binding_level.h"
#include "seaf_data.h"
#include "service_area_restriction.h"
#include "small_data_rate_status_info.h"
#include "trace_data.h"
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
    OpenAPI_list_t *group_list;
    char drx_parameter;
    bool is_sub_rfsp;
    int sub_rfsp;
    bool is_used_rfsp;
    int used_rfsp;
    struct OpenAPI_ambr_s *sub_ue_ambr;
    char *smsf_id;
    struct OpenAPI_seaf_data_s *seaf_data;
    char _5g_mm_capability;
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
    struct OpenAPI_trace_data_s *trace_data;
    char *service_gap_expiry_time;
    char *stn_sr;
    char *c_msisdn;
    char ms_classmark2;
    OpenAPI_list_t *supported_codec_list;
    OpenAPI_list_t *small_data_rate_status_infos;
    OpenAPI_list_t *restricted_primary_rat_list;
    OpenAPI_list_t *restricted_secondary_rat_list;
    struct OpenAPI_v2x_context_s *v2x_context;
    bool is_lte_cat_m_ind;
    int lte_cat_m_ind;
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
    OpenAPI_list_t *group_list,
    char drx_parameter,
    bool is_sub_rfsp,
    int sub_rfsp,
    bool is_used_rfsp,
    int used_rfsp,
    OpenAPI_ambr_t *sub_ue_ambr,
    char *smsf_id,
    OpenAPI_seaf_data_t *seaf_data,
    char _5g_mm_capability,
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
    OpenAPI_trace_data_t *trace_data,
    char *service_gap_expiry_time,
    char *stn_sr,
    char *c_msisdn,
    char ms_classmark2,
    OpenAPI_list_t *supported_codec_list,
    OpenAPI_list_t *small_data_rate_status_infos,
    OpenAPI_list_t *restricted_primary_rat_list,
    OpenAPI_list_t *restricted_secondary_rat_list,
    OpenAPI_v2x_context_t *v2x_context,
    bool is_lte_cat_m_ind,
    int lte_cat_m_ind,
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter,
    OpenAPI_cag_data_t *cag_data,
    bool is_management_mdt_ind,
    int management_mdt_ind,
    OpenAPI_immediate_mdt_conf_t *immediate_mdt_conf,
    OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb,
    bool is_ec_restriction_data_nb,
    int ec_restriction_data_nb,
    bool is_iab_operation_allowed,
    int iab_operation_allowed
);
void OpenAPI_ue_context_free(OpenAPI_ue_context_t *ue_context);
OpenAPI_ue_context_t *OpenAPI_ue_context_parseFromJSON(cJSON *ue_contextJSON);
cJSON *OpenAPI_ue_context_convertToJSON(OpenAPI_ue_context_t *ue_context);
OpenAPI_ue_context_t *OpenAPI_ue_context_copy(OpenAPI_ue_context_t *dst, OpenAPI_ue_context_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_H_ */

