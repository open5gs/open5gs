
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_decision.h"

OpenAPI_sm_policy_decision_t *OpenAPI_sm_policy_decision_create(
    OpenAPI_list_t* sess_rules,
    bool is_pcc_rules_null,
    OpenAPI_list_t* pcc_rules,
    bool is_pcscf_rest_indication,
    int pcscf_rest_indication,
    OpenAPI_list_t* qos_decs,
    bool is_chg_decs_null,
    OpenAPI_list_t* chg_decs,
    OpenAPI_charging_information_t *charging_info,
    OpenAPI_list_t* traff_cont_decs,
    bool is_um_decs_null,
    OpenAPI_list_t* um_decs,
    OpenAPI_list_t* qos_chars,
    bool is_qos_mon_decs_null,
    OpenAPI_list_t* qos_mon_decs,
    bool is_reflective_qo_s_timer,
    int reflective_qo_s_timer,
    bool is_conds_null,
    OpenAPI_list_t* conds,
    char *revalidation_time,
    bool is_offline,
    int offline,
    bool is_online,
    int online,
    bool is_offline_ch_only,
    int offline_ch_only,
    bool is_policy_ctrl_req_triggers_null,
    OpenAPI_list_t *policy_ctrl_req_triggers,
    OpenAPI_list_t *last_req_rule_data,
    OpenAPI_requested_usage_data_t *last_req_usage_data,
    bool is_pra_infos_null,
    OpenAPI_list_t* pra_infos,
    bool is_ipv4_index,
    int ipv4_index,
    bool is_ipv6_index,
    int ipv6_index,
    OpenAPI_qos_flow_usage_e qos_flow_usage,
    OpenAPI_sm_policy_association_release_cause_e rel_cause,
    char *supp_feat,
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts,
    bool is_red_sess_indication,
    int red_sess_indication
)
{
    OpenAPI_sm_policy_decision_t *sm_policy_decision_local_var = ogs_malloc(sizeof(OpenAPI_sm_policy_decision_t));
    ogs_assert(sm_policy_decision_local_var);

    sm_policy_decision_local_var->sess_rules = sess_rules;
    sm_policy_decision_local_var->is_pcc_rules_null = is_pcc_rules_null;
    sm_policy_decision_local_var->pcc_rules = pcc_rules;
    sm_policy_decision_local_var->is_pcscf_rest_indication = is_pcscf_rest_indication;
    sm_policy_decision_local_var->pcscf_rest_indication = pcscf_rest_indication;
    sm_policy_decision_local_var->qos_decs = qos_decs;
    sm_policy_decision_local_var->is_chg_decs_null = is_chg_decs_null;
    sm_policy_decision_local_var->chg_decs = chg_decs;
    sm_policy_decision_local_var->charging_info = charging_info;
    sm_policy_decision_local_var->traff_cont_decs = traff_cont_decs;
    sm_policy_decision_local_var->is_um_decs_null = is_um_decs_null;
    sm_policy_decision_local_var->um_decs = um_decs;
    sm_policy_decision_local_var->qos_chars = qos_chars;
    sm_policy_decision_local_var->is_qos_mon_decs_null = is_qos_mon_decs_null;
    sm_policy_decision_local_var->qos_mon_decs = qos_mon_decs;
    sm_policy_decision_local_var->is_reflective_qo_s_timer = is_reflective_qo_s_timer;
    sm_policy_decision_local_var->reflective_qo_s_timer = reflective_qo_s_timer;
    sm_policy_decision_local_var->is_conds_null = is_conds_null;
    sm_policy_decision_local_var->conds = conds;
    sm_policy_decision_local_var->revalidation_time = revalidation_time;
    sm_policy_decision_local_var->is_offline = is_offline;
    sm_policy_decision_local_var->offline = offline;
    sm_policy_decision_local_var->is_online = is_online;
    sm_policy_decision_local_var->online = online;
    sm_policy_decision_local_var->is_offline_ch_only = is_offline_ch_only;
    sm_policy_decision_local_var->offline_ch_only = offline_ch_only;
    sm_policy_decision_local_var->is_policy_ctrl_req_triggers_null = is_policy_ctrl_req_triggers_null;
    sm_policy_decision_local_var->policy_ctrl_req_triggers = policy_ctrl_req_triggers;
    sm_policy_decision_local_var->last_req_rule_data = last_req_rule_data;
    sm_policy_decision_local_var->last_req_usage_data = last_req_usage_data;
    sm_policy_decision_local_var->is_pra_infos_null = is_pra_infos_null;
    sm_policy_decision_local_var->pra_infos = pra_infos;
    sm_policy_decision_local_var->is_ipv4_index = is_ipv4_index;
    sm_policy_decision_local_var->ipv4_index = ipv4_index;
    sm_policy_decision_local_var->is_ipv6_index = is_ipv6_index;
    sm_policy_decision_local_var->ipv6_index = ipv6_index;
    sm_policy_decision_local_var->qos_flow_usage = qos_flow_usage;
    sm_policy_decision_local_var->rel_cause = rel_cause;
    sm_policy_decision_local_var->supp_feat = supp_feat;
    sm_policy_decision_local_var->tsn_bridge_man_cont = tsn_bridge_man_cont;
    sm_policy_decision_local_var->tsn_port_man_cont_dstt = tsn_port_man_cont_dstt;
    sm_policy_decision_local_var->tsn_port_man_cont_nwtts = tsn_port_man_cont_nwtts;
    sm_policy_decision_local_var->is_red_sess_indication = is_red_sess_indication;
    sm_policy_decision_local_var->red_sess_indication = red_sess_indication;

    return sm_policy_decision_local_var;
}

