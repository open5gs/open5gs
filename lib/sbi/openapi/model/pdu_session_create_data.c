
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_create_data.h"

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
)
{
    OpenAPI_pdu_session_create_data_t *pdu_session_create_data_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_create_data_t));
    ogs_assert(pdu_session_create_data_local_var);

    pdu_session_create_data_local_var->supi = supi;
    pdu_session_create_data_local_var->is_unauthenticated_supi = is_unauthenticated_supi;
    pdu_session_create_data_local_var->unauthenticated_supi = unauthenticated_supi;
    pdu_session_create_data_local_var->pei = pei;
    pdu_session_create_data_local_var->is_pdu_session_id = is_pdu_session_id;
    pdu_session_create_data_local_var->pdu_session_id = pdu_session_id;
    pdu_session_create_data_local_var->dnn = dnn;
    pdu_session_create_data_local_var->selected_dnn = selected_dnn;
    pdu_session_create_data_local_var->s_nssai = s_nssai;
    pdu_session_create_data_local_var->hplmn_snssai = hplmn_snssai;
    pdu_session_create_data_local_var->vsmf_id = vsmf_id;
    pdu_session_create_data_local_var->ismf_id = ismf_id;
    pdu_session_create_data_local_var->serving_network = serving_network;
    pdu_session_create_data_local_var->request_type = request_type;
    pdu_session_create_data_local_var->eps_bearer_id = eps_bearer_id;
    pdu_session_create_data_local_var->pgw_s8c_fteid = pgw_s8c_fteid;
    pdu_session_create_data_local_var->vsmf_pdu_session_uri = vsmf_pdu_session_uri;
    pdu_session_create_data_local_var->ismf_pdu_session_uri = ismf_pdu_session_uri;
    pdu_session_create_data_local_var->vcn_tunnel_info = vcn_tunnel_info;
    pdu_session_create_data_local_var->icn_tunnel_info = icn_tunnel_info;
    pdu_session_create_data_local_var->n9_forwarding_tunnel_info = n9_forwarding_tunnel_info;
    pdu_session_create_data_local_var->additional_cn_tunnel_info = additional_cn_tunnel_info;
    pdu_session_create_data_local_var->an_type = an_type;
    pdu_session_create_data_local_var->additional_an_type = additional_an_type;
    pdu_session_create_data_local_var->rat_type = rat_type;
    pdu_session_create_data_local_var->ue_location = ue_location;
    pdu_session_create_data_local_var->ue_time_zone = ue_time_zone;
    pdu_session_create_data_local_var->add_ue_location = add_ue_location;
    pdu_session_create_data_local_var->gpsi = gpsi;
    pdu_session_create_data_local_var->n1_sm_info_from_ue = n1_sm_info_from_ue;
    pdu_session_create_data_local_var->unknown_n1_sm_info = unknown_n1_sm_info;
    pdu_session_create_data_local_var->supported_features = supported_features;
    pdu_session_create_data_local_var->h_pcf_id = h_pcf_id;
    pdu_session_create_data_local_var->pcf_id = pcf_id;
    pdu_session_create_data_local_var->pcf_group_id = pcf_group_id;
    pdu_session_create_data_local_var->pcf_set_id = pcf_set_id;
    pdu_session_create_data_local_var->is_ho_preparation_indication = is_ho_preparation_indication;
    pdu_session_create_data_local_var->ho_preparation_indication = ho_preparation_indication;
    pdu_session_create_data_local_var->sel_mode = sel_mode;
    pdu_session_create_data_local_var->is_always_on_requested = is_always_on_requested;
    pdu_session_create_data_local_var->always_on_requested = always_on_requested;
    pdu_session_create_data_local_var->udm_group_id = udm_group_id;
    pdu_session_create_data_local_var->routing_indicator = routing_indicator;
    pdu_session_create_data_local_var->is_h_nw_pub_key_id = is_h_nw_pub_key_id;
    pdu_session_create_data_local_var->h_nw_pub_key_id = h_nw_pub_key_id;
    pdu_session_create_data_local_var->eps_interworking_ind = eps_interworking_ind;
    pdu_session_create_data_local_var->v_smf_service_instance_id = v_smf_service_instance_id;
    pdu_session_create_data_local_var->i_smf_service_instance_id = i_smf_service_instance_id;
    pdu_session_create_data_local_var->recovery_time = recovery_time;
    pdu_session_create_data_local_var->roaming_charging_profile = roaming_charging_profile;
    pdu_session_create_data_local_var->charging_id = charging_id;
    pdu_session_create_data_local_var->is_old_pdu_session_id = is_old_pdu_session_id;
    pdu_session_create_data_local_var->old_pdu_session_id = old_pdu_session_id;
    pdu_session_create_data_local_var->eps_bearer_ctx_status = eps_bearer_ctx_status;
    pdu_session_create_data_local_var->amf_nf_id = amf_nf_id;
    pdu_session_create_data_local_var->guami = guami;
    pdu_session_create_data_local_var->max_integrity_protected_data_rate_ul = max_integrity_protected_data_rate_ul;
    pdu_session_create_data_local_var->max_integrity_protected_data_rate_dl = max_integrity_protected_data_rate_dl;
    pdu_session_create_data_local_var->is_cp_ciot_enabled = is_cp_ciot_enabled;
    pdu_session_create_data_local_var->cp_ciot_enabled = cp_ciot_enabled;
    pdu_session_create_data_local_var->is_cp_only_ind = is_cp_only_ind;
    pdu_session_create_data_local_var->cp_only_ind = cp_only_ind;
    pdu_session_create_data_local_var->is_invoke_nef = is_invoke_nef;
    pdu_session_create_data_local_var->invoke_nef = invoke_nef;
    pdu_session_create_data_local_var->is_ma_request_ind = is_ma_request_ind;
    pdu_session_create_data_local_var->ma_request_ind = ma_request_ind;
    pdu_session_create_data_local_var->is_ma_nw_upgrade_ind = is_ma_nw_upgrade_ind;
    pdu_session_create_data_local_var->ma_nw_upgrade_ind = ma_nw_upgrade_ind;
    pdu_session_create_data_local_var->dnai_list = dnai_list;
    pdu_session_create_data_local_var->presence_in_ladn = presence_in_ladn;
    pdu_session_create_data_local_var->secondary_rat_usage_info = secondary_rat_usage_info;
    pdu_session_create_data_local_var->small_data_rate_status = small_data_rate_status;
    pdu_session_create_data_local_var->apn_rate_status = apn_rate_status;
    pdu_session_create_data_local_var->is_dl_serving_plmn_rate_ctl = is_dl_serving_plmn_rate_ctl;
    pdu_session_create_data_local_var->dl_serving_plmn_rate_ctl = dl_serving_plmn_rate_ctl;
    pdu_session_create_data_local_var->up_security_info = up_security_info;
    pdu_session_create_data_local_var->vplmn_qos = vplmn_qos;
    pdu_session_create_data_local_var->old_sm_context_ref = old_sm_context_ref;
    pdu_session_create_data_local_var->redundant_pdu_session_info = redundant_pdu_session_info;
    pdu_session_create_data_local_var->old_pdu_session_ref = old_pdu_session_ref;
    pdu_session_create_data_local_var->is_sm_policy_notify_ind = is_sm_policy_notify_ind;
    pdu_session_create_data_local_var->sm_policy_notify_ind = sm_policy_notify_ind;
    pdu_session_create_data_local_var->is_pcf_ue_callback_info_null = is_pcf_ue_callback_info_null;
    pdu_session_create_data_local_var->pcf_ue_callback_info = pcf_ue_callback_info;
    pdu_session_create_data_local_var->satellite_backhaul_cat = satellite_backhaul_cat;
    pdu_session_create_data_local_var->is_upip_supported = is_upip_supported;
    pdu_session_create_data_local_var->upip_supported = upip_supported;
    pdu_session_create_data_local_var->up_cnx_state = up_cnx_state;
    pdu_session_create_data_local_var->is_disaster_roaming_ind = is_disaster_roaming_ind;
    pdu_session_create_data_local_var->disaster_roaming_ind = disaster_roaming_ind;

    return pdu_session_create_data_local_var;
}

