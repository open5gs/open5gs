
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcc_rule.h"

OpenAPI_pcc_rule_t *OpenAPI_pcc_rule_create(
    OpenAPI_list_t *flow_infos,
    char *app_id,
    int cont_ver,
    char *pcc_rule_id,
    int precedence,
    OpenAPI_af_sig_protocol_t *af_sig_protocol,
    int app_reloc,
    OpenAPI_list_t *ref_qos_data,
    OpenAPI_list_t *ref_alt_qos_params,
    OpenAPI_list_t *ref_tc_data,
    OpenAPI_list_t *ref_chg_data,
    OpenAPI_list_t *ref_chg_n3g_data,
    OpenAPI_list_t *ref_um_data,
    OpenAPI_list_t *ref_um_n3g_data,
    char *ref_cond_data,
    OpenAPI_list_t *ref_qos_mon,
    int addr_preser_ind,
    OpenAPI_tscai_input_container_t *tscai_input_dl,
    OpenAPI_tscai_input_container_t *tscai_input_ul
    )
{
    OpenAPI_pcc_rule_t *pcc_rule_local_var = OpenAPI_malloc(sizeof(OpenAPI_pcc_rule_t));
    if (!pcc_rule_local_var) {
        return NULL;
    }
    pcc_rule_local_var->flow_infos = flow_infos;
    pcc_rule_local_var->app_id = app_id;
    pcc_rule_local_var->cont_ver = cont_ver;
    pcc_rule_local_var->pcc_rule_id = pcc_rule_id;
    pcc_rule_local_var->precedence = precedence;
    pcc_rule_local_var->af_sig_protocol = af_sig_protocol;
    pcc_rule_local_var->app_reloc = app_reloc;
    pcc_rule_local_var->ref_qos_data = ref_qos_data;
    pcc_rule_local_var->ref_alt_qos_params = ref_alt_qos_params;
    pcc_rule_local_var->ref_tc_data = ref_tc_data;
    pcc_rule_local_var->ref_chg_data = ref_chg_data;
    pcc_rule_local_var->ref_chg_n3g_data = ref_chg_n3g_data;
    pcc_rule_local_var->ref_um_data = ref_um_data;
    pcc_rule_local_var->ref_um_n3g_data = ref_um_n3g_data;
    pcc_rule_local_var->ref_cond_data = ref_cond_data;
    pcc_rule_local_var->ref_qos_mon = ref_qos_mon;
    pcc_rule_local_var->addr_preser_ind = addr_preser_ind;
    pcc_rule_local_var->tscai_input_dl = tscai_input_dl;
    pcc_rule_local_var->tscai_input_ul = tscai_input_ul;

    return pcc_rule_local_var;
}

