
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "alternative_service_requirements_data.h"

OpenAPI_alternative_service_requirements_data_t *OpenAPI_alternative_service_requirements_data_create(
    char *alt_qos_param_set_ref,
    char *gbr_ul,
    char *gbr_dl,
    bool is_pdb,
    int pdb,
    char *per,
    bool is_aver_window,
    int aver_window,
    bool is_max_data_burst_vol_null,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    bool is_pdu_set_qos_dl_null,
    OpenAPI_pdu_set_qos_para_rm_t *pdu_set_qos_dl,
    bool is_pdu_set_qos_ul_null,
    OpenAPI_pdu_set_qos_para_rm_t *pdu_set_qos_ul
)
{
    OpenAPI_alternative_service_requirements_data_t *alternative_service_requirements_data_local_var = ogs_malloc(sizeof(OpenAPI_alternative_service_requirements_data_t));
    ogs_assert(alternative_service_requirements_data_local_var);

    alternative_service_requirements_data_local_var->alt_qos_param_set_ref = alt_qos_param_set_ref;
    alternative_service_requirements_data_local_var->gbr_ul = gbr_ul;
    alternative_service_requirements_data_local_var->gbr_dl = gbr_dl;
    alternative_service_requirements_data_local_var->is_pdb = is_pdb;
    alternative_service_requirements_data_local_var->pdb = pdb;
    alternative_service_requirements_data_local_var->per = per;
    alternative_service_requirements_data_local_var->is_aver_window = is_aver_window;
    alternative_service_requirements_data_local_var->aver_window = aver_window;
    alternative_service_requirements_data_local_var->is_max_data_burst_vol_null = is_max_data_burst_vol_null;
    alternative_service_requirements_data_local_var->is_max_data_burst_vol = is_max_data_burst_vol;
    alternative_service_requirements_data_local_var->max_data_burst_vol = max_data_burst_vol;
    alternative_service_requirements_data_local_var->is_pdu_set_qos_dl_null = is_pdu_set_qos_dl_null;
    alternative_service_requirements_data_local_var->pdu_set_qos_dl = pdu_set_qos_dl;
    alternative_service_requirements_data_local_var->is_pdu_set_qos_ul_null = is_pdu_set_qos_ul_null;
    alternative_service_requirements_data_local_var->pdu_set_qos_ul = pdu_set_qos_ul;

    return alternative_service_requirements_data_local_var;
}

void OpenAPI_alternative_service_requirements_data_free(OpenAPI_alternative_service_requirements_data_t *alternative_service_requirements_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == alternative_service_requirements_data) {
        return;
    }
    if (alternative_service_requirements_data->alt_qos_param_set_ref) {
        ogs_free(alternative_service_requirements_data->alt_qos_param_set_ref);
        alternative_service_requirements_data->alt_qos_param_set_ref = NULL;
    }
    if (alternative_service_requirements_data->gbr_ul) {
        ogs_free(alternative_service_requirements_data->gbr_ul);
        alternative_service_requirements_data->gbr_ul = NULL;
    }
    if (alternative_service_requirements_data->gbr_dl) {
        ogs_free(alternative_service_requirements_data->gbr_dl);
        alternative_service_requirements_data->gbr_dl = NULL;
    }
    if (alternative_service_requirements_data->per) {
        ogs_free(alternative_service_requirements_data->per);
        alternative_service_requirements_data->per = NULL;
    }
    if (alternative_service_requirements_data->pdu_set_qos_dl) {
        OpenAPI_pdu_set_qos_para_rm_free(alternative_service_requirements_data->pdu_set_qos_dl);
        alternative_service_requirements_data->pdu_set_qos_dl = NULL;
    }
    if (alternative_service_requirements_data->pdu_set_qos_ul) {
        OpenAPI_pdu_set_qos_para_rm_free(alternative_service_requirements_data->pdu_set_qos_ul);
        alternative_service_requirements_data->pdu_set_qos_ul = NULL;
    }
    ogs_free(alternative_service_requirements_data);
}