void OpenAPI_sm_policy_decision_free(OpenAPI_sm_policy_decision_t *sm_policy_decision)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_policy_decision) {
        return;
    }
    if (sm_policy_decision->sess_rules) {
        OpenAPI_list_for_each(sm_policy_decision->sess_rules, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_session_rule_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_decision->sess_rules);
        sm_policy_decision->sess_rules = NULL;
    }
    if (sm_policy_decision->pcc_rules) {
        OpenAPI_list_for_each(sm_policy_decision->pcc_rules, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_pcc_rule_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_decision->pcc_rules);
        sm_policy_decision->pcc_rules = NULL;
    }
    if (sm_policy_decision->qos_decs) {
        OpenAPI_list_for_each(sm_policy_decision->qos_decs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_qos_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_decision->qos_decs);
        sm_policy_decision->qos_decs = NULL;
    }
    if (sm_policy_decision->chg_decs) {
        OpenAPI_list_for_each(sm_policy_decision->chg_decs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_charging_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_decision->chg_decs);
        sm_policy_decision->chg_decs = NULL;
    }
    if (sm_policy_decision->charging_info) {
        OpenAPI_charging_information_free(sm_policy_decision->charging_info);
        sm_policy_decision->charging_info = NULL;
    }
    if (sm_policy_decision->traff_cont_decs) {
        OpenAPI_list_for_each(sm_policy_decision->traff_cont_decs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_traffic_control_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_decision->traff_cont_decs);
        sm_policy_decision->traff_cont_decs = NULL;
    }
    if (sm_policy_decision->um_decs) {
        OpenAPI_list_for_each(sm_policy_decision->um_decs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_usage_monitoring_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_decision->um_decs);
        sm_policy_decision->um_decs = NULL;
    }
    if (sm_policy_decision->qos_chars) {
        OpenAPI_list_for_each(sm_policy_decision->qos_chars, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_qos_characteristics_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_decision->qos_chars);
        sm_policy_decision->qos_chars = NULL;
    }
    if (sm_policy_decision->qos_mon_decs) {
        OpenAPI_list_for_each(sm_policy_decision->qos_mon_decs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_qos_monitoring_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_decision->qos_mon_decs);
        sm_policy_decision->qos_mon_decs = NULL;
    }
    if (sm_policy_decision->conds) {
        OpenAPI_list_for_each(sm_policy_decision->conds, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_condition_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_decision->conds);
        sm_policy_decision->conds = NULL;
    }
    if (sm_policy_decision->revalidation_time) {
        ogs_free(sm_policy_decision->revalidation_time);
        sm_policy_decision->revalidation_time = NULL;
    }
    if (sm_policy_decision->policy_ctrl_req_triggers) {
        OpenAPI_list_free(sm_policy_decision->policy_ctrl_req_triggers);
        sm_policy_decision->policy_ctrl_req_triggers = NULL;
    }
    if (sm_policy_decision->last_req_rule_data) {
        OpenAPI_list_for_each(sm_policy_decision->last_req_rule_data, node) {
            OpenAPI_requested_rule_data_free(node->data);
        }
        OpenAPI_list_free(sm_policy_decision->last_req_rule_data);
        sm_policy_decision->last_req_rule_data = NULL;
    }
    if (sm_policy_decision->last_req_usage_data) {
        OpenAPI_requested_usage_data_free(sm_policy_decision->last_req_usage_data);
        sm_policy_decision->last_req_usage_data = NULL;
    }
    if (sm_policy_decision->pra_infos) {
        OpenAPI_list_for_each(sm_policy_decision->pra_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_rm_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_decision->pra_infos);
        sm_policy_decision->pra_infos = NULL;
    }
    if (sm_policy_decision->supp_feat) {
        ogs_free(sm_policy_decision->supp_feat);
        sm_policy_decision->supp_feat = NULL;
    }
    if (sm_policy_decision->tsn_bridge_man_cont) {
        OpenAPI_bridge_management_container_free(sm_policy_decision->tsn_bridge_man_cont);
        sm_policy_decision->tsn_bridge_man_cont = NULL;
    }
    if (sm_policy_decision->tsn_port_man_cont_dstt) {
        OpenAPI_port_management_container_free(sm_policy_decision->tsn_port_man_cont_dstt);
        sm_policy_decision->tsn_port_man_cont_dstt = NULL;
    }
    if (sm_policy_decision->tsn_port_man_cont_nwtts) {
        OpenAPI_list_for_each(sm_policy_decision->tsn_port_man_cont_nwtts, node) {
            OpenAPI_port_management_container_free(node->data);
        }
        OpenAPI_list_free(sm_policy_decision->tsn_port_man_cont_nwtts);
        sm_policy_decision->tsn_port_man_cont_nwtts = NULL;
    }
    ogs_free(sm_policy_decision);
}