void OpenAPI_pdu_session_create_data_free(OpenAPI_pdu_session_create_data_t *pdu_session_create_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_create_data) {
        return;
    }
    if (pdu_session_create_data->supi) {
        ogs_free(pdu_session_create_data->supi);
        pdu_session_create_data->supi = NULL;
    }
    if (pdu_session_create_data->pei) {
        ogs_free(pdu_session_create_data->pei);
        pdu_session_create_data->pei = NULL;
    }
    if (pdu_session_create_data->dnn) {
        ogs_free(pdu_session_create_data->dnn);
        pdu_session_create_data->dnn = NULL;
    }
    if (pdu_session_create_data->selected_dnn) {
        ogs_free(pdu_session_create_data->selected_dnn);
        pdu_session_create_data->selected_dnn = NULL;
    }
    if (pdu_session_create_data->s_nssai) {
        OpenAPI_snssai_free(pdu_session_create_data->s_nssai);
        pdu_session_create_data->s_nssai = NULL;
    }
    if (pdu_session_create_data->hplmn_snssai) {
        OpenAPI_snssai_free(pdu_session_create_data->hplmn_snssai);
        pdu_session_create_data->hplmn_snssai = NULL;
    }
    if (pdu_session_create_data->vsmf_id) {
        ogs_free(pdu_session_create_data->vsmf_id);
        pdu_session_create_data->vsmf_id = NULL;
    }
    if (pdu_session_create_data->ismf_id) {
        ogs_free(pdu_session_create_data->ismf_id);
        pdu_session_create_data->ismf_id = NULL;
    }
    if (pdu_session_create_data->serving_network) {
        OpenAPI_plmn_id_nid_free(pdu_session_create_data->serving_network);
        pdu_session_create_data->serving_network = NULL;
    }
    if (pdu_session_create_data->eps_bearer_id) {
        OpenAPI_list_for_each(pdu_session_create_data->eps_bearer_id, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pdu_session_create_data->eps_bearer_id);
        pdu_session_create_data->eps_bearer_id = NULL;
    }
    if (pdu_session_create_data->pgw_s8c_fteid) {
        ogs_free(pdu_session_create_data->pgw_s8c_fteid);
        pdu_session_create_data->pgw_s8c_fteid = NULL;
    }
    if (pdu_session_create_data->vsmf_pdu_session_uri) {
        ogs_free(pdu_session_create_data->vsmf_pdu_session_uri);
        pdu_session_create_data->vsmf_pdu_session_uri = NULL;
    }
    if (pdu_session_create_data->ismf_pdu_session_uri) {
        ogs_free(pdu_session_create_data->ismf_pdu_session_uri);
        pdu_session_create_data->ismf_pdu_session_uri = NULL;
    }
    if (pdu_session_create_data->vcn_tunnel_info) {
        OpenAPI_tunnel_info_free(pdu_session_create_data->vcn_tunnel_info);
        pdu_session_create_data->vcn_tunnel_info = NULL;
    }
    if (pdu_session_create_data->icn_tunnel_info) {
        OpenAPI_tunnel_info_free(pdu_session_create_data->icn_tunnel_info);
        pdu_session_create_data->icn_tunnel_info = NULL;
    }
    if (pdu_session_create_data->n9_forwarding_tunnel_info) {
        OpenAPI_tunnel_info_free(pdu_session_create_data->n9_forwarding_tunnel_info);
        pdu_session_create_data->n9_forwarding_tunnel_info = NULL;
    }
    if (pdu_session_create_data->additional_cn_tunnel_info) {
        OpenAPI_tunnel_info_free(pdu_session_create_data->additional_cn_tunnel_info);
        pdu_session_create_data->additional_cn_tunnel_info = NULL;
    }
    if (pdu_session_create_data->ue_location) {
        OpenAPI_user_location_free(pdu_session_create_data->ue_location);
        pdu_session_create_data->ue_location = NULL;
    }
    if (pdu_session_create_data->ue_time_zone) {
        ogs_free(pdu_session_create_data->ue_time_zone);
        pdu_session_create_data->ue_time_zone = NULL;
    }
    if (pdu_session_create_data->add_ue_location) {
        OpenAPI_user_location_free(pdu_session_create_data->add_ue_location);
        pdu_session_create_data->add_ue_location = NULL;
    }
    if (pdu_session_create_data->gpsi) {
        ogs_free(pdu_session_create_data->gpsi);
        pdu_session_create_data->gpsi = NULL;
    }
    if (pdu_session_create_data->n1_sm_info_from_ue) {
        OpenAPI_ref_to_binary_data_free(pdu_session_create_data->n1_sm_info_from_ue);
        pdu_session_create_data->n1_sm_info_from_ue = NULL;
    }
    if (pdu_session_create_data->unknown_n1_sm_info) {
        OpenAPI_ref_to_binary_data_free(pdu_session_create_data->unknown_n1_sm_info);
        pdu_session_create_data->unknown_n1_sm_info = NULL;
    }
    if (pdu_session_create_data->supported_features) {
        ogs_free(pdu_session_create_data->supported_features);
        pdu_session_create_data->supported_features = NULL;
    }
    if (pdu_session_create_data->h_pcf_id) {
        ogs_free(pdu_session_create_data->h_pcf_id);
        pdu_session_create_data->h_pcf_id = NULL;
    }
    if (pdu_session_create_data->pcf_id) {
        ogs_free(pdu_session_create_data->pcf_id);
        pdu_session_create_data->pcf_id = NULL;
    }
    if (pdu_session_create_data->pcf_group_id) {
        ogs_free(pdu_session_create_data->pcf_group_id);
        pdu_session_create_data->pcf_group_id = NULL;
    }
    if (pdu_session_create_data->pcf_set_id) {
        ogs_free(pdu_session_create_data->pcf_set_id);
        pdu_session_create_data->pcf_set_id = NULL;
    }
    if (pdu_session_create_data->udm_group_id) {
        ogs_free(pdu_session_create_data->udm_group_id);
        pdu_session_create_data->udm_group_id = NULL;
    }
    if (pdu_session_create_data->routing_indicator) {
        ogs_free(pdu_session_create_data->routing_indicator);
        pdu_session_create_data->routing_indicator = NULL;
    }
    if (pdu_session_create_data->v_smf_service_instance_id) {
        ogs_free(pdu_session_create_data->v_smf_service_instance_id);
        pdu_session_create_data->v_smf_service_instance_id = NULL;
    }
    if (pdu_session_create_data->i_smf_service_instance_id) {
        ogs_free(pdu_session_create_data->i_smf_service_instance_id);
        pdu_session_create_data->i_smf_service_instance_id = NULL;
    }
    if (pdu_session_create_data->recovery_time) {
        ogs_free(pdu_session_create_data->recovery_time);
        pdu_session_create_data->recovery_time = NULL;
    }
    if (pdu_session_create_data->roaming_charging_profile) {
        OpenAPI_roaming_charging_profile_free(pdu_session_create_data->roaming_charging_profile);
        pdu_session_create_data->roaming_charging_profile = NULL;
    }
    if (pdu_session_create_data->charging_id) {
        ogs_free(pdu_session_create_data->charging_id);
        pdu_session_create_data->charging_id = NULL;
    }
    if (pdu_session_create_data->eps_bearer_ctx_status) {
        ogs_free(pdu_session_create_data->eps_bearer_ctx_status);
        pdu_session_create_data->eps_bearer_ctx_status = NULL;
    }
    if (pdu_session_create_data->amf_nf_id) {
        ogs_free(pdu_session_create_data->amf_nf_id);
        pdu_session_create_data->amf_nf_id = NULL;
    }
    if (pdu_session_create_data->guami) {
        OpenAPI_guami_free(pdu_session_create_data->guami);
        pdu_session_create_data->guami = NULL;
    }
    if (pdu_session_create_data->dnai_list) {
        OpenAPI_list_for_each(pdu_session_create_data->dnai_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pdu_session_create_data->dnai_list);
        pdu_session_create_data->dnai_list = NULL;
    }
    if (pdu_session_create_data->secondary_rat_usage_info) {
        OpenAPI_list_for_each(pdu_session_create_data->secondary_rat_usage_info, node) {
            OpenAPI_secondary_rat_usage_info_free(node->data);
        }
        OpenAPI_list_free(pdu_session_create_data->secondary_rat_usage_info);
        pdu_session_create_data->secondary_rat_usage_info = NULL;
    }
    if (pdu_session_create_data->small_data_rate_status) {
        OpenAPI_small_data_rate_status_free(pdu_session_create_data->small_data_rate_status);
        pdu_session_create_data->small_data_rate_status = NULL;
    }
    if (pdu_session_create_data->apn_rate_status) {
        OpenAPI_apn_rate_status_free(pdu_session_create_data->apn_rate_status);
        pdu_session_create_data->apn_rate_status = NULL;
    }
    if (pdu_session_create_data->up_security_info) {
        OpenAPI_up_security_info_free(pdu_session_create_data->up_security_info);
        pdu_session_create_data->up_security_info = NULL;
    }
    if (pdu_session_create_data->vplmn_qos) {
        OpenAPI_vplmn_qos_free(pdu_session_create_data->vplmn_qos);
        pdu_session_create_data->vplmn_qos = NULL;
    }
    if (pdu_session_create_data->old_sm_context_ref) {
        ogs_free(pdu_session_create_data->old_sm_context_ref);
        pdu_session_create_data->old_sm_context_ref = NULL;
    }
    if (pdu_session_create_data->redundant_pdu_session_info) {
        OpenAPI_redundant_pdu_session_information_free(pdu_session_create_data->redundant_pdu_session_info);
        pdu_session_create_data->redundant_pdu_session_info = NULL;
    }
    if (pdu_session_create_data->old_pdu_session_ref) {
        ogs_free(pdu_session_create_data->old_pdu_session_ref);
        pdu_session_create_data->old_pdu_session_ref = NULL;
    }
    if (pdu_session_create_data->pcf_ue_callback_info) {
        OpenAPI_pcf_ue_callback_info_free(pdu_session_create_data->pcf_ue_callback_info);
        pdu_session_create_data->pcf_ue_callback_info = NULL;
    }
    ogs_free(pdu_session_create_data);
}

