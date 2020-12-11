
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hsmf_update_data.h"

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
    int pause_charging,
    int pti,
    OpenAPI_ref_to_binary_data_t *n1_sm_info_from_ue,
    OpenAPI_ref_to_binary_data_t *unknown_n1_sm_info,
    OpenAPI_list_t *qos_flows_rel_notify_list,
    OpenAPI_list_t *qos_flows_notify_list,
    OpenAPI_list_t *notify_list,
    OpenAPI_list_t *eps_bearer_id,
    int ho_preparation_indication,
    OpenAPI_list_t *revoke_ebi_list,
    OpenAPI_cause_e cause,
    OpenAPI_ng_ap_cause_t *ng_ap_cause,
    int _5g_mm_cause_value,
    int always_on_requested,
    OpenAPI_eps_interworking_indication_e eps_interworking_ind,
    OpenAPI_list_t *secondary_rat_usage_report,
    OpenAPI_list_t *secondary_rat_usage_info,
    int an_type_can_be_changed,
    OpenAPI_ma_release_indication_e ma_release_ind,
    int ma_nw_upgrade_ind,
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
    int dl_serving_plmn_rate_ctl,
    OpenAPI_list_t *dnai_list,
    char *supported_features,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile,
    OpenAPI_mo_exception_data_flag_t *mo_exp_data_ind,
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter,
    OpenAPI_vplmn_qos_t *vplmn_qos
    )
{
    OpenAPI_hsmf_update_data_t *hsmf_update_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_hsmf_update_data_t));
    if (!hsmf_update_data_local_var) {
        return NULL;
    }
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
    hsmf_update_data_local_var->pause_charging = pause_charging;
    hsmf_update_data_local_var->pti = pti;
    hsmf_update_data_local_var->n1_sm_info_from_ue = n1_sm_info_from_ue;
    hsmf_update_data_local_var->unknown_n1_sm_info = unknown_n1_sm_info;
    hsmf_update_data_local_var->qos_flows_rel_notify_list = qos_flows_rel_notify_list;
    hsmf_update_data_local_var->qos_flows_notify_list = qos_flows_notify_list;
    hsmf_update_data_local_var->notify_list = notify_list;
    hsmf_update_data_local_var->eps_bearer_id = eps_bearer_id;
    hsmf_update_data_local_var->ho_preparation_indication = ho_preparation_indication;
    hsmf_update_data_local_var->revoke_ebi_list = revoke_ebi_list;
    hsmf_update_data_local_var->cause = cause;
    hsmf_update_data_local_var->ng_ap_cause = ng_ap_cause;
    hsmf_update_data_local_var->_5g_mm_cause_value = _5g_mm_cause_value;
    hsmf_update_data_local_var->always_on_requested = always_on_requested;
    hsmf_update_data_local_var->eps_interworking_ind = eps_interworking_ind;
    hsmf_update_data_local_var->secondary_rat_usage_report = secondary_rat_usage_report;
    hsmf_update_data_local_var->secondary_rat_usage_info = secondary_rat_usage_info;
    hsmf_update_data_local_var->an_type_can_be_changed = an_type_can_be_changed;
    hsmf_update_data_local_var->ma_release_ind = ma_release_ind;
    hsmf_update_data_local_var->ma_nw_upgrade_ind = ma_nw_upgrade_ind;
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
    hsmf_update_data_local_var->dl_serving_plmn_rate_ctl = dl_serving_plmn_rate_ctl;
    hsmf_update_data_local_var->dnai_list = dnai_list;
    hsmf_update_data_local_var->supported_features = supported_features;
    hsmf_update_data_local_var->roaming_charging_profile = roaming_charging_profile;
    hsmf_update_data_local_var->mo_exp_data_ind = mo_exp_data_ind;
    hsmf_update_data_local_var->mo_exp_data_counter = mo_exp_data_counter;
    hsmf_update_data_local_var->vplmn_qos = vplmn_qos;

    return hsmf_update_data_local_var;
}

