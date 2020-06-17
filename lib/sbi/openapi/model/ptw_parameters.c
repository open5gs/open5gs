
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ptw_parameters.h"

OpenAPI_ptw_parameters_t *OpenAPI_ptw_parameters_create(
    OpenAPI_operation_mode_e operation_mode,
    char *ptw_value
    )
{
    OpenAPI_ptw_parameters_t *ptw_parameters_local_var = OpenAPI_malloc(sizeof(OpenAPI_ptw_parameters_t));
    if (!ptw_parameters_local_var) {
        return NULL;
    }
    ptw_parameters_local_var->operation_mode = operation_mode;
    ptw_parameters_local_var->ptw_value = ptw_value;

    return ptw_parameters_local_var;
}

void OpenAPI_ptw_parameters_free(OpenAPI_ptw_parameters_t *ptw_parameters)
{
    if (NULL == ptw_parameters) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ptw_parameters->ptw_value);
    ogs_free(ptw_parameters);
}

cJSON *OpenAPI_ptw_parameters_convertToJSON(OpenAPI_ptw_parameters_t *ptw_parameters)
{
    cJSON *item = NULL;

    if (ptw_parameters == NULL) {
        ogs_error("OpenAPI_ptw_parameters_convertToJSON() failed [PtwParameters]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ptw_parameters->operation_mode) {
        ogs_error("OpenAPI_ptw_parameters_convertToJSON() failed [operation_mode]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "operationMode", OpenAPI_operation_mode_ToString(ptw_parameters->operation_mode)) == NULL) {
        ogs_error("OpenAPI_ptw_parameters_convertToJSON() failed [operation_mode]");
        goto end;
    }

    if (!ptw_parameters->ptw_value) {
        ogs_error("OpenAPI_ptw_parameters_convertToJSON() failed [ptw_value]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "ptwValue", ptw_parameters->ptw_value) == NULL) {
        ogs_error("OpenAPI_ptw_parameters_convertToJSON() failed [ptw_value]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ptw_parameters_t *OpenAPI_ptw_parameters_parseFromJSON(cJSON *ptw_parametersJSON)
{
    OpenAPI_ptw_parameters_t *ptw_parameters_local_var = NULL;
    cJSON *operation_mode = cJSON_GetObjectItemCaseSensitive(ptw_parametersJSON, "operationMode");
    if (!operation_mode) {
        ogs_error("OpenAPI_ptw_parameters_parseFromJSON() failed [operation_mode]");
        goto end;
    }

    OpenAPI_operation_mode_e operation_modeVariable;

    if (!cJSON_IsString(operation_mode)) {
        ogs_error("OpenAPI_ptw_parameters_parseFromJSON() failed [operation_mode]");
        goto end;
    }
    operation_modeVariable = OpenAPI_operation_mode_FromString(operation_mode->valuestring);

    cJSON *ptw_value = cJSON_GetObjectItemCaseSensitive(ptw_parametersJSON, "ptwValue");
    if (!ptw_value) {
        ogs_error("OpenAPI_ptw_parameters_parseFromJSON() failed [ptw_value]");
        goto end;
    }


    if (!cJSON_IsString(ptw_value)) {
        ogs_error("OpenAPI_ptw_parameters_parseFromJSON() failed [ptw_value]");
        goto end;
    }

    ptw_parameters_local_var = OpenAPI_ptw_parameters_create (
        operation_modeVariable,
        ogs_strdup(ptw_value->valuestring)
        );

    return ptw_parameters_local_var;
end:
    return NULL;
}

OpenAPI_ptw_parameters_t *OpenAPI_ptw_parameters_copy(OpenAPI_ptw_parameters_t *dst, OpenAPI_ptw_parameters_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ptw_parameters_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ptw_parameters_convertToJSON() failed");
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

    OpenAPI_ptw_parameters_free(dst);
    dst = OpenAPI_ptw_parameters_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

