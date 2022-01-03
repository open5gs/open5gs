
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "invalid_param_1.h"

OpenAPI_invalid_param_1_t *OpenAPI_invalid_param_1_create(
    char *param,
    char *reason
)
{
    OpenAPI_invalid_param_1_t *invalid_param_1_local_var = ogs_malloc(sizeof(OpenAPI_invalid_param_1_t));
    ogs_assert(invalid_param_1_local_var);

    invalid_param_1_local_var->param = param;
    invalid_param_1_local_var->reason = reason;

    return invalid_param_1_local_var;
}

void OpenAPI_invalid_param_1_free(OpenAPI_invalid_param_1_t *invalid_param_1)
{
    if (NULL == invalid_param_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(invalid_param_1->param);
    ogs_free(invalid_param_1->reason);
    ogs_free(invalid_param_1);
}

cJSON *OpenAPI_invalid_param_1_convertToJSON(OpenAPI_invalid_param_1_t *invalid_param_1)
{
    cJSON *item = NULL;

    if (invalid_param_1 == NULL) {
        ogs_error("OpenAPI_invalid_param_1_convertToJSON() failed [InvalidParam_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "param", invalid_param_1->param) == NULL) {
        ogs_error("OpenAPI_invalid_param_1_convertToJSON() failed [param]");
        goto end;
    }

    if (invalid_param_1->reason) {
    if (cJSON_AddStringToObject(item, "reason", invalid_param_1->reason) == NULL) {
        ogs_error("OpenAPI_invalid_param_1_convertToJSON() failed [reason]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_invalid_param_1_t *OpenAPI_invalid_param_1_parseFromJSON(cJSON *invalid_param_1JSON)
{
    OpenAPI_invalid_param_1_t *invalid_param_1_local_var = NULL;
    cJSON *param = cJSON_GetObjectItemCaseSensitive(invalid_param_1JSON, "param");
    if (!param) {
        ogs_error("OpenAPI_invalid_param_1_parseFromJSON() failed [param]");
        goto end;
    }

    if (!cJSON_IsString(param)) {
        ogs_error("OpenAPI_invalid_param_1_parseFromJSON() failed [param]");
        goto end;
    }

    cJSON *reason = cJSON_GetObjectItemCaseSensitive(invalid_param_1JSON, "reason");

    if (reason) {
    if (!cJSON_IsString(reason)) {
        ogs_error("OpenAPI_invalid_param_1_parseFromJSON() failed [reason]");
        goto end;
    }
    }

    invalid_param_1_local_var = OpenAPI_invalid_param_1_create (
        ogs_strdup(param->valuestring),
        reason ? ogs_strdup(reason->valuestring) : NULL
    );

    return invalid_param_1_local_var;
end:
    return NULL;
}

OpenAPI_invalid_param_1_t *OpenAPI_invalid_param_1_copy(OpenAPI_invalid_param_1_t *dst, OpenAPI_invalid_param_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_invalid_param_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_invalid_param_1_convertToJSON() failed");
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

    OpenAPI_invalid_param_1_free(dst);
    dst = OpenAPI_invalid_param_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