cJSON *OpenAPI_sm_policy_decision_convertToJSON(OpenAPI_sm_policy_decision_t *sm_policy_decision)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_policy_decision == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [SmPolicyDecision]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_policy_decision->sess_rules) {
    cJSON *sess_rules = cJSON_AddObjectToObject(item, "sessRules");
    if (sess_rules == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [sess_rules]");
        goto end;
    }
    cJSON *localMapObject = sess_rules;
    if (sm_policy_decision->sess_rules) {
        OpenAPI_list_for_each(sm_policy_decision->sess_rules, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [sess_rules]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [sess_rules]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_session_rule_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (sm_policy_decision->pcc_rules) {
    cJSON *pcc_rules = cJSON_AddObjectToObject(item, "pccRules");
    if (pcc_rules == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [pcc_rules]");
        goto end;
    }
    cJSON *localMapObject = pcc_rules;
    if (sm_policy_decision->pcc_rules) {
        OpenAPI_list_for_each(sm_policy_decision->pcc_rules, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [pcc_rules]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [pcc_rules]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_pcc_rule_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    } else if (sm_policy_decision->is_pcc_rules_null) {
        if (cJSON_AddNullToObject(item, "pccRules") == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [pcc_rules]");
            goto end;
        }
    }

    if (sm_policy_decision->is_pcscf_rest_indication) {
    if (cJSON_AddBoolToObject(item, "pcscfRestIndication", sm_policy_decision->pcscf_rest_indication) == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [pcscf_rest_indication]");
        goto end;
    }
    }

    if (sm_policy_decision->qos_decs) {
    cJSON *qos_decs = cJSON_AddObjectToObject(item, "qosDecs");
    if (qos_decs == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_decs]");
        goto end;
    }
    cJSON *localMapObject = qos_decs;
    if (sm_policy_decision->qos_decs) {
        OpenAPI_list_for_each(sm_policy_decision->qos_decs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_decs]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_decs]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_qos_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (sm_policy_decision->chg_decs) {
    cJSON *chg_decs = cJSON_AddObjectToObject(item, "chgDecs");
    if (chg_decs == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [chg_decs]");
        goto end;
    }
    cJSON *localMapObject = chg_decs;
    if (sm_policy_decision->chg_decs) {
        OpenAPI_list_for_each(sm_policy_decision->chg_decs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [chg_decs]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [chg_decs]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_charging_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    } else if (sm_policy_decision->is_chg_decs_null) {
        if (cJSON_AddNullToObject(item, "chgDecs") == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [chg_decs]");
            goto end;
        }
    }

    if (sm_policy_decision->charging_info) {
    cJSON *charging_info_local_JSON = OpenAPI_charging_information_convertToJSON(sm_policy_decision->charging_info);
    if (charging_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [charging_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "chargingInfo", charging_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [charging_info]");
        goto end;
    }
    }

    if (sm_policy_decision->traff_cont_decs) {
    cJSON *traff_cont_decs = cJSON_AddObjectToObject(item, "traffContDecs");
    if (traff_cont_decs == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [traff_cont_decs]");
        goto end;
    }
    cJSON *localMapObject = traff_cont_decs;
    if (sm_policy_decision->traff_cont_decs) {
        OpenAPI_list_for_each(sm_policy_decision->traff_cont_decs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [traff_cont_decs]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [traff_cont_decs]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_traffic_control_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (sm_policy_decision->um_decs) {
    cJSON *um_decs = cJSON_AddObjectToObject(item, "umDecs");
    if (um_decs == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [um_decs]");
        goto end;
    }
    cJSON *localMapObject = um_decs;
    if (sm_policy_decision->um_decs) {
        OpenAPI_list_for_each(sm_policy_decision->um_decs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [um_decs]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [um_decs]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_usage_monitoring_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    } else if (sm_policy_decision->is_um_decs_null) {
        if (cJSON_AddNullToObject(item, "umDecs") == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [um_decs]");
            goto end;
        }
    }

    if (sm_policy_decision->qos_chars) {
    cJSON *qos_chars = cJSON_AddObjectToObject(item, "qosChars");
    if (qos_chars == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_chars]");
        goto end;
    }
    cJSON *localMapObject = qos_chars;
    if (sm_policy_decision->qos_chars) {
        OpenAPI_list_for_each(sm_policy_decision->qos_chars, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_chars]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_chars]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_qos_characteristics_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (sm_policy_decision->qos_mon_decs) {
    cJSON *qos_mon_decs = cJSON_AddObjectToObject(item, "qosMonDecs");
    if (qos_mon_decs == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_mon_decs]");
        goto end;
    }
    cJSON *localMapObject = qos_mon_decs;
    if (sm_policy_decision->qos_mon_decs) {
        OpenAPI_list_for_each(sm_policy_decision->qos_mon_decs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_mon_decs]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_mon_decs]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_qos_monitoring_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    } else if (sm_policy_decision->is_qos_mon_decs_null) {
        if (cJSON_AddNullToObject(item, "qosMonDecs") == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_mon_decs]");
            goto end;
        }
    }

    if (sm_policy_decision->is_reflective_qo_s_timer) {
    if (cJSON_AddNumberToObject(item, "reflectiveQoSTimer", sm_policy_decision->reflective_qo_s_timer) == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [reflective_qo_s_timer]");
        goto end;
    }
    }

    if (sm_policy_decision->conds) {
    cJSON *conds = cJSON_AddObjectToObject(item, "conds");
    if (conds == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [conds]");
        goto end;
    }
    cJSON *localMapObject = conds;
    if (sm_policy_decision->conds) {
        OpenAPI_list_for_each(sm_policy_decision->conds, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [conds]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [conds]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_condition_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    } else if (sm_policy_decision->is_conds_null) {
        if (cJSON_AddNullToObject(item, "conds") == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [conds]");
            goto end;
        }
    }

    if (sm_policy_decision->revalidation_time) {
    if (cJSON_AddStringToObject(item, "revalidationTime", sm_policy_decision->revalidation_time) == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [revalidation_time]");
        goto end;
    }
    }

    if (sm_policy_decision->is_offline) {
    if (cJSON_AddBoolToObject(item, "offline", sm_policy_decision->offline) == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [offline]");
        goto end;
    }
    }

    if (sm_policy_decision->is_online) {
    if (cJSON_AddBoolToObject(item, "online", sm_policy_decision->online) == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [online]");
        goto end;
    }
    }

    if (sm_policy_decision->is_offline_ch_only) {
    if (cJSON_AddBoolToObject(item, "offlineChOnly", sm_policy_decision->offline_ch_only) == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [offline_ch_only]");
        goto end;
    }
    }

    if (sm_policy_decision->policy_ctrl_req_triggers != OpenAPI_policy_control_request_trigger_NULL) {
    cJSON *policy_ctrl_req_triggersList = cJSON_AddArrayToObject(item, "policyCtrlReqTriggers");
    if (policy_ctrl_req_triggersList == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [policy_ctrl_req_triggers]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_decision->policy_ctrl_req_triggers, node) {
        if (cJSON_AddStringToObject(policy_ctrl_req_triggersList, "", OpenAPI_policy_control_request_trigger_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [policy_ctrl_req_triggers]");
            goto end;
        }
    }
    } else if (sm_policy_decision->is_policy_ctrl_req_triggers_null) {
        if (cJSON_AddNullToObject(item, "policyCtrlReqTriggers") == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [policy_ctrl_req_triggers]");
            goto end;
        }
    }

    if (sm_policy_decision->last_req_rule_data) {
    cJSON *last_req_rule_dataList = cJSON_AddArrayToObject(item, "lastReqRuleData");
    if (last_req_rule_dataList == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [last_req_rule_data]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_decision->last_req_rule_data, node) {
        cJSON *itemLocal = OpenAPI_requested_rule_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [last_req_rule_data]");
            goto end;
        }
        cJSON_AddItemToArray(last_req_rule_dataList, itemLocal);
    }
    }

    if (sm_policy_decision->last_req_usage_data) {
    cJSON *last_req_usage_data_local_JSON = OpenAPI_requested_usage_data_convertToJSON(sm_policy_decision->last_req_usage_data);
    if (last_req_usage_data_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [last_req_usage_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lastReqUsageData", last_req_usage_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [last_req_usage_data]");
        goto end;
    }
    }

    if (sm_policy_decision->pra_infos) {
    cJSON *pra_infos = cJSON_AddObjectToObject(item, "praInfos");
    if (pra_infos == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [pra_infos]");
        goto end;
    }
    cJSON *localMapObject = pra_infos;
    if (sm_policy_decision->pra_infos) {
        OpenAPI_list_for_each(sm_policy_decision->pra_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [pra_infos]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [pra_infos]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_rm_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    } else if (sm_policy_decision->is_pra_infos_null) {
        if (cJSON_AddNullToObject(item, "praInfos") == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [pra_infos]");
            goto end;
        }
    }

    if (sm_policy_decision->is_ipv4_index) {
    if (cJSON_AddNumberToObject(item, "ipv4Index", sm_policy_decision->ipv4_index) == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [ipv4_index]");
        goto end;
    }
    }

    if (sm_policy_decision->is_ipv6_index) {
    if (cJSON_AddNumberToObject(item, "ipv6Index", sm_policy_decision->ipv6_index) == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [ipv6_index]");
        goto end;
    }
    }

    if (sm_policy_decision->qos_flow_usage != OpenAPI_qos_flow_usage_NULL) {
    if (cJSON_AddStringToObject(item, "qosFlowUsage", OpenAPI_qos_flow_usage_ToString(sm_policy_decision->qos_flow_usage)) == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_flow_usage]");
        goto end;
    }
    }

    if (sm_policy_decision->rel_cause != OpenAPI_sm_policy_association_release_cause_NULL) {
    if (cJSON_AddStringToObject(item, "relCause", OpenAPI_sm_policy_association_release_cause_ToString(sm_policy_decision->rel_cause)) == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [rel_cause]");
        goto end;
    }
    }

    if (sm_policy_decision->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", sm_policy_decision->supp_feat) == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

    if (sm_policy_decision->tsn_bridge_man_cont) {
    cJSON *tsn_bridge_man_cont_local_JSON = OpenAPI_bridge_management_container_convertToJSON(sm_policy_decision->tsn_bridge_man_cont);
    if (tsn_bridge_man_cont_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnBridgeManCont", tsn_bridge_man_cont_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [tsn_bridge_man_cont]");
        goto end;
    }
    }

    if (sm_policy_decision->tsn_port_man_cont_dstt) {
    cJSON *tsn_port_man_cont_dstt_local_JSON = OpenAPI_port_management_container_convertToJSON(sm_policy_decision->tsn_port_man_cont_dstt);
    if (tsn_port_man_cont_dstt_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnPortManContDstt", tsn_port_man_cont_dstt_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    }

    if (sm_policy_decision->tsn_port_man_cont_nwtts) {
    cJSON *tsn_port_man_cont_nwttsList = cJSON_AddArrayToObject(item, "tsnPortManContNwtts");
    if (tsn_port_man_cont_nwttsList == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [tsn_port_man_cont_nwtts]");
        goto end;
    }
    OpenAPI_list_for_each(sm_policy_decision->tsn_port_man_cont_nwtts, node) {
        cJSON *itemLocal = OpenAPI_port_management_container_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }
        cJSON_AddItemToArray(tsn_port_man_cont_nwttsList, itemLocal);
    }
    }

    if (sm_policy_decision->is_red_sess_indication) {
    if (cJSON_AddBoolToObject(item, "redSessIndication", sm_policy_decision->red_sess_indication) == NULL) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [red_sess_indication]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_policy_decision_t *OpenAPI_sm_policy_decision_parseFromJSON(cJSON *sm_policy_decisionJSON)
{
    OpenAPI_sm_policy_decision_t *sm_policy_decision_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sess_rules = NULL;
    OpenAPI_list_t *sess_rulesList = NULL;
    cJSON *pcc_rules = NULL;
    OpenAPI_list_t *pcc_rulesList = NULL;
    cJSON *pcscf_rest_indication = NULL;
    cJSON *qos_decs = NULL;
    OpenAPI_list_t *qos_decsList = NULL;
    cJSON *chg_decs = NULL;
    OpenAPI_list_t *chg_decsList = NULL;
    cJSON *charging_info = NULL;
    OpenAPI_charging_information_t *charging_info_local_nonprim = NULL;
    cJSON *traff_cont_decs = NULL;
    OpenAPI_list_t *traff_cont_decsList = NULL;
    cJSON *um_decs = NULL;
    OpenAPI_list_t *um_decsList = NULL;
    cJSON *qos_chars = NULL;
    OpenAPI_list_t *qos_charsList = NULL;
    cJSON *qos_mon_decs = NULL;
    OpenAPI_list_t *qos_mon_decsList = NULL;
    cJSON *reflective_qo_s_timer = NULL;
    cJSON *conds = NULL;
    OpenAPI_list_t *condsList = NULL;
    cJSON *revalidation_time = NULL;
    cJSON *offline = NULL;
    cJSON *online = NULL;
    cJSON *offline_ch_only = NULL;
    cJSON *policy_ctrl_req_triggers = NULL;
    OpenAPI_list_t *policy_ctrl_req_triggersList = NULL;
    cJSON *last_req_rule_data = NULL;
    OpenAPI_list_t *last_req_rule_dataList = NULL;
    cJSON *last_req_usage_data = NULL;
    OpenAPI_requested_usage_data_t *last_req_usage_data_local_nonprim = NULL;
    cJSON *pra_infos = NULL;
    OpenAPI_list_t *pra_infosList = NULL;
    cJSON *ipv4_index = NULL;
    cJSON *ipv6_index = NULL;
    cJSON *qos_flow_usage = NULL;
    OpenAPI_qos_flow_usage_e qos_flow_usageVariable = 0;
    cJSON *rel_cause = NULL;
    OpenAPI_sm_policy_association_release_cause_e rel_causeVariable = 0;
    cJSON *supp_feat = NULL;
    cJSON *tsn_bridge_man_cont = NULL;
    OpenAPI_bridge_management_container_t *tsn_bridge_man_cont_local_nonprim = NULL;
    cJSON *tsn_port_man_cont_dstt = NULL;
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt_local_nonprim = NULL;
    cJSON *tsn_port_man_cont_nwtts = NULL;
    OpenAPI_list_t *tsn_port_man_cont_nwttsList = NULL;
    cJSON *red_sess_indication = NULL;
    sess_rules = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "sessRules");
    if (sess_rules) {
        cJSON *sess_rules_local_map = NULL;
        if (!cJSON_IsObject(sess_rules) && !cJSON_IsNull(sess_rules)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [sess_rules]");
            goto end;
        }
        if (cJSON_IsObject(sess_rules)) {
            sess_rulesList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(sess_rules_local_map, sess_rules) {
                cJSON *localMapObject = sess_rules_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_session_rule_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(sess_rulesList, localMapKeyPair);
            }
        }
    }

    pcc_rules = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "pccRules");
    if (pcc_rules) {
    if (!cJSON_IsNull(pcc_rules)) {
        cJSON *pcc_rules_local_map = NULL;
        if (!cJSON_IsObject(pcc_rules) && !cJSON_IsNull(pcc_rules)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [pcc_rules]");
            goto end;
        }
        if (cJSON_IsObject(pcc_rules)) {
            pcc_rulesList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(pcc_rules_local_map, pcc_rules) {
                cJSON *localMapObject = pcc_rules_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_pcc_rule_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(pcc_rulesList, localMapKeyPair);
            }
        }
    }
    }

    pcscf_rest_indication = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "pcscfRestIndication");
    if (pcscf_rest_indication) {
    if (!cJSON_IsBool(pcscf_rest_indication)) {
        ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [pcscf_rest_indication]");
        goto end;
    }
    }

    qos_decs = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "qosDecs");
    if (qos_decs) {
        cJSON *qos_decs_local_map = NULL;
        if (!cJSON_IsObject(qos_decs) && !cJSON_IsNull(qos_decs)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [qos_decs]");
            goto end;
        }
        if (cJSON_IsObject(qos_decs)) {
            qos_decsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(qos_decs_local_map, qos_decs) {
                cJSON *localMapObject = qos_decs_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_qos_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(qos_decsList, localMapKeyPair);
            }
        }
    }

    chg_decs = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "chgDecs");
    if (chg_decs) {
    if (!cJSON_IsNull(chg_decs)) {
        cJSON *chg_decs_local_map = NULL;
        if (!cJSON_IsObject(chg_decs) && !cJSON_IsNull(chg_decs)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [chg_decs]");
            goto end;
        }
        if (cJSON_IsObject(chg_decs)) {
            chg_decsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(chg_decs_local_map, chg_decs) {
                cJSON *localMapObject = chg_decs_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_charging_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(chg_decsList, localMapKeyPair);
            }
        }
    }
    }

    charging_info = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "chargingInfo");
    if (charging_info) {
    charging_info_local_nonprim = OpenAPI_charging_information_parseFromJSON(charging_info);
    if (!charging_info_local_nonprim) {
        ogs_error("OpenAPI_charging_information_parseFromJSON failed [charging_info]");
        goto end;
    }
    }

    traff_cont_decs = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "traffContDecs");
    if (traff_cont_decs) {
        cJSON *traff_cont_decs_local_map = NULL;
        if (!cJSON_IsObject(traff_cont_decs) && !cJSON_IsNull(traff_cont_decs)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [traff_cont_decs]");
            goto end;
        }
        if (cJSON_IsObject(traff_cont_decs)) {
            traff_cont_decsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(traff_cont_decs_local_map, traff_cont_decs) {
                cJSON *localMapObject = traff_cont_decs_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_traffic_control_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(traff_cont_decsList, localMapKeyPair);
            }
        }
    }

    um_decs = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "umDecs");
    if (um_decs) {
    if (!cJSON_IsNull(um_decs)) {
        cJSON *um_decs_local_map = NULL;
        if (!cJSON_IsObject(um_decs) && !cJSON_IsNull(um_decs)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [um_decs]");
            goto end;
        }
        if (cJSON_IsObject(um_decs)) {
            um_decsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(um_decs_local_map, um_decs) {
                cJSON *localMapObject = um_decs_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_usage_monitoring_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(um_decsList, localMapKeyPair);
            }
        }
    }
    }

    qos_chars = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "qosChars");
    if (qos_chars) {
        cJSON *qos_chars_local_map = NULL;
        if (!cJSON_IsObject(qos_chars) && !cJSON_IsNull(qos_chars)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [qos_chars]");
            goto end;
        }
        if (cJSON_IsObject(qos_chars)) {
            qos_charsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(qos_chars_local_map, qos_chars) {
                cJSON *localMapObject = qos_chars_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_qos_characteristics_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(qos_charsList, localMapKeyPair);
            }
        }
    }

    qos_mon_decs = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "qosMonDecs");
    if (qos_mon_decs) {
    if (!cJSON_IsNull(qos_mon_decs)) {
        cJSON *qos_mon_decs_local_map = NULL;
        if (!cJSON_IsObject(qos_mon_decs) && !cJSON_IsNull(qos_mon_decs)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [qos_mon_decs]");
            goto end;
        }
        if (cJSON_IsObject(qos_mon_decs)) {
            qos_mon_decsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(qos_mon_decs_local_map, qos_mon_decs) {
                cJSON *localMapObject = qos_mon_decs_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_qos_monitoring_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(qos_mon_decsList, localMapKeyPair);
            }
        }
    }
    }

    reflective_qo_s_timer = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "reflectiveQoSTimer");
    if (reflective_qo_s_timer) {
    if (!cJSON_IsNumber(reflective_qo_s_timer)) {
        ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [reflective_qo_s_timer]");
        goto end;
    }
    }

    conds = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "conds");
    if (conds) {
    if (!cJSON_IsNull(conds)) {
        cJSON *conds_local_map = NULL;
        if (!cJSON_IsObject(conds) && !cJSON_IsNull(conds)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [conds]");
            goto end;
        }
        if (cJSON_IsObject(conds)) {
            condsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(conds_local_map, conds) {
                cJSON *localMapObject = conds_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_condition_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(condsList, localMapKeyPair);
            }
        }
    }
    }

    revalidation_time = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "revalidationTime");
    if (revalidation_time) {
    if (!cJSON_IsString(revalidation_time) && !cJSON_IsNull(revalidation_time)) {
        ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [revalidation_time]");
        goto end;
    }
    }

    offline = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "offline");
    if (offline) {
    if (!cJSON_IsBool(offline)) {
        ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [offline]");
        goto end;
    }
    }

    online = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "online");
    if (online) {
    if (!cJSON_IsBool(online)) {
        ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [online]");
        goto end;
    }
    }

    offline_ch_only = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "offlineChOnly");
    if (offline_ch_only) {
    if (!cJSON_IsBool(offline_ch_only)) {
        ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [offline_ch_only]");
        goto end;
    }
    }

    policy_ctrl_req_triggers = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "policyCtrlReqTriggers");
    if (policy_ctrl_req_triggers) {
    if (!cJSON_IsNull(policy_ctrl_req_triggers)) {
        cJSON *policy_ctrl_req_triggers_local = NULL;
        if (!cJSON_IsArray(policy_ctrl_req_triggers)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [policy_ctrl_req_triggers]");
            goto end;
        }

        policy_ctrl_req_triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(policy_ctrl_req_triggers_local, policy_ctrl_req_triggers) {
            OpenAPI_policy_control_request_trigger_e localEnum = OpenAPI_policy_control_request_trigger_NULL;
            if (!cJSON_IsString(policy_ctrl_req_triggers_local)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [policy_ctrl_req_triggers]");
                goto end;
            }
            localEnum = OpenAPI_policy_control_request_trigger_FromString(policy_ctrl_req_triggers_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"policy_ctrl_req_triggers\" is not supported. Ignoring it ...",
                         policy_ctrl_req_triggers_local->valuestring);
            } else {
                OpenAPI_list_add(policy_ctrl_req_triggersList, (void *)localEnum);
            }
        }
        if (policy_ctrl_req_triggersList->count == 0) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed: Expected policy_ctrl_req_triggersList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }
    }

    last_req_rule_data = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "lastReqRuleData");
    if (last_req_rule_data) {
        cJSON *last_req_rule_data_local = NULL;
        if (!cJSON_IsArray(last_req_rule_data)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [last_req_rule_data]");
            goto end;
        }

        last_req_rule_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(last_req_rule_data_local, last_req_rule_data) {
            if (!cJSON_IsObject(last_req_rule_data_local)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [last_req_rule_data]");
                goto end;
            }
            OpenAPI_requested_rule_data_t *last_req_rule_dataItem = OpenAPI_requested_rule_data_parseFromJSON(last_req_rule_data_local);
            if (!last_req_rule_dataItem) {
                ogs_error("No last_req_rule_dataItem");
                goto end;
            }
            OpenAPI_list_add(last_req_rule_dataList, last_req_rule_dataItem);
        }
    }

    last_req_usage_data = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "lastReqUsageData");
    if (last_req_usage_data) {
    last_req_usage_data_local_nonprim = OpenAPI_requested_usage_data_parseFromJSON(last_req_usage_data);
    if (!last_req_usage_data_local_nonprim) {
        ogs_error("OpenAPI_requested_usage_data_parseFromJSON failed [last_req_usage_data]");
        goto end;
    }
    }

    pra_infos = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "praInfos");
    if (pra_infos) {
    if (!cJSON_IsNull(pra_infos)) {
        cJSON *pra_infos_local_map = NULL;
        if (!cJSON_IsObject(pra_infos) && !cJSON_IsNull(pra_infos)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [pra_infos]");
            goto end;
        }
        if (cJSON_IsObject(pra_infos)) {
            pra_infosList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(pra_infos_local_map, pra_infos) {
                cJSON *localMapObject = pra_infos_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_rm_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(pra_infosList, localMapKeyPair);
            }
        }
    }
    }

    ipv4_index = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "ipv4Index");
    if (ipv4_index) {
    if (!cJSON_IsNumber(ipv4_index)) {
        ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [ipv4_index]");
        goto end;
    }
    }

    ipv6_index = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "ipv6Index");
    if (ipv6_index) {
    if (!cJSON_IsNumber(ipv6_index)) {
        ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [ipv6_index]");
        goto end;
    }
    }

    qos_flow_usage = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "qosFlowUsage");
    if (qos_flow_usage) {
    if (!cJSON_IsString(qos_flow_usage)) {
        ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [qos_flow_usage]");
        goto end;
    }
    qos_flow_usageVariable = OpenAPI_qos_flow_usage_FromString(qos_flow_usage->valuestring);
    }

    rel_cause = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "relCause");
    if (rel_cause) {
    if (!cJSON_IsString(rel_cause)) {
        ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [rel_cause]");
        goto end;
    }
    rel_causeVariable = OpenAPI_sm_policy_association_release_cause_FromString(rel_cause->valuestring);
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    tsn_bridge_man_cont = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "tsnBridgeManCont");
    if (tsn_bridge_man_cont) {
    tsn_bridge_man_cont_local_nonprim = OpenAPI_bridge_management_container_parseFromJSON(tsn_bridge_man_cont);
    if (!tsn_bridge_man_cont_local_nonprim) {
        ogs_error("OpenAPI_bridge_management_container_parseFromJSON failed [tsn_bridge_man_cont]");
        goto end;
    }
    }

    tsn_port_man_cont_dstt = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "tsnPortManContDstt");
    if (tsn_port_man_cont_dstt) {
    tsn_port_man_cont_dstt_local_nonprim = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_dstt);
    if (!tsn_port_man_cont_dstt_local_nonprim) {
        ogs_error("OpenAPI_port_management_container_parseFromJSON failed [tsn_port_man_cont_dstt]");
        goto end;
    }
    }

    tsn_port_man_cont_nwtts = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "tsnPortManContNwtts");
    if (tsn_port_man_cont_nwtts) {
        cJSON *tsn_port_man_cont_nwtts_local = NULL;
        if (!cJSON_IsArray(tsn_port_man_cont_nwtts)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }

        tsn_port_man_cont_nwttsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tsn_port_man_cont_nwtts_local, tsn_port_man_cont_nwtts) {
            if (!cJSON_IsObject(tsn_port_man_cont_nwtts_local)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
                goto end;
            }
            OpenAPI_port_management_container_t *tsn_port_man_cont_nwttsItem = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_nwtts_local);
            if (!tsn_port_man_cont_nwttsItem) {
                ogs_error("No tsn_port_man_cont_nwttsItem");
                goto end;
            }
            OpenAPI_list_add(tsn_port_man_cont_nwttsList, tsn_port_man_cont_nwttsItem);
        }
    }

    red_sess_indication = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "redSessIndication");
    if (red_sess_indication) {
    if (!cJSON_IsBool(red_sess_indication)) {
        ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [red_sess_indication]");
        goto end;
    }
    }

    sm_policy_decision_local_var = OpenAPI_sm_policy_decision_create (
        sess_rules ? sess_rulesList : NULL,
        pcc_rules && cJSON_IsNull(pcc_rules) ? true : false,
        pcc_rules ? pcc_rulesList : NULL,
        pcscf_rest_indication ? true : false,
        pcscf_rest_indication ? pcscf_rest_indication->valueint : 0,
        qos_decs ? qos_decsList : NULL,
        chg_decs && cJSON_IsNull(chg_decs) ? true : false,
        chg_decs ? chg_decsList : NULL,
        charging_info ? charging_info_local_nonprim : NULL,
        traff_cont_decs ? traff_cont_decsList : NULL,
        um_decs && cJSON_IsNull(um_decs) ? true : false,
        um_decs ? um_decsList : NULL,
        qos_chars ? qos_charsList : NULL,
        qos_mon_decs && cJSON_IsNull(qos_mon_decs) ? true : false,
        qos_mon_decs ? qos_mon_decsList : NULL,
        reflective_qo_s_timer ? true : false,
        reflective_qo_s_timer ? reflective_qo_s_timer->valuedouble : 0,
        conds && cJSON_IsNull(conds) ? true : false,
        conds ? condsList : NULL,
        revalidation_time && !cJSON_IsNull(revalidation_time) ? ogs_strdup(revalidation_time->valuestring) : NULL,
        offline ? true : false,
        offline ? offline->valueint : 0,
        online ? true : false,
        online ? online->valueint : 0,
        offline_ch_only ? true : false,
        offline_ch_only ? offline_ch_only->valueint : 0,
        policy_ctrl_req_triggers && cJSON_IsNull(policy_ctrl_req_triggers) ? true : false,
        policy_ctrl_req_triggers ? policy_ctrl_req_triggersList : NULL,
        last_req_rule_data ? last_req_rule_dataList : NULL,
        last_req_usage_data ? last_req_usage_data_local_nonprim : NULL,
        pra_infos && cJSON_IsNull(pra_infos) ? true : false,
        pra_infos ? pra_infosList : NULL,
        ipv4_index ? true : false,
        ipv4_index ? ipv4_index->valuedouble : 0,
        ipv6_index ? true : false,
        ipv6_index ? ipv6_index->valuedouble : 0,
        qos_flow_usage ? qos_flow_usageVariable : 0,
        rel_cause ? rel_causeVariable : 0,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        tsn_bridge_man_cont ? tsn_bridge_man_cont_local_nonprim : NULL,
        tsn_port_man_cont_dstt ? tsn_port_man_cont_dstt_local_nonprim : NULL,
        tsn_port_man_cont_nwtts ? tsn_port_man_cont_nwttsList : NULL,
        red_sess_indication ? true : false,
        red_sess_indication ? red_sess_indication->valueint : 0
    );

    return sm_policy_decision_local_var;
