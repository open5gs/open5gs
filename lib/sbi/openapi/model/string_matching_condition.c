
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "string_matching_condition.h"

OpenAPI_string_matching_condition_t *OpenAPI_string_matching_condition_create(
    char *matching_string,
    OpenAPI_matching_operator_e matching_operator
)
{
    OpenAPI_string_matching_condition_t *string_matching_condition_local_var = ogs_malloc(sizeof(OpenAPI_string_matching_condition_t));
    ogs_assert(string_matching_condition_local_var);

    string_matching_condition_local_var->matching_string = matching_string;
    string_matching_condition_local_var->matching_operator = matching_operator;

    return string_matching_condition_local_var;
}

void OpenAPI_string_matching_condition_free(OpenAPI_string_matching_condition_t *string_matching_condition)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == string_matching_condition) {
        return;
    }
    if (string_matching_condition->matching_string) {
        ogs_free(string_matching_condition->matching_string);
        string_matching_condition->matching_string = NULL;
    }
    ogs_free(string_matching_condition);
}

cJSON *OpenAPI_string_matching_condition_convertToJSON(OpenAPI_string_matching_condition_t *string_matching_condition)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (string_matching_condition == NULL) {
        ogs_error("OpenAPI_string_matching_condition_convertToJSON() failed [StringMatchingCondition]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (string_matching_condition->matching_string) {
    if (cJSON_AddStringToObject(item, "matchingString", string_matching_condition->matching_string) == NULL) {
        ogs_error("OpenAPI_string_matching_condition_convertToJSON() failed [matching_string]");
        goto end;
    }
    }

    if (string_matching_condition->matching_operator == OpenAPI_matching_operator_NULL) {
        ogs_error("OpenAPI_string_matching_condition_convertToJSON() failed [matching_operator]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "matchingOperator", OpenAPI_matching_operator_ToString(string_matching_condition->matching_operator)) == NULL) {
        ogs_error("OpenAPI_string_matching_condition_convertToJSON() failed [matching_operator]");
        goto end;
    }

end:
    return item;
}

OpenAPI_string_matching_condition_t *OpenAPI_string_matching_condition_parseFromJSON(cJSON *string_matching_conditionJSON)
{
    OpenAPI_string_matching_condition_t *string_matching_condition_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *matching_string = NULL;
    cJSON *matching_operator = NULL;
    OpenAPI_matching_operator_e matching_operatorVariable = 0;
    matching_string = cJSON_GetObjectItemCaseSensitive(string_matching_conditionJSON, "matchingString");
    if (matching_string) {
    if (!cJSON_IsString(matching_string) && !cJSON_IsNull(matching_string)) {
        ogs_error("OpenAPI_string_matching_condition_parseFromJSON() failed [matching_string]");
        goto end;
    }
    }

    matching_operator = cJSON_GetObjectItemCaseSensitive(string_matching_conditionJSON, "matchingOperator");
    if (!matching_operator) {
        ogs_error("OpenAPI_string_matching_condition_parseFromJSON() failed [matching_operator]");
        goto end;
    }
    if (!cJSON_IsString(matching_operator)) {
        ogs_error("OpenAPI_string_matching_condition_parseFromJSON() failed [matching_operator]");
        goto end;
    }
    matching_operatorVariable = OpenAPI_matching_operator_FromString(matching_operator->valuestring);

    string_matching_condition_local_var = OpenAPI_string_matching_condition_create (
        matching_string && !cJSON_IsNull(matching_string) ? ogs_strdup(matching_string->valuestring) : NULL,
        matching_operatorVariable
    );

    return string_matching_condition_local_var;
end:
    return NULL;
}

OpenAPI_string_matching_condition_t *OpenAPI_string_matching_condition_copy(OpenAPI_string_matching_condition_t *dst, OpenAPI_string_matching_condition_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_string_matching_condition_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_string_matching_condition_convertToJSON() failed");
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

    OpenAPI_string_matching_condition_free(dst);
    dst = OpenAPI_string_matching_condition_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