void OpenAPI_pcc_rule_free(OpenAPI_pcc_rule_t *pcc_rule)
{
    if (NULL == pcc_rule) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(pcc_rule->flow_infos, node) {
        OpenAPI_flow_information_free(node->data);
    }
    OpenAPI_list_free(pcc_rule->flow_infos);
    ogs_free(pcc_rule->app_id);
    ogs_free(pcc_rule->pcc_rule_id);
    OpenAPI_af_sig_protocol_free(pcc_rule->af_sig_protocol);
    OpenAPI_list_for_each(pcc_rule->ref_qos_data, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcc_rule->ref_qos_data);
    OpenAPI_list_for_each(pcc_rule->ref_alt_qos_params, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcc_rule->ref_alt_qos_params);
    OpenAPI_list_for_each(pcc_rule->ref_tc_data, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcc_rule->ref_tc_data);
    OpenAPI_list_for_each(pcc_rule->ref_chg_data, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcc_rule->ref_chg_data);
    OpenAPI_list_for_each(pcc_rule->ref_chg_n3g_data, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcc_rule->ref_chg_n3g_data);
    OpenAPI_list_for_each(pcc_rule->ref_um_data, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcc_rule->ref_um_data);
    OpenAPI_list_for_each(pcc_rule->ref_um_n3g_data, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcc_rule->ref_um_n3g_data);
    ogs_free(pcc_rule->ref_cond_data);
    OpenAPI_list_for_each(pcc_rule->ref_qos_mon, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(pcc_rule->ref_qos_mon);
    OpenAPI_tscai_input_container_free(pcc_rule->tscai_input_dl);
    OpenAPI_tscai_input_container_free(pcc_rule->tscai_input_ul);
    ogs_free(pcc_rule);
}

cJSON *OpenAPI_pcc_rule_convertToJSON(OpenAPI_pcc_rule_t *pcc_rule)
{
    cJSON *item = NULL;

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

        OpenAPI_lnode_t *flow_infos_node;
        if (pcc_rule->flow_infos) {
            OpenAPI_list_for_each(pcc_rule->flow_infos, flow_infos_node) {
                cJSON *itemLocal = OpenAPI_flow_information_convertToJSON(flow_infos_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [flow_infos]");
                    goto end;
                }
                cJSON_AddItemToArray(flow_infosList, itemLocal);
            }
        }
    }

    if (pcc_rule->app_id) {
        if (cJSON_AddStringToObject(item, "appId", pcc_rule->app_id) == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [app_id]");
            goto end;
        }
    }

    if (pcc_rule->cont_ver) {
        if (cJSON_AddNumberToObject(item, "contVer", pcc_rule->cont_ver) == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [cont_ver]");
            goto end;
        }
    }

    if (!pcc_rule->pcc_rule_id) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [pcc_rule_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "pccRuleId", pcc_rule->pcc_rule_id) == NULL) {
        ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [pcc_rule_id]");
        goto end;
    }

    if (pcc_rule->precedence) {
        if (cJSON_AddNumberToObject(item, "precedence", pcc_rule->precedence) == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [precedence]");
            goto end;
        }
    }

    if (pcc_rule->af_sig_protocol) {
        cJSON *af_sig_protocol_local_JSON = OpenAPI_af_sig_protocol_convertToJSON(pcc_rule->af_sig_protocol);
        if (af_sig_protocol_local_JSON == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [af_sig_protocol]");
            goto end;
        }
        cJSON_AddItemToObject(item, "afSigProtocol", af_sig_protocol_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [af_sig_protocol]");
            goto end;
        }
    }

    if (pcc_rule->app_reloc) {
        if (cJSON_AddBoolToObject(item, "appReloc", pcc_rule->app_reloc) == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [app_reloc]");
            goto end;
        }
    }

    if (pcc_rule->ref_qos_data) {
        cJSON *ref_qos_data = cJSON_AddArrayToObject(item, "refQosData");
        if (ref_qos_data == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_qos_data]");
            goto end;
        }

        OpenAPI_lnode_t *ref_qos_data_node;
        OpenAPI_list_for_each(pcc_rule->ref_qos_data, ref_qos_data_node)  {
            if (cJSON_AddStringToObject(ref_qos_data, "", (char*)ref_qos_data_node->data) == NULL) {
                ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_qos_data]");
                goto end;
            }
        }
    }

    if (pcc_rule->ref_alt_qos_params) {
        cJSON *ref_alt_qos_params = cJSON_AddArrayToObject(item, "refAltQosParams");
        if (ref_alt_qos_params == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_alt_qos_params]");
            goto end;
        }

        OpenAPI_lnode_t *ref_alt_qos_params_node;
        OpenAPI_list_for_each(pcc_rule->ref_alt_qos_params, ref_alt_qos_params_node)  {
            if (cJSON_AddStringToObject(ref_alt_qos_params, "", (char*)ref_alt_qos_params_node->data) == NULL) {
                ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_alt_qos_params]");
                goto end;
            }
        }
    }

    if (pcc_rule->ref_tc_data) {
        cJSON *ref_tc_data = cJSON_AddArrayToObject(item, "refTcData");
        if (ref_tc_data == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_tc_data]");
            goto end;
        }

        OpenAPI_lnode_t *ref_tc_data_node;
        OpenAPI_list_for_each(pcc_rule->ref_tc_data, ref_tc_data_node)  {
            if (cJSON_AddStringToObject(ref_tc_data, "", (char*)ref_tc_data_node->data) == NULL) {
                ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_tc_data]");
                goto end;
            }
        }
    }

    if (pcc_rule->ref_chg_data) {
        cJSON *ref_chg_data = cJSON_AddArrayToObject(item, "refChgData");
        if (ref_chg_data == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_chg_data]");
            goto end;
        }

        OpenAPI_lnode_t *ref_chg_data_node;
        OpenAPI_list_for_each(pcc_rule->ref_chg_data, ref_chg_data_node)  {
            if (cJSON_AddStringToObject(ref_chg_data, "", (char*)ref_chg_data_node->data) == NULL) {
                ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_chg_data]");
                goto end;
            }
        }
    }

    if (pcc_rule->ref_chg_n3g_data) {
        cJSON *ref_chg_n3g_data = cJSON_AddArrayToObject(item, "refChgN3gData");
        if (ref_chg_n3g_data == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_chg_n3g_data]");
            goto end;
        }

        OpenAPI_lnode_t *ref_chg_n3g_data_node;
        OpenAPI_list_for_each(pcc_rule->ref_chg_n3g_data, ref_chg_n3g_data_node)  {
            if (cJSON_AddStringToObject(ref_chg_n3g_data, "", (char*)ref_chg_n3g_data_node->data) == NULL) {
                ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_chg_n3g_data]");
                goto end;
            }
        }
    }

    if (pcc_rule->ref_um_data) {
        cJSON *ref_um_data = cJSON_AddArrayToObject(item, "refUmData");
        if (ref_um_data == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_um_data]");
            goto end;
        }

        OpenAPI_lnode_t *ref_um_data_node;
        OpenAPI_list_for_each(pcc_rule->ref_um_data, ref_um_data_node)  {
            if (cJSON_AddStringToObject(ref_um_data, "", (char*)ref_um_data_node->data) == NULL) {
                ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_um_data]");
                goto end;
            }
        }
    }

    if (pcc_rule->ref_um_n3g_data) {
        cJSON *ref_um_n3g_data = cJSON_AddArrayToObject(item, "refUmN3gData");
        if (ref_um_n3g_data == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_um_n3g_data]");
            goto end;
        }

        OpenAPI_lnode_t *ref_um_n3g_data_node;
        OpenAPI_list_for_each(pcc_rule->ref_um_n3g_data, ref_um_n3g_data_node)  {
            if (cJSON_AddStringToObject(ref_um_n3g_data, "", (char*)ref_um_n3g_data_node->data) == NULL) {
                ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_um_n3g_data]");
                goto end;
            }
        }
    }

    if (pcc_rule->ref_cond_data) {
        if (cJSON_AddStringToObject(item, "refCondData", pcc_rule->ref_cond_data) == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_cond_data]");
            goto end;
        }
    }

    if (pcc_rule->ref_qos_mon) {
        cJSON *ref_qos_mon = cJSON_AddArrayToObject(item, "refQosMon");
        if (ref_qos_mon == NULL) {
            ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_qos_mon]");
            goto end;
        }

        OpenAPI_lnode_t *ref_qos_mon_node;
        OpenAPI_list_for_each(pcc_rule->ref_qos_mon, ref_qos_mon_node)  {
            if (cJSON_AddStringToObject(ref_qos_mon, "", (char*)ref_qos_mon_node->data) == NULL) {
                ogs_error("OpenAPI_pcc_rule_convertToJSON() failed [ref_qos_mon]");
                goto end;
            }
        }
    }

    if (pcc_rule->addr_preser_ind) {
        if (cJSON_AddBoolToObject(item, "addrPreserInd", pcc_rule->addr_preser_ind) == NULL) {
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
    }

end:
    return item;
}

