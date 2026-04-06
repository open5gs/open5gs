
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "resource_usage_requirement.h"

OpenAPI_resource_usage_requirement_t *OpenAPI_resource_usage_requirement_create(
    OpenAPI_traffic_direction_e tfc_dirc,
    OpenAPI_value_expression_e val_exp
)
{
    OpenAPI_resource_usage_requirement_t *resource_usage_requirement_local_var = ogs_malloc(sizeof(OpenAPI_resource_usage_requirement_t));
    ogs_assert(resource_usage_requirement_local_var);

    resource_usage_requirement_local_var->tfc_dirc = tfc_dirc;
    resource_usage_requirement_local_var->val_exp = val_exp;

    return resource_usage_requirement_local_var;
}

void OpenAPI_resource_usage_requirement_free(OpenAPI_resource_usage_requirement_t *resource_usage_requirement)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == resource_usage_requirement) {
        return;
    }
    ogs_free(resource_usage_requirement);
}

cJSON *OpenAPI_resource_usage_requirement_convertToJSON(OpenAPI_resource_usage_requirement_t *resource_usage_requirement)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (resource_usage_requirement == NULL) {
        ogs_error("OpenAPI_resource_usage_requirement_convertToJSON() failed [ResourceUsageRequirement]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (resource_usage_requirement->tfc_dirc != OpenAPI_traffic_direction_NULL) {
    if (cJSON_AddStringToObject(item, "tfcDirc", OpenAPI_traffic_direction_ToString(resource_usage_requirement->tfc_dirc)) == NULL) {
        ogs_error("OpenAPI_resource_usage_requirement_convertToJSON() failed [tfc_dirc]");
        goto end;
    }
    }

    if (resource_usage_requirement->val_exp != OpenAPI_value_expression_NULL) {
    if (cJSON_AddStringToObject(item, "valExp", OpenAPI_value_expression_ToString(resource_usage_requirement->val_exp)) == NULL) {
        ogs_error("OpenAPI_resource_usage_requirement_convertToJSON() failed [val_exp]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_resource_usage_requirement_t *OpenAPI_resource_usage_requirement_parseFromJSON(cJSON *resource_usage_requirementJSON)
{
    OpenAPI_resource_usage_requirement_t *resource_usage_requirement_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tfc_dirc = NULL;
    OpenAPI_traffic_direction_e tfc_dircVariable = 0;
    cJSON *val_exp = NULL;
    OpenAPI_value_expression_e val_expVariable = 0;
    tfc_dirc = cJSON_GetObjectItemCaseSensitive(resource_usage_requirementJSON, "tfcDirc");
    if (tfc_dirc) {
    if (!cJSON_IsString(tfc_dirc)) {
        ogs_error("OpenAPI_resource_usage_requirement_parseFromJSON() failed [tfc_dirc]");
        goto end;
    }
    tfc_dircVariable = OpenAPI_traffic_direction_FromString(tfc_dirc->valuestring);
    }

    val_exp = cJSON_GetObjectItemCaseSensitive(resource_usage_requirementJSON, "valExp");
    if (val_exp) {
    if (!cJSON_IsString(val_exp)) {
        ogs_error("OpenAPI_resource_usage_requirement_parseFromJSON() failed [val_exp]");
        goto end;
    }
    val_expVariable = OpenAPI_value_expression_FromString(val_exp->valuestring);
    }

    resource_usage_requirement_local_var = OpenAPI_resource_usage_requirement_create (
        tfc_dirc ? tfc_dircVariable : 0,
        val_exp ? val_expVariable : 0
    );

    return resource_usage_requirement_local_var;
end:
    return NULL;
}

OpenAPI_resource_usage_requirement_t *OpenAPI_resource_usage_requirement_copy(OpenAPI_resource_usage_requirement_t *dst, OpenAPI_resource_usage_requirement_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_resource_usage_requirement_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_resource_usage_requirement_convertToJSON() failed");
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

    OpenAPI_resource_usage_requirement_free(dst);
    dst = OpenAPI_resource_usage_requirement_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

