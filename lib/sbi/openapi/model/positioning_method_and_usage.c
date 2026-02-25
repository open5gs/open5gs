
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "positioning_method_and_usage.h"

OpenAPI_positioning_method_and_usage_t *OpenAPI_positioning_method_and_usage_create(
    OpenAPI_positioning_method_t *method,
    OpenAPI_positioning_mode_t *mode,
    OpenAPI_usage_t *usage,
    bool is_method_code,
    int method_code
)
{
    OpenAPI_positioning_method_and_usage_t *positioning_method_and_usage_local_var = ogs_malloc(sizeof(OpenAPI_positioning_method_and_usage_t));
    ogs_assert(positioning_method_and_usage_local_var);

    positioning_method_and_usage_local_var->method = method;
    positioning_method_and_usage_local_var->mode = mode;
    positioning_method_and_usage_local_var->usage = usage;
    positioning_method_and_usage_local_var->is_method_code = is_method_code;
    positioning_method_and_usage_local_var->method_code = method_code;

    return positioning_method_and_usage_local_var;
}

void OpenAPI_positioning_method_and_usage_free(OpenAPI_positioning_method_and_usage_t *positioning_method_and_usage)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == positioning_method_and_usage) {
        return;
    }
    if (positioning_method_and_usage->method) {
        OpenAPI_positioning_method_free(positioning_method_and_usage->method);
        positioning_method_and_usage->method = NULL;
    }
    if (positioning_method_and_usage->mode) {
        OpenAPI_positioning_mode_free(positioning_method_and_usage->mode);
        positioning_method_and_usage->mode = NULL;
    }
    if (positioning_method_and_usage->usage) {
        OpenAPI_usage_free(positioning_method_and_usage->usage);
        positioning_method_and_usage->usage = NULL;
    }
    ogs_free(positioning_method_and_usage);
}

cJSON *OpenAPI_positioning_method_and_usage_convertToJSON(OpenAPI_positioning_method_and_usage_t *positioning_method_and_usage)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (positioning_method_and_usage == NULL) {
        ogs_error("OpenAPI_positioning_method_and_usage_convertToJSON() failed [PositioningMethodAndUsage]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!positioning_method_and_usage->method) {
        ogs_error("OpenAPI_positioning_method_and_usage_convertToJSON() failed [method]");
        return NULL;
    }
    cJSON *method_local_JSON = OpenAPI_positioning_method_convertToJSON(positioning_method_and_usage->method);
    if (method_local_JSON == NULL) {
        ogs_error("OpenAPI_positioning_method_and_usage_convertToJSON() failed [method]");
        goto end;
    }
    cJSON_AddItemToObject(item, "method", method_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_positioning_method_and_usage_convertToJSON() failed [method]");
        goto end;
    }

    if (!positioning_method_and_usage->mode) {
        ogs_error("OpenAPI_positioning_method_and_usage_convertToJSON() failed [mode]");
        return NULL;
    }
    cJSON *mode_local_JSON = OpenAPI_positioning_mode_convertToJSON(positioning_method_and_usage->mode);
    if (mode_local_JSON == NULL) {
        ogs_error("OpenAPI_positioning_method_and_usage_convertToJSON() failed [mode]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mode", mode_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_positioning_method_and_usage_convertToJSON() failed [mode]");
        goto end;
    }

    if (!positioning_method_and_usage->usage) {
        ogs_error("OpenAPI_positioning_method_and_usage_convertToJSON() failed [usage]");
        return NULL;
    }
    cJSON *usage_local_JSON = OpenAPI_usage_convertToJSON(positioning_method_and_usage->usage);
    if (usage_local_JSON == NULL) {
        ogs_error("OpenAPI_positioning_method_and_usage_convertToJSON() failed [usage]");
        goto end;
    }
    cJSON_AddItemToObject(item, "usage", usage_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_positioning_method_and_usage_convertToJSON() failed [usage]");
        goto end;
    }

    if (positioning_method_and_usage->is_method_code) {
    if (cJSON_AddNumberToObject(item, "methodCode", positioning_method_and_usage->method_code) == NULL) {
        ogs_error("OpenAPI_positioning_method_and_usage_convertToJSON() failed [method_code]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_positioning_method_and_usage_t *OpenAPI_positioning_method_and_usage_parseFromJSON(cJSON *positioning_method_and_usageJSON)
{
    OpenAPI_positioning_method_and_usage_t *positioning_method_and_usage_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *method = NULL;
    OpenAPI_positioning_method_t *method_local_nonprim = NULL;
    cJSON *mode = NULL;
    OpenAPI_positioning_mode_t *mode_local_nonprim = NULL;
    cJSON *usage = NULL;
    OpenAPI_usage_t *usage_local_nonprim = NULL;
    cJSON *method_code = NULL;
    method = cJSON_GetObjectItemCaseSensitive(positioning_method_and_usageJSON, "method");
    if (!method) {
        ogs_error("OpenAPI_positioning_method_and_usage_parseFromJSON() failed [method]");
        goto end;
    }
    method_local_nonprim = OpenAPI_positioning_method_parseFromJSON(method);
    if (!method_local_nonprim) {
        ogs_error("OpenAPI_positioning_method_parseFromJSON failed [method]");
        goto end;
    }

    mode = cJSON_GetObjectItemCaseSensitive(positioning_method_and_usageJSON, "mode");
    if (!mode) {
        ogs_error("OpenAPI_positioning_method_and_usage_parseFromJSON() failed [mode]");
        goto end;
    }
    mode_local_nonprim = OpenAPI_positioning_mode_parseFromJSON(mode);
    if (!mode_local_nonprim) {
        ogs_error("OpenAPI_positioning_mode_parseFromJSON failed [mode]");
        goto end;
    }

    usage = cJSON_GetObjectItemCaseSensitive(positioning_method_and_usageJSON, "usage");
    if (!usage) {
        ogs_error("OpenAPI_positioning_method_and_usage_parseFromJSON() failed [usage]");
        goto end;
    }
    usage_local_nonprim = OpenAPI_usage_parseFromJSON(usage);
    if (!usage_local_nonprim) {
        ogs_error("OpenAPI_usage_parseFromJSON failed [usage]");
        goto end;
    }

    method_code = cJSON_GetObjectItemCaseSensitive(positioning_method_and_usageJSON, "methodCode");
    if (method_code) {
    if (!cJSON_IsNumber(method_code)) {
        ogs_error("OpenAPI_positioning_method_and_usage_parseFromJSON() failed [method_code]");
        goto end;
    }
    }

    positioning_method_and_usage_local_var = OpenAPI_positioning_method_and_usage_create (
        method_local_nonprim,
        mode_local_nonprim,
        usage_local_nonprim,
        method_code ? true : false,
        method_code ? method_code->valuedouble : 0
    );

    return positioning_method_and_usage_local_var;
end:
    if (method_local_nonprim) {
        OpenAPI_positioning_method_free(method_local_nonprim);
        method_local_nonprim = NULL;
    }
    if (mode_local_nonprim) {
        OpenAPI_positioning_mode_free(mode_local_nonprim);
        mode_local_nonprim = NULL;
    }
    if (usage_local_nonprim) {
        OpenAPI_usage_free(usage_local_nonprim);
        usage_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_positioning_method_and_usage_t *OpenAPI_positioning_method_and_usage_copy(OpenAPI_positioning_method_and_usage_t *dst, OpenAPI_positioning_method_and_usage_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_positioning_method_and_usage_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_positioning_method_and_usage_convertToJSON() failed");
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

    OpenAPI_positioning_method_and_usage_free(dst);
    dst = OpenAPI_positioning_method_and_usage_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

