
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bdt_data.h"

OpenAPI_bdt_data_t *OpenAPI_bdt_data_create(
    char *asp_id,
    OpenAPI_transfer_policy_t *trans_policy,
    char *bdt_ref_id,
    OpenAPI_network_area_info_2_t *nw_area_info,
    bool is_num_of_ues,
    int num_of_ues,
    OpenAPI_usage_threshold_t *vol_per_ue,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *traffic_des,
    OpenAPI_bdt_policy_status_t *bdtp_status,
    char *supp_feat,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_bdt_data_t *bdt_data_local_var = ogs_malloc(sizeof(OpenAPI_bdt_data_t));
    ogs_assert(bdt_data_local_var);

    bdt_data_local_var->asp_id = asp_id;
    bdt_data_local_var->trans_policy = trans_policy;
    bdt_data_local_var->bdt_ref_id = bdt_ref_id;
    bdt_data_local_var->nw_area_info = nw_area_info;
    bdt_data_local_var->is_num_of_ues = is_num_of_ues;
    bdt_data_local_var->num_of_ues = num_of_ues;
    bdt_data_local_var->vol_per_ue = vol_per_ue;
    bdt_data_local_var->dnn = dnn;
    bdt_data_local_var->snssai = snssai;
    bdt_data_local_var->traffic_des = traffic_des;
    bdt_data_local_var->bdtp_status = bdtp_status;
    bdt_data_local_var->supp_feat = supp_feat;
    bdt_data_local_var->reset_ids = reset_ids;

    return bdt_data_local_var;
}

void OpenAPI_bdt_data_free(OpenAPI_bdt_data_t *bdt_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == bdt_data) {
        return;
    }
    if (bdt_data->asp_id) {
        ogs_free(bdt_data->asp_id);
        bdt_data->asp_id = NULL;
    }
    if (bdt_data->trans_policy) {
        OpenAPI_transfer_policy_free(bdt_data->trans_policy);
        bdt_data->trans_policy = NULL;
    }
    if (bdt_data->bdt_ref_id) {
        ogs_free(bdt_data->bdt_ref_id);
        bdt_data->bdt_ref_id = NULL;
    }
    if (bdt_data->nw_area_info) {
        OpenAPI_network_area_info_2_free(bdt_data->nw_area_info);
        bdt_data->nw_area_info = NULL;
    }
    if (bdt_data->vol_per_ue) {
        OpenAPI_usage_threshold_free(bdt_data->vol_per_ue);
        bdt_data->vol_per_ue = NULL;
    }
    if (bdt_data->dnn) {
        ogs_free(bdt_data->dnn);
        bdt_data->dnn = NULL;
    }
    if (bdt_data->snssai) {
        OpenAPI_snssai_free(bdt_data->snssai);
        bdt_data->snssai = NULL;
    }
    if (bdt_data->traffic_des) {
        ogs_free(bdt_data->traffic_des);
        bdt_data->traffic_des = NULL;
    }
    if (bdt_data->bdtp_status) {
        OpenAPI_bdt_policy_status_free(bdt_data->bdtp_status);
        bdt_data->bdtp_status = NULL;
    }
    if (bdt_data->supp_feat) {
        ogs_free(bdt_data->supp_feat);
        bdt_data->supp_feat = NULL;
    }
    if (bdt_data->reset_ids) {
        OpenAPI_list_for_each(bdt_data->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(bdt_data->reset_ids);
        bdt_data->reset_ids = NULL;
    }
    ogs_free(bdt_data);
}

