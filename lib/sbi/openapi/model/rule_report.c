
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rule_report.h"

OpenAPI_rule_report_t *OpenAPI_rule_report_create(
    OpenAPI_list_t *pcc_rule_ids,
    OpenAPI_rule_status_e rule_status,
    OpenAPI_list_t *cont_vers,
    OpenAPI_failure_code_e failure_code,
    OpenAPI_final_unit_action_t *fin_unit_act,
    OpenAPI_list_t *ran_nas_rel_causes,
    char *alt_qos_param_id
)
{
    OpenAPI_rule_report_t *rule_report_local_var = ogs_malloc(sizeof(OpenAPI_rule_report_t));
    ogs_assert(rule_report_local_var);

    rule_report_local_var->pcc_rule_ids = pcc_rule_ids;
    rule_report_local_var->rule_status = rule_status;
    rule_report_local_var->cont_vers = cont_vers;
    rule_report_local_var->failure_code = failure_code;
    rule_report_local_var->fin_unit_act = fin_unit_act;
    rule_report_local_var->ran_nas_rel_causes = ran_nas_rel_causes;
    rule_report_local_var->alt_qos_param_id = alt_qos_param_id;

    return rule_report_local_var;
}

void OpenAPI_rule_report_free(OpenAPI_rule_report_t *rule_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == rule_report) {
        return;
    }
    if (rule_report->pcc_rule_ids) {
        OpenAPI_list_for_each(rule_report->pcc_rule_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rule_report->pcc_rule_ids);
        rule_report->pcc_rule_ids = NULL;
    }
    if (rule_report->cont_vers) {
        OpenAPI_list_for_each(rule_report->cont_vers, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rule_report->cont_vers);
        rule_report->cont_vers = NULL;
    }
    if (rule_report->fin_unit_act) {
        OpenAPI_final_unit_action_free(rule_report->fin_unit_act);
        rule_report->fin_unit_act = NULL;
    }
    if (rule_report->ran_nas_rel_causes) {
        OpenAPI_list_for_each(rule_report->ran_nas_rel_causes, node) {
            OpenAPI_ran_nas_rel_cause_free(node->data);
        }
        OpenAPI_list_free(rule_report->ran_nas_rel_causes);
        rule_report->ran_nas_rel_causes = NULL;
    }
    if (rule_report->alt_qos_param_id) {
        ogs_free(rule_report->alt_qos_param_id);
        rule_report->alt_qos_param_id = NULL;
    }
    ogs_free(rule_report);
}

