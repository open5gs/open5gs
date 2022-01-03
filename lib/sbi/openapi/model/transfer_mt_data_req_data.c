
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "transfer_mt_data_req_data.h"

OpenAPI_transfer_mt_data_req_data_t *OpenAPI_transfer_mt_data_req_data_create(
    OpenAPI_ref_to_binary_data_t *mt_data
)
{
    OpenAPI_transfer_mt_data_req_data_t *transfer_mt_data_req_data_local_var = ogs_malloc(sizeof(OpenAPI_transfer_mt_data_req_data_t));
    ogs_assert(transfer_mt_data_req_data_local_var);

    transfer_mt_data_req_data_local_var->mt_data = mt_data;

    return transfer_mt_data_req_data_local_var;
}

void OpenAPI_transfer_mt_data_req_data_free(OpenAPI_transfer_mt_data_req_data_t *transfer_mt_data_req_data)
{
    if (NULL == transfer_mt_data_req_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_ref_to_binary_data_free(transfer_mt_data_req_data->mt_data);
    ogs_free(transfer_mt_data_req_data);
}

cJSON *OpenAPI_transfer_mt_data_req_data_convertToJSON(OpenAPI_transfer_mt_data_req_data_t *transfer_mt_data_req_data)
{
    cJSON *item = NULL;

    if (transfer_mt_data_req_data == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_req_data_convertToJSON() failed [TransferMtDataReqData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *mt_data_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(transfer_mt_data_req_data->mt_data);
    if (mt_data_local_JSON == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_req_data_convertToJSON() failed [mt_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mtData", mt_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_transfer_mt_data_req_data_convertToJSON() failed [mt_data]");
        goto end;
    }

end:
    return item;
}

OpenAPI_transfer_mt_data_req_data_t *OpenAPI_transfer_mt_data_req_data_parseFromJSON(cJSON *transfer_mt_data_req_dataJSON)
{
    OpenAPI_transfer_mt_data_req_data_t *transfer_mt_data_req_data_local_var = NULL;
    cJSON *mt_data = cJSON_GetObjectItemCaseSensitive(transfer_mt_data_req_dataJSON, "mtData");
    if (!mt_data) {
        ogs_error("OpenAPI_transfer_mt_data_req_data_parseFromJSON() failed [mt_data]");
        goto end;
    }

    OpenAPI_ref_to_binary_data_t *mt_data_local_nonprim = NULL;
    mt_data_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(mt_data);

    transfer_mt_data_req_data_local_var = OpenAPI_transfer_mt_data_req_data_create (
        mt_data_local_nonprim
    );

    return transfer_mt_data_req_data_local_var;
end:
    return NULL;
}

OpenAPI_transfer_mt_data_req_data_t *OpenAPI_transfer_mt_data_req_data_copy(OpenAPI_transfer_mt_data_req_data_t *dst, OpenAPI_transfer_mt_data_req_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_transfer_mt_data_req_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_transfer_mt_data_req_data_convertToJSON() failed");
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

    OpenAPI_transfer_mt_data_req_data_free(dst);
    dst = OpenAPI_transfer_mt_data_req_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

