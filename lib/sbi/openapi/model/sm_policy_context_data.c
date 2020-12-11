
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_context_data.h"

OpenAPI_sm_policy_context_data_t *OpenAPI_sm_policy_context_data_create(
    OpenAPI_acc_net_ch_id_t *acc_net_ch_id,
    OpenAPI_acc_net_charging_address_t *charg_entity_addr,
    char *gpsi,
    char *supi,
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
    int num_of_pack_filter,
    int online,
    int offline,
    int _3gpp_ps_data_off_status,
    int ref_qos_indication,
    OpenAPI_trace_data_t *trace_req,
    OpenAPI_snssai_t *slice_info,
    OpenAPI_qos_flow_usage_t *qos_flow_usage,
    OpenAPI_serving_nf_identity_t *serv_nf_id,
    char *supp_feat,
    char *smf_id,
    char *recovery_time,
    OpenAPI_ma_pdu_indication_t *ma_pdu_ind,
    OpenAPI_atsss_capability_t *atsss_capab
    )
{
    OpenAPI_sm_policy_context_data_t *sm_policy_context_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_sm_policy_context_data_t));
    if (!sm_policy_context_data_local_var) {
        return NULL;
    }
    sm_policy_context_data_local_var->acc_net_ch_id = acc_net_ch_id;
    sm_policy_context_data_local_var->charg_entity_addr = charg_entity_addr;
    sm_policy_context_data_local_var->gpsi = gpsi;
    sm_policy_context_data_local_var->supi = supi;
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
    sm_policy_context_data_local_var->num_of_pack_filter = num_of_pack_filter;
    sm_policy_context_data_local_var->online = online;
    sm_policy_context_data_local_var->offline = offline;
    sm_policy_context_data_local_var->_3gpp_ps_data_off_status = _3gpp_ps_data_off_status;
    sm_policy_context_data_local_var->ref_qos_indication = ref_qos_indication;
    sm_policy_context_data_local_var->trace_req = trace_req;
    sm_policy_context_data_local_var->slice_info = slice_info;
    sm_policy_context_data_local_var->qos_flow_usage = qos_flow_usage;
    sm_policy_context_data_local_var->serv_nf_id = serv_nf_id;
    sm_policy_context_data_local_var->supp_feat = supp_feat;
    sm_policy_context_data_local_var->smf_id = smf_id;
    sm_policy_context_data_local_var->recovery_time = recovery_time;
    sm_policy_context_data_local_var->ma_pdu_ind = ma_pdu_ind;
    sm_policy_context_data_local_var->atsss_capab = atsss_capab;

    return sm_policy_context_data_local_var;
}

