
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context.h"

OpenAPI_sm_context_t *OpenAPI_sm_context_create(
    int pdu_session_id,
    char *dnn,
    char *selected_dnn,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_snssai_t *alt_snssai,
    OpenAPI_snssai_t *hplmn_snssai,
    OpenAPI_snssai_t *alt_hplmn_snssai,
    OpenAPI_pdu_session_type_e pdu_session_type,
    char *gpsi,
    char *h_smf_uri,
    char *smf_uri,
    char *pdu_session_ref,
    char *inter_plmn_api_root,
    char *intra_plmn_api_root,
    char *pcf_id,
    char *pcf_group_id,
    char *pcf_set_id,
    OpenAPI_dnn_selection_mode_e sel_mode,
    char *udm_group_id,
    char *routing_indicator,
    bool is_h_nw_pub_key_id,
    int h_nw_pub_key_id,
    OpenAPI_ambr_t *session_ambr,
    OpenAPI_list_t *qos_flows_list,
    char *h_smf_instance_id,
    char *smf_instance_id,
    char *pdu_session_smf_set_id,
    char *pdu_session_smf_service_set_id,
    OpenAPI_sbi_binding_level_e pdu_session_smf_binding,
    bool is_enable_pause_charging,
    int enable_pause_charging,
    char *ue_ipv4_address,
    char *ue_ipv6_prefix,
    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info,
    OpenAPI_list_t *eps_bearer_info,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl,
    bool is_always_on_granted,
    int always_on_granted,
    OpenAPI_up_security_t *up_security,
    char *h_smf_service_instance_id,
    char *smf_service_instance_id,
    char *recovery_time,
    bool is_forwarding_ind,
    int forwarding_ind,
    OpenAPI_tunnel_info_t *psa_tunnel_info,
    char *charging_id,
    char *smf_charging_id,
    OpenAPI_charging_information_t *charging_info,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile,
    bool is_nef_ext_buf_support_ind,
    int nef_ext_buf_support_ind,
    bool is_ipv6_index,
    int ipv6_index,
    OpenAPI_ip_address_t *dn_aaa_address,
    OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_info,
    OpenAPI_qos_flow_tunnel_t *ran_tunnel_info,
    OpenAPI_list_t *add_ran_tunnel_info,
    OpenAPI_qos_flow_tunnel_t *red_ran_tunnel_info,
    OpenAPI_list_t *add_red_ran_tunnel_info,
    bool is_nspu_support_ind,
    int nspu_support_ind,
    char *smf_binding_info,
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_cat,
    char *ssc_mode,
    bool is_dlset_support_ind,
    int dlset_support_ind,
    bool is_n9fsc_support_ind,
    int n9fsc_support_ind,
    bool is_anchor_smf_oauth2_required,
    int anchor_smf_oauth2_required,
    OpenAPI_list_t *full_dnai_list,
    bool is_hrsbo_auth_req_ind,
    int hrsbo_auth_req_ind,
    OpenAPI_ip_address_t *h_dns_addr,
    OpenAPI_ip_address_t *h_plmn_addr,
    OpenAPI_list_t *vplmn_offloading_info_list,
    bool is_vplmn_dl_ambr_null,
    OpenAPI_vplmn_dl_ambr_t *vplmn_dl_ambr,
    OpenAPI_list_t *offload_ids,
    char *target_dnai,
    OpenAPI_traffic_influence_info_t *traffic_influ_info,
    OpenAPI_list_t *pending_update_info_list,
    OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh,
    bool is_local_offloading_mgt_allowed_ind,
    int local_offloading_mgt_allowed_ind,
    OpenAPI_ip_address_t *dns_addr,
    OpenAPI_ip_address_t *psa_upf_addr,
    OpenAPI_list_t *local_offloading_info_list,
    bool is_priority_user_ind,
    int priority_user_ind,
    OpenAPI_qos_monitoring_info_t *qos_monitoring_info
)
{
    OpenAPI_sm_context_t *sm_context_local_var = ogs_malloc(sizeof(OpenAPI_sm_context_t));
    ogs_assert(sm_context_local_var);

    sm_context_local_var->pdu_session_id = pdu_session_id;
    sm_context_local_var->dnn = dnn;
    sm_context_local_var->selected_dnn = selected_dnn;
    sm_context_local_var->s_nssai = s_nssai;
    sm_context_local_var->alt_snssai = alt_snssai;
    sm_context_local_var->hplmn_snssai = hplmn_snssai;
    sm_context_local_var->alt_hplmn_snssai = alt_hplmn_snssai;
    sm_context_local_var->pdu_session_type = pdu_session_type;
    sm_context_local_var->gpsi = gpsi;
    sm_context_local_var->h_smf_uri = h_smf_uri;
    sm_context_local_var->smf_uri = smf_uri;
    sm_context_local_var->pdu_session_ref = pdu_session_ref;
    sm_context_local_var->inter_plmn_api_root = inter_plmn_api_root;
    sm_context_local_var->intra_plmn_api_root = intra_plmn_api_root;
    sm_context_local_var->pcf_id = pcf_id;
    sm_context_local_var->pcf_group_id = pcf_group_id;
    sm_context_local_var->pcf_set_id = pcf_set_id;
    sm_context_local_var->sel_mode = sel_mode;
    sm_context_local_var->udm_group_id = udm_group_id;
    sm_context_local_var->routing_indicator = routing_indicator;
    sm_context_local_var->is_h_nw_pub_key_id = is_h_nw_pub_key_id;
    sm_context_local_var->h_nw_pub_key_id = h_nw_pub_key_id;
    sm_context_local_var->session_ambr = session_ambr;
    sm_context_local_var->qos_flows_list = qos_flows_list;
    sm_context_local_var->h_smf_instance_id = h_smf_instance_id;
    sm_context_local_var->smf_instance_id = smf_instance_id;
    sm_context_local_var->pdu_session_smf_set_id = pdu_session_smf_set_id;
    sm_context_local_var->pdu_session_smf_service_set_id = pdu_session_smf_service_set_id;
    sm_context_local_var->pdu_session_smf_binding = pdu_session_smf_binding;
    sm_context_local_var->is_enable_pause_charging = is_enable_pause_charging;
    sm_context_local_var->enable_pause_charging = enable_pause_charging;
    sm_context_local_var->ue_ipv4_address = ue_ipv4_address;
    sm_context_local_var->ue_ipv6_prefix = ue_ipv6_prefix;
    sm_context_local_var->eps_pdn_cnx_info = eps_pdn_cnx_info;
    sm_context_local_var->eps_bearer_info = eps_bearer_info;
    sm_context_local_var->max_integrity_protected_data_rate = max_integrity_protected_data_rate;
    sm_context_local_var->max_integrity_protected_data_rate_dl = max_integrity_protected_data_rate_dl;
    sm_context_local_var->is_always_on_granted = is_always_on_granted;
    sm_context_local_var->always_on_granted = always_on_granted;
    sm_context_local_var->up_security = up_security;
    sm_context_local_var->h_smf_service_instance_id = h_smf_service_instance_id;
    sm_context_local_var->smf_service_instance_id = smf_service_instance_id;
    sm_context_local_var->recovery_time = recovery_time;
    sm_context_local_var->is_forwarding_ind = is_forwarding_ind;
    sm_context_local_var->forwarding_ind = forwarding_ind;
    sm_context_local_var->psa_tunnel_info = psa_tunnel_info;
    sm_context_local_var->charging_id = charging_id;
    sm_context_local_var->smf_charging_id = smf_charging_id;
    sm_context_local_var->charging_info = charging_info;
    sm_context_local_var->roaming_charging_profile = roaming_charging_profile;
    sm_context_local_var->is_nef_ext_buf_support_ind = is_nef_ext_buf_support_ind;
    sm_context_local_var->nef_ext_buf_support_ind = nef_ext_buf_support_ind;
    sm_context_local_var->is_ipv6_index = is_ipv6_index;
    sm_context_local_var->ipv6_index = ipv6_index;
    sm_context_local_var->dn_aaa_address = dn_aaa_address;
    sm_context_local_var->redundant_pdu_session_info = redundant_pdu_session_info;
    sm_context_local_var->ran_tunnel_info = ran_tunnel_info;
    sm_context_local_var->add_ran_tunnel_info = add_ran_tunnel_info;
    sm_context_local_var->red_ran_tunnel_info = red_ran_tunnel_info;
    sm_context_local_var->add_red_ran_tunnel_info = add_red_ran_tunnel_info;
    sm_context_local_var->is_nspu_support_ind = is_nspu_support_ind;
    sm_context_local_var->nspu_support_ind = nspu_support_ind;
    sm_context_local_var->smf_binding_info = smf_binding_info;
    sm_context_local_var->satellite_backhaul_cat = satellite_backhaul_cat;
    sm_context_local_var->ssc_mode = ssc_mode;
    sm_context_local_var->is_dlset_support_ind = is_dlset_support_ind;
    sm_context_local_var->dlset_support_ind = dlset_support_ind;
    sm_context_local_var->is_n9fsc_support_ind = is_n9fsc_support_ind;
    sm_context_local_var->n9fsc_support_ind = n9fsc_support_ind;
    sm_context_local_var->is_anchor_smf_oauth2_required = is_anchor_smf_oauth2_required;
    sm_context_local_var->anchor_smf_oauth2_required = anchor_smf_oauth2_required;
    sm_context_local_var->full_dnai_list = full_dnai_list;
    sm_context_local_var->is_hrsbo_auth_req_ind = is_hrsbo_auth_req_ind;
    sm_context_local_var->hrsbo_auth_req_ind = hrsbo_auth_req_ind;
    sm_context_local_var->h_dns_addr = h_dns_addr;
    sm_context_local_var->h_plmn_addr = h_plmn_addr;
    sm_context_local_var->vplmn_offloading_info_list = vplmn_offloading_info_list;
    sm_context_local_var->is_vplmn_dl_ambr_null = is_vplmn_dl_ambr_null;
    sm_context_local_var->vplmn_dl_ambr = vplmn_dl_ambr;
    sm_context_local_var->offload_ids = offload_ids;
    sm_context_local_var->target_dnai = target_dnai;
    sm_context_local_var->traffic_influ_info = traffic_influ_info;
    sm_context_local_var->pending_update_info_list = pending_update_info_list;
    sm_context_local_var->eas_info_to_refresh = eas_info_to_refresh;
    sm_context_local_var->is_local_offloading_mgt_allowed_ind = is_local_offloading_mgt_allowed_ind;
    sm_context_local_var->local_offloading_mgt_allowed_ind = local_offloading_mgt_allowed_ind;
    sm_context_local_var->dns_addr = dns_addr;
    sm_context_local_var->psa_upf_addr = psa_upf_addr;
    sm_context_local_var->local_offloading_info_list = local_offloading_info_list;
    sm_context_local_var->is_priority_user_ind = is_priority_user_ind;
    sm_context_local_var->priority_user_ind = priority_user_ind;
    sm_context_local_var->qos_monitoring_info = qos_monitoring_info;

    return sm_context_local_var;
}

