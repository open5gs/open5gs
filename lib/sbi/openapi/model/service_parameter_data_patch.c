
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_parameter_data_patch.h"

OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_create(
    char *param_over_pc5,
    char *param_over_uu
)
{
    OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch_local_var = ogs_malloc(sizeof(OpenAPI_service_parameter_data_patch_t));
    ogs_assert(service_parameter_data_patch_local_var);

    service_parameter_data_patch_local_var->param_over_pc5 = param_over_pc5;
    service_parameter_data_patch_local_var->param_over_uu = param_over_uu;

    return service_parameter_data_patch_local_var;
}

void OpenAPI_service_parameter_data_patch_free(OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch)
{
    if (NULL == service_parameter_data_patch) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(service_parameter_data_patch->param_over_pc5);
    ogs_free(service_parameter_data_patch->param_over_uu);
    ogs_free(service_parameter_data_patch);
}

cJSON *OpenAPI_service_parameter_data_patch_convertToJSON(OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch)
{
    cJSON *item = NULL;

    if (service_parameter_data_patch == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [ServiceParameterDataPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (service_parameter_data_patch->param_over_pc5) {
    if (cJSON_AddStringToObject(item, "paramOverPc5", service_parameter_data_patch->param_over_pc5) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_over_pc5]");
        goto end;
    }
    }

    if (service_parameter_data_patch->param_over_uu) {
    if (cJSON_AddStringToObject(item, "paramOverUu", service_parameter_data_patch->param_over_uu) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed [param_over_uu]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_parseFromJSON(cJSON *service_parameter_data_patchJSON)
{
    OpenAPI_service_parameter_data_patch_t *service_parameter_data_patch_local_var = NULL;
    cJSON *param_over_pc5 = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramOverPc5");

    if (param_over_pc5) {
    if (!cJSON_IsString(param_over_pc5)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_over_pc5]");
        goto end;
    }
    }

    cJSON *param_over_uu = cJSON_GetObjectItemCaseSensitive(service_parameter_data_patchJSON, "paramOverUu");

    if (param_over_uu) {
    if (!cJSON_IsString(param_over_uu)) {
        ogs_error("OpenAPI_service_parameter_data_patch_parseFromJSON() failed [param_over_uu]");
        goto end;
    }
    }

    service_parameter_data_patch_local_var = OpenAPI_service_parameter_data_patch_create (
        param_over_pc5 ? ogs_strdup(param_over_pc5->valuestring) : NULL,
        param_over_uu ? ogs_strdup(param_over_uu->valuestring) : NULL
    );

    return service_parameter_data_patch_local_var;
end:
    return NULL;
}

OpenAPI_service_parameter_data_patch_t *OpenAPI_service_parameter_data_patch_copy(OpenAPI_service_parameter_data_patch_t *dst, OpenAPI_service_parameter_data_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_parameter_data_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_parameter_data_patch_convertToJSON() failed");
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

    OpenAPI_service_parameter_data_patch_free(dst);
    dst = OpenAPI_service_parameter_data_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

