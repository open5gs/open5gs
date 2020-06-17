
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_information_transfer_rsp_data.h"

OpenAPI_n2_information_transfer_rsp_data_t *OpenAPI_n2_information_transfer_rsp_data_create(
    OpenAPI_n2_information_transfer_result_e result,
    OpenAPI_pws_response_data_t *pws_rsp_data,
    char *supported_features
    )
{
    OpenAPI_n2_information_transfer_rsp_data_t *n2_information_transfer_rsp_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_n2_information_transfer_rsp_data_t));
    if (!n2_information_transfer_rsp_data_local_var) {
        return NULL;
    }
    n2_information_transfer_rsp_data_local_var->result = result;
    n2_information_transfer_rsp_data_local_var->pws_rsp_data = pws_rsp_data;
    n2_information_transfer_rsp_data_local_var->supported_features = supported_features;

    return n2_information_transfer_rsp_data_local_var;
}

void OpenAPI_n2_information_transfer_rsp_data_free(OpenAPI_n2_information_transfer_rsp_data_t *n2_information_transfer_rsp_data)
{
    if (NULL == n2_information_transfer_rsp_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_pws_response_data_free(n2_information_transfer_rsp_data->pws_rsp_data);
    ogs_free(n2_information_transfer_rsp_data->supported_features);
    ogs_free(n2_information_transfer_rsp_data);
}

cJSON *OpenAPI_n2_information_transfer_rsp_data_convertToJSON(OpenAPI_n2_information_transfer_rsp_data_t *n2_information_transfer_rsp_data)
{
    cJSON *item = NULL;

    if (n2_information_transfer_rsp_data == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed [N2InformationTransferRspData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!n2_information_transfer_rsp_data->result) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed [result]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "result", OpenAPI_n2_information_transfer_result_ToString(n2_information_transfer_rsp_data->result)) == NULL) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_convertToJSON() failed [result]");
        goto end;
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

end:
    return item;
}

OpenAPI_n2_information_transfer_rsp_data_t *OpenAPI_n2_information_transfer_rsp_data_parseFromJSON(cJSON *n2_information_transfer_rsp_dataJSON)
{
    OpenAPI_n2_information_transfer_rsp_data_t *n2_information_transfer_rsp_data_local_var = NULL;
    cJSON *result = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_rsp_dataJSON, "result");
    if (!result) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_parseFromJSON() failed [result]");
        goto end;
    }

    OpenAPI_n2_information_transfer_result_e resultVariable;

    if (!cJSON_IsString(result)) {
        ogs_error("OpenAPI_n2_information_transfer_rsp_data_parseFromJSON() failed [result]");
        goto end;
    }
    resultVariable = OpenAPI_n2_information_transfer_result_FromString(result->valuestring);

    cJSON *pws_rsp_data = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_rsp_dataJSON, "pwsRspData");

    OpenAPI_pws_response_data_t *pws_rsp_data_local_nonprim = NULL;
    if (pws_rsp_data) {
        pws_rsp_data_local_nonprim = OpenAPI_pws_response_data_parseFromJSON(pws_rsp_data);
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(n2_information_transfer_rsp_dataJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_n2_information_transfer_rsp_data_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    n2_information_transfer_rsp_data_local_var = OpenAPI_n2_information_transfer_rsp_data_create (
        resultVariable,
        pws_rsp_data ? pws_rsp_data_local_nonprim : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
        );

    return n2_information_transfer_rsp_data_local_var;
end:
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

