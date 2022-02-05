
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "error_report.h"

OpenAPI_error_report_t *OpenAPI_error_report_create(
    OpenAPI_problem_details_t *error,
    OpenAPI_list_t *rule_reports,
    OpenAPI_list_t *sess_rule_reports,
    OpenAPI_list_t *pol_dec_failure_reports,
    char *alt_qos_param_id
)
{
    OpenAPI_error_report_t *error_report_local_var = ogs_malloc(sizeof(OpenAPI_error_report_t));
    ogs_assert(error_report_local_var);

    error_report_local_var->error = error;
    error_report_local_var->rule_reports = rule_reports;
    error_report_local_var->sess_rule_reports = sess_rule_reports;
    error_report_local_var->pol_dec_failure_reports = pol_dec_failure_reports;
    error_report_local_var->alt_qos_param_id = alt_qos_param_id;

    return error_report_local_var;
}

void OpenAPI_error_report_free(OpenAPI_error_report_t *error_report)
{
    if (NULL == error_report) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_problem_details_free(error_report->error);
    OpenAPI_list_for_each(error_report->rule_reports, node) {
        OpenAPI_rule_report_free(node->data);
    }
    OpenAPI_list_free(error_report->rule_reports);
    OpenAPI_list_for_each(error_report->sess_rule_reports, node) {
        OpenAPI_session_rule_report_free(node->data);
    }
    OpenAPI_list_free(error_report->sess_rule_reports);
    OpenAPI_list_free(error_report->pol_dec_failure_reports);
    ogs_free(error_report->alt_qos_param_id);
    ogs_free(error_report);
}

