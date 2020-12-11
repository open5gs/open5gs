
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rule_status.h"

OpenAPI_rule_status_t *OpenAPI_rule_status_create(
    )
{
    OpenAPI_rule_status_t *rule_status_local_var = OpenAPI_malloc(sizeof(OpenAPI_rule_status_t));
    if (!rule_status_local_var) {
        return NULL;
    }

    return rule_status_local_var;
}

void OpenAPI_rule_status_free(OpenAPI_rule_status_t *rule_status)
{
    if (NULL == rule_status) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(rule_status);
}

cJSON *OpenAPI_rule_status_convertToJSON(OpenAPI_rule_status_t *rule_status)
{
    cJSON *item = NULL;

    if (rule_status == NULL) {
        ogs_error("OpenAPI_rule_status_convertToJSON() failed [RuleStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_rule_status_t *OpenAPI_rule_status_parseFromJSON(cJSON *rule_statusJSON)
{
    OpenAPI_rule_status_t *rule_status_local_var = NULL;
    rule_status_local_var = OpenAPI_rule_status_create (
        );

    return rule_status_local_var;
end:
    return NULL;
}

OpenAPI_rule_status_t *OpenAPI_rule_status_copy(OpenAPI_rule_status_t *dst, OpenAPI_rule_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rule_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rule_status_convertToJSON() failed");
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

    OpenAPI_rule_status_free(dst);
    dst = OpenAPI_rule_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

