
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
    OpenAPI_list_t *ran_nas_rel_causes
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

    return rule_report_local_var;
}

void OpenAPI_rule_report_free(OpenAPI_rule_report_t *rule_report)
{
    if (NULL == rule_report) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(rule_report->pcc_rule_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(rule_report->pcc_rule_ids);
    OpenAPI_list_for_each(rule_report->cont_vers, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(rule_report->cont_vers);
    OpenAPI_final_unit_action_free(rule_report->fin_unit_act);
    OpenAPI_list_for_each(rule_report->ran_nas_rel_causes, node) {
        OpenAPI_ran_nas_rel_cause_free(node->data);
    }
    OpenAPI_list_free(rule_report->ran_nas_rel_causes);
    ogs_free(rule_report);
}

cJSON *OpenAPI_rule_report_convertToJSON(OpenAPI_rule_report_t *rule_report)
{
    cJSON *item = NULL;

    if (rule_report == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [RuleReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *pcc_rule_ids = cJSON_AddArrayToObject(item, "pccRuleIds");
    if (pcc_rule_ids == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [pcc_rule_ids]");
        goto end;
    }

    OpenAPI_lnode_t *pcc_rule_ids_node;
    OpenAPI_list_for_each(rule_report->pcc_rule_ids, pcc_rule_ids_node)  {
    if (cJSON_AddStringToObject(pcc_rule_ids, "", (char*)pcc_rule_ids_node->data) == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [pcc_rule_ids]");
        goto end;
    }
                    }

    if (cJSON_AddStringToObject(item, "ruleStatus", OpenAPI_rule_status_ToString(rule_report->rule_status)) == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [rule_status]");
        goto end;
    }

    if (rule_report->cont_vers) {
    cJSON *cont_vers = cJSON_AddArrayToObject(item, "contVers");
    if (cont_vers == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [cont_vers]");
        goto end;
    }

    OpenAPI_lnode_t *cont_vers_node;
    OpenAPI_list_for_each(rule_report->cont_vers, cont_vers_node)  {
    if (cJSON_AddNumberToObject(cont_vers, "", *(double *)cont_vers_node->data) == NULL) {
        ogs_error("OpenAPI_rule_report_convertToJSON() failed [cont_vers]");
        goto end;
    }
                    }
    }

    if (rule_report->failure_code) {
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

    OpenAPI_lnode_t *ran_nas_rel_causes_node;
    if (rule_report->ran_nas_rel_causes) {
        OpenAPI_list_for_each(rule_report->ran_nas_rel_causes, ran_nas_rel_causes_node) {
            cJSON *itemLocal = OpenAPI_ran_nas_rel_cause_convertToJSON(ran_nas_rel_causes_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_rule_report_convertToJSON() failed [ran_nas_rel_causes]");
                goto end;
            }
            cJSON_AddItemToArray(ran_nas_rel_causesList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_rule_report_t *OpenAPI_rule_report_parseFromJSON(cJSON *rule_reportJSON)
{
    OpenAPI_rule_report_t *rule_report_local_var = NULL;
    cJSON *pcc_rule_ids = cJSON_GetObjectItemCaseSensitive(rule_reportJSON, "pccRuleIds");
    if (!pcc_rule_ids) {
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [pcc_rule_ids]");
        goto end;
    }

    OpenAPI_list_t *pcc_rule_idsList;
    cJSON *pcc_rule_ids_local;
    if (!cJSON_IsArray(pcc_rule_ids)) {
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [pcc_rule_ids]");
        goto end;
    }
    pcc_rule_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(pcc_rule_ids_local, pcc_rule_ids) {
    if (!cJSON_IsString(pcc_rule_ids_local)) {
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [pcc_rule_ids]");
        goto end;
    }
    OpenAPI_list_add(pcc_rule_idsList , ogs_strdup(pcc_rule_ids_local->valuestring));
    }

    cJSON *rule_status = cJSON_GetObjectItemCaseSensitive(rule_reportJSON, "ruleStatus");
    if (!rule_status) {
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [rule_status]");
        goto end;
    }

    OpenAPI_rule_status_e rule_statusVariable;
    if (!cJSON_IsString(rule_status)) {
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [rule_status]");
        goto end;
    }
    rule_statusVariable = OpenAPI_rule_status_FromString(rule_status->valuestring);

    cJSON *cont_vers = cJSON_GetObjectItemCaseSensitive(rule_reportJSON, "contVers");

    OpenAPI_list_t *cont_versList;
    if (cont_vers) {
    cJSON *cont_vers_local;
    if (!cJSON_IsArray(cont_vers)) {
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [cont_vers]");
        goto end;
    }
    cont_versList = OpenAPI_list_create();

    cJSON_ArrayForEach(cont_vers_local, cont_vers) {
    if (!cJSON_IsNumber(cont_vers_local)) {
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [cont_vers]");
        goto end;
    }
    OpenAPI_list_add(cont_versList , &cont_vers_local->valuedouble);
    }
    }

    cJSON *failure_code = cJSON_GetObjectItemCaseSensitive(rule_reportJSON, "failureCode");

    OpenAPI_failure_code_e failure_codeVariable;
    if (failure_code) {
    if (!cJSON_IsString(failure_code)) {
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [failure_code]");
        goto end;
    }
    failure_codeVariable = OpenAPI_failure_code_FromString(failure_code->valuestring);
    }

    cJSON *fin_unit_act = cJSON_GetObjectItemCaseSensitive(rule_reportJSON, "finUnitAct");

    OpenAPI_final_unit_action_t *fin_unit_act_local_nonprim = NULL;
    if (fin_unit_act) {
    fin_unit_act_local_nonprim = OpenAPI_final_unit_action_parseFromJSON(fin_unit_act);
    }

    cJSON *ran_nas_rel_causes = cJSON_GetObjectItemCaseSensitive(rule_reportJSON, "ranNasRelCauses");

    OpenAPI_list_t *ran_nas_rel_causesList;
    if (ran_nas_rel_causes) {
    cJSON *ran_nas_rel_causes_local_nonprimitive;
    if (!cJSON_IsArray(ran_nas_rel_causes)){
        ogs_error("OpenAPI_rule_report_parseFromJSON() failed [ran_nas_rel_causes]");
        goto end;
    }

    ran_nas_rel_causesList = OpenAPI_list_create();

    cJSON_ArrayForEach(ran_nas_rel_causes_local_nonprimitive, ran_nas_rel_causes ) {
        if (!cJSON_IsObject(ran_nas_rel_causes_local_nonprimitive)) {
            ogs_error("OpenAPI_rule_report_parseFromJSON() failed [ran_nas_rel_causes]");
            goto end;
        }
        OpenAPI_ran_nas_rel_cause_t *ran_nas_rel_causesItem = OpenAPI_ran_nas_rel_cause_parseFromJSON(ran_nas_rel_causes_local_nonprimitive);

        if (!ran_nas_rel_causesItem) {
            ogs_error("No ran_nas_rel_causesItem");
            OpenAPI_list_free(ran_nas_rel_causesList);
            goto end;
        }

        OpenAPI_list_add(ran_nas_rel_causesList, ran_nas_rel_causesItem);
    }
    }

    rule_report_local_var = OpenAPI_rule_report_create (
        pcc_rule_idsList,
        rule_statusVariable,
        cont_vers ? cont_versList : NULL,
        failure_code ? failure_codeVariable : 0,
        fin_unit_act ? fin_unit_act_local_nonprim : NULL,
        ran_nas_rel_causes ? ran_nas_rel_causesList : NULL
    );

    return rule_report_local_var;
end:
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

