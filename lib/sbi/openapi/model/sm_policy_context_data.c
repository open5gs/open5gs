
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_context_data.h"

OpenAPI_sm_policy_context_data_t *OpenAPI_sm_policy_context_data_create(
    OpenAPI_acc_net_ch_id_t *acc_net_ch_id,
    OpenAPI_acc_net_charging_address_t *charg_entity_addr,
    char *gpsi,
    char *supi,
    bool is_invalid_supi,
    int invalid_supi,
    OpenAPI_list_t *inter_grp_ids,
    int pdu_session_id,
    OpenAPI_pdu_session_type_e pdu_session_type,
    char *chargingcharacteristics,
    char *dnn,
    OpenAPI_dnn_selection_mode_e dnn_sel_mode,
    char *notification_uri,
    OpenAPI_access_type_e access_type,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_additional_access_info_t *add_access_info,
    OpenAPI_plmn_id_nid_t *serving_network,
    OpenAPI_user_location_t *user_location_info,
    char *ue_time_zone,
    char *pei,
    char *ipv4_address,
    char *ipv6_address_prefix,
    char *ip_domain,
    OpenAPI_ambr_t *subs_sess_ambr,
    char *auth_prof_index,
    OpenAPI_subscribed_default_qos_t *subs_def_qos,
    OpenAPI_vplmn_qos_t *vplmn_qos,
    bool is_num_of_pack_filter,
    int num_of_pack_filter,
    bool is_online,
    int online,
    bool is_offline,
    int offline,
    bool is__3gpp_ps_data_off_status,
    int _3gpp_ps_data_off_status,
    bool is_ref_qos_indication,
    int ref_qos_indication,
    bool is_trace_req_null,
    OpenAPI_trace_data_t *trace_req,
    OpenAPI_snssai_t *slice_info,
    OpenAPI_qos_flow_usage_e qos_flow_usage,
    OpenAPI_serving_nf_identity_t *serv_nf_id,
    char *supp_feat,
    char *smf_id,
    char *recovery_time,
    OpenAPI_ma_pdu_indication_e ma_pdu_ind,
    OpenAPI_npcf_atsss_capability_e atsss_capab,
    OpenAPI_list_t *ipv4_frame_route_list,
    OpenAPI_list_t *ipv6_frame_route_list,
    OpenAPI_satellite_backhaul_category_e sat_backhaul_category,
    bool is_pcf_ue_info_null,
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_info,
    OpenAPI_list_t *pvs_info,
    bool is_onboard_ind,
    int onboard_ind,
    OpenAPI_list_t *nwdaf_datas
)
{
    OpenAPI_sm_policy_context_data_t *sm_policy_context_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_policy_context_data_t));
    ogs_assert(sm_policy_context_data_local_var);

    sm_policy_context_data_local_var->acc_net_ch_id = acc_net_ch_id;
    sm_policy_context_data_local_var->charg_entity_addr = charg_entity_addr;
    sm_policy_context_data_local_var->gpsi = gpsi;
    sm_policy_context_data_local_var->supi = supi;
    sm_policy_context_data_local_var->is_invalid_supi = is_invalid_supi;
    sm_policy_context_data_local_var->invalid_supi = invalid_supi;
    sm_policy_context_data_local_var->inter_grp_ids = inter_grp_ids;
    sm_policy_context_data_local_var->pdu_session_id = pdu_session_id;
    sm_policy_context_data_local_var->pdu_session_type = pdu_session_type;
    sm_policy_context_data_local_var->chargingcharacteristics = chargingcharacteristics;
    sm_policy_context_data_local_var->dnn = dnn;
    sm_policy_context_data_local_var->dnn_sel_mode = dnn_sel_mode;
    sm_policy_context_data_local_var->notification_uri = notification_uri;
    sm_policy_context_data_local_var->access_type = access_type;
    sm_policy_context_data_local_var->rat_type = rat_type;
    sm_policy_context_data_local_var->add_access_info = add_access_info;
    sm_policy_context_data_local_var->serving_network = serving_network;
    sm_policy_context_data_local_var->user_location_info = user_location_info;
    sm_policy_context_data_local_var->ue_time_zone = ue_time_zone;
    sm_policy_context_data_local_var->pei = pei;
    sm_policy_context_data_local_var->ipv4_address = ipv4_address;
    sm_policy_context_data_local_var->ipv6_address_prefix = ipv6_address_prefix;
    sm_policy_context_data_local_var->ip_domain = ip_domain;
    sm_policy_context_data_local_var->subs_sess_ambr = subs_sess_ambr;
    sm_policy_context_data_local_var->auth_prof_index = auth_prof_index;
    sm_policy_context_data_local_var->subs_def_qos = subs_def_qos;
    sm_policy_context_data_local_var->vplmn_qos = vplmn_qos;
    sm_policy_context_data_local_var->is_num_of_pack_filter = is_num_of_pack_filter;
    sm_policy_context_data_local_var->num_of_pack_filter = num_of_pack_filter;
    sm_policy_context_data_local_var->is_online = is_online;
    sm_policy_context_data_local_var->online = online;
    sm_policy_context_data_local_var->is_offline = is_offline;
    sm_policy_context_data_local_var->offline = offline;
    sm_policy_context_data_local_var->is__3gpp_ps_data_off_status = is__3gpp_ps_data_off_status;
    sm_policy_context_data_local_var->_3gpp_ps_data_off_status = _3gpp_ps_data_off_status;
    sm_policy_context_data_local_var->is_ref_qos_indication = is_ref_qos_indication;
    sm_policy_context_data_local_var->ref_qos_indication = ref_qos_indication;
    sm_policy_context_data_local_var->is_trace_req_null = is_trace_req_null;
    sm_policy_context_data_local_var->trace_req = trace_req;
    sm_policy_context_data_local_var->slice_info = slice_info;
    sm_policy_context_data_local_var->qos_flow_usage = qos_flow_usage;
    sm_policy_context_data_local_var->serv_nf_id = serv_nf_id;
    sm_policy_context_data_local_var->supp_feat = supp_feat;
    sm_policy_context_data_local_var->smf_id = smf_id;
    sm_policy_context_data_local_var->recovery_time = recovery_time;
    sm_policy_context_data_local_var->ma_pdu_ind = ma_pdu_ind;
    sm_policy_context_data_local_var->atsss_capab = atsss_capab;
    sm_policy_context_data_local_var->ipv4_frame_route_list = ipv4_frame_route_list;
    sm_policy_context_data_local_var->ipv6_frame_route_list = ipv6_frame_route_list;
    sm_policy_context_data_local_var->sat_backhaul_category = sat_backhaul_category;
    sm_policy_context_data_local_var->is_pcf_ue_info_null = is_pcf_ue_info_null;
    sm_policy_context_data_local_var->pcf_ue_info = pcf_ue_info;
    sm_policy_context_data_local_var->pvs_info = pvs_info;
    sm_policy_context_data_local_var->is_onboard_ind = is_onboard_ind;
    sm_policy_context_data_local_var->onboard_ind = onboard_ind;
    sm_policy_context_data_local_var->nwdaf_datas = nwdaf_datas;

    return sm_policy_context_data_local_var;
}