void OpenAPI_hsmf_update_data_free(OpenAPI_hsmf_update_data_t *hsmf_update_data)
{
    if (NULL == hsmf_update_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(hsmf_update_data->pei);
    OpenAPI_tunnel_info_free(hsmf_update_data->vcn_tunnel_info);
    OpenAPI_tunnel_info_free(hsmf_update_data->icn_tunnel_info);
    OpenAPI_tunnel_info_free(hsmf_update_data->additional_cn_tunnel_info);
    OpenAPI_plmn_id_nid_free(hsmf_update_data->serving_network);
    OpenAPI_user_location_free(hsmf_update_data->ue_location);
    ogs_free(hsmf_update_data->ue_time_zone);
    OpenAPI_user_location_free(hsmf_update_data->add_ue_location);
    OpenAPI_ref_to_binary_data_free(hsmf_update_data->n1_sm_info_from_ue);
    OpenAPI_ref_to_binary_data_free(hsmf_update_data->unknown_n1_sm_info);
    OpenAPI_list_for_each(hsmf_update_data->qos_flows_rel_notify_list, node) {
        OpenAPI_qos_flow_item_free(node->data);
    }
    OpenAPI_list_free(hsmf_update_data->qos_flows_rel_notify_list);
    OpenAPI_list_for_each(hsmf_update_data->qos_flows_notify_list, node) {
        OpenAPI_qos_flow_notify_item_free(node->data);
    }
    OpenAPI_list_free(hsmf_update_data->qos_flows_notify_list);
    OpenAPI_list_for_each(hsmf_update_data->notify_list, node) {
        OpenAPI_pdu_session_notify_item_free(node->data);
    }
    OpenAPI_list_free(hsmf_update_data->notify_list);
    OpenAPI_list_for_each(hsmf_update_data->eps_bearer_id, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(hsmf_update_data->eps_bearer_id);
    OpenAPI_list_for_each(hsmf_update_data->revoke_ebi_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(hsmf_update_data->revoke_ebi_list);
    OpenAPI_ng_ap_cause_free(hsmf_update_data->ng_ap_cause);
    OpenAPI_list_for_each(hsmf_update_data->secondary_rat_usage_report, node) {
        OpenAPI_secondary_rat_usage_report_free(node->data);
    }
    OpenAPI_list_free(hsmf_update_data->secondary_rat_usage_report);
    OpenAPI_list_for_each(hsmf_update_data->secondary_rat_usage_info, node) {
        OpenAPI_secondary_rat_usage_info_free(node->data);
    }
    OpenAPI_list_free(hsmf_update_data->secondary_rat_usage_info);
    OpenAPI_list_for_each(hsmf_update_data->psa_info, node) {
        OpenAPI_psa_information_free(node->data);
    }
    OpenAPI_list_free(hsmf_update_data->psa_info);
    OpenAPI_ulcl_bp_information_free(hsmf_update_data->ulcl_bp_info);
    OpenAPI_n4_information_free(hsmf_update_data->n4_info);
    OpenAPI_n4_information_free(hsmf_update_data->n4_info_ext1);
    OpenAPI_n4_information_free(hsmf_update_data->n4_info_ext2);
    ogs_free(hsmf_update_data->vsmf_pdu_session_uri);
    ogs_free(hsmf_update_data->vsmf_id);
    ogs_free(hsmf_update_data->v_smf_service_instance_id);
    ogs_free(hsmf_update_data->ismf_pdu_session_uri);
    ogs_free(hsmf_update_data->ismf_id);
    ogs_free(hsmf_update_data->i_smf_service_instance_id);
    OpenAPI_list_for_each(hsmf_update_data->dnai_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(hsmf_update_data->dnai_list);
    ogs_free(hsmf_update_data->supported_features);
    OpenAPI_roaming_charging_profile_free(hsmf_update_data->roaming_charging_profile);
    OpenAPI_mo_exception_data_flag_free(hsmf_update_data->mo_exp_data_ind);
    OpenAPI_mo_exp_data_counter_free(hsmf_update_data->mo_exp_data_counter);
    OpenAPI_vplmn_qos_free(hsmf_update_data->vplmn_qos);
    ogs_free(hsmf_update_data);
}

cJSON *OpenAPI_hsmf_update_data_convertToJSON(OpenAPI_hsmf_update_data_t *hsmf_update_data)
{
    cJSON *item = NULL;

    if (hsmf_update_data == NULL) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [HsmfUpdateData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!hsmf_update_data->request_indication) {
        ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [request_indication]");
        goto end;
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

    if (hsmf_update_data->an_type) {
        if (cJSON_AddStringToObject(item, "anType", OpenAPI_access_type_ToString(hsmf_update_data->an_type)) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [an_type]");
            goto end;
        }
    }

    if (hsmf_update_data->additional_an_type) {
        if (cJSON_AddStringToObject(item, "additionalAnType", OpenAPI_access_type_ToString(hsmf_update_data->additional_an_type)) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [additional_an_type]");
            goto end;
        }
    }

    if (hsmf_update_data->rat_type) {
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

    if (hsmf_update_data->pause_charging) {
        if (cJSON_AddBoolToObject(item, "pauseCharging", hsmf_update_data->pause_charging) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [pause_charging]");
            goto end;
        }
    }

    if (hsmf_update_data->pti) {
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

        OpenAPI_lnode_t *qos_flows_rel_notify_list_node;
        if (hsmf_update_data->qos_flows_rel_notify_list) {
            OpenAPI_list_for_each(hsmf_update_data->qos_flows_rel_notify_list, qos_flows_rel_notify_list_node) {
                cJSON *itemLocal = OpenAPI_qos_flow_item_convertToJSON(qos_flows_rel_notify_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [qos_flows_rel_notify_list]");
                    goto end;
                }
                cJSON_AddItemToArray(qos_flows_rel_notify_listList, itemLocal);
            }
        }
    }

    if (hsmf_update_data->qos_flows_notify_list) {
        cJSON *qos_flows_notify_listList = cJSON_AddArrayToObject(item, "qosFlowsNotifyList");
        if (qos_flows_notify_listList == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [qos_flows_notify_list]");
            goto end;
        }

        OpenAPI_lnode_t *qos_flows_notify_list_node;
        if (hsmf_update_data->qos_flows_notify_list) {
            OpenAPI_list_for_each(hsmf_update_data->qos_flows_notify_list, qos_flows_notify_list_node) {
                cJSON *itemLocal = OpenAPI_qos_flow_notify_item_convertToJSON(qos_flows_notify_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [qos_flows_notify_list]");
                    goto end;
                }
                cJSON_AddItemToArray(qos_flows_notify_listList, itemLocal);
            }
        }
    }

    if (hsmf_update_data->notify_list) {
        cJSON *notify_listList = cJSON_AddArrayToObject(item, "NotifyList");
        if (notify_listList == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [notify_list]");
            goto end;
        }

        OpenAPI_lnode_t *notify_list_node;
        if (hsmf_update_data->notify_list) {
            OpenAPI_list_for_each(hsmf_update_data->notify_list, notify_list_node) {
                cJSON *itemLocal = OpenAPI_pdu_session_notify_item_convertToJSON(notify_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [notify_list]");
                    goto end;
                }
                cJSON_AddItemToArray(notify_listList, itemLocal);
            }
        }
    }

    if (hsmf_update_data->eps_bearer_id) {
        cJSON *eps_bearer_id = cJSON_AddArrayToObject(item, "epsBearerId");
        if (eps_bearer_id == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [eps_bearer_id]");
            goto end;
        }

        OpenAPI_lnode_t *eps_bearer_id_node;
        OpenAPI_list_for_each(hsmf_update_data->eps_bearer_id, eps_bearer_id_node)  {
            if (cJSON_AddNumberToObject(eps_bearer_id, "", *(double *)eps_bearer_id_node->data) == NULL) {
                ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [eps_bearer_id]");
                goto end;
            }
        }
    }

    if (hsmf_update_data->ho_preparation_indication) {
        if (cJSON_AddBoolToObject(item, "hoPreparationIndication", hsmf_update_data->ho_preparation_indication) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ho_preparation_indication]");
            goto end;
        }
    }

    if (hsmf_update_data->revoke_ebi_list) {
        cJSON *revoke_ebi_list = cJSON_AddArrayToObject(item, "revokeEbiList");
        if (revoke_ebi_list == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [revoke_ebi_list]");
            goto end;
        }

        OpenAPI_lnode_t *revoke_ebi_list_node;
        OpenAPI_list_for_each(hsmf_update_data->revoke_ebi_list, revoke_ebi_list_node)  {
            if (cJSON_AddNumberToObject(revoke_ebi_list, "", *(double *)revoke_ebi_list_node->data) == NULL) {
                ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [revoke_ebi_list]");
                goto end;
            }
        }
    }

    if (hsmf_update_data->cause) {
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

    if (hsmf_update_data->_5g_mm_cause_value) {
        if (cJSON_AddNumberToObject(item, "5gMmCauseValue", hsmf_update_data->_5g_mm_cause_value) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [_5g_mm_cause_value]");
            goto end;
        }
    }

    if (hsmf_update_data->always_on_requested) {
        if (cJSON_AddBoolToObject(item, "alwaysOnRequested", hsmf_update_data->always_on_requested) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [always_on_requested]");
            goto end;
        }
    }

    if (hsmf_update_data->eps_interworking_ind) {
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

        OpenAPI_lnode_t *secondary_rat_usage_report_node;
        if (hsmf_update_data->secondary_rat_usage_report) {
            OpenAPI_list_for_each(hsmf_update_data->secondary_rat_usage_report, secondary_rat_usage_report_node) {
                cJSON *itemLocal = OpenAPI_secondary_rat_usage_report_convertToJSON(secondary_rat_usage_report_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [secondary_rat_usage_report]");
                    goto end;
                }
                cJSON_AddItemToArray(secondary_rat_usage_reportList, itemLocal);
            }
        }
    }

    if (hsmf_update_data->secondary_rat_usage_info) {
        cJSON *secondary_rat_usage_infoList = cJSON_AddArrayToObject(item, "secondaryRatUsageInfo");
        if (secondary_rat_usage_infoList == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [secondary_rat_usage_info]");
            goto end;
        }

        OpenAPI_lnode_t *secondary_rat_usage_info_node;
        if (hsmf_update_data->secondary_rat_usage_info) {
            OpenAPI_list_for_each(hsmf_update_data->secondary_rat_usage_info, secondary_rat_usage_info_node) {
                cJSON *itemLocal = OpenAPI_secondary_rat_usage_info_convertToJSON(secondary_rat_usage_info_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [secondary_rat_usage_info]");
                    goto end;
                }
                cJSON_AddItemToArray(secondary_rat_usage_infoList, itemLocal);
            }
        }
    }

    if (hsmf_update_data->an_type_can_be_changed) {
        if (cJSON_AddBoolToObject(item, "anTypeCanBeChanged", hsmf_update_data->an_type_can_be_changed) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [an_type_can_be_changed]");
            goto end;
        }
    }

    if (hsmf_update_data->ma_release_ind) {
        if (cJSON_AddStringToObject(item, "maReleaseInd", OpenAPI_ma_release_indication_ToString(hsmf_update_data->ma_release_ind)) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ma_release_ind]");
            goto end;
        }
    }

    if (hsmf_update_data->ma_nw_upgrade_ind) {
        if (cJSON_AddBoolToObject(item, "maNwUpgradeInd", hsmf_update_data->ma_nw_upgrade_ind) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ma_nw_upgrade_ind]");
            goto end;
        }
    }

    if (hsmf_update_data->ma_request_ind) {
        if (cJSON_AddBoolToObject(item, "maRequestInd", hsmf_update_data->ma_request_ind) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [ma_request_ind]");
            goto end;
        }
    }

    if (hsmf_update_data->unavailable_access_ind) {
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

        OpenAPI_lnode_t *psa_info_node;
        if (hsmf_update_data->psa_info) {
            OpenAPI_list_for_each(hsmf_update_data->psa_info, psa_info_node) {
                cJSON *itemLocal = OpenAPI_psa_information_convertToJSON(psa_info_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [psa_info]");
                    goto end;
                }
                cJSON_AddItemToArray(psa_infoList, itemLocal);
            }
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

    if (hsmf_update_data->presence_in_ladn) {
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

    if (hsmf_update_data->dl_serving_plmn_rate_ctl) {
        if (cJSON_AddNumberToObject(item, "dlServingPlmnRateCtl", hsmf_update_data->dl_serving_plmn_rate_ctl) == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [dl_serving_plmn_rate_ctl]");
            goto end;
        }
    }

    if (hsmf_update_data->dnai_list) {
        cJSON *dnai_list = cJSON_AddArrayToObject(item, "dnaiList");
        if (dnai_list == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [dnai_list]");
            goto end;
        }

        OpenAPI_lnode_t *dnai_list_node;
        OpenAPI_list_for_each(hsmf_update_data->dnai_list, dnai_list_node)  {
            if (cJSON_AddStringToObject(dnai_list, "", (char*)dnai_list_node->data) == NULL) {
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

    if (hsmf_update_data->mo_exp_data_ind) {
        cJSON *mo_exp_data_ind_local_JSON = OpenAPI_mo_exception_data_flag_convertToJSON(hsmf_update_data->mo_exp_data_ind);
        if (mo_exp_data_ind_local_JSON == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [mo_exp_data_ind]");
            goto end;
        }
        cJSON_AddItemToObject(item, "moExpDataInd", mo_exp_data_ind_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_hsmf_update_data_convertToJSON() failed [mo_exp_data_ind]");
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

end:
    return item;
}

OpenAPI_hsmf_update_data_t *OpenAPI_hsmf_update_data_parseFromJSON(cJSON *hsmf_update_dataJSON)
{
    OpenAPI_hsmf_update_data_t *hsmf_update_data_local_var = NULL;
    cJSON *request_indication = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "requestIndication");
    if (!request_indication) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [request_indication]");
        goto end;
    }

    OpenAPI_request_indication_e request_indicationVariable;

    if (!cJSON_IsString(request_indication)) {
        ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [request_indication]");
        goto end;
    }
    request_indicationVariable = OpenAPI_request_indication_FromString(request_indication->valuestring);

    cJSON *pei = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "pei");

    if (pei) {
        if (!cJSON_IsString(pei)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [pei]");
            goto end;
        }
    }

    cJSON *vcn_tunnel_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "vcnTunnelInfo");

    OpenAPI_tunnel_info_t *vcn_tunnel_info_local_nonprim = NULL;
    if (vcn_tunnel_info) {
        vcn_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(vcn_tunnel_info);
    }

    cJSON *icn_tunnel_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "icnTunnelInfo");

    OpenAPI_tunnel_info_t *icn_tunnel_info_local_nonprim = NULL;
    if (icn_tunnel_info) {
        icn_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(icn_tunnel_info);
    }

    cJSON *additional_cn_tunnel_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "additionalCnTunnelInfo");

    OpenAPI_tunnel_info_t *additional_cn_tunnel_info_local_nonprim = NULL;
    if (additional_cn_tunnel_info) {
        additional_cn_tunnel_info_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(additional_cn_tunnel_info);
    }

    cJSON *serving_network = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "servingNetwork");

    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    if (serving_network) {
        serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    }

    cJSON *an_type = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "anType");

    OpenAPI_access_type_e an_typeVariable;
    if (an_type) {
        if (!cJSON_IsString(an_type)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [an_type]");
            goto end;
        }
        an_typeVariable = OpenAPI_access_type_FromString(an_type->valuestring);
    }

    cJSON *additional_an_type = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "additionalAnType");

    OpenAPI_access_type_e additional_an_typeVariable;
    if (additional_an_type) {
        if (!cJSON_IsString(additional_an_type)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [additional_an_type]");
            goto end;
        }
        additional_an_typeVariable = OpenAPI_access_type_FromString(additional_an_type->valuestring);
    }

    cJSON *rat_type = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ratType");

    OpenAPI_rat_type_e rat_typeVariable;
    if (rat_type) {
        if (!cJSON_IsString(rat_type)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [rat_type]");
            goto end;
        }
        rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    cJSON *ue_location = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ueLocation");

    OpenAPI_user_location_t *ue_location_local_nonprim = NULL;
    if (ue_location) {
        ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    }

    cJSON *ue_time_zone = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ueTimeZone");

    if (ue_time_zone) {
        if (!cJSON_IsString(ue_time_zone)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ue_time_zone]");
            goto end;
        }
    }

    cJSON *add_ue_location = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "addUeLocation");

    OpenAPI_user_location_t *add_ue_location_local_nonprim = NULL;
    if (add_ue_location) {
        add_ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(add_ue_location);
    }

    cJSON *pause_charging = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "pauseCharging");

    if (pause_charging) {
        if (!cJSON_IsBool(pause_charging)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [pause_charging]");
            goto end;
        }
    }

    cJSON *pti = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "pti");

    if (pti) {
        if (!cJSON_IsNumber(pti)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [pti]");
            goto end;
        }
    }

    cJSON *n1_sm_info_from_ue = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "n1SmInfoFromUe");

    OpenAPI_ref_to_binary_data_t *n1_sm_info_from_ue_local_nonprim = NULL;
    if (n1_sm_info_from_ue) {
        n1_sm_info_from_ue_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_info_from_ue);
    }

    cJSON *unknown_n1_sm_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "unknownN1SmInfo");

    OpenAPI_ref_to_binary_data_t *unknown_n1_sm_info_local_nonprim = NULL;
    if (unknown_n1_sm_info) {
        unknown_n1_sm_info_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(unknown_n1_sm_info);
    }

    cJSON *qos_flows_rel_notify_list = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "qosFlowsRelNotifyList");

    OpenAPI_list_t *qos_flows_rel_notify_listList;
    if (qos_flows_rel_notify_list) {
        cJSON *qos_flows_rel_notify_list_local_nonprimitive;
        if (!cJSON_IsArray(qos_flows_rel_notify_list)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [qos_flows_rel_notify_list]");
            goto end;
        }

        qos_flows_rel_notify_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_flows_rel_notify_list_local_nonprimitive, qos_flows_rel_notify_list ) {
            if (!cJSON_IsObject(qos_flows_rel_notify_list_local_nonprimitive)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [qos_flows_rel_notify_list]");
                goto end;
            }
            OpenAPI_qos_flow_item_t *qos_flows_rel_notify_listItem = OpenAPI_qos_flow_item_parseFromJSON(qos_flows_rel_notify_list_local_nonprimitive);

            OpenAPI_list_add(qos_flows_rel_notify_listList, qos_flows_rel_notify_listItem);
        }
    }

    cJSON *qos_flows_notify_list = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "qosFlowsNotifyList");

    OpenAPI_list_t *qos_flows_notify_listList;
    if (qos_flows_notify_list) {
        cJSON *qos_flows_notify_list_local_nonprimitive;
        if (!cJSON_IsArray(qos_flows_notify_list)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [qos_flows_notify_list]");
            goto end;
        }

        qos_flows_notify_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_flows_notify_list_local_nonprimitive, qos_flows_notify_list ) {
            if (!cJSON_IsObject(qos_flows_notify_list_local_nonprimitive)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [qos_flows_notify_list]");
                goto end;
            }
            OpenAPI_qos_flow_notify_item_t *qos_flows_notify_listItem = OpenAPI_qos_flow_notify_item_parseFromJSON(qos_flows_notify_list_local_nonprimitive);

            OpenAPI_list_add(qos_flows_notify_listList, qos_flows_notify_listItem);
        }
    }

    cJSON *notify_list = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "NotifyList");

    OpenAPI_list_t *notify_listList;
    if (notify_list) {
        cJSON *notify_list_local_nonprimitive;
        if (!cJSON_IsArray(notify_list)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [notify_list]");
            goto end;
        }

        notify_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(notify_list_local_nonprimitive, notify_list ) {
            if (!cJSON_IsObject(notify_list_local_nonprimitive)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [notify_list]");
                goto end;
            }
            OpenAPI_pdu_session_notify_item_t *notify_listItem = OpenAPI_pdu_session_notify_item_parseFromJSON(notify_list_local_nonprimitive);

            OpenAPI_list_add(notify_listList, notify_listItem);
        }
    }

    cJSON *eps_bearer_id = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "epsBearerId");

    OpenAPI_list_t *eps_bearer_idList;
    if (eps_bearer_id) {
        cJSON *eps_bearer_id_local;
        if (!cJSON_IsArray(eps_bearer_id)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [eps_bearer_id]");
            goto end;
        }
        eps_bearer_idList = OpenAPI_list_create();

        cJSON_ArrayForEach(eps_bearer_id_local, eps_bearer_id) {
            if (!cJSON_IsNumber(eps_bearer_id_local)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [eps_bearer_id]");
                goto end;
            }
            OpenAPI_list_add(eps_bearer_idList, &eps_bearer_id_local->valuedouble);
        }
    }

    cJSON *ho_preparation_indication = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "hoPreparationIndication");

    if (ho_preparation_indication) {
        if (!cJSON_IsBool(ho_preparation_indication)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ho_preparation_indication]");
            goto end;
        }
    }

    cJSON *revoke_ebi_list = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "revokeEbiList");

    OpenAPI_list_t *revoke_ebi_listList;
    if (revoke_ebi_list) {
        cJSON *revoke_ebi_list_local;
        if (!cJSON_IsArray(revoke_ebi_list)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [revoke_ebi_list]");
            goto end;
        }
        revoke_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(revoke_ebi_list_local, revoke_ebi_list) {
            if (!cJSON_IsNumber(revoke_ebi_list_local)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [revoke_ebi_list]");
                goto end;
            }
            OpenAPI_list_add(revoke_ebi_listList, &revoke_ebi_list_local->valuedouble);
        }
    }

    cJSON *cause = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "cause");

    OpenAPI_cause_e causeVariable;
    if (cause) {
        if (!cJSON_IsString(cause)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [cause]");
            goto end;
        }
        causeVariable = OpenAPI_cause_FromString(cause->valuestring);
    }

    cJSON *ng_ap_cause = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ngApCause");

    OpenAPI_ng_ap_cause_t *ng_ap_cause_local_nonprim = NULL;
    if (ng_ap_cause) {
        ng_ap_cause_local_nonprim = OpenAPI_ng_ap_cause_parseFromJSON(ng_ap_cause);
    }

    cJSON *_5g_mm_cause_value = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "5gMmCauseValue");

    if (_5g_mm_cause_value) {
        if (!cJSON_IsNumber(_5g_mm_cause_value)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [_5g_mm_cause_value]");
            goto end;
        }
    }

    cJSON *always_on_requested = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "alwaysOnRequested");

    if (always_on_requested) {
        if (!cJSON_IsBool(always_on_requested)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [always_on_requested]");
            goto end;
        }
    }

    cJSON *eps_interworking_ind = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "epsInterworkingInd");

    OpenAPI_eps_interworking_indication_e eps_interworking_indVariable;
    if (eps_interworking_ind) {
        if (!cJSON_IsString(eps_interworking_ind)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [eps_interworking_ind]");
            goto end;
        }
        eps_interworking_indVariable = OpenAPI_eps_interworking_indication_FromString(eps_interworking_ind->valuestring);
    }

    cJSON *secondary_rat_usage_report = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "secondaryRatUsageReport");

    OpenAPI_list_t *secondary_rat_usage_reportList;
    if (secondary_rat_usage_report) {
        cJSON *secondary_rat_usage_report_local_nonprimitive;
        if (!cJSON_IsArray(secondary_rat_usage_report)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [secondary_rat_usage_report]");
            goto end;
        }

        secondary_rat_usage_reportList = OpenAPI_list_create();

        cJSON_ArrayForEach(secondary_rat_usage_report_local_nonprimitive, secondary_rat_usage_report ) {
            if (!cJSON_IsObject(secondary_rat_usage_report_local_nonprimitive)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [secondary_rat_usage_report]");
                goto end;
            }
            OpenAPI_secondary_rat_usage_report_t *secondary_rat_usage_reportItem = OpenAPI_secondary_rat_usage_report_parseFromJSON(secondary_rat_usage_report_local_nonprimitive);

            OpenAPI_list_add(secondary_rat_usage_reportList, secondary_rat_usage_reportItem);
        }
    }

    cJSON *secondary_rat_usage_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "secondaryRatUsageInfo");

    OpenAPI_list_t *secondary_rat_usage_infoList;
    if (secondary_rat_usage_info) {
        cJSON *secondary_rat_usage_info_local_nonprimitive;
        if (!cJSON_IsArray(secondary_rat_usage_info)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [secondary_rat_usage_info]");
            goto end;
        }

        secondary_rat_usage_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(secondary_rat_usage_info_local_nonprimitive, secondary_rat_usage_info ) {
            if (!cJSON_IsObject(secondary_rat_usage_info_local_nonprimitive)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [secondary_rat_usage_info]");
                goto end;
            }
            OpenAPI_secondary_rat_usage_info_t *secondary_rat_usage_infoItem = OpenAPI_secondary_rat_usage_info_parseFromJSON(secondary_rat_usage_info_local_nonprimitive);

            OpenAPI_list_add(secondary_rat_usage_infoList, secondary_rat_usage_infoItem);
        }
    }

    cJSON *an_type_can_be_changed = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "anTypeCanBeChanged");

    if (an_type_can_be_changed) {
        if (!cJSON_IsBool(an_type_can_be_changed)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [an_type_can_be_changed]");
            goto end;
        }
    }

    cJSON *ma_release_ind = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "maReleaseInd");

    OpenAPI_ma_release_indication_e ma_release_indVariable;
    if (ma_release_ind) {
        if (!cJSON_IsString(ma_release_ind)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ma_release_ind]");
            goto end;
        }
        ma_release_indVariable = OpenAPI_ma_release_indication_FromString(ma_release_ind->valuestring);
    }

    cJSON *ma_nw_upgrade_ind = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "maNwUpgradeInd");

    if (ma_nw_upgrade_ind) {
        if (!cJSON_IsBool(ma_nw_upgrade_ind)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ma_nw_upgrade_ind]");
            goto end;
        }
    }

    cJSON *ma_request_ind = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "maRequestInd");

    if (ma_request_ind) {
        if (!cJSON_IsBool(ma_request_ind)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ma_request_ind]");
            goto end;
        }
    }

    cJSON *unavailable_access_ind = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "unavailableAccessInd");

    OpenAPI_unavailable_access_indication_e unavailable_access_indVariable;
    if (unavailable_access_ind) {
        if (!cJSON_IsString(unavailable_access_ind)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [unavailable_access_ind]");
            goto end;
        }
        unavailable_access_indVariable = OpenAPI_unavailable_access_indication_FromString(unavailable_access_ind->valuestring);
    }

    cJSON *psa_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "psaInfo");

    OpenAPI_list_t *psa_infoList;
    if (psa_info) {
        cJSON *psa_info_local_nonprimitive;
        if (!cJSON_IsArray(psa_info)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [psa_info]");
            goto end;
        }

        psa_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(psa_info_local_nonprimitive, psa_info ) {
            if (!cJSON_IsObject(psa_info_local_nonprimitive)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [psa_info]");
                goto end;
            }
            OpenAPI_psa_information_t *psa_infoItem = OpenAPI_psa_information_parseFromJSON(psa_info_local_nonprimitive);

            OpenAPI_list_add(psa_infoList, psa_infoItem);
        }
    }

    cJSON *ulcl_bp_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ulclBpInfo");

    OpenAPI_ulcl_bp_information_t *ulcl_bp_info_local_nonprim = NULL;
    if (ulcl_bp_info) {
        ulcl_bp_info_local_nonprim = OpenAPI_ulcl_bp_information_parseFromJSON(ulcl_bp_info);
    }

    cJSON *n4_info = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "n4Info");

    OpenAPI_n4_information_t *n4_info_local_nonprim = NULL;
    if (n4_info) {
        n4_info_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info);
    }

    cJSON *n4_info_ext1 = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "n4InfoExt1");

    OpenAPI_n4_information_t *n4_info_ext1_local_nonprim = NULL;
    if (n4_info_ext1) {
        n4_info_ext1_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info_ext1);
    }

    cJSON *n4_info_ext2 = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "n4InfoExt2");

    OpenAPI_n4_information_t *n4_info_ext2_local_nonprim = NULL;
    if (n4_info_ext2) {
        n4_info_ext2_local_nonprim = OpenAPI_n4_information_parseFromJSON(n4_info_ext2);
    }

    cJSON *presence_in_ladn = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "presenceInLadn");

    OpenAPI_presence_state_e presence_in_ladnVariable;
    if (presence_in_ladn) {
        if (!cJSON_IsString(presence_in_ladn)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [presence_in_ladn]");
            goto end;
        }
        presence_in_ladnVariable = OpenAPI_presence_state_FromString(presence_in_ladn->valuestring);
    }

    cJSON *vsmf_pdu_session_uri = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "vsmfPduSessionUri");

    if (vsmf_pdu_session_uri) {
        if (!cJSON_IsString(vsmf_pdu_session_uri)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [vsmf_pdu_session_uri]");
            goto end;
        }
    }

    cJSON *vsmf_id = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "vsmfId");

    if (vsmf_id) {
        if (!cJSON_IsString(vsmf_id)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [vsmf_id]");
            goto end;
        }
    }

    cJSON *v_smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "vSmfServiceInstanceId");

    if (v_smf_service_instance_id) {
        if (!cJSON_IsString(v_smf_service_instance_id)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [v_smf_service_instance_id]");
            goto end;
        }
    }

    cJSON *ismf_pdu_session_uri = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ismfPduSessionUri");

    if (ismf_pdu_session_uri) {
        if (!cJSON_IsString(ismf_pdu_session_uri)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ismf_pdu_session_uri]");
            goto end;
        }
    }

    cJSON *ismf_id = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "ismfId");

    if (ismf_id) {
        if (!cJSON_IsString(ismf_id)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [ismf_id]");
            goto end;
        }
    }

    cJSON *i_smf_service_instance_id = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "iSmfServiceInstanceId");

    if (i_smf_service_instance_id) {
        if (!cJSON_IsString(i_smf_service_instance_id)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [i_smf_service_instance_id]");
            goto end;
        }
    }

    cJSON *dl_serving_plmn_rate_ctl = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "dlServingPlmnRateCtl");

    if (dl_serving_plmn_rate_ctl) {
        if (!cJSON_IsNumber(dl_serving_plmn_rate_ctl)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [dl_serving_plmn_rate_ctl]");
            goto end;
        }
    }

    cJSON *dnai_list = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "dnaiList");

    OpenAPI_list_t *dnai_listList;
    if (dnai_list) {
        cJSON *dnai_list_local;
        if (!cJSON_IsArray(dnai_list)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [dnai_list]");
            goto end;
        }
        dnai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnai_list_local, dnai_list) {
            if (!cJSON_IsString(dnai_list_local)) {
                ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [dnai_list]");
                goto end;
            }
            OpenAPI_list_add(dnai_listList, ogs_strdup(dnai_list_local->valuestring));
        }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_hsmf_update_data_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    cJSON *roaming_charging_profile = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "roamingChargingProfile");

    OpenAPI_roaming_charging_profile_t *roaming_charging_profile_local_nonprim = NULL;
    if (roaming_charging_profile) {
        roaming_charging_profile_local_nonprim = OpenAPI_roaming_charging_profile_parseFromJSON(roaming_charging_profile);
    }

    cJSON *mo_exp_data_ind = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "moExpDataInd");

    OpenAPI_mo_exception_data_flag_t *mo_exp_data_ind_local_nonprim = NULL;
    if (mo_exp_data_ind) {
        mo_exp_data_ind_local_nonprim = OpenAPI_mo_exception_data_flag_parseFromJSON(mo_exp_data_ind);
    }

    cJSON *mo_exp_data_counter = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "moExpDataCounter");

    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter_local_nonprim = NULL;
    if (mo_exp_data_counter) {
        mo_exp_data_counter_local_nonprim = OpenAPI_mo_exp_data_counter_parseFromJSON(mo_exp_data_counter);
    }

    cJSON *vplmn_qos = cJSON_GetObjectItemCaseSensitive(hsmf_update_dataJSON, "vplmnQos");

    OpenAPI_vplmn_qos_t *vplmn_qos_local_nonprim = NULL;
    if (vplmn_qos) {
        vplmn_qos_local_nonprim = OpenAPI_vplmn_qos_parseFromJSON(vplmn_qos);
    }

    hsmf_update_data_local_var = OpenAPI_hsmf_update_data_create (
        request_indicationVariable,
        pei ? ogs_strdup(pei->valuestring) : NULL,
        vcn_tunnel_info ? vcn_tunnel_info_local_nonprim : NULL,
        icn_tunnel_info ? icn_tunnel_info_local_nonprim : NULL,
        additional_cn_tunnel_info ? additional_cn_tunnel_info_local_nonprim : NULL,
        serving_network ? serving_network_local_nonprim : NULL,
        an_type ? an_typeVariable : 0,
        additional_an_type ? additional_an_typeVariable : 0,
        rat_type ? rat_typeVariable : 0,
        ue_location ? ue_location_local_nonprim : NULL,
        ue_time_zone ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        add_ue_location ? add_ue_location_local_nonprim : NULL,
        pause_charging ? pause_charging->valueint : 0,
        pti ? pti->valuedouble : 0,
        n1_sm_info_from_ue ? n1_sm_info_from_ue_local_nonprim : NULL,
        unknown_n1_sm_info ? unknown_n1_sm_info_local_nonprim : NULL,
        qos_flows_rel_notify_list ? qos_flows_rel_notify_listList : NULL,
        qos_flows_notify_list ? qos_flows_notify_listList : NULL,
        notify_list ? notify_listList : NULL,
        eps_bearer_id ? eps_bearer_idList : NULL,
        ho_preparation_indication ? ho_preparation_indication->valueint : 0,
        revoke_ebi_list ? revoke_ebi_listList : NULL,
        cause ? causeVariable : 0,
        ng_ap_cause ? ng_ap_cause_local_nonprim : NULL,
        _5g_mm_cause_value ? _5g_mm_cause_value->valuedouble : 0,
        always_on_requested ? always_on_requested->valueint : 0,
        eps_interworking_ind ? eps_interworking_indVariable : 0,
        secondary_rat_usage_report ? secondary_rat_usage_reportList : NULL,
        secondary_rat_usage_info ? secondary_rat_usage_infoList : NULL,
        an_type_can_be_changed ? an_type_can_be_changed->valueint : 0,
        ma_release_ind ? ma_release_indVariable : 0,
        ma_nw_upgrade_ind ? ma_nw_upgrade_ind->valueint : 0,
        ma_request_ind ? ma_request_ind->valueint : 0,
        unavailable_access_ind ? unavailable_access_indVariable : 0,
        psa_info ? psa_infoList : NULL,
        ulcl_bp_info ? ulcl_bp_info_local_nonprim : NULL,
        n4_info ? n4_info_local_nonprim : NULL,
        n4_info_ext1 ? n4_info_ext1_local_nonprim : NULL,
        n4_info_ext2 ? n4_info_ext2_local_nonprim : NULL,
        presence_in_ladn ? presence_in_ladnVariable : 0,
        vsmf_pdu_session_uri ? ogs_strdup(vsmf_pdu_session_uri->valuestring) : NULL,
        vsmf_id ? ogs_strdup(vsmf_id->valuestring) : NULL,
        v_smf_service_instance_id ? ogs_strdup(v_smf_service_instance_id->valuestring) : NULL,
        ismf_pdu_session_uri ? ogs_strdup(ismf_pdu_session_uri->valuestring) : NULL,
        ismf_id ? ogs_strdup(ismf_id->valuestring) : NULL,
        i_smf_service_instance_id ? ogs_strdup(i_smf_service_instance_id->valuestring) : NULL,
        dl_serving_plmn_rate_ctl ? dl_serving_plmn_rate_ctl->valuedouble : 0,
        dnai_list ? dnai_listList : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        roaming_charging_profile ? roaming_charging_profile_local_nonprim : NULL,
        mo_exp_data_ind ? mo_exp_data_ind_local_nonprim : NULL,
        mo_exp_data_counter ? mo_exp_data_counter_local_nonprim : NULL,
        vplmn_qos ? vplmn_qos_local_nonprim : NULL
        );

    return hsmf_update_data_local_var;
end:
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

