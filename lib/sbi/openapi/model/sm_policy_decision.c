
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_decision.h"

OpenAPI_sm_policy_decision_t *OpenAPI_sm_policy_decision_create(
    OpenAPI_list_t* sess_rules,
    OpenAPI_list_t* pcc_rules,
    int pcscf_rest_indication,
    OpenAPI_list_t* qos_decs,
    OpenAPI_list_t* chg_decs,
    OpenAPI_charging_information_t *charging_info,
    OpenAPI_list_t* traff_cont_decs,
    OpenAPI_list_t* um_decs,
    OpenAPI_list_t* qos_chars,
    OpenAPI_list_t* qos_mon_decs,
    int reflective_qo_s_timer,
    OpenAPI_list_t* conds,
    char *revalidation_time,
    int offline,
    int online,
    OpenAPI_list_t *policy_ctrl_req_triggers,
    OpenAPI_list_t *last_req_rule_data,
    OpenAPI_requested_usage_data_t *last_req_usage_data,
    OpenAPI_list_t* pra_infos,
    int ipv4_index,
    int ipv6_index,
    OpenAPI_qos_flow_usage_t *qos_flow_usage,
    OpenAPI_sm_policy_association_release_cause_t *rel_cause,
    char *supp_feat,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts
    )
{
    OpenAPI_sm_policy_decision_t *sm_policy_decision_local_var = OpenAPI_malloc(sizeof(OpenAPI_sm_policy_decision_t));
    if (!sm_policy_decision_local_var) {
        return NULL;
    }
    sm_policy_decision_local_var->sess_rules = sess_rules;
    sm_policy_decision_local_var->pcc_rules = pcc_rules;
    sm_policy_decision_local_var->pcscf_rest_indication = pcscf_rest_indication;
    sm_policy_decision_local_var->qos_decs = qos_decs;
    sm_policy_decision_local_var->chg_decs = chg_decs;
    sm_policy_decision_local_var->charging_info = charging_info;
    sm_policy_decision_local_var->traff_cont_decs = traff_cont_decs;
    sm_policy_decision_local_var->um_decs = um_decs;
    sm_policy_decision_local_var->qos_chars = qos_chars;
    sm_policy_decision_local_var->qos_mon_decs = qos_mon_decs;
    sm_policy_decision_local_var->reflective_qo_s_timer = reflective_qo_s_timer;
    sm_policy_decision_local_var->conds = conds;
    sm_policy_decision_local_var->revalidation_time = revalidation_time;
    sm_policy_decision_local_var->offline = offline;
    sm_policy_decision_local_var->online = online;
    sm_policy_decision_local_var->policy_ctrl_req_triggers = policy_ctrl_req_triggers;
    sm_policy_decision_local_var->last_req_rule_data = last_req_rule_data;
    sm_policy_decision_local_var->last_req_usage_data = last_req_usage_data;
    sm_policy_decision_local_var->pra_infos = pra_infos;
    sm_policy_decision_local_var->ipv4_index = ipv4_index;
    sm_policy_decision_local_var->ipv6_index = ipv6_index;
    sm_policy_decision_local_var->qos_flow_usage = qos_flow_usage;
    sm_policy_decision_local_var->rel_cause = rel_cause;
    sm_policy_decision_local_var->supp_feat = supp_feat;
    sm_policy_decision_local_var->tsn_port_man_cont_dstt = tsn_port_man_cont_dstt;
    sm_policy_decision_local_var->tsn_port_man_cont_nwtts = tsn_port_man_cont_nwtts;

    return sm_policy_decision_local_var;
}