void OpenAPI_sm_policy_context_data_free(OpenAPI_sm_policy_context_data_t *sm_policy_context_data)
{
    if (NULL == sm_policy_context_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_acc_net_ch_id_free(sm_policy_context_data->acc_net_ch_id);
    OpenAPI_acc_net_charging_address_free(sm_policy_context_data->charg_entity_addr);
    ogs_free(sm_policy_context_data->gpsi);
    ogs_free(sm_policy_context_data->supi);
    OpenAPI_list_for_each(sm_policy_context_data->inter_grp_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sm_policy_context_data->inter_grp_ids);
    ogs_free(sm_policy_context_data->chargingcharacteristics);
    ogs_free(sm_policy_context_data->dnn);
    ogs_free(sm_policy_context_data->notification_uri);
    OpenAPI_additional_access_info_free(sm_policy_context_data->add_access_info);
    OpenAPI_plmn_id_nid_free(sm_policy_context_data->serving_network);
    OpenAPI_user_location_free(sm_policy_context_data->user_location_info);
    ogs_free(sm_policy_context_data->ue_time_zone);
    ogs_free(sm_policy_context_data->pei);
    ogs_free(sm_policy_context_data->ipv4_address);
    ogs_free(sm_policy_context_data->ipv6_address_prefix);
    ogs_free(sm_policy_context_data->ip_domain);
    OpenAPI_ambr_free(sm_policy_context_data->subs_sess_ambr);
    ogs_free(sm_policy_context_data->auth_prof_index);
    OpenAPI_subscribed_default_qos_free(sm_policy_context_data->subs_def_qos);
    OpenAPI_trace_data_free(sm_policy_context_data->trace_req);
    OpenAPI_snssai_free(sm_policy_context_data->slice_info);
    OpenAPI_qos_flow_usage_free(sm_policy_context_data->qos_flow_usage);
    OpenAPI_serving_nf_identity_free(sm_policy_context_data->serv_nf_id);
    ogs_free(sm_policy_context_data->supp_feat);
    ogs_free(sm_policy_context_data->smf_id);
    ogs_free(sm_policy_context_data->recovery_time);
    OpenAPI_ma_pdu_indication_free(sm_policy_context_data->ma_pdu_ind);
    OpenAPI_atsss_capability_free(sm_policy_context_data->atsss_capab);
    ogs_free(sm_policy_context_data);
}

cJSON *OpenAPI_sm_policy_context_data_convertToJSON(OpenAPI_sm_policy_context_data_t *sm_policy_context_data)
{
    cJSON *item = NULL;

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
        goto end;
    }
    if (cJSON_AddStringToObject(item, "supi", sm_policy_context_data->supi) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [supi]");
        goto end;
    }

    if (sm_policy_context_data->inter_grp_ids) {
        cJSON *inter_grp_ids = cJSON_AddArrayToObject(item, "interGrpIds");
        if (inter_grp_ids == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [inter_grp_ids]");
            goto end;
        }

        OpenAPI_lnode_t *inter_grp_ids_node;
        OpenAPI_list_for_each(sm_policy_context_data->inter_grp_ids, inter_grp_ids_node)  {
            if (cJSON_AddStringToObject(inter_grp_ids, "", (char*)inter_grp_ids_node->data) == NULL) {
                ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [inter_grp_ids]");
                goto end;
            }
        }
    }

    if (!sm_policy_context_data->pdu_session_id) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [pdu_session_id]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "pduSessionId", sm_policy_context_data->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [pdu_session_id]");
        goto end;
    }

    if (!sm_policy_context_data->pdu_session_type) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [pdu_session_type]");
        goto end;
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
        goto end;
    }
    if (cJSON_AddStringToObject(item, "dnn", sm_policy_context_data->dnn) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [dnn]");
        goto end;
    }

    if (sm_policy_context_data->dnn_sel_mode) {
        if (cJSON_AddStringToObject(item, "dnnSelMode", OpenAPI_dnn_selection_mode_ToString(sm_policy_context_data->dnn_sel_mode)) == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [dnn_sel_mode]");
            goto end;
        }
    }

    if (!sm_policy_context_data->notification_uri) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [notification_uri]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "notificationUri", sm_policy_context_data->notification_uri) == NULL) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [notification_uri]");
        goto end;
    }

    if (sm_policy_context_data->access_type) {
        if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(sm_policy_context_data->access_type)) == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [access_type]");
            goto end;
        }
    }

    if (sm_policy_context_data->rat_type) {
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

    if (sm_policy_context_data->num_of_pack_filter) {
        if (cJSON_AddNumberToObject(item, "numOfPackFilter", sm_policy_context_data->num_of_pack_filter) == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [num_of_pack_filter]");
            goto end;
        }
    }

    if (sm_policy_context_data->online) {
        if (cJSON_AddBoolToObject(item, "online", sm_policy_context_data->online) == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [online]");
            goto end;
        }
    }

    if (sm_policy_context_data->offline) {
        if (cJSON_AddBoolToObject(item, "offline", sm_policy_context_data->offline) == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [offline]");
            goto end;
        }
    }

    if (sm_policy_context_data->_3gpp_ps_data_off_status) {
        if (cJSON_AddBoolToObject(item, "3gppPsDataOffStatus", sm_policy_context_data->_3gpp_ps_data_off_status) == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [_3gpp_ps_data_off_status]");
            goto end;
        }
    }

    if (sm_policy_context_data->ref_qos_indication) {
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
    }

    if (!sm_policy_context_data->slice_info) {
        ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [slice_info]");
        goto end;
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

    if (sm_policy_context_data->qos_flow_usage) {
        cJSON *qos_flow_usage_local_JSON = OpenAPI_qos_flow_usage_convertToJSON(sm_policy_context_data->qos_flow_usage);
        if (qos_flow_usage_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [qos_flow_usage]");
            goto end;
        }
        cJSON_AddItemToObject(item, "qosFlowUsage", qos_flow_usage_local_JSON);
        if (item->child == NULL) {
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

    if (sm_policy_context_data->ma_pdu_ind) {
        cJSON *ma_pdu_ind_local_JSON = OpenAPI_ma_pdu_indication_convertToJSON(sm_policy_context_data->ma_pdu_ind);
        if (ma_pdu_ind_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [ma_pdu_ind]");
            goto end;
        }
        cJSON_AddItemToObject(item, "maPduInd", ma_pdu_ind_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [ma_pdu_ind]");
            goto end;
        }
    }

    if (sm_policy_context_data->atsss_capab) {
        cJSON *atsss_capab_local_JSON = OpenAPI_atsss_capability_convertToJSON(sm_policy_context_data->atsss_capab);
        if (atsss_capab_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [atsss_capab]");
            goto end;
        }
        cJSON_AddItemToObject(item, "atsssCapab", atsss_capab_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sm_policy_context_data_convertToJSON() failed [atsss_capab]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_sm_policy_context_data_t *OpenAPI_sm_policy_context_data_parseFromJSON(cJSON *sm_policy_context_dataJSON)
{
    OpenAPI_sm_policy_context_data_t *sm_policy_context_data_local_var = NULL;
    cJSON *acc_net_ch_id = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "accNetChId");

    OpenAPI_acc_net_ch_id_t *acc_net_ch_id_local_nonprim = NULL;
    if (acc_net_ch_id) {
        acc_net_ch_id_local_nonprim = OpenAPI_acc_net_ch_id_parseFromJSON(acc_net_ch_id);
    }

    cJSON *charg_entity_addr = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "chargEntityAddr");

    OpenAPI_acc_net_charging_address_t *charg_entity_addr_local_nonprim = NULL;
    if (charg_entity_addr) {
        charg_entity_addr_local_nonprim = OpenAPI_acc_net_charging_address_parseFromJSON(charg_entity_addr);
    }

    cJSON *gpsi = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "gpsi");

    if (gpsi) {
        if (!cJSON_IsString(gpsi)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [gpsi]");
            goto end;
        }
    }

    cJSON *supi = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [supi]");
        goto end;
    }


    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [supi]");
        goto end;
    }

    cJSON *inter_grp_ids = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "interGrpIds");

    OpenAPI_list_t *inter_grp_idsList;
    if (inter_grp_ids) {
        cJSON *inter_grp_ids_local;
        if (!cJSON_IsArray(inter_grp_ids)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [inter_grp_ids]");
            goto end;
        }
        inter_grp_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(inter_grp_ids_local, inter_grp_ids) {
            if (!cJSON_IsString(inter_grp_ids_local)) {
                ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [inter_grp_ids]");
                goto end;
            }
            OpenAPI_list_add(inter_grp_idsList, ogs_strdup(inter_grp_ids_local->valuestring));
        }
    }

    cJSON *pdu_session_id = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "pduSessionId");
    if (!pdu_session_id) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }


    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    cJSON *pdu_session_type = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "pduSessionType");
    if (!pdu_session_type) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [pdu_session_type]");
        goto end;
    }

    OpenAPI_pdu_session_type_e pdu_session_typeVariable;

    if (!cJSON_IsString(pdu_session_type)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [pdu_session_type]");
        goto end;
    }
    pdu_session_typeVariable = OpenAPI_pdu_session_type_FromString(pdu_session_type->valuestring);

    cJSON *chargingcharacteristics = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "chargingcharacteristics");

    if (chargingcharacteristics) {
        if (!cJSON_IsString(chargingcharacteristics)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [chargingcharacteristics]");
            goto end;
        }
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [dnn]");
        goto end;
    }


    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *dnn_sel_mode = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "dnnSelMode");

    OpenAPI_dnn_selection_mode_e dnn_sel_modeVariable;
    if (dnn_sel_mode) {
        if (!cJSON_IsString(dnn_sel_mode)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [dnn_sel_mode]");
            goto end;
        }
        dnn_sel_modeVariable = OpenAPI_dnn_selection_mode_FromString(dnn_sel_mode->valuestring);
    }

    cJSON *notification_uri = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "notificationUri");
    if (!notification_uri) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [notification_uri]");
        goto end;
    }


    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    cJSON *access_type = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "accessType");

    OpenAPI_access_type_e access_typeVariable;
    if (access_type) {
        if (!cJSON_IsString(access_type)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [access_type]");
            goto end;
        }
        access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    cJSON *rat_type = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "ratType");

    OpenAPI_rat_type_e rat_typeVariable;
    if (rat_type) {
        if (!cJSON_IsString(rat_type)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [rat_type]");
            goto end;
        }
        rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    cJSON *add_access_info = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "addAccessInfo");

    OpenAPI_additional_access_info_t *add_access_info_local_nonprim = NULL;
    if (add_access_info) {
        add_access_info_local_nonprim = OpenAPI_additional_access_info_parseFromJSON(add_access_info);
    }

    cJSON *serving_network = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "servingNetwork");

    OpenAPI_plmn_id_nid_t *serving_network_local_nonprim = NULL;
    if (serving_network) {
        serving_network_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(serving_network);
    }

    cJSON *user_location_info = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "userLocationInfo");

    OpenAPI_user_location_t *user_location_info_local_nonprim = NULL;
    if (user_location_info) {
        user_location_info_local_nonprim = OpenAPI_user_location_parseFromJSON(user_location_info);
    }

    cJSON *ue_time_zone = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "ueTimeZone");

    if (ue_time_zone) {
        if (!cJSON_IsString(ue_time_zone)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ue_time_zone]");
            goto end;
        }
    }

    cJSON *pei = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "pei");

    if (pei) {
        if (!cJSON_IsString(pei)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [pei]");
            goto end;
        }
    }

    cJSON *ipv4_address = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "ipv4Address");

    if (ipv4_address) {
        if (!cJSON_IsString(ipv4_address)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ipv4_address]");
            goto end;
        }
    }

    cJSON *ipv6_address_prefix = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "ipv6AddressPrefix");

    if (ipv6_address_prefix) {
        if (!cJSON_IsString(ipv6_address_prefix)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ipv6_address_prefix]");
            goto end;
        }
    }

    cJSON *ip_domain = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "ipDomain");

    if (ip_domain) {
        if (!cJSON_IsString(ip_domain)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ip_domain]");
            goto end;
        }
    }

    cJSON *subs_sess_ambr = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "subsSessAmbr");

    OpenAPI_ambr_t *subs_sess_ambr_local_nonprim = NULL;
    if (subs_sess_ambr) {
        subs_sess_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(subs_sess_ambr);
    }

    cJSON *auth_prof_index = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "authProfIndex");

    if (auth_prof_index) {
        if (!cJSON_IsString(auth_prof_index)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [auth_prof_index]");
            goto end;
        }
    }

    cJSON *subs_def_qos = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "subsDefQos");

    OpenAPI_subscribed_default_qos_t *subs_def_qos_local_nonprim = NULL;
    if (subs_def_qos) {
        subs_def_qos_local_nonprim = OpenAPI_subscribed_default_qos_parseFromJSON(subs_def_qos);
    }

    cJSON *num_of_pack_filter = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "numOfPackFilter");

    if (num_of_pack_filter) {
        if (!cJSON_IsNumber(num_of_pack_filter)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [num_of_pack_filter]");
            goto end;
        }
    }

    cJSON *online = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "online");

    if (online) {
        if (!cJSON_IsBool(online)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [online]");
            goto end;
        }
    }

    cJSON *offline = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "offline");

    if (offline) {
        if (!cJSON_IsBool(offline)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [offline]");
            goto end;
        }
    }

    cJSON *_3gpp_ps_data_off_status = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "3gppPsDataOffStatus");

    if (_3gpp_ps_data_off_status) {
        if (!cJSON_IsBool(_3gpp_ps_data_off_status)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [_3gpp_ps_data_off_status]");
            goto end;
        }
    }

    cJSON *ref_qos_indication = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "refQosIndication");

    if (ref_qos_indication) {
        if (!cJSON_IsBool(ref_qos_indication)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [ref_qos_indication]");
            goto end;
        }
    }

    cJSON *trace_req = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "traceReq");

    OpenAPI_trace_data_t *trace_req_local_nonprim = NULL;
    if (trace_req) {
        trace_req_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_req);
    }

    cJSON *slice_info = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "sliceInfo");
    if (!slice_info) {
        ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [slice_info]");
        goto end;
    }

    OpenAPI_snssai_t *slice_info_local_nonprim = NULL;

    slice_info_local_nonprim = OpenAPI_snssai_parseFromJSON(slice_info);

    cJSON *qos_flow_usage = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "qosFlowUsage");

    OpenAPI_qos_flow_usage_t *qos_flow_usage_local_nonprim = NULL;
    if (qos_flow_usage) {
        qos_flow_usage_local_nonprim = OpenAPI_qos_flow_usage_parseFromJSON(qos_flow_usage);
    }

    cJSON *serv_nf_id = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "servNfId");

    OpenAPI_serving_nf_identity_t *serv_nf_id_local_nonprim = NULL;
    if (serv_nf_id) {
        serv_nf_id_local_nonprim = OpenAPI_serving_nf_identity_parseFromJSON(serv_nf_id);
    }

    cJSON *supp_feat = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "suppFeat");

    if (supp_feat) {
        if (!cJSON_IsString(supp_feat)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [supp_feat]");
            goto end;
        }
    }

    cJSON *smf_id = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "smfId");

    if (smf_id) {
        if (!cJSON_IsString(smf_id)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [smf_id]");
            goto end;
        }
    }

    cJSON *recovery_time = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "recoveryTime");

    if (recovery_time) {
        if (!cJSON_IsString(recovery_time)) {
            ogs_error("OpenAPI_sm_policy_context_data_parseFromJSON() failed [recovery_time]");
            goto end;
        }
    }

    cJSON *ma_pdu_ind = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "maPduInd");

    OpenAPI_ma_pdu_indication_t *ma_pdu_ind_local_nonprim = NULL;
    if (ma_pdu_ind) {
        ma_pdu_ind_local_nonprim = OpenAPI_ma_pdu_indication_parseFromJSON(ma_pdu_ind);
    }

    cJSON *atsss_capab = cJSON_GetObjectItemCaseSensitive(sm_policy_context_dataJSON, "atsssCapab");

    OpenAPI_atsss_capability_t *atsss_capab_local_nonprim = NULL;
    if (atsss_capab) {
        atsss_capab_local_nonprim = OpenAPI_atsss_capability_parseFromJSON(atsss_capab);
    }

    sm_policy_context_data_local_var = OpenAPI_sm_policy_context_data_create (
        acc_net_ch_id ? acc_net_ch_id_local_nonprim : NULL,
        charg_entity_addr ? charg_entity_addr_local_nonprim : NULL,
        gpsi ? ogs_strdup(gpsi->valuestring) : NULL,
        ogs_strdup(supi->valuestring),
        inter_grp_ids ? inter_grp_idsList : NULL,
        pdu_session_id->valuedouble,
        pdu_session_typeVariable,
        chargingcharacteristics ? ogs_strdup(chargingcharacteristics->valuestring) : NULL,
        ogs_strdup(dnn->valuestring),
        dnn_sel_mode ? dnn_sel_modeVariable : 0,
        ogs_strdup(notification_uri->valuestring),
        access_type ? access_typeVariable : 0,
        rat_type ? rat_typeVariable : 0,
        add_access_info ? add_access_info_local_nonprim : NULL,
        serving_network ? serving_network_local_nonprim : NULL,
        user_location_info ? user_location_info_local_nonprim : NULL,
        ue_time_zone ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        pei ? ogs_strdup(pei->valuestring) : NULL,
        ipv4_address ? ogs_strdup(ipv4_address->valuestring) : NULL,
        ipv6_address_prefix ? ogs_strdup(ipv6_address_prefix->valuestring) : NULL,
        ip_domain ? ogs_strdup(ip_domain->valuestring) : NULL,
        subs_sess_ambr ? subs_sess_ambr_local_nonprim : NULL,
        auth_prof_index ? ogs_strdup(auth_prof_index->valuestring) : NULL,
        subs_def_qos ? subs_def_qos_local_nonprim : NULL,
        num_of_pack_filter ? num_of_pack_filter->valuedouble : 0,
        online ? online->valueint : 0,
        offline ? offline->valueint : 0,
        _3gpp_ps_data_off_status ? _3gpp_ps_data_off_status->valueint : 0,
        ref_qos_indication ? ref_qos_indication->valueint : 0,
        trace_req ? trace_req_local_nonprim : NULL,
        slice_info_local_nonprim,
        qos_flow_usage ? qos_flow_usage_local_nonprim : NULL,
        serv_nf_id ? serv_nf_id_local_nonprim : NULL,
        supp_feat ? ogs_strdup(supp_feat->valuestring) : NULL,
        smf_id ? ogs_strdup(smf_id->valuestring) : NULL,
        recovery_time ? ogs_strdup(recovery_time->valuestring) : NULL,
        ma_pdu_ind ? ma_pdu_ind_local_nonprim : NULL,
        atsss_capab ? atsss_capab_local_nonprim : NULL
        );

    return sm_policy_context_data_local_var;
end:
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

