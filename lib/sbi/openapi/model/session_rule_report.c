
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "session_rule_report.h"

OpenAPI_session_rule_report_t *OpenAPI_session_rule_report_create(
    OpenAPI_list_t *rule_ids,
    OpenAPI_rule_status_e rule_status,
    OpenAPI_session_rule_failure_code_e sess_rule_failure_code,
    OpenAPI_list_t *policy_dec_failure_reports
)
{
    OpenAPI_session_rule_report_t *session_rule_report_local_var = ogs_malloc(sizeof(OpenAPI_session_rule_report_t));
    ogs_assert(session_rule_report_local_var);

    session_rule_report_local_var->rule_ids = rule_ids;
    session_rule_report_local_var->rule_status = rule_status;
    session_rule_report_local_var->sess_rule_failure_code = sess_rule_failure_code;
    session_rule_report_local_var->policy_dec_failure_reports = policy_dec_failure_reports;

    return session_rule_report_local_var;
}

void OpenAPI_session_rule_report_free(OpenAPI_session_rule_report_t *session_rule_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == session_rule_report) {
        return;
    }
    if (session_rule_report->rule_ids) {
        OpenAPI_list_for_each(session_rule_report->rule_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(session_rule_report->rule_ids);
        session_rule_report->rule_ids = NULL;
    }
    if (session_rule_report->policy_dec_failure_reports) {
        OpenAPI_list_free(session_rule_report->policy_dec_failure_reports);
        session_rule_report->policy_dec_failure_reports = NULL;
    }
    ogs_free(session_rule_report);
}

