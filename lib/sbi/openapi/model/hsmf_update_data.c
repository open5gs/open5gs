
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hsmf_update_data.h"

char *OpenAPI_sm_policy_notify_indhsmf_update_data_ToString(OpenAPI_hsmf_update_data_sm_policy_notify_ind_e sm_policy_notify_ind)
{
    const char *sm_policy_notify_indArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(sm_policy_notify_indArray) / sizeof(sm_policy_notify_indArray[0]);
    if (sm_policy_notify_ind < sizeofArray)
        return (char *)sm_policy_notify_indArray[sm_policy_notify_ind];
    else
        return (char *)"Unknown";
}

OpenAPI_hsmf_update_data_sm_policy_notify_ind_e OpenAPI_sm_policy_notify_indhsmf_update_data_FromString(char* sm_policy_notify_ind)
{
    int stringToReturn = 0;
    const char *sm_policy_notify_indArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(sm_policy_notify_indArray) / sizeof(sm_policy_notify_indArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(sm_policy_notify_ind, sm_policy_notify_indArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}
OpenAPI_hsmf_update_data_t *OpenAPI_hsmf_update_data_create(
    OpenAPI_request_indication_e request_indication,
    char *pei,
    OpenAPI_tunnel_info_t *vcn_tunnel_info,
    OpenAPI_tunnel_info_t *icn_tunnel_info,
    OpenAPI_tunnel_info_t *additional_cn_tunnel_info,
    OpenAPI_plmn_id_nid_t *serving_network,
    OpenAPI_access_type_e an_type,
    OpenAPI_access_type_e additional_an_type,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone,
    OpenAPI_user_location_t *add_ue_location,
    bool is_pause_charging,
    int pause_charging,
    bool is_pti,
    int pti,
    OpenAPI_ref_to_binary_data_t *n1_sm_info_from_ue,
    OpenAPI_ref_to_binary_data_t *unknown_n1_sm_info,
    OpenAPI_list_t *qos_flows_rel_notify_list,
    OpenAPI_list_t *qos_flows_notify_list,
    OpenAPI_list_t *notify_list,
    OpenAPI_list_t *eps_bearer_id,
    bool is_ho_preparation_indication,
    int ho_preparation_indication,
    OpenAPI_list_t *revoke_ebi_list,
    OpenAPI_cause_e cause,
    OpenAPI_ng_ap_cause_t *ng_ap_cause,
    bool is__5g_mm_cause_value,
    int _5g_mm_cause_value,
    bool is_always_on_requested,
    int always_on_requested,
    OpenAPI_eps_interworking_indication_e eps_interworking_ind,
    OpenAPI_list_t *secondary_rat_usage_report,
    OpenAPI_list_t *secondary_rat_usage_info,
    bool is_an_type_can_be_changed,
    int an_type_can_be_changed,
    OpenAPI_ma_release_indication_e ma_release_ind,
    bool is_ma_nw_upgrade_ind,
    int ma_nw_upgrade_ind,
    bool is_ma_request_ind,
    int ma_request_ind,
    OpenAPI_unavailable_access_indication_e unavailable_access_ind,
    OpenAPI_list_t *psa_info,
    OpenAPI_ulcl_bp_information_t *ulcl_bp_info,
    OpenAPI_n4_information_t *n4_info,
    OpenAPI_n4_information_t *n4_info_ext1,
    OpenAPI_n4_information_t *n4_info_ext2,
    OpenAPI_presence_state_e presence_in_ladn,
    char *vsmf_pdu_session_uri,
    char *vsmf_id,
    char *v_smf_service_instance_id,
    char *ismf_pdu_session_uri,
    char *ismf_id,
    char *i_smf_service_instance_id,
    bool is_dl_serving_plmn_rate_ctl_null,
    bool is_dl_serving_plmn_rate_ctl,
    int dl_serving_plmn_rate_ctl,
    OpenAPI_list_t *dnai_list,
    char *supported_features,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile,
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter,
    OpenAPI_vplmn_qos_t *vplmn_qos,
    OpenAPI_security_result_t *security_result,
    OpenAPI_up_security_info_t *up_security_info,
    char *amf_nf_id,
    OpenAPI_guami_t *guami,
    OpenAPI_list_t *secondary_rat_usage_data_report_container,
    OpenAPI_hsmf_update_data_sm_policy_notify_ind_e sm_policy_notify_ind,
    bool is_pcf_ue_callback_info_null,
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info,
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_cat,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ul,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl,
    OpenAPI_up_cnx_state_e up_cnx_state
)
{
    OpenAPI_hsmf_update_data_t *hsmf_update_data_local_var = ogs_malloc(sizeof(OpenAPI_hsmf_update_data_t));
    ogs_assert(hsmf_update_data_local_var);

    hsmf_update_data_local_var->request_indication = request_indication;
    hsmf_update_data_local_var->pei = pei;
    hsmf_update_data_local_var->vcn_tunnel_info = vcn_tunnel_info;
    hsmf_update_data_local_var->icn_tunnel_info = icn_tunnel_info;
    hsmf_update_data_local_var->additional_cn_tunnel_info = additional_cn_tunnel_info;
    hsmf_update_data_local_var->serving_network = serving_network;
    hsmf_update_data_local_var->an_type = an_type;
    hsmf_update_data_local_var->additional_an_type = additional_an_type;
    hsmf_update_data_local_var->rat_type = rat_type;
    hsmf_update_data_local_var->ue_location = ue_location;
    hsmf_update_data_local_var->ue_time_zone = ue_time_zone;
    hsmf_update_data_local_var->add_ue_location = add_ue_location;
    hsmf_update_data_local_var->is_pause_charging = is_pause_charging;
    hsmf_update_data_local_var->pause_charging = pause_charging;
    hsmf_update_data_local_var->is_pti = is_pti;
    hsmf_update_data_local_var->pti = pti;
    hsmf_update_data_local_var->n1_sm_info_from_ue = n1_sm_info_from_ue;
    hsmf_update_data_local_var->unknown_n1_sm_info = unknown_n1_sm_info;
    hsmf_update_data_local_var->qos_flows_rel_notify_list = qos_flows_rel_notify_list;
    hsmf_update_data_local_var->qos_flows_notify_list = qos_flows_notify_list;
    hsmf_update_data_local_var->notify_list = notify_list;
    hsmf_update_data_local_var->eps_bearer_id = eps_bearer_id;
    hsmf_update_data_local_var->is_ho_preparation_indication = is_ho_preparation_indication;
    hsmf_update_data_local_var->ho_preparation_indication = ho_preparation_indication;
    hsmf_update_data_local_var->revoke_ebi_list = revoke_ebi_list;
    hsmf_update_data_local_var->cause = cause;
    hsmf_update_data_local_var->ng_ap_cause = ng_ap_cause;
    hsmf_update_data_local_var->is__5g_mm_cause_value = is__5g_mm_cause_value;
    hsmf_update_data_local_var->_5g_mm_cause_value = _5g_mm_cause_value;
    hsmf_update_data_local_var->is_always_on_requested = is_always_on_requested;
    hsmf_update_data_local_var->always_on_requested = always_on_requested;
    hsmf_update_data_local_var->eps_interworking_ind = eps_interworking_ind;
    hsmf_update_data_local_var->secondary_rat_usage_report = secondary_rat_usage_report;
    hsmf_update_data_local_var->secondary_rat_usage_info = secondary_rat_usage_info;
    hsmf_update_data_local_var->is_an_type_can_be_changed = is_an_type_can_be_changed;
    hsmf_update_data_local_var->an_type_can_be_changed = an_type_can_be_changed;
    hsmf_update_data_local_var->ma_release_ind = ma_release_ind;
    hsmf_update_data_local_var->is_ma_nw_upgrade_ind = is_ma_nw_upgrade_ind;
    hsmf_update_data_local_var->ma_nw_upgrade_ind = ma_nw_upgrade_ind;
    hsmf_update_data_local_var->is_ma_request_ind = is_ma_request_ind;
    hsmf_update_data_local_var->ma_request_ind = ma_request_ind;
    hsmf_update_data_local_var->unavailable_access_ind = unavailable_access_ind;
    hsmf_update_data_local_var->psa_info = psa_info;
    hsmf_update_data_local_var->ulcl_bp_info = ulcl_bp_info;
    hsmf_update_data_local_var->n4_info = n4_info;
    hsmf_update_data_local_var->n4_info_ext1 = n4_info_ext1;
    hsmf_update_data_local_var->n4_info_ext2 = n4_info_ext2;
    hsmf_update_data_local_var->presence_in_ladn = presence_in_ladn;
    hsmf_update_data_local_var->vsmf_pdu_session_uri = vsmf_pdu_session_uri;
    hsmf_update_data_local_var->vsmf_id = vsmf_id;
    hsmf_update_data_local_var->v_smf_service_instance_id = v_smf_service_instance_id;
    hsmf_update_data_local_var->ismf_pdu_session_uri = ismf_pdu_session_uri;
    hsmf_update_data_local_var->ismf_id = ismf_id;
    hsmf_update_data_local_var->i_smf_service_instance_id = i_smf_service_instance_id;
    hsmf_update_data_local_var->is_dl_serving_plmn_rate_ctl_null = is_dl_serving_plmn_rate_ctl_null;
    hsmf_update_data_local_var->is_dl_serving_plmn_rate_ctl = is_dl_serving_plmn_rate_ctl;
    hsmf_update_data_local_var->dl_serving_plmn_rate_ctl = dl_serving_plmn_rate_ctl;
    hsmf_update_data_local_var->dnai_list = dnai_list;
    hsmf_update_data_local_var->supported_features = supported_features;
    hsmf_update_data_local_var->roaming_charging_profile = roaming_charging_profile;
    hsmf_update_data_local_var->mo_exp_data_counter = mo_exp_data_counter;
    hsmf_update_data_local_var->vplmn_qos = vplmn_qos;
    hsmf_update_data_local_var->security_result = security_result;
    hsmf_update_data_local_var->up_security_info = up_security_info;
    hsmf_update_data_local_var->amf_nf_id = amf_nf_id;
    hsmf_update_data_local_var->guami = guami;
    hsmf_update_data_local_var->secondary_rat_usage_data_report_container = secondary_rat_usage_data_report_container;
    hsmf_update_data_local_var->sm_policy_notify_ind = sm_policy_notify_ind;
    hsmf_update_data_local_var->is_pcf_ue_callback_info_null = is_pcf_ue_callback_info_null;
    hsmf_update_data_local_var->pcf_ue_callback_info = pcf_ue_callback_info;
    hsmf_update_data_local_var->satellite_backhaul_cat = satellite_backhaul_cat;
    hsmf_update_data_local_var->max_integrity_protected_data_rate_ul = max_integrity_protected_data_rate_ul;
    hsmf_update_data_local_var->max_integrity_protected_data_rate_dl = max_integrity_protected_data_rate_dl;
    hsmf_update_data_local_var->up_cnx_state = up_cnx_state;

    return hsmf_update_data_local_var;
}

void OpenAPI_hsmf_update_data_free(OpenAPI_hsmf_update_data_t *hsmf_update_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == hsmf_update_data) {
        return;
    }
    if (hsmf_update_data->pei) {
        ogs_free(hsmf_update_data->pei);
        hsmf_update_data->pei = NULL;
    }
    if (hsmf_update_data->vcn_tunnel_info) {
        OpenAPI_tunnel_info_free(hsmf_update_data->vcn_tunnel_info);
        hsmf_update_data->vcn_tunnel_info = NULL;
    }
    if (hsmf_update_data->icn_tunnel_info) {
        OpenAPI_tunnel_info_free(hsmf_update_data->icn_tunnel_info);
        hsmf_update_data->icn_tunnel_info = NULL;
    }
    if (hsmf_update_data->additional_cn_tunnel_info) {
        OpenAPI_tunnel_info_free(hsmf_update_data->additional_cn_tunnel_info);
        hsmf_update_data->additional_cn_tunnel_info = NULL;
    }
    if (hsmf_update_data->serving_network) {
        OpenAPI_plmn_id_nid_free(hsmf_update_data->serving_network);
        hsmf_update_data->serving_network = NULL;
    }
    if (hsmf_update_data->ue_location) {
        OpenAPI_user_location_free(hsmf_update_data->ue_location);
        hsmf_update_data->ue_location = NULL;
    }
    if (hsmf_update_data->ue_time_zone) {
        ogs_free(hsmf_update_data->ue_time_zone);
        hsmf_update_data->ue_time_zone = NULL;
    }
    if (hsmf_update_data->add_ue_location) {
        OpenAPI_user_location_free(hsmf_update_data->add_ue_location);
        hsmf_update_data->add_ue_location = NULL;
    }
    if (hsmf_update_data->n1_sm_info_from_ue) {
        OpenAPI_ref_to_binary_data_free(hsmf_update_data->n1_sm_info_from_ue);
        hsmf_update_data->n1_sm_info_from_ue = NULL;
    }
    if (hsmf_update_data->unknown_n1_sm_info) {
        OpenAPI_ref_to_binary_data_free(hsmf_update_data->unknown_n1_sm_info);
        hsmf_update_data->unknown_n1_sm_info = NULL;
    }
    if (hsmf_update_data->qos_flows_rel_notify_list) {
        OpenAPI_list_for_each(hsmf_update_data->qos_flows_rel_notify_list, node) {
            OpenAPI_qos_flow_item_free(node->data);
        }
        OpenAPI_list_free(hsmf_update_data->qos_flows_rel_notify_list);
        hsmf_update_data->qos_flows_rel_notify_list = NULL;
    }
    if (hsmf_update_data->qos_flows_notify_list) {
        OpenAPI_list_for_each(hsmf_update_data->qos_flows_notify_list, node) {
            OpenAPI_qos_flow_notify_item_free(node->data);
        }
        OpenAPI_list_free(hsmf_update_data->qos_flows_notify_list);
        hsmf_update_data->qos_flows_notify_list = NULL;
    }
    if (hsmf_update_data->notify_list) {
        OpenAPI_list_for_each(hsmf_update_data->notify_list, node) {
            OpenAPI_pdu_session_notify_item_free(node->data);
        }
        OpenAPI_list_free(hsmf_update_data->notify_list);
        hsmf_update_data->notify_list = NULL;
    }
    if (hsmf_update_data->eps_bearer_id) {
        OpenAPI_list_for_each(hsmf_update_data->eps_bearer_id, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(hsmf_update_data->eps_bearer_id);
        hsmf_update_data->eps_bearer_id = NULL;
    }
    if (hsmf_update_data->revoke_ebi_list) {
        OpenAPI_list_for_each(hsmf_update_data->revoke_ebi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(hsmf_update_data->revoke_ebi_list);
        hsmf_update_data->revoke_ebi_list = NULL;
    }
    if (hsmf_update_data->ng_ap_cause) {
        OpenAPI_ng_ap_cause_free(hsmf_update_data->ng_ap_cause);
        hsmf_update_data->ng_ap_cause = NULL;
    }
    if (hsmf_update_data->secondary_rat_usage_report) {
        OpenAPI_list_for_each(hsmf_update_data->secondary_rat_usage_report, node) {
            OpenAPI_secondary_rat_usage_report_free(node->data);
        }
        OpenAPI_list_free(hsmf_update_data->secondary_rat_usage_report);
        hsmf_update_data->secondary_rat_usage_report = NULL;
    }
    if (hsmf_update_data->secondary_rat_usage_info) {
        OpenAPI_list_for_each(hsmf_update_data->secondary_rat_usage_info, node) {
            OpenAPI_secondary_rat_usage_info_free(node->data);
        }
        OpenAPI_list_free(hsmf_update_data->secondary_rat_usage_info);
        hsmf_update_data->secondary_rat_usage_info = NULL;
    }
    if (hsmf_update_data->psa_info) {
        OpenAPI_list_for_each(hsmf_update_data->psa_info, node) {
            OpenAPI_psa_information_free(node->data);
        }
        OpenAPI_list_free(hsmf_update_data->psa_info);
        hsmf_update_data->psa_info = NULL;
    }
    if (hsmf_update_data->ulcl_bp_info) {
        OpenAPI_ulcl_bp_information_free(hsmf_update_data->ulcl_bp_info);
        hsmf_update_data->ulcl_bp_info = NULL;
    }
    if (hsmf_update_data->n4_info) {
        OpenAPI_n4_information_free(hsmf_update_data->n4_info);
        hsmf_update_data->n4_info = NULL;
    }
    if (hsmf_update_data->n4_info_ext1) {
        OpenAPI_n4_information_free(hsmf_update_data->n4_info_ext1);
        hsmf_update_data->n4_info_ext1 = NULL;
    }
    if (hsmf_update_data->n4_info_ext2) {
        OpenAPI_n4_information_free(hsmf_update_data->n4_info_ext2);
        hsmf_update_data->n4_info_ext2 = NULL;
    }
    if (hsmf_update_data->vsmf_pdu_session_uri) {
        ogs_free(hsmf_update_data->vsmf_pdu_session_uri);
        hsmf_update_data->vsmf_pdu_session_uri = NULL;
    }
    if (hsmf_update_data->vsmf_id) {
        ogs_free(hsmf_update_data->vsmf_id);
        hsmf_update_data->vsmf_id = NULL;
    }
    if (hsmf_update_data->v_smf_service_instance_id) {
        ogs_free(hsmf_update_data->v_smf_service_instance_id);
        hsmf_update_data->v_smf_service_instance_id = NULL;
    }
    if (hsmf_update_data->ismf_pdu_session_uri) {
        ogs_free(hsmf_update_data->ismf_pdu_session_uri);
        hsmf_update_data->ismf_pdu_session_uri = NULL;
    }
    if (hsmf_update_data->ismf_id) {
        ogs_free(hsmf_update_data->ismf_id);
        hsmf_update_data->ismf_id = NULL;
    }
    if (hsmf_update_data->i_smf_service_instance_id) {
        ogs_free(hsmf_update_data->i_smf_service_instance_id);
        hsmf_update_data->i_smf_service_instance_id = NULL;
    }
    if (hsmf_update_data->dnai_list) {
        OpenAPI_list_for_each(hsmf_update_data->dnai_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(hsmf_update_data->dnai_list);
        hsmf_update_data->dnai_list = NULL;
    }
    if (hsmf_update_data->supported_features) {
        ogs_free(hsmf_update_data->supported_features);
        hsmf_update_data->supported_features = NULL;
    }
    if (hsmf_update_data->roaming_charging_profile) {
        OpenAPI_roaming_charging_profile_free(hsmf_update_data->roaming_charging_profile);
        hsmf_update_data->roaming_charging_profile = NULL;
    }
    if (hsmf_update_data->mo_exp_data_counter) {
        OpenAPI_mo_exp_data_counter_free(hsmf_update_data->mo_exp_data_counter);
        hsmf_update_data->mo_exp_data_counter = NULL;
    }
    if (hsmf_update_data->vplmn_qos) {
        OpenAPI_vplmn_qos_free(hsmf_update_data->vplmn_qos);
        hsmf_update_data->vplmn_qos = NULL;
    }
    if (hsmf_update_data->security_result) {
        OpenAPI_security_result_free(hsmf_update_data->security_result);
        hsmf_update_data->security_result = NULL;
    }
    if (hsmf_update_data->up_security_info) {
        OpenAPI_up_security_info_free(hsmf_update_data->up_security_info);
        hsmf_update_data->up_security_info = NULL;
    }
    if (hsmf_update_data->amf_nf_id) {
        ogs_free(hsmf_update_data->amf_nf_id);
        hsmf_update_data->amf_nf_id = NULL;
    }
    if (hsmf_update_data->guami) {
        OpenAPI_guami_free(hsmf_update_data->guami);
        hsmf_update_data->guami = NULL;
    }
    if (hsmf_update_data->secondary_rat_usage_data_report_container) {
        OpenAPI_list_for_each(hsmf_update_data->secondary_rat_usage_data_report_container, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(hsmf_update_data->secondary_rat_usage_data_report_container);
        hsmf_update_data->secondary_rat_usage_data_report_container = NULL;
    }
    if (hsmf_update_data->pcf_ue_callback_info) {
        OpenAPI_pcf_ue_callback_info_free(hsmf_update_data->pcf_ue_callback_info);
        hsmf_update_data->pcf_ue_callback_info = NULL;
    }
    ogs_free(hsmf_update_data);
}

cJSON *OpenAPI_hsmf_update_data_convertToJSON(OpenAPI_hsmf_update_data_t *hsmf_update_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (hsmf_update_data == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [HsmfUpdateData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (hsmf_update_data->request_indication == OpenAPI_request_indication_NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [request_indication]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "requestIndication", OpenAPI_request_indication_ToString(hsmf_update_data->request_indication)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [request_indication]");
        goto end;
    }

    if (hsmf_update_data->pei) {
    if (cJSON_AddStringToObject(item, "pei", hsmf_update_data->pei) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (hsmf_update_data->vcn_tunnel_info) {
    cJSON *vcn_tunnel_info_local_JSON = OpenAPI_tunnel_info_convertToJSON(hsmf_update_data->vcn_tunnel_info);
    if (vcn_tunnel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [vcn_tunnel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vcnTunnelInfo", vcn_tunnel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [vcn_tunnel_info]");
        goto end;
    }
    }

    if (hsmf_update_data->icn_tunnel_info) {
    cJSON *icn_tunnel_info_local_JSON = OpenAPI_tunnel_info_convertToJSON(hsmf_update_data->icn_tunnel_info);
    if (icn_tunnel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [icn_tunnel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "icnTunnelInfo", icn_tunnel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [icn_tunnel_info]");
        goto end;
    }
    }

    if (hsmf_update_data->additional_cn_tunnel_info) {
    cJSON *additional_cn_tunnel_info_local_JSON = OpenAPI_tunnel_info_convertToJSON(hsmf_update_data->additional_cn_tunnel_info);
    if (additional_cn_tunnel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [additional_cn_tunnel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "additionalCnTunnelInfo", additional_cn_tunnel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [additional_cn_tunnel_info]");
        goto end;
    }
    }

    if (hsmf_update_data->serving_network) {
    cJSON *serving_network_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(hsmf_update_data->serving_network);
    if (serving_network_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetwork", serving_network_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    }

    if (hsmf_update_data->an_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "anType", OpenAPI_access_type_ToString(hsmf_update_data->an_type)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [an_type]");
        goto end;
    }
    }

    if (hsmf_update_data->additional_an_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "additionalAnType", OpenAPI_access_type_ToString(hsmf_update_data->additional_an_type)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [additional_an_type]");
        goto end;
    }
    }

    if (hsmf_update_data->rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(hsmf_update_data->rat_type)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (hsmf_update_data->ue_location) {
    cJSON *ue_location_local_JSON = OpenAPI_user_location_convertToJSON(hsmf_update_data->ue_location);
    if (ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLocation", ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    }

    if (hsmf_update_data->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", hsmf_update_data->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (hsmf_update_data->add_ue_location) {
    cJSON *add_ue_location_local_JSON = OpenAPI_user_location_convertToJSON(hsmf_update_data->add_ue_location);
    if (add_ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [add_ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "addUeLocation", add_ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [add_ue_location]");
        goto end;
    }
    }

    if (hsmf_update_data->is_pause_charging) {
    if (cJSON_AddBoolToObject(item, "pauseCharging", hsmf_update_data->pause_charging) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [pause_charging]");
        goto end;
    }
    }

    if (hsmf_update_data->is_pti) {
    if (cJSON_AddNumberToObject(item, "pti", hsmf_update_data->pti) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [pti]");
        goto end;
    }
    }

    if (hsmf_update_data->n1_sm_info_from_ue) {
    cJSON *n1_sm_info_from_ue_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(hsmf_update_data->n1_sm_info_from_ue);
    if (n1_sm_info_from_ue_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [n1_sm_info_from_ue]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n1SmInfoFromUe", n1_sm_info_from_ue_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [n1_sm_info_from_ue]");
        goto end;
    }
    }

    if (hsmf_update_data->unknown_n1_sm_info) {
    cJSON *unknown_n1_sm_info_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(hsmf_update_data->unknown_n1_sm_info);
    if (unknown_n1_sm_info_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [unknown_n1_sm_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "unknownN1SmInfo", unknown_n1_sm_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [unknown_n1_sm_info]");
        goto end;
    }
    }

    if (hsmf_update_data->qos_flows_rel_notify_list) {
    cJSON *qos_flows_rel_notify_listList = cJSON_AddArrayToObject(item, "qosFlowsRelNotifyList");
    if (qos_flows_rel_notify_listList == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [qos_flows_rel_notify_list]");
        goto end;
    }
    OpenAPI_list_for_each(hsmf_update_data->qos_flows_rel_notify_list, node) {
        cJSON *itemLocal = OpenAPI_qos_flow_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [qos_flows_rel_notify_list]");
            goto end;
        }
        cJSON_AddItemToArray(qos_flows_rel_notify_listList, itemLocal);
    }
    }

    if (hsmf_update_data->qos_flows_notify_list) {
    cJSON *qos_flows_notify_listList = cJSON_AddArrayToObject(item, "qosFlowsNotifyList");
    if (qos_flows_notify_listList == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [qos_flows_notify_list]");
        goto end;
    }
    OpenAPI_list_for_each(hsmf_update_data->qos_flows_notify_list, node) {
        cJSON *itemLocal = OpenAPI_qos_flow_notify_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [qos_flows_notify_list]");
            goto end;
        }
        cJSON_AddItemToArray(qos_flows_notify_listList, itemLocal);
    }
    }

    if (hsmf_update_data->notify_list) {
    cJSON *notify_listList = cJSON_AddArrayToObject(item, "NotifyList");
    if (notify_listList == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [notify_list]");
        goto end;
    }
    OpenAPI_list_for_each(hsmf_update_data->notify_list, node) {
        cJSON *itemLocal = OpenAPI_pdu_session_notify_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [notify_list]");
            goto end;
        }
        cJSON_AddItemToArray(notify_listList, itemLocal);
    }
    }

    if (hsmf_update_data->eps_bearer_id) {
    cJSON *eps_bearer_idList = cJSON_AddArrayToObject(item, "epsBearerId");
    if (eps_bearer_idList == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [eps_bearer_id]");
        goto end;
    }
    OpenAPI_list_for_each(hsmf_update_data->eps_bearer_id, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [eps_bearer_id]");
            goto end;
        }
        if (cJSON_AddNumberToObject(eps_bearer_idList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [eps_bearer_id]");
            goto end;
        }
    }
    }

    if (hsmf_update_data->is_ho_preparation_indication) {
    if (cJSON_AddBoolToObject(item, "hoPreparationIndication", hsmf_update_data->ho_preparation_indication) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ho_preparation_indication]");
        goto end;
    }
    }

    if (hsmf_update_data->revoke_ebi_list) {
    cJSON *revoke_ebi_listList = cJSON_AddArrayToObject(item, "revokeEbiList");
    if (revoke_ebi_listList == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [revoke_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(hsmf_update_data->revoke_ebi_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [revoke_ebi_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(revoke_ebi_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [revoke_ebi_list]");
            goto end;
        }
    }
    }

    if (hsmf_update_data->cause != OpenAPI_cause_NULL) {
    if (cJSON_AddStringToObject(item, "cause", OpenAPI_cause_ToString(hsmf_update_data->cause)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [cause]");
        goto end;
    }
    }

    if (hsmf_update_data->ng_ap_cause) {
    cJSON *ng_ap_cause_local_JSON = OpenAPI_ng_ap_cause_convertToJSON(hsmf_update_data->ng_ap_cause);
    if (ng_ap_cause_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ng_ap_cause]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ngApCause", ng_ap_cause_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ng_ap_cause]");
        goto end;
    }
    }

    if (hsmf_update_data->is__5g_mm_cause_value) {
    if (cJSON_AddNumberToObject(item, "5gMmCauseValue", hsmf_update_data->_5g_mm_cause_value) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [_5g_mm_cause_value]");
        goto end;
    }
    }

    if (hsmf_update_data->is_always_on_requested) {
    if (cJSON_AddBoolToObject(item, "alwaysOnRequested", hsmf_update_data->always_on_requested) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [always_on_requested]");
        goto end;
    }
    }

    if (hsmf_update_data->eps_interworking_ind != OpenAPI_eps_interworking_indication_NULL) {
    if (cJSON_AddStringToObject(item, "epsInterworkingInd", OpenAPI_eps_interworking_indication_ToString(hsmf_update_data->eps_interworking_ind)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [eps_interworking_ind]");
        goto end;
    }
    }

    if (hsmf_update_data->secondary_rat_usage_report) {
    cJSON *secondary_rat_usage_reportList = cJSON_AddArrayToObject(item, "secondaryRatUsageReport");
    if (secondary_rat_usage_reportList == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [secondary_rat_usage_report]");
        goto end;
    }
    OpenAPI_list_for_each(hsmf_update_data->secondary_rat_usage_report, node) {
        cJSON *itemLocal = OpenAPI_secondary_rat_usage_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [secondary_rat_usage_report]");
            goto end;
        }
        cJSON_AddItemToArray(secondary_rat_usage_reportList, itemLocal);
    }
    }

    if (hsmf_update_data->secondary_rat_usage_info) {
    cJSON *secondary_rat_usage_infoList = cJSON_AddArrayToObject(item, "secondaryRatUsageInfo");
    if (secondary_rat_usage_infoList == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [secondary_rat_usage_info]");
        goto end;
    }
    OpenAPI_list_for_each(hsmf_update_data->secondary_rat_usage_info, node) {
        cJSON *itemLocal = OpenAPI_secondary_rat_usage_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [secondary_rat_usage_info]");
            goto end;
        }
        cJSON_AddItemToArray(secondary_rat_usage_infoList, itemLocal);
    }
    }

    if (hsmf_update_data->is_an_type_can_be_changed) {
    if (cJSON_AddBoolToObject(item, "anTypeCanBeChanged", hsmf_update_data->an_type_can_be_changed) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [an_type_can_be_changed]");
        goto end;
    }
    }

    if (hsmf_update_data->ma_release_ind != OpenAPI_ma_release_indication_NULL) {
    if (cJSON_AddStringToObject(item, "maReleaseInd", OpenAPI_ma_release_indication_ToString(hsmf_update_data->ma_release_ind)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ma_release_ind]");
        goto end;
    }
    }

    if (hsmf_update_data->is_ma_nw_upgrade_ind) {
    if (cJSON_AddBoolToObject(item, "maNwUpgradeInd", hsmf_update_data->ma_nw_upgrade_ind) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ma_nw_upgrade_ind]");
        goto end;
    }
    }

    if (hsmf_update_data->is_ma_request_ind) {
    if (cJSON_AddBoolToObject(item, "maRequestInd", hsmf_update_data->ma_request_ind) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ma_request_ind]");
        goto end;
    }
    }

    if (hsmf_update_data->unavailable_access_ind != OpenAPI_unavailable_access_indication_NULL) {
    if (cJSON_AddStringToObject(item, "unavailableAccessInd", OpenAPI_unavailable_access_indication_ToString(hsmf_update_data->unavailable_access_ind)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [unavailable_access_ind]");
        goto end;
    }
    }

    if (hsmf_update_data->psa_info) {
    cJSON *psa_infoList = cJSON_AddArrayToObject(item, "psaInfo");
    if (psa_infoList == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [psa_info]");
        goto end;
    }
    OpenAPI_list_for_each(hsmf_update_data->psa_info, node) {
        cJSON *itemLocal = OpenAPI_psa_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [psa_info]");
            goto end;
        }
        cJSON_AddItemToArray(psa_infoList, itemLocal);
    }
    }

    if (hsmf_update_data->ulcl_bp_info) {
    cJSON *ulcl_bp_info_local_JSON = OpenAPI_ulcl_bp_information_convertToJSON(hsmf_update_data->ulcl_bp_info);
    if (ulcl_bp_info_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ulcl_bp_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ulclBpInfo", ulcl_bp_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ulcl_bp_info]");
        goto end;
    }
    }

    if (hsmf_update_data->n4_info) {
    cJSON *n4_info_local_JSON = OpenAPI_n4_information_convertToJSON(hsmf_update_data->n4_info);
    if (n4_info_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [n4_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n4Info", n4_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [n4_info]");
        goto end;
    }
    }

    if (hsmf_update_data->n4_info_ext1) {
    cJSON *n4_info_ext1_local_JSON = OpenAPI_n4_information_convertToJSON(hsmf_update_data->n4_info_ext1);
    if (n4_info_ext1_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [n4_info_ext1]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n4InfoExt1", n4_info_ext1_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [n4_info_ext1]");
        goto end;
    }
    }

    if (hsmf_update_data->n4_info_ext2) {
    cJSON *n4_info_ext2_local_JSON = OpenAPI_n4_information_convertToJSON(hsmf_update_data->n4_info_ext2);
    if (n4_info_ext2_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [n4_info_ext2]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n4InfoExt2", n4_info_ext2_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [n4_info_ext2]");
        goto end;
    }
    }

    if (hsmf_update_data->presence_in_ladn != OpenAPI_presence_state_NULL) {
    if (cJSON_AddStringToObject(item, "presenceInLadn", OpenAPI_presence_state_ToString(hsmf_update_data->presence_in_ladn)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [presence_in_ladn]");
        goto end;
    }
    }

    if (hsmf_update_data->vsmf_pdu_session_uri) {
    if (cJSON_AddStringToObject(item, "vsmfPduSessionUri", hsmf_update_data->vsmf_pdu_session_uri) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [vsmf_pdu_session_uri]");
        goto end;
    }
    }

    if (hsmf_update_data->vsmf_id) {
    if (cJSON_AddStringToObject(item, "vsmfId", hsmf_update_data->vsmf_id) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [vsmf_id]");
        goto end;
    }
    }

    if (hsmf_update_data->v_smf_service_instance_id) {
    if (cJSON_AddStringToObject(item, "vSmfServiceInstanceId", hsmf_update_data->v_smf_service_instance_id) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [v_smf_service_instance_id]");
        goto end;
    }
    }

    if (hsmf_update_data->ismf_pdu_session_uri) {
    if (cJSON_AddStringToObject(item, "ismfPduSessionUri", hsmf_update_data->ismf_pdu_session_uri) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ismf_pdu_session_uri]");
        goto end;
    }
    }

    if (hsmf_update_data->ismf_id) {
    if (cJSON_AddStringToObject(item, "ismfId", hsmf_update_data->ismf_id) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ismf_id]");
        goto end;
    }
    }

    if (hsmf_update_data->i_smf_service_instance_id) {
    if (cJSON_AddStringToObject(item, "iSmfServiceInstanceId", hsmf_update_data->i_smf_service_instance_id) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [i_smf_service_instance_id]");
        goto end;
    }
    }

    if (hsmf_update_data->is_dl_serving_plmn_rate_ctl) {
    if (cJSON_AddNumberToObject(item, "dlServingPlmnRateCtl", hsmf_update_data->dl_serving_plmn_rate_ctl) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [dl_serving_plmn_rate_ctl]");
        goto end;
    }
    } else if (hsmf_update_data->is_dl_serving_plmn_rate_ctl_null) {
        if (cJSON_AddNullToObject(item, "dlServingPlmnRateCtl") == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [dl_serving_plmn_rate_ctl]");
            goto end;
        }
    }

    if (hsmf_update_data->dnai_list) {
    cJSON *dnai_listList = cJSON_AddArrayToObject(item, "dnaiList");
    if (dnai_listList == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [dnai_list]");
        goto end;
    }
    OpenAPI_list_for_each(hsmf_update_data->dnai_list, node) {
        if (cJSON_AddStringToObject(dnai_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [dnai_list]");
            goto end;
        }
    }
    }

    if (hsmf_update_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", hsmf_update_data->supported_features) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (hsmf_update_data->roaming_charging_profile) {
    cJSON *roaming_charging_profile_local_JSON = OpenAPI_roaming_charging_profile_convertToJSON(hsmf_update_data->roaming_charging_profile);
    if (roaming_charging_profile_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [roaming_charging_profile]");
        goto end;
    }
    cJSON_AddItemToObject(item, "roamingChargingProfile", roaming_charging_profile_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [roaming_charging_profile]");
        goto end;
    }
    }

    if (hsmf_update_data->mo_exp_data_counter) {
    cJSON *mo_exp_data_counter_local_JSON = OpenAPI_mo_exp_data_counter_convertToJSON(hsmf_update_data->mo_exp_data_counter);
    if (mo_exp_data_counter_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [mo_exp_data_counter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "moExpDataCounter", mo_exp_data_counter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [mo_exp_data_counter]");
        goto end;
    }
    }

    if (hsmf_update_data->vplmn_qos) {
    cJSON *vplmn_qos_local_JSON = OpenAPI_vplmn_qos_convertToJSON(hsmf_update_data->vplmn_qos);
    if (vplmn_qos_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [vplmn_qos]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vplmnQos", vplmn_qos_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [vplmn_qos]");
        goto end;
    }
    }

    if (hsmf_update_data->security_result) {
    cJSON *security_result_local_JSON = OpenAPI_security_result_convertToJSON(hsmf_update_data->security_result);
    if (security_result_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [security_result]");
        goto end;
    }
    cJSON_AddItemToObject(item, "securityResult", security_result_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [security_result]");
        goto end;
    }
    }

    if (hsmf_update_data->up_security_info) {
    cJSON *up_security_info_local_JSON = OpenAPI_up_security_info_convertToJSON(hsmf_update_data->up_security_info);
    if (up_security_info_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [up_security_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upSecurityInfo", up_security_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [up_security_info]");
        goto end;
    }
    }

    if (hsmf_update_data->amf_nf_id) {
    if (cJSON_AddStringToObject(item, "amfNfId", hsmf_update_data->amf_nf_id) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [amf_nf_id]");
        goto end;
    }
    }

    if (hsmf_update_data->guami) {
    cJSON *guami_local_JSON = OpenAPI_guami_convertToJSON(hsmf_update_data->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [guami]");
        goto end;
    }
    }

    if (hsmf_update_data->secondary_rat_usage_data_report_container) {
    cJSON *secondary_rat_usage_data_report_containerList = cJSON_AddArrayToObject(item, "secondaryRatUsageDataReportContainer");
    if (secondary_rat_usage_data_report_containerList == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [secondary_rat_usage_data_report_container]");
        goto end;
    }
    OpenAPI_list_for_each(hsmf_update_data->secondary_rat_usage_data_report_container, node) {
        if (cJSON_AddStringToObject(secondary_rat_usage_data_report_containerList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [secondary_rat_usage_data_report_container]");
            goto end;
        }
    }
    }

    if (hsmf_update_data->sm_policy_notify_ind != OpenAPI_hsmf_update_data_SMPOLICYNOTIFYIND_NULL) {
    if (cJSON_AddStringToObject(item, "smPolicyNotifyInd", OpenAPI_sm_policy_notify_indhsmf_update_data_ToString(hsmf_update_data->sm_policy_notify_ind)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [sm_policy_notify_ind]");
        goto end;
    }
    }

    if (hsmf_update_data->pcf_ue_callback_info) {
    cJSON *pcf_ue_callback_info_local_JSON = OpenAPI_pcf_ue_callback_info_convertToJSON(hsmf_update_data->pcf_ue_callback_info);
    if (pcf_ue_callback_info_local_JSON == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [pcf_ue_callback_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pcfUeCallbackInfo", pcf_ue_callback_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [pcf_ue_callback_info]");
        goto end;
    }
    } else if (hsmf_update_data->is_pcf_ue_callback_info_null) {
        if (cJSON_AddNullToObject(item, "pcfUeCallbackInfo") == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [pcf_ue_callback_info]");
            goto end;
        }
    }

    if (hsmf_update_data->satellite_backhaul_cat != OpenAPI_satellite_backhaul_category_NULL) {
    if (cJSON_AddStringToObject(item, "satelliteBackhaulCat", OpenAPI_satellite_backhaul_category_ToString(hsmf_update_data->satellite_backhaul_cat)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [satellite_backhaul_cat]");
        goto end;
    }
    }

    if (hsmf_update_data->max_integrity_protected_data_rate_ul != OpenAPI_max_integrity_protected_data_rate_NULL) {
    if (cJSON_AddStringToObject(item, "maxIntegrityProtectedDataRateUl", OpenAPI_max_integrity_protected_data_rate_ToString(hsmf_update_data->max_integrity_protected_data_rate_ul)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [max_integrity_protected_data_rate_ul]");
        goto end;
    }
    }

    if (hsmf_update_data->max_integrity_protected_data_rate_dl != OpenAPI_max_integrity_protected_data_rate_NULL) {
    if (cJSON_AddStringToObject(item, "maxIntegrityProtectedDataRateDl", OpenAPI_max_integrity_protected_data_rate_ToString(hsmf_update_data->max_integrity_protected_data_rate_dl)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [max_integrity_protected_data_rate_dl]");
        goto end;
    }
    }

    if (hsmf_update_data->up_cnx_state != OpenAPI_up_cnx_state_NULL) {
    if (cJSON_AddStringToObject(item, "upCnxState", OpenAPI_up_cnx_state_ToString(hsmf_update_data->up_cnx_state)) == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [up_cnx_state]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_hsmf_update_data_t *OpenAPI_hsmf_update_data_parseFromJSON(cJSON *hsmf_update_dataJSON)
{
    OpenAPI_hsmf_update_data_t *hsmf_update_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *request_indication = NULL;
    OpenAPI_request_indication_e request_indicationVariable = 0;
    cJSON *pei = NULL;
    cJSON *vcn_tunnel_info = NULL;
    OpenAPI_tunnel_info_t *vcn_tunnel_info_local_nonprim = NULL;
    cJSON *icn_tunnel_info = NULL;
    OpenAPI_tunnel_info_t *icn_tunnel_info_local_nonprim = NULL;
    cJSON *additional_cn_tunnel_info = NULL;
    OpenAPI_tunnel_info_t *additional_cn_tunnel_info_local_nonprim = NULL;
    cJSON *serving_network = NULL;
    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
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
    cJSON *pause_charging = NULL;
    cJSON *pti = NULL;
    cJSON *n1_sm_info_from_ue = NULL;
    OpenAPI_ref_to_binary_data_t *n1_sm_info_from_ue_local_nonprim = NULL;
    cJSON *unknown_n1_sm_info = NULL;
    OpenAPI_ref_to_binary_data_t *unknown_n1_sm_info_local_nonprim = NULL;
    cJSON *qos_flows_rel_notify_list = NULL;
    OpenAPI_list_t *qos_flows_rel_notify_listList = NULL;
    cJSON *qos_flows_notify_list = NULL;
    OpenAPI_list_t *qos_flows_notify_listList = NULL;
    cJSON *notify_list = NULL;
    OpenAPI_list_t *notify_listList = NULL;
    cJSON *eps_bearer_id = NULL;
    OpenAPI_list_t *eps_bearer_idList = NULL;
    cJSON *ho_preparation_indication = NULL;
    cJSON *revoke_ebi_list = NULL;
    OpenAPI_list_t *revoke_ebi_listList = NULL;
    cJSON *cause = NULL;
    OpenAPI_cause_e causeVariable = 0;
    cJSON *ng_ap_cause = NULL;
    OpenAPI_ng_ap_cause_t *ng_ap_cause_local_nonprim = NULL;
    cJSON *_5g_mm_cause_value = NULL;
    cJSON *always_on_requested = NULL;
    cJSON *eps_interworking_ind = NULL;
    OpenAPI_eps_interworking_indication_e eps_interworking_indVariable = 0;
    cJSON *secondary_rat_usage_report = NULL;
    OpenAPI_list_t *secondary_rat_usage_reportList = NULL;
    cJSON *secondary_rat_usage_info = NULL;
    OpenAPI_list_t *secondary_rat_usage_infoList = NULL;
    cJSON *an_type_can_be_changed = NULL;
    cJSON *ma_release_ind = NULL;
    OpenAPI_ma_release_indication_e ma_release_indVariable = 0;
    cJSON *ma_nw_upgrade_ind = NULL;
    cJSON *ma_request_ind = NULL;
    cJSON *unavailable_access_ind = NULL;
    OpenAPI_unavailable_access_indication_e unavailable_access_indVariable = 0;
    cJSON *psa_info = NULL;
    OpenAPI_list_t *psa_infoList = NULL;
    cJSON *ulcl_bp_info = NULL;
    OpenAPI_ulcl_bp_information_t *ulcl_bp_info_local_nonprim = NULL;
    cJSON *n4_info = NULL;
    OpenAPI_n4_information_t *n4_info_local_nonprim = NULL;
    cJSON *n4_info_ext1 = NULL;
    OpenAPI_n4_information_t *n4_info_ext1_local_nonprim = NULL;
    cJSON *n4_info_ext2 = NULL;
    OpenAPI_n4_information_t *n4_info_ext2_local_nonprim = NULL;
    cJSON *presence_in_ladn = NULL;
    OpenAPI_presence_state_e presence_in_ladnVariable = 0;
    cJSON *vsmf_pdu_session_uri = NULL;
    cJSON *vsmf_id = NULL;
    cJSON *v_smf_service_instance_id = NULL;
    cJSON *ismf_pdu_session_uri = NULL;
    cJSON *ismf_id = NULL;
    cJSON *i_smf_service_instance_id = NULL;
    cJSON *dl_serving_plmn_rate_ctl = NULL;
    cJSON *dnai_list = NULL;
    OpenAPI_list_t *dnai_listList = NULL;
    cJSON *supported_features = NULL;
    cJSON *roaming_charging_profile = NULL;
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile_local_nonprim = NULL;
    cJSON *mo_exp_data_counter = NULL;
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter_local_nonprim = NULL;
    cJSON *vplmn_qos = NULL;
    OpenAPI_vplmn_qos_t *vplmn_qos_local_nonprim = NULL;
    cJSON *security_result = NULL;
    OpenAPI_security_result_t *security_result_local_nonprim = NULL;
    cJSON *up_security_info = NULL;
    OpenAPI_up_security_info_t *up_security_info_local_nonprim = NULL;
    cJSON *amf_nf_id = NULL;
    cJSON *guami = NULL;
    OpenAPI_guami_t *guami_local_nonprim = NULL;
    cJSON *secondary_rat_usage_data_report_container = NULL;
    OpenAPI_list_t *secondary_rat_usage_data_report_containerList = NULL;
    cJSON *sm_policy_notify_ind = NULL;
    OpenAPI_hsmf_update_data_sm_policy_notify_ind_e sm_policy_notify_indVariable = 0;
    cJSON *pcf_ue_callback_info = NULL;
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info_local_nonprim = NULL;
    cJSON *satellite_backhaul_cat = NULL;
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_catVariable = 0;
    cJSON *max_integrity_protected_data_rate_ul = NULL;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ulVariable = 0;
    cJSON *max_integrity_protected_data_rate_dl = NULL;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dlVariable = 0;
    cJSON *up_cnx_state = NULL;
    OpenAPI_up_cnx_state_e up_cnx_stateVariable = 0;
    request_indication = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "requestIndication");
    if (!request_indication) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [request_indication]");
        goto end;
    }
    if (!cJSON_IsString(request_indication)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [request_indication]");
        goto end;
    }
    request_indicationVariable = OpenAPI_request_indication_FromString(request_indication->valuestring);

    pei = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    vcn_tunnel_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "vcnTunnelInfo");
    if (vcn_tunnel_info) {
    vcn_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(vcn_tunnel_info);
    if (!vcn_tunnel_info_local_nonprim) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON failed [vcn_tunnel_info]");
        goto end;
    }
    }

    icn_tunnel_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "icnTunnelInfo");
    if (icn_tunnel_info) {
    icn_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(icn_tunnel_info);
    if (!icn_tunnel_info_local_nonprim) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON failed [icn_tunnel_info]");
        goto end;
    }
    }

    additional_cn_tunnel_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "additionalCnTunnelInfo");
    if (additional_cn_tunnel_info) {
    additional_cn_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(additional_cn_tunnel_info);
    if (!additional_cn_tunnel_info_local_nonprim) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON failed [additional_cn_tunnel_info]");
        goto end;
    }
    }

    serving_network = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "servingNetwork");
    if (serving_network) {
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    if (!serving_network_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [serving_network]");
        goto end;
    }
    }

    an_type = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "anType");
    if (an_type) {
    if (!cJSON_IsString(an_type)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [an_type]");
        goto end;
    }
    an_typeVariable = OpenAPI_access_type_FromString(an_type->valuestring);
    }

    additional_an_type = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "additionalAnType");
    if (additional_an_type) {
    if (!cJSON_IsString(additional_an_type)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [additional_an_type]");
        goto end;
    }
    additional_an_typeVariable = OpenAPI_access_type_FromString(additional_an_type->valuestring);
    }

    rat_type = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ratType");
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    ue_location = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ueLocation");
    if (ue_location) {
    ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    if (!ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_location]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    add_ue_location = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "addUeLocation");
    if (add_ue_location) {
    add_ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(add_ue_location);
    if (!add_ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [add_ue_location]");
        goto end;
    }
    }

    pause_charging = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "pauseCharging");
    if (pause_charging) {
    if (!cJSON_IsBool(pause_charging)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [pause_charging]");
        goto end;
    }
    }

    pti = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "pti");
    if (pti) {
    if (!cJSON_IsNumber(pti)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [pti]");
        goto end;
    }
    }

    n1_sm_info_from_ue = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "n1SmInfoFromUe");
    if (n1_sm_info_from_ue) {
    n1_sm_info_from_ue_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_info_from_ue);
    if (!n1_sm_info_from_ue_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n1_sm_info_from_ue]");
        goto end;
    }
    }

    unknown_n1_sm_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "unknownN1SmInfo");
    if (unknown_n1_sm_info) {
    unknown_n1_sm_info_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(unknown_n1_sm_info);
    if (!unknown_n1_sm_info_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [unknown_n1_sm_info]");
        goto end;
    }
    }

    qos_flows_rel_notify_list = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "qosFlowsRelNotifyList");
    if (qos_flows_rel_notify_list) {
        cJSON *qos_flows_rel_notify_list_local = NULL;
        if (!cJSON_IsArray(qos_flows_rel_notify_list)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [qos_flows_rel_notify_list]");
            goto end;
        }

        qos_flows_rel_notify_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_flows_rel_notify_list_local, qos_flows_rel_notify_list) {
            if (!cJSON_IsObject(qos_flows_rel_notify_list_local)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [qos_flows_rel_notify_list]");
                goto end;
            }
            OpenAPI_qos_flow_item_t *qos_flows_rel_notify_listItem = OpenAPI_qos_flow_item_parseFromJSON(qos_flows_rel_notify_list_local);
            if (!qos_flows_rel_notify_listItem) {
                ogs_error("No qos_flows_rel_notify_listItem");
                goto end;
            }
            OpenAPI_list_add(qos_flows_rel_notify_listList, qos_flows_rel_notify_listItem);
        }
    }

    qos_flows_notify_list = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "qosFlowsNotifyList");
    if (qos_flows_notify_list) {
        cJSON *qos_flows_notify_list_local = NULL;
        if (!cJSON_IsArray(qos_flows_notify_list)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [qos_flows_notify_list]");
            goto end;
        }

        qos_flows_notify_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_flows_notify_list_local, qos_flows_notify_list) {
            if (!cJSON_IsObject(qos_flows_notify_list_local)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [qos_flows_notify_list]");
                goto end;
            }
            OpenAPI_qos_flow_notify_item_t *qos_flows_notify_listItem = OpenAPI_qos_flow_notify_item_parseFromJSON(qos_flows_notify_list_local);
            if (!qos_flows_notify_listItem) {
                ogs_error("No qos_flows_notify_listItem");
                goto end;
            }
            OpenAPI_list_add(qos_flows_notify_listList, qos_flows_notify_listItem);
        }
    }

    notify_list = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "NotifyList");
    if (notify_list) {
        cJSON *notify_list_local = NULL;
        if (!cJSON_IsArray(notify_list)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [notify_list]");
            goto end;
        }

        notify_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(notify_list_local, notify_list) {
            if (!cJSON_IsObject(notify_list_local)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [notify_list]");
                goto end;
            }
            OpenAPI_pdu_session_notify_item_t *notify_listItem = OpenAPI_pdu_session_notify_item_parseFromJSON(notify_list_local);
            if (!notify_listItem) {
                ogs_error("No notify_listItem");
                goto end;
            }
            OpenAPI_list_add(notify_listList, notify_listItem);
        }
    }

    eps_bearer_id = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "epsBearerId");
    if (eps_bearer_id) {
        cJSON *eps_bearer_id_local = NULL;
        if (!cJSON_IsArray(eps_bearer_id)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [eps_bearer_id]");
            goto end;
        }

        eps_bearer_idList = OpenAPI_list_create();

        cJSON_ArrayForEach(eps_bearer_id_local, eps_bearer_id) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(eps_bearer_id_local)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [eps_bearer_id]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [eps_bearer_id]");
                goto end;
            }
            *localDouble = eps_bearer_id_local->valuedouble;
            OpenAPI_list_add(eps_bearer_idList, localDouble);
        }
    }

    ho_preparation_indication = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "hoPreparationIndication");
    if (ho_preparation_indication) {
    if (!cJSON_IsBool(ho_preparation_indication)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ho_preparation_indication]");
        goto end;
    }
    }

    revoke_ebi_list = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "revokeEbiList");
    if (revoke_ebi_list) {
        cJSON *revoke_ebi_list_local = NULL;
        if (!cJSON_IsArray(revoke_ebi_list)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [revoke_ebi_list]");
            goto end;
        }

        revoke_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(revoke_ebi_list_local, revoke_ebi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(revoke_ebi_list_local)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [revoke_ebi_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [revoke_ebi_list]");
                goto end;
            }
            *localDouble = revoke_ebi_list_local->valuedouble;
            OpenAPI_list_add(revoke_ebi_listList, localDouble);
        }
    }

    cause = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [cause]");
        goto end;
    }
    causeVariable = OpenAPI_cause_FromString(cause->valuestring);
    }

    ng_ap_cause = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ngApCause");
    if (ng_ap_cause) {
    ng_ap_cause_local_nonprim = OpenAPI_ng_ap_cause_parseFromJSON(ng_ap_cause);
    if (!ng_ap_cause_local_nonprim) {
        ogs_error("OpenAPI_ng_ap_cause_parseFromJSON failed [ng_ap_cause]");
        goto end;
    }
    }

    _5g_mm_cause_value = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "5gMmCauseValue");
    if (_5g_mm_cause_value) {
    if (!cJSON_IsNumber(_5g_mm_cause_value)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [_5g_mm_cause_value]");
        goto end;
    }
    }

    always_on_requested = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "alwaysOnRequested");
    if (always_on_requested) {
    if (!cJSON_IsBool(always_on_requested)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [always_on_requested]");
        goto end;
    }
    }

    eps_interworking_ind = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "epsInterworkingInd");
    if (eps_interworking_ind) {
    if (!cJSON_IsString(eps_interworking_ind)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [eps_interworking_ind]");
        goto end;
    }
    eps_interworking_indVariable = OpenAPI_eps_interworking_indication_FromString(eps_interworking_ind->valuestring);
    }

    secondary_rat_usage_report = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "secondaryRatUsageReport");
    if (secondary_rat_usage_report) {
        cJSON *secondary_rat_usage_report_local = NULL;
        if (!cJSON_IsArray(secondary_rat_usage_report)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [secondary_rat_usage_report]");
            goto end;
        }

        secondary_rat_usage_reportList = OpenAPI_list_create();

        cJSON_ArrayForEach(secondary_rat_usage_report_local, secondary_rat_usage_report) {
            if (!cJSON_IsObject(secondary_rat_usage_report_local)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [secondary_rat_usage_report]");
                goto end;
            }
            OpenAPI_secondary_rat_usage_report_t *secondary_rat_usage_reportItem = OpenAPI_secondary_rat_usage_report_parseFromJSON(secondary_rat_usage_report_local);
            if (!secondary_rat_usage_reportItem) {
                ogs_error("No secondary_rat_usage_reportItem");
                goto end;
            }
            OpenAPI_list_add(secondary_rat_usage_reportList, secondary_rat_usage_reportItem);
        }
    }

    secondary_rat_usage_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "secondaryRatUsageInfo");
    if (secondary_rat_usage_info) {
        cJSON *secondary_rat_usage_info_local = NULL;
        if (!cJSON_IsArray(secondary_rat_usage_info)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [secondary_rat_usage_info]");
            goto end;
        }

        secondary_rat_usage_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(secondary_rat_usage_info_local, secondary_rat_usage_info) {
            if (!cJSON_IsObject(secondary_rat_usage_info_local)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [secondary_rat_usage_info]");
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

    an_type_can_be_changed = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "anTypeCanBeChanged");
    if (an_type_can_be_changed) {
    if (!cJSON_IsBool(an_type_can_be_changed)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [an_type_can_be_changed]");
        goto end;
    }
    }

    ma_release_ind = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "maReleaseInd");
    if (ma_release_ind) {
    if (!cJSON_IsString(ma_release_ind)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ma_release_ind]");
        goto end;
    }
    ma_release_indVariable = OpenAPI_ma_release_indication_FromString(ma_release_ind->valuestring);
    }

    ma_nw_upgrade_ind = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "maNwUpgradeInd");
    if (ma_nw_upgrade_ind) {
    if (!cJSON_IsBool(ma_nw_upgrade_ind)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ma_nw_upgrade_ind]");
        goto end;
    }
    }

    ma_request_ind = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "maRequestInd");
    if (ma_request_ind) {
    if (!cJSON_IsBool(ma_request_ind)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ma_request_ind]");
        goto end;
    }
    }

    unavailable_access_ind = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "unavailableAccessInd");
    if (unavailable_access_ind) {
    if (!cJSON_IsString(unavailable_access_ind)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [unavailable_access_ind]");
        goto end;
    }
    unavailable_access_indVariable = OpenAPI_unavailable_access_indication_FromString(unavailable_access_ind->valuestring);
    }

    psa_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "psaInfo");
    if (psa_info) {
        cJSON *psa_info_local = NULL;
        if (!cJSON_IsArray(psa_info)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [psa_info]");
            goto end;
        }

        psa_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(psa_info_local, psa_info) {
            if (!cJSON_IsObject(psa_info_local)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [psa_info]");
                goto end;
            }
            OpenAPI_psa_information_t *psa_infoItem = OpenAPI_psa_information_parseFromJSON(psa_info_local);
            if (!psa_infoItem) {
                ogs_error("No psa_infoItem");
                goto end;
            }
            OpenAPI_list_add(psa_infoList, psa_infoItem);
        }
    }

    ulcl_bp_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ulclBpInfo");
    if (ulcl_bp_info) {
    ulcl_bp_info_local_nonprim = OpenAPI_ulcl_bp_information_parseFromJSON(ulcl_bp_info);
    if (!ulcl_bp_info_local_nonprim) {
        ogs_error("OpenAPI_ulcl_bp_information_parseFromJSON failed [ulcl_bp_info]");
        goto end;
    }
    }

    n4_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "n4Info");
    if (n4_info) {
    n4_info_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info);
    if (!n4_info_local_nonprim) {
        ogs_error("OpenAPI_n4_information_parseFromJSON failed [n4_info]");
        goto end;
    }
    }

    n4_info_ext1 = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "n4InfoExt1");
    if (n4_info_ext1) {
    n4_info_ext1_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info_ext1);
    if (!n4_info_ext1_local_nonprim) {
        ogs_error("OpenAPI_n4_information_parseFromJSON failed [n4_info_ext1]");
        goto end;
    }
    }

    n4_info_ext2 = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "n4InfoExt2");
    if (n4_info_ext2) {
    n4_info_ext2_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info_ext2);
    if (!n4_info_ext2_local_nonprim) {
        ogs_error("OpenAPI_n4_information_parseFromJSON failed [n4_info_ext2]");
        goto end;
    }
    }

    presence_in_ladn = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "presenceInLadn");
    if (presence_in_ladn) {
    if (!cJSON_IsString(presence_in_ladn)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [presence_in_ladn]");
        goto end;
    }
    presence_in_ladnVariable = OpenAPI_presence_state_FromString(presence_in_ladn->valuestring);
    }

    vsmf_pdu_session_uri = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "vsmfPduSessionUri");
    if (vsmf_pdu_session_uri) {
    if (!cJSON_IsString(vsmf_pdu_session_uri) && !cJSON_IsNull(vsmf_pdu_session_uri)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [vsmf_pdu_session_uri]");
        goto end;
    }
    }

    vsmf_id = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "vsmfId");
    if (vsmf_id) {
    if (!cJSON_IsString(vsmf_id) && !cJSON_IsNull(vsmf_id)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [vsmf_id]");
        goto end;
    }
    }

    v_smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "vSmfServiceInstanceId");
    if (v_smf_service_instance_id) {
    if (!cJSON_IsString(v_smf_service_instance_id) && !cJSON_IsNull(v_smf_service_instance_id)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [v_smf_service_instance_id]");
        goto end;
    }
    }

    ismf_pdu_session_uri = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ismfPduSessionUri");
    if (ismf_pdu_session_uri) {
    if (!cJSON_IsString(ismf_pdu_session_uri) && !cJSON_IsNull(ismf_pdu_session_uri)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ismf_pdu_session_uri]");
        goto end;
    }
    }

    ismf_id = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ismfId");
    if (ismf_id) {
    if (!cJSON_IsString(ismf_id) && !cJSON_IsNull(ismf_id)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ismf_id]");
        goto end;
    }
    }

    i_smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "iSmfServiceInstanceId");
    if (i_smf_service_instance_id) {
    if (!cJSON_IsString(i_smf_service_instance_id) && !cJSON_IsNull(i_smf_service_instance_id)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [i_smf_service_instance_id]");
        goto end;
    }
    }

    dl_serving_plmn_rate_ctl = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "dlServingPlmnRateCtl");
    if (dl_serving_plmn_rate_ctl) {
    if (!cJSON_IsNull(dl_serving_plmn_rate_ctl)) {
    if (!cJSON_IsNumber(dl_serving_plmn_rate_ctl)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [dl_serving_plmn_rate_ctl]");
        goto end;
    }
    }
    }

    dnai_list = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "dnaiList");
    if (dnai_list) {
        cJSON *dnai_list_local = NULL;
        if (!cJSON_IsArray(dnai_list)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [dnai_list]");
            goto end;
        }

        dnai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnai_list_local, dnai_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnai_list_local)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [dnai_list]");
                goto end;
            }
            OpenAPI_list_add(dnai_listList, ogs_strdup(dnai_list_local->valuestring));
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    roaming_charging_profile = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "roamingChargingProfile");
    if (roaming_charging_profile) {
    roaming_charging_profile_local_nonprim = OpenAPI_roaming_charging_profile_parseFromJSON(roaming_charging_profile);
    if (!roaming_charging_profile_local_nonprim) {
        ogs_error("OpenAPI_roaming_charging_profile_parseFromJSON failed [roaming_charging_profile]");
        goto end;
    }
    }

    mo_exp_data_counter = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "moExpDataCounter");
    if (mo_exp_data_counter) {
    mo_exp_data_counter_local_nonprim = OpenAPI_mo_exp_data_counter_parseFromJSON(mo_exp_data_counter);
    if (!mo_exp_data_counter_local_nonprim) {
        ogs_error("OpenAPI_mo_exp_data_counter_parseFromJSON failed [mo_exp_data_counter]");
        goto end;
    }
    }

    vplmn_qos = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "vplmnQos");
    if (vplmn_qos) {
    vplmn_qos_local_nonprim = OpenAPI_vplmn_qos_parseFromJSON(vplmn_qos);
    if (!vplmn_qos_local_nonprim) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON failed [vplmn_qos]");
        goto end;
    }
    }

    security_result = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "securityResult");
    if (security_result) {
    security_result_local_nonprim = OpenAPI_security_result_parseFromJSON(security_result);
    if (!security_result_local_nonprim) {
        ogs_error("OpenAPI_security_result_parseFromJSON failed [security_result]");
        goto end;
    }
    }

    up_security_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "upSecurityInfo");
    if (up_security_info) {
    up_security_info_local_nonprim = OpenAPI_up_security_info_parseFromJSON(up_security_info);
    if (!up_security_info_local_nonprim) {
        ogs_error("OpenAPI_up_security_info_parseFromJSON failed [up_security_info]");
        goto end;
    }
    }

    amf_nf_id = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "amfNfId");
    if (amf_nf_id) {
    if (!cJSON_IsString(amf_nf_id) && !cJSON_IsNull(amf_nf_id)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [amf_nf_id]");
        goto end;
    }
    }

    guami = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "guami");
    if (guami) {
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    if (!guami_local_nonprim) {
        ogs_error("OpenAPI_guami_parseFromJSON failed [guami]");
        goto end;
    }
    }

    secondary_rat_usage_data_report_container = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "secondaryRatUsageDataReportContainer");
    if (secondary_rat_usage_data_report_container) {
        cJSON *secondary_rat_usage_data_report_container_local = NULL;
        if (!cJSON_IsArray(secondary_rat_usage_data_report_container)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [secondary_rat_usage_data_report_container]");
            goto end;
        }

        secondary_rat_usage_data_report_containerList = OpenAPI_list_create();

        cJSON_ArrayForEach(secondary_rat_usage_data_report_container_local, secondary_rat_usage_data_report_container) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(secondary_rat_usage_data_report_container_local)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [secondary_rat_usage_data_report_container]");
                goto end;
            }
            OpenAPI_list_add(secondary_rat_usage_data_report_containerList, ogs_strdup(secondary_rat_usage_data_report_container_local->valuestring));
        }
    }

    sm_policy_notify_ind = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "smPolicyNotifyInd");
    if (sm_policy_notify_ind) {
    if (!cJSON_IsString(sm_policy_notify_ind)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [sm_policy_notify_ind]");
        goto end;
    }
    sm_policy_notify_indVariable = OpenAPI_sm_policy_notify_indhsmf_update_data_FromString(sm_policy_notify_ind->valuestring);
    }

    pcf_ue_callback_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "pcfUeCallbackInfo");
    if (pcf_ue_callback_info) {
    if (!cJSON_IsNull(pcf_ue_callback_info)) {
    pcf_ue_callback_info_local_nonprim = OpenAPI_pcf_ue_callback_info_parseFromJSON(pcf_ue_callback_info);
    if (!pcf_ue_callback_info_local_nonprim) {
        ogs_error("OpenAPI_pcf_ue_callback_info_parseFromJSON failed [pcf_ue_callback_info]");
        goto end;
    }
    }
    }

    satellite_backhaul_cat = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "satelliteBackhaulCat");
    if (satellite_backhaul_cat) {
    if (!cJSON_IsString(satellite_backhaul_cat)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [satellite_backhaul_cat]");
        goto end;
    }
    satellite_backhaul_catVariable = OpenAPI_satellite_backhaul_category_FromString(satellite_backhaul_cat->valuestring);
    }

    max_integrity_protected_data_rate_ul = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "maxIntegrityProtectedDataRateUl");
    if (max_integrity_protected_data_rate_ul) {
    if (!cJSON_IsString(max_integrity_protected_data_rate_ul)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [max_integrity_protected_data_rate_ul]");
        goto end;
    }
    max_integrity_protected_data_rate_ulVariable = OpenAPI_max_integrity_protected_data_rate_FromString(max_integrity_protected_data_rate_ul->valuestring);
    }

    max_integrity_protected_data_rate_dl = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "maxIntegrityProtectedDataRateDl");
    if (max_integrity_protected_data_rate_dl) {
    if (!cJSON_IsString(max_integrity_protected_data_rate_dl)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [max_integrity_protected_data_rate_dl]");
        goto end;
    }
    max_integrity_protected_data_rate_dlVariable = OpenAPI_max_integrity_protected_data_rate_FromString(max_integrity_protected_data_rate_dl->valuestring);
    }

    up_cnx_state = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "upCnxState");
    if (up_cnx_state) {
    if (!cJSON_IsString(up_cnx_state)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [up_cnx_state]");
        goto end;
    }
    up_cnx_stateVariable = OpenAPI_up_cnx_state_FromString(up_cnx_state->valuestring);
    }

    hsmf_update_data_local_var = OpenAPI_hsmf_update_data_create (
        request_indicationVariable,
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        vcn_tunnel_info ? vcn_tunnel_info_local_nonprim : NULL,
        icn_tunnel_info ? icn_tunnel_info_local_nonprim : NULL,
        additional_cn_tunnel_info ? additional_cn_tunnel_info_local_nonprim : NULL,
        serving_network ? serving_network_local_nonprim : NULL,
        an_type ? an_typeVariable : 0,
        additional_an_type ? additional_an_typeVariable : 0,
        rat_type ? rat_typeVariable : 0,
        ue_location ? ue_location_local_nonprim : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        add_ue_location ? add_ue_location_local_nonprim : NULL,
        pause_charging ? true : false,
        pause_charging ? pause_charging->valueint : 0,
        pti ? true : false,
        pti ? pti->valuedouble : 0,
        n1_sm_info_from_ue ? n1_sm_info_from_ue_local_nonprim : NULL,
        unknown_n1_sm_info ? unknown_n1_sm_info_local_nonprim : NULL,
        qos_flows_rel_notify_list ? qos_flows_rel_notify_listList : NULL,
        qos_flows_notify_list ? qos_flows_notify_listList : NULL,
        notify_list ? notify_listList : NULL,
        eps_bearer_id ? eps_bearer_idList : NULL,
        ho_preparation_indication ? true : false,
        ho_preparation_indication ? ho_preparation_indication->valueint : 0,
        revoke_ebi_list ? revoke_ebi_listList : NULL,
        cause ? causeVariable : 0,
        ng_ap_cause ? ng_ap_cause_local_nonprim : NULL,
        _5g_mm_cause_value ? true : false,
        _5g_mm_cause_value ? _5g_mm_cause_value->valuedouble : 0,
        always_on_requested ? true : false,
        always_on_requested ? always_on_requested->valueint : 0,
        eps_interworking_ind ? eps_interworking_indVariable : 0,
        secondary_rat_usage_report ? secondary_rat_usage_reportList : NULL,
        secondary_rat_usage_info ? secondary_rat_usage_infoList : NULL,
        an_type_can_be_changed ? true : false,
        an_type_can_be_changed ? an_type_can_be_changed->valueint : 0,
        ma_release_ind ? ma_release_indVariable : 0,
        ma_nw_upgrade_ind ? true : false,
        ma_nw_upgrade_ind ? ma_nw_upgrade_ind->valueint : 0,
        ma_request_ind ? true : false,
        ma_request_ind ? ma_request_ind->valueint : 0,
        unavailable_access_ind ? unavailable_access_indVariable : 0,
        psa_info ? psa_infoList : NULL,
        ulcl_bp_info ? ulcl_bp_info_local_nonprim : NULL,
        n4_info ? n4_info_local_nonprim : NULL,
        n4_info_ext1 ? n4_info_ext1_local_nonprim : NULL,
        n4_info_ext2 ? n4_info_ext2_local_nonprim : NULL,
        presence_in_ladn ? presence_in_ladnVariable : 0,
        vsmf_pdu_session_uri && !cJSON_IsNull(vsmf_pdu_session_uri) ? ogs_strdup(vsmf_pdu_session_uri->valuestring) : NULL,
        vsmf_id && !cJSON_IsNull(vsmf_id) ? ogs_strdup(vsmf_id->valuestring) : NULL,
        v_smf_service_instance_id && !cJSON_IsNull(v_smf_service_instance_id) ? ogs_strdup(v_smf_service_instance_id->valuestring) : NULL,
        ismf_pdu_session_uri && !cJSON_IsNull(ismf_pdu_session_uri) ? ogs_strdup(ismf_pdu_session_uri->valuestring) : NULL,
        ismf_id && !cJSON_IsNull(ismf_id) ? ogs_strdup(ismf_id->valuestring) : NULL,
        i_smf_service_instance_id && !cJSON_IsNull(i_smf_service_instance_id) ? ogs_strdup(i_smf_service_instance_id->valuestring) : NULL,
        dl_serving_plmn_rate_ctl && cJSON_IsNull(dl_serving_plmn_rate_ctl) ? true : false,
        dl_serving_plmn_rate_ctl ? true : false,
        dl_serving_plmn_rate_ctl ? dl_serving_plmn_rate_ctl->valuedouble : 0,
        dnai_list ? dnai_listList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        roaming_charging_profile ? roaming_charging_profile_local_nonprim : NULL,
        mo_exp_data_counter ? mo_exp_data_counter_local_nonprim : NULL,
        vplmn_qos ? vplmn_qos_local_nonprim : NULL,
        security_result ? security_result_local_nonprim : NULL,
        up_security_info ? up_security_info_local_nonprim : NULL,
        amf_nf_id && !cJSON_IsNull(amf_nf_id) ? ogs_strdup(amf_nf_id->valuestring) : NULL,
        guami ? guami_local_nonprim : NULL,
        secondary_rat_usage_data_report_container ? secondary_rat_usage_data_report_containerList : NULL,
        sm_policy_notify_ind ? sm_policy_notify_indVariable : 0,
        pcf_ue_callback_info && cJSON_IsNull(pcf_ue_callback_info) ? true : false,
        pcf_ue_callback_info ? pcf_ue_callback_info_local_nonprim : NULL,
        satellite_backhaul_cat ? satellite_backhaul_catVariable : 0,
        max_integrity_protected_data_rate_ul ? max_integrity_protected_data_rate_ulVariable : 0,
        max_integrity_protected_data_rate_dl ? max_integrity_protected_data_rate_dlVariable : 0,
        up_cnx_state ? up_cnx_stateVariable : 0
    );

    return hsmf_update_data_local_var;
