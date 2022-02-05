
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "partial_success_report.h"

OpenAPI_partial_success_report_t *OpenAPI_partial_success_report_create(
    OpenAPI_failure_cause_e failure_cause,
    OpenAPI_list_t *rule_reports,
    OpenAPI_list_t *sess_rule_reports,
    OpenAPI_ue_camping_rep_t *ue_camping_rep,
    OpenAPI_list_t *policy_dec_failure_reports
)
{
    OpenAPI_partial_success_report_t *partial_success_report_local_var = ogs_malloc(sizeof(OpenAPI_partial_success_report_t));
    ogs_assert(partial_success_report_local_var);

    partial_success_report_local_var->failure_cause = failure_cause;
    partial_success_report_local_var->rule_reports = rule_reports;
    partial_success_report_local_var->sess_rule_reports = sess_rule_reports;
    partial_success_report_local_var->ue_camping_rep = ue_camping_rep;
    partial_success_report_local_var->policy_dec_failure_reports = policy_dec_failure_reports;

    return partial_success_report_local_var;
}

void OpenAPI_partial_success_report_free(OpenAPI_partial_success_report_t *partial_success_report)
{
    if (NULL == partial_success_report) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(partial_success_report->rule_reports, node) {
        OpenAPI_rule_report_free(node->data);
    }
    OpenAPI_list_free(partial_success_report->rule_reports);
    OpenAPI_list_for_each(partial_success_report->sess_rule_reports, node) {
        OpenAPI_session_rule_report_free(node->data);
    }
    OpenAPI_list_free(partial_success_report->sess_rule_reports);
    OpenAPI_ue_camping_rep_free(partial_success_report->ue_camping_rep);
    OpenAPI_list_free(partial_success_report->policy_dec_failure_reports);
    ogs_free(partial_success_report);
}

