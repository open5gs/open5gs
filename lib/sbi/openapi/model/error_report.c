
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "error_report.h"

OpenAPI_error_report_t *OpenAPI_error_report_create(
    OpenAPI_problem_details_t *error,
    OpenAPI_list_t *rule_reports,
    OpenAPI_list_t *sess_rule_reports,
    OpenAPI_list_t *pol_dec_failure_reports,
    OpenAPI_list_t *invalid_policy_decs
)
{
    OpenAPI_error_report_t *error_report_local_var = ogs_malloc(sizeof(OpenAPI_error_report_t));
    ogs_assert(error_report_local_var);

    error_report_local_var->error = error;
    error_report_local_var->rule_reports = rule_reports;
    error_report_local_var->sess_rule_reports = sess_rule_reports;
    error_report_local_var->pol_dec_failure_reports = pol_dec_failure_reports;
    error_report_local_var->invalid_policy_decs = invalid_policy_decs;

    return error_report_local_var;
}

void OpenAPI_error_report_free(OpenAPI_error_report_t *error_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == error_report) {
        return;
    }
    if (error_report->error) {
        OpenAPI_problem_details_free(error_report->error);
        error_report->error = NULL;
    }
    if (error_report->rule_reports) {
        OpenAPI_list_for_each(error_report->rule_reports, node) {
            OpenAPI_rule_report_free(node->data);
        }
        OpenAPI_list_free(error_report->rule_reports);
        error_report->rule_reports = NULL;
    }
    if (error_report->sess_rule_reports) {
        OpenAPI_list_for_each(error_report->sess_rule_reports, node) {
            OpenAPI_session_rule_report_free(node->data);
        }
        OpenAPI_list_free(error_report->sess_rule_reports);
        error_report->sess_rule_reports = NULL;
    }
    if (error_report->pol_dec_failure_reports) {
        OpenAPI_list_free(error_report->pol_dec_failure_reports);
        error_report->pol_dec_failure_reports = NULL;
    }
    if (error_report->invalid_policy_decs) {
        OpenAPI_list_for_each(error_report->invalid_policy_decs, node) {
            OpenAPI_invalid_param_free(node->data);
        }
        OpenAPI_list_free(error_report->invalid_policy_decs);
        error_report->invalid_policy_decs = NULL;
    }
    ogs_free(error_report);
}

