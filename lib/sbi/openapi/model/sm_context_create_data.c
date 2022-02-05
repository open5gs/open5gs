
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
    OpenAPI_trace_data_t *trace_data,
    char *udm_group_id,
    char *routing_indicator,
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
    int ran_unchanged_ind
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
    sm_context_create_data_local_var->trace_data = trace_data;
    sm_context_create_data_local_var->udm_group_id = udm_group_id;
    sm_context_create_data_local_var->routing_indicator = routing_indicator;
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

    return sm_context_create_data_local_var;
}

void OpenAPI_sm_context_create_data_free(OpenAPI_sm_context_create_data_t *sm_context_create_data)
{
    if (NULL == sm_context_create_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sm_context_create_data->supi);
    ogs_free(sm_context_create_data->pei);
    ogs_free(sm_context_create_data->gpsi);
    ogs_free(sm_context_create_data->dnn);
    ogs_free(sm_context_create_data->selected_dnn);
    OpenAPI_snssai_free(sm_context_create_data->s_nssai);
    OpenAPI_snssai_free(sm_context_create_data->hplmn_snssai);
    ogs_free(sm_context_create_data->serving_nf_id);
    OpenAPI_guami_free(sm_context_create_data->guami);
    ogs_free(sm_context_create_data->service_name);
    OpenAPI_plmn_id_nid_free(sm_context_create_data->serving_network);
    OpenAPI_ref_to_binary_data_free(sm_context_create_data->n1_sm_msg);
    OpenAPI_user_location_free(sm_context_create_data->ue_location);
    ogs_free(sm_context_create_data->ue_time_zone);
    OpenAPI_user_location_free(sm_context_create_data->add_ue_location);
    ogs_free(sm_context_create_data->sm_context_status_uri);
    ogs_free(sm_context_create_data->h_smf_uri);
    ogs_free(sm_context_create_data->h_smf_id);
    ogs_free(sm_context_create_data->smf_uri);
    ogs_free(sm_context_create_data->smf_id);
    OpenAPI_list_for_each(sm_context_create_data->additional_hsmf_uri, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sm_context_create_data->additional_hsmf_uri);
    OpenAPI_list_for_each(sm_context_create_data->additional_hsmf_id, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sm_context_create_data->additional_hsmf_id);
    OpenAPI_list_for_each(sm_context_create_data->additional_smf_uri, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sm_context_create_data->additional_smf_uri);
    OpenAPI_list_for_each(sm_context_create_data->additional_smf_id, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sm_context_create_data->additional_smf_id);
    OpenAPI_list_for_each(sm_context_create_data->pdu_sessions_activate_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sm_context_create_data->pdu_sessions_activate_list);
    ogs_free(sm_context_create_data->ue_eps_pdn_connection);
    ogs_free(sm_context_create_data->pcf_id);
    ogs_free(sm_context_create_data->pcf_group_id);
    ogs_free(sm_context_create_data->pcf_set_id);
    ogs_free(sm_context_create_data->nrf_uri);
    ogs_free(sm_context_create_data->supported_features);
    OpenAPI_list_for_each(sm_context_create_data->backup_amf_info, node) {
        OpenAPI_backup_amf_info_free(node->data);
    }
    OpenAPI_list_free(sm_context_create_data->backup_amf_info);
    OpenAPI_trace_data_free(sm_context_create_data->trace_data);
    ogs_free(sm_context_create_data->udm_group_id);
    ogs_free(sm_context_create_data->routing_indicator);
    OpenAPI_ng_ran_target_id_free(sm_context_create_data->target_id);
    ogs_free(sm_context_create_data->eps_bearer_ctx_status);
    OpenAPI_ref_to_binary_data_free(sm_context_create_data->n2_sm_info);
    OpenAPI_ref_to_binary_data_free(sm_context_create_data->n2_sm_info_ext1);
    ogs_free(sm_context_create_data->sm_context_ref);
    ogs_free(sm_context_create_data->sm_context_smf_id);
    ogs_free(sm_context_create_data->sm_context_smf_set_id);
    ogs_free(sm_context_create_data->sm_context_smf_service_set_id);
    OpenAPI_small_data_rate_status_free(sm_context_create_data->small_data_rate_status);
    OpenAPI_apn_rate_status_free(sm_context_create_data->apn_rate_status);
    OpenAPI_ddn_failure_subs_free(sm_context_create_data->ddn_failure_subs);
    ogs_free(sm_context_create_data->old_smf_id);
    ogs_free(sm_context_create_data->old_sm_context_ref);
    OpenAPI_w_agf_info_free(sm_context_create_data->w_agf_info);
    OpenAPI_tngf_info_free(sm_context_create_data->tngf_info);
    OpenAPI_twif_info_free(sm_context_create_data->twif_info);
    ogs_free(sm_context_create_data);
}