void OpenAPI_sm_context_free(OpenAPI_sm_context_t *sm_context)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_context) {
        return;
    }
    if (sm_context->dnn) {
        ogs_free(sm_context->dnn);
        sm_context->dnn = NULL;
    }
    if (sm_context->selected_dnn) {
        ogs_free(sm_context->selected_dnn);
        sm_context->selected_dnn = NULL;
    }
    if (sm_context->s_nssai) {
        OpenAPI_snssai_free(sm_context->s_nssai);
        sm_context->s_nssai = NULL;
    }
    if (sm_context->alt_snssai) {
        OpenAPI_snssai_free(sm_context->alt_snssai);
        sm_context->alt_snssai = NULL;
    }
    if (sm_context->hplmn_snssai) {
        OpenAPI_snssai_free(sm_context->hplmn_snssai);
        sm_context->hplmn_snssai = NULL;
    }
    if (sm_context->alt_hplmn_snssai) {
        OpenAPI_snssai_free(sm_context->alt_hplmn_snssai);
        sm_context->alt_hplmn_snssai = NULL;
    }
    if (sm_context->gpsi) {
        ogs_free(sm_context->gpsi);
        sm_context->gpsi = NULL;
    }
    if (sm_context->h_smf_uri) {
        ogs_free(sm_context->h_smf_uri);
        sm_context->h_smf_uri = NULL;
    }
    if (sm_context->smf_uri) {
        ogs_free(sm_context->smf_uri);
        sm_context->smf_uri = NULL;
    }
    if (sm_context->pdu_session_ref) {
        ogs_free(sm_context->pdu_session_ref);
        sm_context->pdu_session_ref = NULL;
    }
    if (sm_context->inter_plmn_api_root) {
        ogs_free(sm_context->inter_plmn_api_root);
        sm_context->inter_plmn_api_root = NULL;
    }
    if (sm_context->intra_plmn_api_root) {
        ogs_free(sm_context->intra_plmn_api_root);
        sm_context->intra_plmn_api_root = NULL;
    }
    if (sm_context->pcf_id) {
        ogs_free(sm_context->pcf_id);
        sm_context->pcf_id = NULL;
    }
    if (sm_context->pcf_group_id) {
        ogs_free(sm_context->pcf_group_id);
        sm_context->pcf_group_id = NULL;
    }
    if (sm_context->pcf_set_id) {
        ogs_free(sm_context->pcf_set_id);
        sm_context->pcf_set_id = NULL;
    }
    if (sm_context->udm_group_id) {
        ogs_free(sm_context->udm_group_id);
        sm_context->udm_group_id = NULL;
    }
    if (sm_context->routing_indicator) {
        ogs_free(sm_context->routing_indicator);
        sm_context->routing_indicator = NULL;
    }
    if (sm_context->session_ambr) {
        OpenAPI_ambr_free(sm_context->session_ambr);
        sm_context->session_ambr = NULL;
    }
    if (sm_context->qos_flows_list) {
        OpenAPI_list_for_each(sm_context->qos_flows_list, node) {
            OpenAPI_qos_flow_setup_item_free(node->data);
        }
        OpenAPI_list_free(sm_context->qos_flows_list);
        sm_context->qos_flows_list = NULL;
    }
    if (sm_context->h_smf_instance_id) {
        ogs_free(sm_context->h_smf_instance_id);
        sm_context->h_smf_instance_id = NULL;
    }
    if (sm_context->smf_instance_id) {
        ogs_free(sm_context->smf_instance_id);
        sm_context->smf_instance_id = NULL;
    }
    if (sm_context->pdu_session_smf_set_id) {
        ogs_free(sm_context->pdu_session_smf_set_id);
        sm_context->pdu_session_smf_set_id = NULL;
    }
    if (sm_context->pdu_session_smf_service_set_id) {
        ogs_free(sm_context->pdu_session_smf_service_set_id);
        sm_context->pdu_session_smf_service_set_id = NULL;
    }
    if (sm_context->ue_ipv4_address) {
        ogs_free(sm_context->ue_ipv4_address);
        sm_context->ue_ipv4_address = NULL;
    }
    if (sm_context->ue_ipv6_prefix) {
        ogs_free(sm_context->ue_ipv6_prefix);
        sm_context->ue_ipv6_prefix = NULL;
    }
    if (sm_context->eps_pdn_cnx_info) {
        OpenAPI_eps_pdn_cnx_info_free(sm_context->eps_pdn_cnx_info);
        sm_context->eps_pdn_cnx_info = NULL;
    }
    if (sm_context->eps_bearer_info) {
        OpenAPI_list_for_each(sm_context->eps_bearer_info, node) {
            OpenAPI_eps_bearer_info_free(node->data);
        }
        OpenAPI_list_free(sm_context->eps_bearer_info);
        sm_context->eps_bearer_info = NULL;
    }
    if (sm_context->up_security) {
        OpenAPI_up_security_free(sm_context->up_security);
        sm_context->up_security = NULL;
    }
    if (sm_context->h_smf_service_instance_id) {
        ogs_free(sm_context->h_smf_service_instance_id);
        sm_context->h_smf_service_instance_id = NULL;
    }
    if (sm_context->smf_service_instance_id) {
        ogs_free(sm_context->smf_service_instance_id);
        sm_context->smf_service_instance_id = NULL;
    }
    if (sm_context->recovery_time) {
        ogs_free(sm_context->recovery_time);
        sm_context->recovery_time = NULL;
    }
    if (sm_context->psa_tunnel_info) {
        OpenAPI_tunnel_info_free(sm_context->psa_tunnel_info);
        sm_context->psa_tunnel_info = NULL;
    }
    if (sm_context->charging_id) {
        ogs_free(sm_context->charging_id);
        sm_context->charging_id = NULL;
    }
    if (sm_context->smf_charging_id) {
        ogs_free(sm_context->smf_charging_id);
        sm_context->smf_charging_id = NULL;
    }
    if (sm_context->charging_info) {
        OpenAPI_charging_information_free(sm_context->charging_info);
        sm_context->charging_info = NULL;
    }
    if (sm_context->roaming_charging_profile) {
        OpenAPI_roaming_charging_profile_free(sm_context->roaming_charging_profile);
        sm_context->roaming_charging_profile = NULL;
    }
    if (sm_context->dn_aaa_address) {
        OpenAPI_ip_address_free(sm_context->dn_aaa_address);
        sm_context->dn_aaa_address = NULL;
    }
    if (sm_context->redundant_pdu_session_info) {
        OpenAPI_redundant_pdu_session_information_free(sm_context->redundant_pdu_session_info);
        sm_context->redundant_pdu_session_info = NULL;
    }
    if (sm_context->ran_tunnel_info) {
        OpenAPI_qos_flow_tunnel_free(sm_context->ran_tunnel_info);
        sm_context->ran_tunnel_info = NULL;
    }
    if (sm_context->add_ran_tunnel_info) {
        OpenAPI_list_for_each(sm_context->add_ran_tunnel_info, node) {
            OpenAPI_qos_flow_tunnel_free(node->data);
        }
        OpenAPI_list_free(sm_context->add_ran_tunnel_info);
        sm_context->add_ran_tunnel_info = NULL;
    }
    if (sm_context->red_ran_tunnel_info) {
        OpenAPI_qos_flow_tunnel_free(sm_context->red_ran_tunnel_info);
        sm_context->red_ran_tunnel_info = NULL;
    }
    if (sm_context->add_red_ran_tunnel_info) {
        OpenAPI_list_for_each(sm_context->add_red_ran_tunnel_info, node) {
            OpenAPI_qos_flow_tunnel_free(node->data);
        }
        OpenAPI_list_free(sm_context->add_red_ran_tunnel_info);
        sm_context->add_red_ran_tunnel_info = NULL;
    }
    if (sm_context->smf_binding_info) {
        ogs_free(sm_context->smf_binding_info);
        sm_context->smf_binding_info = NULL;
    }
    if (sm_context->ssc_mode) {
        ogs_free(sm_context->ssc_mode);
        sm_context->ssc_mode = NULL;
    }
    if (sm_context->full_dnai_list) {
        OpenAPI_list_for_each(sm_context->full_dnai_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context->full_dnai_list);
        sm_context->full_dnai_list = NULL;
    }
    if (sm_context->h_dns_addr) {
        OpenAPI_ip_address_free(sm_context->h_dns_addr);
        sm_context->h_dns_addr = NULL;
    }
    if (sm_context->h_plmn_addr) {
        OpenAPI_ip_address_free(sm_context->h_plmn_addr);
        sm_context->h_plmn_addr = NULL;
    }
    if (sm_context->vplmn_offloading_info_list) {
        OpenAPI_list_for_each(sm_context->vplmn_offloading_info_list, node) {
            OpenAPI_vplmn_offloading_info_free(node->data);
        }
        OpenAPI_list_free(sm_context->vplmn_offloading_info_list);
        sm_context->vplmn_offloading_info_list = NULL;
    }
    if (sm_context->vplmn_dl_ambr) {
        OpenAPI_vplmn_dl_ambr_free(sm_context->vplmn_dl_ambr);
        sm_context->vplmn_dl_ambr = NULL;
    }
    if (sm_context->offload_ids) {
        OpenAPI_list_for_each(sm_context->offload_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context->offload_ids);
        sm_context->offload_ids = NULL;
    }
    if (sm_context->target_dnai) {
        ogs_free(sm_context->target_dnai);
        sm_context->target_dnai = NULL;
    }
    if (sm_context->traffic_influ_info) {
        OpenAPI_traffic_influence_info_free(sm_context->traffic_influ_info);
        sm_context->traffic_influ_info = NULL;
    }
    if (sm_context->pending_update_info_list) {
        OpenAPI_list_free(sm_context->pending_update_info_list);
        sm_context->pending_update_info_list = NULL;
    }
    if (sm_context->eas_info_to_refresh) {
        OpenAPI_eas_info_to_refresh_free(sm_context->eas_info_to_refresh);
        sm_context->eas_info_to_refresh = NULL;
    }
    if (sm_context->dns_addr) {
        OpenAPI_ip_address_free(sm_context->dns_addr);
        sm_context->dns_addr = NULL;
    }
    if (sm_context->psa_upf_addr) {
        OpenAPI_ip_address_free(sm_context->psa_upf_addr);
        sm_context->psa_upf_addr = NULL;
    }
    if (sm_context->local_offloading_info_list) {
        OpenAPI_list_for_each(sm_context->local_offloading_info_list, node) {
            OpenAPI_local_offloading_management_info_free(node->data);
        }
        OpenAPI_list_free(sm_context->local_offloading_info_list);
        sm_context->local_offloading_info_list = NULL;
    }
    if (sm_context->qos_monitoring_info) {
        OpenAPI_qos_monitoring_info_free(sm_context->qos_monitoring_info);
        sm_context->qos_monitoring_info = NULL;
    }
    ogs_free(sm_context);
}

