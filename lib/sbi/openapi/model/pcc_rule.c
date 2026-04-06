
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcc_rule.h"

OpenAPI_pcc_rule_t *OpenAPI_pcc_rule_create(
    OpenAPI_list_t *flow_infos,
    char *app_id,
    char *app_descriptor,
    bool is_cont_ver,
    int cont_ver,
    OpenAPI_protocol_description_t *proto_desc_dl,
    OpenAPI_protocol_description_t *proto_desc_ul,
    char *pcc_rule_id,
    bool is_precedence,
    int precedence,
    OpenAPI_af_sig_protocol_e af_sig_protocol,
    bool is_app_reloc,
    int app_reloc,
    bool is_eas_redis_ind,
    int eas_redis_ind,
    OpenAPI_list_t *ref_qos_data,
    OpenAPI_list_t *ref_alt_qos_params,
    OpenAPI_list_t *ref_tc_data,
    bool is_ref_chg_data_null,
    OpenAPI_list_t *ref_chg_data,
    bool is_ref_chg_n3g_data_null,
    OpenAPI_list_t *ref_chg_n3g_data,
    bool is_ref_um_data_null,
    OpenAPI_list_t *ref_um_data,
    bool is_ref_um_n3g_data_null,
    OpenAPI_list_t *ref_um_n3g_data,
    bool is_ref_cond_data_null,
    char *ref_cond_data,
    bool is_ref_qos_mon_null,
    OpenAPI_list_t *ref_qos_mon,
    bool is_addr_preser_ind_null,
    bool is_addr_preser_ind,
    int addr_preser_ind,
    bool is_tscai_input_dl_null,
    OpenAPI_tscai_input_container_t *tscai_input_dl,
    bool is_tscai_input_ul_null,
    OpenAPI_tscai_input_container_t *tscai_input_ul,
    bool is_tscai_time_dom,
    int tscai_time_dom,
    bool is_cap_bat_adaptation,
    int cap_bat_adaptation,
    OpenAPI_downlink_data_notification_control_t *dd_notif_ctrl,
    bool is_dd_notif_ctrl2_null,
    OpenAPI_downlink_data_notification_control_rm_t *dd_notif_ctrl2,
    bool is_dis_ue_notif_null,
    bool is_dis_ue_notif,
    int dis_ue_notif,
    bool is_pack_filt_all_prec,
    int pack_filt_all_prec,
    OpenAPI_list_t* nsc_supp_feats,
    bool is_call_info_null,
    OpenAPI_call_info_t *call_info,
    OpenAPI_traffic_para_data_t *traff_para_data,
    char *multi_modal_id,
    bool is_exp_tran_ind_null,
    bool is_exp_tran_ind,
    int exp_tran_ind,
    bool is_ul_br_rec_ind_null,
    bool is_ul_br_rec_ind,
    int ul_br_rec_ind
)
{
    OpenAPI_pcc_rule_t *pcc_rule_local_var = ogs_malloc(sizeof(OpenAPI_pcc_rule_t));
    ogs_assert(pcc_rule_local_var);

    pcc_rule_local_var->flow_infos = flow_infos;
    pcc_rule_local_var->app_id = app_id;
    pcc_rule_local_var->app_descriptor = app_descriptor;
    pcc_rule_local_var->is_cont_ver = is_cont_ver;
    pcc_rule_local_var->cont_ver = cont_ver;
    pcc_rule_local_var->proto_desc_dl = proto_desc_dl;
    pcc_rule_local_var->proto_desc_ul = proto_desc_ul;
    pcc_rule_local_var->pcc_rule_id = pcc_rule_id;
    pcc_rule_local_var->is_precedence = is_precedence;
    pcc_rule_local_var->precedence = precedence;
    pcc_rule_local_var->af_sig_protocol = af_sig_protocol;
    pcc_rule_local_var->is_app_reloc = is_app_reloc;
    pcc_rule_local_var->app_reloc = app_reloc;
    pcc_rule_local_var->is_eas_redis_ind = is_eas_redis_ind;
    pcc_rule_local_var->eas_redis_ind = eas_redis_ind;
    pcc_rule_local_var->ref_qos_data = ref_qos_data;
    pcc_rule_local_var->ref_alt_qos_params = ref_alt_qos_params;
    pcc_rule_local_var->ref_tc_data = ref_tc_data;
    pcc_rule_local_var->is_ref_chg_data_null = is_ref_chg_data_null;
    pcc_rule_local_var->ref_chg_data = ref_chg_data;
    pcc_rule_local_var->is_ref_chg_n3g_data_null = is_ref_chg_n3g_data_null;
    pcc_rule_local_var->ref_chg_n3g_data = ref_chg_n3g_data;
    pcc_rule_local_var->is_ref_um_data_null = is_ref_um_data_null;
    pcc_rule_local_var->ref_um_data = ref_um_data;
    pcc_rule_local_var->is_ref_um_n3g_data_null = is_ref_um_n3g_data_null;
    pcc_rule_local_var->ref_um_n3g_data = ref_um_n3g_data;
    pcc_rule_local_var->is_ref_cond_data_null = is_ref_cond_data_null;
    pcc_rule_local_var->ref_cond_data = ref_cond_data;
    pcc_rule_local_var->is_ref_qos_mon_null = is_ref_qos_mon_null;
    pcc_rule_local_var->ref_qos_mon = ref_qos_mon;
    pcc_rule_local_var->is_addr_preser_ind_null = is_addr_preser_ind_null;
    pcc_rule_local_var->is_addr_preser_ind = is_addr_preser_ind;
    pcc_rule_local_var->addr_preser_ind = addr_preser_ind;
    pcc_rule_local_var->is_tscai_input_dl_null = is_tscai_input_dl_null;
    pcc_rule_local_var->tscai_input_dl = tscai_input_dl;
    pcc_rule_local_var->is_tscai_input_ul_null = is_tscai_input_ul_null;
    pcc_rule_local_var->tscai_input_ul = tscai_input_ul;
    pcc_rule_local_var->is_tscai_time_dom = is_tscai_time_dom;
    pcc_rule_local_var->tscai_time_dom = tscai_time_dom;
    pcc_rule_local_var->is_cap_bat_adaptation = is_cap_bat_adaptation;
    pcc_rule_local_var->cap_bat_adaptation = cap_bat_adaptation;
    pcc_rule_local_var->dd_notif_ctrl = dd_notif_ctrl;
    pcc_rule_local_var->is_dd_notif_ctrl2_null = is_dd_notif_ctrl2_null;
    pcc_rule_local_var->dd_notif_ctrl2 = dd_notif_ctrl2;
    pcc_rule_local_var->is_dis_ue_notif_null = is_dis_ue_notif_null;
    pcc_rule_local_var->is_dis_ue_notif = is_dis_ue_notif;
    pcc_rule_local_var->dis_ue_notif = dis_ue_notif;
    pcc_rule_local_var->is_pack_filt_all_prec = is_pack_filt_all_prec;
    pcc_rule_local_var->pack_filt_all_prec = pack_filt_all_prec;
    pcc_rule_local_var->nsc_supp_feats = nsc_supp_feats;
    pcc_rule_local_var->is_call_info_null = is_call_info_null;
    pcc_rule_local_var->call_info = call_info;
    pcc_rule_local_var->traff_para_data = traff_para_data;
    pcc_rule_local_var->multi_modal_id = multi_modal_id;
    pcc_rule_local_var->is_exp_tran_ind_null = is_exp_tran_ind_null;
    pcc_rule_local_var->is_exp_tran_ind = is_exp_tran_ind;
    pcc_rule_local_var->exp_tran_ind = exp_tran_ind;
    pcc_rule_local_var->is_ul_br_rec_ind_null = is_ul_br_rec_ind_null;
    pcc_rule_local_var->is_ul_br_rec_ind = is_ul_br_rec_ind;
    pcc_rule_local_var->ul_br_rec_ind = ul_br_rec_ind;

    return pcc_rule_local_var;
}