void OpenAPI_sm_policy_context_data_free(OpenAPI_sm_policy_context_data_t *sm_policy_context_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_policy_context_data) {
        return;
    }
    if (sm_policy_context_data->acc_net_ch_id) {
        OpenAPI_acc_net_ch_id_free(sm_policy_context_data->acc_net_ch_id);
        sm_policy_context_data->acc_net_ch_id = NULL;
    }
    if (sm_policy_context_data->charg_entity_addr) {
        OpenAPI_acc_net_charging_address_free(sm_policy_context_data->charg_entity_addr);
        sm_policy_context_data->charg_entity_addr = NULL;
    }
    if (sm_policy_context_data->gpsi) {
        ogs_free(sm_policy_context_data->gpsi);
        sm_policy_context_data->gpsi = NULL;
    }
    if (sm_policy_context_data->supi) {
        ogs_free(sm_policy_context_data->supi);
        sm_policy_context_data->supi = NULL;
    }
    if (sm_policy_context_data->inter_grp_ids) {
        OpenAPI_list_for_each(sm_policy_context_data->inter_grp_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_policy_context_data->inter_grp_ids);
        sm_policy_context_data->inter_grp_ids = NULL;
    }
    if (sm_policy_context_data->chargingcharacteristics) {
        ogs_free(sm_policy_context_data->chargingcharacteristics);
        sm_policy_context_data->chargingcharacteristics = NULL;
    }
    if (sm_policy_context_data->dnn) {
        ogs_free(sm_policy_context_data->dnn);
        sm_policy_context_data->dnn = NULL;
    }
    if (sm_policy_context_data->notification_uri) {
        ogs_free(sm_policy_context_data->notification_uri);
        sm_policy_context_data->notification_uri = NULL;
    }
    if (sm_policy_context_data->add_access_info) {
        OpenAPI_additional_access_info_free(sm_policy_context_data->add_access_info);
        sm_policy_context_data->add_access_info = NULL;
    }
    if (sm_policy_context_data->serving_network) {
        OpenAPI_plmn_id_nid_free(sm_policy_context_data->serving_network);
        sm_policy_context_data->serving_network = NULL;
    }
    if (sm_policy_context_data->user_location_info) {
        OpenAPI_user_location_free(sm_policy_context_data->user_location_info);
        sm_policy_context_data->user_location_info = NULL;
    }
    if (sm_policy_context_data->ue_time_zone) {
        ogs_free(sm_policy_context_data->ue_time_zone);
        sm_policy_context_data->ue_time_zone = NULL;
    }
    if (sm_policy_context_data->pei) {
        ogs_free(sm_policy_context_data->pei);
        sm_policy_context_data->pei = NULL;
    }
    if (sm_policy_context_data->ipv4_address) {
        ogs_free(sm_policy_context_data->ipv4_address);
        sm_policy_context_data->ipv4_address = NULL;
    }
    if (sm_policy_context_data->ipv6_address_prefix) {
        ogs_free(sm_policy_context_data->ipv6_address_prefix);
        sm_policy_context_data->ipv6_address_prefix = NULL;
    }
    if (sm_policy_context_data->ip_domain) {
        ogs_free(sm_policy_context_data->ip_domain);
        sm_policy_context_data->ip_domain = NULL;
    }
    if (sm_policy_context_data->subs_sess_ambr) {
        OpenAPI_ambr_free(sm_policy_context_data->subs_sess_ambr);
        sm_policy_context_data->subs_sess_ambr = NULL;
    }
    if (sm_policy_context_data->auth_prof_index) {
        ogs_free(sm_policy_context_data->auth_prof_index);
        sm_policy_context_data->auth_prof_index = NULL;
    }
    if (sm_policy_context_data->subs_def_qos) {
        OpenAPI_subscribed_default_qos_free(sm_policy_context_data->subs_def_qos);
        sm_policy_context_data->subs_def_qos = NULL;
    }
    if (sm_policy_context_data->vplmn_qos) {
        OpenAPI_vplmn_qos_free(sm_policy_context_data->vplmn_qos);
        sm_policy_context_data->vplmn_qos = NULL;
    }
    if (sm_policy_context_data->trace_req) {
        OpenAPI_trace_data_free(sm_policy_context_data->trace_req);
        sm_policy_context_data->trace_req = NULL;
    }
    if (sm_policy_context_data->slice_info) {
        OpenAPI_snssai_free(sm_policy_context_data->slice_info);
        sm_policy_context_data->slice_info = NULL;
    }
    if (sm_policy_context_data->serv_nf_id) {
        OpenAPI_serving_nf_identity_free(sm_policy_context_data->serv_nf_id);
        sm_policy_context_data->serv_nf_id = NULL;
    }
    if (sm_policy_context_data->supp_feat) {
        ogs_free(sm_policy_context_data->supp_feat);
        sm_policy_context_data->supp_feat = NULL;
    }
    if (sm_policy_context_data->smf_id) {
        ogs_free(sm_policy_context_data->smf_id);
        sm_policy_context_data->smf_id = NULL;
    }
    if (sm_policy_context_data->recovery_time) {
        ogs_free(sm_policy_context_data->recovery_time);
        sm_policy_context_data->recovery_time = NULL;
    }
    if (sm_policy_context_data->ipv4_frame_route_list) {
        OpenAPI_list_for_each(sm_policy_context_data->ipv4_frame_route_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_policy_context_data->ipv4_frame_route_list);
        sm_policy_context_data->ipv4_frame_route_list = NULL;
    }
    if (sm_policy_context_data->ipv6_frame_route_list) {
        OpenAPI_list_for_each(sm_policy_context_data->ipv6_frame_route_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sm_policy_context_data->ipv6_frame_route_list);
        sm_policy_context_data->ipv6_frame_route_list = NULL;
    }
    if (sm_policy_context_data->pcf_ue_info) {
        OpenAPI_pcf_ue_callback_info_free(sm_policy_context_data->pcf_ue_info);
        sm_policy_context_data->pcf_ue_info = NULL;
    }
    if (sm_policy_context_data->pvs_info) {
        OpenAPI_list_for_each(sm_policy_context_data->pvs_info, node) {
            OpenAPI_server_addressing_info_free(node->data);
        }
        OpenAPI_list_free(sm_policy_context_data->pvs_info);
        sm_policy_context_data->pvs_info = NULL;
    }
    if (sm_policy_context_data->nwdaf_datas) {
        OpenAPI_list_for_each(sm_policy_context_data->nwdaf_datas, node) {
            OpenAPI_nwdaf_data_free(node->data);
        }
        OpenAPI_list_free(sm_policy_context_data->nwdaf_datas);
        sm_policy_context_data->nwdaf_datas = NULL;
    }
    ogs_free(sm_policy_context_data);
}

