
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "session_rule_report.h"

OpenAPI_session_rule_report_t *OpenAPI_session_rule_report_create(
    OpenAPI_list_t *rule_ids,
    OpenAPI_rule_status_t *rule_status,
    OpenAPI_session_rule_failure_code_t *sess_rule_failure_code
    )
{
    OpenAPI_session_rule_report_t *session_rule_report_local_var = OpenAPI_malloc(sizeof(OpenAPI_session_rule_report_t));
    if (!session_rule_report_local_var) {
        return NULL;
    }
    session_rule_report_local_var->rule_ids = rule_ids;
    session_rule_report_local_var->rule_status = rule_status;
    session_rule_report_local_var->sess_rule_failure_code = sess_rule_failure_code;

    return session_rule_report_local_var;
}

void OpenAPI_session_rule_report_free(OpenAPI_session_rule_report_t *session_rule_report)
{
    if (NULL == session_rule_report) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(session_rule_report->rule_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(session_rule_report->rule_ids);
    OpenAPI_rule_status_free(session_rule_report->rule_status);
    OpenAPI_session_rule_failure_code_free(session_rule_report->sess_rule_failure_code);
    ogs_free(session_rule_report);
}

cJSON *OpenAPI_session_rule_report_convertToJSON(OpenAPI_session_rule_report_t *session_rule_report)
{
    cJSON *item = NULL;

    if (session_rule_report == NULL) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [SessionRuleReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!session_rule_report->rule_ids) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [rule_ids]");
        goto end;
    }
    cJSON *rule_ids = cJSON_AddArrayToObject(item, "ruleIds");
    if (rule_ids == NULL) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [rule_ids]");
        goto end;
    }

    OpenAPI_lnode_t *rule_ids_node;
    OpenAPI_list_for_each(session_rule_report->rule_ids, rule_ids_node)  {
        if (cJSON_AddStringToObject(rule_ids, "", (char*)rule_ids_node->data) == NULL) {
            ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [rule_ids]");
            goto end;
        }
    }

    if (!session_rule_report->rule_status) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [rule_status]");
        goto end;
    }
    cJSON *rule_status_local_JSON = OpenAPI_rule_status_convertToJSON(session_rule_report->rule_status);
    if (rule_status_local_JSON == NULL) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [rule_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ruleStatus", rule_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [rule_status]");
        goto end;
    }

    if (session_rule_report->sess_rule_failure_code) {
        cJSON *sess_rule_failure_code_local_JSON = OpenAPI_session_rule_failure_code_convertToJSON(session_rule_report->sess_rule_failure_code);
        if (sess_rule_failure_code_local_JSON == NULL) {
            ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [sess_rule_failure_code]");
            goto end;
        }
        cJSON_AddItemToObject(item, "sessRuleFailureCode", sess_rule_failure_code_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_session_rule_report_convertToJSON() failed [sess_rule_failure_code]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_session_rule_report_t *OpenAPI_session_rule_report_parseFromJSON(cJSON *session_rule_reportJSON)
{
    OpenAPI_session_rule_report_t *session_rule_report_local_var = NULL;
    cJSON *rule_ids = cJSON_GetObjectItemCaseSensitive(session_rule_reportJSON, "ruleIds");
    if (!rule_ids) {
        ogs_error("OpenAPI_session_rule_report_parseFromJSON() failed [rule_ids]");
        goto end;
    }

    OpenAPI_list_t *rule_idsList;

    cJSON *rule_ids_local;
    if (!cJSON_IsArray(rule_ids)) {
        ogs_error("OpenAPI_session_rule_report_parseFromJSON() failed [rule_ids]");
        goto end;
    }
    rule_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(rule_ids_local, rule_ids) {
        if (!cJSON_IsString(rule_ids_local)) {
            ogs_error("OpenAPI_session_rule_report_parseFromJSON() failed [rule_ids]");
            goto end;
        }
        OpenAPI_list_add(rule_idsList, ogs_strdup(rule_ids_local->valuestring));
    }

    cJSON *rule_status = cJSON_GetObjectItemCaseSensitive(session_rule_reportJSON, "ruleStatus");
    if (!rule_status) {
        ogs_error("OpenAPI_session_rule_report_parseFromJSON() failed [rule_status]");
        goto end;
    }

    OpenAPI_rule_status_t *rule_status_local_nonprim = NULL;

    rule_status_local_nonprim = OpenAPI_rule_status_parseFromJSON(rule_status);

    cJSON *sess_rule_failure_code = cJSON_GetObjectItemCaseSensitive(session_rule_reportJSON, "sessRuleFailureCode");

    OpenAPI_session_rule_failure_code_t *sess_rule_failure_code_local_nonprim = NULL;
    if (sess_rule_failure_code) {
        sess_rule_failure_code_local_nonprim = OpenAPI_session_rule_failure_code_parseFromJSON(sess_rule_failure_code);
    }

    session_rule_report_local_var = OpenAPI_session_rule_report_create (
        rule_idsList,
        rule_status_local_nonprim,
        sess_rule_failure_code ? sess_rule_failure_code_local_nonprim : NULL
        );

    return session_rule_report_local_var;
end:
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

