
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "session_rule_failure_code.h"

OpenAPI_session_rule_failure_code_t *OpenAPI_session_rule_failure_code_create(
    )
{
    OpenAPI_session_rule_failure_code_t *session_rule_failure_code_local_var = OpenAPI_malloc(sizeof(OpenAPI_session_rule_failure_code_t));
    if (!session_rule_failure_code_local_var) {
        return NULL;
    }

    return session_rule_failure_code_local_var;
}

void OpenAPI_session_rule_failure_code_free(OpenAPI_session_rule_failure_code_t *session_rule_failure_code)
{
    if (NULL == session_rule_failure_code) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(session_rule_failure_code);
}

cJSON *OpenAPI_session_rule_failure_code_convertToJSON(OpenAPI_session_rule_failure_code_t *session_rule_failure_code)
{
    cJSON *item = NULL;

    if (session_rule_failure_code == NULL) {
        ogs_error("OpenAPI_session_rule_failure_code_convertToJSON() failed [SessionRuleFailureCode]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_session_rule_failure_code_t *OpenAPI_session_rule_failure_code_parseFromJSON(cJSON *session_rule_failure_codeJSON)
{
    OpenAPI_session_rule_failure_code_t *session_rule_failure_code_local_var = NULL;
    session_rule_failure_code_local_var = OpenAPI_session_rule_failure_code_create (
        );

    return session_rule_failure_code_local_var;
end:
    return NULL;
}

OpenAPI_session_rule_failure_code_t *OpenAPI_session_rule_failure_code_copy(OpenAPI_session_rule_failure_code_t *dst, OpenAPI_session_rule_failure_code_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_session_rule_failure_code_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_session_rule_failure_code_convertToJSON() failed");
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

    OpenAPI_session_rule_failure_code_free(dst);
    dst = OpenAPI_session_rule_failure_code_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

