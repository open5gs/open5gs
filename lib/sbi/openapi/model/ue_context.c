
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context.h"

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
)
{
    OpenAPI_ue_context_t *ue_context_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_t));
    ogs_assert(ue_context_local_var);

    ue_context_local_var->supi = supi;
    ue_context_local_var->is_supi_unauth_ind = is_supi_unauth_ind;
    ue_context_local_var->supi_unauth_ind = supi_unauth_ind;
    ue_context_local_var->gpsi_list = gpsi_list;
    ue_context_local_var->pei = pei;
    ue_context_local_var->udm_group_id = udm_group_id;
    ue_context_local_var->ausf_group_id = ausf_group_id;
    ue_context_local_var->pcf_group_id = pcf_group_id;
    ue_context_local_var->routing_indicator = routing_indicator;
    ue_context_local_var->is_h_nw_pub_key_id = is_h_nw_pub_key_id;
    ue_context_local_var->h_nw_pub_key_id = h_nw_pub_key_id;
    ue_context_local_var->group_list = group_list;
    ue_context_local_var->drx_parameter = drx_parameter;
    ue_context_local_var->is_sub_rfsp = is_sub_rfsp;
    ue_context_local_var->sub_rfsp = sub_rfsp;
    ue_context_local_var->is_used_rfsp = is_used_rfsp;
    ue_context_local_var->used_rfsp = used_rfsp;
    ue_context_local_var->sub_ue_ambr = sub_ue_ambr;
    ue_context_local_var->sub_ue_slice_mbr_list = sub_ue_slice_mbr_list;
    ue_context_local_var->smsf_id = smsf_id;
    ue_context_local_var->seaf_data = seaf_data;
    ue_context_local_var->_5g_mm_capability = _5g_mm_capability;
    ue_context_local_var->pcf_id = pcf_id;
    ue_context_local_var->pcf_set_id = pcf_set_id;
    ue_context_local_var->pcf_amp_service_set_id = pcf_amp_service_set_id;
    ue_context_local_var->pcf_uep_service_set_id = pcf_uep_service_set_id;
    ue_context_local_var->pcf_binding = pcf_binding;
    ue_context_local_var->pcf_am_policy_uri = pcf_am_policy_uri;
    ue_context_local_var->am_policy_req_trigger_list = am_policy_req_trigger_list;
    ue_context_local_var->pcf_ue_policy_uri = pcf_ue_policy_uri;
    ue_context_local_var->ue_policy_req_trigger_list = ue_policy_req_trigger_list;
    ue_context_local_var->hpcf_id = hpcf_id;
    ue_context_local_var->hpcf_set_id = hpcf_set_id;
    ue_context_local_var->restricted_rat_list = restricted_rat_list;
    ue_context_local_var->forbidden_area_list = forbidden_area_list;
    ue_context_local_var->service_area_restriction = service_area_restriction;
    ue_context_local_var->restricted_core_nw_type_list = restricted_core_nw_type_list;
    ue_context_local_var->event_subscription_list = event_subscription_list;
    ue_context_local_var->mm_context_list = mm_context_list;
    ue_context_local_var->session_context_list = session_context_list;
    ue_context_local_var->eps_interworking_info = eps_interworking_info;
    ue_context_local_var->is_trace_data_null = is_trace_data_null;
    ue_context_local_var->trace_data = trace_data;
    ue_context_local_var->service_gap_expiry_time = service_gap_expiry_time;
    ue_context_local_var->stn_sr = stn_sr;
    ue_context_local_var->c_msisdn = c_msisdn;
    ue_context_local_var->ms_classmark2 = ms_classmark2;
    ue_context_local_var->supported_codec_list = supported_codec_list;
    ue_context_local_var->small_data_rate_status_infos = small_data_rate_status_infos;
    ue_context_local_var->restricted_primary_rat_list = restricted_primary_rat_list;
    ue_context_local_var->restricted_secondary_rat_list = restricted_secondary_rat_list;
    ue_context_local_var->v2x_context = v2x_context;
    ue_context_local_var->is_lte_cat_m_ind = is_lte_cat_m_ind;
    ue_context_local_var->lte_cat_m_ind = lte_cat_m_ind;
    ue_context_local_var->is_red_cap_ind = is_red_cap_ind;
    ue_context_local_var->red_cap_ind = red_cap_ind;
    ue_context_local_var->mo_exp_data_counter = mo_exp_data_counter;
    ue_context_local_var->cag_data = cag_data;
    ue_context_local_var->is_management_mdt_ind = is_management_mdt_ind;
    ue_context_local_var->management_mdt_ind = management_mdt_ind;
    ue_context_local_var->immediate_mdt_conf = immediate_mdt_conf;
    ue_context_local_var->ec_restriction_data_wb = ec_restriction_data_wb;
    ue_context_local_var->is_ec_restriction_data_nb = is_ec_restriction_data_nb;
    ue_context_local_var->ec_restriction_data_nb = ec_restriction_data_nb;
    ue_context_local_var->is_iab_operation_allowed = is_iab_operation_allowed;
    ue_context_local_var->iab_operation_allowed = iab_operation_allowed;
    ue_context_local_var->prose_context = prose_context;
    ue_context_local_var->analytics_subscription_list = analytics_subscription_list;
    ue_context_local_var->pcf_amp_binding_info = pcf_amp_binding_info;
    ue_context_local_var->pcf_uep_binding_info = pcf_uep_binding_info;
    ue_context_local_var->used_service_area_restriction = used_service_area_restriction;
    ue_context_local_var->pra_in_am_policy = pra_in_am_policy;
    ue_context_local_var->pra_in_ue_policy = pra_in_ue_policy;
    ue_context_local_var->updp_subscription_data = updp_subscription_data;
    ue_context_local_var->sm_policy_notify_pdu_list = sm_policy_notify_pdu_list;
    ue_context_local_var->is_pcf_ue_callback_info_null = is_pcf_ue_callback_info_null;
    ue_context_local_var->pcf_ue_callback_info = pcf_ue_callback_info;
    ue_context_local_var->ue_positioning_cap = ue_positioning_cap;
    ue_context_local_var->is_asti_distribution_indication = is_asti_distribution_indication;
    ue_context_local_var->asti_distribution_indication = asti_distribution_indication;
    ue_context_local_var->is_ts_error_budget = is_ts_error_budget;
    ue_context_local_var->ts_error_budget = ts_error_budget;
    ue_context_local_var->is_snpn_onboard_ind = is_snpn_onboard_ind;
    ue_context_local_var->snpn_onboard_ind = snpn_onboard_ind;
    ue_context_local_var->is_smf_sel_info_null = is_smf_sel_info_null;
    ue_context_local_var->smf_sel_info = smf_sel_info;
    ue_context_local_var->pcf_ue_slice_mbr_list = pcf_ue_slice_mbr_list;
    ue_context_local_var->smsf_set_id = smsf_set_id;
    ue_context_local_var->smsf_service_set_id = smsf_service_set_id;
    ue_context_local_var->smsf_binding_info = smsf_binding_info;
    ue_context_local_var->is_disaster_roaming_ind = is_disaster_roaming_ind;
    ue_context_local_var->disaster_roaming_ind = disaster_roaming_ind;
    ue_context_local_var->disaster_plmn = disaster_plmn;

    return ue_context_local_var;
}

