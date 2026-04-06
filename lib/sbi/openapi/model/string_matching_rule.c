
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "string_matching_rule.h"

OpenAPI_string_matching_rule_t *OpenAPI_string_matching_rule_create(
    OpenAPI_list_t *string_matching_conditions
)
{
    OpenAPI_string_matching_rule_t *string_matching_rule_local_var = ogs_malloc(sizeof(OpenAPI_string_matching_rule_t));
    ogs_assert(string_matching_rule_local_var);

    string_matching_rule_local_var->string_matching_conditions = string_matching_conditions;

    return string_matching_rule_local_var;
}

void OpenAPI_string_matching_rule_free(OpenAPI_string_matching_rule_t *string_matching_rule)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == string_matching_rule) {
        return;
    }
    if (string_matching_rule->string_matching_conditions) {
        OpenAPI_list_for_each(string_matching_rule->string_matching_conditions, node) {
            OpenAPI_string_matching_condition_free(node->data);
        }
        OpenAPI_list_free(string_matching_rule->string_matching_conditions);
        string_matching_rule->string_matching_conditions = NULL;
    }
    ogs_free(string_matching_rule);
}

cJSON *OpenAPI_string_matching_rule_convertToJSON(OpenAPI_string_matching_rule_t *string_matching_rule)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (string_matching_rule == NULL) {
        ogs_error("OpenAPI_string_matching_rule_convertToJSON() failed [StringMatchingRule]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!string_matching_rule->string_matching_conditions) {
        ogs_error("OpenAPI_string_matching_rule_convertToJSON() failed [string_matching_conditions]");
        return NULL;
    }
    cJSON *string_matching_conditionsList = cJSON_AddArrayToObject(item, "stringMatchingConditions");
    if (string_matching_conditionsList == NULL) {
        ogs_error("OpenAPI_string_matching_rule_convertToJSON() failed [string_matching_conditions]");
        goto end;
    }
    OpenAPI_list_for_each(string_matching_rule->string_matching_conditions, node) {
        cJSON *itemLocal = OpenAPI_string_matching_condition_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_string_matching_rule_convertToJSON() failed [string_matching_conditions]");
            goto end;
        }
        cJSON_AddItemToArray(string_matching_conditionsList, itemLocal);
    }

end:
    return item;
}

OpenAPI_string_matching_rule_t *OpenAPI_string_matching_rule_parseFromJSON(cJSON *string_matching_ruleJSON)
{
    OpenAPI_string_matching_rule_t *string_matching_rule_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *string_matching_conditions = NULL;
    OpenAPI_list_t *string_matching_conditionsList = NULL;
    string_matching_conditions = cJSON_GetObjectItemCaseSensitive(string_matching_ruleJSON, "stringMatchingConditions");
    if (!string_matching_conditions) {
        ogs_error("OpenAPI_string_matching_rule_parseFromJSON() failed [string_matching_conditions]");
        goto end;
    }
        cJSON *string_matching_conditions_local = NULL;
        if (!cJSON_IsArray(string_matching_conditions)) {
            ogs_error("OpenAPI_string_matching_rule_parseFromJSON() failed [string_matching_conditions]");
            goto end;
        }

        string_matching_conditionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(string_matching_conditions_local, string_matching_conditions) {
            if (!cJSON_IsObject(string_matching_conditions_local)) {
                ogs_error("OpenAPI_string_matching_rule_parseFromJSON() failed [string_matching_conditions]");
                goto end;
            }
            OpenAPI_string_matching_condition_t *string_matching_conditionsItem = OpenAPI_string_matching_condition_parseFromJSON(string_matching_conditions_local);
            if (!string_matching_conditionsItem) {
                ogs_error("No string_matching_conditionsItem");
                goto end;
            }
            OpenAPI_list_add(string_matching_conditionsList, string_matching_conditionsItem);
        }

    string_matching_rule_local_var = OpenAPI_string_matching_rule_create (
        string_matching_conditionsList
    );

    return string_matching_rule_local_var;
end:
    if (string_matching_conditionsList) {
        OpenAPI_list_for_each(string_matching_conditionsList, node) {
            OpenAPI_string_matching_condition_free(node->data);
        }
        OpenAPI_list_free(string_matching_conditionsList);
        string_matching_conditionsList = NULL;
    }
    return NULL;
}

OpenAPI_string_matching_rule_t *OpenAPI_string_matching_rule_copy(OpenAPI_string_matching_rule_t *dst, OpenAPI_string_matching_rule_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_string_matching_rule_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_string_matching_rule_convertToJSON() failed");
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

    OpenAPI_string_matching_rule_free(dst);
    dst = OpenAPI_string_matching_rule_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

