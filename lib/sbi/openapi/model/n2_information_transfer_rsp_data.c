
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_information_transfer_rsp_data.h"

OpenAPI_n2_information_transfer_rsp_data_t *OpenAPI_n2_information_transfer_rsp_data_create(
    OpenAPI_n2_information_transfer_result_e result,
    OpenAPI_list_t *nrppa_rsp_per_ngran_list,
    OpenAPI_pws_response_data_t *pws_rsp_data,
    char *supported_features,
    OpenAPI_list_t *tss_rsp_per_ngran_list
)
{
    OpenAPI_n2_information_transfer_rsp_data_t *n2_information_transfer_rsp_data_local_var = ogs_malloc(sizeof(OpenAPI_n2_information_transfer_rsp_data_t));
    ogs_assert(n2_information_transfer_rsp_data_local_var);

    n2_information_transfer_rsp_data_local_var->result = result;
    n2_information_transfer_rsp_data_local_var->nrppa_rsp_per_ngran_list = nrppa_rsp_per_ngran_list;
    n2_information_transfer_rsp_data_local_var->pws_rsp_data = pws_rsp_data;
    n2_information_transfer_rsp_data_local_var->supported_features = supported_features;
    n2_information_transfer_rsp_data_local_var->tss_rsp_per_ngran_list = tss_rsp_per_ngran_list;

    return n2_information_transfer_rsp_data_local_var;
}

void OpenAPI_n2_information_transfer_rsp_data_free(OpenAPI_n2_information_transfer_rsp_data_t *n2_information_transfer_rsp_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n2_information_transfer_rsp_data) {
        return;
    }
    if (n2_information_transfer_rsp_data->nrppa_rsp_per_ngran_list) {
        OpenAPI_list_for_each(n2_information_transfer_rsp_data->nrppa_rsp_per_ngran_list, node) {
            OpenAPI_nrppa_rsp_per_ngran_free(node->data);
        }
        OpenAPI_list_free(n2_information_transfer_rsp_data->nrppa_rsp_per_ngran_list);
        n2_information_transfer_rsp_data->nrppa_rsp_per_ngran_list = NULL;
    }
    if (n2_information_transfer_rsp_data->pws_rsp_data) {
        OpenAPI_pws_response_data_free(n2_information_transfer_rsp_data->pws_rsp_data);
        n2_information_transfer_rsp_data->pws_rsp_data = NULL;
    }
    if (n2_information_transfer_rsp_data->supported_features) {
        ogs_free(n2_information_transfer_rsp_data->supported_features);
        n2_information_transfer_rsp_data->supported_features = NULL;
    }
    if (n2_information_transfer_rsp_data->tss_rsp_per_ngran_list) {
        OpenAPI_list_for_each(n2_information_transfer_rsp_data->tss_rsp_per_ngran_list, node) {
            OpenAPI_tss_rsp_per_ngran_free(node->data);
        }
        OpenAPI_list_free(n2_information_transfer_rsp_data->tss_rsp_per_ngran_list);
        n2_information_transfer_rsp_data->tss_rsp_per_ngran_list = NULL;
    }
    ogs_free(n2_information_transfer_rsp_data);
}

