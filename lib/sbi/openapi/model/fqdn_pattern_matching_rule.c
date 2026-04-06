
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "fqdn_pattern_matching_rule.h"

OpenAPI_fqdn_pattern_matching_rule_t *OpenAPI_fqdn_pattern_matching_rule_create(
    char *regex,
    OpenAPI_string_matching_rule_t *string_matching_rule
)
{
    OpenAPI_fqdn_pattern_matching_rule_t *fqdn_pattern_matching_rule_local_var = ogs_malloc(sizeof(OpenAPI_fqdn_pattern_matching_rule_t));
    ogs_assert(fqdn_pattern_matching_rule_local_var);

    fqdn_pattern_matching_rule_local_var->regex = regex;
    fqdn_pattern_matching_rule_local_var->string_matching_rule = string_matching_rule;

    return fqdn_pattern_matching_rule_local_var;
}

void OpenAPI_fqdn_pattern_matching_rule_free(OpenAPI_fqdn_pattern_matching_rule_t *fqdn_pattern_matching_rule)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == fqdn_pattern_matching_rule) {
        return;
    }
    if (fqdn_pattern_matching_rule->regex) {
        ogs_free(fqdn_pattern_matching_rule->regex);
        fqdn_pattern_matching_rule->regex = NULL;
    }
    if (fqdn_pattern_matching_rule->string_matching_rule) {
        OpenAPI_string_matching_rule_free(fqdn_pattern_matching_rule->string_matching_rule);
        fqdn_pattern_matching_rule->string_matching_rule = NULL;
    }
    ogs_free(fqdn_pattern_matching_rule);
}

cJSON *OpenAPI_fqdn_pattern_matching_rule_convertToJSON(OpenAPI_fqdn_pattern_matching_rule_t *fqdn_pattern_matching_rule)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (fqdn_pattern_matching_rule == NULL) {
        ogs_error("OpenAPI_fqdn_pattern_matching_rule_convertToJSON() failed [FqdnPatternMatchingRule]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (fqdn_pattern_matching_rule->regex) {
    if (cJSON_AddStringToObject(item, "regex", fqdn_pattern_matching_rule->regex) == NULL) {
        ogs_error("OpenAPI_fqdn_pattern_matching_rule_convertToJSON() failed [regex]");
        goto end;
    }
    }

    if (fqdn_pattern_matching_rule->string_matching_rule) {
    cJSON *string_matching_rule_local_JSON = OpenAPI_string_matching_rule_convertToJSON(fqdn_pattern_matching_rule->string_matching_rule);
    if (string_matching_rule_local_JSON == NULL) {
        ogs_error("OpenAPI_fqdn_pattern_matching_rule_convertToJSON() failed [string_matching_rule]");
        goto end;
    }
    cJSON_AddItemToObject(item, "stringMatchingRule", string_matching_rule_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_fqdn_pattern_matching_rule_convertToJSON() failed [string_matching_rule]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_fqdn_pattern_matching_rule_t *OpenAPI_fqdn_pattern_matching_rule_parseFromJSON(cJSON *fqdn_pattern_matching_ruleJSON)
{
    OpenAPI_fqdn_pattern_matching_rule_t *fqdn_pattern_matching_rule_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *regex = NULL;
    cJSON *string_matching_rule = NULL;
    OpenAPI_string_matching_rule_t *string_matching_rule_local_nonprim = NULL;
    regex = cJSON_GetObjectItemCaseSensitive(fqdn_pattern_matching_ruleJSON, "regex");
    if (regex) {
    if (!cJSON_IsString(regex) && !cJSON_IsNull(regex)) {
        ogs_error("OpenAPI_fqdn_pattern_matching_rule_parseFromJSON() failed [regex]");
        goto end;
    }
    }

    string_matching_rule = cJSON_GetObjectItemCaseSensitive(fqdn_pattern_matching_ruleJSON, "stringMatchingRule");
    if (string_matching_rule) {
    string_matching_rule_local_nonprim = OpenAPI_string_matching_rule_parseFromJSON(string_matching_rule);
    if (!string_matching_rule_local_nonprim) {
        ogs_error("OpenAPI_string_matching_rule_parseFromJSON failed [string_matching_rule]");
        goto end;
    }
    }

    fqdn_pattern_matching_rule_local_var = OpenAPI_fqdn_pattern_matching_rule_create (
        regex && !cJSON_IsNull(regex) ? ogs_strdup(regex->valuestring) : NULL,
        string_matching_rule ? string_matching_rule_local_nonprim : NULL
    );

    return fqdn_pattern_matching_rule_local_var;
end:
    if (string_matching_rule_local_nonprim) {
        OpenAPI_string_matching_rule_free(string_matching_rule_local_nonprim);
        string_matching_rule_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_fqdn_pattern_matching_rule_t *OpenAPI_fqdn_pattern_matching_rule_copy(OpenAPI_fqdn_pattern_matching_rule_t *dst, OpenAPI_fqdn_pattern_matching_rule_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_fqdn_pattern_matching_rule_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_fqdn_pattern_matching_rule_convertToJSON() failed");
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

    OpenAPI_fqdn_pattern_matching_rule_free(dst);
    dst = OpenAPI_fqdn_pattern_matching_rule_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

