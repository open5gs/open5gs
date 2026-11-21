
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "variable_part.h"

OpenAPI_variable_part_t *OpenAPI_variable_part_create(
    OpenAPI_variable_part_type_e variable_part_type,
    OpenAPI_list_t *variable_part_value,
    bool is_variable_part_order,
    int variable_part_order
)
{
    OpenAPI_variable_part_t *variable_part_local_var = ogs_malloc(sizeof(OpenAPI_variable_part_t));
    ogs_assert(variable_part_local_var);

    variable_part_local_var->variable_part_type = variable_part_type;
    variable_part_local_var->variable_part_value = variable_part_value;
    variable_part_local_var->is_variable_part_order = is_variable_part_order;
    variable_part_local_var->variable_part_order = variable_part_order;

    return variable_part_local_var;
}

void OpenAPI_variable_part_free(OpenAPI_variable_part_t *variable_part)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == variable_part) {
        return;
    }
    if (variable_part->variable_part_value) {
        OpenAPI_list_for_each(variable_part->variable_part_value, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(variable_part->variable_part_value);
        variable_part->variable_part_value = NULL;
    }
    ogs_free(variable_part);
}

cJSON *OpenAPI_variable_part_convertToJSON(OpenAPI_variable_part_t *variable_part)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (variable_part == NULL) {
        ogs_error("OpenAPI_variable_part_convertToJSON() failed [VariablePart]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (variable_part->variable_part_type == OpenAPI_variable_part_type_NULL) {
        ogs_error("OpenAPI_variable_part_convertToJSON() failed [variable_part_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "variablePartType", OpenAPI_variable_part_type_ToString(variable_part->variable_part_type)) == NULL) {
        ogs_error("OpenAPI_variable_part_convertToJSON() failed [variable_part_type]");
        goto end;
    }

    if (!variable_part->variable_part_value) {
        ogs_error("OpenAPI_variable_part_convertToJSON() failed [variable_part_value]");
        return NULL;
    }
    cJSON *variable_part_valueList = cJSON_AddArrayToObject(item, "variablePartValue");
    if (variable_part_valueList == NULL) {
        ogs_error("OpenAPI_variable_part_convertToJSON() failed [variable_part_value]");
        goto end;
    }
    OpenAPI_list_for_each(variable_part->variable_part_value, node) {
        if (cJSON_AddStringToObject(variable_part_valueList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_variable_part_convertToJSON() failed [variable_part_value]");
            goto end;
        }
    }

    if (variable_part->is_variable_part_order) {
    if (cJSON_AddNumberToObject(item, "variablePartOrder", variable_part->variable_part_order) == NULL) {
        ogs_error("OpenAPI_variable_part_convertToJSON() failed [variable_part_order]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_variable_part_t *OpenAPI_variable_part_parseFromJSON(cJSON *variable_partJSON)
{
    OpenAPI_variable_part_t *variable_part_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *variable_part_type = NULL;
    OpenAPI_variable_part_type_e variable_part_typeVariable = 0;
    cJSON *variable_part_value = NULL;
    OpenAPI_list_t *variable_part_valueList = NULL;
    cJSON *variable_part_order = NULL;
    variable_part_type = cJSON_GetObjectItemCaseSensitive(variable_partJSON, "variablePartType");
    if (!variable_part_type) {
        ogs_error("OpenAPI_variable_part_parseFromJSON() failed [variable_part_type]");
        goto end;
    }
    if (!cJSON_IsString(variable_part_type)) {
        ogs_error("OpenAPI_variable_part_parseFromJSON() failed [variable_part_type]");
        goto end;
    }
    variable_part_typeVariable = OpenAPI_variable_part_type_FromString(variable_part_type->valuestring);

    variable_part_value = cJSON_GetObjectItemCaseSensitive(variable_partJSON, "variablePartValue");
    if (!variable_part_value) {
        ogs_error("OpenAPI_variable_part_parseFromJSON() failed [variable_part_value]");
        goto end;
    }
        cJSON *variable_part_value_local = NULL;
        if (!cJSON_IsArray(variable_part_value)) {
            ogs_error("OpenAPI_variable_part_parseFromJSON() failed [variable_part_value]");
            goto end;
        }

        variable_part_valueList = OpenAPI_list_create();

        cJSON_ArrayForEach(variable_part_value_local, variable_part_value) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(variable_part_value_local)) {
                ogs_error("OpenAPI_variable_part_parseFromJSON() failed [variable_part_value]");
                goto end;
            }
            OpenAPI_list_add(variable_part_valueList, ogs_strdup(variable_part_value_local->valuestring));
        }

    variable_part_order = cJSON_GetObjectItemCaseSensitive(variable_partJSON, "variablePartOrder");
    if (variable_part_order) {
    if (!cJSON_IsNumber(variable_part_order)) {
        ogs_error("OpenAPI_variable_part_parseFromJSON() failed [variable_part_order]");
        goto end;
    }
    }

    variable_part_local_var = OpenAPI_variable_part_create (
        variable_part_typeVariable,
        variable_part_valueList,
        variable_part_order ? true : false,
        variable_part_order ? variable_part_order->valuedouble : 0
    );

    return variable_part_local_var;
end:
    if (variable_part_valueList) {
        OpenAPI_list_for_each(variable_part_valueList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(variable_part_valueList);
        variable_part_valueList = NULL;
    }
    return NULL;
}

OpenAPI_variable_part_t *OpenAPI_variable_part_copy(OpenAPI_variable_part_t *dst, OpenAPI_variable_part_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_variable_part_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_variable_part_convertToJSON() failed");
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

    OpenAPI_variable_part_free(dst);
    dst = OpenAPI_variable_part_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