end:
    if (vcn_tunnel_info_local_nonprim) {
        OpenAPI_tunnel_info_free(vcn_tunnel_info_local_nonprim);
        vcn_tunnel_info_local_nonprim = NULL;
    }
    if (icn_tunnel_info_local_nonprim) {
        OpenAPI_tunnel_info_free(icn_tunnel_info_local_nonprim);
        icn_tunnel_info_local_nonprim = NULL;
    }
    if (additional_cn_tunnel_info_local_nonprim) {
        OpenAPI_tunnel_info_free(additional_cn_tunnel_info_local_nonprim);
        additional_cn_tunnel_info_local_nonprim = NULL;
    }
    if (serving_network_local_nonprim) {
        OpenAPI_plmn_id_nid_free(serving_network_local_nonprim);
        serving_network_local_nonprim = NULL;
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
    if (qos_flows_rel_notify_listList) {
        OpenAPI_list_for_each(qos_flows_rel_notify_listList, node) {
            OpenAPI_qos_flow_item_free(node->data);
        }
        OpenAPI_list_free(qos_flows_rel_notify_listList);
        qos_flows_rel_notify_listList = NULL;
    }
    if (qos_flows_notify_listList) {
        OpenAPI_list_for_each(qos_flows_notify_listList, node) {
            OpenAPI_qos_flow_notify_item_free(node->data);
        }
        OpenAPI_list_free(qos_flows_notify_listList);
        qos_flows_notify_listList = NULL;
    }
    if (notify_listList) {
        OpenAPI_list_for_each(notify_listList, node) {
            OpenAPI_pdu_session_notify_item_free(node->data);
        }
        OpenAPI_list_free(notify_listList);
        notify_listList = NULL;
    }
    if (eps_bearer_idList) {
        OpenAPI_list_for_each(eps_bearer_idList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(eps_bearer_idList);
        eps_bearer_idList = NULL;
    }
    if (revoke_ebi_listList) {
        OpenAPI_list_for_each(revoke_ebi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(revoke_ebi_listList);
        revoke_ebi_listList = NULL;
    }
    if (ng_ap_cause_local_nonprim) {
        OpenAPI_ng_ap_cause_free(ng_ap_cause_local_nonprim);
        ng_ap_cause_local_nonprim = NULL;
    }
    if (secondary_rat_usage_reportList) {
        OpenAPI_list_for_each(secondary_rat_usage_reportList, node) {
            OpenAPI_secondary_rat_usage_report_free(node->data);
        }
        OpenAPI_list_free(secondary_rat_usage_reportList);
        secondary_rat_usage_reportList = NULL;
    }
    if (secondary_rat_usage_infoList) {
        OpenAPI_list_for_each(secondary_rat_usage_infoList, node) {
            OpenAPI_secondary_rat_usage_info_free(node->data);
        }
        OpenAPI_list_free(secondary_rat_usage_infoList);
        secondary_rat_usage_infoList = NULL;
    }
    if (psa_infoList) {
        OpenAPI_list_for_each(psa_infoList, node) {
            OpenAPI_psa_information_free(node->data);
        }
        OpenAPI_list_free(psa_infoList);
        psa_infoList = NULL;
    }
    if (ulcl_bp_info_local_nonprim) {
        OpenAPI_ulcl_bp_information_free(ulcl_bp_info_local_nonprim);
        ulcl_bp_info_local_nonprim = NULL;
    }
    if (n4_info_local_nonprim) {
        OpenAPI_n4_information_free(n4_info_local_nonprim);
        n4_info_local_nonprim = NULL;
    }
    if (n4_info_ext1_local_nonprim) {
        OpenAPI_n4_information_free(n4_info_ext1_local_nonprim);
        n4_info_ext1_local_nonprim = NULL;
    }
    if (n4_info_ext2_local_nonprim) {
        OpenAPI_n4_information_free(n4_info_ext2_local_nonprim);
        n4_info_ext2_local_nonprim = NULL;
    }
    if (dnai_listList) {
        OpenAPI_list_for_each(dnai_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnai_listList);
        dnai_listList = NULL;
    }
    if (roaming_charging_profile_local_nonprim) {
        OpenAPI_roaming_charging_profile_free(roaming_charging_profile_local_nonprim);
        roaming_charging_profile_local_nonprim = NULL;
    }
    if (mo_exp_data_counter_local_nonprim) {
        OpenAPI_mo_exp_data_counter_free(mo_exp_data_counter_local_nonprim);
        mo_exp_data_counter_local_nonprim = NULL;
    }
    if (vplmn_qos_local_nonprim) {
        OpenAPI_vplmn_qos_free(vplmn_qos_local_nonprim);
        vplmn_qos_local_nonprim = NULL;
    }
    if (security_result_local_nonprim) {
        OpenAPI_security_result_free(security_result_local_nonprim);
        security_result_local_nonprim = NULL;
    }
    if (up_security_info_local_nonprim) {
        OpenAPI_up_security_info_free(up_security_info_local_nonprim);
        up_security_info_local_nonprim = NULL;
    }
    if (guami_local_nonprim) {
        OpenAPI_guami_free(guami_local_nonprim);
        guami_local_nonprim = NULL;
    }
    if (secondary_rat_usage_data_report_containerList) {
        OpenAPI_list_for_each(secondary_rat_usage_data_report_containerList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(secondary_rat_usage_data_report_containerList);
        secondary_rat_usage_data_report_containerList = NULL;
    }
    if (pcf_ue_callback_info_local_nonprim) {
        OpenAPI_pcf_ue_callback_info_free(pcf_ue_callback_info_local_nonprim);
        pcf_ue_callback_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_hsmf_update_data_t *OpenAPI_hsmf_update_data_copy(OpenAPI_hsmf_update_data_t *dst, OpenAPI_hsmf_update_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_hsmf_update_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed");
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

    OpenAPI_hsmf_update_data_free(dst);
    dst = OpenAPI_hsmf_update_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

