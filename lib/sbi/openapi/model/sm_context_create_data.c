
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_create_data.h"

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
)
{
    OpenAPI_sm_context_create_data_t *sm_context_create_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_context_create_data_t));
    ogs_assert(sm_context_create_data_local_var);

    sm_context_create_data_local_var->supi = supi;
    sm_context_create_data_local_var->is_unauthenticated_supi = is_unauthenticated_supi;
    sm_context_create_data_local_var->unauthenticated_supi = unauthenticated_supi;
    sm_context_create_data_local_var->pei = pei;
    sm_context_create_data_local_var->gpsi = gpsi;
    sm_context_create_data_local_var->is_pdu_session_id = is_pdu_session_id;
    sm_context_create_data_local_var->pdu_session_id = pdu_session_id;
    sm_context_create_data_local_var->dnn = dnn;
    sm_context_create_data_local_var->selected_dnn = selected_dnn;
    sm_context_create_data_local_var->s_nssai = s_nssai;
    sm_context_create_data_local_var->hplmn_snssai = hplmn_snssai;
    sm_context_create_data_local_var->serving_nf_id = serving_nf_id;
    sm_context_create_data_local_var->guami = guami;
    sm_context_create_data_local_var->service_name = service_name;
    sm_context_create_data_local_var->serving_network = serving_network;
    sm_context_create_data_local_var->request_type = request_type;
    sm_context_create_data_local_var->n1_sm_msg = n1_sm_msg;
    sm_context_create_data_local_var->an_type = an_type;
    sm_context_create_data_local_var->additional_an_type = additional_an_type;
    sm_context_create_data_local_var->rat_type = rat_type;
    sm_context_create_data_local_var->presence_in_ladn = presence_in_ladn;
    sm_context_create_data_local_var->ue_location = ue_location;
    sm_context_create_data_local_var->ue_time_zone = ue_time_zone;
    sm_context_create_data_local_var->add_ue_location = add_ue_location;
    sm_context_create_data_local_var->sm_context_status_uri = sm_context_status_uri;
    sm_context_create_data_local_var->h_smf_uri = h_smf_uri;
    sm_context_create_data_local_var->h_smf_id = h_smf_id;
    sm_context_create_data_local_var->smf_uri = smf_uri;
    sm_context_create_data_local_var->smf_id = smf_id;
    sm_context_create_data_local_var->additional_hsmf_uri = additional_hsmf_uri;
    sm_context_create_data_local_var->additional_hsmf_id = additional_hsmf_id;
    sm_context_create_data_local_var->additional_smf_uri = additional_smf_uri;
    sm_context_create_data_local_var->additional_smf_id = additional_smf_id;
    sm_context_create_data_local_var->is_old_pdu_session_id = is_old_pdu_session_id;
    sm_context_create_data_local_var->old_pdu_session_id = old_pdu_session_id;
    sm_context_create_data_local_var->pdu_sessions_activate_list = pdu_sessions_activate_list;
    sm_context_create_data_local_var->ue_eps_pdn_connection = ue_eps_pdn_connection;
    sm_context_create_data_local_var->ho_state = ho_state;
    sm_context_create_data_local_var->pcf_id = pcf_id;
    sm_context_create_data_local_var->pcf_group_id = pcf_group_id;
    sm_context_create_data_local_var->pcf_set_id = pcf_set_id;
    sm_context_create_data_local_var->nrf_uri = nrf_uri;
    sm_context_create_data_local_var->supported_features = supported_features;
    sm_context_create_data_local_var->sel_mode = sel_mode;
    sm_context_create_data_local_var->backup_amf_info = backup_amf_info;
    sm_context_create_data_local_var->is_trace_data_null = is_trace_data_null;
    sm_context_create_data_local_var->trace_data = trace_data;
    sm_context_create_data_local_var->udm_group_id = udm_group_id;
    sm_context_create_data_local_var->routing_indicator = routing_indicator;
    sm_context_create_data_local_var->is_h_nw_pub_key_id = is_h_nw_pub_key_id;
    sm_context_create_data_local_var->h_nw_pub_key_id = h_nw_pub_key_id;
    sm_context_create_data_local_var->eps_interworking_ind = eps_interworking_ind;
    sm_context_create_data_local_var->is_indirect_forwarding_flag = is_indirect_forwarding_flag;
    sm_context_create_data_local_var->indirect_forwarding_flag = indirect_forwarding_flag;
    sm_context_create_data_local_var->is_direct_forwarding_flag = is_direct_forwarding_flag;
    sm_context_create_data_local_var->direct_forwarding_flag = direct_forwarding_flag;
    sm_context_create_data_local_var->target_id = target_id;
    sm_context_create_data_local_var->eps_bearer_ctx_status = eps_bearer_ctx_status;
    sm_context_create_data_local_var->is_cp_ciot_enabled = is_cp_ciot_enabled;
    sm_context_create_data_local_var->cp_ciot_enabled = cp_ciot_enabled;
    sm_context_create_data_local_var->is_cp_only_ind = is_cp_only_ind;
    sm_context_create_data_local_var->cp_only_ind = cp_only_ind;
    sm_context_create_data_local_var->is_invoke_nef = is_invoke_nef;
    sm_context_create_data_local_var->invoke_nef = invoke_nef;
    sm_context_create_data_local_var->is_ma_request_ind = is_ma_request_ind;
    sm_context_create_data_local_var->ma_request_ind = ma_request_ind;
    sm_context_create_data_local_var->is_ma_nw_upgrade_ind = is_ma_nw_upgrade_ind;
    sm_context_create_data_local_var->ma_nw_upgrade_ind = ma_nw_upgrade_ind;
    sm_context_create_data_local_var->n2_sm_info = n2_sm_info;
    sm_context_create_data_local_var->n2_sm_info_type = n2_sm_info_type;
    sm_context_create_data_local_var->n2_sm_info_ext1 = n2_sm_info_ext1;
    sm_context_create_data_local_var->n2_sm_info_type_ext1 = n2_sm_info_type_ext1;
    sm_context_create_data_local_var->sm_context_ref = sm_context_ref;
    sm_context_create_data_local_var->sm_context_smf_plmn_id = sm_context_smf_plmn_id;
    sm_context_create_data_local_var->sm_context_smf_id = sm_context_smf_id;
    sm_context_create_data_local_var->sm_context_smf_set_id = sm_context_smf_set_id;
    sm_context_create_data_local_var->sm_context_smf_service_set_id = sm_context_smf_service_set_id;
    sm_context_create_data_local_var->sm_context_smf_binding = sm_context_smf_binding;
    sm_context_create_data_local_var->up_cnx_state = up_cnx_state;
    sm_context_create_data_local_var->small_data_rate_status = small_data_rate_status;
    sm_context_create_data_local_var->apn_rate_status = apn_rate_status;
    sm_context_create_data_local_var->is_extended_nas_sm_timer_ind = is_extended_nas_sm_timer_ind;
    sm_context_create_data_local_var->extended_nas_sm_timer_ind = extended_nas_sm_timer_ind;
    sm_context_create_data_local_var->is_dl_data_waiting_ind = is_dl_data_waiting_ind;
    sm_context_create_data_local_var->dl_data_waiting_ind = dl_data_waiting_ind;
    sm_context_create_data_local_var->ddn_failure_subs = ddn_failure_subs;
    sm_context_create_data_local_var->is_smf_transfer_ind = is_smf_transfer_ind;
    sm_context_create_data_local_var->smf_transfer_ind = smf_transfer_ind;
    sm_context_create_data_local_var->old_smf_id = old_smf_id;
    sm_context_create_data_local_var->old_sm_context_ref = old_sm_context_ref;
    sm_context_create_data_local_var->w_agf_info = w_agf_info;
    sm_context_create_data_local_var->tngf_info = tngf_info;
    sm_context_create_data_local_var->twif_info = twif_info;
    sm_context_create_data_local_var->is_ran_unchanged_ind = is_ran_unchanged_ind;
    sm_context_create_data_local_var->ran_unchanged_ind = ran_unchanged_ind;
    sm_context_create_data_local_var->is_same_pcf_selection_ind = is_same_pcf_selection_ind;
    sm_context_create_data_local_var->same_pcf_selection_ind = same_pcf_selection_ind;
    sm_context_create_data_local_var->target_dnai = target_dnai;
    sm_context_create_data_local_var->nrf_management_uri = nrf_management_uri;
    sm_context_create_data_local_var->nrf_discovery_uri = nrf_discovery_uri;
    sm_context_create_data_local_var->nrf_access_token_uri = nrf_access_token_uri;
    sm_context_create_data_local_var->nrf_oauth2_required = nrf_oauth2_required;
    sm_context_create_data_local_var->smf_binding_info = smf_binding_info;
    sm_context_create_data_local_var->pvs_info = pvs_info;
    sm_context_create_data_local_var->is_onboarding_ind = is_onboarding_ind;
    sm_context_create_data_local_var->onboarding_ind = onboarding_ind;
    sm_context_create_data_local_var->old_pdu_session_ref = old_pdu_session_ref;
    sm_context_create_data_local_var->is_sm_policy_notify_ind = is_sm_policy_notify_ind;
    sm_context_create_data_local_var->sm_policy_notify_ind = sm_policy_notify_ind;
    sm_context_create_data_local_var->is_pcf_ue_callback_info_null = is_pcf_ue_callback_info_null;
    sm_context_create_data_local_var->pcf_ue_callback_info = pcf_ue_callback_info;
    sm_context_create_data_local_var->satellite_backhaul_cat = satellite_backhaul_cat;
    sm_context_create_data_local_var->is_upip_supported = is_upip_supported;
    sm_context_create_data_local_var->upip_supported = upip_supported;
    sm_context_create_data_local_var->is_uav_authenticated = is_uav_authenticated;
    sm_context_create_data_local_var->uav_authenticated = uav_authenticated;
    sm_context_create_data_local_var->is_disaster_roaming_ind = is_disaster_roaming_ind;
    sm_context_create_data_local_var->disaster_roaming_ind = disaster_roaming_ind;
    sm_context_create_data_local_var->is_anchor_smf_oauth2_required = is_anchor_smf_oauth2_required;
    sm_context_create_data_local_var->anchor_smf_oauth2_required = anchor_smf_oauth2_required;
    sm_context_create_data_local_var->is_sm_context_smf_oauth2_required = is_sm_context_smf_oauth2_required;
    sm_context_create_data_local_var->sm_context_smf_oauth2_required = sm_context_smf_oauth2_required;

    return sm_context_create_data_local_var;
}

