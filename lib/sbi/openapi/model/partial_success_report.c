
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "partial_success_report.h"

OpenAPI_partial_success_report_t *OpenAPI_partial_success_report_create(
    OpenAPI_failure_cause_t *failure_cause,
    OpenAPI_list_t *rule_reports,
    OpenAPI_list_t *sess_rule_reports,
    OpenAPI_ue_camping_rep_t *ue_camping_rep
    )
{
    OpenAPI_partial_success_report_t *partial_success_report_local_var = OpenAPI_malloc(sizeof(OpenAPI_partial_success_report_t));
    if (!partial_success_report_local_var) {
        return NULL;
    }
    partial_success_report_local_var->failure_cause = failure_cause;
    partial_success_report_local_var->rule_reports = rule_reports;
    partial_success_report_local_var->sess_rule_reports = sess_rule_reports;
    partial_success_report_local_var->ue_camping_rep = ue_camping_rep;

    return partial_success_report_local_var;
}

void OpenAPI_partial_success_report_free(OpenAPI_partial_success_report_t *partial_success_report)
{
    if (NULL == partial_success_report) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_failure_cause_free(partial_success_report->failure_cause);
    OpenAPI_list_for_each(partial_success_report->rule_reports, node) {
        OpenAPI_rule_report_free(node->data);
    }
    OpenAPI_list_free(partial_success_report->rule_reports);
    OpenAPI_list_for_each(partial_success_report->sess_rule_reports, node) {
        OpenAPI_session_rule_report_free(node->data);
    }
    OpenAPI_list_free(partial_success_report->sess_rule_reports);
    OpenAPI_ue_camping_rep_free(partial_success_report->ue_camping_rep);
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
    if (!partial_success_report->failure_cause) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [failure_cause]");
        goto end;
    }
    cJSON *failure_cause_local_JSON = OpenAPI_failure_cause_convertToJSON(partial_success_report->failure_cause);
    if (failure_cause_local_JSON == NULL) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [failure_cause]");
        goto end;
    }
    cJSON_AddItemToObject(item, "failureCause", failure_cause_local_JSON);
    if (item->child == NULL) {
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

    OpenAPI_failure_cause_t *failure_cause_local_nonprim = NULL;

    failure_cause_local_nonprim = OpenAPI_failure_cause_parseFromJSON(failure_cause);

    cJSON *rule_reports = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "ruleReports");

    OpenAPI_list_t *rule_reportsList;
    if (rule_reports) {
        cJSON *rule_reports_local_nonprimitive;
        if (!cJSON_IsArray(rule_reports)) {
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

            OpenAPI_list_add(rule_reportsList, rule_reportsItem);
        }
    }

    cJSON *sess_rule_reports = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "sessRuleReports");

    OpenAPI_list_t *sess_rule_reportsList;
    if (sess_rule_reports) {
        cJSON *sess_rule_reports_local_nonprimitive;
        if (!cJSON_IsArray(sess_rule_reports)) {
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

            OpenAPI_list_add(sess_rule_reportsList, sess_rule_reportsItem);
        }
    }

    cJSON *ue_camping_rep = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "ueCampingRep");

    OpenAPI_ue_camping_rep_t *ue_camping_rep_local_nonprim = NULL;
    if (ue_camping_rep) {
        ue_camping_rep_local_nonprim = OpenAPI_ue_camping_rep_parseFromJSON(ue_camping_rep);
    }

    partial_success_report_local_var = OpenAPI_partial_success_report_create (
        failure_cause_local_nonprim,
        rule_reports ? rule_reportsList : NULL,
        sess_rule_reports ? sess_rule_reportsList : NULL,
        ue_camping_rep ? ue_camping_rep_local_nonprim : NULL
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

