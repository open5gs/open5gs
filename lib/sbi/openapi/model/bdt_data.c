
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bdt_data.h"

OpenAPI_bdt_data_t *OpenAPI_bdt_data_create(
    char *asp_id,
    OpenAPI_transfer_policy_t *trans_policy,
    char *bdt_ref_id,
    OpenAPI_network_area_info_2_t *nw_area_info,
    int num_of_ues,
    OpenAPI_usage_threshold_t *vol_per_ue,
    char *dnn,
    OpenAPI_snssai_t *snssai
    )
{
    OpenAPI_bdt_data_t *bdt_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_bdt_data_t));
    if (!bdt_data_local_var) {
        return NULL;
    }
    bdt_data_local_var->asp_id = asp_id;
    bdt_data_local_var->trans_policy = trans_policy;
    bdt_data_local_var->bdt_ref_id = bdt_ref_id;
    bdt_data_local_var->nw_area_info = nw_area_info;
    bdt_data_local_var->num_of_ues = num_of_ues;
    bdt_data_local_var->vol_per_ue = vol_per_ue;
    bdt_data_local_var->dnn = dnn;
    bdt_data_local_var->snssai = snssai;

    return bdt_data_local_var;
}

void OpenAPI_bdt_data_free(OpenAPI_bdt_data_t *bdt_data)
{
    if (NULL == bdt_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(bdt_data->asp_id);
    OpenAPI_transfer_policy_free(bdt_data->trans_policy);
    ogs_free(bdt_data->bdt_ref_id);
    OpenAPI_network_area_info_2_free(bdt_data->nw_area_info);
    OpenAPI_usage_threshold_free(bdt_data->vol_per_ue);
    ogs_free(bdt_data->dnn);
    OpenAPI_snssai_free(bdt_data->snssai);
    ogs_free(bdt_data);
}

cJSON *OpenAPI_bdt_data_convertToJSON(OpenAPI_bdt_data_t *bdt_data)
{
    cJSON *item = NULL;

    if (bdt_data == NULL) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [BdtData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!bdt_data->asp_id) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [asp_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "aspId", bdt_data->asp_id) == NULL) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [asp_id]");
        goto end;
    }

    if (!bdt_data->trans_policy) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [trans_policy]");
        goto end;
    }
    cJSON *trans_policy_local_JSON = OpenAPI_transfer_policy_convertToJSON(bdt_data->trans_policy);
    if (trans_policy_local_JSON == NULL) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [trans_policy]");
        goto end;
    }
    cJSON_AddItemToObject(item, "transPolicy", trans_policy_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [trans_policy]");
        goto end;
    }

    if (bdt_data->bdt_ref_id) {
        if (cJSON_AddStringToObject(item, "bdtRefId", bdt_data->bdt_ref_id) == NULL) {
            ogs_error("OpenAPI_bdt_data_convertToJSON() failed [bdt_ref_id]");
            goto end;
        }
    }

    if (bdt_data->nw_area_info) {
        cJSON *nw_area_info_local_JSON = OpenAPI_network_area_info_2_convertToJSON(bdt_data->nw_area_info);
        if (nw_area_info_local_JSON == NULL) {
            ogs_error("OpenAPI_bdt_data_convertToJSON() failed [nw_area_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "nwAreaInfo", nw_area_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_bdt_data_convertToJSON() failed [nw_area_info]");
            goto end;
        }
    }

    if (bdt_data->num_of_ues) {
        if (cJSON_AddNumberToObject(item, "numOfUes", bdt_data->num_of_ues) == NULL) {
            ogs_error("OpenAPI_bdt_data_convertToJSON() failed [num_of_ues]");
            goto end;
        }
    }

    if (bdt_data->vol_per_ue) {
        cJSON *vol_per_ue_local_JSON = OpenAPI_usage_threshold_convertToJSON(bdt_data->vol_per_ue);
        if (vol_per_ue_local_JSON == NULL) {
            ogs_error("OpenAPI_bdt_data_convertToJSON() failed [vol_per_ue]");
            goto end;
        }
        cJSON_AddItemToObject(item, "volPerUe", vol_per_ue_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_bdt_data_convertToJSON() failed [vol_per_ue]");
            goto end;
        }
    }

    if (bdt_data->dnn) {
        if (cJSON_AddStringToObject(item, "dnn", bdt_data->dnn) == NULL) {
            ogs_error("OpenAPI_bdt_data_convertToJSON() failed [dnn]");
            goto end;
        }
    }

    if (bdt_data->snssai) {
        cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(bdt_data->snssai);
        if (snssai_local_JSON == NULL) {
            ogs_error("OpenAPI_bdt_data_convertToJSON() failed [snssai]");
            goto end;
        }
        cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_bdt_data_convertToJSON() failed [snssai]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_bdt_data_t *OpenAPI_bdt_data_parseFromJSON(cJSON *bdt_dataJSON)
{
    OpenAPI_bdt_data_t *bdt_data_local_var = NULL;
    cJSON *asp_id = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "aspId");
    if (!asp_id) {
        ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [asp_id]");
        goto end;
    }


    if (!cJSON_IsString(asp_id)) {
        ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [asp_id]");
        goto end;
    }

    cJSON *trans_policy = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "transPolicy");
    if (!trans_policy) {
        ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [trans_policy]");
        goto end;
    }

    OpenAPI_transfer_policy_t *trans_policy_local_nonprim = NULL;

    trans_policy_local_nonprim = OpenAPI_transfer_policy_parseFromJSON(trans_policy);

    cJSON *bdt_ref_id = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "bdtRefId");

    if (bdt_ref_id) {
        if (!cJSON_IsString(bdt_ref_id)) {
            ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [bdt_ref_id]");
            goto end;
        }
    }

    cJSON *nw_area_info = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "nwAreaInfo");

    OpenAPI_network_area_info_2_t *nw_area_info_local_nonprim = NULL;
    if (nw_area_info) {
        nw_area_info_local_nonprim = OpenAPI_network_area_info_2_parseFromJSON(nw_area_info);
    }

    cJSON *num_of_ues = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "numOfUes");

    if (num_of_ues) {
        if (!cJSON_IsNumber(num_of_ues)) {
            ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [num_of_ues]");
            goto end;
        }
    }

    cJSON *vol_per_ue = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "volPerUe");

    OpenAPI_usage_threshold_t *vol_per_ue_local_nonprim = NULL;
    if (vol_per_ue) {
        vol_per_ue_local_nonprim = OpenAPI_usage_threshold_parseFromJSON(vol_per_ue);
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "dnn");

    if (dnn) {
        if (!cJSON_IsString(dnn)) {
            ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [dnn]");
            goto end;
        }
    }

    cJSON *snssai = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "snssai");

    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    if (snssai) {
        snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    }

    bdt_data_local_var = OpenAPI_bdt_data_create (
        ogs_strdup(asp_id->valuestring),
        trans_policy_local_nonprim,
        bdt_ref_id ? ogs_strdup(bdt_ref_id->valuestring) : NULL,
        nw_area_info ? nw_area_info_local_nonprim : NULL,
        num_of_ues ? num_of_ues->valuedouble : 0,
        vol_per_ue ? vol_per_ue_local_nonprim : NULL,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL
        );

    return bdt_data_local_var;
end:
    return NULL;
}

OpenAPI_bdt_data_t *OpenAPI_bdt_data_copy(OpenAPI_bdt_data_t *dst, OpenAPI_bdt_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bdt_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed");
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

    OpenAPI_bdt_data_free(dst);
    dst = OpenAPI_bdt_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