void OpenAPI_pcc_rule_free(OpenAPI_pcc_rule_t *pcc_rule)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcc_rule) {
        return;
    }
    if (pcc_rule->flow_infos) {
        OpenAPI_list_for_each(pcc_rule->flow_infos, node) {
            OpenAPI_flow_information_free(node->data);
        }
        OpenAPI_list_free(pcc_rule->flow_infos);
        pcc_rule->flow_infos = NULL;
    }
    if (pcc_rule->app_id) {
        ogs_free(pcc_rule->app_id);
        pcc_rule->app_id = NULL;
    }
    if (pcc_rule->app_descriptor) {
        ogs_free(pcc_rule->app_descriptor);
        pcc_rule->app_descriptor = NULL;
    }
    if (pcc_rule->proto_desc_dl) {
        OpenAPI_protocol_description_free(pcc_rule->proto_desc_dl);
        pcc_rule->proto_desc_dl = NULL;
    }
    if (pcc_rule->proto_desc_ul) {
        OpenAPI_protocol_description_free(pcc_rule->proto_desc_ul);
        pcc_rule->proto_desc_ul = NULL;
    }
    if (pcc_rule->pcc_rule_id) {
        ogs_free(pcc_rule->pcc_rule_id);
        pcc_rule->pcc_rule_id = NULL;
    }
    if (pcc_rule->ref_qos_data) {
        OpenAPI_list_for_each(pcc_rule->ref_qos_data, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcc_rule->ref_qos_data);
        pcc_rule->ref_qos_data = NULL;
    }
    if (pcc_rule->ref_alt_qos_params) {
        OpenAPI_list_for_each(pcc_rule->ref_alt_qos_params, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcc_rule->ref_alt_qos_params);
        pcc_rule->ref_alt_qos_params = NULL;
    }
    if (pcc_rule->ref_tc_data) {
        OpenAPI_list_for_each(pcc_rule->ref_tc_data, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcc_rule->ref_tc_data);
        pcc_rule->ref_tc_data = NULL;
    }
    if (pcc_rule->ref_chg_data) {
        OpenAPI_list_for_each(pcc_rule->ref_chg_data, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcc_rule->ref_chg_data);
        pcc_rule->ref_chg_data = NULL;
    }
    if (pcc_rule->ref_chg_n3g_data) {
        OpenAPI_list_for_each(pcc_rule->ref_chg_n3g_data, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcc_rule->ref_chg_n3g_data);
        pcc_rule->ref_chg_n3g_data = NULL;
    }
    if (pcc_rule->ref_um_data) {
        OpenAPI_list_for_each(pcc_rule->ref_um_data, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcc_rule->ref_um_data);
        pcc_rule->ref_um_data = NULL;
    }
    if (pcc_rule->ref_um_n3g_data) {
        OpenAPI_list_for_each(pcc_rule->ref_um_n3g_data, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcc_rule->ref_um_n3g_data);
        pcc_rule->ref_um_n3g_data = NULL;
    }
    if (pcc_rule->ref_cond_data) {
        ogs_free(pcc_rule->ref_cond_data);
        pcc_rule->ref_cond_data = NULL;
    }
    if (pcc_rule->ref_qos_mon) {
        OpenAPI_list_for_each(pcc_rule->ref_qos_mon, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcc_rule->ref_qos_mon);
        pcc_rule->ref_qos_mon = NULL;
    }
    if (pcc_rule->tscai_input_dl) {
        OpenAPI_tscai_input_container_free(pcc_rule->tscai_input_dl);
        pcc_rule->tscai_input_dl = NULL;
    }
    if (pcc_rule->tscai_input_ul) {
        OpenAPI_tscai_input_container_free(pcc_rule->tscai_input_ul);
        pcc_rule->tscai_input_ul = NULL;
    }
    if (pcc_rule->dd_notif_ctrl) {
        OpenAPI_downlink_data_notification_control_free(pcc_rule->dd_notif_ctrl);
        pcc_rule->dd_notif_ctrl = NULL;
    }
    if (pcc_rule->dd_notif_ctrl2) {
        OpenAPI_downlink_data_notification_control_rm_free(pcc_rule->dd_notif_ctrl2);
        pcc_rule->dd_notif_ctrl2 = NULL;
    }
    if (pcc_rule->nsc_supp_feats) {
        OpenAPI_list_for_each(pcc_rule->nsc_supp_feats, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pcc_rule->nsc_supp_feats);
        pcc_rule->nsc_supp_feats = NULL;
    }
    if (pcc_rule->call_info) {
        OpenAPI_call_info_free(pcc_rule->call_info);
        pcc_rule->call_info = NULL;
    }
    if (pcc_rule->traff_para_data) {
        OpenAPI_traffic_para_data_free(pcc_rule->traff_para_data);
        pcc_rule->traff_para_data = NULL;
    }
    if (pcc_rule->multi_modal_id) {
        ogs_free(pcc_rule->multi_modal_id);
        pcc_rule->multi_modal_id = NULL;
    }
    ogs_free(pcc_rule);
}