cJSON *OpenAPI_sm_context_convertToJSON(OpenAPI_sm_context_t *sm_context)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_context == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [SmContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "pduSessionId", sm_context->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pdu_session_id]");
        goto end;
    }

    if (!sm_context->dnn) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", sm_context->dnn) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [dnn]");
        goto end;
    }

    if (sm_context->selected_dnn) {
    if (cJSON_AddStringToObject(item, "selectedDnn", sm_context->selected_dnn) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [selected_dnn]");
        goto end;
    }
    }

    if (!sm_context->s_nssai) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [s_nssai]");
        return NULL;
    }
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(sm_context->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [s_nssai]");
        goto end;
    }

    if (sm_context->alt_snssai) {
    cJSON *alt_snssai_local_JSON = OpenAPI_snssai_convertToJSON(sm_context->alt_snssai);
    if (alt_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [alt_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "altSnssai", alt_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [alt_snssai]");
        goto end;
    }
    }

    if (sm_context->hplmn_snssai) {
    cJSON *hplmn_snssai_local_JSON = OpenAPI_snssai_convertToJSON(sm_context->hplmn_snssai);
    if (hplmn_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [hplmn_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hplmnSnssai", hplmn_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [hplmn_snssai]");
        goto end;
    }
    }

    if (sm_context->alt_hplmn_snssai) {
    cJSON *alt_hplmn_snssai_local_JSON = OpenAPI_snssai_convertToJSON(sm_context->alt_hplmn_snssai);
    if (alt_hplmn_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [alt_hplmn_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "altHplmnSnssai", alt_hplmn_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [alt_hplmn_snssai]");
        goto end;
    }
    }

    if (sm_context->pdu_session_type == OpenAPI_pdu_session_type_NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pdu_session_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "pduSessionType", OpenAPI_pdu_session_type_ToString(sm_context->pdu_session_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pdu_session_type]");
        goto end;
    }

    if (sm_context->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", sm_context->gpsi) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (sm_context->h_smf_uri) {
    if (cJSON_AddStringToObject(item, "hSmfUri", sm_context->h_smf_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [h_smf_uri]");
        goto end;
    }
    }

    if (sm_context->smf_uri) {
    if (cJSON_AddStringToObject(item, "smfUri", sm_context->smf_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [smf_uri]");
        goto end;
    }
    }

    if (sm_context->pdu_session_ref) {
    if (cJSON_AddStringToObject(item, "pduSessionRef", sm_context->pdu_session_ref) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pdu_session_ref]");
        goto end;
    }
    }

    if (sm_context->inter_plmn_api_root) {
    if (cJSON_AddStringToObject(item, "interPlmnApiRoot", sm_context->inter_plmn_api_root) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [inter_plmn_api_root]");
        goto end;
    }
    }

    if (sm_context->intra_plmn_api_root) {
    if (cJSON_AddStringToObject(item, "intraPlmnApiRoot", sm_context->intra_plmn_api_root) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [intra_plmn_api_root]");
        goto end;
    }
    }

    if (sm_context->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", sm_context->pcf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (sm_context->pcf_group_id) {
    if (cJSON_AddStringToObject(item, "pcfGroupId", sm_context->pcf_group_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pcf_group_id]");
        goto end;
    }
    }

    if (sm_context->pcf_set_id) {
    if (cJSON_AddStringToObject(item, "pcfSetId", sm_context->pcf_set_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    if (sm_context->sel_mode != OpenAPI_dnn_selection_mode_NULL) {
    if (cJSON_AddStringToObject(item, "selMode", OpenAPI_dnn_selection_mode_ToString(sm_context->sel_mode)) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [sel_mode]");
        goto end;
    }
    }

    if (sm_context->udm_group_id) {
    if (cJSON_AddStringToObject(item, "udmGroupId", sm_context->udm_group_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [udm_group_id]");
        goto end;
    }
    }

    if (sm_context->routing_indicator) {
    if (cJSON_AddStringToObject(item, "routingIndicator", sm_context->routing_indicator) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [routing_indicator]");
        goto end;
    }
    }

    if (sm_context->is_h_nw_pub_key_id) {
    if (cJSON_AddNumberToObject(item, "hNwPubKeyId", sm_context->h_nw_pub_key_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [h_nw_pub_key_id]");
        goto end;
    }
    }

    if (!sm_context->session_ambr) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [session_ambr]");
        return NULL;
    }
    cJSON *session_ambr_local_JSON = OpenAPI_ambr_convertToJSON(sm_context->session_ambr);
    if (session_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [session_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sessionAmbr", session_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [session_ambr]");
        goto end;
    }

    if (!sm_context->qos_flows_list) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [qos_flows_list]");
        return NULL;
    }
    cJSON *qos_flows_listList = cJSON_AddArrayToObject(item, "qosFlowsList");
    if (qos_flows_listList == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [qos_flows_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context->qos_flows_list, node) {
        cJSON *itemLocal = OpenAPI_qos_flow_setup_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [qos_flows_list]");
            goto end;
        }
        cJSON_AddItemToArray(qos_flows_listList, itemLocal);
    }

    if (sm_context->h_smf_instance_id) {
    if (cJSON_AddStringToObject(item, "hSmfInstanceId", sm_context->h_smf_instance_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [h_smf_instance_id]");
        goto end;
    }
    }

    if (sm_context->smf_instance_id) {
    if (cJSON_AddStringToObject(item, "smfInstanceId", sm_context->smf_instance_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [smf_instance_id]");
        goto end;
    }
    }

    if (sm_context->pdu_session_smf_set_id) {
    if (cJSON_AddStringToObject(item, "pduSessionSmfSetId", sm_context->pdu_session_smf_set_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pdu_session_smf_set_id]");
        goto end;
    }
    }

    if (sm_context->pdu_session_smf_service_set_id) {
    if (cJSON_AddStringToObject(item, "pduSessionSmfServiceSetId", sm_context->pdu_session_smf_service_set_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pdu_session_smf_service_set_id]");
        goto end;
    }
    }

    if (sm_context->pdu_session_smf_binding != OpenAPI_sbi_binding_level_NULL) {
    if (cJSON_AddStringToObject(item, "pduSessionSmfBinding", OpenAPI_sbi_binding_level_ToString(sm_context->pdu_session_smf_binding)) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pdu_session_smf_binding]");
        goto end;
    }
    }

    if (sm_context->is_enable_pause_charging) {
    if (cJSON_AddBoolToObject(item, "enablePauseCharging", sm_context->enable_pause_charging) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [enable_pause_charging]");
        goto end;
    }
    }

    if (sm_context->ue_ipv4_address) {
    if (cJSON_AddStringToObject(item, "ueIpv4Address", sm_context->ue_ipv4_address) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [ue_ipv4_address]");
        goto end;
    }
    }

    if (sm_context->ue_ipv6_prefix) {
    if (cJSON_AddStringToObject(item, "ueIpv6Prefix", sm_context->ue_ipv6_prefix) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [ue_ipv6_prefix]");
        goto end;
    }
    }

    if (sm_context->eps_pdn_cnx_info) {
    cJSON *eps_pdn_cnx_info_local_JSON = OpenAPI_eps_pdn_cnx_info_convertToJSON(sm_context->eps_pdn_cnx_info);
    if (eps_pdn_cnx_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [eps_pdn_cnx_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "epsPdnCnxInfo", eps_pdn_cnx_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [eps_pdn_cnx_info]");
        goto end;
    }
    }

    if (sm_context->eps_bearer_info) {
    cJSON *eps_bearer_infoList = cJSON_AddArrayToObject(item, "epsBearerInfo");
    if (eps_bearer_infoList == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [eps_bearer_info]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context->eps_bearer_info, node) {
        cJSON *itemLocal = OpenAPI_eps_bearer_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [eps_bearer_info]");
            goto end;
        }
        cJSON_AddItemToArray(eps_bearer_infoList, itemLocal);
    }
    }

    if (sm_context->max_integrity_protected_data_rate != OpenAPI_max_integrity_protected_data_rate_NULL) {
    if (cJSON_AddStringToObject(item, "maxIntegrityProtectedDataRate", OpenAPI_max_integrity_protected_data_rate_ToString(sm_context->max_integrity_protected_data_rate)) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [max_integrity_protected_data_rate]");
        goto end;
    }
    }

    if (sm_context->max_integrity_protected_data_rate_dl != OpenAPI_max_integrity_protected_data_rate_NULL) {
    if (cJSON_AddStringToObject(item, "maxIntegrityProtectedDataRateDl", OpenAPI_max_integrity_protected_data_rate_ToString(sm_context->max_integrity_protected_data_rate_dl)) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [max_integrity_protected_data_rate_dl]");
        goto end;
    }
    }

    if (sm_context->is_always_on_granted) {
    if (cJSON_AddBoolToObject(item, "alwaysOnGranted", sm_context->always_on_granted) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [always_on_granted]");
        goto end;
    }
    }

    if (sm_context->up_security) {
    cJSON *up_security_local_JSON = OpenAPI_up_security_convertToJSON(sm_context->up_security);
    if (up_security_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [up_security]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upSecurity", up_security_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [up_security]");
        goto end;
    }
    }

    if (sm_context->h_smf_service_instance_id) {
    if (cJSON_AddStringToObject(item, "hSmfServiceInstanceId", sm_context->h_smf_service_instance_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [h_smf_service_instance_id]");
        goto end;
    }
    }

    if (sm_context->smf_service_instance_id) {
    if (cJSON_AddStringToObject(item, "smfServiceInstanceId", sm_context->smf_service_instance_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [smf_service_instance_id]");
        goto end;
    }
    }

    if (sm_context->recovery_time) {
    if (cJSON_AddStringToObject(item, "recoveryTime", sm_context->recovery_time) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [recovery_time]");
        goto end;
    }
    }

    if (sm_context->is_forwarding_ind) {
    if (cJSON_AddBoolToObject(item, "forwardingInd", sm_context->forwarding_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [forwarding_ind]");
        goto end;
    }
    }

    if (sm_context->psa_tunnel_info) {
    cJSON *psa_tunnel_info_local_JSON = OpenAPI_tunnel_info_convertToJSON(sm_context->psa_tunnel_info);
    if (psa_tunnel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [psa_tunnel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "psaTunnelInfo", psa_tunnel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [psa_tunnel_info]");
        goto end;
    }
    }

    if (sm_context->charging_id) {
    if (cJSON_AddStringToObject(item, "chargingId", sm_context->charging_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [charging_id]");
        goto end;
    }
    }

    if (sm_context->smf_charging_id) {
    if (cJSON_AddStringToObject(item, "smfChargingId", sm_context->smf_charging_id) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [smf_charging_id]");
        goto end;
    }
    }

    if (sm_context->charging_info) {
    cJSON *charging_info_local_JSON = OpenAPI_charging_information_convertToJSON(sm_context->charging_info);
    if (charging_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [charging_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "chargingInfo", charging_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [charging_info]");
        goto end;
    }
    }

    if (sm_context->roaming_charging_profile) {
    cJSON *roaming_charging_profile_local_JSON = OpenAPI_roaming_charging_profile_convertToJSON(sm_context->roaming_charging_profile);
    if (roaming_charging_profile_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [roaming_charging_profile]");
        goto end;
    }
    cJSON_AddItemToObject(item, "roamingChargingProfile", roaming_charging_profile_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [roaming_charging_profile]");
        goto end;
    }
    }

    if (sm_context->is_nef_ext_buf_support_ind) {
    if (cJSON_AddBoolToObject(item, "nefExtBufSupportInd", sm_context->nef_ext_buf_support_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [nef_ext_buf_support_ind]");
        goto end;
    }
    }

    if (sm_context->is_ipv6_index) {
    if (cJSON_AddNumberToObject(item, "ipv6Index", sm_context->ipv6_index) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [ipv6_index]");
        goto end;
    }
    }

    if (sm_context->dn_aaa_address) {
    cJSON *dn_aaa_address_local_JSON = OpenAPI_ip_address_convertToJSON(sm_context->dn_aaa_address);
    if (dn_aaa_address_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [dn_aaa_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dnAaaAddress", dn_aaa_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [dn_aaa_address]");
        goto end;
    }
    }

    if (sm_context->redundant_pdu_session_info) {
    cJSON *redundant_pdu_session_info_local_JSON = OpenAPI_redundant_pdu_session_information_convertToJSON(sm_context->redundant_pdu_session_info);
    if (redundant_pdu_session_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [redundant_pdu_session_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "redundantPduSessionInfo", redundant_pdu_session_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [redundant_pdu_session_info]");
        goto end;
    }
    }

    if (sm_context->ran_tunnel_info) {
    cJSON *ran_tunnel_info_local_JSON = OpenAPI_qos_flow_tunnel_convertToJSON(sm_context->ran_tunnel_info);
    if (ran_tunnel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [ran_tunnel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ranTunnelInfo", ran_tunnel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [ran_tunnel_info]");
        goto end;
    }
    }

    if (sm_context->add_ran_tunnel_info) {
    cJSON *add_ran_tunnel_infoList = cJSON_AddArrayToObject(item, "addRanTunnelInfo");
    if (add_ran_tunnel_infoList == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [add_ran_tunnel_info]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context->add_ran_tunnel_info, node) {
        cJSON *itemLocal = OpenAPI_qos_flow_tunnel_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [add_ran_tunnel_info]");
            goto end;
        }
        cJSON_AddItemToArray(add_ran_tunnel_infoList, itemLocal);
    }
    }

    if (sm_context->red_ran_tunnel_info) {
    cJSON *red_ran_tunnel_info_local_JSON = OpenAPI_qos_flow_tunnel_convertToJSON(sm_context->red_ran_tunnel_info);
    if (red_ran_tunnel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [red_ran_tunnel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "redRanTunnelInfo", red_ran_tunnel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [red_ran_tunnel_info]");
        goto end;
    }
    }

    if (sm_context->add_red_ran_tunnel_info) {
    cJSON *add_red_ran_tunnel_infoList = cJSON_AddArrayToObject(item, "addRedRanTunnelInfo");
    if (add_red_ran_tunnel_infoList == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [add_red_ran_tunnel_info]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context->add_red_ran_tunnel_info, node) {
        cJSON *itemLocal = OpenAPI_qos_flow_tunnel_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [add_red_ran_tunnel_info]");
            goto end;
        }
        cJSON_AddItemToArray(add_red_ran_tunnel_infoList, itemLocal);
    }
    }

    if (sm_context->is_nspu_support_ind) {
    if (cJSON_AddBoolToObject(item, "nspuSupportInd", sm_context->nspu_support_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [nspu_support_ind]");
        goto end;
    }
    }

    if (sm_context->smf_binding_info) {
    if (cJSON_AddStringToObject(item, "smfBindingInfo", sm_context->smf_binding_info) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [smf_binding_info]");
        goto end;
    }
    }

    if (sm_context->satellite_backhaul_cat != OpenAPI_satellite_backhaul_category_NULL) {
    if (cJSON_AddStringToObject(item, "satelliteBackhaulCat", OpenAPI_satellite_backhaul_category_ToString(sm_context->satellite_backhaul_cat)) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [satellite_backhaul_cat]");
        goto end;
    }
    }

    if (sm_context->ssc_mode) {
    if (cJSON_AddStringToObject(item, "sscMode", sm_context->ssc_mode) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [ssc_mode]");
        goto end;
    }
    }

    if (sm_context->is_dlset_support_ind) {
    if (cJSON_AddBoolToObject(item, "dlsetSupportInd", sm_context->dlset_support_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [dlset_support_ind]");
        goto end;
    }
    }

    if (sm_context->is_n9fsc_support_ind) {
    if (cJSON_AddBoolToObject(item, "n9fscSupportInd", sm_context->n9fsc_support_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [n9fsc_support_ind]");
        goto end;
    }
    }

    if (sm_context->is_anchor_smf_oauth2_required) {
    if (cJSON_AddBoolToObject(item, "anchorSmfOauth2Required", sm_context->anchor_smf_oauth2_required) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [anchor_smf_oauth2_required]");
        goto end;
    }
    }

    if (sm_context->full_dnai_list) {
    cJSON *full_dnai_listList = cJSON_AddArrayToObject(item, "fullDnaiList");
    if (full_dnai_listList == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [full_dnai_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context->full_dnai_list, node) {
        if (cJSON_AddStringToObject(full_dnai_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [full_dnai_list]");
            goto end;
        }
    }
    }

    if (sm_context->is_hrsbo_auth_req_ind) {
    if (cJSON_AddBoolToObject(item, "hrsboAuthReqInd", sm_context->hrsbo_auth_req_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [hrsbo_auth_req_ind]");
        goto end;
    }
    }

    if (sm_context->h_dns_addr) {
    cJSON *h_dns_addr_local_JSON = OpenAPI_ip_address_convertToJSON(sm_context->h_dns_addr);
    if (h_dns_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [h_dns_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hDnsAddr", h_dns_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [h_dns_addr]");
        goto end;
    }
    }

    if (sm_context->h_plmn_addr) {
    cJSON *h_plmn_addr_local_JSON = OpenAPI_ip_address_convertToJSON(sm_context->h_plmn_addr);
    if (h_plmn_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [h_plmn_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hPlmnAddr", h_plmn_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [h_plmn_addr]");
        goto end;
    }
    }

    if (sm_context->vplmn_offloading_info_list) {
    cJSON *vplmn_offloading_info_listList = cJSON_AddArrayToObject(item, "vplmnOffloadingInfoList");
    if (vplmn_offloading_info_listList == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [vplmn_offloading_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context->vplmn_offloading_info_list, node) {
        cJSON *itemLocal = OpenAPI_vplmn_offloading_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [vplmn_offloading_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(vplmn_offloading_info_listList, itemLocal);
    }
    }

    if (sm_context->vplmn_dl_ambr) {
    cJSON *vplmn_dl_ambr_local_JSON = OpenAPI_vplmn_dl_ambr_convertToJSON(sm_context->vplmn_dl_ambr);
    if (vplmn_dl_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [vplmn_dl_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vplmnDlAmbr", vplmn_dl_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [vplmn_dl_ambr]");
        goto end;
    }
    } else if (sm_context->is_vplmn_dl_ambr_null) {
        if (cJSON_AddNullToObject(item, "vplmnDlAmbr") == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [vplmn_dl_ambr]");
            goto end;
        }
    }

    if (sm_context->offload_ids) {
    cJSON *offload_idsList = cJSON_AddArrayToObject(item, "offloadIds");
    if (offload_idsList == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [offload_ids]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context->offload_ids, node) {
        if (cJSON_AddStringToObject(offload_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [offload_ids]");
            goto end;
        }
    }
    }

    if (sm_context->target_dnai) {
    if (cJSON_AddStringToObject(item, "targetDnai", sm_context->target_dnai) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [target_dnai]");
        goto end;
    }
    }

    if (sm_context->traffic_influ_info) {
    cJSON *traffic_influ_info_local_JSON = OpenAPI_traffic_influence_info_convertToJSON(sm_context->traffic_influ_info);
    if (traffic_influ_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [traffic_influ_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "trafficInfluInfo", traffic_influ_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [traffic_influ_info]");
        goto end;
    }
    }

    if (sm_context->pending_update_info_list != OpenAPI_pending_update_info_NULL) {
    cJSON *pending_update_info_listList = cJSON_AddArrayToObject(item, "pendingUpdateInfoList");
    if (pending_update_info_listList == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [pending_update_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context->pending_update_info_list, node) {
        if (cJSON_AddStringToObject(pending_update_info_listList, "", OpenAPI_pending_update_info_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [pending_update_info_list]");
            goto end;
        }
    }
    }

    if (sm_context->eas_info_to_refresh) {
    cJSON *eas_info_to_refresh_local_JSON = OpenAPI_eas_info_to_refresh_convertToJSON(sm_context->eas_info_to_refresh);
    if (eas_info_to_refresh_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [eas_info_to_refresh]");
        goto end;
    }
    cJSON_AddItemToObject(item, "easInfoToRefresh", eas_info_to_refresh_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [eas_info_to_refresh]");
        goto end;
    }
    }

    if (sm_context->is_local_offloading_mgt_allowed_ind) {
    if (cJSON_AddBoolToObject(item, "localOffloadingMgtAllowedInd", sm_context->local_offloading_mgt_allowed_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [local_offloading_mgt_allowed_ind]");
        goto end;
    }
    }

    if (sm_context->dns_addr) {
    cJSON *dns_addr_local_JSON = OpenAPI_ip_address_convertToJSON(sm_context->dns_addr);
    if (dns_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [dns_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dnsAddr", dns_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [dns_addr]");
        goto end;
    }
    }

    if (sm_context->psa_upf_addr) {
    cJSON *psa_upf_addr_local_JSON = OpenAPI_ip_address_convertToJSON(sm_context->psa_upf_addr);
    if (psa_upf_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [psa_upf_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "psaUpfAddr", psa_upf_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [psa_upf_addr]");
        goto end;
    }
    }

    if (sm_context->local_offloading_info_list) {
    cJSON *local_offloading_info_listList = cJSON_AddArrayToObject(item, "localOffloadingInfoList");
    if (local_offloading_info_listList == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [local_offloading_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context->local_offloading_info_list, node) {
        cJSON *itemLocal = OpenAPI_local_offloading_management_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_convertToJSON() failed [local_offloading_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(local_offloading_info_listList, itemLocal);
    }
    }

    if (sm_context->is_priority_user_ind) {
    if (cJSON_AddBoolToObject(item, "priorityUserInd", sm_context->priority_user_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [priority_user_ind]");
        goto end;
    }
    }

    if (sm_context->qos_monitoring_info) {
    cJSON *qos_monitoring_info_local_JSON = OpenAPI_qos_monitoring_info_convertToJSON(sm_context->qos_monitoring_info);
    if (qos_monitoring_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [qos_monitoring_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qosMonitoringInfo", qos_monitoring_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed [qos_monitoring_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_context_t *OpenAPI_sm_context_parseFromJSON(cJSON *sm_contextJSON)
{
    OpenAPI_sm_context_t *sm_context_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_session_id = NULL;
    cJSON *dnn = NULL;
    cJSON *selected_dnn = NULL;
    cJSON *s_nssai = NULL;
    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    cJSON *alt_snssai = NULL;
    OpenAPI_snssai_t *alt_snssai_local_nonprim = NULL;
    cJSON *hplmn_snssai = NULL;
    OpenAPI_snssai_t *hplmn_snssai_local_nonprim = NULL;
    cJSON *alt_hplmn_snssai = NULL;
    OpenAPI_snssai_t *alt_hplmn_snssai_local_nonprim = NULL;
    cJSON *pdu_session_type = NULL;
    OpenAPI_pdu_session_type_e pdu_session_typeVariable = 0;
    cJSON *gpsi = NULL;
    cJSON *h_smf_uri = NULL;
    cJSON *smf_uri = NULL;
    cJSON *pdu_session_ref = NULL;
    cJSON *inter_plmn_api_root = NULL;
    cJSON *intra_plmn_api_root = NULL;
    cJSON *pcf_id = NULL;
    cJSON *pcf_group_id = NULL;
    cJSON *pcf_set_id = NULL;
    cJSON *sel_mode = NULL;
    OpenAPI_dnn_selection_mode_e sel_modeVariable = 0;
    cJSON *udm_group_id = NULL;
    cJSON *routing_indicator = NULL;
    cJSON *h_nw_pub_key_id = NULL;
    cJSON *session_ambr = NULL;
    OpenAPI_ambr_t *session_ambr_local_nonprim = NULL;
    cJSON *qos_flows_list = NULL;
    OpenAPI_list_t *qos_flows_listList = NULL;
    cJSON *h_smf_instance_id = NULL;
    cJSON *smf_instance_id = NULL;
    cJSON *pdu_session_smf_set_id = NULL;
    cJSON *pdu_session_smf_service_set_id = NULL;
    cJSON *pdu_session_smf_binding = NULL;
    OpenAPI_sbi_binding_level_e pdu_session_smf_bindingVariable = 0;
    cJSON *enable_pause_charging = NULL;
    cJSON *ue_ipv4_address = NULL;
    cJSON *ue_ipv6_prefix = NULL;
    cJSON *eps_pdn_cnx_info = NULL;
    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info_local_nonprim = NULL;
    cJSON *eps_bearer_info = NULL;
    OpenAPI_list_t *eps_bearer_infoList = NULL;
    cJSON *max_integrity_protected_data_rate = NULL;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rateVariable = 0;
    cJSON *max_integrity_protected_data_rate_dl = NULL;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dlVariable = 0;
    cJSON *always_on_granted = NULL;
    cJSON *up_security = NULL;
    OpenAPI_up_security_t *up_security_local_nonprim = NULL;
    cJSON *h_smf_service_instance_id = NULL;
    cJSON *smf_service_instance_id = NULL;
    cJSON *recovery_time = NULL;
    cJSON *forwarding_ind = NULL;
    cJSON *psa_tunnel_info = NULL;
    OpenAPI_tunnel_info_t *psa_tunnel_info_local_nonprim = NULL;
    cJSON *charging_id = NULL;
    cJSON *smf_charging_id = NULL;
    cJSON *charging_info = NULL;
    OpenAPI_charging_information_t *charging_info_local_nonprim = NULL;
    cJSON *roaming_charging_profile = NULL;
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile_local_nonprim = NULL;
    cJSON *nef_ext_buf_support_ind = NULL;
    cJSON *ipv6_index = NULL;
    cJSON *dn_aaa_address = NULL;
    OpenAPI_ip_address_t *dn_aaa_address_local_nonprim = NULL;
    cJSON *redundant_pdu_session_info = NULL;
    OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_info_local_nonprim = NULL;
    cJSON *ran_tunnel_info = NULL;
    OpenAPI_qos_flow_tunnel_t *ran_tunnel_info_local_nonprim = NULL;
    cJSON *add_ran_tunnel_info = NULL;
    OpenAPI_list_t *add_ran_tunnel_infoList = NULL;
    cJSON *red_ran_tunnel_info = NULL;
    OpenAPI_qos_flow_tunnel_t *red_ran_tunnel_info_local_nonprim = NULL;
    cJSON *add_red_ran_tunnel_info = NULL;
    OpenAPI_list_t *add_red_ran_tunnel_infoList = NULL;
    cJSON *nspu_support_ind = NULL;
    cJSON *smf_binding_info = NULL;
    cJSON *satellite_backhaul_cat = NULL;
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_catVariable = 0;
    cJSON *ssc_mode = NULL;
    cJSON *dlset_support_ind = NULL;
    cJSON *n9fsc_support_ind = NULL;
    cJSON *anchor_smf_oauth2_required = NULL;
    cJSON *full_dnai_list = NULL;
    OpenAPI_list_t *full_dnai_listList = NULL;
    cJSON *hrsbo_auth_req_ind = NULL;
    cJSON *h_dns_addr = NULL;
    OpenAPI_ip_address_t *h_dns_addr_local_nonprim = NULL;
    cJSON *h_plmn_addr = NULL;
    OpenAPI_ip_address_t *h_plmn_addr_local_nonprim = NULL;
    cJSON *vplmn_offloading_info_list = NULL;
    OpenAPI_list_t *vplmn_offloading_info_listList = NULL;
    cJSON *vplmn_dl_ambr = NULL;
    OpenAPI_vplmn_dl_ambr_t *vplmn_dl_ambr_local_nonprim = NULL;
    cJSON *offload_ids = NULL;
    OpenAPI_list_t *offload_idsList = NULL;
    cJSON *target_dnai = NULL;
    cJSON *traffic_influ_info = NULL;
    OpenAPI_traffic_influence_info_t *traffic_influ_info_local_nonprim = NULL;
    cJSON *pending_update_info_list = NULL;
    OpenAPI_list_t *pending_update_info_listList = NULL;
    cJSON *eas_info_to_refresh = NULL;
    OpenAPI_eas_info_to_refresh_t *eas_info_to_refresh_local_nonprim = NULL;
    cJSON *local_offloading_mgt_allowed_ind = NULL;
    cJSON *dns_addr = NULL;
    OpenAPI_ip_address_t *dns_addr_local_nonprim = NULL;
    cJSON *psa_upf_addr = NULL;
    OpenAPI_ip_address_t *psa_upf_addr_local_nonprim = NULL;
    cJSON *local_offloading_info_list = NULL;
    OpenAPI_list_t *local_offloading_info_listList = NULL;
    cJSON *priority_user_ind = NULL;
    cJSON *qos_monitoring_info = NULL;
    OpenAPI_qos_monitoring_info_t *qos_monitoring_info_local_nonprim = NULL;
    pdu_session_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pduSessionId");
    if (!pdu_session_id) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    dnn = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [dnn]");
        goto end;
    }

    selected_dnn = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "selectedDnn");
    if (selected_dnn) {
    if (!cJSON_IsString(selected_dnn) && !cJSON_IsNull(selected_dnn)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [selected_dnn]");
        goto end;
    }
    }

    s_nssai = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "sNssai");
    if (!s_nssai) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [s_nssai]");
        goto end;
    }
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    if (!s_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [s_nssai]");
        goto end;
    }

    alt_snssai = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "altSnssai");
    if (alt_snssai) {
    alt_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(alt_snssai);
    if (!alt_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [alt_snssai]");
        goto end;
    }
    }

    hplmn_snssai = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "hplmnSnssai");
    if (hplmn_snssai) {
    hplmn_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(hplmn_snssai);
    if (!hplmn_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [hplmn_snssai]");
        goto end;
    }
    }

    alt_hplmn_snssai = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "altHplmnSnssai");
    if (alt_hplmn_snssai) {
    alt_hplmn_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(alt_hplmn_snssai);
    if (!alt_hplmn_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [alt_hplmn_snssai]");
        goto end;
    }
    }

    pdu_session_type = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pduSessionType");
    if (!pdu_session_type) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pdu_session_type]");
        goto end;
    }
    if (!cJSON_IsString(pdu_session_type)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pdu_session_type]");
        goto end;
    }
    pdu_session_typeVariable = OpenAPI_pdu_session_type_FromString(pdu_session_type->valuestring);

    gpsi = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    h_smf_uri = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "hSmfUri");
    if (h_smf_uri) {
    if (!cJSON_IsString(h_smf_uri) && !cJSON_IsNull(h_smf_uri)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [h_smf_uri]");
        goto end;
    }
    }

    smf_uri = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "smfUri");
    if (smf_uri) {
    if (!cJSON_IsString(smf_uri) && !cJSON_IsNull(smf_uri)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [smf_uri]");
        goto end;
    }
    }

    pdu_session_ref = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pduSessionRef");
    if (pdu_session_ref) {
    if (!cJSON_IsString(pdu_session_ref) && !cJSON_IsNull(pdu_session_ref)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pdu_session_ref]");
        goto end;
    }
    }

    inter_plmn_api_root = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "interPlmnApiRoot");
    if (inter_plmn_api_root) {
    if (!cJSON_IsString(inter_plmn_api_root) && !cJSON_IsNull(inter_plmn_api_root)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [inter_plmn_api_root]");
        goto end;
    }
    }

    intra_plmn_api_root = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "intraPlmnApiRoot");
    if (intra_plmn_api_root) {
    if (!cJSON_IsString(intra_plmn_api_root) && !cJSON_IsNull(intra_plmn_api_root)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [intra_plmn_api_root]");
        goto end;
    }
    }

    pcf_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_group_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pcfGroupId");
    if (pcf_group_id) {
    if (!cJSON_IsString(pcf_group_id) && !cJSON_IsNull(pcf_group_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pcf_group_id]");
        goto end;
    }
    }

    pcf_set_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pcfSetId");
    if (pcf_set_id) {
    if (!cJSON_IsString(pcf_set_id) && !cJSON_IsNull(pcf_set_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    sel_mode = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "selMode");
    if (sel_mode) {
    if (!cJSON_IsString(sel_mode)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [sel_mode]");
        goto end;
    }
    sel_modeVariable = OpenAPI_dnn_selection_mode_FromString(sel_mode->valuestring);
    }

    udm_group_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "udmGroupId");
    if (udm_group_id) {
    if (!cJSON_IsString(udm_group_id) && !cJSON_IsNull(udm_group_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [udm_group_id]");
        goto end;
    }
    }

    routing_indicator = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "routingIndicator");
    if (routing_indicator) {
    if (!cJSON_IsString(routing_indicator) && !cJSON_IsNull(routing_indicator)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [routing_indicator]");
        goto end;
    }
    }

    h_nw_pub_key_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "hNwPubKeyId");
    if (h_nw_pub_key_id) {
    if (!cJSON_IsNumber(h_nw_pub_key_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [h_nw_pub_key_id]");
        goto end;
    }
    }

    session_ambr = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "sessionAmbr");
    if (!session_ambr) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [session_ambr]");
        goto end;
    }
    session_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(session_ambr);
    if (!session_ambr_local_nonprim) {
        ogs_error("OpenAPI_ambr_parseFromJSON failed [session_ambr]");
        goto end;
    }

    qos_flows_list = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "qosFlowsList");
    if (!qos_flows_list) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [qos_flows_list]");
        goto end;
    }
        cJSON *qos_flows_list_local = NULL;
        if (!cJSON_IsArray(qos_flows_list)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [qos_flows_list]");
            goto end;
        }

        qos_flows_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_flows_list_local, qos_flows_list) {
            if (!cJSON_IsObject(qos_flows_list_local)) {
                ogs_error("OpenAPI_sm_context_parseFromJSON() failed [qos_flows_list]");
                goto end;
            }
            OpenAPI_qos_flow_setup_item_t *qos_flows_listItem = OpenAPI_qos_flow_setup_item_parseFromJSON(qos_flows_list_local);
            if (!qos_flows_listItem) {
                ogs_error("No qos_flows_listItem");
                goto end;
            }
            OpenAPI_list_add(qos_flows_listList, qos_flows_listItem);
        }

    h_smf_instance_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "hSmfInstanceId");
    if (h_smf_instance_id) {
    if (!cJSON_IsString(h_smf_instance_id) && !cJSON_IsNull(h_smf_instance_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [h_smf_instance_id]");
        goto end;
    }
    }

    smf_instance_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "smfInstanceId");
    if (smf_instance_id) {
    if (!cJSON_IsString(smf_instance_id) && !cJSON_IsNull(smf_instance_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }
    }

    pdu_session_smf_set_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pduSessionSmfSetId");
    if (pdu_session_smf_set_id) {
    if (!cJSON_IsString(pdu_session_smf_set_id) && !cJSON_IsNull(pdu_session_smf_set_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pdu_session_smf_set_id]");
        goto end;
    }
    }

    pdu_session_smf_service_set_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pduSessionSmfServiceSetId");
    if (pdu_session_smf_service_set_id) {
    if (!cJSON_IsString(pdu_session_smf_service_set_id) && !cJSON_IsNull(pdu_session_smf_service_set_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pdu_session_smf_service_set_id]");
        goto end;
    }
    }

    pdu_session_smf_binding = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pduSessionSmfBinding");
    if (pdu_session_smf_binding) {
    if (!cJSON_IsString(pdu_session_smf_binding)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pdu_session_smf_binding]");
        goto end;
    }
    pdu_session_smf_bindingVariable = OpenAPI_sbi_binding_level_FromString(pdu_session_smf_binding->valuestring);
    }

    enable_pause_charging = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "enablePauseCharging");
    if (enable_pause_charging) {
    if (!cJSON_IsBool(enable_pause_charging)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [enable_pause_charging]");
        goto end;
    }
    }

    ue_ipv4_address = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "ueIpv4Address");
    if (ue_ipv4_address) {
    if (!cJSON_IsString(ue_ipv4_address) && !cJSON_IsNull(ue_ipv4_address)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [ue_ipv4_address]");
        goto end;
    }
    }

    ue_ipv6_prefix = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "ueIpv6Prefix");
    if (ue_ipv6_prefix) {
    if (!cJSON_IsString(ue_ipv6_prefix) && !cJSON_IsNull(ue_ipv6_prefix)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [ue_ipv6_prefix]");
        goto end;
    }
    }

    eps_pdn_cnx_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "epsPdnCnxInfo");
    if (eps_pdn_cnx_info) {
    eps_pdn_cnx_info_local_nonprim = OpenAPI_eps_pdn_cnx_info_parseFromJSON(eps_pdn_cnx_info);
    if (!eps_pdn_cnx_info_local_nonprim) {
        ogs_error("OpenAPI_eps_pdn_cnx_info_parseFromJSON failed [eps_pdn_cnx_info]");
        goto end;
    }
    }

    eps_bearer_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "epsBearerInfo");
    if (eps_bearer_info) {
        cJSON *eps_bearer_info_local = NULL;
        if (!cJSON_IsArray(eps_bearer_info)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [eps_bearer_info]");
            goto end;
        }

        eps_bearer_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(eps_bearer_info_local, eps_bearer_info) {
            if (!cJSON_IsObject(eps_bearer_info_local)) {
                ogs_error("OpenAPI_sm_context_parseFromJSON() failed [eps_bearer_info]");
                goto end;
            }
            OpenAPI_eps_bearer_info_t *eps_bearer_infoItem = OpenAPI_eps_bearer_info_parseFromJSON(eps_bearer_info_local);
            if (!eps_bearer_infoItem) {
                ogs_error("No eps_bearer_infoItem");
                goto end;
            }
            OpenAPI_list_add(eps_bearer_infoList, eps_bearer_infoItem);
        }
    }

    max_integrity_protected_data_rate = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "maxIntegrityProtectedDataRate");
    if (max_integrity_protected_data_rate) {
    if (!cJSON_IsString(max_integrity_protected_data_rate)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [max_integrity_protected_data_rate]");
        goto end;
    }
    max_integrity_protected_data_rateVariable = OpenAPI_max_integrity_protected_data_rate_FromString(max_integrity_protected_data_rate->valuestring);
    }

    max_integrity_protected_data_rate_dl = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "maxIntegrityProtectedDataRateDl");
    if (max_integrity_protected_data_rate_dl) {
    if (!cJSON_IsString(max_integrity_protected_data_rate_dl)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [max_integrity_protected_data_rate_dl]");
        goto end;
    }
    max_integrity_protected_data_rate_dlVariable = OpenAPI_max_integrity_protected_data_rate_FromString(max_integrity_protected_data_rate_dl->valuestring);
    }

    always_on_granted = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "alwaysOnGranted");
    if (always_on_granted) {
    if (!cJSON_IsBool(always_on_granted)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [always_on_granted]");
        goto end;
    }
    }

    up_security = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "upSecurity");
    if (up_security) {
    up_security_local_nonprim = OpenAPI_up_security_parseFromJSON(up_security);
    if (!up_security_local_nonprim) {
        ogs_error("OpenAPI_up_security_parseFromJSON failed [up_security]");
        goto end;
    }
    }

    h_smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "hSmfServiceInstanceId");
    if (h_smf_service_instance_id) {
    if (!cJSON_IsString(h_smf_service_instance_id) && !cJSON_IsNull(h_smf_service_instance_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [h_smf_service_instance_id]");
        goto end;
    }
    }

    smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "smfServiceInstanceId");
    if (smf_service_instance_id) {
    if (!cJSON_IsString(smf_service_instance_id) && !cJSON_IsNull(smf_service_instance_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [smf_service_instance_id]");
        goto end;
    }
    }

    recovery_time = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "recoveryTime");
    if (recovery_time) {
    if (!cJSON_IsString(recovery_time) && !cJSON_IsNull(recovery_time)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [recovery_time]");
        goto end;
    }
    }

    forwarding_ind = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "forwardingInd");
    if (forwarding_ind) {
    if (!cJSON_IsBool(forwarding_ind)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [forwarding_ind]");
        goto end;
    }
    }

    psa_tunnel_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "psaTunnelInfo");
    if (psa_tunnel_info) {
    psa_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(psa_tunnel_info);
    if (!psa_tunnel_info_local_nonprim) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON failed [psa_tunnel_info]");
        goto end;
    }
    }

    charging_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "chargingId");
    if (charging_id) {
    if (!cJSON_IsString(charging_id) && !cJSON_IsNull(charging_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [charging_id]");
        goto end;
    }
    }

    smf_charging_id = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "smfChargingId");
    if (smf_charging_id) {
    if (!cJSON_IsString(smf_charging_id) && !cJSON_IsNull(smf_charging_id)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [smf_charging_id]");
        goto end;
    }
    }

    charging_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "chargingInfo");
    if (charging_info) {
    charging_info_local_nonprim = OpenAPI_charging_information_parseFromJSON(charging_info);
    if (!charging_info_local_nonprim) {
        ogs_error("OpenAPI_charging_information_parseFromJSON failed [charging_info]");
        goto end;
    }
    }

    roaming_charging_profile = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "roamingChargingProfile");
    if (roaming_charging_profile) {
    roaming_charging_profile_local_nonprim = OpenAPI_roaming_charging_profile_parseFromJSON(roaming_charging_profile);
    if (!roaming_charging_profile_local_nonprim) {
        ogs_error("OpenAPI_roaming_charging_profile_parseFromJSON failed [roaming_charging_profile]");
        goto end;
    }
    }

    nef_ext_buf_support_ind = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "nefExtBufSupportInd");
    if (nef_ext_buf_support_ind) {
    if (!cJSON_IsBool(nef_ext_buf_support_ind)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [nef_ext_buf_support_ind]");
        goto end;
    }
    }

    ipv6_index = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "ipv6Index");
    if (ipv6_index) {
    if (!cJSON_IsNumber(ipv6_index)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [ipv6_index]");
        goto end;
    }
    }

    dn_aaa_address = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "dnAaaAddress");
    if (dn_aaa_address) {
    dn_aaa_address_local_nonprim = OpenAPI_ip_address_parseFromJSON(dn_aaa_address);
    if (!dn_aaa_address_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [dn_aaa_address]");
        goto end;
    }
    }

    redundant_pdu_session_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "redundantPduSessionInfo");
    if (redundant_pdu_session_info) {
    redundant_pdu_session_info_local_nonprim = OpenAPI_redundant_pdu_session_information_parseFromJSON(redundant_pdu_session_info);
    if (!redundant_pdu_session_info_local_nonprim) {
        ogs_error("OpenAPI_redundant_pdu_session_information_parseFromJSON failed [redundant_pdu_session_info]");
        goto end;
    }
    }

    ran_tunnel_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "ranTunnelInfo");
    if (ran_tunnel_info) {
    ran_tunnel_info_local_nonprim = OpenAPI_qos_flow_tunnel_parseFromJSON(ran_tunnel_info);
    if (!ran_tunnel_info_local_nonprim) {
        ogs_error("OpenAPI_qos_flow_tunnel_parseFromJSON failed [ran_tunnel_info]");
        goto end;
    }
    }

    add_ran_tunnel_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "addRanTunnelInfo");
    if (add_ran_tunnel_info) {
        cJSON *add_ran_tunnel_info_local = NULL;
        if (!cJSON_IsArray(add_ran_tunnel_info)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [add_ran_tunnel_info]");
            goto end;
        }

        add_ran_tunnel_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_ran_tunnel_info_local, add_ran_tunnel_info) {
            if (!cJSON_IsObject(add_ran_tunnel_info_local)) {
                ogs_error("OpenAPI_sm_context_parseFromJSON() failed [add_ran_tunnel_info]");
                goto end;
            }
            OpenAPI_qos_flow_tunnel_t *add_ran_tunnel_infoItem = OpenAPI_qos_flow_tunnel_parseFromJSON(add_ran_tunnel_info_local);
            if (!add_ran_tunnel_infoItem) {
                ogs_error("No add_ran_tunnel_infoItem");
                goto end;
            }
            OpenAPI_list_add(add_ran_tunnel_infoList, add_ran_tunnel_infoItem);
        }
    }

    red_ran_tunnel_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "redRanTunnelInfo");
    if (red_ran_tunnel_info) {
    red_ran_tunnel_info_local_nonprim = OpenAPI_qos_flow_tunnel_parseFromJSON(red_ran_tunnel_info);
    if (!red_ran_tunnel_info_local_nonprim) {
        ogs_error("OpenAPI_qos_flow_tunnel_parseFromJSON failed [red_ran_tunnel_info]");
        goto end;
    }
    }

    add_red_ran_tunnel_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "addRedRanTunnelInfo");
    if (add_red_ran_tunnel_info) {
        cJSON *add_red_ran_tunnel_info_local = NULL;
        if (!cJSON_IsArray(add_red_ran_tunnel_info)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [add_red_ran_tunnel_info]");
            goto end;
        }

        add_red_ran_tunnel_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_red_ran_tunnel_info_local, add_red_ran_tunnel_info) {
            if (!cJSON_IsObject(add_red_ran_tunnel_info_local)) {
                ogs_error("OpenAPI_sm_context_parseFromJSON() failed [add_red_ran_tunnel_info]");
                goto end;
            }
            OpenAPI_qos_flow_tunnel_t *add_red_ran_tunnel_infoItem = OpenAPI_qos_flow_tunnel_parseFromJSON(add_red_ran_tunnel_info_local);
            if (!add_red_ran_tunnel_infoItem) {
                ogs_error("No add_red_ran_tunnel_infoItem");
                goto end;
            }
            OpenAPI_list_add(add_red_ran_tunnel_infoList, add_red_ran_tunnel_infoItem);
        }
    }

    nspu_support_ind = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "nspuSupportInd");
    if (nspu_support_ind) {
    if (!cJSON_IsBool(nspu_support_ind)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [nspu_support_ind]");
        goto end;
    }
    }

    smf_binding_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "smfBindingInfo");
    if (smf_binding_info) {
    if (!cJSON_IsString(smf_binding_info) && !cJSON_IsNull(smf_binding_info)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [smf_binding_info]");
        goto end;
    }
    }

    satellite_backhaul_cat = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "satelliteBackhaulCat");
    if (satellite_backhaul_cat) {
    if (!cJSON_IsString(satellite_backhaul_cat)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [satellite_backhaul_cat]");
        goto end;
    }
    satellite_backhaul_catVariable = OpenAPI_satellite_backhaul_category_FromString(satellite_backhaul_cat->valuestring);
    }

    ssc_mode = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "sscMode");
    if (ssc_mode) {
    if (!cJSON_IsString(ssc_mode) && !cJSON_IsNull(ssc_mode)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [ssc_mode]");
        goto end;
    }
    }

    dlset_support_ind = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "dlsetSupportInd");
    if (dlset_support_ind) {
    if (!cJSON_IsBool(dlset_support_ind)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [dlset_support_ind]");
        goto end;
    }
    }

    n9fsc_support_ind = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "n9fscSupportInd");
    if (n9fsc_support_ind) {
    if (!cJSON_IsBool(n9fsc_support_ind)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [n9fsc_support_ind]");
        goto end;
    }
    }

    anchor_smf_oauth2_required = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "anchorSmfOauth2Required");
    if (anchor_smf_oauth2_required) {
    if (!cJSON_IsBool(anchor_smf_oauth2_required)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [anchor_smf_oauth2_required]");
        goto end;
    }
    }

    full_dnai_list = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "fullDnaiList");
    if (full_dnai_list) {
        cJSON *full_dnai_list_local = NULL;
        if (!cJSON_IsArray(full_dnai_list)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [full_dnai_list]");
            goto end;
        }

        full_dnai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(full_dnai_list_local, full_dnai_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(full_dnai_list_local)) {
                ogs_error("OpenAPI_sm_context_parseFromJSON() failed [full_dnai_list]");
                goto end;
            }
            OpenAPI_list_add(full_dnai_listList, ogs_strdup(full_dnai_list_local->valuestring));
        }
    }

    hrsbo_auth_req_ind = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "hrsboAuthReqInd");
    if (hrsbo_auth_req_ind) {
    if (!cJSON_IsBool(hrsbo_auth_req_ind)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [hrsbo_auth_req_ind]");
        goto end;
    }
    }

    h_dns_addr = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "hDnsAddr");
    if (h_dns_addr) {
    h_dns_addr_local_nonprim = OpenAPI_ip_address_parseFromJSON(h_dns_addr);
    if (!h_dns_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [h_dns_addr]");
        goto end;
    }
    }

    h_plmn_addr = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "hPlmnAddr");
    if (h_plmn_addr) {
    h_plmn_addr_local_nonprim = OpenAPI_ip_address_parseFromJSON(h_plmn_addr);
    if (!h_plmn_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [h_plmn_addr]");
        goto end;
    }
    }

    vplmn_offloading_info_list = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "vplmnOffloadingInfoList");
    if (vplmn_offloading_info_list) {
        cJSON *vplmn_offloading_info_list_local = NULL;
        if (!cJSON_IsArray(vplmn_offloading_info_list)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [vplmn_offloading_info_list]");
            goto end;
        }

        vplmn_offloading_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(vplmn_offloading_info_list_local, vplmn_offloading_info_list) {
            if (!cJSON_IsObject(vplmn_offloading_info_list_local)) {
                ogs_error("OpenAPI_sm_context_parseFromJSON() failed [vplmn_offloading_info_list]");
                goto end;
            }
            OpenAPI_vplmn_offloading_info_t *vplmn_offloading_info_listItem = OpenAPI_vplmn_offloading_info_parseFromJSON(vplmn_offloading_info_list_local);
            if (!vplmn_offloading_info_listItem) {
                ogs_error("No vplmn_offloading_info_listItem");
                goto end;
            }
            OpenAPI_list_add(vplmn_offloading_info_listList, vplmn_offloading_info_listItem);
        }
    }

    vplmn_dl_ambr = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "vplmnDlAmbr");
    if (vplmn_dl_ambr) {
    if (!cJSON_IsNull(vplmn_dl_ambr)) {
    vplmn_dl_ambr_local_nonprim = OpenAPI_vplmn_dl_ambr_parseFromJSON(vplmn_dl_ambr);
    if (!vplmn_dl_ambr_local_nonprim) {
        ogs_error("OpenAPI_vplmn_dl_ambr_parseFromJSON failed [vplmn_dl_ambr]");
        goto end;
    }
    }
    }

    offload_ids = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "offloadIds");
    if (offload_ids) {
        cJSON *offload_ids_local = NULL;
        if (!cJSON_IsArray(offload_ids)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [offload_ids]");
            goto end;
        }

        offload_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(offload_ids_local, offload_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(offload_ids_local)) {
                ogs_error("OpenAPI_sm_context_parseFromJSON() failed [offload_ids]");
                goto end;
            }
            OpenAPI_list_add(offload_idsList, ogs_strdup(offload_ids_local->valuestring));
        }
    }

    target_dnai = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "targetDnai");
    if (target_dnai) {
    if (!cJSON_IsString(target_dnai) && !cJSON_IsNull(target_dnai)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [target_dnai]");
        goto end;
    }
    }

    traffic_influ_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "trafficInfluInfo");
    if (traffic_influ_info) {
    traffic_influ_info_local_nonprim = OpenAPI_traffic_influence_info_parseFromJSON(traffic_influ_info);
    if (!traffic_influ_info_local_nonprim) {
        ogs_error("OpenAPI_traffic_influence_info_parseFromJSON failed [traffic_influ_info]");
        goto end;
    }
    }

    pending_update_info_list = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "pendingUpdateInfoList");
    if (pending_update_info_list) {
        cJSON *pending_update_info_list_local = NULL;
        if (!cJSON_IsArray(pending_update_info_list)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pending_update_info_list]");
            goto end;
        }

        pending_update_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(pending_update_info_list_local, pending_update_info_list) {
            OpenAPI_pending_update_info_e localEnum = OpenAPI_pending_update_info_NULL;
            if (!cJSON_IsString(pending_update_info_list_local)) {
                ogs_error("OpenAPI_sm_context_parseFromJSON() failed [pending_update_info_list]");
                goto end;
            }
            localEnum = OpenAPI_pending_update_info_FromString(pending_update_info_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"pending_update_info_list\" is not supported. Ignoring it ...",
                         pending_update_info_list_local->valuestring);
            } else {
                OpenAPI_list_add(pending_update_info_listList, (void *)localEnum);
            }
        }
        if (pending_update_info_listList->count == 0) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed: Expected pending_update_info_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    eas_info_to_refresh = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "easInfoToRefresh");
    if (eas_info_to_refresh) {
    eas_info_to_refresh_local_nonprim = OpenAPI_eas_info_to_refresh_parseFromJSON(eas_info_to_refresh);
    if (!eas_info_to_refresh_local_nonprim) {
        ogs_error("OpenAPI_eas_info_to_refresh_parseFromJSON failed [eas_info_to_refresh]");
        goto end;
    }
    }

    local_offloading_mgt_allowed_ind = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "localOffloadingMgtAllowedInd");
    if (local_offloading_mgt_allowed_ind) {
    if (!cJSON_IsBool(local_offloading_mgt_allowed_ind)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [local_offloading_mgt_allowed_ind]");
        goto end;
    }
    }

    dns_addr = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "dnsAddr");
    if (dns_addr) {
    dns_addr_local_nonprim = OpenAPI_ip_address_parseFromJSON(dns_addr);
    if (!dns_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [dns_addr]");
        goto end;
    }
    }

    psa_upf_addr = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "psaUpfAddr");
    if (psa_upf_addr) {
    psa_upf_addr_local_nonprim = OpenAPI_ip_address_parseFromJSON(psa_upf_addr);
    if (!psa_upf_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_address_parseFromJSON failed [psa_upf_addr]");
        goto end;
    }
    }

    local_offloading_info_list = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "localOffloadingInfoList");
    if (local_offloading_info_list) {
        cJSON *local_offloading_info_list_local = NULL;
        if (!cJSON_IsArray(local_offloading_info_list)) {
            ogs_error("OpenAPI_sm_context_parseFromJSON() failed [local_offloading_info_list]");
            goto end;
        }

        local_offloading_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(local_offloading_info_list_local, local_offloading_info_list) {
            if (!cJSON_IsObject(local_offloading_info_list_local)) {
                ogs_error("OpenAPI_sm_context_parseFromJSON() failed [local_offloading_info_list]");
                goto end;
            }
            OpenAPI_local_offloading_management_info_t *local_offloading_info_listItem = OpenAPI_local_offloading_management_info_parseFromJSON(local_offloading_info_list_local);
            if (!local_offloading_info_listItem) {
                ogs_error("No local_offloading_info_listItem");
                goto end;
            }
            OpenAPI_list_add(local_offloading_info_listList, local_offloading_info_listItem);
        }
    }

    priority_user_ind = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "priorityUserInd");
    if (priority_user_ind) {
    if (!cJSON_IsBool(priority_user_ind)) {
        ogs_error("OpenAPI_sm_context_parseFromJSON() failed [priority_user_ind]");
        goto end;
    }
    }

    qos_monitoring_info = cJSON_GetObjectItemCaseSensitive(sm_contextJSON, "qosMonitoringInfo");
    if (qos_monitoring_info) {
    qos_monitoring_info_local_nonprim = OpenAPI_qos_monitoring_info_parseFromJSON(qos_monitoring_info);
    if (!qos_monitoring_info_local_nonprim) {
        ogs_error("OpenAPI_qos_monitoring_info_parseFromJSON failed [qos_monitoring_info]");
        goto end;
    }
    }

    sm_context_local_var = OpenAPI_sm_context_create (
        
        pdu_session_id->valuedouble,
        ogs_strdup(dnn->valuestring),
        selected_dnn && !cJSON_IsNull(selected_dnn) ? ogs_strdup(selected_dnn->valuestring) : NULL,
        s_nssai_local_nonprim,
        alt_snssai ? alt_snssai_local_nonprim : NULL,
        hplmn_snssai ? hplmn_snssai_local_nonprim : NULL,
        alt_hplmn_snssai ? alt_hplmn_snssai_local_nonprim : NULL,
        pdu_session_typeVariable,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        h_smf_uri && !cJSON_IsNull(h_smf_uri) ? ogs_strdup(h_smf_uri->valuestring) : NULL,
        smf_uri && !cJSON_IsNull(smf_uri) ? ogs_strdup(smf_uri->valuestring) : NULL,
        pdu_session_ref && !cJSON_IsNull(pdu_session_ref) ? ogs_strdup(pdu_session_ref->valuestring) : NULL,
        inter_plmn_api_root && !cJSON_IsNull(inter_plmn_api_root) ? ogs_strdup(inter_plmn_api_root->valuestring) : NULL,
        intra_plmn_api_root && !cJSON_IsNull(intra_plmn_api_root) ? ogs_strdup(intra_plmn_api_root->valuestring) : NULL,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_group_id && !cJSON_IsNull(pcf_group_id) ? ogs_strdup(pcf_group_id->valuestring) : NULL,
        pcf_set_id && !cJSON_IsNull(pcf_set_id) ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        sel_mode ? sel_modeVariable : 0,
        udm_group_id && !cJSON_IsNull(udm_group_id) ? ogs_strdup(udm_group_id->valuestring) : NULL,
        routing_indicator && !cJSON_IsNull(routing_indicator) ? ogs_strdup(routing_indicator->valuestring) : NULL,
        h_nw_pub_key_id ? true : false,
        h_nw_pub_key_id ? h_nw_pub_key_id->valuedouble : 0,
        session_ambr_local_nonprim,
        qos_flows_listList,
        h_smf_instance_id && !cJSON_IsNull(h_smf_instance_id) ? ogs_strdup(h_smf_instance_id->valuestring) : NULL,
        smf_instance_id && !cJSON_IsNull(smf_instance_id) ? ogs_strdup(smf_instance_id->valuestring) : NULL,
        pdu_session_smf_set_id && !cJSON_IsNull(pdu_session_smf_set_id) ? ogs_strdup(pdu_session_smf_set_id->valuestring) : NULL,
        pdu_session_smf_service_set_id && !cJSON_IsNull(pdu_session_smf_service_set_id) ? ogs_strdup(pdu_session_smf_service_set_id->valuestring) : NULL,
        pdu_session_smf_binding ? pdu_session_smf_bindingVariable : 0,
        enable_pause_charging ? true : false,
        enable_pause_charging ? enable_pause_charging->valueint : 0,
        ue_ipv4_address && !cJSON_IsNull(ue_ipv4_address) ? ogs_strdup(ue_ipv4_address->valuestring) : NULL,
        ue_ipv6_prefix && !cJSON_IsNull(ue_ipv6_prefix) ? ogs_strdup(ue_ipv6_prefix->valuestring) : NULL,
        eps_pdn_cnx_info ? eps_pdn_cnx_info_local_nonprim : NULL,
        eps_bearer_info ? eps_bearer_infoList : NULL,
        max_integrity_protected_data_rate ? max_integrity_protected_data_rateVariable : 0,
        max_integrity_protected_data_rate_dl ? max_integrity_protected_data_rate_dlVariable : 0,
        always_on_granted ? true : false,
        always_on_granted ? always_on_granted->valueint : 0,
        up_security ? up_security_local_nonprim : NULL,
        h_smf_service_instance_id && !cJSON_IsNull(h_smf_service_instance_id) ? ogs_strdup(h_smf_service_instance_id->valuestring) : NULL,
        smf_service_instance_id && !cJSON_IsNull(smf_service_instance_id) ? ogs_strdup(smf_service_instance_id->valuestring) : NULL,
        recovery_time && !cJSON_IsNull(recovery_time) ? ogs_strdup(recovery_time->valuestring) : NULL,
        forwarding_ind ? true : false,
        forwarding_ind ? forwarding_ind->valueint : 0,
        psa_tunnel_info ? psa_tunnel_info_local_nonprim : NULL,
        charging_id && !cJSON_IsNull(charging_id) ? ogs_strdup(charging_id->valuestring) : NULL,
        smf_charging_id && !cJSON_IsNull(smf_charging_id) ? ogs_strdup(smf_charging_id->valuestring) : NULL,
        charging_info ? charging_info_local_nonprim : NULL,
        roaming_charging_profile ? roaming_charging_profile_local_nonprim : NULL,
        nef_ext_buf_support_ind ? true : false,
        nef_ext_buf_support_ind ? nef_ext_buf_support_ind->valueint : 0,
        ipv6_index ? true : false,
        ipv6_index ? ipv6_index->valuedouble : 0,
        dn_aaa_address ? dn_aaa_address_local_nonprim : NULL,
        redundant_pdu_session_info ? redundant_pdu_session_info_local_nonprim : NULL,
        ran_tunnel_info ? ran_tunnel_info_local_nonprim : NULL,
        add_ran_tunnel_info ? add_ran_tunnel_infoList : NULL,
        red_ran_tunnel_info ? red_ran_tunnel_info_local_nonprim : NULL,
        add_red_ran_tunnel_info ? add_red_ran_tunnel_infoList : NULL,
        nspu_support_ind ? true : false,
        nspu_support_ind ? nspu_support_ind->valueint : 0,
        smf_binding_info && !cJSON_IsNull(smf_binding_info) ? ogs_strdup(smf_binding_info->valuestring) : NULL,
        satellite_backhaul_cat ? satellite_backhaul_catVariable : 0,
        ssc_mode && !cJSON_IsNull(ssc_mode) ? ogs_strdup(ssc_mode->valuestring) : NULL,
        dlset_support_ind ? true : false,
        dlset_support_ind ? dlset_support_ind->valueint : 0,
        n9fsc_support_ind ? true : false,
        n9fsc_support_ind ? n9fsc_support_ind->valueint : 0,
        anchor_smf_oauth2_required ? true : false,
        anchor_smf_oauth2_required ? anchor_smf_oauth2_required->valueint : 0,
        full_dnai_list ? full_dnai_listList : NULL,
        hrsbo_auth_req_ind ? true : false,
        hrsbo_auth_req_ind ? hrsbo_auth_req_ind->valueint : 0,
        h_dns_addr ? h_dns_addr_local_nonprim : NULL,
        h_plmn_addr ? h_plmn_addr_local_nonprim : NULL,
        vplmn_offloading_info_list ? vplmn_offloading_info_listList : NULL,
        vplmn_dl_ambr && cJSON_IsNull(vplmn_dl_ambr) ? true : false,
        vplmn_dl_ambr ? vplmn_dl_ambr_local_nonprim : NULL,
        offload_ids ? offload_idsList : NULL,
        target_dnai && !cJSON_IsNull(target_dnai) ? ogs_strdup(target_dnai->valuestring) : NULL,
        traffic_influ_info ? traffic_influ_info_local_nonprim : NULL,
        pending_update_info_list ? pending_update_info_listList : NULL,
        eas_info_to_refresh ? eas_info_to_refresh_local_nonprim : NULL,
        local_offloading_mgt_allowed_ind ? true : false,
        local_offloading_mgt_allowed_ind ? local_offloading_mgt_allowed_ind->valueint : 0,
        dns_addr ? dns_addr_local_nonprim : NULL,
        psa_upf_addr ? psa_upf_addr_local_nonprim : NULL,
        local_offloading_info_list ? local_offloading_info_listList : NULL,
        priority_user_ind ? true : false,
        priority_user_ind ? priority_user_ind->valueint : 0,
        qos_monitoring_info ? qos_monitoring_info_local_nonprim : NULL
    );

    return sm_context_local_var;