cJSON *OpenAPI_sm_policy_context_data_convertToJSON(OpenAPI_sm_policy_context_data_t *sm_policy_context_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_policy_context_data == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [SmPolicyContextData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_policy_context_data->acc_net_ch_id) {
    cJSON *acc_net_ch_id_local_JSON = OpenAPI_acc_net_ch_id_convertToJSON(sm_policy_context_data->acc_net_ch_id);
    if (acc_net_ch_id_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [acc_net_ch_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accNetChId", acc_net_ch_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [acc_net_ch_id]");
        goto end;
    }
    }

    if (sm_policy_context_data->charg_entity_addr) {
    cJSON *charg_entity_addr_local_JSON = OpenAPI_acc_net_charging_address_convertToJSON(sm_policy_context_data->charg_entity_addr);
    if (charg_entity_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [charg_entity_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "chargEntityAddr", charg_entity_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [charg_entity_addr]");
        goto end;
    }
    }

    if (sm_policy_context_data->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", sm_policy_context_data->gpsi) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (!sm_policy_context_data->supi) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [supi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supi", sm_policy_context_data->supi) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [supi]");
        goto end;
    }

    if (sm_policy_context_data->is_invalid_supi) {
    if (cJSON_AddBoolToObject(item, "invalidSupi", sm_policy_context_data->invalid_supi) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [invalid_supi]");
        goto end;
    }
    }

    if (sm_policy_context_data->inter_grp_ids) {
    cJSON *inter_grp_idsList = cJSON_AddArrayToObject(item, "interGrpIds");
    if (inter_grp_idsList == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [inter_grp_ids]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_context_data->inter_grp_ids, node) {
        if (cJSON_AddStringToObject(inter_grp_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [inter_grp_ids]");
            goto end;
        }
    }
    }

    if (cJSON_AddNumberToObject(item, "pduSessionId", sm_policy_context_data->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [pdu_session_id]");
        goto end;
    }

    if (sm_policy_context_data->pdu_session_type == OpenAPI_pdu_session_type_NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [pdu_session_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "pduSessionType", OpenAPI_pdu_session_type_ToString(sm_policy_context_data->pdu_session_type)) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [pdu_session_type]");
        goto end;
    }

    if (sm_policy_context_data->chargingcharacteristics) {
    if (cJSON_AddStringToObject(item, "chargingcharacteristics", sm_policy_context_data->chargingcharacteristics) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [chargingcharacteristics]");
        goto end;
    }
    }

    if (!sm_policy_context_data->dnn) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", sm_policy_context_data->dnn) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [dnn]");
        goto end;
    }

    if (sm_policy_context_data->dnn_sel_mode != OpenAPI_dnn_selection_mode_NULL) {
    if (cJSON_AddStringToObject(item, "dnnSelMode", OpenAPI_dnn_selection_mode_ToString(sm_policy_context_data->dnn_sel_mode)) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [dnn_sel_mode]");
        goto end;
    }
    }

    if (!sm_policy_context_data->notification_uri) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [notification_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notificationUri", sm_policy_context_data->notification_uri) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [notification_uri]");
        goto end;
    }

    if (sm_policy_context_data->access_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(sm_policy_context_data->access_type)) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [access_type]");
        goto end;
    }
    }

    if (sm_policy_context_data->rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(sm_policy_context_data->rat_type)) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (sm_policy_context_data->add_access_info) {
    cJSON *add_access_info_local_JSON = OpenAPI_additional_access_info_convertToJSON(sm_policy_context_data->add_access_info);
    if (add_access_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [add_access_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "addAccessInfo", add_access_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [add_access_info]");
        goto end;
    }
    }

    if (sm_policy_context_data->serving_network) {
    cJSON *serving_network_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(sm_policy_context_data->serving_network);
    if (serving_network_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetwork", serving_network_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [serving_network]");
        goto end;
    }
    }

    if (sm_policy_context_data->user_location_info) {
    cJSON *user_location_info_local_JSON = OpenAPI_user_location_convertToJSON(sm_policy_context_data->user_location_info);
    if (user_location_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [user_location_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationInfo", user_location_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [user_location_info]");
        goto end;
    }
    }

    if (sm_policy_context_data->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", sm_policy_context_data->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (sm_policy_context_data->pei) {
    if (cJSON_AddStringToObject(item, "pei", sm_policy_context_data->pei) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (sm_policy_context_data->ipv4_address) {
    if (cJSON_AddStringToObject(item, "ipv4Address", sm_policy_context_data->ipv4_address) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [ipv4_address]");
        goto end;
    }
    }

    if (sm_policy_context_data->ipv6_address_prefix) {
    if (cJSON_AddStringToObject(item, "ipv6AddressPrefix", sm_policy_context_data->ipv6_address_prefix) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [ipv6_address_prefix]");
        goto end;
    }
    }

    if (sm_policy_context_data->ip_domain) {
    if (cJSON_AddStringToObject(item, "ipDomain", sm_policy_context_data->ip_domain) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [ip_domain]");
        goto end;
    }
    }

    if (sm_policy_context_data->subs_sess_ambr) {
    cJSON *subs_sess_ambr_local_JSON = OpenAPI_ambr_convertToJSON(sm_policy_context_data->subs_sess_ambr);
    if (subs_sess_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [subs_sess_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "subsSessAmbr", subs_sess_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [subs_sess_ambr]");
        goto end;
    }
    }

    if (sm_policy_context_data->auth_prof_index) {
    if (cJSON_AddStringToObject(item, "authProfIndex", sm_policy_context_data->auth_prof_index) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [auth_prof_index]");
        goto end;
    }
    }

    if (sm_policy_context_data->subs_def_qos) {
    cJSON *subs_def_qos_local_JSON = OpenAPI_subscribed_default_qos_convertToJSON(sm_policy_context_data->subs_def_qos);
    if (subs_def_qos_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [subs_def_qos]");
        goto end;
    }
    cJSON_AddItemToObject(item, "subsDefQos", subs_def_qos_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [subs_def_qos]");
        goto end;
    }
    }

    if (sm_policy_context_data->vplmn_qos) {
    cJSON *vplmn_qos_local_JSON = OpenAPI_vplmn_qos_convertToJSON(sm_policy_context_data->vplmn_qos);
    if (vplmn_qos_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [vplmn_qos]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vplmnQos", vplmn_qos_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [vplmn_qos]");
        goto end;
    }
    }

    if (sm_policy_context_data->is_num_of_pack_filter) {
    if (cJSON_AddNumberToObject(item, "numOfPackFilter", sm_policy_context_data->num_of_pack_filter) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [num_of_pack_filter]");
        goto end;
    }
    }

    if (sm_policy_context_data->is_online) {
    if (cJSON_AddBoolToObject(item, "online", sm_policy_context_data->online) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [online]");
        goto end;
    }
    }

    if (sm_policy_context_data->is_offline) {
    if (cJSON_AddBoolToObject(item, "offline", sm_policy_context_data->offline) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [offline]");
        goto end;
    }
    }

    if (sm_policy_context_data->is__3gpp_ps_data_off_status) {
    if (cJSON_AddBoolToObject(item, "3gppPsDataOffStatus", sm_policy_context_data->_3gpp_ps_data_off_status) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [_3gpp_ps_data_off_status]");
        goto end;
    }
    }

    if (sm_policy_context_data->is_ref_qos_indication) {
    if (cJSON_AddBoolToObject(item, "refQosIndication", sm_policy_context_data->ref_qos_indication) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [ref_qos_indication]");
        goto end;
    }
    }

    if (sm_policy_context_data->trace_req) {
    cJSON *trace_req_local_JSON = OpenAPI_trace_data_convertToJSON(sm_policy_context_data->trace_req);
    if (trace_req_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [trace_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceReq", trace_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [trace_req]");
        goto end;
    }
    } else if (sm_policy_context_data->is_trace_req_null) {
        if (cJSON_AddNullToObject(item, "traceReq") == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [trace_req]");
            goto end;
        }
    }

    if (!sm_policy_context_data->slice_info) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [slice_info]");
        return NULL;
    }
    cJSON *slice_info_local_JSON = OpenAPI_snssai_convertToJSON(sm_policy_context_data->slice_info);
    if (slice_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [slice_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sliceInfo", slice_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [slice_info]");
        goto end;
    }

    if (sm_policy_context_data->qos_flow_usage != OpenAPI_qos_flow_usage_NULL) {
    if (cJSON_AddStringToObject(item, "qosFlowUsage", OpenAPI_qos_flow_usage_ToString(sm_policy_context_data->qos_flow_usage)) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [qos_flow_usage]");
        goto end;
    }
    }

    if (sm_policy_context_data->serv_nf_id) {
    cJSON *serv_nf_id_local_JSON = OpenAPI_serving_nf_identity_convertToJSON(sm_policy_context_data->serv_nf_id);
    if (serv_nf_id_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [serv_nf_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servNfId", serv_nf_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [serv_nf_id]");
        goto end;
    }
    }

    if (sm_policy_context_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", sm_policy_context_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

    if (sm_policy_context_data->smf_id) {
    if (cJSON_AddStringToObject(item, "smfId", sm_policy_context_data->smf_id) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [smf_id]");
        goto end;
    }
    }

    if (sm_policy_context_data->recovery_time) {
    if (cJSON_AddStringToObject(item, "recoveryTime", sm_policy_context_data->recovery_time) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [recovery_time]");
        goto end;
    }
    }

    if (sm_policy_context_data->ma_pdu_ind != OpenAPI_ma_pdu_indication_NULL) {
    if (cJSON_AddStringToObject(item, "maPduInd", OpenAPI_ma_pdu_indication_ToString(sm_policy_context_data->ma_pdu_ind)) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [ma_pdu_ind]");
        goto end;
    }
    }

    if (sm_policy_context_data->atsss_capab != OpenAPI_npcf_atsss_capability_NULL) {
    if (cJSON_AddStringToObject(item, "atsssCapab", OpenAPI_npcf_atsss_capability_ToString(sm_policy_context_data->atsss_capab)) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [atsss_capab]");
        goto end;
    }
    }

    if (sm_policy_context_data->ipv4_frame_route_list) {
    cJSON *ipv4_frame_route_listList = cJSON_AddArrayToObject(item, "ipv4FrameRouteList");
    if (ipv4_frame_route_listList == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [ipv4_frame_route_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_context_data->ipv4_frame_route_list, node) {
        if (cJSON_AddStringToObject(ipv4_frame_route_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [ipv4_frame_route_list]");
            goto end;
        }
    }
    }

    if (sm_policy_context_data->ipv6_frame_route_list) {
    cJSON *ipv6_frame_route_listList = cJSON_AddArrayToObject(item, "ipv6FrameRouteList");
    if (ipv6_frame_route_listList == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [ipv6_frame_route_list]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_context_data->ipv6_frame_route_list, node) {
        if (cJSON_AddStringToObject(ipv6_frame_route_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [ipv6_frame_route_list]");
            goto end;
        }
    }
    }

    if (sm_policy_context_data->sat_backhaul_category != OpenAPI_satellite_backhaul_category_NULL) {
    if (cJSON_AddStringToObject(item, "satBackhaulCategory", OpenAPI_satellite_backhaul_category_ToString(sm_policy_context_data->sat_backhaul_category)) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [sat_backhaul_category]");
        goto end;
    }
    }

    if (sm_policy_context_data->pcf_ue_info) {
    cJSON *pcf_ue_info_local_JSON = OpenAPI_pcf_ue_callback_info_convertToJSON(sm_policy_context_data->pcf_ue_info);
    if (pcf_ue_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [pcf_ue_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pcfUeInfo", pcf_ue_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [pcf_ue_info]");
        goto end;
    }
    } else if (sm_policy_context_data->is_pcf_ue_info_null) {
        if (cJSON_AddNullToObject(item, "pcfUeInfo") == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [pcf_ue_info]");
            goto end;
        }
    }

    if (sm_policy_context_data->pvs_info) {
    cJSON *pvs_infoList = cJSON_AddArrayToObject(item, "pvsInfo");
    if (pvs_infoList == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [pvs_info]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_context_data->pvs_info, node) {
        cJSON *itemLocal = OpenAPI_server_addressing_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [pvs_info]");
            goto end;
        }
        cJSON_AddItemToArray(pvs_infoList, itemLocal);
    }
    }

    if (sm_policy_context_data->is_onboard_ind) {
    if (cJSON_AddBoolToObject(item, "onboardInd", sm_policy_context_data->onboard_ind) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [onboard_ind]");
        goto end;
    }
    }

    if (sm_policy_context_data->nwdaf_datas) {
    cJSON *nwdaf_datasList = cJSON_AddArrayToObject(item, "nwdafDatas");
    if (nwdaf_datasList == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [nwdaf_datas]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_context_data->nwdaf_datas, node) {
        cJSON *itemLocal = OpenAPI_nwdaf_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [nwdaf_datas]");
            goto end;
        }
        cJSON_AddItemToArray(nwdaf_datasList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_sm_policy_context_data_t *OpenAPI_sm_policy_context_data_parseFromJSON(cJSON *sm_policy_context_dataJSON)
{
    OpenAPI_sm_policy_context_data_t *sm_policy_context_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *acc_net_ch_id = NULL;
    OpenAPI_acc_net_ch_id_t *acc_net_ch_id_local_nonprim = NULL;
    cJSON *charg_entity_addr = NULL;
    OpenAPI_acc_net_charging_address_t *charg_entity_addr_local_nonprim = NULL;
    cJSON *gpsi = NULL;
    cJSON *supi = NULL;
    cJSON *invalid_supi = NULL;
    cJSON *inter_grp_ids = NULL;
    OpenAPI_list_t *inter_grp_idsList = NULL;
    cJSON *pdu_session_id = NULL;
    cJSON *pdu_session_type = NULL;
    OpenAPI_pdu_session_type_e pdu_session_typeVariable = 0;
    cJSON *chargingcharacteristics = NULL;
    cJSON *dnn = NULL;
    cJSON *dnn_sel_mode = NULL;
    OpenAPI_dnn_selection_mode_e dnn_sel_modeVariable = 0;
    cJSON *notification_uri = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    cJSON *rat_type = NULL;
    OpenAPI_rat_type_e rat_typeVariable = 0;
    cJSON *add_access_info = NULL;
    OpenAPI_additional_access_info_t *add_access_info_local_nonprim = NULL;
    cJSON *serving_network = NULL;
    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    cJSON *user_location_info = NULL;
    OpenAPI_user_location_t *user_location_info_local_nonprim = NULL;
    cJSON *ue_time_zone = NULL;
    cJSON *pei = NULL;
    cJSON *ipv4_address = NULL;
    cJSON *ipv6_address_prefix = NULL;
    cJSON *ip_domain = NULL;
    cJSON *subs_sess_ambr = NULL;
    OpenAPI_ambr_t *subs_sess_ambr_local_nonprim = NULL;
    cJSON *auth_prof_index = NULL;
    cJSON *subs_def_qos = NULL;
    OpenAPI_subscribed_default_qos_t *subs_def_qos_local_nonprim = NULL;
    cJSON *vplmn_qos = NULL;
    OpenAPI_vplmn_qos_t *vplmn_qos_local_nonprim = NULL;
    cJSON *num_of_pack_filter = NULL;
    cJSON *online = NULL;
    cJSON *offline = NULL;
    cJSON *_3gpp_ps_data_off_status = NULL;
    cJSON *ref_qos_indication = NULL;
    cJSON *trace_req = NULL;
    OpenAPI_trace_data_t *trace_req_local_nonprim = NULL;
    cJSON *slice_info = NULL;
    OpenAPI_snssai_t *slice_info_local_nonprim = NULL;
    cJSON *qos_flow_usage = NULL;
    OpenAPI_qos_flow_usage_e qos_flow_usageVariable = 0;
    cJSON *serv_nf_id = NULL;
    OpenAPI_serving_nf_identity_t *serv_nf_id_local_nonprim = NULL;
    cJSON *supp_feat = NULL;
    cJSON *smf_id = NULL;
    cJSON *recovery_time = NULL;
    cJSON *ma_pdu_ind = NULL;
    OpenAPI_ma_pdu_indication_e ma_pdu_indVariable = 0;
    cJSON *atsss_capab = NULL;
    OpenAPI_npcf_atsss_capability_e atsss_capabVariable = 0;
    cJSON *ipv4_frame_route_list = NULL;
    OpenAPI_list_t *ipv4_frame_route_listList = NULL;
    cJSON *ipv6_frame_route_list = NULL;
    OpenAPI_list_t *ipv6_frame_route_listList = NULL;
    cJSON *sat_backhaul_category = NULL;
    OpenAPI_satellite_backhaul_category_e sat_backhaul_categoryVariable = 0;
    cJSON *pcf_ue_info = NULL;
    OpenAPI_pcf_ue_callback_info_t *pcf_ue_info_local_nonprim = NULL;
    cJSON *pvs_info = NULL;
    OpenAPI_list_t *pvs_infoList = NULL;
    cJSON *onboard_ind = NULL;
    cJSON *nwdaf_datas = NULL;
    OpenAPI_list_t *nwdaf_datasList = NULL;
    acc_net_ch_id = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "accNetChId");
    if (acc_net_ch_id) {
    acc_net_ch_id_local_nonprim = OpenAPI_acc_net_ch_id_parseFromJSON(acc_net_ch_id);
    if (!acc_net_ch_id_local_nonprim) {
        ogs_error("OpenAPI_acc_net_ch_id_parseFromJSON failed [acc_net_ch_id]");
        goto end;
    }
    }

    charg_entity_addr = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "chargEntityAddr");
    if (charg_entity_addr) {
    charg_entity_addr_local_nonprim = OpenAPI_acc_net_charging_address_parseFromJSON(charg_entity_addr);
    if (!charg_entity_addr_local_nonprim) {
        ogs_error("OpenAPI_acc_net_charging_address_parseFromJSON failed [charg_entity_addr]");
        goto end;
    }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    supi = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [supi]");
        goto end;
    }
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [supi]");
        goto end;
    }

    invalid_supi = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "invalidSupi");
    if (invalid_supi) {
    if (!cJSON_IsBool(invalid_supi)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [invalid_supi]");
        goto end;
    }
    }

    inter_grp_ids = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "interGrpIds");
    if (inter_grp_ids) {
        cJSON *inter_grp_ids_local = NULL;
        if (!cJSON_IsArray(inter_grp_ids)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [inter_grp_ids]");
            goto end;
        }

        inter_grp_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(inter_grp_ids_local, inter_grp_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(inter_grp_ids_local)) {
                ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [inter_grp_ids]");
                goto end;
            }
            OpenAPI_list_add(inter_grp_idsList, ogs_strdup(inter_grp_ids_local->valuestring));
        }
    }

    pdu_session_id = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "pduSessionId");
    if (!pdu_session_id) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    pdu_session_type = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "pduSessionType");
    if (!pdu_session_type) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [pdu_session_type]");
        goto end;
    }
    if (!cJSON_IsString(pdu_session_type)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [pdu_session_type]");
        goto end;
    }
    pdu_session_typeVariable = OpenAPI_pdu_session_type_FromString(pdu_session_type->valuestring);

    chargingcharacteristics = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "chargingcharacteristics");
    if (chargingcharacteristics) {
    if (!cJSON_IsString(chargingcharacteristics) && !cJSON_IsNull(chargingcharacteristics)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [chargingcharacteristics]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [dnn]");
        goto end;
    }

    dnn_sel_mode = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "dnnSelMode");
    if (dnn_sel_mode) {
    if (!cJSON_IsString(dnn_sel_mode)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [dnn_sel_mode]");
        goto end;
    }
    dnn_sel_modeVariable = OpenAPI_dnn_selection_mode_FromString(dnn_sel_mode->valuestring);
    }

    notification_uri = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "notificationUri");
    if (!notification_uri) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [notification_uri]");
        goto end;
    }
    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    access_type = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "accessType");
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    rat_type = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "ratType");
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    add_access_info = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "addAccessInfo");
    if (add_access_info) {
    add_access_info_local_nonprim = OpenAPI_additional_access_info_parseFromJSON(add_access_info);
    if (!add_access_info_local_nonprim) {
        ogs_error("OpenAPI_additional_access_info_parseFromJSON failed [add_access_info]");
        goto end;
    }
    }

    serving_network = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "servingNetwork");
    if (serving_network) {
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    if (!serving_network_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [serving_network]");
        goto end;
    }
    }

    user_location_info = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "userLocationInfo");
    if (user_location_info) {
    user_location_info_local_nonprim = OpenAPI_user_location_parseFromJSON(user_location_info);
    if (!user_location_info_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [user_location_info]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    pei = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    ipv4_address = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "ipv4Address");
    if (ipv4_address) {
    if (!cJSON_IsString(ipv4_address) && !cJSON_IsNull(ipv4_address)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ipv4_address]");
        goto end;
    }
    }

    ipv6_address_prefix = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "ipv6AddressPrefix");
    if (ipv6_address_prefix) {
    if (!cJSON_IsString(ipv6_address_prefix) && !cJSON_IsNull(ipv6_address_prefix)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ipv6_address_prefix]");
        goto end;
    }
    }

    ip_domain = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "ipDomain");
    if (ip_domain) {
    if (!cJSON_IsString(ip_domain) && !cJSON_IsNull(ip_domain)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ip_domain]");
        goto end;
    }
    }

    subs_sess_ambr = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "subsSessAmbr");
    if (subs_sess_ambr) {
    subs_sess_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(subs_sess_ambr);
    if (!subs_sess_ambr_local_nonprim) {
        ogs_error("OpenAPI_ambr_parseFromJSON failed [subs_sess_ambr]");
        goto end;
    }
    }

    auth_prof_index = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "authProfIndex");
    if (auth_prof_index) {
    if (!cJSON_IsString(auth_prof_index) && !cJSON_IsNull(auth_prof_index)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [auth_prof_index]");
        goto end;
    }
    }

    subs_def_qos = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "subsDefQos");
    if (subs_def_qos) {
    subs_def_qos_local_nonprim = OpenAPI_subscribed_default_qos_parseFromJSON(subs_def_qos);
    if (!subs_def_qos_local_nonprim) {
        ogs_error("OpenAPI_subscribed_default_qos_parseFromJSON failed [subs_def_qos]");
        goto end;
    }
    }

    vplmn_qos = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "vplmnQos");
    if (vplmn_qos) {
    vplmn_qos_local_nonprim = OpenAPI_vplmn_qos_parseFromJSON(vplmn_qos);
    if (!vplmn_qos_local_nonprim) {
        ogs_error("OpenAPI_vplmn_qos_parseFromJSON failed [vplmn_qos]");
        goto end;
    }
    }

    num_of_pack_filter = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "numOfPackFilter");
    if (num_of_pack_filter) {
    if (!cJSON_IsNumber(num_of_pack_filter)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [num_of_pack_filter]");
        goto end;
    }
    }

    online = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "online");
    if (online) {
    if (!cJSON_IsBool(online)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [online]");
        goto end;
    }
    }

    offline = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "offline");
    if (offline) {
    if (!cJSON_IsBool(offline)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [offline]");
        goto end;
    }
    }

    _3gpp_ps_data_off_status = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "3gppPsDataOffStatus");
    if (_3gpp_ps_data_off_status) {
    if (!cJSON_IsBool(_3gpp_ps_data_off_status)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [_3gpp_ps_data_off_status]");
        goto end;
    }
    }

    ref_qos_indication = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "refQosIndication");
    if (ref_qos_indication) {
    if (!cJSON_IsBool(ref_qos_indication)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ref_qos_indication]");
        goto end;
    }
    }

    trace_req = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "traceReq");
    if (trace_req) {
    if (!cJSON_IsNull(trace_req)) {
    trace_req_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_req);
    if (!trace_req_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_req]");
        goto end;
    }
    }
    }

    slice_info = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "sliceInfo");
    if (!slice_info) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [slice_info]");
        goto end;
    }
    slice_info_local_nonprim = OpenAPI_snssai_parseFromJSON(slice_info);
    if (!slice_info_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [slice_info]");
        goto end;
    }

    qos_flow_usage = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "qosFlowUsage");
    if (qos_flow_usage) {
    if (!cJSON_IsString(qos_flow_usage)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [qos_flow_usage]");
        goto end;
    }
    qos_flow_usageVariable = OpenAPI_qos_flow_usage_FromString(qos_flow_usage->valuestring);
    }

    serv_nf_id = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "servNfId");
    if (serv_nf_id) {
    serv_nf_id_local_nonprim = OpenAPI_serving_nf_identity_parseFromJSON(serv_nf_id);
    if (!serv_nf_id_local_nonprim) {
        ogs_error("OpenAPI_serving_nf_identity_parseFromJSON failed [serv_nf_id]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    smf_id = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "smfId");
    if (smf_id) {
    if (!cJSON_IsString(smf_id) && !cJSON_IsNull(smf_id)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [smf_id]");
        goto end;
    }
    }

    recovery_time = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "recoveryTime");
    if (recovery_time) {
    if (!cJSON_IsString(recovery_time) && !cJSON_IsNull(recovery_time)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [recovery_time]");
        goto end;
    }
    }

    ma_pdu_ind = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "maPduInd");
    if (ma_pdu_ind) {
    if (!cJSON_IsString(ma_pdu_ind)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ma_pdu_ind]");
        goto end;
    }
    ma_pdu_indVariable = OpenAPI_ma_pdu_indication_FromString(ma_pdu_ind->valuestring);
    }

    atsss_capab = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "atsssCapab");
    if (atsss_capab) {
    if (!cJSON_IsString(atsss_capab)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [atsss_capab]");
        goto end;
    }
    atsss_capabVariable = OpenAPI_npcf_atsss_capability_FromString(atsss_capab->valuestring);
    }

    ipv4_frame_route_list = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "ipv4FrameRouteList");
    if (ipv4_frame_route_list) {
        cJSON *ipv4_frame_route_list_local = NULL;
        if (!cJSON_IsArray(ipv4_frame_route_list)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ipv4_frame_route_list]");
            goto end;
        }

        ipv4_frame_route_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_frame_route_list_local, ipv4_frame_route_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv4_frame_route_list_local)) {
                ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ipv4_frame_route_list]");
                goto end;
            }
            OpenAPI_list_add(ipv4_frame_route_listList, ogs_strdup(ipv4_frame_route_list_local->valuestring));
        }
    }

    ipv6_frame_route_list = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "ipv6FrameRouteList");
    if (ipv6_frame_route_list) {
        cJSON *ipv6_frame_route_list_local = NULL;
        if (!cJSON_IsArray(ipv6_frame_route_list)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ipv6_frame_route_list]");
            goto end;
        }

        ipv6_frame_route_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_frame_route_list_local, ipv6_frame_route_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ipv6_frame_route_list_local)) {
                ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ipv6_frame_route_list]");
                goto end;
            }
            OpenAPI_list_add(ipv6_frame_route_listList, ogs_strdup(ipv6_frame_route_list_local->valuestring));
        }
    }

    sat_backhaul_category = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "satBackhaulCategory");
    if (sat_backhaul_category) {
    if (!cJSON_IsString(sat_backhaul_category)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [sat_backhaul_category]");
        goto end;
    }
    sat_backhaul_categoryVariable = OpenAPI_satellite_backhaul_category_FromString(sat_backhaul_category->valuestring);
    }

    pcf_ue_info = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "pcfUeInfo");
    if (pcf_ue_info) {
    if (!cJSON_IsNull(pcf_ue_info)) {
    pcf_ue_info_local_nonprim = OpenAPI_pcf_ue_callback_info_parseFromJSON(pcf_ue_info);
    if (!pcf_ue_info_local_nonprim) {
        ogs_error("OpenAPI_pcf_ue_callback_info_parseFromJSON failed [pcf_ue_info]");
        goto end;
    }
    }
    }

    pvs_info = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "pvsInfo");
    if (pvs_info) {
        cJSON *pvs_info_local = NULL;
        if (!cJSON_IsArray(pvs_info)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [pvs_info]");
            goto end;
        }

        pvs_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(pvs_info_local, pvs_info) {
            if (!cJSON_IsObject(pvs_info_local)) {
                ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [pvs_info]");
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

    onboard_ind = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "onboardInd");
    if (onboard_ind) {
    if (!cJSON_IsBool(onboard_ind)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [onboard_ind]");
        goto end;
    }
    }

    nwdaf_datas = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "nwdafDatas");
    if (nwdaf_datas) {
        cJSON *nwdaf_datas_local = NULL;
        if (!cJSON_IsArray(nwdaf_datas)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [nwdaf_datas]");
            goto end;
        }

        nwdaf_datasList = OpenAPI_list_create();

        cJSON_ArrayForEach(nwdaf_datas_local, nwdaf_datas) {
            if (!cJSON_IsObject(nwdaf_datas_local)) {
                ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [nwdaf_datas]");
                goto end;
            }
            OpenAPI_nwdaf_data_t *nwdaf_datasItem = OpenAPI_nwdaf_data_parseFromJSON(nwdaf_datas_local);
            if (!nwdaf_datasItem) {
                ogs_error("No nwdaf_datasItem");
                goto end;
            }
            OpenAPI_list_add(nwdaf_datasList, nwdaf_datasItem);
        }
    }

    sm_policy_context_data_local_var = OpenAPI_sm_policy_context_data_create (
        acc_net_ch_id ? acc_net_ch_id_local_nonprim : NULL,
        charg_entity_addr ? charg_entity_addr_local_nonprim : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        ogs_strdup(supi->valuestring),
        invalid_supi ? true : false,
        invalid_supi ? invalid_supi->valueint : 0,
        inter_grp_ids ? inter_grp_idsList : NULL,
        
        pdu_session_id->valuedouble,
        pdu_session_typeVariable,
        chargingcharacteristics && !cJSON_IsNull(chargingcharacteristics) ? ogs_strdup(chargingcharacteristics->valuestring) : NULL,
        ogs_strdup(dnn->valuestring),
        dnn_sel_mode ? dnn_sel_modeVariable : 0,
        ogs_strdup(notification_uri->valuestring),
        access_type ? access_typeVariable : 0,
        rat_type ? rat_typeVariable : 0,
        add_access_info ? add_access_info_local_nonprim : NULL,
        serving_network ? serving_network_local_nonprim : NULL,
        user_location_info ? user_location_info_local_nonprim : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        ipv4_address && !cJSON_IsNull(ipv4_address) ? ogs_strdup(ipv4_address->valuestring) : NULL,
        ipv6_address_prefix && !cJSON_IsNull(ipv6_address_prefix) ? ogs_strdup(ipv6_address_prefix->valuestring) : NULL,
        ip_domain && !cJSON_IsNull(ip_domain) ? ogs_strdup(ip_domain->valuestring) : NULL,
        subs_sess_ambr ? subs_sess_ambr_local_nonprim : NULL,
        auth_prof_index && !cJSON_IsNull(auth_prof_index) ? ogs_strdup(auth_prof_index->valuestring) : NULL,
        subs_def_qos ? subs_def_qos_local_nonprim : NULL,
        vplmn_qos ? vplmn_qos_local_nonprim : NULL,
        num_of_pack_filter ? true : false,
        num_of_pack_filter ? num_of_pack_filter->valuedouble : 0,
        online ? true : false,
        online ? online->valueint : 0,
        offline ? true : false,
        offline ? offline->valueint : 0,
        _3gpp_ps_data_off_status ? true : false,
        _3gpp_ps_data_off_status ? _3gpp_ps_data_off_status->valueint : 0,
        ref_qos_indication ? true : false,
        ref_qos_indication ? ref_qos_indication->valueint : 0,
        trace_req && cJSON_IsNull(trace_req) ? true : false,
        trace_req ? trace_req_local_nonprim : NULL,
        slice_info_local_nonprim,
        qos_flow_usage ? qos_flow_usageVariable : 0,
        serv_nf_id ? serv_nf_id_local_nonprim : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        smf_id && !cJSON_IsNull(smf_id) ? ogs_strdup(smf_id->valuestring) : NULL,
        recovery_time && !cJSON_IsNull(recovery_time) ? ogs_strdup(recovery_time->valuestring) : NULL,
        ma_pdu_ind ? ma_pdu_indVariable : 0,
        atsss_capab ? atsss_capabVariable : 0,
        ipv4_frame_route_list ? ipv4_frame_route_listList : NULL,
        ipv6_frame_route_list ? ipv6_frame_route_listList : NULL,
        sat_backhaul_category ? sat_backhaul_categoryVariable : 0,
        pcf_ue_info && cJSON_IsNull(pcf_ue_info) ? true : false,
        pcf_ue_info ? pcf_ue_info_local_nonprim : NULL,
        pvs_info ? pvs_infoList : NULL,
        onboard_ind ? true : false,
        onboard_ind ? onboard_ind->valueint : 0,
        nwdaf_datas ? nwdaf_datasList : NULL
    );

    return sm_policy_context_data_local_var;
end:
    if (acc_net_ch_id_local_nonprim) {
        OpenAPI_acc_net_ch_id_free(acc_net_ch_id_local_nonprim);
        acc_net_ch_id_local_nonprim = NULL;
    }
    if (charg_entity_addr_local_nonprim) {
        OpenAPI_acc_net_charging_address_free(charg_entity_addr_local_nonprim);
        charg_entity_addr_local_nonprim = NULL;
    }
    if (inter_grp_idsList) {
        OpenAPI_list_for_each(inter_grp_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(inter_grp_idsList);
        inter_grp_idsList = NULL;
    }
    if (add_access_info_local_nonprim) {
        OpenAPI_additional_access_info_free(add_access_info_local_nonprim);
        add_access_info_local_nonprim = NULL;
    }
    if (serving_network_local_nonprim) {
        OpenAPI_plmn_id_nid_free(serving_network_local_nonprim);
        serving_network_local_nonprim = NULL;
    }
    if (user_location_info_local_nonprim) {
        OpenAPI_user_location_free(user_location_info_local_nonprim);
        user_location_info_local_nonprim = NULL;
    }
    if (subs_sess_ambr_local_nonprim) {
        OpenAPI_ambr_free(subs_sess_ambr_local_nonprim);
        subs_sess_ambr_local_nonprim = NULL;
    }
    if (subs_def_qos_local_nonprim) {
        OpenAPI_subscribed_default_qos_free(subs_def_qos_local_nonprim);
        subs_def_qos_local_nonprim = NULL;
    }
    if (vplmn_qos_local_nonprim) {
        OpenAPI_vplmn_qos_free(vplmn_qos_local_nonprim);
        vplmn_qos_local_nonprim = NULL;
    }
    if (trace_req_local_nonprim) {
        OpenAPI_trace_data_free(trace_req_local_nonprim);
        trace_req_local_nonprim = NULL;
    }
    if (slice_info_local_nonprim) {
        OpenAPI_snssai_free(slice_info_local_nonprim);
        slice_info_local_nonprim = NULL;
    }
    if (serv_nf_id_local_nonprim) {
        OpenAPI_serving_nf_identity_free(serv_nf_id_local_nonprim);
        serv_nf_id_local_nonprim = NULL;
    }
    if (ipv4_frame_route_listList) {
        OpenAPI_list_for_each(ipv4_frame_route_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv4_frame_route_listList);
        ipv4_frame_route_listList = NULL;
    }
    if (ipv6_frame_route_listList) {
        OpenAPI_list_for_each(ipv6_frame_route_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ipv6_frame_route_listList);
        ipv6_frame_route_listList = NULL;
    }
    if (pcf_ue_info_local_nonprim) {
        OpenAPI_pcf_ue_callback_info_free(pcf_ue_info_local_nonprim);
        pcf_ue_info_local_nonprim = NULL;
    }
    if (pvs_infoList) {
        OpenAPI_list_for_each(pvs_infoList, node) {
            OpenAPI_server_addressing_info_free(node->data);
        }
        OpenAPI_list_free(pvs_infoList);
        pvs_infoList = NULL;
    }
    if (nwdaf_datasList) {
        OpenAPI_list_for_each(nwdaf_datasList, node) {
            OpenAPI_nwdaf_data_free(node->data);
        }
        OpenAPI_list_free(nwdaf_datasList);
        nwdaf_datasList = NULL;
    }
    return NULL;
}

OpenAPI_sm_policy_context_data_t *OpenAPI_sm_policy_context_data_copy(OpenAPI_sm_policy_context_data_t *dst, OpenAPI_sm_policy_context_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_policy_context_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed");
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

    OpenAPI_sm_policy_context_data_free(dst);
    dst = OpenAPI_sm_policy_context_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