cJSON *OpenAPI_partial_success_report_convertToJSON(OpenAPI_partial_success_report_t *partial_success_report)
{
    cJSON *item = NULL;

    if (partial_success_report == NULL) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [PartialSuccessReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "failureCause", OpenAPI_failure_cause_ToString(partial_success_report->failure_cause)) == NULL) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [failure_cause]");
        goto end;
    }

    if (partial_success_report->rule_reports) {
    cJSON *rule_reportsList = cJSON_AddArrayToObject(item, "ruleReports");
    if (rule_reportsList == NULL) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [rule_reports]");
        goto end;
    }

    OpenAPI_lnode_t *rule_reports_node;
    if (partial_success_report->rule_reports) {
        OpenAPI_list_for_each(partial_success_report->rule_reports, rule_reports_node) {
            cJSON *itemLocal = OpenAPI_rule_report_convertToJSON(rule_reports_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [rule_reports]");
                goto end;
            }
            cJSON_AddItemToArray(rule_reportsList, itemLocal);
        }
    }
    }

    if (partial_success_report->sess_rule_reports) {
    cJSON *sess_rule_reportsList = cJSON_AddArrayToObject(item, "sessRuleReports");
    if (sess_rule_reportsList == NULL) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [sess_rule_reports]");
        goto end;
    }

    OpenAPI_lnode_t *sess_rule_reports_node;
    if (partial_success_report->sess_rule_reports) {
        OpenAPI_list_for_each(partial_success_report->sess_rule_reports, sess_rule_reports_node) {
            cJSON *itemLocal = OpenAPI_session_rule_report_convertToJSON(sess_rule_reports_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [sess_rule_reports]");
                goto end;
            }
            cJSON_AddItemToArray(sess_rule_reportsList, itemLocal);
        }
    }
    }

    if (partial_success_report->ue_camping_rep) {
    cJSON *ue_camping_rep_local_JSON = OpenAPI_ue_camping_rep_convertToJSON(partial_success_report->ue_camping_rep);
    if (ue_camping_rep_local_JSON == NULL) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [ue_camping_rep]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueCampingRep", ue_camping_rep_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [ue_camping_rep]");
        goto end;
    }
    }

    if (partial_success_report->policy_dec_failure_reports) {
    cJSON *policy_dec_failure_reports = cJSON_AddArrayToObject(item, "policyDecFailureReports");
    if (policy_dec_failure_reports == NULL) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [policy_dec_failure_reports]");
        goto end;
    }
    OpenAPI_lnode_t *policy_dec_failure_reports_node;
    OpenAPI_list_for_each(partial_success_report->policy_dec_failure_reports, policy_dec_failure_reports_node) {
        if (cJSON_AddStringToObject(policy_dec_failure_reports, "", OpenAPI_policy_decision_failure_code_ToString((intptr_t)policy_dec_failure_reports_node->data)) == NULL) {
            ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [policy_dec_failure_reports]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_partial_success_report_t *OpenAPI_partial_success_report_parseFromJSON(cJSON *partial_success_reportJSON)
{
    OpenAPI_partial_success_report_t *partial_success_report_local_var = NULL;
    cJSON *failure_cause = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "failureCause");
    if (!failure_cause) {
        ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [failure_cause]");
        goto end;
    }

    OpenAPI_failure_cause_e failure_causeVariable;
    if (!cJSON_IsString(failure_cause)) {
        ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [failure_cause]");
        goto end;
    }
    failure_causeVariable = OpenAPI_failure_cause_FromString(failure_cause->valuestring);

    cJSON *rule_reports = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "ruleReports");

    OpenAPI_list_t *rule_reportsList;
    if (rule_reports) {
    cJSON *rule_reports_local_nonprimitive;
    if (!cJSON_IsArray(rule_reports)){
        ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [rule_reports]");
        goto end;
    }

    rule_reportsList = OpenAPI_list_create();

    cJSON_ArrayForEach(rule_reports_local_nonprimitive, rule_reports ) {
        if (!cJSON_IsObject(rule_reports_local_nonprimitive)) {
            ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [rule_reports]");
            goto end;
        }
        OpenAPI_rule_report_t *rule_reportsItem = OpenAPI_rule_report_parseFromJSON(rule_reports_local_nonprimitive);

        if (!rule_reportsItem) {
            ogs_error("No rule_reportsItem");
            OpenAPI_list_free(rule_reportsList);
            goto end;
        }

        OpenAPI_list_add(rule_reportsList, rule_reportsItem);
    }
    }

    cJSON *sess_rule_reports = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "sessRuleReports");

    OpenAPI_list_t *sess_rule_reportsList;
    if (sess_rule_reports) {
    cJSON *sess_rule_reports_local_nonprimitive;
    if (!cJSON_IsArray(sess_rule_reports)){
        ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [sess_rule_reports]");
        goto end;
    }

    sess_rule_reportsList = OpenAPI_list_create();

    cJSON_ArrayForEach(sess_rule_reports_local_nonprimitive, sess_rule_reports ) {
        if (!cJSON_IsObject(sess_rule_reports_local_nonprimitive)) {
            ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [sess_rule_reports]");
            goto end;
        }
        OpenAPI_session_rule_report_t *sess_rule_reportsItem = OpenAPI_session_rule_report_parseFromJSON(sess_rule_reports_local_nonprimitive);

        if (!sess_rule_reportsItem) {
            ogs_error("No sess_rule_reportsItem");
            OpenAPI_list_free(sess_rule_reportsList);
            goto end;
        }

        OpenAPI_list_add(sess_rule_reportsList, sess_rule_reportsItem);
    }
    }

    cJSON *ue_camping_rep = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "ueCampingRep");

    OpenAPI_ue_camping_rep_t *ue_camping_rep_local_nonprim = NULL;
    if (ue_camping_rep) {
    ue_camping_rep_local_nonprim = OpenAPI_ue_camping_rep_parseFromJSON(ue_camping_rep);
    }

    cJSON *policy_dec_failure_reports = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "policyDecFailureReports");

    OpenAPI_list_t *policy_dec_failure_reportsList;
    if (policy_dec_failure_reports) {
    cJSON *policy_dec_failure_reports_local_nonprimitive;
    if (!cJSON_IsArray(policy_dec_failure_reports)) {
        ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [policy_dec_failure_reports]");
        goto end;
    }

    policy_dec_failure_reportsList = OpenAPI_list_create();

    cJSON_ArrayForEach(policy_dec_failure_reports_local_nonprimitive, policy_dec_failure_reports ) {
        if (!cJSON_IsString(policy_dec_failure_reports_local_nonprimitive)){
            ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [policy_dec_failure_reports]");
            goto end;
        }

        OpenAPI_list_add(policy_dec_failure_reportsList, (void *)OpenAPI_policy_decision_failure_code_FromString(policy_dec_failure_reports_local_nonprimitive->valuestring));
    }
    }

    partial_success_report_local_var = OpenAPI_partial_success_report_create (
        failure_causeVariable,
        rule_reports ? rule_reportsList : NULL,
        sess_rule_reports ? sess_rule_reportsList : NULL,
        ue_camping_rep ? ue_camping_rep_local_nonprim : NULL,
        policy_dec_failure_reports ? policy_dec_failure_reportsList : NULL
    );

    return partial_success_report_local_var;
end:
    return NULL;
}

OpenAPI_partial_success_report_t *OpenAPI_partial_success_report_copy(OpenAPI_partial_success_report_t *dst, OpenAPI_partial_success_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_partial_success_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed");
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

    OpenAPI_partial_success_report_free(dst);
    dst = OpenAPI_partial_success_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