cJSON *OpenAPI_session_rule_report_convertToJSON(OpenAPI_session_rule_report_t *session_rule_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (session_rule_report == NULL) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [SessionRuleReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!session_rule_report->rule_ids) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [rule_ids]");
        return NULL;
    }
    cJSON *rule_idsList = cJSON_AddArrayToObject(item, "ruleIds");
    if (rule_idsList == NULL) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [rule_ids]");
        goto end;
    }
    OpenAPI_list_for_each(session_rule_report->rule_ids, node) {
        if (cJSON_AddStringToObject(rule_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [rule_ids]");
            goto end;
        }
    }

    if (session_rule_report->rule_status == OpenAPI_rule_status_NULL) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [rule_status]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ruleStatus", OpenAPI_rule_status_ToString(session_rule_report->rule_status)) == NULL) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [rule_status]");
        goto end;
    }

    if (session_rule_report->sess_rule_failure_code != OpenAPI_session_rule_failure_code_NULL) {
    if (cJSON_AddStringToObject(item, "sessRuleFailureCode", OpenAPI_session_rule_failure_code_ToString(session_rule_report->sess_rule_failure_code)) == NULL) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [sess_rule_failure_code]");
        goto end;
    }
    }

    if (session_rule_report->policy_dec_failure_reports != OpenAPI_policy_decision_failure_code_NULL) {
    cJSON *policy_dec_failure_reportsList = cJSON_AddArrayToObject(item, "policyDecFailureReports");
    if (policy_dec_failure_reportsList == NULL) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [policy_dec_failure_reports]");
        goto end;
    }
    OpenAPI_list_for_each(session_rule_report->policy_dec_failure_reports, node) {
        if (cJSON_AddStringToObject(policy_dec_failure_reportsList, "", OpenAPI_policy_decision_failure_code_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [policy_dec_failure_reports]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_session_rule_report_t *OpenAPI_session_rule_report_parseFromJSON(cJSON *session_rule_reportJSON)
{
    OpenAPI_session_rule_report_t *session_rule_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rule_ids = NULL;
    OpenAPI_list_t *rule_idsList = NULL;
    cJSON *rule_status = NULL;
    OpenAPI_rule_status_e rule_statusVariable = 0;
    cJSON *sess_rule_failure_code = NULL;
    OpenAPI_session_rule_failure_code_e sess_rule_failure_codeVariable = 0;
    cJSON *policy_dec_failure_reports = NULL;
    OpenAPI_list_t *policy_dec_failure_reportsList = NULL;
    rule_ids = cJSON_GetObjectItemCaseSensitive(session_rule_reportJSON, "ruleIds");
    if (!rule_ids) {
        ogs_error("OpenAPI_session_rule_report_parseFromJSON() failed [rule_ids]");
        goto end;
    }
        cJSON *rule_ids_local = NULL;
        if (!cJSON_IsArray(rule_ids)) {
            ogs_error("OpenAPI_session_rule_report_parseFromJSON() failed [rule_ids]");
            goto end;
        }

        rule_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(rule_ids_local, rule_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(rule_ids_local)) {
                ogs_error("OpenAPI_session_rule_report_parseFromJSON() failed [rule_ids]");
                goto end;
            }
            OpenAPI_list_add(rule_idsList, ogs_strdup(rule_ids_local->valuestring));
        }

    rule_status = cJSON_GetObjectItemCaseSensitive(session_rule_reportJSON, "ruleStatus");
    if (!rule_status) {
        ogs_error("OpenAPI_session_rule_report_parseFromJSON() failed [rule_status]");
        goto end;
    }
    if (!cJSON_IsString(rule_status)) {
        ogs_error("OpenAPI_session_rule_report_parseFromJSON() failed [rule_status]");
        goto end;
    }
    rule_statusVariable = OpenAPI_rule_status_FromString(rule_status->valuestring);

    sess_rule_failure_code = cJSON_GetObjectItemCaseSensitive(session_rule_reportJSON, "sessRuleFailureCode");
    if (sess_rule_failure_code) {
    if (!cJSON_IsString(sess_rule_failure_code)) {
        ogs_error("OpenAPI_session_rule_report_parseFromJSON() failed [sess_rule_failure_code]");
        goto end;
    }
    sess_rule_failure_codeVariable = OpenAPI_session_rule_failure_code_FromString(sess_rule_failure_code->valuestring);
    }

    policy_dec_failure_reports = cJSON_GetObjectItemCaseSensitive(session_rule_reportJSON, "policyDecFailureReports");
    if (policy_dec_failure_reports) {
        cJSON *policy_dec_failure_reports_local = NULL;
        if (!cJSON_IsArray(policy_dec_failure_reports)) {
            ogs_error("OpenAPI_session_rule_report_parseFromJSON() failed [policy_dec_failure_reports]");
            goto end;
        }

        policy_dec_failure_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(policy_dec_failure_reports_local, policy_dec_failure_reports) {
            OpenAPI_policy_decision_failure_code_e localEnum = OpenAPI_policy_decision_failure_code_NULL;
            if (!cJSON_IsString(policy_dec_failure_reports_local)) {
                ogs_error("OpenAPI_session_rule_report_parseFromJSON() failed [policy_dec_failure_reports]");
                goto end;
            }
            localEnum = OpenAPI_policy_decision_failure_code_FromString(policy_dec_failure_reports_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"policy_dec_failure_reports\" is not supported. Ignoring it ...",
                         policy_dec_failure_reports_local->valuestring);
            } else {
                OpenAPI_list_add(policy_dec_failure_reportsList, (void *)localEnum);
            }
        }
        if (policy_dec_failure_reportsList->count == 0) {
            ogs_error("OpenAPI_session_rule_report_parseFromJSON() failed: Expected policy_dec_failure_reportsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    session_rule_report_local_var = OpenAPI_session_rule_report_create (
        rule_idsList,
        rule_statusVariable,
        sess_rule_failure_code ? sess_rule_failure_codeVariable : 0,
        policy_dec_failure_reports ? policy_dec_failure_reportsList : NULL
    );

    return session_rule_report_local_var;
end:
    if (rule_idsList) {
        OpenAPI_list_for_each(rule_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(rule_idsList);
        rule_idsList = NULL;
    }
    if (policy_dec_failure_reportsList) {
        OpenAPI_list_free(policy_dec_failure_reportsList);
        policy_dec_failure_reportsList = NULL;
    }
    return NULL;
}

OpenAPI_session_rule_report_t *OpenAPI_session_rule_report_copy(OpenAPI_session_rule_report_t *dst, OpenAPI_session_rule_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_session_rule_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed");
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

    OpenAPI_session_rule_report_free(dst);
    dst = OpenAPI_session_rule_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

