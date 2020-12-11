
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rule_operation.h"

OpenAPI_rule_operation_t *OpenAPI_rule_operation_create(
    )
{
    OpenAPI_rule_operation_t *rule_operation_local_var = OpenAPI_malloc(sizeof(OpenAPI_rule_operation_t));
    if (!rule_operation_local_var) {
        return NULL;
    }

    return rule_operation_local_var;
}

void OpenAPI_rule_operation_free(OpenAPI_rule_operation_t *rule_operation)
{
    if (NULL == rule_operation) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(rule_operation);
}

cJSON *OpenAPI_rule_operation_convertToJSON(OpenAPI_rule_operation_t *rule_operation)
{
    cJSON *item = NULL;

    if (rule_operation == NULL) {
        ogs_error("OpenAPI_rule_operation_convertToJSON() failed [RuleOperation]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_rule_operation_t *OpenAPI_rule_operation_parseFromJSON(cJSON *rule_operationJSON)
{
    OpenAPI_rule_operation_t *rule_operation_local_var = NULL;
    rule_operation_local_var = OpenAPI_rule_operation_create (
        );

    return rule_operation_local_var;
end:
    return NULL;
}

OpenAPI_rule_operation_t *OpenAPI_rule_operation_copy(OpenAPI_rule_operation_t *dst, OpenAPI_rule_operation_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rule_operation_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rule_operation_convertToJSON() failed");
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

    OpenAPI_rule_operation_free(dst);
    dst = OpenAPI_rule_operation_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