cJSON *OpenAPI_error_report_convertToJSON(OpenAPI_error_report_t *error_report)
{
    cJSON *item = NULL;

    if (error_report == NULL) {
        ogs_error("OpenAPI_error_report_convertToJSON() failed [ErrorReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (error_report->error) {
    cJSON *error_local_JSON = OpenAPI_problem_details_convertToJSON(error_report->error);
    if (error_local_JSON == NULL) {
        ogs_error("OpenAPI_error_report_convertToJSON() failed [error]");
        goto end;
    }
    cJSON_AddItemToObject(item, "error", error_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_error_report_convertToJSON() failed [error]");
        goto end;
    }
    }

    if (error_report->rule_reports) {
    cJSON *rule_reportsList = cJSON_AddArrayToObject(item, "ruleReports");
    if (rule_reportsList == NULL) {
        ogs_error("OpenAPI_error_report_convertToJSON() failed [rule_reports]");
        goto end;
    }

    OpenAPI_lnode_t *rule_reports_node;
    if (error_report->rule_reports) {
        OpenAPI_list_for_each(error_report->rule_reports, rule_reports_node) {
            cJSON *itemLocal = OpenAPI_rule_report_convertToJSON(rule_reports_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_error_report_convertToJSON() failed [rule_reports]");
                goto end;
            }
            cJSON_AddItemToArray(rule_reportsList, itemLocal);
        }
    }
    }

    if (error_report->sess_rule_reports) {
    cJSON *sess_rule_reportsList = cJSON_AddArrayToObject(item, "sessRuleReports");
    if (sess_rule_reportsList == NULL) {
        ogs_error("OpenAPI_error_report_convertToJSON() failed [sess_rule_reports]");
        goto end;
    }

    OpenAPI_lnode_t *sess_rule_reports_node;
    if (error_report->sess_rule_reports) {
        OpenAPI_list_for_each(error_report->sess_rule_reports, sess_rule_reports_node) {
            cJSON *itemLocal = OpenAPI_session_rule_report_convertToJSON(sess_rule_reports_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_error_report_convertToJSON() failed [sess_rule_reports]");
                goto end;
            }
            cJSON_AddItemToArray(sess_rule_reportsList, itemLocal);
        }
    }
    }

    if (error_report->pol_dec_failure_reports) {
    cJSON *pol_dec_failure_reports = cJSON_AddArrayToObject(item, "polDecFailureReports");
    if (pol_dec_failure_reports == NULL) {
        ogs_error("OpenAPI_error_report_convertToJSON() failed [pol_dec_failure_reports]");
        goto end;
    }
    OpenAPI_lnode_t *pol_dec_failure_reports_node;
    OpenAPI_list_for_each(error_report->pol_dec_failure_reports, pol_dec_failure_reports_node) {
        if (cJSON_AddStringToObject(pol_dec_failure_reports, "", OpenAPI_policy_decision_failure_code_ToString((intptr_t)pol_dec_failure_reports_node->data)) == NULL) {
            ogs_error("OpenAPI_error_report_convertToJSON() failed [pol_dec_failure_reports]");
            goto end;
        }
    }
    }

    if (error_report->alt_qos_param_id) {
    if (cJSON_AddStringToObject(item, "altQosParamId", error_report->alt_qos_param_id) == NULL) {
        ogs_error("OpenAPI_error_report_convertToJSON() failed [alt_qos_param_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_error_report_t *OpenAPI_error_report_parseFromJSON(cJSON *error_reportJSON)
{
    OpenAPI_error_report_t *error_report_local_var = NULL;
    cJSON *error = cJSON_GetObjectItemCaseSensitive(error_reportJSON, "error");

    OpenAPI_problem_details_t *error_local_nonprim = NULL;
    if (error) {
    error_local_nonprim = OpenAPI_problem_details_parseFromJSON(error);
    }

    cJSON *rule_reports = cJSON_GetObjectItemCaseSensitive(error_reportJSON, "ruleReports");

    OpenAPI_list_t *rule_reportsList;
    if (rule_reports) {
    cJSON *rule_reports_local_nonprimitive;
    if (!cJSON_IsArray(rule_reports)){
        ogs_error("OpenAPI_error_report_parseFromJSON() failed [rule_reports]");
        goto end;
    }

    rule_reportsList = OpenAPI_list_create();

    cJSON_ArrayForEach(rule_reports_local_nonprimitive, rule_reports ) {
        if (!cJSON_IsObject(rule_reports_local_nonprimitive)) {
            ogs_error("OpenAPI_error_report_parseFromJSON() failed [rule_reports]");
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

    cJSON *sess_rule_reports = cJSON_GetObjectItemCaseSensitive(error_reportJSON, "sessRuleReports");

    OpenAPI_list_t *sess_rule_reportsList;
    if (sess_rule_reports) {
    cJSON *sess_rule_reports_local_nonprimitive;
    if (!cJSON_IsArray(sess_rule_reports)){
        ogs_error("OpenAPI_error_report_parseFromJSON() failed [sess_rule_reports]");
        goto end;
    }

    sess_rule_reportsList = OpenAPI_list_create();

    cJSON_ArrayForEach(sess_rule_reports_local_nonprimitive, sess_rule_reports ) {
        if (!cJSON_IsObject(sess_rule_reports_local_nonprimitive)) {
            ogs_error("OpenAPI_error_report_parseFromJSON() failed [sess_rule_reports]");
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

    cJSON *pol_dec_failure_reports = cJSON_GetObjectItemCaseSensitive(error_reportJSON, "polDecFailureReports");

    OpenAPI_list_t *pol_dec_failure_reportsList;
    if (pol_dec_failure_reports) {
    cJSON *pol_dec_failure_reports_local_nonprimitive;
    if (!cJSON_IsArray(pol_dec_failure_reports)) {
        ogs_error("OpenAPI_error_report_parseFromJSON() failed [pol_dec_failure_reports]");
        goto end;
    }

    pol_dec_failure_reportsList = OpenAPI_list_create();

    cJSON_ArrayForEach(pol_dec_failure_reports_local_nonprimitive, pol_dec_failure_reports ) {
        if (!cJSON_IsString(pol_dec_failure_reports_local_nonprimitive)){
            ogs_error("OpenAPI_error_report_parseFromJSON() failed [pol_dec_failure_reports]");
            goto end;
        }

        OpenAPI_list_add(pol_dec_failure_reportsList, (void *)OpenAPI_policy_decision_failure_code_FromString(pol_dec_failure_reports_local_nonprimitive->valuestring));
    }
    }

    cJSON *alt_qos_param_id = cJSON_GetObjectItemCaseSensitive(error_reportJSON, "altQosParamId");

    if (alt_qos_param_id) {
    if (!cJSON_IsString(alt_qos_param_id)) {
        ogs_error("OpenAPI_error_report_parseFromJSON() failed [alt_qos_param_id]");
        goto end;
    }
    }

    error_report_local_var = OpenAPI_error_report_create (
        error ? error_local_nonprim : NULL,
        rule_reports ? rule_reportsList : NULL,
        sess_rule_reports ? sess_rule_reportsList : NULL,
        pol_dec_failure_reports ? pol_dec_failure_reportsList : NULL,
        alt_qos_param_id ? ogs_strdup(alt_qos_param_id->valuestring) : NULL
    );

    return error_report_local_var;
end:
    return NULL;
}

OpenAPI_error_report_t *OpenAPI_error_report_copy(OpenAPI_error_report_t *dst, OpenAPI_error_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_error_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_error_report_convertToJSON() failed");
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

    OpenAPI_error_report_free(dst);
    dst = OpenAPI_error_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

