
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "asti_allowed_info.h"

OpenAPI_asti_allowed_info_t *OpenAPI_asti_allowed_info_create(
    int asti_allowed,
    OpenAPI_list_t *coverage_area,
    bool is_uu_time_sync_err_bdgt,
    int uu_time_sync_err_bdgt,
    OpenAPI_list_t *temp_vals,
    bool is_clock_quality_metrics_ind,
    int clock_quality_metrics_ind,
    bool is_acceptable_ind,
    int acceptable_ind,
    OpenAPI_clock_quality_acceptance_criterion_t *clock_quality_acceptance_criteria
)
{
    OpenAPI_asti_allowed_info_t *asti_allowed_info_local_var = ogs_malloc(sizeof(OpenAPI_asti_allowed_info_t));
    ogs_assert(asti_allowed_info_local_var);

    asti_allowed_info_local_var->asti_allowed = asti_allowed;
    asti_allowed_info_local_var->coverage_area = coverage_area;
    asti_allowed_info_local_var->is_uu_time_sync_err_bdgt = is_uu_time_sync_err_bdgt;
    asti_allowed_info_local_var->uu_time_sync_err_bdgt = uu_time_sync_err_bdgt;
    asti_allowed_info_local_var->temp_vals = temp_vals;
    asti_allowed_info_local_var->is_clock_quality_metrics_ind = is_clock_quality_metrics_ind;
    asti_allowed_info_local_var->clock_quality_metrics_ind = clock_quality_metrics_ind;
    asti_allowed_info_local_var->is_acceptable_ind = is_acceptable_ind;
    asti_allowed_info_local_var->acceptable_ind = acceptable_ind;
    asti_allowed_info_local_var->clock_quality_acceptance_criteria = clock_quality_acceptance_criteria;

    return asti_allowed_info_local_var;
}

void OpenAPI_asti_allowed_info_free(OpenAPI_asti_allowed_info_t *asti_allowed_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == asti_allowed_info) {
        return;
    }
    if (asti_allowed_info->coverage_area) {
        OpenAPI_list_for_each(asti_allowed_info->coverage_area, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(asti_allowed_info->coverage_area);
        asti_allowed_info->coverage_area = NULL;
    }
    if (asti_allowed_info->temp_vals) {
        OpenAPI_list_for_each(asti_allowed_info->temp_vals, node) {
            OpenAPI_temporal_validity_free(node->data);
        }
        OpenAPI_list_free(asti_allowed_info->temp_vals);
        asti_allowed_info->temp_vals = NULL;
    }
    if (asti_allowed_info->clock_quality_acceptance_criteria) {
        OpenAPI_clock_quality_acceptance_criterion_free(asti_allowed_info->clock_quality_acceptance_criteria);
        asti_allowed_info->clock_quality_acceptance_criteria = NULL;
    }
    ogs_free(asti_allowed_info);
}