void OpenAPI_ue_context_free(OpenAPI_ue_context_t *ue_context)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_context) {
        return;
    }
    if (ue_context->supi) {
        ogs_free(ue_context->supi);
        ue_context->supi = NULL;
    }
    if (ue_context->gpsi_list) {
        OpenAPI_list_for_each(ue_context->gpsi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_context->gpsi_list);
        ue_context->gpsi_list = NULL;
    }
    if (ue_context->pei) {
        ogs_free(ue_context->pei);
        ue_context->pei = NULL;
    }
    if (ue_context->udm_group_id) {
        ogs_free(ue_context->udm_group_id);
        ue_context->udm_group_id = NULL;
    }
    if (ue_context->ausf_group_id) {
        ogs_free(ue_context->ausf_group_id);
        ue_context->ausf_group_id = NULL;
    }
    if (ue_context->pcf_group_id) {
        ogs_free(ue_context->pcf_group_id);
        ue_context->pcf_group_id = NULL;
    }
    if (ue_context->routing_indicator) {
        ogs_free(ue_context->routing_indicator);
        ue_context->routing_indicator = NULL;
    }
    if (ue_context->group_list) {
        OpenAPI_list_for_each(ue_context->group_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_context->group_list);
        ue_context->group_list = NULL;
    }
    if (ue_context->drx_parameter) {
        ogs_free(ue_context->drx_parameter);
        ue_context->drx_parameter = NULL;
    }
    if (ue_context->sub_ue_ambr) {
        OpenAPI_ambr_free(ue_context->sub_ue_ambr);
        ue_context->sub_ue_ambr = NULL;
    }
    if (ue_context->sub_ue_slice_mbr_list) {
        OpenAPI_list_for_each(ue_context->sub_ue_slice_mbr_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_slice_mbr_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_context->sub_ue_slice_mbr_list);
        ue_context->sub_ue_slice_mbr_list = NULL;
    }
    if (ue_context->smsf_id) {
        ogs_free(ue_context->smsf_id);
        ue_context->smsf_id = NULL;
    }
    if (ue_context->seaf_data) {
        OpenAPI_seaf_data_free(ue_context->seaf_data);
        ue_context->seaf_data = NULL;
    }
    if (ue_context->_5g_mm_capability) {
        ogs_free(ue_context->_5g_mm_capability);
        ue_context->_5g_mm_capability = NULL;
    }
    if (ue_context->pcf_id) {
        ogs_free(ue_context->pcf_id);
        ue_context->pcf_id = NULL;
    }
    if (ue_context->pcf_set_id) {
        ogs_free(ue_context->pcf_set_id);
        ue_context->pcf_set_id = NULL;
    }
    if (ue_context->pcf_amp_service_set_id) {
        ogs_free(ue_context->pcf_amp_service_set_id);
        ue_context->pcf_amp_service_set_id = NULL;
    }
    if (ue_context->pcf_uep_service_set_id) {
        ogs_free(ue_context->pcf_uep_service_set_id);
        ue_context->pcf_uep_service_set_id = NULL;
    }
    if (ue_context->pcf_am_policy_uri) {
        ogs_free(ue_context->pcf_am_policy_uri);
        ue_context->pcf_am_policy_uri = NULL;
    }
    if (ue_context->am_policy_req_trigger_list) {
        OpenAPI_list_free(ue_context->am_policy_req_trigger_list);
        ue_context->am_policy_req_trigger_list = NULL;
    }
    if (ue_context->pcf_ue_policy_uri) {
        ogs_free(ue_context->pcf_ue_policy_uri);
        ue_context->pcf_ue_policy_uri = NULL;
    }
    if (ue_context->ue_policy_req_trigger_list) {
        OpenAPI_list_free(ue_context->ue_policy_req_trigger_list);
        ue_context->ue_policy_req_trigger_list = NULL;
    }
    if (ue_context->hpcf_id) {
        ogs_free(ue_context->hpcf_id);
        ue_context->hpcf_id = NULL;
    }
    if (ue_context->hpcf_set_id) {
        ogs_free(ue_context->hpcf_set_id);
        ue_context->hpcf_set_id = NULL;
    }
    if (ue_context->restricted_rat_list) {
        OpenAPI_list_free(ue_context->restricted_rat_list);
        ue_context->restricted_rat_list = NULL;
    }
    if (ue_context->forbidden_area_list) {
        OpenAPI_list_for_each(ue_context->forbidden_area_list, node) {
            OpenAPI_area_free(node->data);
        }
        OpenAPI_list_free(ue_context->forbidden_area_list);
        ue_context->forbidden_area_list = NULL;
    }
    if (ue_context->service_area_restriction) {
        OpenAPI_service_area_restriction_free(ue_context->service_area_restriction);
        ue_context->service_area_restriction = NULL;
    }
    if (ue_context->restricted_core_nw_type_list) {
        OpenAPI_list_free(ue_context->restricted_core_nw_type_list);
        ue_context->restricted_core_nw_type_list = NULL;
    }
    if (ue_context->event_subscription_list) {
        OpenAPI_list_for_each(ue_context->event_subscription_list, node) {
            OpenAPI_ext_amf_event_subscription_free(node->data);
        }
        OpenAPI_list_free(ue_context->event_subscription_list);
        ue_context->event_subscription_list = NULL;
    }
    if (ue_context->mm_context_list) {
        OpenAPI_list_for_each(ue_context->mm_context_list, node) {
            OpenAPI_mm_context_free(node->data);
        }
        OpenAPI_list_free(ue_context->mm_context_list);
        ue_context->mm_context_list = NULL;
    }
    if (ue_context->session_context_list) {
        OpenAPI_list_for_each(ue_context->session_context_list, node) {
            OpenAPI_pdu_session_context_free(node->data);
        }
        OpenAPI_list_free(ue_context->session_context_list);
        ue_context->session_context_list = NULL;
    }
    if (ue_context->eps_interworking_info) {
        OpenAPI_eps_interworking_info_free(ue_context->eps_interworking_info);
        ue_context->eps_interworking_info = NULL;
    }
    if (ue_context->trace_data) {
        OpenAPI_trace_data_free(ue_context->trace_data);
        ue_context->trace_data = NULL;
    }
    if (ue_context->service_gap_expiry_time) {
        ogs_free(ue_context->service_gap_expiry_time);
        ue_context->service_gap_expiry_time = NULL;
    }
    if (ue_context->stn_sr) {
        ogs_free(ue_context->stn_sr);
        ue_context->stn_sr = NULL;
    }
    if (ue_context->c_msisdn) {
        ogs_free(ue_context->c_msisdn);
        ue_context->c_msisdn = NULL;
    }
    if (ue_context->ms_classmark2) {
        ogs_free(ue_context->ms_classmark2);
        ue_context->ms_classmark2 = NULL;
    }
    if (ue_context->supported_codec_list) {
        OpenAPI_list_for_each(ue_context->supported_codec_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_context->supported_codec_list);
        ue_context->supported_codec_list = NULL;
    }
    if (ue_context->small_data_rate_status_infos) {
        OpenAPI_list_for_each(ue_context->small_data_rate_status_infos, node) {
            OpenAPI_small_data_rate_status_info_free(node->data);
        }
        OpenAPI_list_free(ue_context->small_data_rate_status_infos);
        ue_context->small_data_rate_status_infos = NULL;
    }
    if (ue_context->restricted_primary_rat_list) {
        OpenAPI_list_free(ue_context->restricted_primary_rat_list);
        ue_context->restricted_primary_rat_list = NULL;
    }
    if (ue_context->restricted_secondary_rat_list) {
        OpenAPI_list_free(ue_context->restricted_secondary_rat_list);
        ue_context->restricted_secondary_rat_list = NULL;
    }
    if (ue_context->v2x_context) {
        OpenAPI_v2x_context_free(ue_context->v2x_context);
        ue_context->v2x_context = NULL;
    }
    if (ue_context->mo_exp_data_counter) {
        OpenAPI_mo_exp_data_counter_free(ue_context->mo_exp_data_counter);
        ue_context->mo_exp_data_counter = NULL;
    }
    if (ue_context->cag_data) {
        OpenAPI_cag_data_free(ue_context->cag_data);
        ue_context->cag_data = NULL;
    }
    if (ue_context->immediate_mdt_conf) {
        OpenAPI_immediate_mdt_conf_free(ue_context->immediate_mdt_conf);
        ue_context->immediate_mdt_conf = NULL;
    }
    if (ue_context->ec_restriction_data_wb) {
        OpenAPI_ec_restriction_data_wb_free(ue_context->ec_restriction_data_wb);
        ue_context->ec_restriction_data_wb = NULL;
    }
    if (ue_context->prose_context) {
        OpenAPI_prose_context_free(ue_context->prose_context);
        ue_context->prose_context = NULL;
    }
    if (ue_context->analytics_subscription_list) {
        OpenAPI_list_for_each(ue_context->analytics_subscription_list, node) {
            OpenAPI_analytics_subscription_free(node->data);
        }
        OpenAPI_list_free(ue_context->analytics_subscription_list);
        ue_context->analytics_subscription_list = NULL;
    }
    if (ue_context->pcf_amp_binding_info) {
        ogs_free(ue_context->pcf_amp_binding_info);
        ue_context->pcf_amp_binding_info = NULL;
    }
    if (ue_context->pcf_uep_binding_info) {
        ogs_free(ue_context->pcf_uep_binding_info);
        ue_context->pcf_uep_binding_info = NULL;
    }
    if (ue_context->used_service_area_restriction) {
        OpenAPI_service_area_restriction_free(ue_context->used_service_area_restriction);
        ue_context->used_service_area_restriction = NULL;
    }
    if (ue_context->pra_in_am_policy) {
        OpenAPI_list_for_each(ue_context->pra_in_am_policy, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_context->pra_in_am_policy);
        ue_context->pra_in_am_policy = NULL;
    }
    if (ue_context->pra_in_ue_policy) {
        OpenAPI_list_for_each(ue_context->pra_in_ue_policy, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_context->pra_in_ue_policy);
        ue_context->pra_in_ue_policy = NULL;
    }
    if (ue_context->updp_subscription_data) {
        OpenAPI_updp_subscription_data_free(ue_context->updp_subscription_data);
        ue_context->updp_subscription_data = NULL;
    }
    if (ue_context->sm_policy_notify_pdu_list) {
        OpenAPI_list_for_each(ue_context->sm_policy_notify_pdu_list, node) {
            OpenAPI_pdu_session_info_free(node->data);
        }
        OpenAPI_list_free(ue_context->sm_policy_notify_pdu_list);
        ue_context->sm_policy_notify_pdu_list = NULL;
    }
    if (ue_context->pcf_ue_callback_info) {
        OpenAPI_pcf_ue_callback_info_free(ue_context->pcf_ue_callback_info);
        ue_context->pcf_ue_callback_info = NULL;
    }
    if (ue_context->ue_positioning_cap) {
        ogs_free(ue_context->ue_positioning_cap);
        ue_context->ue_positioning_cap = NULL;
    }
    if (ue_context->smf_sel_info) {
        OpenAPI_smf_selection_data_free(ue_context->smf_sel_info);
        ue_context->smf_sel_info = NULL;
    }
    if (ue_context->pcf_ue_slice_mbr_list) {
        OpenAPI_list_for_each(ue_context->pcf_ue_slice_mbr_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_slice_mbr_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_context->pcf_ue_slice_mbr_list);
        ue_context->pcf_ue_slice_mbr_list = NULL;
    }
    if (ue_context->smsf_set_id) {
        ogs_free(ue_context->smsf_set_id);
        ue_context->smsf_set_id = NULL;
    }
    if (ue_context->smsf_service_set_id) {
        ogs_free(ue_context->smsf_service_set_id);
        ue_context->smsf_service_set_id = NULL;
    }
    if (ue_context->smsf_binding_info) {
        ogs_free(ue_context->smsf_binding_info);
        ue_context->smsf_binding_info = NULL;
    }
    if (ue_context->disaster_plmn) {
        OpenAPI_plmn_id_free(ue_context->disaster_plmn);
        ue_context->disaster_plmn = NULL;
    }
    ogs_free(ue_context);
}

