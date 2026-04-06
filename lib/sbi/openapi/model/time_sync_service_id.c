
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "time_sync_service_id.h"

OpenAPI_time_sync_service_id_t *OpenAPI_time_sync_service_id_create(
    char *dnn,
    OpenAPI_snssai_t *s_nssai,
    char *reference,
    OpenAPI_list_t *temp_vals,
    OpenAPI_list_t *coverage_area,
    bool is_uu_time_sync_err_bdgt,
    int uu_time_sync_err_bdgt
)
{
    OpenAPI_time_sync_service_id_t *time_sync_service_id_local_var = ogs_malloc(sizeof(OpenAPI_time_sync_service_id_t));
    ogs_assert(time_sync_service_id_local_var);

    time_sync_service_id_local_var->dnn = dnn;
    time_sync_service_id_local_var->s_nssai = s_nssai;
    time_sync_service_id_local_var->reference = reference;
    time_sync_service_id_local_var->temp_vals = temp_vals;
    time_sync_service_id_local_var->coverage_area = coverage_area;
    time_sync_service_id_local_var->is_uu_time_sync_err_bdgt = is_uu_time_sync_err_bdgt;
    time_sync_service_id_local_var->uu_time_sync_err_bdgt = uu_time_sync_err_bdgt;

    return time_sync_service_id_local_var;
}

void OpenAPI_time_sync_service_id_free(OpenAPI_time_sync_service_id_t *time_sync_service_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == time_sync_service_id) {
        return;
    }
    if (time_sync_service_id->dnn) {
        ogs_free(time_sync_service_id->dnn);
        time_sync_service_id->dnn = NULL;
    }
    if (time_sync_service_id->s_nssai) {
        OpenAPI_snssai_free(time_sync_service_id->s_nssai);
        time_sync_service_id->s_nssai = NULL;
    }
    if (time_sync_service_id->reference) {
        ogs_free(time_sync_service_id->reference);
        time_sync_service_id->reference = NULL;
    }
    if (time_sync_service_id->temp_vals) {
        OpenAPI_list_for_each(time_sync_service_id->temp_vals, node) {
            OpenAPI_temporal_validity_free(node->data);
        }
        OpenAPI_list_free(time_sync_service_id->temp_vals);
        time_sync_service_id->temp_vals = NULL;
    }
    if (time_sync_service_id->coverage_area) {
        OpenAPI_list_for_each(time_sync_service_id->coverage_area, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(time_sync_service_id->coverage_area);
        time_sync_service_id->coverage_area = NULL;
    }
    ogs_free(time_sync_service_id);
}