OpenAPI_pcc_rule_t *OpenAPI_pcc_rule_parseFromJSON(cJSON *pcc_ruleJSON)
{
    OpenAPI_pcc_rule_t *pcc_rule_local_var = NULL;
    cJSON *flow_infos = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "flowInfos");

    OpenAPI_list_t *flow_infosList;
    if (flow_infos) {
        cJSON *flow_infos_local_nonprimitive;
        if (!cJSON_IsArray(flow_infos)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [flow_infos]");
            goto end;
        }

        flow_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(flow_infos_local_nonprimitive, flow_infos ) {
            if (!cJSON_IsObject(flow_infos_local_nonprimitive)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [flow_infos]");
                goto end;
            }
            OpenAPI_flow_information_t *flow_infosItem = OpenAPI_flow_information_parseFromJSON(flow_infos_local_nonprimitive);

            OpenAPI_list_add(flow_infosList, flow_infosItem);
        }
    }

    cJSON *app_id = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "appId");

    if (app_id) {
        if (!cJSON_IsString(app_id)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [app_id]");
            goto end;
        }
    }

    cJSON *cont_ver = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "contVer");

    if (cont_ver) {
        if (!cJSON_IsNumber(cont_ver)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [cont_ver]");
            goto end;
        }
    }

    cJSON *pcc_rule_id = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "pccRuleId");
    if (!pcc_rule_id) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [pcc_rule_id]");
        goto end;
    }


    if (!cJSON_IsString(pcc_rule_id)) {
        ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [pcc_rule_id]");
        goto end;
    }

    cJSON *precedence = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "precedence");

    if (precedence) {
        if (!cJSON_IsNumber(precedence)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [precedence]");
            goto end;
        }
    }

    cJSON *af_sig_protocol = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "afSigProtocol");

    OpenAPI_af_sig_protocol_t *af_sig_protocol_local_nonprim = NULL;
    if (af_sig_protocol) {
        af_sig_protocol_local_nonprim = OpenAPI_af_sig_protocol_parseFromJSON(af_sig_protocol);
    }

    cJSON *app_reloc = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "appReloc");

    if (app_reloc) {
        if (!cJSON_IsBool(app_reloc)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [app_reloc]");
            goto end;
        }
    }

    cJSON *ref_qos_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refQosData");

    OpenAPI_list_t *ref_qos_dataList;
    if (ref_qos_data) {
        cJSON *ref_qos_data_local;
        if (!cJSON_IsArray(ref_qos_data)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_qos_data]");
            goto end;
        }
        ref_qos_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_qos_data_local, ref_qos_data) {
            if (!cJSON_IsString(ref_qos_data_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_qos_data]");
                goto end;
            }
            OpenAPI_list_add(ref_qos_dataList, ogs_strdup(ref_qos_data_local->valuestring));
        }
    }

    cJSON *ref_alt_qos_params = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refAltQosParams");

    OpenAPI_list_t *ref_alt_qos_paramsList;
    if (ref_alt_qos_params) {
        cJSON *ref_alt_qos_params_local;
        if (!cJSON_IsArray(ref_alt_qos_params)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_alt_qos_params]");
            goto end;
        }
        ref_alt_qos_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_alt_qos_params_local, ref_alt_qos_params) {
            if (!cJSON_IsString(ref_alt_qos_params_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_alt_qos_params]");
                goto end;
            }
            OpenAPI_list_add(ref_alt_qos_paramsList, ogs_strdup(ref_alt_qos_params_local->valuestring));
        }
    }

    cJSON *ref_tc_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refTcData");

    OpenAPI_list_t *ref_tc_dataList;
    if (ref_tc_data) {
        cJSON *ref_tc_data_local;
        if (!cJSON_IsArray(ref_tc_data)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_tc_data]");
            goto end;
        }
        ref_tc_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_tc_data_local, ref_tc_data) {
            if (!cJSON_IsString(ref_tc_data_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_tc_data]");
                goto end;
            }
            OpenAPI_list_add(ref_tc_dataList, ogs_strdup(ref_tc_data_local->valuestring));
        }
    }

    cJSON *ref_chg_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refChgData");

    OpenAPI_list_t *ref_chg_dataList;
    if (ref_chg_data) {
        cJSON *ref_chg_data_local;
        if (!cJSON_IsArray(ref_chg_data)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_chg_data]");
            goto end;
        }
        ref_chg_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_chg_data_local, ref_chg_data) {
            if (!cJSON_IsString(ref_chg_data_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_chg_data]");
                goto end;
            }
            OpenAPI_list_add(ref_chg_dataList, ogs_strdup(ref_chg_data_local->valuestring));
        }
    }

    cJSON *ref_chg_n3g_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refChgN3gData");

    OpenAPI_list_t *ref_chg_n3g_dataList;
    if (ref_chg_n3g_data) {
        cJSON *ref_chg_n3g_data_local;
        if (!cJSON_IsArray(ref_chg_n3g_data)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_chg_n3g_data]");
            goto end;
        }
        ref_chg_n3g_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_chg_n3g_data_local, ref_chg_n3g_data) {
            if (!cJSON_IsString(ref_chg_n3g_data_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_chg_n3g_data]");
                goto end;
            }
            OpenAPI_list_add(ref_chg_n3g_dataList, ogs_strdup(ref_chg_n3g_data_local->valuestring));
        }
    }

    cJSON *ref_um_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refUmData");

    OpenAPI_list_t *ref_um_dataList;
    if (ref_um_data) {
        cJSON *ref_um_data_local;
        if (!cJSON_IsArray(ref_um_data)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_um_data]");
            goto end;
        }
        ref_um_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_um_data_local, ref_um_data) {
            if (!cJSON_IsString(ref_um_data_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_um_data]");
                goto end;
            }
            OpenAPI_list_add(ref_um_dataList, ogs_strdup(ref_um_data_local->valuestring));
        }
    }

    cJSON *ref_um_n3g_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refUmN3gData");

    OpenAPI_list_t *ref_um_n3g_dataList;
    if (ref_um_n3g_data) {
        cJSON *ref_um_n3g_data_local;
        if (!cJSON_IsArray(ref_um_n3g_data)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_um_n3g_data]");
            goto end;
        }
        ref_um_n3g_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_um_n3g_data_local, ref_um_n3g_data) {
            if (!cJSON_IsString(ref_um_n3g_data_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_um_n3g_data]");
                goto end;
            }
            OpenAPI_list_add(ref_um_n3g_dataList, ogs_strdup(ref_um_n3g_data_local->valuestring));
        }
    }

    cJSON *ref_cond_data = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refCondData");

    if (ref_cond_data) {
        if (!cJSON_IsString(ref_cond_data)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_cond_data]");
            goto end;
        }
    }

    cJSON *ref_qos_mon = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "refQosMon");

    OpenAPI_list_t *ref_qos_monList;
    if (ref_qos_mon) {
        cJSON *ref_qos_mon_local;
        if (!cJSON_IsArray(ref_qos_mon)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_qos_mon]");
            goto end;
        }
        ref_qos_monList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_qos_mon_local, ref_qos_mon) {
            if (!cJSON_IsString(ref_qos_mon_local)) {
                ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [ref_qos_mon]");
                goto end;
            }
            OpenAPI_list_add(ref_qos_monList, ogs_strdup(ref_qos_mon_local->valuestring));
        }
    }

    cJSON *addr_preser_ind = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "addrPreserInd");

    if (addr_preser_ind) {
        if (!cJSON_IsBool(addr_preser_ind)) {
            ogs_error("OpenAPI_pcc_rule_parseFromJSON() failed [addr_preser_ind]");
            goto end;
        }
    }

    cJSON *tscai_input_dl = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "tscaiInputDl");

    OpenAPI_tscai_input_container_t *tscai_input_dl_local_nonprim = NULL;
    if (tscai_input_dl) {
        tscai_input_dl_local_nonprim = OpenAPI_tscai_input_container_parseFromJSON(tscai_input_dl);
    }

    cJSON *tscai_input_ul = cJSON_GetObjectItemCaseSensitive(pcc_ruleJSON, "tscaiInputUl");

    OpenAPI_tscai_input_container_t *tscai_input_ul_local_nonprim = NULL;
    if (tscai_input_ul) {
        tscai_input_ul_local_nonprim = OpenAPI_tscai_input_container_parseFromJSON(tscai_input_ul);
    }

    pcc_rule_local_var = OpenAPI_pcc_rule_create (
        flow_infos ? flow_infosList : NULL,
        app_id ? ogs_strdup(app_id->valuestring) : NULL,
        cont_ver ? cont_ver->valuedouble : 0,
        ogs_strdup(pcc_rule_id->valuestring),
        precedence ? precedence->valuedouble : 0,
        af_sig_protocol ? af_sig_protocol_local_nonprim : NULL,
        app_reloc ? app_reloc->valueint : 0,
        ref_qos_data ? ref_qos_dataList : NULL,
        ref_alt_qos_params ? ref_alt_qos_paramsList : NULL,
        ref_tc_data ? ref_tc_dataList : NULL,
        ref_chg_data ? ref_chg_dataList : NULL,
        ref_chg_n3g_data ? ref_chg_n3g_dataList : NULL,
        ref_um_data ? ref_um_dataList : NULL,
        ref_um_n3g_data ? ref_um_n3g_dataList : NULL,
        ref_cond_data ? ogs_strdup(ref_cond_data->valuestring) : NULL,
        ref_qos_mon ? ref_qos_monList : NULL,
        addr_preser_ind ? addr_preser_ind->valueint : 0,
        tscai_input_dl ? tscai_input_dl_local_nonprim : NULL,
        tscai_input_ul ? tscai_input_ul_local_nonprim : NULL
        );

    return pcc_rule_local_var;
end:
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

