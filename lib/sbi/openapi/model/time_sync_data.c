
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "time_sync_data.h"

OpenAPI_time_sync_data_t *OpenAPI_time_sync_data_create(
    int authorized,
    bool is_uu_time_sync_err_bdgt,
    int uu_time_sync_err_bdgt,
    OpenAPI_list_t *temp_vals,
    OpenAPI_list_t *coverage_area,
    OpenAPI_clock_quality_detail_level_e clock_quality_detail_level,
    OpenAPI_clock_quality_acceptance_criterion_t *clock_quality_acceptance_criterion
)
{
    OpenAPI_time_sync_data_t *time_sync_data_local_var = ogs_malloc(sizeof(OpenAPI_time_sync_data_t));
    ogs_assert(time_sync_data_local_var);

    time_sync_data_local_var->authorized = authorized;
    time_sync_data_local_var->is_uu_time_sync_err_bdgt = is_uu_time_sync_err_bdgt;
    time_sync_data_local_var->uu_time_sync_err_bdgt = uu_time_sync_err_bdgt;
    time_sync_data_local_var->temp_vals = temp_vals;
    time_sync_data_local_var->coverage_area = coverage_area;
    time_sync_data_local_var->clock_quality_detail_level = clock_quality_detail_level;
    time_sync_data_local_var->clock_quality_acceptance_criterion = clock_quality_acceptance_criterion;

    return time_sync_data_local_var;
}

void OpenAPI_time_sync_data_free(OpenAPI_time_sync_data_t *time_sync_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == time_sync_data) {
        return;
    }
    if (time_sync_data->temp_vals) {
        OpenAPI_list_for_each(time_sync_data->temp_vals, node) {
            OpenAPI_temporal_validity_free(node->data);
        }
        OpenAPI_list_free(time_sync_data->temp_vals);
        time_sync_data->temp_vals = NULL;
    }
    if (time_sync_data->coverage_area) {
        OpenAPI_list_for_each(time_sync_data->coverage_area, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(time_sync_data->coverage_area);
        time_sync_data->coverage_area = NULL;
    }
    if (time_sync_data->clock_quality_acceptance_criterion) {
        OpenAPI_clock_quality_acceptance_criterion_free(time_sync_data->clock_quality_acceptance_criterion);
        time_sync_data->clock_quality_acceptance_criterion = NULL;
    }
    ogs_free(time_sync_data);
}

