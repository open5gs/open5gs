
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_update_data.h"

char *OpenAPI_sm_policy_notify_indsm_context_update_data_ToString(OpenAPI_sm_context_update_data_sm_policy_notify_ind_e sm_policy_notify_ind)
{
    const char *sm_policy_notify_indArray[] =  { "NULL", "true" };
    size_t sizeofArray = sizeof(sm_policy_notify_indArray) / sizeof(sm_policy_notify_indArray[0]);
    if (sm_policy_notify_ind < sizeofArray)
        return (char *)sm_policy_notify_indArray[sm_policy_notify_ind];
    else
        return (char *)"Unknown";
}

OpenAPI_sm_context_update_data_sm_policy_notify_ind_e OpenAPI_sm_policy_notify_indsm_context_update_data_FromString(char* sm_policy_notify_ind)
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
OpenAPI_sm_context_update_data_t *OpenAPI_sm_context_update_data_create(
    char *pei,
    char *serving_nf_id,
    OpenAPI_guami_t *guami,
    OpenAPI_plmn_id_nid_t *serving_network,
    bool is_backup_amf_info_null,
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
    OpenAPI_tunnel_info_t *n9_dl_forwarding_tunnel,
    bool is_n9_inactivity_timer,
    int n9_inactivity_timer,
    OpenAPI_list_t *eps_bearer_setup,
    OpenAPI_list_t *revoke_ebi_list,
    bool is_release,
    int release,
    OpenAPI_cause_e cause,
    OpenAPI_ng_ap_cause_t *ng_ap_cause,
    bool is__5g_mm_cause_value,
    int _5g_mm_cause_value,
    OpenAPI_snssai_t *s_nssai,
    bool is_trace_data_null,
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
    char *forwarding_f_teid,
    OpenAPI_list_t *forwarding_bearer_contexts,
    OpenAPI_ddn_failure_subs_t *ddn_failure_subs,
    bool is_skip_n2_pdu_session_res_rel_ind,
    int skip_n2_pdu_session_res_rel_ind,
    OpenAPI_list_t *secondary_rat_usage_data_report_container,
    OpenAPI_sm_context_update_data_sm_policy_notify_ind_e sm_policy_notify_ind,
    bool is_pcf_ue_callback_info_null,
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info,
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_cat
)
{
    OpenAPI_sm_context_update_data_t *sm_context_update_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_context_update_data_t));
    ogs_assert(sm_context_update_data_local_var);

    sm_context_update_data_local_var->pei = pei;
    sm_context_update_data_local_var->serving_nf_id = serving_nf_id;
    sm_context_update_data_local_var->guami = guami;
    sm_context_update_data_local_var->serving_network = serving_network;
    sm_context_update_data_local_var->is_backup_amf_info_null = is_backup_amf_info_null;
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
    sm_context_update_data_local_var->n9_dl_forwarding_tunnel = n9_dl_forwarding_tunnel;
    sm_context_update_data_local_var->is_n9_inactivity_timer = is_n9_inactivity_timer;
    sm_context_update_data_local_var->n9_inactivity_timer = n9_inactivity_timer;
    sm_context_update_data_local_var->eps_bearer_setup = eps_bearer_setup;
    sm_context_update_data_local_var->revoke_ebi_list = revoke_ebi_list;
    sm_context_update_data_local_var->is_release = is_release;
    sm_context_update_data_local_var->release = release;
    sm_context_update_data_local_var->cause = cause;
    sm_context_update_data_local_var->ng_ap_cause = ng_ap_cause;
    sm_context_update_data_local_var->is__5g_mm_cause_value = is__5g_mm_cause_value;
    sm_context_update_data_local_var->_5g_mm_cause_value = _5g_mm_cause_value;
    sm_context_update_data_local_var->s_nssai = s_nssai;
    sm_context_update_data_local_var->is_trace_data_null = is_trace_data_null;
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
    sm_context_update_data_local_var->is_skip_n2_pdu_session_res_rel_ind = is_skip_n2_pdu_session_res_rel_ind;
    sm_context_update_data_local_var->skip_n2_pdu_session_res_rel_ind = skip_n2_pdu_session_res_rel_ind;
    sm_context_update_data_local_var->secondary_rat_usage_data_report_container = secondary_rat_usage_data_report_container;
    sm_context_update_data_local_var->sm_policy_notify_ind = sm_policy_notify_ind;
    sm_context_update_data_local_var->is_pcf_ue_callback_info_null = is_pcf_ue_callback_info_null;
    sm_context_update_data_local_var->pcf_ue_callback_info = pcf_ue_callback_info;
    sm_context_update_data_local_var->satellite_backhaul_cat = satellite_backhaul_cat;

    return sm_context_update_data_local_var;
}