cJSON *OpenAPI_n2_information_transfer_rsp_data_convertToJSON(OpenAPI_n2_information_transfer_rsp_data_t *n2_information_transfer_rsp_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n2_information_transfer_rsp_data == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed [N2InformationTransferRspData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n2_information_transfer_rsp_data->result == OpenAPI_n2_information_transfer_result_NULL) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed [result]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "result", OpenAPI_n2_information_transfer_result_ToString(n2_information_transfer_rsp_data->result)) == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed [result]");
        goto end;
    }

    if (n2_information_transfer_rsp_data->nrppa_rsp_per_ngran_list) {
    cJSON *nrppa_rsp_per_ngran_listList = cJSON_AddArrayToObject(item, "nrppaRspPerNgranList");
    if (nrppa_rsp_per_ngran_listList == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed [nrppa_rsp_per_ngran_list]");
        goto end;
    }
    OpenAPI_list_for_each(n2_information_transfer_rsp_data->nrppa_rsp_per_ngran_list, node) {
        cJSON *itemLocal = OpenAPI_nrppa_rsp_per_ngran_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed [nrppa_rsp_per_ngran_list]");
            goto end;
        }
        cJSON_AddItemToArray(nrppa_rsp_per_ngran_listList, itemLocal);
    }
    }

    if (n2_information_transfer_rsp_data->pws_rsp_data) {
    cJSON *pws_rsp_data_local_JSON = OpenAPI_pws_response_data_convertToJSON(n2_information_transfer_rsp_data->pws_rsp_data);
    if (pws_rsp_data_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed [pws_rsp_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pwsRspData", pws_rsp_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed [pws_rsp_data]");
        goto end;
    }
    }

    if (n2_information_transfer_rsp_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", n2_information_transfer_rsp_data->supported_features) == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (n2_information_transfer_rsp_data->tss_rsp_per_ngran_list) {
    cJSON *tss_rsp_per_ngran_listList = cJSON_AddArrayToObject(item, "tssRspPerNgranList");
    if (tss_rsp_per_ngran_listList == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed [tss_rsp_per_ngran_list]");
        goto end;
    }
    OpenAPI_list_for_each(n2_information_transfer_rsp_data->tss_rsp_per_ngran_list, node) {
        cJSON *itemLocal = OpenAPI_tss_rsp_per_ngran_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed [tss_rsp_per_ngran_list]");
            goto end;
        }
        cJSON_AddItemToArray(tss_rsp_per_ngran_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_n2_information_transfer_rsp_data_t *OpenAPI_n2_information_transfer_rsp_data_parseFromJSON(cJSON *n2_information_transfer_rsp_dataJSON)
{
    OpenAPI_n2_information_transfer_rsp_data_t *n2_information_transfer_rsp_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *result = NULL;
    OpenAPI_n2_information_transfer_result_e resultVariable = 0;
    cJSON *nrppa_rsp_per_ngran_list = NULL;
    OpenAPI_list_t *nrppa_rsp_per_ngran_listList = NULL;
    cJSON *pws_rsp_data = NULL;
    OpenAPI_pws_response_data_t *pws_rsp_data_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    cJSON *tss_rsp_per_ngran_list = NULL;
    OpenAPI_list_t *tss_rsp_per_ngran_listList = NULL;
    result = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_rsp_dataJSON, "result");
    if (!result) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_parseFromJSON() failed [result]");
        goto end;
    }
    if (!cJSON_IsString(result)) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_parseFromJSON() failed [result]");
        goto end;
    }
    resultVariable = OpenAPI_n2_information_transfer_result_FromString(result->valuestring);

    nrppa_rsp_per_ngran_list = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_rsp_dataJSON, "nrppaRspPerNgranList");
    if (nrppa_rsp_per_ngran_list) {
        cJSON *nrppa_rsp_per_ngran_list_local = NULL;
        if (!cJSON_IsArray(nrppa_rsp_per_ngran_list)) {
            ogs_error("OpenAPI_n2_information_transfer_rsp_data_parseFromJSON() failed [nrppa_rsp_per_ngran_list]");
            goto end;
        }

        nrppa_rsp_per_ngran_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nrppa_rsp_per_ngran_list_local, nrppa_rsp_per_ngran_list) {
            if (!cJSON_IsObject(nrppa_rsp_per_ngran_list_local)) {
                ogs_error("OpenAPI_n2_information_transfer_rsp_data_parseFromJSON() failed [nrppa_rsp_per_ngran_list]");
                goto end;
            }
            OpenAPI_nrppa_rsp_per_ngran_t *nrppa_rsp_per_ngran_listItem = OpenAPI_nrppa_rsp_per_ngran_parseFromJSON(nrppa_rsp_per_ngran_list_local);
            if (!nrppa_rsp_per_ngran_listItem) {
                ogs_error("No nrppa_rsp_per_ngran_listItem");
                goto end;
            }
            OpenAPI_list_add(nrppa_rsp_per_ngran_listList, nrppa_rsp_per_ngran_listItem);
        }
    }

    pws_rsp_data = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_rsp_dataJSON, "pwsRspData");
    if (pws_rsp_data) {
    pws_rsp_data_local_nonprim = OpenAPI_pws_response_data_parseFromJSON(pws_rsp_data);
    if (!pws_rsp_data_local_nonprim) {
        ogs_error("OpenAPI_pws_response_data_parseFromJSON failed [pws_rsp_data]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_rsp_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    tss_rsp_per_ngran_list = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_rsp_dataJSON, "tssRspPerNgranList");
    if (tss_rsp_per_ngran_list) {
        cJSON *tss_rsp_per_ngran_list_local = NULL;
        if (!cJSON_IsArray(tss_rsp_per_ngran_list)) {
            ogs_error("OpenAPI_n2_information_transfer_rsp_data_parseFromJSON() failed [tss_rsp_per_ngran_list]");
            goto end;
        }

        tss_rsp_per_ngran_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tss_rsp_per_ngran_list_local, tss_rsp_per_ngran_list) {
            if (!cJSON_IsObject(tss_rsp_per_ngran_list_local)) {
                ogs_error("OpenAPI_n2_information_transfer_rsp_data_parseFromJSON() failed [tss_rsp_per_ngran_list]");
                goto end;
            }
            OpenAPI_tss_rsp_per_ngran_t *tss_rsp_per_ngran_listItem = OpenAPI_tss_rsp_per_ngran_parseFromJSON(tss_rsp_per_ngran_list_local);
            if (!tss_rsp_per_ngran_listItem) {
                ogs_error("No tss_rsp_per_ngran_listItem");
                goto end;
            }
            OpenAPI_list_add(tss_rsp_per_ngran_listList, tss_rsp_per_ngran_listItem);
        }
    }

    n2_information_transfer_rsp_data_local_var = OpenAPI_n2_information_transfer_rsp_data_create (
        resultVariable,
        nrppa_rsp_per_ngran_list ? nrppa_rsp_per_ngran_listList : NULL,
        pws_rsp_data ? pws_rsp_data_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        tss_rsp_per_ngran_list ? tss_rsp_per_ngran_listList : NULL
    );

    return n2_information_transfer_rsp_data_local_var;
end:
    if (nrppa_rsp_per_ngran_listList) {
        OpenAPI_list_for_each(nrppa_rsp_per_ngran_listList, node) {
            OpenAPI_nrppa_rsp_per_ngran_free(node->data);
        }
        OpenAPI_list_free(nrppa_rsp_per_ngran_listList);
        nrppa_rsp_per_ngran_listList = NULL;
    }
    if (pws_rsp_data_local_nonprim) {
        OpenAPI_pws_response_data_free(pws_rsp_data_local_nonprim);
        pws_rsp_data_local_nonprim = NULL;
    }
    if (tss_rsp_per_ngran_listList) {
        OpenAPI_list_for_each(tss_rsp_per_ngran_listList, node) {
            OpenAPI_tss_rsp_per_ngran_free(node->data);
        }
        OpenAPI_list_free(tss_rsp_per_ngran_listList);
        tss_rsp_per_ngran_listList = NULL;
    }
    return NULL;
}

OpenAPI_n2_information_transfer_rsp_data_t *OpenAPI_n2_information_transfer_rsp_data_copy(OpenAPI_n2_information_transfer_rsp_data_t *dst, OpenAPI_n2_information_transfer_rsp_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_information_transfer_rsp_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed");
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

    OpenAPI_n2_information_transfer_rsp_data_free(dst);
    dst = OpenAPI_n2_information_transfer_rsp_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

