
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "operator_specific_data_container_value.h"

OpenAPI_operator_specific_data_container_value_t *OpenAPI_operator_specific_data_container_value_create(
)
{
    OpenAPI_operator_specific_data_container_value_t *operator_specific_data_container_value_local_var = ogs_malloc(sizeof(OpenAPI_operator_specific_data_container_value_t));
    ogs_assert(operator_specific_data_container_value_local_var);


    return operator_specific_data_container_value_local_var;
}

void OpenAPI_operator_specific_data_container_value_free(OpenAPI_operator_specific_data_container_value_t *operator_specific_data_container_value)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == operator_specific_data_container_value) {
        return;
    }
    ogs_free(operator_specific_data_container_value);
}

cJSON *OpenAPI_operator_specific_data_container_value_convertToJSON(OpenAPI_operator_specific_data_container_value_t *operator_specific_data_container_value)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (operator_specific_data_container_value == NULL) {
        ogs_error("OpenAPI_operator_specific_data_container_value_convertToJSON() failed [OperatorSpecificDataContainer_value]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_operator_specific_data_container_value_t *OpenAPI_operator_specific_data_container_value_parseFromJSON(cJSON *operator_specific_data_container_valueJSON)
{
    OpenAPI_operator_specific_data_container_value_t *operator_specific_data_container_value_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    operator_specific_data_container_value_local_var = OpenAPI_operator_specific_data_container_value_create (
    );

    return operator_specific_data_container_value_local_var;
end:
    return NULL;
}

OpenAPI_operator_specific_data_container_value_t *OpenAPI_operator_specific_data_container_value_copy(OpenAPI_operator_specific_data_container_value_t *dst, OpenAPI_operator_specific_data_container_value_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_operator_specific_data_container_value_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_operator_specific_data_container_value_convertToJSON() failed");
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

    OpenAPI_operator_specific_data_container_value_free(dst);
    dst = OpenAPI_operator_specific_data_container_value_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

