
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_update_data.h"

OpenAPI_sm_context_update_data_t *OpenAPI_sm_context_update_data_create(
    char *pei,
    char *serving_nf_id,
    OpenAPI_guami_t *guami,
    OpenAPI_plmn_id_nid_t *serving_network,
    OpenAPI_list_t *backup_amf_info,
    OpenAPI_access_type_e an_type,
    OpenAPI_access_type_e additional_an_type,
    OpenAPI_access_type_e an_type_to_reactivate,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_presence_state_e presence_in_ladn,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone,
    OpenAPI_user_location_t *add_ue_location,
    OpenAPI_up_cnx_state_e up_cnx_state,
    OpenAPI_ho_state_e ho_state,
    bool is_to_be_switched,
    int to_be_switched,
    bool is_failed_to_be_switched,
    int failed_to_be_switched,
    OpenAPI_ref_to_binary_data_t *n1_sm_msg,
    OpenAPI_ref_to_binary_data_t *n2_sm_info,
    OpenAPI_n2_sm_info_type_e n2_sm_info_type,
    OpenAPI_ng_ran_target_id_t *target_id,
    char *target_serving_nf_id,
    char *sm_context_status_uri,
    bool is_data_forwarding,
    int data_forwarding,
    OpenAPI_tunnel_info_t *n9_forwarding_tunnel,
    OpenAPI_list_t *n9_dl_forwarding_tnl_list,
    OpenAPI_list_t *n9_ul_forwarding_tnl_list,
    OpenAPI_list_t *eps_bearer_setup,
    OpenAPI_list_t *revoke_ebi_list,
    bool is_release,
    int release,
    OpenAPI_cause_e cause,
    OpenAPI_ng_ap_cause_t *ng_ap_cause,
    bool is__5g_mm_cause_value,
    int _5g_mm_cause_value,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_trace_data_t *trace_data,
    OpenAPI_eps_interworking_indication_e eps_interworking_ind,
    bool is_an_type_can_be_changed,
    int an_type_can_be_changed,
    OpenAPI_ref_to_binary_data_t *n2_sm_info_ext1,
    OpenAPI_n2_sm_info_type_e n2_sm_info_type_ext1,
    OpenAPI_ma_release_indication_e ma_release_ind,
    bool is_ma_nw_upgrade_ind,
    int ma_nw_upgrade_ind,
    bool is_ma_request_ind,
    int ma_request_ind,
    OpenAPI_exemption_ind_t *exemption_ind,
    char *supported_features,
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter,
    bool is_extended_nas_sm_timer_ind,
    int extended_nas_sm_timer_ind,
    char forwarding_f_teid,
    OpenAPI_list_t *forwarding_bearer_contexts,
    OpenAPI_ddn_failure_subs_t *ddn_failure_subs
)
{
    OpenAPI_sm_context_update_data_t *sm_context_update_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_context_update_data_t));
    ogs_assert(sm_context_update_data_local_var);

    sm_context_update_data_local_var->pei = pei;
    sm_context_update_data_local_var->serving_nf_id = serving_nf_id;
    sm_context_update_data_local_var->guami = guami;
    sm_context_update_data_local_var->serving_network = serving_network;
    sm_context_update_data_local_var->backup_amf_info = backup_amf_info;
    sm_context_update_data_local_var->an_type = an_type;
    sm_context_update_data_local_var->additional_an_type = additional_an_type;
    sm_context_update_data_local_var->an_type_to_reactivate = an_type_to_reactivate;
    sm_context_update_data_local_var->rat_type = rat_type;
    sm_context_update_data_local_var->presence_in_ladn = presence_in_ladn;
    sm_context_update_data_local_var->ue_location = ue_location;
    sm_context_update_data_local_var->ue_time_zone = ue_time_zone;
    sm_context_update_data_local_var->add_ue_location = add_ue_location;
    sm_context_update_data_local_var->up_cnx_state = up_cnx_state;
    sm_context_update_data_local_var->ho_state = ho_state;
    sm_context_update_data_local_var->is_to_be_switched = is_to_be_switched;
    sm_context_update_data_local_var->to_be_switched = to_be_switched;
    sm_context_update_data_local_var->is_failed_to_be_switched = is_failed_to_be_switched;
    sm_context_update_data_local_var->failed_to_be_switched = failed_to_be_switched;
    sm_context_update_data_local_var->n1_sm_msg = n1_sm_msg;
    sm_context_update_data_local_var->n2_sm_info = n2_sm_info;
    sm_context_update_data_local_var->n2_sm_info_type = n2_sm_info_type;
    sm_context_update_data_local_var->target_id = target_id;
    sm_context_update_data_local_var->target_serving_nf_id = target_serving_nf_id;
    sm_context_update_data_local_var->sm_context_status_uri = sm_context_status_uri;
    sm_context_update_data_local_var->is_data_forwarding = is_data_forwarding;
    sm_context_update_data_local_var->data_forwarding = data_forwarding;
    sm_context_update_data_local_var->n9_forwarding_tunnel = n9_forwarding_tunnel;
    sm_context_update_data_local_var->n9_dl_forwarding_tnl_list = n9_dl_forwarding_tnl_list;
    sm_context_update_data_local_var->n9_ul_forwarding_tnl_list = n9_ul_forwarding_tnl_list;
    sm_context_update_data_local_var->eps_bearer_setup = eps_bearer_setup;
    sm_context_update_data_local_var->revoke_ebi_list = revoke_ebi_list;
    sm_context_update_data_local_var->is_release = is_release;
    sm_context_update_data_local_var->release = release;
    sm_context_update_data_local_var->cause = cause;
    sm_context_update_data_local_var->ng_ap_cause = ng_ap_cause;
    sm_context_update_data_local_var->is__5g_mm_cause_value = is__5g_mm_cause_value;
    sm_context_update_data_local_var->_5g_mm_cause_value = _5g_mm_cause_value;
    sm_context_update_data_local_var->s_nssai = s_nssai;
    sm_context_update_data_local_var->trace_data = trace_data;
    sm_context_update_data_local_var->eps_interworking_ind = eps_interworking_ind;
    sm_context_update_data_local_var->is_an_type_can_be_changed = is_an_type_can_be_changed;
    sm_context_update_data_local_var->an_type_can_be_changed = an_type_can_be_changed;
    sm_context_update_data_local_var->n2_sm_info_ext1 = n2_sm_info_ext1;
    sm_context_update_data_local_var->n2_sm_info_type_ext1 = n2_sm_info_type_ext1;
    sm_context_update_data_local_var->ma_release_ind = ma_release_ind;
    sm_context_update_data_local_var->is_ma_nw_upgrade_ind = is_ma_nw_upgrade_ind;
    sm_context_update_data_local_var->ma_nw_upgrade_ind = ma_nw_upgrade_ind;
    sm_context_update_data_local_var->is_ma_request_ind = is_ma_request_ind;
    sm_context_update_data_local_var->ma_request_ind = ma_request_ind;
    sm_context_update_data_local_var->exemption_ind = exemption_ind;
    sm_context_update_data_local_var->supported_features = supported_features;
    sm_context_update_data_local_var->mo_exp_data_counter = mo_exp_data_counter;
    sm_context_update_data_local_var->is_extended_nas_sm_timer_ind = is_extended_nas_sm_timer_ind;
    sm_context_update_data_local_var->extended_nas_sm_timer_ind = extended_nas_sm_timer_ind;
    sm_context_update_data_local_var->forwarding_f_teid = forwarding_f_teid;
    sm_context_update_data_local_var->forwarding_bearer_contexts = forwarding_bearer_contexts;
    sm_context_update_data_local_var->ddn_failure_subs = ddn_failure_subs;

    return sm_context_update_data_local_var;
}