cJSON *OpenAPI_alternative_service_requirements_data_convertToJSON(OpenAPI_alternative_service_requirements_data_t *alternative_service_requirements_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (alternative_service_requirements_data == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [AlternativeServiceRequirementsData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!alternative_service_requirements_data->alt_qos_param_set_ref) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [alt_qos_param_set_ref]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "altQosParamSetRef", alternative_service_requirements_data->alt_qos_param_set_ref) == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [alt_qos_param_set_ref]");
        goto end;
    }

    if (alternative_service_requirements_data->gbr_ul) {
    if (cJSON_AddStringToObject(item, "gbrUl", alternative_service_requirements_data->gbr_ul) == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [gbr_ul]");
        goto end;
    }
    }

    if (alternative_service_requirements_data->gbr_dl) {
    if (cJSON_AddStringToObject(item, "gbrDl", alternative_service_requirements_data->gbr_dl) == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [gbr_dl]");
        goto end;
    }
    }

    if (alternative_service_requirements_data->is_pdb) {
    if (cJSON_AddNumberToObject(item, "pdb", alternative_service_requirements_data->pdb) == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [pdb]");
        goto end;
    }
    }

    if (alternative_service_requirements_data->per) {
    if (cJSON_AddStringToObject(item, "per", alternative_service_requirements_data->per) == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [per]");
        goto end;
    }
    }

    if (alternative_service_requirements_data->is_aver_window) {
    if (cJSON_AddNumberToObject(item, "averWindow", alternative_service_requirements_data->aver_window) == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [aver_window]");
        goto end;
    }
    }

    if (alternative_service_requirements_data->is_max_data_burst_vol) {
    if (cJSON_AddNumberToObject(item, "maxDataBurstVol", alternative_service_requirements_data->max_data_burst_vol) == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [max_data_burst_vol]");
        goto end;
    }
    } else if (alternative_service_requirements_data->is_max_data_burst_vol_null) {
        if (cJSON_AddNullToObject(item, "maxDataBurstVol") == NULL) {
            ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [max_data_burst_vol]");
            goto end;
        }
    }

    if (alternative_service_requirements_data->pdu_set_qos_dl) {
    cJSON *pdu_set_qos_dl_local_JSON = OpenAPI_pdu_set_qos_para_rm_convertToJSON(alternative_service_requirements_data->pdu_set_qos_dl);
    if (pdu_set_qos_dl_local_JSON == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [pdu_set_qos_dl]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pduSetQosDl", pdu_set_qos_dl_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [pdu_set_qos_dl]");
        goto end;
    }
    } else if (alternative_service_requirements_data->is_pdu_set_qos_dl_null) {
        if (cJSON_AddNullToObject(item, "pduSetQosDl") == NULL) {
            ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [pdu_set_qos_dl]");
            goto end;
        }
    }

    if (alternative_service_requirements_data->pdu_set_qos_ul) {
    cJSON *pdu_set_qos_ul_local_JSON = OpenAPI_pdu_set_qos_para_rm_convertToJSON(alternative_service_requirements_data->pdu_set_qos_ul);
    if (pdu_set_qos_ul_local_JSON == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [pdu_set_qos_ul]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pduSetQosUl", pdu_set_qos_ul_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [pdu_set_qos_ul]");
        goto end;
    }
    } else if (alternative_service_requirements_data->is_pdu_set_qos_ul_null) {
        if (cJSON_AddNullToObject(item, "pduSetQosUl") == NULL) {
            ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed [pdu_set_qos_ul]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_alternative_service_requirements_data_t *OpenAPI_alternative_service_requirements_data_parseFromJSON(cJSON *alternative_service_requirements_dataJSON)
{
    OpenAPI_alternative_service_requirements_data_t *alternative_service_requirements_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *alt_qos_param_set_ref = NULL;
    cJSON *gbr_ul = NULL;
    cJSON *gbr_dl = NULL;
    cJSON *pdb = NULL;
    cJSON *per = NULL;
    cJSON *aver_window = NULL;
    cJSON *max_data_burst_vol = NULL;
    cJSON *pdu_set_qos_dl = NULL;
    OpenAPI_pdu_set_qos_para_rm_t *pdu_set_qos_dl_local_nonprim = NULL;
    cJSON *pdu_set_qos_ul = NULL;
    OpenAPI_pdu_set_qos_para_rm_t *pdu_set_qos_ul_local_nonprim = NULL;
    alt_qos_param_set_ref = cJSON_GetObjectItemCaseSensitive(alternative_service_requirements_dataJSON, "altQosParamSetRef");
    if (!alt_qos_param_set_ref) {
        ogs_error("OpenAPI_alternative_service_requirements_data_parseFromJSON() failed [alt_qos_param_set_ref]");
        goto end;
    }
    if (!cJSON_IsString(alt_qos_param_set_ref)) {
        ogs_error("OpenAPI_alternative_service_requirements_data_parseFromJSON() failed [alt_qos_param_set_ref]");
        goto end;
    }

    gbr_ul = cJSON_GetObjectItemCaseSensitive(alternative_service_requirements_dataJSON, "gbrUl");
    if (gbr_ul) {
    if (!cJSON_IsString(gbr_ul) && !cJSON_IsNull(gbr_ul)) {
        ogs_error("OpenAPI_alternative_service_requirements_data_parseFromJSON() failed [gbr_ul]");
        goto end;
    }
    }

    gbr_dl = cJSON_GetObjectItemCaseSensitive(alternative_service_requirements_dataJSON, "gbrDl");
    if (gbr_dl) {
    if (!cJSON_IsString(gbr_dl) && !cJSON_IsNull(gbr_dl)) {
        ogs_error("OpenAPI_alternative_service_requirements_data_parseFromJSON() failed [gbr_dl]");
        goto end;
    }
    }

    pdb = cJSON_GetObjectItemCaseSensitive(alternative_service_requirements_dataJSON, "pdb");
    if (pdb) {
    if (!cJSON_IsNumber(pdb)) {
        ogs_error("OpenAPI_alternative_service_requirements_data_parseFromJSON() failed [pdb]");
        goto end;
    }
    }

    per = cJSON_GetObjectItemCaseSensitive(alternative_service_requirements_dataJSON, "per");
    if (per) {
    if (!cJSON_IsString(per) && !cJSON_IsNull(per)) {
        ogs_error("OpenAPI_alternative_service_requirements_data_parseFromJSON() failed [per]");
        goto end;
    }
    }

    aver_window = cJSON_GetObjectItemCaseSensitive(alternative_service_requirements_dataJSON, "averWindow");
    if (aver_window) {
    if (!cJSON_IsNumber(aver_window)) {
        ogs_error("OpenAPI_alternative_service_requirements_data_parseFromJSON() failed [aver_window]");
        goto end;
    }
    }

    max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(alternative_service_requirements_dataJSON, "maxDataBurstVol");
    if (max_data_burst_vol) {
    if (!cJSON_IsNull(max_data_burst_vol)) {
    if (!cJSON_IsNumber(max_data_burst_vol)) {
        ogs_error("OpenAPI_alternative_service_requirements_data_parseFromJSON() failed [max_data_burst_vol]");
        goto end;
    }
    }
    }

    pdu_set_qos_dl = cJSON_GetObjectItemCaseSensitive(alternative_service_requirements_dataJSON, "pduSetQosDl");
    if (pdu_set_qos_dl) {
    if (!cJSON_IsNull(pdu_set_qos_dl)) {
    pdu_set_qos_dl_local_nonprim = OpenAPI_pdu_set_qos_para_rm_parseFromJSON(pdu_set_qos_dl);
    if (!pdu_set_qos_dl_local_nonprim) {
        ogs_error("OpenAPI_pdu_set_qos_para_rm_parseFromJSON failed [pdu_set_qos_dl]");
        goto end;
    }
    }
    }

    pdu_set_qos_ul = cJSON_GetObjectItemCaseSensitive(alternative_service_requirements_dataJSON, "pduSetQosUl");
    if (pdu_set_qos_ul) {
    if (!cJSON_IsNull(pdu_set_qos_ul)) {
    pdu_set_qos_ul_local_nonprim = OpenAPI_pdu_set_qos_para_rm_parseFromJSON(pdu_set_qos_ul);
    if (!pdu_set_qos_ul_local_nonprim) {
        ogs_error("OpenAPI_pdu_set_qos_para_rm_parseFromJSON failed [pdu_set_qos_ul]");
        goto end;
    }
    }
    }

    alternative_service_requirements_data_local_var = OpenAPI_alternative_service_requirements_data_create (
        ogs_strdup(alt_qos_param_set_ref->valuestring),
        gbr_ul && !cJSON_IsNull(gbr_ul) ? ogs_strdup(gbr_ul->valuestring) : NULL,
        gbr_dl && !cJSON_IsNull(gbr_dl) ? ogs_strdup(gbr_dl->valuestring) : NULL,
        pdb ? true : false,
        pdb ? pdb->valuedouble : 0,
        per && !cJSON_IsNull(per) ? ogs_strdup(per->valuestring) : NULL,
        aver_window ? true : false,
        aver_window ? aver_window->valuedouble : 0,
        max_data_burst_vol && cJSON_IsNull(max_data_burst_vol) ? true : false,
        max_data_burst_vol ? true : false,
        max_data_burst_vol ? max_data_burst_vol->valuedouble : 0,
        pdu_set_qos_dl && cJSON_IsNull(pdu_set_qos_dl) ? true : false,
        pdu_set_qos_dl ? pdu_set_qos_dl_local_nonprim : NULL,
        pdu_set_qos_ul && cJSON_IsNull(pdu_set_qos_ul) ? true : false,
        pdu_set_qos_ul ? pdu_set_qos_ul_local_nonprim : NULL
    );

    return alternative_service_requirements_data_local_var;
end:
    if (pdu_set_qos_dl_local_nonprim) {
        OpenAPI_pdu_set_qos_para_rm_free(pdu_set_qos_dl_local_nonprim);
        pdu_set_qos_dl_local_nonprim = NULL;
    }
    if (pdu_set_qos_ul_local_nonprim) {
        OpenAPI_pdu_set_qos_para_rm_free(pdu_set_qos_ul_local_nonprim);
        pdu_set_qos_ul_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_alternative_service_requirements_data_t *OpenAPI_alternative_service_requirements_data_copy(OpenAPI_alternative_service_requirements_data_t *dst, OpenAPI_alternative_service_requirements_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_alternative_service_requirements_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_alternative_service_requirements_data_convertToJSON() failed");
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

    OpenAPI_alternative_service_requirements_data_free(dst);
    dst = OpenAPI_alternative_service_requirements_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