cJSON *OpenAPI_asti_allowed_info_convertToJSON(OpenAPI_asti_allowed_info_t *asti_allowed_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (asti_allowed_info == NULL) {
        ogs_error("OpenAPI_asti_allowed_info_convertToJSON() failed [AstiAllowedInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddBoolToObject(item, "astiAllowed", asti_allowed_info->asti_allowed) == NULL) {
        ogs_error("OpenAPI_asti_allowed_info_convertToJSON() failed [asti_allowed]");
        goto end;
    }

    if (asti_allowed_info->coverage_area) {
    cJSON *coverage_areaList = cJSON_AddArrayToObject(item, "coverageArea");
    if (coverage_areaList == NULL) {
        ogs_error("OpenAPI_asti_allowed_info_convertToJSON() failed [coverage_area]");
        goto end;
    }
    OpenAPI_list_for_each(asti_allowed_info->coverage_area, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_asti_allowed_info_convertToJSON() failed [coverage_area]");
            goto end;
        }
        cJSON_AddItemToArray(coverage_areaList, itemLocal);
    }
    }

    if (asti_allowed_info->is_uu_time_sync_err_bdgt) {
    if (cJSON_AddNumberToObject(item, "uuTimeSyncErrBdgt", asti_allowed_info->uu_time_sync_err_bdgt) == NULL) {
        ogs_error("OpenAPI_asti_allowed_info_convertToJSON() failed [uu_time_sync_err_bdgt]");
        goto end;
    }
    }

    if (asti_allowed_info->temp_vals) {
    cJSON *temp_valsList = cJSON_AddArrayToObject(item, "tempVals");
    if (temp_valsList == NULL) {
        ogs_error("OpenAPI_asti_allowed_info_convertToJSON() failed [temp_vals]");
        goto end;
    }
    OpenAPI_list_for_each(asti_allowed_info->temp_vals, node) {
        cJSON *itemLocal = OpenAPI_temporal_validity_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_asti_allowed_info_convertToJSON() failed [temp_vals]");
            goto end;
        }
        cJSON_AddItemToArray(temp_valsList, itemLocal);
    }
    }

    if (asti_allowed_info->is_clock_quality_metrics_ind) {
    if (cJSON_AddBoolToObject(item, "clockQualityMetricsInd", asti_allowed_info->clock_quality_metrics_ind) == NULL) {
        ogs_error("OpenAPI_asti_allowed_info_convertToJSON() failed [clock_quality_metrics_ind]");
        goto end;
    }
    }

    if (asti_allowed_info->is_acceptable_ind) {
    if (cJSON_AddBoolToObject(item, "acceptableInd", asti_allowed_info->acceptable_ind) == NULL) {
        ogs_error("OpenAPI_asti_allowed_info_convertToJSON() failed [acceptable_ind]");
        goto end;
    }
    }

    if (asti_allowed_info->clock_quality_acceptance_criteria) {
    cJSON *clock_quality_acceptance_criteria_local_JSON = OpenAPI_clock_quality_acceptance_criterion_convertToJSON(asti_allowed_info->clock_quality_acceptance_criteria);
    if (clock_quality_acceptance_criteria_local_JSON == NULL) {
        ogs_error("OpenAPI_asti_allowed_info_convertToJSON() failed [clock_quality_acceptance_criteria]");
        goto end;
    }
    cJSON_AddItemToObject(item, "clockQualityAcceptanceCriteria", clock_quality_acceptance_criteria_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_asti_allowed_info_convertToJSON() failed [clock_quality_acceptance_criteria]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_asti_allowed_info_t *OpenAPI_asti_allowed_info_parseFromJSON(cJSON *asti_allowed_infoJSON)
{
    OpenAPI_asti_allowed_info_t *asti_allowed_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *asti_allowed = NULL;
    cJSON *coverage_area = NULL;
    OpenAPI_list_t *coverage_areaList = NULL;
    cJSON *uu_time_sync_err_bdgt = NULL;
    cJSON *temp_vals = NULL;
    OpenAPI_list_t *temp_valsList = NULL;
    cJSON *clock_quality_metrics_ind = NULL;
    cJSON *acceptable_ind = NULL;
    cJSON *clock_quality_acceptance_criteria = NULL;
    OpenAPI_clock_quality_acceptance_criterion_t *clock_quality_acceptance_criteria_local_nonprim = NULL;
    asti_allowed = cJSON_GetObjectItemCaseSensitive(asti_allowed_infoJSON, "astiAllowed");
    if (!asti_allowed) {
        ogs_error("OpenAPI_asti_allowed_info_parseFromJSON() failed [asti_allowed]");
        goto end;
    }
    if (!cJSON_IsBool(asti_allowed)) {
        ogs_error("OpenAPI_asti_allowed_info_parseFromJSON() failed [asti_allowed]");
        goto end;
    }

    coverage_area = cJSON_GetObjectItemCaseSensitive(asti_allowed_infoJSON, "coverageArea");
    if (coverage_area) {
        cJSON *coverage_area_local = NULL;
        if (!cJSON_IsArray(coverage_area)) {
            ogs_error("OpenAPI_asti_allowed_info_parseFromJSON() failed [coverage_area]");
            goto end;
        }

        coverage_areaList = OpenAPI_list_create();

        cJSON_ArrayForEach(coverage_area_local, coverage_area) {
            if (!cJSON_IsObject(coverage_area_local)) {
                ogs_error("OpenAPI_asti_allowed_info_parseFromJSON() failed [coverage_area]");
                goto end;
            }
            OpenAPI_tai_t *coverage_areaItem = OpenAPI_tai_parseFromJSON(coverage_area_local);
            if (!coverage_areaItem) {
                ogs_error("No coverage_areaItem");
                goto end;
            }
            OpenAPI_list_add(coverage_areaList, coverage_areaItem);
        }
    }

    uu_time_sync_err_bdgt = cJSON_GetObjectItemCaseSensitive(asti_allowed_infoJSON, "uuTimeSyncErrBdgt");
    if (uu_time_sync_err_bdgt) {
    if (!cJSON_IsNumber(uu_time_sync_err_bdgt)) {
        ogs_error("OpenAPI_asti_allowed_info_parseFromJSON() failed [uu_time_sync_err_bdgt]");
        goto end;
    }
    }

    temp_vals = cJSON_GetObjectItemCaseSensitive(asti_allowed_infoJSON, "tempVals");
    if (temp_vals) {
        cJSON *temp_vals_local = NULL;
        if (!cJSON_IsArray(temp_vals)) {
            ogs_error("OpenAPI_asti_allowed_info_parseFromJSON() failed [temp_vals]");
            goto end;
        }

        temp_valsList = OpenAPI_list_create();

        cJSON_ArrayForEach(temp_vals_local, temp_vals) {
            if (!cJSON_IsObject(temp_vals_local)) {
                ogs_error("OpenAPI_asti_allowed_info_parseFromJSON() failed [temp_vals]");
                goto end;
            }
            OpenAPI_temporal_validity_t *temp_valsItem = OpenAPI_temporal_validity_parseFromJSON(temp_vals_local);
            if (!temp_valsItem) {
                ogs_error("No temp_valsItem");
                goto end;
            }
            OpenAPI_list_add(temp_valsList, temp_valsItem);
        }
    }

    clock_quality_metrics_ind = cJSON_GetObjectItemCaseSensitive(asti_allowed_infoJSON, "clockQualityMetricsInd");
    if (clock_quality_metrics_ind) {
    if (!cJSON_IsBool(clock_quality_metrics_ind)) {
        ogs_error("OpenAPI_asti_allowed_info_parseFromJSON() failed [clock_quality_metrics_ind]");
        goto end;
    }
    }

    acceptable_ind = cJSON_GetObjectItemCaseSensitive(asti_allowed_infoJSON, "acceptableInd");
    if (acceptable_ind) {
    if (!cJSON_IsBool(acceptable_ind)) {
        ogs_error("OpenAPI_asti_allowed_info_parseFromJSON() failed [acceptable_ind]");
        goto end;
    }
    }

    clock_quality_acceptance_criteria = cJSON_GetObjectItemCaseSensitive(asti_allowed_infoJSON, "clockQualityAcceptanceCriteria");
    if (clock_quality_acceptance_criteria) {
    clock_quality_acceptance_criteria_local_nonprim = OpenAPI_clock_quality_acceptance_criterion_parseFromJSON(clock_quality_acceptance_criteria);
    if (!clock_quality_acceptance_criteria_local_nonprim) {
        ogs_error("OpenAPI_clock_quality_acceptance_criterion_parseFromJSON failed [clock_quality_acceptance_criteria]");
        goto end;
    }
    }

    asti_allowed_info_local_var = OpenAPI_asti_allowed_info_create (
        
        asti_allowed->valueint,
        coverage_area ? coverage_areaList : NULL,
        uu_time_sync_err_bdgt ? true : false,
        uu_time_sync_err_bdgt ? uu_time_sync_err_bdgt->valuedouble : 0,
        temp_vals ? temp_valsList : NULL,
        clock_quality_metrics_ind ? true : false,
        clock_quality_metrics_ind ? clock_quality_metrics_ind->valueint : 0,
        acceptable_ind ? true : false,
        acceptable_ind ? acceptable_ind->valueint : 0,
        clock_quality_acceptance_criteria ? clock_quality_acceptance_criteria_local_nonprim : NULL
    );

    return asti_allowed_info_local_var;
end:
    if (coverage_areaList) {
        OpenAPI_list_for_each(coverage_areaList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(coverage_areaList);
        coverage_areaList = NULL;
    }
    if (temp_valsList) {
        OpenAPI_list_for_each(temp_valsList, node) {
            OpenAPI_temporal_validity_free(node->data);
        }
        OpenAPI_list_free(temp_valsList);
        temp_valsList = NULL;
    }
    if (clock_quality_acceptance_criteria_local_nonprim) {
        OpenAPI_clock_quality_acceptance_criterion_free(clock_quality_acceptance_criteria_local_nonprim);
        clock_quality_acceptance_criteria_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_asti_allowed_info_t *OpenAPI_asti_allowed_info_copy(OpenAPI_asti_allowed_info_t *dst, OpenAPI_asti_allowed_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_asti_allowed_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_asti_allowed_info_convertToJSON() failed");
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

    OpenAPI_asti_allowed_info_free(dst);
    dst = OpenAPI_asti_allowed_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