void OpenAPI_sm_context_create_data_free(OpenAPI_sm_context_create_data_t *sm_context_create_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_context_create_data) {
        return;
    }
    if (sm_context_create_data->supi) {
        ogs_free(sm_context_create_data->supi);
        sm_context_create_data->supi = NULL;
    }
    if (sm_context_create_data->pei) {
        ogs_free(sm_context_create_data->pei);
        sm_context_create_data->pei = NULL;
    }
    if (sm_context_create_data->gpsi) {
        ogs_free(sm_context_create_data->gpsi);
        sm_context_create_data->gpsi = NULL;
    }
    if (sm_context_create_data->dnn) {
        ogs_free(sm_context_create_data->dnn);
        sm_context_create_data->dnn = NULL;
    }
    if (sm_context_create_data->selected_dnn) {
        ogs_free(sm_context_create_data->selected_dnn);
        sm_context_create_data->selected_dnn = NULL;
    }
    if (sm_context_create_data->s_nssai) {
        OpenAPI_snssai_free(sm_context_create_data->s_nssai);
        sm_context_create_data->s_nssai = NULL;
    }
    if (sm_context_create_data->hplmn_snssai) {
        OpenAPI_snssai_free(sm_context_create_data->hplmn_snssai);
        sm_context_create_data->hplmn_snssai = NULL;
    }
    if (sm_context_create_data->serving_nf_id) {
        ogs_free(sm_context_create_data->serving_nf_id);
        sm_context_create_data->serving_nf_id = NULL;
    }
    if (sm_context_create_data->guami) {
        OpenAPI_guami_free(sm_context_create_data->guami);
        sm_context_create_data->guami = NULL;
    }
    if (sm_context_create_data->service_name) {
        ogs_free(sm_context_create_data->service_name);
        sm_context_create_data->service_name = NULL;
    }
    if (sm_context_create_data->serving_network) {
        OpenAPI_plmn_id_nid_free(sm_context_create_data->serving_network);
        sm_context_create_data->serving_network = NULL;
    }
    if (sm_context_create_data->n1_sm_msg) {
        OpenAPI_ref_to_binary_data_free(sm_context_create_data->n1_sm_msg);
        sm_context_create_data->n1_sm_msg = NULL;
    }
    if (sm_context_create_data->ue_location) {
        OpenAPI_user_location_free(sm_context_create_data->ue_location);
        sm_context_create_data->ue_location = NULL;
    }
    if (sm_context_create_data->ue_time_zone) {
        ogs_free(sm_context_create_data->ue_time_zone);
        sm_context_create_data->ue_time_zone = NULL;
    }
    if (sm_context_create_data->add_ue_location) {
        OpenAPI_user_location_free(sm_context_create_data->add_ue_location);
        sm_context_create_data->add_ue_location = NULL;
    }
    if (sm_context_create_data->sm_context_status_uri) {
        ogs_free(sm_context_create_data->sm_context_status_uri);
        sm_context_create_data->sm_context_status_uri = NULL;
    }
    if (sm_context_create_data->h_smf_uri) {
        ogs_free(sm_context_create_data->h_smf_uri);
        sm_context_create_data->h_smf_uri = NULL;
    }
    if (sm_context_create_data->h_smf_id) {
        ogs_free(sm_context_create_data->h_smf_id);
        sm_context_create_data->h_smf_id = NULL;
    }
    if (sm_context_create_data->smf_uri) {
        ogs_free(sm_context_create_data->smf_uri);
        sm_context_create_data->smf_uri = NULL;
    }
    if (sm_context_create_data->smf_id) {
        ogs_free(sm_context_create_data->smf_id);
        sm_context_create_data->smf_id = NULL;
    }
    if (sm_context_create_data->additional_hsmf_uri) {
        OpenAPI_list_for_each(sm_context_create_data->additional_hsmf_uri, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_create_data->additional_hsmf_uri);
        sm_context_create_data->additional_hsmf_uri = NULL;
    }
    if (sm_context_create_data->additional_hsmf_id) {
        OpenAPI_list_for_each(sm_context_create_data->additional_hsmf_id, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_create_data->additional_hsmf_id);
        sm_context_create_data->additional_hsmf_id = NULL;
    }
    if (sm_context_create_data->additional_smf_uri) {
        OpenAPI_list_for_each(sm_context_create_data->additional_smf_uri, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_create_data->additional_smf_uri);
        sm_context_create_data->additional_smf_uri = NULL;
    }
    if (sm_context_create_data->additional_smf_id) {
        OpenAPI_list_for_each(sm_context_create_data->additional_smf_id, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_create_data->additional_smf_id);
        sm_context_create_data->additional_smf_id = NULL;
    }
    if (sm_context_create_data->pdu_sessions_activate_list) {
        OpenAPI_list_for_each(sm_context_create_data->pdu_sessions_activate_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_create_data->pdu_sessions_activate_list);
        sm_context_create_data->pdu_sessions_activate_list = NULL;
    }
    if (sm_context_create_data->ue_eps_pdn_connection) {
        ogs_free(sm_context_create_data->ue_eps_pdn_connection);
        sm_context_create_data->ue_eps_pdn_connection = NULL;
    }
    if (sm_context_create_data->pcf_id) {
        ogs_free(sm_context_create_data->pcf_id);
        sm_context_create_data->pcf_id = NULL;
    }
    if (sm_context_create_data->pcf_group_id) {
        ogs_free(sm_context_create_data->pcf_group_id);
        sm_context_create_data->pcf_group_id = NULL;
    }
    if (sm_context_create_data->pcf_set_id) {
        ogs_free(sm_context_create_data->pcf_set_id);
        sm_context_create_data->pcf_set_id = NULL;
    }
    if (sm_context_create_data->nrf_uri) {
        ogs_free(sm_context_create_data->nrf_uri);
        sm_context_create_data->nrf_uri = NULL;
    }
    if (sm_context_create_data->supported_features) {
        ogs_free(sm_context_create_data->supported_features);
        sm_context_create_data->supported_features = NULL;
    }
    if (sm_context_create_data->backup_amf_info) {
        OpenAPI_list_for_each(sm_context_create_data->backup_amf_info, node) {
            OpenAPI_backup_amf_info_free(node->data);
        }
        OpenAPI_list_free(sm_context_create_data->backup_amf_info);
        sm_context_create_data->backup_amf_info = NULL;
    }
    if (sm_context_create_data->trace_data) {
        OpenAPI_trace_data_free(sm_context_create_data->trace_data);
        sm_context_create_data->trace_data = NULL;
    }
    if (sm_context_create_data->udm_group_id) {
        ogs_free(sm_context_create_data->udm_group_id);
        sm_context_create_data->udm_group_id = NULL;
    }
    if (sm_context_create_data->routing_indicator) {
        ogs_free(sm_context_create_data->routing_indicator);
        sm_context_create_data->routing_indicator = NULL;
    }
    if (sm_context_create_data->target_id) {
        OpenAPI_ng_ran_target_id_free(sm_context_create_data->target_id);
        sm_context_create_data->target_id = NULL;
    }
    if (sm_context_create_data->eps_bearer_ctx_status) {
        ogs_free(sm_context_create_data->eps_bearer_ctx_status);
        sm_context_create_data->eps_bearer_ctx_status = NULL;
    }
    if (sm_context_create_data->n2_sm_info) {
        OpenAPI_ref_to_binary_data_free(sm_context_create_data->n2_sm_info);
        sm_context_create_data->n2_sm_info = NULL;
    }
    if (sm_context_create_data->n2_sm_info_ext1) {
        OpenAPI_ref_to_binary_data_free(sm_context_create_data->n2_sm_info_ext1);
        sm_context_create_data->n2_sm_info_ext1 = NULL;
    }
    if (sm_context_create_data->sm_context_ref) {
        ogs_free(sm_context_create_data->sm_context_ref);
        sm_context_create_data->sm_context_ref = NULL;
    }
    if (sm_context_create_data->sm_context_smf_plmn_id) {
        OpenAPI_plmn_id_nid_free(sm_context_create_data->sm_context_smf_plmn_id);
        sm_context_create_data->sm_context_smf_plmn_id = NULL;
    }
    if (sm_context_create_data->sm_context_smf_id) {
        ogs_free(sm_context_create_data->sm_context_smf_id);
        sm_context_create_data->sm_context_smf_id = NULL;
    }
    if (sm_context_create_data->sm_context_smf_set_id) {
        ogs_free(sm_context_create_data->sm_context_smf_set_id);
        sm_context_create_data->sm_context_smf_set_id = NULL;
    }
    if (sm_context_create_data->sm_context_smf_service_set_id) {
        ogs_free(sm_context_create_data->sm_context_smf_service_set_id);
        sm_context_create_data->sm_context_smf_service_set_id = NULL;
    }
    if (sm_context_create_data->small_data_rate_status) {
        OpenAPI_small_data_rate_status_free(sm_context_create_data->small_data_rate_status);
        sm_context_create_data->small_data_rate_status = NULL;
    }
    if (sm_context_create_data->apn_rate_status) {
        OpenAPI_apn_rate_status_free(sm_context_create_data->apn_rate_status);
        sm_context_create_data->apn_rate_status = NULL;
    }
    if (sm_context_create_data->ddn_failure_subs) {
        OpenAPI_ddn_failure_subs_free(sm_context_create_data->ddn_failure_subs);
        sm_context_create_data->ddn_failure_subs = NULL;
    }
    if (sm_context_create_data->old_smf_id) {
        ogs_free(sm_context_create_data->old_smf_id);
        sm_context_create_data->old_smf_id = NULL;
    }
    if (sm_context_create_data->old_sm_context_ref) {
        ogs_free(sm_context_create_data->old_sm_context_ref);
        sm_context_create_data->old_sm_context_ref = NULL;
    }
    if (sm_context_create_data->w_agf_info) {
        OpenAPI_w_agf_info_free(sm_context_create_data->w_agf_info);
        sm_context_create_data->w_agf_info = NULL;
    }
    if (sm_context_create_data->tngf_info) {
        OpenAPI_tngf_info_free(sm_context_create_data->tngf_info);
        sm_context_create_data->tngf_info = NULL;
    }
    if (sm_context_create_data->twif_info) {
        OpenAPI_twif_info_free(sm_context_create_data->twif_info);
        sm_context_create_data->twif_info = NULL;
    }
    if (sm_context_create_data->target_dnai) {
        ogs_free(sm_context_create_data->target_dnai);
        sm_context_create_data->target_dnai = NULL;
    }
    if (sm_context_create_data->nrf_management_uri) {
        ogs_free(sm_context_create_data->nrf_management_uri);
        sm_context_create_data->nrf_management_uri = NULL;
    }
    if (sm_context_create_data->nrf_discovery_uri) {
        ogs_free(sm_context_create_data->nrf_discovery_uri);
        sm_context_create_data->nrf_discovery_uri = NULL;
    }
    if (sm_context_create_data->nrf_access_token_uri) {
        ogs_free(sm_context_create_data->nrf_access_token_uri);
        sm_context_create_data->nrf_access_token_uri = NULL;
    }
    if (sm_context_create_data->nrf_oauth2_required) {
        OpenAPI_list_for_each(sm_context_create_data->nrf_oauth2_required, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_context_create_data->nrf_oauth2_required);
        sm_context_create_data->nrf_oauth2_required = NULL;
    }
    if (sm_context_create_data->smf_binding_info) {
        ogs_free(sm_context_create_data->smf_binding_info);
        sm_context_create_data->smf_binding_info = NULL;
    }
    if (sm_context_create_data->pvs_info) {
        OpenAPI_list_for_each(sm_context_create_data->pvs_info, node) {
            OpenAPI_server_addressing_info_free(node->data);
        }
        OpenAPI_list_free(sm_context_create_data->pvs_info);
        sm_context_create_data->pvs_info = NULL;
    }
    if (sm_context_create_data->old_pdu_session_ref) {
        ogs_free(sm_context_create_data->old_pdu_session_ref);
        sm_context_create_data->old_pdu_session_ref = NULL;
    }
    if (sm_context_create_data->pcf_ue_callback_info) {
        OpenAPI_pcf_ue_callback_info_free(sm_context_create_data->pcf_ue_callback_info);
        sm_context_create_data->pcf_ue_callback_info = NULL;
    }
    ogs_free(sm_context_create_data);
}