cJSON *OpenAPI_error_report_convertToJSON(OpenAPI_error_report_t *error_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    OpenAPI_list_for_each(error_report->rule_reports, node) {
        cJSON *itemLocal = OpenAPI_rule_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_error_report_convertToJSON() failed [rule_reports]");
            goto end;
        }
        cJSON_AddItemToArray(rule_reportsList, itemLocal);
    }
    }

    if (error_report->sess_rule_reports) {
    cJSON *sess_rule_reportsList = cJSON_AddArrayToObject(item, "sessRuleReports");
    if (sess_rule_reportsList == NULL) {
        ogs_error("OpenAPI_error_report_convertToJSON() failed [sess_rule_reports]");
        goto end;
    }
    OpenAPI_list_for_each(error_report->sess_rule_reports, node) {
        cJSON *itemLocal = OpenAPI_session_rule_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_error_report_convertToJSON() failed [sess_rule_reports]");
            goto end;
        }
        cJSON_AddItemToArray(sess_rule_reportsList, itemLocal);
    }
    }

    if (error_report->pol_dec_failure_reports != OpenAPI_policy_decision_failure_code_NULL) {
    cJSON *pol_dec_failure_reportsList = cJSON_AddArrayToObject(item, "polDecFailureReports");
    if (pol_dec_failure_reportsList == NULL) {
        ogs_error("OpenAPI_error_report_convertToJSON() failed [pol_dec_failure_reports]");
        goto end;
    }
    OpenAPI_list_for_each(error_report->pol_dec_failure_reports, node) {
        if (cJSON_AddStringToObject(pol_dec_failure_reportsList, "", OpenAPI_policy_decision_failure_code_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_error_report_convertToJSON() failed [pol_dec_failure_reports]");
            goto end;
        }
    }
    }

    if (error_report->invalid_policy_decs) {
    cJSON *invalid_policy_decsList = cJSON_AddArrayToObject(item, "invalidPolicyDecs");
    if (invalid_policy_decsList == NULL) {
        ogs_error("OpenAPI_error_report_convertToJSON() failed [invalid_policy_decs]");
        goto end;
    }
    OpenAPI_list_for_each(error_report->invalid_policy_decs, node) {
        cJSON *itemLocal = OpenAPI_invalid_param_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_error_report_convertToJSON() failed [invalid_policy_decs]");
            goto end;
        }
        cJSON_AddItemToArray(invalid_policy_decsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_error_report_t *OpenAPI_error_report_parseFromJSON(cJSON *error_reportJSON)
{
    OpenAPI_error_report_t *error_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *error = NULL;
    OpenAPI_problem_details_t *error_local_nonprim = NULL;
    cJSON *rule_reports = NULL;
    OpenAPI_list_t *rule_reportsList = NULL;
    cJSON *sess_rule_reports = NULL;
    OpenAPI_list_t *sess_rule_reportsList = NULL;
    cJSON *pol_dec_failure_reports = NULL;
    OpenAPI_list_t *pol_dec_failure_reportsList = NULL;
    cJSON *invalid_policy_decs = NULL;
    OpenAPI_list_t *invalid_policy_decsList = NULL;
    error = cJSON_GetObjectItemCaseSensitive(error_reportJSON, "error");
    if (error) {
    error_local_nonprim = OpenAPI_problem_details_parseFromJSON(error);
    if (!error_local_nonprim) {
        ogs_error("OpenAPI_problem_details_parseFromJSON failed [error]");
        goto end;
    }
    }

    rule_reports = cJSON_GetObjectItemCaseSensitive(error_reportJSON, "ruleReports");
    if (rule_reports) {
        cJSON *rule_reports_local = NULL;
        if (!cJSON_IsArray(rule_reports)) {
            ogs_error("OpenAPI_error_report_parseFromJSON() failed [rule_reports]");
            goto end;
        }

        rule_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(rule_reports_local, rule_reports) {
            if (!cJSON_IsObject(rule_reports_local)) {
                ogs_error("OpenAPI_error_report_parseFromJSON() failed [rule_reports]");
                goto end;
            }
            OpenAPI_rule_report_t *rule_reportsItem = OpenAPI_rule_report_parseFromJSON(rule_reports_local);
            if (!rule_reportsItem) {
                ogs_error("No rule_reportsItem");
                goto end;
            }
            OpenAPI_list_add(rule_reportsList, rule_reportsItem);
        }
    }

    sess_rule_reports = cJSON_GetObjectItemCaseSensitive(error_reportJSON, "sessRuleReports");
    if (sess_rule_reports) {
        cJSON *sess_rule_reports_local = NULL;
        if (!cJSON_IsArray(sess_rule_reports)) {
            ogs_error("OpenAPI_error_report_parseFromJSON() failed [sess_rule_reports]");
            goto end;
        }

        sess_rule_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(sess_rule_reports_local, sess_rule_reports) {
            if (!cJSON_IsObject(sess_rule_reports_local)) {
                ogs_error("OpenAPI_error_report_parseFromJSON() failed [sess_rule_reports]");
                goto end;
            }
            OpenAPI_session_rule_report_t *sess_rule_reportsItem = OpenAPI_session_rule_report_parseFromJSON(sess_rule_reports_local);
            if (!sess_rule_reportsItem) {
                ogs_error("No sess_rule_reportsItem");
                goto end;
            }
            OpenAPI_list_add(sess_rule_reportsList, sess_rule_reportsItem);
        }
    }

    pol_dec_failure_reports = cJSON_GetObjectItemCaseSensitive(error_reportJSON, "polDecFailureReports");
    if (pol_dec_failure_reports) {
        cJSON *pol_dec_failure_reports_local = NULL;
        if (!cJSON_IsArray(pol_dec_failure_reports)) {
            ogs_error("OpenAPI_error_report_parseFromJSON() failed [pol_dec_failure_reports]");
            goto end;
        }

        pol_dec_failure_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pol_dec_failure_reports_local, pol_dec_failure_reports) {
            OpenAPI_policy_decision_failure_code_e localEnum = OpenAPI_policy_decision_failure_code_NULL;
            if (!cJSON_IsString(pol_dec_failure_reports_local)) {
                ogs_error("OpenAPI_error_report_parseFromJSON() failed [pol_dec_failure_reports]");
                goto end;
            }
            localEnum = OpenAPI_policy_decision_failure_code_FromString(pol_dec_failure_reports_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"pol_dec_failure_reports\" is not supported. Ignoring it ...",
                         pol_dec_failure_reports_local->valuestring);
            } else {
                OpenAPI_list_add(pol_dec_failure_reportsList, (void *)localEnum);
            }
        }
        if (pol_dec_failure_reportsList->count == 0) {
            ogs_error("OpenAPI_error_report_parseFromJSON() failed: Expected pol_dec_failure_reportsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    invalid_policy_decs = cJSON_GetObjectItemCaseSensitive(error_reportJSON, "invalidPolicyDecs");
    if (invalid_policy_decs) {
        cJSON *invalid_policy_decs_local = NULL;
        if (!cJSON_IsArray(invalid_policy_decs)) {
            ogs_error("OpenAPI_error_report_parseFromJSON() failed [invalid_policy_decs]");
            goto end;
        }

        invalid_policy_decsList = OpenAPI_list_create();

        cJSON_ArrayForEach(invalid_policy_decs_local, invalid_policy_decs) {
            if (!cJSON_IsObject(invalid_policy_decs_local)) {
                ogs_error("OpenAPI_error_report_parseFromJSON() failed [invalid_policy_decs]");
                goto end;
            }
            OpenAPI_invalid_param_t *invalid_policy_decsItem = OpenAPI_invalid_param_parseFromJSON(invalid_policy_decs_local);
            if (!invalid_policy_decsItem) {
                ogs_error("No invalid_policy_decsItem");
                goto end;
            }
            OpenAPI_list_add(invalid_policy_decsList, invalid_policy_decsItem);
        }
    }

    error_report_local_var = OpenAPI_error_report_create (
        error ? error_local_nonprim : NULL,
        rule_reports ? rule_reportsList : NULL,
        sess_rule_reports ? sess_rule_reportsList : NULL,
        pol_dec_failure_reports ? pol_dec_failure_reportsList : NULL,
        invalid_policy_decs ? invalid_policy_decsList : NULL
    );

    return error_report_local_var;
end:
    if (error_local_nonprim) {
        OpenAPI_problem_details_free(error_local_nonprim);
        error_local_nonprim = NULL;
    }
    if (rule_reportsList) {
        OpenAPI_list_for_each(rule_reportsList, node) {
            OpenAPI_rule_report_free(node->data);
        }
        OpenAPI_list_free(rule_reportsList);
        rule_reportsList = NULL;
    }
    if (sess_rule_reportsList) {
        OpenAPI_list_for_each(sess_rule_reportsList, node) {
            OpenAPI_session_rule_report_free(node->data);
        }
        OpenAPI_list_free(sess_rule_reportsList);
        sess_rule_reportsList = NULL;
    }
    if (pol_dec_failure_reportsList) {
        OpenAPI_list_free(pol_dec_failure_reportsList);
        pol_dec_failure_reportsList = NULL;
    }
    if (invalid_policy_decsList) {
        OpenAPI_list_for_each(invalid_policy_decsList, node) {
            OpenAPI_invalid_param_free(node->data);
        }
        OpenAPI_list_free(invalid_policy_decsList);
        invalid_policy_decsList = NULL;
    }
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