void OpenAPI_sm_context_update_data_free(OpenAPI_sm_context_update_data_t *sm_context_update_data)
{
    if (NULL == sm_context_update_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sm_context_update_data->pei);
    ogs_free(sm_context_update_data->serving_nf_id);
    OpenAPI_guami_free(sm_context_update_data->guami);
    OpenAPI_plmn_id_nid_free(sm_context_update_data->serving_network);
    OpenAPI_list_for_each(sm_context_update_data->backup_amf_info, node) {
        OpenAPI_backup_amf_info_free(node->data);
    }
    OpenAPI_list_free(sm_context_update_data->backup_amf_info);
    OpenAPI_user_location_free(sm_context_update_data->ue_location);
    ogs_free(sm_context_update_data->ue_time_zone);
    OpenAPI_user_location_free(sm_context_update_data->add_ue_location);
    OpenAPI_ref_to_binary_data_free(sm_context_update_data->n1_sm_msg);
    OpenAPI_ref_to_binary_data_free(sm_context_update_data->n2_sm_info);
    OpenAPI_ng_ran_target_id_free(sm_context_update_data->target_id);
    ogs_free(sm_context_update_data->target_serving_nf_id);
    ogs_free(sm_context_update_data->sm_context_status_uri);
    OpenAPI_tunnel_info_free(sm_context_update_data->n9_forwarding_tunnel);
    OpenAPI_list_for_each(sm_context_update_data->n9_dl_forwarding_tnl_list, node) {
        OpenAPI_indirect_data_forwarding_tunnel_info_free(node->data);
    }
    OpenAPI_list_free(sm_context_update_data->n9_dl_forwarding_tnl_list);
    OpenAPI_list_for_each(sm_context_update_data->n9_ul_forwarding_tnl_list, node) {
        OpenAPI_indirect_data_forwarding_tunnel_info_free(node->data);
    }
    OpenAPI_list_free(sm_context_update_data->n9_ul_forwarding_tnl_list);
    OpenAPI_list_for_each(sm_context_update_data->eps_bearer_setup, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sm_context_update_data->eps_bearer_setup);
    OpenAPI_list_for_each(sm_context_update_data->revoke_ebi_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sm_context_update_data->revoke_ebi_list);
    OpenAPI_ng_ap_cause_free(sm_context_update_data->ng_ap_cause);
    OpenAPI_snssai_free(sm_context_update_data->s_nssai);
    OpenAPI_trace_data_free(sm_context_update_data->trace_data);
    OpenAPI_ref_to_binary_data_free(sm_context_update_data->n2_sm_info_ext1);
    OpenAPI_exemption_ind_free(sm_context_update_data->exemption_ind);
    ogs_free(sm_context_update_data->supported_features);
    OpenAPI_mo_exp_data_counter_free(sm_context_update_data->mo_exp_data_counter);
    OpenAPI_list_for_each(sm_context_update_data->forwarding_bearer_contexts, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sm_context_update_data->forwarding_bearer_contexts);
    OpenAPI_ddn_failure_subs_free(sm_context_update_data->ddn_failure_subs);
    ogs_free(sm_context_update_data);
}

