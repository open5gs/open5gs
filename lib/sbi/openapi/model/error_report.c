
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "error_report.h"

OpenAPI_error_report_t *OpenAPI_error_report_create(
    OpenAPI_problem_details_t *error,
    OpenAPI_list_t *rule_reports,
    OpenAPI_list_t *sess_rule_reports
    )
{
    OpenAPI_error_report_t *error_report_local_var = OpenAPI_malloc(sizeof(OpenAPI_error_report_t));
    if (!error_report_local_var) {
        return NULL;
    }
    error_report_local_var->error = error;
    error_report_local_var->rule_reports = rule_reports;
    error_report_local_var->sess_rule_reports = sess_rule_reports;

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
        if (!cJSON_IsArray(rule_reports)) {
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

            OpenAPI_list_add(rule_reportsList, rule_reportsItem);
        }
    }

    cJSON *sess_rule_reports = cJSON_GetObjectItemCaseSensitive(error_reportJSON, "sessRuleReports");

    OpenAPI_list_t *sess_rule_reportsList;
    if (sess_rule_reports) {
        cJSON *sess_rule_reports_local_nonprimitive;
        if (!cJSON_IsArray(sess_rule_reports)) {
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

            OpenAPI_list_add(sess_rule_reportsList, sess_rule_reportsItem);
        }
    }

    error_report_local_var = OpenAPI_error_report_create (
        error ? error_local_nonprim : NULL,
        rule_reports ? rule_reportsList : NULL,
        sess_rule_reports ? sess_rule_reportsList : NULL
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

