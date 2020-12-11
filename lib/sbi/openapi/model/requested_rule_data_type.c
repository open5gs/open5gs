
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "requested_rule_data_type.h"

OpenAPI_requested_rule_data_type_t *OpenAPI_requested_rule_data_type_create(
    )
{
    OpenAPI_requested_rule_data_type_t *requested_rule_data_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_requested_rule_data_type_t));
    if (!requested_rule_data_type_local_var) {
        return NULL;
    }

    return requested_rule_data_type_local_var;
}

void OpenAPI_requested_rule_data_type_free(OpenAPI_requested_rule_data_type_t *requested_rule_data_type)
{
    if (NULL == requested_rule_data_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(requested_rule_data_type);
}

cJSON *OpenAPI_requested_rule_data_type_convertToJSON(OpenAPI_requested_rule_data_type_t *requested_rule_data_type)
{
    cJSON *item = NULL;

    if (requested_rule_data_type == NULL) {
        ogs_error("OpenAPI_requested_rule_data_type_convertToJSON() failed [RequestedRuleDataType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_requested_rule_data_type_t *OpenAPI_requested_rule_data_type_parseFromJSON(cJSON *requested_rule_data_typeJSON)
{
    OpenAPI_requested_rule_data_type_t *requested_rule_data_type_local_var = NULL;
    requested_rule_data_type_local_var = OpenAPI_requested_rule_data_type_create (
        );

    return requested_rule_data_type_local_var;
end:
    return NULL;
}

OpenAPI_requested_rule_data_type_t *OpenAPI_requested_rule_data_type_copy(OpenAPI_requested_rule_data_type_t *dst, OpenAPI_requested_rule_data_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_requested_rule_data_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_requested_rule_data_type_convertToJSON() failed");
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

    OpenAPI_requested_rule_data_type_free(dst);
    dst = OpenAPI_requested_rule_data_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