end:
    if (s_nssai_local_nonprim) {
        OpenAPI_snssai_free(s_nssai_local_nonprim);
        s_nssai_local_nonprim = NULL;
    }
    if (alt_snssai_local_nonprim) {
        OpenAPI_snssai_free(alt_snssai_local_nonprim);
        alt_snssai_local_nonprim = NULL;
    }
    if (hplmn_snssai_local_nonprim) {
        OpenAPI_snssai_free(hplmn_snssai_local_nonprim);
        hplmn_snssai_local_nonprim = NULL;
    }
    if (alt_hplmn_snssai_local_nonprim) {
        OpenAPI_snssai_free(alt_hplmn_snssai_local_nonprim);
        alt_hplmn_snssai_local_nonprim = NULL;
    }
    if (session_ambr_local_nonprim) {
        OpenAPI_ambr_free(session_ambr_local_nonprim);
        session_ambr_local_nonprim = NULL;
    }
    if (qos_flows_listList) {
        OpenAPI_list_for_each(qos_flows_listList, node) {
            OpenAPI_qos_flow_setup_item_free(node->data);
        }
        OpenAPI_list_free(qos_flows_listList);
        qos_flows_listList = NULL;
    }
    if (eps_pdn_cnx_info_local_nonprim) {
        OpenAPI_eps_pdn_cnx_info_free(eps_pdn_cnx_info_local_nonprim);
        eps_pdn_cnx_info_local_nonprim = NULL;
    }
    if (eps_bearer_infoList) {
        OpenAPI_list_for_each(eps_bearer_infoList, node) {
            OpenAPI_eps_bearer_info_free(node->data);
        }
        OpenAPI_list_free(eps_bearer_infoList);
        eps_bearer_infoList = NULL;
    }
    if (up_security_local_nonprim) {
        OpenAPI_up_security_free(up_security_local_nonprim);
        up_security_local_nonprim = NULL;
    }
    if (psa_tunnel_info_local_nonprim) {
        OpenAPI_tunnel_info_free(psa_tunnel_info_local_nonprim);
        psa_tunnel_info_local_nonprim = NULL;
    }
    if (charging_info_local_nonprim) {
        OpenAPI_charging_information_free(charging_info_local_nonprim);
        charging_info_local_nonprim = NULL;
    }
    if (roaming_charging_profile_local_nonprim) {
        OpenAPI_roaming_charging_profile_free(roaming_charging_profile_local_nonprim);
        roaming_charging_profile_local_nonprim = NULL;
    }
    if (dn_aaa_address_local_nonprim) {
        OpenAPI_ip_address_free(dn_aaa_address_local_nonprim);
        dn_aaa_address_local_nonprim = NULL;
    }
    if (redundant_pdu_session_info_local_nonprim) {
        OpenAPI_redundant_pdu_session_information_free(redundant_pdu_session_info_local_nonprim);
        redundant_pdu_session_info_local_nonprim = NULL;
    }
    if (ran_tunnel_info_local_nonprim) {
        OpenAPI_qos_flow_tunnel_free(ran_tunnel_info_local_nonprim);
        ran_tunnel_info_local_nonprim = NULL;
    }
    if (add_ran_tunnel_infoList) {
        OpenAPI_list_for_each(add_ran_tunnel_infoList, node) {
            OpenAPI_qos_flow_tunnel_free(node->data);
        }
        OpenAPI_list_free(add_ran_tunnel_infoList);
        add_ran_tunnel_infoList = NULL;
    }
    if (red_ran_tunnel_info_local_nonprim) {
        OpenAPI_qos_flow_tunnel_free(red_ran_tunnel_info_local_nonprim);
        red_ran_tunnel_info_local_nonprim = NULL;
    }
    if (add_red_ran_tunnel_infoList) {
        OpenAPI_list_for_each(add_red_ran_tunnel_infoList, node) {
            OpenAPI_qos_flow_tunnel_free(node->data);
        }
        OpenAPI_list_free(add_red_ran_tunnel_infoList);
        add_red_ran_tunnel_infoList = NULL;
    }
    if (full_dnai_listList) {
        OpenAPI_list_for_each(full_dnai_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(full_dnai_listList);
        full_dnai_listList = NULL;
    }
    if (h_dns_addr_local_nonprim) {
        OpenAPI_ip_address_free(h_dns_addr_local_nonprim);
        h_dns_addr_local_nonprim = NULL;
    }
    if (h_plmn_addr_local_nonprim) {
        OpenAPI_ip_address_free(h_plmn_addr_local_nonprim);
        h_plmn_addr_local_nonprim = NULL;
    }
    if (vplmn_offloading_info_listList) {
        OpenAPI_list_for_each(vplmn_offloading_info_listList, node) {
            OpenAPI_vplmn_offloading_info_free(node->data);
        }
        OpenAPI_list_free(vplmn_offloading_info_listList);
        vplmn_offloading_info_listList = NULL;
    }
    if (vplmn_dl_ambr_local_nonprim) {
        OpenAPI_vplmn_dl_ambr_free(vplmn_dl_ambr_local_nonprim);
        vplmn_dl_ambr_local_nonprim = NULL;
    }
    if (offload_idsList) {
        OpenAPI_list_for_each(offload_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(offload_idsList);
        offload_idsList = NULL;
    }
    if (traffic_influ_info_local_nonprim) {
        OpenAPI_traffic_influence_info_free(traffic_influ_info_local_nonprim);
        traffic_influ_info_local_nonprim = NULL;
    }
    if (pending_update_info_listList) {
        OpenAPI_list_free(pending_update_info_listList);
        pending_update_info_listList = NULL;
    }
    if (eas_info_to_refresh_local_nonprim) {
        OpenAPI_eas_info_to_refresh_free(eas_info_to_refresh_local_nonprim);
        eas_info_to_refresh_local_nonprim = NULL;
    }
    if (dns_addr_local_nonprim) {
        OpenAPI_ip_address_free(dns_addr_local_nonprim);
        dns_addr_local_nonprim = NULL;
    }
    if (psa_upf_addr_local_nonprim) {
        OpenAPI_ip_address_free(psa_upf_addr_local_nonprim);
        psa_upf_addr_local_nonprim = NULL;
    }
    if (local_offloading_info_listList) {
        OpenAPI_list_for_each(local_offloading_info_listList, node) {
            OpenAPI_local_offloading_management_info_free(node->data);
        }
        OpenAPI_list_free(local_offloading_info_listList);
        local_offloading_info_listList = NULL;
    }
    if (qos_monitoring_info_local_nonprim) {
        OpenAPI_qos_monitoring_info_free(qos_monitoring_info_local_nonprim);
        qos_monitoring_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sm_context_t *OpenAPI_sm_context_copy(OpenAPI_sm_context_t *dst, OpenAPI_sm_context_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_convertToJSON() failed");
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

    OpenAPI_sm_context_free(dst);
    dst = OpenAPI_sm_context_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