cJSON *OpenAPI_pdu_session_create_data_convertToJSON(OpenAPI_pdu_session_create_data_t *pdu_session_create_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_create_data == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [PduSessionCreateData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_session_create_data->supi) {
    if (cJSON_AddStringToObject(item, "supi", pdu_session_create_data->supi) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_unauthenticated_supi) {
    if (cJSON_AddBoolToObject(item, "unauthenticatedSupi", pdu_session_create_data->unauthenticated_supi) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [unauthenticated_supi]");
        goto end;
    }
    }

    if (pdu_session_create_data->pei) {
    if (cJSON_AddStringToObject(item, "pei", pdu_session_create_data->pei) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_pdu_session_id) {
    if (cJSON_AddNumberToObject(item, "pduSessionId", pdu_session_create_data->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [pdu_session_id]");
        goto end;
    }
    }

    if (!pdu_session_create_data->dnn) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", pdu_session_create_data->dnn) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [dnn]");
        goto end;
    }

    if (pdu_session_create_data->selected_dnn) {
    if (cJSON_AddStringToObject(item, "selectedDnn", pdu_session_create_data->selected_dnn) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [selected_dnn]");
        goto end;
    }
    }

    if (pdu_session_create_data->s_nssai) {
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(pdu_session_create_data->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [s_nssai]");
        goto end;
    }
    }

    if (pdu_session_create_data->hplmn_snssai) {
    cJSON *hplmn_snssai_local_JSON = OpenAPI_snssai_convertToJSON(pdu_session_create_data->hplmn_snssai);
    if (hplmn_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [hplmn_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hplmnSnssai", hplmn_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [hplmn_snssai]");
        goto end;
    }
    }

    if (pdu_session_create_data->vsmf_id) {
    if (cJSON_AddStringToObject(item, "vsmfId", pdu_session_create_data->vsmf_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [vsmf_id]");
        goto end;
    }
    }

    if (pdu_session_create_data->ismf_id) {
    if (cJSON_AddStringToObject(item, "ismfId", pdu_session_create_data->ismf_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [ismf_id]");
        goto end;
    }
    }

    if (!pdu_session_create_data->serving_network) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [serving_network]");
        return NULL;
    }
    cJSON *serving_network_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(pdu_session_create_data->serving_network);
    if (serving_network_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetwork", serving_network_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [serving_network]");
        goto end;
    }

    if (pdu_session_create_data->request_type != OpenAPI_request_type_NULL) {
    if (cJSON_AddStringToObject(item, "requestType", OpenAPI_request_type_ToString(pdu_session_create_data->request_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [request_type]");
        goto end;
    }
    }

    if (pdu_session_create_data->eps_bearer_id) {
    cJSON *eps_bearer_idList = cJSON_AddArrayToObject(item, "epsBearerId");
    if (eps_bearer_idList == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [eps_bearer_id]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_session_create_data->eps_bearer_id, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [eps_bearer_id]");
            goto end;
        }
        if (cJSON_AddNumberToObject(eps_bearer_idList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [eps_bearer_id]");
            goto end;
        }
    }
    }

    if (pdu_session_create_data->pgw_s8c_fteid) {
    if (cJSON_AddStringToObject(item, "pgwS8cFteid", pdu_session_create_data->pgw_s8c_fteid) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [pgw_s8c_fteid]");
        goto end;
    }
    }

    if (pdu_session_create_data->vsmf_pdu_session_uri) {
    if (cJSON_AddStringToObject(item, "vsmfPduSessionUri", pdu_session_create_data->vsmf_pdu_session_uri) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [vsmf_pdu_session_uri]");
        goto end;
    }
    }

    if (pdu_session_create_data->ismf_pdu_session_uri) {
    if (cJSON_AddStringToObject(item, "ismfPduSessionUri", pdu_session_create_data->ismf_pdu_session_uri) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [ismf_pdu_session_uri]");
        goto end;
    }
    }

    if (pdu_session_create_data->vcn_tunnel_info) {
    cJSON *vcn_tunnel_info_local_JSON = OpenAPI_tunnel_info_convertToJSON(pdu_session_create_data->vcn_tunnel_info);
    if (vcn_tunnel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [vcn_tunnel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vcnTunnelInfo", vcn_tunnel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [vcn_tunnel_info]");
        goto end;
    }
    }

    if (pdu_session_create_data->icn_tunnel_info) {
    cJSON *icn_tunnel_info_local_JSON = OpenAPI_tunnel_info_convertToJSON(pdu_session_create_data->icn_tunnel_info);
    if (icn_tunnel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [icn_tunnel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "icnTunnelInfo", icn_tunnel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [icn_tunnel_info]");
        goto end;
    }
    }

    if (pdu_session_create_data->n9_forwarding_tunnel_info) {
    cJSON *n9_forwarding_tunnel_info_local_JSON = OpenAPI_tunnel_info_convertToJSON(pdu_session_create_data->n9_forwarding_tunnel_info);
    if (n9_forwarding_tunnel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [n9_forwarding_tunnel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n9ForwardingTunnelInfo", n9_forwarding_tunnel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [n9_forwarding_tunnel_info]");
        goto end;
    }
    }

    if (pdu_session_create_data->additional_cn_tunnel_info) {
    cJSON *additional_cn_tunnel_info_local_JSON = OpenAPI_tunnel_info_convertToJSON(pdu_session_create_data->additional_cn_tunnel_info);
    if (additional_cn_tunnel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [additional_cn_tunnel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "additionalCnTunnelInfo", additional_cn_tunnel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [additional_cn_tunnel_info]");
        goto end;
    }
    }

    if (pdu_session_create_data->an_type == OpenAPI_access_type_NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [an_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "anType", OpenAPI_access_type_ToString(pdu_session_create_data->an_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [an_type]");
        goto end;
    }

    if (pdu_session_create_data->additional_an_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "additionalAnType", OpenAPI_access_type_ToString(pdu_session_create_data->additional_an_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [additional_an_type]");
        goto end;
    }
    }

    if (pdu_session_create_data->rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(pdu_session_create_data->rat_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (pdu_session_create_data->ue_location) {
    cJSON *ue_location_local_JSON = OpenAPI_user_location_convertToJSON(pdu_session_create_data->ue_location);
    if (ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLocation", ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    }

    if (pdu_session_create_data->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", pdu_session_create_data->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (pdu_session_create_data->add_ue_location) {
    cJSON *add_ue_location_local_JSON = OpenAPI_user_location_convertToJSON(pdu_session_create_data->add_ue_location);
    if (add_ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [add_ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "addUeLocation", add_ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [add_ue_location]");
        goto end;
    }
    }

    if (pdu_session_create_data->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", pdu_session_create_data->gpsi) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (pdu_session_create_data->n1_sm_info_from_ue) {
    cJSON *n1_sm_info_from_ue_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(pdu_session_create_data->n1_sm_info_from_ue);
    if (n1_sm_info_from_ue_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [n1_sm_info_from_ue]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n1SmInfoFromUe", n1_sm_info_from_ue_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [n1_sm_info_from_ue]");
        goto end;
    }
    }

    if (pdu_session_create_data->unknown_n1_sm_info) {
    cJSON *unknown_n1_sm_info_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(pdu_session_create_data->unknown_n1_sm_info);
    if (unknown_n1_sm_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [unknown_n1_sm_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "unknownN1SmInfo", unknown_n1_sm_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [unknown_n1_sm_info]");
        goto end;
    }
    }

    if (pdu_session_create_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", pdu_session_create_data->supported_features) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (pdu_session_create_data->h_pcf_id) {
    if (cJSON_AddStringToObject(item, "hPcfId", pdu_session_create_data->h_pcf_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [h_pcf_id]");
        goto end;
    }
    }

    if (pdu_session_create_data->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", pdu_session_create_data->pcf_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (pdu_session_create_data->pcf_group_id) {
    if (cJSON_AddStringToObject(item, "pcfGroupId", pdu_session_create_data->pcf_group_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [pcf_group_id]");
        goto end;
    }
    }

    if (pdu_session_create_data->pcf_set_id) {
    if (cJSON_AddStringToObject(item, "pcfSetId", pdu_session_create_data->pcf_set_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_ho_preparation_indication) {
    if (cJSON_AddBoolToObject(item, "hoPreparationIndication", pdu_session_create_data->ho_preparation_indication) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [ho_preparation_indication]");
        goto end;
    }
    }

    if (pdu_session_create_data->sel_mode != OpenAPI_dnn_selection_mode_NULL) {
    if (cJSON_AddStringToObject(item, "selMode", OpenAPI_dnn_selection_mode_ToString(pdu_session_create_data->sel_mode)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [sel_mode]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_always_on_requested) {
    if (cJSON_AddBoolToObject(item, "alwaysOnRequested", pdu_session_create_data->always_on_requested) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [always_on_requested]");
        goto end;
    }
    }

    if (pdu_session_create_data->udm_group_id) {
    if (cJSON_AddStringToObject(item, "udmGroupId", pdu_session_create_data->udm_group_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [udm_group_id]");
        goto end;
    }
    }

    if (pdu_session_create_data->routing_indicator) {
    if (cJSON_AddStringToObject(item, "routingIndicator", pdu_session_create_data->routing_indicator) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [routing_indicator]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_h_nw_pub_key_id) {
    if (cJSON_AddNumberToObject(item, "hNwPubKeyId", pdu_session_create_data->h_nw_pub_key_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [h_nw_pub_key_id]");
        goto end;
    }
    }

    if (pdu_session_create_data->eps_interworking_ind != OpenAPI_eps_interworking_indication_NULL) {
    if (cJSON_AddStringToObject(item, "epsInterworkingInd", OpenAPI_eps_interworking_indication_ToString(pdu_session_create_data->eps_interworking_ind)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [eps_interworking_ind]");
        goto end;
    }
    }

    if (pdu_session_create_data->v_smf_service_instance_id) {
    if (cJSON_AddStringToObject(item, "vSmfServiceInstanceId", pdu_session_create_data->v_smf_service_instance_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [v_smf_service_instance_id]");
        goto end;
    }
    }

    if (pdu_session_create_data->i_smf_service_instance_id) {
    if (cJSON_AddStringToObject(item, "iSmfServiceInstanceId", pdu_session_create_data->i_smf_service_instance_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [i_smf_service_instance_id]");
        goto end;
    }
    }

    if (pdu_session_create_data->recovery_time) {
    if (cJSON_AddStringToObject(item, "recoveryTime", pdu_session_create_data->recovery_time) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [recovery_time]");
        goto end;
    }
    }

    if (pdu_session_create_data->roaming_charging_profile) {
    cJSON *roaming_charging_profile_local_JSON = OpenAPI_roaming_charging_profile_convertToJSON(pdu_session_create_data->roaming_charging_profile);
    if (roaming_charging_profile_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [roaming_charging_profile]");
        goto end;
    }
    cJSON_AddItemToObject(item, "roamingChargingProfile", roaming_charging_profile_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [roaming_charging_profile]");
        goto end;
    }
    }

    if (pdu_session_create_data->charging_id) {
    if (cJSON_AddStringToObject(item, "chargingId", pdu_session_create_data->charging_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [charging_id]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_old_pdu_session_id) {
    if (cJSON_AddNumberToObject(item, "oldPduSessionId", pdu_session_create_data->old_pdu_session_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [old_pdu_session_id]");
        goto end;
    }
    }

    if (pdu_session_create_data->eps_bearer_ctx_status) {
    if (cJSON_AddStringToObject(item, "epsBearerCtxStatus", pdu_session_create_data->eps_bearer_ctx_status) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [eps_bearer_ctx_status]");
        goto end;
    }
    }

    if (pdu_session_create_data->amf_nf_id) {
    if (cJSON_AddStringToObject(item, "amfNfId", pdu_session_create_data->amf_nf_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [amf_nf_id]");
        goto end;
    }
    }

    if (pdu_session_create_data->guami) {
    cJSON *guami_local_JSON = OpenAPI_guami_convertToJSON(pdu_session_create_data->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [guami]");
        goto end;
    }
    }

    if (pdu_session_create_data->max_integrity_protected_data_rate_ul != OpenAPI_max_integrity_protected_data_rate_NULL) {
    if (cJSON_AddStringToObject(item, "maxIntegrityProtectedDataRateUl", OpenAPI_max_integrity_protected_data_rate_ToString(pdu_session_create_data->max_integrity_protected_data_rate_ul)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [max_integrity_protected_data_rate_ul]");
        goto end;
    }
    }

    if (pdu_session_create_data->max_integrity_protected_data_rate_dl != OpenAPI_max_integrity_protected_data_rate_NULL) {
    if (cJSON_AddStringToObject(item, "maxIntegrityProtectedDataRateDl", OpenAPI_max_integrity_protected_data_rate_ToString(pdu_session_create_data->max_integrity_protected_data_rate_dl)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [max_integrity_protected_data_rate_dl]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_cp_ciot_enabled) {
    if (cJSON_AddBoolToObject(item, "cpCiotEnabled", pdu_session_create_data->cp_ciot_enabled) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [cp_ciot_enabled]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_cp_only_ind) {
    if (cJSON_AddBoolToObject(item, "cpOnlyInd", pdu_session_create_data->cp_only_ind) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [cp_only_ind]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_invoke_nef) {
    if (cJSON_AddBoolToObject(item, "invokeNef", pdu_session_create_data->invoke_nef) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [invoke_nef]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_ma_request_ind) {
    if (cJSON_AddBoolToObject(item, "maRequestInd", pdu_session_create_data->ma_request_ind) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [ma_request_ind]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_ma_nw_upgrade_ind) {
    if (cJSON_AddBoolToObject(item, "maNwUpgradeInd", pdu_session_create_data->ma_nw_upgrade_ind) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [ma_nw_upgrade_ind]");
        goto end;
    }
    }

    if (pdu_session_create_data->dnai_list) {
    cJSON *dnai_listList = cJSON_AddArrayToObject(item, "dnaiList");
    if (dnai_listList == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [dnai_list]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_session_create_data->dnai_list, node) {
        if (cJSON_AddStringToObject(dnai_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [dnai_list]");
            goto end;
        }
    }
    }

    if (pdu_session_create_data->presence_in_ladn != OpenAPI_presence_state_NULL) {
    if (cJSON_AddStringToObject(item, "presenceInLadn", OpenAPI_presence_state_ToString(pdu_session_create_data->presence_in_ladn)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [presence_in_ladn]");
        goto end;
    }
    }

    if (pdu_session_create_data->secondary_rat_usage_info) {
    cJSON *secondary_rat_usage_infoList = cJSON_AddArrayToObject(item, "secondaryRatUsageInfo");
    if (secondary_rat_usage_infoList == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [secondary_rat_usage_info]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_session_create_data->secondary_rat_usage_info, node) {
        cJSON *itemLocal = OpenAPI_secondary_rat_usage_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [secondary_rat_usage_info]");
            goto end;
        }
        cJSON_AddItemToArray(secondary_rat_usage_infoList, itemLocal);
    }
    }

    if (pdu_session_create_data->small_data_rate_status) {
    cJSON *small_data_rate_status_local_JSON = OpenAPI_small_data_rate_status_convertToJSON(pdu_session_create_data->small_data_rate_status);
    if (small_data_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smallDataRateStatus", small_data_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    }

    if (pdu_session_create_data->apn_rate_status) {
    cJSON *apn_rate_status_local_JSON = OpenAPI_apn_rate_status_convertToJSON(pdu_session_create_data->apn_rate_status);
    if (apn_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "apnRateStatus", apn_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_dl_serving_plmn_rate_ctl) {
    if (cJSON_AddNumberToObject(item, "dlServingPlmnRateCtl", pdu_session_create_data->dl_serving_plmn_rate_ctl) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [dl_serving_plmn_rate_ctl]");
        goto end;
    }
    }

    if (pdu_session_create_data->up_security_info) {
    cJSON *up_security_info_local_JSON = OpenAPI_up_security_info_convertToJSON(pdu_session_create_data->up_security_info);
    if (up_security_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [up_security_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upSecurityInfo", up_security_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [up_security_info]");
        goto end;
    }
    }

    if (pdu_session_create_data->vplmn_qos) {
    cJSON *vplmn_qos_local_JSON = OpenAPI_vplmn_qos_convertToJSON(pdu_session_create_data->vplmn_qos);
    if (vplmn_qos_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [vplmn_qos]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vplmnQos", vplmn_qos_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [vplmn_qos]");
        goto end;
    }
    }

    if (pdu_session_create_data->old_sm_context_ref) {
    if (cJSON_AddStringToObject(item, "oldSmContextRef", pdu_session_create_data->old_sm_context_ref) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [old_sm_context_ref]");
        goto end;
    }
    }

    if (pdu_session_create_data->redundant_pdu_session_info) {
    cJSON *redundant_pdu_session_info_local_JSON = OpenAPI_redundant_pdu_session_information_convertToJSON(pdu_session_create_data->redundant_pdu_session_info);
    if (redundant_pdu_session_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [redundant_pdu_session_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "redundantPduSessionInfo", redundant_pdu_session_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [redundant_pdu_session_info]");
        goto end;
    }
    }

    if (pdu_session_create_data->old_pdu_session_ref) {
    if (cJSON_AddStringToObject(item, "oldPduSessionRef", pdu_session_create_data->old_pdu_session_ref) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [old_pdu_session_ref]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_sm_policy_notify_ind) {
    if (cJSON_AddBoolToObject(item, "smPolicyNotifyInd", pdu_session_create_data->sm_policy_notify_ind) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [sm_policy_notify_ind]");
        goto end;
    }
    }

    if (pdu_session_create_data->pcf_ue_callback_info) {
    cJSON *pcf_ue_callback_info_local_JSON = OpenAPI_pcf_ue_callback_info_convertToJSON(pdu_session_create_data->pcf_ue_callback_info);
    if (pcf_ue_callback_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [pcf_ue_callback_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pcfUeCallbackInfo", pcf_ue_callback_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [pcf_ue_callback_info]");
        goto end;
    }
    } else if (pdu_session_create_data->is_pcf_ue_callback_info_null) {
        if (cJSON_AddNullToObject(item, "pcfUeCallbackInfo") == NULL) {
            ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [pcf_ue_callback_info]");
            goto end;
        }
    }

    if (pdu_session_create_data->satellite_backhaul_cat != OpenAPI_satellite_backhaul_category_NULL) {
    if (cJSON_AddStringToObject(item, "satelliteBackhaulCat", OpenAPI_satellite_backhaul_category_ToString(pdu_session_create_data->satellite_backhaul_cat)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [satellite_backhaul_cat]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_upip_supported) {
    if (cJSON_AddBoolToObject(item, "upipSupported", pdu_session_create_data->upip_supported) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [upip_supported]");
        goto end;
    }
    }

    if (pdu_session_create_data->up_cnx_state != OpenAPI_up_cnx_state_NULL) {
    if (cJSON_AddStringToObject(item, "upCnxState", OpenAPI_up_cnx_state_ToString(pdu_session_create_data->up_cnx_state)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [up_cnx_state]");
        goto end;
    }
    }

    if (pdu_session_create_data->is_disaster_roaming_ind) {
    if (cJSON_AddBoolToObject(item, "disasterRoamingInd", pdu_session_create_data->disaster_roaming_ind) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [disaster_roaming_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_create_data_t *OpenAPI_pdu_session_create_data_parseFromJSON(cJSON *pdu_session_create_dataJSON)
{
    OpenAPI_pdu_session_create_data_t *pdu_session_create_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *unauthenticated_supi = NULL;
    cJSON *pei = NULL;
    cJSON *pdu_session_id = NULL;
    cJSON *dnn = NULL;
    cJSON *selected_dnn = NULL;
    cJSON *s_nssai = NULL;
    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    cJSON *hplmn_snssai = NULL;
    OpenAPI_snssai_t *hplmn_snssai_local_nonprim = NULL;
    cJSON *vsmf_id = NULL;
    cJSON *ismf_id = NULL;
    cJSON *serving_network = NULL;
    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    cJSON *request_type = NULL;
    OpenAPI_request_type_e request_typeVariable = 0;
    cJSON *eps_bearer_id = NULL;
    OpenAPI_list_t *eps_bearer_idList = NULL;
    cJSON *pgw_s8c_fteid = NULL;
    cJSON *vsmf_pdu_session_uri = NULL;
    cJSON *ismf_pdu_session_uri = NULL;
    cJSON *vcn_tunnel_info = NULL;
    OpenAPI_tunnel_info_t *vcn_tunnel_info_local_nonprim = NULL;
    cJSON *icn_tunnel_info = NULL;
    OpenAPI_tunnel_info_t *icn_tunnel_info_local_nonprim = NULL;
    cJSON *n9_forwarding_tunnel_info = NULL;
    OpenAPI_tunnel_info_t *n9_forwarding_tunnel_info_local_nonprim = NULL;
    cJSON *additional_cn_tunnel_info = NULL;
    OpenAPI_tunnel_info_t *additional_cn_tunnel_info_local_nonprim = NULL;
    cJSON *an_type = NULL;
    OpenAPI_access_type_e an_typeVariable = 0;
    cJSON *additional_an_type = NULL;
    OpenAPI_access_type_e additional_an_typeVariable = 0;
    cJSON *rat_type = NULL;
    OpenAPI_rat_type_e rat_typeVariable = 0;
    cJSON *ue_location = NULL;
    OpenAPI_user_location_t *ue_location_local_nonprim = NULL;
    cJSON *ue_time_zone = NULL;
    cJSON *add_ue_location = NULL;
    OpenAPI_user_location_t *add_ue_location_local_nonprim = NULL;
    cJSON *gpsi = NULL;
    cJSON *n1_sm_info_from_ue = NULL;
    OpenAPI_ref_to_binary_data_t *n1_sm_info_from_ue_local_nonprim = NULL;
    cJSON *unknown_n1_sm_info = NULL;
    OpenAPI_ref_to_binary_data_t *unknown_n1_sm_info_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    cJSON *h_pcf_id = NULL;
    cJSON *pcf_id = NULL;
    cJSON *pcf_group_id = NULL;
    cJSON *pcf_set_id = NULL;
    cJSON *ho_preparation_indication = NULL;
    cJSON *sel_mode = NULL;
    OpenAPI_dnn_selection_mode_e sel_modeVariable = 0;
    cJSON *always_on_requested = NULL;
    cJSON *udm_group_id = NULL;
    cJSON *routing_indicator = NULL;
    cJSON *h_nw_pub_key_id = NULL;
    cJSON *eps_interworking_ind = NULL;
    OpenAPI_eps_interworking_indication_e eps_interworking_indVariable = 0;
    cJSON *v_smf_service_instance_id = NULL;
    cJSON *i_smf_service_instance_id = NULL;
    cJSON *recovery_time = NULL;
    cJSON *roaming_charging_profile = NULL;
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile_local_nonprim = NULL;
    cJSON *charging_id = NULL;
    cJSON *old_pdu_session_id = NULL;
    cJSON *eps_bearer_ctx_status = NULL;
    cJSON *amf_nf_id = NULL;
    cJSON *guami = NULL;
    OpenAPI_guami_t *guami_local_nonprim = NULL;
    cJSON *max_integrity_protected_data_rate_ul = NULL;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ulVariable = 0;
    cJSON *max_integrity_protected_data_rate_dl = NULL;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dlVariable = 0;
    cJSON *cp_ciot_enabled = NULL;
    cJSON *cp_only_ind = NULL;
    cJSON *invoke_nef = NULL;
    cJSON *ma_request_ind = NULL;
    cJSON *ma_nw_upgrade_ind = NULL;
    cJSON *dnai_list = NULL;
    OpenAPI_list_t *dnai_listList = NULL;
    cJSON *presence_in_ladn = NULL;
    OpenAPI_presence_state_e presence_in_ladnVariable = 0;
    cJSON *secondary_rat_usage_info = NULL;
    OpenAPI_list_t *secondary_rat_usage_infoList = NULL;
    cJSON *small_data_rate_status = NULL;
    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    cJSON *apn_rate_status = NULL;
    OpenAPI_apn_rate_status_t *apn_rate_status_local_nonprim = NULL;
    cJSON *dl_serving_plmn_rate_ctl = NULL;
    cJSON *up_security_info = NULL;
    OpenAPI_up_security_info_t *up_security_info_local_nonprim = NULL;
    cJSON *vplmn_qos = NULL;
    OpenAPI_vplmn_qos_t *vplmn_qos_local_nonprim = NULL;
    cJSON *old_sm_context_ref = NULL;
    cJSON *redundant_pdu_session_info = NULL;
    OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_info_local_nonprim = NULL;
    cJSON *old_pdu_session_ref = NULL;
    cJSON *sm_policy_notify_ind = NULL;
    cJSON *pcf_ue_callback_info = NULL;
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info_local_nonprim = NULL;
    cJSON *satellite_backhaul_cat = NULL;
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_catVariable = 0;
    cJSON *upip_supported = NULL;
    cJSON *up_cnx_state = NULL;
    OpenAPI_up_cnx_state_e up_cnx_stateVariable = 0;
    cJSON *disaster_roaming_ind = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    unauthenticated_supi = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "unauthenticatedSupi");
    if (unauthenticated_supi) {
    if (!cJSON_IsBool(unauthenticated_supi)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [unauthenticated_supi]");
        goto end;
    }
    }

    pei = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    pdu_session_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "pduSessionId");
    if (pdu_session_id) {
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [dnn]");
        goto end;
    }

    selected_dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "selectedDnn");
    if (selected_dnn) {
    if (!cJSON_IsString(selected_dnn) && !cJSON_IsNull(selected_dnn)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [selected_dnn]");
        goto end;
    }
    }

    s_nssai = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "sNssai");
    if (s_nssai) {
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    if (!s_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [s_nssai]");
        goto end;
    }
    }

    hplmn_snssai = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "hplmnSnssai");
    if (hplmn_snssai) {
    hplmn_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(hplmn_snssai);
    if (!hplmn_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [hplmn_snssai]");
        goto end;
    }
    }

    vsmf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "vsmfId");
    if (vsmf_id) {
    if (!cJSON_IsString(vsmf_id) && !cJSON_IsNull(vsmf_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [vsmf_id]");
        goto end;
    }
    }

    ismf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "ismfId");
    if (ismf_id) {
    if (!cJSON_IsString(ismf_id) && !cJSON_IsNull(ismf_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [ismf_id]");
        goto end;
    }
    }

    serving_network = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "servingNetwork");
    if (!serving_network) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [serving_network]");
        goto end;
    }
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    if (!serving_network_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [serving_network]");
        goto end;
    }

    request_type = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "requestType");
    if (request_type) {
    if (!cJSON_IsString(request_type)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [request_type]");
        goto end;
    }
    request_typeVariable = OpenAPI_request_type_FromString(request_type->valuestring);
    }

    eps_bearer_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "epsBearerId");
    if (eps_bearer_id) {
        cJSON *eps_bearer_id_local = NULL;
        if (!cJSON_IsArray(eps_bearer_id)) {
            ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [eps_bearer_id]");
            goto end;
        }

        eps_bearer_idList = OpenAPI_list_create();

        cJSON_ArrayForEach(eps_bearer_id_local, eps_bearer_id) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(eps_bearer_id_local)) {
                ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [eps_bearer_id]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [eps_bearer_id]");
                goto end;
            }
            *localDouble = eps_bearer_id_local->valuedouble;
            OpenAPI_list_add(eps_bearer_idList, localDouble);
        }
    }

    pgw_s8c_fteid = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "pgwS8cFteid");
    if (pgw_s8c_fteid) {
    if (!cJSON_IsString(pgw_s8c_fteid) && !cJSON_IsNull(pgw_s8c_fteid)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [pgw_s8c_fteid]");
        goto end;
    }
    }

    vsmf_pdu_session_uri = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "vsmfPduSessionUri");
    if (vsmf_pdu_session_uri) {
    if (!cJSON_IsString(vsmf_pdu_session_uri) && !cJSON_IsNull(vsmf_pdu_session_uri)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [vsmf_pdu_session_uri]");
        goto end;
    }
    }

    ismf_pdu_session_uri = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "ismfPduSessionUri");
    if (ismf_pdu_session_uri) {
    if (!cJSON_IsString(ismf_pdu_session_uri) && !cJSON_IsNull(ismf_pdu_session_uri)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [ismf_pdu_session_uri]");
        goto end;
    }
    }

    vcn_tunnel_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "vcnTunnelInfo");
    if (vcn_tunnel_info) {
    vcn_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(vcn_tunnel_info);
    if (!vcn_tunnel_info_local_nonprim) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON failed [vcn_tunnel_info]");
        goto end;
    }
    }

    icn_tunnel_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "icnTunnelInfo");
    if (icn_tunnel_info) {
    icn_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(icn_tunnel_info);
    if (!icn_tunnel_info_local_nonprim) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON failed [icn_tunnel_info]");
        goto end;
    }
    }

    n9_forwarding_tunnel_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "n9ForwardingTunnelInfo");
    if (n9_forwarding_tunnel_info) {
    n9_forwarding_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(n9_forwarding_tunnel_info);
    if (!n9_forwarding_tunnel_info_local_nonprim) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON failed [n9_forwarding_tunnel_info]");
        goto end;
    }
    }

    additional_cn_tunnel_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "additionalCnTunnelInfo");
    if (additional_cn_tunnel_info) {
    additional_cn_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(additional_cn_tunnel_info);
    if (!additional_cn_tunnel_info_local_nonprim) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON failed [additional_cn_tunnel_info]");
        goto end;
    }
    }

    an_type = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "anType");
    if (!an_type) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [an_type]");
        goto end;
    }
    if (!cJSON_IsString(an_type)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [an_type]");
        goto end;
    }
    an_typeVariable = OpenAPI_access_type_FromString(an_type->valuestring);

    additional_an_type = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "additionalAnType");
    if (additional_an_type) {
    if (!cJSON_IsString(additional_an_type)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [additional_an_type]");
        goto end;
    }
    additional_an_typeVariable = OpenAPI_access_type_FromString(additional_an_type->valuestring);
    }

    rat_type = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "ratType");
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    ue_location = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "ueLocation");
    if (ue_location) {
    ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    if (!ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_location]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    add_ue_location = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "addUeLocation");
    if (add_ue_location) {
    add_ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(add_ue_location);
    if (!add_ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [add_ue_location]");
        goto end;
    }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    n1_sm_info_from_ue = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "n1SmInfoFromUe");
    if (n1_sm_info_from_ue) {
    n1_sm_info_from_ue_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_info_from_ue);
    if (!n1_sm_info_from_ue_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n1_sm_info_from_ue]");
        goto end;
    }
    }

    unknown_n1_sm_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "unknownN1SmInfo");
    if (unknown_n1_sm_info) {
    unknown_n1_sm_info_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(unknown_n1_sm_info);
    if (!unknown_n1_sm_info_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [unknown_n1_sm_info]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    h_pcf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "hPcfId");
    if (h_pcf_id) {
    if (!cJSON_IsString(h_pcf_id) && !cJSON_IsNull(h_pcf_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [h_pcf_id]");
        goto end;
    }
    }

    pcf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_group_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "pcfGroupId");
    if (pcf_group_id) {
    if (!cJSON_IsString(pcf_group_id) && !cJSON_IsNull(pcf_group_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [pcf_group_id]");
        goto end;
    }
    }

    pcf_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "pcfSetId");
    if (pcf_set_id) {
    if (!cJSON_IsString(pcf_set_id) && !cJSON_IsNull(pcf_set_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    ho_preparation_indication = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "hoPreparationIndication");
    if (ho_preparation_indication) {
    if (!cJSON_IsBool(ho_preparation_indication)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [ho_preparation_indication]");
        goto end;
    }
    }

    sel_mode = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "selMode");
    if (sel_mode) {
    if (!cJSON_IsString(sel_mode)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [sel_mode]");
        goto end;
    }
    sel_modeVariable = OpenAPI_dnn_selection_mode_FromString(sel_mode->valuestring);
    }

    always_on_requested = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "alwaysOnRequested");
    if (always_on_requested) {
    if (!cJSON_IsBool(always_on_requested)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [always_on_requested]");
        goto end;
    }
    }

    udm_group_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "udmGroupId");
    if (udm_group_id) {
    if (!cJSON_IsString(udm_group_id) && !cJSON_IsNull(udm_group_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [udm_group_id]");
        goto end;
    }
    }

    routing_indicator = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "routingIndicator");
    if (routing_indicator) {
    if (!cJSON_IsString(routing_indicator) && !cJSON_IsNull(routing_indicator)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [routing_indicator]");
        goto end;
    }
    }

    h_nw_pub_key_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "hNwPubKeyId");
    if (h_nw_pub_key_id) {
    if (!cJSON_IsNumber(h_nw_pub_key_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [h_nw_pub_key_id]");
        goto end;
    }
    }

    eps_interworking_ind = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "epsInterworkingInd");
    if (eps_interworking_ind) {
    if (!cJSON_IsString(eps_interworking_ind)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [eps_interworking_ind]");
        goto end;
    }
    eps_interworking_indVariable = OpenAPI_eps_interworking_indication_FromString(eps_interworking_ind->valuestring);
    }

    v_smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "vSmfServiceInstanceId");
    if (v_smf_service_instance_id) {
    if (!cJSON_IsString(v_smf_service_instance_id) && !cJSON_IsNull(v_smf_service_instance_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [v_smf_service_instance_id]");
        goto end;
    }
    }

    i_smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "iSmfServiceInstanceId");
    if (i_smf_service_instance_id) {
    if (!cJSON_IsString(i_smf_service_instance_id) && !cJSON_IsNull(i_smf_service_instance_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [i_smf_service_instance_id]");
        goto end;
    }
    }

    recovery_time = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "recoveryTime");
    if (recovery_time) {
    if (!cJSON_IsString(recovery_time) && !cJSON_IsNull(recovery_time)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [recovery_time]");
        goto end;
    }
    }

    roaming_charging_profile = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "roamingChargingProfile");
    if (roaming_charging_profile) {
    roaming_charging_profile_local_nonprim = OpenAPI_roaming_charging_profile_parseFromJSON(roaming_charging_profile);
    if (!roaming_charging_profile_local_nonprim) {
        ogs_error("OpenAPI_roaming_charging_profile_parseFromJSON failed [roaming_charging_profile]");
        goto end;
    }
    }

    charging_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "chargingId");
    if (charging_id) {
    if (!cJSON_IsString(charging_id) && !cJSON_IsNull(charging_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [charging_id]");
        goto end;
    }
    }

    old_pdu_session_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "oldPduSessionId");
    if (old_pdu_session_id) {
    if (!cJSON_IsNumber(old_pdu_session_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [old_pdu_session_id]");
        goto end;
    }
    }

    eps_bearer_ctx_status = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "epsBearerCtxStatus");
    if (eps_bearer_ctx_status) {
    if (!cJSON_IsString(eps_bearer_ctx_status) && !cJSON_IsNull(eps_bearer_ctx_status)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [eps_bearer_ctx_status]");
        goto end;
    }
    }

    amf_nf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "amfNfId");
    if (amf_nf_id) {
    if (!cJSON_IsString(amf_nf_id) && !cJSON_IsNull(amf_nf_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [amf_nf_id]");
        goto end;
    }
    }

    guami = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "guami");
    if (guami) {
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    if (!guami_local_nonprim) {
        ogs_error("OpenAPI_guami_parseFromJSON failed [guami]");
        goto end;
    }
    }

    max_integrity_protected_data_rate_ul = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "maxIntegrityProtectedDataRateUl");
    if (max_integrity_protected_data_rate_ul) {
    if (!cJSON_IsString(max_integrity_protected_data_rate_ul)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [max_integrity_protected_data_rate_ul]");
        goto end;
    }
    max_integrity_protected_data_rate_ulVariable = OpenAPI_max_integrity_protected_data_rate_FromString(max_integrity_protected_data_rate_ul->valuestring);
    }

    max_integrity_protected_data_rate_dl = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "maxIntegrityProtectedDataRateDl");
    if (max_integrity_protected_data_rate_dl) {
    if (!cJSON_IsString(max_integrity_protected_data_rate_dl)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [max_integrity_protected_data_rate_dl]");
        goto end;
    }
    max_integrity_protected_data_rate_dlVariable = OpenAPI_max_integrity_protected_data_rate_FromString(max_integrity_protected_data_rate_dl->valuestring);
    }

    cp_ciot_enabled = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "cpCiotEnabled");
    if (cp_ciot_enabled) {
    if (!cJSON_IsBool(cp_ciot_enabled)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [cp_ciot_enabled]");
        goto end;
    }
    }

    cp_only_ind = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "cpOnlyInd");
    if (cp_only_ind) {
    if (!cJSON_IsBool(cp_only_ind)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [cp_only_ind]");
        goto end;
    }
    }

    invoke_nef = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "invokeNef");
    if (invoke_nef) {
    if (!cJSON_IsBool(invoke_nef)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [invoke_nef]");
        goto end;
    }
    }

    ma_request_ind = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "maRequestInd");
    if (ma_request_ind) {
    if (!cJSON_IsBool(ma_request_ind)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [ma_request_ind]");
        goto end;
    }
    }

    ma_nw_upgrade_ind = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "maNwUpgradeInd");
    if (ma_nw_upgrade_ind) {
    if (!cJSON_IsBool(ma_nw_upgrade_ind)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [ma_nw_upgrade_ind]");
        goto end;
    }
    }

    dnai_list = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "dnaiList");
    if (dnai_list) {
        cJSON *dnai_list_local = NULL;
        if (!cJSON_IsArray(dnai_list)) {
            ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [dnai_list]");
            goto end;
        }

        dnai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnai_list_local, dnai_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnai_list_local)) {
                ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [dnai_list]");
                goto end;
            }
            OpenAPI_list_add(dnai_listList, ogs_strdup(dnai_list_local->valuestring));
        }
    }

    presence_in_ladn = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "presenceInLadn");
    if (presence_in_ladn) {
    if (!cJSON_IsString(presence_in_ladn)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [presence_in_ladn]");
        goto end;
    }
    presence_in_ladnVariable = OpenAPI_presence_state_FromString(presence_in_ladn->valuestring);
    }

    secondary_rat_usage_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "secondaryRatUsageInfo");
    if (secondary_rat_usage_info) {
        cJSON *secondary_rat_usage_info_local = NULL;
        if (!cJSON_IsArray(secondary_rat_usage_info)) {
            ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [secondary_rat_usage_info]");
            goto end;
        }

        secondary_rat_usage_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(secondary_rat_usage_info_local, secondary_rat_usage_info) {
            if (!cJSON_IsObject(secondary_rat_usage_info_local)) {
                ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [secondary_rat_usage_info]");
                goto end;
            }
            OpenAPI_secondary_rat_usage_info_t *secondary_rat_usage_infoItem = OpenAPI_secondary_rat_usage_info_parseFromJSON(secondary_rat_usage_info_local);
            if (!secondary_rat_usage_infoItem) {
                ogs_error("No secondary_rat_usage_infoItem");
                goto end;
            }
            OpenAPI_list_add(secondary_rat_usage_infoList, secondary_rat_usage_infoItem);
        }
    }

    small_data_rate_status = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "smallDataRateStatus");
    if (small_data_rate_status) {
    small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    if (!small_data_rate_status_local_nonprim) {
        ogs_error("OpenAPI_small_data_rate_status_parseFromJSON failed [small_data_rate_status]");
        goto end;
    }
    }

    apn_rate_status = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "apnRateStatus");
    if (apn_rate_status) {
    apn_rate_status_local_nonprim = OpenAPI_apn_rate_status_parseFromJSON(apn_rate_status);
    if (!apn_rate_status_local_nonprim) {
        ogs_error("OpenAPI_apn_rate_status_parseFromJSON failed [apn_rate_status]");
        goto end;
    }
    }

    dl_serving_plmn_rate_ctl = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "dlServingPlmnRateCtl");
    if (dl_serving_plmn_rate_ctl) {
    if (!cJSON_IsNumber(dl_serving_plmn_rate_ctl)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [dl_serving_plmn_rate_ctl]");
        goto end;
    }
    }

    up_security_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "upSecurityInfo");
    if (up_security_info) {
    up_security_info_local_nonprim = OpenAPI_up_security_info_parseFromJSON(up_security_info);
    if (!up_security_info_local_nonprim) {
        ogs_error("OpenAPI_up_security_info_parseFromJSON failed [up_security_info]");
        goto end;
    }
    }

    vplmn_qos = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "vplmnQos");
    if (vplmn_qos) {
    vplmn_qos_local_nonprim = OpenAPI_vplmn_qos_parseFromJSON(vplmn_qos);
    if (!vplmn_qos_local_nonprim) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON failed [vplmn_qos]");
        goto end;
    }
    }

    old_sm_context_ref = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "oldSmContextRef");
    if (old_sm_context_ref) {
    if (!cJSON_IsString(old_sm_context_ref) && !cJSON_IsNull(old_sm_context_ref)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [old_sm_context_ref]");
        goto end;
    }
    }

    redundant_pdu_session_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "redundantPduSessionInfo");
    if (redundant_pdu_session_info) {
    redundant_pdu_session_info_local_nonprim = OpenAPI_redundant_pdu_session_information_parseFromJSON(redundant_pdu_session_info);
    if (!redundant_pdu_session_info_local_nonprim) {
        ogs_error("OpenAPI_redundant_pdu_session_information_parseFromJSON failed [redundant_pdu_session_info]");
        goto end;
    }
    }

    old_pdu_session_ref = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "oldPduSessionRef");
    if (old_pdu_session_ref) {
    if (!cJSON_IsString(old_pdu_session_ref) && !cJSON_IsNull(old_pdu_session_ref)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [old_pdu_session_ref]");
        goto end;
    }
    }

    sm_policy_notify_ind = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "smPolicyNotifyInd");
    if (sm_policy_notify_ind) {
    if (!cJSON_IsBool(sm_policy_notify_ind)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [sm_policy_notify_ind]");
        goto end;
    }
    }

    pcf_ue_callback_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "pcfUeCallbackInfo");
    if (pcf_ue_callback_info) {
    if (!cJSON_IsNull(pcf_ue_callback_info)) {
    pcf_ue_callback_info_local_nonprim = OpenAPI_pcf_ue_callback_info_parseFromJSON(pcf_ue_callback_info);
    if (!pcf_ue_callback_info_local_nonprim) {
        ogs_error("OpenAPI_pcf_ue_callback_info_parseFromJSON failed [pcf_ue_callback_info]");
        goto end;
    }
    }
    }

    satellite_backhaul_cat = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "satelliteBackhaulCat");
    if (satellite_backhaul_cat) {
    if (!cJSON_IsString(satellite_backhaul_cat)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [satellite_backhaul_cat]");
        goto end;
    }
    satellite_backhaul_catVariable = OpenAPI_satellite_backhaul_category_FromString(satellite_backhaul_cat->valuestring);
    }

    upip_supported = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "upipSupported");
    if (upip_supported) {
    if (!cJSON_IsBool(upip_supported)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [upip_supported]");
        goto end;
    }
    }

    up_cnx_state = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "upCnxState");
    if (up_cnx_state) {
    if (!cJSON_IsString(up_cnx_state)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [up_cnx_state]");
        goto end;
    }
    up_cnx_stateVariable = OpenAPI_up_cnx_state_FromString(up_cnx_state->valuestring);
    }

    disaster_roaming_ind = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "disasterRoamingInd");
    if (disaster_roaming_ind) {
    if (!cJSON_IsBool(disaster_roaming_ind)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [disaster_roaming_ind]");
        goto end;
    }
    }

    pdu_session_create_data_local_var = OpenAPI_pdu_session_create_data_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        unauthenticated_supi ? true : false,
        unauthenticated_supi ? unauthenticated_supi->valueint : 0,
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        pdu_session_id ? true : false,
        pdu_session_id ? pdu_session_id->valuedouble : 0,
        ogs_strdup(dnn->valuestring),
        selected_dnn && !cJSON_IsNull(selected_dnn) ? ogs_strdup(selected_dnn->valuestring) : NULL,
        s_nssai ? s_nssai_local_nonprim : NULL,
        hplmn_snssai ? hplmn_snssai_local_nonprim : NULL,
        vsmf_id && !cJSON_IsNull(vsmf_id) ? ogs_strdup(vsmf_id->valuestring) : NULL,
        ismf_id && !cJSON_IsNull(ismf_id) ? ogs_strdup(ismf_id->valuestring) : NULL,
        serving_network_local_nonprim,
        request_type ? request_typeVariable : 0,
        eps_bearer_id ? eps_bearer_idList : NULL,
        pgw_s8c_fteid && !cJSON_IsNull(pgw_s8c_fteid) ? ogs_strdup(pgw_s8c_fteid->valuestring) : NULL,
        vsmf_pdu_session_uri && !cJSON_IsNull(vsmf_pdu_session_uri) ? ogs_strdup(vsmf_pdu_session_uri->valuestring) : NULL,
        ismf_pdu_session_uri && !cJSON_IsNull(ismf_pdu_session_uri) ? ogs_strdup(ismf_pdu_session_uri->valuestring) : NULL,
        vcn_tunnel_info ? vcn_tunnel_info_local_nonprim : NULL,
        icn_tunnel_info ? icn_tunnel_info_local_nonprim : NULL,
        n9_forwarding_tunnel_info ? n9_forwarding_tunnel_info_local_nonprim : NULL,
        additional_cn_tunnel_info ? additional_cn_tunnel_info_local_nonprim : NULL,
        an_typeVariable,
        additional_an_type ? additional_an_typeVariable : 0,
        rat_type ? rat_typeVariable : 0,
        ue_location ? ue_location_local_nonprim : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        add_ue_location ? add_ue_location_local_nonprim : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        n1_sm_info_from_ue ? n1_sm_info_from_ue_local_nonprim : NULL,
        unknown_n1_sm_info ? unknown_n1_sm_info_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        h_pcf_id && !cJSON_IsNull(h_pcf_id) ? ogs_strdup(h_pcf_id->valuestring) : NULL,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_group_id && !cJSON_IsNull(pcf_group_id) ? ogs_strdup(pcf_group_id->valuestring) : NULL,
        pcf_set_id && !cJSON_IsNull(pcf_set_id) ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        ho_preparation_indication ? true : false,
        ho_preparation_indication ? ho_preparation_indication->valueint : 0,
        sel_mode ? sel_modeVariable : 0,
        always_on_requested ? true : false,
        always_on_requested ? always_on_requested->valueint : 0,
        udm_group_id && !cJSON_IsNull(udm_group_id) ? ogs_strdup(udm_group_id->valuestring) : NULL,
        routing_indicator && !cJSON_IsNull(routing_indicator) ? ogs_strdup(routing_indicator->valuestring) : NULL,
        h_nw_pub_key_id ? true : false,
        h_nw_pub_key_id ? h_nw_pub_key_id->valuedouble : 0,
        eps_interworking_ind ? eps_interworking_indVariable : 0,
        v_smf_service_instance_id && !cJSON_IsNull(v_smf_service_instance_id) ? ogs_strdup(v_smf_service_instance_id->valuestring) : NULL,
        i_smf_service_instance_id && !cJSON_IsNull(i_smf_service_instance_id) ? ogs_strdup(i_smf_service_instance_id->valuestring) : NULL,
        recovery_time && !cJSON_IsNull(recovery_time) ? ogs_strdup(recovery_time->valuestring) : NULL,
        roaming_charging_profile ? roaming_charging_profile_local_nonprim : NULL,
        charging_id && !cJSON_IsNull(charging_id) ? ogs_strdup(charging_id->valuestring) : NULL,
        old_pdu_session_id ? true : false,
        old_pdu_session_id ? old_pdu_session_id->valuedouble : 0,
        eps_bearer_ctx_status && !cJSON_IsNull(eps_bearer_ctx_status) ? ogs_strdup(eps_bearer_ctx_status->valuestring) : NULL,
        amf_nf_id && !cJSON_IsNull(amf_nf_id) ? ogs_strdup(amf_nf_id->valuestring) : NULL,
        guami ? guami_local_nonprim : NULL,
        max_integrity_protected_data_rate_ul ? max_integrity_protected_data_rate_ulVariable : 0,
        max_integrity_protected_data_rate_dl ? max_integrity_protected_data_rate_dlVariable : 0,
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
        dnai_list ? dnai_listList : NULL,
        presence_in_ladn ? presence_in_ladnVariable : 0,
        secondary_rat_usage_info ? secondary_rat_usage_infoList : NULL,
        small_data_rate_status ? small_data_rate_status_local_nonprim : NULL,
        apn_rate_status ? apn_rate_status_local_nonprim : NULL,
        dl_serving_plmn_rate_ctl ? true : false,
        dl_serving_plmn_rate_ctl ? dl_serving_plmn_rate_ctl->valuedouble : 0,
        up_security_info ? up_security_info_local_nonprim : NULL,
        vplmn_qos ? vplmn_qos_local_nonprim : NULL,
        old_sm_context_ref && !cJSON_IsNull(old_sm_context_ref) ? ogs_strdup(old_sm_context_ref->valuestring) : NULL,
        redundant_pdu_session_info ? redundant_pdu_session_info_local_nonprim : NULL,
        old_pdu_session_ref && !cJSON_IsNull(old_pdu_session_ref) ? ogs_strdup(old_pdu_session_ref->valuestring) : NULL,
        sm_policy_notify_ind ? true : false,
        sm_policy_notify_ind ? sm_policy_notify_ind->valueint : 0,
        pcf_ue_callback_info && cJSON_IsNull(pcf_ue_callback_info) ? true : false,
        pcf_ue_callback_info ? pcf_ue_callback_info_local_nonprim : NULL,
        satellite_backhaul_cat ? satellite_backhaul_catVariable : 0,
        upip_supported ? true : false,
        upip_supported ? upip_supported->valueint : 0,
        up_cnx_state ? up_cnx_stateVariable : 0,
        disaster_roaming_ind ? true : false,
        disaster_roaming_ind ? disaster_roaming_ind->valueint : 0
    );

    return pdu_session_create_data_local_var;