cJSON *OpenAPI_sm_context_create_data_convertToJSON(OpenAPI_sm_context_create_data_t *sm_context_create_data)
{
    cJSON *item = NULL;

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

    if (sm_context_create_data->request_type) {
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

    if (cJSON_AddStringToObject(item, "anType", OpenAPI_access_type_ToString(sm_context_create_data->an_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [an_type]");
        goto end;
    }

    if (sm_context_create_data->additional_an_type) {
    if (cJSON_AddStringToObject(item, "additionalAnType", OpenAPI_access_type_ToString(sm_context_create_data->additional_an_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_an_type]");
        goto end;
    }
    }

    if (sm_context_create_data->rat_type) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(sm_context_create_data->rat_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (sm_context_create_data->presence_in_ladn) {
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
    cJSON *additional_hsmf_uri = cJSON_AddArrayToObject(item, "additionalHsmfUri");
    if (additional_hsmf_uri == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_hsmf_uri]");
        goto end;
    }

    OpenAPI_lnode_t *additional_hsmf_uri_node;
    OpenAPI_list_for_each(sm_context_create_data->additional_hsmf_uri, additional_hsmf_uri_node)  {
    if (cJSON_AddStringToObject(additional_hsmf_uri, "", (char*)additional_hsmf_uri_node->data) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_hsmf_uri]");
        goto end;
    }
                    }
    }

    if (sm_context_create_data->additional_hsmf_id) {
    cJSON *additional_hsmf_id = cJSON_AddArrayToObject(item, "additionalHsmfId");
    if (additional_hsmf_id == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_hsmf_id]");
        goto end;
    }

    OpenAPI_lnode_t *additional_hsmf_id_node;
    OpenAPI_list_for_each(sm_context_create_data->additional_hsmf_id, additional_hsmf_id_node)  {
    if (cJSON_AddStringToObject(additional_hsmf_id, "", (char*)additional_hsmf_id_node->data) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_hsmf_id]");
        goto end;
    }
                    }
    }

    if (sm_context_create_data->additional_smf_uri) {
    cJSON *additional_smf_uri = cJSON_AddArrayToObject(item, "additionalSmfUri");
    if (additional_smf_uri == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_smf_uri]");
        goto end;
    }

    OpenAPI_lnode_t *additional_smf_uri_node;
    OpenAPI_list_for_each(sm_context_create_data->additional_smf_uri, additional_smf_uri_node)  {
    if (cJSON_AddStringToObject(additional_smf_uri, "", (char*)additional_smf_uri_node->data) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_smf_uri]");
        goto end;
    }
                    }
    }

    if (sm_context_create_data->additional_smf_id) {
    cJSON *additional_smf_id = cJSON_AddArrayToObject(item, "additionalSmfId");
    if (additional_smf_id == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [additional_smf_id]");
        goto end;
    }

    OpenAPI_lnode_t *additional_smf_id_node;
    OpenAPI_list_for_each(sm_context_create_data->additional_smf_id, additional_smf_id_node)  {
    if (cJSON_AddStringToObject(additional_smf_id, "", (char*)additional_smf_id_node->data) == NULL) {
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
    cJSON *pdu_sessions_activate_list = cJSON_AddArrayToObject(item, "pduSessionsActivateList");
    if (pdu_sessions_activate_list == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [pdu_sessions_activate_list]");
        goto end;
    }

    OpenAPI_lnode_t *pdu_sessions_activate_list_node;
    OpenAPI_list_for_each(sm_context_create_data->pdu_sessions_activate_list, pdu_sessions_activate_list_node)  {
    if (cJSON_AddNumberToObject(pdu_sessions_activate_list, "", *(double *)pdu_sessions_activate_list_node->data) == NULL) {
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

    if (sm_context_create_data->ho_state) {
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

    if (sm_context_create_data->sel_mode) {
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

    OpenAPI_lnode_t *backup_amf_info_node;
    if (sm_context_create_data->backup_amf_info) {
        OpenAPI_list_for_each(sm_context_create_data->backup_amf_info, backup_amf_info_node) {
            cJSON *itemLocal = OpenAPI_backup_amf_info_convertToJSON(backup_amf_info_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [backup_amf_info]");
                goto end;
            }
            cJSON_AddItemToArray(backup_amf_infoList, itemLocal);
        }
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

    if (sm_context_create_data->eps_interworking_ind) {
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

    if (sm_context_create_data->n2_sm_info_type) {
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

    if (sm_context_create_data->n2_sm_info_type_ext1) {
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

    if (sm_context_create_data->sm_context_smf_binding) {
    if (cJSON_AddStringToObject(item, "smContextSmfBinding", OpenAPI_sbi_binding_level_ToString(sm_context_create_data->sm_context_smf_binding)) == NULL) {
        ogs_error("OpenAPI_sm_context_create_data_convertToJSON() failed [sm_context_smf_binding]");
        goto end;
    }
    }

    if (sm_context_create_data->up_cnx_state) {
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

end:
    return item;
}

OpenAPI_sm_context_create_data_t *OpenAPI_sm_context_create_data_parseFromJSON(cJSON *sm_context_create_dataJSON)
{
    OpenAPI_sm_context_create_data_t *sm_context_create_data_local_var = NULL;
    cJSON *supi = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "supi");

    if (supi) {
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    cJSON *unauthenticated_supi = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "unauthenticatedSupi");

    if (unauthenticated_supi) {
    if (!cJSON_IsBool(unauthenticated_supi)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [unauthenticated_supi]");
        goto end;
    }
    }

    cJSON *pei = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pei");

    if (pei) {
    if (!cJSON_IsString(pei)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    cJSON *gpsi = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "gpsi");

    if (gpsi) {
    if (!cJSON_IsString(gpsi)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    cJSON *pdu_session_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pduSessionId");

    if (pdu_session_id) {
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "dnn");

    if (dnn) {
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    cJSON *selected_dnn = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "selectedDnn");

    if (selected_dnn) {
    if (!cJSON_IsString(selected_dnn)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [selected_dnn]");
        goto end;
    }
    }

    cJSON *s_nssai = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "sNssai");

    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    if (s_nssai) {
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    }

    cJSON *hplmn_snssai = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "hplmnSnssai");

    OpenAPI_snssai_t *hplmn_snssai_local_nonprim = NULL;
    if (hplmn_snssai) {
    hplmn_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(hplmn_snssai);
    }

    cJSON *serving_nf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "servingNfId");
    if (!serving_nf_id) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [serving_nf_id]");
        goto end;
    }

    if (!cJSON_IsString(serving_nf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [serving_nf_id]");
        goto end;
    }

    cJSON *guami = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "guami");

    OpenAPI_guami_t *guami_local_nonprim = NULL;
    if (guami) {
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    }

    cJSON *service_name = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "serviceName");

    if (service_name) {
    if (!cJSON_IsString(service_name)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [service_name]");
        goto end;
    }
    }

    cJSON *serving_network = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "servingNetwork");
    if (!serving_network) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [serving_network]");
        goto end;
    }

    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);

    cJSON *request_type = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "requestType");

    OpenAPI_request_type_e request_typeVariable;
    if (request_type) {
    if (!cJSON_IsString(request_type)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [request_type]");
        goto end;
    }
    request_typeVariable = OpenAPI_request_type_FromString(request_type->valuestring);
    }

    cJSON *n1_sm_msg = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "n1SmMsg");

    OpenAPI_ref_to_binary_data_t *n1_sm_msg_local_nonprim = NULL;
    if (n1_sm_msg) {
    n1_sm_msg_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_msg);
    }

    cJSON *an_type = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "anType");
    if (!an_type) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [an_type]");
        goto end;
    }

    OpenAPI_access_type_e an_typeVariable;
    if (!cJSON_IsString(an_type)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [an_type]");
        goto end;
    }
    an_typeVariable = OpenAPI_access_type_FromString(an_type->valuestring);

    cJSON *additional_an_type = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "additionalAnType");

    OpenAPI_access_type_e additional_an_typeVariable;
    if (additional_an_type) {
    if (!cJSON_IsString(additional_an_type)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_an_type]");
        goto end;
    }
    additional_an_typeVariable = OpenAPI_access_type_FromString(additional_an_type->valuestring);
    }

    cJSON *rat_type = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "ratType");

    OpenAPI_rat_type_e rat_typeVariable;
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    cJSON *presence_in_ladn = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "presenceInLadn");

    OpenAPI_presence_state_e presence_in_ladnVariable;
    if (presence_in_ladn) {
    if (!cJSON_IsString(presence_in_ladn)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [presence_in_ladn]");
        goto end;
    }
    presence_in_ladnVariable = OpenAPI_presence_state_FromString(presence_in_ladn->valuestring);
    }

    cJSON *ue_location = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "ueLocation");

    OpenAPI_user_location_t *ue_location_local_nonprim = NULL;
    if (ue_location) {
    ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    }

    cJSON *ue_time_zone = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "ueTimeZone");

    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    cJSON *add_ue_location = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "addUeLocation");

    OpenAPI_user_location_t *add_ue_location_local_nonprim = NULL;
    if (add_ue_location) {
    add_ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(add_ue_location);
    }

    cJSON *sm_context_status_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextStatusUri");
    if (!sm_context_status_uri) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_status_uri]");
        goto end;
    }

    if (!cJSON_IsString(sm_context_status_uri)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_status_uri]");
        goto end;
    }

    cJSON *h_smf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "hSmfUri");

    if (h_smf_uri) {
    if (!cJSON_IsString(h_smf_uri)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [h_smf_uri]");
        goto end;
    }
    }

    cJSON *h_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "hSmfId");

    if (h_smf_id) {
    if (!cJSON_IsString(h_smf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [h_smf_id]");
        goto end;
    }
    }

    cJSON *smf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smfUri");

    if (smf_uri) {
    if (!cJSON_IsString(smf_uri)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [smf_uri]");
        goto end;
    }
    }

    cJSON *smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smfId");

    if (smf_id) {
    if (!cJSON_IsString(smf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [smf_id]");
        goto end;
    }
    }

    cJSON *additional_hsmf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "additionalHsmfUri");

    OpenAPI_list_t *additional_hsmf_uriList;
    if (additional_hsmf_uri) {
    cJSON *additional_hsmf_uri_local;
    if (!cJSON_IsArray(additional_hsmf_uri)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_hsmf_uri]");
        goto end;
    }
    additional_hsmf_uriList = OpenAPI_list_create();

    cJSON_ArrayForEach(additional_hsmf_uri_local, additional_hsmf_uri) {
    if (!cJSON_IsString(additional_hsmf_uri_local)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_hsmf_uri]");
        goto end;
    }
    OpenAPI_list_add(additional_hsmf_uriList , ogs_strdup(additional_hsmf_uri_local->valuestring));
    }
    }

    cJSON *additional_hsmf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "additionalHsmfId");

    OpenAPI_list_t *additional_hsmf_idList;
    if (additional_hsmf_id) {
    cJSON *additional_hsmf_id_local;
    if (!cJSON_IsArray(additional_hsmf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_hsmf_id]");
        goto end;
    }
    additional_hsmf_idList = OpenAPI_list_create();

    cJSON_ArrayForEach(additional_hsmf_id_local, additional_hsmf_id) {
    if (!cJSON_IsString(additional_hsmf_id_local)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_hsmf_id]");
        goto end;
    }
    OpenAPI_list_add(additional_hsmf_idList , ogs_strdup(additional_hsmf_id_local->valuestring));
    }
    }

    cJSON *additional_smf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "additionalSmfUri");

    OpenAPI_list_t *additional_smf_uriList;
    if (additional_smf_uri) {
    cJSON *additional_smf_uri_local;
    if (!cJSON_IsArray(additional_smf_uri)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_smf_uri]");
        goto end;
    }
    additional_smf_uriList = OpenAPI_list_create();

    cJSON_ArrayForEach(additional_smf_uri_local, additional_smf_uri) {
    if (!cJSON_IsString(additional_smf_uri_local)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_smf_uri]");
        goto end;
    }
    OpenAPI_list_add(additional_smf_uriList , ogs_strdup(additional_smf_uri_local->valuestring));
    }
    }

    cJSON *additional_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "additionalSmfId");

    OpenAPI_list_t *additional_smf_idList;
    if (additional_smf_id) {
    cJSON *additional_smf_id_local;
    if (!cJSON_IsArray(additional_smf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_smf_id]");
        goto end;
    }
    additional_smf_idList = OpenAPI_list_create();

    cJSON_ArrayForEach(additional_smf_id_local, additional_smf_id) {
    if (!cJSON_IsString(additional_smf_id_local)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [additional_smf_id]");
        goto end;
    }
    OpenAPI_list_add(additional_smf_idList , ogs_strdup(additional_smf_id_local->valuestring));
    }
    }

    cJSON *old_pdu_session_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "oldPduSessionId");

    if (old_pdu_session_id) {
    if (!cJSON_IsNumber(old_pdu_session_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [old_pdu_session_id]");
        goto end;
    }
    }

    cJSON *pdu_sessions_activate_list = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pduSessionsActivateList");

    OpenAPI_list_t *pdu_sessions_activate_listList;
    if (pdu_sessions_activate_list) {
    cJSON *pdu_sessions_activate_list_local;
    if (!cJSON_IsArray(pdu_sessions_activate_list)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pdu_sessions_activate_list]");
        goto end;
    }
    pdu_sessions_activate_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(pdu_sessions_activate_list_local, pdu_sessions_activate_list) {
    if (!cJSON_IsNumber(pdu_sessions_activate_list_local)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pdu_sessions_activate_list]");
        goto end;
    }
    OpenAPI_list_add(pdu_sessions_activate_listList , &pdu_sessions_activate_list_local->valuedouble);
    }
    }

    cJSON *ue_eps_pdn_connection = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "ueEpsPdnConnection");

    if (ue_eps_pdn_connection) {
    if (!cJSON_IsString(ue_eps_pdn_connection)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [ue_eps_pdn_connection]");
        goto end;
    }
    }

    cJSON *ho_state = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "hoState");

    OpenAPI_ho_state_e ho_stateVariable;
    if (ho_state) {
    if (!cJSON_IsString(ho_state)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [ho_state]");
        goto end;
    }
    ho_stateVariable = OpenAPI_ho_state_FromString(ho_state->valuestring);
    }

    cJSON *pcf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pcfId");

    if (pcf_id) {
    if (!cJSON_IsString(pcf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    cJSON *pcf_group_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pcfGroupId");

    if (pcf_group_id) {
    if (!cJSON_IsString(pcf_group_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pcf_group_id]");
        goto end;
    }
    }

    cJSON *pcf_set_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "pcfSetId");

    if (pcf_set_id) {
    if (!cJSON_IsString(pcf_set_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    cJSON *nrf_uri = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "nrfUri");

    if (nrf_uri) {
    if (!cJSON_IsString(nrf_uri)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [nrf_uri]");
        goto end;
    }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *sel_mode = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "selMode");

    OpenAPI_dnn_selection_mode_e sel_modeVariable;
    if (sel_mode) {
    if (!cJSON_IsString(sel_mode)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sel_mode]");
        goto end;
    }
    sel_modeVariable = OpenAPI_dnn_selection_mode_FromString(sel_mode->valuestring);
    }

    cJSON *backup_amf_info = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "backupAmfInfo");

    OpenAPI_list_t *backup_amf_infoList;
    if (backup_amf_info) {
    cJSON *backup_amf_info_local_nonprimitive;
    if (!cJSON_IsArray(backup_amf_info)){
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [backup_amf_info]");
        goto end;
    }

    backup_amf_infoList = OpenAPI_list_create();

    cJSON_ArrayForEach(backup_amf_info_local_nonprimitive, backup_amf_info ) {
        if (!cJSON_IsObject(backup_amf_info_local_nonprimitive)) {
            ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [backup_amf_info]");
            goto end;
        }
        OpenAPI_backup_amf_info_t *backup_amf_infoItem = OpenAPI_backup_amf_info_parseFromJSON(backup_amf_info_local_nonprimitive);

        if (!backup_amf_infoItem) {
            ogs_error("No backup_amf_infoItem");
            OpenAPI_list_free(backup_amf_infoList);
            goto end;
        }

        OpenAPI_list_add(backup_amf_infoList, backup_amf_infoItem);
    }
    }

    cJSON *trace_data = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "traceData");

    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    if (trace_data) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    }

    cJSON *udm_group_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "udmGroupId");

    if (udm_group_id) {
    if (!cJSON_IsString(udm_group_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [udm_group_id]");
        goto end;
    }
    }

    cJSON *routing_indicator = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "routingIndicator");

    if (routing_indicator) {
    if (!cJSON_IsString(routing_indicator)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [routing_indicator]");
        goto end;
    }
    }

    cJSON *eps_interworking_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "epsInterworkingInd");

    OpenAPI_eps_interworking_indication_e eps_interworking_indVariable;
    if (eps_interworking_ind) {
    if (!cJSON_IsString(eps_interworking_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [eps_interworking_ind]");
        goto end;
    }
    eps_interworking_indVariable = OpenAPI_eps_interworking_indication_FromString(eps_interworking_ind->valuestring);
    }

    cJSON *indirect_forwarding_flag = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "indirectForwardingFlag");

    if (indirect_forwarding_flag) {
    if (!cJSON_IsBool(indirect_forwarding_flag)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [indirect_forwarding_flag]");
        goto end;
    }
    }

    cJSON *direct_forwarding_flag = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "directForwardingFlag");

    if (direct_forwarding_flag) {
    if (!cJSON_IsBool(direct_forwarding_flag)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [direct_forwarding_flag]");
        goto end;
    }
    }

    cJSON *target_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "targetId");

    OpenAPI_ng_ran_target_id_t *target_id_local_nonprim = NULL;
    if (target_id) {
    target_id_local_nonprim = OpenAPI_ng_ran_target_id_parseFromJSON(target_id);
    }

    cJSON *eps_bearer_ctx_status = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "epsBearerCtxStatus");

    if (eps_bearer_ctx_status) {
    if (!cJSON_IsString(eps_bearer_ctx_status)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [eps_bearer_ctx_status]");
        goto end;
    }
    }

    cJSON *cp_ciot_enabled = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "cpCiotEnabled");

    if (cp_ciot_enabled) {
    if (!cJSON_IsBool(cp_ciot_enabled)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [cp_ciot_enabled]");
        goto end;
    }
    }

    cJSON *cp_only_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "cpOnlyInd");

    if (cp_only_ind) {
    if (!cJSON_IsBool(cp_only_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [cp_only_ind]");
        goto end;
    }
    }

    cJSON *invoke_nef = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "invokeNef");

    if (invoke_nef) {
    if (!cJSON_IsBool(invoke_nef)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [invoke_nef]");
        goto end;
    }
    }

    cJSON *ma_request_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "maRequestInd");

    if (ma_request_ind) {
    if (!cJSON_IsBool(ma_request_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [ma_request_ind]");
        goto end;
    }
    }

    cJSON *ma_nw_upgrade_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "maNwUpgradeInd");

    if (ma_nw_upgrade_ind) {
    if (!cJSON_IsBool(ma_nw_upgrade_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [ma_nw_upgrade_ind]");
        goto end;
    }
    }

    cJSON *n2_sm_info = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "n2SmInfo");

    OpenAPI_ref_to_binary_data_t *n2_sm_info_local_nonprim = NULL;
    if (n2_sm_info) {
    n2_sm_info_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n2_sm_info);
    }

    cJSON *n2_sm_info_type = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "n2SmInfoType");

    OpenAPI_n2_sm_info_type_e n2_sm_info_typeVariable;
    if (n2_sm_info_type) {
    if (!cJSON_IsString(n2_sm_info_type)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [n2_sm_info_type]");
        goto end;
    }
    n2_sm_info_typeVariable = OpenAPI_n2_sm_info_type_FromString(n2_sm_info_type->valuestring);
    }

    cJSON *n2_sm_info_ext1 = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "n2SmInfoExt1");

    OpenAPI_ref_to_binary_data_t *n2_sm_info_ext1_local_nonprim = NULL;
    if (n2_sm_info_ext1) {
    n2_sm_info_ext1_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n2_sm_info_ext1);
    }

    cJSON *n2_sm_info_type_ext1 = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "n2SmInfoTypeExt1");

    OpenAPI_n2_sm_info_type_e n2_sm_info_type_ext1Variable;
    if (n2_sm_info_type_ext1) {
    if (!cJSON_IsString(n2_sm_info_type_ext1)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [n2_sm_info_type_ext1]");
        goto end;
    }
    n2_sm_info_type_ext1Variable = OpenAPI_n2_sm_info_type_FromString(n2_sm_info_type_ext1->valuestring);
    }

    cJSON *sm_context_ref = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextRef");

    if (sm_context_ref) {
    if (!cJSON_IsString(sm_context_ref)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_ref]");
        goto end;
    }
    }

    cJSON *sm_context_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextSmfId");

    if (sm_context_smf_id) {
    if (!cJSON_IsString(sm_context_smf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_smf_id]");
        goto end;
    }
    }

    cJSON *sm_context_smf_set_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextSmfSetId");

    if (sm_context_smf_set_id) {
    if (!cJSON_IsString(sm_context_smf_set_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_smf_set_id]");
        goto end;
    }
    }

    cJSON *sm_context_smf_service_set_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextSmfServiceSetId");

    if (sm_context_smf_service_set_id) {
    if (!cJSON_IsString(sm_context_smf_service_set_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_smf_service_set_id]");
        goto end;
    }
    }

    cJSON *sm_context_smf_binding = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smContextSmfBinding");

    OpenAPI_sbi_binding_level_e sm_context_smf_bindingVariable;
    if (sm_context_smf_binding) {
    if (!cJSON_IsString(sm_context_smf_binding)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [sm_context_smf_binding]");
        goto end;
    }
    sm_context_smf_bindingVariable = OpenAPI_sbi_binding_level_FromString(sm_context_smf_binding->valuestring);
    }

    cJSON *up_cnx_state = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "upCnxState");

    OpenAPI_up_cnx_state_e up_cnx_stateVariable;
    if (up_cnx_state) {
    if (!cJSON_IsString(up_cnx_state)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [up_cnx_state]");
        goto end;
    }
    up_cnx_stateVariable = OpenAPI_up_cnx_state_FromString(up_cnx_state->valuestring);
    }

    cJSON *small_data_rate_status = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smallDataRateStatus");

    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    if (small_data_rate_status) {
    small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    }

    cJSON *apn_rate_status = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "apnRateStatus");

    OpenAPI_apn_rate_status_t *apn_rate_status_local_nonprim = NULL;
    if (apn_rate_status) {
    apn_rate_status_local_nonprim = OpenAPI_apn_rate_status_parseFromJSON(apn_rate_status);
    }

    cJSON *extended_nas_sm_timer_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "extendedNasSmTimerInd");

    if (extended_nas_sm_timer_ind) {
    if (!cJSON_IsBool(extended_nas_sm_timer_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [extended_nas_sm_timer_ind]");
        goto end;
    }
    }

    cJSON *dl_data_waiting_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "dlDataWaitingInd");

    if (dl_data_waiting_ind) {
    if (!cJSON_IsBool(dl_data_waiting_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [dl_data_waiting_ind]");
        goto end;
    }
    }

    cJSON *ddn_failure_subs = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "ddnFailureSubs");

    OpenAPI_ddn_failure_subs_t *ddn_failure_subs_local_nonprim = NULL;
    if (ddn_failure_subs) {
    ddn_failure_subs_local_nonprim = OpenAPI_ddn_failure_subs_parseFromJSON(ddn_failure_subs);
    }

    cJSON *smf_transfer_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "smfTransferInd");

    if (smf_transfer_ind) {
    if (!cJSON_IsBool(smf_transfer_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [smf_transfer_ind]");
        goto end;
    }
    }

    cJSON *old_smf_id = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "oldSmfId");

    if (old_smf_id) {
    if (!cJSON_IsString(old_smf_id)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [old_smf_id]");
        goto end;
    }
    }

    cJSON *old_sm_context_ref = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "oldSmContextRef");

    if (old_sm_context_ref) {
    if (!cJSON_IsString(old_sm_context_ref)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [old_sm_context_ref]");
        goto end;
    }
    }

    cJSON *w_agf_info = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "wAgfInfo");

    OpenAPI_w_agf_info_t *w_agf_info_local_nonprim = NULL;
    if (w_agf_info) {
    w_agf_info_local_nonprim = OpenAPI_w_agf_info_parseFromJSON(w_agf_info);
    }

    cJSON *tngf_info = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "tngfInfo");

    OpenAPI_tngf_info_t *tngf_info_local_nonprim = NULL;
    if (tngf_info) {
    tngf_info_local_nonprim = OpenAPI_tngf_info_parseFromJSON(tngf_info);
    }

    cJSON *twif_info = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "twifInfo");

    OpenAPI_twif_info_t *twif_info_local_nonprim = NULL;
    if (twif_info) {
    twif_info_local_nonprim = OpenAPI_twif_info_parseFromJSON(twif_info);
    }

    cJSON *ran_unchanged_ind = cJSON_GetObjectItemCaseSensitive(sm_context_create_dataJSON, "ranUnchangedInd");

    if (ran_unchanged_ind) {
    if (!cJSON_IsBool(ran_unchanged_ind)) {
        ogs_error("OpenAPI_sm_context_create_data_parseFromJSON() failed [ran_unchanged_ind]");
        goto end;
    }
    }

    sm_context_create_data_local_var = OpenAPI_sm_context_create_data_create (
        supi ? ogs_strdup(supi->valuestring) : NULL,
        unauthenticated_supi ? true : false,
        unauthenticated_supi ? unauthenticated_supi->valueint : 0,
        pei ? ogs_strdup(pei->valuestring) : NULL,
        gpsi ? ogs_strdup(gpsi->valuestring) : NULL,
        pdu_session_id ? true : false,
        pdu_session_id ? pdu_session_id->valuedouble : 0,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        selected_dnn ? ogs_strdup(selected_dnn->valuestring) : NULL,
        s_nssai ? s_nssai_local_nonprim : NULL,
        hplmn_snssai ? hplmn_snssai_local_nonprim : NULL,
        ogs_strdup(serving_nf_id->valuestring),
        guami ? guami_local_nonprim : NULL,
        service_name ? ogs_strdup(service_name->valuestring) : NULL,
        serving_network_local_nonprim,
        request_type ? request_typeVariable : 0,
        n1_sm_msg ? n1_sm_msg_local_nonprim : NULL,
        an_typeVariable,
        additional_an_type ? additional_an_typeVariable : 0,
        rat_type ? rat_typeVariable : 0,
        presence_in_ladn ? presence_in_ladnVariable : 0,
        ue_location ? ue_location_local_nonprim : NULL,
        ue_time_zone ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        add_ue_location ? add_ue_location_local_nonprim : NULL,
        ogs_strdup(sm_context_status_uri->valuestring),
        h_smf_uri ? ogs_strdup(h_smf_uri->valuestring) : NULL,
        h_smf_id ? ogs_strdup(h_smf_id->valuestring) : NULL,
        smf_uri ? ogs_strdup(smf_uri->valuestring) : NULL,
        smf_id ? ogs_strdup(smf_id->valuestring) : NULL,
        additional_hsmf_uri ? additional_hsmf_uriList : NULL,
        additional_hsmf_id ? additional_hsmf_idList : NULL,
        additional_smf_uri ? additional_smf_uriList : NULL,
        additional_smf_id ? additional_smf_idList : NULL,
        old_pdu_session_id ? true : false,
        old_pdu_session_id ? old_pdu_session_id->valuedouble : 0,
        pdu_sessions_activate_list ? pdu_sessions_activate_listList : NULL,
        ue_eps_pdn_connection ? ogs_strdup(ue_eps_pdn_connection->valuestring) : NULL,
        ho_state ? ho_stateVariable : 0,
        pcf_id ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_group_id ? ogs_strdup(pcf_group_id->valuestring) : NULL,
        pcf_set_id ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        nrf_uri ? ogs_strdup(nrf_uri->valuestring) : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        sel_mode ? sel_modeVariable : 0,
        backup_amf_info ? backup_amf_infoList : NULL,
        trace_data ? trace_data_local_nonprim : NULL,
        udm_group_id ? ogs_strdup(udm_group_id->valuestring) : NULL,
        routing_indicator ? ogs_strdup(routing_indicator->valuestring) : NULL,
        eps_interworking_ind ? eps_interworking_indVariable : 0,
        indirect_forwarding_flag ? true : false,
        indirect_forwarding_flag ? indirect_forwarding_flag->valueint : 0,
        direct_forwarding_flag ? true : false,
        direct_forwarding_flag ? direct_forwarding_flag->valueint : 0,
        target_id ? target_id_local_nonprim : NULL,
        eps_bearer_ctx_status ? ogs_strdup(eps_bearer_ctx_status->valuestring) : NULL,
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
        sm_context_ref ? ogs_strdup(sm_context_ref->valuestring) : NULL,
        sm_context_smf_id ? ogs_strdup(sm_context_smf_id->valuestring) : NULL,
        sm_context_smf_set_id ? ogs_strdup(sm_context_smf_set_id->valuestring) : NULL,
        sm_context_smf_service_set_id ? ogs_strdup(sm_context_smf_service_set_id->valuestring) : NULL,
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
        old_smf_id ? ogs_strdup(old_smf_id->valuestring) : NULL,
        old_sm_context_ref ? ogs_strdup(old_sm_context_ref->valuestring) : NULL,
        w_agf_info ? w_agf_info_local_nonprim : NULL,
        tngf_info ? tngf_info_local_nonprim : NULL,
        twif_info ? twif_info_local_nonprim : NULL,
        ran_unchanged_ind ? true : false,
        ran_unchanged_ind ? ran_unchanged_ind->valueint : 0
    );

    return sm_context_create_data_local_var;
end:
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

