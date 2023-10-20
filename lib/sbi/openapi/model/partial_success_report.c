
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "partial_success_report.h"

OpenAPI_partial_success_report_t *OpenAPI_partial_success_report_create(
    OpenAPI_failure_cause_e failure_cause,
    OpenAPI_list_t *rule_reports,
    OpenAPI_list_t *sess_rule_reports,
    OpenAPI_ue_camping_rep_t *ue_camping_rep,
    OpenAPI_list_t *policy_dec_failure_reports,
    OpenAPI_list_t *invalid_policy_decs
)
{
    OpenAPI_partial_success_report_t *partial_success_report_local_var = ogs_malloc(sizeof(OpenAPI_partial_success_report_t));
    ogs_assert(partial_success_report_local_var);

    partial_success_report_local_var->failure_cause = failure_cause;
    partial_success_report_local_var->rule_reports = rule_reports;
    partial_success_report_local_var->sess_rule_reports = sess_rule_reports;
    partial_success_report_local_var->ue_camping_rep = ue_camping_rep;
    partial_success_report_local_var->policy_dec_failure_reports = policy_dec_failure_reports;
    partial_success_report_local_var->invalid_policy_decs = invalid_policy_decs;

    return partial_success_report_local_var;
}

void OpenAPI_partial_success_report_free(OpenAPI_partial_success_report_t *partial_success_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == partial_success_report) {
        return;
    }
    if (partial_success_report->rule_reports) {
        OpenAPI_list_for_each(partial_success_report->rule_reports, node) {
            OpenAPI_rule_report_free(node->data);
        }
        OpenAPI_list_free(partial_success_report->rule_reports);
        partial_success_report->rule_reports = NULL;
    }
    if (partial_success_report->sess_rule_reports) {
        OpenAPI_list_for_each(partial_success_report->sess_rule_reports, node) {
            OpenAPI_session_rule_report_free(node->data);
        }
        OpenAPI_list_free(partial_success_report->sess_rule_reports);
        partial_success_report->sess_rule_reports = NULL;
    }
    if (partial_success_report->ue_camping_rep) {
        OpenAPI_ue_camping_rep_free(partial_success_report->ue_camping_rep);
        partial_success_report->ue_camping_rep = NULL;
    }
    if (partial_success_report->policy_dec_failure_reports) {
        OpenAPI_list_free(partial_success_report->policy_dec_failure_reports);
        partial_success_report->policy_dec_failure_reports = NULL;
    }
    if (partial_success_report->invalid_policy_decs) {
        OpenAPI_list_for_each(partial_success_report->invalid_policy_decs, node) {
            OpenAPI_invalid_param_free(node->data);
        }
        OpenAPI_list_free(partial_success_report->invalid_policy_decs);
        partial_success_report->invalid_policy_decs = NULL;
    }
    ogs_free(partial_success_report);
}