cJSON *OpenAPI_rule_report_convertToJSON(OpenAPI_rule_report_t *rule_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (rule_report == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [RuleReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!rule_report->pcc_rule_ids) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [pcc_rule_ids]");
        return NULL;
    }
    cJSON *pcc_rule_idsList = cJSON_AddArrayToObject(item, "pccRuleIds");
    if (pcc_rule_idsList == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [pcc_rule_ids]");
        goto end;
    }
    OpenAPI_list_for_each(rule_report->pcc_rule_ids, node) {
        if (cJSON_AddStringToObject(pcc_rule_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_rule_report_convertToJSON() failed [pcc_rule_ids]");
            goto end;
        }
    }

    if (rule_report->rule_status == OpenAPI_rule_status_NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [rule_status]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ruleStatus", OpenAPI_rule_status_ToString(rule_report->rule_status)) == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [rule_status]");
        goto end;
    }

    if (rule_report->cont_vers) {
    cJSON *cont_versList = cJSON_AddArrayToObject(item, "contVers");
    if (cont_versList == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [cont_vers]");
        goto end;
    }
    OpenAPI_list_for_each(rule_report->cont_vers, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_rule_report_convertToJSON() failed [cont_vers]");
            goto end;
        }
        if (cJSON_AddNumberToObject(cont_versList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_rule_report_convertToJSON() failed [cont_vers]");
            goto end;
        }
    }
    }

    if (rule_report->failure_code != OpenAPI_failure_code_NULL) {
    if (cJSON_AddStringToObject(item, "failureCode", OpenAPI_failure_code_ToString(rule_report->failure_code)) == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [failure_code]");
        goto end;
    }
    }

    if (rule_report->fin_unit_act) {
    cJSON *fin_unit_act_local_JSON = OpenAPI_final_unit_action_convertToJSON(rule_report->fin_unit_act);
    if (fin_unit_act_local_JSON == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [fin_unit_act]");
        goto end;
    }
    cJSON_AddItemToObject(item, "finUnitAct", fin_unit_act_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [fin_unit_act]");
        goto end;
    }
    }

    if (rule_report->ran_nas_rel_causes) {
    cJSON *ran_nas_rel_causesList = cJSON_AddArrayToObject(item, "ranNasRelCauses");
    if (ran_nas_rel_causesList == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [ran_nas_rel_causes]");
        goto end;
    }
    OpenAPI_list_for_each(rule_report->ran_nas_rel_causes, node) {
        cJSON *itemLocal = OpenAPI_ran_nas_rel_cause_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_rule_report_convertToJSON() failed [ran_nas_rel_causes]");
            goto end;
        }
        cJSON_AddItemToArray(ran_nas_rel_causesList, itemLocal);
    }
    }

    if (rule_report->alt_qos_param_id) {
    if (cJSON_AddStringToObject(item, "altQosParamId", rule_report->alt_qos_param_id) == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [alt_qos_param_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_rule_report_t *OpenAPI_rule_report_parseFromJSON(cJSON *rule_reportJSON)
{
    OpenAPI_rule_report_t *rule_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pcc_rule_ids = NULL;
    OpenAPI_list_t *pcc_rule_idsList = NULL;
    cJSON *rule_status = NULL;
    OpenAPI_rule_status_e rule_statusVariable = 0;
    cJSON *cont_vers = NULL;
    OpenAPI_list_t *cont_versList = NULL;
    cJSON *failure_code = NULL;
    OpenAPI_failure_code_e failure_codeVariable = 0;
    cJSON *fin_unit_act = NULL;
    OpenAPI_final_unit_action_t *fin_unit_act_local_nonprim = NULL;
    cJSON *ran_nas_rel_causes = NULL;
    OpenAPI_list_t *ran_nas_rel_causesList = NULL;
    cJSON *alt_qos_param_id = NULL;
    pcc_rule_ids = cJSON_GetObjectItemCaseSensitive(rule_reportJSON, "pccRuleIds");
    if (!pcc_rule_ids) {
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [pcc_rule_ids]");
        goto end;
    }
        cJSON *pcc_rule_ids_local = NULL;
        if (!cJSON_IsArray(pcc_rule_ids)) {
            ogs_error("OpenAPI_rule_report_parseFromJSON() failed [pcc_rule_ids]");
            goto end;
        }

        pcc_rule_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcc_rule_ids_local, pcc_rule_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(pcc_rule_ids_local)) {
                ogs_error("OpenAPI_rule_report_parseFromJSON() failed [pcc_rule_ids]");
                goto end;
            }
            OpenAPI_list_add(pcc_rule_idsList, ogs_strdup(pcc_rule_ids_local->valuestring));
        }

    rule_status = cJSON_GetObjectItemCaseSensitive(rule_reportJSON, "ruleStatus");
    if (!rule_status) {
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [rule_status]");
        goto end;
    }
    if (!cJSON_IsString(rule_status)) {
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [rule_status]");
        goto end;
    }
    rule_statusVariable = OpenAPI_rule_status_FromString(rule_status->valuestring);

    cont_vers = cJSON_GetObjectItemCaseSensitive(rule_reportJSON, "contVers");
    if (cont_vers) {
        cJSON *cont_vers_local = NULL;
        if (!cJSON_IsArray(cont_vers)) {
            ogs_error("OpenAPI_rule_report_parseFromJSON() failed [cont_vers]");
            goto end;
        }

        cont_versList = OpenAPI_list_create();

        cJSON_ArrayForEach(cont_vers_local, cont_vers) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(cont_vers_local)) {
                ogs_error("OpenAPI_rule_report_parseFromJSON() failed [cont_vers]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_rule_report_parseFromJSON() failed [cont_vers]");
                goto end;
            }
            *localDouble = cont_vers_local->valuedouble;
            OpenAPI_list_add(cont_versList, localDouble);
        }
    }

    failure_code = cJSON_GetObjectItemCaseSensitive(rule_reportJSON, "failureCode");
    if (failure_code) {
    if (!cJSON_IsString(failure_code)) {
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [failure_code]");
        goto end;
    }
    failure_codeVariable = OpenAPI_failure_code_FromString(failure_code->valuestring);
    }

    fin_unit_act = cJSON_GetObjectItemCaseSensitive(rule_reportJSON, "finUnitAct");
    if (fin_unit_act) {
    fin_unit_act_local_nonprim = OpenAPI_final_unit_action_parseFromJSON(fin_unit_act);
    if (!fin_unit_act_local_nonprim) {
        ogs_error("OpenAPI_final_unit_action_parseFromJSON failed [fin_unit_act]");
        goto end;
    }
    }

    ran_nas_rel_causes = cJSON_GetObjectItemCaseSensitive(rule_reportJSON, "ranNasRelCauses");
    if (ran_nas_rel_causes) {
        cJSON *ran_nas_rel_causes_local = NULL;
        if (!cJSON_IsArray(ran_nas_rel_causes)) {
            ogs_error("OpenAPI_rule_report_parseFromJSON() failed [ran_nas_rel_causes]");
            goto end;
        }

        ran_nas_rel_causesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ran_nas_rel_causes_local, ran_nas_rel_causes) {
            if (!cJSON_IsObject(ran_nas_rel_causes_local)) {
                ogs_error("OpenAPI_rule_report_parseFromJSON() failed [ran_nas_rel_causes]");
                goto end;
            }
            OpenAPI_ran_nas_rel_cause_t *ran_nas_rel_causesItem = OpenAPI_ran_nas_rel_cause_parseFromJSON(ran_nas_rel_causes_local);
            if (!ran_nas_rel_causesItem) {
                ogs_error("No ran_nas_rel_causesItem");
                goto end;
            }
            OpenAPI_list_add(ran_nas_rel_causesList, ran_nas_rel_causesItem);
        }
    }

    alt_qos_param_id = cJSON_GetObjectItemCaseSensitive(rule_reportJSON, "altQosParamId");
    if (alt_qos_param_id) {
    if (!cJSON_IsString(alt_qos_param_id) && !cJSON_IsNull(alt_qos_param_id)) {
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [alt_qos_param_id]");
        goto end;
    }
    }

    rule_report_local_var = OpenAPI_rule_report_create (
        pcc_rule_idsList,
        rule_statusVariable,
        cont_vers ? cont_versList : NULL,
        failure_code ? failure_codeVariable : 0,
        fin_unit_act ? fin_unit_act_local_nonprim : NULL,
        ran_nas_rel_causes ? ran_nas_rel_causesList : NULL,
        alt_qos_param_id && !cJSON_IsNull(alt_qos_param_id) ? ogs_strdup(alt_qos_param_id->valuestring) : NULL
    );

    return rule_report_local_var;
end:
    if (pcc_rule_idsList) {
        OpenAPI_list_for_each(pcc_rule_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pcc_rule_idsList);
        pcc_rule_idsList = NULL;
    }
    if (cont_versList) {
        OpenAPI_list_for_each(cont_versList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(cont_versList);
        cont_versList = NULL;
    }
    if (fin_unit_act_local_nonprim) {
        OpenAPI_final_unit_action_free(fin_unit_act_local_nonprim);
        fin_unit_act_local_nonprim = NULL;
    }
    if (ran_nas_rel_causesList) {
        OpenAPI_list_for_each(ran_nas_rel_causesList, node) {
            OpenAPI_ran_nas_rel_cause_free(node->data);
        }
        OpenAPI_list_free(ran_nas_rel_causesList);
        ran_nas_rel_causesList = NULL;
    }
    return NULL;
}

OpenAPI_rule_report_t *OpenAPI_rule_report_copy(OpenAPI_rule_report_t *dst, OpenAPI_rule_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rule_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed");
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

    OpenAPI_rule_report_free(dst);
    dst = OpenAPI_rule_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