cJSON *OpenAPI_pcc_rule_convertToJSON(OpenAPI_pcc_rule_t *pcc_rule)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcc_rule == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [PccRule]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pcc_rule->flow_infos) {
    cJSON *flow_infosList = cJSON_AddArrayToObject(item, "flowInfos");
    if (flow_infosList == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [flow_infos]");
        goto end;
    }
    OpenAPI_list_for_each(pcc_rule->flow_infos, node) {
        cJSON *itemLocal = OpenAPI_flow_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [flow_infos]");
            goto end;
        }
        cJSON_AddItemToArray(flow_infosList, itemLocal);
    }
    }

    if (pcc_rule->app_id) {
    if (cJSON_AddStringToObject(item, "appId", pcc_rule->app_id) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [app_id]");
        goto end;
    }
    }

    if (pcc_rule->app_descriptor) {
    if (cJSON_AddStringToObject(item, "appDescriptor", pcc_rule->app_descriptor) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [app_descriptor]");
        goto end;
    }
    }

    if (pcc_rule->is_cont_ver) {
    if (cJSON_AddNumberToObject(item, "contVer", pcc_rule->cont_ver) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [cont_ver]");
        goto end;
    }
    }

    if (pcc_rule->proto_desc_dl) {
    cJSON *proto_desc_dl_local_JSON = OpenAPI_protocol_description_convertToJSON(pcc_rule->proto_desc_dl);
    if (proto_desc_dl_local_JSON == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [proto_desc_dl]");
        goto end;
    }
    cJSON_AddItemToObject(item, "protoDescDl", proto_desc_dl_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [proto_desc_dl]");
        goto end;
    }
    }

    if (pcc_rule->proto_desc_ul) {
    cJSON *proto_desc_ul_local_JSON = OpenAPI_protocol_description_convertToJSON(pcc_rule->proto_desc_ul);
    if (proto_desc_ul_local_JSON == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [proto_desc_ul]");
        goto end;
    }
    cJSON_AddItemToObject(item, "protoDescUl", proto_desc_ul_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [proto_desc_ul]");
        goto end;
    }
    }

    if (!pcc_rule->pcc_rule_id) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [pcc_rule_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "pccRuleId", pcc_rule->pcc_rule_id) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [pcc_rule_id]");
        goto end;
    }

    if (pcc_rule->is_precedence) {
    if (cJSON_AddNumberToObject(item, "precedence", pcc_rule->precedence) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [precedence]");
        goto end;
    }
    }

    if (pcc_rule->af_sig_protocol != OpenAPI_af_sig_protocol_NULL) {
    if (cJSON_AddStringToObject(item, "afSigProtocol", OpenAPI_af_sig_protocol_ToString(pcc_rule->af_sig_protocol)) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [af_sig_protocol]");
        goto end;
    }
    }

    if (pcc_rule->is_app_reloc) {
    if (cJSON_AddBoolToObject(item, "appReloc", pcc_rule->app_reloc) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [app_reloc]");
        goto end;
    }
    }

    if (pcc_rule->is_eas_redis_ind) {
    if (cJSON_AddBoolToObject(item, "easRedisInd", pcc_rule->eas_redis_ind) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [eas_redis_ind]");
        goto end;
    }
    }

    if (pcc_rule->ref_qos_data) {
    cJSON *ref_qos_dataList = cJSON_AddArrayToObject(item, "refQosData");
    if (ref_qos_dataList == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_qos_data]");
        goto end;
    }
    OpenAPI_list_for_each(pcc_rule->ref_qos_data, node) {
        if (cJSON_AddStringToObject(ref_qos_dataList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_qos_data]");
            goto end;
        }
    }
    }

    if (pcc_rule->ref_alt_qos_params) {
    cJSON *ref_alt_qos_paramsList = cJSON_AddArrayToObject(item, "refAltQosParams");
    if (ref_alt_qos_paramsList == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_alt_qos_params]");
        goto end;
    }
    OpenAPI_list_for_each(pcc_rule->ref_alt_qos_params, node) {
        if (cJSON_AddStringToObject(ref_alt_qos_paramsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_alt_qos_params]");
            goto end;
        }
    }
    }

    if (pcc_rule->ref_tc_data) {
    cJSON *ref_tc_dataList = cJSON_AddArrayToObject(item, "refTcData");
    if (ref_tc_dataList == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_tc_data]");
        goto end;
    }
    OpenAPI_list_for_each(pcc_rule->ref_tc_data, node) {
        if (cJSON_AddStringToObject(ref_tc_dataList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_tc_data]");
            goto end;
        }
    }
    }

    if (pcc_rule->ref_chg_data) {
    cJSON *ref_chg_dataList = cJSON_AddArrayToObject(item, "refChgData");
    if (ref_chg_dataList == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_chg_data]");
        goto end;
    }
    OpenAPI_list_for_each(pcc_rule->ref_chg_data, node) {
        if (cJSON_AddStringToObject(ref_chg_dataList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_chg_data]");
            goto end;
        }
    }
    } else if (pcc_rule->is_ref_chg_data_null) {
        if (cJSON_AddNullToObject(item, "refChgData") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_chg_data]");
            goto end;
        }
    }

    if (pcc_rule->ref_chg_n3g_data) {
    cJSON *ref_chg_n3g_dataList = cJSON_AddArrayToObject(item, "refChgN3gData");
    if (ref_chg_n3g_dataList == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_chg_n3g_data]");
        goto end;
    }
    OpenAPI_list_for_each(pcc_rule->ref_chg_n3g_data, node) {
        if (cJSON_AddStringToObject(ref_chg_n3g_dataList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_chg_n3g_data]");
            goto end;
        }
    }
    } else if (pcc_rule->is_ref_chg_n3g_data_null) {
        if (cJSON_AddNullToObject(item, "refChgN3gData") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_chg_n3g_data]");
            goto end;
        }
    }

    if (pcc_rule->ref_um_data) {
    cJSON *ref_um_dataList = cJSON_AddArrayToObject(item, "refUmData");
    if (ref_um_dataList == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_um_data]");
        goto end;
    }
    OpenAPI_list_for_each(pcc_rule->ref_um_data, node) {
        if (cJSON_AddStringToObject(ref_um_dataList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_um_data]");
            goto end;
        }
    }
    } else if (pcc_rule->is_ref_um_data_null) {
        if (cJSON_AddNullToObject(item, "refUmData") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_um_data]");
            goto end;
        }
    }

    if (pcc_rule->ref_um_n3g_data) {
    cJSON *ref_um_n3g_dataList = cJSON_AddArrayToObject(item, "refUmN3gData");
    if (ref_um_n3g_dataList == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_um_n3g_data]");
        goto end;
    }
    OpenAPI_list_for_each(pcc_rule->ref_um_n3g_data, node) {
        if (cJSON_AddStringToObject(ref_um_n3g_dataList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_um_n3g_data]");
            goto end;
        }
    }
    } else if (pcc_rule->is_ref_um_n3g_data_null) {
        if (cJSON_AddNullToObject(item, "refUmN3gData") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_um_n3g_data]");
            goto end;
        }
    }

    if (pcc_rule->ref_cond_data) {
    if (cJSON_AddStringToObject(item, "refCondData", pcc_rule->ref_cond_data) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_cond_data]");
        goto end;
    }
    } else if (pcc_rule->is_ref_cond_data_null) {
        if (cJSON_AddNullToObject(item, "refCondData") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_cond_data]");
            goto end;
        }
    }

    if (pcc_rule->ref_qos_mon) {
    cJSON *ref_qos_monList = cJSON_AddArrayToObject(item, "refQosMon");
    if (ref_qos_monList == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_qos_mon]");
        goto end;
    }
    OpenAPI_list_for_each(pcc_rule->ref_qos_mon, node) {
        if (cJSON_AddStringToObject(ref_qos_monList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_qos_mon]");
            goto end;
        }
    }
    } else if (pcc_rule->is_ref_qos_mon_null) {
        if (cJSON_AddNullToObject(item, "refQosMon") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_qos_mon]");
            goto end;
        }
    }

    if (pcc_rule->is_addr_preser_ind) {
    if (cJSON_AddBoolToObject(item, "addrPreserInd", pcc_rule->addr_preser_ind) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [addr_preser_ind]");
        goto end;
    }
    } else if (pcc_rule->is_addr_preser_ind_null) {
        if (cJSON_AddNullToObject(item, "addrPreserInd") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [addr_preser_ind]");
            goto end;
        }
    }

    if (pcc_rule->tscai_input_dl) {
    cJSON *tscai_input_dl_local_JSON = OpenAPI_tscai_input_container_convertToJSON(pcc_rule->tscai_input_dl);
    if (tscai_input_dl_local_JSON == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [tscai_input_dl]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tscaiInputDl", tscai_input_dl_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [tscai_input_dl]");
        goto end;
    }
    } else if (pcc_rule->is_tscai_input_dl_null) {
        if (cJSON_AddNullToObject(item, "tscaiInputDl") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [tscai_input_dl]");
            goto end;
        }
    }

    if (pcc_rule->tscai_input_ul) {
    cJSON *tscai_input_ul_local_JSON = OpenAPI_tscai_input_container_convertToJSON(pcc_rule->tscai_input_ul);
    if (tscai_input_ul_local_JSON == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [tscai_input_ul]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tscaiInputUl", tscai_input_ul_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [tscai_input_ul]");
        goto end;
    }
    } else if (pcc_rule->is_tscai_input_ul_null) {
        if (cJSON_AddNullToObject(item, "tscaiInputUl") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [tscai_input_ul]");
            goto end;
        }
    }

    if (pcc_rule->is_tscai_time_dom) {
    if (cJSON_AddNumberToObject(item, "tscaiTimeDom", pcc_rule->tscai_time_dom) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [tscai_time_dom]");
        goto end;
    }
    }

    if (pcc_rule->is_cap_bat_adaptation) {
    if (cJSON_AddBoolToObject(item, "capBatAdaptation", pcc_rule->cap_bat_adaptation) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [cap_bat_adaptation]");
        goto end;
    }
    }

    if (pcc_rule->dd_notif_ctrl) {
    cJSON *dd_notif_ctrl_local_JSON = OpenAPI_downlink_data_notification_control_convertToJSON(pcc_rule->dd_notif_ctrl);
    if (dd_notif_ctrl_local_JSON == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [dd_notif_ctrl]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ddNotifCtrl", dd_notif_ctrl_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [dd_notif_ctrl]");
        goto end;
    }
    }

    if (pcc_rule->dd_notif_ctrl2) {
    cJSON *dd_notif_ctrl2_local_JSON = OpenAPI_downlink_data_notification_control_rm_convertToJSON(pcc_rule->dd_notif_ctrl2);
    if (dd_notif_ctrl2_local_JSON == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [dd_notif_ctrl2]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ddNotifCtrl2", dd_notif_ctrl2_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [dd_notif_ctrl2]");
        goto end;
    }
    } else if (pcc_rule->is_dd_notif_ctrl2_null) {
        if (cJSON_AddNullToObject(item, "ddNotifCtrl2") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [dd_notif_ctrl2]");
            goto end;
        }
    }

    if (pcc_rule->is_dis_ue_notif) {
    if (cJSON_AddBoolToObject(item, "disUeNotif", pcc_rule->dis_ue_notif) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [dis_ue_notif]");
        goto end;
    }
    } else if (pcc_rule->is_dis_ue_notif_null) {
        if (cJSON_AddNullToObject(item, "disUeNotif") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [dis_ue_notif]");
            goto end;
        }
    }

    if (pcc_rule->is_pack_filt_all_prec) {
    if (cJSON_AddNumberToObject(item, "packFiltAllPrec", pcc_rule->pack_filt_all_prec) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [pack_filt_all_prec]");
        goto end;
    }
    }

    if (pcc_rule->nsc_supp_feats) {
    cJSON *nsc_supp_feats = cJSON_AddObjectToObject(item, "nscSuppFeats");
    if (nsc_supp_feats == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [nsc_supp_feats]");
        goto end;
    }
    cJSON *localMapObject = nsc_supp_feats;
    if (pcc_rule->nsc_supp_feats) {
        OpenAPI_list_for_each(pcc_rule->nsc_supp_feats, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [nsc_supp_feats]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [nsc_supp_feats]");
                goto end;
            }
            if (cJSON_AddStringToObject(localMapObject, localKeyValue->key, (char*)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

    if (pcc_rule->call_info) {
    cJSON *call_info_local_JSON = OpenAPI_call_info_convertToJSON(pcc_rule->call_info);
    if (call_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [call_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "callInfo", call_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [call_info]");
        goto end;
    }
    } else if (pcc_rule->is_call_info_null) {
        if (cJSON_AddNullToObject(item, "callInfo") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [call_info]");
            goto end;
        }
    }

    if (pcc_rule->traff_para_data) {
    cJSON *traff_para_data_local_JSON = OpenAPI_traffic_para_data_convertToJSON(pcc_rule->traff_para_data);
    if (traff_para_data_local_JSON == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [traff_para_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traffParaData", traff_para_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [traff_para_data]");
        goto end;
    }
    }

    if (pcc_rule->multi_modal_id) {
    if (cJSON_AddStringToObject(item, "multiModalId", pcc_rule->multi_modal_id) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [multi_modal_id]");
        goto end;
    }
    }

    if (pcc_rule->is_exp_tran_ind) {
    if (cJSON_AddBoolToObject(item, "expTranInd", pcc_rule->exp_tran_ind) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [exp_tran_ind]");
        goto end;
    }
    } else if (pcc_rule->is_exp_tran_ind_null) {
        if (cJSON_AddNullToObject(item, "expTranInd") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [exp_tran_ind]");
            goto end;
        }
    }

    if (pcc_rule->is_ul_br_rec_ind) {
    if (cJSON_AddBoolToObject(item, "ulBrRecInd", pcc_rule->ul_br_rec_ind) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ul_br_rec_ind]");
        goto end;
    }
    } else if (pcc_rule->is_ul_br_rec_ind_null) {
        if (cJSON_AddNullToObject(item, "ulBrRecInd") == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ul_br_rec_ind]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_pcc_rule_t *OpenAPI_pcc_rule_parseFromJSON(cJSON *pcc_ruleJSON)
{
    OpenAPI_pcc_rule_t *pcc_rule_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *flow_infos = NULL;
    OpenAPI_list_t *flow_infosList = NULL;
    cJSON *app_id = NULL;
    cJSON *app_descriptor = NULL;
    cJSON *cont_ver = NULL;
    cJSON *proto_desc_dl = NULL;
    OpenAPI_protocol_description_t *proto_desc_dl_local_nonprim = NULL;
    cJSON *proto_desc_ul = NULL;
    OpenAPI_protocol_description_t *proto_desc_ul_local_nonprim = NULL;
    cJSON *pcc_rule_id = NULL;
    cJSON *precedence = NULL;
    cJSON *af_sig_protocol = NULL;
    OpenAPI_af_sig_protocol_e af_sig_protocolVariable = 0;
    cJSON *app_reloc = NULL;
    cJSON *eas_redis_ind = NULL;
    cJSON *ref_qos_data = NULL;
    OpenAPI_list_t *ref_qos_dataList = NULL;
    cJSON *ref_alt_qos_params = NULL;
    OpenAPI_list_t *ref_alt_qos_paramsList = NULL;
    cJSON *ref_tc_data = NULL;
    OpenAPI_list_t *ref_tc_dataList = NULL;
    cJSON *ref_chg_data = NULL;
    OpenAPI_list_t *ref_chg_dataList = NULL;
    cJSON *ref_chg_n3g_data = NULL;
    OpenAPI_list_t *ref_chg_n3g_dataList = NULL;
    cJSON *ref_um_data = NULL;
    OpenAPI_list_t *ref_um_dataList = NULL;
    cJSON *ref_um_n3g_data = NULL;
    OpenAPI_list_t *ref_um_n3g_dataList = NULL;
    cJSON *ref_cond_data = NULL;
    cJSON *ref_qos_mon = NULL;
    OpenAPI_list_t *ref_qos_monList = NULL;
    cJSON *addr_preser_ind = NULL;
    cJSON *tscai_input_dl = NULL;
    OpenAPI_tscai_input_container_t *tscai_input_dl_local_nonprim = NULL;
    cJSON *tscai_input_ul = NULL;
    OpenAPI_tscai_input_container_t *tscai_input_ul_local_nonprim = NULL;
    cJSON *tscai_time_dom = NULL;
    cJSON *cap_bat_adaptation = NULL;
    cJSON *dd_notif_ctrl = NULL;
    OpenAPI_downlink_data_notification_control_t *dd_notif_ctrl_local_nonprim = NULL;
    cJSON *dd_notif_ctrl2 = NULL;
    OpenAPI_downlink_data_notification_control_rm_t *dd_notif_ctrl2_local_nonprim = NULL;
    cJSON *dis_ue_notif = NULL;
    cJSON *pack_filt_all_prec = NULL;
    cJSON *nsc_supp_feats = NULL;
    OpenAPI_list_t *nsc_supp_featsList = NULL;
    cJSON *call_info = NULL;
    OpenAPI_call_info_t *call_info_local_nonprim = NULL;
    cJSON *traff_para_data = NULL;
    OpenAPI_traffic_para_data_t *traff_para_data_local_nonprim = NULL;
    cJSON *multi_modal_id = NULL;
    cJSON *exp_tran_ind = NULL;
    cJSON *ul_br_rec_ind = NULL;
    flow_infos = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "flowInfos");
    if (flow_infos) {
        cJSON *flow_infos_local = NULL;
        if (!cJSON_IsArray(flow_infos)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [flow_infos]");
            goto end;
        }

        flow_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(flow_infos_local, flow_infos) {
            if (!cJSON_IsObject(flow_infos_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [flow_infos]");
                goto end;
            }
            OpenAPI_flow_information_t *flow_infosItem = OpenAPI_flow_information_parseFromJSON(flow_infos_local);
            if (!flow_infosItem) {
                ogs_error("No flow_infosItem");
                goto end;
            }
            OpenAPI_list_add(flow_infosList, flow_infosItem);
        }
    }

    app_id = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "appId");
    if (app_id) {
    if (!cJSON_IsString(app_id) && !cJSON_IsNull(app_id)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [app_id]");
        goto end;
    }
    }

    app_descriptor = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "appDescriptor");
    if (app_descriptor) {
    if (!cJSON_IsString(app_descriptor) && !cJSON_IsNull(app_descriptor)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [app_descriptor]");
        goto end;
    }
    }

    cont_ver = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "contVer");
    if (cont_ver) {
    if (!cJSON_IsNumber(cont_ver)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [cont_ver]");
        goto end;
    }
    }

    proto_desc_dl = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "protoDescDl");
    if (proto_desc_dl) {
    proto_desc_dl_local_nonprim = OpenAPI_protocol_description_parseFromJSON(proto_desc_dl);
    if (!proto_desc_dl_local_nonprim) {
        ogs_error("OpenAPI_protocol_description_parseFromJSON failed [proto_desc_dl]");
        goto end;
    }
    }

    proto_desc_ul = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "protoDescUl");
    if (proto_desc_ul) {
    proto_desc_ul_local_nonprim = OpenAPI_protocol_description_parseFromJSON(proto_desc_ul);
    if (!proto_desc_ul_local_nonprim) {
        ogs_error("OpenAPI_protocol_description_parseFromJSON failed [proto_desc_ul]");
        goto end;
    }
    }

    pcc_rule_id = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "pccRuleId");
    if (!pcc_rule_id) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [pcc_rule_id]");
        goto end;
    }
    if (!cJSON_IsString(pcc_rule_id)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [pcc_rule_id]");
        goto end;
    }

    precedence = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "precedence");
    if (precedence) {
    if (!cJSON_IsNumber(precedence)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [precedence]");
        goto end;
    }
    }

    af_sig_protocol = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "afSigProtocol");
    if (af_sig_protocol) {
    if (!cJSON_IsString(af_sig_protocol)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [af_sig_protocol]");
        goto end;
    }
    af_sig_protocolVariable = OpenAPI_af_sig_protocol_FromString(af_sig_protocol->valuestring);
    }

    app_reloc = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "appReloc");
    if (app_reloc) {
    if (!cJSON_IsBool(app_reloc)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [app_reloc]");
        goto end;
    }
    }

    eas_redis_ind = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "easRedisInd");
    if (eas_redis_ind) {
    if (!cJSON_IsBool(eas_redis_ind)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [eas_redis_ind]");
        goto end;
    }
    }

    ref_qos_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refQosData");
    if (ref_qos_data) {
        cJSON *ref_qos_data_local = NULL;
        if (!cJSON_IsArray(ref_qos_data)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_qos_data]");
            goto end;
        }

        ref_qos_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_qos_data_local, ref_qos_data) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ref_qos_data_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_qos_data]");
                goto end;
            }
            OpenAPI_list_add(ref_qos_dataList, ogs_strdup(ref_qos_data_local->valuestring));
        }
    }

    ref_alt_qos_params = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refAltQosParams");
    if (ref_alt_qos_params) {
        cJSON *ref_alt_qos_params_local = NULL;
        if (!cJSON_IsArray(ref_alt_qos_params)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_alt_qos_params]");
            goto end;
        }

        ref_alt_qos_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_alt_qos_params_local, ref_alt_qos_params) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ref_alt_qos_params_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_alt_qos_params]");
                goto end;
            }
            OpenAPI_list_add(ref_alt_qos_paramsList, ogs_strdup(ref_alt_qos_params_local->valuestring));
        }
    }

    ref_tc_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refTcData");
    if (ref_tc_data) {
        cJSON *ref_tc_data_local = NULL;
        if (!cJSON_IsArray(ref_tc_data)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_tc_data]");
            goto end;
        }

        ref_tc_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_tc_data_local, ref_tc_data) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ref_tc_data_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_tc_data]");
                goto end;
            }
            OpenAPI_list_add(ref_tc_dataList, ogs_strdup(ref_tc_data_local->valuestring));
        }
    }

    ref_chg_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refChgData");
    if (ref_chg_data) {
    if (!cJSON_IsNull(ref_chg_data)) {
        cJSON *ref_chg_data_local = NULL;
        if (!cJSON_IsArray(ref_chg_data)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_chg_data]");
            goto end;
        }

        ref_chg_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_chg_data_local, ref_chg_data) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ref_chg_data_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_chg_data]");
                goto end;
            }
            OpenAPI_list_add(ref_chg_dataList, ogs_strdup(ref_chg_data_local->valuestring));
        }
    }
    }

    ref_chg_n3g_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refChgN3gData");
    if (ref_chg_n3g_data) {
    if (!cJSON_IsNull(ref_chg_n3g_data)) {
        cJSON *ref_chg_n3g_data_local = NULL;
        if (!cJSON_IsArray(ref_chg_n3g_data)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_chg_n3g_data]");
            goto end;
        }

        ref_chg_n3g_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_chg_n3g_data_local, ref_chg_n3g_data) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ref_chg_n3g_data_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_chg_n3g_data]");
                goto end;
            }
            OpenAPI_list_add(ref_chg_n3g_dataList, ogs_strdup(ref_chg_n3g_data_local->valuestring));
        }
    }
    }

    ref_um_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refUmData");
    if (ref_um_data) {
    if (!cJSON_IsNull(ref_um_data)) {
        cJSON *ref_um_data_local = NULL;
        if (!cJSON_IsArray(ref_um_data)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_um_data]");
            goto end;
        }

        ref_um_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_um_data_local, ref_um_data) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ref_um_data_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_um_data]");
                goto end;
            }
            OpenAPI_list_add(ref_um_dataList, ogs_strdup(ref_um_data_local->valuestring));
        }
    }
    }

    ref_um_n3g_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refUmN3gData");
    if (ref_um_n3g_data) {
    if (!cJSON_IsNull(ref_um_n3g_data)) {
        cJSON *ref_um_n3g_data_local = NULL;
        if (!cJSON_IsArray(ref_um_n3g_data)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_um_n3g_data]");
            goto end;
        }

        ref_um_n3g_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_um_n3g_data_local, ref_um_n3g_data) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ref_um_n3g_data_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_um_n3g_data]");
                goto end;
            }
            OpenAPI_list_add(ref_um_n3g_dataList, ogs_strdup(ref_um_n3g_data_local->valuestring));
        }
    }
    }

    ref_cond_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refCondData");
    if (ref_cond_data) {
    if (!cJSON_IsNull(ref_cond_data)) {
    if (!cJSON_IsString(ref_cond_data) && !cJSON_IsNull(ref_cond_data)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_cond_data]");
        goto end;
    }
    }
    }

    ref_qos_mon = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refQosMon");
    if (ref_qos_mon) {
    if (!cJSON_IsNull(ref_qos_mon)) {
        cJSON *ref_qos_mon_local = NULL;
        if (!cJSON_IsArray(ref_qos_mon)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_qos_mon]");
            goto end;
        }

        ref_qos_monList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_qos_mon_local, ref_qos_mon) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ref_qos_mon_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_qos_mon]");
                goto end;
            }
            OpenAPI_list_add(ref_qos_monList, ogs_strdup(ref_qos_mon_local->valuestring));
        }
    }
    }

    addr_preser_ind = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "addrPreserInd");
    if (addr_preser_ind) {
    if (!cJSON_IsNull(addr_preser_ind)) {
    if (!cJSON_IsBool(addr_preser_ind)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [addr_preser_ind]");
        goto end;
    }
    }
    }

    tscai_input_dl = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "tscaiInputDl");
    if (tscai_input_dl) {
    if (!cJSON_IsNull(tscai_input_dl)) {
    tscai_input_dl_local_nonprim = OpenAPI_tscai_input_container_parseFromJSON(tscai_input_dl);
    if (!tscai_input_dl_local_nonprim) {
        ogs_error("OpenAPI_tscai_input_container_parseFromJSON failed [tscai_input_dl]");
        goto end;
    }
    }
    }

    tscai_input_ul = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "tscaiInputUl");
    if (tscai_input_ul) {
    if (!cJSON_IsNull(tscai_input_ul)) {
    tscai_input_ul_local_nonprim = OpenAPI_tscai_input_container_parseFromJSON(tscai_input_ul);
    if (!tscai_input_ul_local_nonprim) {
        ogs_error("OpenAPI_tscai_input_container_parseFromJSON failed [tscai_input_ul]");
        goto end;
    }
    }
    }

    tscai_time_dom = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "tscaiTimeDom");
    if (tscai_time_dom) {
    if (!cJSON_IsNumber(tscai_time_dom)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [tscai_time_dom]");
        goto end;
    }
    }

    cap_bat_adaptation = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "capBatAdaptation");
    if (cap_bat_adaptation) {
    if (!cJSON_IsBool(cap_bat_adaptation)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [cap_bat_adaptation]");
        goto end;
    }
    }

    dd_notif_ctrl = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "ddNotifCtrl");
    if (dd_notif_ctrl) {
    dd_notif_ctrl_local_nonprim = OpenAPI_downlink_data_notification_control_parseFromJSON(dd_notif_ctrl);
    if (!dd_notif_ctrl_local_nonprim) {
        ogs_error("OpenAPI_downlink_data_notification_control_parseFromJSON failed [dd_notif_ctrl]");
        goto end;
    }
    }

    dd_notif_ctrl2 = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "ddNotifCtrl2");
    if (dd_notif_ctrl2) {
    if (!cJSON_IsNull(dd_notif_ctrl2)) {
    dd_notif_ctrl2_local_nonprim = OpenAPI_downlink_data_notification_control_rm_parseFromJSON(dd_notif_ctrl2);
    if (!dd_notif_ctrl2_local_nonprim) {
        ogs_error("OpenAPI_downlink_data_notification_control_rm_parseFromJSON failed [dd_notif_ctrl2]");
        goto end;
    }
    }
    }

    dis_ue_notif = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "disUeNotif");
    if (dis_ue_notif) {
    if (!cJSON_IsNull(dis_ue_notif)) {
    if (!cJSON_IsBool(dis_ue_notif)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [dis_ue_notif]");
        goto end;
    }
    }
    }

    pack_filt_all_prec = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "packFiltAllPrec");
    if (pack_filt_all_prec) {
    if (!cJSON_IsNumber(pack_filt_all_prec)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [pack_filt_all_prec]");
        goto end;
    }
    }

    nsc_supp_feats = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "nscSuppFeats");
    if (nsc_supp_feats) {
        cJSON *nsc_supp_feats_local_map = NULL;
        if (!cJSON_IsObject(nsc_supp_feats) && !cJSON_IsNull(nsc_supp_feats)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [nsc_supp_feats]");
            goto end;
        }
        if (cJSON_IsObject(nsc_supp_feats)) {
            nsc_supp_featsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(nsc_supp_feats_local_map, nsc_supp_feats) {
                cJSON *localMapObject = nsc_supp_feats_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsString(localMapObject)) {
                    ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [inner]");
                    goto end;
                }
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), ogs_strdup(localMapObject->valuestring));
                OpenAPI_list_add(nsc_supp_featsList, localMapKeyPair);
            }
        }
    }

    call_info = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "callInfo");
    if (call_info) {
    if (!cJSON_IsNull(call_info)) {
    call_info_local_nonprim = OpenAPI_call_info_parseFromJSON(call_info);
    if (!call_info_local_nonprim) {
        ogs_error("OpenAPI_call_info_parseFromJSON failed [call_info]");
        goto end;
    }
    }
    }

    traff_para_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "traffParaData");
    if (traff_para_data) {
    traff_para_data_local_nonprim = OpenAPI_traffic_para_data_parseFromJSON(traff_para_data);
    if (!traff_para_data_local_nonprim) {
        ogs_error("OpenAPI_traffic_para_data_parseFromJSON failed [traff_para_data]");
        goto end;
    }
    }

    multi_modal_id = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "multiModalId");
    if (multi_modal_id) {
    if (!cJSON_IsString(multi_modal_id) && !cJSON_IsNull(multi_modal_id)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [multi_modal_id]");
        goto end;
    }
    }

    exp_tran_ind = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "expTranInd");
    if (exp_tran_ind) {
    if (!cJSON_IsNull(exp_tran_ind)) {
    if (!cJSON_IsBool(exp_tran_ind)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [exp_tran_ind]");
        goto end;
    }
    }
    }

    ul_br_rec_ind = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "ulBrRecInd");
    if (ul_br_rec_ind) {
    if (!cJSON_IsNull(ul_br_rec_ind)) {
    if (!cJSON_IsBool(ul_br_rec_ind)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ul_br_rec_ind]");
        goto end;
    }
    }
    }

    pcc_rule_local_var = OpenAPI_pcc_rule_create (
        flow_infos ? flow_infosList : NULL,
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL,
        app_descriptor && !cJSON_IsNull(app_descriptor) ? ogs_strdup(app_descriptor->valuestring) : NULL,
        cont_ver ? true : false,
        cont_ver ? cont_ver->valuedouble : 0,
        proto_desc_dl ? proto_desc_dl_local_nonprim : NULL,
        proto_desc_ul ? proto_desc_ul_local_nonprim : NULL,
        ogs_strdup(pcc_rule_id->valuestring),
        precedence ? true : false,
        precedence ? precedence->valuedouble : 0,
        af_sig_protocol ? af_sig_protocolVariable : 0,
        app_reloc ? true : false,
        app_reloc ? app_reloc->valueint : 0,
        eas_redis_ind ? true : false,
        eas_redis_ind ? eas_redis_ind->valueint : 0,
        ref_qos_data ? ref_qos_dataList : NULL,
        ref_alt_qos_params ? ref_alt_qos_paramsList : NULL,
        ref_tc_data ? ref_tc_dataList : NULL,
        ref_chg_data && cJSON_IsNull(ref_chg_data) ? true : false,
        ref_chg_data ? ref_chg_dataList : NULL,
        ref_chg_n3g_data && cJSON_IsNull(ref_chg_n3g_data) ? true : false,
        ref_chg_n3g_data ? ref_chg_n3g_dataList : NULL,
        ref_um_data && cJSON_IsNull(ref_um_data) ? true : false,
        ref_um_data ? ref_um_dataList : NULL,
        ref_um_n3g_data && cJSON_IsNull(ref_um_n3g_data) ? true : false,
        ref_um_n3g_data ? ref_um_n3g_dataList : NULL,
        ref_cond_data && cJSON_IsNull(ref_cond_data) ? true : false,
        ref_cond_data && !cJSON_IsNull(ref_cond_data) ? ogs_strdup(ref_cond_data->valuestring) : NULL,
        ref_qos_mon && cJSON_IsNull(ref_qos_mon) ? true : false,
        ref_qos_mon ? ref_qos_monList : NULL,
        addr_preser_ind && cJSON_IsNull(addr_preser_ind) ? true : false,
        addr_preser_ind ? true : false,
        addr_preser_ind ? addr_preser_ind->valueint : 0,
        tscai_input_dl && cJSON_IsNull(tscai_input_dl) ? true : false,
        tscai_input_dl ? tscai_input_dl_local_nonprim : NULL,
        tscai_input_ul && cJSON_IsNull(tscai_input_ul) ? true : false,
        tscai_input_ul ? tscai_input_ul_local_nonprim : NULL,
        tscai_time_dom ? true : false,
        tscai_time_dom ? tscai_time_dom->valuedouble : 0,
        cap_bat_adaptation ? true : false,
        cap_bat_adaptation ? cap_bat_adaptation->valueint : 0,
        dd_notif_ctrl ? dd_notif_ctrl_local_nonprim : NULL,
        dd_notif_ctrl2 && cJSON_IsNull(dd_notif_ctrl2) ? true : false,
        dd_notif_ctrl2 ? dd_notif_ctrl2_local_nonprim : NULL,
        dis_ue_notif && cJSON_IsNull(dis_ue_notif) ? true : false,
        dis_ue_notif ? true : false,
        dis_ue_notif ? dis_ue_notif->valueint : 0,
        pack_filt_all_prec ? true : false,
        pack_filt_all_prec ? pack_filt_all_prec->valuedouble : 0,
        nsc_supp_feats ? nsc_supp_featsList : NULL,
        call_info && cJSON_IsNull(call_info) ? true : false,
        call_info ? call_info_local_nonprim : NULL,
        traff_para_data ? traff_para_data_local_nonprim : NULL,
        multi_modal_id && !cJSON_IsNull(multi_modal_id) ? ogs_strdup(multi_modal_id->valuestring) : NULL,
        exp_tran_ind && cJSON_IsNull(exp_tran_ind) ? true : false,
        exp_tran_ind ? true : false,
        exp_tran_ind ? exp_tran_ind->valueint : 0,
        ul_br_rec_ind && cJSON_IsNull(ul_br_rec_ind) ? true : false,
        ul_br_rec_ind ? true : false,
        ul_br_rec_ind ? ul_br_rec_ind->valueint : 0
    );

    return pcc_rule_local_var;