cJSON *OpenAPI_partial_success_report_convertToJSON(OpenAPI_partial_success_report_t *partial_success_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (partial_success_report == NULL) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [PartialSuccessReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (partial_success_report->failure_cause == OpenAPI_failure_cause_NULL) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [failure_cause]");
        return NULL;
    }
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
    OpenAPI_list_for_each(partial_success_report->rule_reports, node) {
        cJSON *itemLocal = OpenAPI_rule_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [rule_reports]");
            goto end;
        }
        cJSON_AddItemToArray(rule_reportsList, itemLocal);
    }
    }

    if (partial_success_report->sess_rule_reports) {
    cJSON *sess_rule_reportsList = cJSON_AddArrayToObject(item, "sessRuleReports");
    if (sess_rule_reportsList == NULL) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [sess_rule_reports]");
        goto end;
    }
    OpenAPI_list_for_each(partial_success_report->sess_rule_reports, node) {
        cJSON *itemLocal = OpenAPI_session_rule_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [sess_rule_reports]");
            goto end;
        }
        cJSON_AddItemToArray(sess_rule_reportsList, itemLocal);
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

    if (partial_success_report->policy_dec_failure_reports != OpenAPI_policy_decision_failure_code_NULL) {
    cJSON *policy_dec_failure_reportsList = cJSON_AddArrayToObject(item, "policyDecFailureReports");
    if (policy_dec_failure_reportsList == NULL) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [policy_dec_failure_reports]");
        goto end;
    }
    OpenAPI_list_for_each(partial_success_report->policy_dec_failure_reports, node) {
        if (cJSON_AddStringToObject(policy_dec_failure_reportsList, "", OpenAPI_policy_decision_failure_code_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [policy_dec_failure_reports]");
            goto end;
        }
    }
    }

    if (partial_success_report->invalid_policy_decs) {
    cJSON *invalid_policy_decsList = cJSON_AddArrayToObject(item, "invalidPolicyDecs");
    if (invalid_policy_decsList == NULL) {
        ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [invalid_policy_decs]");
        goto end;
    }
    OpenAPI_list_for_each(partial_success_report->invalid_policy_decs, node) {
        cJSON *itemLocal = OpenAPI_invalid_param_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_partial_success_report_convertToJSON() failed [invalid_policy_decs]");
            goto end;
        }
        cJSON_AddItemToArray(invalid_policy_decsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_partial_success_report_t *OpenAPI_partial_success_report_parseFromJSON(cJSON *partial_success_reportJSON)
{
    OpenAPI_partial_success_report_t *partial_success_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *failure_cause = NULL;
    OpenAPI_failure_cause_e failure_causeVariable = 0;
    cJSON *rule_reports = NULL;
    OpenAPI_list_t *rule_reportsList = NULL;
    cJSON *sess_rule_reports = NULL;
    OpenAPI_list_t *sess_rule_reportsList = NULL;
    cJSON *ue_camping_rep = NULL;
    OpenAPI_ue_camping_rep_t *ue_camping_rep_local_nonprim = NULL;
    cJSON *policy_dec_failure_reports = NULL;
    OpenAPI_list_t *policy_dec_failure_reportsList = NULL;
    cJSON *invalid_policy_decs = NULL;
    OpenAPI_list_t *invalid_policy_decsList = NULL;
    failure_cause = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "failureCause");
    if (!failure_cause) {
        ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [failure_cause]");
        goto end;
    }
    if (!cJSON_IsString(failure_cause)) {
        ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [failure_cause]");
        goto end;
    }
    failure_causeVariable = OpenAPI_failure_cause_FromString(failure_cause->valuestring);

    rule_reports = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "ruleReports");
    if (rule_reports) {
        cJSON *rule_reports_local = NULL;
        if (!cJSON_IsArray(rule_reports)) {
            ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [rule_reports]");
            goto end;
        }

        rule_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(rule_reports_local, rule_reports) {
            if (!cJSON_IsObject(rule_reports_local)) {
                ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [rule_reports]");
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

    sess_rule_reports = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "sessRuleReports");
    if (sess_rule_reports) {
        cJSON *sess_rule_reports_local = NULL;
        if (!cJSON_IsArray(sess_rule_reports)) {
            ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [sess_rule_reports]");
            goto end;
        }

        sess_rule_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(sess_rule_reports_local, sess_rule_reports) {
            if (!cJSON_IsObject(sess_rule_reports_local)) {
                ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [sess_rule_reports]");
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

    ue_camping_rep = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "ueCampingRep");
    if (ue_camping_rep) {
    ue_camping_rep_local_nonprim = OpenAPI_ue_camping_rep_parseFromJSON(ue_camping_rep);
    if (!ue_camping_rep_local_nonprim) {
        ogs_error("OpenAPI_ue_camping_rep_parseFromJSON failed [ue_camping_rep]");
        goto end;
    }
    }

    policy_dec_failure_reports = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "policyDecFailureReports");
    if (policy_dec_failure_reports) {
        cJSON *policy_dec_failure_reports_local = NULL;
        if (!cJSON_IsArray(policy_dec_failure_reports)) {
            ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [policy_dec_failure_reports]");
            goto end;
        }

        policy_dec_failure_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(policy_dec_failure_reports_local, policy_dec_failure_reports) {
            OpenAPI_policy_decision_failure_code_e localEnum = OpenAPI_policy_decision_failure_code_NULL;
            if (!cJSON_IsString(policy_dec_failure_reports_local)) {
                ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [policy_dec_failure_reports]");
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
            ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed: Expected policy_dec_failure_reportsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    invalid_policy_decs = cJSON_GetObjectItemCaseSensitive(partial_success_reportJSON, "invalidPolicyDecs");
    if (invalid_policy_decs) {
        cJSON *invalid_policy_decs_local = NULL;
        if (!cJSON_IsArray(invalid_policy_decs)) {
            ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [invalid_policy_decs]");
            goto end;
        }

        invalid_policy_decsList = OpenAPI_list_create();

        cJSON_ArrayForEach(invalid_policy_decs_local, invalid_policy_decs) {
            if (!cJSON_IsObject(invalid_policy_decs_local)) {
                ogs_error("OpenAPI_partial_success_report_parseFromJSON() failed [invalid_policy_decs]");
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

    partial_success_report_local_var = OpenAPI_partial_success_report_create (
        failure_causeVariable,
        rule_reports ? rule_reportsList : NULL,
        sess_rule_reports ? sess_rule_reportsList : NULL,
        ue_camping_rep ? ue_camping_rep_local_nonprim : NULL,
        policy_dec_failure_reports ? policy_dec_failure_reportsList : NULL,
        invalid_policy_decs ? invalid_policy_decsList : NULL
    );

    return partial_success_report_local_var;
end:
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
    if (ue_camping_rep_local_nonprim) {
        OpenAPI_ue_camping_rep_free(ue_camping_rep_local_nonprim);
        ue_camping_rep_local_nonprim = NULL;
    }
    if (policy_dec_failure_reportsList) {
        OpenAPI_list_free(policy_dec_failure_reportsList);
        policy_dec_failure_reportsList = NULL;
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

