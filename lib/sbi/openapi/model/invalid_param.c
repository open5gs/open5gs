
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "invalid_param.h"

OpenAPI_invalid_param_t *OpenAPI_invalid_param_create(
    char *param,
    char *reason
    )
{
    OpenAPI_invalid_param_t *invalid_param_local_var = OpenAPI_malloc(sizeof(OpenAPI_invalid_param_t));
    if (!invalid_param_local_var) {
        return NULL;
    }
    invalid_param_local_var->param = param;
    invalid_param_local_var->reason = reason;

    return invalid_param_local_var;
}

void OpenAPI_invalid_param_free(OpenAPI_invalid_param_t *invalid_param)
{
    if (NULL == invalid_param) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(invalid_param->param);
    ogs_free(invalid_param->reason);
    ogs_free(invalid_param);
}

cJSON *OpenAPI_invalid_param_convertToJSON(OpenAPI_invalid_param_t *invalid_param)
{
    cJSON *item = NULL;

    if (invalid_param == NULL) {
        ogs_error("OpenAPI_invalid_param_convertToJSON() failed [InvalidParam]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!invalid_param->param) {
        ogs_error("OpenAPI_invalid_param_convertToJSON() failed [param]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "param", invalid_param->param) == NULL) {
        ogs_error("OpenAPI_invalid_param_convertToJSON() failed [param]");
        goto end;
    }

    if (invalid_param->reason) {
        if (cJSON_AddStringToObject(item, "reason", invalid_param->reason) == NULL) {
            ogs_error("OpenAPI_invalid_param_convertToJSON() failed [reason]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_invalid_param_t *OpenAPI_invalid_param_parseFromJSON(cJSON *invalid_paramJSON)
{
    OpenAPI_invalid_param_t *invalid_param_local_var = NULL;
    cJSON *param = cJSON_GetObjectItemCaseSensitive(invalid_paramJSON, "param");
    if (!param) {
        ogs_error("OpenAPI_invalid_param_parseFromJSON() failed [param]");
        goto end;
    }


    if (!cJSON_IsString(param)) {
        ogs_error("OpenAPI_invalid_param_parseFromJSON() failed [param]");
        goto end;
    }

    cJSON *reason = cJSON_GetObjectItemCaseSensitive(invalid_paramJSON, "reason");

    if (reason) {
        if (!cJSON_IsString(reason)) {
            ogs_error("OpenAPI_invalid_param_parseFromJSON() failed [reason]");
            goto end;
        }
    }

    invalid_param_local_var = OpenAPI_invalid_param_create (
        ogs_strdup(param->valuestring),
        reason ? ogs_strdup(reason->valuestring) : NULL
        );

    return invalid_param_local_var;
end:
    return NULL;
}

OpenAPI_invalid_param_t *OpenAPI_invalid_param_copy(OpenAPI_invalid_param_t *dst, OpenAPI_invalid_param_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_invalid_param_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_invalid_param_convertToJSON() failed");
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

    OpenAPI_invalid_param_free(dst);
    dst = OpenAPI_invalid_param_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