void OpenAPI_sm_context_update_data_free(OpenAPI_sm_context_update_data_t *sm_context_update_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_context_update_data) {
        return;
    }
    if (sm_context_update_data->pei) {
        ogs_free(sm_context_update_data->pei);
        sm_context_update_data->pei = NULL;
    }
    if (sm_context_update_data->serving_nf_id) {
        ogs_free(sm_context_update_data->serving_nf_id);
        sm_context_update_data->serving_nf_id = NULL;
    }
    if (sm_context_update_data->guami) {
        OpenAPI_guami_free(sm_context_update_data->guami);
        sm_context_update_data->guami = NULL;
    }
    if (sm_context_update_data->serving_network) {
        OpenAPI_plmn_id_nid_free(sm_context_update_data->serving_network);
        sm_context_update_data->serving_network = NULL;
    }
    if (sm_context_update_data->backup_amf_info) {
        OpenAPI_list_for_each(sm_context_update_data->backup_amf_info, node) {
            OpenAPI_backup_amf_info_free(node->data);
        }
        OpenAPI_list_free(sm_context_update_data->backup_amf_info);
        sm_context_update_data->backup_amf_info = NULL;
    }
    if (sm_context_update_data->ue_location) {
        OpenAPI_user_location_free(sm_context_update_data->ue_location);
        sm_context_update_data->ue_location = NULL;
    }
    if (sm_context_update_data->ue_time_zone) {
        ogs_free(sm_context_update_data->ue_time_zone);
        sm_context_update_data->ue_time_zone = NULL;
    }
    if (sm_context_update_data->add_ue_location) {
        OpenAPI_user_location_free(sm_context_update_data->add_ue_location);
        sm_context_update_data->add_ue_location = NULL;
    }
    if (sm_context_update_data->n1_sm_msg) {
        OpenAPI_ref_to_binary_data_free(sm_context_update_data->n1_sm_msg);
        sm_context_update_data->n1_sm_msg = NULL;
    }
    if (sm_context_update_data->n2_sm_info) {
        OpenAPI_ref_to_binary_data_free(sm_context_update_data->n2_sm_info);
        sm_context_update_data->n2_sm_info = NULL;
    }
    if (sm_context_update_data->target_id) {
        OpenAPI_ng_ran_target_id_free(sm_context_update_data->target_id);
        sm_context_update_data->target_id = NULL;
    }
    if (sm_context_update_data->target_serving_nf_id) {
        ogs_free(sm_context_update_data->target_serving_nf_id);
        sm_context_update_data->target_serving_nf_id = NULL;
    }
    if (sm_context_update_data->sm_context_status_uri) {
        ogs_free(sm_context_update_data->sm_context_status_uri);
        sm_context_update_data->sm_context_status_uri = NULL;
    }
    if (sm_context_update_data->n9_forwarding_tunnel) {
        OpenAPI_tunnel_info_free(sm_context_update_data->n9_forwarding_tunnel);
        sm_context_update_data->n9_forwarding_tunnel = NULL;
    }
    if (sm_context_update_data->n9_dl_forwarding_tnl_list) {
        OpenAPI_list_for_each(sm_context_update_data->n9_dl_forwarding_tnl_list, node) {
            OpenAPI_indirect_data_forwarding_tunnel_info_free(node->data);
        }
        OpenAPI_list_free(sm_context_update_data->n9_dl_forwarding_tnl_list);
        sm_context_update_data->n9_dl_forwarding_tnl_list = NULL;
    }
    if (sm_context_update_data->n9_ul_forwarding_tnl_list) {
        OpenAPI_list_for_each(sm_context_update_data->n9_ul_forwarding_tnl_list, node) {
            OpenAPI_indirect_data_forwarding_tunnel_info_free(node->data);
        }
        OpenAPI_list_free(sm_context_update_data->n9_ul_forwarding_tnl_list);
        sm_context_update_data->n9_ul_forwarding_tnl_list = NULL;
    }
    if (sm_context_update_data->n9_dl_forwarding_tunnel) {
        OpenAPI_tunnel_info_free(sm_context_update_data->n9_dl_forwarding_tunnel);
        sm_context_update_data->n9_dl_forwarding_tunnel = NULL;
    }
    if (sm_context_update_data->eps_bearer_setup) {
        OpenAPI_list_for_each(sm_context_update_data->eps_bearer_setup, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_update_data->eps_bearer_setup);
        sm_context_update_data->eps_bearer_setup = NULL;
    }
    if (sm_context_update_data->revoke_ebi_list) {
        OpenAPI_list_for_each(sm_context_update_data->revoke_ebi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_update_data->revoke_ebi_list);
        sm_context_update_data->revoke_ebi_list = NULL;
    }
    if (sm_context_update_data->ng_ap_cause) {
        OpenAPI_ng_ap_cause_free(sm_context_update_data->ng_ap_cause);
        sm_context_update_data->ng_ap_cause = NULL;
    }
    if (sm_context_update_data->s_nssai) {
        OpenAPI_snssai_free(sm_context_update_data->s_nssai);
        sm_context_update_data->s_nssai = NULL;
    }
    if (sm_context_update_data->trace_data) {
        OpenAPI_trace_data_free(sm_context_update_data->trace_data);
        sm_context_update_data->trace_data = NULL;
    }
    if (sm_context_update_data->n2_sm_info_ext1) {
        OpenAPI_ref_to_binary_data_free(sm_context_update_data->n2_sm_info_ext1);
        sm_context_update_data->n2_sm_info_ext1 = NULL;
    }
    if (sm_context_update_data->exemption_ind) {
        OpenAPI_exemption_ind_free(sm_context_update_data->exemption_ind);
        sm_context_update_data->exemption_ind = NULL;
    }
    if (sm_context_update_data->supported_features) {
        ogs_free(sm_context_update_data->supported_features);
        sm_context_update_data->supported_features = NULL;
    }
    if (sm_context_update_data->mo_exp_data_counter) {
        OpenAPI_mo_exp_data_counter_free(sm_context_update_data->mo_exp_data_counter);
        sm_context_update_data->mo_exp_data_counter = NULL;
    }
    if (sm_context_update_data->forwarding_f_teid) {
        ogs_free(sm_context_update_data->forwarding_f_teid);
        sm_context_update_data->forwarding_f_teid = NULL;
    }
    if (sm_context_update_data->forwarding_bearer_contexts) {
        OpenAPI_list_for_each(sm_context_update_data->forwarding_bearer_contexts, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_update_data->forwarding_bearer_contexts);
        sm_context_update_data->forwarding_bearer_contexts = NULL;
    }
    if (sm_context_update_data->ddn_failure_subs) {
        OpenAPI_ddn_failure_subs_free(sm_context_update_data->ddn_failure_subs);
        sm_context_update_data->ddn_failure_subs = NULL;
    }
    if (sm_context_update_data->secondary_rat_usage_data_report_container) {
        OpenAPI_list_for_each(sm_context_update_data->secondary_rat_usage_data_report_container, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_context_update_data->secondary_rat_usage_data_report_container);
        sm_context_update_data->secondary_rat_usage_data_report_container = NULL;
    }
    if (sm_context_update_data->pcf_ue_callback_info) {
        OpenAPI_pcf_ue_callback_info_free(sm_context_update_data->pcf_ue_callback_info);
        sm_context_update_data->pcf_ue_callback_info = NULL;
    }
    ogs_free(sm_context_update_data);
}