cJSON *OpenAPI_sm_context_create_data_convertToJSON(OpenAPI_sm_context_create_data_t *sm_context_create_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_context_create_data == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [SmContextCreateData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_context_create_data->supi) {
    if (cJSON_AddStringToObject(item, "supi", sm_context_create_data->supi) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (sm_context_create_data->is_unauthenticated_supi) {
    if (cJSON_AddBoolToObject(item, "unauthenticatedSupi", sm_context_create_data->unauthenticated_supi) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [unauthenticated_supi]");
        goto end;
    }
    }

    if (sm_context_create_data->pei) {
    if (cJSON_AddStringToObject(item, "pei", sm_context_create_data->pei) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (sm_context_create_data->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", sm_context_create_data->gpsi) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (sm_context_create_data->is_pdu_session_id) {
    if (cJSON_AddNumberToObject(item, "pduSessionId", sm_context_create_data->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pdu_session_id]");
        goto end;
    }
    }

    if (sm_context_create_data->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", sm_context_create_data->dnn) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (sm_context_create_data->selected_dnn) {
    if (cJSON_AddStringToObject(item, "selectedDnn", sm_context_create_data->selected_dnn) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [selected_dnn]");
        goto end;
    }
    }

    if (sm_context_create_data->s_nssai) {
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(sm_context_create_data->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [s_nssai]");
        goto end;
    }
    }

    if (sm_context_create_data->hplmn_snssai) {
    cJSON *hplmn_snssai_local_JSON = OpenAPI_snssai_convertToJSON(sm_context_create_data->hplmn_snssai);
    if (hplmn_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [hplmn_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hplmnSnssai", hplmn_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [hplmn_snssai]");
        goto end;
    }
    }

    if (!sm_context_create_data->serving_nf_id) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [serving_nf_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "servingNfId", sm_context_create_data->serving_nf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [serving_nf_id]");
        goto end;
    }

    if (sm_context_create_data->guami) {
    cJSON *guami_local_JSON = OpenAPI_guami_convertToJSON(sm_context_create_data->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [guami]");
        goto end;
    }
    }

    if (sm_context_create_data->service_name) {
    if (cJSON_AddStringToObject(item, "serviceName", sm_context_create_data->service_name) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [service_name]");
        goto end;
    }
    }

    if (!sm_context_create_data->serving_network) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [serving_network]");
        return NULL;
    }
    cJSON *serving_network_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(sm_context_create_data->serving_network);
    if (serving_network_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetwork", serving_network_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [serving_network]");
        goto end;
    }

    if (sm_context_create_data->request_type != OpenAPI_request_type_NULL) {
    if (cJSON_AddStringToObject(item, "requestType", OpenAPI_request_type_ToString(sm_context_create_data->request_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [request_type]");
        goto end;
    }
    }

    if (sm_context_create_data->n1_sm_msg) {
    cJSON *n1_sm_msg_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(sm_context_create_data->n1_sm_msg);
    if (n1_sm_msg_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [n1_sm_msg]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n1SmMsg", n1_sm_msg_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [n1_sm_msg]");
        goto end;
    }
    }

    if (sm_context_create_data->an_type == OpenAPI_access_type_NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [an_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "anType", OpenAPI_access_type_ToString(sm_context_create_data->an_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [an_type]");
        goto end;
    }

    if (sm_context_create_data->additional_an_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "additionalAnType", OpenAPI_access_type_ToString(sm_context_create_data->additional_an_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_an_type]");
        goto end;
    }
    }

    if (sm_context_create_data->rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(sm_context_create_data->rat_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (sm_context_create_data->presence_in_ladn != OpenAPI_presence_state_NULL) {
    if (cJSON_AddStringToObject(item, "presenceInLadn", OpenAPI_presence_state_ToString(sm_context_create_data->presence_in_ladn)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [presence_in_ladn]");
        goto end;
    }
    }

    if (sm_context_create_data->ue_location) {
    cJSON *ue_location_local_JSON = OpenAPI_user_location_convertToJSON(sm_context_create_data->ue_location);
    if (ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLocation", ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    }

    if (sm_context_create_data->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", sm_context_create_data->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (sm_context_create_data->add_ue_location) {
    cJSON *add_ue_location_local_JSON = OpenAPI_user_location_convertToJSON(sm_context_create_data->add_ue_location);
    if (add_ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [add_ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "addUeLocation", add_ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [add_ue_location]");
        goto end;
    }
    }

    if (!sm_context_create_data->sm_context_status_uri) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [sm_context_status_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "smContextStatusUri", sm_context_create_data->sm_context_status_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [sm_context_status_uri]");
        goto end;
    }

    if (sm_context_create_data->h_smf_uri) {
    if (cJSON_AddStringToObject(item, "hSmfUri", sm_context_create_data->h_smf_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [h_smf_uri]");
        goto end;
    }
    }

    if (sm_context_create_data->h_smf_id) {
    if (cJSON_AddStringToObject(item, "hSmfId", sm_context_create_data->h_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [h_smf_id]");
        goto end;
    }
    }

    if (sm_context_create_data->smf_uri) {
    if (cJSON_AddStringToObject(item, "smfUri", sm_context_create_data->smf_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [smf_uri]");
        goto end;
    }
    }

    if (sm_context_create_data->smf_id) {
    if (cJSON_AddStringToObject(item, "smfId", sm_context_create_data->smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [smf_id]");
        goto end;
    }
    }

    if (sm_context_create_data->additional_hsmf_uri) {
    cJSON *additional_hsmf_uriList = cJSON_AddArrayToObject(item, "additionalHsmfUri");
    if (additional_hsmf_uriList == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_hsmf_uri]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_create_data->additional_hsmf_uri, node) {
        if (cJSON_AddStringToObject(additional_hsmf_uriList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_hsmf_uri]");
            goto end;
        }
    }
    }

    if (sm_context_create_data->additional_hsmf_id) {
    cJSON *additional_hsmf_idList = cJSON_AddArrayToObject(item, "additionalHsmfId");
    if (additional_hsmf_idList == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_hsmf_id]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_create_data->additional_hsmf_id, node) {
        if (cJSON_AddStringToObject(additional_hsmf_idList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_hsmf_id]");
            goto end;
        }
    }
    }

    if (sm_context_create_data->additional_smf_uri) {
    cJSON *additional_smf_uriList = cJSON_AddArrayToObject(item, "additionalSmfUri");
    if (additional_smf_uriList == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_smf_uri]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_create_data->additional_smf_uri, node) {
        if (cJSON_AddStringToObject(additional_smf_uriList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_smf_uri]");
            goto end;
        }
    }
    }

    if (sm_context_create_data->additional_smf_id) {
    cJSON *additional_smf_idList = cJSON_AddArrayToObject(item, "additionalSmfId");
    if (additional_smf_idList == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_smf_id]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_create_data->additional_smf_id, node) {
        if (cJSON_AddStringToObject(additional_smf_idList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_smf_id]");
            goto end;
        }
    }
    }

    if (sm_context_create_data->is_old_pdu_session_id) {
    if (cJSON_AddNumberToObject(item, "oldPduSessionId", sm_context_create_data->old_pdu_session_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [old_pdu_session_id]");
        goto end;
    }
    }

    if (sm_context_create_data->pdu_sessions_activate_list) {
    cJSON *pdu_sessions_activate_listList = cJSON_AddArrayToObject(item, "pduSessionsActivateList");
    if (pdu_sessions_activate_listList == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pdu_sessions_activate_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_create_data->pdu_sessions_activate_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pdu_sessions_activate_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(pdu_sessions_activate_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pdu_sessions_activate_list]");
            goto end;
        }
    }
    }

    if (sm_context_create_data->ue_eps_pdn_connection) {
    if (cJSON_AddStringToObject(item, "ueEpsPdnConnection", sm_context_create_data->ue_eps_pdn_connection) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [ue_eps_pdn_connection]");
        goto end;
    }
    }

    if (sm_context_create_data->ho_state != OpenAPI_ho_state_NULL) {
    if (cJSON_AddStringToObject(item, "hoState", OpenAPI_ho_state_ToString(sm_context_create_data->ho_state)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [ho_state]");
        goto end;
    }
    }

    if (sm_context_create_data->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", sm_context_create_data->pcf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (sm_context_create_data->pcf_group_id) {
    if (cJSON_AddStringToObject(item, "pcfGroupId", sm_context_create_data->pcf_group_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pcf_group_id]");
        goto end;
    }
    }

    if (sm_context_create_data->pcf_set_id) {
    if (cJSON_AddStringToObject(item, "pcfSetId", sm_context_create_data->pcf_set_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    if (sm_context_create_data->nrf_uri) {
    if (cJSON_AddStringToObject(item, "nrfUri", sm_context_create_data->nrf_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [nrf_uri]");
        goto end;
    }
    }

    if (sm_context_create_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", sm_context_create_data->supported_features) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (sm_context_create_data->sel_mode != OpenAPI_dnn_selection_mode_NULL) {
    if (cJSON_AddStringToObject(item, "selMode", OpenAPI_dnn_selection_mode_ToString(sm_context_create_data->sel_mode)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [sel_mode]");
        goto end;
    }
    }

    if (sm_context_create_data->backup_amf_info) {
    cJSON *backup_amf_infoList = cJSON_AddArrayToObject(item, "backupAmfInfo");
    if (backup_amf_infoList == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [backup_amf_info]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_create_data->backup_amf_info, node) {
        cJSON *itemLocal = OpenAPI_backup_amf_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [backup_amf_info]");
            goto end;
        }
        cJSON_AddItemToArray(backup_amf_infoList, itemLocal);
    }
    }

    if (sm_context_create_data->trace_data) {
    cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(sm_context_create_data->trace_data);
    if (trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [trace_data]");
        goto end;
    }
    } else if (sm_context_create_data->is_trace_data_null) {
        if (cJSON_AddNullToObject(item, "traceData") == NULL) {
            ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

    if (sm_context_create_data->udm_group_id) {
    if (cJSON_AddStringToObject(item, "udmGroupId", sm_context_create_data->udm_group_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [udm_group_id]");
        goto end;
    }
    }

    if (sm_context_create_data->routing_indicator) {
    if (cJSON_AddStringToObject(item, "routingIndicator", sm_context_create_data->routing_indicator) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [routing_indicator]");
        goto end;
    }
    }

    if (sm_context_create_data->is_h_nw_pub_key_id) {
    if (cJSON_AddNumberToObject(item, "hNwPubKeyId", sm_context_create_data->h_nw_pub_key_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [h_nw_pub_key_id]");
        goto end;
    }
    }

    if (sm_context_create_data->eps_interworking_ind != OpenAPI_eps_interworking_indication_NULL) {
    if (cJSON_AddStringToObject(item, "epsInterworkingInd", OpenAPI_eps_interworking_indication_ToString(sm_context_create_data->eps_interworking_ind)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [eps_interworking_ind]");
        goto end;
    }
    }

    if (sm_context_create_data->is_indirect_forwarding_flag) {
    if (cJSON_AddBoolToObject(item, "indirectForwardingFlag", sm_context_create_data->indirect_forwarding_flag) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [indirect_forwarding_flag]");
        goto end;
    }
    }

    if (sm_context_create_data->is_direct_forwarding_flag) {
    if (cJSON_AddBoolToObject(item, "directForwardingFlag", sm_context_create_data->direct_forwarding_flag) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [direct_forwarding_flag]");
        goto end;
    }
    }

    if (sm_context_create_data->target_id) {
    cJSON *target_id_local_JSON = OpenAPI_ng_ran_target_id_convertToJSON(sm_context_create_data->target_id);
    if (target_id_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [target_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetId", target_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [target_id]");
        goto end;
    }
    }

    if (sm_context_create_data->eps_bearer_ctx_status) {
    if (cJSON_AddStringToObject(item, "epsBearerCtxStatus", sm_context_create_data->eps_bearer_ctx_status) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [eps_bearer_ctx_status]");
        goto end;
    }
    }

    if (sm_context_create_data->is_cp_ciot_enabled) {
    if (cJSON_AddBoolToObject(item, "cpCiotEnabled", sm_context_create_data->cp_ciot_enabled) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [cp_ciot_enabled]");
        goto end;
    }
    }

    if (sm_context_create_data->is_cp_only_ind) {
    if (cJSON_AddBoolToObject(item, "cpOnlyInd", sm_context_create_data->cp_only_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [cp_only_ind]");
        goto end;
    }
    }

    if (sm_context_create_data->is_invoke_nef) {
    if (cJSON_AddBoolToObject(item, "invokeNef", sm_context_create_data->invoke_nef) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [invoke_nef]");
        goto end;
    }
    }

    if (sm_context_create_data->is_ma_request_ind) {
    if (cJSON_AddBoolToObject(item, "maRequestInd", sm_context_create_data->ma_request_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [ma_request_ind]");
        goto end;
    }
    }

    if (sm_context_create_data->is_ma_nw_upgrade_ind) {
    if (cJSON_AddBoolToObject(item, "maNwUpgradeInd", sm_context_create_data->ma_nw_upgrade_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [ma_nw_upgrade_ind]");
        goto end;
    }
    }

    if (sm_context_create_data->n2_sm_info) {
    cJSON *n2_sm_info_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(sm_context_create_data->n2_sm_info);
    if (n2_sm_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [n2_sm_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2SmInfo", n2_sm_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [n2_sm_info]");
        goto end;
    }
    }

    if (sm_context_create_data->n2_sm_info_type != OpenAPI_n2_sm_info_type_NULL) {
    if (cJSON_AddStringToObject(item, "n2SmInfoType", OpenAPI_n2_sm_info_type_ToString(sm_context_create_data->n2_sm_info_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [n2_sm_info_type]");
        goto end;
    }
    }

    if (sm_context_create_data->n2_sm_info_ext1) {
    cJSON *n2_sm_info_ext1_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(sm_context_create_data->n2_sm_info_ext1);
    if (n2_sm_info_ext1_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [n2_sm_info_ext1]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2SmInfoExt1", n2_sm_info_ext1_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [n2_sm_info_ext1]");
        goto end;
    }
    }

    if (sm_context_create_data->n2_sm_info_type_ext1 != OpenAPI_n2_sm_info_type_NULL) {
    if (cJSON_AddStringToObject(item, "n2SmInfoTypeExt1", OpenAPI_n2_sm_info_type_ToString(sm_context_create_data->n2_sm_info_type_ext1)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [n2_sm_info_type_ext1]");
        goto end;
    }
    }

    if (sm_context_create_data->sm_context_ref) {
    if (cJSON_AddStringToObject(item, "smContextRef", sm_context_create_data->sm_context_ref) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [sm_context_ref]");
        goto end;
    }
    }

    if (sm_context_create_data->sm_context_smf_plmn_id) {
    cJSON *sm_context_smf_plmn_id_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(sm_context_create_data->sm_context_smf_plmn_id);
    if (sm_context_smf_plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [sm_context_smf_plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smContextSmfPlmnId", sm_context_smf_plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [sm_context_smf_plmn_id]");
        goto end;
    }
    }

    if (sm_context_create_data->sm_context_smf_id) {
    if (cJSON_AddStringToObject(item, "smContextSmfId", sm_context_create_data->sm_context_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [sm_context_smf_id]");
        goto end;
    }
    }

    if (sm_context_create_data->sm_context_smf_set_id) {
    if (cJSON_AddStringToObject(item, "smContextSmfSetId", sm_context_create_data->sm_context_smf_set_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [sm_context_smf_set_id]");
        goto end;
    }
    }

    if (sm_context_create_data->sm_context_smf_service_set_id) {
    if (cJSON_AddStringToObject(item, "smContextSmfServiceSetId", sm_context_create_data->sm_context_smf_service_set_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [sm_context_smf_service_set_id]");
        goto end;
    }
    }

    if (sm_context_create_data->sm_context_smf_binding != OpenAPI_sbi_binding_level_NULL) {
    if (cJSON_AddStringToObject(item, "smContextSmfBinding", OpenAPI_sbi_binding_level_ToString(sm_context_create_data->sm_context_smf_binding)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [sm_context_smf_binding]");
        goto end;
    }
    }

    if (sm_context_create_data->up_cnx_state != OpenAPI_up_cnx_state_NULL) {
    if (cJSON_AddStringToObject(item, "upCnxState", OpenAPI_up_cnx_state_ToString(sm_context_create_data->up_cnx_state)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [up_cnx_state]");
        goto end;
    }
    }

    if (sm_context_create_data->small_data_rate_status) {
    cJSON *small_data_rate_status_local_JSON = OpenAPI_small_data_rate_status_convertToJSON(sm_context_create_data->small_data_rate_status);
    if (small_data_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smallDataRateStatus", small_data_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    }

    if (sm_context_create_data->apn_rate_status) {
    cJSON *apn_rate_status_local_JSON = OpenAPI_apn_rate_status_convertToJSON(sm_context_create_data->apn_rate_status);
    if (apn_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "apnRateStatus", apn_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    }

    if (sm_context_create_data->is_extended_nas_sm_timer_ind) {
    if (cJSON_AddBoolToObject(item, "extendedNasSmTimerInd", sm_context_create_data->extended_nas_sm_timer_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [extended_nas_sm_timer_ind]");
        goto end;
    }
    }

    if (sm_context_create_data->is_dl_data_waiting_ind) {
    if (cJSON_AddBoolToObject(item, "dlDataWaitingInd", sm_context_create_data->dl_data_waiting_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [dl_data_waiting_ind]");
        goto end;
    }
    }

    if (sm_context_create_data->ddn_failure_subs) {
    cJSON *ddn_failure_subs_local_JSON = OpenAPI_ddn_failure_subs_convertToJSON(sm_context_create_data->ddn_failure_subs);
    if (ddn_failure_subs_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [ddn_failure_subs]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ddnFailureSubs", ddn_failure_subs_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [ddn_failure_subs]");
        goto end;
    }
    }

    if (sm_context_create_data->is_smf_transfer_ind) {
    if (cJSON_AddBoolToObject(item, "smfTransferInd", sm_context_create_data->smf_transfer_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [smf_transfer_ind]");
        goto end;
    }
    }

    if (sm_context_create_data->old_smf_id) {
    if (cJSON_AddStringToObject(item, "oldSmfId", sm_context_create_data->old_smf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [old_smf_id]");
        goto end;
    }
    }

    if (sm_context_create_data->old_sm_context_ref) {
    if (cJSON_AddStringToObject(item, "oldSmContextRef", sm_context_create_data->old_sm_context_ref) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [old_sm_context_ref]");
        goto end;
    }
    }

    if (sm_context_create_data->w_agf_info) {
    cJSON *w_agf_info_local_JSON = OpenAPI_w_agf_info_convertToJSON(sm_context_create_data->w_agf_info);
    if (w_agf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [w_agf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "wAgfInfo", w_agf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [w_agf_info]");
        goto end;
    }
    }

    if (sm_context_create_data->tngf_info) {
    cJSON *tngf_info_local_JSON = OpenAPI_tngf_info_convertToJSON(sm_context_create_data->tngf_info);
    if (tngf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [tngf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tngfInfo", tngf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [tngf_info]");
        goto end;
    }
    }

    if (sm_context_create_data->twif_info) {
    cJSON *twif_info_local_JSON = OpenAPI_twif_info_convertToJSON(sm_context_create_data->twif_info);
    if (twif_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [twif_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "twifInfo", twif_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [twif_info]");
        goto end;
    }
    }

    if (sm_context_create_data->is_ran_unchanged_ind) {
    if (cJSON_AddBoolToObject(item, "ranUnchangedInd", sm_context_create_data->ran_unchanged_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [ran_unchanged_ind]");
        goto end;
    }
    }

    if (sm_context_create_data->is_same_pcf_selection_ind) {
    if (cJSON_AddBoolToObject(item, "samePcfSelectionInd", sm_context_create_data->same_pcf_selection_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [same_pcf_selection_ind]");
        goto end;
    }
    }

    if (sm_context_create_data->target_dnai) {
    if (cJSON_AddStringToObject(item, "targetDnai", sm_context_create_data->target_dnai) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [target_dnai]");
        goto end;
    }
    }

    if (sm_context_create_data->nrf_management_uri) {
    if (cJSON_AddStringToObject(item, "nrfManagementUri", sm_context_create_data->nrf_management_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [nrf_management_uri]");
        goto end;
    }
    }

    if (sm_context_create_data->nrf_discovery_uri) {
    if (cJSON_AddStringToObject(item, "nrfDiscoveryUri", sm_context_create_data->nrf_discovery_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [nrf_discovery_uri]");
        goto end;
    }
    }

    if (sm_context_create_data->nrf_access_token_uri) {
    if (cJSON_AddStringToObject(item, "nrfAccessTokenUri", sm_context_create_data->nrf_access_token_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [nrf_access_token_uri]");
        goto end;
    }
    }

    if (sm_context_create_data->nrf_oauth2_required) {
    cJSON *nrf_oauth2_required = cJSON_AddObjectToObject(item, "nrfOauth2Required");
    if (nrf_oauth2_required == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [nrf_oauth2_required]");
        goto end;
    }
    cJSON *localMapObject = nrf_oauth2_required;
    if (sm_context_create_data->nrf_oauth2_required) {
        OpenAPI_list_for_each(sm_context_create_data->nrf_oauth2_required, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [nrf_oauth2_required]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [nrf_oauth2_required]");
                goto end;
            }
            if (cJSON_AddBoolToObject(localMapObject, localKeyValue->key, (uintptr_t)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

    if (sm_context_create_data->smf_binding_info) {
    if (cJSON_AddStringToObject(item, "smfBindingInfo", sm_context_create_data->smf_binding_info) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [smf_binding_info]");
        goto end;
    }
    }

    if (sm_context_create_data->pvs_info) {
    cJSON *pvs_infoList = cJSON_AddArrayToObject(item, "pvsInfo");
    if (pvs_infoList == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pvs_info]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_create_data->pvs_info, node) {
        cJSON *itemLocal = OpenAPI_server_addressing_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pvs_info]");
            goto end;
        }
        cJSON_AddItemToArray(pvs_infoList, itemLocal);
    }
    }

    if (sm_context_create_data->is_onboarding_ind) {
    if (cJSON_AddBoolToObject(item, "onboardingInd", sm_context_create_data->onboarding_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [onboarding_ind]");
        goto end;
    }
    }

    if (sm_context_create_data->old_pdu_session_ref) {
    if (cJSON_AddStringToObject(item, "oldPduSessionRef", sm_context_create_data->old_pdu_session_ref) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [old_pdu_session_ref]");
        goto end;
    }
    }

    if (sm_context_create_data->is_sm_policy_notify_ind) {
    if (cJSON_AddBoolToObject(item, "smPolicyNotifyInd", sm_context_create_data->sm_policy_notify_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [sm_policy_notify_ind]");
        goto end;
    }
    }

    if (sm_context_create_data->pcf_ue_callback_info) {
    cJSON *pcf_ue_callback_info_local_JSON = OpenAPI_pcf_ue_callback_info_convertToJSON(sm_context_create_data->pcf_ue_callback_info);
    if (pcf_ue_callback_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pcf_ue_callback_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pcfUeCallbackInfo", pcf_ue_callback_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pcf_ue_callback_info]");
        goto end;
    }
    } else if (sm_context_create_data->is_pcf_ue_callback_info_null) {
        if (cJSON_AddNullToObject(item, "pcfUeCallbackInfo") == NULL) {
            ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pcf_ue_callback_info]");
            goto end;
        }
    }

    if (sm_context_create_data->satellite_backhaul_cat != OpenAPI_satellite_backhaul_category_NULL) {
    if (cJSON_AddStringToObject(item, "satelliteBackhaulCat", OpenAPI_satellite_backhaul_category_ToString(sm_context_create_data->satellite_backhaul_cat)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [satellite_backhaul_cat]");
        goto end;
    }
    }

    if (sm_context_create_data->is_upip_supported) {
    if (cJSON_AddBoolToObject(item, "upipSupported", sm_context_create_data->upip_supported) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [upip_supported]");
        goto end;
    }
    }

    if (sm_context_create_data->is_uav_authenticated) {
    if (cJSON_AddBoolToObject(item, "uavAuthenticated", sm_context_create_data->uav_authenticated) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [uav_authenticated]");
        goto end;
    }
    }

    if (sm_context_create_data->is_disaster_roaming_ind) {
    if (cJSON_AddBoolToObject(item, "disasterRoamingInd", sm_context_create_data->disaster_roaming_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [disaster_roaming_ind]");
        goto end;
    }
    }

    if (sm_context_create_data->is_anchor_smf_oauth2_required) {
    if (cJSON_AddBoolToObject(item, "anchorSmfOauth2Required", sm_context_create_data->anchor_smf_oauth2_required) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [anchor_smf_oauth2_required]");
        goto end;
    }
    }

    if (sm_context_create_data->is_sm_context_smf_oauth2_required) {
    if (cJSON_AddBoolToObject(item, "smContextSmfOauth2Required", sm_context_create_data->sm_context_smf_oauth2_required) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [sm_context_smf_oauth2_required]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_context_create_data_t *OpenAPI_sm_context_create_data_parseFromJSON(cJSON *sm_context_create_dataJSON)
{
    OpenAPI_sm_context_create_data_t *sm_context_create_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *unauthenticated_supi = NULL;
    cJSON *pei = NULL;
    cJSON *gpsi = NULL;
    cJSON *pdu_session_id = NULL;
    cJSON *dnn = NULL;
    cJSON *selected_dnn = NULL;
    cJSON *s_nssai = NULL;
    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    cJSON *hplmn_snssai = NULL;
    OpenAPI_snssai_t *hplmn_snssai_local_nonprim = NULL;
    cJSON *serving_nf_id = NULL;
    cJSON *guami = NULL;
    OpenAPI_guami_t *guami_local_nonprim = NULL;
    cJSON *service_name = NULL;
    cJSON *serving_network = NULL;
    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    cJSON *request_type = NULL;
    OpenAPI_request_type_e request_typeVariable = 0;
    cJSON *n1_sm_msg = NULL;
    OpenAPI_ref_to_binary_data_t *n1_sm_msg_local_nonprim = NULL;
    cJSON *an_type = NULL;
    OpenAPI_access_type_e an_typeVariable = 0;
    cJSON *additional_an_type = NULL;
    OpenAPI_access_type_e additional_an_typeVariable = 0;
    cJSON *rat_type = NULL;
    OpenAPI_rat_type_e rat_typeVariable = 0;
    cJSON *presence_in_ladn = NULL;
    OpenAPI_presence_state_e presence_in_ladnVariable = 0;
    cJSON *ue_location = NULL;
    OpenAPI_user_location_t *ue_location_local_nonprim = NULL;
    cJSON *ue_time_zone = NULL;
    cJSON *add_ue_location = NULL;
    OpenAPI_user_location_t *add_ue_location_local_nonprim = NULL;
    cJSON *sm_context_status_uri = NULL;
    cJSON *h_smf_uri = NULL;
    cJSON *h_smf_id = NULL;
    cJSON *smf_uri = NULL;
    cJSON *smf_id = NULL;
    cJSON *additional_hsmf_uri = NULL;
    OpenAPI_list_t *additional_hsmf_uriList = NULL;
    cJSON *additional_hsmf_id = NULL;
    OpenAPI_list_t *additional_hsmf_idList = NULL;
    cJSON *additional_smf_uri = NULL;
    OpenAPI_list_t *additional_smf_uriList = NULL;
    cJSON *additional_smf_id = NULL;
    OpenAPI_list_t *additional_smf_idList = NULL;
    cJSON *old_pdu_session_id = NULL;
    cJSON *pdu_sessions_activate_list = NULL;
    OpenAPI_list_t *pdu_sessions_activate_listList = NULL;
    cJSON *ue_eps_pdn_connection = NULL;
    cJSON *ho_state = NULL;
    OpenAPI_ho_state_e ho_stateVariable = 0;
    cJSON *pcf_id = NULL;
    cJSON *pcf_group_id = NULL;
    cJSON *pcf_set_id = NULL;
    cJSON *nrf_uri = NULL;
    cJSON *supported_features = NULL;
    cJSON *sel_mode = NULL;
    OpenAPI_dnn_selection_mode_e sel_modeVariable = 0;
    cJSON *backup_amf_info = NULL;
    OpenAPI_list_t *backup_amf_infoList = NULL;
    cJSON *trace_data = NULL;
    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    cJSON *udm_group_id = NULL;
    cJSON *routing_indicator = NULL;
    cJSON *h_nw_pub_key_id = NULL;
    cJSON *eps_interworking_ind = NULL;
    OpenAPI_eps_interworking_indication_e eps_interworking_indVariable = 0;
    cJSON *indirect_forwarding_flag = NULL;
    cJSON *direct_forwarding_flag = NULL;
    cJSON *target_id = NULL;
    OpenAPI_ng_ran_target_id_t *target_id_local_nonprim = NULL;
    cJSON *eps_bearer_ctx_status = NULL;
    cJSON *cp_ciot_enabled = NULL;
    cJSON *cp_only_ind = NULL;
    cJSON *invoke_nef = NULL;
    cJSON *ma_request_ind = NULL;
    cJSON *ma_nw_upgrade_ind = NULL;
    cJSON *n2_sm_info = NULL;
    OpenAPI_ref_to_binary_data_t *n2_sm_info_local_nonprim = NULL;
    cJSON *n2_sm_info_type = NULL;
    OpenAPI_n2_sm_info_type_e n2_sm_info_typeVariable = 0;
    cJSON *n2_sm_info_ext1 = NULL;
    OpenAPI_ref_to_binary_data_t *n2_sm_info_ext1_local_nonprim = NULL;
    cJSON *n2_sm_info_type_ext1 = NULL;
    OpenAPI_n2_sm_info_type_e n2_sm_info_type_ext1Variable = 0;
    cJSON *sm_context_ref = NULL;
    cJSON *sm_context_smf_plmn_id = NULL;
    OpenAPI_plmn_id_nid_t *sm_context_smf_plmn_id_local_nonprim = NULL;
    cJSON *sm_context_smf_id = NULL;
    cJSON *sm_context_smf_set_id = NULL;
    cJSON *sm_context_smf_service_set_id = NULL;
    cJSON *sm_context_smf_binding = NULL;
    OpenAPI_sbi_binding_level_e sm_context_smf_bindingVariable = 0;
    cJSON *up_cnx_state = NULL;
    OpenAPI_up_cnx_state_e up_cnx_stateVariable = 0;
    cJSON *small_data_rate_status = NULL;
    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    cJSON *apn_rate_status = NULL;
    OpenAPI_apn_rate_status_t *apn_rate_status_local_nonprim = NULL;
    cJSON *extended_nas_sm_timer_ind = NULL;
    cJSON *dl_data_waiting_ind = NULL;
    cJSON *ddn_failure_subs = NULL;
    OpenAPI_ddn_failure_subs_t *ddn_failure_subs_local_nonprim = NULL;
    cJSON *smf_transfer_ind = NULL;
    cJSON *old_smf_id = NULL;
    cJSON *old_sm_context_ref = NULL;
    cJSON *w_agf_info = NULL;
    OpenAPI_w_agf_info_t *w_agf_info_local_nonprim = NULL;
    cJSON *tngf_info = NULL;
    OpenAPI_tngf_info_t *tngf_info_local_nonprim = NULL;
    cJSON *twif_info = NULL;
    OpenAPI_twif_info_t *twif_info_local_nonprim = NULL;
    cJSON *ran_unchanged_ind = NULL;
    cJSON *same_pcf_selection_ind = NULL;
    cJSON *target_dnai = NULL;
    cJSON *nrf_management_uri = NULL;
    cJSON *nrf_discovery_uri = NULL;
    cJSON *nrf_access_token_uri = NULL;
    cJSON *nrf_oauth2_required = NULL;
    OpenAPI_list_t *nrf_oauth2_requiredList = NULL;
    cJSON *smf_binding_info = NULL;
    cJSON *pvs_info = NULL;
    OpenAPI_list_t *pvs_infoList = NULL;
    cJSON *onboarding_ind = NULL;
    cJSON *old_pdu_session_ref = NULL;
    cJSON *sm_policy_notify_ind = NULL;
    cJSON *pcf_ue_callback_info = NULL;
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info_local_nonprim = NULL;
    cJSON *satellite_backhaul_cat = NULL;
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_catVariable = 0;
    cJSON *upip_supported = NULL;
    cJSON *uav_authenticated = NULL;
    cJSON *disaster_roaming_ind = NULL;
    cJSON *anchor_smf_oauth2_required = NULL;
    cJSON *sm_context_smf_oauth2_required = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    unauthenticated_supi = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "unauthenticatedSupi");
    if (unauthenticated_supi) {
    if (!cJSON_IsBool(unauthenticated_supi)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [unauthenticated_supi]");
        goto end;
    }
    }

    pei = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    pdu_session_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pduSessionId");
    if (pdu_session_id) {
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    selected_dnn = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "selectedDnn");
    if (selected_dnn) {
    if (!cJSON_IsString(selected_dnn) && !cJSON_IsNull(selected_dnn)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [selected_dnn]");
        goto end;
    }
    }

    s_nssai = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "sNssai");
    if (s_nssai) {
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    if (!s_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [s_nssai]");
        goto end;
    }
    }

    hplmn_snssai = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "hplmnSnssai");
    if (hplmn_snssai) {
    hplmn_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(hplmn_snssai);
    if (!hplmn_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [hplmn_snssai]");
        goto end;
    }
    }

    serving_nf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "servingNfId");
    if (!serving_nf_id) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [serving_nf_id]");
        goto end;
    }
    if (!cJSON_IsString(serving_nf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [serving_nf_id]");
        goto end;
    }

    guami = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "guami");
    if (guami) {
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    if (!guami_local_nonprim) {
        ogs_error("OpenAPI_guami_parseFromJSON failed [guami]");
        goto end;
    }
    }

    service_name = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "serviceName");
    if (service_name) {
    if (!cJSON_IsString(service_name) && !cJSON_IsNull(service_name)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [service_name]");
        goto end;
    }
    }

    serving_network = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "servingNetwork");
    if (!serving_network) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [serving_network]");
        goto end;
    }
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    if (!serving_network_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [serving_network]");
        goto end;
    }

    request_type = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "requestType");
    if (request_type) {
    if (!cJSON_IsString(request_type)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [request_type]");
        goto end;
    }
    request_typeVariable = OpenAPI_request_type_FromString(request_type->valuestring);
    }

    n1_sm_msg = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "n1SmMsg");
    if (n1_sm_msg) {
    n1_sm_msg_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_msg);
    if (!n1_sm_msg_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n1_sm_msg]");
        goto end;
    }
    }

    an_type = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "anType");
    if (!an_type) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [an_type]");
        goto end;
    }
    if (!cJSON_IsString(an_type)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [an_type]");
        goto end;
    }
    an_typeVariable = OpenAPI_access_type_FromString(an_type->valuestring);

    additional_an_type = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "additionalAnType");
    if (additional_an_type) {
    if (!cJSON_IsString(additional_an_type)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_an_type]");
        goto end;
    }
    additional_an_typeVariable = OpenAPI_access_type_FromString(additional_an_type->valuestring);
    }

    rat_type = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "ratType");
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    presence_in_ladn = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "presenceInLadn");
    if (presence_in_ladn) {
    if (!cJSON_IsString(presence_in_ladn)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [presence_in_ladn]");
        goto end;
    }
    presence_in_ladnVariable = OpenAPI_presence_state_FromString(presence_in_ladn->valuestring);
    }

    ue_location = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "ueLocation");
    if (ue_location) {
    ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    if (!ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_location]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    add_ue_location = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "addUeLocation");
    if (add_ue_location) {
    add_ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(add_ue_location);
    if (!add_ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [add_ue_location]");
        goto end;
    }
    }

    sm_context_status_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextStatusUri");
    if (!sm_context_status_uri) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_status_uri]");
        goto end;
    }
    if (!cJSON_IsString(sm_context_status_uri)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_status_uri]");
        goto end;
    }

    h_smf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "hSmfUri");
    if (h_smf_uri) {
    if (!cJSON_IsString(h_smf_uri) && !cJSON_IsNull(h_smf_uri)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [h_smf_uri]");
        goto end;
    }
    }

    h_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "hSmfId");
    if (h_smf_id) {
    if (!cJSON_IsString(h_smf_id) && !cJSON_IsNull(h_smf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [h_smf_id]");
        goto end;
    }
    }

    smf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smfUri");
    if (smf_uri) {
    if (!cJSON_IsString(smf_uri) && !cJSON_IsNull(smf_uri)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [smf_uri]");
        goto end;
    }
    }

    smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smfId");
    if (smf_id) {
    if (!cJSON_IsString(smf_id) && !cJSON_IsNull(smf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [smf_id]");
        goto end;
    }
    }

    additional_hsmf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "additionalHsmfUri");
    if (additional_hsmf_uri) {
        cJSON *additional_hsmf_uri_local = NULL;
        if (!cJSON_IsArray(additional_hsmf_uri)) {
            ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_hsmf_uri]");
            goto end;
        }

        additional_hsmf_uriList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_hsmf_uri_local, additional_hsmf_uri) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(additional_hsmf_uri_local)) {
                ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_hsmf_uri]");
                goto end;
            }
            OpenAPI_list_add(additional_hsmf_uriList, ogs_strdup(additional_hsmf_uri_local->valuestring));
        }
    }

    additional_hsmf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "additionalHsmfId");
    if (additional_hsmf_id) {
        cJSON *additional_hsmf_id_local = NULL;
        if (!cJSON_IsArray(additional_hsmf_id)) {
            ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_hsmf_id]");
            goto end;
        }

        additional_hsmf_idList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_hsmf_id_local, additional_hsmf_id) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(additional_hsmf_id_local)) {
                ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_hsmf_id]");
                goto end;
            }
            OpenAPI_list_add(additional_hsmf_idList, ogs_strdup(additional_hsmf_id_local->valuestring));
        }
    }

    additional_smf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "additionalSmfUri");
    if (additional_smf_uri) {
        cJSON *additional_smf_uri_local = NULL;
        if (!cJSON_IsArray(additional_smf_uri)) {
            ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_smf_uri]");
            goto end;
        }

        additional_smf_uriList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_smf_uri_local, additional_smf_uri) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(additional_smf_uri_local)) {
                ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_smf_uri]");
                goto end;
            }
            OpenAPI_list_add(additional_smf_uriList, ogs_strdup(additional_smf_uri_local->valuestring));
        }
    }

    additional_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "additionalSmfId");
    if (additional_smf_id) {
        cJSON *additional_smf_id_local = NULL;
        if (!cJSON_IsArray(additional_smf_id)) {
            ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_smf_id]");
            goto end;
        }

        additional_smf_idList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_smf_id_local, additional_smf_id) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(additional_smf_id_local)) {
                ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_smf_id]");
                goto end;
            }
            OpenAPI_list_add(additional_smf_idList, ogs_strdup(additional_smf_id_local->valuestring));
        }
    }

    old_pdu_session_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "oldPduSessionId");
    if (old_pdu_session_id) {
    if (!cJSON_IsNumber(old_pdu_session_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [old_pdu_session_id]");
        goto end;
    }
    }

    pdu_sessions_activate_list = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pduSessionsActivateList");
    if (pdu_sessions_activate_list) {
        cJSON *pdu_sessions_activate_list_local = NULL;
        if (!cJSON_IsArray(pdu_sessions_activate_list)) {
            ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pdu_sessions_activate_list]");
            goto end;
        }

        pdu_sessions_activate_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_sessions_activate_list_local, pdu_sessions_activate_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(pdu_sessions_activate_list_local)) {
                ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pdu_sessions_activate_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pdu_sessions_activate_list]");
                goto end;
            }
            *localDouble = pdu_sessions_activate_list_local->valuedouble;
            OpenAPI_list_add(pdu_sessions_activate_listList, localDouble);
        }
    }

    ue_eps_pdn_connection = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "ueEpsPdnConnection");
    if (ue_eps_pdn_connection) {
    if (!cJSON_IsString(ue_eps_pdn_connection) && !cJSON_IsNull(ue_eps_pdn_connection)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [ue_eps_pdn_connection]");
        goto end;
    }
    }

    ho_state = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "hoState");
    if (ho_state) {
    if (!cJSON_IsString(ho_state)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [ho_state]");
        goto end;
    }
    ho_stateVariable = OpenAPI_ho_state_FromString(ho_state->valuestring);
    }

    pcf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_group_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pcfGroupId");
    if (pcf_group_id) {
    if (!cJSON_IsString(pcf_group_id) && !cJSON_IsNull(pcf_group_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pcf_group_id]");
        goto end;
    }
    }

    pcf_set_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pcfSetId");
    if (pcf_set_id) {
    if (!cJSON_IsString(pcf_set_id) && !cJSON_IsNull(pcf_set_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    nrf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "nrfUri");
    if (nrf_uri) {
    if (!cJSON_IsString(nrf_uri) && !cJSON_IsNull(nrf_uri)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [nrf_uri]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    sel_mode = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "selMode");
    if (sel_mode) {
    if (!cJSON_IsString(sel_mode)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sel_mode]");
        goto end;
    }
    sel_modeVariable = OpenAPI_dnn_selection_mode_FromString(sel_mode->valuestring);
    }

    backup_amf_info = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "backupAmfInfo");
    if (backup_amf_info) {
        cJSON *backup_amf_info_local = NULL;
        if (!cJSON_IsArray(backup_amf_info)) {
            ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [backup_amf_info]");
            goto end;
        }

        backup_amf_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(backup_amf_info_local, backup_amf_info) {
            if (!cJSON_IsObject(backup_amf_info_local)) {
                ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [backup_amf_info]");
                goto end;
            }
            OpenAPI_backup_amf_info_t *backup_amf_infoItem = OpenAPI_backup_amf_info_parseFromJSON(backup_amf_info_local);
            if (!backup_amf_infoItem) {
                ogs_error("No backup_amf_infoItem");
                goto end;
            }
            OpenAPI_list_add(backup_amf_infoList, backup_amf_infoItem);
        }
    }

    trace_data = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "traceData");
    if (trace_data) {
    if (!cJSON_IsNull(trace_data)) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    if (!trace_data_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_data]");
        goto end;
    }
    }
    }

    udm_group_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "udmGroupId");
    if (udm_group_id) {
    if (!cJSON_IsString(udm_group_id) && !cJSON_IsNull(udm_group_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [udm_group_id]");
        goto end;
    }
    }

    routing_indicator = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "routingIndicator");
    if (routing_indicator) {
    if (!cJSON_IsString(routing_indicator) && !cJSON_IsNull(routing_indicator)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [routing_indicator]");
        goto end;
    }
    }

    h_nw_pub_key_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "hNwPubKeyId");
    if (h_nw_pub_key_id) {
    if (!cJSON_IsNumber(h_nw_pub_key_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [h_nw_pub_key_id]");
        goto end;
    }
    }

    eps_interworking_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "epsInterworkingInd");
    if (eps_interworking_ind) {
    if (!cJSON_IsString(eps_interworking_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [eps_interworking_ind]");
        goto end;
    }
    eps_interworking_indVariable = OpenAPI_eps_interworking_indication_FromString(eps_interworking_ind->valuestring);
    }

    indirect_forwarding_flag = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "indirectForwardingFlag");
    if (indirect_forwarding_flag) {
    if (!cJSON_IsBool(indirect_forwarding_flag)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [indirect_forwarding_flag]");
        goto end;
    }
    }

    direct_forwarding_flag = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "directForwardingFlag");
    if (direct_forwarding_flag) {
    if (!cJSON_IsBool(direct_forwarding_flag)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [direct_forwarding_flag]");
        goto end;
    }
    }

    target_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "targetId");
    if (target_id) {
    target_id_local_nonprim = OpenAPI_ng_ran_target_id_parseFromJSON(target_id);
    if (!target_id_local_nonprim) {
        ogs_error("OpenAPI_ng_ran_target_id_parseFromJSON failed [target_id]");
        goto end;
    }
    }

    eps_bearer_ctx_status = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "epsBearerCtxStatus");
    if (eps_bearer_ctx_status) {
    if (!cJSON_IsString(eps_bearer_ctx_status) && !cJSON_IsNull(eps_bearer_ctx_status)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [eps_bearer_ctx_status]");
        goto end;
    }
    }

    cp_ciot_enabled = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "cpCiotEnabled");
    if (cp_ciot_enabled) {
    if (!cJSON_IsBool(cp_ciot_enabled)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [cp_ciot_enabled]");
        goto end;
    }
    }

    cp_only_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "cpOnlyInd");
    if (cp_only_ind) {
    if (!cJSON_IsBool(cp_only_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [cp_only_ind]");
        goto end;
    }
    }

    invoke_nef = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "invokeNef");
    if (invoke_nef) {
    if (!cJSON_IsBool(invoke_nef)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [invoke_nef]");
        goto end;
    }
    }

    ma_request_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "maRequestInd");
    if (ma_request_ind) {
    if (!cJSON_IsBool(ma_request_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [ma_request_ind]");
        goto end;
    }
    }

    ma_nw_upgrade_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "maNwUpgradeInd");
    if (ma_nw_upgrade_ind) {
    if (!cJSON_IsBool(ma_nw_upgrade_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [ma_nw_upgrade_ind]");
        goto end;
    }
    }

    n2_sm_info = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "n2SmInfo");
    if (n2_sm_info) {
    n2_sm_info_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n2_sm_info);
    if (!n2_sm_info_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n2_sm_info]");
        goto end;
    }
    }

    n2_sm_info_type = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "n2SmInfoType");
    if (n2_sm_info_type) {
    if (!cJSON_IsString(n2_sm_info_type)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [n2_sm_info_type]");
        goto end;
    }
    n2_sm_info_typeVariable = OpenAPI_n2_sm_info_type_FromString(n2_sm_info_type->valuestring);
    }

    n2_sm_info_ext1 = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "n2SmInfoExt1");
    if (n2_sm_info_ext1) {
    n2_sm_info_ext1_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n2_sm_info_ext1);
    if (!n2_sm_info_ext1_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n2_sm_info_ext1]");
        goto end;
    }
    }

    n2_sm_info_type_ext1 = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "n2SmInfoTypeExt1");
    if (n2_sm_info_type_ext1) {
    if (!cJSON_IsString(n2_sm_info_type_ext1)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [n2_sm_info_type_ext1]");
        goto end;
    }
    n2_sm_info_type_ext1Variable = OpenAPI_n2_sm_info_type_FromString(n2_sm_info_type_ext1->valuestring);
    }

    sm_context_ref = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextRef");
    if (sm_context_ref) {
    if (!cJSON_IsString(sm_context_ref) && !cJSON_IsNull(sm_context_ref)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_ref]");
        goto end;
    }
    }

    sm_context_smf_plmn_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextSmfPlmnId");
    if (sm_context_smf_plmn_id) {
    sm_context_smf_plmn_id_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(sm_context_smf_plmn_id);
    if (!sm_context_smf_plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [sm_context_smf_plmn_id]");
        goto end;
    }
    }

    sm_context_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextSmfId");
    if (sm_context_smf_id) {
    if (!cJSON_IsString(sm_context_smf_id) && !cJSON_IsNull(sm_context_smf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_smf_id]");
        goto end;
    }
    }

    sm_context_smf_set_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextSmfSetId");
    if (sm_context_smf_set_id) {
    if (!cJSON_IsString(sm_context_smf_set_id) && !cJSON_IsNull(sm_context_smf_set_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_smf_set_id]");
        goto end;
    }
    }

    sm_context_smf_service_set_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextSmfServiceSetId");
    if (sm_context_smf_service_set_id) {
    if (!cJSON_IsString(sm_context_smf_service_set_id) && !cJSON_IsNull(sm_context_smf_service_set_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_smf_service_set_id]");
        goto end;
    }
    }

    sm_context_smf_binding = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextSmfBinding");
    if (sm_context_smf_binding) {
    if (!cJSON_IsString(sm_context_smf_binding)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_smf_binding]");
        goto end;
    }
    sm_context_smf_bindingVariable = OpenAPI_sbi_binding_level_FromString(sm_context_smf_binding->valuestring);
    }

    up_cnx_state = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "upCnxState");
    if (up_cnx_state) {
    if (!cJSON_IsString(up_cnx_state)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [up_cnx_state]");
        goto end;
    }
    up_cnx_stateVariable = OpenAPI_up_cnx_state_FromString(up_cnx_state->valuestring);
    }

    small_data_rate_status = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smallDataRateStatus");
    if (small_data_rate_status) {
    small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    if (!small_data_rate_status_local_nonprim) {
        ogs_error("OpenAPI_small_data_rate_status_parseFromJSON failed [small_data_rate_status]");
        goto end;
    }
    }

    apn_rate_status = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "apnRateStatus");
    if (apn_rate_status) {
    apn_rate_status_local_nonprim = OpenAPI_apn_rate_status_parseFromJSON(apn_rate_status);
    if (!apn_rate_status_local_nonprim) {
        ogs_error("OpenAPI_apn_rate_status_parseFromJSON failed [apn_rate_status]");
        goto end;
    }
    }

    extended_nas_sm_timer_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "extendedNasSmTimerInd");
    if (extended_nas_sm_timer_ind) {
    if (!cJSON_IsBool(extended_nas_sm_timer_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [extended_nas_sm_timer_ind]");
        goto end;
    }
    }

    dl_data_waiting_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "dlDataWaitingInd");
    if (dl_data_waiting_ind) {
    if (!cJSON_IsBool(dl_data_waiting_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [dl_data_waiting_ind]");
        goto end;
    }
    }

    ddn_failure_subs = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "ddnFailureSubs");
    if (ddn_failure_subs) {
    ddn_failure_subs_local_nonprim = OpenAPI_ddn_failure_subs_parseFromJSON(ddn_failure_subs);
    if (!ddn_failure_subs_local_nonprim) {
        ogs_error("OpenAPI_ddn_failure_subs_parseFromJSON failed [ddn_failure_subs]");
        goto end;
    }
    }

    smf_transfer_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smfTransferInd");
    if (smf_transfer_ind) {
    if (!cJSON_IsBool(smf_transfer_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [smf_transfer_ind]");
        goto end;
    }
    }

    old_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "oldSmfId");
    if (old_smf_id) {
    if (!cJSON_IsString(old_smf_id) && !cJSON_IsNull(old_smf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [old_smf_id]");
        goto end;
    }
    }

    old_sm_context_ref = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "oldSmContextRef");
    if (old_sm_context_ref) {
    if (!cJSON_IsString(old_sm_context_ref) && !cJSON_IsNull(old_sm_context_ref)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [old_sm_context_ref]");
        goto end;
    }
    }

    w_agf_info = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "wAgfInfo");
    if (w_agf_info) {
    w_agf_info_local_nonprim = OpenAPI_w_agf_info_parseFromJSON(w_agf_info);
    if (!w_agf_info_local_nonprim) {
        ogs_error("OpenAPI_w_agf_info_parseFromJSON failed [w_agf_info]");
        goto end;
    }
    }

    tngf_info = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "tngfInfo");
    if (tngf_info) {
    tngf_info_local_nonprim = OpenAPI_tngf_info_parseFromJSON(tngf_info);
    if (!tngf_info_local_nonprim) {
        ogs_error("OpenAPI_tngf_info_parseFromJSON failed [tngf_info]");
        goto end;
    }
    }

    twif_info = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "twifInfo");
    if (twif_info) {
    twif_info_local_nonprim = OpenAPI_twif_info_parseFromJSON(twif_info);
    if (!twif_info_local_nonprim) {
        ogs_error("OpenAPI_twif_info_parseFromJSON failed [twif_info]");
        goto end;
    }
    }

    ran_unchanged_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "ranUnchangedInd");
    if (ran_unchanged_ind) {
    if (!cJSON_IsBool(ran_unchanged_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [ran_unchanged_ind]");
        goto end;
    }
    }

    same_pcf_selection_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "samePcfSelectionInd");
    if (same_pcf_selection_ind) {
    if (!cJSON_IsBool(same_pcf_selection_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [same_pcf_selection_ind]");
        goto end;
    }
    }

    target_dnai = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "targetDnai");
    if (target_dnai) {
    if (!cJSON_IsString(target_dnai) && !cJSON_IsNull(target_dnai)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [target_dnai]");
        goto end;
    }
    }

    nrf_management_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "nrfManagementUri");
    if (nrf_management_uri) {
    if (!cJSON_IsString(nrf_management_uri) && !cJSON_IsNull(nrf_management_uri)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [nrf_management_uri]");
        goto end;
    }
    }

    nrf_discovery_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "nrfDiscoveryUri");
    if (nrf_discovery_uri) {
    if (!cJSON_IsString(nrf_discovery_uri) && !cJSON_IsNull(nrf_discovery_uri)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [nrf_discovery_uri]");
        goto end;
    }
    }

    nrf_access_token_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "nrfAccessTokenUri");
    if (nrf_access_token_uri) {
    if (!cJSON_IsString(nrf_access_token_uri) && !cJSON_IsNull(nrf_access_token_uri)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [nrf_access_token_uri]");
        goto end;
    }
    }

    nrf_oauth2_required = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "nrfOauth2Required");
    if (nrf_oauth2_required) {
        cJSON *nrf_oauth2_required_local_map = NULL;
        if (!cJSON_IsObject(nrf_oauth2_required) && !cJSON_IsNull(nrf_oauth2_required)) {
            ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [nrf_oauth2_required]");
            goto end;
        }
        if (cJSON_IsObject(nrf_oauth2_required)) {
            nrf_oauth2_requiredList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(nrf_oauth2_required_local_map, nrf_oauth2_required) {
                cJSON *localMapObject = nrf_oauth2_required_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsBool(localMapObject)) {
                    ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                localInt = (int *)ogs_calloc(1, sizeof(int));
                if (!localInt) {
                    ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                *localInt = localMapObject->valueint;
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), localInt);
                OpenAPI_list_add(nrf_oauth2_requiredList, localMapKeyPair);
            }
        }
    }

    smf_binding_info = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smfBindingInfo");
    if (smf_binding_info) {
    if (!cJSON_IsString(smf_binding_info) && !cJSON_IsNull(smf_binding_info)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [smf_binding_info]");
        goto end;
    }
    }

    pvs_info = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pvsInfo");
    if (pvs_info) {
        cJSON *pvs_info_local = NULL;
        if (!cJSON_IsArray(pvs_info)) {
            ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pvs_info]");
            goto end;
        }

        pvs_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(pvs_info_local, pvs_info) {
            if (!cJSON_IsObject(pvs_info_local)) {
                ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pvs_info]");
                goto end;
            }
            OpenAPI_server_addressing_info_t *pvs_infoItem = OpenAPI_server_addressing_info_parseFromJSON(pvs_info_local);
            if (!pvs_infoItem) {
                ogs_error("No pvs_infoItem");
                goto end;
            }
            OpenAPI_list_add(pvs_infoList, pvs_infoItem);
        }
    }

    onboarding_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "onboardingInd");
    if (onboarding_ind) {
    if (!cJSON_IsBool(onboarding_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [onboarding_ind]");
        goto end;
    }
    }

    old_pdu_session_ref = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "oldPduSessionRef");
    if (old_pdu_session_ref) {
    if (!cJSON_IsString(old_pdu_session_ref) && !cJSON_IsNull(old_pdu_session_ref)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [old_pdu_session_ref]");
        goto end;
    }
    }

    sm_policy_notify_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smPolicyNotifyInd");
    if (sm_policy_notify_ind) {
    if (!cJSON_IsBool(sm_policy_notify_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_policy_notify_ind]");
        goto end;
    }
    }

    pcf_ue_callback_info = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pcfUeCallbackInfo");
    if (pcf_ue_callback_info) {
    if (!cJSON_IsNull(pcf_ue_callback_info)) {
    pcf_ue_callback_info_local_nonprim = OpenAPI_pcf_ue_callback_info_parseFromJSON(pcf_ue_callback_info);
    if (!pcf_ue_callback_info_local_nonprim) {
        ogs_error("OpenAPI_pcf_ue_callback_info_parseFromJSON failed [pcf_ue_callback_info]");
        goto end;
    }
    }
    }

    satellite_backhaul_cat = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "satelliteBackhaulCat");
    if (satellite_backhaul_cat) {
    if (!cJSON_IsString(satellite_backhaul_cat)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [satellite_backhaul_cat]");
        goto end;
    }
    satellite_backhaul_catVariable = OpenAPI_satellite_backhaul_category_FromString(satellite_backhaul_cat->valuestring);
    }

    upip_supported = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "upipSupported");
    if (upip_supported) {
    if (!cJSON_IsBool(upip_supported)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [upip_supported]");
        goto end;
    }
    }

    uav_authenticated = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "uavAuthenticated");
    if (uav_authenticated) {
    if (!cJSON_IsBool(uav_authenticated)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [uav_authenticated]");
        goto end;
    }
    }

    disaster_roaming_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "disasterRoamingInd");
    if (disaster_roaming_ind) {
    if (!cJSON_IsBool(disaster_roaming_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [disaster_roaming_ind]");
        goto end;
    }
    }

    anchor_smf_oauth2_required = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "anchorSmfOauth2Required");
    if (anchor_smf_oauth2_required) {
    if (!cJSON_IsBool(anchor_smf_oauth2_required)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [anchor_smf_oauth2_required]");
        goto end;
    }
    }

    sm_context_smf_oauth2_required = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextSmfOauth2Required");
    if (sm_context_smf_oauth2_required) {
    if (!cJSON_IsBool(sm_context_smf_oauth2_required)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_smf_oauth2_required]");
        goto end;
    }
    }

    sm_context_create_data_local_var = OpenAPI_sm_context_create_data_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        unauthenticated_supi ? true : false,
        unauthenticated_supi ? unauthenticated_supi->valueint : 0,
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        pdu_session_id ? true : false,
        pdu_session_id ? pdu_session_id->valuedouble : 0,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        selected_dnn && !cJSON_IsNull(selected_dnn) ? ogs_strdup(selected_dnn->valuestring) : NULL,
        s_nssai ? s_nssai_local_nonprim : NULL,
        hplmn_snssai ? hplmn_snssai_local_nonprim : NULL,
        ogs_strdup(serving_nf_id->valuestring),
        guami ? guami_local_nonprim : NULL,
        service_name && !cJSON_IsNull(service_name) ? ogs_strdup(service_name->valuestring) : NULL,
        serving_network_local_nonprim,
        request_type ? request_typeVariable : 0,
        n1_sm_msg ? n1_sm_msg_local_nonprim : NULL,
        an_typeVariable,
        additional_an_type ? additional_an_typeVariable : 0,
        rat_type ? rat_typeVariable : 0,
        presence_in_ladn ? presence_in_ladnVariable : 0,
        ue_location ? ue_location_local_nonprim : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        add_ue_location ? add_ue_location_local_nonprim : NULL,
        ogs_strdup(sm_context_status_uri->valuestring),
        h_smf_uri && !cJSON_IsNull(h_smf_uri) ? ogs_strdup(h_smf_uri->valuestring) : NULL,
        h_smf_id && !cJSON_IsNull(h_smf_id) ? ogs_strdup(h_smf_id->valuestring) : NULL,
        smf_uri && !cJSON_IsNull(smf_uri) ? ogs_strdup(smf_uri->valuestring) : NULL,
        smf_id && !cJSON_IsNull(smf_id) ? ogs_strdup(smf_id->valuestring) : NULL,
        additional_hsmf_uri ? additional_hsmf_uriList : NULL,
        additional_hsmf_id ? additional_hsmf_idList : NULL,
        additional_smf_uri ? additional_smf_uriList : NULL,
        additional_smf_id ? additional_smf_idList : NULL,
        old_pdu_session_id ? true : false,
        old_pdu_session_id ? old_pdu_session_id->valuedouble : 0,
        pdu_sessions_activate_list ? pdu_sessions_activate_listList : NULL,
        ue_eps_pdn_connection && !cJSON_IsNull(ue_eps_pdn_connection) ? ogs_strdup(ue_eps_pdn_connection->valuestring) : NULL,
        ho_state ? ho_stateVariable : 0,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_group_id && !cJSON_IsNull(pcf_group_id) ? ogs_strdup(pcf_group_id->valuestring) : NULL,
        pcf_set_id && !cJSON_IsNull(pcf_set_id) ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        nrf_uri && !cJSON_IsNull(nrf_uri) ? ogs_strdup(nrf_uri->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        sel_mode ? sel_modeVariable : 0,
        backup_amf_info ? backup_amf_infoList : NULL,
        trace_data && cJSON_IsNull(trace_data) ? true : false,
        trace_data ? trace_data_local_nonprim : NULL,
        udm_group_id && !cJSON_IsNull(udm_group_id) ? ogs_strdup(udm_group_id->valuestring) : NULL,
        routing_indicator && !cJSON_IsNull(routing_indicator) ? ogs_strdup(routing_indicator->valuestring) : NULL,
        h_nw_pub_key_id ? true : false,
        h_nw_pub_key_id ? h_nw_pub_key_id->valuedouble : 0,
        eps_interworking_ind ? eps_interworking_indVariable : 0,
        indirect_forwarding_flag ? true : false,
        indirect_forwarding_flag ? indirect_forwarding_flag->valueint : 0,
        direct_forwarding_flag ? true : false,
        direct_forwarding_flag ? direct_forwarding_flag->valueint : 0,
        target_id ? target_id_local_nonprim : NULL,
        eps_bearer_ctx_status && !cJSON_IsNull(eps_bearer_ctx_status) ? ogs_strdup(eps_bearer_ctx_status->valuestring) : NULL,
        cp_ciot_enabled ? true : false,
        cp_ciot_enabled ? cp_ciot_enabled->valueint : 0,
        cp_only_ind ? true : false,
        cp_only_ind ? cp_only_ind->valueint : 0,
        invoke_nef ? true : false,
        invoke_nef ? invoke_nef->valueint : 0,
        ma_request_ind ? true : false,
        ma_request_ind ? ma_request_ind->valueint : 0,
        ma_nw_upgrade_ind ? true : false,
        ma_nw_upgrade_ind ? ma_nw_upgrade_ind->valueint : 0,
        n2_sm_info ? n2_sm_info_local_nonprim : NULL,
        n2_sm_info_type ? n2_sm_info_typeVariable : 0,
        n2_sm_info_ext1 ? n2_sm_info_ext1_local_nonprim : NULL,
        n2_sm_info_type_ext1 ? n2_sm_info_type_ext1Variable : 0,
        sm_context_ref && !cJSON_IsNull(sm_context_ref) ? ogs_strdup(sm_context_ref->valuestring) : NULL,
        sm_context_smf_plmn_id ? sm_context_smf_plmn_id_local_nonprim : NULL,
        sm_context_smf_id && !cJSON_IsNull(sm_context_smf_id) ? ogs_strdup(sm_context_smf_id->valuestring) : NULL,
        sm_context_smf_set_id && !cJSON_IsNull(sm_context_smf_set_id) ? ogs_strdup(sm_context_smf_set_id->valuestring) : NULL,
        sm_context_smf_service_set_id && !cJSON_IsNull(sm_context_smf_service_set_id) ? ogs_strdup(sm_context_smf_service_set_id->valuestring) : NULL,
        sm_context_smf_binding ? sm_context_smf_bindingVariable : 0,
        up_cnx_state ? up_cnx_stateVariable : 0,
        small_data_rate_status ? small_data_rate_status_local_nonprim : NULL,
        apn_rate_status ? apn_rate_status_local_nonprim : NULL,
        extended_nas_sm_timer_ind ? true : false,
        extended_nas_sm_timer_ind ? extended_nas_sm_timer_ind->valueint : 0,
        dl_data_waiting_ind ? true : false,
        dl_data_waiting_ind ? dl_data_waiting_ind->valueint : 0,
        ddn_failure_subs ? ddn_failure_subs_local_nonprim : NULL,
        smf_transfer_ind ? true : false,
        smf_transfer_ind ? smf_transfer_ind->valueint : 0,
        old_smf_id && !cJSON_IsNull(old_smf_id) ? ogs_strdup(old_smf_id->valuestring) : NULL,
        old_sm_context_ref && !cJSON_IsNull(old_sm_context_ref) ? ogs_strdup(old_sm_context_ref->valuestring) : NULL,
        w_agf_info ? w_agf_info_local_nonprim : NULL,
        tngf_info ? tngf_info_local_nonprim : NULL,
        twif_info ? twif_info_local_nonprim : NULL,
        ran_unchanged_ind ? true : false,
        ran_unchanged_ind ? ran_unchanged_ind->valueint : 0,
        same_pcf_selection_ind ? true : false,
        same_pcf_selection_ind ? same_pcf_selection_ind->valueint : 0,
        target_dnai && !cJSON_IsNull(target_dnai) ? ogs_strdup(target_dnai->valuestring) : NULL,
        nrf_management_uri && !cJSON_IsNull(nrf_management_uri) ? ogs_strdup(nrf_management_uri->valuestring) : NULL,
        nrf_discovery_uri && !cJSON_IsNull(nrf_discovery_uri) ? ogs_strdup(nrf_discovery_uri->valuestring) : NULL,
        nrf_access_token_uri && !cJSON_IsNull(nrf_access_token_uri) ? ogs_strdup(nrf_access_token_uri->valuestring) : NULL,
        nrf_oauth2_required ? nrf_oauth2_requiredList : NULL,
        smf_binding_info && !cJSON_IsNull(smf_binding_info) ? ogs_strdup(smf_binding_info->valuestring) : NULL,
        pvs_info ? pvs_infoList : NULL,
        onboarding_ind ? true : false,
        onboarding_ind ? onboarding_ind->valueint : 0,
        old_pdu_session_ref && !cJSON_IsNull(old_pdu_session_ref) ? ogs_strdup(old_pdu_session_ref->valuestring) : NULL,
        sm_policy_notify_ind ? true : false,
        sm_policy_notify_ind ? sm_policy_notify_ind->valueint : 0,
        pcf_ue_callback_info && cJSON_IsNull(pcf_ue_callback_info) ? true : false,
        pcf_ue_callback_info ? pcf_ue_callback_info_local_nonprim : NULL,
        satellite_backhaul_cat ? satellite_backhaul_catVariable : 0,
        upip_supported ? true : false,
        upip_supported ? upip_supported->valueint : 0,
        uav_authenticated ? true : false,
        uav_authenticated ? uav_authenticated->valueint : 0,
        disaster_roaming_ind ? true : false,
        disaster_roaming_ind ? disaster_roaming_ind->valueint : 0,
        anchor_smf_oauth2_required ? true : false,
        anchor_smf_oauth2_required ? anchor_smf_oauth2_required->valueint : 0,
        sm_context_smf_oauth2_required ? true : false,
        sm_context_smf_oauth2_required ? sm_context_smf_oauth2_required->valueint : 0
    );

    return sm_context_create_data_local_var;