cJSON *OpenAPI_sm_context_update_data_convertToJSON(OpenAPI_sm_context_update_data_t *sm_context_update_data)
{
    cJSON *item = NULL;

    if (sm_context_update_data == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [SmContextUpdateData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_context_update_data->pei) {
    if (cJSON_AddStringToObject(item, "pei", sm_context_update_data->pei) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (sm_context_update_data->serving_nf_id) {
    if (cJSON_AddStringToObject(item, "servingNfId", sm_context_update_data->serving_nf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [serving_nf_id]");
        goto end;
    }
    }

    if (sm_context_update_data->guami) {
    cJSON *guami_local_JSON = OpenAPI_guami_convertToJSON(sm_context_update_data->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [guami]");
        goto end;
    }
    }

    if (sm_context_update_data->serving_network) {
    cJSON *serving_network_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(sm_context_update_data->serving_network);
    if (serving_network_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetwork", serving_network_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    }

    if (sm_context_update_data->backup_amf_info) {
    cJSON *backup_amf_infoList = cJSON_AddArrayToObject(item, "backupAmfInfo");
    if (backup_amf_infoList == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [backup_amf_info]");
        goto end;
    }

    OpenAPI_lnode_t *backup_amf_info_node;
    if (sm_context_update_data->backup_amf_info) {
        OpenAPI_list_for_each(sm_context_update_data->backup_amf_info, backup_amf_info_node) {
            cJSON *itemLocal = OpenAPI_backup_amf_info_convertToJSON(backup_amf_info_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [backup_amf_info]");
                goto end;
            }
            cJSON_AddItemToArray(backup_amf_infoList, itemLocal);
        }
    }
    }

    if (sm_context_update_data->an_type) {
    if (cJSON_AddStringToObject(item, "anType", OpenAPI_access_type_ToString(sm_context_update_data->an_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [an_type]");
        goto end;
    }
    }

    if (sm_context_update_data->additional_an_type) {
    if (cJSON_AddStringToObject(item, "additionalAnType", OpenAPI_access_type_ToString(sm_context_update_data->additional_an_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [additional_an_type]");
        goto end;
    }
    }

    if (sm_context_update_data->an_type_to_reactivate) {
    if (cJSON_AddStringToObject(item, "anTypeToReactivate", OpenAPI_access_type_ToString(sm_context_update_data->an_type_to_reactivate)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [an_type_to_reactivate]");
        goto end;
    }
    }

    if (sm_context_update_data->rat_type) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(sm_context_update_data->rat_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (sm_context_update_data->presence_in_ladn) {
    if (cJSON_AddStringToObject(item, "presenceInLadn", OpenAPI_presence_state_ToString(sm_context_update_data->presence_in_ladn)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [presence_in_ladn]");
        goto end;
    }
    }

    if (sm_context_update_data->ue_location) {
    cJSON *ue_location_local_JSON = OpenAPI_user_location_convertToJSON(sm_context_update_data->ue_location);
    if (ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLocation", ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    }

    if (sm_context_update_data->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", sm_context_update_data->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (sm_context_update_data->add_ue_location) {
    cJSON *add_ue_location_local_JSON = OpenAPI_user_location_convertToJSON(sm_context_update_data->add_ue_location);
    if (add_ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [add_ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "addUeLocation", add_ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [add_ue_location]");
        goto end;
    }
    }

    if (sm_context_update_data->up_cnx_state) {
    if (cJSON_AddStringToObject(item, "upCnxState", OpenAPI_up_cnx_state_ToString(sm_context_update_data->up_cnx_state)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [up_cnx_state]");
        goto end;
    }
    }

    if (sm_context_update_data->ho_state) {
    if (cJSON_AddStringToObject(item, "hoState", OpenAPI_ho_state_ToString(sm_context_update_data->ho_state)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [ho_state]");
        goto end;
    }
    }

    if (sm_context_update_data->is_to_be_switched) {
    if (cJSON_AddBoolToObject(item, "toBeSwitched", sm_context_update_data->to_be_switched) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [to_be_switched]");
        goto end;
    }
    }

    if (sm_context_update_data->is_failed_to_be_switched) {
    if (cJSON_AddBoolToObject(item, "failedToBeSwitched", sm_context_update_data->failed_to_be_switched) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [failed_to_be_switched]");
        goto end;
    }
    }

    if (sm_context_update_data->n1_sm_msg) {
    cJSON *n1_sm_msg_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(sm_context_update_data->n1_sm_msg);
    if (n1_sm_msg_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n1_sm_msg]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n1SmMsg", n1_sm_msg_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n1_sm_msg]");
        goto end;
    }
    }

    if (sm_context_update_data->n2_sm_info) {
    cJSON *n2_sm_info_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(sm_context_update_data->n2_sm_info);
    if (n2_sm_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n2_sm_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2SmInfo", n2_sm_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n2_sm_info]");
        goto end;
    }
    }

    if (sm_context_update_data->n2_sm_info_type) {
    if (cJSON_AddStringToObject(item, "n2SmInfoType", OpenAPI_n2_sm_info_type_ToString(sm_context_update_data->n2_sm_info_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n2_sm_info_type]");
        goto end;
    }
    }

    if (sm_context_update_data->target_id) {
    cJSON *target_id_local_JSON = OpenAPI_ng_ran_target_id_convertToJSON(sm_context_update_data->target_id);
    if (target_id_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [target_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetId", target_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [target_id]");
        goto end;
    }
    }

    if (sm_context_update_data->target_serving_nf_id) {
    if (cJSON_AddStringToObject(item, "targetServingNfId", sm_context_update_data->target_serving_nf_id) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [target_serving_nf_id]");
        goto end;
    }
    }

    if (sm_context_update_data->sm_context_status_uri) {
    if (cJSON_AddStringToObject(item, "smContextStatusUri", sm_context_update_data->sm_context_status_uri) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [sm_context_status_uri]");
        goto end;
    }
    }

    if (sm_context_update_data->is_data_forwarding) {
    if (cJSON_AddBoolToObject(item, "dataForwarding", sm_context_update_data->data_forwarding) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [data_forwarding]");
        goto end;
    }
    }

    if (sm_context_update_data->n9_forwarding_tunnel) {
    cJSON *n9_forwarding_tunnel_local_JSON = OpenAPI_tunnel_info_convertToJSON(sm_context_update_data->n9_forwarding_tunnel);
    if (n9_forwarding_tunnel_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n9_forwarding_tunnel]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n9ForwardingTunnel", n9_forwarding_tunnel_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n9_forwarding_tunnel]");
        goto end;
    }
    }

    if (sm_context_update_data->n9_dl_forwarding_tnl_list) {
    cJSON *n9_dl_forwarding_tnl_listList = cJSON_AddArrayToObject(item, "n9DlForwardingTnlList");
    if (n9_dl_forwarding_tnl_listList == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n9_dl_forwarding_tnl_list]");
        goto end;
    }

    OpenAPI_lnode_t *n9_dl_forwarding_tnl_list_node;
    if (sm_context_update_data->n9_dl_forwarding_tnl_list) {
        OpenAPI_list_for_each(sm_context_update_data->n9_dl_forwarding_tnl_list, n9_dl_forwarding_tnl_list_node) {
            cJSON *itemLocal = OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON(n9_dl_forwarding_tnl_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n9_dl_forwarding_tnl_list]");
                goto end;
            }
            cJSON_AddItemToArray(n9_dl_forwarding_tnl_listList, itemLocal);
        }
    }
    }

    if (sm_context_update_data->n9_ul_forwarding_tnl_list) {
    cJSON *n9_ul_forwarding_tnl_listList = cJSON_AddArrayToObject(item, "n9UlForwardingTnlList");
    if (n9_ul_forwarding_tnl_listList == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n9_ul_forwarding_tnl_list]");
        goto end;
    }

    OpenAPI_lnode_t *n9_ul_forwarding_tnl_list_node;
    if (sm_context_update_data->n9_ul_forwarding_tnl_list) {
        OpenAPI_list_for_each(sm_context_update_data->n9_ul_forwarding_tnl_list, n9_ul_forwarding_tnl_list_node) {
            cJSON *itemLocal = OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON(n9_ul_forwarding_tnl_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n9_ul_forwarding_tnl_list]");
                goto end;
            }
            cJSON_AddItemToArray(n9_ul_forwarding_tnl_listList, itemLocal);
        }
    }
    }

    if (sm_context_update_data->eps_bearer_setup) {
    cJSON *eps_bearer_setup = cJSON_AddArrayToObject(item, "epsBearerSetup");
    if (eps_bearer_setup == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [eps_bearer_setup]");
        goto end;
    }

    OpenAPI_lnode_t *eps_bearer_setup_node;
    OpenAPI_list_for_each(sm_context_update_data->eps_bearer_setup, eps_bearer_setup_node)  {
    if (cJSON_AddStringToObject(eps_bearer_setup, "", (char*)eps_bearer_setup_node->data) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [eps_bearer_setup]");
        goto end;
    }
                    }
    }

    if (sm_context_update_data->revoke_ebi_list) {
    cJSON *revoke_ebi_list = cJSON_AddArrayToObject(item, "revokeEbiList");
    if (revoke_ebi_list == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [revoke_ebi_list]");
        goto end;
    }

    OpenAPI_lnode_t *revoke_ebi_list_node;
    OpenAPI_list_for_each(sm_context_update_data->revoke_ebi_list, revoke_ebi_list_node)  {
    if (cJSON_AddNumberToObject(revoke_ebi_list, "", *(double *)revoke_ebi_list_node->data) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [revoke_ebi_list]");
        goto end;
    }
                    }
    }

    if (sm_context_update_data->is_release) {
    if (cJSON_AddBoolToObject(item, "release", sm_context_update_data->release) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [release]");
        goto end;
    }
    }

    if (sm_context_update_data->cause) {
    if (cJSON_AddStringToObject(item, "cause", OpenAPI_cause_ToString(sm_context_update_data->cause)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [cause]");
        goto end;
    }
    }

    if (sm_context_update_data->ng_ap_cause) {
    cJSON *ng_ap_cause_local_JSON = OpenAPI_ng_ap_cause_convertToJSON(sm_context_update_data->ng_ap_cause);
    if (ng_ap_cause_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [ng_ap_cause]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ngApCause", ng_ap_cause_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [ng_ap_cause]");
        goto end;
    }
    }

    if (sm_context_update_data->is__5g_mm_cause_value) {
    if (cJSON_AddNumberToObject(item, "5gMmCauseValue", sm_context_update_data->_5g_mm_cause_value) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [_5g_mm_cause_value]");
        goto end;
    }
    }

    if (sm_context_update_data->s_nssai) {
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(sm_context_update_data->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [s_nssai]");
        goto end;
    }
    }

    if (sm_context_update_data->trace_data) {
    cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(sm_context_update_data->trace_data);
    if (trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [trace_data]");
        goto end;
    }
    }

    if (sm_context_update_data->eps_interworking_ind) {
    if (cJSON_AddStringToObject(item, "epsInterworkingInd", OpenAPI_eps_interworking_indication_ToString(sm_context_update_data->eps_interworking_ind)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [eps_interworking_ind]");
        goto end;
    }
    }

    if (sm_context_update_data->is_an_type_can_be_changed) {
    if (cJSON_AddBoolToObject(item, "anTypeCanBeChanged", sm_context_update_data->an_type_can_be_changed) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [an_type_can_be_changed]");
        goto end;
    }
    }

    if (sm_context_update_data->n2_sm_info_ext1) {
    cJSON *n2_sm_info_ext1_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(sm_context_update_data->n2_sm_info_ext1);
    if (n2_sm_info_ext1_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n2_sm_info_ext1]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2SmInfoExt1", n2_sm_info_ext1_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n2_sm_info_ext1]");
        goto end;
    }
    }

    if (sm_context_update_data->n2_sm_info_type_ext1) {
    if (cJSON_AddStringToObject(item, "n2SmInfoTypeExt1", OpenAPI_n2_sm_info_type_ToString(sm_context_update_data->n2_sm_info_type_ext1)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n2_sm_info_type_ext1]");
        goto end;
    }
    }

    if (sm_context_update_data->ma_release_ind) {
    if (cJSON_AddStringToObject(item, "maReleaseInd", OpenAPI_ma_release_indication_ToString(sm_context_update_data->ma_release_ind)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [ma_release_ind]");
        goto end;
    }
    }

    if (sm_context_update_data->is_ma_nw_upgrade_ind) {
    if (cJSON_AddBoolToObject(item, "maNwUpgradeInd", sm_context_update_data->ma_nw_upgrade_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [ma_nw_upgrade_ind]");
        goto end;
    }
    }

    if (sm_context_update_data->is_ma_request_ind) {
    if (cJSON_AddBoolToObject(item, "maRequestInd", sm_context_update_data->ma_request_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [ma_request_ind]");
        goto end;
    }
    }

    if (sm_context_update_data->exemption_ind) {
    cJSON *exemption_ind_local_JSON = OpenAPI_exemption_ind_convertToJSON(sm_context_update_data->exemption_ind);
    if (exemption_ind_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [exemption_ind]");
        goto end;
    }
    cJSON_AddItemToObject(item, "exemptionInd", exemption_ind_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [exemption_ind]");
        goto end;
    }
    }

    if (sm_context_update_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", sm_context_update_data->supported_features) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (sm_context_update_data->mo_exp_data_counter) {
    cJSON *mo_exp_data_counter_local_JSON = OpenAPI_mo_exp_data_counter_convertToJSON(sm_context_update_data->mo_exp_data_counter);
    if (mo_exp_data_counter_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [mo_exp_data_counter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "moExpDataCounter", mo_exp_data_counter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [mo_exp_data_counter]");
        goto end;
    }
    }

    if (sm_context_update_data->is_extended_nas_sm_timer_ind) {
    if (cJSON_AddBoolToObject(item, "extendedNasSmTimerInd", sm_context_update_data->extended_nas_sm_timer_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [extended_nas_sm_timer_ind]");
        goto end;
    }
    }

    if (sm_context_update_data->forwarding_f_teid) {
    if (cJSON_AddNumberToObject(item, "forwardingFTeid", sm_context_update_data->forwarding_f_teid) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [forwarding_f_teid]");
        goto end;
    }
    }

    if (sm_context_update_data->forwarding_bearer_contexts) {
    cJSON *forwarding_bearer_contexts = cJSON_AddArrayToObject(item, "forwardingBearerContexts");
    if (forwarding_bearer_contexts == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [forwarding_bearer_contexts]");
        goto end;
    }

    OpenAPI_lnode_t *forwarding_bearer_contexts_node;
    OpenAPI_list_for_each(sm_context_update_data->forwarding_bearer_contexts, forwarding_bearer_contexts_node)  {
    if (cJSON_AddStringToObject(forwarding_bearer_contexts, "", (char*)forwarding_bearer_contexts_node->data) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [forwarding_bearer_contexts]");
        goto end;
    }
                    }
    }

    if (sm_context_update_data->ddn_failure_subs) {
    cJSON *ddn_failure_subs_local_JSON = OpenAPI_ddn_failure_subs_convertToJSON(sm_context_update_data->ddn_failure_subs);
    if (ddn_failure_subs_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [ddn_failure_subs]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ddnFailureSubs", ddn_failure_subs_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [ddn_failure_subs]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_context_update_data_t *OpenAPI_sm_context_update_data_parseFromJSON(cJSON *sm_context_update_dataJSON)
{
    OpenAPI_sm_context_update_data_t *sm_context_update_data_local_var = NULL;
    cJSON *pei = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "pei");

    if (pei) {
    if (!cJSON_IsString(pei)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    cJSON *serving_nf_id = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "servingNfId");

    if (serving_nf_id) {
    if (!cJSON_IsString(serving_nf_id)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [serving_nf_id]");
        goto end;
    }
    }

    cJSON *guami = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "guami");

    OpenAPI_guami_t *guami_local_nonprim = NULL;
    if (guami) {
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    }

    cJSON *serving_network = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "servingNetwork");

    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    if (serving_network) {
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    }

    cJSON *backup_amf_info = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "backupAmfInfo");

    OpenAPI_list_t *backup_amf_infoList;
    if (backup_amf_info) {
    cJSON *backup_amf_info_local_nonprimitive;
    if (!cJSON_IsArray(backup_amf_info)){
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [backup_amf_info]");
        goto end;
    }

    backup_amf_infoList = OpenAPI_list_create();

    cJSON_ArrayForEach(backup_amf_info_local_nonprimitive, backup_amf_info ) {
        if (!cJSON_IsObject(backup_amf_info_local_nonprimitive)) {
            ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [backup_amf_info]");
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

    cJSON *an_type = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "anType");

    OpenAPI_access_type_e an_typeVariable;
    if (an_type) {
    if (!cJSON_IsString(an_type)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [an_type]");
        goto end;
    }
    an_typeVariable = OpenAPI_access_type_FromString(an_type->valuestring);
    }

    cJSON *additional_an_type = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "additionalAnType");

    OpenAPI_access_type_e additional_an_typeVariable;
    if (additional_an_type) {
    if (!cJSON_IsString(additional_an_type)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [additional_an_type]");
        goto end;
    }
    additional_an_typeVariable = OpenAPI_access_type_FromString(additional_an_type->valuestring);
    }

    cJSON *an_type_to_reactivate = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "anTypeToReactivate");

    OpenAPI_access_type_e an_type_to_reactivateVariable;
    if (an_type_to_reactivate) {
    if (!cJSON_IsString(an_type_to_reactivate)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [an_type_to_reactivate]");
        goto end;
    }
    an_type_to_reactivateVariable = OpenAPI_access_type_FromString(an_type_to_reactivate->valuestring);
    }

    cJSON *rat_type = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "ratType");

    OpenAPI_rat_type_e rat_typeVariable;
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    cJSON *presence_in_ladn = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "presenceInLadn");

    OpenAPI_presence_state_e presence_in_ladnVariable;
    if (presence_in_ladn) {
    if (!cJSON_IsString(presence_in_ladn)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [presence_in_ladn]");
        goto end;
    }
    presence_in_ladnVariable = OpenAPI_presence_state_FromString(presence_in_ladn->valuestring);
    }

    cJSON *ue_location = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "ueLocation");

    OpenAPI_user_location_t *ue_location_local_nonprim = NULL;
    if (ue_location) {
    ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    }

    cJSON *ue_time_zone = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "ueTimeZone");

    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    cJSON *add_ue_location = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "addUeLocation");

    OpenAPI_user_location_t *add_ue_location_local_nonprim = NULL;
    if (add_ue_location) {
    add_ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(add_ue_location);
    }

    cJSON *up_cnx_state = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "upCnxState");

    OpenAPI_up_cnx_state_e up_cnx_stateVariable;
    if (up_cnx_state) {
    if (!cJSON_IsString(up_cnx_state)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [up_cnx_state]");
        goto end;
    }
    up_cnx_stateVariable = OpenAPI_up_cnx_state_FromString(up_cnx_state->valuestring);
    }

    cJSON *ho_state = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "hoState");

    OpenAPI_ho_state_e ho_stateVariable;
    if (ho_state) {
    if (!cJSON_IsString(ho_state)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [ho_state]");
        goto end;
    }
    ho_stateVariable = OpenAPI_ho_state_FromString(ho_state->valuestring);
    }

    cJSON *to_be_switched = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "toBeSwitched");

    if (to_be_switched) {
    if (!cJSON_IsBool(to_be_switched)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [to_be_switched]");
        goto end;
    }
    }

    cJSON *failed_to_be_switched = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "failedToBeSwitched");

    if (failed_to_be_switched) {
    if (!cJSON_IsBool(failed_to_be_switched)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [failed_to_be_switched]");
        goto end;
    }
    }

    cJSON *n1_sm_msg = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n1SmMsg");

    OpenAPI_ref_to_binary_data_t *n1_sm_msg_local_nonprim = NULL;
    if (n1_sm_msg) {
    n1_sm_msg_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_msg);
    }

    cJSON *n2_sm_info = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n2SmInfo");

    OpenAPI_ref_to_binary_data_t *n2_sm_info_local_nonprim = NULL;
    if (n2_sm_info) {
    n2_sm_info_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n2_sm_info);
    }

    cJSON *n2_sm_info_type = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n2SmInfoType");

    OpenAPI_n2_sm_info_type_e n2_sm_info_typeVariable;
    if (n2_sm_info_type) {
    if (!cJSON_IsString(n2_sm_info_type)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [n2_sm_info_type]");
        goto end;
    }
    n2_sm_info_typeVariable = OpenAPI_n2_sm_info_type_FromString(n2_sm_info_type->valuestring);
    }

    cJSON *target_id = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "targetId");

    OpenAPI_ng_ran_target_id_t *target_id_local_nonprim = NULL;
    if (target_id) {
    target_id_local_nonprim = OpenAPI_ng_ran_target_id_parseFromJSON(target_id);
    }

    cJSON *target_serving_nf_id = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "targetServingNfId");

    if (target_serving_nf_id) {
    if (!cJSON_IsString(target_serving_nf_id)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [target_serving_nf_id]");
        goto end;
    }
    }

    cJSON *sm_context_status_uri = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "smContextStatusUri");

    if (sm_context_status_uri) {
    if (!cJSON_IsString(sm_context_status_uri)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [sm_context_status_uri]");
        goto end;
    }
    }

    cJSON *data_forwarding = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "dataForwarding");

    if (data_forwarding) {
    if (!cJSON_IsBool(data_forwarding)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [data_forwarding]");
        goto end;
    }
    }

    cJSON *n9_forwarding_tunnel = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n9ForwardingTunnel");

    OpenAPI_tunnel_info_t *n9_forwarding_tunnel_local_nonprim = NULL;
    if (n9_forwarding_tunnel) {
    n9_forwarding_tunnel_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(n9_forwarding_tunnel);
    }

    cJSON *n9_dl_forwarding_tnl_list = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n9DlForwardingTnlList");

    OpenAPI_list_t *n9_dl_forwarding_tnl_listList;
    if (n9_dl_forwarding_tnl_list) {
    cJSON *n9_dl_forwarding_tnl_list_local_nonprimitive;
    if (!cJSON_IsArray(n9_dl_forwarding_tnl_list)){
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [n9_dl_forwarding_tnl_list]");
        goto end;
    }

    n9_dl_forwarding_tnl_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(n9_dl_forwarding_tnl_list_local_nonprimitive, n9_dl_forwarding_tnl_list ) {
        if (!cJSON_IsObject(n9_dl_forwarding_tnl_list_local_nonprimitive)) {
            ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [n9_dl_forwarding_tnl_list]");
            goto end;
        }
        OpenAPI_indirect_data_forwarding_tunnel_info_t *n9_dl_forwarding_tnl_listItem = OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON(n9_dl_forwarding_tnl_list_local_nonprimitive);

        if (!n9_dl_forwarding_tnl_listItem) {
            ogs_error("No n9_dl_forwarding_tnl_listItem");
            OpenAPI_list_free(n9_dl_forwarding_tnl_listList);
            goto end;
        }

        OpenAPI_list_add(n9_dl_forwarding_tnl_listList, n9_dl_forwarding_tnl_listItem);
    }
    }

    cJSON *n9_ul_forwarding_tnl_list = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n9UlForwardingTnlList");

    OpenAPI_list_t *n9_ul_forwarding_tnl_listList;
    if (n9_ul_forwarding_tnl_list) {
    cJSON *n9_ul_forwarding_tnl_list_local_nonprimitive;
    if (!cJSON_IsArray(n9_ul_forwarding_tnl_list)){
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [n9_ul_forwarding_tnl_list]");
        goto end;
    }

    n9_ul_forwarding_tnl_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(n9_ul_forwarding_tnl_list_local_nonprimitive, n9_ul_forwarding_tnl_list ) {
        if (!cJSON_IsObject(n9_ul_forwarding_tnl_list_local_nonprimitive)) {
            ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [n9_ul_forwarding_tnl_list]");
            goto end;
        }
        OpenAPI_indirect_data_forwarding_tunnel_info_t *n9_ul_forwarding_tnl_listItem = OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON(n9_ul_forwarding_tnl_list_local_nonprimitive);

        if (!n9_ul_forwarding_tnl_listItem) {
            ogs_error("No n9_ul_forwarding_tnl_listItem");
            OpenAPI_list_free(n9_ul_forwarding_tnl_listList);
            goto end;
        }

        OpenAPI_list_add(n9_ul_forwarding_tnl_listList, n9_ul_forwarding_tnl_listItem);
    }
    }

    cJSON *eps_bearer_setup = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "epsBearerSetup");

    OpenAPI_list_t *eps_bearer_setupList;
    if (eps_bearer_setup) {
    cJSON *eps_bearer_setup_local;
    if (!cJSON_IsArray(eps_bearer_setup)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [eps_bearer_setup]");
        goto end;
    }
    eps_bearer_setupList = OpenAPI_list_create();

    cJSON_ArrayForEach(eps_bearer_setup_local, eps_bearer_setup) {
    if (!cJSON_IsString(eps_bearer_setup_local)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [eps_bearer_setup]");
        goto end;
    }
    OpenAPI_list_add(eps_bearer_setupList , ogs_strdup(eps_bearer_setup_local->valuestring));
    }
    }

    cJSON *revoke_ebi_list = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "revokeEbiList");

    OpenAPI_list_t *revoke_ebi_listList;
    if (revoke_ebi_list) {
    cJSON *revoke_ebi_list_local;
    if (!cJSON_IsArray(revoke_ebi_list)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [revoke_ebi_list]");
        goto end;
    }
    revoke_ebi_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(revoke_ebi_list_local, revoke_ebi_list) {
    if (!cJSON_IsNumber(revoke_ebi_list_local)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [revoke_ebi_list]");
        goto end;
    }
    OpenAPI_list_add(revoke_ebi_listList , &revoke_ebi_list_local->valuedouble);
    }
    }

    cJSON *release = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "release");

    if (release) {
    if (!cJSON_IsBool(release)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [release]");
        goto end;
    }
    }

    cJSON *cause = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "cause");

    OpenAPI_cause_e causeVariable;
    if (cause) {
    if (!cJSON_IsString(cause)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [cause]");
        goto end;
    }
    causeVariable = OpenAPI_cause_FromString(cause->valuestring);
    }

    cJSON *ng_ap_cause = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "ngApCause");

    OpenAPI_ng_ap_cause_t *ng_ap_cause_local_nonprim = NULL;
    if (ng_ap_cause) {
    ng_ap_cause_local_nonprim = OpenAPI_ng_ap_cause_parseFromJSON(ng_ap_cause);
    }

    cJSON *_5g_mm_cause_value = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "5gMmCauseValue");

    if (_5g_mm_cause_value) {
    if (!cJSON_IsNumber(_5g_mm_cause_value)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [_5g_mm_cause_value]");
        goto end;
    }
    }

    cJSON *s_nssai = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "sNssai");

    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    if (s_nssai) {
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    }

    cJSON *trace_data = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "traceData");

    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    if (trace_data) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    }

    cJSON *eps_interworking_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "epsInterworkingInd");

    OpenAPI_eps_interworking_indication_e eps_interworking_indVariable;
    if (eps_interworking_ind) {
    if (!cJSON_IsString(eps_interworking_ind)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [eps_interworking_ind]");
        goto end;
    }
    eps_interworking_indVariable = OpenAPI_eps_interworking_indication_FromString(eps_interworking_ind->valuestring);
    }

    cJSON *an_type_can_be_changed = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "anTypeCanBeChanged");

    if (an_type_can_be_changed) {
    if (!cJSON_IsBool(an_type_can_be_changed)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [an_type_can_be_changed]");
        goto end;
    }
    }

    cJSON *n2_sm_info_ext1 = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n2SmInfoExt1");

    OpenAPI_ref_to_binary_data_t *n2_sm_info_ext1_local_nonprim = NULL;
    if (n2_sm_info_ext1) {
    n2_sm_info_ext1_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n2_sm_info_ext1);
    }

    cJSON *n2_sm_info_type_ext1 = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n2SmInfoTypeExt1");

    OpenAPI_n2_sm_info_type_e n2_sm_info_type_ext1Variable;
    if (n2_sm_info_type_ext1) {
    if (!cJSON_IsString(n2_sm_info_type_ext1)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [n2_sm_info_type_ext1]");
        goto end;
    }
    n2_sm_info_type_ext1Variable = OpenAPI_n2_sm_info_type_FromString(n2_sm_info_type_ext1->valuestring);
    }

    cJSON *ma_release_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "maReleaseInd");

    OpenAPI_ma_release_indication_e ma_release_indVariable;
    if (ma_release_ind) {
    if (!cJSON_IsString(ma_release_ind)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [ma_release_ind]");
        goto end;
    }
    ma_release_indVariable = OpenAPI_ma_release_indication_FromString(ma_release_ind->valuestring);
    }

    cJSON *ma_nw_upgrade_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "maNwUpgradeInd");

    if (ma_nw_upgrade_ind) {
    if (!cJSON_IsBool(ma_nw_upgrade_ind)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [ma_nw_upgrade_ind]");
        goto end;
    }
    }

    cJSON *ma_request_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "maRequestInd");

    if (ma_request_ind) {
    if (!cJSON_IsBool(ma_request_ind)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [ma_request_ind]");
        goto end;
    }
    }

    cJSON *exemption_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "exemptionInd");

    OpenAPI_exemption_ind_t *exemption_ind_local_nonprim = NULL;
    if (exemption_ind) {
    exemption_ind_local_nonprim = OpenAPI_exemption_ind_parseFromJSON(exemption_ind);
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cJSON *mo_exp_data_counter = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "moExpDataCounter");

    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter_local_nonprim = NULL;
    if (mo_exp_data_counter) {
    mo_exp_data_counter_local_nonprim = OpenAPI_mo_exp_data_counter_parseFromJSON(mo_exp_data_counter);
    }

    cJSON *extended_nas_sm_timer_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "extendedNasSmTimerInd");

    if (extended_nas_sm_timer_ind) {
    if (!cJSON_IsBool(extended_nas_sm_timer_ind)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [extended_nas_sm_timer_ind]");
        goto end;
    }
    }

    cJSON *forwarding_f_teid = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "forwardingFTeid");

    if (forwarding_f_teid) {
    if (!cJSON_IsNumber(forwarding_f_teid)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [forwarding_f_teid]");
        goto end;
    }
    }

    cJSON *forwarding_bearer_contexts = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "forwardingBearerContexts");

    OpenAPI_list_t *forwarding_bearer_contextsList;
    if (forwarding_bearer_contexts) {
    cJSON *forwarding_bearer_contexts_local;
    if (!cJSON_IsArray(forwarding_bearer_contexts)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [forwarding_bearer_contexts]");
        goto end;
    }
    forwarding_bearer_contextsList = OpenAPI_list_create();

    cJSON_ArrayForEach(forwarding_bearer_contexts_local, forwarding_bearer_contexts) {
    if (!cJSON_IsString(forwarding_bearer_contexts_local)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [forwarding_bearer_contexts]");
        goto end;
    }
    OpenAPI_list_add(forwarding_bearer_contextsList , ogs_strdup(forwarding_bearer_contexts_local->valuestring));
    }
    }

    cJSON *ddn_failure_subs = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "ddnFailureSubs");

    OpenAPI_ddn_failure_subs_t *ddn_failure_subs_local_nonprim = NULL;
    if (ddn_failure_subs) {
    ddn_failure_subs_local_nonprim = OpenAPI_ddn_failure_subs_parseFromJSON(ddn_failure_subs);
    }

    sm_context_update_data_local_var = OpenAPI_sm_context_update_data_create (
        pei ? ogs_strdup(pei->valuestring) : NULL,
        serving_nf_id ? ogs_strdup(serving_nf_id->valuestring) : NULL,
        guami ? guami_local_nonprim : NULL,
        serving_network ? serving_network_local_nonprim : NULL,
        backup_amf_info ? backup_amf_infoList : NULL,
        an_type ? an_typeVariable : 0,
        additional_an_type ? additional_an_typeVariable : 0,
        an_type_to_reactivate ? an_type_to_reactivateVariable : 0,
        rat_type ? rat_typeVariable : 0,
        presence_in_ladn ? presence_in_ladnVariable : 0,
        ue_location ? ue_location_local_nonprim : NULL,
        ue_time_zone ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        add_ue_location ? add_ue_location_local_nonprim : NULL,
        up_cnx_state ? up_cnx_stateVariable : 0,
        ho_state ? ho_stateVariable : 0,
        to_be_switched ? true : false,
        to_be_switched ? to_be_switched->valueint : 0,
        failed_to_be_switched ? true : false,
        failed_to_be_switched ? failed_to_be_switched->valueint : 0,
        n1_sm_msg ? n1_sm_msg_local_nonprim : NULL,
        n2_sm_info ? n2_sm_info_local_nonprim : NULL,
        n2_sm_info_type ? n2_sm_info_typeVariable : 0,
        target_id ? target_id_local_nonprim : NULL,
        target_serving_nf_id ? ogs_strdup(target_serving_nf_id->valuestring) : NULL,
        sm_context_status_uri ? ogs_strdup(sm_context_status_uri->valuestring) : NULL,
        data_forwarding ? true : false,
        data_forwarding ? data_forwarding->valueint : 0,
        n9_forwarding_tunnel ? n9_forwarding_tunnel_local_nonprim : NULL,
        n9_dl_forwarding_tnl_list ? n9_dl_forwarding_tnl_listList : NULL,
        n9_ul_forwarding_tnl_list ? n9_ul_forwarding_tnl_listList : NULL,
        eps_bearer_setup ? eps_bearer_setupList : NULL,
        revoke_ebi_list ? revoke_ebi_listList : NULL,
        release ? true : false,
        release ? release->valueint : 0,
        cause ? causeVariable : 0,
        ng_ap_cause ? ng_ap_cause_local_nonprim : NULL,
        _5g_mm_cause_value ? true : false,
        _5g_mm_cause_value ? _5g_mm_cause_value->valuedouble : 0,
        s_nssai ? s_nssai_local_nonprim : NULL,
        trace_data ? trace_data_local_nonprim : NULL,
        eps_interworking_ind ? eps_interworking_indVariable : 0,
        an_type_can_be_changed ? true : false,
        an_type_can_be_changed ? an_type_can_be_changed->valueint : 0,
        n2_sm_info_ext1 ? n2_sm_info_ext1_local_nonprim : NULL,
        n2_sm_info_type_ext1 ? n2_sm_info_type_ext1Variable : 0,
        ma_release_ind ? ma_release_indVariable : 0,
        ma_nw_upgrade_ind ? true : false,
        ma_nw_upgrade_ind ? ma_nw_upgrade_ind->valueint : 0,
        ma_request_ind ? true : false,
        ma_request_ind ? ma_request_ind->valueint : 0,
        exemption_ind ? exemption_ind_local_nonprim : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        mo_exp_data_counter ? mo_exp_data_counter_local_nonprim : NULL,
        extended_nas_sm_timer_ind ? true : false,
        extended_nas_sm_timer_ind ? extended_nas_sm_timer_ind->valueint : 0,
        forwarding_f_teid ? forwarding_f_teid->valueint : 0,
        forwarding_bearer_contexts ? forwarding_bearer_contextsList : NULL,
        ddn_failure_subs ? ddn_failure_subs_local_nonprim : NULL
    );

    return sm_context_update_data_local_var;
end:
    return NULL;
}

OpenAPI_sm_context_update_data_t *OpenAPI_sm_context_update_data_copy(OpenAPI_sm_context_update_data_t *dst, OpenAPI_sm_context_update_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_update_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed");
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

    OpenAPI_sm_context_update_data_free(dst);
    dst = OpenAPI_sm_context_update_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

