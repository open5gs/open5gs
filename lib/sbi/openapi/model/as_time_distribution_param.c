
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "as_time_distribution_param.h"

OpenAPI_as_time_distribution_param_t *OpenAPI_as_time_distribution_param_create(
    bool is_as_time_dist_ind,
    int as_time_dist_ind,
    bool is_uu_error_budget_null,
    bool is_uu_error_budget,
    int uu_error_budget,
    OpenAPI_clock_quality_detail_level_e clk_qlt_det_lvl,
    bool is_clk_qlt_acpt_cri_null,
    OpenAPI_clock_quality_acceptance_criterion_rm_t *clk_qlt_acpt_cri
)
{
    OpenAPI_as_time_distribution_param_t *as_time_distribution_param_local_var = ogs_malloc(sizeof(OpenAPI_as_time_distribution_param_t));
    ogs_assert(as_time_distribution_param_local_var);

    as_time_distribution_param_local_var->is_as_time_dist_ind = is_as_time_dist_ind;
    as_time_distribution_param_local_var->as_time_dist_ind = as_time_dist_ind;
    as_time_distribution_param_local_var->is_uu_error_budget_null = is_uu_error_budget_null;
    as_time_distribution_param_local_var->is_uu_error_budget = is_uu_error_budget;
    as_time_distribution_param_local_var->uu_error_budget = uu_error_budget;
    as_time_distribution_param_local_var->clk_qlt_det_lvl = clk_qlt_det_lvl;
    as_time_distribution_param_local_var->is_clk_qlt_acpt_cri_null = is_clk_qlt_acpt_cri_null;
    as_time_distribution_param_local_var->clk_qlt_acpt_cri = clk_qlt_acpt_cri;

    return as_time_distribution_param_local_var;
}

void OpenAPI_as_time_distribution_param_free(OpenAPI_as_time_distribution_param_t *as_time_distribution_param)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == as_time_distribution_param) {
        return;
    }
    if (as_time_distribution_param->clk_qlt_acpt_cri) {
        OpenAPI_clock_quality_acceptance_criterion_rm_free(as_time_distribution_param->clk_qlt_acpt_cri);
        as_time_distribution_param->clk_qlt_acpt_cri = NULL;
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

    if (as_time_distribution_param->clk_qlt_det_lvl != OpenAPI_clock_quality_detail_level_NULL) {
    if (cJSON_AddStringToObject(item, "clkQltDetLvl", OpenAPI_clock_quality_detail_level_ToString(as_time_distribution_param->clk_qlt_det_lvl)) == NULL) {
        ogs_error("OpenAPI_as_time_distribution_param_convertToJSON() failed [clk_qlt_det_lvl]");
        goto end;
    }
    }

    if (as_time_distribution_param->clk_qlt_acpt_cri) {
    cJSON *clk_qlt_acpt_cri_local_JSON = OpenAPI_clock_quality_acceptance_criterion_rm_convertToJSON(as_time_distribution_param->clk_qlt_acpt_cri);
    if (clk_qlt_acpt_cri_local_JSON == NULL) {
        ogs_error("OpenAPI_as_time_distribution_param_convertToJSON() failed [clk_qlt_acpt_cri]");
        goto end;
    }
    cJSON_AddItemToObject(item, "clkQltAcptCri", clk_qlt_acpt_cri_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_as_time_distribution_param_convertToJSON() failed [clk_qlt_acpt_cri]");
        goto end;
    }
    } else if (as_time_distribution_param->is_clk_qlt_acpt_cri_null) {
        if (cJSON_AddNullToObject(item, "clkQltAcptCri") == NULL) {
            ogs_error("OpenAPI_as_time_distribution_param_convertToJSON() failed [clk_qlt_acpt_cri]");
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
    cJSON *clk_qlt_det_lvl = NULL;
    OpenAPI_clock_quality_detail_level_e clk_qlt_det_lvlVariable = 0;
    cJSON *clk_qlt_acpt_cri = NULL;
    OpenAPI_clock_quality_acceptance_criterion_rm_t *clk_qlt_acpt_cri_local_nonprim = NULL;
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

    clk_qlt_det_lvl = cJSON_GetObjectItemCaseSensitive(as_time_distribution_paramJSON, "clkQltDetLvl");
    if (clk_qlt_det_lvl) {
    if (!cJSON_IsString(clk_qlt_det_lvl)) {
        ogs_error("OpenAPI_as_time_distribution_param_parseFromJSON() failed [clk_qlt_det_lvl]");
        goto end;
    }
    clk_qlt_det_lvlVariable = OpenAPI_clock_quality_detail_level_FromString(clk_qlt_det_lvl->valuestring);
    }

    clk_qlt_acpt_cri = cJSON_GetObjectItemCaseSensitive(as_time_distribution_paramJSON, "clkQltAcptCri");
    if (clk_qlt_acpt_cri) {
    if (!cJSON_IsNull(clk_qlt_acpt_cri)) {
    clk_qlt_acpt_cri_local_nonprim = OpenAPI_clock_quality_acceptance_criterion_rm_parseFromJSON(clk_qlt_acpt_cri);
    if (!clk_qlt_acpt_cri_local_nonprim) {
        ogs_error("OpenAPI_clock_quality_acceptance_criterion_rm_parseFromJSON failed [clk_qlt_acpt_cri]");
        goto end;
    }
    }
    }

    as_time_distribution_param_local_var = OpenAPI_as_time_distribution_param_create (
        as_time_dist_ind ? true : false,
        as_time_dist_ind ? as_time_dist_ind->valueint : 0,
        uu_error_budget && cJSON_IsNull(uu_error_budget) ? true : false,
        uu_error_budget ? true : false,
        uu_error_budget ? uu_error_budget->valuedouble : 0,
        clk_qlt_det_lvl ? clk_qlt_det_lvlVariable : 0,
        clk_qlt_acpt_cri && cJSON_IsNull(clk_qlt_acpt_cri) ? true : false,
        clk_qlt_acpt_cri ? clk_qlt_acpt_cri_local_nonprim : NULL
    );

    return as_time_distribution_param_local_var;
end:
    if (clk_qlt_acpt_cri_local_nonprim) {
        OpenAPI_clock_quality_acceptance_criterion_rm_free(clk_qlt_acpt_cri_local_nonprim);
        clk_qlt_acpt_cri_local_nonprim = NULL;
    }
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