cJSON *OpenAPI_sm_context_update_data_convertToJSON(OpenAPI_sm_context_update_data_t *sm_context_update_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_list_for_each(sm_context_update_data->backup_amf_info, node) {
        cJSON *itemLocal = OpenAPI_backup_amf_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [backup_amf_info]");
            goto end;
        }
        cJSON_AddItemToArray(backup_amf_infoList, itemLocal);
    }
    } else if (sm_context_update_data->is_backup_amf_info_null) {
        if (cJSON_AddNullToObject(item, "backupAmfInfo") == NULL) {
            ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [backup_amf_info]");
            goto end;
        }
    }

    if (sm_context_update_data->an_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "anType", OpenAPI_access_type_ToString(sm_context_update_data->an_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [an_type]");
        goto end;
    }
    }

    if (sm_context_update_data->additional_an_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "additionalAnType", OpenAPI_access_type_ToString(sm_context_update_data->additional_an_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [additional_an_type]");
        goto end;
    }
    }

    if (sm_context_update_data->an_type_to_reactivate != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "anTypeToReactivate", OpenAPI_access_type_ToString(sm_context_update_data->an_type_to_reactivate)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [an_type_to_reactivate]");
        goto end;
    }
    }

    if (sm_context_update_data->rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(sm_context_update_data->rat_type)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (sm_context_update_data->presence_in_ladn != OpenAPI_presence_state_NULL) {
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

    if (sm_context_update_data->up_cnx_state != OpenAPI_up_cnx_state_NULL) {
    if (cJSON_AddStringToObject(item, "upCnxState", OpenAPI_up_cnx_state_ToString(sm_context_update_data->up_cnx_state)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [up_cnx_state]");
        goto end;
    }
    }

    if (sm_context_update_data->ho_state != OpenAPI_ho_state_NULL) {
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

    if (sm_context_update_data->n2_sm_info_type != OpenAPI_n2_sm_info_type_NULL) {
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
    OpenAPI_list_for_each(sm_context_update_data->n9_dl_forwarding_tnl_list, node) {
        cJSON *itemLocal = OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n9_dl_forwarding_tnl_list]");
            goto end;
        }
        cJSON_AddItemToArray(n9_dl_forwarding_tnl_listList, itemLocal);
    }
    }

    if (sm_context_update_data->n9_ul_forwarding_tnl_list) {
    cJSON *n9_ul_forwarding_tnl_listList = cJSON_AddArrayToObject(item, "n9UlForwardingTnlList");
    if (n9_ul_forwarding_tnl_listList == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n9_ul_forwarding_tnl_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_update_data->n9_ul_forwarding_tnl_list, node) {
        cJSON *itemLocal = OpenAPI_indirect_data_forwarding_tunnel_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n9_ul_forwarding_tnl_list]");
            goto end;
        }
        cJSON_AddItemToArray(n9_ul_forwarding_tnl_listList, itemLocal);
    }
    }

    if (sm_context_update_data->n9_dl_forwarding_tunnel) {
    cJSON *n9_dl_forwarding_tunnel_local_JSON = OpenAPI_tunnel_info_convertToJSON(sm_context_update_data->n9_dl_forwarding_tunnel);
    if (n9_dl_forwarding_tunnel_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n9_dl_forwarding_tunnel]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n9DlForwardingTunnel", n9_dl_forwarding_tunnel_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n9_dl_forwarding_tunnel]");
        goto end;
    }
    }

    if (sm_context_update_data->is_n9_inactivity_timer) {
    if (cJSON_AddNumberToObject(item, "n9InactivityTimer", sm_context_update_data->n9_inactivity_timer) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n9_inactivity_timer]");
        goto end;
    }
    }

    if (sm_context_update_data->eps_bearer_setup) {
    cJSON *eps_bearer_setupList = cJSON_AddArrayToObject(item, "epsBearerSetup");
    if (eps_bearer_setupList == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [eps_bearer_setup]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_update_data->eps_bearer_setup, node) {
        if (cJSON_AddStringToObject(eps_bearer_setupList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [eps_bearer_setup]");
            goto end;
        }
    }
    }

    if (sm_context_update_data->revoke_ebi_list) {
    cJSON *revoke_ebi_listList = cJSON_AddArrayToObject(item, "revokeEbiList");
    if (revoke_ebi_listList == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [revoke_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_update_data->revoke_ebi_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [revoke_ebi_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(revoke_ebi_listList, "", *(double *)node->data) == NULL) {
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

    if (sm_context_update_data->cause != OpenAPI_cause_NULL) {
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
    } else if (sm_context_update_data->is_trace_data_null) {
        if (cJSON_AddNullToObject(item, "traceData") == NULL) {
            ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

    if (sm_context_update_data->eps_interworking_ind != OpenAPI_eps_interworking_indication_NULL) {
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

    if (sm_context_update_data->n2_sm_info_type_ext1 != OpenAPI_n2_sm_info_type_NULL) {
    if (cJSON_AddStringToObject(item, "n2SmInfoTypeExt1", OpenAPI_n2_sm_info_type_ToString(sm_context_update_data->n2_sm_info_type_ext1)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [n2_sm_info_type_ext1]");
        goto end;
    }
    }

    if (sm_context_update_data->ma_release_ind != OpenAPI_ma_release_indication_NULL) {
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
    if (cJSON_AddStringToObject(item, "forwardingFTeid", sm_context_update_data->forwarding_f_teid) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [forwarding_f_teid]");
        goto end;
    }
    }

    if (sm_context_update_data->forwarding_bearer_contexts) {
    cJSON *forwarding_bearer_contextsList = cJSON_AddArrayToObject(item, "forwardingBearerContexts");
    if (forwarding_bearer_contextsList == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [forwarding_bearer_contexts]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_update_data->forwarding_bearer_contexts, node) {
        if (cJSON_AddStringToObject(forwarding_bearer_contextsList, "", (char*)node->data) == NULL) {
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

    if (sm_context_update_data->is_skip_n2_pdu_session_res_rel_ind) {
    if (cJSON_AddBoolToObject(item, "skipN2PduSessionResRelInd", sm_context_update_data->skip_n2_pdu_session_res_rel_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [skip_n2_pdu_session_res_rel_ind]");
        goto end;
    }
    }

    if (sm_context_update_data->secondary_rat_usage_data_report_container) {
    cJSON *secondary_rat_usage_data_report_containerList = cJSON_AddArrayToObject(item, "secondaryRatUsageDataReportContainer");
    if (secondary_rat_usage_data_report_containerList == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [secondary_rat_usage_data_report_container]");
        goto end;
    }
    OpenAPI_list_for_each(sm_context_update_data->secondary_rat_usage_data_report_container, node) {
        if (cJSON_AddStringToObject(secondary_rat_usage_data_report_containerList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [secondary_rat_usage_data_report_container]");
            goto end;
        }
    }
    }

    if (sm_context_update_data->sm_policy_notify_ind != OpenAPI_sm_context_update_data_SMPOLICYNOTIFYIND_NULL) {
    if (cJSON_AddStringToObject(item, "smPolicyNotifyInd", OpenAPI_sm_policy_notify_indsm_context_update_data_ToString(sm_context_update_data->sm_policy_notify_ind)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [sm_policy_notify_ind]");
        goto end;
    }
    }

    if (sm_context_update_data->pcf_ue_callback_info) {
    cJSON *pcf_ue_callback_info_local_JSON = OpenAPI_pcf_ue_callback_info_convertToJSON(sm_context_update_data->pcf_ue_callback_info);
    if (pcf_ue_callback_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [pcf_ue_callback_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pcfUeCallbackInfo", pcf_ue_callback_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [pcf_ue_callback_info]");
        goto end;
    }
    } else if (sm_context_update_data->is_pcf_ue_callback_info_null) {
        if (cJSON_AddNullToObject(item, "pcfUeCallbackInfo") == NULL) {
            ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [pcf_ue_callback_info]");
            goto end;
        }
    }

    if (sm_context_update_data->satellite_backhaul_cat != OpenAPI_satellite_backhaul_category_NULL) {
    if (cJSON_AddStringToObject(item, "satelliteBackhaulCat", OpenAPI_satellite_backhaul_category_ToString(sm_context_update_data->satellite_backhaul_cat)) == NULL) {
        ogs_error("OpenAPI_sm_context_update_data_convertToJSON() failed [satellite_backhaul_cat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_context_update_data_t *OpenAPI_sm_context_update_data_parseFromJSON(cJSON *sm_context_update_dataJSON)
{
    OpenAPI_sm_context_update_data_t *sm_context_update_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pei = NULL;
    cJSON *serving_nf_id = NULL;
    cJSON *guami = NULL;
    OpenAPI_guami_t *guami_local_nonprim = NULL;
    cJSON *serving_network = NULL;
    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    cJSON *backup_amf_info = NULL;
    OpenAPI_list_t *backup_amf_infoList = NULL;
    cJSON *an_type = NULL;
    OpenAPI_access_type_e an_typeVariable = 0;
    cJSON *additional_an_type = NULL;
    OpenAPI_access_type_e additional_an_typeVariable = 0;
    cJSON *an_type_to_reactivate = NULL;
    OpenAPI_access_type_e an_type_to_reactivateVariable = 0;
    cJSON *rat_type = NULL;
    OpenAPI_rat_type_e rat_typeVariable = 0;
    cJSON *presence_in_ladn = NULL;
    OpenAPI_presence_state_e presence_in_ladnVariable = 0;
    cJSON *ue_location = NULL;
    OpenAPI_user_location_t *ue_location_local_nonprim = NULL;
    cJSON *ue_time_zone = NULL;
    cJSON *add_ue_location = NULL;
    OpenAPI_user_location_t *add_ue_location_local_nonprim = NULL;
    cJSON *up_cnx_state = NULL;
    OpenAPI_up_cnx_state_e up_cnx_stateVariable = 0;
    cJSON *ho_state = NULL;
    OpenAPI_ho_state_e ho_stateVariable = 0;
    cJSON *to_be_switched = NULL;
    cJSON *failed_to_be_switched = NULL;
    cJSON *n1_sm_msg = NULL;
    OpenAPI_ref_to_binary_data_t *n1_sm_msg_local_nonprim = NULL;
    cJSON *n2_sm_info = NULL;
    OpenAPI_ref_to_binary_data_t *n2_sm_info_local_nonprim = NULL;
    cJSON *n2_sm_info_type = NULL;
    OpenAPI_n2_sm_info_type_e n2_sm_info_typeVariable = 0;
    cJSON *target_id = NULL;
    OpenAPI_ng_ran_target_id_t *target_id_local_nonprim = NULL;
    cJSON *target_serving_nf_id = NULL;
    cJSON *sm_context_status_uri = NULL;
    cJSON *data_forwarding = NULL;
    cJSON *n9_forwarding_tunnel = NULL;
    OpenAPI_tunnel_info_t *n9_forwarding_tunnel_local_nonprim = NULL;
    cJSON *n9_dl_forwarding_tnl_list = NULL;
    OpenAPI_list_t *n9_dl_forwarding_tnl_listList = NULL;
    cJSON *n9_ul_forwarding_tnl_list = NULL;
    OpenAPI_list_t *n9_ul_forwarding_tnl_listList = NULL;
    cJSON *n9_dl_forwarding_tunnel = NULL;
    OpenAPI_tunnel_info_t *n9_dl_forwarding_tunnel_local_nonprim = NULL;
    cJSON *n9_inactivity_timer = NULL;
    cJSON *eps_bearer_setup = NULL;
    OpenAPI_list_t *eps_bearer_setupList = NULL;
    cJSON *revoke_ebi_list = NULL;
    OpenAPI_list_t *revoke_ebi_listList = NULL;
    cJSON *release = NULL;
    cJSON *cause = NULL;
    OpenAPI_cause_e causeVariable = 0;
    cJSON *ng_ap_cause = NULL;
    OpenAPI_ng_ap_cause_t *ng_ap_cause_local_nonprim = NULL;
    cJSON *_5g_mm_cause_value = NULL;
    cJSON *s_nssai = NULL;
    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    cJSON *trace_data = NULL;
    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    cJSON *eps_interworking_ind = NULL;
    OpenAPI_eps_interworking_indication_e eps_interworking_indVariable = 0;
    cJSON *an_type_can_be_changed = NULL;
    cJSON *n2_sm_info_ext1 = NULL;
    OpenAPI_ref_to_binary_data_t *n2_sm_info_ext1_local_nonprim = NULL;
    cJSON *n2_sm_info_type_ext1 = NULL;
    OpenAPI_n2_sm_info_type_e n2_sm_info_type_ext1Variable = 0;
    cJSON *ma_release_ind = NULL;
    OpenAPI_ma_release_indication_e ma_release_indVariable = 0;
    cJSON *ma_nw_upgrade_ind = NULL;
    cJSON *ma_request_ind = NULL;
    cJSON *exemption_ind = NULL;
    OpenAPI_exemption_ind_t *exemption_ind_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    cJSON *mo_exp_data_counter = NULL;
    OpenAPI_mo_exp_data_counter_t *mo_exp_data_counter_local_nonprim = NULL;
    cJSON *extended_nas_sm_timer_ind = NULL;
    cJSON *forwarding_f_teid = NULL;
    cJSON *forwarding_bearer_contexts = NULL;
    OpenAPI_list_t *forwarding_bearer_contextsList = NULL;
    cJSON *ddn_failure_subs = NULL;
    OpenAPI_ddn_failure_subs_t *ddn_failure_subs_local_nonprim = NULL;
    cJSON *skip_n2_pdu_session_res_rel_ind = NULL;
    cJSON *secondary_rat_usage_data_report_container = NULL;
    OpenAPI_list_t *secondary_rat_usage_data_report_containerList = NULL;
    cJSON *sm_policy_notify_ind = NULL;
    OpenAPI_sm_context_update_data_sm_policy_notify_ind_e sm_policy_notify_indVariable = 0;
    cJSON *pcf_ue_callback_info = NULL;
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_callback_info_local_nonprim = NULL;
    cJSON *satellite_backhaul_cat = NULL;
    OpenAPI_satellite_backhaul_category_e satellite_backhaul_catVariable = 0;
    pei = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    serving_nf_id = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "servingNfId");
    if (serving_nf_id) {
    if (!cJSON_IsString(serving_nf_id) && !cJSON_IsNull(serving_nf_id)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [serving_nf_id]");
        goto end;
    }
    }

    guami = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "guami");
    if (guami) {
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    if (!guami_local_nonprim) {
        ogs_error("OpenAPI_guami_parseFromJSON failed [guami]");
        goto end;
    }
    }

    serving_network = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "servingNetwork");
    if (serving_network) {
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    if (!serving_network_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [serving_network]");
        goto end;
    }
    }

    backup_amf_info = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "backupAmfInfo");
    if (backup_amf_info) {
    if (!cJSON_IsNull(backup_amf_info)) {
        cJSON *backup_amf_info_local = NULL;
        if (!cJSON_IsArray(backup_amf_info)) {
            ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [backup_amf_info]");
            goto end;
        }

        backup_amf_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(backup_amf_info_local, backup_amf_info) {
            if (!cJSON_IsObject(backup_amf_info_local)) {
                ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [backup_amf_info]");
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
    }

    an_type = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "anType");
    if (an_type) {
    if (!cJSON_IsString(an_type)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [an_type]");
        goto end;
    }
    an_typeVariable = OpenAPI_access_type_FromString(an_type->valuestring);
    }

    additional_an_type = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "additionalAnType");
    if (additional_an_type) {
    if (!cJSON_IsString(additional_an_type)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [additional_an_type]");
        goto end;
    }
    additional_an_typeVariable = OpenAPI_access_type_FromString(additional_an_type->valuestring);
    }

    an_type_to_reactivate = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "anTypeToReactivate");
    if (an_type_to_reactivate) {
    if (!cJSON_IsString(an_type_to_reactivate)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [an_type_to_reactivate]");
        goto end;
    }
    an_type_to_reactivateVariable = OpenAPI_access_type_FromString(an_type_to_reactivate->valuestring);
    }

    rat_type = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "ratType");
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    presence_in_ladn = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "presenceInLadn");
    if (presence_in_ladn) {
    if (!cJSON_IsString(presence_in_ladn)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [presence_in_ladn]");
        goto end;
    }
    presence_in_ladnVariable = OpenAPI_presence_state_FromString(presence_in_ladn->valuestring);
    }

    ue_location = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "ueLocation");
    if (ue_location) {
    ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    if (!ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_location]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    add_ue_location = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "addUeLocation");
    if (add_ue_location) {
    add_ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(add_ue_location);
    if (!add_ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [add_ue_location]");
        goto end;
    }
    }

    up_cnx_state = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "upCnxState");
    if (up_cnx_state) {
    if (!cJSON_IsString(up_cnx_state)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [up_cnx_state]");
        goto end;
    }
    up_cnx_stateVariable = OpenAPI_up_cnx_state_FromString(up_cnx_state->valuestring);
    }

    ho_state = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "hoState");
    if (ho_state) {
    if (!cJSON_IsString(ho_state)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [ho_state]");
        goto end;
    }
    ho_stateVariable = OpenAPI_ho_state_FromString(ho_state->valuestring);
    }

    to_be_switched = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "toBeSwitched");
    if (to_be_switched) {
    if (!cJSON_IsBool(to_be_switched)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [to_be_switched]");
        goto end;
    }
    }

    failed_to_be_switched = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "failedToBeSwitched");
    if (failed_to_be_switched) {
    if (!cJSON_IsBool(failed_to_be_switched)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [failed_to_be_switched]");
        goto end;
    }
    }

    n1_sm_msg = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n1SmMsg");
    if (n1_sm_msg) {
    n1_sm_msg_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_msg);
    if (!n1_sm_msg_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n1_sm_msg]");
        goto end;
    }
    }

    n2_sm_info = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n2SmInfo");
    if (n2_sm_info) {
    n2_sm_info_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n2_sm_info);
    if (!n2_sm_info_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n2_sm_info]");
        goto end;
    }
    }

    n2_sm_info_type = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n2SmInfoType");
    if (n2_sm_info_type) {
    if (!cJSON_IsString(n2_sm_info_type)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [n2_sm_info_type]");
        goto end;
    }
    n2_sm_info_typeVariable = OpenAPI_n2_sm_info_type_FromString(n2_sm_info_type->valuestring);
    }

    target_id = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "targetId");
    if (target_id) {
    target_id_local_nonprim = OpenAPI_ng_ran_target_id_parseFromJSON(target_id);
    if (!target_id_local_nonprim) {
        ogs_error("OpenAPI_ng_ran_target_id_parseFromJSON failed [target_id]");
        goto end;
    }
    }

    target_serving_nf_id = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "targetServingNfId");
    if (target_serving_nf_id) {
    if (!cJSON_IsString(target_serving_nf_id) && !cJSON_IsNull(target_serving_nf_id)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [target_serving_nf_id]");
        goto end;
    }
    }

    sm_context_status_uri = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "smContextStatusUri");
    if (sm_context_status_uri) {
    if (!cJSON_IsString(sm_context_status_uri) && !cJSON_IsNull(sm_context_status_uri)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [sm_context_status_uri]");
        goto end;
    }
    }

    data_forwarding = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "dataForwarding");
    if (data_forwarding) {
    if (!cJSON_IsBool(data_forwarding)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [data_forwarding]");
        goto end;
    }
    }

    n9_forwarding_tunnel = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n9ForwardingTunnel");
    if (n9_forwarding_tunnel) {
    n9_forwarding_tunnel_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(n9_forwarding_tunnel);
    if (!n9_forwarding_tunnel_local_nonprim) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON failed [n9_forwarding_tunnel]");
        goto end;
    }
    }

    n9_dl_forwarding_tnl_list = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n9DlForwardingTnlList");
    if (n9_dl_forwarding_tnl_list) {
        cJSON *n9_dl_forwarding_tnl_list_local = NULL;
        if (!cJSON_IsArray(n9_dl_forwarding_tnl_list)) {
            ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [n9_dl_forwarding_tnl_list]");
            goto end;
        }

        n9_dl_forwarding_tnl_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(n9_dl_forwarding_tnl_list_local, n9_dl_forwarding_tnl_list) {
            if (!cJSON_IsObject(n9_dl_forwarding_tnl_list_local)) {
                ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [n9_dl_forwarding_tnl_list]");
                goto end;
            }
            OpenAPI_indirect_data_forwarding_tunnel_info_t *n9_dl_forwarding_tnl_listItem = OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON(n9_dl_forwarding_tnl_list_local);
            if (!n9_dl_forwarding_tnl_listItem) {
                ogs_error("No n9_dl_forwarding_tnl_listItem");
                goto end;
            }
            OpenAPI_list_add(n9_dl_forwarding_tnl_listList, n9_dl_forwarding_tnl_listItem);
        }
    }

    n9_ul_forwarding_tnl_list = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n9UlForwardingTnlList");
    if (n9_ul_forwarding_tnl_list) {
        cJSON *n9_ul_forwarding_tnl_list_local = NULL;
        if (!cJSON_IsArray(n9_ul_forwarding_tnl_list)) {
            ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [n9_ul_forwarding_tnl_list]");
            goto end;
        }

        n9_ul_forwarding_tnl_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(n9_ul_forwarding_tnl_list_local, n9_ul_forwarding_tnl_list) {
            if (!cJSON_IsObject(n9_ul_forwarding_tnl_list_local)) {
                ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [n9_ul_forwarding_tnl_list]");
                goto end;
            }
            OpenAPI_indirect_data_forwarding_tunnel_info_t *n9_ul_forwarding_tnl_listItem = OpenAPI_indirect_data_forwarding_tunnel_info_parseFromJSON(n9_ul_forwarding_tnl_list_local);
            if (!n9_ul_forwarding_tnl_listItem) {
                ogs_error("No n9_ul_forwarding_tnl_listItem");
                goto end;
            }
            OpenAPI_list_add(n9_ul_forwarding_tnl_listList, n9_ul_forwarding_tnl_listItem);
        }
    }

    n9_dl_forwarding_tunnel = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n9DlForwardingTunnel");
    if (n9_dl_forwarding_tunnel) {
    n9_dl_forwarding_tunnel_local_nonprim = OpenAPI_tunnel_info_parseFromJSON(n9_dl_forwarding_tunnel);
    if (!n9_dl_forwarding_tunnel_local_nonprim) {
        ogs_error("OpenAPI_tunnel_info_parseFromJSON failed [n9_dl_forwarding_tunnel]");
        goto end;
    }
    }

    n9_inactivity_timer = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n9InactivityTimer");
    if (n9_inactivity_timer) {
    if (!cJSON_IsNumber(n9_inactivity_timer)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [n9_inactivity_timer]");
        goto end;
    }
    }

    eps_bearer_setup = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "epsBearerSetup");
    if (eps_bearer_setup) {
        cJSON *eps_bearer_setup_local = NULL;
        if (!cJSON_IsArray(eps_bearer_setup)) {
            ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [eps_bearer_setup]");
            goto end;
        }

        eps_bearer_setupList = OpenAPI_list_create();

        cJSON_ArrayForEach(eps_bearer_setup_local, eps_bearer_setup) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(eps_bearer_setup_local)) {
                ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [eps_bearer_setup]");
                goto end;
            }
            OpenAPI_list_add(eps_bearer_setupList, ogs_strdup(eps_bearer_setup_local->valuestring));
        }
    }

    revoke_ebi_list = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "revokeEbiList");
    if (revoke_ebi_list) {
        cJSON *revoke_ebi_list_local = NULL;
        if (!cJSON_IsArray(revoke_ebi_list)) {
            ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [revoke_ebi_list]");
            goto end;
        }

        revoke_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(revoke_ebi_list_local, revoke_ebi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(revoke_ebi_list_local)) {
                ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [revoke_ebi_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [revoke_ebi_list]");
                goto end;
            }
            *localDouble = revoke_ebi_list_local->valuedouble;
            OpenAPI_list_add(revoke_ebi_listList, localDouble);
        }
    }

    release = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "release");
    if (release) {
    if (!cJSON_IsBool(release)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [release]");
        goto end;
    }
    }

    cause = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [cause]");
        goto end;
    }
    causeVariable = OpenAPI_cause_FromString(cause->valuestring);
    }

    ng_ap_cause = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "ngApCause");
    if (ng_ap_cause) {
    ng_ap_cause_local_nonprim = OpenAPI_ng_ap_cause_parseFromJSON(ng_ap_cause);
    if (!ng_ap_cause_local_nonprim) {
        ogs_error("OpenAPI_ng_ap_cause_parseFromJSON failed [ng_ap_cause]");
        goto end;
    }
    }

    _5g_mm_cause_value = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "5gMmCauseValue");
    if (_5g_mm_cause_value) {
    if (!cJSON_IsNumber(_5g_mm_cause_value)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [_5g_mm_cause_value]");
        goto end;
    }
    }

    s_nssai = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "sNssai");
    if (s_nssai) {
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    if (!s_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [s_nssai]");
        goto end;
    }
    }

    trace_data = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "traceData");
    if (trace_data) {
    if (!cJSON_IsNull(trace_data)) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    if (!trace_data_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_data]");
        goto end;
    }
    }
    }

    eps_interworking_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "epsInterworkingInd");
    if (eps_interworking_ind) {
    if (!cJSON_IsString(eps_interworking_ind)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [eps_interworking_ind]");
        goto end;
    }
    eps_interworking_indVariable = OpenAPI_eps_interworking_indication_FromString(eps_interworking_ind->valuestring);
    }

    an_type_can_be_changed = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "anTypeCanBeChanged");
    if (an_type_can_be_changed) {
    if (!cJSON_IsBool(an_type_can_be_changed)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [an_type_can_be_changed]");
        goto end;
    }
    }

    n2_sm_info_ext1 = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n2SmInfoExt1");
    if (n2_sm_info_ext1) {
    n2_sm_info_ext1_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n2_sm_info_ext1);
    if (!n2_sm_info_ext1_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n2_sm_info_ext1]");
        goto end;
    }
    }

    n2_sm_info_type_ext1 = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "n2SmInfoTypeExt1");
    if (n2_sm_info_type_ext1) {
    if (!cJSON_IsString(n2_sm_info_type_ext1)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [n2_sm_info_type_ext1]");
        goto end;
    }
    n2_sm_info_type_ext1Variable = OpenAPI_n2_sm_info_type_FromString(n2_sm_info_type_ext1->valuestring);
    }

    ma_release_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "maReleaseInd");
    if (ma_release_ind) {
    if (!cJSON_IsString(ma_release_ind)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [ma_release_ind]");
        goto end;
    }
    ma_release_indVariable = OpenAPI_ma_release_indication_FromString(ma_release_ind->valuestring);
    }

    ma_nw_upgrade_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "maNwUpgradeInd");
    if (ma_nw_upgrade_ind) {
    if (!cJSON_IsBool(ma_nw_upgrade_ind)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [ma_nw_upgrade_ind]");
        goto end;
    }
    }

    ma_request_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "maRequestInd");
    if (ma_request_ind) {
    if (!cJSON_IsBool(ma_request_ind)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [ma_request_ind]");
        goto end;
    }
    }

    exemption_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "exemptionInd");
    if (exemption_ind) {
    exemption_ind_local_nonprim = OpenAPI_exemption_ind_parseFromJSON(exemption_ind);
    if (!exemption_ind_local_nonprim) {
        ogs_error("OpenAPI_exemption_ind_parseFromJSON failed [exemption_ind]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    mo_exp_data_counter = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "moExpDataCounter");
    if (mo_exp_data_counter) {
    mo_exp_data_counter_local_nonprim = OpenAPI_mo_exp_data_counter_parseFromJSON(mo_exp_data_counter);
    if (!mo_exp_data_counter_local_nonprim) {
        ogs_error("OpenAPI_mo_exp_data_counter_parseFromJSON failed [mo_exp_data_counter]");
        goto end;
    }
    }

    extended_nas_sm_timer_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "extendedNasSmTimerInd");
    if (extended_nas_sm_timer_ind) {
    if (!cJSON_IsBool(extended_nas_sm_timer_ind)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [extended_nas_sm_timer_ind]");
        goto end;
    }
    }

    forwarding_f_teid = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "forwardingFTeid");
    if (forwarding_f_teid) {
    if (!cJSON_IsString(forwarding_f_teid) && !cJSON_IsNull(forwarding_f_teid)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [forwarding_f_teid]");
        goto end;
    }
    }

    forwarding_bearer_contexts = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "forwardingBearerContexts");
    if (forwarding_bearer_contexts) {
        cJSON *forwarding_bearer_contexts_local = NULL;
        if (!cJSON_IsArray(forwarding_bearer_contexts)) {
            ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [forwarding_bearer_contexts]");
            goto end;
        }

        forwarding_bearer_contextsList = OpenAPI_list_create();

        cJSON_ArrayForEach(forwarding_bearer_contexts_local, forwarding_bearer_contexts) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(forwarding_bearer_contexts_local)) {
                ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [forwarding_bearer_contexts]");
                goto end;
            }
            OpenAPI_list_add(forwarding_bearer_contextsList, ogs_strdup(forwarding_bearer_contexts_local->valuestring));
        }
    }

    ddn_failure_subs = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "ddnFailureSubs");
    if (ddn_failure_subs) {
    ddn_failure_subs_local_nonprim = OpenAPI_ddn_failure_subs_parseFromJSON(ddn_failure_subs);
    if (!ddn_failure_subs_local_nonprim) {
        ogs_error("OpenAPI_ddn_failure_subs_parseFromJSON failed [ddn_failure_subs]");
        goto end;
    }
    }

    skip_n2_pdu_session_res_rel_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "skipN2PduSessionResRelInd");
    if (skip_n2_pdu_session_res_rel_ind) {
    if (!cJSON_IsBool(skip_n2_pdu_session_res_rel_ind)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [skip_n2_pdu_session_res_rel_ind]");
        goto end;
    }
    }

    secondary_rat_usage_data_report_container = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "secondaryRatUsageDataReportContainer");
    if (secondary_rat_usage_data_report_container) {
        cJSON *secondary_rat_usage_data_report_container_local = NULL;
        if (!cJSON_IsArray(secondary_rat_usage_data_report_container)) {
            ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [secondary_rat_usage_data_report_container]");
            goto end;
        }

        secondary_rat_usage_data_report_containerList = OpenAPI_list_create();

        cJSON_ArrayForEach(secondary_rat_usage_data_report_container_local, secondary_rat_usage_data_report_container) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(secondary_rat_usage_data_report_container_local)) {
                ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [secondary_rat_usage_data_report_container]");
                goto end;
            }
            OpenAPI_list_add(secondary_rat_usage_data_report_containerList, ogs_strdup(secondary_rat_usage_data_report_container_local->valuestring));
        }
    }

    sm_policy_notify_ind = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "smPolicyNotifyInd");
    if (sm_policy_notify_ind) {
    if (!cJSON_IsString(sm_policy_notify_ind)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [sm_policy_notify_ind]");
        goto end;
    }
    sm_policy_notify_indVariable = OpenAPI_sm_policy_notify_indsm_context_update_data_FromString(sm_policy_notify_ind->valuestring);
    }

    pcf_ue_callback_info = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "pcfUeCallbackInfo");
    if (pcf_ue_callback_info) {
    if (!cJSON_IsNull(pcf_ue_callback_info)) {
    pcf_ue_callback_info_local_nonprim = OpenAPI_pcf_ue_callback_info_parseFromJSON(pcf_ue_callback_info);
    if (!pcf_ue_callback_info_local_nonprim) {
        ogs_error("OpenAPI_pcf_ue_callback_info_parseFromJSON failed [pcf_ue_callback_info]");
        goto end;
    }
    }
    }

    satellite_backhaul_cat = cJSON_GetObjectItemCaseSensitive(sm_context_update_dataJSON, "satelliteBackhaulCat");
    if (satellite_backhaul_cat) {
    if (!cJSON_IsString(satellite_backhaul_cat)) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON() failed [satellite_backhaul_cat]");
        goto end;
    }
    satellite_backhaul_catVariable = OpenAPI_satellite_backhaul_category_FromString(satellite_backhaul_cat->valuestring);
    }

    sm_context_update_data_local_var = OpenAPI_sm_context_update_data_create (
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        serving_nf_id && !cJSON_IsNull(serving_nf_id) ? ogs_strdup(serving_nf_id->valuestring) : NULL,
        guami ? guami_local_nonprim : NULL,
        serving_network ? serving_network_local_nonprim : NULL,
        backup_amf_info && cJSON_IsNull(backup_amf_info) ? true : false,
        backup_amf_info ? backup_amf_infoList : NULL,
        an_type ? an_typeVariable : 0,
        additional_an_type ? additional_an_typeVariable : 0,
        an_type_to_reactivate ? an_type_to_reactivateVariable : 0,
        rat_type ? rat_typeVariable : 0,
        presence_in_ladn ? presence_in_ladnVariable : 0,
        ue_location ? ue_location_local_nonprim : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL,
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
        target_serving_nf_id && !cJSON_IsNull(target_serving_nf_id) ? ogs_strdup(target_serving_nf_id->valuestring) : NULL,
        sm_context_status_uri && !cJSON_IsNull(sm_context_status_uri) ? ogs_strdup(sm_context_status_uri->valuestring) : NULL,
        data_forwarding ? true : false,
        data_forwarding ? data_forwarding->valueint : 0,
        n9_forwarding_tunnel ? n9_forwarding_tunnel_local_nonprim : NULL,
        n9_dl_forwarding_tnl_list ? n9_dl_forwarding_tnl_listList : NULL,
        n9_ul_forwarding_tnl_list ? n9_ul_forwarding_tnl_listList : NULL,
        n9_dl_forwarding_tunnel ? n9_dl_forwarding_tunnel_local_nonprim : NULL,
        n9_inactivity_timer ? true : false,
        n9_inactivity_timer ? n9_inactivity_timer->valuedouble : 0,
        eps_bearer_setup ? eps_bearer_setupList : NULL,
        revoke_ebi_list ? revoke_ebi_listList : NULL,
        release ? true : false,
        release ? release->valueint : 0,
        cause ? causeVariable : 0,
        ng_ap_cause ? ng_ap_cause_local_nonprim : NULL,
        _5g_mm_cause_value ? true : false,
        _5g_mm_cause_value ? _5g_mm_cause_value->valuedouble : 0,
        s_nssai ? s_nssai_local_nonprim : NULL,
        trace_data && cJSON_IsNull(trace_data) ? true : false,
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
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        mo_exp_data_counter ? mo_exp_data_counter_local_nonprim : NULL,
        extended_nas_sm_timer_ind ? true : false,
        extended_nas_sm_timer_ind ? extended_nas_sm_timer_ind->valueint : 0,
        forwarding_f_teid && !cJSON_IsNull(forwarding_f_teid) ? ogs_strdup(forwarding_f_teid->valuestring) : NULL,
        forwarding_bearer_contexts ? forwarding_bearer_contextsList : NULL,
        ddn_failure_subs ? ddn_failure_subs_local_nonprim : NULL,
        skip_n2_pdu_session_res_rel_ind ? true : false,
        skip_n2_pdu_session_res_rel_ind ? skip_n2_pdu_session_res_rel_ind->valueint : 0,
        secondary_rat_usage_data_report_container ? secondary_rat_usage_data_report_containerList : NULL,
        sm_policy_notify_ind ? sm_policy_notify_indVariable : 0,
        pcf_ue_callback_info && cJSON_IsNull(pcf_ue_callback_info) ? true : false,
        pcf_ue_callback_info ? pcf_ue_callback_info_local_nonprim : NULL,
        satellite_backhaul_cat ? satellite_backhaul_catVariable : 0
    );

    return sm_context_update_data_local_var;