end:
    if (s_nssai_local_nonprim) {
        OpenAPI_snssai_free(s_nssai_local_nonprim);
        s_nssai_local_nonprim = NULL;
    }
    if (hplmn_snssai_local_nonprim) {
        OpenAPI_snssai_free(hplmn_snssai_local_nonprim);
        hplmn_snssai_local_nonprim = NULL;
    }
    if (serving_network_local_nonprim) {
        OpenAPI_plmn_id_nid_free(serving_network_local_nonprim);
        serving_network_local_nonprim = NULL;
    }
    if (eps_bearer_idList) {
        OpenAPI_list_for_each(eps_bearer_idList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(eps_bearer_idList);
        eps_bearer_idList = NULL;
    }
    if (vcn_tunnel_info_local_nonprim) {
        OpenAPI_tunnel_info_free(vcn_tunnel_info_local_nonprim);
        vcn_tunnel_info_local_nonprim = NULL;
    }
    if (icn_tunnel_info_local_nonprim) {
        OpenAPI_tunnel_info_free(icn_tunnel_info_local_nonprim);
        icn_tunnel_info_local_nonprim = NULL;
    }
    if (n9_forwarding_tunnel_info_local_nonprim) {
        OpenAPI_tunnel_info_free(n9_forwarding_tunnel_info_local_nonprim);
        n9_forwarding_tunnel_info_local_nonprim = NULL;
    }
    if (additional_cn_tunnel_info_local_nonprim) {
        OpenAPI_tunnel_info_free(additional_cn_tunnel_info_local_nonprim);
        additional_cn_tunnel_info_local_nonprim = NULL;
    }
    if (ue_location_local_nonprim) {
        OpenAPI_user_location_free(ue_location_local_nonprim);
        ue_location_local_nonprim = NULL;
    }
    if (add_ue_location_local_nonprim) {
        OpenAPI_user_location_free(add_ue_location_local_nonprim);
        add_ue_location_local_nonprim = NULL;
    }
    if (n1_sm_info_from_ue_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(n1_sm_info_from_ue_local_nonprim);
        n1_sm_info_from_ue_local_nonprim = NULL;
    }
    if (unknown_n1_sm_info_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(unknown_n1_sm_info_local_nonprim);
        unknown_n1_sm_info_local_nonprim = NULL;
    }
    if (roaming_charging_profile_local_nonprim) {
        OpenAPI_roaming_charging_profile_free(roaming_charging_profile_local_nonprim);
        roaming_charging_profile_local_nonprim = NULL;
    }
    if (guami_local_nonprim) {
        OpenAPI_guami_free(guami_local_nonprim);
        guami_local_nonprim = NULL;
    }
    if (dnai_listList) {
        OpenAPI_list_for_each(dnai_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnai_listList);
        dnai_listList = NULL;
    }
    if (secondary_rat_usage_infoList) {
        OpenAPI_list_for_each(secondary_rat_usage_infoList, node) {
            OpenAPI_secondary_rat_usage_info_free(node->data);
        }
        OpenAPI_list_free(secondary_rat_usage_infoList);
        secondary_rat_usage_infoList = NULL;
    }
    if (small_data_rate_status_local_nonprim) {
        OpenAPI_small_data_rate_status_free(small_data_rate_status_local_nonprim);
        small_data_rate_status_local_nonprim = NULL;
    }
    if (apn_rate_status_local_nonprim) {
        OpenAPI_apn_rate_status_free(apn_rate_status_local_nonprim);
        apn_rate_status_local_nonprim = NULL;
    }
    if (up_security_info_local_nonprim) {
        OpenAPI_up_security_info_free(up_security_info_local_nonprim);
        up_security_info_local_nonprim = NULL;
    }
    if (vplmn_qos_local_nonprim) {
        OpenAPI_vplmn_qos_free(vplmn_qos_local_nonprim);
        vplmn_qos_local_nonprim = NULL;
    }
    if (redundant_pdu_session_info_local_nonprim) {
        OpenAPI_redundant_pdu_session_information_free(redundant_pdu_session_info_local_nonprim);
        redundant_pdu_session_info_local_nonprim = NULL;
    }
    if (pcf_ue_callback_info_local_nonprim) {
        OpenAPI_pcf_ue_callback_info_free(pcf_ue_callback_info_local_nonprim);
        pcf_ue_callback_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pdu_session_create_data_t *OpenAPI_pdu_session_create_data_copy(OpenAPI_pdu_session_create_data_t *dst, OpenAPI_pdu_session_create_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_create_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed");
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

    OpenAPI_pdu_session_create_data_free(dst);
    dst = OpenAPI_pdu_session_create_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