cJSON *OpenAPI_time_sync_service_id_convertToJSON(OpenAPI_time_sync_service_id_t *time_sync_service_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (time_sync_service_id == NULL) {
        ogs_error("OpenAPI_time_sync_service_id_convertToJSON() failed [TimeSyncServiceId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (time_sync_service_id->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", time_sync_service_id->dnn) == NULL) {
        ogs_error("OpenAPI_time_sync_service_id_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (time_sync_service_id->s_nssai) {
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(time_sync_service_id->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_time_sync_service_id_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_time_sync_service_id_convertToJSON() failed [s_nssai]");
        goto end;
    }
    }

    if (!time_sync_service_id->reference) {
        ogs_error("OpenAPI_time_sync_service_id_convertToJSON() failed [reference]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "reference", time_sync_service_id->reference) == NULL) {
        ogs_error("OpenAPI_time_sync_service_id_convertToJSON() failed [reference]");
        goto end;
    }

    if (time_sync_service_id->temp_vals) {
    cJSON *temp_valsList = cJSON_AddArrayToObject(item, "tempVals");
    if (temp_valsList == NULL) {
        ogs_error("OpenAPI_time_sync_service_id_convertToJSON() failed [temp_vals]");
        goto end;
    }
    OpenAPI_list_for_each(time_sync_service_id->temp_vals, node) {
        cJSON *itemLocal = OpenAPI_temporal_validity_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_time_sync_service_id_convertToJSON() failed [temp_vals]");
            goto end;
        }
        cJSON_AddItemToArray(temp_valsList, itemLocal);
    }
    }

    if (time_sync_service_id->coverage_area) {
    cJSON *coverage_areaList = cJSON_AddArrayToObject(item, "coverageArea");
    if (coverage_areaList == NULL) {
        ogs_error("OpenAPI_time_sync_service_id_convertToJSON() failed [coverage_area]");
        goto end;
    }
    OpenAPI_list_for_each(time_sync_service_id->coverage_area, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_time_sync_service_id_convertToJSON() failed [coverage_area]");
            goto end;
        }
        cJSON_AddItemToArray(coverage_areaList, itemLocal);
    }
    }

    if (time_sync_service_id->is_uu_time_sync_err_bdgt) {
    if (cJSON_AddNumberToObject(item, "uuTimeSyncErrBdgt", time_sync_service_id->uu_time_sync_err_bdgt) == NULL) {
        ogs_error("OpenAPI_time_sync_service_id_convertToJSON() failed [uu_time_sync_err_bdgt]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_time_sync_service_id_t *OpenAPI_time_sync_service_id_parseFromJSON(cJSON *time_sync_service_idJSON)
{
    OpenAPI_time_sync_service_id_t *time_sync_service_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *s_nssai = NULL;
    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    cJSON *reference = NULL;
    cJSON *temp_vals = NULL;
    OpenAPI_list_t *temp_valsList = NULL;
    cJSON *coverage_area = NULL;
    OpenAPI_list_t *coverage_areaList = NULL;
    cJSON *uu_time_sync_err_bdgt = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(time_sync_service_idJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_time_sync_service_id_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    s_nssai = cJSON_GetObjectItemCaseSensitive(time_sync_service_idJSON, "sNssai");
    if (s_nssai) {
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    if (!s_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [s_nssai]");
        goto end;
    }
    }

    reference = cJSON_GetObjectItemCaseSensitive(time_sync_service_idJSON, "reference");
    if (!reference) {
        ogs_error("OpenAPI_time_sync_service_id_parseFromJSON() failed [reference]");
        goto end;
    }
    if (!cJSON_IsString(reference)) {
        ogs_error("OpenAPI_time_sync_service_id_parseFromJSON() failed [reference]");
        goto end;
    }

    temp_vals = cJSON_GetObjectItemCaseSensitive(time_sync_service_idJSON, "tempVals");
    if (temp_vals) {
        cJSON *temp_vals_local = NULL;
        if (!cJSON_IsArray(temp_vals)) {
            ogs_error("OpenAPI_time_sync_service_id_parseFromJSON() failed [temp_vals]");
            goto end;
        }

        temp_valsList = OpenAPI_list_create();

        cJSON_ArrayForEach(temp_vals_local, temp_vals) {
            if (!cJSON_IsObject(temp_vals_local)) {
                ogs_error("OpenAPI_time_sync_service_id_parseFromJSON() failed [temp_vals]");
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

    coverage_area = cJSON_GetObjectItemCaseSensitive(time_sync_service_idJSON, "coverageArea");
    if (coverage_area) {
        cJSON *coverage_area_local = NULL;
        if (!cJSON_IsArray(coverage_area)) {
            ogs_error("OpenAPI_time_sync_service_id_parseFromJSON() failed [coverage_area]");
            goto end;
        }

        coverage_areaList = OpenAPI_list_create();

        cJSON_ArrayForEach(coverage_area_local, coverage_area) {
            if (!cJSON_IsObject(coverage_area_local)) {
                ogs_error("OpenAPI_time_sync_service_id_parseFromJSON() failed [coverage_area]");
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

    uu_time_sync_err_bdgt = cJSON_GetObjectItemCaseSensitive(time_sync_service_idJSON, "uuTimeSyncErrBdgt");
    if (uu_time_sync_err_bdgt) {
    if (!cJSON_IsNumber(uu_time_sync_err_bdgt)) {
        ogs_error("OpenAPI_time_sync_service_id_parseFromJSON() failed [uu_time_sync_err_bdgt]");
        goto end;
    }
    }

    time_sync_service_id_local_var = OpenAPI_time_sync_service_id_create (
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        s_nssai ? s_nssai_local_nonprim : NULL,
        ogs_strdup(reference->valuestring),
        temp_vals ? temp_valsList : NULL,
        coverage_area ? coverage_areaList : NULL,
        uu_time_sync_err_bdgt ? true : false,
        uu_time_sync_err_bdgt ? uu_time_sync_err_bdgt->valuedouble : 0
    );

    return time_sync_service_id_local_var;
end:
    if (s_nssai_local_nonprim) {
        OpenAPI_snssai_free(s_nssai_local_nonprim);
        s_nssai_local_nonprim = NULL;
    }
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
    return NULL;
}

OpenAPI_time_sync_service_id_t *OpenAPI_time_sync_service_id_copy(OpenAPI_time_sync_service_id_t *dst, OpenAPI_time_sync_service_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_time_sync_service_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_time_sync_service_id_convertToJSON() failed");
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

    OpenAPI_time_sync_service_id_free(dst);
    dst = OpenAPI_time_sync_service_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

