
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
    bool is_ho_preparation_indication,
    int ho_preparation_indication,
    OpenAPI_dnn_selection_mode_e sel_mode,
    bool is_always_on_requested,
    int always_on_requested,
    char *udm_group_id,
    char *routing_indicator,
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
    OpenAPI_vplmn_qos_t *vplmn_qos
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

    return pdu_session_create_data_local_var;
}

void OpenAPI_pdu_session_create_data_free(OpenAPI_pdu_session_create_data_t *pdu_session_create_data)
{
    if (NULL == pdu_session_create_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pdu_session_create_data->supi);
    ogs_free(pdu_session_create_data->pei);
    ogs_free(pdu_session_create_data->dnn);
    ogs_free(pdu_session_create_data->selected_dnn);
    OpenAPI_snssai_free(pdu_session_create_data->s_nssai);
    ogs_free(pdu_session_create_data->vsmf_id);
    ogs_free(pdu_session_create_data->ismf_id);
    OpenAPI_plmn_id_nid_free(pdu_session_create_data->serving_network);
    OpenAPI_list_for_each(pdu_session_create_data->eps_bearer_id, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pdu_session_create_data->eps_bearer_id);
    ogs_free(pdu_session_create_data->vsmf_pdu_session_uri);
    ogs_free(pdu_session_create_data->ismf_pdu_session_uri);
    OpenAPI_tunnel_info_free(pdu_session_create_data->vcn_tunnel_info);
    OpenAPI_tunnel_info_free(pdu_session_create_data->icn_tunnel_info);
    OpenAPI_tunnel_info_free(pdu_session_create_data->n9_forwarding_tunnel_info);
    OpenAPI_tunnel_info_free(pdu_session_create_data->additional_cn_tunnel_info);
    OpenAPI_user_location_free(pdu_session_create_data->ue_location);
    ogs_free(pdu_session_create_data->ue_time_zone);
    OpenAPI_user_location_free(pdu_session_create_data->add_ue_location);
    ogs_free(pdu_session_create_data->gpsi);
    OpenAPI_ref_to_binary_data_free(pdu_session_create_data->n1_sm_info_from_ue);
    OpenAPI_ref_to_binary_data_free(pdu_session_create_data->unknown_n1_sm_info);
    ogs_free(pdu_session_create_data->supported_features);
    ogs_free(pdu_session_create_data->h_pcf_id);
    ogs_free(pdu_session_create_data->pcf_id);
    ogs_free(pdu_session_create_data->pcf_group_id);
    ogs_free(pdu_session_create_data->pcf_set_id);
    ogs_free(pdu_session_create_data->udm_group_id);
    ogs_free(pdu_session_create_data->routing_indicator);
    ogs_free(pdu_session_create_data->v_smf_service_instance_id);
    ogs_free(pdu_session_create_data->i_smf_service_instance_id);
    ogs_free(pdu_session_create_data->recovery_time);
    OpenAPI_roaming_charging_profile_free(pdu_session_create_data->roaming_charging_profile);
    ogs_free(pdu_session_create_data->charging_id);
    ogs_free(pdu_session_create_data->eps_bearer_ctx_status);
    ogs_free(pdu_session_create_data->amf_nf_id);
    OpenAPI_guami_free(pdu_session_create_data->guami);
    OpenAPI_list_for_each(pdu_session_create_data->dnai_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pdu_session_create_data->dnai_list);
    OpenAPI_list_for_each(pdu_session_create_data->secondary_rat_usage_info, node) {
        OpenAPI_secondary_rat_usage_info_free(node->data);
    }
    OpenAPI_list_free(pdu_session_create_data->secondary_rat_usage_info);
    OpenAPI_small_data_rate_status_free(pdu_session_create_data->small_data_rate_status);
    OpenAPI_apn_rate_status_free(pdu_session_create_data->apn_rate_status);
    OpenAPI_up_security_info_free(pdu_session_create_data->up_security_info);
    OpenAPI_vplmn_qos_free(pdu_session_create_data->vplmn_qos);
    ogs_free(pdu_session_create_data);
}

