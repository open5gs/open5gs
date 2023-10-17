
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "as_time_distribution_param.h"

OpenAPI_as_time_distribution_param_t *OpenAPI_as_time_distribution_param_create(
    bool is_as_time_dist_ind,
    int as_time_dist_ind,
    bool is_uu_error_budget_null,
    bool is_uu_error_budget,
    int uu_error_budget
)
{
    OpenAPI_as_time_distribution_param_t *as_time_distribution_param_local_var = ogs_malloc(sizeof(OpenAPI_as_time_distribution_param_t));
    ogs_assert(as_time_distribution_param_local_var);

    as_time_distribution_param_local_var->is_as_time_dist_ind = is_as_time_dist_ind;
    as_time_distribution_param_local_var->as_time_dist_ind = as_time_dist_ind;
    as_time_distribution_param_local_var->is_uu_error_budget_null = is_uu_error_budget_null;
    as_time_distribution_param_local_var->is_uu_error_budget = is_uu_error_budget;
    as_time_distribution_param_local_var->uu_error_budget = uu_error_budget;

    return as_time_distribution_param_local_var;
}

void OpenAPI_as_time_distribution_param_free(OpenAPI_as_time_distribution_param_t *as_time_distribution_param)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == as_time_distribution_param) {
        return;
    }
    ogs_free(as_time_distribution_param);
}

cJSON *OpenAPI_as_time_distribution_param_convertToJSON(OpenAPI_as_time_distribution_param_t *as_time_distribution_param)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (as_time_distribution_param == NULL) {
        ogs_error("OpenAPI_as_time_distribution_param_convertToJSON() failed [AsTimeDistributionParam]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (as_time_distribution_param->is_as_time_dist_ind) {
    if (cJSON_AddBoolToObject(item, "asTimeDistInd", as_time_distribution_param->as_time_dist_ind) == NULL) {
        ogs_error("OpenAPI_as_time_distribution_param_convertToJSON() failed [as_time_dist_ind]");
        goto end;
    }
    }

    if (as_time_distribution_param->is_uu_error_budget) {
    if (cJSON_AddNumberToObject(item, "uuErrorBudget", as_time_distribution_param->uu_error_budget) == NULL) {
        ogs_error("OpenAPI_as_time_distribution_param_convertToJSON() failed [uu_error_budget]");
        goto end;
    }
    } else if (as_time_distribution_param->is_uu_error_budget_null) {
        if (cJSON_AddNullToObject(item, "uuErrorBudget") == NULL) {
            ogs_error("OpenAPI_as_time_distribution_param_convertToJSON() failed [uu_error_budget]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_as_time_distribution_param_t *OpenAPI_as_time_distribution_param_parseFromJSON(cJSON *as_time_distribution_paramJSON)
{
    OpenAPI_as_time_distribution_param_t *as_time_distribution_param_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *as_time_dist_ind = NULL;
    cJSON *uu_error_budget = NULL;
    as_time_dist_ind = cJSON_GetObjectItemCaseSensitive(as_time_distribution_paramJSON, "asTimeDistInd");
    if (as_time_dist_ind) {
    if (!cJSON_IsBool(as_time_dist_ind)) {
        ogs_error("OpenAPI_as_time_distribution_param_parseFromJSON() failed [as_time_dist_ind]");
        goto end;
    }
    }

    uu_error_budget = cJSON_GetObjectItemCaseSensitive(as_time_distribution_paramJSON, "uuErrorBudget");
    if (uu_error_budget) {
    if (!cJSON_IsNull(uu_error_budget)) {
    if (!cJSON_IsNumber(uu_error_budget)) {
        ogs_error("OpenAPI_as_time_distribution_param_parseFromJSON() failed [uu_error_budget]");
        goto end;
    }
    }
    }

    as_time_distribution_param_local_var = OpenAPI_as_time_distribution_param_create (
        as_time_dist_ind ? true : false,
        as_time_dist_ind ? as_time_dist_ind->valueint : 0,
        uu_error_budget && cJSON_IsNull(uu_error_budget) ? true : false,
        uu_error_budget ? true : false,
        uu_error_budget ? uu_error_budget->valuedouble : 0
    );

    return as_time_distribution_param_local_var;
end:
    return NULL;
}

OpenAPI_as_time_distribution_param_t *OpenAPI_as_time_distribution_param_copy(OpenAPI_as_time_distribution_param_t *dst, OpenAPI_as_time_distribution_param_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_as_time_distribution_param_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_as_time_distribution_param_convertToJSON() failed");
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

    OpenAPI_as_time_distribution_param_free(dst);
    dst = OpenAPI_as_time_distribution_param_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