end:
    if (guami_local_nonprim) {
        OpenAPI_guami_free(guami_local_nonprim);
        guami_local_nonprim = NULL;
    }
    if (serving_network_local_nonprim) {
        OpenAPI_plmn_id_nid_free(serving_network_local_nonprim);
        serving_network_local_nonprim = NULL;
    }
    if (backup_amf_infoList) {
        OpenAPI_list_for_each(backup_amf_infoList, node) {
            OpenAPI_backup_amf_info_free(node->data);
        }
        OpenAPI_list_free(backup_amf_infoList);
        backup_amf_infoList = NULL;
    }
    if (ue_location_local_nonprim) {
        OpenAPI_user_location_free(ue_location_local_nonprim);
        ue_location_local_nonprim = NULL;
    }
    if (add_ue_location_local_nonprim) {
        OpenAPI_user_location_free(add_ue_location_local_nonprim);
        add_ue_location_local_nonprim = NULL;
    }
    if (n1_sm_msg_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(n1_sm_msg_local_nonprim);
        n1_sm_msg_local_nonprim = NULL;
    }
    if (n2_sm_info_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(n2_sm_info_local_nonprim);
        n2_sm_info_local_nonprim = NULL;
    }
    if (target_id_local_nonprim) {
        OpenAPI_ng_ran_target_id_free(target_id_local_nonprim);
        target_id_local_nonprim = NULL;
    }
    if (n9_forwarding_tunnel_local_nonprim) {
        OpenAPI_tunnel_info_free(n9_forwarding_tunnel_local_nonprim);
        n9_forwarding_tunnel_local_nonprim = NULL;
    }
    if (n9_dl_forwarding_tnl_listList) {
        OpenAPI_list_for_each(n9_dl_forwarding_tnl_listList, node) {
            OpenAPI_indirect_data_forwarding_tunnel_info_free(node->data);
        }
        OpenAPI_list_free(n9_dl_forwarding_tnl_listList);
        n9_dl_forwarding_tnl_listList = NULL;
    }
    if (n9_ul_forwarding_tnl_listList) {
        OpenAPI_list_for_each(n9_ul_forwarding_tnl_listList, node) {
            OpenAPI_indirect_data_forwarding_tunnel_info_free(node->data);
        }
        OpenAPI_list_free(n9_ul_forwarding_tnl_listList);
        n9_ul_forwarding_tnl_listList = NULL;
    }
    if (n9_dl_forwarding_tunnel_local_nonprim) {
        OpenAPI_tunnel_info_free(n9_dl_forwarding_tunnel_local_nonprim);
        n9_dl_forwarding_tunnel_local_nonprim = NULL;
    }
    if (eps_bearer_setupList) {
        OpenAPI_list_for_each(eps_bearer_setupList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(eps_bearer_setupList);
        eps_bearer_setupList = NULL;
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
    if (s_nssai_local_nonprim) {
        OpenAPI_snssai_free(s_nssai_local_nonprim);
        s_nssai_local_nonprim = NULL;
    }
    if (trace_data_local_nonprim) {
        OpenAPI_trace_data_free(trace_data_local_nonprim);
        trace_data_local_nonprim = NULL;
    }
    if (n2_sm_info_ext1_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(n2_sm_info_ext1_local_nonprim);
        n2_sm_info_ext1_local_nonprim = NULL;
    }
    if (exemption_ind_local_nonprim) {
        OpenAPI_exemption_ind_free(exemption_ind_local_nonprim);
        exemption_ind_local_nonprim = NULL;
    }
    if (mo_exp_data_counter_local_nonprim) {
        OpenAPI_mo_exp_data_counter_free(mo_exp_data_counter_local_nonprim);
        mo_exp_data_counter_local_nonprim = NULL;
    }
    if (forwarding_bearer_contextsList) {
        OpenAPI_list_for_each(forwarding_bearer_contextsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(forwarding_bearer_contextsList);
        forwarding_bearer_contextsList = NULL;
    }
    if (ddn_failure_subs_local_nonprim) {
        OpenAPI_ddn_failure_subs_free(ddn_failure_subs_local_nonprim);
        ddn_failure_subs_local_nonprim = NULL;
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

