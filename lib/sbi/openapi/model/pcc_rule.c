
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
    OpenAPI_downlink_data_notification_control_t *dd_notif_ctrl,
    bool is_dd_notif_ctrl2_null,
    OpenAPI_downlink_data_notification_control_rm_t *dd_notif_ctrl2,
    bool is_dis_ue_notif_null,
    bool is_dis_ue_notif,
    int dis_ue_notif,
    bool is_pack_filt_all_prec,
    int pack_filt_all_prec
)
{
    OpenAPI_pcc_rule_t *pcc_rule_local_var = ogs_malloc(sizeof(OpenAPI_pcc_rule_t));
    ogs_assert(pcc_rule_local_var);

    pcc_rule_local_var->flow_infos = flow_infos;
    pcc_rule_local_var->app_id = app_id;
    pcc_rule_local_var->app_descriptor = app_descriptor;
    pcc_rule_local_var->is_cont_ver = is_cont_ver;
    pcc_rule_local_var->cont_ver = cont_ver;
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
    pcc_rule_local_var->dd_notif_ctrl = dd_notif_ctrl;
    pcc_rule_local_var->is_dd_notif_ctrl2_null = is_dd_notif_ctrl2_null;
    pcc_rule_local_var->dd_notif_ctrl2 = dd_notif_ctrl2;
    pcc_rule_local_var->is_dis_ue_notif_null = is_dis_ue_notif_null;
    pcc_rule_local_var->is_dis_ue_notif = is_dis_ue_notif;
    pcc_rule_local_var->dis_ue_notif = dis_ue_notif;
    pcc_rule_local_var->is_pack_filt_all_prec = is_pack_filt_all_prec;
    pcc_rule_local_var->pack_filt_all_prec = pack_filt_all_prec;

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
    cJSON *dd_notif_ctrl = NULL;
    OpenAPI_downlink_data_notification_control_t *dd_notif_ctrl_local_nonprim = NULL;
    cJSON *dd_notif_ctrl2 = NULL;
    OpenAPI_downlink_data_notification_control_rm_t *dd_notif_ctrl2_local_nonprim = NULL;
    cJSON *dis_ue_notif = NULL;
    cJSON *pack_filt_all_prec = NULL;
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

    pcc_rule_local_var = OpenAPI_pcc_rule_create (
        flow_infos ? flow_infosList : NULL,
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL,
        app_descriptor && !cJSON_IsNull(app_descriptor) ? ogs_strdup(app_descriptor->valuestring) : NULL,
        cont_ver ? true : false,
        cont_ver ? cont_ver->valuedouble : 0,
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
        dd_notif_ctrl ? dd_notif_ctrl_local_nonprim : NULL,
        dd_notif_ctrl2 && cJSON_IsNull(dd_notif_ctrl2) ? true : false,
        dd_notif_ctrl2 ? dd_notif_ctrl2_local_nonprim : NULL,
        dis_ue_notif && cJSON_IsNull(dis_ue_notif) ? true : false,
        dis_ue_notif ? true : false,
        dis_ue_notif ? dis_ue_notif->valueint : 0,
        pack_filt_all_prec ? true : false,
        pack_filt_all_prec ? pack_filt_all_prec->valuedouble : 0
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