cJSON *OpenAPI_ue_context_convertToJSON(OpenAPI_ue_context_t *ue_context)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_context == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [UeContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_context->supi) {
    if (cJSON_AddStringToObject(item, "supi", ue_context->supi) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (ue_context->is_supi_unauth_ind) {
    if (cJSON_AddBoolToObject(item, "supiUnauthInd", ue_context->supi_unauth_ind) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [supi_unauth_ind]");
        goto end;
    }
    }

    if (ue_context->gpsi_list) {
    cJSON *gpsi_listList = cJSON_AddArrayToObject(item, "gpsiList");
    if (gpsi_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [gpsi_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->gpsi_list, node) {
        if (cJSON_AddStringToObject(gpsi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [gpsi_list]");
            goto end;
        }
    }
    }

    if (ue_context->pei) {
    if (cJSON_AddStringToObject(item, "pei", ue_context->pei) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (ue_context->udm_group_id) {
    if (cJSON_AddStringToObject(item, "udmGroupId", ue_context->udm_group_id) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [udm_group_id]");
        goto end;
    }
    }

    if (ue_context->ausf_group_id) {
    if (cJSON_AddStringToObject(item, "ausfGroupId", ue_context->ausf_group_id) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [ausf_group_id]");
        goto end;
    }
    }

    if (ue_context->pcf_group_id) {
    if (cJSON_AddStringToObject(item, "pcfGroupId", ue_context->pcf_group_id) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_group_id]");
        goto end;
    }
    }

    if (ue_context->routing_indicator) {
    if (cJSON_AddStringToObject(item, "routingIndicator", ue_context->routing_indicator) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [routing_indicator]");
        goto end;
    }
    }

    if (ue_context->is_h_nw_pub_key_id) {
    if (cJSON_AddNumberToObject(item, "hNwPubKeyId", ue_context->h_nw_pub_key_id) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [h_nw_pub_key_id]");
        goto end;
    }
    }

    if (ue_context->group_list) {
    cJSON *group_listList = cJSON_AddArrayToObject(item, "groupList");
    if (group_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [group_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->group_list, node) {
        if (cJSON_AddStringToObject(group_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [group_list]");
            goto end;
        }
    }
    }

    if (ue_context->drx_parameter) {
    if (cJSON_AddStringToObject(item, "drxParameter", ue_context->drx_parameter) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [drx_parameter]");
        goto end;
    }
    }

    if (ue_context->is_sub_rfsp) {
    if (cJSON_AddNumberToObject(item, "subRfsp", ue_context->sub_rfsp) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [sub_rfsp]");
        goto end;
    }
    }

    if (ue_context->is_used_rfsp) {
    if (cJSON_AddNumberToObject(item, "usedRfsp", ue_context->used_rfsp) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [used_rfsp]");
        goto end;
    }
    }

    if (ue_context->sub_ue_ambr) {
    cJSON *sub_ue_ambr_local_JSON = OpenAPI_ambr_convertToJSON(ue_context->sub_ue_ambr);
    if (sub_ue_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [sub_ue_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "subUeAmbr", sub_ue_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [sub_ue_ambr]");
        goto end;
    }
    }

    if (ue_context->sub_ue_slice_mbr_list) {
    cJSON *sub_ue_slice_mbr_list = cJSON_AddObjectToObject(item, "subUeSliceMbrList");
    if (sub_ue_slice_mbr_list == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [sub_ue_slice_mbr_list]");
        goto end;
    }
    cJSON *localMapObject = sub_ue_slice_mbr_list;
    if (ue_context->sub_ue_slice_mbr_list) {
        OpenAPI_list_for_each(ue_context->sub_ue_slice_mbr_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [sub_ue_slice_mbr_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [sub_ue_slice_mbr_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_slice_mbr_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (ue_context->smsf_id) {
    if (cJSON_AddStringToObject(item, "smsfId", ue_context->smsf_id) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [smsf_id]");
        goto end;
    }
    }

    if (ue_context->seaf_data) {
    cJSON *seaf_data_local_JSON = OpenAPI_seaf_data_convertToJSON(ue_context->seaf_data);
    if (seaf_data_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [seaf_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "seafData", seaf_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [seaf_data]");
        goto end;
    }
    }

    if (ue_context->_5g_mm_capability) {
    if (cJSON_AddStringToObject(item, "5gMmCapability", ue_context->_5g_mm_capability) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [_5g_mm_capability]");
        goto end;
    }
    }

    if (ue_context->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", ue_context->pcf_id) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (ue_context->pcf_set_id) {
    if (cJSON_AddStringToObject(item, "pcfSetId", ue_context->pcf_set_id) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    if (ue_context->pcf_amp_service_set_id) {
    if (cJSON_AddStringToObject(item, "pcfAmpServiceSetId", ue_context->pcf_amp_service_set_id) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_amp_service_set_id]");
        goto end;
    }
    }

    if (ue_context->pcf_uep_service_set_id) {
    if (cJSON_AddStringToObject(item, "pcfUepServiceSetId", ue_context->pcf_uep_service_set_id) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_uep_service_set_id]");
        goto end;
    }
    }

    if (ue_context->pcf_binding != OpenAPI_sbi_binding_level_NULL) {
    if (cJSON_AddStringToObject(item, "pcfBinding", OpenAPI_sbi_binding_level_ToString(ue_context->pcf_binding)) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_binding]");
        goto end;
    }
    }

    if (ue_context->pcf_am_policy_uri) {
    if (cJSON_AddStringToObject(item, "pcfAmPolicyUri", ue_context->pcf_am_policy_uri) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_am_policy_uri]");
        goto end;
    }
    }

    if (ue_context->am_policy_req_trigger_list != OpenAPI_policy_req_trigger_NULL) {
    cJSON *am_policy_req_trigger_listList = cJSON_AddArrayToObject(item, "amPolicyReqTriggerList");
    if (am_policy_req_trigger_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [am_policy_req_trigger_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->am_policy_req_trigger_list, node) {
        if (cJSON_AddStringToObject(am_policy_req_trigger_listList, "", OpenAPI_policy_req_trigger_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [am_policy_req_trigger_list]");
            goto end;
        }
    }
    }

    if (ue_context->pcf_ue_policy_uri) {
    if (cJSON_AddStringToObject(item, "pcfUePolicyUri", ue_context->pcf_ue_policy_uri) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_ue_policy_uri]");
        goto end;
    }
    }

    if (ue_context->ue_policy_req_trigger_list != OpenAPI_policy_req_trigger_NULL) {
    cJSON *ue_policy_req_trigger_listList = cJSON_AddArrayToObject(item, "uePolicyReqTriggerList");
    if (ue_policy_req_trigger_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [ue_policy_req_trigger_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->ue_policy_req_trigger_list, node) {
        if (cJSON_AddStringToObject(ue_policy_req_trigger_listList, "", OpenAPI_policy_req_trigger_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [ue_policy_req_trigger_list]");
            goto end;
        }
    }
    }

    if (ue_context->hpcf_id) {
    if (cJSON_AddStringToObject(item, "hpcfId", ue_context->hpcf_id) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [hpcf_id]");
        goto end;
    }
    }

    if (ue_context->hpcf_set_id) {
    if (cJSON_AddStringToObject(item, "hpcfSetId", ue_context->hpcf_set_id) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [hpcf_set_id]");
        goto end;
    }
    }

    if (ue_context->restricted_rat_list != OpenAPI_rat_type_NULL) {
    cJSON *restricted_rat_listList = cJSON_AddArrayToObject(item, "restrictedRatList");
    if (restricted_rat_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_rat_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->restricted_rat_list, node) {
        if (cJSON_AddStringToObject(restricted_rat_listList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_rat_list]");
            goto end;
        }
    }
    }

    if (ue_context->forbidden_area_list) {
    cJSON *forbidden_area_listList = cJSON_AddArrayToObject(item, "forbiddenAreaList");
    if (forbidden_area_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [forbidden_area_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->forbidden_area_list, node) {
        cJSON *itemLocal = OpenAPI_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [forbidden_area_list]");
            goto end;
        }
        cJSON_AddItemToArray(forbidden_area_listList, itemLocal);
    }
    }

    if (ue_context->service_area_restriction) {
    cJSON *service_area_restriction_local_JSON = OpenAPI_service_area_restriction_convertToJSON(ue_context->service_area_restriction);
    if (service_area_restriction_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [service_area_restriction]");
        goto end;
    }
    cJSON_AddItemToObject(item, "serviceAreaRestriction", service_area_restriction_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [service_area_restriction]");
        goto end;
    }
    }

    if (ue_context->restricted_core_nw_type_list != OpenAPI_core_network_type_NULL) {
    cJSON *restricted_core_nw_type_listList = cJSON_AddArrayToObject(item, "restrictedCoreNwTypeList");
    if (restricted_core_nw_type_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_core_nw_type_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->restricted_core_nw_type_list, node) {
        if (cJSON_AddStringToObject(restricted_core_nw_type_listList, "", OpenAPI_core_network_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_core_nw_type_list]");
            goto end;
        }
    }
    }

    if (ue_context->event_subscription_list) {
    cJSON *event_subscription_listList = cJSON_AddArrayToObject(item, "eventSubscriptionList");
    if (event_subscription_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [event_subscription_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->event_subscription_list, node) {
        cJSON *itemLocal = OpenAPI_ext_amf_event_subscription_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [event_subscription_list]");
            goto end;
        }
        cJSON_AddItemToArray(event_subscription_listList, itemLocal);
    }
    }

    if (ue_context->mm_context_list) {
    cJSON *mm_context_listList = cJSON_AddArrayToObject(item, "mmContextList");
    if (mm_context_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [mm_context_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->mm_context_list, node) {
        cJSON *itemLocal = OpenAPI_mm_context_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [mm_context_list]");
            goto end;
        }
        cJSON_AddItemToArray(mm_context_listList, itemLocal);
    }
    }

    if (ue_context->session_context_list) {
    cJSON *session_context_listList = cJSON_AddArrayToObject(item, "sessionContextList");
    if (session_context_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [session_context_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->session_context_list, node) {
        cJSON *itemLocal = OpenAPI_pdu_session_context_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [session_context_list]");
            goto end;
        }
        cJSON_AddItemToArray(session_context_listList, itemLocal);
    }
    }

    if (ue_context->eps_interworking_info) {
    cJSON *eps_interworking_info_local_JSON = OpenAPI_eps_interworking_info_convertToJSON(ue_context->eps_interworking_info);
    if (eps_interworking_info_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [eps_interworking_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "epsInterworkingInfo", eps_interworking_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [eps_interworking_info]");
        goto end;
    }
    }

    if (ue_context->trace_data) {
    cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(ue_context->trace_data);
    if (trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [trace_data]");
        goto end;
    }
    } else if (ue_context->is_trace_data_null) {
        if (cJSON_AddNullToObject(item, "traceData") == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

    if (ue_context->service_gap_expiry_time) {
    if (cJSON_AddStringToObject(item, "serviceGapExpiryTime", ue_context->service_gap_expiry_time) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [service_gap_expiry_time]");
        goto end;
    }
    }

    if (ue_context->stn_sr) {
    if (cJSON_AddStringToObject(item, "stnSr", ue_context->stn_sr) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [stn_sr]");
        goto end;
    }
    }

    if (ue_context->c_msisdn) {
    if (cJSON_AddStringToObject(item, "cMsisdn", ue_context->c_msisdn) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [c_msisdn]");
        goto end;
    }
    }

    if (ue_context->ms_classmark2) {
    if (cJSON_AddStringToObject(item, "msClassmark2", ue_context->ms_classmark2) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [ms_classmark2]");
        goto end;
    }
    }

    if (ue_context->supported_codec_list) {
    cJSON *supported_codec_listList = cJSON_AddArrayToObject(item, "supportedCodecList");
    if (supported_codec_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [supported_codec_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->supported_codec_list, node) {
        if (cJSON_AddStringToObject(supported_codec_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [supported_codec_list]");
            goto end;
        }
    }
    }

    if (ue_context->small_data_rate_status_infos) {
    cJSON *small_data_rate_status_infosList = cJSON_AddArrayToObject(item, "smallDataRateStatusInfos");
    if (small_data_rate_status_infosList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [small_data_rate_status_infos]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->small_data_rate_status_infos, node) {
        cJSON *itemLocal = OpenAPI_small_data_rate_status_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [small_data_rate_status_infos]");
            goto end;
        }
        cJSON_AddItemToArray(small_data_rate_status_infosList, itemLocal);
    }
    }

    if (ue_context->restricted_primary_rat_list != OpenAPI_rat_type_NULL) {
    cJSON *restricted_primary_rat_listList = cJSON_AddArrayToObject(item, "restrictedPrimaryRatList");
    if (restricted_primary_rat_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_primary_rat_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->restricted_primary_rat_list, node) {
        if (cJSON_AddStringToObject(restricted_primary_rat_listList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_primary_rat_list]");
            goto end;
        }
    }
    }

    if (ue_context->restricted_secondary_rat_list != OpenAPI_rat_type_NULL) {
    cJSON *restricted_secondary_rat_listList = cJSON_AddArrayToObject(item, "restrictedSecondaryRatList");
    if (restricted_secondary_rat_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_secondary_rat_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->restricted_secondary_rat_list, node) {
        if (cJSON_AddStringToObject(restricted_secondary_rat_listList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [restricted_secondary_rat_list]");
            goto end;
        }
    }
    }

    if (ue_context->v2x_context) {
    cJSON *v2x_context_local_JSON = OpenAPI_v2x_context_convertToJSON(ue_context->v2x_context);
    if (v2x_context_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [v2x_context]");
        goto end;
    }
    cJSON_AddItemToObject(item, "v2xContext", v2x_context_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [v2x_context]");
        goto end;
    }
    }

    if (ue_context->is_lte_cat_m_ind) {
    if (cJSON_AddBoolToObject(item, "lteCatMInd", ue_context->lte_cat_m_ind) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [lte_cat_m_ind]");
        goto end;
    }
    }

    if (ue_context->is_red_cap_ind) {
    if (cJSON_AddBoolToObject(item, "redCapInd", ue_context->red_cap_ind) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [red_cap_ind]");
        goto end;
    }
    }

    if (ue_context->mo_exp_data_counter) {
    cJSON *mo_exp_data_counter_local_JSON = OpenAPI_mo_exp_data_counter_convertToJSON(ue_context->mo_exp_data_counter);
    if (mo_exp_data_counter_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [mo_exp_data_counter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "moExpDataCounter", mo_exp_data_counter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [mo_exp_data_counter]");
        goto end;
    }
    }

    if (ue_context->cag_data) {
    cJSON *cag_data_local_JSON = OpenAPI_cag_data_convertToJSON(ue_context->cag_data);
    if (cag_data_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [cag_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "cagData", cag_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [cag_data]");
        goto end;
    }
    }

    if (ue_context->is_management_mdt_ind) {
    if (cJSON_AddBoolToObject(item, "managementMdtInd", ue_context->management_mdt_ind) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [management_mdt_ind]");
        goto end;
    }
    }

    if (ue_context->immediate_mdt_conf) {
    cJSON *immediate_mdt_conf_local_JSON = OpenAPI_immediate_mdt_conf_convertToJSON(ue_context->immediate_mdt_conf);
    if (immediate_mdt_conf_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [immediate_mdt_conf]");
        goto end;
    }
    cJSON_AddItemToObject(item, "immediateMdtConf", immediate_mdt_conf_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [immediate_mdt_conf]");
        goto end;
    }
    }

    if (ue_context->ec_restriction_data_wb) {
    cJSON *ec_restriction_data_wb_local_JSON = OpenAPI_ec_restriction_data_wb_convertToJSON(ue_context->ec_restriction_data_wb);
    if (ec_restriction_data_wb_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [ec_restriction_data_wb]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecRestrictionDataWb", ec_restriction_data_wb_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [ec_restriction_data_wb]");
        goto end;
    }
    }

    if (ue_context->is_ec_restriction_data_nb) {
    if (cJSON_AddBoolToObject(item, "ecRestrictionDataNb", ue_context->ec_restriction_data_nb) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [ec_restriction_data_nb]");
        goto end;
    }
    }

    if (ue_context->is_iab_operation_allowed) {
    if (cJSON_AddBoolToObject(item, "iabOperationAllowed", ue_context->iab_operation_allowed) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [iab_operation_allowed]");
        goto end;
    }
    }

    if (ue_context->prose_context) {
    cJSON *prose_context_local_JSON = OpenAPI_prose_context_convertToJSON(ue_context->prose_context);
    if (prose_context_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [prose_context]");
        goto end;
    }
    cJSON_AddItemToObject(item, "proseContext", prose_context_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [prose_context]");
        goto end;
    }
    }

    if (ue_context->analytics_subscription_list) {
    cJSON *analytics_subscription_listList = cJSON_AddArrayToObject(item, "analyticsSubscriptionList");
    if (analytics_subscription_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [analytics_subscription_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->analytics_subscription_list, node) {
        cJSON *itemLocal = OpenAPI_analytics_subscription_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [analytics_subscription_list]");
            goto end;
        }
        cJSON_AddItemToArray(analytics_subscription_listList, itemLocal);
    }
    }

    if (ue_context->pcf_amp_binding_info) {
    if (cJSON_AddStringToObject(item, "pcfAmpBindingInfo", ue_context->pcf_amp_binding_info) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_amp_binding_info]");
        goto end;
    }
    }

    if (ue_context->pcf_uep_binding_info) {
    if (cJSON_AddStringToObject(item, "pcfUepBindingInfo", ue_context->pcf_uep_binding_info) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_uep_binding_info]");
        goto end;
    }
    }

    if (ue_context->used_service_area_restriction) {
    cJSON *used_service_area_restriction_local_JSON = OpenAPI_service_area_restriction_convertToJSON(ue_context->used_service_area_restriction);
    if (used_service_area_restriction_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [used_service_area_restriction]");
        goto end;
    }
    cJSON_AddItemToObject(item, "usedServiceAreaRestriction", used_service_area_restriction_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [used_service_area_restriction]");
        goto end;
    }
    }

    if (ue_context->pra_in_am_policy) {
    cJSON *pra_in_am_policy = cJSON_AddObjectToObject(item, "praInAmPolicy");
    if (pra_in_am_policy == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pra_in_am_policy]");
        goto end;
    }
    cJSON *localMapObject = pra_in_am_policy;
    if (ue_context->pra_in_am_policy) {
        OpenAPI_list_for_each(ue_context->pra_in_am_policy, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [pra_in_am_policy]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [pra_in_am_policy]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (ue_context->pra_in_ue_policy) {
    cJSON *pra_in_ue_policy = cJSON_AddObjectToObject(item, "praInUePolicy");
    if (pra_in_ue_policy == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pra_in_ue_policy]");
        goto end;
    }
    cJSON *localMapObject = pra_in_ue_policy;
    if (ue_context->pra_in_ue_policy) {
        OpenAPI_list_for_each(ue_context->pra_in_ue_policy, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [pra_in_ue_policy]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [pra_in_ue_policy]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (ue_context->updp_subscription_data) {
    cJSON *updp_subscription_data_local_JSON = OpenAPI_updp_subscription_data_convertToJSON(ue_context->updp_subscription_data);
    if (updp_subscription_data_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [updp_subscription_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "updpSubscriptionData", updp_subscription_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [updp_subscription_data]");
        goto end;
    }
    }

    if (ue_context->sm_policy_notify_pdu_list) {
    cJSON *sm_policy_notify_pdu_listList = cJSON_AddArrayToObject(item, "smPolicyNotifyPduList");
    if (sm_policy_notify_pdu_listList == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [sm_policy_notify_pdu_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context->sm_policy_notify_pdu_list, node) {
        cJSON *itemLocal = OpenAPI_pdu_session_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [sm_policy_notify_pdu_list]");
            goto end;
        }
        cJSON_AddItemToArray(sm_policy_notify_pdu_listList, itemLocal);
    }
    }

    if (ue_context->pcf_ue_callback_info) {
    cJSON *pcf_ue_callback_info_local_JSON = OpenAPI_pcf_ue_callback_info_convertToJSON(ue_context->pcf_ue_callback_info);
    if (pcf_ue_callback_info_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_ue_callback_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pcfUeCallbackInfo", pcf_ue_callback_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_ue_callback_info]");
        goto end;
    }
    } else if (ue_context->is_pcf_ue_callback_info_null) {
        if (cJSON_AddNullToObject(item, "pcfUeCallbackInfo") == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_ue_callback_info]");
            goto end;
        }
    }

    if (ue_context->ue_positioning_cap) {
    if (cJSON_AddStringToObject(item, "uePositioningCap", ue_context->ue_positioning_cap) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [ue_positioning_cap]");
        goto end;
    }
    }

    if (ue_context->is_asti_distribution_indication) {
    if (cJSON_AddBoolToObject(item, "astiDistributionIndication", ue_context->asti_distribution_indication) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [asti_distribution_indication]");
        goto end;
    }
    }

    if (ue_context->is_ts_error_budget) {
    if (cJSON_AddNumberToObject(item, "tsErrorBudget", ue_context->ts_error_budget) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [ts_error_budget]");
        goto end;
    }
    }

    if (ue_context->is_snpn_onboard_ind) {
    if (cJSON_AddBoolToObject(item, "snpnOnboardInd", ue_context->snpn_onboard_ind) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [snpn_onboard_ind]");
        goto end;
    }
    }

    if (ue_context->smf_sel_info) {
    cJSON *smf_sel_info_local_JSON = OpenAPI_smf_selection_data_convertToJSON(ue_context->smf_sel_info);
    if (smf_sel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [smf_sel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smfSelInfo", smf_sel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [smf_sel_info]");
        goto end;
    }
    } else if (ue_context->is_smf_sel_info_null) {
        if (cJSON_AddNullToObject(item, "smfSelInfo") == NULL) {
            ogs_error("OpenAPI_ue_context_convertToJSON() failed [smf_sel_info]");
            goto end;
        }
    }

    if (ue_context->pcf_ue_slice_mbr_list) {
    cJSON *pcf_ue_slice_mbr_list = cJSON_AddObjectToObject(item, "pcfUeSliceMbrList");
    if (pcf_ue_slice_mbr_list == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_ue_slice_mbr_list]");
        goto end;
    }
    cJSON *localMapObject = pcf_ue_slice_mbr_list;
    if (ue_context->pcf_ue_slice_mbr_list) {
        OpenAPI_list_for_each(ue_context->pcf_ue_slice_mbr_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_ue_slice_mbr_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [pcf_ue_slice_mbr_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_slice_mbr_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_context_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (ue_context->smsf_set_id) {
    if (cJSON_AddStringToObject(item, "smsfSetId", ue_context->smsf_set_id) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [smsf_set_id]");
        goto end;
    }
    }

    if (ue_context->smsf_service_set_id) {
    if (cJSON_AddStringToObject(item, "smsfServiceSetId", ue_context->smsf_service_set_id) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [smsf_service_set_id]");
        goto end;
    }
    }

    if (ue_context->smsf_binding_info) {
    if (cJSON_AddStringToObject(item, "smsfBindingInfo", ue_context->smsf_binding_info) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [smsf_binding_info]");
        goto end;
    }
    }

    if (ue_context->is_disaster_roaming_ind) {
    if (cJSON_AddBoolToObject(item, "disasterRoamingInd", ue_context->disaster_roaming_ind) == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [disaster_roaming_ind]");
        goto end;
    }
    }

    if (ue_context->disaster_plmn) {
    cJSON *disaster_plmn_local_JSON = OpenAPI_plmn_id_convertToJSON(ue_context->disaster_plmn);
    if (disaster_plmn_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [disaster_plmn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "disasterPlmn", disaster_plmn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed [disaster_plmn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_context_t *OpenAPI_ue_context_parseFromJSON(cJSON *ue_contextJSON)
{
    OpenAPI_ue_context_t *ue_context_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *supi_unauth_ind = NULL;
    cJSON *gpsi_list = NULL;
    OpenAPI_list_t *gpsi_listList = NULL;
    cJSON *pei = NULL;
    cJSON *udm_group_id = NULL;
    cJSON *ausf_group_id = NULL;
    cJSON *pcf_group_id = NULL;
    cJSON *routing_indicator = NULL;
    cJSON *h_nw_pub_key_id = NULL;
    cJSON *group_list = NULL;
    OpenAPI_list_t *group_listList = NULL;
    cJSON *drx_parameter = NULL;
    cJSON *sub_rfsp = NULL;
    cJSON *used_rfsp = NULL;
    cJSON *sub_ue_ambr = NULL;
    OpenAPI_ambr_t *sub_ue_ambr_local_nonprim = NULL;
    cJSON *sub_ue_slice_mbr_list = NULL;
    OpenAPI_list_t *sub_ue_slice_mbr_listList = NULL;
    cJSON *smsf_id = NULL;
    cJSON *seaf_data = NULL;
    OpenAPI_seaf_data_t *seaf_data_local_nonprim = NULL;
    cJSON *_5g_mm_capability = NULL;
    cJSON *pcf_id = NULL;
    cJSON *pcf_set_id = NULL;
    cJSON *pcf_amp_service_set_id = NULL;
    cJSON *pcf_uep_service_set_id = NULL;
    cJSON *pcf_binding = NULL;
    OpenAPI_sbi_binding_level_e pcf_bindingVariable = 0;
    cJSON *pcf_am_policy_uri = NULL;
    cJSON *am_policy_req_trigger_list = NULL;
    OpenAPI_list_t *am_policy_req_trigger_listList = NULL;
    cJSON *pcf_ue_policy_uri = NULL;
    cJSON *ue_policy_req_trigger_list = NULL;
    OpenAPI_list_t *ue_policy_req_trigger_listList = NULL;
    cJSON *hpcf_id = NULL;
    cJSON *hpcf_set_id = NULL;
    cJSON *restricted_rat_list = NULL;
    OpenAPI_list_t *restricted_rat_listList = NULL;
    cJSON *forbidden_area_list = NULL;
    OpenAPI_list_t *forbidden_area_listList = NULL;
    cJSON *service_area_restriction = NULL;
    OpenAPI_service_area_restriction_t *service_area_restriction_local_nonprim = NULL;
    cJSON *restricted_core_nw_type_list = NULL;
    OpenAPI_list_t *restricted_core_nw_type_listList = NULL;
    cJSON *event_subscription_list = NULL;
    OpenAPI_list_t *event_subscription_listList = NULL;
    cJSON *mm_context_list = NULL;
    OpenAPI_list_t *mm_context_listList = NULL;
    cJSON *session_context_list = NULL;
    OpenAPI_list_t *session_context_listList = NULL;
    cJSON *eps_interworking_info = NULL;
    OpenAPI_eps_interworking_info_t *eps_interworking_info_local_nonprim = NULL;
    cJSON *trace_data = NULL;
    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    cJSON *service_gap_expiry_time = NULL;
    cJSON *stn_sr = NULL;
    cJSON *c_msisdn = NULL;
    cJSON *ms_classmark2 = NULL;
    cJSON *supported_codec_list = NULL;
    OpenAPI_list_t *supported_codec_listList = NULL;
    cJSON *small_data_rate_status_infos = NULL;
    OpenAPI_list_t *small_data_rate_status_infosList = NULL;
    cJSON *restricted_primary_rat_list = NULL;
    OpenAPI_list_t *restricted_primary_rat_listList = NULL;
    cJSON *restricted_secondary_rat_list = NULL;
    OpenAPI_list_t *restricted_secondary_rat_listList = NULL;
    cJSON *v2x_context = NULL;
    OpenAPI_v2x_context_t *v2x_context_local_nonprim = NULL;
    cJSON *lte_cat_m_ind = NULL;
    cJSON *red_cap_ind = NULL;
    cJSON *mo_exp_data_counter = NULL;
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter_local_nonprim = NULL;
    cJSON *cag_data = NULL;
    OpenAPI_cag_data_t *cag_data_local_nonprim = NULL;
    cJSON *management_mdt_ind = NULL;
    cJSON *immediate_mdt_conf = NULL;
    OpenAPI_immediate_mdt_conf_t *immediate_mdt_conf_local_nonprim = NULL;
    cJSON *ec_restriction_data_wb = NULL;
    OpenAPI_ec_restriction_data_wb_t *ec_restriction_data_wb_local_nonprim = NULL;
    cJSON *ec_restriction_data_nb = NULL;
    cJSON *iab_operation_allowed = NULL;
    cJSON *prose_context = NULL;
    OpenAPI_prose_context_t *prose_context_local_nonprim = NULL;
    cJSON *analytics_subscription_list = NULL;
    OpenAPI_list_t *analytics_subscription_listList = NULL;
    cJSON *pcf_amp_binding_info = NULL;
    cJSON *pcf_uep_binding_info = NULL;
    cJSON *used_service_area_restriction = NULL;
    OpenAPI_service_area_restriction_t *used_service_area_restriction_local_nonprim = NULL;
    cJSON *pra_in_am_policy = NULL;
    OpenAPI_list_t *pra_in_am_policyList = NULL;
    cJSON *pra_in_ue_policy = NULL;
    OpenAPI_list_t *pra_in_ue_policyList = NULL;
    cJSON *updp_subscription_data = NULL;
    OpenAPI_updp_subscription_data_t *updp_subscription_data_local_nonprim = NULL;
    cJSON *sm_policy_notify_pdu_list = NULL;
    OpenAPI_list_t *sm_policy_notify_pdu_listList = NULL;
    cJSON *pcf_ue_callback_info = NULL;
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info_local_nonprim = NULL;
    cJSON *ue_positioning_cap = NULL;
    cJSON *asti_distribution_indication = NULL;
    cJSON *ts_error_budget = NULL;
    cJSON *snpn_onboard_ind = NULL;
    cJSON *smf_sel_info = NULL;
    OpenAPI_smf_selection_data_t *smf_sel_info_local_nonprim = NULL;
    cJSON *pcf_ue_slice_mbr_list = NULL;
    OpenAPI_list_t *pcf_ue_slice_mbr_listList = NULL;
    cJSON *smsf_set_id = NULL;
    cJSON *smsf_service_set_id = NULL;
    cJSON *smsf_binding_info = NULL;
    cJSON *disaster_roaming_ind = NULL;
    cJSON *disaster_plmn = NULL;
    OpenAPI_plmn_id_t *disaster_plmn_local_nonprim = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    supi_unauth_ind = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "supiUnauthInd");
    if (supi_unauth_ind) {
    if (!cJSON_IsBool(supi_unauth_ind)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [supi_unauth_ind]");
        goto end;
    }
    }

    gpsi_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "gpsiList");
    if (gpsi_list) {
        cJSON *gpsi_list_local = NULL;
        if (!cJSON_IsArray(gpsi_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [gpsi_list]");
            goto end;
        }

        gpsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_list_local, gpsi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gpsi_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [gpsi_list]");
                goto end;
            }
            OpenAPI_list_add(gpsi_listList, ogs_strdup(gpsi_list_local->valuestring));
        }
    }

    pei = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    udm_group_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "udmGroupId");
    if (udm_group_id) {
    if (!cJSON_IsString(udm_group_id) && !cJSON_IsNull(udm_group_id)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [udm_group_id]");
        goto end;
    }
    }

    ausf_group_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "ausfGroupId");
    if (ausf_group_id) {
    if (!cJSON_IsString(ausf_group_id) && !cJSON_IsNull(ausf_group_id)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [ausf_group_id]");
        goto end;
    }
    }

    pcf_group_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfGroupId");
    if (pcf_group_id) {
    if (!cJSON_IsString(pcf_group_id) && !cJSON_IsNull(pcf_group_id)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_group_id]");
        goto end;
    }
    }

    routing_indicator = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "routingIndicator");
    if (routing_indicator) {
    if (!cJSON_IsString(routing_indicator) && !cJSON_IsNull(routing_indicator)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [routing_indicator]");
        goto end;
    }
    }

    h_nw_pub_key_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "hNwPubKeyId");
    if (h_nw_pub_key_id) {
    if (!cJSON_IsNumber(h_nw_pub_key_id)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [h_nw_pub_key_id]");
        goto end;
    }
    }

    group_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "groupList");
    if (group_list) {
        cJSON *group_list_local = NULL;
        if (!cJSON_IsArray(group_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [group_list]");
            goto end;
        }

        group_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(group_list_local, group_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(group_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [group_list]");
                goto end;
            }
            OpenAPI_list_add(group_listList, ogs_strdup(group_list_local->valuestring));
        }
    }

    drx_parameter = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "drxParameter");
    if (drx_parameter) {
    if (!cJSON_IsString(drx_parameter) && !cJSON_IsNull(drx_parameter)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [drx_parameter]");
        goto end;
    }
    }

    sub_rfsp = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "subRfsp");
    if (sub_rfsp) {
    if (!cJSON_IsNumber(sub_rfsp)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [sub_rfsp]");
        goto end;
    }
    }

    used_rfsp = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "usedRfsp");
    if (used_rfsp) {
    if (!cJSON_IsNumber(used_rfsp)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [used_rfsp]");
        goto end;
    }
    }

    sub_ue_ambr = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "subUeAmbr");
    if (sub_ue_ambr) {
    sub_ue_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(sub_ue_ambr);
    if (!sub_ue_ambr_local_nonprim) {
        ogs_error("OpenAPI_ambr_parseFromJSON failed [sub_ue_ambr]");
        goto end;
    }
    }

    sub_ue_slice_mbr_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "subUeSliceMbrList");
    if (sub_ue_slice_mbr_list) {
        cJSON *sub_ue_slice_mbr_list_local_map = NULL;
        if (!cJSON_IsObject(sub_ue_slice_mbr_list) && !cJSON_IsNull(sub_ue_slice_mbr_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [sub_ue_slice_mbr_list]");
            goto end;
        }
        if (cJSON_IsObject(sub_ue_slice_mbr_list)) {
            sub_ue_slice_mbr_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(sub_ue_slice_mbr_list_local_map, sub_ue_slice_mbr_list) {
                cJSON *localMapObject = sub_ue_slice_mbr_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_slice_mbr_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_ue_context_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(sub_ue_slice_mbr_listList, localMapKeyPair);
            }
        }
    }

    smsf_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "smsfId");
    if (smsf_id) {
    if (!cJSON_IsString(smsf_id) && !cJSON_IsNull(smsf_id)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [smsf_id]");
        goto end;
    }
    }

    seaf_data = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "seafData");
    if (seaf_data) {
    seaf_data_local_nonprim = OpenAPI_seaf_data_parseFromJSON(seaf_data);
    if (!seaf_data_local_nonprim) {
        ogs_error("OpenAPI_seaf_data_parseFromJSON failed [seaf_data]");
        goto end;
    }
    }

    _5g_mm_capability = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "5gMmCapability");
    if (_5g_mm_capability) {
    if (!cJSON_IsString(_5g_mm_capability) && !cJSON_IsNull(_5g_mm_capability)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [_5g_mm_capability]");
        goto end;
    }
    }

    pcf_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_set_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfSetId");
    if (pcf_set_id) {
    if (!cJSON_IsString(pcf_set_id) && !cJSON_IsNull(pcf_set_id)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    pcf_amp_service_set_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfAmpServiceSetId");
    if (pcf_amp_service_set_id) {
    if (!cJSON_IsString(pcf_amp_service_set_id) && !cJSON_IsNull(pcf_amp_service_set_id)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_amp_service_set_id]");
        goto end;
    }
    }

    pcf_uep_service_set_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfUepServiceSetId");
    if (pcf_uep_service_set_id) {
    if (!cJSON_IsString(pcf_uep_service_set_id) && !cJSON_IsNull(pcf_uep_service_set_id)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_uep_service_set_id]");
        goto end;
    }
    }

    pcf_binding = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfBinding");
    if (pcf_binding) {
    if (!cJSON_IsString(pcf_binding)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_binding]");
        goto end;
    }
    pcf_bindingVariable = OpenAPI_sbi_binding_level_FromString(pcf_binding->valuestring);
    }

    pcf_am_policy_uri = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfAmPolicyUri");
    if (pcf_am_policy_uri) {
    if (!cJSON_IsString(pcf_am_policy_uri) && !cJSON_IsNull(pcf_am_policy_uri)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_am_policy_uri]");
        goto end;
    }
    }

    am_policy_req_trigger_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "amPolicyReqTriggerList");
    if (am_policy_req_trigger_list) {
        cJSON *am_policy_req_trigger_list_local = NULL;
        if (!cJSON_IsArray(am_policy_req_trigger_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [am_policy_req_trigger_list]");
            goto end;
        }

        am_policy_req_trigger_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(am_policy_req_trigger_list_local, am_policy_req_trigger_list) {
            OpenAPI_policy_req_trigger_e localEnum = OpenAPI_policy_req_trigger_NULL;
            if (!cJSON_IsString(am_policy_req_trigger_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [am_policy_req_trigger_list]");
                goto end;
            }
            localEnum = OpenAPI_policy_req_trigger_FromString(am_policy_req_trigger_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"am_policy_req_trigger_list\" is not supported. Ignoring it ...",
                         am_policy_req_trigger_list_local->valuestring);
            } else {
                OpenAPI_list_add(am_policy_req_trigger_listList, (void *)localEnum);
            }
        }
        if (am_policy_req_trigger_listList->count == 0) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed: Expected am_policy_req_trigger_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    pcf_ue_policy_uri = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfUePolicyUri");
    if (pcf_ue_policy_uri) {
    if (!cJSON_IsString(pcf_ue_policy_uri) && !cJSON_IsNull(pcf_ue_policy_uri)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_ue_policy_uri]");
        goto end;
    }
    }

    ue_policy_req_trigger_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "uePolicyReqTriggerList");
    if (ue_policy_req_trigger_list) {
        cJSON *ue_policy_req_trigger_list_local = NULL;
        if (!cJSON_IsArray(ue_policy_req_trigger_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [ue_policy_req_trigger_list]");
            goto end;
        }

        ue_policy_req_trigger_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_policy_req_trigger_list_local, ue_policy_req_trigger_list) {
            OpenAPI_policy_req_trigger_e localEnum = OpenAPI_policy_req_trigger_NULL;
            if (!cJSON_IsString(ue_policy_req_trigger_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [ue_policy_req_trigger_list]");
                goto end;
            }
            localEnum = OpenAPI_policy_req_trigger_FromString(ue_policy_req_trigger_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"ue_policy_req_trigger_list\" is not supported. Ignoring it ...",
                         ue_policy_req_trigger_list_local->valuestring);
            } else {
                OpenAPI_list_add(ue_policy_req_trigger_listList, (void *)localEnum);
            }
        }
        if (ue_policy_req_trigger_listList->count == 0) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed: Expected ue_policy_req_trigger_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    hpcf_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "hpcfId");
    if (hpcf_id) {
    if (!cJSON_IsString(hpcf_id) && !cJSON_IsNull(hpcf_id)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [hpcf_id]");
        goto end;
    }
    }

    hpcf_set_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "hpcfSetId");
    if (hpcf_set_id) {
    if (!cJSON_IsString(hpcf_set_id) && !cJSON_IsNull(hpcf_set_id)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [hpcf_set_id]");
        goto end;
    }
    }

    restricted_rat_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "restrictedRatList");
    if (restricted_rat_list) {
        cJSON *restricted_rat_list_local = NULL;
        if (!cJSON_IsArray(restricted_rat_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_rat_list]");
            goto end;
        }

        restricted_rat_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(restricted_rat_list_local, restricted_rat_list) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(restricted_rat_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_rat_list]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(restricted_rat_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"restricted_rat_list\" is not supported. Ignoring it ...",
                         restricted_rat_list_local->valuestring);
            } else {
                OpenAPI_list_add(restricted_rat_listList, (void *)localEnum);
            }
        }
        if (restricted_rat_listList->count == 0) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed: Expected restricted_rat_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    forbidden_area_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "forbiddenAreaList");
    if (forbidden_area_list) {
        cJSON *forbidden_area_list_local = NULL;
        if (!cJSON_IsArray(forbidden_area_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [forbidden_area_list]");
            goto end;
        }

        forbidden_area_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(forbidden_area_list_local, forbidden_area_list) {
            if (!cJSON_IsObject(forbidden_area_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [forbidden_area_list]");
                goto end;
            }
            OpenAPI_area_t *forbidden_area_listItem = OpenAPI_area_parseFromJSON(forbidden_area_list_local);
            if (!forbidden_area_listItem) {
                ogs_error("No forbidden_area_listItem");
                goto end;
            }
            OpenAPI_list_add(forbidden_area_listList, forbidden_area_listItem);
        }
    }

    service_area_restriction = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "serviceAreaRestriction");
    if (service_area_restriction) {
    service_area_restriction_local_nonprim = OpenAPI_service_area_restriction_parseFromJSON(service_area_restriction);
    if (!service_area_restriction_local_nonprim) {
        ogs_error("OpenAPI_service_area_restriction_parseFromJSON failed [service_area_restriction]");
        goto end;
    }
    }

    restricted_core_nw_type_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "restrictedCoreNwTypeList");
    if (restricted_core_nw_type_list) {
        cJSON *restricted_core_nw_type_list_local = NULL;
        if (!cJSON_IsArray(restricted_core_nw_type_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_core_nw_type_list]");
            goto end;
        }

        restricted_core_nw_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(restricted_core_nw_type_list_local, restricted_core_nw_type_list) {
            OpenAPI_core_network_type_e localEnum = OpenAPI_core_network_type_NULL;
            if (!cJSON_IsString(restricted_core_nw_type_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_core_nw_type_list]");
                goto end;
            }
            localEnum = OpenAPI_core_network_type_FromString(restricted_core_nw_type_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"restricted_core_nw_type_list\" is not supported. Ignoring it ...",
                         restricted_core_nw_type_list_local->valuestring);
            } else {
                OpenAPI_list_add(restricted_core_nw_type_listList, (void *)localEnum);
            }
        }
        if (restricted_core_nw_type_listList->count == 0) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed: Expected restricted_core_nw_type_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    event_subscription_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "eventSubscriptionList");
    if (event_subscription_list) {
        cJSON *event_subscription_list_local = NULL;
        if (!cJSON_IsArray(event_subscription_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [event_subscription_list]");
            goto end;
        }

        event_subscription_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_subscription_list_local, event_subscription_list) {
            if (!cJSON_IsObject(event_subscription_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [event_subscription_list]");
                goto end;
            }
            OpenAPI_ext_amf_event_subscription_t *event_subscription_listItem = OpenAPI_ext_amf_event_subscription_parseFromJSON(event_subscription_list_local);
            if (!event_subscription_listItem) {
                ogs_error("No event_subscription_listItem");
                goto end;
            }
            OpenAPI_list_add(event_subscription_listList, event_subscription_listItem);
        }
    }

    mm_context_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "mmContextList");
    if (mm_context_list) {
        cJSON *mm_context_list_local = NULL;
        if (!cJSON_IsArray(mm_context_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [mm_context_list]");
            goto end;
        }

        mm_context_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(mm_context_list_local, mm_context_list) {
            if (!cJSON_IsObject(mm_context_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [mm_context_list]");
                goto end;
            }
            OpenAPI_mm_context_t *mm_context_listItem = OpenAPI_mm_context_parseFromJSON(mm_context_list_local);
            if (!mm_context_listItem) {
                ogs_error("No mm_context_listItem");
                goto end;
            }
            OpenAPI_list_add(mm_context_listList, mm_context_listItem);
        }
    }

    session_context_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "sessionContextList");
    if (session_context_list) {
        cJSON *session_context_list_local = NULL;
        if (!cJSON_IsArray(session_context_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [session_context_list]");
            goto end;
        }

        session_context_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(session_context_list_local, session_context_list) {
            if (!cJSON_IsObject(session_context_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [session_context_list]");
                goto end;
            }
            OpenAPI_pdu_session_context_t *session_context_listItem = OpenAPI_pdu_session_context_parseFromJSON(session_context_list_local);
            if (!session_context_listItem) {
                ogs_error("No session_context_listItem");
                goto end;
            }
            OpenAPI_list_add(session_context_listList, session_context_listItem);
        }
    }

    eps_interworking_info = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "epsInterworkingInfo");
    if (eps_interworking_info) {
    eps_interworking_info_local_nonprim = OpenAPI_eps_interworking_info_parseFromJSON(eps_interworking_info);
    if (!eps_interworking_info_local_nonprim) {
        ogs_error("OpenAPI_eps_interworking_info_parseFromJSON failed [eps_interworking_info]");
        goto end;
    }
    }

    trace_data = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "traceData");
    if (trace_data) {
    if (!cJSON_IsNull(trace_data)) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    if (!trace_data_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_data]");
        goto end;
    }
    }
    }

    service_gap_expiry_time = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "serviceGapExpiryTime");
    if (service_gap_expiry_time) {
    if (!cJSON_IsString(service_gap_expiry_time) && !cJSON_IsNull(service_gap_expiry_time)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [service_gap_expiry_time]");
        goto end;
    }
    }

    stn_sr = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "stnSr");
    if (stn_sr) {
    if (!cJSON_IsString(stn_sr) && !cJSON_IsNull(stn_sr)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [stn_sr]");
        goto end;
    }
    }

    c_msisdn = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "cMsisdn");
    if (c_msisdn) {
    if (!cJSON_IsString(c_msisdn) && !cJSON_IsNull(c_msisdn)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [c_msisdn]");
        goto end;
    }
    }

    ms_classmark2 = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "msClassmark2");
    if (ms_classmark2) {
    if (!cJSON_IsString(ms_classmark2) && !cJSON_IsNull(ms_classmark2)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [ms_classmark2]");
        goto end;
    }
    }

    supported_codec_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "supportedCodecList");
    if (supported_codec_list) {
        cJSON *supported_codec_list_local = NULL;
        if (!cJSON_IsArray(supported_codec_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [supported_codec_list]");
            goto end;
        }

        supported_codec_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(supported_codec_list_local, supported_codec_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supported_codec_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [supported_codec_list]");
                goto end;
            }
            OpenAPI_list_add(supported_codec_listList, ogs_strdup(supported_codec_list_local->valuestring));
        }
    }

    small_data_rate_status_infos = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "smallDataRateStatusInfos");
    if (small_data_rate_status_infos) {
        cJSON *small_data_rate_status_infos_local = NULL;
        if (!cJSON_IsArray(small_data_rate_status_infos)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [small_data_rate_status_infos]");
            goto end;
        }

        small_data_rate_status_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(small_data_rate_status_infos_local, small_data_rate_status_infos) {
            if (!cJSON_IsObject(small_data_rate_status_infos_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [small_data_rate_status_infos]");
                goto end;
            }
            OpenAPI_small_data_rate_status_info_t *small_data_rate_status_infosItem = OpenAPI_small_data_rate_status_info_parseFromJSON(small_data_rate_status_infos_local);
            if (!small_data_rate_status_infosItem) {
                ogs_error("No small_data_rate_status_infosItem");
                goto end;
            }
            OpenAPI_list_add(small_data_rate_status_infosList, small_data_rate_status_infosItem);
        }
    }

    restricted_primary_rat_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "restrictedPrimaryRatList");
    if (restricted_primary_rat_list) {
        cJSON *restricted_primary_rat_list_local = NULL;
        if (!cJSON_IsArray(restricted_primary_rat_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_primary_rat_list]");
            goto end;
        }

        restricted_primary_rat_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(restricted_primary_rat_list_local, restricted_primary_rat_list) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(restricted_primary_rat_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_primary_rat_list]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(restricted_primary_rat_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"restricted_primary_rat_list\" is not supported. Ignoring it ...",
                         restricted_primary_rat_list_local->valuestring);
            } else {
                OpenAPI_list_add(restricted_primary_rat_listList, (void *)localEnum);
            }
        }
        if (restricted_primary_rat_listList->count == 0) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed: Expected restricted_primary_rat_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    restricted_secondary_rat_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "restrictedSecondaryRatList");
    if (restricted_secondary_rat_list) {
        cJSON *restricted_secondary_rat_list_local = NULL;
        if (!cJSON_IsArray(restricted_secondary_rat_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_secondary_rat_list]");
            goto end;
        }

        restricted_secondary_rat_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(restricted_secondary_rat_list_local, restricted_secondary_rat_list) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(restricted_secondary_rat_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [restricted_secondary_rat_list]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(restricted_secondary_rat_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"restricted_secondary_rat_list\" is not supported. Ignoring it ...",
                         restricted_secondary_rat_list_local->valuestring);
            } else {
                OpenAPI_list_add(restricted_secondary_rat_listList, (void *)localEnum);
            }
        }
        if (restricted_secondary_rat_listList->count == 0) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed: Expected restricted_secondary_rat_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    v2x_context = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "v2xContext");
    if (v2x_context) {
    v2x_context_local_nonprim = OpenAPI_v2x_context_parseFromJSON(v2x_context);
    if (!v2x_context_local_nonprim) {
        ogs_error("OpenAPI_v2x_context_parseFromJSON failed [v2x_context]");
        goto end;
    }
    }

    lte_cat_m_ind = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "lteCatMInd");
    if (lte_cat_m_ind) {
    if (!cJSON_IsBool(lte_cat_m_ind)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [lte_cat_m_ind]");
        goto end;
    }
    }

    red_cap_ind = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "redCapInd");
    if (red_cap_ind) {
    if (!cJSON_IsBool(red_cap_ind)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [red_cap_ind]");
        goto end;
    }
    }

    mo_exp_data_counter = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "moExpDataCounter");
    if (mo_exp_data_counter) {
    mo_exp_data_counter_local_nonprim = OpenAPI_mo_exp_data_counter_parseFromJSON(mo_exp_data_counter);
    if (!mo_exp_data_counter_local_nonprim) {
        ogs_error("OpenAPI_mo_exp_data_counter_parseFromJSON failed [mo_exp_data_counter]");
        goto end;
    }
    }

    cag_data = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "cagData");
    if (cag_data) {
    cag_data_local_nonprim = OpenAPI_cag_data_parseFromJSON(cag_data);
    if (!cag_data_local_nonprim) {
        ogs_error("OpenAPI_cag_data_parseFromJSON failed [cag_data]");
        goto end;
    }
    }

    management_mdt_ind = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "managementMdtInd");
    if (management_mdt_ind) {
    if (!cJSON_IsBool(management_mdt_ind)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [management_mdt_ind]");
        goto end;
    }
    }

    immediate_mdt_conf = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "immediateMdtConf");
    if (immediate_mdt_conf) {
    immediate_mdt_conf_local_nonprim = OpenAPI_immediate_mdt_conf_parseFromJSON(immediate_mdt_conf);
    if (!immediate_mdt_conf_local_nonprim) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON failed [immediate_mdt_conf]");
        goto end;
    }
    }

    ec_restriction_data_wb = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "ecRestrictionDataWb");
    if (ec_restriction_data_wb) {
    ec_restriction_data_wb_local_nonprim = OpenAPI_ec_restriction_data_wb_parseFromJSON(ec_restriction_data_wb);
    if (!ec_restriction_data_wb_local_nonprim) {
        ogs_error("OpenAPI_ec_restriction_data_wb_parseFromJSON failed [ec_restriction_data_wb]");
        goto end;
    }
    }

    ec_restriction_data_nb = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "ecRestrictionDataNb");
    if (ec_restriction_data_nb) {
    if (!cJSON_IsBool(ec_restriction_data_nb)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [ec_restriction_data_nb]");
        goto end;
    }
    }

    iab_operation_allowed = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "iabOperationAllowed");
    if (iab_operation_allowed) {
    if (!cJSON_IsBool(iab_operation_allowed)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [iab_operation_allowed]");
        goto end;
    }
    }

    prose_context = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "proseContext");
    if (prose_context) {
    prose_context_local_nonprim = OpenAPI_prose_context_parseFromJSON(prose_context);
    if (!prose_context_local_nonprim) {
        ogs_error("OpenAPI_prose_context_parseFromJSON failed [prose_context]");
        goto end;
    }
    }

    analytics_subscription_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "analyticsSubscriptionList");
    if (analytics_subscription_list) {
        cJSON *analytics_subscription_list_local = NULL;
        if (!cJSON_IsArray(analytics_subscription_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [analytics_subscription_list]");
            goto end;
        }

        analytics_subscription_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(analytics_subscription_list_local, analytics_subscription_list) {
            if (!cJSON_IsObject(analytics_subscription_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [analytics_subscription_list]");
                goto end;
            }
            OpenAPI_analytics_subscription_t *analytics_subscription_listItem = OpenAPI_analytics_subscription_parseFromJSON(analytics_subscription_list_local);
            if (!analytics_subscription_listItem) {
                ogs_error("No analytics_subscription_listItem");
                goto end;
            }
            OpenAPI_list_add(analytics_subscription_listList, analytics_subscription_listItem);
        }
    }

    pcf_amp_binding_info = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfAmpBindingInfo");
    if (pcf_amp_binding_info) {
    if (!cJSON_IsString(pcf_amp_binding_info) && !cJSON_IsNull(pcf_amp_binding_info)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_amp_binding_info]");
        goto end;
    }
    }

    pcf_uep_binding_info = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfUepBindingInfo");
    if (pcf_uep_binding_info) {
    if (!cJSON_IsString(pcf_uep_binding_info) && !cJSON_IsNull(pcf_uep_binding_info)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_uep_binding_info]");
        goto end;
    }
    }

    used_service_area_restriction = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "usedServiceAreaRestriction");
    if (used_service_area_restriction) {
    used_service_area_restriction_local_nonprim = OpenAPI_service_area_restriction_parseFromJSON(used_service_area_restriction);
    if (!used_service_area_restriction_local_nonprim) {
        ogs_error("OpenAPI_service_area_restriction_parseFromJSON failed [used_service_area_restriction]");
        goto end;
    }
    }

    pra_in_am_policy = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "praInAmPolicy");
    if (pra_in_am_policy) {
        cJSON *pra_in_am_policy_local_map = NULL;
        if (!cJSON_IsObject(pra_in_am_policy) && !cJSON_IsNull(pra_in_am_policy)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pra_in_am_policy]");
            goto end;
        }
        if (cJSON_IsObject(pra_in_am_policy)) {
            pra_in_am_policyList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(pra_in_am_policy_local_map, pra_in_am_policy) {
                cJSON *localMapObject = pra_in_am_policy_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_ue_context_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(pra_in_am_policyList, localMapKeyPair);
            }
        }
    }

    pra_in_ue_policy = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "praInUePolicy");
    if (pra_in_ue_policy) {
        cJSON *pra_in_ue_policy_local_map = NULL;
        if (!cJSON_IsObject(pra_in_ue_policy) && !cJSON_IsNull(pra_in_ue_policy)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pra_in_ue_policy]");
            goto end;
        }
        if (cJSON_IsObject(pra_in_ue_policy)) {
            pra_in_ue_policyList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(pra_in_ue_policy_local_map, pra_in_ue_policy) {
                cJSON *localMapObject = pra_in_ue_policy_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_ue_context_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(pra_in_ue_policyList, localMapKeyPair);
            }
        }
    }

    updp_subscription_data = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "updpSubscriptionData");
    if (updp_subscription_data) {
    updp_subscription_data_local_nonprim = OpenAPI_updp_subscription_data_parseFromJSON(updp_subscription_data);
    if (!updp_subscription_data_local_nonprim) {
        ogs_error("OpenAPI_updp_subscription_data_parseFromJSON failed [updp_subscription_data]");
        goto end;
    }
    }

    sm_policy_notify_pdu_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "smPolicyNotifyPduList");
    if (sm_policy_notify_pdu_list) {
        cJSON *sm_policy_notify_pdu_list_local = NULL;
        if (!cJSON_IsArray(sm_policy_notify_pdu_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [sm_policy_notify_pdu_list]");
            goto end;
        }

        sm_policy_notify_pdu_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(sm_policy_notify_pdu_list_local, sm_policy_notify_pdu_list) {
            if (!cJSON_IsObject(sm_policy_notify_pdu_list_local)) {
                ogs_error("OpenAPI_ue_context_parseFromJSON() failed [sm_policy_notify_pdu_list]");
                goto end;
            }
            OpenAPI_pdu_session_info_t *sm_policy_notify_pdu_listItem = OpenAPI_pdu_session_info_parseFromJSON(sm_policy_notify_pdu_list_local);
            if (!sm_policy_notify_pdu_listItem) {
                ogs_error("No sm_policy_notify_pdu_listItem");
                goto end;
            }
            OpenAPI_list_add(sm_policy_notify_pdu_listList, sm_policy_notify_pdu_listItem);
        }
    }

    pcf_ue_callback_info = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfUeCallbackInfo");
    if (pcf_ue_callback_info) {
    if (!cJSON_IsNull(pcf_ue_callback_info)) {
    pcf_ue_callback_info_local_nonprim = OpenAPI_pcf_ue_callback_info_parseFromJSON(pcf_ue_callback_info);
    if (!pcf_ue_callback_info_local_nonprim) {
        ogs_error("OpenAPI_pcf_ue_callback_info_parseFromJSON failed [pcf_ue_callback_info]");
        goto end;
    }
    }
    }

    ue_positioning_cap = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "uePositioningCap");
    if (ue_positioning_cap) {
    if (!cJSON_IsString(ue_positioning_cap) && !cJSON_IsNull(ue_positioning_cap)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [ue_positioning_cap]");
        goto end;
    }
    }

    asti_distribution_indication = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "astiDistributionIndication");
    if (asti_distribution_indication) {
    if (!cJSON_IsBool(asti_distribution_indication)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [asti_distribution_indication]");
        goto end;
    }
    }

    ts_error_budget = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "tsErrorBudget");
    if (ts_error_budget) {
    if (!cJSON_IsNumber(ts_error_budget)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [ts_error_budget]");
        goto end;
    }
    }

    snpn_onboard_ind = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "snpnOnboardInd");
    if (snpn_onboard_ind) {
    if (!cJSON_IsBool(snpn_onboard_ind)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [snpn_onboard_ind]");
        goto end;
    }
    }

    smf_sel_info = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "smfSelInfo");
    if (smf_sel_info) {
    if (!cJSON_IsNull(smf_sel_info)) {
    smf_sel_info_local_nonprim = OpenAPI_smf_selection_data_parseFromJSON(smf_sel_info);
    if (!smf_sel_info_local_nonprim) {
        ogs_error("OpenAPI_smf_selection_data_parseFromJSON failed [smf_sel_info]");
        goto end;
    }
    }
    }

    pcf_ue_slice_mbr_list = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "pcfUeSliceMbrList");
    if (pcf_ue_slice_mbr_list) {
        cJSON *pcf_ue_slice_mbr_list_local_map = NULL;
        if (!cJSON_IsObject(pcf_ue_slice_mbr_list) && !cJSON_IsNull(pcf_ue_slice_mbr_list)) {
            ogs_error("OpenAPI_ue_context_parseFromJSON() failed [pcf_ue_slice_mbr_list]");
            goto end;
        }
        if (cJSON_IsObject(pcf_ue_slice_mbr_list)) {
            pcf_ue_slice_mbr_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(pcf_ue_slice_mbr_list_local_map, pcf_ue_slice_mbr_list) {
                cJSON *localMapObject = pcf_ue_slice_mbr_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_slice_mbr_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_ue_context_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(pcf_ue_slice_mbr_listList, localMapKeyPair);
            }
        }
    }

    smsf_set_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "smsfSetId");
    if (smsf_set_id) {
    if (!cJSON_IsString(smsf_set_id) && !cJSON_IsNull(smsf_set_id)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [smsf_set_id]");
        goto end;
    }
    }

    smsf_service_set_id = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "smsfServiceSetId");
    if (smsf_service_set_id) {
    if (!cJSON_IsString(smsf_service_set_id) && !cJSON_IsNull(smsf_service_set_id)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [smsf_service_set_id]");
        goto end;
    }
    }

    smsf_binding_info = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "smsfBindingInfo");
    if (smsf_binding_info) {
    if (!cJSON_IsString(smsf_binding_info) && !cJSON_IsNull(smsf_binding_info)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [smsf_binding_info]");
        goto end;
    }
    }

    disaster_roaming_ind = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "disasterRoamingInd");
    if (disaster_roaming_ind) {
    if (!cJSON_IsBool(disaster_roaming_ind)) {
        ogs_error("OpenAPI_ue_context_parseFromJSON() failed [disaster_roaming_ind]");
        goto end;
    }
    }

    disaster_plmn = cJSON_GetObjectItemCaseSensitive(ue_contextJSON, "disasterPlmn");
    if (disaster_plmn) {
    disaster_plmn_local_nonprim = OpenAPI_plmn_id_parseFromJSON(disaster_plmn);
    if (!disaster_plmn_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [disaster_plmn]");
        goto end;
    }
    }

    ue_context_local_var = OpenAPI_ue_context_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        supi_unauth_ind ? true : false,
        supi_unauth_ind ? supi_unauth_ind->valueint : 0,
        gpsi_list ? gpsi_listList : NULL,
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        udm_group_id && !cJSON_IsNull(udm_group_id) ? ogs_strdup(udm_group_id->valuestring) : NULL,
        ausf_group_id && !cJSON_IsNull(ausf_group_id) ? ogs_strdup(ausf_group_id->valuestring) : NULL,
        pcf_group_id && !cJSON_IsNull(pcf_group_id) ? ogs_strdup(pcf_group_id->valuestring) : NULL,
        routing_indicator && !cJSON_IsNull(routing_indicator) ? ogs_strdup(routing_indicator->valuestring) : NULL,
        h_nw_pub_key_id ? true : false,
        h_nw_pub_key_id ? h_nw_pub_key_id->valuedouble : 0,
        group_list ? group_listList : NULL,
        drx_parameter && !cJSON_IsNull(drx_parameter) ? ogs_strdup(drx_parameter->valuestring) : NULL,
        sub_rfsp ? true : false,
        sub_rfsp ? sub_rfsp->valuedouble : 0,
        used_rfsp ? true : false,
        used_rfsp ? used_rfsp->valuedouble : 0,
        sub_ue_ambr ? sub_ue_ambr_local_nonprim : NULL,
        sub_ue_slice_mbr_list ? sub_ue_slice_mbr_listList : NULL,
        smsf_id && !cJSON_IsNull(smsf_id) ? ogs_strdup(smsf_id->valuestring) : NULL,
        seaf_data ? seaf_data_local_nonprim : NULL,
        _5g_mm_capability && !cJSON_IsNull(_5g_mm_capability) ? ogs_strdup(_5g_mm_capability->valuestring) : NULL,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_set_id && !cJSON_IsNull(pcf_set_id) ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        pcf_amp_service_set_id && !cJSON_IsNull(pcf_amp_service_set_id) ? ogs_strdup(pcf_amp_service_set_id->valuestring) : NULL,
        pcf_uep_service_set_id && !cJSON_IsNull(pcf_uep_service_set_id) ? ogs_strdup(pcf_uep_service_set_id->valuestring) : NULL,
        pcf_binding ? pcf_bindingVariable : 0,
        pcf_am_policy_uri && !cJSON_IsNull(pcf_am_policy_uri) ? ogs_strdup(pcf_am_policy_uri->valuestring) : NULL,
        am_policy_req_trigger_list ? am_policy_req_trigger_listList : NULL,
        pcf_ue_policy_uri && !cJSON_IsNull(pcf_ue_policy_uri) ? ogs_strdup(pcf_ue_policy_uri->valuestring) : NULL,
        ue_policy_req_trigger_list ? ue_policy_req_trigger_listList : NULL,
        hpcf_id && !cJSON_IsNull(hpcf_id) ? ogs_strdup(hpcf_id->valuestring) : NULL,
        hpcf_set_id && !cJSON_IsNull(hpcf_set_id) ? ogs_strdup(hpcf_set_id->valuestring) : NULL,
        restricted_rat_list ? restricted_rat_listList : NULL,
        forbidden_area_list ? forbidden_area_listList : NULL,
        service_area_restriction ? service_area_restriction_local_nonprim : NULL,
        restricted_core_nw_type_list ? restricted_core_nw_type_listList : NULL,
        event_subscription_list ? event_subscription_listList : NULL,
        mm_context_list ? mm_context_listList : NULL,
        session_context_list ? session_context_listList : NULL,
        eps_interworking_info ? eps_interworking_info_local_nonprim : NULL,
        trace_data && cJSON_IsNull(trace_data) ? true : false,
        trace_data ? trace_data_local_nonprim : NULL,
        service_gap_expiry_time && !cJSON_IsNull(service_gap_expiry_time) ? ogs_strdup(service_gap_expiry_time->valuestring) : NULL,
        stn_sr && !cJSON_IsNull(stn_sr) ? ogs_strdup(stn_sr->valuestring) : NULL,
        c_msisdn && !cJSON_IsNull(c_msisdn) ? ogs_strdup(c_msisdn->valuestring) : NULL,
        ms_classmark2 && !cJSON_IsNull(ms_classmark2) ? ogs_strdup(ms_classmark2->valuestring) : NULL,
        supported_codec_list ? supported_codec_listList : NULL,
        small_data_rate_status_infos ? small_data_rate_status_infosList : NULL,
        restricted_primary_rat_list ? restricted_primary_rat_listList : NULL,
        restricted_secondary_rat_list ? restricted_secondary_rat_listList : NULL,
        v2x_context ? v2x_context_local_nonprim : NULL,
        lte_cat_m_ind ? true : false,
        lte_cat_m_ind ? lte_cat_m_ind->valueint : 0,
        red_cap_ind ? true : false,
        red_cap_ind ? red_cap_ind->valueint : 0,
        mo_exp_data_counter ? mo_exp_data_counter_local_nonprim : NULL,
        cag_data ? cag_data_local_nonprim : NULL,
        management_mdt_ind ? true : false,
        management_mdt_ind ? management_mdt_ind->valueint : 0,
        immediate_mdt_conf ? immediate_mdt_conf_local_nonprim : NULL,
        ec_restriction_data_wb ? ec_restriction_data_wb_local_nonprim : NULL,
        ec_restriction_data_nb ? true : false,
        ec_restriction_data_nb ? ec_restriction_data_nb->valueint : 0,
        iab_operation_allowed ? true : false,
        iab_operation_allowed ? iab_operation_allowed->valueint : 0,
        prose_context ? prose_context_local_nonprim : NULL,
        analytics_subscription_list ? analytics_subscription_listList : NULL,
        pcf_amp_binding_info && !cJSON_IsNull(pcf_amp_binding_info) ? ogs_strdup(pcf_amp_binding_info->valuestring) : NULL,
        pcf_uep_binding_info && !cJSON_IsNull(pcf_uep_binding_info) ? ogs_strdup(pcf_uep_binding_info->valuestring) : NULL,
        used_service_area_restriction ? used_service_area_restriction_local_nonprim : NULL,
        pra_in_am_policy ? pra_in_am_policyList : NULL,
        pra_in_ue_policy ? pra_in_ue_policyList : NULL,
        updp_subscription_data ? updp_subscription_data_local_nonprim : NULL,
        sm_policy_notify_pdu_list ? sm_policy_notify_pdu_listList : NULL,
        pcf_ue_callback_info && cJSON_IsNull(pcf_ue_callback_info) ? true : false,
        pcf_ue_callback_info ? pcf_ue_callback_info_local_nonprim : NULL,
        ue_positioning_cap && !cJSON_IsNull(ue_positioning_cap) ? ogs_strdup(ue_positioning_cap->valuestring) : NULL,
        asti_distribution_indication ? true : false,
        asti_distribution_indication ? asti_distribution_indication->valueint : 0,
        ts_error_budget ? true : false,
        ts_error_budget ? ts_error_budget->valuedouble : 0,
        snpn_onboard_ind ? true : false,
        snpn_onboard_ind ? snpn_onboard_ind->valueint : 0,
        smf_sel_info && cJSON_IsNull(smf_sel_info) ? true : false,
        smf_sel_info ? smf_sel_info_local_nonprim : NULL,
        pcf_ue_slice_mbr_list ? pcf_ue_slice_mbr_listList : NULL,
        smsf_set_id && !cJSON_IsNull(smsf_set_id) ? ogs_strdup(smsf_set_id->valuestring) : NULL,
        smsf_service_set_id && !cJSON_IsNull(smsf_service_set_id) ? ogs_strdup(smsf_service_set_id->valuestring) : NULL,
        smsf_binding_info && !cJSON_IsNull(smsf_binding_info) ? ogs_strdup(smsf_binding_info->valuestring) : NULL,
        disaster_roaming_ind ? true : false,
        disaster_roaming_ind ? disaster_roaming_ind->valueint : 0,
        disaster_plmn ? disaster_plmn_local_nonprim : NULL
    );

    return ue_context_local_var;