end:
    if (flow_infosList) {
        OpenAPI_list_for_each(flow_infosList, node) {
            OpenAPI_flow_information_free(node->data);
        }
        OpenAPI_list_free(flow_infosList);
        flow_infosList = NULL;
    }
    if (proto_desc_dl_local_nonprim) {
        OpenAPI_protocol_description_free(proto_desc_dl_local_nonprim);
        proto_desc_dl_local_nonprim = NULL;
    }
    if (proto_desc_ul_local_nonprim) {
        OpenAPI_protocol_description_free(proto_desc_ul_local_nonprim);
        proto_desc_ul_local_nonprim = NULL;
    }
    if (ref_qos_dataList) {
        OpenAPI_list_for_each(ref_qos_dataList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ref_qos_dataList);
        ref_qos_dataList = NULL;
    }
    if (ref_alt_qos_paramsList) {
        OpenAPI_list_for_each(ref_alt_qos_paramsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ref_alt_qos_paramsList);
        ref_alt_qos_paramsList = NULL;
    }
    if (ref_tc_dataList) {
        OpenAPI_list_for_each(ref_tc_dataList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ref_tc_dataList);
        ref_tc_dataList = NULL;
    }
    if (ref_chg_dataList) {
        OpenAPI_list_for_each(ref_chg_dataList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ref_chg_dataList);
        ref_chg_dataList = NULL;
    }
    if (ref_chg_n3g_dataList) {
        OpenAPI_list_for_each(ref_chg_n3g_dataList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ref_chg_n3g_dataList);
        ref_chg_n3g_dataList = NULL;
    }
    if (ref_um_dataList) {
        OpenAPI_list_for_each(ref_um_dataList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ref_um_dataList);
        ref_um_dataList = NULL;
    }
    if (ref_um_n3g_dataList) {
        OpenAPI_list_for_each(ref_um_n3g_dataList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ref_um_n3g_dataList);
        ref_um_n3g_dataList = NULL;
    }
    if (ref_qos_monList) {
        OpenAPI_list_for_each(ref_qos_monList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ref_qos_monList);
        ref_qos_monList = NULL;
    }
    if (tscai_input_dl_local_nonprim) {
        OpenAPI_tscai_input_container_free(tscai_input_dl_local_nonprim);
        tscai_input_dl_local_nonprim = NULL;
    }
    if (tscai_input_ul_local_nonprim) {
        OpenAPI_tscai_input_container_free(tscai_input_ul_local_nonprim);
        tscai_input_ul_local_nonprim = NULL;
    }
    if (dd_notif_ctrl_local_nonprim) {
        OpenAPI_downlink_data_notification_control_free(dd_notif_ctrl_local_nonprim);
        dd_notif_ctrl_local_nonprim = NULL;
    }
    if (dd_notif_ctrl2_local_nonprim) {
        OpenAPI_downlink_data_notification_control_rm_free(dd_notif_ctrl2_local_nonprim);
        dd_notif_ctrl2_local_nonprim = NULL;
    }
    if (nsc_supp_featsList) {
        OpenAPI_list_for_each(nsc_supp_featsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nsc_supp_featsList);
        nsc_supp_featsList = NULL;
    }
    if (call_info_local_nonprim) {
        OpenAPI_call_info_free(call_info_local_nonprim);
        call_info_local_nonprim = NULL;
    }
    if (traff_para_data_local_nonprim) {
        OpenAPI_traffic_para_data_free(traff_para_data_local_nonprim);
        traff_para_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pcc_rule_t *OpenAPI_pcc_rule_copy(OpenAPI_pcc_rule_t *dst, OpenAPI_pcc_rule_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcc_rule_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed");
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

    OpenAPI_pcc_rule_free(dst);
    dst = OpenAPI_pcc_rule_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