end:
    if (sess_rulesList) {
        OpenAPI_list_for_each(sess_rulesList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_session_rule_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sess_rulesList);
        sess_rulesList = NULL;
    }
    if (pcc_rulesList) {
        OpenAPI_list_for_each(pcc_rulesList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_pcc_rule_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pcc_rulesList);
        pcc_rulesList = NULL;
    }
    if (qos_decsList) {
        OpenAPI_list_for_each(qos_decsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_qos_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(qos_decsList);
        qos_decsList = NULL;
    }
    if (chg_decsList) {
        OpenAPI_list_for_each(chg_decsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_charging_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(chg_decsList);
        chg_decsList = NULL;
    }
    if (charging_info_local_nonprim) {
        OpenAPI_charging_information_free(charging_info_local_nonprim);
        charging_info_local_nonprim = NULL;
    }
    if (traff_cont_decsList) {
        OpenAPI_list_for_each(traff_cont_decsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_traffic_control_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(traff_cont_decsList);
        traff_cont_decsList = NULL;
    }
    if (um_decsList) {
        OpenAPI_list_for_each(um_decsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_usage_monitoring_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(um_decsList);
        um_decsList = NULL;
    }
    if (qos_charsList) {
        OpenAPI_list_for_each(qos_charsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_qos_characteristics_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(qos_charsList);
        qos_charsList = NULL;
    }
    if (qos_mon_decsList) {
        OpenAPI_list_for_each(qos_mon_decsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_qos_monitoring_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(qos_mon_decsList);
        qos_mon_decsList = NULL;
    }
    if (condsList) {
        OpenAPI_list_for_each(condsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_condition_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(condsList);
        condsList = NULL;
    }
    if (policy_ctrl_req_triggersList) {
        OpenAPI_list_free(policy_ctrl_req_triggersList);
        policy_ctrl_req_triggersList = NULL;
    }
    if (last_req_rule_dataList) {
        OpenAPI_list_for_each(last_req_rule_dataList, node) {
            OpenAPI_requested_rule_data_free(node->data);
        }
        OpenAPI_list_free(last_req_rule_dataList);
        last_req_rule_dataList = NULL;
    }
    if (last_req_usage_data_local_nonprim) {
        OpenAPI_requested_usage_data_free(last_req_usage_data_local_nonprim);
        last_req_usage_data_local_nonprim = NULL;
    }
    if (pra_infosList) {
        OpenAPI_list_for_each(pra_infosList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_rm_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pra_infosList);
        pra_infosList = NULL;
    }
    if (tsn_bridge_man_cont_local_nonprim) {
        OpenAPI_bridge_management_container_free(tsn_bridge_man_cont_local_nonprim);
        tsn_bridge_man_cont_local_nonprim = NULL;
    }
    if (tsn_port_man_cont_dstt_local_nonprim) {
        OpenAPI_port_management_container_free(tsn_port_man_cont_dstt_local_nonprim);
        tsn_port_man_cont_dstt_local_nonprim = NULL;
    }
    if (tsn_port_man_cont_nwttsList) {
        OpenAPI_list_for_each(tsn_port_man_cont_nwttsList, node) {
            OpenAPI_port_management_container_free(node->data);
        }
        OpenAPI_list_free(tsn_port_man_cont_nwttsList);
        tsn_port_man_cont_nwttsList = NULL;
    }
    return NULL;
}

OpenAPI_sm_policy_decision_t *OpenAPI_sm_policy_decision_copy(OpenAPI_sm_policy_decision_t *dst, OpenAPI_sm_policy_decision_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_policy_decision_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed");
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

    OpenAPI_sm_policy_decision_free(dst);
    dst = OpenAPI_sm_policy_decision_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