end:
    if (s_nssai_local_nonprim) {
        OpenAPI_snssai_free(s_nssai_local_nonprim);
        s_nssai_local_nonprim = NULL;
    }
    if (hplmn_snssai_local_nonprim) {
        OpenAPI_snssai_free(hplmn_snssai_local_nonprim);
        hplmn_snssai_local_nonprim = NULL;
    }
    if (guami_local_nonprim) {
        OpenAPI_guami_free(guami_local_nonprim);
        guami_local_nonprim = NULL;
    }
    if (serving_network_local_nonprim) {
        OpenAPI_plmn_id_nid_free(serving_network_local_nonprim);
        serving_network_local_nonprim = NULL;
    }
    if (n1_sm_msg_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(n1_sm_msg_local_nonprim);
        n1_sm_msg_local_nonprim = NULL;
    }
    if (ue_location_local_nonprim) {
        OpenAPI_user_location_free(ue_location_local_nonprim);
        ue_location_local_nonprim = NULL;
    }
    if (add_ue_location_local_nonprim) {
        OpenAPI_user_location_free(add_ue_location_local_nonprim);
        add_ue_location_local_nonprim = NULL;
    }
    if (additional_hsmf_uriList) {
        OpenAPI_list_for_each(additional_hsmf_uriList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(additional_hsmf_uriList);
        additional_hsmf_uriList = NULL;
    }
    if (additional_hsmf_idList) {
        OpenAPI_list_for_each(additional_hsmf_idList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(additional_hsmf_idList);
        additional_hsmf_idList = NULL;
    }
    if (additional_smf_uriList) {
        OpenAPI_list_for_each(additional_smf_uriList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(additional_smf_uriList);
        additional_smf_uriList = NULL;
    }
    if (additional_smf_idList) {
        OpenAPI_list_for_each(additional_smf_idList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(additional_smf_idList);
        additional_smf_idList = NULL;
    }
    if (pdu_sessions_activate_listList) {
        OpenAPI_list_for_each(pdu_sessions_activate_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pdu_sessions_activate_listList);
        pdu_sessions_activate_listList = NULL;
    }
    if (backup_amf_infoList) {
        OpenAPI_list_for_each(backup_amf_infoList, node) {
            OpenAPI_backup_amf_info_free(node->data);
        }
        OpenAPI_list_free(backup_amf_infoList);
        backup_amf_infoList = NULL;
    }
    if (trace_data_local_nonprim) {
        OpenAPI_trace_data_free(trace_data_local_nonprim);
        trace_data_local_nonprim = NULL;
    }
    if (target_id_local_nonprim) {
        OpenAPI_ng_ran_target_id_free(target_id_local_nonprim);
        target_id_local_nonprim = NULL;
    }
    if (n2_sm_info_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(n2_sm_info_local_nonprim);
        n2_sm_info_local_nonprim = NULL;
    }
    if (n2_sm_info_ext1_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(n2_sm_info_ext1_local_nonprim);
        n2_sm_info_ext1_local_nonprim = NULL;
    }
    if (sm_context_smf_plmn_id_local_nonprim) {
        OpenAPI_plmn_id_nid_free(sm_context_smf_plmn_id_local_nonprim);
        sm_context_smf_plmn_id_local_nonprim = NULL;
    }
    if (small_data_rate_status_local_nonprim) {
        OpenAPI_small_data_rate_status_free(small_data_rate_status_local_nonprim);
        small_data_rate_status_local_nonprim = NULL;
    }
    if (apn_rate_status_local_nonprim) {
        OpenAPI_apn_rate_status_free(apn_rate_status_local_nonprim);
        apn_rate_status_local_nonprim = NULL;
    }
    if (ddn_failure_subs_local_nonprim) {
        OpenAPI_ddn_failure_subs_free(ddn_failure_subs_local_nonprim);
        ddn_failure_subs_local_nonprim = NULL;
    }
    if (w_agf_info_local_nonprim) {
        OpenAPI_w_agf_info_free(w_agf_info_local_nonprim);
        w_agf_info_local_nonprim = NULL;
    }
    if (tngf_info_local_nonprim) {
        OpenAPI_tngf_info_free(tngf_info_local_nonprim);
        tngf_info_local_nonprim = NULL;
    }
    if (twif_info_local_nonprim) {
        OpenAPI_twif_info_free(twif_info_local_nonprim);
        twif_info_local_nonprim = NULL;
    }
    if (nrf_oauth2_requiredList) {
        OpenAPI_list_for_each(nrf_oauth2_requiredList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nrf_oauth2_requiredList);
        nrf_oauth2_requiredList = NULL;
    }
    if (pvs_infoList) {
        OpenAPI_list_for_each(pvs_infoList, node) {
            OpenAPI_server_addressing_info_free(node->data);
        }
        OpenAPI_list_free(pvs_infoList);
        pvs_infoList = NULL;
    }
    if (pcf_ue_callback_info_local_nonprim) {
        OpenAPI_pcf_ue_callback_info_free(pcf_ue_callback_info_local_nonprim);
        pcf_ue_callback_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sm_context_create_data_t *OpenAPI_sm_context_create_data_copy(OpenAPI_sm_context_create_data_t *dst, OpenAPI_sm_context_create_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_create_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed");
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

    OpenAPI_sm_context_create_data_free(dst);
    dst = OpenAPI_sm_context_create_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