cJSON *OpenAPI_bdt_data_convertToJSON(OpenAPI_bdt_data_t *bdt_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (bdt_data == NULL) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [BdtData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!bdt_data->asp_id) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [asp_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "aspId", bdt_data->asp_id) == NULL) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [asp_id]");
        goto end;
    }

    if (!bdt_data->trans_policy) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [trans_policy]");
        return NULL;
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

    if (bdt_data->is_num_of_ues) {
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

    if (bdt_data->traffic_des) {
    if (cJSON_AddStringToObject(item, "trafficDes", bdt_data->traffic_des) == NULL) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [traffic_des]");
        goto end;
    }
    }

    if (bdt_data->bdtp_status) {
    cJSON *bdtp_status_local_JSON = OpenAPI_bdt_policy_status_convertToJSON(bdt_data->bdtp_status);
    if (bdtp_status_local_JSON == NULL) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [bdtp_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "bdtpStatus", bdtp_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [bdtp_status]");
        goto end;
    }
    }

    if (bdt_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", bdt_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

    if (bdt_data->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_bdt_data_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(bdt_data->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_bdt_data_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_bdt_data_t *OpenAPI_bdt_data_parseFromJSON(cJSON *bdt_dataJSON)
{
    OpenAPI_bdt_data_t *bdt_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *asp_id = NULL;
    cJSON *trans_policy = NULL;
    OpenAPI_transfer_policy_t *trans_policy_local_nonprim = NULL;
    cJSON *bdt_ref_id = NULL;
    cJSON *nw_area_info = NULL;
    OpenAPI_network_area_info_2_t *nw_area_info_local_nonprim = NULL;
    cJSON *num_of_ues = NULL;
    cJSON *vol_per_ue = NULL;
    OpenAPI_usage_threshold_t *vol_per_ue_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *traffic_des = NULL;
    cJSON *bdtp_status = NULL;
    OpenAPI_bdt_policy_status_t *bdtp_status_local_nonprim = NULL;
    cJSON *supp_feat = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    asp_id = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "aspId");
    if (!asp_id) {
        ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [asp_id]");
        goto end;
    }
    if (!cJSON_IsString(asp_id)) {
        ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [asp_id]");
        goto end;
    }

    trans_policy = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "transPolicy");
    if (!trans_policy) {
        ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [trans_policy]");
        goto end;
    }
    trans_policy_local_nonprim = OpenAPI_transfer_policy_parseFromJSON(trans_policy);
    if (!trans_policy_local_nonprim) {
        ogs_error("OpenAPI_transfer_policy_parseFromJSON failed [trans_policy]");
        goto end;
    }

    bdt_ref_id = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "bdtRefId");
    if (bdt_ref_id) {
    if (!cJSON_IsString(bdt_ref_id) && !cJSON_IsNull(bdt_ref_id)) {
        ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [bdt_ref_id]");
        goto end;
    }
    }

    nw_area_info = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "nwAreaInfo");
    if (nw_area_info) {
    nw_area_info_local_nonprim = OpenAPI_network_area_info_2_parseFromJSON(nw_area_info);
    if (!nw_area_info_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_2_parseFromJSON failed [nw_area_info]");
        goto end;
    }
    }

    num_of_ues = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "numOfUes");
    if (num_of_ues) {
    if (!cJSON_IsNumber(num_of_ues)) {
        ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [num_of_ues]");
        goto end;
    }
    }

    vol_per_ue = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "volPerUe");
    if (vol_per_ue) {
    vol_per_ue_local_nonprim = OpenAPI_usage_threshold_parseFromJSON(vol_per_ue);
    if (!vol_per_ue_local_nonprim) {
        ogs_error("OpenAPI_usage_threshold_parseFromJSON failed [vol_per_ue]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    traffic_des = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "trafficDes");
    if (traffic_des) {
    if (!cJSON_IsString(traffic_des) && !cJSON_IsNull(traffic_des)) {
        ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [traffic_des]");
        goto end;
    }
    }

    bdtp_status = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "bdtpStatus");
    if (bdtp_status) {
    bdtp_status_local_nonprim = OpenAPI_bdt_policy_status_parseFromJSON(bdtp_status);
    if (!bdtp_status_local_nonprim) {
        ogs_error("OpenAPI_bdt_policy_status_parseFromJSON failed [bdtp_status]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(bdt_dataJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_bdt_data_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    bdt_data_local_var = OpenAPI_bdt_data_create (
        ogs_strdup(asp_id->valuestring),
        trans_policy_local_nonprim,
        bdt_ref_id && !cJSON_IsNull(bdt_ref_id) ? ogs_strdup(bdt_ref_id->valuestring) : NULL,
        nw_area_info ? nw_area_info_local_nonprim : NULL,
        num_of_ues ? true : false,
        num_of_ues ? num_of_ues->valuedouble : 0,
        vol_per_ue ? vol_per_ue_local_nonprim : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        traffic_des && !cJSON_IsNull(traffic_des) ? ogs_strdup(traffic_des->valuestring) : NULL,
        bdtp_status ? bdtp_status_local_nonprim : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return bdt_data_local_var;
end:
    if (trans_policy_local_nonprim) {
        OpenAPI_transfer_policy_free(trans_policy_local_nonprim);
        trans_policy_local_nonprim = NULL;
    }
    if (nw_area_info_local_nonprim) {
        OpenAPI_network_area_info_2_free(nw_area_info_local_nonprim);
        nw_area_info_local_nonprim = NULL;
    }
    if (vol_per_ue_local_nonprim) {
        OpenAPI_usage_threshold_free(vol_per_ue_local_nonprim);
        vol_per_ue_local_nonprim = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (bdtp_status_local_nonprim) {
        OpenAPI_bdt_policy_status_free(bdtp_status_local_nonprim);
        bdtp_status_local_nonprim = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
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