void OpenAPI_sm_policy_decision_free(OpenAPI_sm_policy_decision_t *sm_policy_decision)
{
    if (NULL == sm_policy_decision) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(sm_policy_decision->sess_rules, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_session_rule_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_decision->sess_rules);
    OpenAPI_list_for_each(sm_policy_decision->pcc_rules, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_pcc_rule_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_decision->pcc_rules);
    OpenAPI_list_for_each(sm_policy_decision->qos_decs, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_qos_data_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_decision->qos_decs);
    OpenAPI_list_for_each(sm_policy_decision->chg_decs, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_charging_data_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_decision->chg_decs);
    OpenAPI_charging_information_free(sm_policy_decision->charging_info);
    OpenAPI_list_for_each(sm_policy_decision->traff_cont_decs, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_traffic_control_data_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_decision->traff_cont_decs);
    OpenAPI_list_for_each(sm_policy_decision->um_decs, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_usage_monitoring_data_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_decision->um_decs);
    OpenAPI_list_for_each(sm_policy_decision->qos_chars, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_qos_characteristics_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_decision->qos_chars);
    OpenAPI_list_for_each(sm_policy_decision->qos_mon_decs, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_qos_monitoring_data_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_decision->qos_mon_decs);
    OpenAPI_list_for_each(sm_policy_decision->conds, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_condition_data_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_decision->conds);
    ogs_free(sm_policy_decision->revalidation_time);
    OpenAPI_list_for_each(sm_policy_decision->policy_ctrl_req_triggers, node) {
        OpenAPI_policy_control_request_trigger_free(node->data);
    }
    OpenAPI_list_free(sm_policy_decision->policy_ctrl_req_triggers);
    OpenAPI_list_for_each(sm_policy_decision->last_req_rule_data, node) {
        OpenAPI_requested_rule_data_free(node->data);
    }
    OpenAPI_list_free(sm_policy_decision->last_req_rule_data);
    OpenAPI_requested_usage_data_free(sm_policy_decision->last_req_usage_data);
    OpenAPI_list_for_each(sm_policy_decision->pra_infos, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_presence_info_rm_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(sm_policy_decision->pra_infos);
    OpenAPI_qos_flow_usage_free(sm_policy_decision->qos_flow_usage);
    OpenAPI_sm_policy_association_release_cause_free(sm_policy_decision->rel_cause);
    ogs_free(sm_policy_decision->supp_feat);
    OpenAPI_port_management_container_free(sm_policy_decision->tsn_port_man_cont_dstt);
    OpenAPI_list_for_each(sm_policy_decision->tsn_port_man_cont_nwtts, node) {
        OpenAPI_port_management_container_free(node->data);
    }
    OpenAPI_list_free(sm_policy_decision->tsn_port_man_cont_nwtts);
    ogs_free(sm_policy_decision);
}

cJSON *OpenAPI_sm_policy_decision_convertToJSON(OpenAPI_sm_policy_decision_t *sm_policy_decision)
{
    cJSON *item = NULL;

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
        OpenAPI_lnode_t *sess_rules_node;
        if (sm_policy_decision->sess_rules) {
            OpenAPI_list_for_each(sm_policy_decision->sess_rules, sess_rules_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)sess_rules_node->data;
                cJSON *itemLocal = OpenAPI_session_rule_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [sess_rules]");
                    goto end;
                }
                cJSON_AddItemToObject(sess_rules, localKeyValue->key, itemLocal);
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
        OpenAPI_lnode_t *pcc_rules_node;
        if (sm_policy_decision->pcc_rules) {
            OpenAPI_list_for_each(sm_policy_decision->pcc_rules, pcc_rules_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)pcc_rules_node->data;
                cJSON *itemLocal = OpenAPI_pcc_rule_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [pcc_rules]");
                    goto end;
                }
                cJSON_AddItemToObject(pcc_rules, localKeyValue->key, itemLocal);
            }
        }
    }

    if (sm_policy_decision->pcscf_rest_indication) {
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
        OpenAPI_lnode_t *qos_decs_node;
        if (sm_policy_decision->qos_decs) {
            OpenAPI_list_for_each(sm_policy_decision->qos_decs, qos_decs_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)qos_decs_node->data;
                cJSON *itemLocal = OpenAPI_qos_data_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_decs]");
                    goto end;
                }
                cJSON_AddItemToObject(qos_decs, localKeyValue->key, itemLocal);
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
        OpenAPI_lnode_t *chg_decs_node;
        if (sm_policy_decision->chg_decs) {
            OpenAPI_list_for_each(sm_policy_decision->chg_decs, chg_decs_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)chg_decs_node->data;
                cJSON *itemLocal = OpenAPI_charging_data_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [chg_decs]");
                    goto end;
                }
                cJSON_AddItemToObject(chg_decs, localKeyValue->key, itemLocal);
            }
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
        OpenAPI_lnode_t *traff_cont_decs_node;
        if (sm_policy_decision->traff_cont_decs) {
            OpenAPI_list_for_each(sm_policy_decision->traff_cont_decs, traff_cont_decs_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)traff_cont_decs_node->data;
                cJSON *itemLocal = OpenAPI_traffic_control_data_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [traff_cont_decs]");
                    goto end;
                }
                cJSON_AddItemToObject(traff_cont_decs, localKeyValue->key, itemLocal);
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
        OpenAPI_lnode_t *um_decs_node;
        if (sm_policy_decision->um_decs) {
            OpenAPI_list_for_each(sm_policy_decision->um_decs, um_decs_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)um_decs_node->data;
                cJSON *itemLocal = OpenAPI_usage_monitoring_data_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [um_decs]");
                    goto end;
                }
                cJSON_AddItemToObject(um_decs, localKeyValue->key, itemLocal);
            }
        }
    }

    if (sm_policy_decision->qos_chars) {
        cJSON *qos_chars = cJSON_AddObjectToObject(item, "qosChars");
        if (qos_chars == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_chars]");
            goto end;
        }
        cJSON *localMapObject = qos_chars;
        OpenAPI_lnode_t *qos_chars_node;
        if (sm_policy_decision->qos_chars) {
            OpenAPI_list_for_each(sm_policy_decision->qos_chars, qos_chars_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)qos_chars_node->data;
                cJSON *itemLocal = OpenAPI_qos_characteristics_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_chars]");
                    goto end;
                }
                cJSON_AddItemToObject(qos_chars, localKeyValue->key, itemLocal);
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
        OpenAPI_lnode_t *qos_mon_decs_node;
        if (sm_policy_decision->qos_mon_decs) {
            OpenAPI_list_for_each(sm_policy_decision->qos_mon_decs, qos_mon_decs_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)qos_mon_decs_node->data;
                cJSON *itemLocal = OpenAPI_qos_monitoring_data_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_mon_decs]");
                    goto end;
                }
                cJSON_AddItemToObject(qos_mon_decs, localKeyValue->key, itemLocal);
            }
        }
    }

    if (sm_policy_decision->reflective_qo_s_timer) {
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
        OpenAPI_lnode_t *conds_node;
        if (sm_policy_decision->conds) {
            OpenAPI_list_for_each(sm_policy_decision->conds, conds_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)conds_node->data;
                cJSON *itemLocal = OpenAPI_condition_data_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [conds]");
                    goto end;
                }
                cJSON_AddItemToObject(conds, localKeyValue->key, itemLocal);
            }
        }
    }

    if (sm_policy_decision->revalidation_time) {
        if (cJSON_AddStringToObject(item, "revalidationTime", sm_policy_decision->revalidation_time) == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [revalidation_time]");
            goto end;
        }
    }

    if (sm_policy_decision->offline) {
        if (cJSON_AddBoolToObject(item, "offline", sm_policy_decision->offline) == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [offline]");
            goto end;
        }
    }

    if (sm_policy_decision->online) {
        if (cJSON_AddBoolToObject(item, "online", sm_policy_decision->online) == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [online]");
            goto end;
        }
    }

    if (sm_policy_decision->policy_ctrl_req_triggers) {
        cJSON *policy_ctrl_req_triggersList = cJSON_AddArrayToObject(item, "policyCtrlReqTriggers");
        if (policy_ctrl_req_triggersList == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [policy_ctrl_req_triggers]");
            goto end;
        }

        OpenAPI_lnode_t *policy_ctrl_req_triggers_node;
        if (sm_policy_decision->policy_ctrl_req_triggers) {
            OpenAPI_list_for_each(sm_policy_decision->policy_ctrl_req_triggers, policy_ctrl_req_triggers_node) {
                cJSON *itemLocal = OpenAPI_policy_control_request_trigger_convertToJSON(policy_ctrl_req_triggers_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [policy_ctrl_req_triggers]");
                    goto end;
                }
                cJSON_AddItemToArray(policy_ctrl_req_triggersList, itemLocal);
            }
        }
    }

    if (sm_policy_decision->last_req_rule_data) {
        cJSON *last_req_rule_dataList = cJSON_AddArrayToObject(item, "lastReqRuleData");
        if (last_req_rule_dataList == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [last_req_rule_data]");
            goto end;
        }

        OpenAPI_lnode_t *last_req_rule_data_node;
        if (sm_policy_decision->last_req_rule_data) {
            OpenAPI_list_for_each(sm_policy_decision->last_req_rule_data, last_req_rule_data_node) {
                cJSON *itemLocal = OpenAPI_requested_rule_data_convertToJSON(last_req_rule_data_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [last_req_rule_data]");
                    goto end;
                }
                cJSON_AddItemToArray(last_req_rule_dataList, itemLocal);
            }
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
        OpenAPI_lnode_t *pra_infos_node;
        if (sm_policy_decision->pra_infos) {
            OpenAPI_list_for_each(sm_policy_decision->pra_infos, pra_infos_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)pra_infos_node->data;
                cJSON *itemLocal = OpenAPI_presence_info_rm_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [pra_infos]");
                    goto end;
                }
                cJSON_AddItemToObject(pra_infos, localKeyValue->key, itemLocal);
            }
        }
    }

    if (sm_policy_decision->ipv4_index) {
        if (cJSON_AddNumberToObject(item, "ipv4Index", sm_policy_decision->ipv4_index) == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [ipv4_index]");
            goto end;
        }
    }

    if (sm_policy_decision->ipv6_index) {
        if (cJSON_AddNumberToObject(item, "ipv6Index", sm_policy_decision->ipv6_index) == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [ipv6_index]");
            goto end;
        }
    }

    if (sm_policy_decision->qos_flow_usage) {
        cJSON *qos_flow_usage_local_JSON = OpenAPI_qos_flow_usage_convertToJSON(sm_policy_decision->qos_flow_usage);
        if (qos_flow_usage_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_flow_usage]");
            goto end;
        }
        cJSON_AddItemToObject(item, "qosFlowUsage", qos_flow_usage_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [qos_flow_usage]");
            goto end;
        }
    }

    if (sm_policy_decision->rel_cause) {
        cJSON *rel_cause_local_JSON = OpenAPI_sm_policy_association_release_cause_convertToJSON(sm_policy_decision->rel_cause);
        if (rel_cause_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [rel_cause]");
            goto end;
        }
        cJSON_AddItemToObject(item, "relCause", rel_cause_local_JSON);
        if (item->child == NULL) {
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

        OpenAPI_lnode_t *tsn_port_man_cont_nwtts_node;
        if (sm_policy_decision->tsn_port_man_cont_nwtts) {
            OpenAPI_list_for_each(sm_policy_decision->tsn_port_man_cont_nwtts, tsn_port_man_cont_nwtts_node) {
                cJSON *itemLocal = OpenAPI_port_management_container_convertToJSON(tsn_port_man_cont_nwtts_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_sm_policy_decision_convertToJSON() failed [tsn_port_man_cont_nwtts]");
                    goto end;
                }
                cJSON_AddItemToArray(tsn_port_man_cont_nwttsList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_sm_policy_decision_t *OpenAPI_sm_policy_decision_parseFromJSON(cJSON *sm_policy_decisionJSON)
{
    OpenAPI_sm_policy_decision_t *sm_policy_decision_local_var = NULL;
    cJSON *sess_rules = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "sessRules");

    OpenAPI_list_t *sess_rulesList;
    if (sess_rules) {
        cJSON *sess_rules_local_map;
        if (!cJSON_IsObject(sess_rules)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [sess_rules]");
            goto end;
        }
        sess_rulesList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(sess_rules_local_map, sess_rules) {
            cJSON *localMapObject = sess_rules_local_map;
            if (!cJSON_IsObject(sess_rules_local_map)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [sess_rules]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_session_rule_parseFromJSON(localMapObject));
            OpenAPI_list_add(sess_rulesList, localMapKeyPair);
        }
    }

    cJSON *pcc_rules = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "pccRules");

    OpenAPI_list_t *pcc_rulesList;
    if (pcc_rules) {
        cJSON *pcc_rules_local_map;
        if (!cJSON_IsObject(pcc_rules)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [pcc_rules]");
            goto end;
        }
        pcc_rulesList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(pcc_rules_local_map, pcc_rules) {
            cJSON *localMapObject = pcc_rules_local_map;
            if (!cJSON_IsObject(pcc_rules_local_map)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [pcc_rules]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_pcc_rule_parseFromJSON(localMapObject));
            OpenAPI_list_add(pcc_rulesList, localMapKeyPair);
        }
    }

    cJSON *pcscf_rest_indication = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "pcscfRestIndication");

    if (pcscf_rest_indication) {
        if (!cJSON_IsBool(pcscf_rest_indication)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [pcscf_rest_indication]");
            goto end;
        }
    }

    cJSON *qos_decs = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "qosDecs");

    OpenAPI_list_t *qos_decsList;
    if (qos_decs) {
        cJSON *qos_decs_local_map;
        if (!cJSON_IsObject(qos_decs)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [qos_decs]");
            goto end;
        }
        qos_decsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(qos_decs_local_map, qos_decs) {
            cJSON *localMapObject = qos_decs_local_map;
            if (!cJSON_IsObject(qos_decs_local_map)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [qos_decs]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_qos_data_parseFromJSON(localMapObject));
            OpenAPI_list_add(qos_decsList, localMapKeyPair);
        }
    }

    cJSON *chg_decs = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "chgDecs");

    OpenAPI_list_t *chg_decsList;
    if (chg_decs) {
        cJSON *chg_decs_local_map;
        if (!cJSON_IsObject(chg_decs)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [chg_decs]");
            goto end;
        }
        chg_decsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(chg_decs_local_map, chg_decs) {
            cJSON *localMapObject = chg_decs_local_map;
            if (!cJSON_IsObject(chg_decs_local_map)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [chg_decs]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_charging_data_parseFromJSON(localMapObject));
            OpenAPI_list_add(chg_decsList, localMapKeyPair);
        }
    }

    cJSON *charging_info = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "chargingInfo");

    OpenAPI_charging_information_t *charging_info_local_nonprim = NULL;
    if (charging_info) {
        charging_info_local_nonprim = OpenAPI_charging_information_parseFromJSON(charging_info);
    }

    cJSON *traff_cont_decs = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "traffContDecs");

    OpenAPI_list_t *traff_cont_decsList;
    if (traff_cont_decs) {
        cJSON *traff_cont_decs_local_map;
        if (!cJSON_IsObject(traff_cont_decs)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [traff_cont_decs]");
            goto end;
        }
        traff_cont_decsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(traff_cont_decs_local_map, traff_cont_decs) {
            cJSON *localMapObject = traff_cont_decs_local_map;
            if (!cJSON_IsObject(traff_cont_decs_local_map)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [traff_cont_decs]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_traffic_control_data_parseFromJSON(localMapObject));
            OpenAPI_list_add(traff_cont_decsList, localMapKeyPair);
        }
    }

    cJSON *um_decs = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "umDecs");

    OpenAPI_list_t *um_decsList;
    if (um_decs) {
        cJSON *um_decs_local_map;
        if (!cJSON_IsObject(um_decs)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [um_decs]");
            goto end;
        }
        um_decsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(um_decs_local_map, um_decs) {
            cJSON *localMapObject = um_decs_local_map;
            if (!cJSON_IsObject(um_decs_local_map)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [um_decs]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_usage_monitoring_data_parseFromJSON(localMapObject));
            OpenAPI_list_add(um_decsList, localMapKeyPair);
        }
    }

    cJSON *qos_chars = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "qosChars");

    OpenAPI_list_t *qos_charsList;
    if (qos_chars) {
        cJSON *qos_chars_local_map;
        if (!cJSON_IsObject(qos_chars)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [qos_chars]");
            goto end;
        }
        qos_charsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(qos_chars_local_map, qos_chars) {
            cJSON *localMapObject = qos_chars_local_map;
            if (!cJSON_IsObject(qos_chars_local_map)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [qos_chars]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_qos_characteristics_parseFromJSON(localMapObject));
            OpenAPI_list_add(qos_charsList, localMapKeyPair);
        }
    }

    cJSON *qos_mon_decs = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "qosMonDecs");

    OpenAPI_list_t *qos_mon_decsList;
    if (qos_mon_decs) {
        cJSON *qos_mon_decs_local_map;
        if (!cJSON_IsObject(qos_mon_decs)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [qos_mon_decs]");
            goto end;
        }
        qos_mon_decsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(qos_mon_decs_local_map, qos_mon_decs) {
            cJSON *localMapObject = qos_mon_decs_local_map;
            if (!cJSON_IsObject(qos_mon_decs_local_map)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [qos_mon_decs]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_qos_monitoring_data_parseFromJSON(localMapObject));
            OpenAPI_list_add(qos_mon_decsList, localMapKeyPair);
        }
    }

    cJSON *reflective_qo_s_timer = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "reflectiveQoSTimer");

    if (reflective_qo_s_timer) {
        if (!cJSON_IsNumber(reflective_qo_s_timer)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [reflective_qo_s_timer]");
            goto end;
        }
    }

    cJSON *conds = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "conds");

    OpenAPI_list_t *condsList;
    if (conds) {
        cJSON *conds_local_map;
        if (!cJSON_IsObject(conds)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [conds]");
            goto end;
        }
        condsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(conds_local_map, conds) {
            cJSON *localMapObject = conds_local_map;
            if (!cJSON_IsObject(conds_local_map)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [conds]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_condition_data_parseFromJSON(localMapObject));
            OpenAPI_list_add(condsList, localMapKeyPair);
        }
    }

    cJSON *revalidation_time = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "revalidationTime");

    if (revalidation_time) {
        if (!cJSON_IsString(revalidation_time)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [revalidation_time]");
            goto end;
        }
    }

    cJSON *offline = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "offline");

    if (offline) {
        if (!cJSON_IsBool(offline)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [offline]");
            goto end;
        }
    }

    cJSON *online = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "online");

    if (online) {
        if (!cJSON_IsBool(online)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [online]");
            goto end;
        }
    }

    cJSON *policy_ctrl_req_triggers = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "policyCtrlReqTriggers");

    OpenAPI_list_t *policy_ctrl_req_triggersList;
    if (policy_ctrl_req_triggers) {
        cJSON *policy_ctrl_req_triggers_local_nonprimitive;
        if (!cJSON_IsArray(policy_ctrl_req_triggers)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [policy_ctrl_req_triggers]");
            goto end;
        }

        policy_ctrl_req_triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(policy_ctrl_req_triggers_local_nonprimitive, policy_ctrl_req_triggers ) {
            if (!cJSON_IsObject(policy_ctrl_req_triggers_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [policy_ctrl_req_triggers]");
                goto end;
            }
            OpenAPI_policy_control_request_trigger_t *policy_ctrl_req_triggersItem = OpenAPI_policy_control_request_trigger_parseFromJSON(policy_ctrl_req_triggers_local_nonprimitive);

            OpenAPI_list_add(policy_ctrl_req_triggersList, policy_ctrl_req_triggersItem);
        }
    }

    cJSON *last_req_rule_data = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "lastReqRuleData");

    OpenAPI_list_t *last_req_rule_dataList;
    if (last_req_rule_data) {
        cJSON *last_req_rule_data_local_nonprimitive;
        if (!cJSON_IsArray(last_req_rule_data)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [last_req_rule_data]");
            goto end;
        }

        last_req_rule_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(last_req_rule_data_local_nonprimitive, last_req_rule_data ) {
            if (!cJSON_IsObject(last_req_rule_data_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [last_req_rule_data]");
                goto end;
            }
            OpenAPI_requested_rule_data_t *last_req_rule_dataItem = OpenAPI_requested_rule_data_parseFromJSON(last_req_rule_data_local_nonprimitive);

            OpenAPI_list_add(last_req_rule_dataList, last_req_rule_dataItem);
        }
    }

    cJSON *last_req_usage_data = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "lastReqUsageData");

    OpenAPI_requested_usage_data_t *last_req_usage_data_local_nonprim = NULL;
    if (last_req_usage_data) {
        last_req_usage_data_local_nonprim = OpenAPI_requested_usage_data_parseFromJSON(last_req_usage_data);
    }

    cJSON *pra_infos = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "praInfos");

    OpenAPI_list_t *pra_infosList;
    if (pra_infos) {
        cJSON *pra_infos_local_map;
        if (!cJSON_IsObject(pra_infos)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [pra_infos]");
            goto end;
        }
        pra_infosList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(pra_infos_local_map, pra_infos) {
            cJSON *localMapObject = pra_infos_local_map;
            if (!cJSON_IsObject(pra_infos_local_map)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [pra_infos]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_presence_info_rm_parseFromJSON(localMapObject));
            OpenAPI_list_add(pra_infosList, localMapKeyPair);
        }
    }

    cJSON *ipv4_index = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "ipv4Index");

    if (ipv4_index) {
        if (!cJSON_IsNumber(ipv4_index)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [ipv4_index]");
            goto end;
        }
    }

    cJSON *ipv6_index = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "ipv6Index");

    if (ipv6_index) {
        if (!cJSON_IsNumber(ipv6_index)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [ipv6_index]");
            goto end;
        }
    }

    cJSON *qos_flow_usage = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "qosFlowUsage");

    OpenAPI_qos_flow_usage_t *qos_flow_usage_local_nonprim = NULL;
    if (qos_flow_usage) {
        qos_flow_usage_local_nonprim = OpenAPI_qos_flow_usage_parseFromJSON(qos_flow_usage);
    }

    cJSON *rel_cause = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "relCause");

    OpenAPI_sm_policy_association_release_cause_t *rel_cause_local_nonprim = NULL;
    if (rel_cause) {
        rel_cause_local_nonprim = OpenAPI_sm_policy_association_release_cause_parseFromJSON(rel_cause);
    }

    cJSON *supp_feat = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "suppFeat");

    if (supp_feat) {
        if (!cJSON_IsString(supp_feat)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [supp_feat]");
            goto end;
        }
    }

    cJSON *tsn_port_man_cont_dstt = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "tsnPortManContDstt");

    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt_local_nonprim = NULL;
    if (tsn_port_man_cont_dstt) {
        tsn_port_man_cont_dstt_local_nonprim = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_dstt);
    }

    cJSON *tsn_port_man_cont_nwtts = cJSON_GetObjectItemCaseSensitive(sm_policy_decisionJSON, "tsnPortManContNwtts");

    OpenAPI_list_t *tsn_port_man_cont_nwttsList;
    if (tsn_port_man_cont_nwtts) {
        cJSON *tsn_port_man_cont_nwtts_local_nonprimitive;
        if (!cJSON_IsArray(tsn_port_man_cont_nwtts)) {
            ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }

        tsn_port_man_cont_nwttsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tsn_port_man_cont_nwtts_local_nonprimitive, tsn_port_man_cont_nwtts ) {
            if (!cJSON_IsObject(tsn_port_man_cont_nwtts_local_nonprimitive)) {
                ogs_error("OpenAPI_sm_policy_decision_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
                goto end;
            }
            OpenAPI_port_management_container_t *tsn_port_man_cont_nwttsItem = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_nwtts_local_nonprimitive);

            OpenAPI_list_add(tsn_port_man_cont_nwttsList, tsn_port_man_cont_nwttsItem);
        }
    }

    sm_policy_decision_local_var = OpenAPI_sm_policy_decision_create (
        sess_rules ? sess_rulesList : NULL,
        pcc_rules ? pcc_rulesList : NULL,
        pcscf_rest_indication ? pcscf_rest_indication->valueint : 0,
        qos_decs ? qos_decsList : NULL,
        chg_decs ? chg_decsList : NULL,
        charging_info ? charging_info_local_nonprim : NULL,
        traff_cont_decs ? traff_cont_decsList : NULL,
        um_decs ? um_decsList : NULL,
        qos_chars ? qos_charsList : NULL,
        qos_mon_decs ? qos_mon_decsList : NULL,
        reflective_qo_s_timer ? reflective_qo_s_timer->valuedouble : 0,
        conds ? condsList : NULL,
        revalidation_time ? ogs_strdup(revalidation_time->valuestring) : NULL,
        offline ? offline->valueint : 0,
        online ? online->valueint : 0,
        policy_ctrl_req_triggers ? policy_ctrl_req_triggersList : NULL,
        last_req_rule_data ? last_req_rule_dataList : NULL,
        last_req_usage_data ? last_req_usage_data_local_nonprim : NULL,
        pra_infos ? pra_infosList : NULL,
        ipv4_index ? ipv4_index->valuedouble : 0,
        ipv6_index ? ipv6_index->valuedouble : 0,
        qos_flow_usage ? qos_flow_usage_local_nonprim : NULL,
        rel_cause ? rel_cause_local_nonprim : NULL,
        supp_feat ? ogs_strdup(supp_feat->valuestring) : NULL,
        tsn_port_man_cont_dstt ? tsn_port_man_cont_dstt_local_nonprim : NULL,
        tsn_port_man_cont_nwtts ? tsn_port_man_cont_nwttsList : NULL
        );

    return sm_policy_decision_local_var;
end:
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