cJSON *OpenAPI_time_sync_data_convertToJSON(OpenAPI_time_sync_data_t *time_sync_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (time_sync_data == NULL) {
        ogs_error("OpenAPI_time_sync_data_convertToJSON() failed [TimeSyncData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddBoolToObject(item, "authorized", time_sync_data->authorized) == NULL) {
        ogs_error("OpenAPI_time_sync_data_convertToJSON() failed [authorized]");
        goto end;
    }

    if (time_sync_data->is_uu_time_sync_err_bdgt) {
    if (cJSON_AddNumberToObject(item, "uuTimeSyncErrBdgt", time_sync_data->uu_time_sync_err_bdgt) == NULL) {
        ogs_error("OpenAPI_time_sync_data_convertToJSON() failed [uu_time_sync_err_bdgt]");
        goto end;
    }
    }

    if (time_sync_data->temp_vals) {
    cJSON *temp_valsList = cJSON_AddArrayToObject(item, "tempVals");
    if (temp_valsList == NULL) {
        ogs_error("OpenAPI_time_sync_data_convertToJSON() failed [temp_vals]");
        goto end;
    }
    OpenAPI_list_for_each(time_sync_data->temp_vals, node) {
        cJSON *itemLocal = OpenAPI_temporal_validity_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_time_sync_data_convertToJSON() failed [temp_vals]");
            goto end;
        }
        cJSON_AddItemToArray(temp_valsList, itemLocal);
    }
    }

    if (time_sync_data->coverage_area) {
    cJSON *coverage_areaList = cJSON_AddArrayToObject(item, "coverageArea");
    if (coverage_areaList == NULL) {
        ogs_error("OpenAPI_time_sync_data_convertToJSON() failed [coverage_area]");
        goto end;
    }
    OpenAPI_list_for_each(time_sync_data->coverage_area, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_time_sync_data_convertToJSON() failed [coverage_area]");
            goto end;
        }
        cJSON_AddItemToArray(coverage_areaList, itemLocal);
    }
    }

    if (time_sync_data->clock_quality_detail_level != OpenAPI_clock_quality_detail_level_NULL) {
    if (cJSON_AddStringToObject(item, "clockQualityDetailLevel", OpenAPI_clock_quality_detail_level_ToString(time_sync_data->clock_quality_detail_level)) == NULL) {
        ogs_error("OpenAPI_time_sync_data_convertToJSON() failed [clock_quality_detail_level]");
        goto end;
    }
    }

    if (time_sync_data->clock_quality_acceptance_criterion) {
    cJSON *clock_quality_acceptance_criterion_local_JSON = OpenAPI_clock_quality_acceptance_criterion_convertToJSON(time_sync_data->clock_quality_acceptance_criterion);
    if (clock_quality_acceptance_criterion_local_JSON == NULL) {
        ogs_error("OpenAPI_time_sync_data_convertToJSON() failed [clock_quality_acceptance_criterion]");
        goto end;
    }
    cJSON_AddItemToObject(item, "clockQualityAcceptanceCriterion", clock_quality_acceptance_criterion_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_time_sync_data_convertToJSON() failed [clock_quality_acceptance_criterion]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_time_sync_data_t *OpenAPI_time_sync_data_parseFromJSON(cJSON *time_sync_dataJSON)
{
    OpenAPI_time_sync_data_t *time_sync_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *authorized = NULL;
    cJSON *uu_time_sync_err_bdgt = NULL;
    cJSON *temp_vals = NULL;
    OpenAPI_list_t *temp_valsList = NULL;
    cJSON *coverage_area = NULL;
    OpenAPI_list_t *coverage_areaList = NULL;
    cJSON *clock_quality_detail_level = NULL;
    OpenAPI_clock_quality_detail_level_e clock_quality_detail_levelVariable = 0;
    cJSON *clock_quality_acceptance_criterion = NULL;
    OpenAPI_clock_quality_acceptance_criterion_t *clock_quality_acceptance_criterion_local_nonprim = NULL;
    authorized = cJSON_GetObjectItemCaseSensitive(time_sync_dataJSON, "authorized");
    if (!authorized) {
        ogs_error("OpenAPI_time_sync_data_parseFromJSON() failed [authorized]");
        goto end;
    }
    if (!cJSON_IsBool(authorized)) {
        ogs_error("OpenAPI_time_sync_data_parseFromJSON() failed [authorized]");
        goto end;
    }

    uu_time_sync_err_bdgt = cJSON_GetObjectItemCaseSensitive(time_sync_dataJSON, "uuTimeSyncErrBdgt");
    if (uu_time_sync_err_bdgt) {
    if (!cJSON_IsNumber(uu_time_sync_err_bdgt)) {
        ogs_error("OpenAPI_time_sync_data_parseFromJSON() failed [uu_time_sync_err_bdgt]");
        goto end;
    }
    }

    temp_vals = cJSON_GetObjectItemCaseSensitive(time_sync_dataJSON, "tempVals");
    if (temp_vals) {
        cJSON *temp_vals_local = NULL;
        if (!cJSON_IsArray(temp_vals)) {
            ogs_error("OpenAPI_time_sync_data_parseFromJSON() failed [temp_vals]");
            goto end;
        }

        temp_valsList = OpenAPI_list_create();

        cJSON_ArrayForEach(temp_vals_local, temp_vals) {
            if (!cJSON_IsObject(temp_vals_local)) {
                ogs_error("OpenAPI_time_sync_data_parseFromJSON() failed [temp_vals]");
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

    coverage_area = cJSON_GetObjectItemCaseSensitive(time_sync_dataJSON, "coverageArea");
    if (coverage_area) {
        cJSON *coverage_area_local = NULL;
        if (!cJSON_IsArray(coverage_area)) {
            ogs_error("OpenAPI_time_sync_data_parseFromJSON() failed [coverage_area]");
            goto end;
        }

        coverage_areaList = OpenAPI_list_create();

        cJSON_ArrayForEach(coverage_area_local, coverage_area) {
            if (!cJSON_IsObject(coverage_area_local)) {
                ogs_error("OpenAPI_time_sync_data_parseFromJSON() failed [coverage_area]");
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

    clock_quality_detail_level = cJSON_GetObjectItemCaseSensitive(time_sync_dataJSON, "clockQualityDetailLevel");
    if (clock_quality_detail_level) {
    if (!cJSON_IsString(clock_quality_detail_level)) {
        ogs_error("OpenAPI_time_sync_data_parseFromJSON() failed [clock_quality_detail_level]");
        goto end;
    }
    clock_quality_detail_levelVariable = OpenAPI_clock_quality_detail_level_FromString(clock_quality_detail_level->valuestring);
    }

    clock_quality_acceptance_criterion = cJSON_GetObjectItemCaseSensitive(time_sync_dataJSON, "clockQualityAcceptanceCriterion");
    if (clock_quality_acceptance_criterion) {
    clock_quality_acceptance_criterion_local_nonprim = OpenAPI_clock_quality_acceptance_criterion_parseFromJSON(clock_quality_acceptance_criterion);
    if (!clock_quality_acceptance_criterion_local_nonprim) {
        ogs_error("OpenAPI_clock_quality_acceptance_criterion_parseFromJSON failed [clock_quality_acceptance_criterion]");
        goto end;
    }
    }

    time_sync_data_local_var = OpenAPI_time_sync_data_create (
        
        authorized->valueint,
        uu_time_sync_err_bdgt ? true : false,
        uu_time_sync_err_bdgt ? uu_time_sync_err_bdgt->valuedouble : 0,
        temp_vals ? temp_valsList : NULL,
        coverage_area ? coverage_areaList : NULL,
        clock_quality_detail_level ? clock_quality_detail_levelVariable : 0,
        clock_quality_acceptance_criterion ? clock_quality_acceptance_criterion_local_nonprim : NULL
    );

    return time_sync_data_local_var;
end:
    if (temp_valsList) {
        OpenAPI_list_for_each(temp_valsList, node) {
            OpenAPI_temporal_validity_free(node->data);
        }
        OpenAPI_list_free(temp_valsList);
        temp_valsList = NULL;
    }
    if (coverage_areaList) {
        OpenAPI_list_for_each(coverage_areaList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(coverage_areaList);
        coverage_areaList = NULL;
    }
    if (clock_quality_acceptance_criterion_local_nonprim) {
        OpenAPI_clock_quality_acceptance_criterion_free(clock_quality_acceptance_criterion_local_nonprim);
        clock_quality_acceptance_criterion_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_time_sync_data_t *OpenAPI_time_sync_data_copy(OpenAPI_time_sync_data_t *dst, OpenAPI_time_sync_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_time_sync_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_time_sync_data_convertToJSON() failed");
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

    OpenAPI_time_sync_data_free(dst);
    dst = OpenAPI_time_sync_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