end:
    if (gpsi_listList) {
        OpenAPI_list_for_each(gpsi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gpsi_listList);
        gpsi_listList = NULL;
    }
    if (group_listList) {
        OpenAPI_list_for_each(group_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(group_listList);
        group_listList = NULL;
    }
    if (sub_ue_ambr_local_nonprim) {
        OpenAPI_ambr_free(sub_ue_ambr_local_nonprim);
        sub_ue_ambr_local_nonprim = NULL;
    }
    if (sub_ue_slice_mbr_listList) {
        OpenAPI_list_for_each(sub_ue_slice_mbr_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_slice_mbr_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sub_ue_slice_mbr_listList);
        sub_ue_slice_mbr_listList = NULL;
    }
    if (seaf_data_local_nonprim) {
        OpenAPI_seaf_data_free(seaf_data_local_nonprim);
        seaf_data_local_nonprim = NULL;
    }
    if (am_policy_req_trigger_listList) {
        OpenAPI_list_free(am_policy_req_trigger_listList);
        am_policy_req_trigger_listList = NULL;
    }
    if (ue_policy_req_trigger_listList) {
        OpenAPI_list_free(ue_policy_req_trigger_listList);
        ue_policy_req_trigger_listList = NULL;
    }
    if (restricted_rat_listList) {
        OpenAPI_list_free(restricted_rat_listList);
        restricted_rat_listList = NULL;
    }
    if (forbidden_area_listList) {
        OpenAPI_list_for_each(forbidden_area_listList, node) {
            OpenAPI_area_free(node->data);
        }
        OpenAPI_list_free(forbidden_area_listList);
        forbidden_area_listList = NULL;
    }
    if (service_area_restriction_local_nonprim) {
        OpenAPI_service_area_restriction_free(service_area_restriction_local_nonprim);
        service_area_restriction_local_nonprim = NULL;
    }
    if (restricted_core_nw_type_listList) {
        OpenAPI_list_free(restricted_core_nw_type_listList);
        restricted_core_nw_type_listList = NULL;
    }
    if (event_subscription_listList) {
        OpenAPI_list_for_each(event_subscription_listList, node) {
            OpenAPI_ext_amf_event_subscription_free(node->data);
        }
        OpenAPI_list_free(event_subscription_listList);
        event_subscription_listList = NULL;
    }
    if (mm_context_listList) {
        OpenAPI_list_for_each(mm_context_listList, node) {
            OpenAPI_mm_context_free(node->data);
        }
        OpenAPI_list_free(mm_context_listList);
        mm_context_listList = NULL;
    }
    if (session_context_listList) {
        OpenAPI_list_for_each(session_context_listList, node) {
            OpenAPI_pdu_session_context_free(node->data);
        }
        OpenAPI_list_free(session_context_listList);
        session_context_listList = NULL;
    }
    if (eps_interworking_info_local_nonprim) {
        OpenAPI_eps_interworking_info_free(eps_interworking_info_local_nonprim);
        eps_interworking_info_local_nonprim = NULL;
    }
    if (trace_data_local_nonprim) {
        OpenAPI_trace_data_free(trace_data_local_nonprim);
        trace_data_local_nonprim = NULL;
    }
    if (supported_codec_listList) {
        OpenAPI_list_for_each(supported_codec_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supported_codec_listList);
        supported_codec_listList = NULL;
    }
    if (small_data_rate_status_infosList) {
        OpenAPI_list_for_each(small_data_rate_status_infosList, node) {
            OpenAPI_small_data_rate_status_info_free(node->data);
        }
        OpenAPI_list_free(small_data_rate_status_infosList);
        small_data_rate_status_infosList = NULL;
    }
    if (restricted_primary_rat_listList) {
        OpenAPI_list_free(restricted_primary_rat_listList);
        restricted_primary_rat_listList = NULL;
    }
    if (restricted_secondary_rat_listList) {
        OpenAPI_list_free(restricted_secondary_rat_listList);
        restricted_secondary_rat_listList = NULL;
    }
    if (v2x_context_local_nonprim) {
        OpenAPI_v2x_context_free(v2x_context_local_nonprim);
        v2x_context_local_nonprim = NULL;
    }
    if (mo_exp_data_counter_local_nonprim) {
        OpenAPI_mo_exp_data_counter_free(mo_exp_data_counter_local_nonprim);
        mo_exp_data_counter_local_nonprim = NULL;
    }
    if (cag_data_local_nonprim) {
        OpenAPI_cag_data_free(cag_data_local_nonprim);
        cag_data_local_nonprim = NULL;
    }
    if (immediate_mdt_conf_local_nonprim) {
        OpenAPI_immediate_mdt_conf_free(immediate_mdt_conf_local_nonprim);
        immediate_mdt_conf_local_nonprim = NULL;
    }
    if (ec_restriction_data_wb_local_nonprim) {
        OpenAPI_ec_restriction_data_wb_free(ec_restriction_data_wb_local_nonprim);
        ec_restriction_data_wb_local_nonprim = NULL;
    }
    if (prose_context_local_nonprim) {
        OpenAPI_prose_context_free(prose_context_local_nonprim);
        prose_context_local_nonprim = NULL;
    }
    if (analytics_subscription_listList) {
        OpenAPI_list_for_each(analytics_subscription_listList, node) {
            OpenAPI_analytics_subscription_free(node->data);
        }
        OpenAPI_list_free(analytics_subscription_listList);
        analytics_subscription_listList = NULL;
    }
    if (used_service_area_restriction_local_nonprim) {
        OpenAPI_service_area_restriction_free(used_service_area_restriction_local_nonprim);
        used_service_area_restriction_local_nonprim = NULL;
    }
    if (pra_in_am_policyList) {
        OpenAPI_list_for_each(pra_in_am_policyList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pra_in_am_policyList);
        pra_in_am_policyList = NULL;
    }
    if (pra_in_ue_policyList) {
        OpenAPI_list_for_each(pra_in_ue_policyList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pra_in_ue_policyList);
        pra_in_ue_policyList = NULL;
    }
    if (updp_subscription_data_local_nonprim) {
        OpenAPI_updp_subscription_data_free(updp_subscription_data_local_nonprim);
        updp_subscription_data_local_nonprim = NULL;
    }
    if (sm_policy_notify_pdu_listList) {
        OpenAPI_list_for_each(sm_policy_notify_pdu_listList, node) {
            OpenAPI_pdu_session_info_free(node->data);
        }
        OpenAPI_list_free(sm_policy_notify_pdu_listList);
        sm_policy_notify_pdu_listList = NULL;
    }
    if (pcf_ue_callback_info_local_nonprim) {
        OpenAPI_pcf_ue_callback_info_free(pcf_ue_callback_info_local_nonprim);
        pcf_ue_callback_info_local_nonprim = NULL;
    }
    if (smf_sel_info_local_nonprim) {
        OpenAPI_smf_selection_data_free(smf_sel_info_local_nonprim);
        smf_sel_info_local_nonprim = NULL;
    }
    if (pcf_ue_slice_mbr_listList) {
        OpenAPI_list_for_each(pcf_ue_slice_mbr_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_slice_mbr_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pcf_ue_slice_mbr_listList);
        pcf_ue_slice_mbr_listList = NULL;
    }
    if (disaster_plmn_local_nonprim) {
        OpenAPI_plmn_id_free(disaster_plmn_local_nonprim);
        disaster_plmn_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_context_t *OpenAPI_ue_context_copy(OpenAPI_ue_context_t *dst, OpenAPI_ue_context_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_ue_context_free(dst);
    dst = OpenAPI_ue_context_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