cJSON *OpenAPI_pdu_session_create_data_convertToJSON(OpenAPI_pdu_session_create_data_t *pdu_session_create_data)
{
    cJSON *item = NULL;

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

    if (pdu_session_create_data->request_type) {
    if (cJSON_AddStringToObject(item, "requestType", OpenAPI_request_type_ToString(pdu_session_create_data->request_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [request_type]");
        goto end;
    }
    }

    if (pdu_session_create_data->eps_bearer_id) {
    cJSON *eps_bearer_id = cJSON_AddArrayToObject(item, "epsBearerId");
    if (eps_bearer_id == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [eps_bearer_id]");
        goto end;
    }

    OpenAPI_lnode_t *eps_bearer_id_node;
    OpenAPI_list_for_each(pdu_session_create_data->eps_bearer_id, eps_bearer_id_node)  {
    if (cJSON_AddNumberToObject(eps_bearer_id, "", *(double *)eps_bearer_id_node->data) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [eps_bearer_id]");
        goto end;
    }
                    }
    }

    if (pdu_session_create_data->pgw_s8c_fteid) {
    if (cJSON_AddNumberToObject(item, "pgwS8cFteid", pdu_session_create_data->pgw_s8c_fteid) == NULL) {
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

    if (cJSON_AddStringToObject(item, "anType", OpenAPI_access_type_ToString(pdu_session_create_data->an_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [an_type]");
        goto end;
    }

    if (pdu_session_create_data->additional_an_type) {
    if (cJSON_AddStringToObject(item, "additionalAnType", OpenAPI_access_type_ToString(pdu_session_create_data->additional_an_type)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [additional_an_type]");
        goto end;
    }
    }

    if (pdu_session_create_data->rat_type) {
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

    if (pdu_session_create_data->sel_mode) {
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

    if (pdu_session_create_data->eps_interworking_ind) {
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

    if (pdu_session_create_data->max_integrity_protected_data_rate_ul) {
    if (cJSON_AddStringToObject(item, "maxIntegrityProtectedDataRateUl", OpenAPI_max_integrity_protected_data_rate_ToString(pdu_session_create_data->max_integrity_protected_data_rate_ul)) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [max_integrity_protected_data_rate_ul]");
        goto end;
    }
    }

    if (pdu_session_create_data->max_integrity_protected_data_rate_dl) {
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
    cJSON *dnai_list = cJSON_AddArrayToObject(item, "dnaiList");
    if (dnai_list == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [dnai_list]");
        goto end;
    }

    OpenAPI_lnode_t *dnai_list_node;
    OpenAPI_list_for_each(pdu_session_create_data->dnai_list, dnai_list_node)  {
    if (cJSON_AddStringToObject(dnai_list, "", (char*)dnai_list_node->data) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [dnai_list]");
        goto end;
    }
                    }
    }

    if (pdu_session_create_data->presence_in_ladn) {
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

    OpenAPI_lnode_t *secondary_rat_usage_info_node;
    if (pdu_session_create_data->secondary_rat_usage_info) {
        OpenAPI_list_for_each(pdu_session_create_data->secondary_rat_usage_info, secondary_rat_usage_info_node) {
            cJSON *itemLocal = OpenAPI_secondary_rat_usage_info_convertToJSON(secondary_rat_usage_info_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pdu_session_create_data_convertToJSON() failed [secondary_rat_usage_info]");
                goto end;
            }
            cJSON_AddItemToArray(secondary_rat_usage_infoList, itemLocal);
        }
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

end:
    return item;
}

OpenAPI_pdu_session_create_data_t *OpenAPI_pdu_session_create_data_parseFromJSON(cJSON *pdu_session_create_dataJSON)
{
    OpenAPI_pdu_session_create_data_t *pdu_session_create_data_local_var = NULL;
    cJSON *supi = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "supi");

    if (supi) {
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    cJSON *unauthenticated_supi = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "unauthenticatedSupi");

    if (unauthenticated_supi) {
    if (!cJSON_IsBool(unauthenticated_supi)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [unauthenticated_supi]");
        goto end;
    }
    }

    cJSON *pei = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "pei");

    if (pei) {
    if (!cJSON_IsString(pei)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    cJSON *pdu_session_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "pduSessionId");

    if (pdu_session_id) {
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [dnn]");
        goto end;
    }

    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *selected_dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "selectedDnn");

    if (selected_dnn) {
    if (!cJSON_IsString(selected_dnn)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [selected_dnn]");
        goto end;
    }
    }

    cJSON *s_nssai = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "sNssai");

    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    if (s_nssai) {
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    }

    cJSON *vsmf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "vsmfId");

    if (vsmf_id) {
    if (!cJSON_IsString(vsmf_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [vsmf_id]");
        goto end;
    }
    }

    cJSON *ismf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "ismfId");

    if (ismf_id) {
    if (!cJSON_IsString(ismf_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [ismf_id]");
        goto end;
    }
    }

    cJSON *serving_network = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "servingNetwork");
    if (!serving_network) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [serving_network]");
        goto end;
    }

    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);

    cJSON *request_type = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "requestType");

    OpenAPI_request_type_e request_typeVariable;
    if (request_type) {
    if (!cJSON_IsString(request_type)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [request_type]");
        goto end;
    }
    request_typeVariable = OpenAPI_request_type_FromString(request_type->valuestring);
    }

    cJSON *eps_bearer_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "epsBearerId");

    OpenAPI_list_t *eps_bearer_idList;
    if (eps_bearer_id) {
    cJSON *eps_bearer_id_local;
    if (!cJSON_IsArray(eps_bearer_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [eps_bearer_id]");
        goto end;
    }
    eps_bearer_idList = OpenAPI_list_create();

    cJSON_ArrayForEach(eps_bearer_id_local, eps_bearer_id) {
    if (!cJSON_IsNumber(eps_bearer_id_local)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [eps_bearer_id]");
        goto end;
    }
    OpenAPI_list_add(eps_bearer_idList , &eps_bearer_id_local->valuedouble);
    }
    }

    cJSON *pgw_s8c_fteid = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "pgwS8cFteid");

    if (pgw_s8c_fteid) {
    if (!cJSON_IsNumber(pgw_s8c_fteid)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [pgw_s8c_fteid]");
        goto end;
    }
    }

    cJSON *vsmf_pdu_session_uri = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "vsmfPduSessionUri");

    if (vsmf_pdu_session_uri) {
    if (!cJSON_IsString(vsmf_pdu_session_uri)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [vsmf_pdu_session_uri]");
        goto end;
    }
    }

    cJSON *ismf_pdu_session_uri = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "ismfPduSessionUri");

    if (ismf_pdu_session_uri) {
    if (!cJSON_IsString(ismf_pdu_session_uri)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [ismf_pdu_session_uri]");
        goto end;
    }
    }

    cJSON *vcn_tunnel_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "vcnTunnelInfo");

    OpenAPI_tunnel_info_t *vcn_tunnel_info_local_nonprim = NULL;
    if (vcn_tunnel_info) {
    vcn_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(vcn_tunnel_info);
    }

    cJSON *icn_tunnel_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "icnTunnelInfo");

    OpenAPI_tunnel_info_t *icn_tunnel_info_local_nonprim = NULL;
    if (icn_tunnel_info) {
    icn_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(icn_tunnel_info);
    }

    cJSON *n9_forwarding_tunnel_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "n9ForwardingTunnelInfo");

    OpenAPI_tunnel_info_t *n9_forwarding_tunnel_info_local_nonprim = NULL;
    if (n9_forwarding_tunnel_info) {
    n9_forwarding_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(n9_forwarding_tunnel_info);
    }

    cJSON *additional_cn_tunnel_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "additionalCnTunnelInfo");

    OpenAPI_tunnel_info_t *additional_cn_tunnel_info_local_nonprim = NULL;
    if (additional_cn_tunnel_info) {
    additional_cn_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(additional_cn_tunnel_info);
    }

    cJSON *an_type = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "anType");
    if (!an_type) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [an_type]");
        goto end;
    }

    OpenAPI_access_type_e an_typeVariable;
    if (!cJSON_IsString(an_type)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [an_type]");
        goto end;
    }
    an_typeVariable = OpenAPI_access_type_FromString(an_type->valuestring);

    cJSON *additional_an_type = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "additionalAnType");

    OpenAPI_access_type_e additional_an_typeVariable;
    if (additional_an_type) {
    if (!cJSON_IsString(additional_an_type)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [additional_an_type]");
        goto end;
    }
    additional_an_typeVariable = OpenAPI_access_type_FromString(additional_an_type->valuestring);
    }

    cJSON *rat_type = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "ratType");

    OpenAPI_rat_type_e rat_typeVariable;
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    cJSON *ue_location = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "ueLocation");

    OpenAPI_user_location_t *ue_location_local_nonprim = NULL;
    if (ue_location) {
    ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    }

    cJSON *ue_time_zone = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "ueTimeZone");

    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    cJSON *add_ue_location = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "addUeLocation");

    OpenAPI_user_location_t *add_ue_location_local_nonprim = NULL;
    if (add_ue_location) {
    add_ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(add_ue_location);
    }

    cJSON *gpsi = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "gpsi");

    if (gpsi) {
    if (!cJSON_IsString(gpsi)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    cJSON *n1_sm_info_from_ue = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "n1SmInfoFromUe");

    OpenAPI_ref_to_binary_data_t *n1_sm_info_from_ue_local_nonprim = NULL;
    if (n1_sm_info_from_ue) {
    n1_sm_info_from_ue_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_info_from_ue);
    }

    cJSON *unknown_n1_sm_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "unknownN1SmInfo");

    OpenAPI_ref_to_binary_data_t *unknown_n1_sm_info_local_nonprim = NULL;
    if (unknown_n1_sm_info) {
    unknown_n1_sm_info_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(unknown_n1_sm_info);
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *h_pcf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "hPcfId");

    if (h_pcf_id) {
    if (!cJSON_IsString(h_pcf_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [h_pcf_id]");
        goto end;
    }
    }

    cJSON *pcf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "pcfId");

    if (pcf_id) {
    if (!cJSON_IsString(pcf_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    cJSON *pcf_group_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "pcfGroupId");

    if (pcf_group_id) {
    if (!cJSON_IsString(pcf_group_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [pcf_group_id]");
        goto end;
    }
    }

    cJSON *pcf_set_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "pcfSetId");

    if (pcf_set_id) {
    if (!cJSON_IsString(pcf_set_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    cJSON *ho_preparation_indication = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "hoPreparationIndication");

    if (ho_preparation_indication) {
    if (!cJSON_IsBool(ho_preparation_indication)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [ho_preparation_indication]");
        goto end;
    }
    }

    cJSON *sel_mode = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "selMode");

    OpenAPI_dnn_selection_mode_e sel_modeVariable;
    if (sel_mode) {
    if (!cJSON_IsString(sel_mode)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [sel_mode]");
        goto end;
    }
    sel_modeVariable = OpenAPI_dnn_selection_mode_FromString(sel_mode->valuestring);
    }

    cJSON *always_on_requested = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "alwaysOnRequested");

    if (always_on_requested) {
    if (!cJSON_IsBool(always_on_requested)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [always_on_requested]");
        goto end;
    }
    }

    cJSON *udm_group_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "udmGroupId");

    if (udm_group_id) {
    if (!cJSON_IsString(udm_group_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [udm_group_id]");
        goto end;
    }
    }

    cJSON *routing_indicator = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "routingIndicator");

    if (routing_indicator) {
    if (!cJSON_IsString(routing_indicator)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [routing_indicator]");
        goto end;
    }
    }

    cJSON *eps_interworking_ind = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "epsInterworkingInd");

    OpenAPI_eps_interworking_indication_e eps_interworking_indVariable;
    if (eps_interworking_ind) {
    if (!cJSON_IsString(eps_interworking_ind)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [eps_interworking_ind]");
        goto end;
    }
    eps_interworking_indVariable = OpenAPI_eps_interworking_indication_FromString(eps_interworking_ind->valuestring);
    }

    cJSON *v_smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "vSmfServiceInstanceId");

    if (v_smf_service_instance_id) {
    if (!cJSON_IsString(v_smf_service_instance_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [v_smf_service_instance_id]");
        goto end;
    }
    }

    cJSON *i_smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "iSmfServiceInstanceId");

    if (i_smf_service_instance_id) {
    if (!cJSON_IsString(i_smf_service_instance_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [i_smf_service_instance_id]");
        goto end;
    }
    }

    cJSON *recovery_time = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "recoveryTime");

    if (recovery_time) {
    if (!cJSON_IsString(recovery_time)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [recovery_time]");
        goto end;
    }
    }

    cJSON *roaming_charging_profile = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "roamingChargingProfile");

    OpenAPI_roaming_charging_profile_t *roaming_charging_profile_local_nonprim = NULL;
    if (roaming_charging_profile) {
    roaming_charging_profile_local_nonprim = OpenAPI_roaming_charging_profile_parseFromJSON(roaming_charging_profile);
    }

    cJSON *charging_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "chargingId");

    if (charging_id) {
    if (!cJSON_IsString(charging_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [charging_id]");
        goto end;
    }
    }

    cJSON *old_pdu_session_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "oldPduSessionId");

    if (old_pdu_session_id) {
    if (!cJSON_IsNumber(old_pdu_session_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [old_pdu_session_id]");
        goto end;
    }
    }

    cJSON *eps_bearer_ctx_status = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "epsBearerCtxStatus");

    if (eps_bearer_ctx_status) {
    if (!cJSON_IsString(eps_bearer_ctx_status)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [eps_bearer_ctx_status]");
        goto end;
    }
    }

    cJSON *amf_nf_id = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "amfNfId");

    if (amf_nf_id) {
    if (!cJSON_IsString(amf_nf_id)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [amf_nf_id]");
        goto end;
    }
    }

    cJSON *guami = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "guami");

    OpenAPI_guami_t *guami_local_nonprim = NULL;
    if (guami) {
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    }

    cJSON *max_integrity_protected_data_rate_ul = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "maxIntegrityProtectedDataRateUl");

    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ulVariable;
    if (max_integrity_protected_data_rate_ul) {
    if (!cJSON_IsString(max_integrity_protected_data_rate_ul)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [max_integrity_protected_data_rate_ul]");
        goto end;
    }
    max_integrity_protected_data_rate_ulVariable = OpenAPI_max_integrity_protected_data_rate_FromString(max_integrity_protected_data_rate_ul->valuestring);
    }

    cJSON *max_integrity_protected_data_rate_dl = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "maxIntegrityProtectedDataRateDl");

    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dlVariable;
    if (max_integrity_protected_data_rate_dl) {
    if (!cJSON_IsString(max_integrity_protected_data_rate_dl)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [max_integrity_protected_data_rate_dl]");
        goto end;
    }
    max_integrity_protected_data_rate_dlVariable = OpenAPI_max_integrity_protected_data_rate_FromString(max_integrity_protected_data_rate_dl->valuestring);
    }

    cJSON *cp_ciot_enabled = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "cpCiotEnabled");

    if (cp_ciot_enabled) {
    if (!cJSON_IsBool(cp_ciot_enabled)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [cp_ciot_enabled]");
        goto end;
    }
    }

    cJSON *cp_only_ind = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "cpOnlyInd");

    if (cp_only_ind) {
    if (!cJSON_IsBool(cp_only_ind)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [cp_only_ind]");
        goto end;
    }
    }

    cJSON *invoke_nef = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "invokeNef");

    if (invoke_nef) {
    if (!cJSON_IsBool(invoke_nef)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [invoke_nef]");
        goto end;
    }
    }

    cJSON *ma_request_ind = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "maRequestInd");

    if (ma_request_ind) {
    if (!cJSON_IsBool(ma_request_ind)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [ma_request_ind]");
        goto end;
    }
    }

    cJSON *ma_nw_upgrade_ind = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "maNwUpgradeInd");

    if (ma_nw_upgrade_ind) {
    if (!cJSON_IsBool(ma_nw_upgrade_ind)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [ma_nw_upgrade_ind]");
        goto end;
    }
    }

    cJSON *dnai_list = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "dnaiList");

    OpenAPI_list_t *dnai_listList;
    if (dnai_list) {
    cJSON *dnai_list_local;
    if (!cJSON_IsArray(dnai_list)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [dnai_list]");
        goto end;
    }
    dnai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(dnai_list_local, dnai_list) {
    if (!cJSON_IsString(dnai_list_local)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [dnai_list]");
        goto end;
    }
    OpenAPI_list_add(dnai_listList , ogs_strdup(dnai_list_local->valuestring));
    }
    }

    cJSON *presence_in_ladn = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "presenceInLadn");

    OpenAPI_presence_state_e presence_in_ladnVariable;
    if (presence_in_ladn) {
    if (!cJSON_IsString(presence_in_ladn)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [presence_in_ladn]");
        goto end;
    }
    presence_in_ladnVariable = OpenAPI_presence_state_FromString(presence_in_ladn->valuestring);
    }

    cJSON *secondary_rat_usage_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "secondaryRatUsageInfo");

    OpenAPI_list_t *secondary_rat_usage_infoList;
    if (secondary_rat_usage_info) {
    cJSON *secondary_rat_usage_info_local_nonprimitive;
    if (!cJSON_IsArray(secondary_rat_usage_info)){
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [secondary_rat_usage_info]");
        goto end;
    }

    secondary_rat_usage_infoList = OpenAPI_list_create();

    cJSON_ArrayForEach(secondary_rat_usage_info_local_nonprimitive, secondary_rat_usage_info ) {
        if (!cJSON_IsObject(secondary_rat_usage_info_local_nonprimitive)) {
            ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [secondary_rat_usage_info]");
            goto end;
        }
        OpenAPI_secondary_rat_usage_info_t *secondary_rat_usage_infoItem = OpenAPI_secondary_rat_usage_info_parseFromJSON(secondary_rat_usage_info_local_nonprimitive);

        if (!secondary_rat_usage_infoItem) {
            ogs_error("No secondary_rat_usage_infoItem");
            OpenAPI_list_free(secondary_rat_usage_infoList);
            goto end;
        }

        OpenAPI_list_add(secondary_rat_usage_infoList, secondary_rat_usage_infoItem);
    }
    }

    cJSON *small_data_rate_status = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "smallDataRateStatus");

    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    if (small_data_rate_status) {
    small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    }

    cJSON *apn_rate_status = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "apnRateStatus");

    OpenAPI_apn_rate_status_t *apn_rate_status_local_nonprim = NULL;
    if (apn_rate_status) {
    apn_rate_status_local_nonprim = OpenAPI_apn_rate_status_parseFromJSON(apn_rate_status);
    }

    cJSON *dl_serving_plmn_rate_ctl = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "dlServingPlmnRateCtl");

    if (dl_serving_plmn_rate_ctl) {
    if (!cJSON_IsNumber(dl_serving_plmn_rate_ctl)) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON() failed [dl_serving_plmn_rate_ctl]");
        goto end;
    }
    }

    cJSON *up_security_info = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "upSecurityInfo");

    OpenAPI_up_security_info_t *up_security_info_local_nonprim = NULL;
    if (up_security_info) {
    up_security_info_local_nonprim = OpenAPI_up_security_info_parseFromJSON(up_security_info);
    }

    cJSON *vplmn_qos = cJSON_GetObjectItemCaseSensitive(pdu_session_create_dataJSON, "vplmnQos");

    OpenAPI_vplmn_qos_t *vplmn_qos_local_nonprim = NULL;
    if (vplmn_qos) {
    vplmn_qos_local_nonprim = OpenAPI_vplmn_qos_parseFromJSON(vplmn_qos);
    }

    pdu_session_create_data_local_var = OpenAPI_pdu_session_create_data_create (
        supi ? ogs_strdup(supi->valuestring) : NULL,
        unauthenticated_supi ? true : false,
        unauthenticated_supi ? unauthenticated_supi->valueint : 0,
        pei ? ogs_strdup(pei->valuestring) : NULL,
        pdu_session_id ? true : false,
        pdu_session_id ? pdu_session_id->valuedouble : 0,
        ogs_strdup(dnn->valuestring),
        selected_dnn ? ogs_strdup(selected_dnn->valuestring) : NULL,
        s_nssai ? s_nssai_local_nonprim : NULL,
        vsmf_id ? ogs_strdup(vsmf_id->valuestring) : NULL,
        ismf_id ? ogs_strdup(ismf_id->valuestring) : NULL,
        serving_network_local_nonprim,
        request_type ? request_typeVariable : 0,
        eps_bearer_id ? eps_bearer_idList : NULL,
        pgw_s8c_fteid ? pgw_s8c_fteid->valueint : 0,
        vsmf_pdu_session_uri ? ogs_strdup(vsmf_pdu_session_uri->valuestring) : NULL,
        ismf_pdu_session_uri ? ogs_strdup(ismf_pdu_session_uri->valuestring) : NULL,
        vcn_tunnel_info ? vcn_tunnel_info_local_nonprim : NULL,
        icn_tunnel_info ? icn_tunnel_info_local_nonprim : NULL,
        n9_forwarding_tunnel_info ? n9_forwarding_tunnel_info_local_nonprim : NULL,
        additional_cn_tunnel_info ? additional_cn_tunnel_info_local_nonprim : NULL,
        an_typeVariable,
        additional_an_type ? additional_an_typeVariable : 0,
        rat_type ? rat_typeVariable : 0,
        ue_location ? ue_location_local_nonprim : NULL,
        ue_time_zone ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        add_ue_location ? add_ue_location_local_nonprim : NULL,
        gpsi ? ogs_strdup(gpsi->valuestring) : NULL,
        n1_sm_info_from_ue ? n1_sm_info_from_ue_local_nonprim : NULL,
        unknown_n1_sm_info ? unknown_n1_sm_info_local_nonprim : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        h_pcf_id ? ogs_strdup(h_pcf_id->valuestring) : NULL,
        pcf_id ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_group_id ? ogs_strdup(pcf_group_id->valuestring) : NULL,
        pcf_set_id ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        ho_preparation_indication ? true : false,
        ho_preparation_indication ? ho_preparation_indication->valueint : 0,
        sel_mode ? sel_modeVariable : 0,
        always_on_requested ? true : false,
        always_on_requested ? always_on_requested->valueint : 0,
        udm_group_id ? ogs_strdup(udm_group_id->valuestring) : NULL,
        routing_indicator ? ogs_strdup(routing_indicator->valuestring) : NULL,
        eps_interworking_ind ? eps_interworking_indVariable : 0,
        v_smf_service_instance_id ? ogs_strdup(v_smf_service_instance_id->valuestring) : NULL,
        i_smf_service_instance_id ? ogs_strdup(i_smf_service_instance_id->valuestring) : NULL,
        recovery_time ? ogs_strdup(recovery_time->valuestring) : NULL,
        roaming_charging_profile ? roaming_charging_profile_local_nonprim : NULL,
        charging_id ? ogs_strdup(charging_id->valuestring) : NULL,
        old_pdu_session_id ? true : false,
        old_pdu_session_id ? old_pdu_session_id->valuedouble : 0,
        eps_bearer_ctx_status ? ogs_strdup(eps_bearer_ctx_status->valuestring) : NULL,
        amf_nf_id ? ogs_strdup(amf_nf_id->valuestring) : NULL,
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
        vplmn_qos ? vplmn_qos_local_nonprim : NULL
    );

    return pdu_session_create_data_local_var;
end:
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

