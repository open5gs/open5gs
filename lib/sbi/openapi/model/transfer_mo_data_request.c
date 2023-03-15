
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "transfer_mo_data_request.h"

OpenAPI_transfer_mo_data_request_t *OpenAPI_transfer_mo_data_request_create(
    OpenAPI_transfer_mo_data_req_data_t *json_data,
    OpenAPI_binary_t* binary_mo_data
)
{
    OpenAPI_transfer_mo_data_request_t *transfer_mo_data_request_local_var = ogs_malloc(sizeof(OpenAPI_transfer_mo_data_request_t));
    ogs_assert(transfer_mo_data_request_local_var);

    transfer_mo_data_request_local_var->json_data = json_data;
    transfer_mo_data_request_local_var->binary_mo_data = binary_mo_data;

    return transfer_mo_data_request_local_var;
}

void OpenAPI_transfer_mo_data_request_free(OpenAPI_transfer_mo_data_request_t *transfer_mo_data_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == transfer_mo_data_request) {
        return;
    }
    if (transfer_mo_data_request->json_data) {
        OpenAPI_transfer_mo_data_req_data_free(transfer_mo_data_request->json_data);
        transfer_mo_data_request->json_data = NULL;
    }
    if (transfer_mo_data_request->binary_mo_data) {
        ogs_free(transfer_mo_data_request->binary_mo_data->data);
        transfer_mo_data_request->binary_mo_data = NULL;
    }
    ogs_free(transfer_mo_data_request);
}

cJSON *OpenAPI_transfer_mo_data_request_convertToJSON(OpenAPI_transfer_mo_data_request_t *transfer_mo_data_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (transfer_mo_data_request == NULL) {
        ogs_error("OpenAPI_transfer_mo_data_request_convertToJSON() failed [TransferMoData_request]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (transfer_mo_data_request->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_transfer_mo_data_req_data_convertToJSON(transfer_mo_data_request->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_transfer_mo_data_request_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_transfer_mo_data_request_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (transfer_mo_data_request->binary_mo_data) {
    char* encoded_str_binary_mo_data = OpenAPI_base64encode(transfer_mo_data_request->binary_mo_data->data,transfer_mo_data_request->binary_mo_data->len);
    if (cJSON_AddStringToObject(item, "binaryMoData", encoded_str_binary_mo_data) == NULL) {
        ogs_error("OpenAPI_transfer_mo_data_request_convertToJSON() failed [binary_mo_data]");
        goto end;
    }
    ogs_free(encoded_str_binary_mo_data);
    }

end:
    return item;
}

OpenAPI_transfer_mo_data_request_t *OpenAPI_transfer_mo_data_request_parseFromJSON(cJSON *transfer_mo_data_requestJSON)
{
    OpenAPI_transfer_mo_data_request_t *transfer_mo_data_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_transfer_mo_data_req_data_t *json_data_local_nonprim = NULL;
    cJSON *binary_mo_data = NULL;
    OpenAPI_binary_t *decoded_str_binary_mo_data = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(transfer_mo_data_requestJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_transfer_mo_data_req_data_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_transfer_mo_data_req_data_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_mo_data = cJSON_GetObjectItemCaseSensitive(transfer_mo_data_requestJSON, "binaryMoData");
    if (binary_mo_data) {
    decoded_str_binary_mo_data = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_mo_data);
    if (!cJSON_IsString(binary_mo_data)) {
        ogs_error("OpenAPI_transfer_mo_data_request_parseFromJSON() failed [binary_mo_data]");
        goto end;
    }
    decoded_str_binary_mo_data->data = OpenAPI_base64decode(binary_mo_data->valuestring, strlen(binary_mo_data->valuestring), &decoded_str_binary_mo_data->len);
    if (!decoded_str_binary_mo_data->data) {
        ogs_error("OpenAPI_transfer_mo_data_request_parseFromJSON() failed [binary_mo_data]");
        goto end;
    }
    }

    transfer_mo_data_request_local_var = OpenAPI_transfer_mo_data_request_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_mo_data ? decoded_str_binary_mo_data : NULL
    );

    return transfer_mo_data_request_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_transfer_mo_data_req_data_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_transfer_mo_data_request_t *OpenAPI_transfer_mo_data_request_copy(OpenAPI_transfer_mo_data_request_t *dst, OpenAPI_transfer_mo_data_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_transfer_mo_data_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_transfer_mo_data_request_convertToJSON() failed");
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

    OpenAPI_transfer_mo_data_request_free(dst);
    dst = OpenAPI_transfer_mo_data_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

